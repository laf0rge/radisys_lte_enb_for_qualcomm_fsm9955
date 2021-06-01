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
 
     Name:     X2AP - Management Interface Module
  
     Type:     C source file
  
     @details     C source code for LM Module
  
     File:     tr_lmm.c
  
     Sid:      tr_lmm.c@@/main/2 - Tue Aug 30 18:36:13 2011
  
     Prg:      sy
  
*********************************************************************21*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
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
#include "cm_pasn.x"       /* Common ASn.1 Library           */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT interface structures        */
#include "lcz.x"           /* X2AP Layer management           */
#include "cz.x"            /* X2AP layer structures           */


/** @defgroup lmmmodule X2AP Layer Manager Module
 *  @{
 */

/* Private function declarations */
PRIVATE S16 czLmmSendLmCfm ARGS((
      Pst *pst,                           
      Header *hdr,                           
      U8 type,                               
      CzMngmt *cfm                           
      ));
PRIVATE S16 czLmmGenCfg ARGS((
      CzCfg *gen,       
      CmStatus *status     
      ));
PRIVATE S16 czLmmValidateGenCfg ARGS((
      CzGenCfg *gen        
      ));
PRIVATE S16 czLmmProtCfg ARGS((
      CzCfg *prot,     
      CmStatus *status     
      ));
PRIVATE S16 czLmmCztSapCfg ARGS((
      CzCztSapCfg *cztSap,     
      CmStatus *status         
      ));
PRIVATE S16 czLmmValidateCztSapCfg ARGS((
      CzCztSapCfg *cztSap      
      ));
PRIVATE S16 czLmmSctSapCfg ARGS((
      CzSctSapCfg *sctSap,     
      CmStatus *status         
      ));
PRIVATE S16 czLmmValidateSctSapCfg ARGS((
      CzSctSapCfg *sctSap      
      ));
PRIVATE S16 czLmmPeerCfg ARGS((
      CzPeerLstCfg *peer,     
      CmStatus *status     
      ));
PRIVATE S16 czLmmValidatePeerCfg ARGS((
      CzPeerLstCfg *peer      
      ));
PRIVATE S16 czLmmGenCntrl ARGS((
      CzCntrl *cntrl,         
      CmStatus *status        
      ));
PRIVATE S16 czLmmCztSapCntrl ARGS((
      CzCntrl *cntrl,         
      CmStatus *status        
      ));
PRIVATE S16 czLmmDelCztSap ARGS((
      SpId  sapId, 
      U8    opn
      ));
PRIVATE S16 czLmmSctSapCntrl ARGS((
      CzCntrl *cntrl,         
      CmStatus *status        
      ));

PRIVATE S16 czLmmEndPtCntrl ARGS((
         CzCntrl *cntrl,       
         CmStatus *status 
         ));

PRIVATE S16 czLmmAssocCntrl ARGS((
         CzCntrl *cntrl,         
         CmStatus *status        
         ));
PRIVATE S16 czLmmEnaSctSap ARGS((
      SpId  lSapId         
      ));
PRIVATE S16 czLmmUbndSctSap ARGS((
      SpId  lSapId,        
      U8    opn
      ));
PRIVATE S16 czLmmDelSctSap ARGS((
      SpId  lSapId, 
      U8    opn
      ));
PRIVATE S16 czLmmPeerCntrl ARGS((
      CzCntrl *cntrl,         
      CmStatus *status        
      ));
PRIVATE S16 czLmmValidatePeer ARGS((
      U32  peerId,
      CzPeerCb  **peerCb
      ));
PRIVATE S16 czLmmDelPeer ARGS((
      CzPeerCb *peerCb     
      ));
PRIVATE S16 czLmmGetGenSts ARGS((
      CzGenSts *sts,              
      CmStatus *status,           
      Action action               
      ));
PRIVATE S16 czLmmValidateGenSts ARGS((
      CzGenSts  *sts,         
      Action action           
      ));
PRIVATE S16 czLmmGetPeerNodeSts ARGS((
      CzPeerNodeSts *sts,         
      CmStatus *status,           
      Action action               
      ));
PRIVATE S16 czLmmValidatePNodeSts ARGS((
      CzPeerNodeSts  *sts,         
      Action action                
      ));
PRIVATE S16 czLmmGetGenSta ARGS((
      CzGenSta *sta,         
      CmStatus *status       
      ));     
PRIVATE S16 czLmmGetCztSapSta ARGS((
      CzSSapSta *sta,         
      CmStatus *status       
      ));
PRIVATE S16 czLmmValidateCztSapId ARGS((
      SpId sSapId          
      ));
PRIVATE S16 czLmmGetSctSapSta ARGS((
      CzTSapSta *sta,         
      CmStatus *status       
      ));
PRIVATE S16 czLmmValidateSctSapId ARGS((
      SpId  tSapId          
      ));
PRIVATE S16 czLmmGetPeerSta ARGS((
      CzPeerSta *sta,         
      CmStatus *status        
      ));
PRIVATE S16 czLmmValidatePeerSta ARGS((
      CzPeerSta *sta         
      ));


/* cz001.101 : CR Fixes, Memory Leak fixes. */

/**
@brief X2AP configuration request primitive.

@details
    The layer manager configures the various elements of X2AP using the 
    management configuration procedure. The X2AP configuration request 
    primitive (@ref CzMiLczCfgReq)can be called more than once. 
    @ref CzMiLczCfgReq primitives must be called before the bind primitives 
    are called. The list of the X2AP configuration request primitive 
    types are as follows.

This function implements the following logic. 

switch (cfg->hdr.elmId.elmnt)
   - case STCZGEN 
      - Update the genCfg field in CzCb. 
      - Allocate the maximum static memory required by the X2AP product.
      - Register the timer service by calling SReqTmr.
      - Initialize all the hash bins.
      - Set CfgDone for not configuring again.
      
   - case STCZPROT
      - Check protocol configration done. If yes assumed as re-configration and copy the
        protReCfg reconfigration parameters into nodeCb.protCfg else copy the protCfg parameters in 
        to nodeCb.protCfg. 
      - Set CfgDone for not configuring again.
   - case STCZCZTSAP 
      - If genCfg is not done, send negative confirm to the layer manager.
      - If all the parameters are valid, then allocate the CztSapCb control block.
        - Update the SAP control block with the information provided in the 
          configuration request.
        - Send configuration confirm with the status, success.
   - case STCZSCTSAP 
      - If genCfg is not done, send negative confirm to the layer manager. 
      - If all the parametes are valid, then allocate the czSctSapCb control block.
        - Update the SAP control block with the information provided in the configuration request.
        - Send configuration confirm with the status, success. 
   - case STCZPEER 
      - If protCfg is not done, send negative confirm to the layer manager.
      - Validate the peer list and number with configured number of peers in genral Cfg.
      - Send negative confirm to the layer manager if peerCb already exist.
      - If all the parametes are valid, then allocate the peerCb control block.
        - Update the peerCb control block with the assoc and other information provided in the 
          configuration request.
        - Send configuration confirm with the status, success.
end switch
 
    @param[in] pst Post structure.
    @param[in] cfg Management structure.
    
@return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.

*/

#ifdef ANSI
PUBLIC S16 CzMiLczCfgReq
(
Pst *pst,
CzMngmt *cfg            /* Config structure */
)
#else
PUBLIC S16 CzMiLczCfgReq(pst, cfg)
Pst *pst;
CzMngmt *cfg;           /* Config structure */
#endif
{
   S16 ret = ROK;       /* Return Value */
   CzMngmt *cfgCfm;      /* Config confirm */

   CZ_TRC3(CzMiLczCfgReq);
   cfgCfm = NULLP;


#ifdef SS_MULTIPLE_PROCS
   if( ((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr)) 
                 != ROK) || (czCbPtr == NULLP) )
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT143, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------Configuration Request-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /*SS_MULTIPLE_PROCS */

   CZ_DBG_INFO((CZ_PRNT_BUF,"(pst, elmId (%d))\n", cfg->hdr.elmId.elmnt));

   /* memset the config Cfm structure */
   CZ_ALLOC(cfgCfm,sizeof (CzMngmt));
   if(cfgCfm == NULLP)
   {
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate cfgCfm.\n"));
      CZ_RETVALUE(RFAILED);
   }
   /*Check if General Configuration is done */
   if ((!czCb.init.cfgDone) && (cfg->hdr.elmId.elmnt != STCZGEN))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,": General configuration not done "));

      if ((cfg->hdr.elmId.elmnt == STCZPEER))
      {
         if((cfg->t.cfg.u.peerLst.peerCfg != NULLP) && 
               (cfg->t.cfg.u.peerLst.nmbPeers!= 0))
         {
            (Void)SPutSBuf(czCb.init.region, czCb.init.pool, 
                  (Data*)(cfg->t.cfg.u.peerLst.peerCfg), 
                  (sizeof(CzPeerLstCfg) *  cfg->t.cfg.u.peerLst.nmbPeers));       

            cfg->t.cfg.u.peerLst.peerCfg    =  NULLP;
         }
      }
      cfgCfm->cfm.status = LCM_PRIM_NOK;
      cfgCfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      czLmmSendLmCfm (pst, &cfg->hdr, TCFG, cfgCfm);
      CZ_FREE(cfg, sizeof(CzMngmt));
      CZ_RETVALUE(ROK);
   }/* End of Configuration is done*/
   
  /* swith thorough the type of element passesd for the configration */
   switch (cfg->hdr.elmId.elmnt)
   {
      case STCZGEN:       
      {
         /* send the genral configration or genral reconfigration */
         ret = czLmmGenCfg (&cfg->t.cfg, &cfgCfm->cfm);
         break;
      }
      case STCZPROT:    
      {
         /* protocol configration request */
         ret = czLmmProtCfg (&cfg->t.cfg, &cfgCfm->cfm);
         break;
      }
      case STCZCZTSAP:      
      {
         /* X2AP's upper CZT sap configration */
         ret = czLmmCztSapCfg (&cfg->t.cfg.u.cztSap, &cfgCfm->cfm);
         break;
      }
      case STCZSCTSAP:     
      {
         /* X2AP's lower SCT sap Configration */ 
         ret = czLmmSctSapCfg (&cfg->t.cfg.u.sctSap, &cfgCfm->cfm);
         break;
      }
      case STCZPEER:
      {
         /* X2Ap's Peer list configration with peer transport address */
         ret = czLmmPeerCfg (&cfg->t.cfg.u.peerLst, &cfgCfm->cfm);
         break;
      }
      default:
      {
         /* None of the element mached for the configration */
         CZ_DBG_ERROR((CZ_PRNT_BUF,"invalid \
                  elmnt(%d)",cfg->hdr.elmId.elmnt));

         cfgCfm->cfm.status = LCM_PRIM_NOK;
         cfgCfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         ret=RFAILED; 

      }
      
   }/* end of the switch */

   czLmmSendLmCfm(pst, &cfg->hdr, TCFG, cfgCfm);
      
   CZ_FREE(cfg, sizeof(CzMngmt));

   CZ_RETVALUE(ret); 
    

} /* End of CzMiLczCfgReq */


/**
  @brief X2AP general layer configuration request.
  
  @details 
  This function stores the general X2AP configuration. 
  It calculates the memory requirement and reserves it. It also initializes the
  control blocks, the hash lists and registers the timers. In
  case of error it rolls back and reports error in the confirm.
  If the general configuration is already done it is treated as
  reconfiguration.

  @param[in] cfg         X2AP Layer manager general config structure.
  @param[out] status     Updated status for success/failure.
                -# Success : LCM_REASON_NOT_APPL 
                -# Failure : LCM_REASON_MEM_NOAVAIL
                             LCM_REASON_REGTMR_FAIL
                             LCM_REASON_INVALID_PAR_VAL
                             LCM_REASON_HASHING_FAILED
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/      

#ifdef ANSI
PRIVATE S16 czLmmGenCfg
(
CzCfg     *cfg,       /* General configuration */
CmStatus  *status     /* Return status */
)
#else
PRIVATE S16 czLmmGenCfg(cfg, status)
CzCfg *cfg;       /* General configuration */
CmStatus *status;    /* Return status */
#endif
{
   Size memSize;
   S16  ret;
   U32  cnt;
   CzGenCfg       *gen=NULLP;       /* Configuration structure */
   CzGenReCfg     *genReCfg=NULLP;  /* Reconfiguration structure */

   CZ_TRC2(czLmmGenCfg);

   CZ_DBG_INFO((CZ_PRNT_BUF,"General Configuration.....\n"));
   /* used for the genral configration */
   gen= &cfg->u.gen;   

  /* used for the genral reconfigration */
   genReCfg= &cfg->r.genReCfg;

   /* set the staus to Sucsess */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

  /*if configration is already done. than its a reconfigration 
  * copy the parametrs from GenReCfg structure into CzCb*/
   if ((czCb.init.cfgDone == TRUE)) 
   {

      CZ_DBG_INFO((CZ_PRNT_BUF,"Reconfiguration........\n"));
      /*validate for timer resolution shoult not be zero */
      if ((genReCfg->timeRes <= 0))
      {
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         CZ_RETVALUE(RFAILED);
      }
      
      /*copy the timer resolution into CzCb control block */
      czCb.genCfg.timeRes= genReCfg->timeRes;

      /* Set the debuf Mask for the layer provided from configration */
#ifdef DEBUGP
      czCb.genCfg.dbgMask= genReCfg->dbgMask; 
      czCb.init.dbgMask= genReCfg->dbgMask; 
#endif
      
      /* copy the Max upper and lower memory threshol values */
      czCb.genCfg.memUpperThr=genReCfg->memUpperThr;
      czCb.genCfg.memLowerThr=genReCfg->memLowerThr;
      
      /*return from here for the re-configration */
      CZ_RETVALUE(ROK);
   } /* end of reconfiguration */

   /* Validate the general configuration parameters */
   if((status->reason= czLmmValidateGenCfg(gen)) != LCM_REASON_NOT_APPL)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"General Congifuration failed. Invalid \
               parameters. \n"));
      status->status = LCM_PRIM_NOK;
      CZ_RETVALUE(RFAILED);
   }

   /* copy all the gencfg parametrs into into CzCb control Block */
   CZ_SET_ZERO(&czCb.genCfg,sizeof(CzGenCfg));
   cmMemcpy ((U8 *)&czCb.genCfg, (U8 *)gen, sizeof (CzGenCfg));

/* Set the debuf Mask for the layer provided from configration */
#ifdef DEBUGP
   czCb.init.dbgMask = gen->dbgMask; 
#endif

  /* Calculate the memory requirement for all the  configration sent By Layer manager */
   memSize =  czCb.genCfg.nmbCztSaps * (sizeof(CzCztSapCb) + sizeof(CzCztSapCb *));
   
   memSize += czCb.genCfg.nmbSctSaps * (sizeof(CzSctSapCb) + sizeof(CzSctSapCb *));
   
   memSize += czCb.genCfg.maxNmbPeer    * (sizeof(CzPeerCb)   + sizeof(CzPeerCb *));

   
   /* Reserve the memory for the X2AP*/
   ret = SGetSMem(czCb.init.region, (Size) memSize, &czCb.init.pool);
   if (ret != ROK)
   {
      SLogError(gen->lmPst.srcEnt, gen->lmPst.srcInst, gen->lmPst.srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Failed to Reserve the memory for the X2AP\n"));
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      CZ_RETVALUE (ret);
   }

   /* Initialize the statistics control block  */
   CZ_SET_ZERO(&czCb.genSts,sizeof(CzGenSts));
   
   /* Initialize the status control block  */
   CZ_SET_ZERO(&czCb.genSta, sizeof(CzGenSta));

   /* copy the totl mem size into sts */
   czCb.genSta.memSize = memSize;

   /* Allocate the memory for Upper SAP list */
   CZ_ALLOC(czCb.cztSapLst, (gen->nmbCztSaps * sizeof(CzCztSapCb *)));
   if (((CzCztSapCb *)czCb.cztSapLst == (CzCztSapCb *)NULLP))
   {
      SLogError(gen->lmPst.srcEnt, gen->lmPst.srcInst, gen->lmPst.srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      
      CZ_DBG_FATAL((CZ_PRNT_BUF, "czCb.cztSapLst allocation failed\n"));
      (Void) SPutSMem(czCb.init.region, czCb.init.pool);

      status->status=LCM_PRIM_NOK;
      status->reason=LCM_CAUSE_MEM_ALLOC_FAIL;
      CZ_RETVALUE(RFAILED);
   }
   /* Allocate memory for the Lower SAP list */  
   CZ_ALLOC(czCb.sctSapLst, (gen->nmbSctSaps * sizeof(CzSctSapCb *)));

   if ((CzSctSapCb *)czCb.sctSapLst == (CzSctSapCb *)NULLP)
   {
      SLogError(gen->lmPst.srcEnt, gen->lmPst.srcInst, gen->lmPst.srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF, "czCb.sctSapLst allocation failed\n"));
  
      /* Free the above allocation */
      CZ_FREE(czCb.cztSapLst, (gen->nmbCztSaps * sizeof(CzCztSapCb *)));
      (Void) SPutSMem(czCb.init.region, czCb.init.pool);

      status->status=LCM_PRIM_NOK;
      status->reason=LCM_CAUSE_MEM_ALLOC_FAIL;
      CZ_RETVALUE(RFAILED);
   }
   /* Register the timers */
   ret = czRegInitTmr();

#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed to register the timers.\n"));
      CZ_FREE(czCb.sctSapLst, (gen->nmbSctSaps * sizeof(CzSctSapCb *)));
      CZ_FREE(czCb.cztSapLst, (gen->nmbCztSaps * sizeof(CzCztSapCb *)));

      (Void) SPutSMem(czCb.init.region, czCb.init.pool);

      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_REGTMR_FAIL;
      CZ_RETVALUE (ret);
   }
#endif 
   /* Initialize the elements of CzCb */ 

   for (cnt = 0; cnt < (U32) czCb.genCfg.nmbCztSaps; cnt++)
   {
      /* Initialize the upper SapCb Lst */
      *(czCb.cztSapLst + cnt) = NULLP;
   }

   for (cnt = 0; cnt < (U32) czCb.genCfg.nmbSctSaps; cnt++)
   {
      /* Initialize the Lower  SapCb Lst */
      *(czCb.sctSapLst + cnt) = NULLP;
   }
  
   
   CZ_DBG_INFO((CZ_PRNT_BUF,"Initializing PeerCbLst\n"));
   /* Initialize the the PeerCbLst */
   ret = czNmmInitPeerLst();

#if (ERRCLASS & ERRCLS_DEBUG)
   /* If failed to initialize the peer list rever back all the allocation  */
   if (ret != ROK)
   {
#ifdef SS_MULTIPLE_PROCS
      SDeregTmr(czCb.init.procId, czCb.init.ent, czCb.init.inst, (S16)czCb.genCfg.timeRes,
            (PAIFTMRS16)czActvTmr);
#else
      SDeregTmr(czCb.init.ent, czCb.init.inst, (S16)czCb.genCfg.timeRes,
            (PFS16)czActvTmr);
#endif

      CZ_FREE(czCb.sctSapLst, (gen->nmbSctSaps * sizeof(CzSctSapCb *)));
      CZ_FREE(czCb.cztSapLst, (gen->nmbCztSaps * sizeof(CzCztSapCb *)));


      (Void) SPutSMem(czCb.init.region, czCb.init.pool);

      status->status = LCM_PRIM_NOK;
      status->reason = LCZ_REASON_HASHINIT_FAIL;
      CZ_RETVALUE (ret);
   }
#endif 
   /* copy the Lm pst into X2Ap czCb control block*/
   (Void) cmMemcpy((U8 *)&czCb.init.lmPst, (U8 *) &gen->lmPst,
                   (PTR)sizeof(Pst));

   czCb.init.lmPst.srcProcId = czCb.init.procId;
   czCb.init.lmPst.srcEnt    = czCb.init.ent;
   czCb.init.lmPst.srcInst   = czCb.init.inst;
   czCb.init.lmPst.event     = EVTNONE;

   CZ_DBG_INFO((CZ_PRNT_BUF,"General Configuration Done.\n"));
   /* set the Initial configration done as TRUE */
   czCb.init.cfgDone = TRUE;

   /* Reset the shutdown flag */
   czCb.shutdown = FALSE;

   CZ_RETVALUE(ROK);

} /* End of czGenCfg */

/* cz001.101 : CR Fixes, Memory Leak fixes. */
/**
 @brief X2AP protocol specific configration request.
 
 @details This function stores the X2AP protocol configuration. It is use to set the protocol spacifc parameters 
    It also initializes the control blocks, in cofigure the endp point related information into X2AP.
    If the protocol configration is already done it is treated as reconfiguration.

  @param[in]  cfg         X2AP Layer manager protocol config structure.
  @param[out] status      Updated status for success/failure.

               -# Success : LCM_REASON_NOT_APPL 
               -# Failure : LCM_REASON_MEM_NOAVAIL
                            LCM_REASON_REGTMR_FAIL
                            LCM_REASON_INVALID_PAR_VAL
                            LCM_REASON_HASHING_FAILED
  @return
                -       ROK           - If the Procedure was successful.
                -       RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmProtCfg
(
CzCfg *cfg,     /* Protocol configuration */
CmStatus *status     /* Return status */
)
#else
PRIVATE S16 czLmmProtCfg(cfg, status)
CzCfg *cfg;     /* Protocol configuration */
CmStatus *status;    /* Return status */
#endif
{
   CzProtCfg *prot=NULLP;
   CzProtReCfg *protReCfg=NULLP;
   S16 i;
   CZ_TRC2(czLmmProtCfg);
  
   CZ_DBG_INFO((CZ_PRNT_BUF,"Protocol Configuration.......\n"));
   prot= &cfg->u.prot;
   protReCfg = &cfg->r.protReCfg;
   
   /* set the staus to Sucsess */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;


   /*check if already protocol configration is done if so its a re configration */
   if ((czCb.nodeCb.cfgDone == TRUE))
   {
      if((protReCfg->tX2RELOCOverall==0) 
            || (protReCfg->tRELOCprep )==0
            || (protReCfg->timeToWait==0))
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Timer value ZERO is not allowed in the reconfigration\n"));
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         CZ_RETVALUE(RFAILED);
      }   
      /* Enable/disable  dynamic peer configuration */
      czCb.nodeCb.protCfg.dynamPeerFlag=protReCfg->dynamPeerFlag;  
      czCb.nodeCb.protCfg.defDPeerUSapId=protReCfg->defDPeerUSapId;  

       
      /*Handover prepration Timer */
      czCb.nodeCb.protCfg.tRELOCprep= protReCfg->tRELOCprep; 
      
     /* Handover prepration overall timer*/
      czCb.nodeCb.protCfg.tX2RELOCOverall=protReCfg->tX2RELOCOverall; 
      
      /*Value for the time towait IE */
      czCb.nodeCb.protCfg.timeToWait=protReCfg->timeToWait;
         
      CZ_RETVALUE(ROK);
   } 
 
   /* else configure all the values in Prot Cfg  */
   else 
   { 

      if((prot->tX2RELOCOverall==0) 
               || (prot->tRELOCprep )==0
               || (prot->timeToWait==0))
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Timer value ZERO is not allowed in\
                     the protocol configration\n"));
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_PAR_VAL;
            CZ_RETVALUE(RFAILED);
         }

      /* Enable/disable  dynamic peer configuration */
      czCb.nodeCb.protCfg.dynamPeerFlag=prot->dynamPeerFlag;  
      czCb.nodeCb.protCfg.defDPeerUSapId=prot->defDPeerUSapId;  
       
      /*Handover prepration Timer */
      czCb.nodeCb.protCfg.tRELOCprep= prot->tRELOCprep;       

     /* Handover prepration overall timer*/
      czCb.nodeCb.protCfg.tX2RELOCOverall=prot->tX2RELOCOverall;       

      /*Value for the time towait IE */
      czCb.nodeCb.protCfg.timeToWait=prot->timeToWait;
   }

    if(prot->srcAddr.ipAddr.nmb <= 0)
    {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Ip address num value zero is not \
                  allowed in the protocol configration\n"));
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         CZ_RETVALUE(RFAILED);

    }

   for(i =0; i < prot->srcAddr.ipAddr.nmb; i++)
   {
      if(prot->srcAddr.ipAddr.nAddr[i].type  != CM_IPV4ADDR_TYPE 
#ifdef SB_IPV6_SUPPORTED
/*- IPV6 Changes: changed the condition from || to && -*/
         && prot->srcAddr.ipAddr.nAddr[i].type  != CM_IPV6ADDR_TYPE 
#endif 
        )          

         {

         CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid Ip address Type in  protocol configration\n"));
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         CZ_RETVALUE(RFAILED);
         }
   }
   /* copy the protocol related information into nodeCb control block */
   CZ_SET_ZERO(&czCb.nodeCb.protCfg,sizeof(CzProtCfg));
   cmMemcpy ((U8 *)&czCb.nodeCb.protCfg, (U8 *)prot, sizeof (CzProtCfg));

   CZ_DBG_INFO((CZ_PRNT_BUF,"Protocol Configuration Done\n"));
   /* Set the protocol configration to TRU first time only */   
   czCb.nodeCb.cfgDone = TRUE;

   CZ_RETVALUE(ROK);

} /* czLmmProtCfg */

/**
  @brief This function configures the X2AP upper sap.

  @details
    The upper SAP configuration is sent at least once for each upper SAP.
    There is one upper SAP (CztSap) for the X2AP user.
 
   @param[in]  cztSap    X2AP Layer manager CZT Sap config structure.
   @param[out] status    Updated status for success/failure.

              -# Success : LCM_REASON_NOT_APPL.
              -# Failure : LCM_REASON_INVALID_SAP. 
                           LCZ_REASON_PROTCFG_NOT_DONE. 
                           LCM_REASON_EXCEED_CONF_VAL.
                           LCM_REASON_MEM_NOAVAIL.

   @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
  */

#ifdef ANSI
PRIVATE S16 czLmmCztSapCfg
(
CzCztSapCfg *cztSap,     /* Upper SAP configuration */
CmStatus *status         /* Return status */
)
#else
PRIVATE S16 czLmmCztSapCfg(cztSap, status)
CzCztSapCfg *cztSap;     /* Upper SAP configuration */
CmStatus *status;        /* Return status */
#endif
{
   CzCztSapCb    *cztSapCb=NULLP;

   CZ_TRC2(czLmmCztSapCfg);

   CZ_DBG_INFO((CZ_PRNT_BUF,"SSAP Configuration.SpId is %d.\n", cztSap->spId));
   /* set the staus to Sucsess */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate CZT SAP configuration elements */
   if((status->reason= czLmmValidateCztSapCfg(cztSap))!=LCM_REASON_NOT_APPL)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"SSAP Configuration failed. Validation \
               of parameters failed.\n"));
      status->status = LCM_PRIM_NOK;
      CZ_RETVALUE(RFAILED);
   }


  /*Get upper Sap control block from upper sapCb Lst*/
   cztSapCb = czCb.cztSapLst[cztSap->spId];
  
   /* If sap Control Block already exist it means its reconfigration */
   if ((cztSapCb != NULLP))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"SSAP reconfiguration.\n"));
      /* Reconfiguration */
      cztSapCb->pst.selector  = cztSap->selector;
      cztSapCb->pst.prior     = cztSap->priority;
      cztSapCb->pst.region    = cztSap->mem.region;
      cztSapCb->pst.pool      = cztSap->mem.pool;
      CZ_RETVALUE(ROK);
   }
   
  
   /* Allocate Upper SAP Control Block */
   CZ_ALLOC(cztSapCb ,sizeof(CzCztSapCb)); 
   
   if (cztSapCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate cztSapCb.\n"));
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      CZ_RETVALUE(RFAILED);
   }

   CZ_SET_ZERO(cztSapCb,sizeof(CzCztSapCb));

   /* Initialize the elements of CzCztSapCb */
   cztSapCb->suId = 0;
#ifdef SS_MULTIPLE_PROCS
   cztSapCb->pst.srcProcId = czCb.init.procId;
#else
   cztSapCb->pst.srcProcId = SFndProcId();
#endif
   cztSapCb->pst.srcEnt = czCb.init.ent;
   cztSapCb->pst.srcInst = czCb.init.inst;
   cztSapCb->pst.prior = cztSap->priority;
   cztSapCb->pst.route = cztSap->route;
   cztSapCb->pst.region = cztSap->mem.region;
   cztSapCb->pst.pool = cztSap->mem.pool;
   cztSapCb->pst.selector = cztSap->selector;
   cztSapCb->state = LCZ_SAP_CFG;
   cztSapCb->spId  = cztSap->spId;


   /* Place cztSapCb in czCb's CztSap list */
   czCb.cztSapLst[cztSap->spId] = cztSapCb;

   CZ_DBG_INFO((CZ_PRNT_BUF,"SSAP configuration Done.\n"));
   CZ_RETVALUE(ROK);

} /* czLmmCztSapCfg */

/**
  @brief This function configures the X2AP lower sctp sap.

  @details
  The lower SAP configuration is sent at least once for each lower SAP
  -   There is one lower SAP (SctSap) for the X2AP user.
  -   The lower SAP configurations such as SctSap configuration must
  be performed after the general and protocol configuration for either
  - For SCTP or
  - For Kernel SCTP

  
   @param[in]  sctSap     X2AP Layer manager sct Sap structure.
   @param[out] status     Updated status for success/failure.
                  
                -# Success : LCM_REASON_NOT_APPL.
                -# Failure : LCZ_REASON_PROTCFG_NOT_DONE .
                             LCM_REASON_EXCEED_CONF_VAL. 
                    
   @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
        
 */

#ifdef ANSI
PRIVATE S16 czLmmSctSapCfg
(
CzSctSapCfg *sctSap,     /* lower SAP configuration */
CmStatus *status         /* Return status */
)
#else
PRIVATE S16 czLmmSctSapCfg(sctSap, status)
CzSctSapCfg *sctSap;     /* lower SAP configuration */
CmStatus *status;        /* Return status */
#endif
{
   CzSctSapCb *sctSapCb=NULLP;

   CZ_TRC2(czLmmSctSapCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   CZ_DBG_INFO((CZ_PRNT_BUF,"TSAP Configuration.SpId is %d.\n", sctSap->spId));
   /* Validate the protocol parameters */
   if((status->reason= czLmmValidateSctSapCfg(sctSap))!= LCM_REASON_NOT_APPL)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"TSAP configuration failed. Validation of \
               parameters failed.\n"));
      status->status = LCM_PRIM_NOK;
      CZ_RETVALUE(RFAILED);
   }
   /* Get Sap control block */
   sctSapCb = czCb.sctSapLst[sctSap->suId];

   if (sctSapCb != NULLP)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"TSAP reconfiguration.\n"));
      /* Reconfiguration */
      sctSapCb->pst.prior = sctSap->priority;
      sctSapCb->pst.region = sctSap->mem.region;
      sctSapCb->pst.pool = sctSap->mem.pool;
      sctSapCb->maxBndRetry  = sctSap->maxBndRetry;

      CZ_RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   CZ_ALLOC(sctSapCb, sizeof(CzSctSapCb));

   if (sctSapCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate sctSapCb.\n"));
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      CZ_RETVALUE(RFAILED);
   }

   CZ_SET_ZERO(sctSapCb,sizeof(CzSctSapCb));

   /* Initialize the elements of CzSctSapCb */
   sctSapCb->suId = sctSap->suId;
   sctSapCb->spId = sctSap->spId;

   sctSapCb->pst.selector = sctSap->selector;
   sctSapCb->pst.region = sctSap->mem.region;
   sctSapCb->pst.pool = sctSap->mem.pool;
   sctSapCb->pst.prior = sctSap->priority;
   sctSapCb->pst.route = sctSap->route;
#ifdef SS_MULTIPLE_PROCS
   sctSapCb->pst.srcProcId = czCb.init.procId;
#else
   sctSapCb->pst.srcProcId = SFndProcId();
#endif
   sctSapCb->pst.srcEnt = czCb.init.ent;
   sctSapCb->pst.srcInst = czCb.init.inst;
   sctSapCb->pst.dstProcId = sctSap->dstProcId;
   sctSapCb->pst.dstEnt = sctSap->dstEntId;
   sctSapCb->pst.dstInst = sctSap->dstInstId;
   cmMemcpy ((U8 *)&sctSapCb->tBndTmr, (U8 *)&sctSap->tBndTmr, sizeof(TmrCfg));
   sctSapCb->maxBndRetry  = sctSap->maxBndRetry;
   sctSapCb->trcMask = 0;
   sctSapCb->trcLen = 0;
   sctSapCb->retryCnt = 0;
   cmInitTimers(sctSapCb->timers, CZ_MAX_SCTSAP_TMRS);

   sctSapCb->contEnt = ENTSM;

   sctSapCb->state = LCZ_SAP_CFG;

      sctSapCb->endpCb.port = czCb.nodeCb.protCfg.srcAddr.sctPort;
      cmMemcpy((U8 *)&(sctSapCb->endpCb.srcAddrLst),
         (U8 *)&(czCb.nodeCb.protCfg.srcAddr.ipAddr), sizeof(SctNetAddrLst));

      sctSapCb->endpCb.state = CZ_ENDP_CFG;

   /* Place sctSapCb in CzCb's SctSap list */
   czCb.sctSapLst[sctSap->suId] = sctSapCb;

   CZ_DBG_INFO((CZ_PRNT_BUF,"TSAP configuration done.\n"));
   CZ_RETVALUE(ROK);

} /* czLmmSctSapCfg */

/**
   @brief This function validates and creates peer control block. 
 
   @details
      This fuction is used perform  peer configuration, consists of the information about the manually
      configured peers. After the all configrations, configure the list of peers with the peer information. 
      X2AP allocates the necessary memory for all the peers.if the peerId is already exist then it will send the failure.
      And the association is triggered after receiving a X2AP setup request message from the user.
      Layer manager can call this many times to add the peers. This API initialize the GpmCb and global procedure 
      related Timers.
       
   @param[in]  peer       X2AP Layer manager CzPeerCfg  structure.
   @param[out] status     Updated status for success/failure.

            -# Success : LCM_REASON_NOT_APPL
            -# Failure : LCZ_REASON_PROTCFG_NOT_DONE 
                         LCM_REASON_EXCEED_CONF_VAL
                         LCM_REASON_MEM_NOAVAIL
                    
   @return 
        - ROK           - If the Procedure was successful.
        - RFAILED       - If the procedure was unsuccessful due to any failure.
        
 */
#ifdef ANSI
PRIVATE S16 czLmmPeerCfg
(
CzPeerLstCfg *peer,     /* Peer configuration */
CmStatus *status     /* Return status */
)
#else
PRIVATE S16 czLmmPeerCfg(peer, status)
CzPeerLstCfg *peer;     /* Peer configuration */
CmStatus *status;    /* Return status */
#endif
{
   S16 ret;             /* return value */
   CzPeerCb *peerCb=NULLP;    /* Peer control block */
   U32 loopCnt;               /* index */

   CZ_TRC2(czLmmPeerCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;
   
   CZ_DBG_INFO((CZ_PRNT_BUF,"Peer Configuration\n"));
   /* Validate the peer configuration parameters  */
    ret = czLmmValidatePeerCfg(peer);

   /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
   if ((ret != LCM_REASON_NOT_APPL))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Peer configuration failed. Validation of\
               parameters failed.\n"));
      if(peer->nmbPeers)
      {
            (Void)SPutSBuf(czCb.init.region, czCb.init.pool, 
                  (Data*)(peer->peerCfg), 
                  (sizeof(CzPeerLstCfg) *  peer->nmbPeers));       
      }
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      CZ_RETVALUE(RFAILED);
   }
   
  /* loop through all the peers and check if the peer is allready exist return
   * failure .which holds good for both configration and reconfigration */
   for (loopCnt = 0; loopCnt < peer->nmbPeers;loopCnt++)
   {

      /* Allocate Peer Control Block one by one .
       * this internally initialize gpm contex and gpm timers also init the ue
       * Cb list and resouce status list  */
      
      if ((ret = czNmmAllocPeerCb(peer->peerCfg[loopCnt].peerId, &peerCb)) != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,"Peer configuration failed. \
                  Failed to allocate the peer"));
      (Void)SPutSBuf(czCb.init.region, czCb.init.pool, 
                  (Data*)(peer->peerCfg), 
                  (sizeof(CzPeerLstCfg) *  peer->nmbPeers));       

         peer->peerCfg = NULLP;
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_MEM_NOAVAIL;
         CZ_RETVALUE(RFAILED);
      }

      /* set the dynamic peer suppot */
      peerCb->peerCfg.isDynamCfgd = czCb.nodeCb.protCfg.dynamPeerFlag; 

     /* Encryption algoritham supported by this eNodeb */
      peerCb->peerCfg.eAValue=peer->peerCfg[loopCnt].eAValue; 

      /* Intigrity Alogoritham Supported forthis eNodeb */
      peerCb->peerCfg.iPAValue=peer->peerCfg[loopCnt].iPAValue;

      /*Copy all the association related info into the PeerC Control block */
      cmMemcpy ((U8 *)peerCb->assocCb->assocCfg, 
            (U8 *)&peer->peerCfg[loopCnt].assocCfg, sizeof (CzAssocCfg));
      

      /* Initialize the trace fror the Peer control block */
      peerCb->peerCfg.trcMask = 0;
      peerCb->peerCfg.trcLen = 0;

      /*set the refrence of  upper SapCb into to peer control block */
      peerCb->cztSapCb = czCb.cztSapLst[peer->peerCfg[loopCnt].uSapId]; 

      /* keep the refrence of Sct sapCb control block */
      peerCb->sctSapCb = czCb.sctSapLst[peer->peerCfg[loopCnt].tSapId]; 

      CZ_SET_ZERO(&peerCb->peerCfg.peerSts, sizeof (CzPeerNodeSts));

      /* check the user has provided the global stram Id else set it to ZERO */
      if ((peer->peerCfg[loopCnt].globalProcStrmId.pres))
      {
         peerCb->peerCfg.globalStreamId = peer->peerCfg[loopCnt].globalProcStrmId.val;
      }
      else
      {
         /* default value set by the X2Ap layer */
         peerCb->peerCfg.globalStreamId = 0;
      }

      /* Initalize the UE straem counter to distrubute the load */
      peerCb->nextStreamId =0;

   }

   /* Finally free the memory for peerCfg allocated by the user */
   /* changed size from CzPeerLstCfg to CzPeerCfg */
      (Void)SPutSBuf(czCb.init.region, czCb.init.pool, 
                  (Data*)(peer->peerCfg), 
                  (sizeof(CzPeerCfg) *  peer->nmbPeers));       
   peer->peerCfg = NULLP;

   CZ_DBG_INFO((CZ_PRNT_BUF,"Peer configuration Done.\n"));
   CZ_RETVALUE(ROK);

} /* End of czLmmPeerCfg */

/**
@brief X2AP control request primitive.

@details 
The layer manager uses the management control procedure to control X2AP
elements. The layer manager initiates this procedure. The X2AP control request
primitive (@ref CzMiLczCntrlReq) can be called more than once, any time after
the management configuration procedure.

- if(cfgDone = FALSE) 
   - send negative confirmation to the layer manager; 
   - return; 
- else 
 -  switch(cntrl->hdr.elmId.elmnt)
     - case STCZGEN
           - general (applicable layer-wide) control request.
           - Enable/Disable  the Unsliciated status.
           - Enable/Disable and diable the debug.
           - Enable/Disable the alaram.
           - shutdown the X2AP layer
           
     - case STCZCZTSAP
             X2AP's upper CZT  SAP contrl request 
     - case STCZSCTSAP
           -  X2AP's lower Sct SAP contrl request.
           
     - case STCZPEER
           - peer control request to control the peer trace.
           - To delete the peer from peer List
           
     - case STTPTSRV
           - X2AP's Sctp  End point control Request to enable/disable the end point.
           - using this control request we can open or close the sctp end point.
     - case STASSOC
            - Association control only for for modifying the primary destination address.
            - This primitive is valid for the if the association is already established.
            - will set the Sct destination addres to addres provided in this control request.
            
      - default
            - None of the element mached for the configration. 
            -Invalid Element in control request.
   end switch 

@param[in] pst   Pointer to the pst structure.
@param[in] cntrl Pointer to management structure. 
@return 
        - ROK           - If the procedure was successful.
        - RFAILED       - If the procedure was unsuccessful due to any failure.
*/



#ifdef ANSI
PUBLIC S16 CzMiLczCntrlReq
(
Pst     *pst,               
CzMngmt *cntrl              /* Control structure */
)
#else
PUBLIC S16 CzMiLczCntrlReq(pst, cntrl)
Pst     *pst;               
CzMngmt *cntrl;             /* Control structure */
#endif
{
   S16 ret = ROK;           /* Return value */
   CzMngmt *cntrlCfm;        /* Control confirm */

   CZ_TRC3(CzMiLczCntrlReq);
   cntrlCfm = NULLP;


#ifdef SS_MULTIPLE_PROCS
   if( ((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                 != ROK) || (czCbPtr == NULLP) )
   {
      /*cz001.301:Klockwork fix-Accesing czCb if NULL*/
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT144, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------Control Request-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /*SS_MULTIPLE_PROCS */

   CZ_DBG_INFO((CZ_PRNT_BUF,
      "Control Request (pst, elmId (%d))\n", cntrl->hdr.elmId.elmnt));
   /* memset the config Cfm structure */
   CZ_ALLOC(cntrlCfm, sizeof (CzMngmt));
   if(cntrlCfm == NULLP)
   {
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate ctrlCfm.\n"));
      CZ_RETVALUE(RFAILED);
   }


   /* Check if General Configuration is done */
   if ((!czCb.init.cfgDone)) 
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
                 "Control Request failed. General configuration not done "));

      cntrlCfm->cfm.status = LCM_PRIM_NOK;
      cntrlCfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      czLmmSendLmCfm (pst, &cntrl->hdr, TCNTRL, cntrlCfm);
      CZ_FREE(cntrl, sizeof(CzMngmt));
      CZ_RETVALUE(ROK);
   }

   /* check element */
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STCZGEN:
         {
            /* general (applicable layer-wide) control */
            ret = czLmmGenCntrl(&cntrl->t.cntrl, &cntrlCfm->cfm);
            break;
         }
      case STCZCZTSAP:
         {
            /* X2AP's upper CZT  SAP contrl request */
            ret = czLmmCztSapCntrl(&cntrl->t.cntrl, &cntrlCfm->cfm);
            break;
         }
      case STCZSCTSAP:
         {
            /* X2AP's lower Sct SAP contrl request */
            ret = czLmmSctSapCntrl(&cntrl->t.cntrl, &cntrlCfm->cfm);
            break;
         }
      case STCZPEER:
         {
            /*X2AP's  Peer control request */
            ret = czLmmPeerCntrl(&cntrl->t.cntrl, &cntrlCfm->cfm);
            break;
         }
      case STTPTSRV:
         {
            /*X2AP's Sctp  End point control Request to enable/disable the End
             * point by sending the sct lower sapId*/

            ret = czLmmEndPtCntrl(&cntrl->t.cntrl, &cntrlCfm->cfm);
            break;
         }
      case STASSOC:
         {
            /*Association control only for for modifying the primary destination address  
             * for the established association with the Action as MODIFY*/
            ret = czLmmAssocCntrl(&cntrl->t.cntrl, &cntrlCfm->cfm);
            break;
         }


      default:
         {
            /* None of the element mached for the configration */
            CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid Element in control request"));
            cntrlCfm->cfm.status = LCM_PRIM_NOK;
            cntrlCfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
            break;
         }
   } /* end switch */

   /*Finally send the Confirm to Layer manager */
   czLmmSendLmCfm(pst, &cntrl->hdr, TCNTRL, cntrlCfm);
      
   CZ_FREE(cntrl, sizeof(CzMngmt));

   CZ_RETVALUE(ret);

} /* CzMiLczCntrlReq */

/**
@brief This function initiates and uses the management-control procedure to control X2AP genral elements.The X2AP control request primitive (genCntrlReq) can be called more than once and at any time after the management-configuration procedure.The control request primitive is confirmed by a CzMiLczCntrlCfm primitive.

@details 
  This function implements the following logic \n

- if(cfgDone = FALSE) 
   - send negative confirmation to the layer manager; 
   - return; 
- else 
- switch(cntrl->hdr.elmId.elmnt) 
   -  case STGEN 
        - switch (action) 
        - case AENA 
             - switch(subAction) 
             - case SAUSTA 
                  - enable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SADBG 
                  - set the debug mask; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - enable the trace flag; 
                  - send the control Cfm with success; 
        - case ADISIMM 
             - switch(subAction) 
             - case SAUSTA 
                  - disable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - disable the trace flag; 
                  - send the control Cfm with success; 
        - case ASHUTDOWN 
                  - Abort all the association.   
                  - Delete all the peerCb
                  - unbind and delete Lower sap 
                  - dlete the upper sap.
                  
   - default 
                 - Invalid action 
                 
   @param[in] cntrl    Layer manager management structure.
   @param[out] status     Updated status for success/failure.
   
    @return 
        - ROK           - If the Procedure was successful.
        - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmGenCntrl
(
CzCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmGenCntrl(cntrl, status)
CzCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
   S16   ret=ROK;
   CZ_TRC2(czGenCntrl);
   
   status->status = LCM_PRIM_NOK;

   /* switch through the Action for this element */
   switch (cntrl->action)
   {
      case AENA:
      {
         switch (cntrl->subAction)
         {
#ifdef DEBUGP
            case SADBG:
            {
               CZ_DBG_INFO((CZ_PRNT_BUF,"Control Request to enable debug\n"));
               /* reset the debug mask sent in the cotrol regest to dunamically 
                * update the debug logs */
               
               czCb.init.dbgMask |= cntrl->u.dbg.dbgMask;
               break;
            }
#endif /*DEBUGP*/
#ifdef SS_DIAG
            /* cz001.301: Change for Diagnostics*/
            case SALOG:
            {
               czCb.init.logMask = cntrl->u.logMask;
               break;
            }
#endif
            case SAUSTA: 
            {
               CZ_DBG_INFO((CZ_PRNT_BUF,"Control Request to enable \
                        unsolicitated status.\n"));
               /*Enable the unsoliciated status by setting the sts value
                * in the CzCb control block to TRUE */
               czCb.init.usta = TRUE;
               break;
            }
            default:
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF,"czGenCntrl:Invalid Sub Action "));
               /* return the status with invalid sub action */
               status->reason = LCM_REASON_INVALID_SUBACTION;
               CZ_RETVALUE(RFAILED);
            }
         }
         break;
      }
      case ADISIMM:
      {
         switch (cntrl->subAction)
         {
#ifdef DEBUGP
            case SADBG:
               /* disable  the debug mask sent in the cotrol request to dunamically 
                * update the debug logs */
            {
               CZ_DBG_INFO((CZ_PRNT_BUF,"Control Request to disable debug\n"));
               czCb.init.dbgMask &= ~(cntrl->u.dbg.dbgMask);
               break;
            }
#endif /* DEBUGP */
#ifdef SS_DIAG
            /* cz001.301: Change for Diagnostics*/
            case SALOG:
            {
               czCb.init.logMask = cntrl->u.logMask;
               break;
            } 
#endif
            case SAUSTA:
            {
               
               CZ_DBG_INFO((CZ_PRNT_BUF,"Control Request to disable \
                        unsolicitated status.\n"));
               /*disable  the unsoliciated status by setting the sts valuew 
                * in the CzCb control block to FALSE */
               czCb.init.usta = FALSE;
               break;
            }
            default:
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF,"czGenCntrl:Invalid Sub Action "));
               status->reason = LCM_REASON_INVALID_SUBACTION;
               CZ_RETVALUE(RFAILED);
            }
         }
         break;
      }
      case ASHUTDOWN:
      {
               CZ_DBG_INFO((CZ_PRNT_BUF,"Control Request to shutdown.\n"));
         /* Sen the Shutdown request to shutdown the x2A Player  */
         if ( (ret = czLmmShutdown()) != ROK)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,"czGenCntrl:Failed to shutdown X2AP"));
            status->reason = LCM_REASON_LYR_SPECIFIC;
            CZ_RETVALUE(ret);
         } 
         break;
      }
      default:
      {
   
         CZ_DBG_ERROR((CZ_PRNT_BUF,"czGenCntrl:Invalid Action "));
         status->reason = LCM_REASON_INVALID_ACTION;
         CZ_RETVALUE(RFAILED);
      }
   } 

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   CZ_DBG_INFO((CZ_PRNT_BUF,"Control Request Done...\n"));
   CZ_RETVALUE(ROK);

} /* End of czLmmGenCntrl */

/**
  @brief This function processes the CZT Upper SAP control request.

  @details This primitive is used to Unbind and disable and delete 
          the Upper CZT sap.
  
  This fuction implements the following logic: \n
  if(Validation =FALSE)
    -send negative confirmation to the layer manager
    -retrun
-else     
  - switch (action)
  -  case AUBND_DIS
         - Unbind the upper sap
         - Only change the sap state and return no need to terminate the association.
  - case ADEL
            - check the SAP is bound 
            - send unbind the the upper SAP. 
            - delete the refrence for upper SAP into the peer control block.
            - Finally free the sapCb from the SapCb list.
  - default:
           -Invalid elment action.
       
  @param[in] cntrl    X2AP Layer manager control structure
  @param[out] status  Updated with sucsess/Failure.
      -# Success : LCM_REASON_NOT_APPL
      -# Failure : LCM_REASON_INVALID_SUBACTION
                   LCM_REASON_INVALID_ACTION
 
  @return 
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
 */

#ifdef ANSI
PRIVATE S16 czLmmCztSapCntrl
(
CzCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmCztSapCntrl(cntrl, status)
CzCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
   S16 ret=RFAILED;

   CZ_TRC2(czCztSapCntrl);

   CZ_DBG_INFO((CZ_PRNT_BUF,"SAP control Request.\n"));
   if((status->reason = czLmmValidateCztSapId (cntrl->u.sap.id))!=LCM_REASON_NOT_APPL)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"SAP control failed. Invalid parameterds.\n"));
      status->status = LCM_PRIM_NOK;
      CZ_RETVALUE(ret);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,"CZT SAP spId = %d.\n", cntrl->u.sap.id));
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (cntrl->action)
   {
      case AUBND_DIS:
      {
            czCb.cztSapLst[cntrl->u.sap.id]->state=LCZ_SAP_CFG;
            CZ_DBG_INFO((CZ_PRNT_BUF,"CZT SAP(%d) disabled sucsess",\
                     cntrl->u.sap.id));
            ret=ROK;
            break;
      }
      case ADEL:
      {
         if ((ret = czLmmDelCztSap (cntrl->u.sap.id,CZ_DEL_SAP)) != ROK)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,\
                       "czCztSapCntrl, Couldn't delete the CZT \
                       SAP(%d)",cntrl->u.sap.id));
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_PAR_VAL;
         }
         break;
      }
      default:
      {
          CZ_DBG_ERROR((CZ_PRNT_BUF,
                       "czCztSapCntrl,Invalid elment action "));
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_ACTION;
         break;
      }
   } 

   CZ_RETVALUE(ret);

} /* czLmmCztSapCntrl */

/**
  @brief This function will bind/unbind X2AP lower sap.

  @details
   This fuction is used to enable /disable the trace for the sap.
   and also to bind/unbind  and  delete the lower sap which internally 
   remove all logical connections associated with this SCT Sap
   and deallocate the sctSap.
 
   @param[in] cntrl X2AP Layer manager structure
   @param[out] status     Updated status for success/failure.
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCM_REASON_INVALID_SAP 
                    LCM_REASON_INVALID_STATE
                    LCM_REASON_INVALID_ACTION 
   @return
         - ROK           - If the Procedure was successful.
         - RFAILED       - If the procedure was unsuccessful due to any failure.
 */
#ifdef ANSI
PRIVATE S16 czLmmSctSapCntrl
(
CzCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmSctSapCntrl(cntrl, status)
CzCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
   S16 ret=RFAILED;
   CzSctSapCb *sctSapCb=NULLP;

   CZ_TRC2(czSctSapCntrl);
  
   status->status = LCM_PRIM_NOK;
   status->reason = LCM_REASON_NOT_APPL;
   
   CZ_DBG_INFO((CZ_PRNT_BUF,"SctSap Control for SpId = %d\n", (cntrl->u.lSapTrc.lSapId)));
  switch (cntrl->subAction)
  { 

     case SATRC:
        {
           /* validates the sct sap Id  */
           if((status->reason = 
                    czLmmValidateSctSapId (cntrl->u.lSapTrc.lSapId)) !=
                 LCM_REASON_NOT_APPL)
           {
              CZ_DBG_ERROR((CZ_PRNT_BUF, "czSctSapCntrl, validation failed for the \
                       sapId(%d)",cntrl->u.lSapTrc.lSapId));
              break;
           }
           switch (cntrl->action)
           {

              case AENA:
                 {
                    CZ_DBG_INFO((CZ_PRNT_BUF,
                             "Enable the trace for sctSap sap Id(%d) \n",\
                             cntrl->u.lSapTrc.lSapId));

                    sctSapCb = czCb.sctSapLst[cntrl->u.lSapTrc.lSapId];
                    sctSapCb->trcLen = cntrl->u.lSapTrc.trcLen;
                    sctSapCb->trcMask |= cntrl->u.lSapTrc.trcMask;
                    status->status=LCM_PRIM_OK;
                    ret=ROK;
                    break; 
                 }
              case ADISIMM:
                 {
                    CZ_DBG_INFO((CZ_PRNT_BUF,
                             "Disable the trace for sctSap sap Id(%d) \n",\
                             cntrl->u.lSapTrc.lSapId));
                    /* Disable the trace for this sct sctSap  */
                    sctSapCb = czCb.sctSapLst[cntrl->u.lSapTrc.lSapId];
                    sctSapCb->trcLen = cntrl->u.lSapTrc.trcLen;
                    sctSapCb->trcMask &= ~(cntrl->u.lSapTrc.trcMask);
                    status->status=LCM_PRIM_OK;
                    ret=ROK;
                    break; 
                 } 
              default:
                 {
                    CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid Action for subaction SATRC"));
                    status->reason = LCM_REASON_INVALID_ACTION;
                    break;
                 }

           }
           break;
        }
     case SAELMNT:

        {
           /* validates the sct sap Id  */
           if((status->reason = czLmmValidateSctSapId (cntrl->u.sap.id))!= LCM_REASON_NOT_APPL)
           { 
              CZ_DBG_ERROR((CZ_PRNT_BUF, "validation failed for the \
                       sapId(%d)",cntrl->u.sap.id));
              break;
           }
           status->status=LCM_PRIM_OK;

           switch (cntrl->action)
           {
              case ABND_ENA:
                 {

                    CZ_DBG_INFO((CZ_PRNT_BUF,"Enabling (binding) SctSap\n"));
                    /* this fuction calls the sct bind request and open the end point 
                     * after receiving the Bind cfm */
                    if ((ret = czLmmEnaSctSap (cntrl->u.sap.id)) != ROK)
                    {
                       CZ_DBG_ERROR((CZ_PRNT_BUF, "czSctSapCntrl, Failed to bind and enable the SCT SAP"));
                       status->status=LCM_PRIM_NOK;
                       status->reason = LCM_REASON_LYR_SPECIFIC;
                    }

                    break; 
                 }
              case AUBND_DIS:
                 {
                    CZ_DBG_INFO((CZ_PRNT_BUF,"disabling (unbinding) SctSap\n"));
                    /* Ubind lower sap will terminate all the SCTP association related to this
                     * sap */
                    if ((ret = czLmmUbndSctSap (cntrl->u.sap.id,CZ_UBND_SAP)) != ROK)
                    {
                       CZ_DBG_ERROR((CZ_PRNT_BUF,
                                "czSctSapCntrl, Failed to Unbind and disable the SCT SAP"));
                       status->status=LCM_PRIM_NOK;
                       status->reason = LCM_REASON_LYR_SPECIFIC;
                    }
                    break;
                 }
              case ADEL:
                 {

                    /* call this fuction to delete the lower sap 
                     * which internally Remove all logical connections associated with this SCT Sap
                     * and deallocate the sctSap*/
                    if ((ret = czLmmDelSctSap (cntrl->u.sap.id, CZ_DEL_SAP)) != ROK)
                    {
                       CZ_DBG_ERROR((CZ_PRNT_BUF,"czSctSapCntrl, Couldn't delete the SCT SAP"));
                       status->status = LCM_PRIM_NOK;
                       status->reason = LCM_REASON_LYR_SPECIFIC;
                    }
                    break;
                 }
              default:
                 {
                    CZ_DBG_ERROR((CZ_PRNT_BUF,"czSctSapCntrl, Invalid Action for subaction SAELMNT"));
                    status->status = LCM_PRIM_NOK;
                    status->reason = LCM_REASON_INVALID_ACTION;
                    break;
                 }
           }
           break;
        }
     default:
        {
           CZ_DBG_ERROR((CZ_PRNT_BUF,"czSctSapCntrl, Invalid SubAction"));
           status->reason = LCM_REASON_INVALID_SUBACTION;
           break;
        }

  }
  
  
  CZ_RETVALUE(ret);

} /*End of  czLmmSctSapCntrl */

/**

@brief This function   process the peer control  reguest from Layer manager.

@details  
 The peer control can come when the layer manager
 wants to add another set of peers (or a single peer) other than 
 configured peers. If the list contains a configured peer, then X2AP 
 returns error and does not process the control request. While deleting, 
 layer manager can delete the peers one by one. 
 That means, if there are N number of peers, layer manager has to send N
 control request with elmId.elmnt set to STCZPEER. When X2AP receives a control
 request to delete a specific peer, it first sends SCTP terminate request
 and then deletes the peer after it receives a SCTP terminate confirm. 
  
Process the Peer control request
            First validates the peer control request.
            And process the control depending upon the action 
            Control Action 
             -# AENA :   To Enable the peer trace.
             -# ADISIMM: To Disable the peer trace.
             -# DEL:     To delete The peer.
            The Action Del will reomove and  terminate the Association and 
            removes the peer from the peer list also frees the peerCb from peer
            control block.
            
@param[in]  cntrl   Control structure for X2AP.
@param[out] status  Updated status for success/failure.
  
  @return
         - ROK           - If the Procedure was successful.
         - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmPeerCntrl
(
CzCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmPeerCntrl(cntrl, status)
CzCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
   S16 ret=RFAILED;
   CzPeerCb *peerCb=NULLP;

   CZ_TRC2(czPeerCntrl);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Peer Control Request.\n"));

   status->status = LCM_PRIM_NOK;
   status->reason = LCM_REASON_NOT_APPL;
   
 switch(cntrl->subAction)
 {
    case SATRC:
       {
          /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
          if((status->reason = czLmmValidatePeer(cntrl->u.pNodeTrc.peerId,&peerCb))!= 
                LCM_REASON_NOT_APPL)
          {
             CZ_DBG_ERROR((CZ_PRNT_BUF,"Peer Control Request failed. \
                      Validation failed.\n"));
             break;
          }
          CZ_PRNT_PEERID;

          switch (cntrl->action)
          {
             case AENA:
                {
                   CZ_DBG_INFO((CZ_PRNT_BUF,"Enabling Trace for Peer.\n"));
                   /* Enable the trace for Peer */
                   peerCb->peerCfg.trcLen = cntrl->u.pNodeTrc.trcLen;
                   peerCb->peerCfg.trcMask |= cntrl->u.pNodeTrc.trcMask;
                   status->status=LCM_PRIM_OK;
                   CZ_RETVALUE(ROK);
                }
             case ADISIMM:
                {
                   CZ_DBG_INFO((CZ_PRNT_BUF,"Disabling Trace for Peer.\n"));
                   /* Disable the trace for Peer */
                   peerCb->peerCfg.trcLen = cntrl->u.pNodeTrc.trcLen;
                   peerCb->peerCfg.trcMask &= ~(cntrl->u.pNodeTrc.trcMask);
                   status->status=LCM_PRIM_OK;
                   CZ_RETVALUE(ROK);
                }
             default:
                {
                   CZ_DBG_ERROR((CZ_PRNT_BUF,"czPeerCrntl, Invalid Action for subaction SATRC"));
                   status->reason = LCM_REASON_INVALID_ACTION;
                   break;
                }

                
          }
          break; /* SATRC */
       }
    case SAELMNT:
       {
          /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
          if((status->reason = czLmmValidatePeer(cntrl->u.peer.peerId,&peerCb))!= LCM_REASON_NOT_APPL)        
          {
             CZ_DBG_ERROR((CZ_PRNT_BUF,"Peer Control Request failed. \
                      Validation failed.\n"));
              break;
          }
          CZ_PRNT_PEERID;

          status->status=LCM_PRIM_OK;

          switch (cntrl->action)
          {
             case ADEL:
                {
                   /*Del will reomove and terminate the Association and
                     removes the peer from the peer list also frees the peerCb from peer
                     control block*/
                   if ((ret = czLmmDelPeer(peerCb)) != ROK)
                   {
                      if(ret == ROKPENDING)
                      {

                           status->status=LCM_PRIM_OK_NDONE;
                      }
                      else
                      {
                         CZ_DBG_ERROR((CZ_PRNT_BUF,
                               "czPeerCntrl, Couldn't delete the peer"));
                         status->status=LCM_PRIM_NOK;
                         status->reason = LCM_REASON_LYR_SPECIFIC;
                      }
                   }

                   break;    
                }      
             default:
                {
                   CZ_DBG_ERROR((CZ_PRNT_BUF,"czPeerCntrl, Invalid Action"));
                   status->status = LCM_PRIM_NOK;
                   status->reason = LCM_REASON_INVALID_ACTION;
                   break;
                }
          }/* swith close cntrl->action */
          break;
       }/* CASE SAELMNT */
    default:
       {
          CZ_DBG_ERROR((CZ_PRNT_BUF,"czPeerCntrl, Invalid SubAction"));
          status->reason = LCM_REASON_INVALID_SUBACTION;
          break;

       }
 } /* main switch */
  
 CZ_RETVALUE(ret);

} /* End of czLmmPeerCntrl */

/**
     @brief This function is used to Process the End Point  control request.

     @details This function is used to enable / disable an endpoint on a specific SAP using the sapid
              received in the request.An endpoint is alogical sender and receiver of SCTP datagrams. 
              It is represented to its peers by a unique set of transport addresses (IP address and SCTP port combinations)
              that share a common SCTP port and that do not overlap with the addresses of
              any other endpoint. Associations are established from one endpoint to another
              endpoint. An endpoint can have multiple associations to different endpoints
                This primitive is used to remove an endpoint on a specific SAP. The service
                provider uses an SbUiSctEndpCloseCfm primitive to confirm the successful
                closing of the endpoint. All the associations on this endpoint are terminated
                before the endpoint is closed.
              
              Action Supported
              -# AENA:      To open the end point.
              -# ADISIMM:   To close the end point.
      
     @param[in]  cntrl       The end point cotrol structure. 
     @param[out] status      Updated status for success/failure.
      
     @return 
         - ROK           - If the procedure was successful.
         - RFAILED       - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PRIVATE S16 czLmmEndPtCntrl
(
CzCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmEndPtCntrl(cntrl, status)
CzCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
   S16 ret=ROK;
   SuId  sapId;
   
  /* Variable to store the sct sap control block */
   CzSctSapCb *sctSapCb = NULLP;

   CZ_TRC2(czEndPtCntrl);
   
   CZ_DBG_INFO((CZ_PRNT_BUF,"End-Point Control Request.\n"));
  /*get the Sct sap id provided by Layer manager and validate */
   sapId = cntrl->u.endPtCntrl.tsapId;
 
   /* validate for the number of sctp lower sap 
  * Check if sapId is valid or not */
   if(sapId < 0 || sapId > czCb.genCfg.nmbSctSaps || (sctSapCb = czCb.sctSapLst[sapId]) == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"CzEndpContrlReq: Invalid sapId \n"));
      CZ_SEND_SAPID_ALARM(sapId, LCZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SAP);
      status->status = LCM_PRIM_NOK;
      status->reason =LCM_REASON_INVALID_PAR_VAL;
      CZ_RETVALUE(RFAILED);
   }

   /* If Sap is not bound then return invalid sap */
   if ((sctSapCb->state != LCZ_SAP_ISBOUND))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"CzEndpContrlReq:: Invalid sap \n"));
      CZ_SEND_SAPID_ALARM(sapId, LCZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_STATE);
      status->status = LCM_PRIM_NOK;
      status->reason =LCM_REASON_INVALID_STATE;
      CZ_RETVALUE(RFAILED);
   }


   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;


   switch (cntrl->action)
   {
      case AENA:
         {
            CZ_DBG_INFO((CZ_PRNT_BUF,"Enabling Endpoint.\n"));
            /* Check  the state of end point */
            if(sctSapCb->endpCb.state != CZ_ENDP_CFG)
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF,"Failed. Endpoint not configured.\n"));
               status->status = LCM_PRIM_NOK;
               status->reason =LCM_REASON_INVALID_STATE;
               CZ_RETVALUE(RFAILED);
            }     
            /* Since there is going to be only one end point, we can have
             * 0 as suEndpId, update the same in endpCb */

            CZ_DBG_INFO((CZ_PRNT_BUF,"Invoking SCTP Enpoint Open Request.\n"));
            /* Send end point open request to Tpm  */
#ifdef SCT_ENDP_MULTI_IPADDR
            CzLiSctEndpOpenReq(&sctSapCb->pst, sctSapCb->spId, cntrl->u.endPtCntrl.endPtId,
                  sctSapCb->endpCb.port,
                  (SctNetAddrLst*)&(sctSapCb->endpCb.srcAddrLst));
#else
            CzLiSctEndpOpenReq(&sctSapCb->pst, sctSapCb->spId, cntrl->u.endPtCntrl.endPtId,
                  sctSapCb->endpCb.port,
                  &sctSapCb->endpCb.srcAddrLst.nAddr[0]);
#endif /* SCT_ENDP_MULTI_IPADDR */

            /* Change the state of endpoint to OPEN_IN_PRG */
            /*sctSapCb->endpCb.state = CZ_ENDP_OPEN_IN_PRG;*/
            break;
         }
      case ADISIMM:
         {
            CZ_DBG_INFO((CZ_PRNT_BUF,"Disabling Endpoint.\n"));
            /* Close the endpoint */
            if ((sctSapCb->endpCb.state == CZ_ENDP_OPEN))
            {

            CZ_DBG_INFO((CZ_PRNT_BUF,"Invoking SCTP Enpoint Close Request.\n"));
            /* Send end point open request to Tpm  */
               CzLiSctEndpCloseReq(&sctSapCb->pst, sctSapCb->spId,
                     sctSapCb->endpCb.locSpEndpId, SCT_ENDPID_SP);
               czNmmDeallocAllPeerCb(sctSapCb->spId,CZ_UBND_SAP);
            }
            else
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF,"Failed. Endpoint not open.\n"));
               status->status = LCM_PRIM_NOK;
               status->reason =LCM_REASON_INVALID_STATE;
               CZ_RETVALUE(RFAILED);

            }
            break;
         }
      default:
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,"czEndPtCntrl, Invalid Action"));
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_ACTION;
            ret=RFAILED;

            break;
         }
   }

   CZ_RETVALUE(ret);

} /* End of czLmmEndPtCntrl */


/**

  @brief This function is used to control the SCTP association 
  related fuctionality.

  @details This function is used to change the primary destination IP address.
           this function modifies the current primary destination address for 
           the specified association .The control request has the peerId and
           the primary Ip addres to be set. The new primary address in copied
           into peer Cfg and used later in SetPriCfm.
           This will call the sctp layer to set the primary address.
           Once completed, it responds with an SbUiSctSetPriCfm.

     @param[in]  cntrl       The assoc cotrol structure. 
     @param[out] status      Updated status for success/failure.
     
  @return
         - ROK           - If the procedure was successful.
         - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmAssocCntrl
(
CzCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmAssocCntrl(cntrl, status)
CzCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
 S16 ret=RFAILED;
 /* Variable to store the sct sap control block */
 CzSctSapCb *sctSapCb = NULLP;
 CmNetAddr  *dstAddr=NULLP;
 CzPeerCb   *peerCb=NULLP;
      
  CZ_TRC2(czAssocCntrl);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Assoc Control Request.\n"));
   status->status=LCM_PRIM_NOK;
   
    if((status->reason= 
             czLmmValidatePeer(cntrl->u.modPriAddrCntrl.peerId,&peerCb))!=
          LCM_REASON_NOT_APPL)
    {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Failed. Peer validation failed.\n"));
      CZ_RETVALUE(ret);    
    }
    CZ_PRNT_PEERID;

   sctSapCb=peerCb->sctSapCb; 
   if(sctSapCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
          "there is no sct sap associated with this  peer\n"));
      status->reason=LCM_REASON_INVALID_SAP;
      CZ_RETVALUE(ret);
  }   
   
 if ((sctSapCb->state != LCZ_SAP_ISBOUND) || 
   (sctSapCb->endpCb.state != CZ_ENDP_OPEN) || 
   (peerCb->assocCb->assocState != CZ_ASSOC_UP))
 {
  CZ_DBG_ERROR((CZ_PRNT_BUF,
    "czSetPrimaryAddr: Invalid sap state or endp is not open \
    or association is not UP \n"));
  status->reason = LCM_REASON_INVALID_STATE;
  CZ_RETVALUE(ret);
 } 

 status->status = LCM_PRIM_OK;
 status->reason = LCM_REASON_NOT_APPL;


 switch (cntrl->action)
 {
  case AMODIFY:
     {
        CZ_DBG_INFO((CZ_PRNT_BUF,"Modify Peer Control Request.\n"));
        dstAddr = &cntrl->u.modPriAddrCntrl.priDstAddr;

        if(dstAddr->type != CM_NETADDR_IPV4 && dstAddr->type != CM_NETADDR_IPV6)
        {
           CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid destination address"));
           status->status    = LCM_PRIM_NOK;
           status->reason = LCM_REASON_INVALID_PAR_VAL;
           break;
        }
        /* Keep the new address temporarily in peerCfg */
        cmMemcpy ((U8 *)&peerCb->peerCfg.priDstAddr, (CONSTANT U8 *)dstAddr, 
              sizeof(CmNetAddr));
        CZ_DBG_INFO((CZ_PRNT_BUF,"Invoking SCTP to set primary address.\n"));
        ret = CzLiSctSetPriReq(&(sctSapCb->pst),
              sctSapCb->spId,
              peerCb->assocCb->spAssocId,
              dstAddr);
        if(ret == ROK)
        {
           CZ_DBG_INFO((CZ_PRNT_BUF,"SCTP set primary address done.\n"));
           status->status = LCM_PRIM_OK;
           status->reason = LCM_REASON_NOT_APPL;
        }
        else
        {
           CZ_DBG_ERROR((CZ_PRNT_BUF,"SCTP set primary address failed.\n"));
           /* revert the new address temporarily in peerCfg to the old one */
           cmMemcpy ((U8 *)&peerCb->peerCfg.priDstAddr, \
                 (U8*)&peerCb->assocCb->assocCfg->priDstAddr,
                 sizeof(CmNetAddr));
           status->status = LCM_PRIM_NOK;
           status->reason = LCM_REASON_INVALID_ACTION;
        }
        break;
     }

  default:
   {
   status->status = LCM_PRIM_NOK;
   status->reason = LCM_REASON_INVALID_ACTION;
     break;
   }
 }

 CZ_RETVALUE(ret);

} /* czLmmAssocCntrl */






/**
 @brief X2AP statistics request primitive.
 @details 
  The layer manager gathers statistics information about the elements of X2AP 
  using the management statistics procedure initiated by the layer manager. 
  The X2AP statistics request primitive @ref CzMiLczStsReq can be called more 
  than once and at any time after the management configuration procedure.
  The statistics values are returned by the CzMiLczStsCfm primitive. 
  The statistics counters can be reset using the action parameter. If the
  statistics counters are not explicitly reset, they can be reset when the
  count exceeds the size of the data structure used to store the statistics
  (currently 2^32 - 1). These X2AP statistics request primitive types are
  called peer node statistics. They provide the count of each kind
  of message transmitted or received from the peer node. They also provide 
  the count of unexpected or error messages received and the number of times 
  each kind of timer expired when executing the procedures with the peer node.

@param[in] pst    Pointer to the pst structure.
@param[in] action This defines action by the layer after reading the statistics. 
@param[in] sts    Pointer to management structure. 

 @return
  - ROK           - If the procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PUBLIC S16 CzMiLczStsReq
(
Pst     *pst,                
Action  action,              
CzMngmt *sts                 /* Statistics structure */
)
#else
PUBLIC S16 CzMiLczStsReq(pst, action, sts)
Pst     *pst;                
Action  action;              
CzMngmt *sts;                /* Statistics structure */
#endif
{
   S16 ret = ROK;            /* Return value */  
   CzMngmt *stsCfm;           /* Statistics confirm */

   CZ_TRC3(CzMiLczStsReq);
   stsCfm = NULLP;
  

#ifdef SS_MULTIPLE_PROCS
   if( ((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                 != ROK) || (czCbPtr == NULLP) )
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT145, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------CzMiLczStsReq-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /*SS_MULTIPLE_PROCS */

   CZ_DBG_INFO((CZ_PRNT_BUF,
          "Status Request (pst, action(%d), elmId (%d)) \n", 
           action, sts->hdr.elmId.elmnt));

   /* memset the config Cfm structure */
   CZ_ALLOC(stsCfm, sizeof (CzMngmt));
   if(stsCfm == NULLP)
   {
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate stsCfm.\n"));
      CZ_RETVALUE(RFAILED);
   }
   /* Check if General Configuration is done */
   if (!czCb.init.cfgDone) 
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
                 "Status Request: General configuration not done "));

      stsCfm->cfm.status = LCM_PRIM_NOK;
      stsCfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      czLmmSendLmCfm (pst, &sts->hdr, TSTS, stsCfm);
      CZ_FREE(sts, sizeof(CzMngmt));

      CZ_RETVALUE(ROK);
   }

   /* check element */
   switch (sts->hdr.elmId.elmnt)
   {
      case STCZGEN:              /* General statistics */
      {
         CZ_DBG_INFO((CZ_PRNT_BUF,"Getting General Statistics.\n"));
         ret = czLmmGetGenSts (&stsCfm->t.sts.u.genSts, &stsCfm->cfm, action);
         break;
      }
      case STCZPEER:            /* Peer node statistics */
      {
         CZ_DBG_INFO((CZ_PRNT_BUF,"Getting Peer Statistics.\n"));
         cmMemcpy ((U8 *)&stsCfm->t.sts.u.nodeSts, (U8 *)&sts->t.sts.u.nodeSts,
               sizeof(sts->t.sts.u.nodeSts));
         /* memset the config Cfm structure */
         CZ_SET_ZERO(&stsCfm->t.sts.u.nodeSts.cfm.pdu, sizeof (CzPduSts));

         /* memset the config Cfm structure */
         CZ_SET_ZERO(&stsCfm->t.sts.u.nodeSts.cfm, sizeof (stsCfm->t.sts.u.nodeSts.cfm));
         ret = czLmmGetPeerNodeSts (&stsCfm->t.sts.u.nodeSts, &stsCfm->cfm, action);
         if (ret == RFAILED)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,
                       "Failed to get Peer node statistics"));
         }
         break;
      }
      default:
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,
                    "Invalid element in statistics request"));

         stsCfm->cfm.status = LCM_PRIM_NOK;
         stsCfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } 

   CZ_DBG_INFO((CZ_PRNT_BUF,"Sending Status Confirm\n"));
   /* Issue a statistics confirm */
   czLmmSendLmCfm(pst, &sts->hdr, TSTS, stsCfm);
      
   CZ_FREE(sts, sizeof(CzMngmt));

   CZ_RETVALUE(ret);

} /* CzMiLczStsReq */

/**
  @brief This function gathers the general statistics 

  @details
    This fuction is to get the general statistics updated by the 
    X2AP layer to the Layer manager.And also reset the statistics
    if the action is ZEROSTS.
 
   @param[in] sts       Layer manager general statistics structure
   @param[in] status    Return the status of the request.
   @param[in] action    Action
   @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCM_REASON_INVALID_PAR_VAL
 */

#ifdef ANSI
PRIVATE S16 czLmmGetGenSts
(
CzGenSts *sts,              /* general statistics structure */
CmStatus *status,           /* Return status */
Action action               /* Action */
)
#else
PRIVATE S16 czLmmGetGenSts(sts, status, action)
CzGenSts *sts;              /* general statistics structure */
CmStatus *status;           /* Return status */
Action action;              /* Action */
#endif
{
   S16 ret;

   CZ_TRC2(czGetGenSts);

   ret = czLmmValidateGenSts (sts, action);

   /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
   if (ret != LCM_REASON_NOT_APPL)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Validation of general statistics fialed.\n"));
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      CZ_RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   cmMemcpy ((U8 *)sts, (U8 *)&czCb.genSts, sizeof (CzGenSts));
   if ((action == ZEROSTS))
   {
      CZ_SET_ZERO(&czCb.genSts, sizeof (CzGenSts));
   }

   CZ_RETVALUE(ROK);

} /* End of czLmmGetGenSts */

/**
 
   @brief This function is used to get the peer statistics.

   @details  Gather the peer node statistics for the list of peers.

   @param[in] sts       Layer manager peer statistics structure
   @param[in] status    Return the status of the request.
   @param[in] action    Action
   
   @return
          - ROK           - If the Procedure was successful.
          - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmGetPeerNodeSts
(
CzPeerNodeSts *sts,         /* Node statistics structure */
CmStatus *status,           /* Return status */
Action action               /* Action */
)
#else
PRIVATE S16 czLmmGetPeerNodeSts(sts, status, action)
CzPeerNodeSts *sts;         /* Node statistics structure */
CmStatus *status;           /* Return status */
Action action;              /* Action */
#endif
{
   S16 ret;

   CzPeerCb *peerCb = NULLP;

   CZ_TRC2(czGetPeerNodeSts);

   ret = czLmmValidatePNodeSts (sts, action);

   /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
   if ((ret != LCM_REASON_NOT_APPL))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"Validation of Peer Node Statistics failed.\n"));
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      CZ_RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   czNmmFndPeer(sts->peerId,&peerCb);

   if(peerCb== NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
                 "czGetPeerNodeSts: peerCb is NULLP"));
      CZ_RETVALUE(RFAILED);
   }
   CZ_PRNT_PEERID;

   cmMemcpy((U8 *)&sts->cfm, (U8 *)&peerCb->peerCfg.peerSts.cfm, sizeof (peerCb->peerCfg.peerSts.cfm));
   if ((action == ZEROSTS))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"Resetting Peer Statistics.\n"));
      CZ_SET_ZERO(&peerCb->peerCfg.peerSts.cfm, sizeof (peerCb->peerCfg.peerSts.cfm));
   }

   CZ_RETVALUE(ROK); 

} /* czLmmGetPeerNodeSts */



/**
 @brief X2AP solicited status request primitive.
  
  @details 
   The layer manager gathers solicited status information about the various 
   elements of X2AP by using the management solicited status procedure, which 
   the layer manager initiates. The X2AP status request primitive 
   can be called more than once and at any time after the management 
   configuration procedure.
  
   @param[in] pst Pointer to the post structure.
   @param[in] sta Pointer to the management structure. 

   @return
          - ROK           - If the procedure was successful.
          - RFAILED       - If the procedure was unsuccessful due to any failure.

*/

#ifdef ANSI
PUBLIC S16 CzMiLczStaReq
(
Pst     *pst,                
CzMngmt *sta                 /* Status structure */
)
#else
PUBLIC S16 CzMiLczStaReq(pst, sta)
Pst     *pst;                
CzMngmt *sta;                /* Status structure */
#endif
{
   S16 ret = ROK;            /* Return value */
   CzMngmt *staCfm;           /* Status confirm */

   CZ_TRC3(CzMiLczStaReq)
   staCfm = NULLP;


#ifdef SS_MULTIPLE_PROCS
   if( ((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                 != ROK) || (czCbPtr == NULLP) )
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT146, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,
         "--------Status Request-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /*SS_MULTIPLE_PROCS */

   CZ_DBG_INFO((CZ_PRNT_BUF,
          "Status Request (pst, elmId (%d)) \n", sta->hdr.elmId.elmnt));
   /* memset the config Cfm structure */
   CZ_ALLOC(staCfm, sizeof (CzMngmt));
   if(staCfm == NULLP)
   {
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate staCfm.\n"));
      CZ_RETVALUE(RFAILED);
   }
   /* Check if General Configuration is done */
   if ((!czCb.init.cfgDone))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
                 "Status Request: General configuration not done "));

      staCfm->cfm.status = LCM_PRIM_NOK;
      staCfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      czLmmSendLmCfm (pst, &sta->hdr, TSSTA, staCfm);

      CZ_FREE(sta, sizeof(CzMngmt));

      CZ_RETVALUE(ROK);
   }

   switch (sta->hdr.elmId.elmnt)
   {
      case STCZSID:
      {
         CZ_DBG_INFO((CZ_PRNT_BUF,"Getting SID information.\n"));
         staCfm->cfm.status = LCM_PRIM_OK;
         staCfm->cfm.reason = LCM_REASON_NOT_APPL;
         CZ_FREE(sta->t.ssta.s.sysId.ptNmb, LCZ_MAX_PT_NUM_SIZE);
         SGetSBuf(czCb.init.region, czCb.init.pool, (Data **)&staCfm->t.ssta.s.sysId.ptNmb,
                                            LCZ_MAX_PT_NUM_SIZE);
         (Void) czGetSId (&staCfm->t.ssta.s.sysId);
         break;
      }
      case STCZGEN:
      {
         ret = czLmmGetGenSta (&staCfm->t.ssta.s.genSta, &staCfm->cfm);
         break;
      }
      case STCZCZTSAP:
      {
         cmMemcpy ((U8 *)&staCfm->t.ssta.s.sSapSta, (U8 *)&sta->t.ssta.s.sSapSta,
               sizeof (CzSSapSta));
         ret = czLmmGetCztSapSta (&staCfm->t.ssta.s.sSapSta, &staCfm->cfm);
         break;
      }
      case STCZSCTSAP:
      {
         cmMemcpy ((U8 *)&staCfm->t.ssta.s.tSapSta, (U8 *)&sta->t.ssta.s.tSapSta,
               sizeof (CzTSapSta));
         ret = czLmmGetSctSapSta (&staCfm->t.ssta.s.tSapSta, &staCfm->cfm);
         break;
      }
      case STCZPEER:
      {
         cmMemcpy ((U8 *)&staCfm->t.ssta.s.peerSta, (U8 *)&sta->t.ssta.s.peerSta,
               sizeof (CzPeerSta));
         ret = czLmmGetPeerSta (&staCfm->t.ssta.s.peerSta, &staCfm->cfm);
         break;
      }
      default:
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,
                    "CzMiLczStaReq: Invalid element in Status request"));

         staCfm->cfm.status = LCM_PRIM_NOK;
         staCfm->cfm.reason = LCM_REASON_INVALID_ELMNT;
         ret=RFAILED;
         break;
      }

   } /* end switch */

   czLmmSendLmCfm(pst, &sta->hdr, TSSTA, staCfm);

   CZ_FREE(sta, sizeof(CzMngmt));

   CZ_RETVALUE(ret);

} /* End of CzMiLczStaReq */

/**

   @brief This function is used to get the genral statistics for x2ap layer.

   @details  Gather the general X2AP's resources.
   @param[in]  sta     - Genral statistics.
   @param[out] status  - Return the status of the request.
   
   @return
          - ROK           - If the Procedure was successful.
          - RFAILED       - If the procedure was unsuccessful due to any failure.

*/

#ifdef ANSI
PRIVATE S16 czLmmGetGenSta
(
CzGenSta *sta,         /* General status */
CmStatus *status       /* Return status */
)     
#else    
PRIVATE S16 czLmmGetGenSta(sta, status)
CzGenSta *sta;         /* General status */
CmStatus *status;      /* Return status */
#endif
{        
   CZ_TRC2(czGetGenSta);
      
   CZ_DBG_INFO((CZ_PRNT_BUF,"Getting General Status.\n"));
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   cmMemcpy ((U8 *)sta, (U8 *)&czCb.genSta, sizeof (CzGenSta));

   sta->tRELOCprep= czCb.nodeCb.protCfg.tRELOCprep;
   sta->tX2RELOCOverall= czCb.nodeCb.protCfg.tX2RELOCOverall;
   sta->timeToWait=czCb.nodeCb.protCfg.timeToWait;

   CZ_RETVALUE(ROK);

} /* End of czLmmGetGenSta */

/**

   @brief This function is used to get the upper sap status.

   @details  Gather the general X2AP's Upper Sap status
   @param[in] sta      - Upper sap status
   @param[out] status  - Upper sap status
   
   @return
          - ROK           - If the Procedure was successful.
          - RFAILED       - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PRIVATE S16 czLmmGetCztSapSta
(
CzSSapSta *sta,        
CmStatus *status       
)
#else
PRIVATE S16 czLmmGetCztSapSta(sta, status)
CzSSapSta *sta;       
CmStatus *status;     
#endif
{

   CZ_TRC2(czGetCztSapSta);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Getting CZT SAP Status.\n"));
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;
   
   if((status->reason = czLmmValidateCztSapId (sta->sSapId))!=LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      CZ_RETVALUE(RFAILED);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF,"CZT SAP spId=%d.\n", sta->sSapId));
   /* get the Upper SapCb state from The Upper sap Lst  */  
   sta->status = czCb.cztSapLst[sta->sSapId]->state;
   CZ_RETVALUE(ROK);
} /* end of czLmmGetCztSapSta */

/**

     @brief This function is used to get the lower sap status.

     @details  Gather the X2AP's lower SAP status.
     @param[in] sta      - upper sap status
     @param[out] status  - Upper sap status

     @return
            - ROK           - If the Procedure was successful.
            - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmGetSctSapSta
(
CzTSapSta *sta,         /* lower sap status */
CmStatus *status       /* Return status */
)
#else
PRIVATE S16 czLmmGetSctSapSta(sta, status)
CzTSapSta *sta;         /* lower sap status */
CmStatus *status;      /* Return status */
#endif
{
   S16 ret;

   CZ_TRC2(czGetSctSapSta);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Getting SCT SAP Status.\n"));
   ret = czLmmValidateSctSapId (sta->tSapId);

   /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
   if ((ret != LCM_REASON_NOT_APPL))
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      CZ_RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   sta->status = czCb.sctSapLst[sta->tSapId]->state;

   CZ_DBG_INFO((CZ_PRNT_BUF,"SCT SAP spId=%d.\n", sta->tSapId));
   CZ_RETVALUE(ROK);

} /* End of czLmmGetSctSapSta */

/**
     @brief This function is used to return the peer status..

     @details This function is used to return the peer status. 
      
     @param[in]  sta       The Peer status  structure. 
     @param[out] status  - status of the request
      
     @return 
         - ROK           - If the Procedure was successful.
         - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czLmmGetPeerSta
(
CzPeerSta *sta,         /* Peer status */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 czLmmGetPeerSta(sta, status)
CzPeerSta *sta;         /* Peer status */
CmStatus *status;       /* Return status */  
#endif
{
   CzPeerCb *peerCb = NULLP;
   U32 loopCnt;

   CZ_TRC2(czGetPeerSta);   

   CZ_DBG_INFO((CZ_PRNT_BUF,"Getting Peer Status.\n"));
   /* check if validation failed return cfm with the status = LCM_PRIM_NOK */
  if((status->reason= czLmmValidatePeerSta(sta)) != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      CZ_RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* loop through all the peers spacified in the request and fill the info */ 
   for (loopCnt = 0; loopCnt < sta->nmbPeer; loopCnt++)  
   {

     /* get the peer control blobk by passing peer Id */ 
     czNmmFndPeer(sta->peerStaInfo[loopCnt].peerId,&peerCb);

      /* check for valid peer Cb control block */     
      if (peerCb != NULLP) 
      {
         CZ_PRNT_PEERID;

         CZ_DBG_INFO((CZ_PRNT_BUF,"Filling Peer status.\n"));
         /* copy the association state */
         sta->peerStaInfo[loopCnt].assocState = peerCb->assocCb->assocState;

         /* copy the sct sap state */
         sta->peerStaInfo[loopCnt].tsapState = peerCb->sctSapCb->state;
         
         /*copy the service user suAssocId   */
         sta->peerStaInfo[loopCnt].spAssocId = peerCb->assocCb->spAssocId;

         /*copy the service provider  suAssocId */
         sta->peerStaInfo[loopCnt].suAssocId = peerCb->assocCb->suAssocId;

         /*copy the upper sap Id to which this peer belongs */
         sta->peerStaInfo[loopCnt].sapId = peerCb->cztSapCb->suId;

         /*TODO sta->peerStaInfo[loopCnt].peerENbId = peerCb->cztSapCb.suId;*/

      } /* end of (peerCb != NULLP) */
      else
      {
#ifdef ALIGN_64BIT
         CZ_DBG_ERROR((CZ_PRNT_BUF, " Invalid peerId(%d)",
                       sta->peerStaInfo[loopCnt].peerId));
#else         
         CZ_DBG_ERROR((CZ_PRNT_BUF, " Invalid peerId(%ld)",
                       sta->peerStaInfo[loopCnt].peerId));
#endif
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         CZ_RETVALUE(RFAILED);
      }

   } /* end of for loop */

   /* finally return ROK to the caller */
   CZ_RETVALUE(ROK);

} /* End of czLmmGetPeerSta */



/**
       @brief This function is used to sen the alaram to layer manager.
 
       @details  This function is used to send an alarm / unsolicited status
              indication to the layer manager.
              
       @param[in] category      - Category of the alarm.
       @param[in] event         - event generated.
       @param[in] cause         - Cause for the alarm.
       @param[in] dgn           - Diagnostics .
       
       @return
              - ROK           - If the Procedure was successful.
              - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC Void czLmmSendLmAlarm
(
U16 category,           /* category */
U16 event,              /* event generated */
U16 cause,              /* cause */
CzUstaDgn *dgn          /* Diagnostics */
)
#else
PUBLIC Void czLmmSendLmAlarm(category, event, cause, dgn)
U16 category;           /* category */
U16 event;              /* event generated */
U16 cause;              /* cause */
CzUstaDgn *dgn;         /* Diagnostics */
#endif
{
   CzMngmt *usta;

   CZ_TRC2(czLmmSendLmAlarm);

   usta = NULLP;
   CZ_DBG_INFO((CZ_PRNT_BUF,"Sending Alarm...\n"));
   /* Check if general configuration is done */
   if (czCb.init.cfgDone == FALSE)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "Status Indication requested; general configuration not done."));
      RETVOID;
   }

   if (czCb.init.usta == FALSE)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"Unsolicited Status (Alarm) disabled\n"));
      RETVOID;
   }

   /* initialize the management structure */
   CZ_ALLOC(usta,sizeof(CzMngmt));
   if(usta == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate usta.\n"));
      RETVOID;
   }

   usta->hdr.elmId.elmnt = STCZGEN;
   usta->hdr.entId.ent = czCb.init.ent;
   usta->hdr.entId.inst = czCb.init.inst;

   usta->t.usta.alarm.category = category;
   usta->t.usta.alarm.event = event;
   usta->t.usta.alarm.cause = cause;

   /* copy the usta structure */
   cmMemcpy((U8 *)&usta->t.usta.dgn, (U8 *)dgn, (PTR)sizeof(CzUstaDgn));

   /* update the date and time */
   (Void) SGetDateTime(&usta->t.usta.alarm.dt);

   CzMiLczStaInd(&czCb.init.lmPst, usta);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Alarm Sent.\n"));

   RETVOID;

} /* end of czLmmSendLmAlarm */


/**

     @brief This function to sen the trcces of the Pdus recvied/sent to Layer manager.

     @details  This function is used to send Trace Indication to
            the Layer Manager. 

            
     @param[in] trcLen         - Trace length.
     @param[in] trcParam       - point to trace data.
     @param[in] mBuf           - Raw message buffer.
     
     @return
            - ROK           - If the Procedure was successful.
            - RFAILED       - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PUBLIC S16 czLmmSendLmTrc
(
S32 trcLen,          /* Trace length */
CzTrc *trcParam,     /* trace data */
Buffer *mBuf         /* message buffer */
)
#else
PUBLIC S16 czLmmSendLmTrc(trcLen, trcParam, mBuf)
S32 trcLen;          /* Trace length */
CzTrc *trcParam;     /* trace data */
Buffer *mBuf;        /* message buffer */
#endif
{

   S16 ret;
   /*window compilation fix chaged back */
   MsgLen ml = 0;
   CzMngmt *trc;
   Buffer *trcBuf = NULLP;

   CZ_TRC2(czSendLmTrc);
   trc = NULLP;

   CZ_DBG_INFO((CZ_PRNT_BUF,"Sending Trace...\n"));
   /* Check if general configuration is done */
   if (czCb.init.cfgDone == FALSE)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "Trace Indication requested; general configuration not done."));
      CZ_RETVALUE(ROK);
   }

   if (trcLen == 0)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"Trace disabled\n"));
      CZ_RETVALUE(ROK);
   }

   /* initialize the management structure */
   CZ_ALLOC(trc, sizeof(CzMngmt));
   if(trc == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,"failed, to allocate trc.\n"));
      CZ_RETVALUE(RFAILED);
   }

   /* copy all the trace parameters into Layer manager trc */
   cmMemcpy((U8 *)&trc->t.trc, (U8 *)trcParam, sizeof(CzTrc));
 
   /*Initialize the trc Buffer */
   trcBuf = NULLP;

   if (mBuf != NULLP)
   {
      Buffer *tmpBuf = NULLP;

      (Void)SFndLenMsg(mBuf, &ml);
      if(ml == 0)
      {
         CZ_RETVALUE(ROK);
      }
      if ((trcLen > 0 ) &&  (trcLen < ml))
      {
         ml = trcLen;
      }

      if((ret = SCpyMsgMsg(mBuf, czCb.init.lmPst.region, 
                  czCb.init.lmPst.pool, &trcBuf)) != ROK)
      {
         /* Error message will be send from the SSI fn */
         CZ_DBG_ERROR((CZ_PRNT_BUF, "SCpyMsgMsg() failed."));
         (Void)SPutMsg(trcBuf);
         CZ_RETVALUE(RFAILED);
      }

      if((ret = SSegMsg(trcBuf, ml, &tmpBuf)) != ROK)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "SSegMsg() failed."));
         (Void)SPutMsg(trcBuf);
         CZ_RETVALUE(RFAILED);
      }
      /* free the temporary buffer */
      (Void)SPutMsg(tmpBuf);
   }

   trc->hdr.elmId.elmnt = STCZGEN;
   SGetDateTime(&trc->t.trc.dt);

   CzMiLczTrcInd(&czCb.init.lmPst, trc, trcBuf);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Trace Sent.\n"));

   /* cz001.301 : removed the freeing of the trc pointer,
    * Because we are freeing this inside CzMiLczTrcInd */

   CZ_RETVALUE(ROK);

} /* end of czLmmSendLmTrc */


/**
 
  @brief This function sends confirmation message to Layer manager Called by CzMiLCzCfgReq function
   @param[in] pst         Post structure
   @param[in] hdr         Message header
   @param[in] type        Type of message
   @param[in] cfm         X2AP Layer manager structure 
   @return
        - ROK           - If the Procedure was successful.
        - RFAILED       - If the procedure was unsuccessful due to any failure.
 */

#ifdef ANSI
PRIVATE S16 czLmmSendLmCfm
(
Pst *pst,                           
Header *hdr,                           /* Pointer to the Header structure */
U8 type,                               /* Confirmation Type */
CzMngmt *cfm                           /* Management Structure */
)
#else
PRIVATE S16 czLmmSendLmCfm(pst, hdr, type, cfm)
Pst *pst;
Header *hdr;                           /* Pointer to the Header structure */
U8 type;                               /* Confirmation Type */
CzMngmt *cfm;                          /* Management Structure */
#endif
{

   Pst cfmPst;                         /* Confirmation Post Structure */

   CZ_TRC2(czSendLmCfm);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Sending LM confirm\n"));
   /* Fill up the header in the confirm structure */
   CZ_SET_ZERO(&cfm->hdr,sizeof(cfm->hdr));       
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent = czCb.init.ent;
   cfm->hdr.entId.inst = czCb.init.inst;
   cfm->hdr.transId = hdr->transId;

   /* Fill up the post struct for comfirm */
   cfmPst.srcEnt = czCb.init.ent;
   cfmPst.srcInst = czCb.init.inst;
#ifdef SS_MULTIPLE_PROCS
   cfmPst.srcProcId = czCb.init.procId;
#else
   cfmPst.srcProcId = SFndProcId();
#endif
   cfmPst.dstEnt = pst->srcEnt;
   cfmPst.dstInst = pst->srcInst;
   cfmPst.dstProcId = pst->srcProcId;
   cfmPst.selector = hdr->response.selector;
   cfmPst.prior = hdr->response.prior;
   cfmPst.route = hdr->response.route;
   cfmPst.region = hdr->response.mem.region;
   cfmPst.pool= hdr->response.mem.pool;
 
   /* Send the corresponding confirm */
   switch (type)
   {
      case TCFG:
         CZ_DBG_INFO((CZ_PRNT_BUF,
               "Sending Configuration Confirm (cfmType: %d) \n", type));
         /* Send Configuration Confirm */
         CzMiLczCfgCfm (&cfmPst, cfm);
         break;

      case TCNTRL:
         CZ_DBG_INFO((CZ_PRNT_BUF,
               "Sending Control Confirm (cfmType: %d) \n", type));
         /* Send Control Confirm */
         CzMiLczCntrlCfm (&cfmPst, cfm);
         break;

      case TSTS:
         CZ_DBG_INFO((CZ_PRNT_BUF,
               "Sending Statistics Confirm (cfmType: %d) \n", type));
         /* Send Statistics Confirm */
         CzMiLczStsCfm (&cfmPst, cfm);
         break;

      case TSSTA:
         CZ_DBG_INFO((CZ_PRNT_BUF,
         /* Send Status Confirm */
               "Sending Status Confirm (cfmType: %d) \n", type));
         CzMiLczStaCfm (&cfmPst, cfm);
         break;

      default:
        
         CZ_DBG_INFO((CZ_PRNT_BUF, "czSendLmCfm : Invalid Type \n"));
         break;
   }

   CZ_RETVALUE(ROK); 

} /* czLmmSendLmCfm */



/**
  @brief This function validates the X2AP general configuration.
 
   @param[in] gen    - X2AP Layer manager general config structure.

   @return
       -# Success : LCM_REASON_NOT_APPL 
       -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 czLmmValidateGenCfg
(
CzGenCfg *gen        /* General configuration */
)
#else
PRIVATE S16 czLmmValidateGenCfg(gen)
CzGenCfg *gen;       /* General configuration */
#endif
{
   CZ_TRC2(czValidateGenCfg);
   
   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating General Configuration.\n"));
 /* validate for the number of sctp lower sap */
   if ((gen->nmbSctSaps <= 0))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid lower SAP.\n"));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

 /* validate for the number of czt upper sap */
   if ((gen->nmbCztSaps <= 0))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid number of nmbCztSaps(%d)\n",gen->nmbCztSaps));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

 /* validate for the Maximum number of peers */
   if ((gen->maxNmbPeer == 0))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid number of maxNmbPeer(%d)\n",
                    gen->maxNmbPeer));
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid number of maxNmbPeer(%ld)\n",
                    gen->maxNmbPeer));
#endif
      
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }


 /* validate the timer resolution  */
   if ((gen->timeRes <= 0))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid gen->timeRes (%d)\n",gen->timeRes));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }


   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* czLmmValidateGenCfg */


/**
  @brief This function validates the X2AP CZT upper sap.
 
   @param[in] cztSap  - X2AP Layer manager czt Sap structure
   @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCZ_REASON_PROTCFG_NOT_DONE 
                    LCM_REASON_EXCEED_CONF_VAL 
        
 */
#ifdef ANSI
PRIVATE S16 czLmmValidateCztSapCfg
(
CzCztSapCfg *cztSap      /* Upper SAP configuration */
)
#else
PRIVATE S16 czLmmValidateCztSapCfg(cztSap)
CzCztSapCfg *cztSap;     /* Upper SAP configuration */
#endif
{
   CZ_TRC2(czValidateCztSapCfg);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating CZT SAP Configuration.\n"));
   if ((czCb.nodeCb.cfgDone != TRUE))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"General Configuration not done.\n"));
      CZ_RETVALUE(LCZ_REASON_PROTCFG_NOT_DONE);
   }

   if ( (cztSap->spId <  0) || (cztSap->spId >= czCb.genCfg.nmbCztSaps) )
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"spId out of range.\n"));
      CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
   }

   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* czLmmValidateCztSapCfg */



/**
  @brief This function validates the X2AP lower sap.
 
   @param[in] sctSap  X2AP layer manager sctp sap structure.
   
   @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCZ_REASON_PROTCFG_NOT_DONE 
                    LCM_REASON_EXCEED_CONF_VAL
                    LCM_REASON_INVALID_PAR_VAL
                    
        
 */
#ifdef ANSI
PRIVATE S16 czLmmValidateSctSapCfg
(
CzSctSapCfg *sctSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 czLmmValidateSctSapCfg(sctSap)
CzSctSapCfg *sctSap;     /* Lower SAP configuration */
#endif
{
   CZ_TRC2(czValidateSctSapCfg);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating SCT SAP Configuration.\n"));
   if ((czCb.nodeCb.cfgDone != TRUE))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"General Configuration not done.\n"));
      CZ_RETVALUE(LCZ_REASON_PROTCFG_NOT_DONE);
   }

  /* validate for the number of sctp lower sap */
   if ((sctSap->suId >= czCb.genCfg.nmbSctSaps))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"spId out of range.\n"));
      CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
   }
   /* Range checking for timer */
   if ((sctSap->tBndTmr.val <= 0) ||
       (sctSap->tBndTmr.enb == FALSE))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Timer out of range/disabled.\n"));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* end of czLmmValidateSctSapCfg */


/**
  @brief This function validates the X2AP peer Configration.

  @param[in] peer  - peer pointer contaning the List of peers.

  @return
  -# Success : LCM_REASON_NOT_APPL
  -# Failure : LCZ_REASON_PROTCFG_NOT_DONE 
               LCM_REASON_EXCEED_CONF_VAL
               LCM_REASON_INVALID_PAR_VAL
*/
#ifdef ANSI
PRIVATE S16 czLmmValidatePeerCfg
(
CzPeerLstCfg *peer      /* Peer configuration */
)
#else
PRIVATE S16 czLmmValidatePeerCfg(peer)
CzPeerLstCfg *peer;     /* Peer configuration */
#endif
{
   CzAssocCfg *assocCfg=NULLP;
   CzPeerCb *peerCb=NULLP;
   CzPeerCfg *peerCfg=NULLP;
   U32 loopCnt;
   CzUstaDgn dgn;         /* Diagnostics */
   CZ_TRC2(czValidatePeerCfg);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating Peer Configuration.\n"));
   if ((czCb.nodeCb.cfgDone != TRUE))
   {
      CZ_RETVALUE(LCZ_REASON_PROTCFG_NOT_DONE);
   }

   /* cz001.301: peer->nmbPeers always should be less than
    * the genCfg.maxNmbPeer. ccpu00115102 */
   if ((peer->nmbPeers == 0) || (peer->nmbPeers > czCb.genCfg.maxNmbPeer))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Number of peers out of range.\n"));
      CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);       
   }

   for (loopCnt = 0; loopCnt < peer->nmbPeers; loopCnt++)
   {
           /* Get Peer control block */
       czNmmFndPeer(peer->peerCfg[loopCnt].peerId,&peerCb);
               

      if (peerCb != NULLP)
      {
         CZ_PRNT_PEERID;
            CZ_DBG_ERROR((CZ_PRNT_BUF,
                  "CzPeerLstCfg, peer is already  configured"));

            dgn.alarmType    = LCZ_USTA_DGNVAL_PEER;
            dgn.u.peerInfo.peerId = peer->peerCfg[loopCnt].peerId;
            dgn.u.peerInfo.dstNAddr.type = CM_NETADDR_NOTPRSNT;

            czLmmSendLmAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_LYR_SPECIFIC, 
                  LCM_CAUSE_LYR_SPECIFIC, &dgn);
         CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   if ((peer->peerCfg[loopCnt].eAValue == 0))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Intigrity Algoritham ieAValue should no be zero\n"));
      CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);       
   }

       /* validate for the number of sctp lower sap upper sap nas sapId's */
      if ( (peer->peerCfg[loopCnt].tSapId <  0) || 
           (peer->peerCfg[loopCnt].uSapId <  0) || 
           (peer->peerCfg[loopCnt].tSapId >= czCb.genCfg.nmbSctSaps) ||
           (peer->peerCfg[loopCnt].uSapId >= czCb.genCfg.nmbCztSaps) )
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,"tSapId/uSapId out of range.\n"));
         CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
      }
     
      if ( (czCb.sctSapLst[peer->peerCfg[loopCnt].tSapId] == NULLP) ||
           (czCb.cztSapLst[peer->peerCfg[loopCnt].uSapId] == NULLP) )
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,"tSap/uSap is NULLP.\n"));
         CZ_RETVALUE(LCZ_REASON_LSAPCFG_NOT_DONE);         
      }

         /* validate stream Id for common procedures */ 
      peerCfg = &(peer->peerCfg[loopCnt]);
      assocCfg= &(peer->peerCfg[loopCnt].assocCfg);

      if ( (assocCfg->locOutStrms < CZ_MIN_OUT_STREAMS) || 
          ((peerCfg->globalProcStrmId.pres) &&
          (peerCfg->globalProcStrmId.val > (assocCfg->locOutStrms - 1))))
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid stream parameters.\n"));
         CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }

   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* czLmmValidatePeerCfg */

/**

  @brief This function is used to shuting down the X2AP layer.

  @details  Shutdown the X2AP layer

  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
  */
#ifdef ANSI
PUBLIC S16 czLmmShutdown
(
Void
)
#else
PUBLIC S16 czLmmShutdown()
#endif
{
   S16 loopCnt;
   S16 ret=ROK;
   CzPeerCb *nextCb = NULLP;
   CzPeerCb *peerCb = NULLP;
  /*cz001.301 : freeing dynamPeerAssocLst: Purify Fix */
   CzDynamPeerAssocCb *dynamNextCb = NULLP;
   CzDynamPeerAssocCb *dynamPeerCb = NULLP;

   CZ_TRC2(czShutdown);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Layer Shutting Down..\n"));
   if (czCb.init.cfgDone != TRUE)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"General Configuration not done.\n"));
      CZ_RETVALUE(ret);
   }

    /* Check if previous shutdown request is pending */ 
   if (czCb.shutdown)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "czShutdown: Layer already SHUT DOWN) \n"));
      CZ_RETVALUE (ret);
   }

   czCb.shutdown  = TRUE;
   czCb.nodeCb.cfgDone = FALSE;

   /* Reset all X2AP control blocks and system parameters */

   /* DeInit the hashlists in CzCb */ 
   /*cmHashListDeinit(&czCb.nodeCb.peerCbLst);*/

   CZ_DBG_INFO((CZ_PRNT_BUF,"Deregistering Timers.\n"));
   /* Deregister the timers */
#ifdef SS_MULTIPLE_PROCS
    SDeregTmr(czCb.init.procId,czCb.init.ent, czCb.init.inst, 
                     (S16)czCb.genCfg.timeRes,
                     (PAIFTMRS16)czActvTmr);
#else
    SDeregTmr(czCb.init.ent, czCb.init.inst, 
                        (S16)czCb.genCfg.timeRes,
                        (PFS16)czActvTmr);
#endif

   /* Remove CZT Sap Cb */   
   for (loopCnt = 0; loopCnt < czCb.genCfg.nmbCztSaps; loopCnt++)
   {
      if ((czCb.cztSapLst[loopCnt] != (CzCztSapCb *)NULLP))
      {
         if ((ret = czLmmDelCztSap (czCb.cztSapLst[loopCnt]->spId,CZ_SHUT_DOWN)) != ROK)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,
                     "czCztSapCntrl, Couldn't delete the CZT SAP"));
            CZ_RETVALUE(RFAILED);
         }
      }   

   }

   /* Remove SCT Sap Cb */
   for (loopCnt = 0; loopCnt < czCb.genCfg.nmbSctSaps; loopCnt++)
   {
      if ((czCb.sctSapLst[loopCnt] != (CzSctSapCb *)NULLP))
      {
         /* call this fuction to delete the lower sap 
          * which internally Remove all logical connections associated with 
          * this SCT Sap and deallocate the sctSap */
         if ((ret = czLmmDelSctSap (loopCnt, CZ_SHUT_DOWN)) != ROK)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,"czSctSapCntrl, Couldn't delete the SCT SAP"));
            CZ_RETVALUE(ret);
         }
      }
   }
   nextCb = NULLP;
   while (cmHashListGetNext(&czCb.nodeCb.peerCbLst,
            (PTR)nextCb,
            (PTR *)&peerCb) == ROK)
   {
      if(peerCb != NULLP)
      {
         CZ_PRNT_PEERID;
         CZ_DBG_INFO((CZ_PRNT_BUF,"Deleting PeerCb\n"));
         czNmmDeallocPeerCb(peerCb);     
      }
   }  
  /*cz001.301 : freeing dynamPeerAssocLst: Purify Fix */
   while (cmHashListGetNext(&czCb.nodeCb.dynamPeerAssocLst,
            (PTR)dynamNextCb,
            (PTR *)&dynamPeerCb) == ROK)
   {
         cmHashListDelete(&czCb.nodeCb.dynamPeerAssocLst, (PTR)dynamPeerCb);
         czNmmDeallocDynamPeerCb(dynamPeerCb->peerCbPtr); 
         CZ_FREE(dynamPeerCb, sizeof(CzDynamPeerAssocCb));
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,"Deinitialize PeerCb list.\n"));
   cmHashListDeinit(&czCb.nodeCb.peerCbLst);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Deinitialize dynamic PeerCb list.\n"));
   cmHashListDeinit(&czCb.nodeCb.dynamPeerAssocLst);
   

   if ((czCb.cztSapLst != (CzCztSapCb **)NULLP))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"Freeing CZT SAPs\n"));
      CZ_FREE(czCb.cztSapLst, (czCb.genCfg.nmbCztSaps * sizeof(CzCztSapCb *)));
   }

   if ((czCb.sctSapLst != (CzSctSapCb **)NULLP))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,"Freeing SCT SAPs\n"));
      CZ_FREE(czCb.sctSapLst, (czCb.genCfg.nmbSctSaps * sizeof(CzSctSapCb *)));
   }


   /* call czActvInit with reason SHUTDOWN to de-initialize the czCb */

   CZ_DBG_INFO((CZ_PRNT_BUF,"De-initializing the czCb\n"));
#ifdef SS_MULTIPLE_PROCS
   ret = czActvInit (czCb.init.procId, czCb.init.ent, czCb.init.inst,
               czCb.init.region, SHUTDOWN , (Void **) NULLP);
#else
   ret = czActvInit (czCb.init.ent, czCb.init.inst,
               czCb.init.region, SHUTDOWN);
#endif

   SPutSMem (czCb.init.region, czCb.init.pool);

  CZ_DBG_INFO((CZ_PRNT_BUF,"----------  X2AP SHUT DOWN Complete -----------"));
   CZ_RETVALUE(ret);
   
} /* czLmmShutdown */





/**
  @brief This function deletes the upper CZT sap. 

  @details 
   This fuction is used to delete the upper X2ap sap for the requested sapId.
   Below are the steps to delete the upper sap.
   
  - Get the Upper sapCb, check the SAP is bound 
  - delete the refrence for upper SAP into the peer control block.
  - Finally free the sapCb from the SapCb list.


  @param[in]  sapId    Upper sapId to be deleted. 
  @param[in ] opn      Called for shutdown or delete SAP.

  The opn values will be as below.
  -# If called from shutdown   : CZ_SHUT_DOWN
  -# If called from delete Sap : CZ_DEL_SAP

  @return 
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
  */

#ifdef ANSI
PRIVATE S16 czLmmDelCztSap
(
SpId  sapId,         /* SAP Id */
U8    opn
)
#else
PRIVATE S16 czLmmDelCztSap(sapId,opn)
SpId  sapId;        /* SAP Id */
U8    opn;        /* SAP Id */
#endif
{

   CzCztSapCb  *uSapCb=NULLP;
   CzPeerCb *prev = NULLP;
   CzPeerCb *peerCb = NULLP; 
   S16 ret=ROK;
   
   CZ_TRC2(czDelCztSap);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Deleting CZT SAP SpId %d.\n", sapId));
 
   /* get the Upper SapCb From The Upper sap Lst  */  
   uSapCb = czCb.cztSapLst[sapId];
   
   if (uSapCb->state == LCZ_SAP_ISBOUND)
   {
      uSapCb->state=LCZ_SAP_CFG;
      CZ_DBG_INFO((CZ_PRNT_BUF,":  CZT SAP(%d) disabled sucsess",\
               sapId));
   }

   while (cmHashListGetNext(&czCb.nodeCb.peerCbLst, (PTR)prev,
            (PTR *)&peerCb) == ROK)
   {
      if(((peerCb) && peerCb->cztSapCb != NULLP) &&
         (peerCb->cztSapCb->spId == sapId))
      {
         CZ_PRNT_PEERID;
         CZ_DBG_INFO((CZ_PRNT_BUF,"Deinitializing th sap in peercb.\n"));
         peerCb->cztSapCb =  NULLP;
      }
      prev = peerCb;
   }
   
   /* Free the Upper SapCb From The Upper sap Lst  */  
   CZ_FREE(czCb.cztSapLst[sapId], sizeof(CzCztSapCb)); 

   CZ_RETVALUE(ret);

} /* czLmmDelCztSap */


/**
  @brief This function is used to enable sct sap.

  @details  This fuction is used to enable the lower SAP for the requested 
            lower sap Id. 
  
  @param[in]  lSapId    Lower sapId to be deleted.

  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.
  */
#ifdef ANSI
PRIVATE S16 czLmmEnaSctSap
(
SpId  lSapId         /* SAP Id */
)
#else
PRIVATE S16 czLmmEnaSctSap(lSapId)
SpId  lSapId;        /* SAP Id */
#endif
{
   CzSctSapCb *sctSapCb=NULLP;
   S16 ret;
   CZ_TRC2(czEnaSctSap);

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Enabling SAP spId %d \n", lSapId));
   sctSapCb = czCb.sctSapLst[lSapId];

   if ((sctSapCb->state == LCZ_SAP_ISBOUND) || 
       (sctSapCb->state == LCZ_SAP_BINDING))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
                "czEnaSctSap: SAP is already bound / waiting for bind confirm \n"));
      CZ_RETVALUE(ROK);
   } 

   /* Send Bind Request */

    /* start timer to wait for bind confirm */
   ret = czSchedGenTmr(CZ_TMR_SCT_BIND, (Void*)sctSapCb, CZ_TMR_START);
    
   if(ret != ROK)
    {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Unable to start the Bind Timer\n"));
      CZ_RETVALUE(RFAILED);
    }

   sctSapCb->state = LCZ_SAP_BINDING;
   sctSapCb->contEnt = ENTNC;  /* Entity Not Configured */

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Sending Bind request to SCTP \n"));
   (Void)CzLiSctBndReq(&sctSapCb->pst, sctSapCb->suId, sctSapCb->spId);
   
   CZ_RETVALUE(ROK);

} /* czLmmEnaSctSap */

/**

  @brief This function  unbinds the sct sap.

  @details  
      The layer manager interface calls this function when the layer manager requests
      an SAP be unbound. This function first terminates all the assosiation and sends endpoint 
      close request to sctp.
  
  @param[in]  lSapId    Lower sapId to be deleted .
  @param[in]  opn       Operation varibale.
  
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.


*/
#ifdef ANSI
PRIVATE S16 czLmmUbndSctSap
(
SpId  lSapId,        /* SAP Id */
U8    opn        /* Delete Peer CB or not */
)
#else
PRIVATE S16 czLmmUbndSctSap(lSapId, opn)
SpId  lSapId;        /* SAP Id */
U8    opn;        /* Delete Peer CB or not*/
#endif
{
   S16 ret = ROK;
   CzSctSapCb *sctSapCb=NULLP; 
   
   CZ_TRC2(czUbndSctSap);
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "unbing ing SAP spId %d \n", lSapId));

   sctSapCb = czCb.sctSapLst[lSapId];
   
    if(sctSapCb == NULLP)
    {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
                "Invalid SapId \n"));
      CZ_RETVALUE(RFAILED);
    }
   if ((sctSapCb->state == LCZ_SAP_UBND) ||
       (sctSapCb->state == LCZ_SAP_CFG))
   {
     CZ_DBG_INFO((CZ_PRNT_BUF,
                "SAP already unbound \n"));
      CZ_RETVALUE(ROK);
   }
    /*kworks fix: removed ret != ROK checked*/  
   /* Remove all logical connections associated with this SCT Sap */
    czNmmDeallocAllPeerCb(lSapId,opn);


   /* Close the endpoint */
   if ((sctSapCb->endpCb.state == CZ_ENDP_OPEN))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Sending Endpoint close request to SCTP \n"));
      ret =CzLiSctEndpCloseReq(&sctSapCb->pst, sctSapCb->spId, 
                          sctSapCb->endpCb.locSpEndpId, SCT_ENDPID_SP);
   }

   sctSapCb->state = LCZ_SAP_CFG;
  
   CZ_RETVALUE(ret);

} /* czLmmUbndSctSap */

/**
  @brief This function is used to delete the sct sap

  @details  Delete the lower SAP will internally terminate all the association.
            and free the sct sap from sapcb list.

  @param[in]  lSapId    Upper sapId to be deleted. 
  @param[in ] opn       Called for shutdown or delete SAP.

  The opn valuew will be as below.
  -# If called from shutdown   : CZ_SHUT_DOWN
  -# If called from delete Sap : CZ_DEL_SAP


  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PRIVATE S16 czLmmDelSctSap
(
SpId  lSapId,  /* SAP Id */
U8    opn
)
#else
PRIVATE S16 czLmmDelSctSap(lSapId,opn)
SpId  lSapId;        /* SAP Id */
U8    opn;
#endif
{
   S16 ret=ROK;
   CZ_TRC2(czDelSctSap);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Deleting SCT SAP SpId %d\n", lSapId));
   if(czCb.sctSapLst[lSapId]->state == LCZ_SAP_ISBOUND)
   {
      
      if ((ret = czLmmUbndSctSap (lSapId,opn)) != ROK)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF,
                       "czSctSapCntrl, Failed to Unbind and disable the SCT SAP"));
            CZ_RETVALUE(RFAILED);
         }
   }
   CZ_FREE(czCb.sctSapLst[lSapId], sizeof (CzSctSapCb));
  
   CZ_RETVALUE(ret);

} /* End of czLmmDelSctSap */




/**

   @brief This function is used to validate the peer Id.

   @details  Validates the given peerId by searching the peerCb in the 
             peerCb list. 
   
   @param[in]   peerId  PeerId of type U32.
   @param[out]  peerCb  Pointer return back to caller.
   
  @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCM_REASON_INVALID_PAR_VAL 
                    LCM_REASON_EXCEED_CONF_VAL
*/
#ifdef ANSI
PRIVATE S16 czLmmValidatePeer
(
U32        peerId,       /* general control structure */
CzPeerCb  **peerCb         /* peer control structure */
)
#else
PRIVATE S16 czLmmValidatePeer(peerId,peerCb)
U32 peerId;                 /* general control structure */
CzPeerCb  **peerCb;         /* peer control structure */
#endif
{ 
   S16 ret=RFAILED;
   CZ_TRC2(czValidatePeer);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating Peer.\n"));
   if ((peerCb!=NULLP) && 
       (peerId < czCb.genCfg.maxNmbPeer) && 
       (peerId > 0))
   {
      *peerCb=NULLP;
    if((ret= czNmmFndPeer(peerId,peerCb))!=ROK)
    {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"PeerCb not found.\n"));
     CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
    }
   }
   else
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Peer Id exceed configured value/peerCb in NULLP.\n"));
    CZ_RETVALUE( LCM_REASON_EXCEED_CONF_VAL);
   }
   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* czLmmValidatePeer */

/**
  @brief This function is used to delete the peer.

  @details  Delete the Peer from peer list forcefully.When X2AP receives a control
 request to delete a specific peer, it first sends SCTP terminate request
 and then deletes the peer after it receives a SCTP terminate confirm. 

  
  @param[in] peerCb Peer control block.
  
  @return
  - ROK           - If the Procedure was successful.
  - RFAILED       - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PRIVATE S16 czLmmDelPeer
(
CzPeerCb *peerCb     /* Peer control block */
)
#else
PRIVATE S16 czLmmDelPeer(peerCb)
CzPeerCb *peerCb;    /* Peer control block */
#endif
{
   S16 ret = ROK;

   CZ_TRC2(czDelPeer);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Deleting Peer.\n"));

   peerCb->delPcb = TRUE;

   /* Terminate the association */
   if ((ret = czTpmTermAssoc(peerCb->assocCb, FALSE)) != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "czDelPeer: Failed to terminate an \
               association or association not exist delete the peer"));
      ret = czNmmDeallocPeerCb(peerCb);
   }
   /* cz001.301 returning ROKPENDING, because we have not yet
    * got the confirmation from the SCTP for TermAssoc Req */
   else
   {
      ret = ROKPENDING;
   }
   /* Peer will be deallocated after receiving TermCfm */

   CZ_RETVALUE(ret);
} /* iEnd of czLmmDelPeer */


/**
  @brief This function validates the X2AP general statistics request parameters.

  @param[in] sts     General statistics structure.
  @param[in] action  Action to validate on.
  
  @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCM_REASON_INVALID_PAR_VAL 

*/
#ifdef ANSI
PRIVATE S16 czLmmValidateGenSts
(
CzGenSts  *sts,         /* general statistics  structure */
Action action           /* action */
)
#else
PRIVATE S16 czLmmValidateGenSts(sts, action)
CzGenSts *sts;         /* general statistics structure */
Action action;         /* action */
#endif
{
   S16 ret = LCM_REASON_NOT_APPL;

   CZ_TRC2(czValidateGenSts);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating General Statistics.\n"));

   if ((action != ZEROSTS) && (action != NOZEROSTS))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid action\n"));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   CZ_RETVALUE(ret);

} /* end of czLmmValidateGenSts */



/**
  @brief This function validates the peer node statistics request.

  @details
   This fuction is used to Validate the peer node statistics by searching the peer into
   peer List for the given peer Id.
   
  @param[in] sts     Node statistics structure
  @param[in] action  Action to validate on
  
  @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCM_REASON_INVALID_PAR_VAL 

*/

#ifdef ANSI
PRIVATE S16 czLmmValidatePNodeSts
(
CzPeerNodeSts  *sts,         /* Node statistics  structure */
Action action                /* action */
)
#else
PRIVATE S16 czLmmValidatePNodeSts(sts, action)
CzPeerNodeSts *sts;         /* Node statistics structure */
Action action;              /* action */
#endif
{
   S16 ret = LCM_REASON_NOT_APPL;
   CzPeerCb *peerCb = NULLP;

   CZ_TRC2(czValidatePNodeSts);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating Peer Statistic.\n"));

   if ((action != ZEROSTS )&& (action != NOZEROSTS))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid action\n"));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

  czNmmFndPeer(sts->peerId,&peerCb); 

   if ((peerCb == NULLP))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid PeerId\n"));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   CZ_RETVALUE(ret);

} /* End of czLmmValidatePNodeSts */

/**

  @brief This function validates the upper sap Id.

  @details  Validates the X2AP's upper  sap Id given in the
            request.
             
  @param[in] sSapId    Upper  sap Id.
  
  @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCZ_REASON_PROTCFG_NOT_DONE 

*/
#ifdef ANSI
PRIVATE S16 czLmmValidateCztSapId
(
SpId sSapId        /* Upper sap Id */
)
#else
PRIVATE S16 czLmmValidateCztSapId(sSapId)
SpId sSapId;        /* Upper sap Id */
#endif
{

   CZ_TRC2(czValidateCztSapId);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating CZT SAP\n"));

   if ((sSapId >= czCb.genCfg.nmbCztSaps))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"SapId out of range.\n"));
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   else
   {
   /* check for the valid Upper SapCb from the Upper sapCb Lst  */  
      if ((czCb.cztSapLst[sSapId] == NULLP))
      {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"SapCb is NULLP.\n"));
         CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }

   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* czLmmValidateCztSapId */

/**
  @brief This function validates the Lower Sct sap Id.

  @details  Validates the X2AP's lower sap Id given in the
             request.
  
  @param[in] tSapId    lower sap Id.
  
  @return
       -# Success : LCM_REASON_NOT_APPL
       -# Failure : LCM_REASON_INVALID_PAR_VAL
*/
#ifdef ANSI
PRIVATE S16 czLmmValidateSctSapId
(
SpId   tSapId          /* lower sap status */
)
#else
PRIVATE S16 czLmmValidateSctSapId(tSapId)
SpId   tSapId;         /* lower sap status */
#endif
{
   CZ_TRC2(czValidateSctSapId);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating SCT SAP\n"));

   
   if((tSapId < 0) || (tSapId > czCb.genCfg.nmbSctSaps))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid sapId \n"));
      CZ_SEND_SAPID_ALARM(tSapId, LCZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SPID);
      CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
   }

   /* If Sap is not bound then return invalid sap */
   if ((!czCb.sctSapLst) || (czCb.sctSapLst[tSapId]==NULLP))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Invalid sapId(%d) \n",tSapId));
      CZ_SEND_SAPID_ALARM(tSapId, LCZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SAP);
      CZ_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }


   CZ_RETVALUE(LCM_REASON_NOT_APPL);

} /* End of czLmmValidateSctSapId */



/**
  @brief This function Validates the X2AP Peer Status.

  @details Validates the X2AP Peer Status.
  check the numPeer against the configured one and return error if it failed to 
  find the peerCb int peerCb Lst. 

  @param[in]  sta    The Peer status Lm structure. 

  @return
  -# Success : LCM_REASON_NOT_APPL
  -# Failure : LCM_REASON_EXCEED_CONF_VAL
               LCM_REASON_INVALID_PAR_VAL
  */
#ifdef ANSI
PRIVATE S16 czLmmValidatePeerSta
(
CzPeerSta *sta          /* Peer status */
)
#else
PRIVATE S16 czLmmValidatePeerSta(sta)
CzPeerSta *sta;        /* Peer status */
#endif
{

   CZ_TRC2(czValidatePeerSta);
   CZ_DBG_INFO((CZ_PRNT_BUF,"Validating Peer Status\n"));

   if ((sta->nmbPeer >= czCb.genCfg.maxNmbPeer))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,"Number of peers exceeds the maximum \
               configured value.\n"));
      CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
   }
   CZ_RETVALUE(LCM_REASON_NOT_APPL);
} /* End of czLmmValidatePeerSta*/

/**
  @brief This function used to do the  initialization activities.
 
     @details This function is invoked by system services to initialize the X2AP
              layer. This is an entry point used by X2AP layer to initialize its
              global variables, before becoming operational.
              Allowable values for parameters are specified in ssi.h.
 
     @param[in] procId    - Specify the Proc id of the X2AP task if
                            SS_MULTIPLE_PROCS supported.
     @param[in] entity    - Specify the entity id of the X2AP task.
     @param[in] inst      - Specify the entity id of the X2AP task.
     @param[in] region    - Specifies the memory region from which
                            X2AP should allocate structures and buffers.
     @param[in] reason    - Specifies the reason for calling this
                            initialization function.
     @param[in] xxCb      - Pointer to allocated for X2Ap layer if multiproc is
                            defined .
                            
     @return 
         - ROK           - If the Procedure was successful.
         - RFAILED       - If the procedure was unsuccessful due to any failure.
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 czActvInit
(
ProcId procId,
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason,          /* reason */
Void **xxCb           /* Protocol control Block */
)
#else
PUBLIC S16 czActvInit(procId, entity, inst, region, reason,xxCb)
ProcId procId;         /* Proc Id */
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
Void **xxCb;           /* protocol control block */
#endif 
#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI 
PUBLIC S16 czActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 czActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
#endif /*SS_MULTIPLE_PROCS */
{

#ifdef SS_MULTIPLE_PROCS
   U16 loopCnt=0;
   PRIVATE U16 czFirstCall = FALSE;
   U16 idx=0;
#endif /* SS_MULTIPLE_PROCS */

/*   CZ_TRC2(czActvInit);
   CZ_DBG_INFO((CZ_PRNT_BUF,"X2AP Layer.........\n"));
*/
#ifdef SS_MULTIPLE_PROCS
   if(reason == SHUTDOWN)
   {
/*      CZ_DBG_INFO((CZ_PRNT_BUF,"SHUTDOWN\n"));*/
      CZ_SET_ZERO(&czCb, sizeof(CzCb));
      czCb.used = TRUE;
      czCb.shutdown = TRUE;
      czCb.init.proc  = procId;
      czCb.init.procId=procId;
   }
   else if(reason == NRM_TERM)
   {
/*      CZ_DBG_INFO((CZ_PRNT_BUF,"NRM TERM\n"));*/
      CZ_SET_ZERO(*xxCb, sizeof(CzCb));
      ((CzCb*)(*xxCb))->used = FALSE;

      RETVALUE(ROK);
   }
   else
   {
      if(!czFirstCall)
      {
/*         CZ_DBG_INFO((CZ_PRNT_BUF,"Not a first call\n"));*/
         czFirstCall = TRUE;
         for(loopCnt=0;loopCnt<CZ_MAX_INSTANCES;loopCnt++)
         {
           CZ_SET_ZERO(&czCbLst[loopCnt],sizeof(CzCb));
         }
         idx = 0;
         loopCnt = 0;
      }
      else
      {
/*         CZ_DBG_INFO((CZ_PRNT_BUF,"first call\n"));*/
         for(loopCnt=0;loopCnt<CZ_MAX_INSTANCES;loopCnt++)
         {
            if(czCbLst[loopCnt].used == FALSE)
            {
               czCbPtr=&czCbLst[loopCnt];
               idx = loopCnt;
               break;
            }
         }
      }

      if(loopCnt == CZ_MAX_INSTANCES)
      {
/*         CZ_DBG_ERROR((CZ_PRNT_BUF,"Reached maximum instances.\n"));*/
         RETVALUE(RFAILED);
      }

      czCbLst[idx].used=TRUE;
      czCbPtr=&czCbLst[idx];
      *xxCb=(Void *)&czCbLst[idx];
      czCb.shutdown = FALSE;
      czCb.init.proc   = procId;
      czCb.init.procId = procId;
#ifdef DEBUGP
      czCb.init.dbgMask &= 0x00000002;
#endif
   }
#else /* SS_MULTIPLE_PROCS */
   
   CZ_SET_ZERO(&czCb,sizeof(CzCb));
   if(reason == SHUTDOWN)
   {
      czCb.shutdown = TRUE;
   }
   czCb.init.procId = SFndProcId();
#endif /* SS_MULTIPLE_PROCS */

   czCb.init.ent = entity;
   czCb.init.inst = inst;
   czCb.init.region = region;
   czCb.init.reason = reason;
   czCb.init.cfgDone = FALSE;
   czCb.init.pool = 0;
   czCb.init.acnt = FALSE;
   czCb.init.trc = FALSE;

#ifdef CZ_USTA
   czCb.init.usta = TRUE;
#else
   czCb.init.usta = FALSE;
#endif /* CZ_USTA */

#ifdef DEBUGP
#ifdef CZ_DEBUG
   czCb.init.dbgMask = 0xffffffff;
#else
   czCb.init.dbgMask = 0x00000000;
#endif /* CZ_DEBUG */
#endif /* DEBUGP */
#ifdef SS_DIAG
   /* cz001.301: Chage for Diagnostics*/
   czCb.init.logMask = 0x0;
#endif
   RETVALUE(ROK);

} /* End of czActvInit */

/** @} */

/**********************************************************************

         End of file:     tr_lmm.c@@/main/2 - Tue Aug 30 18:36:13 2011

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    sy            1. LTE-X2AP Initial Release.
         cz001.101  mm            1. CR Fixes, Memory Leak fixes.
/main/2      ---    pt            1. LTE-X2AP 3.1 release.
/main/3  cz001.301  akaranth      1. X2AP build warning fix
                                  2. Change for Diagnostics
                                  3. Purify Fix.
*********************************************************************91*/
