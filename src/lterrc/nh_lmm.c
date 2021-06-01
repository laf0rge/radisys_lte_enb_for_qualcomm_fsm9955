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


/************************************************************************
 
     Name:     LTE RRC Layer Manager
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions.
  
     File:     nh_lmm.c 
  
     Sid:      ni_lmm.c@@/main/3 - Fri Jul  1 01:13:20 2011
  
     Prg:      Reena 
  
**********************************************************************/
/** @defgroup lmm_group Layer manager Module
 *  @{
 */

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=90;
#include "envopt.h"         /* Environment options */
#include "envdep.h"         /* Environment dependent */
#include "envind.h"         /* Environment independent */
#include "gen.h"            /* General layer */
#include "ssi.h"            /* System services */
#include "cm5.h"            /* Common timer module */
#include "cm_hash.h"        /* Common hashlist */
#include "cm_tkns.h"        /* Common tokens */
#include "cm_inet.h"        /* Inet header file             */
#include "cm_mblk.h"        /* Common memory allocation */
#include "cm_llist.h"       /* Common link list */
#include "cm_lte.h"         /* Common LTE structures */
#include "ckw.h"            /* C-RLC interface */
#include "kwu.h"            /* D-RLC interface */
#include "crg.h"            /* C-MAC interface */
#include "cpj.h"            /* C-PDCP interface */
#include "pju.h"            /* D-PDCP interface */
#include "nhu.h"            /* RRC Upper interface */
#include "nh.h"
#include "lnh.h"           /* LM interface */
/* external headers */

/* header/extern include files (.x) */
#include "gen.x"            /* General layer */
#include "ssi.x"            /* System services */
#include "cm5.x"            /* Common timer module */
#include "cm_lib.x"         /* Common library */
#include "cm_hash.x"        /* Common hashlist */
#include "cm_tkns.x"        /* Common tokens */
#include "cm_mblk.x"        /* Common memory allocation */
#include "cm_inet.x"        /* Inet header file             */
#include "cm_llist.x"       /* Common link list */
#include "cm_lte.x"         /* Common LTE structures */
#include "ckw.x"            /* C-RLC interface */
#include "kwu.x"            /* D-RLC interface */
#include "crg.x"            /* C-MAC interface */
#include "cpj.x"            /* C-PDCP interface */
#include "pju.x"            /* D-PDCP interface */
#include "nhu.x"            /* RRC Upper interface */
#include "lnh.x"
#include "nh.x"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */
PRIVATE S16 nhFreeSapLst ARGS((
NhGenCfg *gen,       /* General configuration */
CmStatus *status    /* Return status */
));

PRIVATE S16 nhFreeSapCb ARGS((
Void    /* Return status */
));

PRIVATE S16 nhFreeCellCb ARGS((
Void    /* Return status */
));

PRIVATE S16 nhLSapAlloc ARGS((
NhGenCfg *gen,       /* General configuration */
U8   *numLSaps 
));

PRIVATE S16 nhValidateGenCfg ARGS((
NhGenCfg *gen        /* General configuration */
));

PRIVATE S16 nhValidateProtCfg ARGS((
NhProtCfg *prot
));

PRIVATE S16 nhValidateUSapCfg ARGS((
NhUSapCfg *uSap
));

PRIVATE S16 nhValidateLSapCfg ARGS((
U16       elmntId,
NhLSapCfg *lSap      /* Lower SAP configuration */
));

PRIVATE S16 nhValidateUSapCntrl ARGS((
NhCntrl  *cntrl
));

PRIVATE S16 nhValidateLSapCntrl ARGS((
U8       sapType,
NhCntrl  *cntrl,
NhLSapCb **lSapCb
));

PRIVATE S16 nhValidateSts ARGS((
Action   action
));

PRIVATE S16 nhValidateLSapSta ARGS((
NhLSapCb *lSapCb
));

PRIVATE S16 nhValidateUSapSta ARGS((
NhSapSta *sta          /* Upper SAP status */
));

PUBLIC S16 nhShutdown ARGS((
Void
));

#ifdef LTE_RRC_DISSECTOR
PUBLIC S16 nhDissectrInit ARGS((Void));
#endif


/**************************************************************************
 * Layer Manager Interface primitives
 *************************************************************************/
/**
 * @brief This function is used by the layer manager to configure RRC.
 * @details

<b> NhMiLnhCfgReq </b> 

    The layer manager configures the various elements of RRC using the
    management configuration procedure, which the layer manager
    initiates. The RRC configuration request primitive (NhMiLnhCfgReq)
    can be called more than once. NhMiLnhCfgReq primitives must be
    called before the bind primitives are called. The table lists the
    RRC configuration request primitive types.

    To operate properly, the configuration request primitive types must be
    called in the following order:
      1. General
      2. Protocol
      3. Upper SAP configuration
      4. Each lower SAP configuration

      The general configuration must be done before any other configuration is done.
      The protocol configuration is done at least once for each self-node type. This
      configuration must also be done before the upper SAP configurations.
      The upper SAP configuration is sent at least once for each upper SAP:
      -   There is only one upper SAP (NHUSAP) for the RRC user.
      The lower SAP configuration is sent at least once for each lower SAP:
      -   There are five lower SAPs.
          - PJUSAP and KWUSAP is for data SAPs, while
          - CKWSAP, CRGSAP, and CPJSAP are control SAPs.
      The lower SAP configurations such as PJUSAP, KWUSAP, CKWSAP, CRGSAP, and
      CPJSAP may be configured in any order. However, these configurations must
      be performed only after the general and protocol configuration.
      The System Services primitives are called during the Management configuration procedure.


* @param[in] pst Pointer to the Post structure
* @param[in] cfg Pointer to the Configuration Management structure

  In the header of the configuration request, the values of elmId.elmnt are set as follows:
    -  STNHGEN:         General
    -  STNHUSAP:        Upper SAP
    -  STNHKWUSAP:      Lower KWU SAP
    -  STNHCKWSAP:      Lower CKW SAP
    -  STNHCRGSAP:      Lower CRG SAP
    -  STNHCTFSAP:      Lower CTF SAP
    -  STNHCPJSAP:      Lower CPJ SAP
    -  STNHPJUSAP:      Lower PJU SAP
    -  STNHPROT:        Protocol

 * @return S16
 *  -# ROK
 *  -# RFAILED

*/
#ifdef ANSI
PUBLIC S16 NhMiLnhCfgReq
(
Pst     *pst,                
NhMngmt *cfg            
)
#else /* ANSI */
PUBLIC S16 NhMiLnhCfgReq(pst, cfg)
Pst     *pst;                
NhMngmt *cfg;            
#endif /* ANSI */
{
   S16        ret;      

   TRC3(NhMiLnhCfgReq);

   RLOG1(L_DEBUG, "Received CfgReq from SM element [%d]", cfg->hdr.elmId.elmnt);

   /* This macro checks for NULL pointers being passed */
   if (NH_CHK_INPUT_PARAM(cfg))
   {
      RLOG0(L_FATAL, "GOT NULL cfg");
      RETVALUE(RFAILED);
   }

   /* Configuration Module takes care of the config request */
   ret = nhLMMLnhCfgReq(pst, cfg);

   RETVALUE(ret);

} /* NhMiLnhCfgReq */

/**
 * @brief This function is used by the layer manager to control RRC elements.
 * @details
 *
<b> NhMiLnhCntrlReq </b> 

The layer manager uses the management control procedure to control RR
C
elements. The layer manager initiates this procedure. The RRC control
 request
primitive can be called more than once and at any time after
the management configuration procedure.
The following RRC control request primitive types can be called:
 - Trace control
 - Debug control
 - SAP control

The nhMngmt.u.cntrl.action field specifies the control request primitive type.
The nhMngmt.u.cntrl.subaction field specifies the element to be controlled.
The nhMngmt.u.cntrl structure specifies the parameters that are used by the control request (NhMiLnhCntrlReq) primitive.

* @param[in] pst Pointer to the Post structure
* @param[in] cntrl Pointer to the Control Management structure

  In the header of the statistics request, the values of elmId.elmnt are as follows:
  - STNHGEN: General
  - STNHPROT: Protocol
  - STNHUSAP: Upper SAP
  - STNHKWUSAP: Lower KWU SAP
  - STNHCKWSAP: Lower CKW SAP
  - STNHCRGSAP: Lower CRG SAP
  - STNHCTFSAP: Lower CTF SAP
  - STNHCPJSAP: Lower CPJ SAP
  - STNHPJUSAP: Lower PJU SAP

 * @return S16
 *  -# ROK
 *  -# RFAILED

*/
#ifdef ANSI
PUBLIC S16 NhMiLnhCntrlReq
(
Pst     *pst,                
NhMngmt *cntrl           
)
#else /* ANSI */
PUBLIC S16 NhMiLnhCntrlReq(pst, cntrl)
Pst     *pst;                
NhMngmt *cntrl;          
#endif /* ANSI */
{
   S16        ret;     
   NhMngmt    cntrlCfm;
   NhUstaDgn  dgn;     /* Unsolicited status diagnostics structure */     
 
   TRC3(NhMiLnhCntrlReq);
   RLOG1(L_DEBUG, "Received CntrlReq from SM element [%d]", cntrl->hdr.elmId.elmnt);

   /* This macro checks for NULL pointers being passed */
   if (NH_CHK_INPUT_PARAM(cntrl))
   {
      RLOG0(L_FATAL, "GOT NULL cntrl");
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is done */
   NH_CHK_LYR_CFG(NhMiLnhCntrlReq, ret);

   cmMemset((U8*)&cntrlCfm, 0, sizeof(NhMngmt));

   
   /* nh001.301 : Added code for updating control confirm
    *             information.
    */
   

   if( ret != ROK)
   {
      if (cntrl->u.cntrl.action != ASHUTDOWN)
      {
         cntrlCfm.cfm.status = LCM_PRIM_NOK;
         cntrlCfm.cfm.reason = ret;
      }
      /* Send a confirmation to layer manager */ 
      nhLMMSndLmCfm(pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
      RETVALUE(ROK);
   }

   /* Switch on element */
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STNHGEN:     /* General (applicable layer-wide) control */
         ret = nhGenCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
         break;

      case STNHUSAP:    /* RRC's upper SAP control */  
         ret = nhUSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
         break;

      case STNHKWUSAP:  /* RRC's lower SAP control for RLC Data
                           interface */ 
         ret = nhKwuSapCntrl(&cntrl->u.cntrl,&cntrlCfm.cfm);
         break;

      case STNHCKWSAP:  /* RRC's lower SAP control for RLC Control
                           interface */ 
         ret = nhCkwSapCntrl(&cntrl->u.cntrl,&cntrlCfm.cfm);
         break;

      case STNHCRGSAP:  /* RRC's lower SAP control for MAC Control
                           interface */
         ret = nhCrgSapCntrl(&cntrl->u.cntrl,&cntrlCfm.cfm);
         break;

      case STNHPJUSAP:  /* RRC's lower SAPs control for PDCP Data
                           interface */ 
         ret = nhPjuSapCntrl(&cntrl->u.cntrl,&cntrlCfm.cfm);
         break;

      case STNHCPJSAP:  /* RRC's lower SAPs control for PDCP Control
                           interface */ 
         ret = nhCpjSapCntrl(&cntrl->u.cntrl,&cntrlCfm.cfm);
         break;

#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:  /* RRC's lower SAPs control for PHY Control
                           interface */ 
         ret = nhCtfSapCntrl(&cntrl->u.cntrl,&cntrlCfm.cfm);
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
      {
         /* Invalid element */
         RLOG1(L_ERROR, "Invalid Element in control request [%d]",
            cntrl->hdr.elmId.elmnt);

         cntrlCfm.cfm.status = LCM_PRIM_NOK;
         cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         ret = RFAILED;
         dgn.type    = LNH_USTA_DGNVAL_NONE;
         /* Send Alarm to the layer manager */
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT, 
                       LCM_CAUSE_MGMT_INITIATED, &dgn);
         break;
      }   
   } /* end of switch */

   if (ret != ROKDNA)
   {
      /*  Send a confirmation to layer manager */ 
      nhLMMSndLmCfm(pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
   }

   RETVALUE(ret);
} /* NhMiLnhCntrlReq */

/**
 * @brief This function is used by the layer manager to get statistics from RRC.
 * @details 

<b> NhMiLnhStsReq </b> 

    The layer manager gathers statistics information about the elements of RRC 
    using the management statistics procedure initiated by the layer manager. 
    The RRC statistics request primitive can be called more than once and at any time after the management configuration procedure.
    The statistics values are returned by the <b> NhMiLnhStsCfm </b> primitive. 
    The statistics counters can be reset using the action parameter. If the
    statistics counters are not explicitly reset, they can be reset when the
    count exceeds the size of the data structure used to store the statistics
    (currently 216 - 1). These RRC statistics request primitive types are
    called peer node statistics. They provide the count for each kind
    of message transmitted or received from the peer node. They also provide 
    the count of unexpected or error messages received and the number of times 
    each kind of timer expired when executing the procedures with the peer node.

* @param[in] pst Pointer to the post structure
* @param[in] action This defines action by the layer after reading the statistics 
* @param[in] sts Pointer to the management structure

        The possible values are 
           - ZEROSTS   - Read and reset (set to zero) statistics
           - NOZEROSTS - Does not reset the counters

  In the header of the statistics request, the values of elmId.elmnt field are as follows:
           - STNHGEN:  General
           - STNHUE:   UE related statistics
           - STNHCELL: Cell related statistics

 * @return S16
 *  -# ROK
 *  -# RFAILED

*/
#ifdef ANSI
PUBLIC S16 NhMiLnhStsReq
(
Pst     *pst,                
Action  action,           
NhMngmt *sts             
)
#else /* ANSI */
PUBLIC S16 NhMiLnhStsReq(pst, action, sts)
Pst     *pst;                
Action  action;           
NhMngmt *sts;            
#endif /* ANSI */
{
   S16         ret;   
   NhMngmt     stsCfm;  /* Statistics confirm structure */
   NhUstaDgn   dgn;     /* Unsolicited status diagnostics 
                           structure */ 
   NhActvUeCb  *ueCb;   /* UE control block */
   NhCellCb    *cellCb; /* Cell UE control block */

   TRC3(NhMiLnhStsReq);
 
   RLOG2(L_DEBUG, "NhMiLnhStsReq(action(%d), Received stsReq from SM element(%d)) ",
                  action, sts->hdr.elmId.elmnt);
 
   /* This macro checks for NULL pointers being passed */
   if (NH_CHK_INPUT_PARAM(sts))
   {
      RLOG0(L_FATAL, "GOT NULL sts");
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is done */
   NH_CHK_LYR_CFG(NhMiLnhStsReq, ret);

   cmMemset((U8*)&stsCfm, 0, sizeof(NhMngmt));

   if( ret != ROK)
   {
      stsCfm.cfm.status = LCM_PRIM_NOK;
      stsCfm.cfm.reason = ret;
      /* Send a confirmation to layer manager */ 
      nhLMMSndLmCfm(pst, &sts->hdr, TSTS, &stsCfm);
      RETVALUE(ROK);
   }

   /* Switch on element */
   switch (sts->hdr.elmId.elmnt)
   {
      case STNHGEN:              /* General statistics */
         ret = nhGetGenSts (&stsCfm.u.sts.u.genSts, &stsCfm.cfm, action);
         break;

      case STNHUE:              /* UE  statistics */
         if((cellCb = nhDBMGetCellCb(sts->u.sts.u.ueSts.cellId)) == NULLP)
         {
            RLOG1(L_ERROR, "CELL Does not exist. Element [%d]",
               sts->hdr.elmId.elmnt);

            stsCfm.cfm.status = LCM_PRIM_NOK;
            stsCfm.cfm.reason = LNH_REASON_CELL_DOES_NOT_EXIST;
            ret = RFAILED;
            break;
         }
         if(((ueCb = nhDBMGetUeCb(cellCb,
                                  sts->u.sts.u.ueSts.crnti))) == 
                                                       (NhActvUeCb *)NULLP)
         {
            RLOG1(L_ERROR, "UE Does not exist. Element [%d]",
                sts->hdr.elmId.elmnt);

            stsCfm.cfm.status = LCM_PRIM_NOK;
            stsCfm.cfm.reason = LNH_REASON_UE_DOES_NOT_EXIST;
            ret = RFAILED;
            break;
         }
         ret = nhGetUeSts (&stsCfm.u.sts.u.ueSts, &stsCfm.cfm, action, ueCb);
         break;

      case STNHCELL:              /* Cell statistics */
         if ((cellCb = nhDBMGetCellCb(sts->u.sts.u.cellSts.cellId)) == NULLP)
         {
            RLOG1(L_ERROR, "CELL Does not exist. Element [%d]",
               sts->hdr.elmId.elmnt);

            stsCfm.cfm.status = LCM_PRIM_NOK;
            stsCfm.cfm.reason = LNH_REASON_CELL_DOES_NOT_EXIST;
            ret = RFAILED;
            break;
         }
         ret = nhGetCellSts (&stsCfm.u.sts.u.cellSts, &stsCfm.cfm, action, 
                             cellCb);
         break;

      default:
         RLOG1(L_ERROR, "Invalid element. Element [%d]", sts->hdr.elmId.elmnt);
         stsCfm.cfm.status = LCM_PRIM_NOK;
         stsCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         ret = RFAILED;

         dgn.type    = LNH_USTA_DGNVAL_NONE;
         /* Send Alarm to the layer manager */
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT, 
                                               LCM_CAUSE_MGMT_INITIATED, &dgn);
         break;
   } /* end of switch */
   
   /* Send a confirmation to the layer manager */ 
   nhLMMSndLmCfm(pst, &sts->hdr, TSTS, &stsCfm);

   RETVALUE(ret);
} /* NhMiLnhStsReq */

/**
 * @brief This function is used by the layer manager to get status from RRC.
 * @details 

<b> NhMiLnhStaReq </b> 

   The layer manager gathers solicited status information about the various 
   elements of RRC by using the management solicited status procedure, which 
   the layer manager initiates. The RRC status request primitive 
   can be called more than once and at any time after the management 
   configuration procedure.

   The following RRC status request primitive types are called:
      -  General
      -  System ID
      -  SAP status

   The nhMngmt.hdr.elmId field specifies the status request primitive type.
   The status confirm (NhMiLnhStaCfm) and other system services primitives are 
   called during the status procedure to return the status value.
   The status request and status confirm primitives (NhMiLnhStaReq and
   NhMiLnhStaCfm, respectively) use the nhMngmt.u.ssta structure to specify
   parameters.

* @param[in] pst Pointer to the post structure
* @param[in] sta Pointer to the management structure 

   In the header of the statistics request, the values of elmId.elmnt fields 
   are as follows:

   - STNHGEN: General
   - STNHSID: System ID
   - STNHUSAP: Upper SAP
   - STNHKWUSAP: Lower RLU SAP
   - STNHCKWSAP: Lower C-RLC SAP
   - STNHCRGSAP: Lower MAC SAP
   - STNHCTFSAP: Lower PHY SAP
   - STNHCPJSAP: Lower C-PDCP SAP
   - STNHPJUSAP: Lower PDCP SAP

    Parameters of the NhSsta structure 

    - SAP status
      The sapId needs to be filled, if the elmId.elmnt is one of the
      following:
       -   STNHUSAP
       -   STNHKWUSAP
       -   STNHCKWSAP
       -   STNHCRGSAP
       -   STNHCTFSAP
       -   STNHCPJSAP
       -   STNHPJUSAP

      The other fields are filled and returned by RRC in the status confirm
      primitive. 

 * @return S16
 *  -# ROK
 *  -# RFAILED

*/
#ifdef ANSI
PUBLIC S16 NhMiLnhStaReq
(
Pst     *pst,                
NhMngmt *sta             
)
#else /* ANSI */
PUBLIC S16 NhMiLnhStaReq(pst, sta)
Pst     *pst;               
NhMngmt *sta;          
#endif /* ANSI */
{
   S16        ret;      
   NhMngmt    staCfm;     
   NhUstaDgn  dgn;      /* Unsolicited status diagnostics structure */     

   TRC3(NhMiLnhStaReq);

   RLOG1(L_DEBUG, "Received staReq from SM element [%d]", sta->hdr.elmId.elmnt);
  
   /* This macro checks for NULL pointers being passed */
   if (NH_CHK_INPUT_PARAM(sta))
   {
      RLOG0(L_FATAL, "GOT NULL sta");
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is done */
   NH_CHK_LYR_CFG(NhMiLnhStaReq, ret);

   cmMemset((U8*)&staCfm, 0, sizeof(NhMngmt));

   if( ret != ROK)
   {
      staCfm.cfm.status = LCM_PRIM_NOK;
      staCfm.cfm.reason = ret;
      /* Send a confirmation to layer manager */ 
      /* Check if the layer manager had requested for sysid
       * If so fill in the sys ID
       */
      if(sta->hdr.elmId.elmnt == STNHSID)
      {
         nhGetSId(&staCfm.u.ssta.u.sysId);
      }
      nhLMMSndLmCfm(pst, &sta->hdr, TSSTA, &staCfm);
      RETVALUE(ROK);
   }

   ret = ROK;
   /* Switch on the element */
   switch (sta->hdr.elmId.elmnt)
   {
      case STNHSID:          /* System ID */
         staCfm.cfm.status = LCM_PRIM_OK;
         staCfm.cfm.reason = LCM_REASON_NOT_APPL;
         (Void) nhGetSId(&staCfm.u.ssta.u.sysId);
         break;

      case STNHGEN:          /* General status */
         staCfm.cfm.status = LCM_PRIM_OK;
         staCfm.cfm.reason = LCM_REASON_NOT_APPL;
         cmMemcpy ((U8 *)&staCfm.u.ssta.u.genSta, (U8 *)&nhCb.genSta, 
                   sizeof (NhGenSta));
         break;

      case STNHKWUSAP:         /* RLC - RLC SAP status */
      case STNHCKWSAP:         /* CKW - C-RLC SAP status */
      case STNHCRGSAP:         /* CRG - MAC SAP status */
      case STNHCPJSAP:         /* CPJ - C-PDCP SAP status */
      case STNHPJUSAP:         /* PJU - PDCP SAP status */
#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:         /* CTF - PHY control SAP status */
#endif /* RRC_PHY_CFG_SUPP */
         cmMemcpy ((U8 *)&staCfm.u.ssta.u.sap, (U8 *)&sta->u.ssta.u.sap, 
                    sizeof (NhSapSta));
         ret = nhGetLSapSta (&staCfm.u.ssta.u.sap, sta->hdr.elmId.elmnt,
                             &staCfm.cfm);
         break;

      case STNHUSAP:         /* Upper SAP status */
         cmMemcpy ((U8 *)&staCfm.u.ssta.u.sap, (U8 *)&sta->u.ssta.u.sap, 
                   sizeof (NhSapSta));
         ret = nhGetUSapSta (&staCfm.u.ssta.u.sap, &staCfm.cfm);
         break;

      default:
         RLOG1(L_ERROR, "Invalid Element. Element [%d]", sta->hdr.elmId.elmnt);
         staCfm.cfm.status = LCM_PRIM_NOK;
         staCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         dgn.type          = LNH_USTA_DGNVAL_NONE;
         ret               = RFAILED;
         /* Send Alarm to the layer manager */
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT, 
                       LCM_CAUSE_MGMT_INITIATED, &dgn);
         break;
   } /* end  of switch */
 
   /* Send a confirmation to the layer manager */ 
   nhLMMSndLmCfm(pst, &sta->hdr, TSSTA, &staCfm);
   RETVALUE(ret);
} /* NhMiLnhStaReq */

/**************************************************************************
 * Config Request handling functions 
 *************************************************************************/
/**************************************************************************
 *
 *      Fun:  nhLMMLnhCfgReq 
 *
 *      Desc: Used by Layer Manager to configure the RRC Layer. 
 *
 *      Ret:  ROK     - Success 
 *            RFAILED - Failure
 *
 *      Notes:None. 
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 nhLMMLnhCfgReq
(
Pst     *pst,                
NhMngmt *cfg            
)
#else /* ANSI */
PUBLIC S16 nhLMMLnhCfgReq(pst, cfg)
Pst     *pst;              
NhMngmt *cfg;         
#endif /* ANSI */
{
   S16        ret;      
   NhMngmt    cfgCfm;   /* Configuration confirm */
   NhUstaDgn  dgn;      /* Unsolicited status diagnostics structure */     

   TRC2(nhLMMLnhCfgReq)

   RLOG1(L_DEBUG, "Configuration Received. ElmId[%d]", cfg->hdr.elmId.elmnt);

   cmMemset((U8*)&cfgCfm, 0, sizeof(NhMngmt));

   /* Check if general Configuration done */
   if ((!nhCb.init.cfgDone) && (cfg->hdr.elmId.elmnt != STNHGEN))
   {
      RLOG0(L_ERROR, "General configuration not done.");

     /* NOTE: An alarm cannot be raised here as general configuration 
      * is not done yet  */ 
      cfgCfm.cfm.status = LCM_PRIM_NOK;
      cfgCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      /*  Send a confirmation to the layer manager */ 
      nhLMMSndLmCfm (pst, &cfg->hdr, TCFG, &cfgCfm);
      RETVALUE(RFAILED);
   }

   switch (cfg->hdr.elmId.elmnt)
   {
      case STNHGEN:       /* General configuration */
         ret = nhGenCfg(&cfg->u.cfg.u.gen, &cfgCfm.cfm);
         if (ret != ROK)
         {
            nhFreeSapLst(&cfg->u.cfg.u.gen, &cfgCfm.cfm);
         }
         break;

      case STNHPROT:       /* Protocol configuration */
         ret = nhProtCfg(&cfg->u.cfg.u.prot, &cfgCfm.cfm);
         break;

      case STNHUSAP:      /* Upper SAP configuration */
         ret = nhUSapCfg(&cfg->u.cfg.u.uSap, &cfgCfm.cfm);
         break;

      case STNHCKWSAP:    /* CKW - C-RLC SAP configuration */
      case STNHKWUSAP:    /* KWU - RLC SAP configuration */
      case STNHCRGSAP:    /* CRG - MAC SAP configuration */
      case STNHCPJSAP:    /* CPJ - C-PDCP SAP configuration */
      case STNHPJUSAP:    /* PJU  PDCP SAP configuration */
#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:    /* CTF - PHY control SAP configuration */
#endif /* RRC_PHY_CFG_SUPP */
         ret = nhLSapCfg (&cfg->u.cfg.u.lSap, cfg->hdr.elmId.elmnt, 
                          &cfgCfm.cfm);
         break;

      default:
      {
         RLOG1(L_ERROR, "Invalid element.[%d]", cfg->hdr.elmId.elmnt);
         cfgCfm.cfm.status = LCM_PRIM_NOK;
         cfgCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         ret = RFAILED;
         dgn.type    = LNH_USTA_DGNVAL_NONE;

         /* Send Alarm to the layer manager */
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT, 
                                               LCM_CAUSE_MGMT_INITIATED, &dgn);
         break;
      }
   } /* end of switch */

   /*  Send a confirmation to the layer manager */ 
   nhLMMSndLmCfm(pst, &cfg->hdr, TCFG, &cfgCfm);
   RETVALUE(ROK);

} /* nhLMMLnhCfgReq */

/**************************************************************************
*
*      Fun:   nhValidateGenCfg
*
*      Desc:  Validates the RRC General Configuration.
*
*      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
*             LCM_REASON_<REASON> - The reason for which validation 
*                                   failed
*
*      Notes: None
*
*      File:  nh_lmm.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateGenCfg
(
NhGenCfg *gen        /* General configuration */
)
#else /* ANSI */
PRIVATE S16 nhValidateGenCfg(gen)
NhGenCfg *gen;       /* General configuration */
#endif /* ANSI */
{

   TRC2(nhValidateGenCfg);

   if(gen->msecTmrRes == NH_ZERO_VAL)        
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if ((gen->cfgTmr.enb) && (gen->cfgTmr.val == 0))
   {
      RLOG0(L_ERROR, "nhValidateGenCfg: timer enabled but wait is 0");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhValidateGenCfg */

/**************************************************************************
 *
 *      Fun:   nhGenCfg
 *
 *      Desc:  Stores the general RRC configuration. 
 *             -It calculates the memory requirement and reserves it. 
 *             -It also initializes control blocks, hash lists and 
 *              registers the timers. 
 *             -For an error it rolls back and reports error in 
 *              the confirm.
 *             -If the general configuration is already done it is 
 *              treated as reconfiguration. Not all parameters are 
 *              reconfigurable. 
 *
 *      Ret:   ROK     - All cases 
 *             RFAILED - Invalid input parameters (NULL pointers) 
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGenCfg
(
NhGenCfg *gen,       /* General configuration */
CmStatus *status     /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhGenCfg(gen, status)
NhGenCfg *gen;       /* General configuration */
CmStatus *status;    /* Return status */
#endif /* ANSI */
{
   Size memSize;    
   S16  ret;        
   U8   numLSaps=0; 

   TRC2(nhGenCfg);

   RLOG0(L_DEBUG, "General Configuration Received");
   memSize = NULLP;

   ret = nhValidateGenCfg(gen);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "General configuration Invalid Parameters.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (nhCb.init.cfgDone == TRUE) /* Re-configuration */
   {
      (Void) cmMemcpy((U8 *)&nhCb.init.lmPst, (U8 *) &gen->lmPst, 
                      (PTR)sizeof(Pst));
      nhCb.init.lmPst.srcProcId = nhCb.init.procId;
      nhCb.init.lmPst.srcEnt    = nhCb.init.ent;
      nhCb.init.lmPst.srcInst   = nhCb.init.inst;
      nhCb.init.lmPst.event     = EVTNONE;
      RETVALUE(ROK);
   } /* End of re-configuration */

   /*  Initial configuration */ 
   cmMemcpy ((U8 *)&nhCb.genCfg, (U8 *)gen, sizeof (NhGenCfg));

   /* Memory Allocation for Upper Sap */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, &nhCb.uSapCb, 
            sizeof(NhUSapCb));
   if (NULLP == nhCb.uSapCb)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for uSap.");
      RETVALUE(RFAILED);
   }
   nhCb.uSapCb->state   = LNH_SAP_INIT;

   if ( ROK != nhLSapAlloc(gen, &numLSaps))
   {
      RLOG0(L_FATAL, "Failed to allocate memory for Lower Sap.");
      RETVALUE(RFAILED);
   }

   /* Update the memory figures */
   /* Upper Saps */
   memSize = (sizeof(NhUSapCb) + sizeof(NhUSapCb *));

   /* Lower Saps*/
   memSize += numLSaps * (sizeof(NhLSapCb) + sizeof(NhLSapCb *));

   /* For UE Hash List */
   memSize += SBUFSIZE(NH_UE_HASH_LIST_SIZE * CM_HASH_BINSIZE);

   /* For Cell Hash List */
   memSize += SBUFSIZE(NH_CELL_HASH_LIST_SIZE * CM_HASH_BINSIZE);

   /* Fill the memsize in genSta as this is required when user 
      requests for status */
   nhCb.genSta.memSize = memSize;

   /* LM Pst Information */
   (Void) cmMemcpy((U8 *)&nhCb.init.lmPst, (U8 *) &gen->lmPst, 
                   (PTR)sizeof(Pst));
   nhCb.init.lmPst.srcProcId = nhCb.init.procId;
   nhCb.init.lmPst.srcEnt    = nhCb.init.ent;
   nhCb.init.lmPst.srcInst   = nhCb.init.inst;
   nhCb.init.lmPst.event     = EVTNONE;

   /* Initialize the timer related parameters */
   cmInitTimers(nhCb.timers,(U8)NH_MAX_TMRS);

   /* Register Timers */
   if ((ret = SRegTmr(nhCb.init.ent, nhCb.init.inst,
               (S16)nhCb.genCfg.msecTmrRes, (PFS16)nhActvTmr))!= ROK)
   {
      RLOG0(L_ERROR, "Timer Registration Failed");

      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_REGTMR_FAIL;

      RETVALUE(ret);
   }

   /* Init the list for cellCb */
   cmLListInit(&(nhCb.eNodeBCb.nhCellCbList.cellCbList));
#ifdef LTE_RRC_DISSECTOR
   if(nhCb.genCfg.rrcLogEnable)
   {
      nhDissectrInit();
   }
#endif

   nhCb.init.cfgDone = TRUE;
   RETVALUE(ROK);
} /* nhGenCfg */

/**************************************************************************
 *
 *      Fun:   nhLSapAlloc
 *
 *      Desc:  Allocate memory for lower SAP. 
 *
 *      Ret:   ROK     - all cases 
 *             RFAILED - Invalid input parameters (NULL pointers) 
 *
 *      Notes: None.
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 nhLSapAlloc
(
NhGenCfg *gen,       /* General configuration */
U8   *numLSaps 
)
#else /* ANSI */
PRIVATE S16 nhLSapAlloc(gen, numLSaps)
NhGenCfg *gen;       /* General configuration */
U8   *numLSaps; 
#endif /* ANSI */
{
   U16  i = NH_ZERO_VAL;

   /* nh004.201 - Correction to TRC functionality. */   
   TRC2(nhLSapAlloc)

   /* Memory Allocation for RLC Sap */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, (Data **)&nhCb.kwuSapCbLst, 
            sizeof(NhLSapCb *) * gen->nmbKwuSaps);
   if (NULLP == nhCb.kwuSapCbLst)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for KWU sap(s).");
      RETVALUE(RFAILED);
   }
   else
   {
      /* Initialize each element of the static array of pointers */
      for (i = 0;  i < gen->nmbKwuSaps;  i++)
      {
         nhCb.kwuSapCbLst[i] = NULLP;
      }

      RLOG0(L_DEBUG, "KWU Sap(s) Initialized");
      *numLSaps += gen->nmbKwuSaps;
   }

   /* Memory Allocation for C- RLC Sap */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, &nhCb.ckwSapCbLst, 
            sizeof(NhLSapCb *) * gen->nmbCkwSaps);
   if (NULLP == nhCb.ckwSapCbLst)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for CKW sap(s).");
      RETVALUE(RFAILED);
   }
   else
   {
      /* Initialize each element of the static array of pointers */
      for (i = 0;  i < gen->nmbCkwSaps;  i++)
      {
         nhCb.ckwSapCbLst[i] = NULLP;
      }
      RLOG0(L_DEBUG, "CKW Sap(s) Initialized");
      *numLSaps += gen->nmbCkwSaps;
   }

   /* Memory Allocation for MAC SAP */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, &nhCb.crgSapCbLst, 
            sizeof(NhLSapCb *) * gen->nmbCrgSaps);
   if (NULLP == nhCb.crgSapCbLst)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for CRG sap(s).");
      RETVALUE(RFAILED);
   }
   else
   {
      /* Initialize each element of the static array of pointers */
      for (i = 0;  i < gen->nmbCrgSaps;  i++)
      {
         nhCb.crgSapCbLst[i] = NULLP;
      }
      RLOG0(L_DEBUG, "CRG Sap(s) Initialized");
      *numLSaps += gen->nmbCrgSaps;
   }

   /* Memory Allocation for PDCP SAP */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, &nhCb.pjuSapCbLst, 
            sizeof(NhLSapCb *) * gen->nmbPjuSaps);
   if (NULLP == nhCb.pjuSapCbLst)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for PJU sap(s).");
      RETVALUE(RFAILED);
   }
   else
   {
      /* Initialize each element of the static array of pointers */
      for (i = 0;  i < gen->nmbPjuSaps;  i++)
      {
         nhCb.pjuSapCbLst[i] = NULLP;
      }
      RLOG0(L_DEBUG, "PJU Sap(s) Initialized");
      *numLSaps += gen->nmbPjuSaps;
   }

   /* Memory Allocation for C-PDCP SAP */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, &nhCb.cpjSapCbLst, 
            sizeof(NhLSapCb *) * gen->nmbCpjSaps);
   if (NULLP == nhCb.cpjSapCbLst)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for CPJ sap(s).");
      RETVALUE(RFAILED);
   }
   else
   {
      /*Initialize each element of the static array of pointers */
      for (i = 0;  i < gen->nmbCpjSaps;  i++)
      {
         nhCb.cpjSapCbLst[i] = NULLP;
      }
      RLOG0(L_DEBUG, "CPJ Sap(s) Initialized");
      *numLSaps += gen->nmbCpjSaps;
   }

#ifdef RRC_PHY_CFG_SUPP
   /* Memory Allocation for C-PHY SAP */
   NH_ALLOC(nhCb.init.region, nhCb.init.pool, &nhCb.ctfSapCbLst, 
            sizeof(NhLSapCb *) * gen->nmbCtfSaps);
   if (NULLP == nhCb.ctfSapCbLst)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for CTF sap(s).");
      RETVALUE(RFAILED);
   }
   else
   {
      /*Initialize each element of the static array of pointers */
      for (i = 0;  i < gen->nmbCtfSaps;  i++)
      {
         nhCb.ctfSapCbLst[i] = NULLP;
      }
      RLOG0(L_DEBUG, "CTF Sap(s) Initialized");
      *numLSaps += gen->nmbCtfSaps;
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
}/* End of nhLSapAlloc */
/**************************************************************************
*
*      Fun:   nhValidateProtCfg
*
*      Desc:  Validates the RRC Protocol Configuration Request.
*
*      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
*             LCM_REASON_<REASON> - The reason for which validation 
*                                   failed
*
*      Notes: None
*
*      File:  nh_lmm.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateProtCfg
(
NhProtCfg *prot        
)
#else /* ANSI */
PRIVATE S16 nhValidateProtCfg(prot)
NhProtCfg *prot;      
#endif /* ANSI */
{
   TRC2(nhValidateProtCfg);

   if(prot->maxConn == 0)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhValidateProtCfg */

/**************************************************************************
 *
 *      Fun:   nhProtCfg
 *
 *      Desc:  Stores the RRC protocol configuration. The protocol
 *             configuration contains number of connections.
 *
 *      Ret:   ROK - OK 
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 nhProtCfg
(
NhProtCfg *prot,     
CmStatus  *status    /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhProtCfg(prot, status)
NhProtCfg *prot;    
CmStatus  *status;   /* Return status */
#endif /* ANSI */
{
   S16 ret;     

   TRC2(nhProtCfg);

   RLOG0(L_DEBUG, "Protocol Configuration Received");

   ret = nhValidateProtCfg(prot);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "Protocol Configuration Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }
  
   /* Re-configuration */
   if(nhCb.eNodeBCb.cfgDone == TRUE)
   {
     nhCb.eNodeBCb.protCfg.maxConn = prot->maxConn;
   } /* Re-configuration */
   else
   {
      /* Initial configuration */
      cmMemcpy ((U8 *)&nhCb.eNodeBCb.protCfg, (U8 *)prot, sizeof (NhProtCfg));
      nhCb.eNodeBCb.cfgDone = TRUE;
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;
   RETVALUE(ROK);
} /* nhProtCfg */

/**************************************************************************
*
*      Fun:   nhValidateUSapCfg
*
*      Desc:  Validates the RRC Upper SAP Configuration.
*
*      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
*             LCM_REASON_<REASON> - The reason for which validation 
*                                   failed
*      Notes: None
*
*      File:  nh_lmm.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateUSapCfg
(
NhUSapCfg *uSap      
)
#else /* ANSI */
PRIVATE S16 nhValidateUSapCfg(uSap)
NhUSapCfg *uSap;     
#endif /* ANSI */
{
   TRC2(nhValidateUSapCfg);

   /* Check if eNB configuration is done */
   if (nhCb.eNodeBCb.cfgDone != TRUE)
   {
      RETVALUE(LNH_REASON_PROTCFG_NOT_DONE);
   }

   /* Upper selector can be LC, TC, or LWLC */
   if(uSap->selector > 2)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   /*  Check for invalid priority */ 
   if(uSap->prior > PRIOR3)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhValidateUSapCfg */

/**************************************************************************
 *
 *      Fun:   nhUSapCfg
 *
 *      Desc:  Stores the RRC Upper SAP configuration.
 *
 *      Ret:   ROK - OK 
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 nhUSapCfg
(
NhUSapCfg *uSap,     /* Upper SAP configuration */
CmStatus  *status    /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhUSapCfg(uSap, status)
NhUSapCfg *uSap;     /* Upper SAP configuration */
CmStatus  *status;   /* Return status */
#endif /* ANSI */
{
   S16        ret;   

   TRC2(nhUSapCfg);

   RLOG0(L_DEBUG, "Upper SAP Configuration Received");
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the USap configuration parameters */
   ret = nhValidateUSapCfg(uSap);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "USap configuration Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   if (nhCb.uSapCb->state != LNH_SAP_INIT)
   {
      /* Re-configuration */
      nhCb.uSapCb->pst.prior  = uSap->prior;
#ifndef SS_LOCKLESS_MEMORY
      nhCb.uSapCb->pst.region = uSap->mem.region;
#endif /* SS_LOCKLESS_MEMEORY */
      nhCb.uSapCb->pst.pool   = uSap->mem.pool;
      RETVALUE(ROK);
   }

   /* Fill the parameters */
   nhCb.uSapCb->suId = 0;
   nhCb.uSapCb->spId = uSap->spId;
   nhCb.uSapCb->pst.srcProcId = nhCb.init.procId;
   nhCb.uSapCb->pst.srcEnt = nhCb.init.ent;
   nhCb.uSapCb->pst.srcInst = nhCb.init.inst;
   nhCb.uSapCb->pst.prior = uSap->prior;
   nhCb.uSapCb->pst.route = uSap->route;
#ifdef SS_LOCKLESS_MEMORY
   nhCb.uSapCb->pst.region = nhCb.init.region;
#else
   nhCb.uSapCb->pst.region = uSap->mem.region;
#endif /* SS_LOCKLESS_MEMEORY */
   nhCb.uSapCb->pst.pool = uSap->mem.pool;
   nhCb.uSapCb->pst.selector = uSap->selector;
   nhCb.uSapCb->state = LNH_SAP_CFG;

   RETVALUE(ROK);
} /* nhUSapCfg */

/**************************************************************************
*
*      Fun:   nhValidateLSapCfg
*
*      Desc:  Validates the RRC Lower SAP Configuration parameters. 
*
*      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
*             LCM_REASON_<REASON> - The reason for which validation 
*                                   failed
*      Notes: None
*
*      File:  nh_lmm.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateLSapCfg
(
U16       elmntId,
NhLSapCfg *lSap      /* Lower SAP configuration */
)
#else /* ANSI */
PRIVATE S16 nhValidateLSapCfg(elmntId, lSap)
U16       elmntId;
NhLSapCfg *lSap;     /* Lower SAP configuration */
#endif /* ANSI */
{
   S16 ret;

   TRC2(nhValidateLSapCfg);

   RLOG0(L_DEBUG, "In nhValidateLSapCfg");

   ret = nhUTLValidateLSapSuId(elmntId, lSap->suId);
   if (LCM_REASON_NOT_APPL != ret)
   {
      RETVALUE(ret);
   }

   /*  Check for invalid selector values */ 
   if(lSap->selector > 3)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   /*  Check for invalid priority */ 
   if(lSap->prior > PRIOR3)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if ((lSap->bndTmr.enb) && (lSap->bndTmr.val == 0))
   {
      RLOG0(L_ERROR, "nhValidateLSapCfg: timer enabled but wait is 0");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhValidateLSapCfg */

/**************************************************************************
 *
 *      Fun:   nhLSapCfg
 *
 *      Desc:  Stores the RRC Lower SAP configuration.
 *
 *      Ret:   ROK - OK 
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 nhLSapCfg
(
NhLSapCfg *lSap,     /* Lower SAP configuration */
U16       elmntId,   /* Element ID of lower SAP that is being configured */
CmStatus  *status    /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhLSapCfg(lSap, elmntId, status)
NhLSapCfg *lSap;     /* Lower SAP configuration */
U16       elmntId;   /* Element ID of lower SAP that is being configured */
CmStatus  *status;   /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;     

   TRC2(nhLSapCfg);

   RLOG0(L_DEBUG, "Lower SAP Configuration Received");
   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the LSAP parameters */
   ret = nhValidateLSapCfg(elmntId, lSap);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSAP configuration Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }
   /* Get SAP control block */
   switch(elmntId)
   {
      case STNHKWUSAP:
         lSapCb = nhCb.kwuSapCbLst[lSap->suId];
         break;
      case STNHCKWSAP:
         lSapCb = nhCb.ckwSapCbLst[lSap->suId];
         break;
      case STNHCRGSAP:
         lSapCb = nhCb.crgSapCbLst[lSap->suId];
         break;
      case STNHPJUSAP:
         lSapCb = nhCb.pjuSapCbLst[lSap->suId];
         break;
      case STNHCPJSAP:
         lSapCb = nhCb.cpjSapCbLst[lSap->suId];
         break;
#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:
         lSapCb = nhCb.ctfSapCbLst[lSap->suId];
         break;
#endif /* RRC_PHY_CFG_SUPP */
      default:
         RLOG0(L_ERROR, "Invalid SAP ElmId.");
         status->status = LCM_PRIM_NOK;
         RETVALUE(RFAILED);
   } /* end of switch */

   if (lSapCb != NULLP)
   {
      if (lSapCb->state != LNH_SAP_INIT )
      {
         /* Reconfiguration */
         lSapCb->pst.prior    = lSap->prior;
         lSapCb->pst.region   = lSap->mem.region;
         lSapCb->pst.pool     = lSap->mem.pool;

         RETVALUE(ROK);
      }
   }
   else
   {
      NH_ALLOC(nhCb.init.region, nhCb.init.pool, &lSapCb, 
               sizeof(NhLSapCb));
      if (NULLP == lSapCb)
      {
         RLOG0(L_FATAL, "Failed to allocate memory for lower SAP.");
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_MEM_NOAVAIL;
         RETVALUE(RFAILED);
      }
      switch(elmntId)
      {
         case STNHKWUSAP:
            nhCb.kwuSapCbLst[lSap->suId] = lSapCb;
            break;
         case STNHCKWSAP:
            nhCb.ckwSapCbLst[lSap->suId] = lSapCb;
            break;
         case STNHCRGSAP:
            nhCb.crgSapCbLst[lSap->suId] = lSapCb;
            break;
         case STNHPJUSAP:
            nhCb.pjuSapCbLst[lSap->suId] = lSapCb;
            break;
         case STNHCPJSAP:
            nhCb.cpjSapCbLst[lSap->suId] = lSapCb;
            break;
#ifdef RRC_PHY_CFG_SUPP
         case STNHCTFSAP:
            nhCb.ctfSapCbLst[lSap->suId] = lSapCb;
            break;
#endif /* RRC_PHY_CFG_SUPP */
         default:
            RLOG0(L_ERROR, "Invalid SAP ElmId.");
            status->status = LCM_PRIM_NOK;
            RETVALUE(RFAILED);
      } /* end of switch */
   }

   /* Fill the parameters */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;
   lSapCb->sapElmId = elmntId;
   lSapCb->pst.dstProcId = lSap->procId;
   lSapCb->pst.srcProcId = nhCb.init.procId;
   lSapCb->pst.dstEnt = lSap->ent;
   lSapCb->pst.dstInst = lSap->inst;
   lSapCb->pst.srcEnt = nhCb.init.ent;
   lSapCb->pst.srcInst = nhCb.init.inst;
   lSapCb->pst.prior = lSap->prior;
   lSapCb->pst.route = lSap->route;
   lSapCb->pst.region = lSap->mem.region;
   lSapCb->pst.pool = lSap->mem.pool;
   lSapCb->pst.selector = lSap->selector;
   cmMemcpy ((U8 *)&lSapCb->bndTmr, (U8 *)&lSap->bndTmr, sizeof(TmrCfg));
   lSapCb->bndRetryCnt = 0;
   lSapCb->trc = FALSE;

   cmInitTimers(lSapCb->timers, NH_MAX_LSAP_TMRS);

   lSapCb->state = LNH_SAP_CFG;

   RETVALUE(ROK);
} /* nhLSapCfg */

/**************************************************************************
 * Control Request handling functions 
 *************************************************************************/

/*******************************************************************************
 *
 *      Fun:   nhGenCntrl
 *
 *      Desc:  Process the general control request.
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGenCntrl
(
NhCntrl  *cntrl,        
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhGenCntrl(cntrl, status)
NhCntrl  *cntrl;       
CmStatus *status;       /* Return status */
#endif /* ANSI */
{
   S16   ret = ROK;

   TRC2(nhGenCntrl);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /*  Switch on Action */ 
   switch (cntrl->action)
   {
      case AENA:  /* Action Enable */
      {
         switch (cntrl->subAction)
         {
            case SAUSTA:  /* Sub Action Unsolicited Status(Alarm */
               nhCb.init.usta = TRUE;
               break;

            default:
               status->status = LCM_PRIM_NOK;
               status->reason = LCM_REASON_INVALID_SUBACTION;
               break;
         } /* end of  switch(subAction) */
         break;
      } /* End of Action ENABLE cases */

      case ADISIMM:        /* Action DISABLE */
      {
         switch (cntrl->subAction)
         {
            case SAUSTA:   /* Sub Action Unsolicited Status(Alarm) */
               nhCb.init.usta = FALSE;
               break;

            default:      
               status->status = LCM_PRIM_NOK;
               status->reason = LCM_REASON_INVALID_SUBACTION;
               break;
         } /* end of  switch(subAction) */
         break;
      } /* End of Action DISABLE cases */

      case ASHUTDOWN:       /* Action SHUTDOWN */
            ret = nhShutdown();  
         break;

      default:             
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_ACTION;
         break;
   } /* end of switch(action) */

   RETVALUE(ret);
} /* nhGenCntrl */

/*******************************************************************************
 *
 *      Fun:   nhValidateUSapCntrl
 *
 *      Desc:  Validates the RRC upper SAP control parameters. 
 *
 *      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
 *             LCM_REASON_<REASON> - The reason for which validation 
 *                                   failed
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateUSapCntrl
(
NhCntrl  *cntrl         
)
#else /* ANSI */
PRIVATE S16 nhValidateUSapCntrl(cntrl)
NhCntrl  *cntrl;       
#endif /* ANSI */
{

   TRC2(nhValidateUSapCntrl);
   
   /*  Switch on Action */ 
   switch (cntrl->action)
   {
      case ADEL:         /* Action DELETE :fall through */
      case AUBND_DIS:    /* Action UBIND and DISABLE */
         if (nhCb.uSapCb->state == LNH_SAP_INIT)
         {
             RETVALUE(LCM_REASON_INVALID_PAR_VAL);
         }
         break;
      default:
         RETVALUE(LCM_REASON_INVALID_ACTION);
   } /* end of switch(action) */

   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhValidateUSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhUSapCntrl
 *
 *      Desc:  Process the upper SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhUSapCntrl
(
NhCntrl  *cntrl,         
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhUSapCntrl(cntrl, status)
NhCntrl  *cntrl;         
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16 ret;              

   TRC2(nhUSapCntrl);

   ret = nhValidateUSapCntrl(cntrl);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "USap Control Request Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /*  Switch on action */ 
   switch (cntrl->action)
   {
      case AUBND_DIS:      /* Action UBIND and DISABLE */
         nhCb.uSapCb->state = LNH_SAP_CFG;
         break;

      case ADEL:           /* Action DELETE */
         cmMemset((U8 *)nhCb.uSapCb, 0 , sizeof(NhUSapCb));
         nhCb.uSapCb->state = LNH_SAP_INIT;
         break;

      default:          
         break;
   } /* end of switch (cntrl->action) */

   RETVALUE(ROK);
} /* nhUSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhValidateLSapCntrl
 *
 *      Desc:  Validates the RRC lower SAP control parameters and
 *             returns the lSapCb.
 *
 *      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
 *             LCM_REASON_<REASON> - The reason for which validation 
 *                                   failed
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateLSapCntrl
(
U8       sapType,
NhCntrl  *cntrl,        
NhLSapCb **lSapCb       
)
#else /* ANSI */
PRIVATE S16 nhValidateLSapCntrl(sapType, cntrl, lSapCb)
U8       sapType;
NhCntrl  *cntrl;      
NhLSapCb **lSapCb;    
#endif /* ANSI */
{
   S16   ret;

   TRC2(nhValidateLSapCntrl);

   ret = nhUTLValidateLSapSuId(sapType, cntrl->u.sap.sapId);

   *lSapCb = nhUTLFindLlyrSapFrmSuId(sapType, cntrl->u.sap.sapId);
   if ((LCM_REASON_NOT_APPL != ret) || (NULLP == *lSapCb))
   {
      RETVALUE(LNH_REASON_LSAPCFG_NOT_DONE);
   }

   /*  Switch on action */ 
   switch (cntrl->action)
   {
      case ADEL:        /* Action DELETE: Intentional fall through */
      case ABND_ENA:    /* Action BIND and ENABLE */
      case AUBND_DIS:   /* Action UNBIND and DISABLE */
      case AENA:        /* Enable trace */
      case ADISIMM:    /* Disable trace */   
         if ((*lSapCb)->state == LNH_SAP_INIT)
         {
           RETVALUE(LNH_REASON_LSAPCFG_NOT_DONE);
         }
         break;

      default:
         RETVALUE(LCM_REASON_INVALID_ACTION);
   } /* end of switch (cntrl->action) */

   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhValidateLSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhLSapCntrlActHndl
 *
 *      Desc:  Performs the specified action the SAP.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhLSapCntrlActHndl
(
S16       action,   
S16       sAction,
NhLSapCb *lSapCb,
U8       evntType,
U8       lSapType
)
#else /* ANSI */
PUBLIC S16 nhLSapCntrlActHndl(action, sAction, lSapCb, evntType, lSapType)
S16       action;
S16       sAction;
NhLSapCb *lSapCb;
U8       evntType;
U8       lSapType;
#endif /* ANSI */
{
   S16 ret;

   TRC2(nhLSapCntrlActHndl);
 
   ret = ROK;

   /*  Switch on action */ 
   switch (action)
   {
      case ABND_ENA:     /* Action BIND and ENABLE */
         /* start timer to wait for bind confirm */
         if (lSapCb->bndTmr.enb == TRUE)
         {
            ret = nhSchedGenTmr(lSapCb, (S16)evntType, NH_TMR_START);
         }

         if(ret == ROK)
         {
            lSapCb->state = LNH_SAP_BINDING;
            nhLIMSendBndReq(lSapType, lSapCb);
         }
         break;

      case AUBND_DIS:    /* Action UNBIND and DISABLE */
   
         /*  Stop bind timer related to this SAP 
          *  as this is the only timer that might be running
          */ 
         if (lSapCb->state == LNH_SAP_BINDING)
         {
            ret = nhSchedGenTmr(lSapCb, (S16)evntType, NH_TMR_STOP);
         }

         /*  send a Unbind Request to RLC */ 
         if(ret == ROK)
         {
            nhLIMSendUbndReq(lSapType, lSapCb);
            lSapCb->state = LNH_SAP_CFG;
         }
         break;

      case ADEL:         /* Action  DELETE */
   
         /*  Stop bind timer related to this SAP 
          *  as this is the only timer that might be running
          */ 
         if (lSapCb->state == LNH_SAP_BINDING)
         {
            ret = nhSchedGenTmr(lSapCb, (S16)evntType, NH_TMR_STOP);
         }

         /*  Send an Unbind Request to RLC */ 
         if(ret == ROK)
         {
            nhLIMSendUbndReq(lSapType, lSapCb);
            cmMemset((U8 *)lSapCb, 0, sizeof(NhLSapCb));
            lSapCb->state = LNH_SAP_INIT;
         }
         break;
#ifdef NH_TRACE_ENABLED
      case AENA:   /* trace enabling */
         if (sAction == SATRC)
         {
            lSapCb->trc = TRUE;
         }
         break;
      case ADISIMM:    /* Disable trace */   
         if (sAction == SATRC)
         {
            lSapCb->trc = FALSE;
         }

         break;
#endif /* NH_TRACE_ENABLED */
      default:        
         break;
    } /* end of switch (cntrl->action) */

    RETVALUE(ret);
} /* nhLSapCntrlActHndl */

/*******************************************************************************
 *
 *      Fun:   nhKwuSapCntrl
 *
 *      Desc:  Process the KWU SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhKwuSapCntrl
(
NhCntrl  *cntrl,   
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhKwuSapCntrl(cntrl, status)
NhCntrl  *cntrl;   
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;  
   U8       nmbKwuSaps;

   TRC2(nhKwuSapCntrl);

   nmbKwuSaps = nhCb.genCfg.nmbKwuSaps;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateLSapCntrl (STNHKWUSAP, cntrl, &lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSap Control Request Failed for KWU.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   /* No need to check for ret value, as it will always be ROK */
   nhLSapCntrlActHndl(cntrl->action, cntrl->subAction, lSapCb, 
                            NH_TMR_WAIT_KWU_BNDCFM, STNHKWUSAP); 

   RETVALUE(ROK);
   
} /* nhKwuSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhCkwSapCntrl
 *
 *      Desc:  Process the CKW SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhCkwSapCntrl
(
NhCntrl  *cntrl,   
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhCkwSapCntrl(cntrl, status)
NhCntrl  *cntrl;   
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;  
   U8       nmbCkwSaps;

   TRC2(nhCkwSapCntrl);

   nmbCkwSaps = nhCb.genCfg.nmbCkwSaps;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateLSapCntrl (STNHCKWSAP, cntrl, &lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSap Control Request Failed for CKW.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   /* No need to check for ret value, as it will always be ROK */
   nhLSapCntrlActHndl(cntrl->action, cntrl->subAction, lSapCb, 
                            NH_TMR_WAIT_CKW_BNDCFM, STNHCKWSAP); 

   RETVALUE(ROK);
} /* nhCkwSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhCrgSapCntrl
 *
 *      Desc:  Process the CRG SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhCrgSapCntrl
(
NhCntrl  *cntrl,   
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhCrgSapCntrl(cntrl, status)
NhCntrl  *cntrl;   
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;  
   U8       nmbCrgSaps;

   TRC2(nhCrgSapCntrl);

   nmbCrgSaps = nhCb.genCfg.nmbCrgSaps;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateLSapCntrl (STNHCRGSAP, cntrl, &lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSap Control Request Failed Failed for CRG.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   /* No need to check for ret value, as it will always be ROK */
   nhLSapCntrlActHndl(cntrl->action, cntrl->subAction, lSapCb, 
                            NH_TMR_WAIT_CRG_BNDCFM, STNHCRGSAP); 

   RETVALUE(ROK);
} /* nhCrgSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhPjuSapCntrl
 *
 *      Desc:  Process the PJU SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhPjuSapCntrl
(
NhCntrl  *cntrl,   
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhPjuSapCntrl(cntrl, status)
NhCntrl  *cntrl;   
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;  
   U8       nmbPjuSaps;

   TRC2(nhPjuSapCntrl);

   nmbPjuSaps = nhCb.genCfg.nmbPjuSaps;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateLSapCntrl (STNHPJUSAP, cntrl, &lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSap Control Request Failed for PJU.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   /* No need to check for ret value, as it will always be ROK */
   nhLSapCntrlActHndl(cntrl->action, cntrl->subAction, lSapCb, 
                            NH_TMR_WAIT_PJU_BNDCFM, STNHPJUSAP); 

   RETVALUE(ROK);
} /* nhPjuSapCntrl */

/*******************************************************************************
 *
 *      Fun:   nhCpjSapCntrl
 *
 *      Desc:  Process the CPJ SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhCpjSapCntrl
(
NhCntrl  *cntrl,   
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhCpjSapCntrl(cntrl, status)
NhCntrl  *cntrl;   
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;  
   U8       nmbCpjSaps;

   TRC2(nhCpjSapCntrl);

   nmbCpjSaps = nhCb.genCfg.nmbCpjSaps;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateLSapCntrl (STNHCPJSAP, cntrl, &lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSap Control Request Failed for CPJ");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   /* No need to check for ret value, as it will always be ROK */
   nhLSapCntrlActHndl(cntrl->action, cntrl->subAction, lSapCb, 
                            NH_TMR_WAIT_CPJ_BNDCFM, STNHCPJSAP); 

   RETVALUE(ROK);
} /* nhCpjSapCntrl */

#ifdef RRC_PHY_CFG_SUPP
/*******************************************************************************
 *
 *      Fun:   nhCtfSapCntrl
 *
 *      Desc:  Process the CTF SAP control request.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhCtfSapCntrl
(
NhCntrl  *cntrl,   
CmStatus *status        /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhCtfSapCntrl(cntrl, status)
NhCntrl  *cntrl;   
CmStatus *status;        /* Return status */
#endif /* ANSI */
{
   S16      ret;       
   NhLSapCb *lSapCb;  
   U8       nmbCtfSaps;

   TRC2(nhCtfSapCntrl);

   nmbCtfSaps = nhCb.genCfg.nmbCtfSaps;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateLSapCntrl (STNHCTFSAP, cntrl, &lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSap Control Request Failed for CTF.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   /* No need to check for ret value, as it will always be ROK */
   nhLSapCntrlActHndl(cntrl->action, cntrl->subAction, lSapCb, 
                            NH_TMR_WAIT_CTF_BNDCFM, STNHCTFSAP); 

   RETVALUE(ROK);
} /* nhCtfSapCntrl */
#endif /* RRC_PHY_CFG_SUPP */

/***************************************************************************
 *  Statistics handler functions 
 ***************************************************************************/

/******************************************************************************
 *
 *      Fun:   nhValidateSts
 *
 *      Desc:  Validates the RRC general statistics request.
 *
 *      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
 *             LCM_REASON_<REASON> - The reason for which validation 
 *                                   failed
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateSts
(
Action   action
)
#else /* ANSI */
PRIVATE S16 nhValidateSts(action)
Action   action;
#endif /* ANSI */
{

   TRC2(nhValidateSts);
   if (action != ZEROSTS && action != NOZEROSTS)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* nhValidateSts */

/*******************************************************************************
 *
 *      Fun:   nhGetGenSts
 *
 *      Desc:  Gather the general statistics.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGetGenSts
(
NhGenSts *sts,   
CmStatus *status,           /* Return status */
Action   action
)
#else /* ANSI */
PUBLIC S16 nhGetGenSts(sts, status, action)
NhGenSts *sts;  
CmStatus *status;           /* Return status */
Action   action;
#endif /* ANSI */
{
   S16 ret;   

   TRC2(nhGetGenSts);

   ret = nhValidateSts (action);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "Invalid Action for statistics.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;
   cmMemcpy ((U8 *)sts, (U8 *)&nhCb.genSts, sizeof (NhGenSts));

   if (action == ZEROSTS)
   {
      cmMemset ((U8 *)&nhCb.genSts, 0, sizeof (NhGenSts));
   }
   RETVALUE(ROK);
} /* nhGetGenSts */
 
/******************************************************************************
 *
 *      Fun:   nhGetUeSts
 *
 *      Desc:  Gather the UE statistics.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *****************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGetUeSts
(
NhUeSts     *sts,         
CmStatus    *status,      /* Return status */
Action      action,      
NhActvUeCb  *ueCb       
)
#else /* ANSI */
PUBLIC S16 nhGetUeSts(sts, status, action, ueCb)
NhUeSts     *sts;    
CmStatus    *status;      /* Return status */
Action      action; 
NhActvUeCb  *ueCb;    
#endif /* ANSI */
{
   S16 ret;            

   TRC2(nhGetUeSts);

   ret = nhValidateSts (action);

   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "UE statistics validation Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   cmMemcpy((U8 *)sts, (U8 *)&(ueCb->ueSts),sizeof(NhUeSts));

   if (action == ZEROSTS)
   {
      cmMemset ((U8 *)&(ueCb->ueSts), 0, sizeof(NhUeSts));
   }

   RETVALUE(ROK);

} /* nhGetUeSts */

/******************************************************************************
 *
 *      Fun:   nhGetCellSts
 *
 *      Desc:  Gather the Cell statistics.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *****************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGetCellSts
(
NhCellSts      *sts,         
CmStatus       *status,      /* Return status */
Action         action,      
NhCellCb       *cellCb     
)
#else /* ANSI */
PUBLIC S16 nhGetCellSts(sts, status, action, cellCb)
NhCellSts      *sts;      
CmStatus       *status;      /* Return status */
Action         action;   
NhCellCb       *cellCb; 
#endif /* ANSI */
{
   S16 ret;                 

   TRC2(nhGetCellSts);

   ret = nhValidateSts (action);

   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "CELL statistics validation Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   cmMemcpy((U8 *)sts, (U8 *)&(cellCb->cellSts), 
                                   sizeof (cellCb->cellSts));
   if (action == ZEROSTS)
   {
      cmMemset ((U8 *)&(cellCb->cellSts), 0, 
                                   sizeof (cellCb->cellSts));
   }

   RETVALUE(ROK);

} /* nhGetCellSts */

/******************************************************************************
* Status Request handler functions
******************************************************************************/
/*******************************************************************************
 *
 *      Fun:   nhValidateLSapSta
 *
 *      Desc:  Validates the RRC Lower SAP status.
 *
 *      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
 *             LCM_REASON_INVALID_PAR_VAL - Failure
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateLSapSta
(
NhLSapCb *lSapCb
)
#else /* ANSI */
PRIVATE S16 nhValidateLSapSta(lSapCb)
NhLSapCb *lSapCb;
#endif /* ANSI */
{

   TRC2(nhValidateLSapSta);

   if (lSapCb->state == LNH_SAP_INIT)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   RETVALUE(LCM_REASON_NOT_APPL);

} /* nhValidateLSapSta */
 

/******************************************************************************
 *
 *      Fun:   nhGetLSapSta
 *
 *      Desc:  Gather the lower SAP RRC status.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGetLSapSta
(
NhSapSta *sta,         /* Lower SAP status */
U16      elmntId,      /* Element ID of lower SAP */
CmStatus *status       /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhGetLSapSta(sta, elmntId, status)
NhSapSta *sta;         /* Lower SAP status */
U16      elmntId;      /* Element ID of lower SAP */
CmStatus *status;      /* Return status */
#endif /* ANSI */
{
   S16 ret;     
   NhLSapCb *lSapCb = NULLP;  

   TRC2(nhGetLSapSta);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (elmntId)
   {
      case STNHKWUSAP:         /* KWU SAP status */
         lSapCb = nhCb.kwuSapCbLst[sta->sapId];
         break;

      case STNHCKWSAP:         /* CKW SAP status */
         lSapCb = nhCb.ckwSapCbLst[sta->sapId];
         break;

      case STNHCRGSAP:         /* CRG SAP status */
         lSapCb = nhCb.crgSapCbLst[sta->sapId];
         break;

      case STNHPJUSAP:         /* PJU SAP status */
         lSapCb = nhCb.pjuSapCbLst[sta->sapId];
         break;

      case STNHCPJSAP:         /* CPJ SAP status */
         lSapCb = nhCb.cpjSapCbLst[sta->sapId];
         break;

#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:         /* CTF SAP status */
         lSapCb = nhCb.ctfSapCbLst[sta->sapId];
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
         {
            RLOG0(L_ERROR, "Invalid SAP ElmId.");
            /* would never enter here as the switch statement from its caller
               will take care of this invalid element ID. */
            RETVALUE(RFAILED);
         }
   } /* end of switch(elmntId) */

   ret = nhValidateLSapSta (lSapCb);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "LSAP Status Request validation Failed.");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   sta->sapState  = lSapCb->state;
   RETVALUE(ROK);
} /* nhGetLSapSta */

/******************************************************************************
 *
 *      Fun:   nhValidateUSapSta
 *
 *      Desc:  Validates the RRC Upper SAP status.
 *
 *      Ret:   LCM_REASON_NOT_APPL - Validation succeeded
 *             LCM_REASON_<REASON> - The reason for which validation 
 *                                   failed
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *****************************************************************************/
#ifdef ANSI
PRIVATE S16 nhValidateUSapSta
(
NhSapSta *sta          /* Upper SAP status */
)
#else /* ANSI */
PRIVATE S16 nhValidateUSapSta(sta)
NhSapSta *sta;         /* Upper SAP status */
#endif /* ANSI */
{

   TRC2(nhValidateUSapSta);

   UNUSED(sta);

   if (nhCb.uSapCb->state == LNH_SAP_INIT)
   {
      RETVALUE(LNH_REASON_USAPCFG_NOT_DONE);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* nhValidateUSapSta */
 
/*******************************************************************************
 *
 *      Fun:   nhGetUSapSta
 *
 *      Desc:  Gather the general RRC status.
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ******************************************************************************/
#ifdef ANSI
PUBLIC S16 nhGetUSapSta
(
NhSapSta *sta,         /* Upper SAP status */
CmStatus *status       /* Return status */
)
#else /* ANSI */
PUBLIC S16 nhGetUSapSta(sta, status)
NhSapSta *sta;         /* Upper SAP status */
CmStatus *status;      /* Return status */
#endif /* ANSI */
{
   S16 ret;        /* Local return value */

   TRC2(nhGetUSapSta);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   ret = nhValidateUSapSta (sta);
   if (ret != LCM_REASON_NOT_APPL)
   {
      RLOG0(L_ERROR, "USap Status Request validation Failed ");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   sta->sapState  = nhCb.uSapCb->state;
   RETVALUE(ROK);

} /* nhGetUSapSta */

/**************************************************************************
 *
 *      Fun:   nhLMMSndLmCfm
 *
 *      Desc:  Form the confirm message to be send to the Layer Manager.
 *
 *      Ret:   ROK - OK 
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 nhLMMSndLmCfm
(
Pst     *pst,
Header  *hdr,
U8      type,
NhMngmt *cfm
)
#else /* ANSI */
PUBLIC S16 nhLMMSndLmCfm(pst, hdr, type, cfm)
Pst     *pst;
Header  *hdr;
U8      type;
NhMngmt *cfm;
#endif /* ANSI */
{
   Pst pstCfm;    /* The reply post structure */

   TRC2(nhLMMSndLmCfm);

   /* Fill the reply post structure */
   pstCfm.srcEnt    = nhCb.init.ent;
   pstCfm.srcInst   = nhCb.init.inst;
   pstCfm.srcProcId = SFndProcId();
   pstCfm.dstEnt    = pst->srcEnt;
   pstCfm.dstInst   = pst->srcInst;
   pstCfm.dstProcId = pst->srcProcId;
   pstCfm.selector  = hdr->response.selector;
   pstCfm.prior     = hdr->response.prior;
   pstCfm.route     = hdr->response.route;
#ifdef SS_LOCKLESS_MEMORY
   pstCfm.region    = nhCb.init.region;
#else
   pstCfm.region    = hdr->response.mem.region;
#endif /* SS_LOCKLESS_MEMEORY */
   pstCfm.pool      = hdr->response.mem.pool;

   /* Fill the reply header */
   cmMemset ((U8 *)&cfm->hdr, '\0', sizeof(cfm->hdr));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent   = nhCb.init.ent;
   cfm->hdr.entId.inst  = nhCb.init.inst;
   cfm->hdr.transId     = hdr->transId;
   cfm->hdr.msgType     = hdr->msgType;

   /* Send the corresponding confirm */
   switch (type)
   {
      case TCFG:
         NhMiLnhCfgCfm (&pstCfm, cfm);
         break;

      case TCNTRL:
         NhMiLnhCntrlCfm (&pstCfm, cfm);
         break;

      case TSTS:
         /* update the date and time */
         (Void) SGetDateTime(&cfm->u.sts.dt);
         NhMiLnhStsCfm (&pstCfm, cfm);
         break;

      case TSSTA:
         /* update the date and time */
         (Void) SGetDateTime(&cfm->u.ssta.dt);
         NhMiLnhStaCfm (&pstCfm, cfm);
         break;
      default:
         RLOG0(L_ERROR, "Invalid msgType.");
         break;
   } /* end of switch */
   RETVALUE(ROK);
} /* nhLMMSndLmCfm */

/******************************************************************************
 *
 *      Fun:   nhSendLmAlarm
 *
 *      Desc:  Generate the Confirm to the Layer Manager.
 *
 *      Ret:   RETVOID
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ****************************************************************************/
#ifdef ANSI
PUBLIC Void nhSendLmAlarm
(
U16 category,           
U16 event,             
U16 cause,            
NhUstaDgn *dgn       
)
#else /* ANSI */
PUBLIC Void nhSendLmAlarm(category, event, cause, dgn)
U16 category;       
U16 event;         
U16 cause;        
NhUstaDgn *dgn;  
#endif /* ANSI */
{
   NhMngmt usta;        /* Unsolicited Status Indication structure */

   TRC2(nhSendLmAlarm);
   
   if (nhCb.init.usta == FALSE)
   {
      RETVOID;
   }
 
   /* Initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(usta));
 
   usta.hdr.elmId.elmnt = STNHGEN;
   usta.hdr.entId.ent   = nhCb.init.ent;
   usta.hdr.entId.inst  = nhCb.init.inst;

   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event    = event;
   usta.u.usta.alarm.cause    = cause;

   /* Copy the usta structure */
   cmMemcpy((U8 *)&usta.u.usta.dgn, (U8 *)dgn, (PTR)sizeof(NhUstaDgn));

   /* Update the date and time */
   (Void) SGetDateTime(&usta.u.usta.alarm.dt);
 
   /* Send an unsolicited status indication */
   NhMiLnhStaInd(&nhCb.genCfg.lmPst, &usta);
 
   RETVOID;

} /* nhSendLmAlarm */

/**
 * @brief This function is used to shutdown the RRC layer.
 * @details 
<b> nhShutdown </b> 

 * This function is used to shutdown the RRC Layer. It calls the nhPrcCmplShutdown function to shutdown the RRC layer.
 *

 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PUBLIC S16 nhShutdown
(
Void
)
#else /* ANSI */
PUBLIC S16 nhShutdown(Void)
#endif /* ANSI */
{
   TRC2(nhShutdown)

   if (nhCb.init.cfgDone != TRUE)
   {
      /* Nothing to be done */
      RETVALUE(ROK);
   }

   nhPrcCmplShutdown();
   RETVALUE(ROK);
} /* nhShutdown */

/******************************************************************************
 *
 *      Fun:   nhPrcCmplShutdown
 *
 *      Desc:  This function is called by nhShutdown to shutdown the
 *             RRC Layer. This function deinitializes all the hashlists.
 *             Also, frees all the buffers.
 *
 *      Ret:   RETVOID
 *
 *      Notes: None
 *
 *      File:  nh_lmm.c
 *
 ****************************************************************************/
#ifdef ANSI
PUBLIC Void nhPrcCmplShutdown
(
Void
)
#else /* ANSI */
PUBLIC Void nhPrcCmplShutdown()
#endif /* ANSI */
{

   TRC2(nhPrcCmplShutdown)

   /* Deregister the timers */
   SDeregTmr(nhCb.init.ent, nhCb.init.inst, (S16)nhCb.genCfg.msecTmrRes,
               (PFS16)nhActvTmr); 

   /* Free SAP control block */
   nhFreeSapCb();

   /* Delete the CELL Control Blocks, if allocated any */
   nhFreeCellCb();

   /* return to Init state */
   nhActvInit (nhCb.init.ent, nhCb.init.inst,
               nhCb.init.region, nhCb.init.reason);

   RETVOID;
} /* end of function nhPrcCmplShutdown */

/******************************************************************************
 *
 *      Fun:   nhFreeSapCb
 *
 *      Desc:  This function is called by nhPrcCmplShutdown 
 *             This function frees the SAP control block. 
 *
 *      Ret:   RETVOID
 *
 *      Notes: None.
 *
 *      File:  nh_lmm.c
 *
 ****************************************************************************/
#ifdef ANSI
PRIVATE S16 nhFreeSapCb
(
)
#else /* ANSI */
PRIVATE S16 nhFreeSapCb()
#endif /* ANSI */
{

   U16 i = 0;
   NhLSapCb  *lSapCb = NULLP;

   TRC2(nhFreeSapCb)
   /* Remove the memory allocated to different saps */
   if (nhCb.uSapCb != NULLP)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.uSapCb, 
              sizeof(NhUSapCb));
      nhCb.uSapCb = NULLP;
   }

   /* Delete KWU SAP Cb */
   for(i = 0; i < nhCb.genCfg.nmbKwuSaps; i++)
   {
      lSapCb =  nhCb.kwuSapCbLst[i];
      if (lSapCb != NULLP)
      {
        NH_FREE(NH_REG, NH_POOL, lSapCb, 
                 sizeof(NhLSapCb ));
         nhCb.kwuSapCbLst[i] = NULLP;
      }
   }
   if (nhCb.genCfg.nmbKwuSaps)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.kwuSapCbLst, 
             sizeof(NhLSapCb *) * nhCb.genCfg.nmbKwuSaps);
   }
  
   /* Delete CKW SAP Cb */
   for(i = 0; i < nhCb.genCfg.nmbCkwSaps; i++)
   {
      lSapCb =  nhCb.ckwSapCbLst[i];
      if (lSapCb != NULLP)
      {
        NH_FREE(NH_REG, NH_POOL, lSapCb, 
                 sizeof(NhLSapCb ));
         nhCb.ckwSapCbLst[i] = NULLP;
      }
   }
   if (nhCb.genCfg.nmbCkwSaps)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.ckwSapCbLst, 
             sizeof(NhLSapCb *) * nhCb.genCfg.nmbCkwSaps);
   }

   /* Delete CRG SAP Cb */
   for(i = 0; i < nhCb.genCfg.nmbCrgSaps; i++)
   {
      lSapCb =  nhCb.crgSapCbLst[i];
      if (lSapCb != NULLP)
      {
        NH_FREE(NH_REG, NH_POOL, lSapCb, 
                 sizeof(NhLSapCb ));
         nhCb.crgSapCbLst[i] = NULLP;
      }
   }
   if (nhCb.genCfg.nmbCrgSaps)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.crgSapCbLst, 
             sizeof(NhLSapCb *) * nhCb.genCfg.nmbCrgSaps);
   }

   /* Delete CPJ SAP Cb */
   for(i = 0; i < nhCb.genCfg.nmbCpjSaps; i++)
   {
      lSapCb =  nhCb.cpjSapCbLst[i];
      if (lSapCb != NULLP)
      {
        NH_FREE(NH_REG, NH_POOL, lSapCb, 
                 sizeof(NhLSapCb ));
         nhCb.cpjSapCbLst[i] = NULLP;
      }
   }
   if (nhCb.genCfg.nmbCpjSaps)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.cpjSapCbLst, 
             sizeof(NhLSapCb *) * nhCb.genCfg.nmbCpjSaps);
   }

   /* Delete PJU SAP Cb */
   for(i = 0; i < nhCb.genCfg.nmbPjuSaps; i++)
   {
      lSapCb =  nhCb.pjuSapCbLst[i];
      if (lSapCb != NULLP)
      {
        NH_FREE(NH_REG, NH_POOL, lSapCb, 
                 sizeof(NhLSapCb ));
         nhCb.pjuSapCbLst[i] = NULLP;
      }
   }
   if (nhCb.genCfg.nmbPjuSaps)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.pjuSapCbLst, 
             sizeof(NhLSapCb *) * nhCb.genCfg.nmbPjuSaps);
   }
   
#ifdef RRC_PHY_CFG_SUPP
   /* Delete CTF SAP Cb */
   for(i = 0; i < nhCb.genCfg.nmbCtfSaps; i++)
   {
      lSapCb =  nhCb.ctfSapCbLst[i];
      if (lSapCb != NULLP)
      {
        NH_FREE(NH_REG, NH_POOL, lSapCb, 
                 sizeof(NhLSapCb));
         nhCb.ctfSapCbLst[i] = NULLP;
      }
   }
   if (nhCb.genCfg.nmbCtfSaps)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.ctfSapCbLst, 
             sizeof(NhLSapCb *) * nhCb.genCfg.nmbCtfSaps);
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
} /* nhFreeSapCb */

/******************************************************************************
 *
 *      Fun:   nhFreeCellCb
 *
 *      Desc:  This function is called by nhPrcCmplShutdown 
 *             This function frees the Cell control block before freeing 
 *             the cell Cb it checks if any Ue Cb exist in it. If Ue Cb exist
 *             first free the Ue Cb. 
 *
 *      Ret:   RETVOID
 *
 *      Notes: None.
 *
 *      File:  nh_lmm.c
 *
 ****************************************************************************/
#ifdef ANSI
PRIVATE S16 nhFreeCellCb
(
)
#else /* ANSI */
PRIVATE S16 nhFreeCellCb()
#endif /* ANSI */
{
   S16        cbRetVal;  /* Ret Value from CB operations */
   CmLList    *tmpNode = NULLP;
   NhCellCb   *cellCb = NULLP;
   
   TRC2(nhFreeCellCb)
   
   /* Find Cell Cb*/
   CM_LLIST_FIRST_NODE(&(nhCb.eNodeBCb.nhCellCbList.cellCbList), tmpNode);
   while(NULLP != tmpNode)
   {
      if (NULLP == tmpNode->node)
      {
         RLOG0(L_ERROR, "NULL logChCb");
         RETVALUE(RFAILED);
      }
      cellCb = (NhCellCb *)(tmpNode->node);

      cbRetVal = nhDBMDeleteCellCb((NhCellCb **)&cellCb);
      if (cbRetVal != ROK)
      {
         RLOG0(L_ERROR, "Deleting CELL Control Block Failed ");
      }
      CM_LLIST_NEXT_NODE(&(nhCb.eNodeBCb.nhCellCbList.cellCbList), tmpNode);
   }
   
   RETVALUE(ROK);

}/* nhFreeCellCb */
/******************************************************************************
 *
 *      Fun:   nhFreeSapLst
 *
 *      Desc:  This function is called if there is problem 
 *             in gen configuration. This function frees the memory for
 *             SAP list if it is allocated during gen configuration.
 *
 *      Ret:   RETVOID
 *
 *      Notes: None.
 *
 *      File:  nh_lmm.c
 *
 ****************************************************************************/
#ifdef ANSI
PRIVATE S16 nhFreeSapLst
(
NhGenCfg *gen,       /* General configuration */
CmStatus *status    /* Return status */
)
#else /* ANSI */
PRIVATE S16 nhFreeSapLst(gen, status)
NhGenCfg *gen;       /* General configuration */
CmStatus *status;    /* Return status */
#endif /* ANSI */
{

   TRC2(nhFreeSapLst)


   /* Remove the memory allocated to different saps */
   if (nhCb.uSapCb != NULLP)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.uSapCb, 
              sizeof(NhUSapCb));
      nhCb.uSapCb = NULLP;
   }
   if (nhCb.kwuSapCbLst != NULLP)
   {
      NH_FREE(nhCb.init.region, nhCb.init.pool, nhCb.kwuSapCbLst, 
           sizeof(NhLSapCb *) * gen->nmbKwuSaps);
   }
   if (nhCb.ckwSapCbLst != NULLP)
   {
      NH_FREE(nhCb.init.region, nhCb.init.pool, nhCb.ckwSapCbLst, 
           sizeof(NhLSapCb *) * gen->nmbCkwSaps);
   }
   if (nhCb.crgSapCbLst != NULLP)
   {
      NH_FREE(nhCb.init.region, nhCb.init.pool, nhCb.crgSapCbLst, 
           sizeof(NhLSapCb *) * gen->nmbCrgSaps);
   }
   if (nhCb.pjuSapCbLst != NULLP)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.pjuSapCbLst,
           sizeof(NhLSapCb *) * gen->nmbPjuSaps);
   }
   if (nhCb.cpjSapCbLst != NULLP)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.cpjSapCbLst, 
        (sizeof(NhLSapCb *) * gen->nmbCpjSaps));
   }
#ifdef RRC_PHY_CFG_SUPP
   if (nhCb.ctfSapCbLst != NULLP)
   {
      NH_FREE(NH_REG, NH_POOL, nhCb.ctfSapCbLst, 
        (sizeof(NhLSapCb *) * gen->nmbCtfSaps));
   }
#endif /* RRC_PHY_CFG_SUPP */
   /* if reason is memory allocation failure set the reason and
      status */
   if ((status->reason != LCM_REASON_REGTMR_FAIL) &&
       (status->reason != LCM_REASON_INVALID_PAR_VAL))
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
   }

   RETVALUE(ROK);
} /* end of function nhFreeSapLst */

/** @} */
/**********************************************************************
         End of file:     ni_lmm.c@@/main/3 - Fri Jul  1 01:13:20 2011
 
**********************************************************************/
 
 
/**********************************************************************
        Revision history:
 
**********************************************************************/
/********************************************************************90**
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
           nh004.201  vkulkarni        1. Correction to TRC functionality.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
           nh001.301  sbalakrishna     1. Changed elmnt ID from STGEN to 
                                          STNHGEN.
                                       2. Added condition check for Shutdown
                                          action as subaction must be ignored
                                          in this case
                                       3. Added code for Specified Elements, 
                                          Unsolicited Alarms and Peer SAP 
                                          disabling subactions.
/main/4  nh002.301    njha	       1. Updating control Inform and NH_DIAG_LVL0
*********************************************************************91*/
