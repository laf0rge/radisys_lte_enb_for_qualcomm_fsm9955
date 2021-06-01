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
               

     File:     wr_smm_rrm.c

     Sid:      fc_smm_rrm.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:52:28 2014

     Prg:      Sriky 

*********************************************************************21*/


/* header include files (.h) */
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=15;

#include "wr.h"
#include "wr_cmn.h"
#include "lrm.h"
#include "rmu.h"
#include "rgm.h"

#include "wr_lwr.h"
#include "wr_smm_smallcell.h"
#include "wr_smm_init.h"
#include "wr_utils.h"
#include "lrm.x"
#include "wr_msm_common.h"

/* wr004.102:g++ */
#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

#ifdef PTSMMILRM
PRIVATE S16 PtMiLrmCfgReq ARGS (( Pst *pst, RmMngmt *cfg));
PRIVATE S16 PtMiLrmCntrlReq ARGS (( Pst *pst, RmMngmt *cfg));
PRIVATE S16 PtMiLrmEnbStaInd ARGS (( Pst *pst, RmMngmt *cfg));
PRIVATE S16 PtMiLrmStaInd ARGS (( Pst *pst, RmMngmt *cfg));
#endif /*PTSMMILRM*/

PRIVATE S16 smBuildRmRgmLSapCfg ARGS((  SuId suId, SpId spId ));
PRIVATE S16 smBuildRmRmuUSapCfg ARGS(( SpId spId ));
PRIVATE S16 smBuildRmGenCfg ARGS((Void));
PRIVATE S16 smSndRmuProtCellCfg ARGS((Void));
PRIVATE S16 smSndRmuProtCellDel ARGS((Void));
PRIVATE S16 smSndRmuProtEnbCfg ARGS((Void));
PRIVATE S16 smSndRmuProtMmeCfg ARGS((U16 mmeId));
PRIVATE S16 smSndRmuEnbStaInd ARGS((LrmEnbStaInd *pstEnbStaInfo));

EXTERN S16 smBuildRmuProtCellCfg(LrmCellConfiguration *cellCfg);
EXTERN S16 smBuildRmuProtEnbCfg(LrmEnbConfiguration *enbCfg);
EXTERN S16 smBuildRmuProtCellDel(LrmCellConfiguration *cellCfg);

/*Global variable*/
U16 gWrProtCnt = 0;

EXTERN S16 smBuildRmuProtMmeCfg(U16 mmeId, LrmEnbConfiguration *enbCfg, U32 action);
#ifdef WR_RSYS_OAM  
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
#endif

PRIVATE U8  wrRmCfg = 0;
//PRIVATE U8  wrRmUsapCfgCnt = 0;

/* MAX Number of Service Providers of SM */
#define MAXRMMI 3 

PRIVATE CONSTANT LrmStaInd SmMiLrmStaIndMt[MAXRMMI] =
{
#ifdef LCSMRMMILRM
   cmPkLrmStaInd,
#else
   PtMiLrmStaInd,
#endif
#ifdef TCSMRMMILRM
#ifdef RMU_DM
   DmMiLrmStaInd,
#else
   RmMiLrmStaInd,
#endif /*RMU_DM*/
#else
   PtMiLrmStaInd,
#endif 
#ifdef LWLCSMRMMILRM 
   cmPkLwLcLrmStaInd,
#else
   PtMiLrmStaInd,
#endif 
};

PRIVATE CONSTANT LrmCfgReq SmMiLrmCfgReqMt[MAXRMMI] =
{
#ifdef LCSMRMMILRM
   cmPkLrmCfgReq,
#else
   PtMiLrmCfgReq,
#endif
#ifdef TCSMRMMILRM
#ifdef RMU_DM
   DmMiLrmCfgReq,
#else
   RmMiLrmCfgReq,
#endif /*RMU_DM*/
#else
   PtMiLrmCfgReq,
#endif 
#ifdef LWLCSMRMMILRM 
   cmPkLwLcLrmCfgReq,
#else
   PtMiLrmCfgReq,
#endif 
};

PRIVATE CONSTANT LrmCntrlReq SmMiLrmCntrlReqMt[MAXRMMI] =
{
#ifdef LCSMRMMILRM
   cmPkLrmCntrlReq,
#else
   PtMiLrmCntrlReq,
#endif
#ifdef TCSMRMMILRM
#ifdef RMU_DM
   DmMiLrmCntrlReq,
#else
   RmMiLrmCntrlReq,
#endif
#else
   PtMiLrmCntrlReq,
#endif
#ifdef LWLCSMRMMILRM 
   cmPkLwLcLrmCntrlReq
#else
   PtMiLrmCntrlReq
#endif /*RMU_LWLC*/
};

PRIVATE CONSTANT LrmEnbStatusInd SmMiLrmEnbStaIndMt[MAXRMMI] =
{
#ifdef LCSMRMMILRM
   cmPkLrmEnbStaInd,
#else
   PtMiLrmEnbStaInd,
#endif
#ifdef TCSMRMMILRM
#ifdef RMU_DM
   DmMiLrmEnbStaInd,
#else
   RmMiLrmEnbStaInd,
#endif /*RMU_DM*/
#else
   PtMiLrmEnbStaInd,
#endif 
#ifdef LWLCSMRMMILRM 
   cmPkLwLcLrmEnbStaInd
#else
   PtMiLrmEnbStaInd
#endif 
};

#ifdef __cplusplus
}
#endif /* cplusplus */

/*
 *
 *       Fun:    smRmInit - configure RM 
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_rrm.c
 *
 */


S16 smRmInit(SSTskId    sysTskId)
{
   /* Register RRM TAPA Task */
   if (SRegTTsk((Ent)ENTRM, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            rmActvInit, (ActvTsk)rmActvTsk) != ROK)
   {
      RLOG0(L_FATAL, "RRM Tapa Task Register failed");
      RETVALUE(RFAILED);
   }

   /* Attach RRM TAPA Task */
   if (SAttachTTsk((Ent)ENTRM, (Inst)0, sysTskId)!= ROK)
   {
      RLOG0(L_FATAL, "RRM Tapa Task Attach failed");
      RETVALUE(RFAILED);
   }
      
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    wrSmRmCfg - configure RM 
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_rrm.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmRmCfg
(
Void
)
#else
PUBLIC Void wrSmRmCfg(Void)
#endif /* ANSI */
{
   U8 idx;

   /*tmp*/
   static U8 FLAG = 0; 
   TRC2(wrSmRmCfg)

      gWrProtCnt = 2;
   if(FLAG == 0)
   {
      gWrProtCnt += smCfgCb.noOfCfg;
   }

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif

   if(FLAG == 0)
   {
      smBuildRmGenCfg();
   }
   smBuildRmRmuUSapCfg(WR_RM_SPID);
   smBuildRmRgmLSapCfg(RM_RG_SUID, RM_RG_SPID);

   smSndRmuProtCellCfg();
   smSndRmuProtEnbCfg();

   /* gWrProtCnt += smCfgCb.noOfCfg;*/
   if(FLAG == 0)
   {
      FLAG = 1;
      for (idx = 0; idx < smCfgCb.noOfCfg; idx++)
      {
         smSndRmuProtMmeCfg((smCfgCb.mmeCfg[idx]->mmeId));
      }
   }
   RETVOID;
} /* end of wrSmRmCfg */

/*
*
*       Fun:   smBindUnBindRmToRgmSap
*
*       Desc:  Invoked to bind and unbind RRM to RGM Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindRmToRgmSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindRmToRgmSap(U8 action)
#endif
{
   RmMngmt  *rmMngmt;
   RmCntrl  *cntrl=NULLP;
   Pst      pst;
   TRC2(smBindUnBindRmToRgmSap)
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

      SM_SET_ZERO(&pst, sizeof(Pst));

      SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));

      cntrl = &(rmMngmt->t.cntrl);

   cntrl->action             =  action;
   cntrl->subAction          =  SM_ZERO_VAL;
   cntrl->u.rmSapCntrl.suId  =  SM_ZERO_VAL;
   cntrl->u.rmSapCntrl.spId  =  SM_ZERO_VAL;


      /* Fill Header */
      rmMngmt->hdr.msgType             = TCNTRL;
      rmMngmt->hdr.entId.ent           = ENTRM;
      rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
      rmMngmt->hdr.elmId.elmnt         = STRGMSAP; 

      rmMngmt->hdr.response.prior      = PRIOR0;
      rmMngmt->hdr.response.route      = RTESPEC;
      rmMngmt->hdr.response.mem.region = smCb.init.region;
      rmMngmt->hdr.response.mem.pool   = RM_POOL;
      rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;



   /* Fill Pst */
   pst.event = EVTLRMCNTRLREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   if (action == ABND)
   {
      RLOG0(L_DEBUG, "Cntrl Req to RRM to bind with MAC");
   }
   else
   {
      RLOG0(L_DEBUG, "Cntrl Req to RRM to unbind with MAC");
   }

   /* Send the request to the RRC */
   SmMiLrmCntrlReq(&pst, rmMngmt);

   RETVALUE(ROK);
} /* end of smBindUnBindRmToRgmSap */



/*
*
*       Fun:   smSndRmuEnbStaInd
*
*       Desc:  Invoked to build RMU status indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
PRIVATE S16 smSndRmuEnbStaInd
(
   LrmEnbStaInd             *pstEnbStaInfo
)
{
   RmMngmt              *rmMngmt;
   RmUsta               *usta;
   Pst                  pst;

   TRC3(smSndRmuEnbStaInd);

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usta = &(rmMngmt->t.usta);

   usta->bStaType = LRM_USTA_STA_ENB; 

   usta->u.stEnbStaInfo = *pstEnbStaInfo;


   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STGEN;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event = EVTLRMENBSTAIND;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG, "USTA Request sent");

   /* Send the request to RM */
   SmMiLrmEnbStaInd(&pst, rmMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSendCpuLoadIndToRrm
*
*       Desc:  Invoked to build RMU status indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
PUBLIC S16 smSendCpuLoadToRrm(U8 cpuLoad)
{
   LrmEnbStaInd     stEnbStaInfo;

   stEnbStaInfo.enStaType = LRM_ENBSTATYPE_CPULOAD; 
   stEnbStaInfo.u.bCpuLoad = cpuLoad; 

   smSndRmuEnbStaInd(&stEnbStaInfo);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSendCellUpIndToRrm
*
*       Desc:  Invoked to build RMU status indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
PUBLIC S16 smSendCellUpIndToRrm(Void)
{
   LrmEnbStaInd     stEnbStaInfo;

   /* Send the Indication for all the cells which are UP */

   stEnbStaInfo.enStaType = LRM_ENBSTATYPE_CELLUP; 
   stEnbStaInfo.u.bCellId = 1; 

//   stEnbStaInfo.u.bCellId = smCb.cfgCb.cellId; 

   smSndRmuEnbStaInd(&stEnbStaInfo);

   RETVALUE(ROK);
}


/*
*
*       Fun:   smSndRmuProtEnbCfg
*
*       Desc:  Invoked to build RMU Protocol CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PRIVATE S16 smSndRmuProtEnbCfg
(
 Void
)
#else
PRIVATE S16 smSndRmuProtEnbCfg(Void)
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmEnbConfiguration  *enbCfg;
   Pst                   pst;

   TRC2(smSndRmuProtEnbCfg);

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_ENB_CONFIG;


   enbCfg = &(protoCfg->u.stEnbCfg);
   smBuildRmuProtEnbCfg(enbCfg);



   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event = EVTLRMCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG, "RM Proto Enb Cfg Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}


/*
*
*       Fun:   smSndRmuProtMmeCfg
*
*       Desc:  Invoked to build RMU Protocol CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PRIVATE S16 smSndRmuProtMmeCfg
(
U16          mmeId
)
#else
PRIVATE S16 smSndRmuProtMmeCfg(mmeId)
U16          mmeId
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmEnbConfiguration  *enbCfg;
   Pst                   pst;

   TRC2(smSndRmuProtMmeCfg);

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_ENB_CONFIG;
   enbCfg = &(protoCfg->u.stEnbCfg);

   smBuildRmuProtMmeCfg(mmeId, enbCfg, LRM_CFG_ADD);


   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event = EVTLRMCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG,"RM Proto Mme Cfg Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndRmuProtCellCfg
*
*       Desc:  Invoked to build RMU Protocol CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PRIVATE S16 smSndRmuProtCellCfg
(
Void
)
#else
PRIVATE S16 smSndRmuProtCellCfg(Void)
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmCellConfiguration *cellCfg;
   Pst                   pst;
   
   TRC2(smSndRmuProtCellCfg);

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_CELL_CONFIG;
   cellCfg = &(protoCfg->u.stCellCfg);

   smBuildRmuProtCellCfg(cellCfg);
  

   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG, "RM Proto Cell Cfg Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}



/*
*
*       Fun:   smBuildRmGenCfg
*
*       Desc:  Invoked to build RM GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PRIVATE S16 smBuildRmGenCfg
(
Void
)
#else
PRIVATE S16 smBuildRmGenCfg(Void)
#endif
{
   RmMngmt       *rmMngmt;
   RmGenCfg      *genCfg=NULLP;
   Pst           pst;

   TRC2(smBuildRmGenCfg);

   rmMngmt = NULLP;

   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(rmMngmt->t.cfg.s.genCfg);
   genCfg->maxRmuSaps = 2;
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_RM_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTRM;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = RM_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LWLC;

   genCfg->mem.region      = RM_REGION;
   genCfg->mem.pool        = RM_POOL;
   genCfg->tmrRes          = 10; /*Timer resolution*/ 


   /* Fill Header */
   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STGEN;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG,"RM Gen Cfg Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smBuildRmRgmLSapCfg
*
*       Desc:  Invoked to build RM RMU SAP Config (RMU).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rmm.c
*
*/
#ifdef ANSI
PRIVATE  S16 smBuildRmRgmLSapCfg
(
SuId suId,
SpId spId
)
#else
PRIVATE  S16 smBuildRmRgmLSapCfg (spId,suId)
SuId suId;
SpId spId;
#endif
{
   
   RmMngmt       *rmMngmt;
   RmLSapCfg     *lsap=NULLP;
   Pst           pst;

   TRC2(smBuildRmRgmLSapCfg)

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lsap   = &(rmMngmt->t.cfg.s.rgmSap);

   lsap->suId        = suId;
   lsap->spId        = spId;

   lsap->procId      = SM_RG_PROC;
   lsap->prior       = PRIOR0;
   lsap->route       = RTESPEC;
   lsap->selector    = RGM_SEL_LC;

   if(spId == WR_RG_SPID)
   {
      lsap->ent         = ENTRG;
   }
   else
   {
  RLOG0(L_ERROR,"Invalid spId");
   }
   lsap->inst        = SCH_INST_ID;
   lsap->mem.region  = smCb.init.region;
   lsap->mem.pool    = RM_POOL;

   /* Fill Header */
   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRGMSAP;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;
  
   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;
   
   /* Fill Pst */
   pst.event = EVTLRMCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcProcId = SM_SM_PROC;
   pst.srcEnt    = ENTSM;
   pst.dstProcId = SM_RM_PROC;
   pst.dstEnt    = ENTRM;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

  RLOG0(L_DEBUG, "RRM rmu Sap Cfg Req sent");

   /* Send the request to the RRM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
} /* end of smBuildRmRmuSapCfg */



/*
*
*       Fun:   smBuildRmRmuUSapCfg
*
*       Desc:  Invoked to build RM RMU SAP Config (RMU).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rmm.c
*
*/
#ifdef ANSI
PRIVATE  S16 smBuildRmRmuUSapCfg
(
SpId spId
)
#else
PRIVATE  S16 smBuildRmRmuUSapCfg (spId)
SpId spId;
#endif
{
   
   RmMngmt       *rmMngmt;
   RmUSapCfg     *usap=NULLP;
   Pst           pst;

   TRC2(smBuildRmRmuUSapCfg)

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usap   = &(rmMngmt->t.cfg.s.rmuSap);

   usap->spId        = spId;
   usap->procId      = SM_RM_PROC;
   usap->prior       = PRIOR0;
   usap->route       = RTESPEC;
   usap->selector    = RMU_SEL_LWLC;
   if(spId == WR_RM_SPID)
   {
      usap->ent         = ENTWR;
   }
   else
   {
  RLOG0(L_ERROR,"Invalid spId");
   }
   usap->inst        = SM_INST_ZERO;
   usap->mem.region  = smCb.init.region;
   usap->mem.pool    = RM_POOL;

   /* Fill Header */
   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMUSAP;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;
  
   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;
   
   /* Fill Pst */
   pst.event = EVTLRMCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

  RLOG0(L_DEBUG,"RRM rmu Sap Cfg Req sent");

   /* Send the request to the RRM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
} /* end of smBuildRmRmuSapCfg */

/* IPSec Recovery */
/*
*
*       Fun:   smBuildRmShutDownCntrl
*
*       Desc:  Invoked to shut down RRM
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRmShutDownCntrl
(
Void
)
#else
PUBLIC S16 smBuildRmShutDownCntrl(Void)
#endif
{
   smSndRmuProtCellDel();
   RETVALUE(ROK);
} /* end of  */

/* IPSec Recovery */

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from RM 
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   wr_sm_rm_exms.c
*
*/

#ifdef ANSI
PUBLIC S16 smRmActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smRmActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   /* wr002.102: Added trace macro */
   TRC2(smRmActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LWLCSMRMMILRM 
      case EVTLRMCFGCFM:             /* Config confirm */
         ret = cmUnpkLwLcLrmCfgCfm(SmMiLrmCfgCfm, pst, mBuf);
         break;
      case EVTLRMCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLwLcLrmCntrlCfm(SmMiLrmCntrlCfm, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         RLOG0(L_ERROR,"Received invalid event");
         break;
   }

   RETVALUE(ret);

} /* end of smRmActvTsk */


/**************************************************************************
 *
 *      Fun:   Enb Status indication 
 *
 *      Desc:  This function is used to send status indication to RRM
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_rm_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrmEnbStaInd
(
Pst * pst,
RmMngmt *usta 
)
#else
PUBLIC S16 SmMiLrmEnbStaInd(pst, usta)
Pst * pst;
RmMngmt *usta;
#endif
{
   S16 ret;

   TRC3(SmMiLrmEnbStaInd);

   ret = (*SmMiLrmEnbStaIndMt[pst->selector])(pst, usta);

   if(pst->selector != LRM_SEL_LWLC)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(RmMngmt));
   }

   RETVALUE(ret);

}


/**************************************************************************
 *
 *      Fun:   Status indication 
 *
 *      Desc:  This function is used to send status indication to RRM
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_rm_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrmStaInd
(
Pst * pst,
RmMngmt *usta 
)
#else
PUBLIC S16 SmMiLrmStaInd(pst, usta)
Pst * pst;
RmMngmt *usta;
#endif
{
   S16 ret;

   TRC3(SmMiLrmStaInd);

   ret = (*SmMiLrmStaIndMt[pst->selector])(pst, usta);

   if(pst->selector != LRM_SEL_LWLC)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(RmMngmt));
   }

   RETVALUE(ret);

}

/**************************************************************************
 *
 *      Fun:   Configuration request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_rm_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrmCfgReq
(
Pst * pst,
RmMngmt * cfg
)
#else
PUBLIC S16 SmMiLrmCfgReq(pst, cfg)
Pst * pst;
RmMngmt * cfg;
#endif
{
   S16 ret;

   TRC3(SmMiLrmCfgReq);

   ret = (*SmMiLrmCfgReqMt[pst->selector])(pst, cfg);

   if(pst->selector != LRM_SEL_LWLC)
   {
      SPutSBuf(pst->region, pst->pool,(Data *)cfg, sizeof(RmMngmt));
   }

   RETVALUE(ret);

}

/**************************************************************************
 *
 *      Fun:   Control Request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  smrmptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrmCntrlReq
(
Pst * pst,
RmMngmt * cfg
)
#else
PUBLIC S16 SmMiLrmCntrlReq(pst, cfg)
Pst * pst;
RmMngmt * cfg;
#endif
{
   S16 ret;

   TRC3(SmMiLrmCntrlReq);

   ret = (*SmMiLrmCntrlReqMt[pst->selector])(pst, cfg);

   if(pst->selector != SM_SELECTOR_LWLC)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(RmMngmt));
   }

   RETVALUE(ret);

}

#ifdef PTSMMILRM
#ifdef ANSI
PRIVATE S16 PtMiLrmCfgReq
(
 Pst *pst,
 RmMngmt *cfg
 )
#else
PRIVATE S16 PtMiLrmCfgReq(pst, cfg)
Pst *pst;
RmMngmt *cfg;
#endif
{
   TRC3(PtMiLrmCfgReq);
   UNUSED(cfg);
   UNUSED(pst);
   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(RmMngmt));
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 PtMiLrmCntrlReq
(
 Pst *pst,
 RmMngmt *cntrl
 )
#else
PRIVATE S16 PtMiLrmCntrlReq(pst, cntrl)
Pst *pst;
RmMngmt *cntrl;
#endif
{
   TRC3(PtMiLrmCntrlReq);
   UNUSED(cntrl);
   UNUSED(pst);
   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RmMngmt));
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 PtMiLrmEnbStaInd
(
  Pst     *pst,
  RmMngmt *enbSta
)
#else
PRIVATE S16 PtMiLrmEnbStaInd(pst, usta)
   Pst     *pst;
   RmMngmt *enbSta;
#endif
{
   TRC3(PtMiLrmEnbStaInd);
   UNUSED(enbSta);
   UNUSED(pst);
   SPutSBuf(pst->region, pst->pool, (Data *)enbSta, sizeof(RmMngmt));
   RETVALUE(ROK);
}



#ifdef ANSI
PRIVATE S16 PtMiLrmStaInd
(
  Pst     *pst,
  RmMngmt *usta
)
#else
PRIVATE S16 PtMiLrmStaInd(pst, usta)
   Pst     *pst;
   RmMngmt *usta;
#endif
{
   TRC3(PtMiLrmStaInd);
   UNUSED(usta);
   UNUSED(pst);
   SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(RmMngmt));
   RETVALUE(ROK);
}

#endif /*PTSMMILRM*/



/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_rm_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLrmCfgCfm
(
Pst            *pst,
RmMngmt        *cfm
)
#else
PUBLIC S16 SmMiLrmCfgCfm(pst, cfm)
Pst            *pst;
RmMngmt        *cfm;
#endif
{
   Pst      rmPst;

   TRC2(SmMiLrmCfgCfm);

   SM_SET_ZERO(&rmPst, sizeof(Pst));

   /*IPSec */
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_S1AP_STK_UNBND_DONE)
      {
         smCb.smState = WR_SM_STATE_RRM_SHUTDWN_DONE;
         RLOG0(L_ALWAYS, "RRM SHUTDOWN SUCCESSFULLY PERFORMED.");
         WR_FILL_PST(rmPst, SM_RM_PROC, ENTSM, ENTWR, EVTWRAPPSHUT, WR_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&rmPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after RRM SHUTDOWN Configuration");
         }
         else
         {
            /* Reset the RRM Configuration State */
            wrRmCfg = 0;
            wrRmCfg |= SM_RM_GEN_CFG;
            RLOG0(L_DEBUG, "Self Pst success after RRM SHUTDOWN Configuration");
         } /* end of else part */
      }
      else
      {
         switch (cfm->hdr.elmId.elmnt)
         {
            case STGEN:
               wrRmCfg |= SM_RM_GEN_CFG;
               break;      
            case STRMUSAP:
               wrRmCfg |= SM_RM_RMU_SAP_CFG;
               break;
            case STRGMSAP:
               wrRmCfg |= SM_RM_RGM_SAP_CFG;
               break;
            case STRMPROTOCFG:  
               gWrProtCnt--;
               if(gWrProtCnt == 0)
               {
                  wrRmCfg |= SM_RM_RMU_PROT_CFG;
               }
               break;
            default:
               RLOG1(L_ERROR,"RRM Cfg Cfm received with invalid element:[%d]"
                     ,cfm->hdr.elmId.elmnt);
               break;
         } /* end of switch statement */

         RLOG1(L_DEBUG,"RRM Cfg Cfm received for the element:[%d]"
               ,cfm->hdr.elmId.elmnt);

#ifdef WR_RSYS_OAM
         if(smCb.smState != WR_SM_STATE_OAM_DYN_START)
#endif
         {
            if (wrRmCfg == SM_RM_CONFIGURED)
            {
               smCb.smState = WR_SM_STATE_RRM_CFG_DONE;
               RLOG0(L_ALWAYS, "RRM CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
               smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
               WR_FILL_PST(rmPst, SM_RM_PROC, ENTSM, ENTWR, EVTWRAPPCFG, WR_POOL, SM_SELECTOR_LC);

               if(wrSmSelfPstMsg(&rmPst) != ROK)
               {
                  RLOG0(L_FATAL, "Self Pst failed after RRM Configuration.");
               }
               else
               {
                  RLOG0(L_DEBUG, "Self Pst success after RRM Configuration.");
               } /* end of else part */

            }  /* end of wrRmCfg == WR_SM_RM_CONFIGURED */
         }
      }
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
      RLOG2(L_ERROR,"Configuration Cfm with NOK Received reaso:[%d]"
            "for element:[%d]", cfm->cfm.reason, cfm->hdr.elmId.elmnt);
#ifdef WR_RSYS_OAM  
      /*timer stop :: Raised an alarm */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
   }

   SPutSBuf(pst->region, pst->pool, (Data*)cfm, sizeof(RmMngmt));
   RETVALUE(ROK);
} /* end of SmMiLrmCfgCfm */


/*
 *
 *       Fun:   Control Confirm
 *
 *       Desc:  This function is used by to present Control confirm
 *              information to Layer Managemer.
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  wr_sm_rm_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLrmCntrlCfm
(
Pst            *pst,
RmMngmt        *cntrl
)
#else
PUBLIC S16 SmMiLrmCntrlCfm(pst, cntrl)
Pst            *pst;
RmMngmt        *cntrl;
#endif
{

   Pst  wrPst;

   TRC2(SmMiLrmCntrlCfm)

   SM_SET_ZERO(&wrPst, sizeof(Pst));

   /* Control confirm for unbinding RRM to MAC */
   if ((cntrl->hdr.elmId.elmnt == STRGMSAP) && 
       (smCb.smState == WR_SM_STATE_DINIT))
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
         RLOG0(L_DEBUG,"UNBIND OF RRM WITH MAC SUCCESSFUL");

         smCb.smState = WR_SM_STATE_RRM_STK_UNBND_DONE;
         WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRRRCSTKUNBND, 
            WR_POOL, SM_SELECTOR_TC);
         if(wrSmSelfPstMsg(&wrPst) != ROK)
         {
            RLOG1(L_ERROR,"Failure at Self Pst.[%d]", smCb.smState);
         }
      }
      SPutSBuf(pst->region, pst->pool, (Data*)cntrl, sizeof(RmMngmt));
      RETVALUE(ROK);
   }

   if (cntrl->hdr.elmId.elmnt == STRGMSAP)
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
         RLOG0(L_DEBUG,"BIND OF RRM WITH MAC SUCCESSFUL");

         if (smCb.smState == WR_SM_STATE_RRC_STK_BND_DONE)
         {
            smCb.smState = WR_SM_STATE_RRM_STK_BND_DONE;
         }

         /* Perform the bind of APP and SCTP */
         WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRINITS1SETUP, WR_POOL, SM_SELECTOR_TC);
         if(wrSmSelfPstMsg(&wrPst) != ROK)
         {
            RLOG1(L_FATAL,"Failure at Self Pst.[%d]", smCb.smState);
         }


      } /* end of if statement */
      else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
      {
         RLOG0(L_INFO,"BIND OF RRM WITH MAC IS IN PROGRESS");
      }
      else 
      {
         RLOG0(L_ERROR,"BIND OF RRM WITH MAC FAILED");
      }
   } /* end of if statement */
   else
   {
      RLOG1(L_ERROR,"INVALID BIND CFM RECEIVED for element:[%d]",
         cntrl->hdr.elmId.elmnt);
   }

   SPutSBuf(pst->region, pst->pool, (Data*)cntrl, sizeof(RmMngmt));

   RETVALUE(ROK);

} /* end of SmMiLrmCntrlCfm */

/*
*
*       Fun:   smSndRmuProtCellDel
*
*       Desc:  Invoked to build RMU Protocol CELL DEL.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PRIVATE S16 smSndRmuProtCellDel
(
Void
)
#else
PRIVATE S16 smSndRmuProtCellDel(Void)
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmCellConfiguration *cellCfg;
   Pst                   pst;
   
   TRC2(smSndRmuProtCellDel);

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_CELL_CONFIG;
   cellCfg = &(protoCfg->u.stCellCfg);

   /*smBuildRmuProtCellCfg(cellCfg);*/
     smBuildRmuProtCellDel(cellCfg);

   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG,"RM Proto Cell DEL Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndRmuProtDynMmeCfg
*
*       Desc:  Invoked to build RMU Protocol CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PUBLIC S16 smSndRmuProtDynMmeCfg
(
U16          mmeIdx, U32 action
)
#else
PUBLIC S16 smSndRmuProtDynMmeCfg(mmeIdx, action)
U16          mmeIdx;
U32          action
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmEnbConfiguration  *enbCfg;
   Pst                   pst;

   TRC2(smSndRmuProtMmeCfg);

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_ENB_CONFIG;
   enbCfg = &(protoCfg->u.stEnbCfg);

   smBuildRmuProtMmeCfg(smCfgCb.mmeCfg[mmeIdx]->mmeId, enbCfg, action);


   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event = EVTLRMCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_DEBUG,"RM Proto Mme Cfg Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}/*end of smSndRmuProtDynMmeCfg*/

/*
*
*       Fun:  smFillLrmCellConfig   
*
*       Desc:  Invoked to build RMU ModCell CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PRIVATE Void smFillLrmCellConfig 
(
 LrmCellConfiguration *cellCfg,
 LrmCellConfiguration *modCellCfg
)
#else
PRIVATE Void smFillLrmCellConfig(cellCfg,modCellCfg)
LrmCellConfiguration *cellCfg;
LrmCellConfiguration *modCellCfg;
#endif
{
   U32   reCfgType;
   U32   mask = 1;

   reCfgType = modCellCfg->usReCfgType;
   cellCfg->usReCfgType = modCellCfg->usReCfgType;

   TRC2(smFillLrmCellConfig);

   while(reCfgType)
   {
      if(reCfgType & mask)
      {
         switch(mask)
         {
            case LRM_CELL_RECFGTYPE_ACB_MODATA: 
            {
               reCfgType ^= mask;
               memcpy(&cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData,
                     &modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData,
                     sizeof(CmRrmAcb));
               cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoData = 
                  modCellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoData;
            }
            break;
            case LRM_CELL_RECFGTYPE_ACB_MOSIG:
            {
               reCfgType ^= mask;

               cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoSig = modCellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoSig;
               memcpy(&cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig,
                     &modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig,
                     sizeof(CmRrmAcb));
            }
            break;
            case LRM_CELL_RECFGTYPE_ACB_EMERGENCY:
            {
               reCfgType ^= mask;
               cellCfg->stCellCfgInfo.stAcbInfo.bAcBarringEmergency=
                  modCellCfg->stCellCfgInfo.stAcbInfo.bAcBarringEmergency;
            }
            break;
            case LRM_CELL_RECFGTYPE_CSG_PARAM:
            {
               reCfgType ^= mask;

               cellCfg->stCellCfgInfo.usMaxNonCsgUes = 
                  modCellCfg->stCellCfgInfo.usMaxNonCsgUes;

               cellCfg->stCellCfgInfo.usMaxCsgUes = 
                  modCellCfg->stCellCfgInfo.usMaxCsgUes;
            }
            break;

            case LRM_CELL_RECFGTYPE_TRANS_MODE:
            {
               reCfgType ^= mask;

               cellCfg->stCellCfgInfo.flagTransMode = 
                  modCellCfg->stCellCfgInfo.flagTransMode;
            }
            break;

            default:
            {
               RLOG1(L_INFO,"Invalid cfgType to re-configure RRM [%d]\n", mask);
            }
            break;

         }/*End of switch*/

      }/*End of if*/

      mask = mask << 1;

   }/*End of while*/
}/*End of smFillLrmCellConfig*/

/*
*
*       Fun:   smSndRmuProtModCellCfg
*
*       Desc:  Invoked to build RMU ModCell CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PUBLIC S16 smSndRmuProtModCellCfg
(
U8    cellId,
LrmCellConfiguration modCellCfg
)
#else
PUBLIC S16 smSndRmuProtModCellCfg(cellId, modCellCfg)
U8    cellId;
LrmCellConfiguration modCellCfg
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmCellConfiguration *cellCfg;
   Pst                   pst;
   
   TRC2(smSndRmuProtModCellCfg);
   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
     RLOG0(L_FATAL,"RM Cell Cfg allocation failure");
     RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_CELL_CONFIG;
   cellCfg = &(protoCfg->u.stCellCfg);
   cellCfg->enCellAddModDel = LRM_CFG_MOD;
   cellCfg->stCellCfgInfo.bCellId = cellId;

   smFillLrmCellConfig(cellCfg,&modCellCfg);
   
   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_EVENT, "RM Proto Cell ReCfg Req sent");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndCliRmuProtModCellCfg
*
*       Desc:  Invoked from CLI to build RMU ModCell CFG
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrm.c
*
*/
#ifdef ANSI
PUBLIC S16 smSndCliRmuProtModCellCfg
(
U8    cellId,
U16   maxCsgUEs,
U16   maxNonCsgUEs
)
#else
PUBLIC S16 smSndCliRmuProtModCellCfg(cellId, maxCsgUEs, maxNonCsgUEs)
U8    cellId;
U16   maxCsgUEs;
U16   maxNonCsgUEs;
#endif
{
   RmMngmt              *rmMngmt;
   RmProtoCfg           *protoCfg;
   LrmCellConfiguration *cellCfg;
   Pst                   pst;
   
   TRC2(smSndCliRmuProtModCellCfg);

   if((maxCsgUEs > smCfgCb.maxUeSupp) || 
      (maxNonCsgUEs > smCfgCb.maxUeSupp) ||
      ((maxNonCsgUEs == maxCsgUEs) && (maxCsgUEs < smCfgCb.maxUeSupp) && 
      ((maxCsgUEs + maxNonCsgUEs) != smCfgCb.maxUeSupp)) ||
      (maxCsgUEs + maxNonCsgUEs) < smCfgCb.maxUeSupp)
   {
      RLOG3(L_DEBUG, "Invalid CLI Option: Non CSG UE CFGD [%d] CSG UE CFGD "
            "[%d] Max UE Supported [%d]", maxNonCsgUEs,
            maxCsgUEs, smCfgCb.maxUeSupp);
       RETVALUE(ROK);
   }

   rmMngmt = NULLP;
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, RM_POOL, (Data**)&rmMngmt, sizeof(RmMngmt));
   if(NULLP == rmMngmt)
   {
     RLOG0(L_FATAL,"RM Cell Cfg allocation failure");
     RETVALUE(RFAILED);
   }

   SM_SET_ZERO(rmMngmt, sizeof(RmMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protoCfg = &(rmMngmt->t.protoCfg);

   protoCfg->bCfgType = LRM_CELL_CONFIG;
   cellCfg = &(protoCfg->u.stCellCfg);
   cellCfg->enCellAddModDel = LRM_CFG_MOD;
   cellCfg->usReCfgType = LRM_CELL_RECFGTYPE_CSG_PARAM;
   cellCfg->stCellCfgInfo.usMaxNonCsgUes   = maxNonCsgUEs;
   cellCfg->stCellCfgInfo.usMaxCsgUes      = maxCsgUEs;
   cellCfg->stCellCfgInfo.bCellId        = cellId;

   rmMngmt->hdr.msgType             = TCFG;
   rmMngmt->hdr.msgLen              = 0;
   rmMngmt->hdr.entId.ent           = ENTRM;
   rmMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rmMngmt->hdr.elmId.elmnt         = STRMPROTOCFG;
   rmMngmt->hdr.seqNmb              = 0;
   rmMngmt->hdr.version             = 0;
   rmMngmt->hdr.transId             = 0;

   rmMngmt->hdr.response.prior      = PRIOR0;
   rmMngmt->hdr.response.route      = RTESPEC;
   rmMngmt->hdr.response.mem.region = smCb.init.region;
   rmMngmt->hdr.response.mem.pool   = RM_POOL;
   rmMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRM;
   pst.dstProcId = SM_RM_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = RM_POOL;

   RLOG0(L_EVENT, "CLI configuration sent to RRM");

   /* Send the request to RM */
   SmMiLrmCfgReq(&pst, rmMngmt);

   RETVALUE(ROK);
}

#ifdef __cplusplus
}
#endif /* cplusplus */

/********************************************************************30**
  
         End of file:     fc_smm_rrm.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:52:28 2014
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
*********************************************************************91*/
