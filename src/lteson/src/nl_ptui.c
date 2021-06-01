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
               

     File:     nl_ptui.c

     Sid:      bk_ptui.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:32 2014

     Prg:      subhamay 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=254;
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
#include "rl_interface.h"
#include "rl_common.h"     /* RLOG defines*/
#include "rgr.h"
#include "nlu.h"
#include "lnl.h"           /* NL Interface defines */
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
#include "rgr.x"
#include "lnl.x"           /* NL Interface includes */
#include "nlu.x"
#include "nl.x"            /* SON includes */

#define WR_MAX_UINLU_SEL       3

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */
PRIVATE S16 nlCopyNluNghCellInfo ARGS((
NlCellCb             *cellCb,
NluNeighborReport    *srcCellCfg,
NlTmpNghCellCfg      *tgtCellCfg
));

PRIVATE S16 nlCopyNluNghCellModInfo ARGS((
NluNghCellMod        *srcCellMod,
NlTmpNghCellMod      *tgtCellMod
));

PRIVATE S16 nlCopyNluNghCellAddInfo ARGS ((
NlCellCb             *cellCb,
NluNghCellAdd        *srcCellAdd,
NlTmpNghCellAdd      *tgtCellAdd
));

PRIVATE S16 nlCopyNluNghCellDelInfo ARGS ((
NluNghCellDel        *srcCellDel,
NlTmpNghCellDel      *tgtCellDel
));

PUBLIC NluBndCfm  nlLiNluBndCfmMt[WR_MAX_UINLU_SEL] =
{
#ifdef NLU_LC
   cmPkNluBndCfm,        /* 0 - loosely coupled */
#else
   PtLiNluBndCfm,
#endif /*NLU_LC */
#ifdef SON_ENABLE
   WrLiNluBndCfm,        /* 1 - Tightly couple */
#else
   PtLiNluBndCfm,
#endif /*NLU_TC*/
#ifdef NLU_LWLC
   cmPkNluBndCfm,         /* 2 - Lightweight loose couple */
#else
   PtLiNluBndCfm,
#endif /*RMU_INTF */
};

/**

 * @brief API for bind request from eNB App towards SON.
 *
 * @details
 *
 *     Function: NlUiNluBndReq
 *
 *     This API is invoked by eNB App towards SON to bind NLU SAP.
 *     These API validates the Pst, spId, suId and sends the bind confirm
 *     to eNB App
 *
 *
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluBndReq
(
Pst   *pst,   /* post structure  */
SuId  suId,   /* service user Id */
SpId  spId    /* service provider Id */
)
#else
PUBLIC S16 NlUiNluBndReq(pst, suId, spId)
Pst   *pst;   /* post structure  */
SuId  suId;   /* service user Id */
SpId  spId;   /* service provider Id */
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   U8        status = CM_BND_OK; /* Bind Status */
   TRC3(NlUiNluBndReq)

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = pst->region;
   tmpPst.pool        = pst->pool;
   tmpPst.srcProcId   = nlCb.init.procId;
   tmpPst.srcEnt      = nlCb.init.ent;
   tmpPst.srcInst     = nlCb.init.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;


   if(spId == nlCb.nluSap.spId)
   {
      /* Check the state of the SAP */
      switch (nlCb.nluSap.sapState)
      {
         case LNL_NOT_CFG: /* SAP Not configured */
         {
            status = CM_BND_NOK;
         }
         break;

         case LNL_UNBND: /* SAP is not bound */
         {
            nlCb.nluSap.sapState = LNL_BND;
            nlCb.nluSap.suId = suId;
         }
         break;

         case LNL_BND: /* SAP is already bound*/
         {
         }
         break;

         default: /* Should Never Enter here */
         {
            status = CM_BND_NOK;
         }
         break;
      }
   }
   else
   {
      RLOG1(L_ERROR, "Invalid SAP ID %d: NluUiNluBndReq Failed",spId);
      status = CM_BND_NOK;
   }

   /* Send Bind Confirm with status as set above */
   ret = NlUiNluBndCfm(&tmpPst, nlCb.nluSap.suId, nlCb.nluSap.spId, status);
   RETVALUE(ret);
}  /* NlUiNluBndReq */

/**

 * @brief API for bind confirm from SON towards eNB App.
 *
 * @details
 *
 *     Function: NlUiNluBndCfm
 *
 *     This API is invoked by SON towards eNB App to send bind confirm for 
 *     NLU SAP.
 *
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @param[in]  U8    status
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluBndCfm
(
Pst   *pst,   /* post structure */
SuId  suId,   /* service user Id */
SpId  spId,   /* service provider Id*/
U8    status  /* bind request status */
)
#else
PUBLIC S16 NlUiNluBndCfm(pst, suId, spId, status)
(
Pst    *pst;   /* post structure */
SuId   suId;   /* service user Id */
SpId   spId;   /* service provider Id*/
U8     status; /* bind request status */
)
#endif
{
   TRC3(NlUiNluBndCfm)

   (*nlLiNluBndCfmMt[pst->selector])
      (pst, suId, spId, status);

   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell modify Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyNluNghCellModInfo
 *
 *     This function copy the neighbor cell information received
 *     from eNB-APP to SON module neighbor cell structure
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
PRIVATE S16 nlCopyNluNghCellModInfo
(
NluNghCellMod        *srcCellMod,
NlTmpNghCellMod      *tgtCellMod
)
#else
PRIVATE S16 nlCopyNluNghCellModInfo(srcCellMod, tgtCellMod)
NluNghCellMod       *srcCellMod;
NlTmpNghCellMod      *tgtCellMod;
#endif
{
   U32                   nghCount;
   U32                   nghNghCount;
   NlTmpNghCellModInfo   *nlCellMod  = NULLP;
   NluNghCellModInfo     *nluCellMod = NULLP;
   TRC3(nlCopyNluNghCellModInfo);
   tgtCellMod->numNgh = srcCellMod->numNgh;
   for(nghCount = 0; nghCount < tgtCellMod->numNgh; nghCount ++)
   {
      nlCellMod = &(tgtCellMod->nghCellModInfo[nghCount]);
      nluCellMod = &(srcCellMod->nghCellModInfo[nghCount]);
      cmMemcpy((U8 *)&(nlCellMod->oldEcgi), (U8 *) &(nluCellMod->oldEcgi),
           sizeof(LnlEcgi));
      nlCellMod->nghCellInfo.pci = nluCellMod->nghCellInfo.pci;
      nlCellMod->nghCellInfo.dlEarfcn = nluCellMod->nghCellInfo.dlEarfcn;
      nlCellMod->nghCellInfo.ulEarfcn = nluCellMod->nghCellInfo.ulEarfcn;
      cmMemcpy((U8 *)&(nlCellMod->nghCellInfo.ecgi), 
          (U8 *)&(nluCellMod->nghCellInfo.ecgi), sizeof(LnlEcgi));
      nlCellMod->numNghNgh = nluCellMod->numNghNgh;
      for(nghNghCount = 0; nghNghCount < nlCellMod->numNghNgh; nghNghCount ++)
      {
         nlCellMod->nghNghCellInfo[nghNghCount].pci = 
           nluCellMod->nghNghCellInfo[nghNghCount].pci;
         nlCellMod->nghNghCellInfo[nghNghCount].dlEarfcn = 
           nluCellMod->nghNghCellInfo[nghNghCount].dlEarfcn;
         nlCellMod->nghNghCellInfo[nghNghCount].ulEarfcn = 
           nluCellMod->nghNghCellInfo[nghNghCount].ulEarfcn;
         cmMemcpy((U8 *) &(nlCellMod->nghNghCellInfo[nghNghCount].ecgi),
            (U8 *) &(nluCellMod->nghNghCellInfo[nghNghCount].ecgi), sizeof(LnlEcgi));
      }
   } 
   RETVALUE(ROK);
}
/**
 * @brief Layer Manager neighbor cell delete Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyNluNghCellDelInfo
 *
 *     This function copy the neighbor cell information received
 *     from eNB-APP to SON module neighbor cell structure
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
PRIVATE S16 nlCopyNluNghCellDelInfo
(
NluNghCellDel        *srcCellDel,
NlTmpNghCellDel      *tgtCellDel
)
#else
PRIVATE S16 nlCopyNluNghCellDelInfo(srcCellDel, tgtCellDel)
NluNghCellDel       *srcCellDel;
NlTmpNghCellDel      *tgtCellDel;
#endif
{
   U32   nghCount;
   TRC3(nlCopyNluNghCellDelInfo);
   tgtCellDel->numNgh = srcCellDel->numNgh;
   for(nghCount = 0; nghCount < tgtCellDel->numNgh; nghCount ++)
   {
      cmMemcpy((U8 *)&tgtCellDel->ecgi, 
        (U8 *) &srcCellDel->ecgi, sizeof(LnlEcgi));
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell add Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyNluNghCellAddInfo
 *
 *     This function copy the neighbor cell information received
 *     from eNB-APP to SON module neighbor cell structure
 *
 *  @param[in]  cellCb       : cell control block
 *  @param[in]  srcCellAdd   : neighbor cell configuration parameter
 *                             structure received from SM
 *  @param[in]  tgtCellAdd   : SON neighbor cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCopyNluNghCellAddInfo
(
NlCellCb             *cellCb,
NluNghCellAdd        *srcCellAdd,
NlTmpNghCellAdd      *tgtCellAdd
)
#else
PRIVATE S16 nlCopyNluNghCellAddInfo(cellCb, srcCellAdd, tgtCellAdd)
NlCellCb             *cellCb;
NluNghCellAdd        *srcCellAdd;
NlTmpNghCellAdd      *tgtCellAdd;
#endif
{
   U32                 nghCount;
   U32                 nghNghCount;
   NluNghCellAddInfo   *nluCellAdd = NULLP;
   NlTmpNghCellAddInfo *nlCellAdd = NULLP;
   TRC3(nlCopyNluNghCellAddInfo);
   for(nghCount = 0; nghCount < srcCellAdd->numNgh; nghCount ++)
   {
      nluCellAdd = &(srcCellAdd->nghCellAddInfo[nghCount]);
      nlCellAdd = &(tgtCellAdd->nghCellAddInfo[tgtCellAdd->numNgh]);
      if(!((nluCellAdd->nghCellInfo.pci == cellCb->cellInfo.pci) && 
         (nluCellAdd->nghCellInfo.dlEarfcn == cellCb->cellInfo.dlEarfcn) &&
         (FALSE == cmMemcmp((U8 *)&(cellCb->cellInfo.ecgi), 
           (U8 *) &(nluCellAdd->nghCellInfo.ecgi), sizeof(LnlEcgi)))))
      { 
         tgtCellAdd->numNgh ++;
         nlCellAdd->nghCellInfo.pci = nluCellAdd->nghCellInfo.pci;
         nlCellAdd->nghCellInfo.dlEarfcn = nluCellAdd->nghCellInfo.dlEarfcn;
         nlCellAdd->nghCellInfo.ulEarfcn = nluCellAdd->nghCellInfo.ulEarfcn;
         cmMemcpy((U8 *)&(nlCellAdd->nghCellInfo.ecgi), 
            (U8 *)&(nluCellAdd->nghCellInfo.ecgi), sizeof(LnlEcgi));
         for(nghNghCount = 0; nghNghCount < nluCellAdd->numNghNgh; nghNghCount ++)
         {
            if(!((nluCellAdd->nghNghCellInfo[nghNghCount].pci == cellCb->cellInfo.pci) &&
                 (nluCellAdd->nghNghCellInfo[nghNghCount].dlEarfcn == cellCb->cellInfo.dlEarfcn) &&
                 (FALSE == cmMemcmp((U8 *) &(cellCb->cellInfo.ecgi),
                   (U8 *) &(nluCellAdd->nghNghCellInfo[nghNghCount].ecgi), sizeof(LnlEcgi)))))
            { 
               nlCellAdd->nghNghCellInfo[nlCellAdd->numNghNgh].pci = 
                nluCellAdd->nghNghCellInfo[nghNghCount].pci;
               nlCellAdd->nghNghCellInfo[nlCellAdd->numNghNgh].dlEarfcn = 
                nluCellAdd->nghNghCellInfo[nghNghCount].dlEarfcn;
               nlCellAdd->nghNghCellInfo[nlCellAdd->numNghNgh].ulEarfcn = 
                nluCellAdd->nghNghCellInfo[nghNghCount].ulEarfcn;
               cmMemcpy((U8 *)&(nlCellAdd->nghNghCellInfo[nlCellAdd->numNghNgh].ecgi),
                (U8 *)&(nluCellAdd->nghNghCellInfo[nghNghCount].ecgi), sizeof(LnlEcgi));
               nlCellAdd->numNghNgh ++;
            }
         }
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager neighbor cell Configuration handler.
 *
 * @details
 *
 *     Function : nlCopyNluNghCellInfo
 *
 *     This function copy the neighbor cell information received
 *     from eNB-APP to SON module neighbor cell structure
 *
 *  @param[in]  cellCb       : cell control block
 *  @param[in]  srcCellCfg   : neighbor cell configuration parameter
 *                             structure received from SM
 *  @param[in]  tgtCellCfg   : SON neighbor cell structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlCopyNluNghCellInfo
(
NlCellCb             *cellCb,
NluNeighborReport    *srcCellCfg,
NlTmpNghCellCfg      *tgtCellCfg
)
#else
PRIVATE S16 nlCopyNluNghCellInfo(cellCb, srcCellCfg, tgtCellCfg)
NlCellCb             *cellCb;
NluNeighborReport    *srcCellCfg;
NlTmpNghCellCfg      *tgtCellCfg;
#endif
{
   U8                numEnb;
   TRC3(nlCopyNluNghCellInfo);
   cmMemset((U8 *)tgtCellCfg, 0, sizeof(NlTmpNghCellCfg));
   tgtCellCfg->cellId = srcCellCfg->servCellId;
   tgtCellCfg->numNghEnb = srcCellCfg->numNghEnb;
   for(numEnb = 0; numEnb < tgtCellCfg->numNghEnb; numEnb ++)
   {
      tgtCellCfg->nghEnbCellCfg[numEnb].nghEnbId  = 
          srcCellCfg->nghEnbCellCfg[numEnb].nghEnbId;
      switch(srcCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType)
      {
         case NLU_NGH_ADD :
         {
            tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType = NL_NGH_ADD;
            nlCopyNluNghCellAddInfo(cellCb, &(srcCellCfg->
               nghEnbCellCfg[numEnb].nghCellParam.u.nghCellAdd),
               &(tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellAdd));
            break;
         }
         case NLU_NGH_DEL :
         {
            tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType = NL_NGH_DEL;
            nlCopyNluNghCellDelInfo(&(srcCellCfg->
               nghEnbCellCfg[numEnb].nghCellParam.u.nghCellDel),
               &(tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellDel));
            break;
         }
         case NLU_NGH_MOD :
         {
            tgtCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType = NL_NGH_MOD;
            nlCopyNluNghCellModInfo(&(srcCellCfg->
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

 * @brief API for processing neighbor report request from eNB App 
 *  towards SON.
 *
 * @details
 *
 *     Function: NluUiNeighborReq
 *
 *     This API is invoked by eNB App towards SON to inform about any 
 *     new neighbor detected by eNodeB
 *
 *  @param[in]  Pst                  *pst
 *  @param[in]  SuId                 suId
 *  @param[in]  NluNeighborReport    *nhbr
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluNeighborReq
(
Pst                  *pst,       /* post structure  */
SuId                  suId,      /* service user Id */
NluNeighborReport    *nhbr       /* Report new neighnor structure */
)
#else
PUBLIC S16 NlUiNluNeighborReq(pst, suId, nhbr)
Pst                   *pst;      /* post structure  */
SuId                   suId;     /* service user Id */
NluNeighborReport     *nhbr;     /* Report new neighbor structure */
#endif
{
   
   S16               ret = ROK;
   NlCellCb          *cellCb   = NULLP;
   NlTmpNghCellCfg   *nlCellCfg;
   U32               hashKey;
   NlEventMsg        evtMsg;
   TRC3(NlUiNluNeighborReq);
   hashKey = nhbr->servCellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey,
     sizeof(hashKey), 0, (PTR *)&cellCb))
   {
      RLOG1(L_ERROR, "Failed to cell Cb with cell Id %d", 
            nhbr->servCellId);
      NL_FREE(pst->region, pst->pool, nhbr, sizeof(NluNeighborReport));
      RETVALUE(RFAILED);
   }
   NL_ALLOC(NL_REGION, NL_POOL, &nlCellCfg, sizeof(NlTmpNghCellCfg));
   if(nlCellCfg == NULLP)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for NlTmpNghCellCfg \n");
      RETVALUE(RFAILED);
   }
   nlCopyNluNghCellInfo(cellCb, nhbr, nlCellCfg);
   /*update the possible message type for neighbor reporting*/
   switch(nhbr->msgType)
   {
      case NLU_X2SETUP_REQ_NGH_RPT :
      {
         evtMsg.msgType = SON_X2SETUP_REQ_NGH_CFG;
         break;
      }
      case NLU_X2SETUP_RSP_NGH_RPT :
      {
         evtMsg.msgType = SON_X2SETUP_RSP_NGH_CFG;
         break;
      }
      case NLU_ENB_CFG_UPD_NGH_RPT :
      {
         evtMsg.msgType = SON_ENB_CFG_UPD_NGH_CFG;
         break;
      }
      case NLU_ENB_ANR_NGH_RPT :
      {
         evtMsg.msgType = SON_ANR_NGH_CFG;
         break;
      }
      case NLU_ENB_DEL_NGH_RPT :
      {
         evtMsg.msgType = SON_NGH_CFG;
         break;
      }
      default :
      {
         RLOG1(L_ERROR, "Invalid message type: %d  received from eNB-APP", 
            nhbr->msgType);
         NL_FREE(pst->region, pst->pool, nhbr, sizeof(NluNeighborReport));
         RETVALUE(RFAILED);
      }
   }
   evtMsg.msg = (Void *)nlCellCfg;
   ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_EVENT_NGH_CFG])(&evtMsg,
         cellCb);
   NL_FREE(NL_REGION, NL_POOL, nlCellCfg, sizeof(NlTmpNghCellCfg));
   /* free the memory allocated at eNB APP */
   NL_FREE(pst->region, pst->pool, nhbr, sizeof(NluNeighborReport));
 
   RETVALUE(ret);
}

/**

 * @brief API for processing PCI modification confirm from eNB App
 *  towards SON.
 *
 * @details
 *
 *     Function: NlUiNluPciModIndCfm
 *
 *     This API is invoked by eNB App towards SON to inform about PCI mod cfm
 *     in eNodeB
 *
 *  @param[in]  Pst                  *pst
 *  @param[in]  SuId                 suId
 *  @param[in]  NluPciModIndCfm      *pciMod
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluPciModIndCfm
(
Pst                  *pst,       /* post structure  */
SuId                  suId,      /* service user Id */
NluPciModIndCfm      *pciMod     /* PCI mod confirm structure */
)
#else
PUBLIC S16 NlUiNluPciModIndCfm(pst, suId, pciMod)
Pst                   *pst;      /* post structure  */
SuId                   suId;     /* service user Id */
NluPciModIndCfm       *pciMod;   /* PCI mod confirm structure */
#endif
{
   NlCellCb             *cellCb       = NULLP;
   CmLList              *link         = NULLP;
   NlNghCfgPendMsg      *nghPendMsg   = NULLP;
   NlCellReCfgPendMsg   *reCfgPendMsg = NULLP;
   NlEventMsg            evtMsg;
   U32                   hashKey;
   LnlUstaDgn            alarm;
   TRC3(NlUiNluPciModIndCfm);
   cmMemset((U8 *)&alarm, 0, sizeof(LnlUstaDgn));
   hashKey = pciMod->ecgi.cellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey,
     sizeof(hashKey), 0, (PTR *)&cellCb))
   {
      RLOG1(L_ERROR, "Failed to cell Cb with cell Id %d",
              pciMod->ecgi.cellId);
      NL_FREE(pst->region, pst->pool, pciMod, sizeof(NluPciModIndCfm));
      RETVALUE(RFAILED);
   }
   /* clear the alarm*/
   if(cellCb->pciModAlarmInfo.isAlarmSend)
   {
      alarm.type = cellCb->pciModAlarmInfo.alarmType;;
      alarm.alarmId = cellCb->pciModAlarmInfo.alarmId;
      alarm.severity = LNL_ALARM_SEVERITY_CLEARED;
      alarm.causeType = 0;
      alarm.causeVal = 0;
      alarm.u.addInfo.pci = cellCb->pciModAlarmInfo.oldPci;
      alarm.u.addInfo.cellId = cellCb->cellInfo.ecgi.cellId;
      alarm.u.addInfo.action = LNL_ALARM_AUTO_RESOLVE;
      nlSendLnlAlarm(&alarm);
      cellCb->pciModAlarmInfo.isAlarmSend = FALSE;
   }  
   /*update the cell state*/
   NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_CONFIGURED);
   /*check and pending message present for cell reconfig or neighbor config*/
   /*process first pending neighbor config message if present*/
   CM_LLIST_FIRST_NODE(&cellCb->pciModPendMsg.nghCfgMsgList, link);
   while(NULLP != link)
   {
      if(NL_CELL_STATE_CONFIGURED != cellCb->state)
      {
         break;
      }
      nghPendMsg = (NlNghCfgPendMsg *)link->node;
      evtMsg.msg = &nghPendMsg->nghCellCfg;
      nlConfigNghCellCfgEvtHndlr(&evtMsg, cellCb);
      cmLListDelFrm(&cellCb->pciModPendMsg.nghCfgMsgList, link);
      NL_FREE(NL_REGION, NL_POOL, nghPendMsg, sizeof(NlNghCfgPendMsg));
      link = NULLP;
      CM_LLIST_FIRST_NODE(&cellCb->pciModPendMsg.nghCfgMsgList, link);
   }
   /*process cell reconfig message if present*/
   CM_LLIST_FIRST_NODE(&cellCb->pciModPendMsg.cellReCfgMsgList, link);
   while(NULLP != link)
   {
      if(NL_CELL_STATE_CONFIGURED != cellCb->state)
      {
         break;
      }
      reCfgPendMsg = (NlCellReCfgPendMsg *)link->node;
      evtMsg.msg = &reCfgPendMsg->cellReCfg;
      nlConfigCellReCfgEvtHndlr(&evtMsg, cellCb);
      cmLListDelFrm(&cellCb->pciModPendMsg.cellReCfgMsgList, link);
      NL_FREE(NL_REGION, NL_POOL, reCfgPendMsg, sizeof(NlCellReCfgPendMsg));
      link = NULLP;
      CM_LLIST_FIRST_NODE(&cellCb->pciModPendMsg.cellReCfgMsgList, link);
   } 
   /* free the memory allocated at eNB APP */
   NL_FREE(pst->region, pst->pool, pciMod, sizeof(NluPciModIndCfm));
   RETVALUE(ROK);
}


/**
 * @brief API for processing UE indication from eNB App
 *  towards SON.
 *
 * @details
 *
 *     This API is invoked by eNB App towards SON to inform about any
 *     new UE attached with eNB or detach from eNB
 *
 *  @param[in]  pst    : post structure to free the memory allocated in eNB-APP
 *  @param[in]  suid   : service user Id
 *  @param[in]  ueInfo : UE information structure with events
 *  @return  S16
 *      -# SUCCESS     :ROK
 *      -# FAILURE     :RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluUeInd
(
Pst                  *pst,       /*!< post structure  */
SuId                  suId,      /*!< service user Id */
NluUeInfo            *ueInfo     /*!< UE information message structure */
)
#else
PUBLIC S16 NlUiNluUeInd(pst, suId, ueInfo)
Pst                   *pst;      /*!< post structure  */
SuId                   suId;     /*!< service user Id */
NluUeInfo             *ueInfo;   /*!< UE information message structure */
#endif
{
   S16               ret = ROK;
   NlCellCb          *cellCb   = NULLP;
   U32               hashKey;
   NlEventMsg        evtMsg;
   hashKey = ueInfo->ecgi.cellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey,
      sizeof(hashKey), 0, (PTR *)&cellCb))
   {
       RLOG1(L_ERROR, "Failed to cell Cb with cell Id for UeInd%d",
             ueInfo->ecgi.cellId);
       NL_FREE(pst->region, pst->pool, ueInfo, sizeof(NluUeInfo));
       RETVALUE(RFAILED);
   }
   evtMsg.msg = (Void *)ueInfo;
   if(NLU_UE_ADDED == ueInfo->event)
   {
      evtMsg.msgType = SON_UE_ADD;
      ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_UE_ADD])(&evtMsg,
         cellCb);
   }
   else if(NLU_UE_DELETED == ueInfo->event)
   {
      evtMsg.msgType = SON_UE_DELETE;
      ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_UE_DELETE])(&evtMsg,
         cellCb);
   }
   else if(NLU_UE_CRNTI_CHANGED == ueInfo->event)
   {
      evtMsg.msgType = SON_UE_MODIFY;
      ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_UE_MODIFY])(&evtMsg,
         cellCb);
   }
   /* free the memory allocated at eNB APP */
   NL_FREE(pst->region, pst->pool, ueInfo, sizeof(NluUeInfo));
   RETVALUE(ret);
}

/**
 * @brief API for processing UE measurement report indication from eNB App
 *  towards SON.
 *
 * @details
 *
 *     This API is invoked by eNB App towards SON to inform about UE
 *     measurement report
 *
 *  @param[in]  pst     : post structure to free the memory allocated in eNB-APP
 *  @param[in]  suid    : service user Id
 *  @param[in]  measRpt : UE measurement report message structure 
 *  @return  S16
 *      -# SUCCESS     :ROK
 *      -# FAILURE     :RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluUeMeasReportInd
(
Pst                  *pst,       /*!< post structure  */
SuId                  suId,      /*!< service user Id */
NluUeMeasRpt          *measRpt   /*!< UE measurement report message structure*/
)
#else
PUBLIC S16 NlUiNluUeMeasReportInd(pst, suId, measRpt)
Pst                   *pst;      /*!< post structure  */
SuId                   suId;     /*!< service user Id */
NluUeMeasRpt          *measRpt;  /*!< UE measurement report message structure */
#endif
{
   S16               ret = ROK;
   NlCellCb          *cellCb   = NULLP;
   U32               hashKey;
   NlEventMsg        evtMsg;
   hashKey = measRpt->ecgi.cellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey,
      sizeof(hashKey), 0, (PTR *)&cellCb))
   {
       RLOG1(L_ERROR, "Failed to cell Cb with cell Id for MeasRpt%d",
             measRpt->ecgi.cellId);
       NL_FREE(pst->region, pst->pool, measRpt, sizeof(NluUeMeasRpt));
       RETVALUE(RFAILED);
   }
   evtMsg.msgType = SON_UE_MEAS_RPT;
   evtMsg.msg = (Void *)measRpt;
   ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_UE_MEAS_RPT])(&evtMsg,
         cellCb);
   /* free the memory allocated at eNB APP */
   NL_FREE(pst->region, pst->pool, measRpt, sizeof(NluUeMeasRpt));
   RETVALUE(ret);
}

/* Periodic REM for TPM */
/**

 * @brief API for processing Periodic Cell search Rsponse from eNB App
 *  towards SON.
 *
 * @details
 *
 *     Function: NlUiNluPerdCellSrchRsp
 *
 *     This API is invoked by eNB App towards SON to inform about periodic REM
 *     scan results.
 *
 *  @param[in]  Pst                           *pst
 *  @param[in]  SuId                          suId
 *  @param[in]  NluPeriodicRemCellSearchRsp   *cellSrchRsp
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluPerdCellSrchRsp
(
Pst                           *pst,            /* post structure  */
SuId                          suId,            /* service user Id */
NluPeriodicRemCellSearchRsp   *cellSrchRsp     /* cell search response structure */
)
#else
PUBLIC S16 NlUiNluPerdCellSrchRsp(pst, suId, pciMod)
Pst                           *pst;            /* post structure  */
SuId                          suId;            /* service user Id */
NluPeriodicRemCellSearchRsp   *cellSrchRsp;     /* cell search response structure */
#endif
{
   TRC3(NlUiNluPerdCellSrchRsp);
//   if(TRUE == cellCb->isSmSonCfgDone) 
//   {
      RLOG0(L_INFO,"Periodic Cell search Rsp for REM received at SON");
      nlHdlPeriodicRemCellSrchRsp(cellSrchRsp);
//   }
   NL_FREE(pst->region, pst->pool, cellSrchRsp,
         sizeof(NluPeriodicRemCellSearchRsp));

   RETVALUE(ROK);
}
/* Periodic REM for TPM End */
#ifdef RGR_CQI_REPT

/**
 * @brief API for processing UE CQI report indication from eNB App
 *  towards SON.
 *
 * @details
 *
 *     This API is invoked by eNB App towards SON to inform about 
 *     UE CQI reports 
 *
 *  @param[in]  pst    : post structure to free the memory allocated in eNB-APP
 *  @param[in]  suid   : service user Id
 *  @param[in]  cqiRpt : UE CQI report message structure 
 *  @return  S16
 *      -# SUCCESS     :ROK
 *      -# FAILURE     :RFAILED
 **/
#ifdef ANSI
PUBLIC S16 NlUiNluUeCqiReportInd
(
Pst                  *pst,       /*!< post structure  */
SuId                  suId,      /*!< service user Id */
NluUeCqiRpt          *cqiRpt     /*!< UE CQI report message structure */
)
#else
PUBLIC S16 NlUiNluUeCqiReportInd(pst, suId, cqiRpt)
Pst                   *pst;      /*!< post structure  */
SuId                   suId;     /*!< service user Id */
NluUeCqiRpt           *cqiRpt;   /*!< UE CQI report message structure */
#endif
{
   S16               ret = ROK;
   NlCellCb          *cellCb   = NULLP;
   U32               hashKey;
   NlEventMsg        evtMsg;
   hashKey = cqiRpt->cellId;
   if(ROK != cmHashListFind(&(nlCb.cellCbList), (U8 *)&hashKey,
      sizeof(hashKey), 0, (PTR *)&cellCb))
   {
       RLOG1(L_ERROR, "Failed to cell Cb with cell Id for CqiRpt%d",
             cqiRpt->cellId);
       NL_FREE(pst->region, pst->pool, cqiRpt, sizeof(NluUeCqiRpt));
       RETVALUE(RFAILED);
   }
   evtMsg.msgType = SON_UE_CQI_RPT;
   evtMsg.msg = (Void *)cqiRpt;
   ret = (*nlGlbCellFSM[cellCb->state][NL_CELL_UE_CQI_RPT])(&evtMsg,
         cellCb);
   /* free the memory allocated at eNB APP */
   NL_FREE(pst->region, pst->pool, cqiRpt, sizeof(NluUeCqiRpt));
   RETVALUE(ret);
}

#endif
/********************************************************************30**

           End of file:     bk_ptui.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:32 2014

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

