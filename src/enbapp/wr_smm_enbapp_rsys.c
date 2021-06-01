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
               

     File:     wr_smm_enbapp_rsys.c

     Sid:      fc_smm_enbapp_rsys_brcm.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:52:10 2014

     Prg:      Sriky 

*********************************************************************21*/
#ifdef RM_INTF
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_FILE_ID=116;


static int RLOG_MODULE_ID=2;

#include "rmu.h" /*RRM->FSM */
#endif /*RM_INTF*/

#include "wr.h"
#include "wr_emm.h"
#include "wr_lwr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_smm_init.h"
#include "wr_utils.h"
#include "wr_emm_mme.h"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
#endif
#include "lys.h"
#include "lys.x"
#ifdef LWLCSMMILWR
#define  MAXWRMI  3
#if (!defined(LCSMMILWR) || !defined(WR) ||!defined(LWLCSMMILWR))
#define PTSMMILWR
#endif
#else
#define  MAXWRMI  2
#if (!defined(LCSMMILWR) || !defined(WR))
#define PTSMMILWR
#endif
#endif

PUBLIC S16 SmMiLwrAddNeighCfm(Pst *spst,LwrMngmt *cfg);
PUBLIC S16 SmMiLwrCntrlCfm(Pst *pst,LwrMngmt *cntrl);
PUBLIC S16 SmMiLwrAddNeighCfgReq(Pst *pst,LwrMngmt *cntrl);
PUBLIC S16 smBuildCdmaHrpdaBandClass( LwrMngmt *lwrMng );
PUBLIC S16 smBuildCdma1xRTTBandClass(LwrMngmt *lwrMng);
#ifdef WR_RSYS_OAM
extern PUBLIC S16 smHdlNeighEutraDelCellWithFreq(LwrNeighFreqCfg *delNghEutraFreq);
extern PUBLIC Void msmDynConfigComplete(MsmCfmType msmcfmsuccess,MsmCauseType msmcausena, MsmDynCfmInfo dynCfmInfo);
extern PUBLIC S16 smHdlNeighEnbCfgDel(Void);
#endif
PUBLIC S16 smBuildWrNr1xCdmaFreqCfg
(
 LwrMngmt *lwrMng
);
PUBLIC S16 smBuildWrNr1xCdmaCellCfg
(
 LwrMngmt *lwrMng
);
#ifdef PTSMMILWR
PRIVATE S16 PtMiLwrCfgReq    (Pst *pst, LwrMngmt *cfg);
PRIVATE S16 PtMiLwrCntrlReq  (Pst *pst, LwrMngmt *cntrl);
PRIVATE S16 PtMiLwrLoadInd (Pst *spst, LwrLoadIndInfo *loadIndInfo);
PRIVATE S16 PtMiLwrDynCfgReq(Pst *pst,LwrDynMngmt *dynCfg);
PRIVATE S16 PtMiLwrAddNeighCfm(Pst *spst,LwrMngmt *cfg);
#endif
//PRIVATE S16 smSndWrNrCdmaHrpdFreqCfg(Void);
PRIVATE S16 smSndWrNr1xCdmaFreqCfg(Void);
PRIVATE S16 smSndWrNr1xCdmaCellCfg(Void);
PRIVATE S16 smSndWrNrUtraFddFreqCfg(Void);
PRIVATE S16 smSndWrNrUtraTddFreqCfg(Void);
PRIVATE S16 smSndWrNrEutraFreqCfg(Void);
#ifndef LTE_PAL_ENB
PRIVATE S16 smBuildWrstopPHYCntrl(Void); /* IPSEC*/
#endif
#ifdef WR_RSYS_OAM
EXTERN Void FapAlarmInd(U32 alarmId, U8 severity, U8 causeType, U8 causeVal);
EXTERN MsmLteeNodeBparams lteeNodeBparams;
EXTERN Void MsmOperationalState(U32 opstate);
#endif

#if defined (WR_RSYS_OAM) || (WR_DBG_CIRLOG) 
EXTERN S16 smRegInitTmr ARGS((Void));
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
EXTERN Void smDynCfgFailAlarm ARGS((U32 event));
U32 smDynTmrEvent = 0;
#endif

#ifdef WR_TEST_CODE
EXTERN Void smWrProcCliMsg(Void);
#endif
PRIVATE S16 smBuildWrNluLSapCfg (SuId  suId, SpId  spId);

PRIVATE U32 wrWrCfg = 0;
PRIVATE U8 wrNumPjuSaps = 1;
/* extern declarations */
EXTERN S16 SReInitTmr(void);
/*
*
*       Fun:   Initialize RRC App task.
*
*       Desc:  Invoked to create RRC App TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smWrInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smWrInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smWrInit)

   /* Register ENB TAPA Task */
   if (SRegTTsk((Ent)ENTWR, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR2,
                 wrActvInit, (ActvTsk)wrActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Removed registering of RRM task into WR entity, as new st=ystem task is
    * spawned for new RRM*/

   /* Register RRC APP TAPA Task */
   if (SRegTTsk((Ent)ENTNX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR2,
                 NULLP , (ActvTsk)wrActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }


   /* Attach EGTP AND CL USER TAPA Task */
   /* Mapping of VE to WR as EGTP and CL still send to VE */
   if (SRegTTsk((Ent)ENTVE, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR2,
                 wrActvInit, (ActvTsk)wrActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach ENB TAPA Task */
   if (SAttachTTsk((Ent)ENTWR, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRM TAPA Task */
   if (SAttachTTsk((Ent)ENTWR, (Inst)RRM_INST_ID, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */


   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTNX, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }



   /* Attach EGTP AND CL USER TAPA Task */
   /* Mapping of VE to WR as EGTP and CL still send to VE */
   if (SAttachTTsk((Ent)ENTVE, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG, "RRC App Tapa Task successfully registered and attached to %d",
      sysTskId);

   RETVALUE(ROK);
} /* end of smWrInit */

#ifdef EU_DAT_APP

/*
*
*       Fun:   Initialize Data App task.
*
*       Desc:  Invoked to create Data App TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smEuInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smEuInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smEuInit)
    /* Register Data APP TAPA Task */
   if (SRegTTsk((Ent)ENTEU, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR1,
                 wrDamActvInit , (ActvTsk)wrDamActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

    /* Register Data APP TAPA Task */
   if (SRegTTsk((Ent)ENTPX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR1,
                 NULLP , (ActvTsk)wrDamActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTEU, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTPX, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of smEuInit */

#endif /* EU_DAT_APP */

/*
*
*       Fun:   smSndWrCdmaHrpdBandClass
*
*       Desc:  This function for sending eNodeB Cell band class 
*               Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/

/*
*
*       Fun:   smSndWrCdma1xRTTBandClass
*
*       Desc:  This function for sending eNodeB Cell band class 
*               Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PRIVATE S16 smSndWrCdma1xRTTBandClass
(
void
)
#else
PRIVATE S16 smSndWrCdma1xRTTBandClass
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   if (NULLP == lwrMng)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   SM_SET_ZERO(&pst, sizeof(Pst));
   
   smBuildCdma1xRTTBandClass(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.transId             = LWR_TRANSID_CDMA1XRTT_BAND;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndWrCdma1xRTTBandClass */

/*
*
*       Fun:   smSndWrEnbCfg
*
*       Desc:  This function for sending eNode Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/

#ifdef ANSI
PRIVATE S16 smSndWrEnbCfg
(
void
)
#else
PRIVATE S16 smSndWrEnbCfg()
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, " Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrEnbCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.transId             = LWR_TRANSID_ENBCFG;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndWrCellCfg
*
*       Desc:  This function for sending eNodeB Cell Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PRIVATE S16 smSndWrCellCfg
(
void
)
#else

PRIVATE S16 smSndWrCellCfg()
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   if(RFAILED == smBuildCellCfg(lwrMng))
   {
       RETVALUE(RFAILED);
   } 
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_CELLCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}



/*      Fun:   smFreeCellCfgMem
 *
 *      Desc:  This function to free allocated memory for CellCfg  after geting confirmation
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File: wr_smm_enbapp_rsys.c
 *
 */

#ifdef ANSI
PRIVATE Void smFreeCellCfgMem
(
 LwrAddCellCfg *cell,
 U32 noOfcells
)
#else
U32 idx;
PRIVATE Void smFreeCellCfgMem()
 LwrAddCellCfg *cell,
 U32 noOfcells;
#endif
{
   U32 freingIdx;
   for(freingIdx= 0 ;freingIdx < noOfcells;freingIdx++)
   {
      WR_FREE((cell->cellCfgGrp[freingIdx]),(sizeof(LwrCellCfgGrp)));
   }
}
/*
*
*       Fun:   smSndWrInterFreqNhCellCfg
*
*       Desc:  This function for sending neighbour Cell Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/

/*
*
*       Fun:   smSndWrModCellCfg
*
*       Desc:  This function used to modify cell cfg.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrModCellCfg
(
void
)
#else
PUBLIC S16 smSndWrModCellCfg()
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildModCellCfg(lwrMng);

   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.transId              = LWR_TRANSID_CELL_MOD;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   /* filling the proper selector value */
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}


/*
*
*       Fun:   smSndWrNhUtraFddCellCfg
*
*       Desc:  This function for sending Utra FDD neighbour Cell Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PRIVATE S16 smSndWrNhUtraFddCellCfg
(
void
)
#else
PRIVATE S16 smSndWrNhUtraFddCellCfg()
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;  /* this is ownCellID in case of PLD based conf, jst a logic entity */
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = wrSmDfltNumofUtraFddNrCell;

   smBuildWrNehUtraFddCellCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NHUTRAN_CELLCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}




/*
*
*       Fun:   smSndWrNhUtraTddCellCfg
*
*       Desc:  This function for sending Utra Tdd neighbour Cell Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PRIVATE S16 smSndWrNhUtraTddCellCfg
(
void
)
#else
PRIVATE S16 smSndWrNhUtraTddCellCfg()
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;  /* this is ownCellID in case of PLD based conf, jst a logic entity */
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = wrSmDfltNumofUtraTddNrCell;

   smBuildWrNehUtraTddCellCfg(lwrMng);
   
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NHUTRAN_CELLCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}


/*
*
*       Fun:   smSndWrNhCellCfg
*
*       Desc:  This function for sending neighbour Cell Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/

#ifdef ANSI
PRIVATE S16 smSndWrNhCellCfg
(
void
)
#else
PRIVATE S16 smSndWrNhCellCfg()
#endif
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrNehCellCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NHCELLCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndWrNhCfg
*
*       Desc:  This function for sending Neighbour eNodeB Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
PRIVATE S16 smSndWrNhCfg
(
void
)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrNghCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NHCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndWrMmeCfg
*
*       Desc:  This function for sending MME Configuration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
PRIVATE S16 smSndWrMmeCfg
(
LwrSmMmeCfg *mmeCfg
)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrMmeCfg(lwrMng, mmeCfg);
   
   lwrMng->hdr.msgType             = TCFG;

   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.transId             = LWR_TRANSID_MME;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smSndWrNrCdmaHrpdFreqCfg - configure Neighbour 1xCdma frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */



/*
 *
 *       Fun:    smSndWrNr1xCdmaFreqCfg - configure Neighbour 1xCdma frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */
PRIVATE S16 smSndWrNr1xCdmaFreqCfg(Void)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   if (NULLP == lwrMng)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   SM_SET_ZERO(&pst, sizeof(Pst));
   
   smBuildWrNr1xCdmaFreqCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_CDMA1XRTT_FREQ;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}
 
/*
 *
 *       Fun:    smSndWrNr1xCdmaFreqCfg - configure Neighbour 1xCdma frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */

PRIVATE S16 smSndWrNr1xCdmaCellCfg()
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   if (NULLP == lwrMng)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   SM_SET_ZERO(&pst, sizeof(Pst));
   
   smBuildWrNr1xCdmaCellCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.transId            = LWR_TRANSID_CDMA1XRTT_CELL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smSndWrUtraFddNrFreqCfg - configure Neighbour Utra FDD frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */
PRIVATE S16 smSndWrNrUtraFddFreqCfg(Void)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of
                                           PLD based conf, jst a logic entity */
   /* lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = wrSmDfltNumofUtraNrFreq;*/
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = (U8)smCfgCb.noOfUtraFddFreq;

   smBuildWrNrUtraFddFreqCfg(lwrMng);

   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NRUTRAN;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smSndWrUtraTddNrFreqCfg - configure Neighbour Utra TDD frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */
PRIVATE S16 smSndWrNrUtraTddFreqCfg(Void)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of
                                           PLD based conf, jst a logic entity */
   /* lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = wrSmDfltNumofUtraNrFreq;*/
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = (U8)smCfgCb.noOfUtraTddFreq;

   smBuildWrNrUtraTddFreqCfg(lwrMng);

   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NRUTRAN;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smSndWrNrGeranFreqCfg
 *
 *       Desc:   GERAN frequency configuration filling function
 *
 *       Ret:    S16
 *
 *       Notes:  None
 *
 *       File:   wr_smm_enbapp_rsys.c
 *
 */ 
PRIVATE S16 smSndWrNrGeranFreqCfg(Void)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrNrGeranFreqCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_GERAN_FREQ;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}/* End of smSndWrNrGeranFreqCfg */


/*
 *
 *       Fun:    smSndWrNrGeranCellCfg
 *
 *       Desc:   GERAN cell configuration filling function
 *
 *       Ret:    S16
 *
 *       Notes:  None
 *
 *       File:   wr_smm_enbapp_rsys.c
 *
 */
PRIVATE S16 smSndWrNrGeranCellCfg(Void)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrNrGeranCellCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_GERAN_CELL;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}/* End of smSndWrNrGeranCellCfg */


/*
 *
 *       Fun:    smSndWrEutraNrFreqCfg - configure Neighbour Eutra frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */
PRIVATE S16 smSndWrNrEutraFreqCfg(Void)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   smBuildWrNrEutraFreqCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_NREUTRAN;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}
/*
 *
 *       Fun:    wrSmWrCfg - configure WR 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_sz_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmWrCfg
(
Void
)
#else
PUBLIC Void wrSmWrCfg(Void)
#endif /* ANSI */
{
   
   TRC2(wrSmWrCfg)
   wrWrCfg = 0;  

#ifdef WR_DBG_CIRLOG
   if (smRegInitTmr() != ROK)
   {
      RLOG0(L_ERROR, "smRegInitTmr Failed");
      RETVOID;
   }

   cmInitTimers(&(smCb.cfgCb.logTimer), 1);
   smStartTmr((PTR)&(smCb),SM_TMR_LOG_TMR, SM_TMR_LOG_TMR_VAL);
#endif
   /* To handle FAP ADMIN DOWN-UP scenario */
   wrNumPjuSaps = 1;

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif

   smBuildWrGenCfg();
   
   RETVOID;
} /* end of wrSmWrCfg */



/*
*
*       Fun:   wrSmmSendDynNbrEnbCfg
*
*       Desc:  fill in neighbour Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 wrSmmSendDynNbrEnbCfg
(
U32    dbIndex
)
#else
PUBLIC S16 wrSmmSendDynNbrEnbCfg(dbIndex)
U32    dbIndex;
#endif
{
   LwrMngmt lwrMng;
   Pst pst;


   SM_SET_ZERO(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   smBuildDynWrNbrEnbCfg(dbIndex,&lwrMng);

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_TC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

/*   smCb.smState = WR_SM_STATE_DYN_X2AP_CFG_DONE;*/
   (Void) SmMiLwrAddNeighCfm(&pst, &lwrMng);
  
   RETVALUE(ROK);

}/*wrSmmSendDynNbrEnbCfg */



/*
*
*       Fun:   smBuildWrGenCfg
*
*       Desc:  fill in genCfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrGenCfg
(
Void
)
#else
PUBLIC S16 smBuildWrGenCfg(Void)
#endif
{
   LwrMngmt  *wrMgt = NULLP;
   LwrGenCfg    *cfg = NULLP;
   Pst       pst;
   
   TRC2(smBuildWrGenCfg)
   
   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cfg   = &(wrMgt->t.cfg.t.genCfg);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }

   smBuildGenCfg(wrMgt);

   cfg->lmPst.srcProcId = SM_SM_PROC;
   cfg->lmPst.dstProcId = SM_WR_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTWR;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)WRSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;

   cfg->lmPst.srcProcId = SM_SM_PROC;
   cfg->lmPst.dstProcId = SM_WR_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTWR;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)WRSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = WR_MEM_REGION;
   cfg->lmPst.pool   =  WR_POOL;
   cfg->lmPst.selector = SM_SELECTOR_LWLC;
   cfg->timeRes = 10;
  
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRGEN;
   wrMgt->hdr.elmId.elmntInst1    = 0;
   wrMgt->hdr.elmId.elmntInst2    = 0;
   wrMgt->hdr.elmId.elmntInst3    = 0;
   wrMgt->hdr.seqNmb              = 0;
   wrMgt->hdr.version             = 0;
   wrMgt->hdr.transId             = LWR_TRANSID_GEN;

   wrMgt->hdr.response.prior      = PRIOR0;
   wrMgt->hdr.response.route      = RTESPEC;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "APP Gen Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);
 
   RETVALUE(ROK);
}/* smBuildWrGenCfg */

/*
*
*       Fun:   smBuildWrCtfLSapCfg
*
*       Desc:  Call handler for CTF LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrCtfLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrCtfLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildWrCtfLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_YS_PROC;
#ifdef LTE_ENB_PAL
   cfg->dstEnt       = ENTTF;
#else   
   cfg->dstEnt       = ENTYS;
   cfg->dstEnt       = ENTTF;
#endif /* LTE_ENB_PAL */   
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WRYSSEL;
   cfg->srcEnt       = ENTWR;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRCTFSAP;
   wrMgt->hdr.elmId.elmntInst1    = 0;
   wrMgt->hdr.elmId.elmntInst2    = 0;
   wrMgt->hdr.elmId.elmntInst3    = 0;
   wrMgt->hdr.seqNmb              = 0;
   wrMgt->hdr.version             = 0; 
   wrMgt->hdr.transId             = LWR_TRANSID_CTF;

   wrMgt->hdr.response.prior      = PRIOR0;
   wrMgt->hdr.response.route      = RTESPEC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Ctf LSap Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);

   RETVALUE(ROK);
}/* smBuildWrCtfLSapCfg */

/*
*
*       Fun:   smBuildWrNhuLSapCfg
*
*       Desc:  Call handler for NHU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrNhuLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrNhuLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst pst;
   
   TRC2(smBuildWrNhuLSapCfg)
   
   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_NH_PROC;
   cfg->dstEnt       = ENTNH;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WRNHSEL;
   cfg->srcEnt       = ENTNX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRNHUSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_NHU;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;
   
    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "EnbApp Nhu LSap Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);
   
   RETVALUE(ROK);
}/* smBuildWrNhuLSapCfg */

/*
*
*       Fun:   smBuildWrNluLSapCfg
*
*       Desc:  Call handler for NLU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_enbapp.c
*
*/
#ifdef ANSI
PRIVATE S16 smBuildWrNluLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PRIVATE S16 smBuildWrNluLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;

   TRC2(smBuildWrNluLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   if(NULLP == wrMgt)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_NL_PROC;
   cfg->dstEnt       = ENTNL;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = NLU_SEL_LWLC;
   cfg->srcEnt       = ENTWR;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRNLUSAP;
   wrMgt->hdr.transId            = LWR_TRANSID_NLU;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "ENBAPP to SON LSAP Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);

   RETVALUE(ROK);
}/* smBuildWrNluLSapCfg */

#ifdef RM_INTF
/*
*
*       Fun:   smBuildWrRmuLSapCfg
*
*       Desc:  Call handler for RMU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrRmuLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrRmuLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildWrRmuLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if (NULLP == wrMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);
   if (NULLP == cfg)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/


   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_RM_PROC;
   /* cfg->dstEnt       = ENTKW; */
   cfg->dstEnt       = ENTRM;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = RMU_SEL_LWLC;
   cfg->srcEnt       = ENTWR;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRRMUSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_RMU;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "FSM to RRM LSAP Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);
 
   RETVALUE(ROK);
}/* smBuildWrRmuLSapCfg */
#endif /*RM_INTF*/

#ifdef EU_DAT_APP

/*
*
*       Fun:   smBuildWrPjuLSapCfg
*
*       Desc:  Call handler for PJU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrPjuLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrPjuLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;

   TRC2(smBuildWrPjuLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_PJ_PROC;
   /* Shwetha - adding PDCP's entity Id instead of RLC's for data application's
      lower SAP */
   /* cfg->dstEnt       = ENTKW; */
   cfg->dstEnt       = ENTPJ;
   cfg->dstInst      = (suId == WR_SM_PJUL_INST) ? WR_SM_PJDL_INST : WR_SM_PJUL_INST;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WRPJSEL;
   cfg->srcEnt       = ENTPX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_DATAPP_MEM_REGION;
   cfg->mem.pool     = WR_POOL;
   
   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRPJUSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_PJU;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRM Nhu LSap Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);

   RETVALUE(ROK);
}/* smBuildWrPjuLSapCfg */

#endif  /* EU_DAT_APP */

/*
*
*       Fun:   smBuildWrRgrLSapCfg
*
*       Desc:  Call handler for RGR LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrRgrLSapCfg
(

SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrRgrLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;
   
   TRC2(smBuildWrRgrLSapCfg)
   
   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;
                                                          
   cfg->dstProcId    = SM_RG_PROC;
   cfg->dstEnt       = ENTRG;
   cfg->dstInst      = (Inst)SCH_INST_ID;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WRRGSEL;
   cfg->srcEnt       = ENTNX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRRGRSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_RGR;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Rgr LSap Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);
                                                      
   RETVALUE(ROK);
}/* smBuildWrRgrLSapCfg */
                                                        
                                                       
/*
*
*       Fun:   smBuildWrSztLSapCfg
*
*       Desc:  Call handler for SZT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrSztLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrSztLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;

   TRC2(smBuildWrSztLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_SZ_PROC;
   cfg->dstEnt       = ENTSZ;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WRSZSEL;
   cfg->srcEnt       = ENTWR;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;
   
   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRSZTSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_SZT;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Szt LSap Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);

   RETVALUE(ROK);
}/* smBuildWrSztLSapCfg */

/*
*
*       Fun:   smBuildWrCztLSapCfg
*
*       Desc:  Call handler for CZT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrCztLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrCztLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildWrCztLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_CZ_PROC;
   cfg->dstEnt       = ENTCZ;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WRCZSEL;
   cfg->srcEnt       = ENTWR;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWRCZTSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_CZT;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Czt LSap Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);
   
   RETVALUE(ROK);
}/* smBuildWrCztLSapCfg */

#ifdef EU_DAT_APP

/*
*
*       Fun:   smBuildWrEgtLSapCfg
*
*       Desc:  Call handler for EGT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrEgtLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildWrEgtLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   LwrMngmt    *wrMgt = NULLP;
   LwrLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildWrEgtLSapCfg)

   WR_ALLOC(&wrMgt, sizeof(LwrMngmt));
   if(wrMgt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
  
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(wrMgt->t.cfg.t.lSap);

   if(cfg == NULLP)
   {
      WR_FREE(wrMgt, sizeof(LwrMngmt));
      RETVALUE(RFAILED);
   }
   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_EG_PROC;
   cfg->dstEnt       = ENTEG;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = WRSM_WREGSEL;
   cfg->srcEnt       = ENTVE;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = WR_DATAPP_MEM_REGION;
   cfg->mem.pool     = WR_POOL;

   cfg->maxBndRetry = WRSM_LWR_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = WRSM_LWR_VAL_10;

   if(wrMgt == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   wrMgt->hdr.msgType             = TCFG;
   wrMgt->hdr.msgLen              = 0;
   wrMgt->hdr.entId.ent           = ENTWR;
   wrMgt->hdr.entId.inst          = SM_INST_ZERO;
   wrMgt->hdr.elmId.elmnt         = STWREGTSAP;
   wrMgt->hdr.transId             = LWR_TRANSID_EGT;
   wrMgt->hdr.response.selector   = SM_SELECTOR_LWLC;
   wrMgt->hdr.response.mem.region = WR_MEM_REGION;
   wrMgt->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Egt LSap Cfg sent");
   /* Send the request to the LM */
   (Void) SmMiLwrCfgReq(&pst, wrMgt);
 
   RETVALUE(ROK);
}/* smBuildWrEgtLSapCfg */

#endif  /* EU_DAT_APP  */


/*
 *      FUN:   smBindUnBindWrToSztSap
*
 *      Desc:  Bind and Unbind WR with SCT LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToSztSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToSztSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst        pst;
   S16      ret = ROK;
 
   TRC2(smBindUnBindWrToSztSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
 
   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_SZ_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRSZTSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for S1AP");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for S1AP");
   }

    /* Send the control request to the APP */
   (Void)SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ret);
} /* smBindUnBindWrToSztSap */

#ifdef LTE_HO_SUPPORT
/*
 *      FUN:   smBindUnBindWrToCztSap
*
 *      Desc:  Bind and WR with CZT LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToCztSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToCztSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst        pst;
   S16      ret = ROK;
 
   TRC2(smBindUnBindWrToCztSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_CZ_SUID;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRCZTSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for X2AP");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for X2AP");
   }

    /* Send the control request to the APP */
   (Void)SmMiLwrCntrlReq(&pst, cntrl);
 
   RETVALUE(ret);
} /* smBindUnBindWrToCztSap */
#endif

#ifdef EU_DAT_APP

/*
 *      FUN:   smBindUnBindWrToEgtSap
*
 *      Desc:  Bind and Unbind WR with EGT LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToEgtSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToEgtSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst        pst;
   S16      ret = ROK;
 
   TRC2(smBindUnBindWrToEgtSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_EG_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWREGTSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for EGTP");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for EGTP");
   }

    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, cntrl);
 
   RETVALUE(ret);
} /* smBindUnBindWrToEgtSap */

#endif /* EU_DAT_APP */

/*
 *      FUN:   smBindUnBindWrToNhuSap
*
 *      Desc:  Bind and UnBind WR with NHU LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToNhuSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToNhuSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst        pst;
   S16      ret = ROK;

   TRC2(smBindUnBindWrToNhuSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_NH_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRNHUSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for RRC");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for RRC");
   }

    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ret);
} /* smBindUnBindWrToNhuSap */


/*
 *      FUN:   smBindUnBindWrToCtfSap
*
 *      Desc:  Bind and Unbind WR with CTF LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToCtfSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToCtfSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst       pst;
   S16      ret = ROK;

   TRC2(smBindUnBindWrToCtfSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_YS_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRCTFSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for Ctf");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for Ctf");
   }
    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, cntrl);
 
   RETVALUE(ret);
} /* smBindUnBindWrToCtfSap */

#ifdef RM_INTF 
/*RRM->FSM */
/*
 *      FUN:   smBindUnBindWrToRmuSap
 *
 *      Desc:  Bind and unbind WR with RMU LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  wr_smm_enbapp_rsys.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToRmuSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToRmuSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindUnBindWrToRmuSap)


   SM_SET_ZERO(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.t.cntrl.action        = action;
   cntrl.t.cntrl.subAction    = 0;
   cntrl.t.cntrl.u.sapId       = WR_RM_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTWR;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STWRRMUSAP;
   cntrl.hdr.response.selector   = RMU_SEL_LWLC;
   /* Fill Pst */
   pst.selector  = RMU_SEL_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for RRM");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for RRM");
   }

    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindUnBindWrToRmuSap */


#endif /*RM_INTF*/

/*
 *      FUN:   smBindUnBindWrToNluSap
 *
 *      Desc:  Bind and Unbind WR with NLU LSAP.
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  wr_smm_enbapp_rsys.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToNluSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToNluSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst       pst;
   S16       ret = ROK;

   TRC2(smBindUnBindWrToNluSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction     = 0;
   cntrl->t.cntrl.u.sapId       = WR_NL_SUID;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRNLUSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */

   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;

#ifdef DEBUGP
  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "NL Bnd Cntrl Req to wr for SON\n"));
#endif
   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for SON");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for SON");
   }

    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ret);
} /* smBindUnBindWrToNluSap */

#ifdef EU_DAT_APP

/*
 *      FUN:   smBindUnBindWrToPjuSap
*
 *      Desc:  Bind and Unbind WR with PJU LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToPjuSap
(
Inst inst, U8 action
)
#else
PUBLIC S16 smBindUnBindWrToPjuSap(inst,action)
Inst inst;
U8 action;
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst       pst;
   S16      ret = ROK;

   TRC2(smBindUnBindWrToPjuSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_PJ_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRPJUSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
       RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for PDCP");
   }
   else
   {
       RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for PDCP");
   }

    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, cntrl);
 
   RETVALUE(ret);
} /* smBindUnBindWrToPjuSap */

#endif /* EU_DAT_APP */
/*
 *      FUN:   smBindUnBindWrToRgrSap
*
 *      Desc:  Bind and unbind WR with RGR LSAP. 
*
 *      Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_rsys.c
*
 *
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindWrToRgrSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindWrToRgrSap(U8 action)
#endif /* ANSI */
{
   LwrMngmt  *cntrl = NULLP;
   Pst        pst;
   S16      ret = ROK;
 
   TRC2(smBindUnBindWrToRgrSap)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction    = 0;
   cntrl->t.cntrl.u.sapId       = WR_RG_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRRGRSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "Bnd Cntrl Req to wr for SCH");
   }
   else
   {
      RLOG0(L_DEBUG, "UnBnd Cntrl Req to wr for SCH");
   }

    /* Send the control request to the WR*/
   (Void)SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ret);
} /* smBindUnBindWrToRgrSap */

/*
 *
*       Fun:   smBuildWrErabRelIndCntrl
 *
*       Desc:  Invoked to build and send the eNodeB RAB Release
               Indication message from eNodeB.
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrErabRelIndCntrl 
(
Void
)
#else
PUBLIC S16 smBuildWrErabRelIndCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildWrErabRelIndCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRSNDERABRELIND;
   /* filling proper selector value */
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "Control Req to send ERAB Release Indication");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrGenCntrl */



/*
 *
*       Fun:   smBuildWrGenCntrl
 *
*       Desc:  Invoked to Enable Alarms at APP
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrGenCntrl
(
)
#else
PUBLIC S16 smBuildWrGenCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrGenCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.u.sapId       =  0;
   cntrl->t.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRGEN;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Gen Cntrl Req to wr sent");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrGenCntrl */

/* CNM_DEV */
/*
 *
 *      Fun:   smBuildWrSyncCntrl
 *
 *      Desc:  Invoked to intiate Initial Sync
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_smm_enbapp_rsys.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrSyncCntrl
(
)
#else
PUBLIC S16 smBuildWrSyncCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   
   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrSyncCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRINITALSYNC;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "Start Sync Req to WR");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrSyncCntrl */

/*
 *
*       Fun:   smBuildWrCellCntrl
 *
*       Desc:  Invoked to intimate the APP that Binding is DONE
*              and initiate cell control
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrCellCntrl
(
)
#else
PUBLIC S16 smBuildWrCellCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrCellCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRCELLCFG;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   RLOG0(L_DEBUG, "RRM Cell Cntrl Req to wr");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrCellCntrl */

/*
 *
*       Fun:   smBuildWrS1InitCntrl
 *
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the S1AP association
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrS1InitCntrl
(
)
#else
PUBLIC S16 smBuildWrS1InitCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
  U8  noOfCfg=0; 
   
   TRC2(smBuildWrS1InitCntrl)

#if (defined(WR_PERF_MEAS) || defined(WR_WITHOUT_CNE))
   smBuildWrCellCntrl();
   RETVALUE(ROK);
#endif


   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRS1CONSETUP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   for ( noOfCfg=0; noOfCfg<smCfgCb.noOfCfg; noOfCfg++)
   {
     cntrl->t.cntrl.peerId[noOfCfg] = smCfgCb.mmeCfg[noOfCfg]->mmeId;
     cntrl->t.cntrl.idLst[noOfCfg] = smCfgCb.mmeCfg[noOfCfg]->mmeId;
   }
     cntrl->t.cntrl.numConn = noOfCfg;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Cntrl Req to wr for S1 Init");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrInitCntrl */
/*
 *
*       Fun:   wrSmDynInitS1Setup
 *
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the S1AP association
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 wrSmDynInitS1Setup
(
Void
)
#else
PUBLIC S16 wrSmDynInitS1Setup(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
#ifdef WR_RSYS_OAM
   U32      idx;
#endif
   TRC2(wrSmDynInitS1Setup)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRS1CONSETUP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   cntrl->t.cntrl.numConn = 1;
#ifdef WR_RSYS_OAM
   for(idx = 0; idx < smCfgCb.numMmeAdded; idx++)
   {
     cntrl->t.cntrl.numConn = smCfgCb.numMmeAdded;
     cntrl->t.cntrl.peerId[idx] = smCfgCb.addMmeList[idx];
     cntrl->t.cntrl.idLst[idx] = smCfgCb.addMmeList[idx]; 
   }
#endif/*end of WR_RSYS_OAM*/
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Cntrl Req to wr for S1 Init");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrInitCntrl */



#ifdef LTE_HO_SUPPORT
/*
 *
*       Fun:   smBuildWrX2InitCntrl
 *
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the X2AP association
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrX2InitCntrl
(
Void
)
#else
PUBLIC S16 smBuildWrX2InitCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildWrX2InitCntrl)

   
   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRX2CONSETUP;
   cntrl->hdr.response.selector  = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   smFillX2InitCntrl(cntrl);
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Cntrl Req to wr for X2 Init");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrX2InitCntrl */
#endif /* End of smBuildWrX2InitCntrl */

#ifdef LTE_HO_SUPPORT
/*
 *
*       Fun:   wrSmDynInitX2Setup
 *
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the X2AP association
*
*       Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 wrSmDynInitX2Setup
(
Void
)
#else
PUBLIC S16 wrSmDynInitX2Setup(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   
   TRC2(wrSmDynInitX2Setup)

   
   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRX2CONSETUP;
   cntrl->hdr.response.selector  = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;

   cntrl->t.cntrl.numConn = 1;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRM Cntrl Req to wr for X2 Init");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of wrSmDynInitX2Setup */
#endif

/**
 * @brief This function is used to send enable/disable control request to 
 * the Application layers.
*
 * @details
*
 * Function: smBuildWrLogCntrl
*
 * @param[in] actType
 * @param[in] mask
*
 * @return Void
*/
#ifdef ANSI
PUBLIC S16 smBuildWrLogCntrl
(
 Bool actType,
 U32  mask
)
#else
PUBLIC S16 smBuildWrLogCntrl(actType, mask)
 Bool actType;
 U32  mask;
#endif
{
   LwrMngmt cntrl;
   Pst      pst;
   
   TRC2(smBuildWrLogCntrl)

   SM_SET_ZERO(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));


   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTWR;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STWRGEN;
   cntrl.hdr.response.selector   = SM_SELECTOR_TC;

   if(actType == WR_ON)
   {
      cntrl.t.cntrl.action       =  AENA;
   }
   else
   {
      cntrl.t.cntrl.action       =  ADISIMM;
   }
   cntrl.hdr.response.selector   = SM_SELECTOR_TC;
   cntrl.hdr.response.mem.region = WR_MEM_REGION;
   cntrl.hdr.response.mem.pool   = WR_POOL;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_TC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrLogCntrl */

/*
*
*       Fun:   smBuildWrDbgCntrl
*
*       Desc:  Invoked to enable debug prints
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_wr_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrDbgCntrl
(
)
#else
PUBLIC S16 smBuildWrDbgCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildWrDbgCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRGEN;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.subAction     =  SADBG;

   /* wr003.102: Fix for CRId- 115717 */
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   cntrl->hdr.response.mem.region = WR_MEM_REGION;
   cntrl->hdr.response.mem.pool   = WR_POOL;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRM Debug Cntrl Req to wr");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrInitCntrl */

/* IPSec Recovery */
/*
 *
 *      Fun:   smBuildWrShutDownCntrl
 *
 *      Desc:  Invoked to shut down the APP
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_sm_wr_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrShutDownCntrl
(
)
#else
PUBLIC S16 smBuildWrShutDownCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrShutDownCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        =  ASHUTDOWN;
   /* cntrl->t.cntrl.u.sapId       =  0; */ /* Needed */
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRGEN;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "APP Shutdown Cntrl Req to wr sent");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of  smBuildWrShutDownCntrl */
/* IPSec Recovery */
/*
 *
 *      Fun:   smBuildWrUnbindSz
 *
 *      Desc:  Invoked to unbind SZ
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_smm_enbapp_rsys.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrUnbindSz
(
Void
)
#else
PUBLIC S16 smBuildWrUnbindSz(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrUnbindSz)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        =  AUBND_DIS;
   /* cntrl->t.cntrl.u.sapId       =  0; */ /* Needed */
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRSZTSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "APP Unbind SZ");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of  smBuildWrUnbindSz */

/* IPSec Recovery */
/*
 *
 *      Fun:   smBuildWrUnbindCz
 *
 *      Desc:  Invoked to unbind CZ
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_smm_enbapp_rsys.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrUnbindCz
(
Void
)
#else
PUBLIC S16 smBuildWrUnbindCz(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrUnbindCz)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        =  AUBND_DIS;
   /* cntrl->t.cntrl.u.sapId       =  0; */ /* Needed */
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRCZTSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "APP Unbind to CZ");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of  smBuildWrUnbindCz */

/* IPSec Recovery */
/*
 *
 *      Fun:   smBuildWrUnbindEg
 *
 *      Desc:  Invoked to Unbind the EG
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  wr_smm_enbapp_rsys.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildWrUnbindEg
(
Void
)
#else
PUBLIC S16 smBuildWrUnbindEg(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   /* wr002.102: Corrected function name in trace macro */
   TRC2(smBuildWrUnbindEg)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cntrl->t.cntrl.action        =  AUBND_DIS;
   /* cntrl->t.cntrl.u.sapId       =  0; */ /* Needed */
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWREGTSAP;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "APP Unbind to EGTP");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of  smBuildWrUnbindEG */

/* IPSec Recovery */

/*
*
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smwrexms.c
*
*/
#ifdef ANSI
PUBLIC S16 smWrActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smWrActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smWrActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(reason);
   smCb.init.region = region;
   smCb.init.pool  = 0;
   RETVALUE(ROK);
}


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from WR
*
*       Ret:    ROK  - ok
*
*       Notes: None
*
*       File:   wr_sm_wr_exms.c
*
*/
#ifdef ANSI
PUBLIC S16 smWrActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smWrActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC2(smWrActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#if (defined (LCWRMILWR) || defined(LWLCWRMILWR))
      case EVTLWRCFGREQ:             /* Config Request from SM */
         ret = cmUnpkLwrCfgReq(SmMiLwrAddNeighCfgReq, pst, mBuf);
         break;
      case EVTLWRNGHCFGREQ:             /* Config Request from SM */
         ret = cmUnpkLwrCfgReq(SmMiLwrAddNeighCfgReq, pst, mBuf);
         break;
      case EVTLWRCFGCFM:             /* Config confirm */
         ret = cmUnpkLwrCfgCfm(SmMiLwrCfgCfm, pst, mBuf);
         break;
      case EVTLWRCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLwrCntrlCfm(SmMiLwrCntrlCfm, pst, mBuf);
         break;
      case EVTLWRSTAIND:
         ret = cmUnpkLwrStaInd(SmMiLwrStaInd, pst,mBuf);
         break;
      case EVTLWRDYNCFGCFM:
         ret = cmUnpkLwrDynCfgCfm(SmMiLwrDynCfgCfm, pst, mBuf); 
         break;
      case EVTLWRADDNGHCELLCFGREQ:
         ret = cmUnpkLwrUpDateNhgCellCfgReq(SmMiLwrUpDateNhgCellCfgReq, pst, mBuf);
         break;
#ifdef WR_RSYS_OAM
      case EVTNGHCELLIND:
         ret=cmUnpkLwrNghCellInd(SmMiLwrHdlNghCellInd,pst,mBuf);
         break;
#endif
#endif
      case EVTWRSCTPCFG:
      case EVTWRS1APCFG:
      case EVTWRX2APCFG:
      case EVTWREGTPCFG:
      case EVTWRAPPCFG:
      case EVTWRRRCCFG:
      case EVTWRPDCPCFG:
      case EVTWRRLCCFG:
      case EVTWRMACCFG:
      case EVTWRCLCFG:
      case EVTWRS1APSTKBND:
      case EVTWRX2APSTKBND:
      case EVTWREGTPSTKBND:
      case EVTWRRRCSTKBND:
      case EVTWRRRMSTKBND:
      case EVTWRENBALRMS:
      case EVTWRENBDEBUGS:
      case EVTWRENBLOG:
      case EVTWRINITS1SETUP:
      case EVTWRINITX2SETUP:
      case EVTWRCELLCFG:
      case EVTWRRRMCFG:
      case EVTWRTUCLCFG:
#ifdef E_TM
         if ( wrSmDfltBootMode )
         {
           smWrEtmProcSm(pst->event);
         }
         else
#endif /* E_TM */
         smWrProcSm(pst->event);
         (Void)SPutMsg(mBuf);
         break;
#ifdef WR_TEST_CODE
      case EVTWRCLIRECV:
         smWrProcCliMsg();
         SPutMsg(mBuf);
         break;
#endif /* WR_TEST_CODE */

      case EVTOAMCMDRECV:
         smBuildShutdown();
         SPutMsg(mBuf);
         break;

      case EVTWRTUCLSHUT:       
      case EVTWRSCTPSHUT:     
      case EVTWRS1APSHUT:     
      case EVTWREGTPSHUT:      
      case EVTWRAPPSHUT:       
      case EVTWRRRCSHUT:       
      case EVTWRPDCPSHUT:      
      case EVTWRRLCSHUT:       
      case EVTWRMACSHUT:       
      case EVTWRCLSHUT:        
      case EVTWRX2APSHUT:
      case EVTWRRRMSHUT:
      case EVTWRMACSCHSHUT:
      case EVTWRSONSHUT:
      case EVTWRRRMSTKUNBND:
      case EVTWRRRCSTKUNBND:
      case EVTWRX2APSTKUNBND:
      case EVTWREGTPSTKUNBND:
      case EVTWRS1APSTKUNBND:
           smWrShutdownSm(pst->event);
           (Void)SPutMsg(mBuf);        
          break;      
#ifdef WR_RSYS_OAM
      case EVTSMSTATICCFG:
         ret = cmUnPackSendStaticCfg(smStaticCfgReq, pst, mBuf);
          break;
      case EVTSMDYNCFG:
         ret = cmUnPackSendDynCfg(smDynCfgReq, pst, mBuf);
          break;
      /* ccpu00147960 */
      case EVTSTKSTRSTPREQ:
         ret = smUnPackStartStopStackReq(smStartStopStack, pst, mBuf);
          break;
      /* ccpu00147960 */
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         RLOG1(L_ERROR, "Invalid event [%d] received", pst->event);
         break;
   }

   RETVALUE(ret);
   
} /* end of smWrActvTsk */

/*

Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.
   
The following functions are provided in this file:

     SmMiLwrCfgReq      Configure Request
     SmMiLwrCntrlReq    Control Request

It is assumed that the following functions are provided in the
stack management body files:

     SmMiLwrStaInd      Status Indication
     SmMiLwrStaCfm      Status Confirm
     SmMiLwrStsCfm      Statistics Confirm
     SmMiLwrTrcInd      Trace Indication

*/

#ifdef LWLCSMMILWR
#define  MAXWRMI  3
#else
#define  MAXWRMI  2
#endif

#ifndef  LCSMMILWR
#ifndef  PTSMMILWR
#define  PTSMMILWR
#endif
#else
#ifndef   WR
#ifndef  PTSMMILWR
#define  PTSMMILWR
#endif
#endif
#endif




/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
   
The parameter MAXWRMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXWRMI long
(i.e. there are as many functions as the number of service users).
   
The dispatcysng is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMMILWR) 2 - Lwr (#define WR)
   
*/


/* Configuration request primitive */

PRIVATE LwrCfgReq SmMiLwrCfgReqMt[MAXWRMI] =
{
#ifdef LCSMMILWR
   cmPkLwrCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLwrCfgReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef WR
   WrMiLwrCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLwrCfgReq,          /* 1 - tightly coupled, portable */
#endif
#ifdef LWLCSMMILWR
   cmPkLwrCfgReq,          /* 2 - light weight loosely coupled   */
#else
   PtMiLwrCfgReq,          /* 2 - light weight loosely coupled , portable */
#endif
};


/* Control request primitive */

PRIVATE LwrCntrlReq SmMiLwrCntrlReqMt[MAXWRMI] =
{
#ifdef LCSMMILWR
   cmPkLwrCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLwrCntrlReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef WR
   WrMiLwrCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLwrCntrlReq,          /* 1 - tightly coupled, portable */
#endif
#ifdef LWLCSMMILWR
   cmPkLwrCntrlReq,          /* 2 - light weight loosely coupled  */
#else
   PtMiLwrCntrlReq,          /* 2 - light weight loosely coupled, portable */
#endif
};
   

PRIVATE LwrCfgCfm SmMiLwrAddNeighCfmMt[MAXWRMI] =
{
#ifdef LCSMMILWR
   cmPkLwrAddNeighCfm,       /* 0 - loosely coupled  */
#else
   PtMiLwrAddNeighCfm,       /* 0 - loosely coupled, portable */
#endif
#ifdef WR
   WrMiLwrAddNeighCfm,       /* 2 - tightly coupled, layer management  */
#else
   PtMiLwrAddNeighCfm,       /* 2 - tightly coupled, portable  */
#endif
#ifdef LWLCSMMILWR
   cmPkLwrAddNeighCfm,       /* 1 - light weight loosely coupled  */
#else
   PtMiLwrAddNeighCfm,       /* 1 - light weight loosely coupled, portable */
#endif 
}; 
   

PRIVATE LwrLoadInd SmMiLwrLoadIndMt[MAXWRMI] =
{
#ifdef LCSMMILWR
   cmPkLwrLoadInd,            /* 0 - loosely coupled  */                       
#else                                                                          
   PtMiLwrLoadInd,            /* 0 - loosely coupled, portable */              
#endif
#ifdef WR                                                                      
   WrMiLwrLoadInd,            /* 2 - tightly coupled, layer management  */     
#else                                                                          
   PtMiLwrLoadInd,            /* 2 - tightly coupled, portable  */             
#endif
#ifdef LWLCSMMILWR                                                             
   cmPkLwrLoadInd,            /* 1 - light weight loosely coupled  */          
#else                                                                          
   PtMiLwrLoadInd,            /* 1 - light weight loosely coupled, portable */ 
#endif                                                                         
}; 

/* Dynamic Configuration request primitive */
PRIVATE LwrDynCfgReq SmMiLwrDynCfgReqMt[MAXWRMI] =
{
#ifdef LCSMMILWR
   cmPkLwrDynCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLwrDynCfgReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef WR
   WrMiLwrDynCfgReq,          /* 2 - tightly coupled, layer management */
#else
   PtMiLwrDynCfgReq,          /* 2 - tightly coupled, portable */
#endif
#ifdef LWLCSMMILWR
   cmPkLwrDynCfgReq,          /* 1 - light weight loosely coupled   */
#else
   PtMiLwrDynCfgReq,          /* 1 - light weight loosely coupled , portable */
#endif
}; 
 
/* Nieghbour Cell indication to OAM */
PRIVATE LwrNghCellIndToOam SmMiLwrNghCellInd[MAXWRMI] =
{
#ifdef LCSMMILWR
   NULLP,                            /* 0 - loosely coupled  */
#else
   NULLP,                           /* 0 - loosely coupled, portable */
#endif
#ifdef WR
   NULLP,                           /* 2 - tightly coupled, layer management */
#else
   NULLP,                           /* 2 - tightly coupled, portable */
#endif
#ifdef LWLCSMMILWR
   cmPkLwrNghCellInd,          /* 1 - light weight loosely coupled   */
#else
   NULLP,                           /* 1 - light weight loosely coupled , portable */
#endif
}; 

 
/*
*     layer management interface functions 
*/


#ifdef WR_TEST_CODE
/*
*
*       Fun:   smBuildWrPartRstCntrl
*
*       Desc:  Invoked to build and send the eNodeB RESET (PART)
*              message from eNodeB
*
*       Ret:   ROK
*
*       Notes: None
*
*       File: wr_smm_enbapp.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrPartRstCntrl
(
Void
)
#else
PUBLIC S16 smBuildWrPartRstCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildWrPartRstCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRSNDPARTS1RST;

   /* ccpu00127155 :filling proper selector value */
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "Control Request to send Partial RESET");

   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrPartRstCntrl */


/*
*
*       Fun:   smBuildWrFullRstCntrl
*
*       Desc:  Invoked to build and send the eNodeB RESET (FULL)
*              message from eNodeB
*
*       Ret:   ROK
*
*       Notes: None
*
*       File: wr_sm_enbapp.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrFullRstCntrl
(
Void
)
#else
PUBLIC S16 smBuildWrFullRstCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildWrFullRstCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRSNDFULLS1RST;

   /* ccpu00127155 :filling proper selector value */
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   RLOG0(L_DEBUG, "Control Request to send Full Reset");

   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrFullRstCntrl */

/*
*
*       Fun:   smBuildNghRstCntrl
*
*       Desc:  Invoked to build and send the X2-RESET 
*              message from eNodeB
*
*       Ret:   ROK
*
*       Notes: None
*
*       File: wr_sm_enbapp.c 
*
*/

#ifdef ANSI
PUBLIC S16 smBuildNghRstCntrl
(
Void
)
#else
PUBLIC S16 smBuildNghRstCntrl(Void)
#endif
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildNghRstCntrl)

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRTRIGX2RST;

   /* filling proper selector value */
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "Control Request to send X2-Reset");

   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildWrFullRstCntrl */

/*
*
 *       Fun:   smBuildAnrPrintCntrl
*
 *       Desc:  Invoked to print the neighbour cell
 *              information in ANR
*
 *       Ret:   ROK
*
*       Notes:  None
*
 *       File: wr_sm_enbapp.c
*
*/

#ifdef ANSI
PUBLIC S16 smBuildAnrPrintCntrl
(
Void
)
#else
PUBLIC S16 smBuildAnrPrintCntrl(Void)
#endif
{
   LwrMngmt *cntrl;
   Pst      pst;

   TRC2(smBuildAnrPrintCntrl)

   SM_SET_ZERO(&pst, sizeof(Pst));
   WR_ALLOC(&cntrl, sizeof(LwrMngmt));

   if(cntrl == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRANRPRINTCELL;
     
   /* :filling proper selector value */
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_TC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "Control Request to print neighbour cell Info in ANR");

   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildAnrPrintCntrl */
   

   

#ifdef ENB_OVRLOAD

/*
*
*       Fun:   smSndWrCellReCfgSib2
*
*       Desc:  This function for sending eNodeB SIB2 ReConfiguration to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellReCfgSib2
(
U8 ovldFlag
)
#else
PUBLIC S16 smSndWrCellReCfgSib2(ovldFlag)
U8 ovldFlag;
#endif
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   LwrCellSib2CfgGrp *sib2 =NULLP;
   U8                cfgGrpIndex  = 0;

 
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic cell id */
   //lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = wrSmDfltCellIndex;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = 0;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;
   /* mem_issue_fix: Allocating memory for all the cell group though
    * all the groups are not being used as of now. This helps to avoid
    * multiple allocation and can have a common logic to free the memory */
   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
         (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed. for cell modification");
      RETVALUE(RFAILED);
   }

   /*Filling Sib2 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType
                                                        = WR_CELL_CFG_GRP_SIB2;

   sib2 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.sib2CfgGrp);
   sib2->reCfgType = LWR_CELL_SIB2_CFG_BARRING_INFO;
   wrFillTknU8(&(sib2->acBarringInfo.pres), wrSmDfltACBarrInfoPres);

   if (ovldFlag == CRITICAL)
   {
      sib2->acBarringInfo.acBarringForEmergency                = wrSmDfltACBarrEmergency;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOSignal.pres), 
                                                      wrSmDfltDOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = WR_SIB2_ACBRFTR0;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringTime   = WR_SIB2_ACBRTIME128;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOData.pres), 
                                                       wrSmDfltMOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOData.acBarringFactor   = WR_SIB2_ACBRFTR0;
      sib2->acBarringInfo.acBarringForMOData.acBarringTime     = WR_SIB2_ACBRTIME128;
   }
   else if (ovldFlag == MAJOR)
   {
      sib2->acBarringInfo.acBarringForEmergency                = wrSmDfltACBarrNormal;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOSignal.pres), 
                                                      wrSmDfltDOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = WR_SIB2_ACBRFTR90;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringTime   = WR_SIB2_ACBRTIME32;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOData.pres), 
                                                       wrSmDfltMOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOData.acBarringFactor   = WR_SIB2_ACBRFTR70;
      sib2->acBarringInfo.acBarringForMOData.acBarringTime     = WR_SIB2_ACBRTIME32;
   }
   else if (ovldFlag == MINOR)
   {
      sib2->acBarringInfo.acBarringForEmergency                = wrSmDfltACBarrNormal;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOSignal.pres), 
                                                      wrSmDfltDOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = WR_SIB2_ACBRFTR95;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringTime   = WR_SIB2_ACBRTIME16;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOData.pres), 
                                                       wrSmDfltMOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOData.acBarringFactor   = WR_SIB2_ACBRFTR80;
      sib2->acBarringInfo.acBarringForMOData.acBarringTime     = WR_SIB2_ACBRTIME16;
   }
   else
   {
      sib2->acBarringInfo.acBarringForEmergency                = wrSmDfltACBarrNormal;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOSignal.pres), 
                                                      wrSmDfltDOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = WR_SIB2_ACBRFTR50;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringTime   = WR_SIB2_ACBRTIME4;
      wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOData.pres), 
                                                       wrSmDfltMOACBarrInfoPres);
      sib2->acBarringInfo.acBarringForMOData.acBarringFactor   = WR_SIB2_ACBRFTR50;
      sib2->acBarringInfo.acBarringForMOData.acBarringTime     = WR_SIB2_ACBRTIME4;
   }

   if (ovldFlag != NORMAL)
   {
      sib2->acBarringInfo.acBarringForMOData.acBarringAC11 = wrSmDfltacBarringAC11;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC12 = wrSmDfltacBarringAC12;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC13 = wrSmDfltacBarringAC13;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC14 = wrSmDfltacBarringAC14;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC15 = wrSmDfltacBarringAC15;
 
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC11 = 
         wrSmDfltacBarringAC11;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC12 
         =wrSmDfltacBarringAC12;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC13 = 
         wrSmDfltacBarringAC13;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC14 = 
         wrSmDfltacBarringAC14;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC15 = 
         wrSmDfltacBarringAC15; 
   }
   else
{
      sib2->acBarringInfo.acBarringForMOData.acBarringAC11 = ZERO;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC12 = ZERO;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC13 = ZERO;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC14 = ZERO;
      sib2->acBarringInfo.acBarringForMOData.acBarringAC15 = ZERO;

      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC11 = ZERO;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC12 = ZERO;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC13 = ZERO;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC14 = ZERO;
      sib2->acBarringInfo.acBarringForMOSignal.acBarringAC15 = ZERO;
   }
   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;
 
   /* :filling proper selector value */
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
 
    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
}                                             
#endif /* ENB_OVRLOAD */
#endif /* WR_TEST_CODE */

 
/* LTE_ADV starts */
/*
*
*       Fun:   smBuildLteAdvConfig
*
*       Desc: construct LwrLteAdvancedConfigTransaction
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_enbapp.c
*/
#ifdef ANSI
PUBLIC S16 smBuildLteAdvConfigTrans
(
 LwrLteAdvancedConfigTrans *lteAdvGroup,
 WrEmmlteAdvFeatureActDeact *configInfo
)
#else
PUBLIC S16 smBuildLteAdvConfigTrans(lteAdvGroup, configInfo)
LwrLteAdvancedConfig *lteAdvGroup;
WrEmmlteAdvFeatureActDeact *configInfo;
#endif
{
   cmMemcpy((U8 *)&lteAdvGroup->lteAdvancedConfig, (U8 *)&smCfgCb.lteAdvancedConfig, sizeof(LwrLteAdvancedConfig)); 
/* LTE_ADV_ABS starts */
   if ((configInfo->feature & RGR_ABS) == RGR_ABS) {
       lteAdvGroup->lteAdvancedConfig.absCfg.status = configInfo->flag;
   }
/* LTE_ADV_ABS ends */

   if ((configInfo->feature & RGR_SFR) == RGR_SFR) {
       lteAdvGroup->lteAdvancedConfig.sfrCfg.status = configInfo->flag;

   }

   if ((configInfo->feature & RGR_DSFR) == RGR_DSFR) {
       lteAdvGroup->lteAdvancedConfig.dsfrCfg.status = configInfo->flag;

   }
   if ((configInfo->feature & RGR_RE) == RGR_RE) {
       lteAdvGroup->lteAdvancedConfig.reCfg.status = configInfo->flag;
   }

   lteAdvGroup->pres = configInfo->feature;
   RETVALUE(ROK);

}

/*
*
*       Fun:   smBuildWrLteAdvancedCellCfg
*
*       Desc:  Involed to modify CellCb with LteAdvanced Features
*
*       Ret:   ROK
*
*       Notes: None
*
*       File: wr_smm_enbapp.c
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrLteAdvancedCellReCfg
(
WrEmmlteAdvFeatureActDeact *configInfo
)
#else
PUBLIC S16 smSndWrLteAdvancedCellReCfg(
WrEmmlteAdvFeatureActDeact *configInfo;
)
#endif
{
   LwrMngmt          lwrMng;
   Pst               pst;
   LwrLteAdvancedConfigTrans *lteAdvGroup =NULLP;
   U8                cfgGrpIndex  = 0;

   SM_SET_ZERO(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lwrMng.t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng.t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng.t.protoCfg.t.cellCfg.t.modCell.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic cell id */
   lwrMng.t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng.t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;

   /* mem_issue_fix: Allocating memory for all the cell group though
    * all the groups are not being used as of now. This helps to avoid
    * multiple allocation and can have a common logic to free the memory */
   WR_ALLOC(&(lwrMng.t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
         (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng.t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed for cell modification");
      RETVALUE(RFAILED);
   }

   /*Filling lteAdvGroup params*/
   lwrMng.t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_LTEA;
   lteAdvGroup =&(lwrMng.t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.lteAdvCfgGrp);
   smBuildLteAdvConfigTrans(lteAdvGroup, configInfo);
   cfgGrpIndex++;

   lwrMng.t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;

   lwrMng.hdr.msgType             = TCFG;
   lwrMng.hdr.msgLen              = 0;
   lwrMng.hdr.entId.ent           = ENTWR;
   lwrMng.hdr.entId.inst          = SM_INST_ZERO;
   lwrMng.hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng.hdr.response.selector   = SM_SELECTOR_TC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_TC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, &lwrMng);

   RETVALUE(ROK);
} /* end of smSndWrLteAdvancedCellReCfg*/

/* LTE_ADV ends */
/*
*
*       Fun:  smSndLoadInd
*
*       Desc:  Invoked to build and send ENB recfg to modify cpu load
*
*       Ret:   ROK
*
*       Notes: None
*
*       File: wr_sm_enbapp.c 
*
*/
PUBLIC S16 smSndLoadInd
(
U16                          choice,
U16                          value
)
{
   LwrLoadIndInfo loadIndInfo;
   Pst            pst;

   switch (choice)
   {
      case WRSM_ENB_CPU_LOAD:
         {
            loadIndInfo.cpuLoad = (U8) value;
         }
         break;

      default:
         {
            RLOG1(L_ERROR,  "Wrong Load type :%d", choice);
            RETVALUE(RFAILED);
         }
   }

   pst.selector  = SM_SELECTOR_TC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrLoadInd(&pst, &loadIndInfo);

   RETVALUE(ROK);
}

/*
*
*       Fun:   wrSmmGetX2apFreePeerId
*
*       Desc:  To get free PeerId from list.
*
*       Ret:   ROK on success, RFAILED on error
*

*       Notes: None
*
*       File:  wr_smm_init_merged.c
*
*/
PUBLIC U16 wrSmmGetX2apFreePeerId()
{
   U8                        idCnt;
  
   for(idCnt = 0;idCnt < WR_SMM_MAX_NBR_ENB; idCnt++)
{
      if(smCfgCb.wrX2apPeerIdLst[idCnt].pres == FALSE)
      {
         smCfgCb.wrX2apPeerIdLst[idCnt].pres = TRUE;
         RETVALUE(smCfgCb.wrX2apPeerIdLst[idCnt].val);
      }
   }
   RETVALUE(WR_SMM_INVALID_PEERID);
}


/***************************************************************************
 *  layer management Confirmation handling
 **************************************************************************/

PRIVATE S16 wrSmmGetwrWrCfg
(
    LwrMngmt *cfm 
)
{
   static U8       mmeIdx = 1;
   static U8       utraCfgIdx = 0;
   S16             retVal = ROK;

   switch (cfm->hdr.transId)
   {
      case LWR_TRANSID_GEN: 

         wrWrCfg |= WR_WR_GEN_CFG;
         smBuildWrCtfLSapCfg(WR_YS_SUID, WR_YS_SPID);
         break;      
      case LWR_TRANSID_CTF: 
         wrWrCfg |= WR_WR_CTF_CFG;
         smBuildWrNhuLSapCfg(WR_NH_SUID, WR_NH_SPID);
         break;
      case LWR_TRANSID_NHU:
         wrWrCfg |= WR_WR_NHU_CFG;
            smBuildWrNluLSapCfg(WR_NL_SUID, WR_NL_SPID);
            break;
         case LWR_TRANSID_NLU:
            wrWrCfg |= WR_WR_NLU_SAP_CFG;
         smBuildWrPjuLSapCfg(WR_PJ_SUID, WR_PJ_SPID);
         break;
#ifdef RM_INTF
      case LWR_TRANSID_PJU:
         wrWrCfg |= WR_WR_PJU_CFG;
            if (1 == wrNumPjuSaps)
            {
            smBuildWrPjuLSapCfg(WR_PJ_SUID + 1, WR_PJ_SPID);
            wrNumPjuSaps++;
            }
            else
            {
         smBuildWrRmuLSapCfg(WR_RM_SUID, WR_RM_SPID);
            }
         break;
      case LWR_TRANSID_RMU:
         wrWrCfg |= WR_WR_RMU_SAP_CFG;
         smBuildWrEgtLSapCfg(WR_EG_SUID, WR_EG_SPID);
         break;
#else
      case LWR_TRANSID_PJU:
         wrWrCfg |= WR_WR_PJU_CFG;
            if (!wrNumPjuSaps)
            {
               smBuildWrPjuLSapCfg(WR_PJ_SUID + 1, WR_PJ_SPID);
            }
            else
            {
         smBuildWrEgtLSapCfg(WR_EG_SUID, WR_EG_SPID);
            }
            wrNumPjuSaps++;   // Murtuza:Check This->This is removed in the new code
         break;
#endif
      case LWR_TRANSID_EGT:
         wrWrCfg |= WR_WR_EGT_CFG;
         smBuildWrSztLSapCfg(WR_SZ_SUID, WR_SZ_SPID);
         break;
      case LWR_TRANSID_SZT:
         wrWrCfg |= WR_WR_SZT_CFG;
         smBuildWrCztLSapCfg(WR_CZ_SUID, WR_CZ_SPID);
         break;
      case LWR_TRANSID_CZT:
         wrWrCfg |= WR_WR_CZT_CFG;
         smBuildWrRgrLSapCfg(WR_RG_SUID, WR_RG_SPID);
         break;
      case LWR_TRANSID_RGR:            
         wrWrCfg |= WR_WR_RGR_CFG;
         smSndWrEnbCfg();
         break;
      case LWR_TRANSID_ENBCFG:         
         wrWrCfg |= WR_WR_ENBCFG_CFG;
         smSndWrCellCfg();
         break;
      case LWR_TRANSID_CELLCFG:        
            RLOG0(L_DEBUG, "cell configuration confirm received");
            RLOG0(L_DEBUG, "sending mme configuration");
            wrWrCfg |= WR_WR_CELLCFG_CFG;
            mmeIdx = 1;
            smFreeCellCfgMem(&cfm->t.protoCfg.t.cellCfg.t.addCell ,WR_CELL_CFG_MAX_GRPS );
            smSndWrMmeCfg((smCfgCb.mmeCfg[0]));
         break;
           /* for modification*/
         case LWR_TRANSID_CELL_MOD:
            RLOG0(L_DEBUG, "cell modification confirm received");
            smFreeCellCfgMem(&cfm->t.protoCfg.t.cellCfg.t.modCell ,\
                  cfm->t.protoCfg.t.cellCfg.t.modCell.numOfGrps);
            break;
      case LWR_TRANSID_MME:            
         if(mmeIdx < smCfgCb.noOfCfg)
         {
            smSndWrMmeCfg((smCfgCb.mmeCfg[mmeIdx]));
            mmeIdx++;
         }
            else{
            wrWrCfg |= WR_WR_MME_CFG;
            smSndWrNhCfg();
         }
         break;
      case LWR_TRANSID_NHCFG:          
         wrWrCfg |= WR_WR_NHCFG_CFG;
         smSndWrNrEutraFreqCfg();
         break;
      case LWR_TRANSID_NREUTRAN:       
         wrWrCfg |= WR_WR_NREUTRAN_CFG;
         smSndWrNrUtraFddFreqCfg();
         break;
      case LWR_TRANSID_NRUTRAN:        
         utraCfgIdx++;
         if(utraCfgIdx == 2) 
         {
            wrWrCfg |= WR_WR_NRUTRAN_CFG;
            utraCfgIdx = 0;
            smSndWrNhCellCfg();
         }
         else
         {
            smSndWrNrUtraTddFreqCfg();
         }
         break;
      case LWR_TRANSID_NHCELLCFG:      
         wrWrCfg |= WR_WR_NHCELL_CFG;

         smSndWrNhUtraFddCellCfg();
         break;

      case LWR_TRANSID_NHUTRAN_CELLCFG:
      case LWR_TRANSID_INTER_NHCELLCFG:
         utraCfgIdx++;
         if(utraCfgIdx == 2) 
         {
            wrWrCfg |= WR_WR_NHUTRAN_CELL_CFG;
            wrWrCfg |= WR_WR_INTER_NHCELL_CFG;
            smSndWrCdma1xRTTBandClass();
         }
         else
         {
            smSndWrNhUtraTddCellCfg();
         }
         break;
      case LWR_TRANSID_CDMA1XRTT_BAND:      
         wrWrCfg |= WR_WR_CDMA1XRTT_BAND_CFG;
         smSndWrNr1xCdmaFreqCfg();
         break;
      case LWR_TRANSID_CDMA1XRTT_FREQ:       
         wrWrCfg |= WR_WR_CDMA1XRTT_FREQ_CFG;
         smSndWrNr1xCdmaCellCfg();
         break;
      case LWR_TRANSID_CDMA1XRTT_CELL:      
         wrWrCfg |= WR_WR_CDMA1XRTT_CELL_CFG;
         smSndWrNrGeranFreqCfg();
         break;

      case LWR_TRANSID_GERAN_FREQ: 
         wrWrCfg |= WR_WR_GERAN_FREQ_CFG;
         smSndWrNrGeranCellCfg();
         break;
      case LWR_TRANSID_GERAN_CELL: 
         wrWrCfg |= WR_WR_GERAN_CELL_CFG;
      case LWR_TRANSID_CDMAHRPD_CLASS: 
         wrWrCfg |= WR_WR_CDMAHRPD_FREQ_CFG;
         break; 

      default:
         RLOG1(L_ERROR, "APP Cfg Cfm received for unknown TransId, with "
            "element[%d]",cfm->hdr.elmId.elmnt);
         retVal = RFAILED;
            break;
    }
    RETVALUE(retVal);
}

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:     wr_sm_wr_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLwrCfgCfm
(
Pst     *pst,          /* post structure */
LwrMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLwrCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
LwrMngmt *cfm;          /* configuration */
#endif
{
   Pst      wrPst;
   U8       configUpdated = FALSE;
   S16      retVal = ROK;

   TRC2(SmMiLwrCfgCfm);

   SM_SET_ZERO(&wrPst, sizeof(Pst));
   if(cfm->cfm.status != LCM_PRIM_OK)
   {
      RLOG_ARG1(L_ERROR, DBG_TRNSID, cfm->hdr.transId,"APP Cfg Cfm received for"
            "the element = %d is NOT OK",cfm->hdr.elmId.elmnt);
#ifdef WR_RSYS_OAM
      /*timer stop :: Raised an alarm */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
      RETVALUE(ROK);
   }
   /* ccpu00127154 :Avoiding condition in case of dynamic update */
   if(wrWrCfg != WR_SM_WR_CONFIGURED)
   {
        retVal = wrSmmGetwrWrCfg(cfm);
        if(ROK == retVal)
        {
            configUpdated = TRUE;
        }
   } /* end of if statement wrWrCfg != WR_SM_WR_CONFIGURED*/

   RLOG_ARG1(L_DEBUG, DBG_TRNSID, cfm->hdr.transId,"APP Cfg Cfm received for"
         "the element=%d",cfm->hdr.elmId.elmnt);
#ifdef WR_RSYS_OAM
   if((smCb.smState == WR_SM_STATE_CELL_UP) || (smCb.smState == WR_SM_STATE_OAM_DYN_START))
   {
      if(cfm->t.protoCfg.type == WR_CFG_TYPE_NEIGH_CELL)
      {
         RLOG0(L_DEBUG, "Dynamic Neighbor Cell add completed.");
      }
      RETVALUE(ROK);
   }
#endif
   if(smCb.smState == WR_SM_STATE_CELL_UP)
   {
      if(cfm->t.protoCfg.type == WR_CFG_TYPE_NEIGH_CELL)
      {
         RLOG0(L_INFO, "Dynamic Neighbor Cell add completed.");
      }
      RETVALUE(ROK);
   }

   if (wrWrCfg == WR_SM_WR_CONFIGURED)
   {
      smCb.smState = WR_SM_STATE_APP_CFG_DONE;
      RLOG0(L_ALWAYS, "APP CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
      /*timer stop */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRRRCCFG, WR_POOL, SM_SELECTOR_LWLC);

      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
         RLOG0(L_FATAL, "Self Pst failed after APP Configuration.");
      }
      else
      {
         RLOG0(L_DEBUG, "Self Pst success after APP Configuration.");
      } /* end of else part */

   }

   RETVALUE(ROK);
} /* end of SmMiLwrCfgCfm */


/*
 *
 *       Fun:    SmMiLwrCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrCntrlCfm
(
 Pst         *pst,
 LwrMngmt     *cntrl
 )
#else
PUBLIC S16 SmMiLwrCntrlCfm(pst, cntrl)
   Pst         *pst;
   LwrMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  wrPst;

   /* wr002.102: Added trace macro */

   TRC2(SmMiLwrCntrlCfm)

      SM_SET_ZERO(&wrPst, sizeof(Pst));
   /*IPSec*/
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_RRM_SHUTDWN_DONE)
      {
         smCb.smState = WR_SM_STATE_APP_SHUTDWN_DONE;
         RLOG0(L_ALWAYS, "APP SHUTDOWN SUCCESSFULLY PERFORMED. ");
//         WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRRRCSHUT, WR_POOL, SM_SELECTOR_TC);
         WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRSONSHUT, WR_POOL, SM_SELECTOR_TC);
         if(wrSmSelfPstMsg(&wrPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after APP SHUTDOWN Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after APP SHUTDOWN Configuration.");
         } /* end of else part */
         RETVALUE(ROK);
      }
   }  /* end of LCM_PRIM_OK */


#ifdef WR_RELAY
   if(cntrl->hdr.elmId.elmnt == STWRCTFSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP  WITH CL SUCCESSFUL ");
            /* Unbind RRC to RLC DL */
            smBindUnBindNhToKwuSap(WR_SM_KWDL_INST,AUBND);
         }
         RETVALUE(ROK);
      }
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWRCTFSAP */
   else if(cntrl->hdr.elmId.elmnt == STWRSZTSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_EGTP_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP WITH S1AP SUCCESSFUL");
            /* Unbind S1AP with SCTP */
            smBindUnBindSzToLSap(AUBND);
         }
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_INFO, "Bind of APP with S1AP is in progress.");
      }
   }
#ifdef LTE_HO_SUPPORT
   else if(cntrl->hdr.elmId.elmnt == STWRCZTSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRC_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP WITH X2AP SUCCESSFUL");
            /* Unbind X2AP with SCTP*/
            smBindUnBindCzToLSap(AUBND);
         }
         RETVALUE(ROK);
      }
      RLOG0(L_INFO, "Bind of RRM Tsap with X2AP is in progress.");
   }
#endif
   else if(cntrl->hdr.elmId.elmnt == STWRNHUSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP  WITH RRC SUCCESSFUL ");
            /* Unbind APP to CL */
            smBindUnBindWrToCtfSap(AUBND_DIS);
         }
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_DEBUG, "Bind of APP with RRC (NHU) is completed");
      }
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWRNHUSAP */
   else if(cntrl->hdr.elmId.elmnt == STWRRGRSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
   {
      if(cntrl->cfm.status == LCM_PRIM_OK)
      {
            RLOG0(L_DEBUG,"UNBIND OF APP  WITH MAC SUCCESSFUL ");
            /* Unbind APP To RRM */
            smBindUnBindWrToRmuSap(AUBND_DIS);
         }
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_DEBUG, "Bind of APP with MAC is completed");
      }
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWRRGRSAP */
   else if(cntrl->hdr.elmId.elmnt == STWRRMUSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP  WITH RRM SUCCESSFUL ");
            /* Unbind APP To PDCP */
            smBindUnBindWrToPjuSap(WR_SM_PJUL_INST, AUBND);
         }
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_DEBUG, "Bind of APP with MAC is completed");
      }
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWRRGRSAP */
   else if (cntrl->hdr.elmId.elmnt == STWRGEN)
   {
      RLOG0(L_DEBUG, "Enabling Alarms of App is done.");
      if(cntrl->cfm.status == LCM_PRIM_OK)
      {
         if((smCfgCb.wrDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_WR_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | WR_SM_WR_DBG_MASK);
            /* Trigger Debug Cntrl */
            wrSmHdlEnbDebugs(EVTWRENBDEBUGS);
         }
      }
      else
      {
         RLOG0(L_ERROR, "SmMiLwrCntrlCfm failed!");
      }

   } /* end of else */
   else if( cntrl->hdr.elmId.elmnt == STWRS1CONSETUP)
   {
      RLOG0(L_DEBUG, "Received Control Confirm for S1 Setup.");
#ifdef WR_RSYS_OAM
      /*Triggering x2setup  after admin state changed*/
      if(WR_SM_STATE_OAM_DYN_START == smCb.smState)
      {
         smBuildWrX2InitCntrl();
      }
#endif
         /* smBuildWrCellCntrl();  */

   } /* end of else part */
#ifdef LTE_HO_SUPPORT
   else if( cntrl->hdr.elmId.elmnt == STWRX2CONSETUP)
   {
      RLOG0(L_DEBUG, "Received Control Confirm for X2 Setup.");
   } /* end of else part */
#endif
   else if( cntrl->hdr.elmId.elmnt == STWRCELLCFG)
   {

      RLOG0(L_DEBUG, "Received Control Confirm for CELL CFG.");
   } /* end of else part */
#endif  /* WR_RELAY */
#ifdef EU_DAT_APP
   else if(cntrl->hdr.elmId.elmnt == STWRPJUSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP  WITH PDCP SUCCESSFUL ");
            /* Unbind APP To SON */
            smBindUnBindWrToNluSap(AUBND_DIS);
         }
         RETVALUE(ROK);
      }
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWREGTSAP */
   else if(cntrl->hdr.elmId.elmnt == STWRNLUSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP  WITH SON SUCCESSFUL ");
            /* Unbind APP To RRC */
            smBindUnBindWrToNhuSap(AUBND_DIS);
         }
         RETVALUE(ROK);
      }
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWREGTSAP */
   else if(cntrl->hdr.elmId.elmnt == STWREGTSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_X2AP_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF APP WITH EGTP SUCCESSFUL");
            /* Unbind EGTP with TUCL */
            smBindUnBindEgToHitSap(EG_HI_SPID,AUBND_DIS);
         }
         RETVALUE(ROK);
      }


      RLOG0(L_INFO, "Bind of RRM to EGTP is in progress");
   } /* end of if statement cntrl->hdr.elmId.elmnt == STWRPJUSAP */
#endif /* end of EU_DAT_APP */
   RETVALUE(ROK);
} /* end of SmMiLwrCntrlCfm() */

#ifdef WR_RSYS_OAM
/**
*
*       Fun:  updateMsmNeiFreqParams 
*
*       Desc:  This function is used to update the neighbor frequency of MSM global structure
*              if the confirmation of dynamic configuration request is success.
*
*       Ret:   None
*
*       Notes:  None
*
*       File:   wr_sm_enbapp_rsys.c
*
*/
PRIVATE Void updateMsmNeiFreqParams(MsmLteNeighEutraFreq *dst, LwrNeighEutraFreq *src)
{
   dst->freqIndex = src->freqIndex;
   dst->earfcnUl = src->earfcnUl;
   dst->earfcnDl = src->earfcnDl;
   dst->priority = src->priority;
   dst->qRxLevMin = src->qRxLevMin;
   dst->pMax = src->pMax;
   dst->tReselection = src->tReselection;
   dst->tReselectionSFMed = src->tReselectionSFMed;
   dst->tReselectionSFHigh = src->tReselectionSFHigh;
   dst->sIntraSearch = src->sIntraSearch.pres;
   dst->sIntraSearch = src->sIntraSearch.val;
   dst->sNonIntraSearch = src->sNonIntraSearch.pres;
   dst->sNonIntraSearch = src->sNonIntraSearch.val;
   dst->servThresholdLow = src->servThresholdLow;
   dst->threshXHigh = src->threshXHigh;
   dst->threshXLow = src->threshXLow;
   dst->measBandwidth = src->measBandwidth.pres;
   dst->measBandwidth = src->measBandwidth.val;
   dst->presAntennaPort1 = src->presAntennaPort1;
   dst->neighCellCfg = src->neighCellCfg;
   dst->qOffsetFreq = src->qOffsetFreq;
   dst->offsetFreq = src->offsetFreq;
   dst->qQualMinRel9 = src->qQualMinRel9.val; 
   dst->threshXHighQR9 = src->threshXQRel9.threshXHighQR9;
   dst->threshXLowQR9 = src->threshXQRel9.threshXLowQR9;

   RETVOID;
} 
/**
*
*       Fun:  updateSmNeiFreqParams 
*
*       Desc:  This function is used to update the neighbor frequency of SM global structure
*              if the confirmation of dynamic configuration request is failure.
*
*       Ret:   None
*
*       Notes:  None
*
*       File:   wr_sm_enbapp_rsys.c
*
*/
PRIVATE Void updateSmNeiFreqParams(LwrNeighEutraFreq *dst, MsmLteNeighEutraFreq *src)
{
   dst->freqIndex = src->freqIndex;
   dst->earfcnUl = src->earfcnUl;
   dst->earfcnDl = src->earfcnDl;
   dst->priority = src->priority;
   dst->qRxLevMin = src->qRxLevMin;
   dst->pMax = src->pMax;
   dst->tReselection = src->tReselection;
   dst->tReselectionSFMed = src->tReselectionSFMed;
   dst->tReselectionSFHigh = src->tReselectionSFHigh;
   dst->sIntraSearch.pres = src->sIntraSearch;
   dst->sIntraSearch.val = src->sIntraSearch;
   dst->sNonIntraSearch.pres = src->sNonIntraSearch;
   dst->sNonIntraSearch.val = src->sNonIntraSearch;
   dst->servThresholdLow = src->servThresholdLow;
   dst->threshXHigh = src->threshXHigh;
   dst->threshXLow = src->threshXLow;
   dst->measBandwidth.pres = src->measBandwidth;
   dst->measBandwidth.val = src->measBandwidth;
   dst->presAntennaPort1 = src->presAntennaPort1;
   dst->neighCellCfg = src->neighCellCfg;
   dst->qOffsetFreq = src->qOffsetFreq;
   dst->offsetFreq = src->offsetFreq;
   dst->qQualMinRel9.val = src->qQualMinRel9; 
   dst->threshXQRel9.threshXHighQR9 = src->threshXHighQR9;
   dst->threshXQRel9.threshXLowQR9 = src->threshXLowQR9;

   RETVOID;
} 
/**
*
*       Fun:  updateMsmNeiCellParams 
*
*       Desc:  This function is used to update the neighbor cell of MSM global structure
*              if the confirmation of dynamic configuration request is success.
*
*       Ret:   None
*
*       Notes:  None
*
*       File:   wr_sm_enbapp_rsys.c
*
*/
PRIVATE Void updateMsmNeiCellParams(MsmEUTRANeighCell *dst, LwrEUTRANeighCell *src)
{
   U8 idx;
   dst->nrIndex = src->nrIndex;
   dst->cid = src->enbId;
   dst->pci = src->pci;
   dst->tac = src->tac;
   dst->numPlmn = src->numPlmn;
   for(idx=0;idx < src->numPlmn; idx++)
   {
       dst->plmnId[idx].numMncDigits = src->plmnId[idx].numMncDigits;
       dst->plmnId[idx].mcc[0] = src->plmnId[idx].mcc[0];
       dst->plmnId[idx].mcc[1] = src->plmnId[idx].mcc[1];
       dst->plmnId[idx].mcc[2] = src->plmnId[idx].mcc[2];
       dst->plmnId[idx].mnc[0] = src->plmnId[idx].mnc[0];
       dst->plmnId[idx].mnc[1] = src->plmnId[idx].mnc[1];
       dst->plmnId[idx].mnc[2] = src->plmnId[idx].mnc[2];
   }  
   dst->earfcnDl = src->earfcnDl;
   dst->earfcnUl = src->earfcnUl;
   dst->cio = src->cio;
   dst->qoffset = src->qoffset;
   dst->RSTxPower = src->refSigPower;
   dst->BlackListed = src->blackListed;
   dst->csgAccessMode = src->csgAccessMode;
   dst->csgId = src->csgId;
   RETVOID;
} 
/**
*
*       Fun:  updateSmNeiCellParams 
*
*       Desc:  This function is used to update the neighbor cell of SM global structure
*              if the confirmation of dynamic configuration request is success.
*
*       Ret:   None
*
*       Notes:  None
*
*       File:   wr_sm_enbapp_rsys.c
*
*/
PRIVATE Void updateSmNeiCellParams(LwrEUTRANeighCell *dst, MsmEUTRANeighCell *src)
{
   U8 idx;
   dst->nrIndex = src->nrIndex;
   dst->enbId = src->cid;
   dst->pci = src->pci;
   dst->tac = src->tac;
   dst->numPlmn = src->numPlmn;
   for(idx=0;idx < src->numPlmn; idx++)
   {
       dst->plmnId[idx].numMncDigits = src->plmnId[idx].numMncDigits;
       dst->plmnId[idx].mcc[0] = src->plmnId[idx].mcc[0];
       dst->plmnId[idx].mcc[1] = src->plmnId[idx].mcc[1];
       dst->plmnId[idx].mcc[2] = src->plmnId[idx].mcc[2];
       dst->plmnId[idx].mnc[0] = src->plmnId[idx].mnc[0];
       dst->plmnId[idx].mnc[1] = src->plmnId[idx].mnc[1];
       dst->plmnId[idx].mnc[2] = src->plmnId[idx].mnc[2];
   }  
   dst->earfcnDl = src->earfcnDl;
   dst->earfcnUl = src->earfcnUl;
   dst->cio = src->cio;
   dst->qoffset = src->qoffset;
   dst->refSigPower = src->RSTxPower;
   dst->blackListed = src->BlackListed;
   dst->csgAccessMode = src->csgAccessMode;
   dst->csgId = src->csgId;
   RETVOID;
} 
#endif
/**
 *
*       Fun:   Dynamic Configuration Confirm
 *
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
 *
*       Ret:   None
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrDynCfgCfm
(
Pst         *pst,          /* post structure */
LwrDynMngmt    *dynCfgCfm
)
#else
PUBLIC S16 SmMiLwrDynCfgCfm(pst, dynCfgCfm)
Pst         *pst;          /* post structure */
LwrDynMngmt  *dynCfgCfm;
#endif
   {
#ifdef WR_RSYS_OAM
   smDynTmrEvent = dynCfgCfm->cfgType;

   Header      *dynCfmHdr = &dynCfgCfm->hdr;
   MsmDynCfmInfo dynCfmInfo={0};
   if(dynCfgCfm->cfm.status == LCM_PRIM_OK)
   {
       smStopTmr((PTR)&(smCb),SM_TMR_DYN_CFG_TMR);
       if (LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG == dynCfgCfm->cfgType)
       {
          SmNeigEutraFreqList *nghEutraFreqList;
          PTR                  prevEntry = NULLP;
          U8                   nodeFound = FALSE;
          U8                   idx;
          while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraFreqLstCp), prevEntry, (PTR *)&nghEutraFreqList))
          {
              if((nghEutraFreqList->dynCfgInd == WR_CFG_ADD) ||
                 (nghEutraFreqList->dynCfgInd == WR_CFG_MOD) ||
                 (nghEutraFreqList->dynCfgInd == WR_CFG_DEL))
              {
                 nodeFound = TRUE;
                 break;
              }
              prevEntry = (PTR) nghEutraFreqList;
          }
          if (TRUE == nodeFound)
          {
             if (nghEutraFreqList->dynCfgInd == WR_CFG_ADD)
             {
                nghEutraFreqList->dynCfgInd = WR_CFG_INV;
                idx = lteeNodeBparams.lteNeighFreqCfg.numEutraFreq;
                if (idx < MAX_NUM_FREQ_CFG)
                {
                   updateMsmNeiFreqParams(&(lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq),
                         &(nghEutraFreqList->nghEutraFreqCfg));
                   lteeNodeBparams.lteNeighFreqCfg.numEutraFreq += 1;
                }
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
                dynCfmInfo.action = MSM_DYN_ADD;
                dynCfmInfo.u.neighFreqIndx = nghEutraFreqList->nghEutraFreqCfg.freqIndex;

                msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
             }
             else if (nghEutraFreqList->dynCfgInd == WR_CFG_MOD)
             {
                nghEutraFreqList->dynCfgInd = WR_CFG_INV;
                for(idx = 1; idx < lteeNodeBparams.lteNeighFreqCfg.numEutraFreq; idx++)
                {
                   if (lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq.freqIndex ==
                         nghEutraFreqList->nghEutraFreqCfg.freqIndex)
                   {
                      updateMsmNeiFreqParams(&(lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq),
                            &(nghEutraFreqList->nghEutraFreqCfg));
                      break;
                   }
                }
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_MOD;
                dynCfmInfo.u.neighFreqIndx = nghEutraFreqList->nghEutraFreqCfg.freqIndex;

                msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
             }
             else if (nghEutraFreqList->dynCfgInd == WR_CFG_DEL)
             {
                for(idx = 1; idx < lteeNodeBparams.lteNeighFreqCfg.numEutraFreq; idx++)
                {
                   if (lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq.freqIndex ==
                         nghEutraFreqList->nghEutraFreqCfg.freqIndex)
                   {
                      lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq =
                      lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[(lteeNodeBparams.lteNeighFreqCfg.numEutraFreq - 1)].eutranFreq;
                      break;
                   }
                }
                lteeNodeBparams.lteNeighFreqCfg.numEutraFreq -= 1;
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_DEL;
                dynCfmInfo.u.neighFreqIndx = nghEutraFreqList->nghEutraFreqCfg.freqIndex;
                cmHashListDelete(&(smCfgCb.neighEutraFreqLstCp), (PTR)nghEutraFreqList);
                WR_FREE(nghEutraFreqList, sizeof(SmNeigEutraFreqList));
                msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
             }
          }
       }
       else if (LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG == dynCfgCfm->cfgType)
       {
          SmNeigEutraCellList *nghEutraCellList = NULLP; 
          SmNeigEnbList       *nghEnbCfgList = NULLP;
          PTR                  prevEntry = NULLP;
          U8                   nodeFound = FALSE;
          U8                   idx;
          while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp), prevEntry, (PTR *)&nghEutraCellList))
          {
              if((nghEutraCellList->dynCfgInd == WR_CFG_ADD) ||
                 (nghEutraCellList->dynCfgInd == WR_CFG_MOD) ||
                 (nghEutraCellList->dynCfgInd == WR_CFG_DEL))
              {
                 nodeFound = TRUE;
                 break;
              }
              prevEntry = (PTR) nghEutraCellList;
          }
          if (TRUE == nodeFound)
          {
             if (nghEutraCellList->dynCfgInd == WR_CFG_ADD)
             {
                nghEutraCellList->dynCfgInd = WR_CFG_INV;
                idx = lteeNodeBparams.lteNeighCellCfg.numEutraCells;
                if (idx < MSM_MAX_NUM_NEIGH_CELLS)
                {
                   updateMsmNeiCellParams(&(lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell),
                         &(nghEutraCellList->nghEutraCellCfg));
                   lteeNodeBparams.lteNeighCellCfg.numEutraCells += 1;
                }
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
                dynCfmInfo.action = MSM_DYN_ADD;
                dynCfmInfo.u.neighCellIndex = nghEutraCellList->nghEutraCellCfg.nrIndex;

                msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
             }
             else if (nghEutraCellList->dynCfgInd == WR_CFG_MOD)
             {
                nghEutraCellList->dynCfgInd = WR_CFG_INV;
                for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numEutraCells; idx++)
                {
                   if (lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell.nrIndex ==
                         nghEutraCellList->nghEutraCellCfg.nrIndex)
                   {
                      updateMsmNeiCellParams(&(lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell),
                            &(nghEutraCellList->nghEutraCellCfg));
                      break;
                   }
                }
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_MOD;
                dynCfmInfo.u.neighCellIndex = nghEutraCellList->nghEutraCellCfg.nrIndex;

                msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
             }
             else if (nghEutraCellList->dynCfgInd == WR_CFG_DEL)
             {
                for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numEutraCells; idx++)
                {
                   if (lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell.nrIndex ==
                         nghEutraCellList->nghEutraCellCfg.nrIndex)
                   {
                      lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell =
                         lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[lteeNodeBparams.lteNeighCellCfg.numEutraCells - 1].eutraCell;
                      lteeNodeBparams.lteNeighCellCfg.numEutraCells -= 1;
                      break;
                   }
                }
                
                if(ROK == cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&(nghEutraCellList->nghEutraCellCfg.enbId),
                         sizeof(nghEutraCellList->nghEutraCellCfg.enbId), 0,(PTR*) &nghEnbCfgList))
                {
                   nghEnbCfgList->numCell--;
                }
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_DEL;
                dynCfmInfo.u.neighCellIndex = nghEutraCellList->nghEutraCellCfg.nrIndex;
                cmHashListDelete(&(smCfgCb.neighEutraCellLstCp), (PTR) nghEutraCellList);
                WR_FREE(nghEutraCellList, sizeof(SmNeigEutraCellList));
                msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
             }
          }

       }
       else
       {
         /* Note: This has been added till OAM supports dynamic configuration of
          * MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS once SON updates the same to
          * OAM. As of now, we see that once SON updates, dynamic configuration
          * is not being triigered from OAM. This will be removed once OAM
          * provides the required functionality */
          if(TRUE != smCfgCb.sonTriggeredDynCfg)
          {
          msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
       }
          else
          {
             RLOG1(L_DEBUG, "Confirm Received for Dynamci Config from SON, value is %d",dynCfgCfm->cfgType); 
             smCfgCb.sonTriggeredDynCfg = FALSE;
          }
       }
       if(LWR_TRANSID_MME_DYN_DELCFG == dynCfmHdr->transId)
       {
         smCfgCb.noMmeDelCfm++;
         if(smCfgCb.noMmeDelCfm == smCfgCb.numMmeDel)
         {
           wrSmDynAddMme();
           smCfgCb.noMmeDelCfm = 0;
           smCfgCb.numMmeDel = 0;
         }
       }
       if(LWR_TRANSID_MME_DYN_ADDCFG == dynCfmHdr->transId)
       {
          smCfgCb.noMmeAddCfm++;
          if(smCfgCb.noMmeAddCfm == smCfgCb.numMmeAdded )
          {
             wrSmDynInitS1Setup();
             smCfgCb.noMmeAddCfm = 0;
             smCfgCb.numMmeAdded = 0;
          }
       }
       RLOG_ARG0(L_DEBUG, DBG_TRNSID,dynCfmHdr->transId,"Dynamic Configuration "
          "Confirm Success Received");
   }
   else
   {
       if (LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG == dynCfgCfm->cfgType)
       {
          SmNeigEutraFreqList *nghEutraFreqList;
          PTR                  prevEntry = NULLP;
          U8                   nodeFound = FALSE;
          U8                   idx;
          while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraFreqLstCp), prevEntry, (PTR *)&nghEutraFreqList))
          {
              if((nghEutraFreqList->dynCfgInd == WR_CFG_ADD) ||
                 (nghEutraFreqList->dynCfgInd == WR_CFG_MOD) ||
                 (nghEutraFreqList->dynCfgInd == WR_CFG_DEL))
              {
                 nodeFound = TRUE;
                 break;
              }
              prevEntry = (PTR) nghEutraFreqList;
          }
          if (TRUE == nodeFound)
          {
             if (nghEutraFreqList->dynCfgInd == WR_CFG_ADD)
             {
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
                dynCfmInfo.action = MSM_DYN_ADD;
                dynCfmInfo.u.neighFreqIndx = nghEutraFreqList->nghEutraFreqCfg.freqIndex;
                cmHashListDelete(&(smCfgCb.neighEutraFreqLstCp), (PTR)nghEutraFreqList);
                WR_FREE(nghEutraFreqList, sizeof(SmNeigEutraFreqList));
                msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
             }
             else if (nghEutraFreqList->dynCfgInd == WR_CFG_MOD)
             {
                nghEutraFreqList->dynCfgInd = WR_CFG_INV;
                for(idx = 1; idx < lteeNodeBparams.lteNeighFreqCfg.numEutraFreq; idx++)
                {
                   if (lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq.freqIndex ==
                         nghEutraFreqList->nghEutraFreqCfg.freqIndex)
                   {
                      updateSmNeiFreqParams(&(nghEutraFreqList->nghEutraFreqCfg),
                            &(lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq));
                      break;
                   }
                }

                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_MOD;
                dynCfmInfo.u.neighFreqIndx = nghEutraFreqList->nghEutraFreqCfg.freqIndex;

                msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
             }
             else if (nghEutraFreqList->dynCfgInd == WR_CFG_DEL)
             {
                nghEutraFreqList->dynCfgInd = WR_CFG_INV;

                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_DEL;
                dynCfmInfo.u.neighFreqIndx = nghEutraFreqList->nghEutraFreqCfg.freqIndex;

                msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
             }
          }
       }
       if (LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG == dynCfgCfm->cfgType)
       {
          SmNeigEutraCellList *nghEutraCellList = NULLP;
          PTR                  prevEntry = NULLP;
          U8                   nodeFound = FALSE;
          U8                   idx;
          while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp), prevEntry, (PTR *)&nghEutraCellList))
          {
              if((nghEutraCellList->dynCfgInd == WR_CFG_ADD) ||
                 (nghEutraCellList->dynCfgInd == WR_CFG_MOD) ||
                 (nghEutraCellList->dynCfgInd == WR_CFG_DEL))
              {
                 nodeFound = TRUE;
                 break;
              }
              prevEntry = (PTR)nghEutraCellList;
          }
          if (TRUE == nodeFound)
          {
             if (nghEutraCellList->dynCfgInd == WR_CFG_ADD)
             {
                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
                dynCfmInfo.action = MSM_DYN_ADD;
                dynCfmInfo.u.neighCellIndex = nghEutraCellList->nghEutraCellCfg.nrIndex;
                cmHashListDelete(&(smCfgCb.neighEutraCellLstCp), (PTR)nghEutraCellList);
                WR_FREE(nghEutraCellList, sizeof(SmNeigEutraCellList));
                msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
             }
             else if (nghEutraCellList->dynCfgInd == WR_CFG_MOD)
             {
                nghEutraCellList->dynCfgInd = WR_CFG_INV;
                for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numEutraCells; idx++)
                {
                   if (lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell.nrIndex ==
                         nghEutraCellList->nghEutraCellCfg.nrIndex)
                   {
                      updateSmNeiCellParams(&(nghEutraCellList->nghEutraCellCfg),
                            &(lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell));
                      break;
                   }
                }

                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_MOD;
                dynCfmInfo.u.neighCellIndex = nghEutraCellList->nghEutraCellCfg.nrIndex;

                msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
             }
             else if (nghEutraCellList->dynCfgInd == WR_CFG_DEL)
             {
                nghEutraCellList->dynCfgInd = WR_CFG_INV;

                dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
                dynCfmInfo.action = MSM_ACT_DEL;
                dynCfmInfo.u.neighCellIndex = nghEutraCellList->nghEutraCellCfg.nrIndex;

                msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
             }
          }
       }
       if(dynCfgCfm->cfm.status == LCM_PRIM_NOK)
       {
         /* Note: This has been added till OAM supports dynamic configuration of
          * MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS once SON updates the same to
          * OAM. As of now, we see that once SON updates, dynamic configuration
          * is not being triigered from OAM. This will be removed once OAM
          * provides the required functionality */
          if(TRUE != smCfgCb.sonTriggeredDynCfg)
          {
          /* Inform the Failure to the Messenger (through MSM), 
           * So that Messenger will resend the configuration */
          /*timer stop :: Raised an alarm*/ 
          smStopTmr((PTR)&(smCb),SM_TMR_DYN_CFG_TMR);
          smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
          smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
          smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
          smDynCfgFailAlarm(dynCfgCfm->cfgType);
       }
          else
          {
             RLOG0(L_INFO, "Received Negative Cfm for SON trigger dynamic Config");
             smCfgCb.sonTriggeredDynCfg = FALSE;
          }
       }

       RLOG3(L_DEBUG, "Dynamic Configuration Confirm Received for Transaction Id"
            "= %lu, Status = %d, Reason = %d", dynCfmHdr->transId,
            dynCfgCfm->cfm.status, dynCfgCfm->cfm.reason);
   }
   WR_FREE(dynCfgCfm, sizeof(LwrDynMngmt));

#endif
   RETVALUE(ROK);
}/* end of SmMiLwrDynCfgCfm */ 

/*
 *
 *       Fun:    SmMiLwrStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:  wr_smm_enbapp_rsys.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrStaInd
(
 Pst         *pst,
 LwrMngmt      *usta
 )
#else
PUBLIC S16 SmMiLwrStaInd(pst, usta)
   Pst         *pst;
   LwrMngmt      *usta;
#endif /* ANSI */
{
   Pst        wrPst;
   LwrUstaDgn *dgn;
   // S8         ret = 0;
#if 1
   U8   idx =0;
#endif /* 0 */
   U32        actualTmrVal;
   Bool       isTmrRunning = FALSE;

   /* wr002.102: Added trace macro */
   TRC2(SmMiLwrStaInd)

      SM_SET_ZERO(&wrPst, sizeof(Pst));

   dgn = &(usta->t.usta.dgn);
   if (usta->t.usta.alarm.event == LWR_EVENT_PHY_TX_UP_SUCCESS)
   {    
      smCb.smState = WR_SM_STATE_CELL_UP; 
   }
   if (usta->t.usta.alarm.event == LWR_EVENT_PHY_TX_DOWN_SUCCESS)
   {
#ifndef LTE_PAL_ENB
      if(smCb.smState == WR_SM_STATE_REBOOT_REQUIRED)
      {
         WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"\n SENDING STOP \
                  REQUEST to CL \n"));
         smBuildWrstopPHYCntrl();
      }
      else
      {
         smCb.smState = WR_SM_STATE_CELL_DOWN; 
      }
      RETVALUE(ROK);
   }

   if (usta->t.usta.alarm.event == LWR_EVENT_PHY_STOP_SUCCESS)
   {
#endif
      WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"\n SENDING SHUTDOWN \
               REQUEST to ALL LAYERS \n"));
      /* Raghu DIP changes for common code */

      WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"\n Re-Starting the Timer thread\n"));

      smCb.cellState = FALSE; /* We reached here means cell has been brought down successfully*/
      smCb.smState = WR_SM_STATE_DINIT;
      /*smCb.smState = WR_SM_STATE_RRM_SHUTDWN_DONE;*/
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR,EVTWRRRMSHUT, WR_POOL, SM_SELECTOR_LWLC);
      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
#ifdef DEBUGP
         WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, " Failure at Self Pst. %d\n", smCb.smState));
#endif
      }

   } 

   if (usta->t.usta.alarm.event == LWR_EVENT_CELL_RECFG_SUCCESS)
   {    
#ifndef LTE_PAL_ENB
      if(smCb.smState == WR_SM_STATE_REBOOT_REQUIRED)
      {
         RLOG0(L_DEBUG, "SENDING STOP REQUEST to CL");
         smBuildWrstopPHYCntrl();
         RETVALUE(ROK);
      }
      else
      {
         if(RFAILED == wrEmmMmeBrngCellPwrUp())
         {
            RLOG1(L_ERROR, "Failed to power up cell cell ID [%d]",
                  wrEmmCb.cellCb[idx]->cellId);
            RETVALUE(RFAILED);
         }
         smCb.isPciMod = FALSE;
#ifdef RM_INTF
         smSendCellUpIndToRrm();
#endif

      }
   }

   if (usta->t.usta.alarm.event == LWR_EVENT_PHY_STOP_SUCCESS)
   {
#endif
      RLOG0(L_ALWAYS, "SENDING SHUTDOWN REQUEST to ALL LAYERS ");
      /* Raghu DIP changes for common code */

      RLOG0(L_DEBUG, " Re-Starting the Timer thread ");
#if 0
      ret = SReInitTmr();
      if(ret != ROK)
      {
         RLOG0(L_ERROR, " Failed to Restart the Tmr ");
         RETVALUE(RFAILED);
      }
#endif

      smCb.cellState = FALSE; /* We reached here means cell has been brought down successfully*/
      {
         smCb.smState = WR_SM_STATE_DINIT;
         /*smCb.smState = WR_SM_STATE_RRM_SHUTDWN_DONE;*/
         WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR,EVTWRRRMSTKUNBND, WR_POOL, SM_SELECTOR_LWLC);
      }
      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
         RLOG0(L_ERROR, "Failure at Self Pst.");
      }

   } 
   if ( usta->t.usta.alarm.event == LWR_EVENT_S1CON_SET_SUCCESS)
   {
      /* CNM_DEV */
      /* Added for avoiding second time INIT Sync */
      if(smCb.syncState == FALSE)
      {
         smBuildWrSyncCntrl();
      }
   }
   /* CNM_DEV start */
   if (usta->t.usta.alarm.event == LWR_EVENT_SYNC_NBR_CELL_UNAVAILABLE)
   {
      RLOG0(L_ERROR, "Neighbor not found for Synchronization!!");
      if(smCb.syncState == TRUE)
      {
         /* intermediate sync loss */
         smCb.syncState = FALSE;
         if(RFAILED == wrEmmMmeBrngCellPwrUp())
         {
            RLOG1(L_ERROR, "Failed to power up cell cell ID [%d]",
                  wrEmmCb.cellCb[idx]->cellId);
            RETVALUE(RFAILED);
         }
         /* vrr : need to check */
         /* Setting syncState to True  
          * This is for case when no sync was there during 
          * Init and after that S1 goes down and comes back up again
          * in that case we have to send Power up for Cl
          */
         smCb.syncState = TRUE;

      }
      else
      {
         if (  smCb.reCfg == FALSE)
         {
            S16 ret = RFAILED;
            RLOG0(L_DEBUG, "QCom L1 RF OamClient Register and Create records \n");
            /* AdminState is not updated in lteeNodeBparams structure, 
             * explicitly adding it in global structure to set in Qualcomm
             * configuration file */
#ifdef WR_RSYS_OAM
            /* This state update is only to maintain the state in 
             * lteRsysQcomCfgParam. The same is hardcoded
             * inside to make it work for without_oam case
             */
            lteeNodeBparams.lteRsysQcomCfgParam.adminState = TRUE;
#endif
            ret = wrRegisterAndCreateRecords();
            if ( ret == RFAILED )
            {

               printf("\n Qualcomm L1 RF configuration failed !!!\n ");
               RLOG0(L_ERROR, "Qualcomm L1 RF configuration failed !!!");
               /* Raise alarm towards HeMS */
#ifdef WR_RSYS_OAM
               smStaticCfgFailAlarm();
#endif
            }
         }

         /* Initial sync failure */
         /* Added for avoiding second time cell configuration */
         if(smCb.cellState == FALSE)
         {
            smCb.smState   = WR_SM_STATE_AWAIT_CELL_UP;
            smBuildWrCellCntrl();
            /*vrr */
            /* Setting syncState to True  
             * This is for case when no sync was there during 
             * Init and after that S1 goes down and comes back up again
             * in that case we have to send Power up for Cl
             */
            smCb.syncState = TRUE;

         }
      }
   }
   if (usta->t.usta.alarm.event == LWR_EVENT_INIT_SYNC_SUCCESS)
   {
      if (  smCb.reCfg == FALSE)
      {
         S16 ret = RFAILED;
         RLOG0(L_DEBUG, "QCom L1 RF OamClient Register and Create records \n");
         /* AdminState is not updated in lteeNodeBparams structure, 
          * explicitly adding it in global structure to set in Qualcomm
          * configuration file */
#ifdef WR_RSYS_OAM
         /* This state update is only to maintain the state in 
          * lteRsysQcomCfgParam. The same is hardcoded
          * inside to make it work for without_oam case
          */
         lteeNodeBparams.lteRsysQcomCfgParam.adminState = TRUE;
#endif
         ret = wrRegisterAndCreateRecords();
         if ( ret == RFAILED )
         {

            printf("\n Qualcomm L1 RF configuration failed !!!\n ");
            RLOG0(L_ERROR, "Qualcomm L1 RF configuration failed !!!");
            /* Raise alarm towards HeMS */
#ifdef WR_RSYS_OAM
            smStaticCfgFailAlarm();
#endif
         }
      }

      smCb.syncState = TRUE;
      /* Added for avoiding second time cell configuration */
      if( smCb.cellState == FALSE)
      {
         smCb.smState   = WR_SM_STATE_AWAIT_CELL_UP;
         smBuildWrCellCntrl();
      }
      else
      {
         if(RFAILED == wrEmmMmeBrngCellPwrUp())
         {
            RLOG1(L_FATAL, "Failed to power up cell cell ID [%d]",
                  wrEmmCb.cellCb[idx]->cellId);
            RETVALUE(RFAILED);
         } 
      }
   }
   /* CNM_DEV end */
   if ((usta->t.usta.alarm.event == LWR_EVENT_CELL_CFG_SUCCESS)
         && smCb.smState == WR_SM_STATE_AWAIT_CELL_UP)
   {
      smCb.smState = WR_SM_STATE_CELL_UP;
      /*Added for avoiding second time cell configuration*/
      smCb.cellState = TRUE;
      smCb.reCfg = FALSE;
      /*ccpu00127802*/
      RLOG0(L_ALWAYS, "CELL CONFIGURATION SUCCESSFULLY PERFORMED.");
#if 1 
      /* All MMEs are down, no S1 Connection available with eNodeb */
      if(RFAILED == wrEmmMmeBrngCellPwrUp())
      {
         RLOG1(L_FATAL, "Failed to power up cell cell ID [%d]",
               wrEmmCb.cellCb[idx]->cellId);
         RETVALUE(RFAILED);
      }
#endif /* 1 */
      RLOG0(L_ALWAYS, "CELL CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef CNE_UE_SIM_TEST
      /* eNodeB_Ue_Sim: Send the S1 Setup Now. */
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRINITS1SETUP, WR_POOL, SM_SELECTOR_LWLC);
      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
         RLOG1(L_FATAL, "Failure at Self Pst. %d", smCb.smState);
      }
#else
      /* eNodeB_Ue_Sim: Send the X2 Setup Now. */
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRINITX2SETUP, WR_POOL, SM_SELECTOR_LWLC);
      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
         RLOG1(L_FATAL, "Failure at Self Pst. %d", smCb.smState);
      }

#endif
   } /* end of if statement */
   if ((usta->t.usta.alarm.event == LWR_EVENT_CELL_RECFG_SUCCESS)
         && smCb.smState == WR_SM_STATE_AWAIT_CELL_UP)
   {
      smCb.smState = WR_SM_STATE_CELL_UP;
      smCb.cellState = TRUE;
      smCb.reCfg = FALSE;
      RLOG0(L_ALWAYS, "CELL RECONFIGURATION SUCCESSFULLY PERFORMED.");

      /* Send the X2 Setup Now. */
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRINITX2SETUP, WR_POOL, SM_SELECTOR_LWLC);
      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
         RLOG1(L_ERROR, "Failure at Self Pst. %d", smCb.smState);
      }
   } /* end of if statement */
   /*else if (usta->t.usta.alarm.category == LCM_CATEGORY_INTERFACE)*/
   if (usta->t.usta.alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->t.usta.alarm.event == LCM_EVENT_BND_OK)
      {
         switch (usta->t.usta.alarm.cause)
         {
            case LWR_CAUSE_SZT_SAP_BOUND:
               {
                  if (smCb.smState == WR_SM_STATE_ENB_ALRMS_DONE)
                  {
                     smCb.smState = WR_SM_STATE_S1AP_STK_BND_DONE;
                     RLOG0(L_DEBUG, "BIND OF S1AP STACK SUCCESSFUL");
                     /* Perform the bind of APP and SCTP */
                     WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWREGTPSTKBND, WR_POOL, SM_SELECTOR_LWLC);
                     if(wrSmSelfPstMsg(&wrPst) != ROK)
                     {
                        RLOG1(L_FATAL, "Failure at Self Pst. %d", smCb.smState);
                     }
                  }
                  break;
               }
            case LWR_CAUSE_EGT_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF RRM TO EGTP IS SUCCESSFUL SAPID= %d",
                        dgn->u.sapId);
                  if (smCb.smState == WR_SM_STATE_S1AP_STK_BND_DONE)
                  {
                     smCb.smState = WR_SM_STATE_EGTP_STK_BND_DONE;
                     RLOG0(L_DEBUG, "BIND OF EGTP STACK SUCCESSFUL");
#ifdef LTE_HO_SUPPORT
                     /* Perform the bind of APP and SCTP */
                     WR_FILL_PST(wrPst, SM_WR_PROC,ENTSM, ENTWR, EVTWRX2APSTKBND, WR_POOL, SM_SELECTOR_LWLC);
#else
                     /* Perform the bind of APP and SCTP */
                     WR_FILL_PST(wrPst, SM_WR_PROC,ENTSM, ENTWR, EVTWRRRCSTKBND, WR_POOL, SM_SELECTOR_LWLC);
#endif
                     if(wrSmSelfPstMsg(&wrPst) != ROK)
                     {
                        RLOG1(L_FATAL, "Failure at Self Pst. %d", smCb.smState);
                     }
                  }
                  break;
               }

#ifdef LTE_HO_SUPPORT
            case LWR_CAUSE_CZT_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF RRM TO X2AP IS SUCCESSFUL SAPID= %d",
                        dgn->u.sapId);
                  if (smCb.smState == WR_SM_STATE_EGTP_STK_BND_DONE)
                  {
                     {
                        smCb.smState = WR_SM_STATE_X2AP_STK_BND_DONE;
                        RLOG0(L_DEBUG, "BIND OF X2AP STACK SUCCESSFUL");
                        /* Perform the bind of APP and SCTP */
                        WR_FILL_PST(wrPst, SM_WR_PROC,ENTSM, ENTWR, EVTWRRRCSTKBND, WR_POOL, SM_SELECTOR_LWLC);
                     }

                     if(wrSmSelfPstMsg(&wrPst) != ROK)
                     {
                        RLOG1(L_FATAL, "Failure at Self Pst. %d", smCb.smState);
                     }
                  }
                  break;  
               }  
#endif
            case LWR_CAUSE_CTF_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF APP CTF IS SUCCESSFUL sapId = %d",
                        dgn->u.sapId);
                  /* Initiate Bind Request towards NHU Sap  */
                  smBindUnBindWrToNhuSap(ABND_ENA);
                  break;
               }
            case LWR_CAUSE_NHU_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF APP NHU IS SUCCESSFUL sapId = %d",
                        dgn->u.sapId);
#if 0
                  smBindUnBindWrToNluSap(ABND_ENA);
                  break;
               }
            case LWR_CAUSE_NLU_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF NLU SAP IS SUCCESSFUL sapId = %d",
                        dgn->u.sapId);
                  /* Initiate Bind Request towards Pju Sap  */
#endif
                  smBindUnBindWrToPjuSap(WR_SM_PJUL_INST, ABND_ENA);
                  break;
               }
            case LWR_CAUSE_PJU_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF APP PJU IS SUCCESSFUL sapId = %d",
                        dgn->u.sapId);
#ifdef RM_INTF        /*RRM->FSM */
                  /* Initiate Bind Request towards RMU Sap  */
                  smBindUnBindWrToRmuSap(ABND_ENA);
#else
                  /* Initiate Bind Request towards RGR Sap  */
                  smBindUnBindWrToRgrSap(ABND_ENA);
#endif /*RM_INTF*/
                  break;
               }
#ifdef RM_INTF /*RRM->FSM */
            case LWR_CAUSE_RMU_SAP_BOUND: 
               {
                  RLOG1(L_DEBUG, "BIND OF APP RMU IS SUCCESSFUL sapId = %d",
                        dgn->u.sapId);
                  /* Initiate Bind Request towards RGR Sap  */
                  smBindUnBindWrToRgrSap(ABND_ENA);
                  break;
               }
#endif /*RM_INTF*/
            case LWR_CAUSE_RGR_SAP_BOUND:
               {
                  RLOG1(L_DEBUG, "BIND OF APP RGR IS SUCCESSFUL sapId = %d",
                        dgn->u.sapId);
#ifdef LTE_ENB_PERF
                  smCb.smState = WR_SM_STATE_EGTP_STK_BND_DONE;
#endif

#ifdef LTE_HO_SUPPORT
                  if (smCb.smState == WR_SM_STATE_X2AP_STK_BND_DONE)
#else
                     if (smCb.smState == WR_SM_STATE_EGTP_STK_BND_DONE)
#endif
                     {
                        smCb.smState = WR_SM_STATE_RRC_STK_BND_DONE;
                        RLOG0(L_DEBUG, "BIND OF RRC STACK IS SUCCESSFUL ");
                        /* Perform the bind of APP and SCTP */
#ifdef RM_INTF
                        WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRRRMSTKBND, WR_POOL, SM_SELECTOR_LWLC);
#else
                        WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRINITS1SETUP, WR_POOL, SM_SELECTOR_LWLC);
#endif
                        if(wrSmSelfPstMsg(&wrPst) != ROK)
                        {
                           RLOG1(L_FATAL, "Failure at Self Pst. %d", smCb.smState);
                        }
                     }
                  break;
               }
            default:
               {
                  RLOG1(L_ERROR, "Invalid alarm. %d", usta->t.usta.alarm.cause);
                  break;
               }
         }
      }
      else if (usta->t.usta.alarm.event == LCM_EVENT_BND_FAIL) 
      {
         RLOG1(L_ERROR, "BIND OF APP lsap id[%d] FAILED ", dgn->u.sapId);
      }
   }
   else if (usta->t.usta.alarm.category == LCM_CATEGORY_LYR_SPECIFIC)
   {
      switch(usta->t.usta.alarm.event)
      {
         case LWR_EVENT_ENB_CFGUPD_SUCCESS:
            RLOG0(L_DEBUG, "ENB Config Update Successfully Complete");
            /* TODO : Send indication to OAM */
            break;
         case LWR_EVENT_ENB_CFGUPD_FAIL:
            RLOG1(L_ERROR, "ENB Config Update Failed cause %d",usta->t.usta.alarm.cause);
            /* TODO : Send indication to OAM */
            break;
      }
   }

   else if (usta->t.usta.alarm.category == LCM_CATEGORY_PROTOCOL)
   {
      if (usta->t.usta.alarm.event == LWR_EVENT_CELL_UNAVAILABLE)
      {
         RLOG0(L_ALWAYS, "All S1 connectons are down, Hence Cell is brought down"
               " and Tx switched OFF. !!!");
#ifdef WR_RSYS_OAM
         /*indicate to OAM cell Tx is switch off */
         MsmOperationalState(FALSE);
#endif
         RLOG0(L_INFO, "[ALARM] Sending CELL_UNAVAILABLE to OAM");
      }
      else if (usta->t.usta.alarm.event == LWR_EVENT_CELL_AVAILABLE)
      {
         smCb.cellState = TRUE;
         smCb.reCfg = FALSE;
         RLOG0(L_ALWAYS, "1. CELL CONFIGURATION SUCCESSFULLY PERFORMED.");
         //        rlEnaBleBufferedIO();
         RLOG0(L_INFO, "Enabling Buffered IO");
         /* changes for CR ccpu00147678 */
#ifdef RM_INTF
         smSendCellUpIndToRrm();
#endif
#ifdef WR_RSYS_OAM
         RLOG2(L_DEBUG, "Static Cfg for LOG LEVEL at L3 [%d] & L2 [%d]", 
               lteeNodeBparams.lteLogLvlCfg.enbL3LogLvl,
               lteeNodeBparams.lteLogLvlCfg.enbL2LogLvl);
         rlSetLogLevel(lteeNodeBparams.lteLogLvlCfg.enbL3LogLvl);
         smYsSendRlogInd(lteeNodeBparams.lteLogLvlCfg.enbL2LogLvl);
         /*indicate to OAM cell Tx is switch on */
         MsmOperationalState(TRUE);
         WrSmState prevSmState = smCb.smState;
         smCb.smState = WR_SM_STATE_CELL_UP;
         if(prevSmState == WR_SM_STATE_AWAIT_CELL_UP)
         {
            /* Send the X2 Setup Now. */
            WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRINITX2SETUP, WR_POOL, SM_SELECTOR_LWLC);
            if(wrSmSelfPstMsg(&wrPst) != ROK)
            {
               RLOG1(L_ERROR, "Failure at Self Pst. %d", smCb.smState);
            }
         }
#else
         RLOG1(L_DEBUG, "Static Cfg for LOG LEVEL at L3 & L2 [%d] [without OAM]", wrSmDfltRlLogLvl);
         rlSetLogLevel(wrSmDfltRlLogLvl);
         smYsSendRlogInd(wrSmDfltRlLogLvl);
         smCb.smState = WR_SM_STATE_CELL_UP;
         smCb.cellState = TRUE;
#endif
         RLOG0(L_INFO, "[ALARM] Sending CELL_AVAILABLE to OAM");
         rlStartLogCountLimit();
         /* Start the ANR cell deletion timer */
         if (wrSmDfltAnrCelValidAge >= 10)
         {
            actualTmrVal = 10;
         }
         else
         {
            actualTmrVal = wrSmDfltAnrCelValidAge;
         }

         isTmrRunning = wrIsTmrRunning(&wrEmmCb.cellCb[idx]->nrCb->cellValidAgeTmr,
               WR_TMR_ANR_CELL_MAX_AGE_TMR);
         if (isTmrRunning == FALSE)
         {
            wrStartTmr((PTR)wrEmmCb.cellCb[idx], 
                  WR_TMR_ANR_CELL_MAX_AGE_TMR,actualTmrVal*60*1000);
            RLOG1(L_INFO, 
                  "CELL UP : Started Cell valid age timer with value = %u",
                  actualTmrVal);
         }
      }
      if(usta->t.usta.alarm.event == LWR_EVENT_SEND_ALARM)
      {
#ifdef WR_RSYS_OAM
         FapAlarmInd(dgn->u.alarm.alarmId, dgn->u.alarm.severity, dgn->u.alarm.causeType, dgn->u.alarm.causeVal);
#endif
      }
   }
   /*Fix for ccpu00126143: Handling Alram raised when X2 setup Req fails*/
   else if ((usta->t.usta.alarm.cause == LWR_EVENT_CELL_UNAVAILABLE) &&
         (usta->t.usta.alarm.event == LWR_EVENT_ERRIND_FROM_PEER))
   {
      /* Raise a Alram towards OAM when support is available */
      RLOG0(L_ERROR,"X2-Setup Request Failed because of mismatch in enodeb ID");
   }

   RETVALUE(ROK);
} /* end of SmMiLwrStaInd() */

/****************************************************************************
 * layer management interface functions
 ****************************************************************************/

/*
 *
 *       Fun:   Configuration request
 *
 *       Desc:  Twrs function is used to configure  TUCL
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:     wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrCfgReq
(
 Pst *spst,                /* post structure */
 LwrMngmt *cfg              /* configure */
 )
#else
PUBLIC S16 SmMiLwrCfgReq(spst, cfg)
   Pst *spst;                /* post structure */   
   LwrMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLwrCfgReq)
      /* jump to specific primitive depending on configured selector */
      (*SmMiLwrCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLwrCfgReq */

/*
 *
 *       Fun:   Dynamic Configuration request
 *
 *       Desc:  This function is used to configure enbapp dynamically
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:     wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmMiLwrDynCfgReq
(
 Pst    *spst,                /* post structure */
 LwrDynMngmt *cfg 
 )
#else
PUBLIC S16 SmMiLwrDynCfgReq(spst, cfg)
   Pst    *spst;                /* post structure */   
   LwrDynMngmt *cfg;
#endif
{
   TRC3(SmMiLwrCfgReq)
      /* jump to specific primitive depending on configured selector */
      (*SmMiLwrDynCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLwrDynCfgReq */ 



/*
 *
 *       Fun:   Control request
 *
 *       Desc:  This function is used to send control request to 
 *              TUCL
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:     wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrCntrlReq
(
 Pst *spst,                 /* post structure */
 LwrMngmt *cntrl            /* control */
 )
#else
PUBLIC S16 SmMiLwrCntrlReq(spst, cntrl)
   Pst *spst;                 /* post structure */   
   LwrMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLwrCntrlReq)
      /* jump to specific primitive depending on configured selector */
      (*SmMiLwrCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLwrCntrlReq */

/*
 *
 *       Fun:    SmMiLwrLoadInd - Load indication
 *
 *       Desc:   Send the load Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_enbapp.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrLoadInd
(
 Pst         *spst,
 LwrLoadIndInfo  *loadIndInfo
 )
#else
PUBLIC S16 SmMiLwrLoadInd(spst, loadIndInfo)
   Pst         *spst;
   LwrLoadIndInfo      *loadIndInfo;
#endif /* ANSI */
{

   /* wr002.102: Added trace macro */
   TRC2(SmMiLwrLoadInd)

      /* jump to specific primitive depending on configured selector */
      (*SmMiLwrLoadIndMt[spst->selector])(spst, loadIndInfo);

   RETVALUE(ROK);
} /* end of SmMiLwrLoadInd() */ 


/*
 *
 *       Fun:   Configuration request
 *
 *       Desc:  Twrs function is used to configure  TUCL
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:     wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrAddNeighCfm
(
 Pst *spst,                /* post structure */
 LwrMngmt *cfg              /* configure */
 )
#else
PUBLIC S16 SmMiLwrAddNeighCfm(spst, cfg)
   Pst *spst;                /* post structure */   
   LwrMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLwrAddNeighCfm)
      /* jump to specific primitive depending on configured selector */
      (*SmMiLwrAddNeighCfmMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLwrAddNeighCfm*/ 



/*****************************************************************************
 * Portable function definitions
 *****************************************************************************/

#ifdef PTSMMILWR  

/*
 *
 *       Fun:   Portable configure Request- TUCL
 *
 *       Desc:  
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:     wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLwrAddNeighCfm
(
 Pst *spst,                  /* post structure */
 LwrMngmt *cfg                /* configure */
 )
#else
PRIVATE S16 PtMiLwrAddNeighCfm(spst, cfg)
   Pst *spst;                  /* post structure */
   LwrMngmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLwrAddNeighCfm) 

      UNUSED(spst);
   UNUSED(cfg);

   RETVALUE(ROK);
} /* end of PtMiLwrAddNeighCfm*/ 

/*
 *
 *       Fun:   Portable configure Request
 *
 *       Desc:  
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:  wr_smm_enbapp.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLwrLoadInd
(
 Pst         *spst,
 LwrLoadIndInfo      *loadIndInfo
 )
#else
PRIVATE S16 PtMiLwrLoadInd(spst, loadIndInfo)
   Pst *spst;                  /* post structure */
   LwrLoadIndInfo      *loadIndInfo;
#endif
{
   TRC3(PtMiLwrLoadInd) 

      UNUSED(spst);
   UNUSED(loadIndInfo);

   RETVALUE(ROK);
} /* end of PtMiLwrLoadInd */


/*
 *
 *       Fun:   Portable configure Request- 
 *
 *       Desc:  
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:     wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLwrCfgReq
(
 Pst *spst,                  /* post structure */
 LwrMngmt *cfg                /* configure */
 )
#else
PRIVATE S16 PtMiLwrCfgReq(spst, cfg)
   Pst *spst;                  /* post structure */
   LwrMngmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLwrCfgReq) 

      UNUSED(spst);
   UNUSED(cfg);

   RETVALUE(ROK);
} /* end of PtMiLwrCfgReq */



/*
 *
 *       Fun:   Portable configure Request
 *
 *       Desc:  
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *              
 *       File:     wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLwrDynCfgReq
(
 Pst    *pst,                /* post structure */
 LwrDynMngmt *dynCfg
 )
#else
PRIVATE S16 PtMiLwrDynCfgReq(pst, dynCfg)
   Pst    *pst;                /* post structure */
   LwrDynMngmt dynCfg;
#endif
{
   TRC3(PtMiLwrCfgReq) 

      UNUSED(pst);
   UNUSED(dynCfg);
   RETVALUE(ROK);
} /* end of PtMiLwrDynCfgReq */ 

/*
 *
 *       Fun:   Portable control Request TUCL
 *
 *       Desc:  
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:     wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLwrCntrlReq
(
 Pst *spst,                  /* post structure */
 LwrMngmt *cntrl              /* control */
 )
#else
PRIVATE S16 PtMiLwrCntrlReq(spst, cntrl)
   Pst *spst;                  /* post structure */
   LwrMngmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLwrCntrlReq)

      UNUSED(spst);
   UNUSED(cntrl);

   RETVALUE(ROK);
} /* end of PtMiLwrCntrlReq */

#endif /* PTSMMILWR */ 

#ifndef LTE_PAL_ENB
/*
 *
 *       Fun: smBuildWrstopPHYCntrl
 *
 *       Desc: This function will send the stop request to CL 
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:     wr_smm_enbapp_rsys.c
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildWrstopPHYCntrl(Void)
#else
PRIVATE S16 smBuildWrstopPHYCntrl(Void)
#endif
{
   YsMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2()

      SGetSBuf(0, 0, (Data **)&cntrl, sizeof(YsMngmt));
   SM_SET_ZERO(cntrl, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl->t.cntrl.action        =  ADISIMM;
   cntrl->t.cntrl.s.cellId =  WRSM_CELLID;
   cntrl->t.cntrl.subAction = SASTOPL1; 
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTTF;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTTF;
   pst.dstProcId = SM_YS_PROC;
   pst.srcProcId = SM_SM_PROC;
   RLOG0(L_DEBUG, "Sending Gen Cntrl Req to ys");

   /* Send the request to the CL */
   SmMiLysCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
}/*smBuildWrstopPHYCntrl*/
#endif
/*
 *
*       Fun:   smBldAndSendNghCellInd
 *
*       Desc:  Involed to modify CellCb with LteAdvanced Features
 *
*       Ret:   ROK
 *
*       Notes: None
 *
*       File: wr_smm_enbapp.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBldAndSendNghCellInd
(
LwrNghCellInd *nghCellInd
)
#else
PUBLIC S16 smBldAndSendNghCellInd(
LwrNghCellInd *nghCellInd;

)
#endif
{
   Pst               pst = {0};
   
    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTWR;
   pst.dstEnt    = ENTSM;
   pst.dstProcId = SM_SM_PROC;
   pst.srcProcId = SM_WR_PROC;
   pst.region = smCb.init.region;
   pst.pool = smCb.init.pool;

   RETVALUE(SmMiLwrNghInd(&pst,nghCellInd));
}   


/*
 *
*       Fun:   Neighbour Cell indication
 *
*       Desc:  This function is used to send neigbour cell(added/modified/deleted) details to OAM
 *
 *       Ret:   None
 *
*       Notes: None
 *
*       File:     wr_smm_enbapp_rsys.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmMiLwrNghInd
(
Pst    *spst,                
LwrNghCellInd *cfg 
)
#else
PUBLIC S16 SmMiLwrNghInd(spst, cfg)
Pst    *spst;                   
LwrNghCellInd *cfg;
#endif
{
   TRC3(SmMiLwrNghInd)
   if( SmMiLwrNghCellInd[spst->selector] != NULLP)
   {
      (*SmMiLwrNghCellInd[spst->selector])(spst, cfg); 
   }
   else
   {
      RLOG0(L_ERROR,"Failed to sent neighbour cell indication");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/*
*
*       Fun:   smSndWrPciUpdateReq
*
*       Desc:  Call handler for sending PCI update to eNB-Application
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*              
*       File:  wr_sm_enbapp_rsys.c
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrPciUpdateReq
(
NlUpdatePciCfgInd   *pciUpdate
)
#else
PUBLIC S16 smSndWrPciUpdateReq(pciUpdate)
{
NlUpdatePciCfgInd   *pciUpdate;
}
#endif
{
   LwrMngmt          *lwrMng = NULLP;
   Pst      pst;
   LwrCellCfg        *cellCfg = NULLP;
   TRC3 (smSndWrPciUpdateReq);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   cellCfg = &(lwrMng->t.protoCfg.t.cellCfg);
   cellCfg->action = WR_ACT_MOD;

   cellCfg->t.modCell.cellId = pciUpdate->cellId;
   cellCfg->t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   cellCfg->t.modCell.reCfgType = LWR_CELL_CFG_PHY_PCI_UPDATE;
   cellCfg->t.modCell.physCellId = pciUpdate->pci;

   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_CELLCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSndWrSchdPrbPaUpdateReq
*
*       Desc:  Call handler for sending PRB and PA configuration
*              to eNB-Application
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*              
*       File:  wr_sm_enbapp_rsys.c
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrSchdPrbPaUpdateReq
(
LnlSonCfg   *sonCfg
)
#else
PUBLIC S16 smSndWrSchdPrbPaUpdateReq(sonCfg)
{
LnlSonCfg   *sonCfg;
}
#endif
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   LwrCellCfg        *cellCfg = NULLP;
   LwrAddCellCfg     *modCell = NULLP;
   LwrSonSchdCfg     *schdCfg = NULLP;
   U32               count;
   TRC3 (smSndWrSchdPrbPaUpdateReq);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   cellCfg = &(lwrMng->t.protoCfg.t.cellCfg);
   cellCfg->action = WR_ACT_MOD;
   modCell = &(cellCfg->t.modCell);
   modCell->cellId = sonCfg->ecgi.cellId;
   modCell->cellIndex = WR_DFLT_CELL_IDX;
   modCell->reCfgType = LWR_CELL_SON_UPDATE;
   modCell->sonCfg.cfgType = LNL_SON_SCHD_CFG;
   schdCfg = &(modCell->sonCfg.u.schdCfg);
   schdCfg->numUeCfg = sonCfg->u.schdCfg.numUeCfg;
   for(count = 0; count < schdCfg->numUeCfg; count ++)
   {
      schdCfg->ueCfg[count].crnti = sonCfg->u.schdCfg.ueCfg[count].crnti;
      schdCfg->ueCfg[count].isCellEdge = sonCfg->u.schdCfg.ueCfg[count].isCellEdge;
   }
   for(count  = 0; count < LNL_MAX_PRB_CNT; count ++)
   {
      schdCfg->paLevelPerPrb[count] = sonCfg->u.schdCfg.paLevelPerPrb[count];
   }
   schdCfg->pa = sonCfg->u.schdCfg.pa;
   schdCfg->paLow = sonCfg->u.schdCfg.paLow;
   schdCfg->paHigh = sonCfg->u.schdCfg.paHigh;
   schdCfg->pb = sonCfg->u.schdCfg.pb;

   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.transId             = LWR_TRANSID_CELLCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;
   lwrMng->hdr.response.mem.region = WR_MEM_REGION;
   lwrMng->hdr.response.mem.pool   = WR_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:     fc_smm_enbapp_rsys_brcm.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:52:10 2014

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
 *********************************************************************91*/
