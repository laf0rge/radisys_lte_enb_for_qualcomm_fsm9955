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
               

     File:     wr_mi.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="ENBAPP";
static int RLOG_MODULE_ID=64;
static int RLOG_FILE_ID=30;

#include "wr.h"
#include "wr_cmn.h"
#include "wr_utils.h"
#include "wr_lwr.h"
#include "wr_lwr.h"
#include "wr_emm.h"
#include "wr_umm.h"

#include "wr_ifm_rrc.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_l1.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_pdcp.h"
#include "wr_ifm_egtp.h"
#include "wr_ifm_dam.h"
#include "wr_emm_mme.h"

#ifdef WR_PERF_MEAS 
EXTERN S16 wrPerfActvTmr (Void);
#endif

EXTERN S16 wrGenerateCfm (Pst *pst, Header *hdr, U8 prim, WrMngmt *cfm);
PUBLIC S16 wrNghCfg (WrNghCellCfgLst *nghCfgLst, CmStatus *status, Pst *pst);
PUBLIC S16 wrMmesCfg (WrMmeCfgLst *nghCfgLst, CmStatus *status, Pst *pst);
EXTERN PUBLIC S16 WrMiLwrAddNeighCfgReq(Pst *pst,LwrMngmt *cntrl);
S16  wrGenerateAddNeighCfgReq(LwrMngmt *cfm);

/*
 *       Fun:     wrSendLmAlarm
 *
 *       Desc:    Function trigger alarm
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None
 */
PUBLIC Void wrSendLmAlarm
(
U16                          category,           
U16                          event,             
U16                          cause            
)
{
   WrMngmt                   usta;

   if (wrCb.init.usta == FALSE)
   {
      RETVOID;
   }

   /* Initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(WrMngmt));

   usta.hdr.elmId.elmnt = STWRGEN;
   usta.hdr.entId.ent   = wrCb.init.ent;
   usta.hdr.entId.inst  = wrCb.init.inst;
   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event    = event;
   usta.u.usta.alarm.cause    = cause;

   /* Send an unsolicited status indication */
   WrMiLwrStaInd(&wrCb.init.lmPst, &usta);

   RETVOID;
} /* wrSendLmAlarm */

/*
 *       Fun:     wrActvInit
 *
 *       Desc:    
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  The task initializes its global variables.
 */
PUBLIC S16 wrActvInit
(
Ent                          entity,
Inst                         inst,
Region                       region,
Reason                       reason
)
{
   wrCb.init.procId  = SFndProcId();
   wrCb.init.ent     = entity;
   wrCb.init.inst    = inst;
   wrCb.init.region  = region;
   wrCb.init.reason  = reason;
   wrCb.init.cfgDone = FALSE;
   wrCb.init.pool    = 1;
   wrCb.init.acnt    = FALSE;
   wrCb.init.trc     = FALSE; 
   wrCb.init.usta    = TRUE;
   wrCb.resetCmfState= WR_INIT_STATE_NONE;
   wrCb.mem.region   = region;
   wrCb.mem.pool     = DFLT_POOL;

#ifdef RSYS_WIRESHARK
#if 0 /* Imp: Do not delete this code */
   wrInitWireShark();
#endif
#endif

   wrEmmInit();
   wrUmmInit();
   RETVALUE(ROK);
} /* wrActvInit */

/*
 *       Fun:     wrGenerateCfm 
 *
 *       Desc:    To generate confirms
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None.
 */
PUBLIC S16  wrGenerateCfm
(
Pst                          *pst,
Header                       *hdr,
U8                           prim,
WrMngmt                      *cfm
)
{
   Pst                       cPst;

   /* Fill the pst structure for LM i/f */
   cPst.srcEnt    = wrCb.init.ent;
   cPst.srcInst   = wrCb.init.inst;

   cPst.srcProcId = SFndProcId();

   /*
    * If genconfig is done, fill the destn identity
    * from the pst configured, else use the incoming
    * pst structure for destination identity.
    */
   if (TRUE == wrCb.init.cfgDone)
   {  
      cPst.dstEnt    = wrCb.init.lmPst.dstEnt;
      cPst.dstInst   = wrCb.init.lmPst.dstInst;
      cPst.dstProcId = wrCb.init.lmPst.dstProcId;
   }
   else
   {  
      cPst.dstEnt    = pst->srcEnt;
      cPst.dstInst   = pst->srcInst;
      cPst.dstProcId = pst->srcProcId;
   }

   cPst.selector  = hdr->response.selector;
   cPst.prior     = hdr->response.prior;
   cPst.route     = hdr->response.route;
   cPst.region    = hdr->response.mem.region;
   cPst.pool      = hdr->response.mem.pool;

   /* Fill the Header in cfm structure */
   cmMemset ((U8 *)&cfm->hdr, '\0', sizeof(Header));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent   = wrCb.init.ent;
   cfm->hdr.entId.inst  = wrCb.init.inst;

   /* Fill the transcation for which the cfm is being sent */
   cfm->hdr.transId = hdr->transId;

   switch (prim)
   {  
      case TCFG:
         cmMemset ((U8 *)&cfm->u.cfg, 0, sizeof(WrCfg));
         WrMiLwrCfgCfm (&cPst, cfm);
         break;   

      case TCNTRL:
         WrMiLwrCntrlCfm (&cPst, cfm);
         break;
   }  

   RETVALUE(ROK);
} /* wrGenerateCfm */

/********************************************************************IB**
 Layer Management Incoming Primitives
*********************************************************************IB*/

/*
 *       Fun:     wrUbndLSap
 *
 *       Desc:    This function is invoked to unbind & disable a SAP 
 *                A disconnect is sent and the SAP state is changed
 *                to WR_SAP_CFG without sending a unbind request.
 *
 *       Ret:     ROK or LCM_REASON_INVALID_STATE
 *
 *       Notes :  This function provides run time support for the
 *                stack reinitialization procedure.
 */
PRIVATE S16 wrUbndLSap
(
WrLiSapCb                    *sapCb
)
{
   S16                       retVal;

   TRC2(wrUbndLSap);

   switch(sapCb->sapState)
   {
      case LWR_SAP_BINDING:
      {
         sapCb->sapState = LWR_SAP_UNBOUND;
         retVal = ROK;
         break;
      }
      case LWR_SAP_BOUND:
      {
         sapCb->sapState = LWR_SAP_UNBOUND;
         retVal = ROK;
         break;
      }
      default:
      {
         retVal = LCM_REASON_INVALID_STATE;
         break;
      }
   }

   RETVALUE(retVal);
} /* wrUbndLSap */

/*
 *       Fun:     wrBndLSap
 *
 *       Desc:    This function binds the lower sap.
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  This function is a state transition action handler
 *                for lower SAP ABND_ENA control request. 
 */
PRIVATE S16 wrBndLSap
(
WrLiSapCb                    *sapCb,
CmStatus                     *status,
Elmnt                        elmnt
)
{
   if (LWR_SAP_UNBOUND == sapCb->sapState)
   {
      status->status = LCM_PRIM_OK_NDONE;
      sapCb->sapState = LWR_SAP_BINDING;
      sapCb->bndRetryCnt = 0;

      switch(elmnt)
      {
         case STWRNHUSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_NHU_SAP_BND, sapCb->bndTmr.val);
            WrIfmRrcBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
         case STWRRGRSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_RGR_SAP_BND, sapCb->bndTmr.val);
            WrIfmSchdBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
         case STWRCTFSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_CTF_SAP_BND, sapCb->bndTmr.val);
            WrIfmL1BndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
         case STWRSZTSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_SZT_SAP_BND, sapCb->bndTmr.val);
            WrIfmS1apBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
         case STWRCZTSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_CZT_SAP_BND, sapCb->bndTmr.val);
            WrIfmX2apBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
#ifdef RM_INTF
         case STWRRMUSAP: /*RRM->FSM sck changes*/
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_RMU_SAP_BND, sapCb->bndTmr.val);
            WrUiRmuBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
#endif /*RM_INTF*/
#ifdef EU_DAT_APP
         case STWREGTSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_EGT_SAP_BND, sapCb->bndTmr.val);
            WrIfmEgtpBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
         case STWRPJUSAP:
         {
            wrStartTmr ((PTR)sapCb, WR_TMR_PJU_SAP_BND, sapCb->bndTmr.val);
            WrIfmPdcpBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
#endif 
         default:
            break;
      }
   }
   else
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_STATE;
   }

   RETVALUE (ROK);
} /* wrBndLSap */

/*
 *       Fun:     wrLSapCntrl
 *
 *       Desc:    This function processes control request for the
 *                Lower SAP.
 *
 *       Ret:     ROK - Ok/RFAILED - Failure 
 *
 *       Notes :  This function implements the State transition
 *                for Lower SAP control requests. The function
 *                is the entry point for all control request
 *                events defined.
 *
 */
PRIVATE S16 wrLSapCntrl
(
WrCntrl                      *sapCntrl,
CmStatus                     *status,
Elmnt                        elmnt
)
{
   WrLiSapCb                 *lSapCb = NULLP;
   SuId                      lSapId;
   S16                       retVal;

   retVal = ROK;
   /* Validate the parameters */
   lSapId = sapCntrl->u.sapId;

   switch (elmnt) {
      case STWRRGRSAP:
      {
         lSapCb = wrCb.rgrSap[0];
         break;
      }
      case STWRNHUSAP:
      {
         lSapCb = wrCb.nhuSap[0];
         break;
      }
      case STWRCTFSAP:
      {
         lSapCb = wrCb.ctfSap[0];
         break;
      }
      case STWRSZTSAP:
      {
         lSapCb = wrCb.sztSap[0];
         break;
      }
      case STWRCZTSAP:
      {
         lSapCb = wrCb.cztSap[0];
         break;
      }
#ifdef RM_INTF
      case STWRRMUSAP: /*RRM->FSM sck changes*/
      {
         lSapCb = wrCb.rmuSap[0];
         break;
      }
#endif /*RM_INTF*/
#ifdef EU_DAT_APP
      case STWREGTSAP:
      {
         lSapCb = wrCb.egtSap[0];
         break;
      }
      case STWRPJUSAP:
      {
         lSapCb = wrCb.pjuSap[0];
         break;
      }
#endif
      default:
      {
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(RFAILED);
      }
   }

   /* Initialize the status members to success */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (sapCntrl->action)
   {
      case ABND_ENA:
      {
         /* Bind Lower Sap  */
         wrBndLSap(lSapCb, status, elmnt);
         retVal = ROK;
         break;
      }
      case AUBND_DIS:
      {
         /* Disconnect PVC and DONT send Unbind Request */
         retVal = wrUbndLSap(lSapCb);
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "wrLSapCntrl: Invalid Receiver\
                     State in call to nuUbndLSap");
            status->status = LCM_PRIM_NOK;
            status->reason = retVal;
         }
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "wrLSapCntrl: Invalid Action Value ");
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_ACTION;
         retVal = RFAILED;
         break;
      }
   }

   RETVALUE(retVal);
} /* wrLSapCntrl */

/*
 *       Fun:     wrShutdown
 *
 *       Desc:    
 *
 *       Ret:     None. 
 *
 *       Notes :  This function is invoked on a control request
 *                to shutdown the layer. All the data structures
 *                allocated during configuration are freed.
 */
PRIVATE Void wrShutdown
(
Void
)
{
   RLOG0(L_INFO, "wrShutdown: eNodeB Layer Shutdown");

   /* Reintialize the layer - the reason parameter is not used */
   wrActvInit(wrCb.init.ent, wrCb.init.inst, wrCb.init.region, 
             wrCb.init.reason);
   RETVOID;
} /* wrShutdown */

/*
 *       Fun:     wrGenCfg
 *
 *       Desc:    This function is invoked to do the general 
 *                configuration for RRC User Layer.
 *
 *       Ret:     ROK     - ok
 *                RFAILED - Failure
 *                ROUTRES - Memory Allocation Failure due to resource
 *                          crunch.
 *
 *       Notes :  The general configuration configures the lmPst
 *                to use in all calls to the Stack Manager. This
 *                function reserwrs the static memory requirements
 *                for the layer and configures the static data strs. 
 */
PRIVATE S16 wrGenCfg 
(
WrGenCfg                     *gen,
CmStatus                     *status
)
{
   WrDamCfg                  damCfg;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (TRUE != wrCb.init.cfgDone)
   {
      cmMemcpy((U8 *)&(wrCb.init.lmPst), (U8 *)&(gen->lmPst),
            (PTR)sizeof(Pst));

      wrCb.init.lmPst.srcProcId = gen->lmPst.srcProcId;
      wrCb.init.lmPst.srcEnt    = gen->lmPst.srcEnt;
      wrCb.init.lmPst.srcInst   = gen->lmPst.srcInst;
      wrCb.init.lmPst.event     = EVTNONE;
      wrCb.init.cfgDone = TRUE;
   }
   /* Filling the eNB Type */
   wrEmmCb.enbType = gen->eNBType;
   /*-Fill eNodeB oveload related Parameters-*/
#ifdef ENB_OVRLOAD
   wrEmmCb.eNBOvldParams.critThrldVal = gen->eNBOvldParams.critThrldVal;
   wrEmmCb.eNBOvldParams.majThrldVal = gen->eNBOvldParams.majThrldVal;
   wrEmmCb.eNBOvldParams.minThrldVal = gen->eNBOvldParams.minThrldVal;
   wrEmmCb.eNBOvldParams.critRcvryVal = gen->eNBOvldParams.critRcvryVal;
   wrEmmCb.eNBOvldParams.majRcvryVal = gen->eNBOvldParams.majRcvryVal;
   wrEmmCb.eNBOvldParams.minRcvryVal = gen->eNBOvldParams.minRcvryVal;
   wrEmmCb.eNBOvldParams.eNBOvldMonTimerVal = gen->eNBOvldParams.eNBOvldMonTimerVal;
   wrEmmCb.eNBOvldParams.eNBOvldMonfreq = gen->eNBOvldParams.eNBOvldMonfreq;
   wrEmmCb.eNBOvldParams.eNbOvrLdFlag = gen->eNBOvldParams.eNbOvrLdFlag;
   wrEmmCb.eNBOvldParams.maxCntval = gen->eNBOvldParams.maxCntval;
   
   if((wrEmmCb.eNBOvldParams.eNbOvrLdFlag)
         && (wrEmmCb.eNBOvldParams.eNBOvldMonTimerVal != 0)
         && (wrEmmCb.eNBOvldParams.eNBOvldMonfreq > 1))
   {

      cmInitTimers(&(wrEmmCb.smplTimer), 1);
      cmInitTimers(&(wrEmmCb.eNbOvrldStatUpd), 1);
      /*-If Feature is enabled, Start Sampling and Status Updating Timer-*/
      wrStartTmr((PTR)&wrEmmCb,WR_TMR_OVERLD_SAMPLE,wrEmmCb.eNBOvldParams.eNBOvldMonTimerVal);
      wrStartTmr((PTR)&wrEmmCb,WR_TMR_OVERLD_STATUS,
            wrEmmCb.eNBOvldParams.eNBOvldMonTimerVal * wrEmmCb.eNBOvldParams.eNBOvldMonfreq);

      /*-Initalize the Sample Values-*/
      wrEmmCb.memSample = 0;
      wrEmmCb.cpuSample = 0;
      wrEmmCb.eNBOvldParams.ovldCntVal = 0;
      wrEmmCb.eNBOvldParams.prevOvldFlag = NORMAL;
   }
#endif
   wrCpyCmTptAddr(&wrEmmCb.datAppAddr,&gen->dataAppAddr);
   wrEmmCb.init.cfgDone = TRUE;
    
   if (wrRegInitTmr() != ROK)
   {
      RLOG0(L_ERROR, "wrGenCfg: wrRegInitTmr Failed");
      RETVALUE (RFAILED);
   }

  #if 0 /* the same operation doing by func wrUmmInitRabParams */
   wrEnbParamsInit();
  #endif
   /* This must be replaced by a LM primitive */
   /*wrEmmAddCell(gen->cellCfg.cellId); */ 
   /* DAM Config */
   damCfg.inactvTmrVal = gen->inactvTmrVal;
   damCfg.expiryCnt    = gen->expiryCnt;
   damCfg.endMrkTmrVal = gen->endMrkTmrVal;
   wrIfmDamCfg(&damCfg);

   RETVALUE(ROK);
} /* wrGenCfg */

/*
 *       Fun:     wrLSapCfg
 * 
 *       Desc:    This function is used to configure the Lower SAP.
 * 
 *       Ret:     ROK - ok / RFAILED - Failure 
 *                / ROUTRES - Memory out of resources.
 *
 *       Notes :  None.
 */
PRIVATE S16 wrLSapCfg
(     
WrMngmt                      *cfg,
CmStatus                     *status
)
{
   WrLiSapCb *sap = NULLP;
   WrLSapCfg *lSap = &cfg->u.cfg.u.lSap;

   TRC2(wrLSapCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (NULLP == lSap)
   {
      RLOG0(L_ERROR, "wrLSapCfg: Invalid Sap Cfg ");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_PAR_VAL;
      RETVALUE(RFAILED);
   }

   switch(cfg->hdr.elmId.elmnt)
   {
      case STWRNHUSAP:
      {
         /* Allocate memory */
         WR_ALLOC(&wrCb.nhuSap, sizeof(WrLiSapCb *));
         if(wrCb.nhuSap == NULLP)
         {       
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         } /* end of if statement */

         WR_ALLOC(&wrCb.nhuSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.nhuSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.nhuSap[0];
         break;
      }
      case STWRRGRSAP:
      {
         /* Allocate memory */
         WR_ALLOC(&wrCb.rgrSap, sizeof(WrLiSapCb *));
         if(wrCb.rgrSap == NULLP)
         {       
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         } /* end of if statement */

         WR_ALLOC(&wrCb.rgrSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.rgrSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.rgrSap[0];
         break;
      }
      case STWRCTFSAP:
      {
         /* Allocate memory */
         WR_ALLOC(&wrCb.ctfSap, sizeof(WrLiSapCb *));
         if(wrCb.ctfSap == NULLP)
         {       
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         } /* end of if statement */
         WR_ALLOC(&wrCb.ctfSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.ctfSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.ctfSap[0];
         break;
      }
      case STWRSZTSAP:
      {
         /* Allocate memory */
         WR_ALLOC(&wrCb.sztSap, sizeof(WrLiSapCb *));
         if(wrCb.sztSap == NULLP)
         {       
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         } /* end of if statement */
         WR_ALLOC(&wrCb.sztSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.sztSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.sztSap[0];
         if(wrEmmCb.mmeCont.mmes[0])
            wrEmmCb.mmeCont.mmes[0]->s1apSap = wrCb.sztSap[0];
         break;
      }
#ifdef LTE_HO_SUPPORT
      case STWRCZTSAP:
      {
         /* Allocate memory */
         WR_ALLOC(&wrCb.cztSap, sizeof(WrLiSapCb *));
         if(NULLP == wrCb.cztSap)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&wrCb.cztSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.cztSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.cztSap[0];
         break;
      }
#endif
#ifdef EU_DAT_APP
      case STWREGTSAP:
      {
         /* Allocate memory */
         WR_ALLOC(&wrCb.egtSap, sizeof(WrLiSapCb *));
         if(NULLP == wrCb.egtSap)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&wrCb.egtSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.egtSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.egtSap[0];
         break;
      }
      case STWRPJUSAP:
      {
         WR_ALLOC(&wrCb.pjuSap, sizeof(WrLiSapCb *));
         if(NULLP == wrCb.pjuSap)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&wrCb.pjuSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.pjuSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.pjuSap[0];
         break;
      }
#endif 
#ifdef RM_INTF
      case STWRRMUSAP: /*RRM->FSM sck changes*/
      {
         WR_ALLOC(&wrCb.rmuSap, sizeof(WrLiSapCb *));
         if(NULLP == wrCb.rmuSap)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&wrCb.rmuSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrCb.rmuSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         sap = wrCb.rmuSap[0];
         break;
      }
#endif /*RM_INTF*/


      
      default:
      {
         RLOG0(L_ERROR, "wrLSapCfg: Invalid Sap Element");
         RETVALUE(RFAILED);
         break;
      }
   }

   sap->suId = lSap->suId;
   sap->spId = lSap->spId;
   sap->pst.srcProcId = SFndProcId();
   sap->pst.srcEnt = lSap->srcEnt;
   sap->pst.srcInst = lSap->srcInst;
   sap->pst.dstProcId = lSap->dstProcId;
   sap->pst.dstEnt = lSap->dstEnt;
   sap->pst.dstInst = lSap->dstInst;
   sap->pst.prior = lSap->dstPrior;
   sap->pst.route = lSap->dstRoute;
   sap->pst.selector = lSap->dstSel;
   sap->mem.region = lSap->mem.region;
   sap->mem.pool = lSap->mem.pool;
   sap->maxBndRetry = lSap->maxBndRetry;
   sap->bndTmr.val = lSap->bndTmr.val;
   sap->bndTmr.enb = lSap->bndTmr.enb;
   sap->sapState = LWR_SAP_UNBOUND;
   cmInitTimers(&sap->timer , 1);

   RETVALUE(ROK);
} /* wrLSapCfg */


/*
 *       Fun:     WrMiLwrCfgReq
 *
 *       Desc:    This primitive is called by LM to configure the 
 *                Framing Protocol User 
 *
 *       Ret:     ROK/RFAILED
 *
 *       Notes :  All configuration requests are handled from this
 *                function. 
 */
PUBLIC S16 WrMiLwrCfgReq
(
Pst                          *pst,
WrMngmt                      *cfg
)
{
   WrMngmt                   cfmCfg;
   S16                       retVal;

   TRC3(WrMiLwrCfgReq);

   RLOG1(L_ERROR, "WrMiLwrCfgReq(pst, elmnt(%d))", cfg->hdr.elmId.elmnt);
   if (NULLP == cfg)
   {
      RLOG0(L_ERROR, "WrMiLwrCfgReq: NULL Management structure");
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is already done */
   if ((!wrCb.init.cfgDone) && (STWRGEN != cfg->hdr.elmId.elmnt))
   {
      RLOG0(L_ERROR, "WrMiLwrCfgReq: General configuration not done");

      /* Send Config Confirm to LM with NOK status */
      cfmCfg.cfm.status = LCM_PRIM_NOK;
      cfmCfg.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      wrGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);

      RETVALUE(RFAILED);
   }

   retVal = ROK;
   switch (cfg->hdr.elmId.elmnt)
   {
      case STWRGEN:
      {
         retVal = wrGenCfg(&cfg->u.cfg.u.genCfg, &cfmCfg.cfm);
         break;
      }
      case STWRNGH:
      {
         retVal = wrNghCfg(&cfg->u.cfg.u.NghCellCfgLst, &cfmCfg.cfm, pst);
         break;
      }
      case STWRMME:
      {
         retVal = wrMmesCfg(&cfg->u.cfg.u.mmeCfgLst, &cfmCfg.cfm, pst);
         break;
      }
      case STWRCTFSAP:
      case STWRNHUSAP:
      case STWRRGRSAP:
      case STWRSZTSAP:
      case STWRCZTSAP:
#ifdef RM_INTF
      case STWRRMUSAP: /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
#ifdef EU_DAT_APP
      case STWREGTSAP:
      case STWRPJUSAP:
#endif
      {
         retVal = wrLSapCfg(cfg, &cfmCfg.cfm);
         break;
      }
      default:
      {
         /* Send Config Confirm to LM with NOK status */
         RLOG0(L_ERROR, "WrMiLwrCfgReq: Invalid Element");
         cfmCfg.cfm.status = LCM_PRIM_NOK;
         cfmCfg.cfm.reason = LCM_REASON_INVALID_ELMNT;
         retVal = RFAILED;
         break;
      }
   }

   wrGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);

   RETVALUE(retVal);
} /* WrMiLwrCfgReq */

/*
 *       Fun:     WrMiLwrCntrlReq
 *
 *       Desc:    This primitive is called by LM to issue control 
 *                requests to the RRC User layer
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 WrMiLwrCntrlReq
(
Pst                          *pst,
WrMngmt                      *cntrl
)
{
   U16                        idx = 0;
   WrMngmt                    cntrlCfm;
   S16                        retVal = ROK;
   U16                        mmeIdx = 0;
   U8                         cfgLyr = FALSE;

   TRC3(WrMiLwrCntrlReq);

   cmMemset((U8 *) &cntrlCfm, (U8) 0, (S16) sizeof(WrMngmt));
   RLOG1(L_ERROR, "WrMiLwrCntrlReq(pst, elmnt(%d))", cntrl->hdr.elmId.elmnt);

   /* validate input parameters */
   if (NULLP == cntrl)
   {
      RLOG0(L_ERROR, "WrMiLwrCntrlReq: NULL Management structure");

      RETVALUE(RFAILED);
   }

   /* Is genconfig done  */
   if (TRUE != wrCb.init.cfgDone) 
   {
      RLOG0(L_ERROR, "WrMiLwrCntrlReq: General configuration not done");
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
      cntrlCfm.u.cntrl.subAction = cntrl->u.cntrl.subAction;

      wrGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&(wrCb.lmHdr), (U8 *)(&cntrl->hdr), sizeof(Header));

   cntrlCfm.cfm.status = LCM_PRIM_OK;
   cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;

   /* Identify the element for control */
   switch (cntrl->hdr.elmId.elmnt)
   { 
      case STWRGEN: 
      {
         switch (cntrl->u.cntrl.action)
         {
            case ASHUTDOWN:
            {
               wrShutdown();
               break;
            }
            case AENA:
            {
               switch(cntrl->u.cntrl.subAction)
               {
                  case SAUSTA:  /* Sub Action Unsolicited Status Alarm */
                  {
                     wrCb.init.usta = TRUE;
                     break;
                  }
                  case SADBG:   /* Sub Action DEBUG */
                  {
                     wrCb.init.dbgMask = cntrl->u.cntrl.u.dbgMask;
                     break;
                  }
                  case ADISIMM:        /* Action DISABLE */
                  {
                     wrCb.init.dbgMask &= ~(cntrl->u.cntrl.u.dbgMask);
                     break;
                  }
                  default:
                     break;
               }
               break;
            }
         }
         break;
      }
      case STWRRGRSAP:
      case STWRCTFSAP:
      case STWRNHUSAP: 
      case STWRSZTSAP:
      case STWRCZTSAP:
#ifdef RM_INTF
      case STWRRMUSAP: /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
      {
         retVal = wrLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm,
                                                cntrl->hdr.elmId.elmnt);
         break;
      }
#ifdef EU_DAT_APP
      case STWREGTSAP:
      case STWRPJUSAP:
      {
         retVal = wrLSapCntrl(&(cntrl->u.cntrl), 
                                  &(cntrlCfm.cfm),cntrl->hdr.elmId.elmnt);
         break;
      }
#endif
      case STWRCELLCFG:
      {
         while (mmeIdx < wrEmmCb.mmeCont.numMmes)
         {
            if(wrEmmCb.mmeCont.mmes[idx])
            {
               if(wrEmmCb.mmeCont.mmes[idx]->state == WR_MME_UP)
               {
                  cfgLyr = TRUE;
                  break;
               }
               mmeIdx++;
            }
            idx++;
         }
          
         if(cfgLyr == TRUE)
         {
            retVal = wrFillNhuCellCfgReq(wrEmmCb.cellCb[0]);
         }
         else
         {
            cntrlCfm.cfm.status = LCM_PRIM_NOK;
            cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;
            RLOG0(L_ERROR, "WrMiLwrCntrlReq: S1 Setup Not Done Yet!");
         }
         break;
      }
      case STWRS1CONSETUP:
      {
         /* Triggering S1 Setup Request  */
         while (mmeIdx < wrEmmCb.mmeCont.numMmes)
         {
            if(wrEmmCb.mmeCont.mmes[idx])
            {
               if (wrEmmSetupS1(wrEmmCb.mmeCont.mmes[idx]->mmeId) != ROK)
               {
                  RETVALUE(RFAILED);
               }
               mmeIdx++;
            }
            idx++;
         }
         break;
      }
      /* CNM_DEV start */
      case STWRINITALSYNC:
      {
         wrEmmCnmStartInitalSync();
         break;
      }
      /* CNM_DEV end */

      case STWRX2CONSETUP:
      {
         break;
      }
      case STWRSNDERABRELIND:
      {
         /*Handling of ERAB RELEASE INDICATION*/
         wrUmmPrcErabRelIndctrl();
      }    
      default:
      {
         RLOG0(L_ERROR, "WrMiLwrCntrlReq: Invalid Element");
         retVal = RFAILED;
         break;
      }
   }

   if(retVal == RFAILED)
   {
      RLOG0(L_ERROR, "WrMiLwrCntrlReq: Action Failed ");
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
   }
   cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
   cntrlCfm.u.cntrl.subAction = cntrl->u.cntrl.subAction;

   wrGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

   RETVALUE(retVal);
} /* WrMiLwrCntrlReq */

/*
 *       Fun:     wrNghCfg
 *
 *       Desc:    This primitive is called by LM to configure the 
 *                Neighbor information in APPL. 
 *
 *       Ret:     ROK/RFAILED
 */
PUBLIC S16 wrNghCfg
(
WrNghCellCfgLst              *nghCfgLst,
CmStatus                     *status,
Pst                          *pst
)
{
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (TRUE != wrCb.init.cfgDone)
   {
      RLOG0(L_ERROR, "wrNghCfg :GenCfg not done");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* wrNghCfg */


/*
 *       Fun:     wrMmesCfg
 *
 *       Desc:    This primitive is called by LM to configure the 
 *                MME information in APPL. 
 *
 *       Ret:     ROK/RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrMmesCfg
(
WrMmeCfgLst  *mmeCfgLst,
CmStatus     *status,
Pst          *pst
)
#else
PUBLIC S16 wrMmesCfg(mmeCfgLst, status, pst)
WrMmeCfgLst  *mmeCfgLst;
CmStatus     *status;
Pst          *pst;
#endif
{
   U32                  idx    = 0;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (TRUE != wrCb.init.cfgDone)
   {
      RLOG0(L_ERROR, "wrNghCfg : GenCfg not done");
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < mmeCfgLst->noOfCfg ; idx++)
   {
      wrEmmAddMme(mmeCfgLst->mmeCfg[idx]->mmeId);
   }

   /* update number of MME's configured */
   wrEmmCb.mmeCont.numMmes += mmeCfgLst->noOfCfg;

   /* Freeing the configuration pointers */
   SPutSBuf(pst->region, pst->pool, (Data *)mmeCfgLst->mmeCfg, 
                                sizeof(WrMmeCfg) * mmeCfgLst->noOfCfg);
   RETVALUE(ROK);
}

/*
 *  Fun:     wrGenerateAddNeighCfgReq
 *  
 *  Desc:    To generate confirms
 *  
 *  Ret:     ROK   - ok
 *  
 *  Notes :  None.
 */
#ifdef ANSI
PUBLIC S16  wrGenerateAddNeighCfgReq
(
 LwrMngmt                  *cfm
)
#else
PUBLIC S16  wrGenerateAddNeighCfgReq(LwrMngmt *cfm)
LwrMngmt *cfm;
#endif
{
  /*
   * Fill the Header in cfm structure 
   *cmMemset ((U8 *)&cfm->hdr, '\0', sizeof(Header));
   *cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   *cfm->hdr.entId.ent   = wrCb.init.ent;
   *cfm->hdr.entId.inst  = wrCb.init.inst;

   ** Fill the transcation for which the cfm is being sent 
   *cfm->hdr.transId = hdr->transId;
   *cmMemset ((U8 *)&cfm->u.cfg, 0, sizeof(WrCfg));
   */
   WrMiLwrAddNeighCfgReq (&wrCb.init.lmPst , cfm);
   RETVALUE(ROK);
} /* wrGenerateAddNeighCfgReq */



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
