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

     Name:     SON Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     nl_ex_ms.c

     Sid:      bk_ex_ms.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:11 2014

     Prg:      subhamay 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=248;
static int RLOG_MODULE_ID=256;

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"
#include "rl_common.h"     /* RLOG defines */
#include "rl_interface.h"
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"        /* Common Pasn defines */

#include "lnl.h"           /* NL Interface defines */
#include "nlu.h"
#include "nl.h"            /* SON defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_pasn.x"       /* Common Hashlist definitions */
#include "cm_lte.x"       /* Common Pasn defines */

#include "lnl.x"           /* NL Interface includes */
#include "nlu.x"
#include "nl.x"            /* SON includes */

/* local defines */
PUBLIC NlCb nlCb;
/* local typedefs */
 
/* local externs */

/* forward references */

/************************************************************************
 *
 *      Fun:   nlActvTsk
 *
 *      Desc:  This is the entry point for all loosely coupled messages
 *             that have the SON as the destination. This includes layer
 *             manager messages, eNodeB application messages.
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  nl_ex_ms.c
 *
 ************************************************************************/
#ifdef ANSI
PUBLIC S16 nlActvTsk
(
Pst     *pst,                        /* Post structure */
Buffer  *mBuf                        /* message buffer */
)
#else /* ANSI */
PUBLIC S16 nlActvTsk(pst, mBuf)
Pst     *pst;                        /* Post structure */
Buffer  *mBuf;                       /* message buffer */
#endif /* ANSI */
{

   TRC3(nlActvTsk);
   
   switch(pst->srcEnt)
   {
      
      case ENTSM: /* Layer Manager events */
      {
         switch(pst->event)
         {
            case EVTLNLCFGREQ:
               cmUnpkLnlCfgReq(NlMiLnlCfgReq, pst, mBuf);
               break;
            case EVTLNLCNTRLREQ:
               cmUnpkLnlCntrlReq(NlMiLnlCntrlReq, pst, mBuf);
               break;
            case EVTLNLSTAIND:
               cmUnpkLnlStatInd(NlMiLnlStatInd, pst, mBuf);
               break;
            default:
               RLOG0(L_ERROR, "SON received Invalid Event received from SM \n");
               NL_FREE_MBUF(mBuf);
               break;
         } 
         break;
      }
      case ENTWR : /* eNB Application events */
      {
         switch(pst->event)  
         {
            case EVTNLUBNDREQ:
               cmUnpkNluBndReq(NlUiNluBndReq, pst, mBuf);
               break;
            case EVTNLUNHBRREQ:
               cmUnpkNluNeighborReq(NlUiNluNeighborReq, pst, mBuf);
               break;
            case EVTNLUPCIMODINDCFM:
               cmUnpkNluPciModIndCfm(NlUiNluPciModIndCfm, pst, mBuf);
               break;
            case EVTNLUUEMEASRPTIND :
               cmUnpkNluUeMeasReportInd(NlUiNluUeMeasReportInd, pst, mBuf);
               break;
#ifdef RGR_CQI_REPT
            case EVTNLUUECQIRPTIND :
               cmUnpkNluUeCqiReportInd(NlUiNluUeCqiReportInd, pst, mBuf);
               break;
#endif
            case EVTNLUUEIND :
               cmUnpkNluUeInd(NlUiNluUeInd, pst, mBuf);
               break;
            /* Periodic REM for TPM */
            case EVTNLUPERCELLSRCHRSP:
               cmUnpkNluPerCellSrchRsp(NlUiNluPerdCellSrchRsp, pst, mBuf);
               break;
            /* Periodic REM for TPM End */
            default :
               RLOG0(L_ERROR, "SON received Invalid Event received from eNB-APP  \n");
               NL_FREE_MBUF(mBuf);
               break;
         }
         break;
      }
      case ENTYS :
      {
         switch(pst->event)
         {
            case EVTNLVENDORDSONDATIND :
               nlUnpkVendorDSonDatInd(NlVendorDSonDatInd, pst, mBuf);
               break;
            default :
               RLOG0(L_ERROR, "SON received Invalid Event received from DSON  \n");
               NL_FREE_MBUF(mBuf);
               break;
         }
         break;
      }
      default:
      {
         RLOG0(L_ERROR, " SON received invalid event from unexpected source entity \n");
         NL_FREE_MBUF(mBuf);
         break;
      }
   }
   SExitTsk();
   RETVALUE(ROK);
} /* end of nlActvTsk */

/********************************************************************30**

           End of file:     bk_ex_ms.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:11 2014

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
*********************************************************************91*/

