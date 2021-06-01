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



/**********************************************************************

     Name:     LTE-X2AP Layer 
  
     Type:     C Source file
  
     Desc:     This is the upper interface function handlers. 

     File:     cz_uim.c

     Sid:      tr_uim.c@@/main/2 - Tue Aug 30 18:36:16 2011

     Prg:       

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common Asn.1 library           */
#include "sct.h"           /* SCT interface defines           */
#include "czt.h"           /* CZT interface defines           */
#include "lcz.h"           /* X2AP Layer management           */
#include "cz.h"            /* X2AP layer defines              */
#include "cz_err.h"        /* X2AP - error */


/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASn.1 Library           */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT Interface includes */
#include "lcz.x"           /* LCZ Interface includes */
#include "cz.x"            /* X2AP includes */
/* cz001.301: Diagnostics change*/
#ifdef SS_DIAG
#include "ss_diag.h"        /* Common log file */
#endif
/*Private Functions */


/* Function which will check the SAP Status of Alll Upper API */
PRIVATE S16 czUimGetAndChkCztSap ARGS((SpId    spId, CzCztSapCb  **uSapCb));
PRIVATE S16 czUimValidatePeerAssocRsp ARGS(( CzDynamPeerAssocCb *peerAssocCb,
                                        CztPeerAssocRsp    *peerAssocRsp,
                                        U8                 *reason));
/*kworks fix: changed type of cause U16 to U8*/
PRIVATE S16 czUiDoAudit ARGS ((CztAudEvnt *audit, 
                               CzCztSapCb *cztSapCb, 
                               U8 *cause));

/** @defgroup uimmodule X2AP Upper Interface Module
 *  @{
 */


/** This macro is used to get key to retrieve UE CB */
#define CZ_UIM_GET_KEY(_keyVal, _keyPrefix, _key)  \
      _key = PutHiWord(_keyVal, _keyPrefix);

/** This macro is used to get key to retrieve Resource CB */
#define CZ_UIM_GET_RSRC_KEY(_key, _eNB1MsrmntId, _dir) \
         _key=(_eNB1MsrmntId | _dir);




/**
    @brief CzUiCztBndReq binds a User to the X2AP upper interface

    @details Inside this function X2AP layer software registers this new User and allocates a 
             Service Access Point for this bind and records the identity
             of the service user. It also issues a CzUiCztBndCfm to the 
             service user after completing a successful bind.

    @param[in] pst This is the Pst of the message.
    @param[in] suId The suId reperesnts the User identifier for this SAP.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
 */
  
#ifdef ANSI
PUBLIC S16 CzUiCztBndReq
(
Pst *pst,             /* Post structure */
SuId suId,            /* service user id */
SpId spId             /* service provider id */
)
#else
PUBLIC S16 CzUiCztBndReq(pst, suId, spId)
Pst *pst;             /* Post Structure */
SuId suId;             /* service user id */
SpId spId;             /* service provider id */
#endif
{
   S16        ret;      /* return value */
   CzUstaDgn  dgn;      /* unsolicited status diagnostics structure */     
   CzCztSapCb *uSapCb = NULLP;  /* pointer to current SAP */

   CZ_TRC3(CzUiCztBndReq);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), spId (%d)\n", 
            (Void *)pst, suId, spId));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK) ||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT159, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /*SS_MULTIPLE_PROCS */


    /* Check if general configuration is done */
   CZ_CHK_LYR_CFG(CzUiCztBndReq, ret);
   if( ret != ROK)
   {
      /* Layer is not yet configured. Ignore this
       * stray BndReq and return 
       */
      CZ_RETVALUE(RFAILED);
   }


   /* Validation of input parameters */
   
   ret = RFAILED;

   /* Change the value of the ret based on the presence of uSapCb pointer */ 
   if ((spId < czCb.genCfg.nmbCztSaps) && (spId >= 0))
   { 
      uSapCb = czCb.cztSapLst[spId]; 
      if (uSapCb != NULLP)
      {
         ret = ROK;
      }
   }

   if (ret == RFAILED)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid SAP : spId(%d)\n",
               spId));
      dgn.alarmType    = LCZ_USTA_DGNVAL_SAPID;
      dgn.u.sapId = spId;

      /* Send Alarm to the layer manager */
      czLmmSendLmAlarm(LCM_CATEGORY_INTERFACE, LCZ_EVENT_UI_INV_EVT, 
            LCZ_CAUSE_INV_SPID, &dgn);

      CZ_RETVALUE(RFAILED);
   }

   /* Check if the SAP is already bound */
   if (uSapCb->state != LCZ_SAP_ISBOUND)
   {
      /* copy bind configuration parameters in sap */
      uSapCb->suId          = suId;
      uSapCb->pst.dstProcId = pst->srcProcId;
      uSapCb->pst.dstEnt    = pst->srcEnt;
      uSapCb->pst.dstInst   = pst->srcInst;
 
      /* state bound and enabled */
      uSapCb->state = LCZ_SAP_ISBOUND;
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Bind Request successful for (pst, suId(%d), spId(%d))\n",
               suId, spId));
   }

   /* if the sap is already bound, a Bind confirm is given to
      the user and no state change is made */

   /* send an acknowledgement back to the user */
   (Void) CzUiCztBndCfm(&uSapCb->pst, suId, CM_BND_OK);

   CZ_RETVALUE(ROK);
} /* end of CzUiCztBndReq */



/**
    @brief  CzUiCztUbndReq unbinds a User to the X2AP upper interface

    @details This function unbinds the user SAP. The service access point
             is not de-allocated, so a new bind bind request can be issued
             on this SAP (without reconfiguring the SAP in the service
             provider).

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] reason This explains the reason for unbinding
   
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.

*/ 


#ifdef ANSI
PUBLIC S16 CzUiCztUbndReq
( 
Pst *pst,             /* Post structure */
SpId spId,             /* service provider id */
Reason reason          /* Reason for unbinding */
)   
#else
PUBLIC S16 CzUiCztUbndReq(pst, spId, reason) 
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
Reason reason;         /* Reason for unbinding */
#endif
{
   S16         ret = RFAILED;     /* return value */ 
   CzCztSapCb   *uSapCb = NULLP;  /* pointer to Upper SAP */

   CZ_TRC3(CzUiCztUbndReq);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Reason (%d)\n", 
            (Void *)pst, spId, reason));
#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK)||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT160, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#else /* SS_MULTIPROCS */
   UNUSED(pst);
#endif /*SS_MULTIPLE_PROCS */

   UNUSED(reason);

   /* Check if general configuration is done */
   CZ_CHK_LYR_CFG(CzUiCztUbndReq, ret);
   if( ret != ROK)
   {
      /* Layer is not yet configured. Ignore this
       * stray UnbndReq and return
       */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Unbind Request Failed, Config Not Done \n"));
      CZ_RETVALUE(RFAILED);
   }

   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }

   uSapCb->state = LCZ_SAP_CFG;
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "UnBind Request successful for (pst, spId(%d))\n",
            spId));

   CZ_RETVALUE(ROK);
} /* end of CzUiCztUbndReq */

/**
    @brief  CzUiCztGpReq is called by User for invoking General Procedure

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of  
             the GPM.Also it checks for the Interface level Error Checking.This is
             used for sending the General Procedure.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] gpReq This is the PDU for the Genral Procedure Message

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16 CzUiCztGpReq
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztEvnt *gpReq      /* GP sdus */
)
#else
PUBLIC S16 CzUiCztGpReq(pst, spId, gpReq)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztEvnt *gpReq; /* connectionless sdus */
#endif
{  
   S16         ret = RFAILED;    /* return value */
   CzCztSapCb  *uSapCb=NULLP;    /* Upper Sap */ 
   CzPeerCb    *peer = NULLP;    /* Peer Control Block*/
   U8           msgType;          /* Message Type*/
   S16         procType;       /* Procedure Type*/    
   Bool          isResponse;   /*  */

   CZ_TRC3(CzUiCztGpReq);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Czt Event Address (%p)\n",
            (Void *)pst, spId,(Void *) gpReq));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK)||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT161, (ErrVal)0, "failed, cannot derive czCb");
      if(gpReq)
      {
         CZ_FREEEVNT(gpReq);
      }
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTIPLE_PROCS */

   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));


   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(gpReq)
      {
         CZ_FREEEVNT(gpReq);
      }
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(gpReq == NULLP)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Failed, Null gpReq Pointer\n"));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, 0, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, NULLP);
      CZ_RETVALUE(RFAILED);
   }
#endif
      
   /* Retrieve the X2AP Message type */
   msgType = czUtlGetMsgType(&gpReq->pdu, &isResponse, &procType);
   if (msgType == CZT_MSG_UNKNOWN)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "failed, Unknown Message Received MsgType(%d)\n",
         msgType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, gpReq->peerId, CZT_ERRTYPE_INV_PARAM, 
                         CZT_ERRCAUSE_UNEXP_MSG, gpReq);
      CZ_RETVALUE(RFAILED);
   }
   /*cz001.301: Change for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120001, CZ_DIAG_PEERID, gpReq->peerId,
               "Received GpReq from Application, msgType  = %d",
                msgType, 0, 0, 0)
#endif
   /* Validate whether this is GPM Message or BMP Message */
   if (procType != CZ_GP)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, gpReq->peerId, CZT_ERRTYPE_INV_PARAM, 
                               CZT_ERRCAUSE_UNEXP_MSG, gpReq);
      CZ_RETVALUE(RFAILED);
   }
#ifndef CZ_PERF
   if((isResponse == TRUE) || (msgType == CZT_MSG_ERR_IND))
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, gpReq->peerId, CZT_ERRTYPE_INV_PARAM, 
                               CZT_ERRCAUSE_UNEXP_MSG, gpReq);
      CZ_RETVALUE(RFAILED);
   }
#endif
   ret = czNmmFndPeer(gpReq->peerId, &peer); 
   if(ret != ROK)
   {
#ifndef ALIGN_64BIT      
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer Could not be found for peerId(%ld)\n", 
         gpReq->peerId));
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer Could not be found for peerId(%d)\n", 
         gpReq->peerId));
#endif
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb,gpReq->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_INV_PEERID, gpReq);
      CZ_RETVALUE(RFAILED);
   }
 
   ret = czGpmFsmMsgHandler (peer, gpReq, msgType, CZ_DIR_OUT);  
   
   /* ErrInd will be sent by the respective Module */
   if (ret != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ROK);

} /* end of CzUiCztGpReq */

/**
    @brief  CzUiCztGpRsp is called by User for invoking General Procedure Response

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of 
             the GPM.Also it checks for the Interface level Error Checking.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] gpRsp This is the PDU for the Genral Procedure Message 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16 CzUiCztGpRsp 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztEvnt *gpRsp   /* connectionless sdus */
)
#else
PUBLIC S16 CzUiCztGpRsp(pst, spId, gpRsp)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztEvnt *gpRsp; /* connectionless sdus */
#endif
{
   S16         ret = RFAILED; /* return value */
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CzPeerCb    *peer = NULLP;
   U8          msgType;
   S16         procType;       /* Procedure Type*/    
   Bool          isResponse;

   CZ_TRC3(CzUiCztGpRsp);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Czt Event Address (%p)\n",
            (Void *)pst, spId, (Void *)gpRsp));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK)||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT162, (ErrVal)0, "failed, cannot derive czCb");
      if(gpRsp)
         CZ_FREEEVNT(gpRsp);
      RETVALUE(RFAILED);
   }


#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(gpRsp)
      {
         CZ_FREEEVNT(gpRsp);
      }
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(gpRsp == NULLP)
   {
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, 0, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, NULLP);
      CZ_RETVALUE(RFAILED);
   }
#endif

    /* Retrieve the X2AP Message type */
   msgType = czUtlGetMsgType(&gpRsp->pdu, &isResponse, &procType);
   if (msgType == CZT_MSG_UNKNOWN)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "failed Unknown Message MsgType(%d)Received\n",
         msgType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, gpRsp->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_UNEXP_MSG, gpRsp);
      CZ_RETVALUE(RFAILED);
   }
   /*cz001.301: Change for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120002, CZ_DIAG_PEERID, gpRsp->peerId,
               "Received GpRsp from Application, msgType  = %d",
                msgType, 0, 0, 0)
#endif
   /* Validate whether this is GPM Message or BMP Message */
   if (procType != CZ_GP)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, gpRsp->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_UNEXP_MSG, gpRsp);
      CZ_RETVALUE(RFAILED);
   }
#ifndef CZ_PERF
   if((isResponse == FALSE) || (msgType == CZT_MSG_ERR_IND))
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, gpRsp->peerId, CZT_ERRTYPE_INV_PARAM, 
                               CZT_ERRCAUSE_UNEXP_MSG, gpRsp);
      CZ_RETVALUE(RFAILED);
   }
#endif

   ret = czNmmFndPeer(gpRsp->peerId, &peer); 
   if(ret == RFAILED)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer(%ld) Could not be found\n", gpRsp->peerId));
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer(%d) Could not be found\n", gpRsp->peerId));
#endif
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb,gpRsp->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_INV_PEERID, gpRsp);
      CZ_RETVALUE(RFAILED);
   }
 
   ret = czGpmFsmMsgHandler (peer, gpRsp, msgType, CZ_DIR_OUT);  
   /* ErrInd will be sent by the respective Module */
   if (ret != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ROK);

} /* end of CzUiCztGpRsp */

/**
    @brief  CzUiCztBmpReq is called by User for invoking Mobilty Procedure Request

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of 
             the BMP.Also it checks for the Interface level Error Checking. This is 
             used for all Mobility Procedure for a specific UE.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] bmpReq This is the PDU for the Genral Procedure Message 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16 CzUiCztBmpReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztEvnt *bmpReq  /* connectionless sdus */
)
#else
PUBLIC S16 CzUiCztBmpReq(pst, spId, bmpReq)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztEvnt *bmpReq; /* connectionless sdus */
#endif
{
   S16         ret = RFAILED; /* return value */
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CzPeerCb    *peer = NULLP;
   U8          msgType;
   S16         procType;       /* Procedure Type*/    
   Bool          isResponse;


   CZ_TRC3(CzUiCztBmpReq);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Czt Event Address (%p)\n",
            (Void *)pst, spId, (Void *)bmpReq));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK)||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT163, (ErrVal)0, "failed, cannot derive czCb");
      if(bmpReq)
         CZ_FREEEVNT(bmpReq);
      RETVALUE(RFAILED);
   }


#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(bmpReq)
      {
         CZ_FREEEVNT(bmpReq);
      }
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bmpReq == NULLP)
   {
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, 0, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, NULLP);
      CZ_RETVALUE(RFAILED);
   }
#endif

   /* Retrieve the X2AP Message type */
   msgType = czUtlGetMsgType(&bmpReq->pdu, &isResponse, &procType);
   if (msgType == CZT_MSG_UNKNOWN)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "failed Unknown Message MsgType(%d)Received\n",
         msgType));

      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, bmpReq->peerId, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, bmpReq);
      CZ_RETVALUE(RFAILED);
   }
  /*cz001.301: Cgange for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120003, CZ_DIAG_PEERID, bmpReq->peerId,
               "Received BmpReq from Application, msgType  = %d",
                msgType, 0, 0, 0)
#endif
   if (procType != CZ_BMP)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, bmpReq->peerId, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, bmpReq);
      CZ_RETVALUE(RFAILED);

   }
#ifndef CZ_PERF
   if((isResponse == TRUE) || (msgType == CZT_MSG_ERR_IND))
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, bmpReq->peerId, CZT_ERRTYPE_INV_PARAM, 
                               CZT_ERRCAUSE_UNEXP_MSG, bmpReq);
      CZ_RETVALUE(RFAILED);
   }
#endif

   ret = czNmmFndPeer(bmpReq->peerId, &peer); 
   if(ret == RFAILED)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer Not Found PeerId(%ld) \n", bmpReq->peerId));
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer Not Found PeerId(%d) \n", bmpReq->peerId));
#endif
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb,bmpReq->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_INV_PEERID, bmpReq);
      CZ_RETVALUE(RFAILED);
   }
 
   ret =  czBmpHdlHoEvnt (peer, bmpReq, msgType, CZ_DIR_OUT );

   /* ErrInd will be sent by the respective Module */
   if (ret != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ROK);

} /* end of CzUiCztBmpReq */

/**
    @brief  CzUiCztBmpRsp is called by User for invoking Mobilty Procedure Request

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of 
             the BMP.Also it checks for the Interface level Error Checking.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] bmpRsp This is the PDU for the Genral Procedure Message 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16 CzUiCztBmpRsp 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztEvnt *bmpRsp  /* connectionless sdus */
)
#else
PUBLIC S16 CzUiCztBmpRsp(pst, spId, bmpRsp)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztEvnt *bmpRsp; /* connectionless sdus */
#endif
{
   S16         ret = RFAILED; /* return value */
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CzPeerCb    *peer = NULLP;
   U8          msgType;
   S16         procType;       /* Procedure Type*/    
   Bool          isResponse;

   CZ_TRC3(CzUiCztBmpRsp);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Czt Event Address (%p)\n",
            (Void *)pst, spId, (Void *)bmpRsp));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK)||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT164, (ErrVal)0, "failed, cannot derive czCb");
      if(bmpRsp)
      {
         CZ_FREEEVNT(bmpRsp);
      }
      RETVALUE(RFAILED);
   }

#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

 
 
   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(bmpRsp)
      {
         CZ_FREEEVNT(bmpRsp);
      }
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bmpRsp == NULLP)
   {
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, 0, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, NULLP);
      CZ_RETVALUE(RFAILED);
   }
#endif

   /* Retrieve the X2AP Message type */
   msgType = czUtlGetMsgType(&bmpRsp->pdu, &isResponse, &procType);
   if (msgType == CZT_MSG_UNKNOWN)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "failed Unknown Message MsgType(%d)Received\n",
         msgType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, bmpRsp->peerId, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, bmpRsp);
      CZ_RETVALUE(RFAILED);
   }
  /*cz001.301: Change for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120004, CZ_DIAG_PEERID, bmpRsp->peerId,
               "Received BmpRsp from Application, msgType  = %d",
                msgType, 0, 0, 0)
#endif
   if (procType != CZ_BMP)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, bmpRsp->peerId, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, bmpRsp);
      CZ_RETVALUE(RFAILED);
   }
#ifndef CZ_PERF
   if((isResponse == FALSE) || (msgType == CZT_MSG_ERR_IND))
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, bmpRsp->peerId, CZT_ERRTYPE_INV_PARAM, 
                               CZT_ERRCAUSE_UNEXP_MSG, bmpRsp);
      CZ_RETVALUE(RFAILED);
   }
#endif

   ret = czNmmFndPeer(bmpRsp->peerId, &peer); 
   if(ret == RFAILED)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer Could not be found peerId(%ld)\n",
         bmpRsp->peerId));
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF,
         "Peer Could not be found peerId(%d)\n",
         bmpRsp->peerId));
#endif
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb,bmpRsp->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_INV_PEERID, bmpRsp);
      CZ_RETVALUE(RFAILED);
   }
  
   ret =  czBmpHdlHoEvnt (peer, bmpRsp, msgType, CZ_DIR_OUT );

   /* ErrInd will be sent by the respective Module */
   if (ret != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ROK);

} /* end of CzUiCztBmpRsp */


  
/**
    @brief  CzUiCztGpErrReq is called by User for sending Error Indication Message

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of 
             the BMP.Also it checks for the Interface level Error Checking.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] errReq This is the PDU for the Error Indication Message 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/
#ifdef ANSI
PUBLIC S16 CzUiCztGpErrReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztEvnt *errReq  /* connectionless sdus */
)
#else
PUBLIC S16 CzUiCztGpErrReq(pst, spId, errReq)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztEvnt *errReq; /* connectionless sdus */
#endif
{
   S16         ret = RFAILED; /* return value */
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CzPeerCb    *peer = NULLP;
   U8          msgType;
   S16         procType;       /* Procedure Type*/    
   Bool          isResponse;

   CZ_TRC3(CzUiCztGpErrReq);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Czt Event Address (%p)\n",
            (Void *)pst, spId, (Void *)errReq));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK)||( czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT165, (ErrVal)0, "failed, cannot derive czCb");
      if(errReq) 
         CZ_FREEEVNT(errReq);
      RETVALUE(RFAILED);
   }


#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(errReq) 
      {
         CZ_FREEEVNT(errReq);
      }
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(errReq == NULLP)
   {
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, 0, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_UNEXP_MSG, NULLP);
      CZ_RETVALUE(RFAILED);
   }
#endif

 /* Retrieve the X2AP Message type */
   msgType = czUtlGetMsgType(&errReq->pdu, &isResponse, &procType);
   if (msgType != CZT_MSG_ERR_IND)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "failed Unknown Message MsgType(%d)Received\n",
         msgType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, errReq->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_UNEXP_MSG, errReq);
      CZ_RETVALUE(RFAILED);
   }
  /* cz001.301: Change for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120005, CZ_DIAG_PEERID, errReq->peerId,
               "Received GpErrReq from Application, msgType  = %d",
                msgType, 0, 0, 0)
#endif
   if(procType != CZ_ERR)
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "wrong primitive(%d)  Received\n",procType));
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, errReq->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_UNEXP_MSG, errReq);
      CZ_RETVALUE(RFAILED);
   }

   ret = czNmmFndPeer(errReq->peerId, &peer);
   if(ret == RFAILED)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Peer Could not be found Peer(%ld)\n",
         errReq->peerId));
#else
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Peer Could not be found Peer(%d)\n",
         errReq->peerId));
#endif
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb,errReq->peerId, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_INV_PEERID, errReq);
      CZ_RETVALUE(RFAILED);
   }
   ret = czNmmHdlErrInd(peer, errReq, CZ_DIR_OUT);
   /* ErrInd will be sent by the respective Module */
   if (ret != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ROK);

} /* end of CzUiCztGpErrReq */

/**
    @brief  CzUiCztGpRetxReq is called by User for retransmit the Request  

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of 
             the GPM.It retransmits the Buffered general Procedure which has not
             received the response.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] retxInfo peerId/mesurement ID combined into this 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/
  
#ifdef ANSI
PUBLIC S16 CzUiCztGpRetxReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztReTxInfo *retxInfo
)
#else
PUBLIC S16 CzUiCztGpRetxReq(pst, spId, retxInfo)
Pst   *pst;              /* Post structure */
SpId  spId;             /* service provider id */
CztReTxInfo *retxInfo;
#endif
{
   S16         ret = RFAILED; /* return value */
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CzPeerCb    *peer = NULLP;

   CZ_TRC3(CzUiCztGpRetxReq);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Retransmission info\
            Address (%p)\n", 
            (Void *)pst, spId, (Void *)retxInfo));

#ifdef SS_MULTIPLE_PROCS
   /*cz001.301: x2ap kwork fix*/
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT166, (ErrVal)0, "failed, cannot derive czCb");
      if(retxInfo)
         CZ_FREE(retxInfo, sizeof(CztReTxInfo));
      RETVALUE(RFAILED);
   }


#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

 
   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(retxInfo)
         CZ_FREE(retxInfo, sizeof(CztReTxInfo));
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(retxInfo == NULLP)
   {
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb, 0, CZT_ERRTYPE_INV_PARAM,
            CZT_ERRCAUSE_UNEXP_MSG, NULLP);
      CZ_RETVALUE(RFAILED);
   }
#endif
  /*cz001.301: Change for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120006, CZ_DIAG_PEERID, retxInfo->peerId,
               "Received GpRetxReq from Application, msgType  = %d",
                retxInfo->msgType, 0, 0, 0)
#endif
   ret = czNmmFndPeer(retxInfo->peerId, &peer); 
   if(ret == RFAILED)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Peer Could not be found Peer(%ld)\n",
         retxInfo->peerId));
#else
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Peer Could not be found Peer(%d)\n",
         retxInfo->peerId));
#endif
      /* Error Indication to the user */
      CZ_SEND_LCL_UI_ERR(uSapCb,retxInfo->peerId, CZT_ERRTYPE_INV_PARAM, 
            CZT_ERRCAUSE_INV_PEERID, NULLP);
      CZ_FREE(retxInfo, sizeof(CztReTxInfo));
      CZ_RETVALUE(RFAILED);
   }
   /* pass  the MeasurementID  */
   ret = czGpmRetransmit(peer, retxInfo->msgType, retxInfo->u.eNb1MeasId);
 
   CZ_FREE(retxInfo, sizeof(CztReTxInfo));

   if(ret != ROK)
   {
      CZ_RETVALUE(RFAILED);
   }
   CZ_RETVALUE(ROK);

} /* end of CzUiCztGpRetxReq */

/**
    @brief   CzUiCztRelReq  is called by User for releasing the Resource Locally 

    @details This function searches the Peer Control Block and Layer Control Block
             in case of the Multi Proc and invokes the Entry point function of NMM 
             to release the Resources Locally.
              

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] relReq The release Request Structure. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
  
#ifdef ANSI
PUBLIC S16 CzUiCztRelReq 
(
Pst         *pst,              /* Post structure */
SpId        spId,              /* service provider id */
CztRlsReq   *relReq            /* The release Structure */ 
)
#else
PUBLIC S16 CzUiCztRelReq(pst, spId, relReq)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztRlsReq   *relReq;   /* The release Structure */ 
#endif
{
   S16         ret = RFAILED; /* return value */
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CzPeerCb    *peerCb = NULLP;
   CztRlsCfm    *rlsCfm;
   U32         peerId;   
   U32         ueCbKey = 0;
   CzUeCb      *ueCb = NULLP;
   CzRsrcCb    *rsrcCb = NULLP;

   CZ_TRC3(CzUiCztRelReq);
   rlsCfm = NULLP;

   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Release Structure \
            Address (%p)\n", (Void *)pst, spId, (Void *)relReq));

#ifdef SS_MULTIPLE_PROCS
    /* cz001.301: x2ap warning fix*/
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT167, (ErrVal)0, "failed, cannot derive czCb");
      
      if(relReq)
      {
         CZ_FREE(relReq, sizeof(CztRlsReq));
      }
      RETVALUE(RFAILED);
   }

#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));
  /* cz001.301:Change for Diagnostics*/
#ifdef SS_DIAG
  CZ_DIAG_LVL0(0x04120007, CZ_DIAG_PEERID, relReq->peerId,
            "Received RelReq from Application, Release type = %d, ID type = %d",
                relReq->rlsType, relReq->idType, 0, 0);
#endif
   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(relReq)
         CZ_FREE(relReq, sizeof(CztRlsReq));
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(relReq == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
               "relReq received is null\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   peerId = relReq->peerId;
   CZ_ALLOC(rlsCfm, sizeof(CztRlsCfm));
   if(rlsCfm == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Memory Allocation Failed \n"));
      CZ_RETVALUE(RFAILED);
   }
   rlsCfm->peerId = peerId;
   
   ret = czNmmFndPeer(peerId, &peerCb); 
   if(ret == RFAILED)
   {
#ifndef ALIGN_64BIT      
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Peer Could not be found PeerId(%ld)\n",peerId));
#else
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Peer Could not be found PeerId(%d)\n",peerId));
#endif
      rlsCfm->status = CZT_PRIM_NOK;
      rlsCfm->reason = CZT_REASON_INV_PEERID; 
      rlsCfm->type   = relReq->rlsType;
      CZ_FREE(relReq, sizeof(CztRlsReq));
      (Void) CzUiCztRelCfm (&uSapCb->pst, uSapCb->suId, rlsCfm);
      CZ_RETVALUE(RFAILED);
   }
   CZ_PRNT_PEERID;

   /* If the message Type is UE /RSRC and node type is neither Local nor Remote
    * then it is an error */
   if( ((relReq->rlsType == CZT_RELTYPE_UE ) || 
       (relReq->rlsType == CZT_RELTYPE_RSRC)) &&
       (relReq->idType != CZT_IDTYPE_LOCAL) && 
       (relReq->idType != CZT_IDTYPE_REMOTE))
   {
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
         "Invalid ID Node Type(%d)\n",relReq->idType));
      rlsCfm->status = CZT_PRIM_NOK;
      rlsCfm->reason = CZT_REASON_INV_IDTYPE; 
      rlsCfm->type   = relReq->rlsType;
      CZ_FREE(relReq, sizeof(CztRlsReq));
      (Void) CzUiCztRelCfm (&uSapCb->pst, uSapCb->suId, rlsCfm);
      CZ_RETVALUE(RFAILED);
   }

   switch(relReq->rlsType)
   {
      case CZT_RELTYPE_TERM:
         ret = czNmmDisablePeer(peerCb, TRUE);
         if(ret != ROK)
         {
            rlsCfm->reason = CZT_REASON_FAIL_ASSOCTERM;
         }
         break;

      case CZT_RELTYPE_RESET:
         ret = czNmmDisablePeer(peerCb, FALSE);
         if(ret != ROK)
         {
            rlsCfm->reason = CZT_REASON_FAIL_ASSOCTERM;
         }
         break;

      case CZT_RELTYPE_UE:
         CZ_UIM_GET_KEY(relReq->u.oldUEX2APId, relReq->idType, ueCbKey);
         ret = czNmmFindUeCb(peerCb, ueCbKey, &ueCb);
         if(ret == ROK)
         {
            czNmmDelUeCb(ueCb);
            ueCb = NULLP;
         }
         else
         {
            rlsCfm->reason = CZT_REASON_INV_UEID;
         }
         rlsCfm->oldUEX2APId = relReq->u.oldUEX2APId;
         break;

      case CZT_RELTYPE_RSRC:
         if(relReq->idType == CZT_IDTYPE_LOCAL)
         {
            CZ_UIM_GET_RSRC_KEY(ueCbKey, relReq->u.eNb1MeasId, CZ_ENB1);
         }
         else 
         {
            CZ_UIM_GET_RSRC_KEY(ueCbKey, relReq->u.eNb1MeasId, CZ_ENB2);
         }

         ret = czNmmFindRsrcCb(peerCb, ueCbKey , &rsrcCb);
         if(ret == ROK)
         {
            czNmmDelRsrcCb(rsrcCb);
            rsrcCb = NULLP;
         }
         else
         {
            rlsCfm->reason = CZT_REASON_INV_RSRCID;
         }
         break;

      default:
         CZ_DBG_ERROR( (CZ_PRNT_BUF,
                  "Invalid RELTYPE(%d)\n",relReq->rlsType));
         rlsCfm->reason = CZT_REASON_INV_RELTYPE;
         rlsCfm->status = CZT_PRIM_NOK;
         rlsCfm->type   = relReq->rlsType;
         CZ_FREE(relReq, sizeof(CztRlsReq));
         (Void) CzUiCztRelCfm (&uSapCb->pst, uSapCb->suId, rlsCfm);
         CZ_RETVALUE(RFAILED);
   }

   if(ret != ROK)
   {
      rlsCfm->type = relReq->rlsType;
      rlsCfm->status = CZT_PRIM_NOK;
      CZ_FREE(relReq, sizeof(CztRlsReq));
      (Void) CzUiCztRelCfm (&uSapCb->pst, uSapCb->suId, rlsCfm);
      CZ_RETVALUE(RFAILED);
   }
   /* send an acknowledgement back to the user */
   rlsCfm->status = CZT_PRIM_OK;
   rlsCfm->type = relReq->rlsType;
   rlsCfm->reason = CZT_REASON_NOT_APPL;
   CZ_FREE(relReq, sizeof(CztRlsReq));

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Release Request successful \n"));

   (Void) CzUiCztRelCfm (&uSapCb->pst, uSapCb->suId, rlsCfm);
   CZ_RETVALUE(ROK);
} /* end of CzUiCztLocalRelReq */

/* cz001.101 : Add Audit Feature. */
/**
    @brief   CzUiCztAudReq  is called by User for Audit Purpose

    @details This function Audit the following:
             - State of Peer
             - SCTP Association with Peer

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] audEvnt Audit Event Structure. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/
#ifdef ANSI
PUBLIC S16 CzUiCztAudReq 
(
Pst         *pst,              /* Post structure */
SpId        spId,              /* service provider id */
CztAudEvnt *audEvnt    /* Audit Event */
)
#else
PUBLIC S16 CzUiCztAudReq(pst, spId, audEvnt)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztAudEvnt *audEvnt;   /* Audit Event */
#endif
{
   CzCztSapCb  *uSapCb=NULLP;       /* Upper Sap */
   CztAudEvnt  *audCfm;
   /*kworks fix: changed U16 to U8*/
   U8         cause;

   CZ_TRC3(CzUiCztAudReq);

   audCfm = NULLP;

   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Czt Event Address (%p)\n",
            (Void *)pst, spId, (Void *)audEvnt));

#ifdef SS_MULTIPLE_PROCS
   /* cz001.301: x2ap KWORK warning fix*/
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT168, (ErrVal)0, "failed, cannot derive czCb");
      if(audEvnt)
      {
         CZ_FREE(audEvnt, sizeof(CztAudEvnt));
      }
      RETVALUE(RFAILED);
   }

#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(audEvnt)
         CZ_FREE(audEvnt, sizeof(CztAudEvnt));
      CZ_RETVALUE(RFAILED);
   }

   if(audEvnt == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
               "audEvnt received is null\n"));
      CZ_RETVALUE(RFAILED);
   }

   CZ_ALLOC(audCfm, sizeof(CztAudEvnt));
   if(audCfm == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Memory Allocation Failed \n"));
      RETVALUE(RFAILED);
   }
   cmMemcpy ((U8 *)audCfm, (U8 *)audEvnt, sizeof (CztAudEvnt));
   
   switch(audEvnt->type)
   {
      case CZT_AUD_PEER:
         {
            if (czUiDoAudit(audCfm, uSapCb, &cause) != ROK)
            {
               audCfm->status.status = CZT_PRIM_NOK;
               audCfm->status.cause  = cause;

               if(audEvnt)
                  CZ_FREE(audEvnt, sizeof(CztAudEvnt));

               CzUiCztAudCfm(&uSapCb->pst, uSapCb->suId, audCfm);

               CZ_RETVALUE(RFAILED);
            }
         }
         break;

      case CZT_AUD_FLC:
         {
            CzPeerCb    *peerCb;
            U32      peerId;

            peerId = audCfm->u.audPeer.peerId;

            if(czNmmFndPeer(peerId,&peerCb) != ROK)
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF, "Peer not found. \n"));

               audCfm->status.status = CZT_PRIM_NOK;
               audCfm->status.cause = CZT_ERRCAUSE_INV_PEERID;

               if(audEvnt)
                  CZ_FREE(audEvnt, sizeof(CztAudEvnt));

               CzUiCztAudCfm(&uSapCb->pst, uSapCb->suId, audCfm);

               CZ_RETVALUE(RFAILED);
            }
            CZ_PRNT_PEERID;

            if(peerCb->sctSapCb == NULLP)
            {
               CZ_PRNT_PEERID;
               CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Cb is NULLP. \n"));
               audCfm->status.status = CZT_PRIM_NOK;
               audCfm->status.cause = CZT_ERRCAUSE_SAP_NOTPRST;

               if(audEvnt)
                  CZ_FREE(audEvnt, sizeof(CztAudEvnt));

               CzUiCztAudCfm(&uSapCb->pst, uSapCb->suId, audCfm);

               CZ_RETVALUE(RFAILED);
            }

            if(peerCb->assocCb == NULLP || 
                  peerCb->assocCb->assocState != CZ_ASSOC_UP)
            {
               CZ_PRNT_PEERID;
               CZ_DBG_ERROR((CZ_PRNT_BUF, "Association is not present. \n"));
               audCfm->status.status = CZT_PRIM_NOK;
               audCfm->status.cause = CZT_ERRCAUSE_ASSOC_NOTPRST;

               if(audEvnt)
                  CZ_FREE(audEvnt, sizeof(CztAudEvnt));

               CzUiCztAudCfm(&uSapCb->pst, uSapCb->suId, audCfm);

               CZ_RETVALUE(RFAILED);
            }
            CZ_PRNT_PEERID;
            CZ_DBG_INFO((CZ_PRNT_BUF,
                     "Sending Status Request for the Association for Audit\
                     function in spId(%d))\n",
                     spId));

            /* Send a status request to the sctp layer */
            CzLiSctStaReq(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                  peerCb->assocCb->spAssocId, 
                  &peerCb->assocCb->assocCfg->priDstAddr, 
                  SCT_GET_FLC_INFO);
         }
         break;

      default:
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid Audit Type. \n"));
            audCfm->status.status = CZT_PRIM_NOK;
            audCfm->status.cause = CZT_STACAUSE_INV_TYPE;

            if(audEvnt)
               CZ_FREE(audEvnt, sizeof(CztAudEvnt));

            CzUiCztAudCfm(&uSapCb->pst, uSapCb->suId, audCfm);

            CZ_RETVALUE(RFAILED);
         }
         break;
   }/* switch(audEvnt->type) */

   audCfm->status.status = CZT_PRIM_OK;
   audCfm->status.cause = CZT_STACAUSE_NOT_APPL;

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Audit Request completed successfully. spId(%d))\n",
             spId));
   CzUiCztAudCfm(&uSapCb->pst, uSapCb->suId, audCfm);

   CZ_FREE(audEvnt, sizeof(CztAudEvnt));

   CZ_RETVALUE(ROK);
} /* end of CzUiCztAudReq */


/**
    @brief   Sends the response to AssocInd received from Peer 

    @details This function confirms that the Association is accepted from that particular Peer.

    @param[in] pst This is the Pst of the message.
    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[in] peerAssocRsp The peerAssocRsp reperesnts the Association related response structure.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
  
#ifdef ANSI
PUBLIC S16 CzUiCztDynamPeerAssocRsp
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
CztPeerAssocRsp   *peerAssocRsp   /*Peer Association Response */ 
)
#else
PUBLIC S16 CzUiCztDynamPeerAssocRsp(pst, spId, peerAssocRsp) 
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
CztPeerAssocRsp   *peerAssocRsp;   /*Peer Association Response */ 
#endif
{
   S16                 ret = RFAILED;          /* return value */
   U16                 idx;          /* return value */
   CzCztSapCb         *uSapCb=NULLP;       /* Upper Sap */
   CzDynamPeerAssocCb *peerAssocCb = NULLP;
   CzPeerCb           *peerCb = NULLP;
   SctAssocIndParams   assocParams;
   CztNetAddrLst       sctAddrLst;
   CztStaInd           sta;
   U8                  reason;

   CZ_TRC3(CzUiCztDynamPeerAssocRsp);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), spId (%d), Peer Assoc Response \
            Address (%p)\n", (Void *)pst, spId, (Void *)peerAssocRsp));

#ifdef SS_MULTIPLE_PROCS
   /* cz001.301: KWORK warning fix*/
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                !=ROK))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT169, (ErrVal)0, "failed, cannot derive czCb");
      if(peerAssocRsp)
      {
         CZ_FREE(peerAssocRsp, sizeof(CztPeerAssocRsp));
      }
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------X2AP-----Pst(proc(%d),ent(%d),inst(%d)), spId(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst, spId));

   CZ_SET_ZERO((U8*)&sta, sizeof(CztStaInd));

   if(czUimGetAndChkCztSap(spId, &uSapCb) != ROK)
   {
      if(peerAssocRsp)
      {
         CZ_FREE(peerAssocRsp, sizeof(CztPeerAssocRsp));
      }
      CZ_RETVALUE(RFAILED);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(peerAssocRsp == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
            "peerAssocRsp received is null\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif
   
   /* search for the peerAssoc control block */
   sctAddrLst.nmb = peerAssocRsp->dstAddrLst.nmb;
   for (idx = 0; idx < peerAssocRsp->dstAddrLst.nmb; idx++)
      sctAddrLst.nAddr[idx] = peerAssocRsp->dstAddrLst.nAddr[idx];

   ret = czNmmFindDynamPeerAssocCb(&sctAddrLst, peerAssocRsp->dstPort, 
         &peerAssocCb);
   if (ret != ROK)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "failed to find the Assoc\
               control block for dyamic peer peerId(%ld)\n",
               peerAssocRsp->peerId));
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF, "failed to find the Assoc\
               control block for dyamic peer peerId(%d)\n",
               peerAssocRsp->peerId));
#endif

      if (peerAssocRsp->status.status == CZT_PRIM_OK)
      {
         /* Populate the sta indication structure */
         sta.peerId  = peerAssocRsp->peerId;
         sta.type        = CZT_STA_TYPE_PEER;
         sta.status      = CZT_STATUS_DYNAM_PEERCFG_NOK;
         sta.reason      = CZT_REASON_ASSOC_NOTFOUND;

         /* Send status indication to user */
         (Void) czUimSndStaInd(uSapCb, &sta);
      }

      CZ_FREE(peerAssocRsp, sizeof(CztPeerAssocRsp));
      CZ_RETVALUE(RFAILED);
   }

   peerCb      = peerAssocCb->peerCbPtr;
   assocParams = peerAssocCb->assocParams;

   CZ_PRNT_PEERID;
   if (peerAssocRsp->status.status != CZT_PRIM_OK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
               "user responded not to configure the\
               dynamic peer\n"));
#ifdef SCT3
      CzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                      peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams, 0, 
                      SCT_NOK, NULLP);
#else
      CzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                      peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams, 
                      SCT_NOK, NULLP);
#endif /* end of SCT3 */

      czNmmRemDynamPeerAssoc(peerCb);
      czNmmDeallocDynamPeerCb(peerCb);

      CZ_FREE(peerAssocRsp, sizeof(CztPeerAssocRsp));
      CZ_RETVALUE(ROK);
   }
   do
   {
      ret = czUimValidatePeerAssocRsp(peerAssocCb, peerAssocRsp, &reason);
      if(ret != ROK)
      {
         break;
      }
      /* fill the remaining fields of peer and assoc cb */
      peerCb->peerId                         = peerAssocRsp->peerId;
      peerCb->peerCfg.globalStreamId         = peerAssocRsp->globalStreamId;
      peerCb->assocCb->suAssocId             = peerAssocRsp->peerId;
      peerCb->assocCb->assocCfg->priDstAddr  = peerAssocRsp->priDstAddr;
      peerCb->assocCb->assocCfg->locOutStrms = peerAssocRsp->locOutStrms;
      assocParams.t.cookieParams.suAssocId   = peerCb->assocCb->suAssocId;
      /* updating the SAP CB from the response  */
      peerCb->cztSapCb = uSapCb;
      ret = cmHashListInsert (&czCb.nodeCb.peerCbLst, (PTR)peerCb,
            (U8 *)&peerCb->peerId, sizeof (U32));
      if(ret != ROK)
      {
         reason = CZT_REASON_HASH_INS_FAIL;
         break;
      }
      /* remove the transient peerAssoc control blocks */
      czNmmRemDynamPeerAssoc(peerCb);

      /* Send an alarm indicating dynamic peer is configured */
      CZ_SEND_PEER_ALARM(peerCb->peerId, NULLP, LCZ_EVENT_UI_PEERASSOC_RSP, 
            LCZ_CAUSE_DYNAM_PEERCFG_OK);

      /* Send the association response */
#ifdef SCT3
      CzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
            peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams, 0, SCT_OK,
            NULLP);
#else
      CzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
            peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams, SCT_OK, NULLP);
#endif /* end of SCT3 */

      /* Populate the sta indication structure */
      sta.peerId  = peerCb->peerId;
      sta.type        = CZT_STA_TYPE_PEER;
      sta.status      = CZT_STATUS_DYNAM_PEERCFG_OK;
      sta.reason      = CZT_REASON_NOT_APPL;

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Assoc Response sent successfully.\n"));
      /* Send status indication to user */
      (Void) czUimSndStaInd(uSapCb, &sta);

      CZ_FREE(peerAssocRsp, sizeof(CztPeerAssocRsp));
      CZ_RETVALUE(ROK);
   }while (0);

   /* In case of ret ! = ROK will reach Here */
#ifdef SCT3
   CzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
         peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams, 0, SCT_NOK, NULLP);
#else
   CzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
         peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams, SCT_NOK, NULLP);
#endif /* end of SCT3 */

   czNmmRemDynamPeerAssoc(peerCb);
   czNmmDeallocDynamPeerCb(peerCb);

   /* Populate the sta indication structure */
   sta.peerId  = peerAssocRsp->peerId;
   sta.type        = CZT_STA_TYPE_PEER;
   sta.status      = CZT_STATUS_DYNAM_PEERCFG_NOK;
   sta.reason      = reason;

   /* Send status indication to user */
   (Void) czUimSndStaInd(uSapCb, &sta);

   CZ_FREE(peerAssocRsp, sizeof(CztPeerAssocRsp));
   CZ_RETVALUE(RFAILED);
} /* end of CzUiCztPeerAssocRsp */

/**
    @brief   czUimSndBmpInd is called for sending the Event to the User 

    @details This function checks SAP state and sends the Bmp Indication Event to 
             the Upper Interface. 

    @param[in] peerCb The spId reperesnts the Provider identifier for this SAP.
    @param[in] bmpInd The bmpInd event that is being sent to the User. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16  czUimSndBmpInd
(
 CzPeerCb *peerCb,
 CztEvnt *bmpInd
)
#else
PUBLIC S16 czUimSndBmpInd(peerCb, bmpInd)
 CzPeerCb *peerCb;
 CztEvnt *bmpInd;
#endif
{
   CzCztSapCb  *sapCb;

   CZ_TRC3(czUimSndBmpInd);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Cb address(%p), Czt Event Address (%p)\n", 
            (Void *)peerCb, (Void *)bmpInd));
   CZ_PRNT_PEERID;

   sapCb = peerCb->cztSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d)\n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }


   CZ_DBG_INFO((CZ_PRNT_BUF,
            "BMP Indication to the user.\n"));
   CzUiCztBmpInd(&sapCb->pst, sapCb->suId, bmpInd);

   CZ_RETVALUE(ROK);

} /*czUimSndBmpInd  */

/**
    @brief   czUimSndBmpCfm is called for sending the Event to the User 

    @details This function checks SAP state and sends the Bmp Indication Event to 
             the Upper Interface. 

    @param[in] peerCb The spId reperesnts the Provider identifier for this SAP.
    @param[in] bmpCfm The bmpCfm event that is being sent to the User. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16  czUimSndBmpCfm
(
 CzPeerCb *peerCb,
 CztEvnt *bmpCfm
)
#else
PUBLIC S16 czUimSndBmpCfm(peerCb, bmpCfm)
 CzPeerCb *peerCb;
 CztEvnt *bmpCfm;
#endif
{
   CzCztSapCb  *sapCb;

   CZ_TRC3(czUimSndBmpCfm);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Cb address(%p), Czt Event Address (%p)\n", 
            (Void *)peerCb, (Void *)bmpCfm));

   sapCb = peerCb->cztSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d) \n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "BMP Confirmation to the user.\n"));
   CzUiCztBmpCfm(&sapCb->pst, sapCb->suId, bmpCfm);

   CZ_RETVALUE(ROK);

} /*czUimSndBmpCfm  */



/**
    @brief   czUimSndGpmInd is called for sending the Event to the User 

    @details This function checks SAP state and sends the Bmp Indication Event to 
             the Upper Interface. 

    @param[in] peerCb The spId reperesnts the Provider identifier for this SAP.
    @param[in] gpmInd The gmpInd event that is being sent to the User. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16  czUimSndGpmInd
(
 CzPeerCb *peerCb,
 CztEvnt *gpmInd
)
#else
PUBLIC S16 czUimSndGpmInd(peerCb, gpmInd)
 CzPeerCb *peerCb;
 CztEvnt *gpmInd;
#endif
{
   CzCztSapCb  *sapCb;

   CZ_TRC3(czUimSndGpmInd);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Cb address(%p), Czt Event Address (%p)\n", 
            (Void *)peerCb, (Void *)gpmInd));

   sapCb = peerCb->cztSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d)\n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }


   CZ_DBG_INFO((CZ_PRNT_BUF,
            "GPM Indication to the user.\n"));
   CzUiCztGpInd(&sapCb->pst, sapCb->suId, gpmInd);

   CZ_RETVALUE(ROK);
} /*czUimSndGpmInd  */


/**
    @brief   czUimSndGpmCfm is called for sending the Event to the User 

    @details This function checks SAP state and sends the Bmp Indication Event to 
             the Upper Interface. 

    @param[in] peerCb The spId reperesnts the Provider identifier for this SAP.
    @param[in] gpmCfm The gpmCfm event that is being sent to the User. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PUBLIC S16  czUimSndGpmCfm
(
 CzPeerCb *peerCb,
 CztEvnt *gpmCfm
)
#else
PUBLIC S16 czUimSndGpmCfm(peerCb, gpmCfm)
 CzPeerCb *peerCb;
 CztEvnt *gpmCfm;
#endif
{
   CzCztSapCb  *sapCb;

   CZ_TRC3(czUimSndGpmCfm);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Cb address(%p), Czt Event Address (%p)\n", 
            (Void *)peerCb, (Void *)gpmCfm));

   sapCb = peerCb->cztSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d)\n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "GPM confirmation to the user.\n"));
   CzUiCztGpCfm(&sapCb->pst, sapCb->suId, gpmCfm);

   CZ_RETVALUE(ROK);
} /*czUimSndGpmCfm  */

/**
    @brief   czUimSndGpErrInd is called for Sending Error PDU to User

    @details This function checks SAP state and sends the protocol Error 
              Indication Event to the Upper Interface   

    @param[in] peerCb peerCb is Peer Control Block.
    @param[in] gpErrInd gpErrInd Event passed by the Layer. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

#ifdef ANSI
PUBLIC S16 czUimSndGpErrInd 
(
 CzPeerCb *peerCb,
 CztEvnt *gpErrInd
)
#else
PUBLIC S16 czUimSndGpErrInd(peerCb, gpErrInd)
 CzPeerCb *peerCb;
 CztEvnt *gpErrInd;
#endif
{
   CzCztSapCb  *sapCb;

   CZ_TRC3(czUimSndGpErrInd);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Cb address(%p), Czt Event Address (%p)\n", 
            (Void *)peerCb, (Void *)gpErrInd));

   sapCb = peerCb->cztSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d)\n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }
  
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "GP Error Indication to the user.\n"));
   CzUiCztGpErrInd(&sapCb->pst, sapCb->suId, gpErrInd );

   CZ_RETVALUE(ROK);
} /* czUimSndGpErrInd */


/**
    @brief   czUimSndPeerAssocInd is called for indicating the User About Unknown Peer 

    @details This function will notify the User about the Association Indication
            if the Peer is not among the configured one. 

    @param[in] peerCb peerCb is peer Control Block.
    @param[in] peerAssocInd peerAssocInd  is the information recived pertaining to 
               the Association.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

#ifdef ANSI
PUBLIC S16 czUimSndPeerAssocInd 
(
CzPeerCb        *peerCb,
CztPeerAssocInd *peerAssocInd
)
#else
PUBLIC S16 czUimSndPeerAssocInd(peerCb, peerAssocInd)
CzPeerCb        *peerCb;
CztPeerAssocInd *peerAssocInd;
#endif
{
   CzCztSapCb *sapCb; /* CZT SAP Control Block pointer */

   CZ_TRC3(czUimSndPeerAssocInd);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Cb address(%p), Peer Assoc Ind Address (%p)\n", 
            (Void *)peerCb, (Void *)peerAssocInd));

   sapCb = peerCb->cztSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound :\
               spId(%d) \n",sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }
  
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Dynamic Peer Assoc Indication to the user.\n"));
   CzUiCztDynamPeerAssocInd(&sapCb->pst, sapCb->suId, peerAssocInd);

   CZ_RETVALUE(ROK);
} /* czUimSndPeerAssocInd */

/**
    @brief   czUimSndErrInd is called for indicating the User Error happeened in the Layer

    @details  This Function will notify the User any abnormal behaviour in the Layer 

    @param[in] sapCb sapCb is SAP Control Block.
    @param[in] errInd errInd Structure passed by the Layer. 
    @param[in] event  CztEvnt PDU 
    @param[in] mBuf Buffer to be sent to the User 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

#ifdef ANSI
PUBLIC S16 czUimSndErrInd 
(
CzCztSapCb     *sapCb,
CztLclErrInd   *errInd,
CztEvnt        *event,
Buffer         *mBuf
)
#else
PUBLIC S16 czUimSndErrInd(sapCb, errInd, event, mBuf)
CzCztSapCb     *sapCb;
CztLclErrInd   *errInd;
CztEvnt        *event;
Buffer         *mBuf;
#endif
{
   CztLclErrInd   *errEvent = NULLP;

   CZ_TRC3(czUimSndErrInd);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "SAP Cb address(%p), Error Indication \
            Address (%p), Czt Event Address (%p), Buffer address (%p)\n", 
            (Void *)sapCb, (Void *)errInd, (Void *)event, (Void *)mBuf));

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, 
            LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d) \n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }

   CZ_ALLOC(errEvent , sizeof(CztLclErrInd));

   if(errEvent == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Allocation for "
               "errEvent Failed \n"));
      CZ_RETVALUE(RFAILED);
   }

   /* Set the Parameters */
   errEvent->peerId = errInd->peerId;
   errEvent->causeType = errInd->causeType;
   errEvent->causeValue = errInd->causeValue;
   errEvent->msgType = errInd->msgType;
   /* ccpu00132838 */
   errEvent->u.ueInfo.oldUEX2APId = errInd->u.ueInfo.oldUEX2APId;
   if(errInd->u.ueInfo.newUEX2APId != CZ_UNKNOWN_UE_X2AP_ID)
   {
      errEvent->u.ueInfo.newUEX2APId = errInd->u.ueInfo.newUEX2APId;
   }

   errEvent->evntPres = FALSE;

   /* Allocate the Events and the Buffer */
   if(event != NULLP)
   {  
      errEvent->evntPres=TRUE;
      errEvent->event = event;
   }

   if(mBuf != NULLP)
   {
      SCpyMsgMsg(mBuf ,sapCb->pst.region,sapCb->pst.pool, &errEvent->rcvBuf);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Local Error Indication to the user.\n"));
   CzUiCztLclErrorInd(&sapCb->pst, sapCb->suId, errEvent);

   CZ_RETVALUE(ROK);
} /* czUimSndErrInd */


/**
    @brief   czUimSndStaInd is called for Sending Status Indication to User. 

    @details  This Function will be called by different Modules to send Status 
               Indication to the User in different scenario.

    @param[in] sapCb sapCb is SAP Control Block.
    @param[in] staInd StaInd Event to be sent to the User 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

#ifdef ANSI
PUBLIC S16  czUimSndStaInd
(
CzCztSapCb  *sapCb,
CztStaInd   *staInd
)
#else
PUBLIC S16 czUimSndStaInd(sapCb, staInd)
CzCztSapCb  *sapCb;
CztStaInd   *staInd;
#endif
{
   CztStaInd   *staIndEvent = NULLP;

   CZ_TRC3(czUimSndStaInd);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "SAP Cb address(%p), Status Indication (%p)\n",
            (Void *)sapCb, (Void *)staInd));


   if(sapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not found \n"));
      CZ_RETVALUE(RFAILED);
   }

   if (sapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(sapCb->spId, LCZ_EVENT_UI_MSG_PEER, LCZ_CAUSE_NOT_BIND);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP Not bound : spId(%d) \n",
               sapCb->spId));
      CZ_RETVALUE(RFAILED);
   }

   CZ_ALLOC(staIndEvent , sizeof(CztStaInd));

   if(staIndEvent == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Memory Allocation Failed \n"));
      CZ_RETVALUE(RFAILED);
   }

   CZ_MEM_COPY((U8*)staIndEvent, (U8*)staInd, sizeof(CztStaInd));

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Status Indication to the user.\n"));
   CzUiCztStaInd(&sapCb->pst, sapCb->suId, staIndEvent);

   CZ_RETVALUE(ROK);
} /* czUimSndStaInd */


/**
    @brief   czUimGetAndChkCztSap  is called for checking the SAP State 

    @details This function checks SAP state and returns 
             sapCb if present. This function is called by the Upper Interface 
             Interface fucntions to get the state of the SAP CB

    @param[in] spId The spId reperesnts the Provider identifier for this SAP.
    @param[out] uSapCb The SAP Cb pointer that returns the SAP Control Block. 

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/

  
#ifdef ANSI
PRIVATE S16 czUimGetAndChkCztSap 
(
SpId       spId,
CzCztSapCb **uSapCb
)
#else
PRIVATE S16 czUimGetAndChkCztSap(spId, uSapCb)
SpId       spId;
CzCztSapCb **uSapCb;
#endif
{
   CzUstaDgn dgn; 

   CZ_TRC2(czUimGetAndChkCztSap);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "spId (%d)  USap Cb address(%p) \n", 
            spId, (Void *)uSapCb));


   *uSapCb = NULLP; 

   if ((spId < czCb.genCfg.nmbCztSaps) && (spId >= 0)) 
   { 
      *uSapCb = czCb.cztSapLst[spId]; 

      if ((*uSapCb != NULLP) && 
         ((*uSapCb)->state == LCZ_SAP_ISBOUND)) 
      { 
         CZ_DBG_INFO((CZ_PRNT_BUF,
                  "SAP ok spId(%d))\n",spId));
         CZ_RETVALUE(ROK); 
      } 
      else
      { 
         dgn.alarmType = LCZ_USTA_DGNVAL_SAPID; 
         dgn.u.sapId = spId; 
         czLmmSendLmAlarm (LCM_CATEGORY_INTERFACE, LCZ_EVENT_UI_INV_EVT, 
                        LCZ_CAUSE_INVALID_SAP, &dgn); 

         CZ_DBG_ERROR( (CZ_PRNT_BUF,
                  "SAP Not Found\n"));
         CZ_RETVALUE(RFAILED); 
      } 
   } 
   else 
   { 
      CZ_DBG_ERROR( (CZ_PRNT_BUF,
               "Invalid SAP Id\n"));
      dgn.alarmType = LCZ_USTA_DGNVAL_SAPID; 
      dgn.u.sapId = spId; 
      czLmmSendLmAlarm (LCM_CATEGORY_INTERFACE, LCZ_EVENT_UI_INV_EVT, 
                     LCZ_CAUSE_INV_SPID, &dgn); 

      CZ_RETVALUE (RFAILED); 
   } 

} /* czUimGetAndChkCztSap */

/**
    @brief  czUimValidatePeerAssocRsp   is called for validating the assocRsp 

    @details  This function validates the response received from the user
              for the configuration of dynamic peer.
 
    @param[in] peerAssocCb The peerAssocCb reperesnts the AssocCB that is received 
               when the Association is created.
    @param[in] peerAssocRsp The information that is passed from the User 
    @param[out] reason  The reson to be filled in this function.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/


  
#ifdef ANSI
PRIVATE S16 czUimValidatePeerAssocRsp 
(
CzDynamPeerAssocCb *peerAssocCb,
CztPeerAssocRsp    *peerAssocRsp,
U8                 *reason
)
#else
PRIVATE S16 czUimValidatePeerAssocRsp(peerAssocCb, peerAssocRsp, reason)
CzDynamPeerAssocCb *peerAssocCb;
CztPeerAssocRsp    *peerAssocRsp;
U8                 *reason;
#endif
{
   S16           ret; 
   CzPeerCb     *peer = NULLP;
   CztStrmId     locOutStrms;

   CZ_TRC3(czUimValidatePeerAssocRsp);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Peer Assoc Cb address(%p), Peer Assoc Response \
            Address(%p), Reason (%s)\n", (Void *)peerAssocCb,
            (Void *)peerAssocRsp, reason));

   ret = czNmmFndPeer(peerAssocRsp->peerId, &peer);
   if (peer != NULLP)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "peer already"
               "exists with received peerId(%ld)\n",peerAssocRsp->peerId)); 
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF, "peer already exists with received peerId(%d)\n",
               peerAssocRsp->peerId)); 
#endif
      *reason = CZT_REASON_DUP_PEERID;
      CZ_RETVALUE(RFAILED);
   } 

   if (peerAssocCb->assocParams.type == SCT_ASSOC_IND_COOKIE)
   {
      locOutStrms = peerAssocRsp->locOutStrms;
   }
   else
   {
      locOutStrms = peerAssocCb->peerCbPtr->assocCb->assocCfg->locOutStrms;
   }

   if  (peerAssocRsp->globalStreamId > (locOutStrms-1) )
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "stream id(%d)"
               "recieved is invalid  peerId(%d)\n",
               peerAssocRsp->globalStreamId, peerAssocRsp->peerId)); 
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF, "stream id(%d)"
               "recieved is invalid  peerId(%ld)\n",
               peerAssocRsp->globalStreamId, peerAssocRsp->peerId)); 
#endif
       
      *reason = CZT_REASON_INV_PARAM;

      CZ_RETVALUE(RFAILED);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Peer Assoc Response OK\n"));
   CZ_RETVALUE(ROK);
}

/* cz001.101 : Added Audit feature */
/**
    @brief czUiDoAudit  is called to get the state of Peer.

    @details This function gets the State of Peer under Audit.

    @param[in] audit This is the Audit Event
    @param[in] cztSapCb The Upper SAP Control Block
    @param[out] cause Cause if Audit is not successful.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
 
*/
#ifdef ANSI
PRIVATE S16 czUiDoAudit 
(
CztAudEvnt *audit,   /* Audit Event */
CzCztSapCb *cztSapCb,  /* Upper Sap */
U8        *cause      /*kworks fix : changed type U16 to U8*/      
)
#else
PRIVATE S16 czUiDoAudit(audit, cztSapCb, cause)
CztAudEvnt *audit;   /* Audit Event */
CzCztSapCb *cztSapCb;  /* Upper Sap */
U8        *cause;      /*kworks Fix: changed type U16 to U8*/
#endif
{

   CZ_TRC3(czUiDoAudit);
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Czt Audit Event Address (%p), SAP Cb \
            Address (%p), Cause Address(%p)\
            \n", (Void *)audit, (Void *)cztSapCb, (Void *)cause));

   *cause = CZT_STACAUSE_NOT_APPL;

   switch(audit->type)
   {
      case CZT_AUD_PEER:
      {
         CzPeerCb    *peerCb;
         U32      peerId;

         peerId = audit->u.audPeer.peerId;

         if (czNmmFndPeer(peerId, &peerCb) != ROK)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Peer not found. \n"));
            *cause = CZT_ERRCAUSE_INV_PEERID;

            CZ_RETVALUE(RFAILED);
         }
         CZ_PRNT_PEERID;

         audit->u.audPeer.peerState = peerCb->state;
      }
      break;
      default:
      break;
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Audit Info filled\n"));
   CZ_RETVALUE (ROK); 
}


/** @} */


/**********************************************************************
  
         End of file:     tr_uim.c@@/main/2 - Tue Aug 30 18:36:16 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
                                        2. Add Audit feature.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
/main/2     cz001.301   akaranth        1. Inclusion for Diagnostics
                                        2. Change for Diagnostics
                                        3. KWORK warning fix      
*********************************************************************91*/
