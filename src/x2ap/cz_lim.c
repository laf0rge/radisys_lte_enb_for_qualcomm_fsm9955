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
  
     Desc:     This file handles the lower interface primitives.

     File:     cz_lim.c

     Sid:      tr_lim.c@@/main/2 - Tue Aug 30 18:36:13 2011

     Prg:       

**********************************************************************/
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
#include "cm_pasn.h"       /* common per asn.1                */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "czt.h"           /* CZT interface defines           */
#include "lcz.h"           /* X2AP Layer management           */
#include "cz.h"            /* X2AP layer defines              */
#include "cz_err.h"        /* X2AP - error */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* common per asn.1                */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT interface structures        */
#include "lcz.x"           /* X2AP Layer management           */
#include "cz.x"            /* X2AP layer structures           */
#ifdef SS_DIAG
/* cz001.301:Added for layer Diagnostics*/
#include "ss_diag.h"        /* Common log file */
#endif
/** @defgroup limmodule X2AP Lower Interface Module
 *  @{
 */

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


/**
 @brief 
   This function is used to process the bind confirm
   received from SCTP layer..

 @details
     This function informs the X2AP layer whether it has succeeded in binding to the
     requested SCT SAP. If a failure is reported, this function retries the operation. If it
     succeeds, this function sends a confirmation of the successful operation to the
     entity that requested the bind and send the edpoint open request to SCTP.
  
 
 @param[in] pst      Post structure. 
 @param[in] suId     Service user id.
 @param[in] result   Result of the bind request (SCT_OK/SCT_NOK).

  
 @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16 CzLiSctBndCfm 
(
Pst      *pst,               
SuId      suId,            
SctResult result       
)
#else /* ANSI */
PUBLIC S16 CzLiSctBndCfm (pst, suId, result)
Pst       *pst;               
SuId      suId;              
SctResult result;       
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   CzSctSapCb *sctSapCb = NULLP;
   /* Variable to store the alarm parameters */
   /*cz001.301: Removing redundant variable, Build warning fix */

   CZ_TRC3(CzLiSctBndCfm)
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), SctResult (%d)\n",
            (Void *)pst, suId, result ));
   /*cz001.301: Build warning fix*/
 
#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,
        (Void **)&czCbPtr)) !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT148, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d)) \n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));


   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      /* put error debug print */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid suId: %d from lower \
                                           layer in BndCfm\n", suId));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }

   /* If SapCb is NULLP then return invalid sap */
   if (!czCb.sctSapLst || ( (sctSapCb = czCb.sctSapLst[suId]) == NULLP))
   {
      /* put error debug print */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "NULL SctSapCb \n"));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }

   /* Check if sap stats is already in bound state */
   if (LCZ_SAP_BINDING != sctSapCb->state)
   {
      /* put error debug print */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid Sap state:(%d) \n", suId));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   } /* sap is already bound */
   sctSapCb = czCb.sctSapLst[suId];

   if(ROK != czSchedGenTmr(CZ_TMR_SCT_BIND, sctSapCb, CZ_TMR_STOP))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Stop Timer Failed \n"));
      CZ_RETVALUE(RFAILED);
   }
   
   sctSapCb->retryCnt = 0;
   /* Check the status of bind request */
   if(result == CM_BND_NOK)
   {
      /* Change the state of lower Sap to config */
      sctSapCb->retryCnt = 0;
      sctSapCb->state = LCZ_SAP_CFG;
      /* put error debug print */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Bind Confirm Failed \n"));
      /* Send an alarm with LCM_EVENT_BIND_FAIL */
      CZ_SEND_SAPID_ALARM(suId, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);

      CZ_RETVALUE(RFAILED);
   } 
   else
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, 
             "- Binding with lsap (%d) successful\n", suId));

      /* Change the state of the sap to bound */
      sctSapCb->state = LCZ_SAP_ISBOUND;

      /* Raise an alarm with LCM_EVENT_BND_OK */
      CZ_SEND_SAPID_ALARM(suId, LCM_EVENT_BND_OK, LCM_CAUSE_UNKNOWN);
      
      /* Since there is going to be only one end point, we can have
       * 0 as suEndpId, update the same in endpCb
       */
      sctSapCb->endpCb.locSuEndpId = CZ_LCL_SU_ENDPID;

      /* Change the state of endpoint to OPEN_IN_PRG */
      sctSapCb->endpCb.state = CZ_ENDP_OPEN_IN_PRG;
      /* Send end point open request */
#ifdef SCT_ENDP_MULTI_IPADDR      
      CzLiSctEndpOpenReq(&sctSapCb->pst, sctSapCb->spId, CZ_LCL_SU_ENDPID,
                         sctSapCb->endpCb.port,
                         (SctNetAddrLst*)&(sctSapCb->endpCb.srcAddrLst));
#else /* SCT_ENDP_MULTI_IPADDR */
      CzLiSctEndpOpenReq(&sctSapCb->pst, sctSapCb->spId, CZ_LCL_SU_ENDPID,
                         sctSapCb->endpCb.port,
                         &sctSapCb->endpCb.srcAddrLst.nAddr[0]);
#endif /* SCT_ENDP_MULTI_IPADDR */


   } /* end of if (result == OK) */

   CZ_RETVALUE(ROK);

} /* end of CzLiSctBndCfm */




/**
  @brief 
    SCTP Endp Open Confirm handler.
 
  @details 
    This function is used to process the end point open confirm primitive 
    handler from SCTP layer. Verifies that the result if SCT_OK then move the 
    end point state to CZ_ENDP_OPEN and generate an alarm to layer manager.
  
  @param[in] pst      Post structure. 
  @param[in] suId     Service user id.
  @param[in] suEndpId Service user end point Id.
  @param[in] spEndpId Service provider end point Id.
  @param[in] result   Result of the endpOpenReq (SCT_OK/SCT_NOK).
  @param[in] cause    Cause of the failure.
 
 @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctEndpOpenCfm 
(
Pst        *pst,              
SuId       suId,              
UConnId    suEndpId,          
UConnId    spEndpId,          
SctResult  result,            
SctCause   cause             
)
#else /* ANSI */
PUBLIC S16 CzLiSctEndpOpenCfm (pst, suId, suEndpId, spEndpId, result, cause)
Pst        *pst;              
SuId       suId;              
UConnId    suEndpId;          
UConnId    spEndpId;          
SctResult  result;            
SctCause   cause;             
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   CzSctSapCb *sctSapCb = NULLP;

   CZ_TRC3(CzLiSctEndpOpenCfm);
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suEndpId (%d) \
            spEndpId (%d) SctResult (%d) SctCause (%d)\n", (Void *)pst, suId, 
            suEndpId, spEndpId, result, cause ));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suEndpId (%ld) \
            spEndpId (%ld) SctResult (%d) SctCause (%d)\n", (Void *)pst, suId,
            suEndpId, spEndpId, result, cause ));
#endif /* ALIGN_64BIT */

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT149, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */

   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d))\n",
           pst->dstProcId, pst->dstEnt, pst->dstInst));

   CZ_DBG_INFO((CZ_PRNT_BUF,
   "(suId(%d),suEndpId(%ld),spEndpId(%ld),result(%d),cause(%d))\n",
           suId, suEndpId, spEndpId, result, cause));

   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_OPEN_CFM, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ( (sctSapCb = czCb.sctSapLst[suId]) == NULLP) ||
       (sctSapCb->state != LCZ_SAP_ISBOUND))
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_OPEN_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }
  
   /* Check the result. If endpopen request fails, 
    * then return the error 
    */
   if (result != SCT_OK)
   {
      /* Change the state of end point from OPEN_IN_PRG to CFG */
      sctSapCb->endpCb.state = CZ_ENDP_CFG;
      /* Raise an alarm with the cause received */
      switch(cause)
      {
         case SCT_CAUSE_INV_PAR_VAL:
         {
            cause = LCM_CAUSE_INV_PAR_VAL;    
            break;
         }     
              
         case SCT_CAUSE_ENDP_OPEN:
         {
            cause = LCZ_CAUSE_ENDP_OPEN;    
            break;
         }   
              
         case SCT_CAUSE_EXCEED_CONF_VAL:
         {
            cause = LCZ_CAUSE_EXCEED_CONF_VAL;    
             break;
         }     

         case SCT_CAUSE_OUTRES:
         {
            cause = LCZ_CAUSE_OUTRES;    
            break;
         }

         default:
                 break;   
      } /* end of switch */
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_OPEN_CFM, cause);
      CZ_RETVALUE(RFAILED);
   }
   else
   {
      /* Change the state of end point from OPEN_IN_PRG to OPEN */
      sctSapCb->endpCb.state = CZ_ENDP_OPEN;
      /* Update the spEndpId in sctSapCb */
      sctSapCb->endpCb.locSpEndpId = spEndpId;
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_OPEN_CFM, 
                                LCZ_CAUSE_SCT_ENPOPEN_SUCC);

   } /* end of if result == OK */

   CZ_RETVALUE(ROK);

} /* end of CzLiSctEndpOpenCfm */



/**
  @brief 
    SCTP Endp Close Confirm handler
 
  @details 
    This function is used to process the end point close confirm primitive 
    handler from SCTP layer.
  
  @param[in] pst      Post structure 
  @param[in] suId     Service user id
  @param[in] suEndpId Service user end point Id
  @param[in] result   Result of the end point close request (SCT_OK/SCT_NOK)
  @param[in] cause    Cause of the end point close failure
 
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctEndpCloseCfm 
(
Pst        *pst,              
SuId       suId,              
UConnId    suEndpId,          
SctResult  result,            
SctCause   cause              
)
#else /* ANSI */
PUBLIC S16 CzLiSctEndpCloseCfm (pst, suId, suEndpId, result, cause)
Pst        *pst;              
SuId       suId;              
UConnId    suEndpId;          
SctResult  result;            
SctCause   cause;             
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   CzSctSapCb *sctSapCb = NULLP;

   CZ_TRC3(CzLiSctEndpCloseCfm);

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT150, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suEndpId (%d), \
            SctResult (%d), SctCause (%d)\n",
            (Void *)pst, suId, suEndpId, result, cause ));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suEndpId (%ld), \
            SctResult (%d), SctCause (%d)\n",
            (Void *)pst, suId, suEndpId, result, cause ));
#endif /* ALIGN_64BIT */
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d)) \n",
           pst->dstProcId, pst->dstEnt, pst->dstInst));


   /* Need not proceed further if the layer has been shut down */ 
   if (czCb.shutdown)
      CZ_RETVALUE (ROK);

   /* Check if suID is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId,LCZ_EVENT_LI_ENDP_CLOSE_CFM,LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap control block is not valid then return invalid sap */
   if (!czCb.sctSapLst || ( (sctSapCb = czCb.sctSapLst[suId]) == NULLP))
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_CLOSE_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }
  
   /* Check the result. If endpclose request fails, 
    * then return the error 
    */
   if (result != SCT_OK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, 
             "- EndpCloseCfm fails \n"));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_CLOSE_CFM, 
                                LCZ_CAUSE_INV_PAR_VAL);
      CZ_RETVALUE(RFAILED);
   }
   else
   {
      /* Change the state of end point cfg */
      sctSapCb->endpCb.state = CZ_ENDP_CFG;
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ENDP_CLOSE_CFM, 
                                LCZ_CAUSE_EPCLOSE_OK);
   }

   CZ_RETVALUE(ROK);

} /* end of CzLiSctEndpCloseCfm */


/**
 @brief 
   SCTP Assoc Indication handler

 @details 
   This function is used to process the association indication received from
   SCTP layer.
 
 @param[in] pst          Post structure. 
 @param[in] suId         Service user id.
 @param[in] suEndpId     Service user end point Id.
 @param[in] assocParams  Association parameters.
 @param[in] vsInfo       Vendor specific info. 

  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctAssocInd 
(
Pst               *pst,              
SuId              suId,              
UConnId           suEndpId,          
SctAssocIndParams *assocParams,      
Buffer            *vsInfo            
)
#else /* ANSI */
PUBLIC S16 CzLiSctAssocInd (pst, suId, suEndpId, assocParams, vsInfo)
Pst               *pst;             
SuId              suId;            
UConnId           suEndpId;          
SctAssocIndParams *assocParams;      
Buffer            *vsInfo;           
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   CzSctSapCb *sctSapCb = NULLP;
   /* Variable to store the return value */
   S16         ret;
   
   CZ_TRC3(CzLiSctAssocInd);

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT151, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suEndpId (%d), \
            Association Params Address (%p), Buffer Vendor Spec address (%p)\n",
            (Void *)pst, suId, suEndpId, (Void *)assocParams, (Void *)vsInfo ));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suEndpId (%ld), \
            Association Params Address (%p), Buffer Vendor Spec address (%p)\n",
            (Void *)pst, suId, suEndpId, (Void *)assocParams, (Void *)vsInfo ));
#endif /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d))\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
   
   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ASSOC_IND, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ( (sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ASSOC_IND, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }
   /* Check for pst structure and assocParams */
   /*cz001.301: x2ap kwork fixes*/
   if(!(assocParams))
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ASSOC_IND, LCM_CAUSE_INV_PAR_VAL);
      CZ_RETVALUE(RFAILED);
   }

   /* Since vendor specific info is not used, if present 
    * free the memory
    */
   if(vsInfo)
   {
     CZ_FREEMBUF(vsInfo);
   } /* end of vsInfo */  

   ret = czTpmHndlAssocInd(suId, sctSapCb, assocParams);

   CZ_RETVALUE(ROK);

} /* end of CzLiSctAssocInd */ 


/**
  @brief 
    SCTP Assoc Confirm handler
 
  @details 
    This function is used to process the association confirm received from
    SCTP layer.
  
  @param[in] pst          Post structure 
  @param[in] suId         Service user id
  @param[in] suAssocId    Service user association ID
  @param[in] spAssocId    Service provider association ID
  @param[in] dstNAddrLst  Destination address list
  @param[in] dstPort      Destination port
  @param[in] outStrms     Number of outstreams
  @param[in] vsInfo       Vendor specific info 
 
 @return
 - ROK           - If the Procedure was successful.
 - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctAssocCfm 
(
Pst               *pst,              
SuId              suId,              
UConnId           suAssocId,         
UConnId           spAssocId,         
SctNetAddrLst     *dstNAddrLst,     
SctPort           dstPort,           
SctStrmId         outStrms,          
Buffer            *vsInfo           
)
#else /* ANSI */
PUBLIC S16 CzLiSctAssocCfm (pst, suId, suAssocId, spAssocId, dstNAddrLst, dstPort, outStrms, vsInfo)
Pst               *pst;              
SuId              suId;              
UConnId           suAssocId;         
UConnId           spAssocId;         
SctNetAddrLst     *dstNAddrLst;      
SctPort           dstPort;           
SctStrmId         outStrms;          
Buffer            *vsInfo;           
#endif /* ANSI */
{
   CzPeerCb   *peerCb = NULLP;
   U32         peerId;
   CzSctSapCb *sctSapCb = NULLP;
   S16         ret  = ROK;
   
   CZ_TRC3(CzLiSctAssocCfm);

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT170, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */
   
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId(%d), \
            spAssocId (%d), Destination address List address (%p), \
            SctPort (%d), Outgoing Stream ID (%d), \
            Buffer Vendor Spec Address (%p)\n",
            (Void *)pst, suId, suAssocId, spAssocId, (Void *)dstNAddrLst,
            dstPort, outStrms, (Void *)vsInfo));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId(%ld), \
            spAssocId (%ld), Destination address List address (%p), \
            SctPort (%d), Outgoing Stream ID (%d), \
            Buffer Vendor Spec Address (%p)\n",
            (Void *)pst, suId, suAssocId, spAssocId, (Void *)dstNAddrLst,
            dstPort, outStrms, (Void *)vsInfo));
#endif /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d))\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#ifdef SS_DIAG
  /*cz001.301: Added ofr layer Diagnostics*/
  CZ_DIAG_LVL0(0x04090001, CZ_DIAG_SUASSOCID, suAssocId,
               "Received AssocCfm from SCTP, spAssocId =  %ld",
                 spAssocId, 0, 0, 0);
#endif
   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ASSOC_CFM, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ((sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_ASSOC_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }

   /* Since vendor specific info is not used, if present 
    * free the memory
    */
   if(vsInfo)
   {
      CZ_FREEMBUF(vsInfo);
   } /* end of vsInfo is not NULL */   
           
   peerId = suAssocId;
   ret =  czNmmFndPeer(peerId, &peerCb);
   if (peerCb == NULLP)
   {
      CZ_RETVALUE(RFAILED);
   }
  
   /* Update the spAssocId in assocCb */
   peerCb->assocCb->spAssocId = spAssocId;

   /* Update the negotiated streams as SCTP will be sending the negotiated
    * streams during association establishment.
    */
   peerCb->assocCb->assocCfg->locOutStrms = outStrms;

   /* Call GP FSM if required from here with event CZ_EVENT_GP_ASSOC_REQ_CFM */

   CZ_RETVALUE(ROK);

} /* end of CzLiSctAssocCfm */



/**
  @brief 
    SCTP terminate indication handler 
 
  @details 
    This function is used to process the association terminate indication received 
    from  SCTP layer.
  
  @param[in] pst          Post structure 
  @param[in] suId         Service user id
  @param[in] assocId      Association ID
  @param[in] assocIdType  Association ID type
  @param[in] status       Status of the terminate request
  @param[in] cause        Cause if status is OK
  @param[in] rtrvInfo     Retrieve info if any 
 
 @return
 - ROK           - If the Procedure was successful.
 - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctTermInd 
(
Pst             *pst,            
SuId            suId,           
UConnId         assocId,         
U8              assocIdType,     
SctStatus       status,          
SctCause        cause,           
SctRtrvInfo     *rtrvInfo       
)
#else /* ANSI */
PUBLIC S16 CzLiSctTermInd(pst, suId, assocId, assocIdType, status, cause, rtrvInfo)
Pst             *pst;            
SuId            suId;            
UConnId         assocId;         
U8              assocIdType;     
SctStatus       status;         
SctCause        cause;           
SctRtrvInfo     *rtrvInfo;       
#endif /* ANSI */
{
   /* Variable to store the peer control block */
   S16         ret = RFAILED;
   CzSctSapCb *sctSapCb = NULLP;
   /* Variable to store status indication */
   CztStaInd   sta;
   
   CZ_TRC3(CzLiSctTermInd);

   cmMemset((U8*)&sta, 0, sizeof(CztStaInd));
#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT152, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), Association ID (%d),\
            Association Type (%d), SCTStatus (%d), SctCause (%d), \
            SctRtrvInfo (%p)\n", (Void *)pst, suId, assocId, assocIdType,
            status, cause, (Void *)rtrvInfo));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF,"Pst address(%p), suId (%d), Association ID (%ld),\
            Association Type (%d), SCTStatus (%d), SctCause (%d), \
            SctRtrvInfo (%p)\n", (Void *)pst, suId, assocId, assocIdType,
            status, cause, (Void *)rtrvInfo));
#endif /* ALIGN_64BIT */

   cmMemset((U8*)&sta, 0, sizeof(CztStaInd));
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d)) \n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));

#ifdef SS_DIAG
  /* cz001.301:Added for Layer Diagnostics */
  CZ_DIAG_LVL0(0x04090002, CZ_DIAG_NA, SS_DIAG_INV_ARG,
               "Received TermInd from SCTP, assocId =  %ld assocIdtype =  %ld",
                assocId, assocIdType, 0, 0);
#endif
   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_TERM_IND, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ( (sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_TERM_IND, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }

   /* Retrieve the peerCb.Since suAssocId is same as peerId
    * we can directly retireve the peerCb with index suAssocId 
    * in peerLst
    */
   ret = czTpmHndlTermInd(suId, assocId, assocIdType);
   CZ_RETVALUE(ROK);
} /* end of CzLiSctTermInd */



/**
  @brief 
    SCTP terminate confirm handler 
 
  @details 
    This function is used to process the association terminate confirm received 
    from  SCTP layer.
  
  @param[in] pst          Post structure 
  @param[in] suId         Service user id
  @param[in] suAssocId    Association ID
  @param[in] status       Status of the terminate confirm
  @param[in] cause        Cause if status is OK
 
 @return
 - ROK           - If the Procedure was successful.
 - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctTermCfm 
(
Pst             *pst,            
SuId            suId,           
UConnId         suAssocId,       
SctStatus       status,          
SctCause        cause          
)
#else /* ANSI */
PUBLIC S16 CzLiSctTermCfm(pst, suId, suAssocId, status, cause)
Pst             *pst;            
SuId            suId;           
UConnId         suAssocId;       
SctStatus       status;          
SctCause        cause;          
#endif /* ANSI */
{
   /* Variable to store the peer control block */
   U32 peerId;
   CzPeerCb   *peerCb = NULLP;
   CzSctSapCb *sctSapCb = NULLP;
   S16         ret  = ROK;
   
   CZ_TRC3(CzLiSctTermCfm);

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT153, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), Association ID (%d),\
            SCTStatus (%d), SctCause (%d)\n",
            (Void *)pst, suId, suAssocId, status, cause));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF,"Pst address(%p), suId (%d), Association ID (%ld),\
            SCTStatus (%d), SctCause (%d)\n",
            (Void *)pst, suId, suAssocId, status, cause));
#endif /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d))\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#ifdef SS_DIAG
  /* cz001.301:Added for Layer Diagnostics */
  CZ_DIAG_LVL0(0x04090003, CZ_DIAG_SUASSOCID, suAssocId,
               "Received TermCfm from SCTP", 0, 0, 0, 0);
#endif
   /* Need not proceed further if the layer has been shut down */ 
   if (czCb.shutdown)
      CZ_RETVALUE (ROK);

   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_TERM_CFM, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ((sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_TERM_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }

   /* Retrieve the peerCb.Since suAssocId is same as peerId
    * we can directly retireve the peerCb with index suAssocId 
    * in peerLst
    */
   peerId  = suAssocId;
   ret = czNmmFndPeer(peerId, &peerCb); 
   if (peerCb == NULLP)
   {
      CZ_RETVALUE (RFAILED);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF, 
          ": Association is terminated \n"));

   /* Send an alarm to LM indicating peer is deleted succsessfully */
   CZ_SEND_PEER_ALARM(peerCb->peerId, NULLP, LCZ_EVENT_LI_TERM_CFM, 
                                      LCZ_CAUSE_PEER_DELETED);

   /* Deallocate the peer */
   /* delete peerCb or disablePeer. */
   if(peerCb->delPcb)
     czNmmDeallocPeerCb(peerCb);
   else
     czNmmResetPeer(peerCb);
          
   CZ_RETVALUE (ROK);

} /* end of CzLiSctTermCfm */


/**
  @brief 
    SCTP data indication handler 
 
  @details 
    This function is used to process the SCTP data indication received 
    from  SCTP layer. All X2AP message are received through this interface primitive.
  
  @param[in] pst          Post structure. 
  @param[in] suId         Service user id.
  @param[in] suAssocId    Association ID.
  @param[in] strmId       Stream Id.
  @param[in] indType      Indication Type.
  @param[in] protId       Protocol Id.
  @param[in] mBuf         Buffer received.
 
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctDatInd
(
Pst             *pst,       
SuId            suId,        
UConnId         suAssocId,   
SctStrmId       strmId,       
SctDatIndType   *indType,    
U32             protId,      
Buffer          *mBuf         
)
#else /* ANSI */
PUBLIC S16 CzLiSctDatInd (pst, suId, suAssocId, strmId, indType, protId, mBuf)
Pst             *pst;        
SuId            suId;        
UConnId         suAssocId;    
SctStrmId       strmId;       
SctDatIndType   *indType;     
U32             protId;       
Buffer          *mBuf;       
#endif /* ANSI */
{
   U32          peerId;
   CzPeerCb    *peerCb = NULLP;   
   Status       memStatus;
   CzSctSapCb  *sctSapCb = NULLP;
   CzUsta       sta;
   S16         ret  = ROK;
   
   CZ_TRC3(CzLiSctDatInd);

   cmMemset((U8*)&memStatus, 0 ,sizeof(Status));
   cmMemset((U8*)&sta, 0 ,sizeof(CzUsta));

#ifdef SS_MULTIPLE_PROCS
   if (((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT155, (ErrVal)0, "failed, cannot derive czCb");
      /* cz001.301:Valgrind fix*/
      CZ_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), Association ID (%d),\
            SctStreamId (%d), Sct DataIndication type address (%p), \
            protId (%d), Buffer Vendor Spec address (%p)\n",
            (Void *)pst, suId, suAssocId, strmId, (Void *)indType, protId,
            (Void *)mBuf));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF,"Pst address(%p), suId (%d), Association ID (%ld),\
            SctStreamId (%d), Sct DataIndication type address (%p), \
            protId (%ld), Buffer Vendor Spec address (%p)\n",
            (Void *)pst, suId, suAssocId, strmId, (Void *)indType, protId,
            (Void *)mBuf));
#endif /* ALIGN_64BIT */
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d)) \n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));


   /* Check if suId is valid or not */
   if (suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_DAT_IND, LCM_CAUSE_INV_SPID);
      CZ_FREEMBUF(mBuf);
      CZ_RETVALUE(RFAILED);
   }

   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ((sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_DAT_IND, LCM_CAUSE_INV_SAP);
      CZ_FREEMBUF(mBuf);
      CZ_RETVALUE(RFAILED);
   }

    /* Get the peer control block */
   peerId  = suAssocId;
    ret = czNmmFndPeer(peerId, &peerCb);
    if (peerCb == NULLP)
    { 
       CZ_FREEMBUF(mBuf);
       CZ_RETVALUE(RFAILED);
    } /* end of if peerCb == NULLP */
   
   /* Call czRcvFromLower which will decode the message and
    * send it to appropriate hander
    */
   ret = czTpmRcvFromLower(mBuf, peerCb);   
   if(ret != ROK)
   {
       CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ROK);

} /* end of CzLiSctDatInd */


/**
  @brief 
    The service provider also uses the status indication primitive to return unsolicited
    status information to the service user. 
 
  @details 
    This function is used to process the SCTP status indication received 
    from  SCTP layer.
  
  @param[in] pst          Post structure. 
  @param[in] suId         Service user id.
  @param[in] suAssocId    Service user association ID.
  @param[in] spAssocId    Service provider association ID.
  @param[in] dstNAddr     Destination address.
  @param[in] status       Status.
  @param[in] cause        Cause of the failure.
  @param[in] mBuf         Buffer received.
 
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctStaInd
(
Pst              *pst,              
SuId             suId,              
UConnId          suAssocId,         
UConnId          spAssocId,         
CmNetAddr        *dstNAddr,        
SctStatus        status,            
SctCause         cause,           
Buffer           *mBuf           
)
#else /* ANSI */
PUBLIC S16 CzLiSctStaInd(pst, suId, suAssocId, spAssocId, dstNAddr, status, cause, mBuf)
Pst              *pst;              
SuId             suId;              
UConnId          suAssocId;         
UConnId          spAssocId;         
CmNetAddr        *dstNAddr;         
SctStatus        status;            
SctCause         cause;             
Buffer           *mBuf;             
#endif /* ANSI */
{
   U32         peerId;
   CzSctSapCb  *sctSapCb = NULLP;
   /* Variable to store the count of spId */
   /* Variable to store the status indication structure */
   /* cz001.301: Removing Redundant code, Build warning fixes */

   CZ_TRC3(CzLiSctStaInd);
#ifdef SS_DIAG
  /*cz001.301: Addeed for Layer Diagnostics*/
  CZ_DIAG_LVL0(0x04090004, CZ_DIAG_SUASSOCID, suAssocId,
               "Received Status indication from SCTP, spAssocId = %ld, status = %d, cause = %d",
                spAssocId, status, cause, 0);
#endif
   /*cz001.301:x2ap build warning fixes--here variable "sta" is never used*/


#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT156, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */
   
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%d), \
            spAssocId (%d), Destination Address (%p), SctStatus (%d), \
            SctCause (%d), Buffer Vendor Spec Address (%p)\n",
            (Void *)pst, suId, suAssocId, spAssocId, (Void *)dstNAddr,
            status, cause, (Void *)mBuf ));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%ld), \
            spAssocId (%ld), Destination Address (%p), SctStatus (%d), \
            SctCause (%d), Buffer Vendor Spec Address (%p)\n",
            (Void *)pst, suId, suAssocId, spAssocId, (Void *)dstNAddr,
            status, cause, (Void *)mBuf ));
#endif /* ALIGN_64BIT */

#ifdef SS_DIAG
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d)) \n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif
   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SPID);
/*      CZ_FREEMBUF(mBuf); */
      CZ_RETVALUE(RFAILED);
   }
   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ((sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SAP);
/*      CZ_FREEMBUF(mBuf);*/
      CZ_RETVALUE(RFAILED);
   }

   peerId = suAssocId;
   
   czTpmHndlStaInd(peerId, status, cause, dstNAddr, mBuf);
   
/*   CZ_FREEMBUF(mBuf);*/

   CZ_RETVALUE(ROK);
} /* end of CzLiSctStaInd */


/* cz001.101 : The reason values recieved from SCTP for SctFlcInd are mapped to
 * reason values in X2AP. */

/**
  @brief 
      The service provider uses the flow control
      indication primitive to indicate to the service user the state of memory available for outgoing messages.
  
 @details 
    This function is used to process the SCTP status indication with flow control information
    received from SCTP layer. Indicates change of flow control status.
  
  @param[in] pst          Post structure 
  @param[in] suId         Service user id
  @param[in] suAssocId    Service user association ID
  @param[in] reason       Reson for Flow control indication.
 
  
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16 CzLiSctFlcInd
(
Pst         *pst,         /* Post structure */
SuId        suId,         /* Service user Id */
UConnId     suAssocId,    /* Su Association ID */
Reason      reason        /* Reason */
)
#else /* ANSI */
PUBLIC S16 CzLiSctFlcInd (pst, suId, suAssocId, reason)
Pst         *pst;         /* Post structure */ 
SuId        suId;         /* Service user Id */
UConnId     suAssocId;    /* Su Association ID */
Reason      reason;       /* Reason */
#endif /* ANSI */
{
   S16 ret = RFAILED;
   U32 peerId;
   CzPeerCb *peerCb = NULLP;
   CztStaInd sta;
   CzCztSapCb  *cztSapCb = NULLP;
   CzSctSapCb *sctSapCb = NULLP;

   CZ_TRC3(CzLiSctFlcInd);
   
   cmMemset((U8*)&sta, 0, sizeof(CztStaInd));

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
        !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT146, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }

   CZ_DBGP((CZ_PRNT_BUF,
          "--------X2AP:-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));

#endif /* end of SS_MULTIPLE_PROCS */
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), Association ID (%d),\
            SctReason (%d) \n", (Void *)pst, suId, suAssocId, reason));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF,"Pst address(%p), suId (%d), Association ID (%ld),\
            SctReason (%d) \n", (Void *)pst, suId, suAssocId, reason));
#endif /* ALIGN_64BIT */
   
#ifdef SS_DIAG
  /* cz001.301:Modified for layer Diagnostcs*/
  CZ_DIAG_LVL0(0x04090005, CZ_DIAG_SUASSOCID, suAssocId,
               "Received Flow Control Indication from SCTP, reason = %d",
                reason, 0, 0, 0);
#endif
   /* If suId is not valid, return error */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_FLC_IND, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }

   /* If Sap is not bound then return invalid sap */
   if (!czCb.sctSapLst || ((sctSapCb = czCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LCZ_SAP_ISBOUND)
   {
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_FLC_IND, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }

   /* Get the peer control block */
   peerId  = suAssocId;
   ret = czNmmFndPeer(peerId,&peerCb);
   if (peerCb == NULLP)
   {
      CZ_RETVALUE(RFAILED);
   } /* end of if peerCb == NULLP */

   cztSapCb = peerCb->cztSapCb;

   /* Populate the status indication structure */
   sta.peerId = peerCb->peerId;
   sta.type = CZT_STA_TYPE_FLC;

   switch(reason)
   {
      case SCT_FLC_START:
         sta.status = CZT_STATUS_FLC_START;
         break;

      case SCT_FLC_DROP:
         sta.status = CZT_STATUS_FLC_DROP;
         break;

      case SCT_FLC_STOP:
         sta.status = CZT_STATUS_FLC_STOP;
         break;

      case SCT_FLC_ACTIVE:
         sta.status = CZT_STATUS_FLC_ACTIVE;
         break;

      case SCT_FLC_INACTIVE:
         sta.status = CZT_STATUS_FLC_INACTIVE;
         break;
   
      default:
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
         CZLOGERROR(ERRCLS_DEBUG, ECZXXX, 0,
         "Invalid reason received in flow control indication \n");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

         CZ_RETVALUE(RFAILED);

         }
   }/* switch(reason) */
   
   
   /* Send flow control indication to user */
   czUimSndStaInd(cztSapCb,&sta);

   CZ_SEND_PEER_ALARM(peerCb->peerId, NULLP, LCZ_EVENT_LI_FLC_IND, 
   LCZ_CAUSE_CONGESTION_RECV);

   CZ_RETVALUE(ROK);

} /* end of CzLiSctFlcInd */


/**
  @brief This fuction will be called from SCTP for the sucsessful confirmation
        to the heartbeat request. 

  @details
  This function is used to process the heartbeat confirmation 
  received from SCTP layer.

  @param[in] pst          Post structure. 
  @param[in] suId         Service user id.
  @param[in] suAssocId    Service user association ID.
  @param[in] dstNAddr     Destination address.
  @param[in] status       Status.
  @param[in] result       Result of the endpOpenReq (SCT_OK/SCT_NOK).
  @param[in] cause        Cause of the failure.
 
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.

*/

#ifdef ANSI
PUBLIC S16 CzLiSctHBeatCfm
(
Pst         *pst,         /* Post structure */
SuId        suId,         /* Service user Id */
UConnId     suAssocId,    /* Su Association ID */
CmNetAddr   *dstNAddr,    /* Destination Address */
SctStatus   status,       /* Status of HBeat request */
SctResult   result,       /* Result */
SctCause    cause         /* Cause */
)
#else /* ANSI */
PUBLIC S16 CzLiSctHBeatCfm (pst, suId, suAssocId, dstNAddr, status, result, cause)
Pst         *pst;         /* Post structure */ 
SuId        suId;         /* Service user Id */
UConnId     suAssocId;    /* Su Association ID */
CmNetAddr   *dstNAddr;    /* Destination Address */
SctStatus   status;       /* Status of HBeat request */
SctResult   result;       /* Result */
SctCause    cause;        /* Cause */
#endif /* ANSI */
{
   CZ_TRC3(CzLiSctHBeatCfm);
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%d), \
            Destination Address (%p), SctStatus (%d), \
            SctResult (%d), SctCause (%d)\n",
            (Void *)pst, suId, suAssocId, (Void *)dstNAddr, status, result,
            cause));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%ld), \
            Destination Address (%p), SctStatus (%d), \
            SctResult (%d), SctCause (%d)\n",
            (Void *)pst, suId, suAssocId, (Void *)dstNAddr, status, result,
            cause));
#endif /* ALIGN_64BIT */

   CZ_DBGP((czCb.init.prntBuf,
          "--------X2AP:-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(suAssocId);
   UNUSED(dstNAddr);
   UNUSED(status);
   UNUSED(result);
   UNUSED(cause);

   CZ_RETVALUE(ROK);
} /* end of CzLiSctHBeatCfm */


/**
  @brief This fuction will be called from SCTP for the sucsessful confirmation
        to the Sta request. 

  @details This function is used to process the status confirmation from
     the SCTP layer.

  @param[in] pst          Post structure.
  @param[in] suId         Service user id.
  @param[in] suAssocId    Service user association id. 
  @param[in] dstNAddr     Destination network address list. 
  @param[in] result       Result of the endpOpenReq (SCT_OK/SCT_NOK).
  @param[in] cause        Cause of the failure.
  @param[in] staInfo      Status information.
 
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PUBLIC S16 CzLiSctStaCfm
(
Pst              *pst,              /* Post structure */
SuId             suId,              /* service userid */
UConnId          suAssocId,         /* service user association id */
CmNetAddr        *dstNAddr,         /* destination network address list */
SctResult        result,            /* result */
SctCause         cause,             /* cause */
SctStaInfo       *staInfo           /* status information */
)
#else /* ANSI */
PUBLIC S16 CzLiSctStaCfm(pst, suId, suAssocId, dstNAddr, result, cause, staInfo)
Pst              *pst;              /* Post structure */
SuId             suId;              /* service userid */
UConnId          suAssocId;         /* service user association id */
CmNetAddr        *dstNAddr;         /* destination network address list */
SctResult        result;            /* result */
SctCause         cause;             /* cause */
SctStaInfo       *staInfo;          /* status information */
#endif /* ANSI */
{
   CZ_TRC3(CzLiSctStaCfm);

#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%d), \
            Destination Address (%p), SctResult (%d), \
            SctCause (%d), Status Info Address (%p)\n",
            (Void *)pst, suId, suAssocId, (Void *)dstNAddr, result,
            cause, (Void *)staInfo ));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%ld), \
            Destination Address (%p), SctResult (%d), \
            SctCause (%d), Status Info Address (%p)\n",
            (Void *)pst, suId, suAssocId, (Void *)dstNAddr, result,
            cause, (Void *)staInfo ));
#endif /* ALIGN_64BIT */

   CZ_DBGP((czCb.init.prntBuf,
          "--------X2AP:-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(suAssocId);
   UNUSED(dstNAddr);
   UNUSED(result);
   UNUSED(cause);
   UNUSED(staInfo);

   CZ_RETVALUE(ROK);

} /* end of CzLiSctStaCfm */

/**
  @brief This fuction will be called from SCTP for the sucsessful confirmation
        to the set Primary address request. 

  @details This function is used to process the primary addres confirmation from the SCTP layer
           to the set primary address request.Update the assocCb with the changed primary address.
           The result is informed to the Layer manager through an alarm.
           The SCTP gives back a SetPriCfm,
           - if it indicates success the new address in copied from peer cfg to
           assoc cfg of the peer and it will be used for transport purpose.
           An alarm is given to the user with
           event as LCZ_EVENT_LI_PRI_CFM and cause as LCZ_CAUSE_SCT_SET_PRI_SUCC.
           - If it was a failure the old address is copied from asoc cfg to peer
           cfg and the old address is used for transport purpose. An alarm is given as
           event LCZ_EVENT_LI_PRI_CFM and cause as LCZ_CAUSE_INV_PAR_VAL/LCM_CAUSE_UNKNOWN.
           CtrlCfm is given already to the LCZ with status as OK always
           when the Request was processed.


  @param[in] pst          Post structure. 
  @param[in] suId         Service user id.
  @param[in] suAssocId    Service user association id.
  @param[in] result       Result of the endpOpenReq (SCT_OK/SCT_NOK).
  @param[in] cause        Cause of the failure.
 
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.

*/

/* cz001.101 : Set the primary address in the AssocCfg structure of a peerCb 
 * depending the result in SctSetPriCfm.
*/
#ifdef ANSI
PUBLIC S16 CzLiSctSetPriCfm
(
Pst              *pst,              /* Post structure */
SuId             suId,              /* service userid */
UConnId          suAssocId,         /* service user association id */
SctResult        result,            /* result */
SctCause         cause              /* cause */
)
#else /* ANSI */
PUBLIC S16 CzLiSctSetPriCfm(pst, suId, suAssocId, result, cause)
Pst              *pst;              /* Post structure */
SuId             suId;              /* service userid */
UConnId          suAssocId;         /* service user association id */
SctResult        result;            /* result */
SctCause         cause;             /* cause */
#endif /* ANSI */
{
   U32 peerId;
   /* Variable to store the sct sap control block */
   CzSctSapCb *sctSapCb = NULLP;
   S16 ret=RFAILED;
   CzPeerCb *peerCb=NULLP;
   CZ_TRC3(CzLiSctSetPriCfm);

   UNUSED(pst);
   UNUSED(suAssocId);

   CZ_DBGP((czCb.init.prntBuf,
            "--------X2AP:-----(proc(%d),ent(%d),inst(%d))-------\n",
            pst->dstProcId, pst->dstEnt, pst->dstInst));
#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,
        (Void **)&czCbPtr)) !=ROK) || (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT158, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* end of SS_MULTIPLE_PROCS */
#ifdef ALIGN_64BIT
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%d), \
             SctResult (%d), SctCause (%d)\n",
            (Void *)pst, suId, suAssocId, result, cause));
#else /* ALIGN_64BIT */
   CZ_DBG_PARAM((CZ_PRNT_BUF, "Pst address(%p), suId (%d), suAssocId (%ld), \
             SctResult (%d), SctCause (%d)\n",
            (Void *)pst, suId, suAssocId, result, cause));
#endif /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF,
          "(proc(%d),ent(%d),inst(%d)) \n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));


   /* Check if suId is valid or not */
   if(suId < 0 || suId > czCb.genCfg.nmbSctSaps)
   {
      /* put error debug print */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid suId: %d from lower \
                                           layer in PriCfm\n", suId));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_PRI_CFM, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(RFAILED);
   }

   /* If SapCb is NULLP then return invalid sap */
   if (!czCb.sctSapLst || ( (sctSapCb = czCb.sctSapLst[suId]) == NULLP))
   {
      /* put error debug print */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "NULL SctSapCb \n"));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_PRI_CFM, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(RFAILED);
   }

   peerId = suAssocId;
   ret = czNmmFndPeer(peerId, &peerCb);
   if( ret!=ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Failed to find the peerCb\n"));
      CZ_RETVALUE(ret);
   }
   /* give an Alarm based on the result */
   if(result == SCT_OK)
   {
      /* update the new address assocCfg */
      cmMemcpy ((U8*)&peerCb->assocCb->assocCfg->priDstAddr,\
            (U8 *)&peerCb->peerCfg.priDstAddr, 
            sizeof(CmNetAddr));
      CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_PRI_CFM,
            LCZ_CAUSE_SCT_SET_PRI_SUCC);
   }
   else
   {
      /* revert the new address temporarily in peerCfg to the old one */
      cmMemcpy ((U8 *)&peerCb->peerCfg.priDstAddr, \
            (U8*)&peerCb->assocCb->assocCfg->priDstAddr,
            sizeof(CmNetAddr));
      switch(cause)
      {
         case SCT_CAUSE_INV_PAR_VAL:
            {
               CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_PRI_CFM, 
                     LCZ_CAUSE_INV_PAR_VAL);
               break;
            }
         default:
            {
               CZ_SEND_SAPID_ALARM(suId, LCZ_EVENT_LI_PRI_CFM, 
                     LCM_CAUSE_UNKNOWN);
               break;
            }
      }
   }

   CZ_RETVALUE(ROK);
} /* end of CzLiSctSetPriCfm */


/** @} */
/**********************************************************************
  
         End of file:     tr_lim.c@@/main/2 - Tue Aug 30 18:36:13 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
/main/3     cz001.301   akaranth        1. Added for Layer Diagnostics.
                                        2. Build warning fixes.
                                        3. klockwork warning fix.
                                        4. Valgrind fix.
*********************************************************************91*/
