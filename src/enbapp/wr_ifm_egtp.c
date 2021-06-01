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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_ifm_egtp.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


/* header include files (.hx) */
static const char* RLOG_MODULE_NAME="IFM";

static int RLOG_FILE_ID=21;

static int RLOG_MODULE_ID=4;

#include "wr.h"
#include "wr_lwr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_dam.h"
#include "egt.x"
#include "wr_ifm_egtp.h"
#include "wr_dam_ifm_app.h"/*ccpu00138576*/ 

/* Following matrices define the mapping between the primitives called by the
 * eGTP user (as WrIfmEgtpXXXReq) and the corresponding primitives of the 
 * eGTP layer (service provider) (as EgUiEgtXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCEULIEGT    loose         cmPkEgtXXXReq
 * 1                 EG          tight         EgUiEgtXXXReq
 * 2                LWLCEULIEGT  LW loose      cmPkEgtXXXReq
 */

PUBLIC EgtBndReq WrIfmEgtpBndReqMt [] =
{
   cmPkEgtBndReq,        /* 0 - loosely coupled */
   cmPkEgtBndReq,        /* 1 - light weight loosely coupled */
#ifdef EG
   EgUiEgtBndReq         /* 2 - tightly coupled, stub layer */
#endif /* EG */
};


PUBLIC EgtUbndReq WrIfmEgtpUbndReqMt [] =
{
   cmPkEgtUbndReq,        /* 0 - loosely coupled */
   cmPkEgtUbndReq,        /* 1 - light weight loosely coupled */
#ifdef EG
   EgUiEgtUbndReq         /* 2 - tightly coupled, stub layer */
#endif /* EG */
};


/* Delete Local Tunnel primitive */
PUBLIC EgtEguLclTnlMgmtReq WrIfmEgtpEguLclTnlMgmtReqMt[] =
{
   cmPkEgtEguLclTnlMgmtReq,          /* 0 - loosely coupled */
   cmPkEgtEguLclTnlMgmtReq,          /* 1 - loosely coupled */
#ifdef EG
   EgUiEgtEguLclTnlMgmtReq           /* 1 - tightly coupled, service user EU */
#endif /* EG */
};


/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguStaReq WrIfmEgtpEguStaReqMt[] =
{
   cmPkEgtEguStaReq,          /* 0 - loosely coupled */
   cmPkEgtEguStaReq,          /* 1 - loosely coupled */
#ifdef EG
   EgUiEgtEguStaReq           /* 1 - tightly coupled, service user EU */
#endif /* EG */
};

/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguDatReq WrIfmEgtpEguDatReqMt[] =
{
   cmPkEgtEguDatReq,          /* 0 - loosely coupled */
   cmPkEgtEguDatReq,          /* 1 - loosely coupled */
#ifdef EG
   EgUiEgtEguDatReq           /* 1 - tightly coupled, service user EU */
#endif /* EG */
};


/************************************************************************
              Functions to send message EGTP
************************************************************************/
/** @brief This function resolves the EgtBndReq primitive
 *
 * @details
 *
 *     Function: WrIfmEgtpBndReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the EGTP Bind request 
 *           to EGTP layer.
 *
 *
 * @param[in] pst: POST Data structure. 
 * @param[in] suId: Service User ID.
 * @param[in] spId: Service Provider ID.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmEgtpBndReq
(
Pst                          *pst,
SuId                          suId,
SpId                          spId
)
{
   TRC3(WrIfmEgtpBndReq)

   (*WrIfmEgtpBndReqMt[pst->selector]) (pst, suId, spId);

   RETVALUE(ROK);
} /* end of WrIfmEgtpBndReq */

/** @brief This function resolves the EgtUbndReq primitive 
 *
 * @details
 *
 *     Function: WrIfmEgtpUbndReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the EGTP Unbind request 
 *           to EGTP layer.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   spId: Service Provider ID.
 * @param[in] reason: EGTP Unbind Request reason.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmEgtpUbndReq
(
Pst                          *pst,
SpId                          spId,
Reason                          reason
)
{
   TRC3(WrIfmEgtpUbndReq)

   (*WrIfmEgtpUbndReqMt[pst->selector]) (pst, spId, reason);

   RETVALUE(ROK);
} /* end of WrIfmEgtpUbndReq */

/** @brief This function is used to send Local Tunnel management Request.
 *
 * @details
 *
 *     Function: WrIfmEgtpEguLclTnlMgmtReq 
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the EGTP local tunnel management request 
 *           to EGTP layer.
 *
 * @param[in] egtUEvnt: eGTP-U Event of the eGTP Local Tunnel Management Request.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmEgtpEguLclTnlMgmtReq 
(
EgtUEvnt                     *egtUEvnt
)
{
   Pst                       *pst = &wrDamCb.egtSap[0]->pst;
   SpId                      spId =  wrDamCb.egtSap[0]->spId;
   (*WrIfmEgtpEguLclTnlMgmtReqMt[pst->selector])(pst, spId, egtUEvnt);

   RETVALUE(ROK);
} /* end of WrIfmEgtpEguLclTnlMgmtReq */

/** @brief This function sends the EGTP status Request to EGTP Layer.
 *
 * @details
 *
 *     Function: WrIfmEgtpEguStaReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the EGTP Status Req message 
 *           to EGTP layer.
 *
 * @param[in] egtUEvnt: eGTP-U Event of the eGTP Status Request.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmEgtpEguStaReq 
(
EgtUEvnt                     *egtUEvnt
)
{
   Pst                       *pst = &wrDamCb.egtSap[0]->pst;
   SpId                      spId =  wrDamCb.egtSap[0]->spId;

   (*WrIfmEgtpEguStaReqMt[pst->selector])(pst, spId, egtUEvnt);

   RETVALUE(ROK);
} /* end of WrIfmEgtpEguStaReq */

/** @brief This function is used to send the EGTP Data Request. 
 *
 * @details
 *
 *     Function: WrIfmEgtpEguDatReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the EGTP Data Req message 
 *           to EGTP layer.
 *
 * @param[in] egtUEvnt: eGTP-U Event of the eGTP Data Request.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmEgtpEguDatReq 
(
EgtUEvnt                     *egtUEvnt
)
{
   Pst                       *pst = &wrDamCb.egtSap[0]->pst;
   SpId                      spId =  wrDamCb.egtSap[0]->spId;

   (*WrIfmEgtpEguDatReqMt[pst->selector])(pst, spId, egtUEvnt);

   RETVALUE(ROK);
} /* end of WrIfmEgtpEguDatReq*/

/************************************************************************
      Handler Functions for messages from EGTP
************************************************************************/

/*************************************************************/
/* EGT Interface */
/*************************************************************/

/** @brief This function is used by to present Bind Cfm to EGT.
 *
 * @details
 *
 *     Function: WrIfmEgtpBndCfm
 *
 *         Processing steps:
 *         - This function processing the EGTP bind confirm from EGTP layer.
 *         - If the status is BND_OK then stop the running EGTP Bind timer 
 *           and trigger the alaram.
 *         - Otherwise trigger the alaram.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] status: EGTP Bind status.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   TRC2(WrIfmEgtpBndCfm);
  
   RLOG0(L_INFO, "WrIfmEgtpBndCfm: EGT Bind Cfm");

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      RLOG0(L_DEBUG, "WrIfmEgtpBndCfm: Successful Bind Cfm");
      wrDamStopTmr((PTR)wrDamCb.egtSap[0],WR_TMR_EGT_SAP_BND);
      RLOG0(L_INFO, "[ALARM] Sending EGT_SAP_BND_OK to OAM ");
      wrIfmDamWrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, 
                                                LWR_CAUSE_EGT_SAP_BOUND);
   }
   else
   {
      RLOG0(L_ERROR, "WrIfmEgtpBndCfm: Unsuccessful bind confirm");
      RLOG0(L_FATAL, "[ALARM] Sending EGT_SAP_BND_FAIL to OAM ");
      wrIfmDamWrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                                      LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* end of WrIfmEgtpBndCfm */

/** @brief This function is call handler for eGTP-U Tunnel Mgmt Confirm.
 *
 * @details
 *
 *     Function: WrIfmEgtpEguLclTnlMgmtCfm
 *
 *         Processing steps:
 *         - This function processing the EGTP local tunel management confirm 
 *           by using the  DAM Module function wrDamTnlMgmtCfm.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Local tunnel mangament confirm.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpEguLclTnlMgmtCfm
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   wrDamTnlMgmtCfm(eguMsg);
   /* Free the EgtUEvnt structure */
   WR_FREE_DATA_APP(eguMsg,sizeof(EgtUEvnt));
   RETVALUE(ROK);
} /* end of WrIfmEgtpEguLclTnlMgmtCfm */ 

/** @brief This function is call handler for eGTP-U Data Indication. 
 *
 * @details
 *
 *     Function: WrIfmEgtpEguDatInd
 *
 *         Processing steps:
 *         - This function processing the EGTP Data Indication by using the 
 *           DAM Module function wrDamEgtpDatInd.
 *
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Data Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpEguDatInd
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   S16 ret;
   TRC2(WrIfmEgtpEguDatInd);
   ret = wrDamEgtpDatInd(pst, eguMsg);
   if(ret != ROK)
   {
      if((eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_GPDU) && 
         (eguMsg->u.egMsg->u.mBuf != NULLP))
      {
          WR_FREEMBUF(eguMsg->u.egMsg->u.mBuf);
      }
      cmFreeMem(eguMsg->u.egMsg); 
      SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   }
   RETVALUE(ROK);
} /* End of WrIfmEgtpEguDatInd */


#ifdef FLAT_BUFFER_OPT
/** @brief This function is call handler for eGTP-U Data Indication. 
 *
 * @details
 *
 *     Function: WrIfmEgtpEguDatIndFB
 *
 *         Processing steps:
 *         - This function processing the EGTP Data Indication by using the 
 *           DAM Module function wrDamEgtpDatIndFB. Buffer received from EGTP
 *           is a flat buffer in this case.
 *
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Data Indication (Flat Buffer).
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpEguDatIndFB
(
Pst                          *pst,
FlatBuffer                     *eguMsg,
U32 tnlId
)
{
   S16 ret;
   TRC2(WrIfmEgtpEguDatIndFB);

   ret = wrDamEgtpDatIndFB(pst, eguMsg, tnlId);
   if(ret != ROK)
   {
       /* WR_FREEMFLATBUF(eguMsg); */
   }
   RETVALUE(ROK);
} /* End of WrIfmEgtpEguDatIndFB */
#endif

/** @brief This function is call handler for eGTP-U Status Confirm.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - This function processing the status confirm. 
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Status Confirm.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpEguStaCfm
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   /* We dont invoke StaReq to receive StaCfm. We just release memory and */
   /* go away.                                                            */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));

   RETVALUE(ROK);
} /* end of WrIfmEgtpEguStaCfm */

/** @brief This function is call handler for eGTP-U Unsolicited Status
 *         Indication.
 *
 * @details
 *
 *     Function: WrIfmEgtpEguUStaInd 
 *
 *         Processing steps:
 *         - This function processing the status Indication.
 *
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Status Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpEguUStaInd
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{

   /*Update S1-U interface State*/
   if((eguMsg->u.uStaInd.cfmStatus.status == LCM_PRIM_NOK) 
         && (eguMsg->u.uStaInd.cfmStatus.reason == EGT_GTPU_DEST_REACHABLE))
   {
      wrEmmCb.s1UStatus = TRUE;
     /*127802*/
      RLOG0(L_INFO,"S1-U Path Reachable");
   }
   if((eguMsg->u.uStaInd.cfmStatus.status == LCM_PRIM_NOK) 
         && (eguMsg->u.uStaInd.cfmStatus.reason == EGT_GTPU_DEST_UNREACHABLE))
   {
      wrEmmCb.s1UStatus = FALSE;
      /*127802*/
      RLOG0(L_DEBUG,"S1-U Path UnReachable");
   }

   /* Based on contents of StaInd, we may need to take some action. TODO  */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ROK);
}/* end of WrIfmEgtpEguUStaInd */

/** @brief This function is call handler for eGTP-U Error Indication. 
 *
 * @details
 *
 *     Function: WrIfmEgtpEguErrInd
 *
 *         Processing steps:
 *         - This function is processing the Error indication. 
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Error Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmEgtpEguErrInd
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   U8  ueIdx;
   U8  tnlType;
   U32 lclTeid;

   TRC2(WrIfmEgtpEguErrInd);

   lclTeid = eguMsg->u.errInd.localTeid;
   ueIdx  = (lclTeid & 0x00ffff00) >> 8;
   tnlType= (lclTeid & 0x00000007);


   RLOG_ARG2(L_ERROR, DBG_UEIDX, ueIdx, "EGT Error Ind. Cause:%d LclTnl[%ld]",
            eguMsg->u.errInd.errCause, lclTeid);
   RLOG_STR(L_DEBUG, "EGT Error Ind [TnlType:%s]", (tnlType?"Forwarding":"Nornmal"));
   /* TODO: Send the informtion to Data Application Module */
   /* Freeing EgtUEvnt structure */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));

   RETVALUE(ROK);
} /* end of WrIfmEgtpEguErrInd */

/** @brief This function is call hander for EGTP bind confirm
 *
 * @details
 *
 *     Function: EuLiEgtBndCfm 
 *
 *         Processing steps:
 *         - This function invokes the function WrIfmEgtpBndCfm 
 *           to process the EGTP Bind Confirm.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] status: EGTP Bind Status.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 EuLiEgtBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   RETVALUE(WrIfmEgtpBndCfm(pst,suId,status));
} /* end of EuLiEgtBndCfm */

/** @brief This function is call handler for eGTP Data Indication. 
 *
 * @details
 *
 *     Function: EuLiEgtEguDatInd
 *
 *         Processing steps:
 *         - This function invokes the function WrIfmEgtpEguDatInd
 *           to process the EGTP Data Indication.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Data Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 EuLiEgtEguDatInd
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   RETVALUE(WrIfmEgtpEguDatInd(pst, suId, eguMsg));
} /* End of EuLiEgtEguDatInd */

#ifdef FLAT_BUFFER_OPT
PUBLIC S16 EuLiEgtEguDatIndFB
(
Pst                          *pst,
FlatBuffer                     *eguMsg,
U32              tnlId
)
{
   RETVALUE(WrIfmEgtpEguDatIndFB(pst, eguMsg, tnlId));
} /* End of EuLiEgtEguDatInd */
#endif

/** @brief This function is  call handler for eGTP-U Tunnel Mgmt Confirm.
 *
 * @details
 *
 *     Function: EuLiEgtEguLclTnlMgmtCfm
 *
 *         Processing steps:
 *         - This function is invokes the function WrIfmEgtpEguLclTnlMgmtCfm to
 *           process the Local tunnel management Confirm.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Local tunnel mangament confirm.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   RETVALUE(WrIfmEgtpEguLclTnlMgmtCfm(pst, suId, eguMsg));
} /* end of EuLiEgtEguLclTnlMgmtCfm */

/** @brief This function is call handler for eGTP-U Status Confirm.
 *
 * @details
 *
 *     Function: EuLiEgtEguStaCfm
 *
 *         Processing steps:
 *         - This function invokes the function WrIfmEgtpEguStaCfm to
 *           process the Status Confirm.
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Status Confirm.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 EuLiEgtEguStaCfm
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   RETVALUE(WrIfmEgtpEguStaCfm(pst, suId, eguMsg));
} /* end of EuLiEgtEguStaCfm */

/** @brief This function call handler for eGTP-U Unsolicited Status Indication.
 *
 * @details
 *
 *     Function: EuLiEgtEguUStaInd
 *
 *         Processing steps:
 *         - This function invokes the function WrIfmEgtpEguUStaInd 
 *           to process the status Indication.
 *
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Status Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 EuLiEgtEguUStaInd
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   RETVALUE(WrIfmEgtpEguUStaInd(pst, suId, eguMsg));
} /* end of EuLiEgtEguUStaInd */

/** @brief This function is call handler for eGTP-U Error Indication.
 *
 * @details
 *
 *     Function: EuLiEgtEguErrInd
 *
 *         Processing steps:
 *         - This invokes the function WrIfmEgtpEguErrInd to process the EGTP-U
 *           error indication.
 *
 *
 * @param[in]    pst: POST Data structure.
 * @param[in]   suId: Service User ID.
 * @param[in] eguMsg: eGTP-U Event of Error Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 EuLiEgtEguErrInd
(
Pst                          *pst,
SuId                         suId,
EgtUEvnt                     *eguMsg
)
{
   RETVALUE(WrIfmEgtpEguErrInd(pst, suId, eguMsg));
} /* end of EuLiEgtEguErrInd */




/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
