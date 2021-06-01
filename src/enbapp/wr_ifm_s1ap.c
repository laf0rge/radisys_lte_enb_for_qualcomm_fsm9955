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
               

     File:     wr_ifm_s1ap.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="IFM";

static int RLOG_FILE_ID=26;

static int RLOG_MODULE_ID=4;

#include "wr.h"        /* environment options */
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_ifm_s1ap.h"
#include "wr_lwr.h"
#include "wr_utils.h"
#include "wr_emm_mme.h"
#include "wr_umm_ho.h"
/************************************************************************
                              Prototypes  
************************************************************************/

/* Following matrices define the mapping between the primitives called by the
 * S1AP user (as WrIfmS1apXXXReq) and the corresponding primitives of the 
 * S1AP layer (service provider) (as SzUiSztXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCUZLISZT    loose         cmPkSztXXXReq
 * 1                 SZ          tight         SzUiSztXXXReq
 * 2                LWLCUZLISZT  LW loose      cmPkSztXXXReq
 */


PUBLIC SztBndReq WrIfmS1apBndReqMt [] =
{
   cmPkSztBndReq,        /* 0 - loosely coupled */
   cmPkSztBndReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztBndReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztUbndReq WrIfmS1apUbndReqMt [] =
{
   cmPkSztUbndReq,        /* 0 - loosely coupled */
   cmPkSztUbndReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztUbndReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztUDatReq WrIfmS1apUDatReqMt [] =
{
   cmPkSztUDatReq,        /* 0 - loosely coupled */
   cmPkSztUDatReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztUDatReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};
#ifdef SZTV2
PUBLIC SztAbortS1Req WrIfmS1apAbortS1ReqMt[] =
{
   cmPkSztAbortS1Req,        /* 0 - loosely coupled */
   cmPkSztAbortS1Req,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztAbortS1Req         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};
#endif
PUBLIC SztConnReq WrIfmS1apConReqMt [] =
{
   cmPkSztConReq,        /* 0 - loosely coupled */
   cmPkSztConReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztConReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztConnRsp WrIfmS1apConRspMt [] =
{
   cmPkSztConRsp,        /* 0 - loosely coupled */
   cmPkSztConRsp,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztConRsp         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztRelsReq WrIfmS1apRelReqMt [] =
{
   cmPkSztRelReq,        /* 0 - loosely coupled */
   cmPkSztRelReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztRelReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztRelsRsp WrIfmS1apRelRspMt [] =
{
   cmPkSztRelRsp,        /* 0 - loosely coupled */
   cmPkSztRelRsp,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztRelRsp         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztDatReq WrIfmS1apDatReqMt [] =
{
   cmPkSztDatReq,        /* 0 - loosely coupled */
   cmPkSztDatReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztDatReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};


PUBLIC SztEncDecReq WrIfmS1apEncDecReqMt [] =
{
   cmPkSztEncDecReq,        /* 0 - loosely coupled */
   cmPkSztEncDecReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztEncDecReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};

PUBLIC SztAudReq WrIfmS1apAudReqMt [] =
{
   cmPkSztAudReq,        /* 0 - loosely coupled */
   cmPkSztAudReq,        /* 1 - light weight loosely coupled */
#ifdef SZ
   SzUiSztAudReq         /* 2 - tightly coupled, stub layer */
#endif /* SZ */
};

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PUBLIC SztDynamPeerAssocRsp WrIfmS1apPeerAssocRspMt [] =
{
   cmPkSztPeerAssocRsp,        /* 0 - loosely coupled */
   cmPkSztPeerAssocRsp,        /* 1 - light weight loosely coupled */
   SzUiSztPeerAssocRsp         /* 2 - tightly coupled, stub layer */
};
#endif

/************************************************************************
                       SZT Interface Functions
************************************************************************/

/** @brief This function resolves the SztBndReq primitive 
 *
 * @details
 *
 *     Function: WrIfmS1apBndReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the S1AP SAP bind request message 
 *           to S1AP layer.
 *
 * @param[in] pst: POST Data structure.  
 * @param[in] suId: Service User ID.
 * @param[in] spId: Service Provider ID.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apBndReq
(
Pst                          *pst,
SuId                          suId,
SpId                          spId
)
{
   (*WrIfmS1apBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);
} /* end of WrIfmS1apBndReq */

/** @brief This function resolves the SztUbndReq primitive
 *
 * @details
 *
 *     Function: WrIfmS1apUbndReq 
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the S1AP SAP Unbind request message 
 *           to S1AP layer.
 *
 * @param[in] pst: POST Data Structure.
 * @param[in] spId: Serivice Provider ID.
 * @param[in] reason: Unbind Reason.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apUbndReq
(
Pst                          *pst,
SpId                         spId,
Reason                       reason
)
{
   (*WrIfmS1apUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);
} /* end of WrIfmS1apUbndReq */

/** @brief This function sends S1AP Management message to S1AP. 
 *
 * @details
 *
 *     Function: WrIfmS1apSndMgmtMsg
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the S1AP Management message
 *           to S1AP layer.
 *         - This function is invoked by EMM Module.
 *
 *
 * @param[in] uDatEvnt: S1AP connection-less data event.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apSndMgmtMsg
(
SztUDatEvnt                  *uDatEvnt
)
{
   WrLiSapCb                 *s1apSap = wrCb.sztSap[0];

   (*WrIfmS1apUDatReqMt[(s1apSap->pst.selector)])(&(s1apSap->pst), 
                                                  s1apSap->spId, uDatEvnt);

   RETVALUE(ROK);
} /* end of WrIfmS1apSndMgmtMsg */
#ifdef SZTV2
/** @brief This function sends S1AP Abort Message
 *
 * @details
 *
 *     Function: WrIfmS1apSndAbortMsg
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the S1AP Abort message
 *           to S1AP layer.
 *         - This function is invoked by EMM Module.
 *
 *
 * @param[in] abortS1:
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apSndAbortMsg
(
SztAbortS1    *abortS1
)
{
   WrLiSapCb                 *s1apSap = wrCb.sztSap[0];

   (*WrIfmS1apAbortS1ReqMt[(s1apSap->pst.selector)])(&(s1apSap->pst),
                                                     s1apSap->spId, abortS1);
   RETVALUE(ROK);
}/* end of WrIfmS1apSndAbortMsg */
#endif
/** @brief This function resolves the SztConReq primitive
 *
 * @details
 *
 *     Function: WrIfmS1apConReq 
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Connection Request message 
 *           to S1AP layer.
 *         - This function is invoked by UMM Module.
 *
 * @param[in] conReq: connection Request.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apConReq
(
SztConReq                    *conReq
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   (*WrIfmS1apConReqMt[pst->selector])(pst, wrCb.sztSap[0]->spId, conReq);

   RETVALUE(ROK);
} /* end of WrIfmS1apConReq */

/** @brief This function resolves the SztConRsp primitive
 *
 * @details
 *
 *     Function: WrIfmS1apConRsp 
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Connection Reponse message 
 *           to S1AP layer.
 *
 * @param[in] conRsp: Connection Response.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apConRsp
(
SztConRsp                    *conRsp
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   (*WrIfmS1apConRspMt[pst->selector])(pst, wrCb.sztSap[0]->spId, conRsp);

   RETVALUE(ROK);
} /* end of WrIfmS1apConRsp */

/** @brief This function resolves the SztRelReq primitive 
 *
 * @details
 *
 *     Function: WrIfmS1apRelReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Release Request message 
 *           to S1AP layer.
 *
 * @param[in] relReq: Release Request 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apRelReq
(
SztRelReq                    *relReq
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   (*WrIfmS1apRelReqMt[pst->selector])(pst, wrCb.sztSap[0]->spId, relReq);

   RETVALUE(ROK);
} /* end of WrIfmS1apRelReq */

/** @brief This function resolves the SztRelRsp primitive 
 *
 * @details
 *
 *     Function: WrIfmS1apRelRsp
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Release Resp message 
 *           to S1AP layer.
 *         - This function is invoked by UMM Module.
 *
 * @param[in] relRsp: Release Response.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apRelRsp
(
SztRelRsp                    *relRsp
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   (*WrIfmS1apRelRspMt[pst->selector])(pst, wrCb.sztSap[0]->spId, relRsp);

   RETVALUE(ROK);
} /* WrIfmS1apRelRsp */

/** @brief This function resolves the SztDatReq primitive
 *
 * @details
 *
 *     Function: WrIfmS1apDatReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Dat Request message 
 *           to S1AP layer.
 *         - This function is invoked by UMM Module.
 *
 * @param[in] datEvnt: S1AP Dat event
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apDatReq
(
SztDatEvntReq                *datEvnt
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   if(ROK != 
       (*WrIfmS1apDatReqMt[pst->selector])(pst, wrCb.sztSap[0]->spId, datEvnt))
   {
      RETVALUE(RFAILED);
   }   

   RETVALUE(ROK);
} /* end of WrIfmS1apDatReq */

/** @brief This function resolves the SztEncDecReq primitive 
 *
 * @details
 *
 *     Function: WrIfmS1apEncDecReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Encode Decode Request message 
 *           to S1AP layer.
 *
 * @param[in] sztEncDecEvnt: S1AP encode decode event.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apEncDecReq
(
SztEncDecEvnt                *sztEncDecEvnt
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   (*WrIfmS1apEncDecReqMt[pst->selector])(pst, 
                                          wrCb.sztSap[0]->spId, sztEncDecEvnt);

   RETVALUE(ROK);
} /* WrIfmS1apEncDecReq */

/** @brief This function resolves the SztAudReq primitive
 *
 * @details
 *
 *     Function: WrIfmS1apAudReq
 *
 *         Processing steps:
 *         - Based on the selector value, this primitve will call
 *           tightly colupled, loosely coupled or light weight loosly 
 *           coupled function to send the Audit connection Request message 
 *           to S1AP layer.
 *
 *
 * @param[in] audEvnt:  Audit event.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 WrIfmS1apAudReq
(
SztAudEvnt                   *audEvnt
)
{
   Pst                       *pst = &wrCb.sztSap[0]->pst;

   (*WrIfmS1apAudReqMt[pst->selector])(pst, wrCb.sztSap[0]->spId, audEvnt);

   RETVALUE(ROK);
} /* end of WrIfmS1apAudReq */

/** @brief This function sends the UE specific message to S1AP.
 *
 * @details
 *
 *     Function: wrIfmS1apSndUeMsg
 *
 *         Processing steps:
 *         - This function fills the S1AP Dat event and invokes the wrapper 
 *           function WrIfmS1apDatReq to send to S1AP protocal layer.
 *         - This function is invoked by UMM Module.
 *
 * @param[in] spConnId: Service Provider Connection ID.
 * @param[in]   s1pdu: Prepared S1AP PDU
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmS1apSndUeMsg
(
U32                          spConnId,
S1apPdu                      *s1pdu
)
{
   SztDatEvntReq             datEvt;

   /* populate the event structure */
   datEvt.spConnId = spConnId;
   datEvt.pdu      = s1pdu;

   WrIfmS1apDatReq(&datEvt);

   RETVALUE(ROK);
} /* end of wrIfmS1apSndUeMsg */


/**************************************************************
   SZT interface handler functions
***************************************************************/
/** @brief This function is call handler to S1AP connection 
 *         less data indication. 
 *
 * @details This function is a wrapper function to call EMM module function to
 *          process the S1AP connection-less data indication.
 *
 *     Function: WrIfmS1apUDatInd 
 *
 *         Processing steps:
 *         - Invoke the EMM Module function wrEmmMmePrcRcvdPdu to process 
 *           S1AP connection-less data indication. 
 *         - Free the processed S1AP PDU.
 *
 * @param[in]        pst: Post data structure
 * @param[in]       suId: Service User SAP ID. 
 * @param[out] uDatEvnt: S1AP connection-less data indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apUDatInd
(
Pst                          *pst,
SuId                         suId,
SztUDatEvnt                  *uDatEvnt
)
{
   RLOG0(L_DEBUG, "WrIfmS1apUDatInd: SZT UData Ind");
   wrEmmMmePrcRcvdPdu (uDatEvnt->peerId.val, uDatEvnt->pdu);
   cmFreeMem ((Ptr)(uDatEvnt->pdu));
   RETVALUE(ROK);
} /* end of WrIfmS1apUDatInd */ 

#ifdef SZTV2
/** @brief This function is process the S1AP Connection indication 
 *
 * @details
 *
 *     Function: WrIfmS1apAbortS1Cfm
 *
 *         Processing steps:
 *         - Call Emm Module function to process the message
 *
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[out] conInd: S1AP Abort confirm
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apAbortS1Cfm
(
Pst                          *pst,
SuId                         suId,
SztAbortS1                   *abortS1
)
{
   RLOG0(L_DEBUG, "WrIfmS1apAbortS1Cfm: SZT AbortS1 Cfm");
   wrEmmMmePrcS1Abort(abortS1);
   RETVALUE(ROK);
} /* End of WrIfmS1apAbortS1Cfm */
#endif
/** @brief This function is to process the S1AP connection conformation. 
 *
 * @details This function is a wrapper function to call UMM module function to
 *          process the S1AP Connection Conformation.
 *
 *     Function: WrIfmS1apConCfm
 *
 *         Processing steps:
 *         - Invoke the UMM Module function wrUmmPrcS1apConCfm to process 
 *           S1AP connection conformation. 
 *
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[out] conCfm: S1AP Connection Conformation.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apConCfm
(
Pst                          *pst,
SuId                         suId,
SztConCfm                    *conCfm
)
{
   RLOG0(L_DEBUG, "WrIfmS1apConCfm: SZT Connection Cfm");
   wrUmmPrcS1apConCfm(conCfm);
   RETVALUE(ROK);
} /* end of WrIfmS1apConCfm */

/** @brief This function is to process the S1AP Bind conformation. 
 *
 * @details
 *
 *     Function: WrIfmS1apBndCfm
 *
 *         Processing steps:
 *         - if the status is BIND_OK, then stop the S1AP Bind timer.
 *         - send an ALARAM to layer manager.
 *      
 *     Notes: TODO: Implmentation here should be moved to wr_mi.c
 *
 *
 * @param[in]        pst: Post data structure
 * @param[in]       suId: Service User SAP ID. 
 * @param[in]     status: S1AP Bind conformation.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   /* Send alarm */
   if (CM_BND_OK == status)
   {
      wrStopTmr((PTR)wrCb.sztSap[0],WR_TMR_SZT_SAP_BND);
      RLOG0(L_INFO, "[ALARM] Sending SZT_SAP_BND_OK to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LWR_CAUSE_SZT_SAP_BOUND);
   }
   else
   {
      RLOG0(L_FATAL, "[ALARM] Sending SZT_SAP_BND_FAIL to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }
   RETVALUE(ROK);
} /* End of WrIfmS1apBndCfm */

/** @brief This function is process the S1AP Connection indication 
 *
 * @details
 *
 *     Function: WrIfmS1apConInd
 *
 *         Processing steps:
 *         - Free the S1AP PDU.
 *
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[out] conInd: S1AP Connection Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apConInd
(
Pst                          *pst,
SuId                         suId,
SztConInd                    *conInd
)
{
    /* Send HO Failure and S1AP Con Release if failed to process HO Req */
    TknU8     *mmeUeS1apIdTypeIE = NULLP;
    U32       mme_ue_s1ap_id = 0;
    U32       cause;

    if ( wrUmmPrcS1ConInd(conInd,&cause) != ROK)
    {
       wrSzGetIE(&(conInd->pdu->pdu), Sztid_MME_UE_S1AP_ID, &mmeUeS1apIdTypeIE);
       if(NULLP != mmeUeS1apIdTypeIE)
       {
       mme_ue_s1ap_id = ((SztMME_UE_S1AP_ID *)mmeUeS1apIdTypeIE)->val;
       }
       wrUmmHoTgtSndHoFail(conInd->u.spConnId, mme_ue_s1ap_id, 0, cause);
       WR_FREE_EVNT(conInd->pdu);
    }
   RETVALUE(ROK);
} /* End of WrIfmS1apConInd */
/** @brief This function is a wrapper function to call UMM module function to
 *         process the S1AP Release Conformation. 
 *
 * @details This function is a wrapper function to call UMM module function to
 *          process the S1AP Release Conformation.
 *
 *     Function: WrIfmS1apRelCfm
 *
 *         Processing steps:
 *         - Invoke the UMM Module function wrUmmPrcS1RelCfm to process 
 *           S1AP Release conformation. 
 *
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[in] relCfm: S1AP Release Conformation.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apRelCfm
(
Pst                          *pst,
SuId                         suId,
SztRelCfm                    *relCfm
)
{

   RLOG0(L_DEBUG, "WrIfmS1apRelCfm: SZT Release Cfm");
   wrUmmPrcS1RelCfm(relCfm);
   RETVALUE(ROK);
} /* End of WrIfmS1apRelCfm */

/** @brief This function is a wrapper function to call UMM module function to
 *         process the S1AP Data Indication. 
 *
 * @details
 *
 *     Function: WrIfmS1apDatInd 
 *
 *         Processing steps:
 *         - Invoke the UMM Module function wrUmmPrcS1DatInd to process 
 *           S1AP Data Indication. 
 *
 * @param[in]      pst: Post data structure
 * @param[in]     suId: Service User SAP ID. 
 * @param[in] datEvnt: S1AP Data Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apDatInd
(
Pst                          *pst,
SuId                         suId,
SztDatEvntInd                *datEvnt
)
{
   wrUmmPrcS1DatInd(datEvnt);
   RETVALUE(ROK);
} /* End of WrIfmS1apDatInd */

/** @brief This function is a wrapper function to call UMM module function to
 *         process the S1AP Release Indication. 
 *
 * @details This function is a wrapper function to call UMM module function to
 *          process the S1AP Release Indication.
 *
 *     Function: WrIfmS1apRelInd
 *
 *         Processing steps:
 *         - Invoke the UMM Module function wrUmmPrcS1RelInd to process 
 *           the S1AP Release Indication. 
 *          
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[in]  relInd: S1AP Release Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apRelInd
(
Pst                          *pst,
SuId                         suId,
SztRelInd                    *relInd
)
{
   wrUmmPrcS1RelInd(relInd);
   RETVALUE(ROK);
} /* End of WrIfmS1apRelInd */

/** @brief This function is a wrapper function to call UMM module function to
 *         process the S1AP Data Indication. 
 *
 * @details
 *
 *     Function: WrIfmS1apErrInd 
 *
 *         Processing steps:
 *         - Invoke the UMM Module function wrUmmPrcS1ErrInd to process 
 *           S1AP Data Indication. 
 *
 * @param[in]      pst: Post data structure
 * @param[in]     suId: Service User SAP ID. 
 * @param[in] datEvnt: S1AP Data Indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apErrInd
(
Pst                          *pst,
SuId                         suId,
SztLclErrInd                *errInd
)
{
   if(errInd->causeType == SZT_CAUSE_DUP_CONID)
   {
      wrUmmPrcS1ErrInd(errInd);
   }
   RETVALUE(ROK);
} /* End of WrIfmS1apErrInd */
/** @brief This is a wrapper function to process the status indication.
 *
 * @details This is a wrapper function to process the status indication.
 *
 *     Function: WrIfmS1apStaInd
 *
 *         Processing steps:
 *         - Invoke the function wrEmmPrcS1StaInd to process the 
 *           S1AP status indication.
 *
 * @param[in]        post: Post data structure
 * @param[in]       suId: Service User SAP ID. 
 * @param[in]    sztSta: S1AP Status indication.
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 WrIfmS1apStaInd
(
Pst                          *post,
SuId                         suId,
SztStaInd                    *sztSta
)
{
   wrEmmPrcS1StaInd(sztSta);
   RETVALUE(ROK);
} /* End of WrIfmS1apStaInd */


/**************************************************************
   UZ interface wrapper functions
***************************************************************/
/** @brief This function is call handler for Bind Confirmation. 
 *
 * @details This function is call handler for Bind Confirmation.
 *
 *     Function: UzLiSztBndCfm 
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apBndCfm to process the 
 *           S1AP Bind conformation.
 *          
 * @param[in]        pst: Post data structure
 * @param[in]       suId: Service User SAP ID. 
 * @param[in]     status: S1AP Bind conformation.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   WrIfmS1apBndCfm(pst, suId, status);
   RETVALUE(ROK);
} /* End of UzLiSztBndCfm */

/** @brief This function is call handler for Aduit Confirmation 
 *
 * @details This function is call handler for Aduit Confirmation
 *
 *     Function: UzLiSztAudCfm
 *
 *         Processing steps:
 *         - 
 *     Notes: eNB application does not audit connections. Just go away here
 *
 * @param[in]          pst: Post data structure
 * @param[in]          suId: Service User SAP ID. 
 * @param[out]      audEvnt: S1AP Aduit Confirmation.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztAudCfm
(
Pst                          *pst,
SuId                         suId,
SztAudEvnt                   *audEvnt
)
{
   RETVALUE(ROK);
} /* End of UzLiSztAudCfm */

/** @brief This function is call handler for Status Indication 
 *
 * @details This function is call handler for Status Indication
 *
 *     Function: UzLiSztStaInd 
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apStaInd to process the 
 *           S1AP status indication.
 *          
 * @param[in]        post: Post data structure
 * @param[in]       suId: Service User SAP ID. 
 * @param[in]    sztSta: S1AP Status indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztStaInd
(
Pst                          *post,
SuId                         suId,
SztStaInd                    *sztSta
)
{
   WrIfmS1apStaInd(post, suId, sztSta);
   RETVALUE(ROK);
} /* End of UzLiSztStaInd */

/** @brief This function is call handler for flow control indication. 
 *
 * @details This function is call handler for flow control indication.
 *
 *     Function: UzLiSztFlcInd 
 *
 *         Processing steps:
 *         - 
 *     Notes: Yet to define functionality to manage flow control
 *
 * @param[in]          post: Post data structure
 * @param[in]          suId: Service User SAP ID. 
 * @param[out]       flcInd: S1AP Flow control Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztFlcInd
(
Pst                          *post,
SuId                         suId,
SztFlcInd                    *flcInd
)
{
   RETVALUE(ROK);
} /* End of UzLiSztFlcInd */

/** @brief This function is call handler for Error Indication.
 *
 * @details This function is call handler for Error Indication.
 *
 *     Function: UzLiSztErrInd
 *
 *         Processing steps:
 *         - 
 *       Notes: Yet to define functionality for Error Indication
 *
 *
 * @param[in]          post: Post data structure
 * @param[in]          suId: Service User SAP ID. 
 * @param[out] sztLclErrInd: S1AP Error Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztErrInd
(
Pst                          *post,
SuId                         suId,
SztLclErrInd                 *sztLclErrInd
)
{

   WrCellId      cellId;
   U8            ueIdx = 0;
   WrUeCb        *ueCb = NULLP;
   if(sztLclErrInd->causeType == SZT_CAUSE_INV_CONID)
   {
      WR_GET_CELLID_FRM_SUCONID(sztLclErrInd->transId.val, cellId, ueIdx);
      wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);

      if(ueCb != NULLP)
      {
         /* Fix for ccpu00125889: Added print for Err Ind causeval and type*/
        RLOG_ARG2(L_ERROR, DBG_CRNTI, ueCb->crnti, "S1AP Error Indication Received CauseVal:%d CauseType:%d", 
               sztLclErrInd->causeValue,sztLclErrInd->causeType);
      }
      else
      {
        RLOG2(L_ERROR, "UE got deleted later S1AP Error Indication Received CauseVal:%d CauseType:%d", 
               sztLclErrInd->causeValue,sztLclErrInd->causeType);
      }
   }
   WrIfmS1apErrInd(post,suId,sztLclErrInd);
   RETVALUE(ROK);
} /* End of UzLiSztErrInd */

/** @brief This function is call handler for Connection-less data indication.
 *
 * @details This function is call handler for Connection-less data indication.
 *
 *     Function: UzLiSztUDatInd
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apUDatInd to process the 
 *           S1AP Connection-less data indication.
 *          
 * @param[in]        pst: Post data structure
 * @param[in]       suId: Service User SAP ID. 
 * @param[out] uDatEvnt: S1AP connection-less data indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztUDatInd
(
Pst                          *pst,
SuId                         suId,
SztUDatEvnt                  *uDatEvnt
)
{
   WrIfmS1apUDatInd(pst, suId, uDatEvnt);
   RETVALUE(ROK);
} /* End of UzLiSztUDatInd */

/** @brief This function is call handler for Connection Indication. 
 *
 * @details This function is call handler for Connection Indication.
 *
 *     Function: UzLiSztConInd 
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apConInd to process the S1AP Connection
 *           Indication.
 *          
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[out] conInd: S1AP Connection Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztConInd
(
Pst                          *pst,
SuId                         suId,
SztConInd                    *conInd
)
{
   WrIfmS1apConInd(pst, suId, conInd);
   RETVALUE(ROK);
} /* End of UzLiSztConInd */
#ifdef SZTV2
/** @brief This function is call handler for Abort S1 Confirmation.
 *
 * @details This function is call handler for Abort S1 Confirmation.
 *
 *     Function: UzLiSztAbortS1Cfm
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apAbortS1Cfm to process the S1AP Abort
 *           Conformation.
 *          
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[out] abortS1: S1AP Abort Conformation.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztAbortS1Cfm 
(
Pst                          *pst,
SuId                         suId,
SztAbortS1                  *abortS1
)
{
   WrIfmS1apAbortS1Cfm(pst, suId, abortS1);
   RETVALUE(ROK);
} /* End of UzLiSztAbortS1Cfm */
#endif
/** @brief This function is call handler for Connection Confirmation.
 *
 * @details This function is call handler for Connection Confirmation.
 *
 *     Function: UzLiSztConCfm
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apConCfm to process the S1AP Connection
 *           Conformation.
 *          
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[out] conCfm: S1AP Connection Conformation.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztConCfm
(
Pst                          *pst,
SuId                         suId,
SztConCfm                    *conCfm
)
{
   WrIfmS1apConCfm(pst, suId, conCfm);
   RETVALUE(ROK);
} /* End of UzLiSztConCfm */
/** @brief This function is call handler for Release Indication.
 *
 * @details This function is call handler for Release Indication.
 *
 *     Function: UzLiSztRelInd
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apRelInd to process the S1AP Release 
 *           Indication. 
 *          
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[in] relInd: S1AP Release Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztRelInd
(
Pst                          *pst,
SuId                         suId,
SztRelInd                    *relInd
)
{
   WrIfmS1apRelInd(pst, suId, relInd);
   RETVALUE(ROK);
} /* End of UzLiSztRelInd */

/** @brief This function is call handler for Release Confirmation.
 *
 * @details This function is call handler for Release Confirmation.
 *
 *     Function: UzLiSztRelCfm
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apRelCfm to process the S1AP Release 
 *           Conformation
 *          
 * @param[in]     pst: Post data structure
 * @param[in]    suId: Service User SAP ID. 
 * @param[in] relCfm: S1AP Release Conformation.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztRelCfm
(
Pst                          *pst,
SuId                         suId,
SztRelCfm                    *relCfm
)
{
   WrIfmS1apRelCfm(pst, suId, relCfm);
   RETVALUE(ROK);
} /* End of UzLiSztRelCfm */

/** @brief This function is call handler for connection based data indication.
 *
 * @details This function is call handler for connection based data indication.
 *
 *     Function: UzLiSztDatInd 
 *
 *         Processing steps:
 *         - Invoke the function WrIfmS1apDatInd to process the S1AP Data
 *           Indication. 
 *
 * @param[in]      pst: Post data structure
 * @param[in]     suId: Service User SAP ID. 
 * @param[in] datEvnt: S1AP Data Indication.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztDatInd
(
Pst                          *pst,
SuId                         suId,
SztDatEvntInd                *datEvnt
)
{
   WrIfmS1apDatInd(pst, suId, datEvnt);
   RETVALUE(ROK);
} /* End of UzLiSztDatInd */

PUBLIC S16 WrIfmSztEncDecCfm
(
Pst                          *pst,
SuId                         suId,
SztEncDecEvnt                *sztEncDecEvnt,
SztReqStatus                 *status
)
{

   TRC2(WrIfmSztEncDecCfm);

   RLOG0(L_DEBUG, "WrIfmSztEncDecCfm: S1AP Decode Cfm Received");
   wrUmmPrcSztEncDecCfm(sztEncDecEvnt, status);
   RETVALUE(ROK);
}

/** @brief This function is call handler for Encode/Decode confirmation.
 *
 * @details This function is call handler for Encode/Decode confirmation.
 *
 *     Function: UzLiSztEncDecCfm
 *
 *         Processing steps:
 *         - 
 *     Notes: eNB does not use this functionality yet. Nevertheless, we
 *            must clean up memory if we receive this.
 *
 * @param[in]            pst: Post data structure
 * @param[in]           suId: Service User SAP ID. 
 * @param[out] sztEncDecEvnt: S1AP Encode and Decode Event.
 * @param[out]        status: S1AP Req status.
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 UzLiSztEncDecCfm
(
Pst                          *pst,
SuId                         suId,
SztEncDecEvnt                *sztEncDecEvnt,
SztReqStatus                 *status
)
{
   WrIfmSztEncDecCfm(pst, suId, sztEncDecEvnt, status);
   if ((sztEncDecEvnt->type.val == SZT_REQ_TYPE_ENCODE) 
         ||(sztEncDecEvnt->type.val ==  SZT_REQ_TYPE_DECODE_IE))
   {
      if(sztEncDecEvnt->u.pdu)
      {
         cmFreeMem(sztEncDecEvnt->u.pdu);
      }
   }
   else
   {
      if(sztEncDecEvnt->u.mBuf)
      {
         WR_FREEMBUF(sztEncDecEvnt->u.mBuf);
      }
   }
   RETVALUE(ROK);
} /* End of UzLiSztEncDecCfm */


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
