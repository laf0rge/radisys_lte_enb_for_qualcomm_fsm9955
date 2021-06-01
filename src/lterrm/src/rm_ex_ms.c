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

     Name:     RRM Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     rm_ex_ms.c

     Sid:      qf_ex_ms.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:33 2013

     Prg:     jbm 

*********************************************************************21*/

/* header include files */
#include "rm_cmn.h"
#include "rgm.x"
/*
 *
 *       Fun:    rmActvTsk - RRC user activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   rm_ex_ms.c
 *
 */

PUBLIC S16 rmActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
{
   Mem sMem;
   Size     maxBlkSize;
   maxBlkSize = RM_MEM_PDU_SIZE;

   TRC3(rmActvTsk);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst == (Pst *)NULLP)
   {
//    RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Invalid Pst \n"));
      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }

      RETVALUE(RFAILED);
   }

   if (mBuf == (Buffer *)NULLP)
   {
//      RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Invalid MBuf \n"));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /* Assigning the proper region */
   sMem.region = g_stRmCb.rmInit.region;
   sMem.pool   = RM_MEM_POOL;

   switch (pst->srcEnt)
   {
      case ENTSM:
         switch (pst->event)
         {
	    #ifdef LWLCSMRMMILRM	
            case EVTLRMCFGREQ:
               cmUnpkLwLcLrmCfgReq(RmMiLrmCfgReq, pst, mBuf);
               break;
            case EVTLRMCNTRLREQ:
               cmUnpkLwLcLrmCntrlReq(RmMiLrmCntrlReq, pst, mBuf);
               break;
            case EVTLRMENBSTAIND:
               cmUnpkLwLcLrmEnbStaInd(RmMiLrmEnbStaInd, pst, mBuf);
               break;
	    #endif 

            default:
#ifdef DEBUGP
//               RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Unknown Event from SM\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;

      case ENTEU: 
         switch(pst->event)
         {
               /*RRM_SP3*/
            case EVTRMUUEINACTIND:
               {
                  cmUnpkLwLcRmuUeInactInd (RmUiRmuUeInactInd, pst, mBuf);
                  break;
               }
            default:
#ifdef DEBUGP
//               RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Unknown Evnt from ENTEU.\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;

      case ENTWR: 
         switch(pst->event)
         {
         	case EVTRMUBNDREQ:
         	   cmUnpkLwLcRmuBndReq(RmUiRmuBndReq, pst, mBuf);
         	   break;
            case EVTRMUUEADMITREQ:
            	cmUnpkLwLcRmuUeAdmitReq (RmUiRmuUeAdmitReq, pst, mBuf);
               break;
            case EVTRMUUEHOREQ:
               cmUnpkLwLcRmuUeHoReq (RmUiRmuUeHoReq, pst, mBuf);
               break;
            case EVTRMUUERELREQ:
               cmUnpkLwLcRmuUeRelReq (RmUiRmuUeRelReq, pst, mBuf);
               break;
            case EVTRMUUERECFGREQ:
               cmUnpkLwLcRmuUeRecfgReq (RmUiRmuUeRecfgReq, pst, mBuf);
               break;
            case EVTRMUOVLDSTRTIND:
               cmUnpkLwLcRmuMmeOvldStartInd (RmUiRmuMmeOvldStartInd, pst, mBuf);
               break;
            case EVTRMUOVLDSTPIND:
               cmUnpkLwLcRmuMmeOvldStopInd (RmUiRmuMmeOvldStopInd, pst, mBuf);
               break;
            case EVTRMUFSMSTAIND:
               cmUnpkLwLcRmuFsmStatusInd (RmUiRmuFsmStatusInd, pst, mBuf);
               break;
            case EVTRMUUESPSDISIND:
               cmUnpkLwLcRmuUeSpsDisInd (RmUiRmuUeSpsDisInd, pst, mBuf);
               break;
            default:
#ifdef DEBUGP
//               RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Unknown Evnt from NHU.\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;

      case ENTRG:
         switch (pst->event)
         {
      	    case EVTRGMBNDCFM:
      	       cmUnpkRgmBndCfm(RmLiRgmBndCfm, pst, mBuf);
      	       break;
             case EVTRGMPRBRPRTIND:
               cmUnpkRgmPrbRprtInd (RmLiRgmPrbRprtInd, pst, mBuf);
               break;
      	    case EVTRGMTRANSMODEIND:
      	       cmUnpkRgmTransModeInd(RmLiRgmTransModeInd, pst, mBuf);
      	       break;
            default:
#ifdef DEBUGP
//               RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Unknown event from RGR\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         } 
         break;

      default:
#ifdef DEBUGP
//         RM_DBG_ERROR((RM_PRNT_BUF,"rmActvTsk: Received Msg From Unknow Src.\n"));
#endif
         (Void)SPutMsg(mBuf);
         break;
   }

   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of rmActvTsk() */



/********************************************************************30**

           End of file:     qf_ex_ms.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:33 2013

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
