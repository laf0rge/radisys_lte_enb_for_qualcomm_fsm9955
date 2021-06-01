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

     Type:     

     Desc:     This file contains 
               

     File:     nl_lmm.c

     Sid:      bk_lmm.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:16 2014

     Prg:      subhamay 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=249;
static int RLOG_MODULE_ID=256;

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"        /* Common Pasn defines */
#include "rl_common.h"     /* RLOG defines */   
#include "rl_interface.h"   
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

/* local typedefs */
 
/* local externs */

/* forward references */

PRIVATE U16 nlLnlGenCfg ARGS((
NlCfg          *cfg       /* configuration information */
));

PRIVATE U16 nlNluSapCfg ARGS((

NlCfg          *cfg       /* Configuaration information */
));

PRIVATE S16 nlCellCfg ARGS ((
LnlCellCfg   *cellCfg   /* cell config structure */
));

PRIVATE S16 nlNeighborCellCfg ARGS((
LnlNghCellCfg    *nghCellCfg  /* neighbor cell config structure  */
));

PRIVATE S16 nlCellReCfg ARGS((
LnlCellReCfg     *cellReCfg   /* cell reconfig structure  */
));

PRIVATE Void nlLnlFillCfmPst ARGS((
Pst           *cfmPst,  /* post struture */
Pst           *reqPst,  /* post structure */
NlMngmt       *cfg      /* config request structure */
));

PRIVATE Void nlShutdown ARGS ((
Void
));

PRIVATE S16 nlProcessCellUpInd ARGS ((
LnlEnbStaInd  *statInd
));

PRIVATE S16 nlCopyLnlNghCellInfo ARGS((
LnlNghCellCfg    *srcCellCfg,  /* src neighbor cell config structure  */
NlTmpNghCellCfg  *tgtCellCfg   /* target neighbor cell structure*/
));

PRIVATE S16 nlCopyLnlNghCellAddInfo ARGS((
LnlNghCellAdd    *srcCellAdd,  /* src neighbor cell config structure  */
NlTmpNghCellAdd  *tgtCellAdd   /* target neighbor cell structure*/
));

PRIVATE S16 nlCopyLnlNghCellDelInfo ARGS((
LnlNghCellDel    *srcCellDel,  /* src neighbor cell config structure  */
NlTmpNghCellDel  *tgtCellDel   /* target neighbor cell structure*/
));

PRIVATE S16 nlCopyLnlNghCellModInfo ARGS((
LnlNghCellMod    *srcCellMod,  /* src neighbor cell config structure  */
NlTmpNghCellMod  *tgtCellMod   /* target neighbor cell structure*/
));

/**
 * @brief Layer Manager Configuration request handler.
 *
 * @details
 *
 *     Function : NlMiLnlCfgReq
 *
 *     This function handles the configuration
 *     request received from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions nlHdlGenCfg() or nlHdlSapCfg().
 *     -# Invokes NlMiLnlCfgCfm() to send back the confirmation to the LM.
 *
 *  @param[in]  Pst     : the post structure
 *  @param[in]  NlMngmt : the configuration parameter's structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 NlMiLnlCfgReq
(
Pst      *pst,    /* post structure  */
NlMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 NlMiLnlCfgReq(pst, cfg)
Pst      *pst;    /* post structure */
NlMngmt  *cfg;    /* config structure */
#endif
{
   S16            ret    = LCM_PRIM_OK;
   S16            reason = LCM_REASON_NOT_APPL;
   NlMngmt        *cfm   = NULLP;
   Pst            cfmPst;
   TRC3(NlMiLnlCfgReq);
   NL_ALLOC(NL_REGION, NL_POOL, &cfm, sizeof(NlMngmt));
   if(NULLP == cfm)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for confirm");
      NL_FREE(pst->region, pst->pool, cfg, sizeof(NlMngmt));
      RETVALUE(RFAILED);
   }
   /*create the confirm post structure*/
   nlLnlFillCfmPst(&cfmPst, pst, cfg);
   cfm->hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   cfm->hdr.transId = cfg->hdr.transId;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STNLGEN :
      {
         reason = nlLnlGenCfg(&cfg->u.cfg);
      }
      break;
     
      case STNLUSAP :
      {
         reason = nlNluSapCfg(&cfg->u.cfg);
      }
      break;
      case STNLPROTOCFG :
      {
         switch(cfg->u.protoCfg.bCfgType)
         {
            case LNL_CELL_CFG :
            { 
               reason = nlCellCfg(&cfg->u.protoCfg.u.cellCfg);
            }
            break;
            case LNL_NGH_CELL_CFG :
            {
               reason = nlNeighborCellCfg(&cfg->u.protoCfg.u.nghCellCfg);
            }
            break;
            case LNL_CELL_RECFG :
            {
               reason = nlCellReCfg(&cfg->u.protoCfg.u.cellReCfg);
            }
            break;
            default :
            {
               ret = LCM_PRIM_NOK;
               reason = LCM_REASON_INVALID_ELMNT;
            }
            break;
         }     
      }
      break;
      default :
      {
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
      }
      break;
   }
   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm->cfm.status = ret;
   cfm->cfm.reason = reason;
   /* send the response to SM */
   NlMiLnlCfgCfm(&cfmPst, cfm);
   /* free the memory allocated at SM */
   NL_FREE(pst->region, pst->pool, cfg, sizeof(NlMngmt));
   RETVALUE(ROK);
}

/**
 * @brief SAP Configuration Handler.
 *
 * @details
 *
 *     Function : nlLnlFillCfmPst
 *
 *     This function in called by NlMiLnlCfgReq(). It will fill
 *     the post structre which will be used to send config confirm
 *     message from SON to SM
 *
 *  @param[in]  Pst     *reqPst, post structure received in config request
 *  @param[in]  Pst     *cfmPst, post structure which will be used to send 
 *                           confirm message
 *  @param[in]  NlMngmt *cfg, config requst structure  
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void nlLnlFillCfmPst
(
Pst           *cfmPst,  /* post struture */
Pst           *reqPst,  /* post structure */
NlMngmt       *cfg      /* config request structure */
#else
PRIVATE Void nlLnlFillCfmPst(reqPst, cfmPst, cfg)
Pst           *reqPst;  /* post structure */
Pst           *cfmPst;  /* post structure */
NlMngmt       *cfg;     /* config request structure */
#endif
)
{
   TRC3(nlLnlFillCfmPst)

   cfmPst->srcEnt    = nlCb.init.ent;
   cfmPst->srcInst   = nlCb.init.inst;
   cfmPst->srcProcId = nlCb.init.procId;
   cfmPst->dstEnt    = reqPst->srcEnt;
   cfmPst->dstInst   = reqPst->srcInst;
   cfmPst->dstProcId = reqPst->srcProcId;

   cfmPst->selector  = cfg->hdr.response.selector;
   cfmPst->prior     = cfg->hdr.response.prior;
   cfmPst->route     = cfg->hdr.response.route;
   cfmPst->region    = cfg->hdr.response.mem.region;
   cfmPst->pool      = cfg->hdr.response.mem.pool;
   
   RETVOID;
}


/**
 * @brief SAP Configuration Handler.
 *
 * @details
 *
 *     Function : nlNluSapCfg
 *
 *     This function in called by NlMiLnlCfgReq(). It handles the
 *     interface SAP configuration between SON and eNBAPP. It
 *     initializes the sapState to LNL_UNBND. Returns
 *     reason for success/failure of this function.
 *
 *  @param[in]  NlCfg *cfg, the Configuaration information
 *  @return  U16
 *      -# LCM_REASON_GENCFG_NOT_DONE
 *      -# LCM_REASON_INVALID_PAR_VAL
 *      -# LCM_REASON_NOT_APPL
 **/
#ifdef ANSI
PRIVATE U16 nlNluSapCfg
(
NlCfg   *cfg       /* Configuaration information */
)
#else
PRIVATE U16 nlNluSapCfg(cfg)
NlCfg   *cfg;      /* configuration information */
#endif
{
   U16               ret       = LCM_REASON_NOT_APPL;
   NlUiSapCb         *upSapCfg = NULLP;

   TRC2(nlNluSapCfg)

   /* Check if Gen Config has been done */
   if(nlCb.init.cfgDone != TRUE)
   {
      RLOG0(L_ERROR, "NLU SAP configuration received before Gen config");
      RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }
   if((cfg->u.nluSap.selector != NLU_SEL_LWLC) &&
      (cfg->u.nluSap.selector != NLU_SEL_TC) &&
      (cfg->u.nluSap.selector != NLU_SEL_LC))
   {
      RLOG0(L_ERROR, "Invalid NLU SAP seletor value received");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   if(LNL_NOT_CFG == nlCb.nluSap.sapState)
   {
      nlCb.nluSap.sapState = LNL_UNBND;
   }
   upSapCfg = &nlCb.nluSap;

   upSapCfg->pst.dstEnt = cfg->u.nluSap.ent;
   upSapCfg->pst.dstInst = cfg->u.nluSap.inst;
   upSapCfg->pst.dstProcId = cfg->u.nluSap.procId;
   upSapCfg->pst.srcEnt = nlCb.init.ent;
   upSapCfg->pst.srcInst = nlCb.init.inst;
   upSapCfg->pst.srcProcId = nlCb.init.procId;
   upSapCfg->pst.region = cfg->u.nluSap.mem.region;
   upSapCfg->pst.pool = cfg->u.nluSap.mem.pool;
   upSapCfg->pst.selector = cfg->u.nluSap.selector;
   upSapCfg->pst.route = cfg->u.nluSap.route;
   upSapCfg->pst.intfVer = 0;
   upSapCfg->pst.prior = cfg->u.nluSap.prior;
   upSapCfg->suId = cfg->u.nluSap.suId;
   upSapCfg->spId = cfg->u.nluSap.spId;

   RETVALUE(ret);
}

/**
 * @brief General Configuration Handler.
 *
 * @details
 *
 *     Function : nlLnlGenCfg
 *
 *     This function in called by NlMiLnlCfgReq(). It handles the
 *     general configuration of the LNL layer. 
 *
 *  @param[in]  NlCfg *cfg, the Configuaration information
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_INVALID_PAR_VAL
 **/
#ifdef ANSI
PRIVATE U16 nlLnlGenCfg
(
NlCfg   *cfg   /* Configuaration information */
)
#else
PRIVATE U16 nlLnlGenCfg(cfg)
NlCfg   *cfg;  /* configuration information */      
#endif
{
   Pst    *lmPst = NULLP;
   U16    ret = LCM_REASON_NOT_APPL;
   
   TRC3(nlLnlGenCfg)
   
   lmPst = &(cfg->u.genCfg.lmPst);

   /* Check if General Configuration is done already */
   if (TRUE == nlCb.init.cfgDone)
   {
      RLOG0(L_ERROR, "Gen config received second time");
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }
   if ((lmPst->selector != LNL_SEL_TC) &&
       (lmPst->selector != LNL_SEL_LC) &&
       (lmPst->selector != LNL_SEL_LWLC))
   {
      RLOG0(L_ERROR, "Invalid LNL SAP selector value received");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /* Update the Pst structure for LNL interface */   
   NL_MEM_COPY((U8 *)&nlCb.init.lmPst, (U8 *)lmPst,
             sizeof(Pst));

   nlCb.init.lmPst.srcProcId = nlCb.init.procId;
   nlCb.init.lmPst.srcEnt = nlCb.init.ent;
   nlCb.init.lmPst.srcInst = nlCb.init.inst;
   nlCb.init.lmPst.event = EVTNONE;

   nlCb.genCfg.pciSelType = cfg->u.genCfg.pciSelType;
   nlCb.genCfg.prachSelType = cfg->u.genCfg.prachSelType;
   nlCb.genCfg.earfcnSelType = cfg->u.genCfg.earfcnSelType;
   nlCb.genCfg.txPowerSelType = cfg->u.genCfg.txPowerSelType;
   RLOG3(L_INFO, 
     "Configured PCI Sel Type: %d Earfcn Sel Type: %d Prach Sel Type: %d",
      nlCb.genCfg.pciSelType, nlCb.genCfg.earfcnSelType, nlCb.genCfg.prachSelType);
   nlCb.genCfg.maxUeSupp = cfg->u.genCfg.maxUeSupp;
   nlCb.genCfg.maxUePerTti = cfg->u.genCfg.maxUePerTti;

   /* Initialize SAP States */
   nlCb.nluSap.sapState = LNL_NOT_CFG;

   /* Set Config done in TskInit */
   nlCb.init.cfgDone = TRUE;

   /*Initiate the connection with vendor specific DSON server*/
   if(ROK != nlVendorDSonConnEstReq())
   {      
      RLOG0(L_ERROR, "Established Connection with Vendor DSON Failed");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   else
   {
      RLOG0(L_INFO, "Connection with Vendor DSON successfully established");
   }
   nlCb.timeRes = cfg->u.genCfg.tmrRes;
   if (ROK != nlRegInitTmr())
   {
      RLOG0(L_ERROR, "nlGenCfg: nlRegInitTmr Failed");
      RETVALUE (RFAILED);
   }
   RETVALUE(ret);
}

/**
 * @brief Layer Manager Control request handler.
 *
 * @details
 *
 *     Function : NlMiLnlCntrlReq
 *
 *     This function handles the control
 *     request received from the Layer Manager.
 *     -# Invokes NlMiLnlCntrlCfm() to send back the confirmation to the LM.
 *
 *  @param[in]  Pst     : the post structure
 *  @param[in]  NlMngmt : the control parameter's structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 NlMiLnlCntrlReq
(
Pst      *pst,      /* post structure  */
NlMngmt  *nlMngmt   /* control message structure  */
)
#else
PUBLIC S16 NlMiLnlCntrlReq(pst, nlMngmt)
(
Pst      *pst;      /* post structure */
NlMngmt  *nlMngmt;  /* control message structure */
)
#endif
{
   U16      ret    = LCM_PRIM_OK;
   U16      reason = LCM_REASON_NOT_APPL;
   NlMngmt  *cfm   = NULLP;
   Pst      cfmPst;
   NlCntrl  *cntrl = NULLP;
   /*TODO need to inform specific reason for shutdown*/

   TRC3(NlMiLnlCntrlReq);
   
   NL_ALLOC(NL_REGION, NL_POOL, &cfm, sizeof(NlMngmt));
   if(NULLP == cfm)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for control confirm");
      NL_FREE(pst->region, pst->pool, cntrl, sizeof(NlMngmt));
      RETVALUE(RFAILED);
   }
   /*create the confirm post structure*/
   nlLnlFillCfmPst(&cfmPst, pst, nlMngmt);
   cfm->hdr.elmId.elmnt = nlMngmt->hdr.elmId.elmnt;
   cntrl = &(nlMngmt->u.cntrl);
   if(ASHUTDOWN == cntrl->action)
   {
      /*delete all the configurations as well as stored data*/
      nlShutdown(); 
   }
   cfm->cfm.status = ret;
   cfm->cfm.reason = reason;
     
   /* send the response to SM */
   NlMiLnlCntrlCfm(&cfmPst, cfm);
   /* free the memory allocated at SM */
   NL_FREE(pst->region, pst->pool, nlMngmt, sizeof(NlMngmt));

   RETVALUE(ROK);
}

/**
 * @brief Layer Manager Cell Up indication handler.
 *
 * @details
 *
 *     Function : nlProcessCellUpInd
 *
 *     This function handles the CellUp Indication
 *     request received from the Layer Manager.
 *
 *  @param[in]  statInd     : cell up status indication
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlProcessCellUpInd
(
LnlEnbStaInd   *statInd    /*cell up status indication*/
)
#else
PRIVATE S16 nlProcessCellUpInd(statInd)
LnlEnbStaInd   *statInd;    /*cell up status indication*/
#endif
{
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager Configuration request handler.
 *
 * @details
 *
 *     Function : NlMiLnlStatInd
 *
 *     This function handles the CellUp Indication
 *     request received from the Layer Manager.
 *     
 *  @param[in]  Pst     : the post structure
 *  @param[in]  NlMngmt : the configuration parameter's structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 NlMiLnlStatInd
(
Pst      *pst,    /* post structure  */
NlMngmt  *statInd /* config structure  */
)
#else
PUBLIC S16 NlMiLnlStatInd(pst, statInd)
Pst      *pst;    /* post structure */
NlMngmt  *statInd;/* config structure */
#endif
{
   S16           ret = ROK;
   NlUsta        *usta = NULLP;
   TRC3(NlMiLnlStatInd);
   usta = &(statInd->u.usta);
   do
   {
      switch(usta->staType)
      {
         case LNL_STAT_ENB :
         {
            switch(usta->u.stEnbStaInfo.enStaType)
            {
               case LNL_ENBSTATYPE_CELLUP :
               {
                  ret = nlProcessCellUpInd(&usta->u.stEnbStaInfo);
                  break;
               }
               case LNL_ENBSTATYPE_CPULOAD :
               {

                  break;
               }
               default:
               {
                  RLOG0(L_ERROR, "Invalid eNB status Indication received");
                  ret = RFAILED;
                  break;
               }
            }
            break;
         }
         case LNL_STAT_ALARM :
         {

            break;
         }
         default:
         {
            RLOG0(L_ERROR, "Invalid status type received from SM");
            ret = RFAILED;
            break;
         }
      }
   }while(0);
   /* free the memory allocated at SM */
   NL_FREE(pst->region, pst->pool, statInd, sizeof(NlMngmt));
   RETVALUE(ret);
}

/**
 * @brief Nl shutdown handler
 *
 * @details
 *
 *     Function : nlShutdown
 *
 *     This function will free all the memory dynamically allocated
 *     in NL context
 *
 *  @param[in]  Void   : 
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void nlShutdown
(
Void  
)
#else
PRIVATE Void nlShutdown(Void)
(
)
#endif
{
   NlCellCb        *cellCb        = NULLP;
   NlNeighCellCb   *nghEnbCb      = NULLP;
   CmLListCp       *nghCbLst      = NULLP;
   CmLListCp       *nghNghCbLst   = NULLP;
   CmLListCp       *pendNghLst    = NULLP;
   CmLListCp       *pendReCfgLst  = NULLP;
   CmLList         *nghNode       = NULLP;
   CmLList         *nghNghNode    = NULLP;
   NlNghServCellCb *nghServCb     = NULLP;
   CmLList         *pendNghNode   = NULLP;
   CmLList         *pendReCfgNode = NULLP;
   NlNghNghCellCb  *nghNghCb      = NULLP;
   NlNghCfgPendMsg *pendNghMsg    = NULLP;
   NlCellReCfgPendMsg *pendReCfgMsg = NULLP;
   /*free all the memory allocated dynamically*/
   TRC3(nlShutdown);
   /*delete all the timers*/
   nlStopTimers();
   /*deregistered timer*/
   SDeregTmrMt((Ent)ENTNL, (Inst)0, nlCb.timeRes, nlActvTmr); 
   while(cmHashListGetNext(&nlCb.cellCbList, NULLP, (PTR *)&cellCb) == ROK)
   {
      /*delete the PCI modification/ neighbor modification pending messqages*/
      pendNghLst  = &(cellCb->pciModPendMsg.nghCfgMsgList);
      CM_LLIST_FIRST_NODE(pendNghLst, pendNghNode);
      while(NULLP != pendNghNode)
      {
         pendNghMsg = (NlNghCfgPendMsg *)pendNghNode->node;
         cmLListDelFrm(pendNghLst, pendNghNode);
         NL_FREE(NL_REGION, NL_POOL, pendNghMsg, sizeof(NlNghCfgPendMsg));
         pendNghNode = NULLP;
         CM_LLIST_FIRST_NODE(pendNghLst, pendNghNode);
      }
      pendReCfgLst  = &(cellCb->pciModPendMsg.cellReCfgMsgList);
      CM_LLIST_FIRST_NODE(pendReCfgLst, pendReCfgNode);
      while(NULLP != pendReCfgNode)
      {
         pendReCfgMsg = (NlCellReCfgPendMsg *)pendReCfgNode->node;
         cmLListDelFrm(pendReCfgLst, pendReCfgNode);
         NL_FREE(NL_REGION, NL_POOL, pendReCfgMsg, sizeof(NlCellReCfgPendMsg));
         pendReCfgNode = NULLP;
         CM_LLIST_FIRST_NODE(pendReCfgLst, pendReCfgNode);
      }
      while(cmHashListGetNext(&cellCb->neighEnbCb.nghEnbListCp, NULLP, (PTR *)&nghEnbCb) == ROK)
      {
         nghCbLst = &(nghEnbCb->servCellLst);
         CM_LLIST_FIRST_NODE(nghCbLst, nghNode);
         while(NULLP != nghNode)
         {
            nghServCb = (NlNghServCellCb *)nghNode->node;
            nghNghCbLst = &nghServCb->nghCellLst;
            CM_LLIST_FIRST_NODE(nghNghCbLst, nghNghNode);
            while(NULLP != nghNghNode)
            {
               nghNghCb = (NlNghNghCellCb *)nghNghNode->node;
               cmLListDelFrm(nghNghCbLst, nghNghNode);
               NL_FREE(NL_REGION, NL_POOL, nghNghCb, sizeof(NlNghNghCellCb));
               nghNghNode = NULLP;
               CM_LLIST_FIRST_NODE(nghNghCbLst, nghNghNode);
            }
            cmLListDelFrm(nghCbLst, nghNode);
            NL_FREE(NL_REGION, NL_POOL, nghServCb, sizeof(NlNghServCellCb));
            nghNode = NULLP;
            CM_LLIST_FIRST_NODE(nghCbLst, nghNode);
         }
         cmHashListDelete(&cellCb->neighEnbCb.nghEnbListCp, (PTR )nghEnbCb);
         NL_FREE(NL_REGION, NL_POOL, nghEnbCb, sizeof(NlNeighCellCb));
      }
      cmHashListDeinit(&cellCb->neighEnbCb.nghEnbListCp);
      cmHashListDelete(&nlCb.cellCbList, (PTR )cellCb);
      NL_FREE(NL_REGION, NL_POOL, cellCb, sizeof(NlCellCb));
   }
   cmHashListDeinit(&nlCb.cellCbList);
   /*shutdown broadcom SON*/
   nlShutDownVendorDSon();  
   nlActvInit(nlCb.init.ent, nlCb.init.inst, 
                nlCb.init.region, nlCb.init.reason);
   RETVOID; 
}

/**
 * @brief Layer Manager cell Configuration request handler.
 *
 * @details
 *
 *     Function : nlCellCfg
 *
 *     This function handles the SON cell configuration
 *     request received from the Layer Manager.
 *
 *  @param[in]  cellCfg   : cell configuration parameter structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCellCfg
(
LnlCellCfg    *cellCfg  /* cell config structure  */
)
#else
PRIVATE S16 nlCellCfg(cellCfg)
(
LnlCellCfg   *cellCfg;    /* cell config structure */
)
#endif
{
   NlCellCb               *cellCb  = NULLP;
   U32                    hashKey;
   TRC3(nlCellCfg);

   if(ROK != nlValidateCellEarfcnCfg(cellCfg))
   {
      RLOG0(L_ERROR, "Cell config received with improper EARFCN configuration");
      RETVALUE(RFAILED);
   }
   if(ROK != nlValidateCellPciCfg(cellCfg))
   {
      RLOG0(L_ERROR, "Cell config received with improper PCI configuration");
      RETVALUE(RFAILED);
   }
   if(ROK != nlValidateCellPrachCfg(&cellCfg->prachCfgInfo))
   {
      RLOG0(L_ERROR, "Cell config received with improper PRACH configuration");
      RETVALUE(RFAILED);
   }
   hashKey = cellCfg->cellId;
   if(ROK == cmHashListFind(&(nlCb.cellCbList),
                (U8 *)&hashKey, sizeof(hashKey), 0, (PTR *)&cellCb))
   {
      RLOG1(L_ERROR, "CellCb already created for cell Id %d", cellCfg->cellId);
      RETVALUE(RFAILED);
   }
   NL_ALLOC(NL_REGION, NL_POOL, &cellCb, sizeof(NlCellCb));
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for cell Cb");
      RETVALUE(RFAILED);
   }
   cellCb->cellId = hashKey;
   if(ROK != cmHashListInsert(&(nlCb.cellCbList), (PTR)cellCb,
             (U8 *)&cellCb->cellId, sizeof(cellCb->cellId)))
   {
      RLOG1(L_ERROR, "failed to insert cell Id into hash table with cellId %d", hashKey);
      NL_FREE(NL_REGION, NL_POOL, cellCb, sizeof(NlCellCb));
      RETVALUE(RFAILED);
   }
   cellCb->cellId = cellCfg->cellId;
   cellCb->duplexMode = cellCfg->duplexMode;
   /* store the ECGI information*/
   NL_MEM_COPY(&cellCb->cellInfo.ecgi, &cellCfg->ecgi, sizeof(LnlEcgi));
   cellCb->cellInfo.ecgi.cellId = cellCfg->ecgi.cellId;
   /*Initiate the neighbor cell hash table*/
   cmHashListInit(&(cellCb->neighEnbCb.nghEnbListCp), NL_EUTRAN_HASH_NO_BINS, 0,
       FALSE, CM_HASH_KEYTYPE_U32MOD, nlCb.init.region, nlCb.init.pool);
   /*changing the cell state to INIT*/
   NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_INIT);

   /*Do the Earfcn selection*/
   if(ROK != nlCellSelectEarfcn(cellCb, cellCfg))
   {
      RLOG0(L_ERROR, "Earfcn selection failed");
      RETVALUE(RFAILED);
   }
   /*Do the PCI configuration*/
   if(ROK != nlCellStorePciCfg(cellCb, cellCfg))
   {
      RLOG0(L_ERROR, "storing PCI configuration failed");
      RETVALUE(RFAILED);
   }
   /*Do the PRACH configuration*/
   if(ROK != nlCellStorePrachCfg(cellCb, cellCfg))
   {
      RLOG0(L_ERROR, "storing PRACH configuration failed");
      RETVALUE(RFAILED);
   }
   /*Store the TxPower configuration*/
   if(ROK != nlCellStoreTxPowerCfg(cellCb, cellCfg))
   {
      RLOG0(L_ERROR, "storing Tx Power configuration failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell modify Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyLnlNghCellModInfo
 *
 *     This function copy the neighbor cell information received
 *     from SM to SON module neighbor cell structure
 *
 *  @param[in]  srcCellMod   : neighbor cell configuration parameter
 *                             structure received from SM
 *  @param[in]  tgtCellMod   : SON neighbor cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCopyLnlNghCellModInfo
(
LnlNghCellMod    *srcCellMod,  /* src neighbor cell config structure  */
NlTmpNghCellMod  *tgtCellMod   /* target neighbor cell structure*/
)
#else
PRIVATE S16 nlCopyLnlNghCellModInfo(srcCellMod, tgtCellMod)
(
LnlNghCellMod   *srcCellMod;   /* neighbor cell config structure */
NlTmpNghCellMod *tgtCellMod;   /* target neighbor cell structure*/
)
#endif
{
   U32                   nghCount;
   U32                   nghNghCount;
   NlTmpNghCellModInfo   *nlCellMod  = NULLP;
   LnlNghCellModInfo     *lnlCellMod = NULLP;
   TRC3(nlCopyLnlNghCellModInfo);
   tgtCellMod->numNgh = srcCellMod->numNgh;
   for(nghCount = 0; nghCount < tgtCellMod->numNgh; nghCount ++)
   {
      nlCellMod = &(tgtCellMod->nghCellModInfo[nghCount]);
      lnlCellMod = &(srcCellMod->nghCellModInfo[nghCount]);
      cmMemcpy((U8 *)&(nlCellMod->oldEcgi), (U8 *)&(lnlCellMod->oldEcgi), 
          sizeof(LnlEcgi));
      nlCellMod->nghCellInfo.pci = lnlCellMod->nghCellInfo.pci; 
      nlCellMod->nghCellInfo.dlEarfcn = lnlCellMod->nghCellInfo.dlEarfcn; 
      nlCellMod->nghCellInfo.ulEarfcn = lnlCellMod->nghCellInfo.ulEarfcn; 
      cmMemcpy((U8 *)&(nlCellMod->nghCellInfo.ecgi), 
          (U8 *)&(lnlCellMod->nghCellInfo.ecgi), sizeof(LnlEcgi));
      nlCellMod->numNghNgh = lnlCellMod->numNghNgh;
      for(nghNghCount = 0; nghNghCount < nlCellMod->numNghNgh; nghNghCount ++)
      {
         nlCellMod->nghNghCellInfo[nghNghCount].pci = 
          lnlCellMod->nghNghCellInfo[nghNghCount].pci; 
         nlCellMod->nghNghCellInfo[nghNghCount].dlEarfcn = 
          lnlCellMod->nghNghCellInfo[nghNghCount].dlEarfcn; 
         nlCellMod->nghNghCellInfo[nghNghCount].ulEarfcn = 
          lnlCellMod->nghNghCellInfo[nghNghCount].ulEarfcn; 
         cmMemcpy((U8 *)&(nlCellMod->nghNghCellInfo[nghNghCount].ecgi),
                  (U8 *)&(lnlCellMod->nghNghCellInfo[nghNghCount].ecgi), 
                  sizeof(LnlEcgi));
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell delete Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyLnlNghCellDelInfo
 *
 *     This function copy the neighbor cell information received
 *     from SM to SON module neighbor cell structure
 *
 *  @param[in]  srcCellDel   : neighbor cell configuration parameter
 *                             structure received from SM
 *  @param[in]  tgtCellDel   : SON neighbor cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCopyLnlNghCellDelInfo
(
LnlNghCellDel    *srcCellDel,  /* src neighbor cell config structure  */
NlTmpNghCellDel  *tgtCellDel   /* target neighbor cell structure*/
)
#else
PRIVATE S16 nlCopyLnlNghCellDelInfo(srcCellDel, tgtCellDel)
(
LnlNghCellDel   *srcCellDel;   /* neighbor cell config structure */
NlTmpNghCellDel *tgtCellDel;   /* target neighbor cell structure*/
)
#endif
{
   U32   nghCount;
   TRC3(nlCopyLnlNghCellDelInfo);
   tgtCellDel->numNgh = srcCellDel->numNgh;
   for(nghCount = 0; nghCount < tgtCellDel->numNgh; nghCount ++)
   {
      cmMemcpy((U8 *)&(tgtCellDel->ecgi[nghCount]),
        (U8 *) &(srcCellDel->ecgi[nghCount]), sizeof(LnlEcgi));
   }
   RETVALUE(ROK);
}


/**
 * @brief Copy PRACH Information
 *
 * @details
 *
 *     Function : nlCopyNghPrachInfo
 *
 *     This function copy the neighbor cell prach information received
 *     from SM to SON module neighbor cell structure
 *
 *  @param[in]  srcNghPrachCfg : neighbor cell configuration parameter
 *                               structure received from SM
 *  @param[in]  tgtPrachCfg    : SON neighbor prach cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCopyNghPrachInfo
(
NlPrachCfg       *tgtPrachCfg,      /* target neighbor cell prach cfg */
LnlNghPrachCfg   *srcNghPrachCfg  /* src neighbor cell prach config */
)
#else
PRIVATE S16 nlCopyNghPrachInfo(srcNghPrachCfg, tgtPrachCfg)
(
NlPrachCfg       *tgtPrachCfg;     /* target neighbor cell prach cfg */
LnlNghPrachCfg   *srcNghPrachCfg;  /* src neighbor cell prach config */
)
#endif
{
   tgtPrachCfg->servRootSeqIdx             = srcNghPrachCfg->nghRootSeqIdx;
   tgtPrachCfg->servPrachCfgIdx            = srcNghPrachCfg->nghPrachCfgIdx;
   tgtPrachCfg->servZeroCorrelationZoneCfg = srcNghPrachCfg->nghZeroCorrelationZoneCfg;
   tgtPrachCfg->servPrachFreqOffset        = srcNghPrachCfg->nghPrachFreqOffset;
   tgtPrachCfg->highSpeedFlag              = srcNghPrachCfg->highSpeedFlag;

   tgtPrachCfg->numRootSeqIndex = 0;
   tgtPrachCfg->numPrachCfgIdx = 0;
   tgtPrachCfg->numZeroCorrCfgZone = 0;
   tgtPrachCfg->numFreqOffset = 0;
   RETVALUE(ROK);
}


/**
 * @brief Layer Manager neighbor cell ADD Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyLnlNghCellAddInfo
 *
 *     This function copy the neighbor cell information received
 *     from SM to SON module neighbor cell structure
 *
 *  @param[in]  srcCellAdd   : neighbor cell configuration parameter
 *                             structure received from SM
 *  @param[in]  tgtCellAdd   : SON neighbor cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCopyLnlNghCellAddInfo
(
LnlNghCellAdd    *srcCellAdd,  /* src neighbor cell config structure  */
NlTmpNghCellAdd  *tgtCellAdd   /* target neighbor cell structure*/
)
#else
PRIVATE S16 nlCopyLnlNghCellAddInfo(srcCellAdd, tgtCellAdd)
(
LnlNghCellAdd   *srcCellAdd;   /* neighbor cell config structure */
NlTmpNghCellAdd *tgtCellAdd;   /* target neighbor cell structure*/
)
#endif
{
   U32                    nghCount;
   U32                    nghNghCount;
   NlTmpNghCellAddInfo    *nlCellAddInfo  = NULLP;
   LnlNghCellAddInfo      *lnlCellAddInfo = NULLP;
   TRC3(nlCopyLnlNghCellAddInfo);
   tgtCellAdd->numNgh = srcCellAdd->numNgh;
   for(nghCount = 0; nghCount < tgtCellAdd->numNgh; nghCount ++)
   {
      nlCellAddInfo = &(tgtCellAdd->nghCellAddInfo[nghCount]);
      lnlCellAddInfo = &(srcCellAdd->nghCellAddInfo[nghCount]);
      nlCellAddInfo->nghCellInfo.pci = lnlCellAddInfo->nghCellInfo.pci;
      nlCellAddInfo->nghCellInfo.dlEarfcn = lnlCellAddInfo->nghCellInfo.dlEarfcn;
      nlCellAddInfo->nghCellInfo.ulEarfcn = lnlCellAddInfo->nghCellInfo.ulEarfcn;
      cmMemcpy((U8 *)&(nlCellAddInfo->nghCellInfo.ecgi), 
         (U8 *)&(lnlCellAddInfo->nghCellInfo.ecgi), sizeof(LnlEcgi));
      nlCellAddInfo->numNghNgh = lnlCellAddInfo->numNghNgh;
      for(nghNghCount = 0; nghNghCount < nlCellAddInfo->numNghNgh; nghNghCount ++)
      {
         nlCellAddInfo->nghNghCellInfo[nghNghCount].pci =
           lnlCellAddInfo->nghNghCellInfo[nghNghCount].pci;
         nlCellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn =
           lnlCellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn;
         nlCellAddInfo->nghNghCellInfo[nghNghCount].ulEarfcn =
           lnlCellAddInfo->nghNghCellInfo[nghNghCount].ulEarfcn;
         cmMemcpy((U8 *)&(nlCellAddInfo->nghNghCellInfo[nghNghCount].ecgi), 
               (U8 *) &(lnlCellAddInfo->nghNghCellInfo[nghNghCount].ecgi),
                sizeof(LnlEcgi));
      }

      if(lnlCellAddInfo->nghCellInfo.isNghPrachInfo)
      {
         nlCellAddInfo->nghCellInfo.isPrachCfg = TRUE;

         nlCopyNghPrachInfo((&(nlCellAddInfo->nghCellInfo.prachCfg)),
                        (&(lnlCellAddInfo->nghCellInfo.prachCfgInfo)));
      }
      else
      {
         nlCellAddInfo->nghCellInfo.isPrachCfg = FALSE;
      }
      nlCellAddInfo->nghCellInfo.csgAccessMode = lnlCellAddInfo->nghCellInfo.csgAccessMode;
      nlCellAddInfo->nghCellInfo.csgId = lnlCellAddInfo->nghCellInfo.csgId;
      nlCellAddInfo->nghCellInfo.lteBand = lnlCellAddInfo->nghCellInfo.lteBand;
      nlCellAddInfo->nghCellInfo.tac = lnlCellAddInfo->nghCellInfo.tac;
      nlCellAddInfo->nghCellInfo.ulBw = lnlCellAddInfo->nghCellInfo.ulBw;
      nlCellAddInfo->nghCellInfo.dlBw = lnlCellAddInfo->nghCellInfo.dlBw;
      nlCellAddInfo->nghCellInfo.RSTxPower = lnlCellAddInfo->nghCellInfo.RSTxPower;
      nlCellAddInfo->nghCellInfo.rsrp = lnlCellAddInfo->nghCellInfo.rsrp;
      nlCellAddInfo->nghCellInfo.rssi = lnlCellAddInfo->nghCellInfo.rssi;
      nlCellAddInfo->nghCellInfo.isCellBarred = lnlCellAddInfo->nghCellInfo.isCellBarred;
      
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyLnlNghCellInfo
 *
 *     This function copy the neighbor cell information received 
 *     from SM to SON module neighbor cell structure
 *
 *  @param[in]  srcCellCfg   : neighbor cell configuration parameter 
 *                             structure received from SM
 *  @param[in]  tgtCellCfg   : SON neighbor cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlCopyLnlNghCellInfo
(
LnlNghCellCfg    *srcCellCfg,  /* src neighbor cell config structure  */
NlTmpNghCellCfg  *tgtCellCfg   /* target neighbor cell structure*/
)
#else
PRIVATE S16 nlCopyLnlNghCellInfo(srcCellCfg, tgtCellCfg)
(
LnlNghCellCfg   *srcCellCfg;   /* neighbor cell config structure */
NlTmpNghCellCfg *tgtCellCfg;   /* target neighbor cell structure*/
)
#endif
{
   U8          numEnb;
   TRC3(nlCopyLnlNghCellInfo);
   RLOG1(L_DEBUG,"COpying Neighbor cells, for {%d] Enbs",srcCellCfg->numNghEnb);
   //cmMemset((U8 *)tgtCellCfg, 0, sizeof(NlTmpNghCellCfg));
   tgtCellCfg->cellId = srcCellCfg->servCellId;
   tgtCellCfg->numNghEnb = srcCellCfg->numNghEnb;
   for(numEnb = 0; numEnb < tgtCellCfg->numNghEnb; numEnb ++)
   {
      tgtCellCfg->nghEnbCellCfg[numEnb].nghEnbId = 
       srcCellCfg->nghEnbCellCfg[numEnb].nghEnbId;
      switch(srcCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType)
      {
         case LNL_NGH_ADD :
         {
            tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType = NL_NGH_ADD;
            nlCopyLnlNghCellAddInfo(&(srcCellCfg->
                 nghEnbCellCfg[numEnb].nghCellParam.u.nghCellAdd),
                 &(tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellAdd));
            break;
         }
         case LNL_NGH_DEL :
         {
            tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType = NL_NGH_DEL;
            nlCopyLnlNghCellDelInfo(&(srcCellCfg->
               nghEnbCellCfg[numEnb].nghCellParam.u.nghCellDel),
               &(tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellDel));
            break;
         }
         case LNL_NGH_MOD :
         {
            tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType = NL_NGH_MOD;
            nlCopyLnlNghCellModInfo(&(srcCellCfg->
               nghEnbCellCfg[numEnb].nghCellParam.u.nghCellMod),
               &(tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellMod));
            break;
         }
         default :
         {
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell Configuration request handler.
 *
 * @details
 *
 *     Function : nlNeighborCellCfg
 *
 *     This function handles the SON neighbor cell configuration
 *     request received from the Layer Manager.
 *
 *  @param[in]  nghCellCfg   : neighbor cell configuration parameter structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlNeighborCellCfg
(
LnlNghCellCfg    *nghCellCfg  /* neighbor cell config structure  */
)
#else
PRIVATE S16 nlNeighborCellCfg(nghCellCfg)
(
LnlNghCellCfg   *nghCellCfg;   /* neighbor cell config structure */
)
#endif
{
   S16 ret           = ROK;
   NlTmpNghCellCfg   *nlNghCfg;
   U32               hashKey;
   NlCellCb          *cellCb = NULLP;
   NlEventMsg         eventMsg;
   TRC3(nlNeighborCellCfg);
   NL_ALLOC(NL_REGION, NL_POOL, &nlNghCfg, sizeof(NlTmpNghCellCfg));
   if(nlNghCfg == NULLP)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for NlTmpNghCellCfg \n");
      RETVALUE(RFAILED);
   }
   hashKey = nghCellCfg->servCellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey,
     sizeof(hashKey), 0, (PTR *)&cellCb))
   {
      RLOG1(L_ERROR, "Invalid cellId %d received for neighbor configuration",
       nghCellCfg->servCellId);
      RETVALUE(RFAILED);
   }
   nlCopyLnlNghCellInfo(nghCellCfg, nlNghCfg);
   eventMsg.msgType = SON_NGH_CFG;
   eventMsg.msg = (Void *)nlNghCfg;
   ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_EVENT_NGH_CFG])(&eventMsg, 
         cellCb);
   NL_FREE(NL_REGION, NL_POOL, nlNghCfg, sizeof(NlTmpNghCellCfg));
   RETVALUE(ret);
}

/**
 * @brief Layer Manager cell ReConfiguration request handler.
 *
 * @details
 *
 *     Function : nlCellReCfg
 *
 *     This function handles the SON cell Reconfiguration
 *     request received from the Layer Manager.
 *
 *  @param[in]  cellReCfg    : cell reconfiguration parameter structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCellReCfg
(
LnlCellReCfg     *cellReCfg   /* cell reconfig structure  */
)
#else
PRIVATE S16 nlCellReCfg(cellReCfg)
(
LnlCellReCfg    *cellReCfg;    /* cell reconfig structure */
)
#endif
{
   S16          ret = ROK;
   NlCellCb     *cellCb = NULLP;
   U32          hashKey;
   NlEventMsg   evtMsg;
   TRC3(nlCellReCfg);
   hashKey = cellReCfg->cellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey, 
     sizeof(hashKey), 0, (PTR *)&cellCb))
   {
      RLOG1(L_ERROR, "Invalid cellId %d received for cell reconfiguration",
       cellReCfg->cellId);
      RETVALUE(RFAILED);
   }
   evtMsg.msgType = SON_CELL_RECFG;
   evtMsg.msg = (Void *)cellReCfg;
   ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_EVENT_RECFG])(&evtMsg,
         cellCb);
   RETVALUE(ret);
}

/********************************************************************30**

/********************************************************************30**

           End of file:     bk_lmm.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:16 2014

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

