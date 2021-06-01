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

     Desc:     This file contains the Layer Management interface module implementation.
               The functions for the configuration, control, status and statistics
               request primitives are defined here.

     File:     rm_lmm.c

     Sid:      qf_lmm.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Fri Jul 19 18:22:34 2013

     Prg:     jbm 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=162;

/* header include files */
#include "rm_cmn.h"
#include "lrm.h"
#include "rgm.h"
#include "rm_application.h"

RmCb g_stRmCb;

/* forward references */
PRIVATE U16 rmLrmGenCfg ARGS((
   RmCfg          *cfg
));

PRIVATE U16 rmLrmSapCfg ARGS((
   RmCfg          *cfg,
   Elmnt          sapType
));


PRIVATE Void rmLrmFillCfmPst ARGS((
   Pst           *reqPst,
   Pst           *cfmPst,
   RmMngmt       *cfm
));

PRIVATE Void rmLrmSapCntrl ARGS((
RmMngmt       *cntrl,
RmMngmt       *cfm,
Pst           *cfmPst
));

/**
 * @brief Task Initiation callback function.
 *
 * @details
 *
 *     Function : rmActvInit
 *
 *     This function is supplied as one of parameters during RRM's
 *     task registration. SSI will invoke this function once, after
 *     it creates and attaches this TAPA Task to a system task.
 *
 *  @param[in]  Ent entity, the entity ID of this task.
 *  @param[in]  Inst inst, the instance ID of this task.
 *  @param[in]  Region region, the region ID registered for memory
 *              usage of this task.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rmActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
{
   TRC2(rmActvInit);

   /* Initialize the MAC TskInit structure to zero */
   cmMemset((U8 *)&g_stRmCb, 0, sizeof(RmCb));

   /* Initialize the MAC TskInit with received values */
   g_stRmCb.rmInit.ent = entity;
   g_stRmCb.rmInit.inst = inst;
   g_stRmCb.rmInit.region = region;
   g_stRmCb.rmInit.pool = 0;
   g_stRmCb.rmInit.reason = reason;
   g_stRmCb.rmInit.cfgDone = FALSE;
   g_stRmCb.rmInit.acnt = FALSE;
   g_stRmCb.rmInit.usta = FALSE;
   g_stRmCb.rmInit.trc = FALSE;
#ifdef DEBUGP
#ifdef RM_DEBUG
   /* rm005.201 disabling debugs by default */
   /* g_stRmCb.rmInit.dbgMask = 0xffffffff; */
#endif
#endif /* DEBUGP */
//   g_stRmCb.rmInit.logMask = 0x0;
   g_stRmCb.rmInit.procId = SFndProcId();

   g_stRmCb.rmuSap.numBndRetries = 0;
   g_stRmCb.rgmSap.numBndRetries = 0;

   /* Initialize Sap state */
   g_stRmCb.rmuSap.sapState = LRM_NOT_CFG;
   g_stRmCb.rgmSap.sapState = LRM_NOT_CFG;


   RETVALUE(ROK);

} /* rmActvInit */


/**
 * @brief Layer Manager Configuration request handler.
 *
 * @details
 *
 *     Function : rmCfgReq
 *
 *     This function handles the configuration
 *     request received from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rmHdlGenCfg() or rmHdlSapCfg().
 *     -# Invokes RmMiLrmCfgCfm() to send back the confirmation to the LM.
 *
 *  @param[in]  Pst *pst, the post structure
 *  @param[in]  RmMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/

PUBLIC U16 rmCfgReq
(
RmProtoCfg  *protoCfg,    /* config structure  */
U8		 	event
)
{
	return (g_rObjRRMApp.processSmMsg(event, (U8*)protoCfg));
}
/**
 * @brief Layer Manager Configuration request handler.
 *
 * @details
 *
 *     Function : RmMiLrmCfgReq
 *
 *     This function handles the configuration
 *     request received from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rmHdlGenCfg() or rmHdlSapCfg().
 *     -# Invokes RmMiLrmCfgCfm() to send back the confirmation to the LM.
 *
 *  @param[in]  Pst *pst, the post structure
 *  @param[in]  RmMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 RmMiLrmCfgReq
(
Pst      *pst,    /* post structure  */
RmMngmt  *cfg     /* config structure  */
)
{
   U16       ret = LCM_PRIM_OK;
   U16       reason = LCM_REASON_NOT_APPL;
   RmMngmt   *cfm;
   Pst       cfmPst;

   TRC2(RmMiLrmCfgReq);

   // RMDBGPRM((rmPBuf, "\nRmMiLrmCfgReq(): pst->srcEnt=%d, pst->srcInst=%d",
   // pst->srcEnt, pst->srcInst));

   cfm = (RmMngmt*)rrmAlloc(sizeof(RmMngmt));
  
   if(NULLP == cfm)
   {
      RETVALUE(RFAILED);
   }
   /* Fill the post structure for sending the confirmation */
   rmLrmFillCfmPst(pst, &cfmPst, cfg);

   cmMemset((U8 *)cfm, 0, sizeof(RmMngmt));

   /* rm009.201 -ccpu00116453: Adding transId */
#ifdef LMINT3
   cfm->hdr.transId = cfg->hdr.transId;
#endif


   cfm->hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STRMGEN:
   	 g_stRmCb.genCfg.bndCfmResp.response = cfm->hdr.response;

         reason = rmLrmGenCfg(&cfg->t.cfg);
         //RMDBGINFO((rmPBuf, "\nRmMiLrmCfgReq(): Gen Cfg done with reason=%d.",reason));
         break;
      case STRMUSAP:
      case STRGMSAP:
         reason = rmLrmSapCfg(&cfg->t.cfg, cfg->hdr.elmId.elmnt);
         //RMDBGINFO((rmPBuf, "\nRmMiLrmCfgReq(): SAP(Elmnt=%d) Cfg done with reason=%d.",
         //        cfg->hdr.elmId.elmnt, reason));
         break;
      case STRMPROTOCFG:
         reason = rmCfgReq(&cfg->t.protoCfg, pst->event);
         break;

      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         //RMDBGERR((rmPBuf, "\nRmMiLrmCfgReq(): Invalid Elmnt=%d", cfg->hdr.elmId.elmnt));
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm->cfm.status = ret;
   cfm->cfm.reason = reason;

   RmMiLrmCfgCfm(&cfmPst, cfm);

//   rrmFree((U8 *)cfg, sizeof(RmMngmt));

   RETVALUE(ROK);
}/*-- RmMiLrmCfgReq --*/

/***********************************************************
 *
 *     Func : rmResetCfg
 *
 *
 *     Desc : Handles the CFG reset request
 *
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File : rm_lmm.c
 *
 **********************************************************/
PRIVATE Void rmResetCfg
(
)
{
   TRC2(rmResetCfg)

   /* RMDBGPRM((rmPBuf, "\nrmResetCfg(): CFG Reset Requested")); */

   /* Free the memory held by the Layer */
   /* Timer DeRegistration request to SSI */
   if (SDeregTmrMt(g_stRmCb.rmInit.ent, g_stRmCb.rmInit.inst,
            (S16)g_stRmCb.genCfg.tmrRes, rmActvTmr) != ROK)
   {
      /* RMDBGERR((rmPBuf, "\nrmResetCfg(): Failed to Deregister timer.")); */
      RETVOID;
   }

   //Delete the interface obj

   /* call back the task initialization function to intialize
    * the global RmCb Struct */
   rmActvInit(g_stRmCb.rmInit.ent, g_stRmCb.rmInit.inst, g_stRmCb.rmInit.region,
              g_stRmCb.rmInit.reason);

   RETVOID;
}

/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : RmMiLrmCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable) and layer shutdown.
 *      -# Invokes the RmMiLrmCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RmMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 RmMiLrmCntrlReq
(
Pst      *pst,    /* post structure  */
RmMngmt  *cntrl   /* control structure  */
)
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RmMngmt   *cfm;
   
   TRC2(RmMiLrmCntrlReq);
   
   RLOG3(L_DEBUG, "Control Request for elmnt= %d, action=%d, subaction=%d",
         cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action,
         cntrl->t.cntrl.subAction);

   cfm = (RmMngmt*)rrmAlloc(sizeof(RmMngmt));
   
   if(NULLP == cfm)
   {
      RETVALUE(RFAILED);
   }
      
   /* Fill the post structure for sending the confirmation */
   rmLrmFillCfmPst(pst, &cfmPst, cntrl);

   cmMemset((U8 *)cfm, 0, sizeof(RmMngmt));

#ifdef LMINT3
   cfm->hdr.transId = cntrl->hdr.transId;
#endif

   cfm->hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm->t.cntrl.action = cntrl->t.cntrl.action;
   cfm->t.cntrl.subAction = cntrl->t.cntrl.subAction;

   /* Check if General Config Done*/
   if(g_stRmCb.rmInit.cfgDone != TRUE)
   {
      cfm->cfm.status = LCM_PRIM_NOK;
      cfm->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm->hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      RmMiLrmCntrlCfm(&cfmPst, cfm);
//      RMDBGERR((rmPBuf, "\nRmMiLrmCntrlReq(): Gen Cfg not done."));
      RETVALUE(ROK);
   }
 
   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STRMGEN:
         {
            switch (cntrl->t.cntrl.action)
            {
               case LRM_RESET_CFG:
                  {
                     rmResetCfg();
                     cfm->cfm.status = LCM_PRIM_OK;
                     cfm->cfm.reason = LCM_REASON_NOT_APPL;
                     RmMiLrmCntrlCfm(&cfmPst, cfm);
                     break;
                  }
               default:
                  break;
            }
            break;
         }   
      case STRGMSAP:
         rmLrmSapCntrl(cntrl, cfm, &cfmPst);
         break;
      default:
         cfm->cfm.status = LCM_PRIM_NOK;
         cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         RmMiLrmCntrlCfm(&cfmPst, cfm);

//         RMDBGERR((rmPBuf, "\nRmMiLrmCntrlReq(): invalid elmnt=%d",
//         cntrl->hdr.elmId.elmnt));

         break;
   }

//   rrmFree((U8 *)cntrl, sizeof(RmMngmt));

   RETVALUE(ret);
}/*-- RmMiLrmCntrlReq --*/



/**
 * @brief SAP Configuration Handler.
 *
 * @details
 *
 *     Function : rmLrmSapCfg
 *
 *     This function in called by RmMiLrmCfgReq(). It handles the
 *     interface SAP configuration of the LTE MAC layer. It
 *     initializes the sapState to LRM_UNBND. Returns
 *     reason for success/failure of this function.
 *
 *  @param[in]  RmCfg *cfg, the Configuaration information
 *  @return  U16
 *      -# LCM_REASON_GENCFG_NOT_DONE
 *      -# LCM_REASON_INVALID_SAP
 *      -# LCM_REASON_NOT_APPL
 **/
PRIVATE U16 rmLrmSapCfg
(
RmCfg *cfg,            /* Configuaration information */
Elmnt sapType             /* Sap Type */
)
{
   U16               ret = LCM_REASON_NOT_APPL;
   RmUiSapCb         *upSapCfg = NULLP;
   RmLiSapCb         *lSapCfg = NULLP;

   TRC2(rmLrmSapCfg)

   //RMDBGPRM((rmPBuf, "\nrmLrmSapCfg(): SAP Config Requested for sapType=%d", sapType));
   /* Check if Gen Config has been done */
   if(g_stRmCb.rmInit.cfgDone != TRUE)
      RETVALUE(LCM_REASON_GENCFG_NOT_DONE);

   switch(sapType)
   {
      case STRMUSAP:
         if ((cfg->s.rmuSap.selector != RMU_SEL_LWLC) &&
             (cfg->s.rmuSap.selector != RMU_SEL_TC) &&
             (cfg->s.rmuSap.selector != RMU_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            //RMDBGERR((rmPBuf, "\nrmLrmSapCfg(): unsupported Selector value for RMU."));
            break;
         }
         if(g_stRmCb.rmuSap.sapState == LRM_NOT_CFG)
         {
            g_stRmCb.rmuSap.sapState = LRM_UNBND;
         }
         upSapCfg = &g_stRmCb.rmuSap;

         upSapCfg->pst.dstEnt = cfg->s.rmuSap.ent;
         upSapCfg->pst.dstInst = cfg->s.rmuSap.inst;
         upSapCfg->pst.dstProcId = cfg->s.rmuSap.procId;
         upSapCfg->pst.srcEnt = g_stRmCb.rmInit.ent;
         upSapCfg->pst.srcInst = g_stRmCb.rmInit.inst;
         upSapCfg->pst.srcProcId = g_stRmCb.rmInit.procId;
         upSapCfg->pst.region = cfg->s.rmuSap.mem.region;
         upSapCfg->pst.pool = cfg->s.rmuSap.mem.pool;
         upSapCfg->pst.selector = cfg->s.rmuSap.selector;
         upSapCfg->pst.route = cfg->s.rmuSap.route;
         upSapCfg->pst.intfVer = 0;
         upSapCfg->pst.prior = cfg->s.rmuSap.prior;
         upSapCfg->suId = cfg->s.rmuSap.suId;
         upSapCfg->spId = cfg->s.rmuSap.spId;
         break;

      case STRGMSAP:
         if ((cfg->s.rgmSap.selector != RMU_SEL_LWLC) &&
             (cfg->s.rgmSap.selector != RMU_SEL_TC) &&
             (cfg->s.rgmSap.selector != RMU_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            //RMDBGERR((rmPBuf, "\nrmLrmSapCfg(): unsupported Selector value for RMU."));
            break;
         }
         if(g_stRmCb.rgmSap.sapState == LRM_NOT_CFG)
         {
            g_stRmCb.rgmSap.sapState = LRM_UNBND;
         }

         lSapCfg = &g_stRmCb.rgmSap;

         lSapCfg->pst.dstEnt = cfg->s.rgmSap.ent;
         lSapCfg->pst.dstInst = cfg->s.rgmSap.inst;
         lSapCfg->pst.dstProcId = cfg->s.rgmSap.procId;
         lSapCfg->pst.srcEnt = g_stRmCb.rmInit.ent;
         lSapCfg->pst.srcInst = g_stRmCb.rmInit.inst;
         lSapCfg->pst.srcProcId = g_stRmCb.rmInit.procId;
         lSapCfg->pst.region = cfg->s.rgmSap.mem.region;
         lSapCfg->pst.pool = cfg->s.rgmSap.mem.pool;
         lSapCfg->pst.selector = cfg->s.rgmSap.selector;
         lSapCfg->pst.route = cfg->s.rgmSap.route;
         lSapCfg->pst.intfVer = 0;
         lSapCfg->pst.prior = cfg->s.rgmSap.prior;
         lSapCfg->suId = cfg->s.rgmSap.suId;
         lSapCfg->spId = cfg->s.rgmSap.spId;
         break;

      default:
         /* would never reach here */
         break;
   }

   RETVALUE(ret);
}


/**
 * @brief General Configuration Handler.
 *
 * @details
 *
 *     Function : rmLrmGenCfg
 *
 *     This function in called by RmMiLrmCfgReq(). It handles the
 *     general configuration of the LTE MAC layer. It initializes
 *     the hash lists of RmCb. Returns
 *     reason for success/failure of this function.
 *
 *  @param[in]  RmCfg *cfg, the Configuaration information
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
PRIVATE U16 rmLrmGenCfg
(
RmCfg *cfg            /* Configuaration information */
)
{
   U16    ret = LCM_REASON_NOT_APPL;

   TRC2(rmLrmGenCfg)

   //RMDBGPRM((rmPBuf, "\nrmLrmGenCfg(): General Configuration Request"));
   /* Check if General Configuration is done already */
   if (g_stRmCb.rmInit.cfgDone == TRUE)
   {
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }
   if ((cfg->s.genCfg.lmPst.selector != LRM_SEL_TC) &&
       (cfg->s.genCfg.lmPst.selector != LRM_SEL_LC) &&
       (cfg->s.genCfg.lmPst.selector != LRM_SEL_LWLC))
   {
      //RMDBGERR((rmPBuf, "\nrmLrmSapCfg(): unsupported Selector value for RMU."));
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /* Update the Pst structure for LM interface */
   cmMemcpy((U8 *)&g_stRmCb.rmInit.lmPst, (U8 *)&cfg->s.genCfg.lmPst,
             sizeof(Pst));

   g_stRmCb.rmInit.lmPst.srcProcId = g_stRmCb.rmInit.procId;
   g_stRmCb.rmInit.lmPst.srcEnt = g_stRmCb.rmInit.ent;
   g_stRmCb.rmInit.lmPst.srcInst = g_stRmCb.rmInit.inst;
   g_stRmCb.rmInit.lmPst.event = EVTNONE;

   g_stRmCb.rmInit.region = cfg->s.genCfg.mem.region;
   g_stRmCb.rmInit.pool = cfg->s.genCfg.mem.pool;
   g_stRmCb.genCfg.tmrRes = cfg->s.genCfg.tmrRes;	

   /* Initialize SAP States */
   g_stRmCb.rmuSap.sapState = LRM_NOT_CFG;
   g_stRmCb.rgmSap.sapState = LRM_NOT_CFG;

   /* Initialize the timer blocks */
   cmInitTimers(g_stRmCb.tmrBlk, RM_MAX_TIMER);
   /* Initialzie the timer queue */   
   cmMemset((U8 *)&g_stRmCb.tmrTq, 0, sizeof(CmTqType)*RM_TQ_SIZE);
   /* Initialize the timer control point */
   cmMemset((U8 *)&g_stRmCb.tmrTqCp, 0, sizeof(CmTqCp));

   g_stRmCb.tmrTqCp.tmrLen = RM_TQ_SIZE;
   /* Timer Registration request to SSI */
   if (SRegTmrMt(g_stRmCb.rmInit.ent, g_stRmCb.rmInit.inst,
            (S16)g_stRmCb.genCfg.tmrRes, rmActvTmr) != ROK)
   {
/*      
      RMDBGERR((rmPBuf, "\nrmGenCfg(): Failed to register timer."));
*/
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /* Set Config done in TskInit */
   g_stRmCb.rmInit.cfgDone = TRUE;

   RETVALUE(ret);
}



/***********************************************************
 *
 *     Func : rmLrmSapCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STxxxSAP elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rm_lmm.c 
 *
 **********************************************************/
PRIVATE Void rmLrmSapCntrl 
(
RmMngmt       *cntrl,
RmMngmt       *cfm,
Pst           *cfmPst
)
{
   TRC2(rmLrmSapCntrl)

   RLOG2(L_DEBUG, "Control Req for SAP: %d Action: %d",
         cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action);

   cfm->hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STRGMSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ABND:
            /* Bind Enable Request */
               if ((g_stRmCb.rgmSap.sapState == LRM_NOT_CFG) ||
                   (g_stRmCb.rgmSap.sapState == LRM_BND))
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_SAP;
               }
               else
               {
                  if (g_stRmCb.rgmSap.bndTmr.enb == TRUE)
                  {
                     rmLrmStartTmr(RM_BNDREQ_TMR, g_stRmCb.rgmSap.bndTmr.val, 
                     (PTR)&g_stRmCb.rgmSap);
                  }
                  /* Change SAP state */
                  g_stRmCb.rgmSap.sapState = LRM_WAIT_BNDCFM;
                  g_stRmCb.rgmSap.numBndRetries++;
                  /* Store the response and TransId for sending 
                   * the Control confirm */
                  cmMemcpy((U8 *)&g_stRmCb.genCfg.bndCfmResp.response,
                           (U8 *)&cntrl->hdr.response, sizeof(Resp));
                  g_stRmCb.genCfg.bndCfmResp.transId = cntrl->hdr.transId;

                  /* rm003.201: Sending Status Indication to Layer Manager */
                  cfm->cfm.status = LCM_PRIM_OK_NDONE;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
			
   		         RmMiLrmCntrlCfm(cfmPst, cfm);

                  RLOG0(L_DEBUG,"Sending RGM Bind Request");
                  rmLIMRgmBndReq(g_stRmCb.rgmSap.suId, g_stRmCb.rgmSap.spId);
                  RETVOID;
               }
               break;
            case AUBND:
            /* Unbind request */

               /* Check if the SAP is configured */
               if( (g_stRmCb.rgmSap.sapState == LRM_NOT_CFG) ||
                     (g_stRmCb.rgmSap.sapState == LRM_UNBND))
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_MSGTYPE;
               }
               else
               {
                  RLOG0(L_ERROR,"Sending RGM Un-Bind Request");
                  rmLIMRgmUbndReq(g_stRmCb.rgmSap.spId, RGM_UBNDREQ_MNGMT);
                  if (g_stRmCb.rgmSap.bndTmr.enb == TRUE)
                  {
                     rmLrmStopTmr(RM_BNDREQ_TMR, (PTR)&g_stRmCb.rgmSap);
                  }
                  /* Change SAP state */
                  g_stRmCb.rgmSap.sapState = LRM_UNBND;
                  cfm->cfm.status = LCM_PRIM_OK;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
               }
               break;
            case ADEL:
               /* Delete SAP, does initialization of SAP */
               if ((g_stRmCb.rgmSap.sapState == LRM_WAIT_BNDCFM) ||
                   (g_stRmCb.rgmSap.sapState == LRM_BND))
               {
                  rmLIMRgmUbndReq(g_stRmCb.rgmSap.spId, RGM_UBNDREQ_MNGMT);
                  if (g_stRmCb.rgmSap.bndTmr.enb == TRUE)
                  {
                     rmLrmStopTmr(RM_BNDREQ_TMR, (PTR)&g_stRmCb.rgmSap);
                  }
               }
               cmMemset((U8 *)&g_stRmCb.rgmSap, 0, sizeof(RmLiSapCb));
               g_stRmCb.rgmSap.sapState = LRM_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               /*
               RMDBGERR((rmPBuf, "\nrmLrmSapCntrl(): invalid action=%d",
               cntrl->t.cntrl.action));
               */
               break;
         }
         break;

      case STRMUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               cmMemset((U8 *)&g_stRmCb.rmuSap, 0, sizeof(RmUiSapCb));
               g_stRmCb.rmuSap.sapState = LRM_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               /*
               RMDBGERR((rmPBuf, "\nrmLrmSapCntrl(): invalid action=%d",
               cntrl->t.cntrl.action));
               */
               break;
         }
         break;
      default:
         /* Would never here. */
         RETVOID;
   }

   RmMiLrmCntrlCfm(cfmPst, cfm);
   RETVOID;
}




/**
 * @brief Timer Expiry handler. 
 *
 * @details
 *
 *     Function : rmLrmTmrExpiry
 *     
 *     This is a callback function used as an input parameter to cmPrcTmr()
 *     to check expiry of any timer. In this function, the only concern is
 *     about tmrEvnt=Bind timer.
 *     
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rmLrmTmrExpiry
(
PTR cb,               /* Pointer to timer control block */
S16 tmrEvnt           /* Timer Event */
)
{
   S16        ret = ROK;
   RmLiSapCb  *rgmSap = (RmLiSapCb  *)cb;

   TRC2(rmLrmTmrExpiry)
/*
   RMDBGPRM((rmPBuf, "\nrmLrmTmrExpiry(): Timer Expired for tmrEvnt=%d",tmrEvnt));
  */ 
   switch(tmrEvnt)
   {
      case RM_BNDREQ_TMR:
         rgmSap->numBndRetries++;
         if(rgmSap->numBndRetries > RM_MAX_BNDRETRY)
         {
            /*
            rmLrmStaInd(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, NULLP);
            RMDBGINFO((rmPBuf, "\nrmLrmTmrExpiry(): Max Bind Retries Exceeded."));
            */
         }
         else
         {
/*            RMDBGINFO((rmPBuf, "\nrmLrmTmrExpiry(): Retry Bind"));*/
            /* Restart the bind timer */
            if (rgmSap->bndTmr.enb == TRUE)
            {
               ret = rmLrmStartTmr(RM_BNDREQ_TMR, rgmSap->bndTmr.val,
               cb);
            }

            /* Send bind request */
            rmLIMRgmBndReq(rgmSap->suId, rgmSap->spId);
         }
         break;
      default:
         /*
         RMDBGERR((rmPBuf, "\nrmLrmTmrExpiry(): Invalid tmrEvnt=%d", tmrEvnt));
         */
         ret = RFAILED;
         break;
   }
   RETVALUE(ret);
}

/***********************************************************
 *
 *     Func : rmLrmFillCfmPst
 *
 *
 *     Desc : Fills the Confirmation Post Structure cfmPst using the reqPst
 *            and the cfm->hdr.response.
 *
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File : rm_lmm.c
 *
 **********************************************************/
PRIVATE Void rmLrmFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst,
RmMngmt       *cfm
)
{
   TRC2(rmLrmFillCfmPst)

   cfmPst->srcEnt    = g_stRmCb.rmInit.ent;
   cfmPst->srcInst   = g_stRmCb.rmInit.inst;
   cfmPst->srcProcId = g_stRmCb.rmInit.procId;
   cfmPst->dstEnt    = reqPst->srcEnt;
   cfmPst->dstInst   = reqPst->srcInst;
   cfmPst->dstProcId = reqPst->srcProcId;

   cfmPst->selector  = cfm->hdr.response.selector;
   cfmPst->prior     = cfm->hdr.response.prior;
   cfmPst->route     = cfm->hdr.response.route;
   cfmPst->region    = cfm->hdr.response.mem.region;
   cfmPst->pool      = cfm->hdr.response.mem.pool;

   
   RETVOID;
}

/**
 * @brief Layer Manager Unsolicited Status Indication
 *
 * @details
 *
 *     Function : RmMiLrmEnbStaInd
 *
 *     This API is used by the other modules of MAC to send a unsolicited
 *     status indication to the Layer Manager.
 *
 *  @param[in]
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 RmMiLrmEnbStaInd
(
   Pst *pst,
   RmMngmt *lrmEnbStaInd
)
{
	return (g_rObjRRMApp.processSmMsg(pst->event, (U8*)lrmEnbStaInd));
}

/**
 * @brief Timer start handler. 
 *
 * @details
 *
 *     Function : rmLrmStartTmr
 *     
 *     This function based on the input parameters starts the timer for 
 *     "tmrVal" duration. As of now MAC uses the timer functionality for 
 *     BndReq only. Hence there is no conditional code based on "tmrEvnt".
 *     
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @param[in]  U32   tmrVal,  the Wait Time
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rmLrmStartTmr
(
S16                tmrEvnt,            /* Timer Event */
U32                tmrVal,             /* Wait Time */
PTR                cb                  /* Entry for which Timer Expired */
)
{
   CmTmrArg    arg;

   TRC2(rmLrmStartTmr)

   UNUSED(tmrEvnt);
   /*
#ifndef ALIGN_64BIT
   RMDBGPRM((rmPBuf, "\nrmLrmStartTmr(): Timer start request for tmrEvnt=%d, tmrVal=%ld", 
   tmrEvnt, tmrVal));
#else
   RMDBGPRM((rmPBuf, "rmLrmStartTmr(): Timer start request for tmrEvnt=%d, tmrVal=%d", 
   tmrEvnt, tmrVal));
#endif
*/
   /* Initialize the arg structure */
   cmMemset((U8 *)&arg, 0, sizeof(CmTmrArg));

   arg.tqCp = &g_stRmCb.tmrTqCp;
   arg.tq = g_stRmCb.tmrTq;
   arg.timers = g_stRmCb.tmrBlk;
   arg.cb = cb;
   arg.tNum = 0;
   arg.max = RM_MAX_TIMER;
   arg.evnt = RM_BNDREQ_TMR;
   arg.wait = tmrVal;      

   cmPlcCbTq(&arg);

   RETVALUE(ROK);
}



/**
 * @brief Timer stop handler. 
 *
 * @details
 *
 *     Function : rmLrmStopTmr
 *     
 *     This function based on the input parameters stops the timer for 
 *     "tmrEvnt". As of now MAC uses the timer functionality for 
 *     BndReq only. Hence there is no conditional code based on "tmrEvnt".
 *     Once the bind happens and this timer is stopped, the timer functionality
 *     is deregistered with SSI. As there is no further use of timer processing.
 *     
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
PUBLIC S16 rmLrmStopTmr
(
S16                tmrEvnt,            /* Timer Event */
PTR                cb                  /* Entry for which Timer Expired */
)
{
   CmTmrArg   arg;
   U8         idx;
   S16        ret = ROK; 

   TRC2(rmLrmStopTmr)
/*
   RMDBGPRM((rmPBuf, "\nrmLrmStopTmr(): Timer stop request for tmrEvnt=%d",tmrEvnt));
   ret = RFAILED;
*/
   for(idx=0;idx<RM_MAX_TIMER;idx++)
   {
      /* Search for the Timer Blocks */
      if(g_stRmCb.tmrBlk[idx].tmrEvnt == tmrEvnt)
      {
         /* Initialize the arg structure */
         cmMemset((U8 *)&arg, 0, sizeof(CmTmrArg));

         arg.tqCp = &g_stRmCb.tmrTqCp;
         arg.tq = g_stRmCb.tmrTq;
         arg.timers = g_stRmCb.tmrBlk;
         arg.cb = cb;
         arg.max = RM_MAX_TIMER;
         arg.evnt = NOTUSED;
         arg.wait = NOTUSED;

         arg.tNum = idx;   
         cmRmvCbTq(&arg);
         ret = ROK;
         break;
      }

   }
/*
   RMDBGPRM((rmPBuf, "\nrmLrmStopTmr(): tmrEvnt=%d search result is ret=%d",tmrEvnt,ret));
*/
   RETVALUE(ret);
}

/**
 * @brief LTE MAC timer call back function registered with SSI. 
 *
 * @details
 *
 *     Function :  rmActvTmr
 *     
 *     This function is invoked by SSI for every timer activation
 *     period expiry.
 *     
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rmActvTmr
(
 Ent     ent,
 Inst    inst
)
{
   TRC3(rmActvTmr)

   /* Check if any MAC timer has expired */ 
   cmPrcTmr(&g_stRmCb.tmrTqCp, g_stRmCb.tmrTq, (PFV) rmLrmTmrExpiry);

   RETVALUE(ROK);
 
} /* end of rmActvTmr */

/**
 * @brief Layer Manager Control Confirm generation handler
 *        for Bind Confirm reception at RGM interface.
 *        RmLiRgmBndCfm() forwards the confirmation to this 
 *        function. All SAP state related handling is restricted
 *        to Lrm modules, hence the cfm forwarding.
 *
 * @details
 *
 *     Function : rmLrmBndCfm 
 *     
 *     This API is used by the LIM module of MAC to forward
 *     the Bind Confirm it receives over the RGM interface.
 *     
 *  @param[in]   Pst *pst, Post Structure
 *  @param[in]   SuId suId, Service user ID
 *  @param[in]   U8 status, Status
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rmLrmBndCfm
(
SuId suId,              /* Service user ID */
U8 status               /* Status */
)
{
   S16       ret = ROK;
   RmMngmt   *cntrlCfm;
   Pst       cfmPst;

   TRC3(rmLrmBndCfm)

   RLOG3(L_DEBUG, "Handle Bind Confirm: suId=%d, status=%d, sapState=%d", 
         suId, status, g_stRmCb.rgmSap.sapState);

   /* Check if the suId is valid */
   if(g_stRmCb.rgmSap.suId != suId)
   {
      RLOG2(L_DEBUG, "Incorrect SuId. Configured (%d) Recieved (%d)\n",
            g_stRmCb.rgmSap.suId, suId);
      RETVALUE(RFAILED);
   }

   /* check the Sap State */
   switch(g_stRmCb.rgmSap.sapState)
   {
      case LRM_WAIT_BNDCFM:
         break;
      case LRM_BND:
         /* SAP is already bound */
         RETVALUE(ROK);
      default:
         RETVALUE(RFAILED);
   }

   cntrlCfm = (RmMngmt*)rrmAlloc(sizeof(RmMngmt));
   
   if(NULLP == cntrlCfm)
   {
      RETVALUE(RFAILED);
   }

   cfmPst = g_stRmCb.rmInit.lmPst;

   cmMemset((U8 *)cntrlCfm, 0, sizeof(RmMngmt));

   switch(status)
   {
      case CM_BND_OK: /* status is OK */
         /* Change SAP state to Bound */
         g_stRmCb.rgmSap.sapState = LRM_BND;
         if (g_stRmCb.rgmSap.bndTmr.enb == TRUE)
         {
            ret = rmLrmStopTmr(RM_BNDREQ_TMR, (PTR)&g_stRmCb.rgmSap);
         }
         /* Send Control Confirm with status as OK to Layer Manager */
         cntrlCfm->cfm.status = LCM_PRIM_OK;
         cntrlCfm->cfm.reason = LCM_REASON_NOT_APPL;
         break;

      default:
         /* Change SAP state to UnBound */
         g_stRmCb.rgmSap.sapState = LRM_UNBND;
         if (g_stRmCb.rgmSap.bndTmr.enb == TRUE)
         {
            ret = rmLrmStopTmr(RM_BNDREQ_TMR, (PTR)&g_stRmCb.rgmSap);
         }
         /* Send Control Confirm with status as NOK to Layer Manager */
         cntrlCfm->cfm.status = LCM_PRIM_NOK;
         cntrlCfm->cfm.reason = LCM_REASON_NEG_CFM;
         break;
   }

   g_stRmCb.rgmSap.numBndRetries = 0;

   cntrlCfm->hdr.elmId.elmnt = STRGMSAP;
   cntrlCfm->hdr.transId = g_stRmCb.genCfg.bndCfmResp.transId;

   RLOG0(L_DEBUG, "sending control confirm to LRM");
   ret = RmMiLrmCntrlCfm(&cfmPst, cntrlCfm);

   /*
   RMDBGINFO((rmPBuf, "\nrmLrmBndCfm(): RmMiLrmCntrlCfm  ret=%d.",ret));
   */
   RETVALUE(ret);
}

/********************************************************************30**

           End of file:     qf_lmm.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Fri Jul 19 18:22:34 2013

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
