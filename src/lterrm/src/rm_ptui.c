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

     Desc:     This file contains 
               

     File:     rm_ptui.c

     Sid:      qf_ptui.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:35 2013

     Prg:     jbm 

*********************************************************************21*/
/*
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=138;
*/
/* header include files */
#include "envopt.h"        /* environment options         --*/
#include "envdep.h"        /* environment dependent       --*/
#include "envind.h"        /* environment independent     --*/
#include "gen.h"           /* general                     --*/
#include "ssi.h"           /* system services             --*/
#include "cm5.h"           /* common timer library        --*/
#include "cm_hash.h"       /* common hash library         --*/
#include "cm_llist.h"      /* common linked list library  --*/
#include "cm_inet.h"       /* common socket library       --*/
#include "cm_tpt.h"        /* common transport library    --*/
#include "cm_tkns.h"       /* common tokens               --*/
#include "cm_mblk.h"       /* Common memory allocation      */
#include "cm_inet.h"       /* Common socket library         */

#include "rmu.h"           /* RMU interface structures      */
#include "gen.x"           /* general                     --*/
#include "cm_lte.x"
#include "cm_lte_rrm.x"
#include "rmu.x"           /* RMU interface structures      */

#include "rm_cmn.h"
#include "rm_application.h"

#define WR_MAX_UIRMU_SEL   4 

/************************************************************************
                              Prototypes
************************************************************************/


PUBLIC RmuUeAdmitRspFPtr rmLiRmuUeAdmitRspMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeAdmitRsp,        /* 0 - loosely coupled */
#else
   PtPkRmuUeAdmitRsp,
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeAdmitRsp,    /* 1 - LWLC */
#else
   PtPkRmuUeAdmitRsp,
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeAdmitRsp,        /* 2 - tightly coupled */
#else
   PtPkRmuUeAdmitRsp,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeAdmitRsp       /* 3 - NPLC */
#else
   PtPkRmuUeAdmitRsp
#endif /*RMU_NPLC*/
};


PUBLIC RmuUeHoRspFPtr rmLiRmuUeHoRspMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeHoRsp,           /* 0 - loosely coupled */
#else
   PtPkRmuUeHoRsp,   
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeHoRsp,       /* 1 - LWLC */
#else
   PtPkRmuUeHoRsp,   
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeHoRsp,            /* 2 - tightly coupled */
#else
   PtPkRmuUeHoRsp,   
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeHoRsp         /* 2 - NPLC */
#else
   PtPkRmuUeHoRsp   
#endif /*RMU_NPLC*/
};


PUBLIC RmuUeRelRspFPtr rmLiRmuUeRelRspMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeRelRsp,          /* 0 - loosely coupled */
#else
   PtPkRmuUeRelRsp,  
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeRelRsp,      /* 1 - LWLC */
#else
   PtPkRmuUeRelRsp,  
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeRelRsp,           /* 3 - tightly coupled */
#else
   PtPkRmuUeRelRsp, 
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeRelRsp        /* 2 - NPLC */
#else
   PtPkRmuUeRelRsp  
#endif /*RMU_NPLC*/
};


PUBLIC RmuUeRecfgRspFPtr rmLiRmuUeRecfgRspMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeRecfgRsp,        /* 0 - loosely coupled */
#else
   PtPkRmuUeRecfgRsp,
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeRecfgRsp,    /* 1 - LWLC */
#else
   PtPkRmuUeRecfgRsp,
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeRecfgRsp,        /* 3 - tightly coupled */
#else
   PtPkRmuUeRecfgRsp,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeRecfgRsp      /* 2 - NPLC */
#else
   PtPkRmuUeRecfgRsp
#endif /*RMU_NPLC*/
};


PUBLIC RmuUeERabRelIndFPtr rmLiRmuUeERabRelIndMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeERabRelInd,          /* 0 - loosely coupled */
#else
   PtPkRmuUeERabRelInd,  
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeERabRelInd,      /* 1 - LWLC */
#else
   PtPkRmuUeERabRelInd,  
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeERabRelInd,           /* 3 - tightly coupled */
#else
   PtPkRmuUeERabRelInd,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeERabRelInd        /* 2 - NPLC */
#else
   PtPkRmuUeERabRelInd 
#endif /*RMU_NPLC*/
};


PUBLIC RmuUeRelIndFPtr rmLiRmuUeRelIndMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeRelInd,          /* 0 - loosely coupled */
#else
   PtPkRmuUeRelInd,  
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeRelInd,      /* 1 - LWLC */
#else
   PtPkRmuUeRelInd,  
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeRelInd,           /* 3 - tightly coupled */
#else
   PtPkRmuUeRelInd,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeRelInd        /* 2 - NPLC */
#else
   PtPkRmuUeRelInd 
#endif /*RMU_NPLC*/
};

PUBLIC RmuCellRecfgIndFPtr rmLiRmuCellRecfgIndMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuCellRecfgInd,          /* 0 - loosely coupled */
#else
   PtPkRmuCellRecfgInd,
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuCellRecfgInd,      /* 1 - LWLC */
#else
   PtPkRmuCellRecfgInd,
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuCellRecfgInd,           /* 3 - tightly coupled */
#else
   PtPkRmuCellRecfgInd,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuCellRecfgInd        /* 2 - NPLC */
#else
   PtPkRmuCellRecfgInd
#endif /*RMU_NPLC*/
};

PUBLIC RmuUeRecfgIndFPtr rmLiRmuUeRecfgIndMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuUeRecfgInd,          /* 0 - loosely coupled */
#else
   PtPkRmuUeRecfgInd,
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeRecfgInd,      /* 1 - LWLC */
#else
   PtPkRmuUeRecfgInd,
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuUeRecfgInd,           /* 2 - tightly coupled */
#else
   PtPkRmuUeRecfgInd,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuUeRecfgInd        /* 3 - NPLC */
#else
   PtPkRmuUeRecfgInd
#endif /*RMU_NPLC*/
};

PUBLIC RmuBndCfmFPtr rmLiRmuBndCfmMt[WR_MAX_UIRMU_SEL] =
{
#ifdef RMU_LC
   cmPkRmuBndCfm,        /* 0 - loosely coupled */
#else
   PtPkRmuBndCfm,
#endif /*RMU_LC */
#ifdef RMU_LWLC
   cmPkLwLcRmuBndCfm,    /* 1 - LWLC */
#else
   PtPkRmuBndCfm,
#endif /*RMU_LWLC*/
#ifdef RM_INTF
   WrLiRmuBndCfm,         /* 3 - tightly coupled */
#else
   PtPkRmuBndCfm,
#endif /*RMU_INTF */
#ifdef RMU_NPLC
   cmNoPkRmuBndCfm      /* 2 - NPLC */
#else
   PtPkRmuBndCfm
#endif /*RMU_NPLC*/
};



/************************************************************************
                       Upper Interface Functions
************************************************************************/

/*
 *
 *       Fun:   RmUiRmuUeAdmitRsp
 *
 *       Desc:  This function resolves the WR_RM_UE_ADMIT_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeAdmitRsp
(
Pst *pst,
SuId suId,
RmuUeAdmitRsp *ueAdmitRsp
)
{
   TRC3(RmUiRmuUeAdmitRsp)

   (*rmLiRmuUeAdmitRspMt[pst->selector])
      (pst, suId, ueAdmitRsp);

   RETVALUE(ROK);
} /* RmUiRmuUeAdmitRsp */

/*
 *
 *       Fun:   RmUiRmuUeHoRsp
 *
 *       Desc:  This function resolves the WR_RM_UE_HO_REQ  primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeHoRsp
(
Pst *pst,
SuId suId,
RmuUeHoRsp *ueHoRsp
)
{
   TRC3(RmUiRmuUeHoRsp)

   (*rmLiRmuUeHoRspMt[pst->selector])
      (pst, suId, ueHoRsp);

   RETVALUE(ROK);
} /* RmUiRmuUeHoRsp */



/*
 *
 *       Fun:   RmUiRmuUeRelRsp
 *
 *       Desc:  This function resolves the WR_RM_UE_REL_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeRelRsp
(
Pst *pst,
SuId suId,
RmuUeRelRsp *ueRelRsp
)
{
   TRC3(RmUiRmuUeRelRsp)

   (*rmLiRmuUeRelRspMt[pst->selector])
      (pst, suId, ueRelRsp);

   RETVALUE(ROK);
} /* RmUiWrtUeRelRsp */

/*
 *
 *       Fun:   RmUiRmuUeRecfgRsp
 *
 *       Desc:  This function resolves the WR_RM_UE_RECONFIG_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeRecfgRsp
(
Pst *pst,
SuId suId,
RmuUeRecfgRsp *ueRecfgRsp
)
{
   TRC3(RmUiRmuUeRecfgRsp)

   (*rmLiRmuUeRecfgRspMt[pst->selector])
      (pst, suId, ueRecfgRsp);

   RETVALUE(ROK);
} /* rmLiRmuUeRecfgRsp */

/*
 *
 *       Fun:   RmUiRmuUeRelInd
 *
 *       Desc:  This function resolves the WR_RM_UE_REL_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeRelInd
(
Pst *pst,
SuId suId,
RmuUeRelInd *ueRelInd
)
{
   TRC3(RmUiRmuUeRelInd)

   (*rmLiRmuUeRelIndMt[pst->selector])
      (pst, suId, ueRelInd);

   RETVALUE(ROK);
} /* RmUiWrtUeRelInd */

/*
 *
 *       Fun:   RmUiRmuUeERabRelInd
 *
 *       Desc:  This function resolves the UE RAB REL IND primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeERabRelInd
(
Pst *pst,
SuId suId,
RmuUeERabRelInd *ueRabRelInd
)
{
   TRC3(RmUiRmuUeERabRelInd)

   (*rmLiRmuUeERabRelIndMt[pst->selector])
      (pst, suId, ueRabRelInd);

   RETVALUE(ROK);
} /* RmUiWrtUeERabRelInd */


/*
 *
 *       Fun:   RmUiRmuCellRecfgInd
 *
 *       Desc:  This function sends the Cell Recfg Indication
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuCellRecfgInd
(
Pst *pst,
SuId suId,
RmuCellRecfgInd *pstCellRecfgInfo
)
{
   TRC3(RmUiRmuCellRecfgInd)

   (*rmLiRmuCellRecfgIndMt[pst->selector])
      (pst, suId, pstCellRecfgInfo);

   RETVALUE(ROK);
} /* RmUiRmuCellRecfgInd */

/*
 *
 *       Fun:  RmUiRmuUeRecfgInd 
 *
 *       Desc:  This function sends the Cell Recfg Indication
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  rm_ptui.c
 *
 */
PUBLIC S16 RmUiRmuUeRecfgInd
(
Pst *pst,
SuId suId,
RmuUeRecfgInd *pstUeRecfgInfo
)
{
   TRC3(RmUiRmuUeRecfgInd)

   (*rmLiRmuUeRecfgIndMt[pst->selector])
      (pst, suId, pstUeRecfgInfo);

   RETVALUE(ROK);
} /* RmUiRmuUeRecfgInd */
/*
 *
 *       Fun:   RmUiRmuBndCfm
 *
 *       Desc:  This function resolves the BNDREQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
PUBLIC S16 RmUiRmuBndCfm
(
Pst *pst,
SuId suId,
U8   status
)
{
   TRC3(RmUiRmuBndCfm)

   (*rmLiRmuBndCfmMt[pst->selector])
      (pst, suId, status);

   RETVALUE(ROK);
} /* rmLiRmuUeRecfgCfm */


// Handling Incoming messages at RMU interface from FSM

S16 RmUiRmuMmeOvldStartInd
(
		Pst *pst,
		SuId suId,
		RmuMmeOvldStartInd *mmeOvldStartInd
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8 *)mmeOvldStartInd);
	RETVALUE(ROK);
}

S16 RmUiRmuMmeOvldStopInd
(
		Pst *pst,
		SuId suId,
		RmuMmeOvldStopInd *mmeOvldStopInd
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)mmeOvldStopInd);
   	RETVALUE(ROK);
}

S16 RmUiRmuUeAdmitReq
(
		Pst *pst,
		SuId suId,
		RmuUeAdmitReq *ueAdmitReq
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)ueAdmitReq);
   	RETVALUE(ROK);
}

S16 RmUiRmuUeHoReq
(
		Pst *pst,
		SuId suId,
		RmuUeHoReq *ueHoReq
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)ueHoReq);
   	RETVALUE(ROK);
}

/*RRM_SP3*/
S16 RmUiRmuUeInactInd
(
Pst *pst,
SuId suId,
RmuUeInactInd  *ueInactInd
)
{
   g_rObjRRMApp.processFsmMsg(pst->event, (U8*)ueInactInd);
   RETVALUE(ROK);
}

S16 RmUiRmuUeRelReq
(
		Pst *pst,
		SuId suId,
		RmuUeRelReq *ueRelReq
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)ueRelReq);
	RETVALUE(ROK);
}

S16 RmUiRmuUeRecfgReq
(
		Pst *pst,
		SuId suId,
		RmuUeRecfgReq  *ueRecfgReq
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)ueRecfgReq);
   	RETVALUE(ROK);
}

S16 RmUiRmuFsmStatusInd
(
		Pst *pst,
		SuId suId,
		RmuFsmStatusInd *fsmStaInd
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)fsmStaInd);
   	RETVALUE(ROK);
}



#ifdef RRM_UNUSED_FUNC
S16 RmUiLrmEnbStaInd
(
		Pst *pst,
		SuId suId,
		LrmEnbStaInd *enbStaInd
)
{
	return g_rObjRRMApp.processSmMsg(pst->event, (U8*)enbStaInd);
}
#endif

S16 RmUiRmuUeSpsDisInd
(
		Pst *pst,
		SuId suId,
      RmuUeSpsDisInd *ueSpsDisInd
)
{
	g_rObjRRMApp.processFsmMsg(pst->event, (U8*)ueSpsDisInd);
   	RETVALUE(ROK);
}

/**

 * @brief API for bind request from FSM towards RRM.
 *
 * @details
 *
 *     Function: RmUiRmuBndReq
 *
 *     This API is invoked by FSM towards RRM to bind RMU SAP.
 *     These API validates the Pst, spId, suId and sends the bind confirm to FSM.
 *
 *
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
PUBLIC S16 RmUiRmuBndReq
(
	Pst   	*pst,
	SuId  	suId,
	SpId  	spId
)
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
 //  RmUstaDgn dgn;      /* Alarm diagnostics structure */
   U8        status = CM_BND_OK; /* Bind Status */

   TRC3(RmUiRmuBndReq)

   //RMDBGPRM((rmPBuf,"RmUiRmuBndReq(): suId = %d; spId = %d;", suId, spId));

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = g_stRmCb.rmInit.region;
   tmpPst.pool        = g_stRmCb.rmInit.pool;
   tmpPst.srcProcId   = g_stRmCb.rmInit.procId;
   tmpPst.srcEnt      = g_stRmCb.rmInit.ent;
   tmpPst.srcInst     = g_stRmCb.rmInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;


   if(spId == g_stRmCb.rmuSap.spId)
   {
      /* Check the state of the SAP */
      switch (g_stRmCb.rmuSap.sapState)
      {
         case LRM_NOT_CFG: /* SAP Not configured */
           // RMDBGINFO((RM_PRNT_BUF,"SAP Not Configured\n"));
            status = CM_BND_NOK;
            break;
         case LRM_SAP_UNBOUND: /* SAP is not bound */
          //  RMDBGINFO((RM_PRNT_BUF,"SAP Not yet bound\n"));
            g_stRmCb.rmuSap.sapState = LRM_SAP_BOUND;
            g_stRmCb.rmuSap.suId = suId;
            break;
         case LRM_SAP_BOUND: /* SAP is already bound*/
           // RMDBGINFO((RM_PRNT_BUF,"SAP Is already Bound\n"));
            break;
         default: /* Should Never Enter here */
#if (EFSMLASS & EFSMLS_ADD_RES)
            RMLOGERROR(EFSMLS_INT_PAR, ERM022, (ErrVal)g_stRmCb.rmuSap.sapSta.sapState,
                  "Invalid SAP State:RmUiRmuBndReq failed\n");
#endif
            status = CM_BND_NOK;
            break;
      }
   }
   else
   {
#if (EFSMLASS & EFSMLS_ADD_RES)
      RMLOGERROR(EFSMLS_INT_PAR, ERM023, (ErrVal)g_stRmCb.rmuSap.sapCfg.spId,
            "Invalid SAP Id:RmUiRmuBndReq failed\n");
#endif
      status = CM_BND_NOK;
   }

   /* Send Bind Confirm with status as set above */
   ret = RmUiRmuBndCfm(&tmpPst, g_stRmCb.rmuSap.suId, status);

   RETVALUE(ret);
}  /* RmUiRmuBndReq */


/********************************************************************30**

           End of file:     qf_ptui.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:35 2013

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
