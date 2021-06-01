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

        Name:    LTE RLC - Configuration Manager file

        Type:    C source file

        Desc:    It contains the following configuraiton primtives
                 for different actions
                    -- kwCfgValdtEntCfg
                    -- kwCfgFillRbCb
                    -- kwCfgRbInit
                    -- kwCfgAddRb
                    -- kwCfgReCfgRb
                    -- kwCfgDelRb
                    -- kwCfgReEstRb
                    -- kwCfgDelUe

        File:     gp_cfg.c

        Sid:      gp_cfg_dl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:59 2015

        Prg:      gk

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="CFG";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=191;

/** @file gp_cfg_dl.c
@brief RLC Downlink Configuration Module
**/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* RLC error options */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"

#define KW_MODULE KW_DBGMASK_CFG
/*Added for adding new Ue in onging L2 Meas*/
#ifdef LTE_L2_MEAS
/**
 *
 * @brief Handle modification of UE ID for L2 Meas data structs
 *       
 *
 *  @param[in] ueId     ue ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
PRIVATE S16 kwHdlMeasDlUeIdChg(KwCb *gCb, U8 cellId,U8 oldUeId, U8 newUeId)
{
   KwL2MeasEvtCb *measEvtCb = NULLP;
   KwL2MeasCb    *measCb    = NULLP;
   U16           cntr;
   U16          ueIdx = 0;     

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(gCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr]);
      measCb = &(measEvtCb->measCb);


      if(measCb->measType & LKW_L2MEAS_DL_IP ) 
      {

         for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
         {
            if((measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId) == oldUeId)
            {
               measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId = newUeId;
               break;
            }
         }
      }
#if 0 /* loop for only debugging */
      {
         U32 myIdx =0;

         for (myIdx = 0; myIdx < measCb->val.ipThMeas.numUes; myIdx++)
         {
         }
      }
#endif
   }
   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to delete an UE's L2 Meas ctxt
 *       
*
 *  @param[in] ueId     ue ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
PRIVATE S16 kwDelFrmDlL2Meas(KwCb *gCb, U8 cellId,U8 ueId)
{
   KwL2MeasEvtCb *measEvtCb = NULLP;
   KwL2MeasCb    *measCb    = NULLP;
   U16           cntr;
   U16          ueIdx = 0;     


   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &gCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr];
      measCb = &(measEvtCb->measCb);


      if(measCb->measType & LKW_L2MEAS_DL_IP )
      {

         for(ueIdx = 0; ((ueIdx < measCb->val.ipThMeas.numUes) &&
                       (ueIdx < gCb->genCfg.maxUe)); ueIdx++)
         {
            if((measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId) == ueId)
            {
               measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid = FALSE;
               if (measCb->val.ipThMeas.numUes-1 == ueIdx)
               {
                  measCb->val.ipThMeas.numUes--;
               }
               break;
            }
         }
      }
   }

   RETVALUE(ROK);
}


PRIVATE S16 kwAddToDlL2Meas(KwCb *gCb, KwDlRbCb *kwRbCb,U8 cellId,U8 ueId)
{
   KwL2MeasEvtCb *measEvtCb = NULLP;
   KwL2MeasCb    *measCb    = NULLP;
   U16           cntr;
   U16           cntr1;
   U16            ueIdx = 0;
   U16            qciIdx = 0;
   U16           *numQci;
   U8             freeIdx = gCb->genCfg.maxUe;


   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &gCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr];
      measCb = &(measEvtCb->measCb);

      freeIdx = gCb->genCfg.maxUe;

      if(measCb->measType & 
          (LKW_L2MEAS_ACT_UE | LKW_L2MEAS_UU_LOSS | LKW_L2MEAS_DL_DELAY))
      {
         for(cntr1 =0;((cntr1 < measCb->val.nonIpThMeas.numQci) &&
                       (cntr1 < LKW_MAX_QCI));cntr1++)
         {
            if(measCb->val.nonIpThMeas.qci[cntr1] != kwRbCb->qci)
            {
               measCb->val.nonIpThMeas.qci[cntr1]  = kwRbCb->qci;
               gCb->u.dlCb->kwL2Cb.measOn[kwRbCb->qci] |=measCb->measType;
               break;
            }
         }
      }

      if(((kwRbCb->rbL2Cb.measOn & measCb->measType) == LKW_L2MEAS_NONE))
      {
         if (measCb->measType & LKW_L2MEAS_ACT_UE)
         {
            if((kwRbCb->mode == CM_LTE_MODE_UM) &&
                  (kwRbCb->dir & KW_DIR_DL ))
            {
               if (kwRbCb->m.umDl.sduQ.count)
               {
                  if (kwRbCb->ueCb->numActRb[kwRbCb->qci] == 0)
                  {
                     kwRbCb->ueCb->numActRb[kwRbCb->qci]++;
                     gCb->u.dlCb->kwL2Cb.numActUe[kwRbCb->qci]++;
                  }
               }
            }
            else if (kwRbCb->mode == CM_LTE_MODE_AM)
            {
               if ((kwRbCb->m.amDl.cntrlBo) ||
                     (kwRbCb->m.amDl.retxBo)  ||
                     (kwRbCb->m.amDl.bo))
               {
                  if (kwRbCb->ueCb->numActRb[kwRbCb->qci] == 0)
                  {
                     kwRbCb->ueCb->numActRb[kwRbCb->qci]++;
                     gCb->u.dlCb->kwL2Cb.numActUe[kwRbCb->qci]++;
                  }
               }
            }
         }
      }
      if((measCb->measType & LKW_L2MEAS_DL_IP))
      {

         for(ueIdx = 0; ((ueIdx < measCb->val.ipThMeas.numUes) &&
                         (ueIdx < gCb->genCfg.maxUe)); ueIdx++)
         {
            if ((freeIdx == gCb->genCfg.maxUe) && 
                  (measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid == FALSE))
            {
               freeIdx = ueIdx;
               continue;
            }
            if((measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId) == ueId)
            {
               break;
            }
         }

         if (ueIdx ==  measCb->val.ipThMeas.numUes)
         {
            if (gCb->genCfg.maxUe == measCb->val.ipThMeas.numUes)
            {
               RETVALUE(RFAILED);  
            }
            if (gCb->genCfg.maxUe == freeIdx)
            {               
               measCb->val.ipThMeas.numUes++;
            }
            else
            {
               ueIdx = freeIdx;
            }
            measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid = TRUE;
            cmMemset((U8 *)&measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[0],0x00,(sizeof(KwL2Cntr) *LKW_MAX_QCI));
            measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci = 0;
         }
         measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId = ueId;
         measCb->val.ipThMeas.ueInfoLst[ueIdx].cellId = cellId;
         numQci = &(measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci);

         for (qciIdx =0; ((qciIdx <  *numQci) &&
                           (qciIdx < LKW_MAX_QCI)) ; qciIdx++)
         {
            if (measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] == kwRbCb->qci)
            {
               break;
            }
         }

         /* Fix Klock Warning */
         if ((qciIdx == *numQci) && (qciIdx < LKW_MAX_QCI))
         {
            measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] = kwRbCb->qci;
            (*numQci)++;
         }

         kwUtlPlcMeasDatInL2Sts(&measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[kwRbCb->qci],
               &kwRbCb->rbL2Cb, measCb->measType);
      }
      else if (measCb->measType & 
         (LKW_L2MEAS_DL_DISC | LKW_L2MEAS_DL_DELAY | LKW_L2MEAS_UU_LOSS))
      {
         kwUtlPlcMeasDatInL2Sts(&measCb->val.nonIpThMeas.measData[kwRbCb->qci],
               &kwRbCb->rbL2Cb, measCb->measType);
         measCb->val.nonIpThMeas.qci[kwRbCb->qci] = kwRbCb->qci;
         measCb->val.nonIpThMeas.measData[kwRbCb->qci].totDrbsPerQci++;
      }
      kwRbCb->rbL2Cb.measOn |= measCb->measType;      
   }
   RETVALUE(ROK);
}/*kwAddToDlL2Meas*/ 
#endif /*LTE_L2_MEAS*/

/** 
 * @brief
 *     This primitive fills the RbCb 
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [out]   rbCb     -  RB Control Block
 * @param [out]   ueCb     -  UE Control Block
 * @param [in]    entCfg   -  RLC Entity configuration
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
#ifdef ANSI
PRIVATE S16 kwCfgFillDlRbCb
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
KwDlUeCb        *ueCb,
CkwEntCfgInfo   *entCfg
)
#else
PRIVATE S16 kwCfgFillDlRbCb(gCb,rbCb,entCfg)
KwCb            *gCb;
KwDlRbCb        *rbCb;
KwDlUeCb        *ueCb;
CkwEntCfgInfo   *entCfg;
#endif
{
   TRC3(kwCfgFillDlRbCb)

   RLOG_ARG3(L_DEBUG,DBG_RBID,entCfg->rbId,
         "kwCfgFillRbCb(ueId(%d),cellId(%d) rbType(%d))",
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId, 
                entCfg->rbType);

   /* Initialize according to entMode */
   switch (entCfg->entMode)
   {
      case CM_LTE_MODE_TM:
      {
         rbCb->lch.lChId  = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = entCfg->dir;
         break;
      }

      case CM_LTE_MODE_UM:
      {
         rbCb->lch.lChId  = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = entCfg->dir;
         rbCb->m.umDl.snLen = entCfg->m.umInfo.dl.snLen;
         if (entCfg->m.umInfo.dl.snLen == KW_UM_CFG_5BIT_SN_LEN)
            rbCb->m.umDl.modBitMask = 0x1f;
         else
            rbCb->m.umDl.modBitMask = 0x3ff;

         ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = rbCb;

         break;
      }
      case CM_LTE_MODE_AM:
      {
         /* Down Link Information 
          * indx = 0 as Down Link   */
         rbCb->lch.lChId  = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = KW_DIR_BOTH;
         rbCb->m.amDl.pollPdu = entCfg->m.amInfo.dl.pollPdu;
         rbCb->m.amDl.pollByte = entCfg->m.amInfo.dl.pollByte;
         rbCb->m.amDl.maxRetx = entCfg->m.amInfo.dl.maxRetx;
         rbCb->m.amDl.pollRetxTmrInt = entCfg->m.amInfo.dl.pollRetxTmr;
         cmInitTimers(&(rbCb->m.amDl.pollRetxTmr), 1);
      
         rbCb->m.amDl.vtMs = 512; /* initially VT(MS) is 512 as VT(A) is 0 */
        
         ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = rbCb;
       
         KW_ALLOC(gCb,rbCb->m.amDl.txBuf,
                  ((KW_AM_WIN_SZ << 1) * sizeof(KwTx*)));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (rbCb->m.amDl.txBuf == NULLP)
         {   
            RLOG_ARG2(L_FATAL,DBG_RBID,entCfg->rbId,
                     "Memory allocation failed ueId(%d),cellId(%d)",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RETVALUE(RFAILED);
         }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
         break;
      }
      default:
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,entCfg->rbId, 
                  "Invalid RB Mode ueId(%d),cellId(%d)",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RETVALUE(RFAILED);
      }
   }
   rbCb->mode = entCfg->entMode;
   rbCb->discTmrInt = entCfg->discardTmr;

   RETVALUE(ROK);
} 


/** 
 * @brief This primitive Initializes the RB Cb
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [out]   rbCb     -  RB Control Block
 * @param [in]    ptr      -  Void pointer
 * @param [in]    entCfg   -  Entity Configuration
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
#ifdef ANSI
PRIVATE S16 kwCfgUpdateDlRb
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
void            *ptr,
CkwEntCfgInfo   *entCfg
)
#else
PRIVATE S16 kwCfgUpdateDlRb(gCb,rbCb, ptr, entCfg)
KwCb            *gCb;
KwDlRbCb        *rbCb;
void            *ptr;
CkwEntCfgInfo   *entCfg;
#endif
{
   TRC3(kwCfgUpdateDlRb)
   
   if (rbCb->mode != entCfg->entMode)
   {
      RLOG_ARG4(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
            "RB Mode Mismatch : exp [%d] rcv [%d] UEID:%d CELLID:%d", 
            rbCb->mode, 
            entCfg->entMode,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
      RETVALUE(CKW_CFG_REAS_RB_MODE_MIS);
   }

   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
      {
         KwDlCellCb *cellCb = (KwDlCellCb *)ptr;

         rbCb->dir = entCfg->dir;
         rbCb->lch.lChId = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;

         cellCb->lCh[rbCb->lch.lChId - 1].dlRbCb = rbCb;
         break;
      }

      case CM_LTE_MODE_UM:
      {
         KwDlUeCb *ueCb = (KwDlUeCb *)ptr;

         if (entCfg->lCh[0].type == CM_LTE_LCH_DCCH)
         {
            RETVALUE(CKW_CFG_REAS_LCHTYPE_MIS);
         }
         ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = NULLP;
         ueCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = rbCb;

         rbCb->lch.lChId = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = entCfg->dir;
         break;
      }

      case CM_LTE_MODE_AM:
      {
         KwDlUeCb *ueCb = (KwDlUeCb *)ptr;

         ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = NULLP;
         ueCb->lCh[entCfg->lCh[1].lChId - 1].dlRbCb = rbCb;
         
         /* Down Link */
         rbCb->lch.lChId = entCfg->lCh[1].lChId;
         rbCb->lch.lChType = entCfg->lCh[1].type;
         rbCb->m.amDl.pollRetxTmrInt = entCfg->m.amInfo.dl.pollRetxTmr;
         rbCb->m.amDl.pollPdu = entCfg->m.amInfo.dl.pollPdu;
         rbCb->m.amDl.pollByte = entCfg->m.amInfo.dl.pollByte;
         rbCb->m.amDl.maxRetx = entCfg->m.amInfo.dl.maxRetx;
         
         break;
      }
   }

/* AGHOSH */
   rbCb->discTmrInt = entCfg->discardTmr;
/* AGHOSH */
   RETVALUE(CKW_CFG_REAS_NONE);
} 


/** 
 * @brief 
 *    This primitive adds new RB in Ue/Cell Cb.
 *
 * @details
 *    This function does following steps -
 *    - If UE ID is 0 then
 *    - Check for CELL CB is present
 *    - If yes, Check for RB ID
 *       - If RB ID is present Status Indication with reason
 *       - Else, Create New RB CB in CELL CB
 *    - If no Create New CELL CB and RB CB
 *    - Else,
 *       - Check for UE CB is present
 *       - If yes Check for RB ID
 *          - If RB ID is present Status Indication with reason
 *          - Else, Create New RB CB in UE CB
 *          - If no Create New UE CB and RB CB
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation.
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgAddDlRb
(
KwCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
CkwEntCfgInfo      *entCfg,
CkwEntCfgCfmInfo   *entCfm
)
#else
PUBLIC S16 kwCfgAddDlRb(gCb,ueId, cellId, entCfg, entCfm)
KwCb               *gCb;
CmLteRnti          ueId;
CmLteCellId        cellId;
CkwEntCfgInfo      *entCfg;
CkwEntCfgCfmInfo   *entCfm;
#endif
{
   KwDlUeCb     *ueCb = NULLP;   /* UE Control Block */
   KwDlCellCb   *cellCb;         /* Cell Control Block */
   KwDlRbCb     *kwRbCb;         /* KW RB Control Block */
   U8           reason;          /* Rb Identifier */

   TRC3(kwCfgAddDlRb)

   RLOG_ARG3(L_DEBUG,DBG_RBID,entCfg->rbId, 
         "kwCfgAddRb(cellId(%d),UEID:%d cfgType(%d))",
                cellId, 
                ueId,
                entCfg->cfgType);

   if (cellId == 0)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                          CKW_CFG_REAS_CELL_UNKWN);
      RLOG_ARG1(L_ERROR,DBG_RBID,entCfg->rbId,
               "Add DLRb,CellId is 0 for UEID:%d",
               ueId);
      RETVALUE(RFAILED);
   }


   /* Process Adding new RB */
   if (ueId == 0)
   {
      if(entCfg->rbId >= KW_MAX_RB_PER_CELL)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK,
                             CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG3(L_ERROR,DBG_RBID,entCfg->rbId,
                  "Invalid RbId ,Max is [%d] CELLID:%d UEID:%d",
                  KW_MAX_RB_PER_CELL,
                  cellId,
                  ueId);
         RETVALUE(RFAILED);
      }

      if (((entCfg->lCh[0].type == CM_LTE_LCH_BCCH) || 
           (entCfg->lCh[0].type == CM_LTE_LCH_PCCH) ||
           (entCfg->lCh[0].type == CM_LTE_LCH_CCCH)) &&
          (entCfg->entMode == CM_LTE_MODE_TM))
      {
         /* Cell CB present */
         kwDbmFetchDlCellCb(gCb, cellId, &cellCb);
         if(cellCb)
         {
            /* Get rbCb from cellCb->rbCb List */
            if (( cellCb->rbCb[entCfg->rbId] != NULLP))
            {
               /* Fill entCfm structure */
               KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                                   CKW_CFG_CFM_NOK,
                                   CKW_CFG_REAS_RB_PRSNT);
               RLOG_ARG2(L_WARNING, DBG_CELLID,cellId, 
                        "RbId [%d] already exists UEID:%d",
                        entCfg->rbId,
                        ueId);
               RETVALUE(RFAILED);
            }
         }
         else  /* Cell CB UNKNOWN */
         {
            /* Create CELL CB */
            if ( ROK != kwDbmCreateDlCellCb(gCb,cellId, &cellCb))
            {
               /* Fill entCfm structure */
               KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                                   CKW_CFG_CFM_NOK,
                                   CKW_CFG_REAS_CELL_CREAT_FAIL);
               RLOG_ARG2(L_ERROR,DBG_CELLID,cellId,
                        "cellCb Creation failed RBID:%d UEID:%d",
                        entCfg->rbId,
                        ueId);
               RETVALUE(RFAILED);
            }
         }

         /* Validate LChId */
         if(entCfg->lCh[0].lChId <= 0)
         {
            RLOG_ARG3(L_ERROR,DBG_LCID,entCfg->lCh[0].lChId ,
                     "Invalid LcId CELLID:%d UEID:%d RBID:%d",
                     cellId,
                     ueId,
                     entCfg->rbId);
            /* Fill entCfm structure */                               
            KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                  CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_LCHID);           
            RETVALUE(RFAILED);                                        
         }                                                            

         /* Create RB CB */
         KW_ALLOC(gCb,kwRbCb, sizeof (KwDlRbCb));
         if (!kwRbCb)
         {
            RLOG_ARG2(L_FATAL,DBG_UEID,ueId,
                     "Memory allocation failed for rbId:%d CELLID:%d",
                     entCfg->rbId,
                     ueId);
            /* Fill entCfm structure */                           
            KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
                                CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_CREAT_FAIL); 
            RETVALUE(RFAILED); 
         }
         kwRbCb->rlcId.rbId = entCfg->rbId;
         cellCb->rbCb[entCfg->rbId] = kwRbCb;
         KW_LMM_RB_STS_INC(gCb);
         cellCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = kwRbCb;
      }
      else
      {
         reason= (entCfg->entMode != CM_LTE_MODE_TM)? CKW_CFG_REAS_RB_MODE_MIS:
                                                      CKW_CFG_REAS_LCHTYPE_MIS;
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, reason);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if (!(KW_VALIDATE_UE_RBID(entCfg->rbType, entCfg->rbId)))
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG2(L_ERROR,DBG_RBID, entCfg->rbId,
                  "Invalid RbId for RbType[%d] UEID:%d", 
                  entCfg->rbType,
                  ueId);
         RETVALUE(RFAILED);
      }
      if ((((entCfg->lCh[0].type == CM_LTE_LCH_DCCH) && 
            (entCfg->entMode != CM_LTE_MODE_UM) && 
            (CM_LTE_SRB == entCfg->rbType)) ||
           ((entCfg->lCh[0].type == CM_LTE_LCH_DTCH) && 
            (CM_LTE_DRB == entCfg->rbType))) &&
          (entCfg->entMode != CM_LTE_MODE_TM))
      {
         /* UE CB present */
         if ( kwDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb) == ROK)
         {
            /* Get rbCb from ueCb->rbCb list */
            KW_DBM_GET_RBCB_FROM_UECB(entCfg->rbId, entCfg->rbType, ueCb, kwRbCb);

            if(( kwRbCb != NULLP))
            {
               /* Fill entCfm structure */
               KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                     CKW_CFG_REAS_RB_PRSNT);
               RLOG_ARG2(L_WARNING, DBG_UEID, ueId,
                        "CellId[%u]:rbId [%d] already exists",
                        cellId,
                        entCfg->rbId);
               RETVALUE(RFAILED);
            }
         }
         else  /* UE CB UNKNOWN */
         {
            /* Create UE CB */
            if ( kwDbmCreateDlUeCb(gCb,ueId, cellId, &ueCb) != ROK)
            {
               /* Fill entCfm structure */
               KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                     CKW_CFG_REAS_UE_CREAT_FAIL);
               RLOG_ARG2(L_WARNING, DBG_CELLID,cellId,
                        "UeId [%u]:ueCb Creation Failed RBID:%d",
                        ueId,
                        entCfg->rbId);
               RETVALUE(RFAILED);
            }
         }

         /* Validate LChId for UM and AM modes */
         if ((entCfg->lCh[0].lChId <= 0) ||
             ((entCfg->entMode == CM_LTE_MODE_AM)&&
               (entCfg->lCh[1].lChId <= 0)))
         {
            /* Fill entCfm structure */                               
            KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                  CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_LCHID);           
            RETVALUE(RFAILED);                                        
         }                                                            

         /* Create RB CB */
         KW_ALLOC(gCb,kwRbCb, sizeof (KwDlRbCb));
         if (kwRbCb == NULL)
         {
            /* Fill entCfm structure */                           
            KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,CKW_CFG_CFM_NOK,
                                    CKW_CFG_REAS_RB_CREAT_FAIL); 
            RLOG_ARG2(L_FATAL,DBG_UEID,ueId,
                     "Memory allocation failed RBID:%d CELLID:%d",
                     entCfg->rbId,
                     cellId);
            RETVALUE(RFAILED); 
         }

         /* copy the RB Cb into UECb */
         kwRbCb->rlcId.rbId = entCfg->rbId;
         if(entCfg->rbType == CM_LTE_SRB)
            ueCb->srbCb[entCfg->rbId] = kwRbCb;
         else
            ueCb->drbCb[entCfg->rbId] = kwRbCb;
         
         KW_LMM_RB_STS_INC(gCb);

      }
      else
      {
         if (entCfg->entMode == CM_LTE_MODE_TM)
         {
            reason = CKW_CFG_REAS_RB_MODE_MIS;
         }
         else
         {
            reason = CKW_CFG_REAS_LCHTYPE_MIS;
         }

         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK, reason);
         RETVALUE(RFAILED);
      }
   }


   kwRbCb->rlcId.cellId = cellId;
   kwRbCb->rlcId.ueId   = ueId;
   kwRbCb->rlcId.rbType = entCfg->rbType;
   kwRbCb->inst         = gCb->init.inst;


   /* Fill RB CB */
   if (kwCfgFillDlRbCb(gCb,kwRbCb, ueCb, entCfg) != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_CREAT_FAIL);

      /* Delete RB CB created */
      KW_FREE(gCb,kwRbCb, sizeof(KwDlRbCb));
      RLOG_ARG2(L_ERROR,DBG_RBID, entCfg->rbId,
               "Filling of RbCb failed UEID:%d CELLID:%d",
               ueId,
               cellId);
      RETVALUE(RFAILED);
   }
   kwRbCb->qci = entCfg->qci;
#ifdef LTE_L2_MEAS
   kwRbCb->ueCb =  ueCb;
   if (entCfg->lCh[0].type == CM_LTE_LCH_DTCH)
   {
      /* ccpu00129778 */
      kwAddToDlL2Meas(gCb, kwRbCb,cellId,ueId); 
   }
#endif /* LTE_L2_MEAS */

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} 


/** 
 * @brief
 *    This primitive reconfigures the existing RB in Ue/Cell Cb.
 *
 * @details
 *    This primitive executes following steps in reconfiguration of existing
 *    RB -
 *    - If UE ID is 0 then
 *       - Check for CELL CB is present
 *       - If yes, Check for RB ID
 *          - If RB ID is present Reconfigure the RB CB
 *          - Else, Status Indication with Reason
 *       - Else, Status Indication with Reason
 *    - Else,
 *       - Check for UE CB is present
 *       - If yes, Check for RB ID
 *          - If RB ID is prenset Reconfigure the CELL CB
 *          - Else, Status Indication with Reason
 *       - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgReCfgDlRb
(
KwCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
CkwEntCfgInfo      *entCfg,
CkwEntCfgCfmInfo   *entCfm
)
#else
PUBLIC S16 kwCfgReCfgDlRb(gCb,ueId, cellId, entCfg, entCfm)
KwCb               *gCb;
CmLteRnti          ueId;
CmLteCellId        cellId;
CkwEntCfgInfo      *entCfg;
CkwEntCfgCfmInfo   *entCfm;
#endif
{
   KwDlRbCb     *rbCb;     /* RB Control Block */
   KwDlRbCb     tRbCb;     /* KW RB Control Block */
   KwDlCellCb   *cellCb;   /* Cell Control Block */
   KwDlUeCb     *ueCb;     /* Ue Control Block */
   U8           ret;

   TRC3(kwCfgReCfgDlRb)

   RLOG_ARG3(L_DEBUG,DBG_UEID,ueId,
             "kwCfgReCfgRb(cellId(%d), cfgType(%d)) RBID:%d",
            cellId, entCfg->cfgType,entCfg->rbId);


   /* Check for UeCb or CellCb */
   if (ueId == 0)
   { 
      if(entCfg->rbId >= KW_MAX_RB_PER_CELL)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                             CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG3(L_ERROR,DBG_RBID,entCfg->rbId,
                  "Invalid RbId , Max is [%d] UEID:%d CELLID:%d",
                  KW_MAX_RB_PER_CELL,
                  ueId,
                  cellId);
         RETVALUE(RFAILED);
      }
      /* Get cellCb */
      kwDbmFetchDlCellCb(gCb,cellId, &cellCb);
      if(!cellCb)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                             CKW_CFG_REAS_CELL_UNKWN);
         RLOG_ARG3(L_ERROR,DBG_CELLID,cellId,
                  "CellCb not found ueId:%d RBID:%d CELLID:%d",
                  ueId,
                  entCfg->rbId,
                  cellId);
         RETVALUE(RFAILED);
      }

      /* Get rbCb */
      KW_DBM_GET_CELL_RBCB(entCfg->rbId, cellCb->rbCb, rbCb);

      if (!rbCb)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG2(L_ERROR,DBG_UEID,ueId,
                  "CELLID:%d RBID:%d not found",
                  cellId,
                  entCfg->rbId);
         RETVALUE(RFAILED);
      }

      /* Take backup of rbCb before updating.
       * Because in failure case restore original rbCb
       */
      cmMemcpy((U8 *)&tRbCb, (U8 *)rbCb, sizeof(KwDlRbCb));

      /* Update rbCb */
      ret = kwCfgUpdateDlRb(gCb,rbCb, cellCb,entCfg);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, 
                             entCfg->rbId, 
                             entCfg->rbType, 
                             CKW_CFG_CFM_NOK,
                             ret);

         RLOG_ARG2(L_ERROR,DBG_UEID,ueId,
                  "CELLID:%u RBID:%d updation failed",
                  cellId,
                  entCfg->rbId);
         cmMemcpy((U8*)rbCb, (U8 *)&tRbCb, sizeof(KwDlRbCb));

         RETVALUE(ret);
      }
   }
   else
   {
      if (!(KW_VALIDATE_UE_RBID(entCfg->rbType, entCfg->rbId)))
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG3(L_ERROR,DBG_UEID,ueId,
                  "CELLID:%d Invalid RBID:%d for RbType[%d]",
                  cellId,
                  entCfg->rbId,
                  entCfg->rbType);
         RETVALUE(RFAILED);
      }
      /* Get ueCb */
      ret = kwDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
         RLOG_ARG2(L_ERROR,DBG_CELLID, cellId,
                  "UEID:%d UeCb not found RBID:%d",
                  ueId,
                  entCfg->rbId);
         RETVALUE(ret);
      }

      /* Get rbCb */
      KW_DBM_GET_RBCB_FROM_UECB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

      if ( rbCb == NULLP)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG2(L_ERROR, DBG_UEID,ueId, 
                  "CELLID:%d RBID:%d not found",
                  cellId,
                  entCfg->rbId);
         RETVALUE(ret);
      }

      /* Take backup of rbCb before updating.
       * Because in failure case restore original rbCb
       */
      cmMemcpy((U8 *)&tRbCb, (U8 *)rbCb, sizeof(KwDlRbCb));

      /* Update rbCb */
      ret = kwCfgUpdateDlRb(gCb,rbCb,ueCb, entCfg);
      if (ret != CKW_CFG_REAS_NONE)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               ret);
         RLOG_ARG2(L_ERROR,DBG_UEID,ueId,
                  "CELLID:%d RBID:%d updation failed",
                  cellId,
                  entCfg->rbId);
         cmMemcpy((U8*)rbCb, (U8 *)&tRbCb, sizeof(KwDlRbCb));

         RETVALUE(ret);
      }
   }

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} 


/** 
 * @brief This primitive deletes the existing RB in Ue/Cell Cb.
 *
 * @details
 *    - If UE ID is 0 then
 *      - Check for CELL CB is present
 *       - If yes, Check for RB ID
 *      - If RB ID is prenset Delete the RB CB
 *       - If there is no RB CB exist in CELL CB then Delete CELL CB.
 *      - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Else,
 *      - Check for UE CB is present
 *      - If yes, Check for RB ID
 *        - If RB ID is prenset Delete the RB CB
 *          - If there is no RB CB exist in UE CB then Delete UE CB.
 *        - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgDelDlRb
(
KwCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
CkwEntCfgInfo      *entCfg,
CkwEntCfgCfmInfo   *entCfm
)
#else
PUBLIC S16 kwCfgDelRb(gCb,ueId, cellId, entCfg, entCfm)
KwCb               *gCb;
CmLteRnti          ueId;
CmLteCellId        cellId;
CkwEntCfgInfo      *entCfg;
CkwEntCfgCfmInfo   *entCfm;
#endif
{
   S16          ret;       /* Return Value */
   KwDlUeCb     *ueCb;     /* UE Control Block */
   KwDlCellCb   *cellCb;   /* UE Control Block */
   KwDlRbCb     *kwRbCb;   /* KW RB Control Block */

   TRC3(kwCfgDelDlRb)

   RLOG_ARG3(L_DEBUG,DBG_UEID,ueId, 
            "kwCfgDelRb(RBID(%d), cellId(%d), cfgType(%d))",
            entCfg->rbId, 
            cellId, 
            entCfg->cfgType);

   ret = ROK;

   /* Get cellCb and delete rbCb from it */
   if (ueId == 0)
   {
      if(entCfg->rbId >= KW_MAX_RB_PER_CELL)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG3(L_ERROR,DBG_RBID,entCfg->rbId ,
                  "Invalid RbId, Max is [%d] UEID:%d CELLID:%d",
                  KW_MAX_RB_PER_CELL,
                  ueId,
                  cellId);
         RETVALUE(RFAILED);
      }
      /* Get cellCb */
      kwDbmFetchDlCellCb(gCb,cellId, &cellCb);
      if (!cellCb)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellId,
                  "CellCb not found UEID:%d RBID:%d",
                  ueId,
                  entCfg->rbId);
         RETVALUE(ret);
      }

      /* Get rbCb */
      KW_DBM_GET_CELL_RBCB(entCfg->rbId, cellCb->rbCb, kwRbCb);

      if ( kwRbCb == NULLP)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG2(L_ERROR, DBG_UEID,ueId,
                  "CellId[%u]:RbId[%d] not found",
                  cellId,
                  entCfg->rbId);
         RETVALUE(ret);
      }

      /* Assign NULLP to rbCb in rbCbLst */
      cellCb->rbCb[entCfg->rbId] = NULLP;

      /* Assign NULLP to dlRbCb/ulRbCb.
       * Delete Hashlist allocated for it if any */
      cellCb->lCh[kwRbCb->lch.lChId - 1].dlRbCb = NULLP;
      KW_FREE(gCb,kwRbCb, sizeof(KwDlRbCb));   /*Vartika: Mem leak fix */  
   }
    /* Get ueCb and delete rbCb from it */
   else
   {
      if (!(KW_VALIDATE_UE_RBID(entCfg->rbType, entCfg->rbId)))
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG3(L_ERROR,DBG_RBID, entCfg->rbId,
                  "Invalid RbId for RbType[%d] UEID:%d CELLID:%d", 
                  entCfg->rbType,
                  ueId,
                  cellId);
         RETVALUE(RFAILED);
      }

      /* Get ueCb */
      ret = kwDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
         RLOG_ARG2(L_ERROR,DBG_CELLID, cellId,
                  "UeId [%d]: UeCb not found RBID:%d",
                  ueId,
                  entCfg->rbId);
         RETVALUE(ret);
      }

      /* Get rbCb */
      KW_DBM_GET_RBCB_FROM_UECB(entCfg->rbId, entCfg->rbType, ueCb, kwRbCb);

      if ( kwRbCb == NULLP)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         RLOG_ARG2(L_ERROR, DBG_UEID,ueId,
                  "CellId[%u]:RbId[%d] not found",
                  cellId,
                  entCfg->rbId);
         RETVALUE(ret);
      }

      ueCb->lCh[kwRbCb->lch.lChId - 1].dlRbCb = NULLP;

#ifdef LTE_L2_MEAS
      KW_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(gCb, kwRbCb);
#endif
      /* Free the Buffers of RbCb */
      if( CM_LTE_MODE_UM == kwRbCb->mode)
      {
         kwUmmFreeDlRbCb(gCb,kwRbCb);
         /* Delete RbCb  */
         KW_FREE(gCb,kwRbCb, sizeof(KwDlRbCb));     
      }
      else if( CM_LTE_MODE_AM == kwRbCb->mode)
      {
         kwAmmFreeDlRbCb(gCb,kwRbCb);
      }

      /* Assign NULLP to rbCb in rbCbLst */
      if ( entCfg->rbType == CM_LTE_SRB )
      {
         ueCb->srbCb[entCfg->rbId] = NULLP;
      }
      else
      {
         ueCb->drbCb[entCfg->rbId] = NULLP;
      }
   }

   KW_LMM_RB_STS_DEC(gCb);

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_OK, 
                       CKW_CFG_REAS_NONE);

   RETVALUE(ret);
} 


/** 
 * @brief This primitive re-establish the existing RB in Ue/Cell Cb.
 *
 * @details
 *    - If UE ID is 0 then
 *      - Check for CELL CB is present
 *      - If yes, Check for RB ID
 *        - If RB ID is prenset initialize the parameters of the RB CB
 *        - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Else,
 *      - Check for UE CB is present
 *      - If yes, Check for RB ID
 *        - If RB ID is prenset initialize the parameters of the RB CB
 *        - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgReEstDlRb
(
KwCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
Bool               sndReEstInd,
CkwEntCfgInfo      *entCfg,
CkwEntCfgCfmInfo   *entCfm
)
#else
PUBLIC S16 kwCfgReEstDlRb(gCb,ueId, cellId,sndReEstInd,entCfg, entCfm)
KwCb               *gCb;
CmLteRnti          ueId;
CmLteCellId        cellId;
Bool               sndReEstInd;
CkwEntCfgInfo      *entCfg;
CkwEntCfgCfmInfo   *entCfm;
#endif
{
   KwDlRbCb     *rbCb;   /* RB Control Block */
   CmLteRlcId   rlcId;   /* RLC Identifier */

   TRC3(kwCfgReEstDlRb)

   RLOG_ARG3(L_DEBUG,DBG_RBID,entCfg->rbId,
            "kwCfgReEstDlRb(ueId(%d), cellId(%d), cfgType(%d))",
            ueId, 
            cellId, 
            entCfg->cfgType);

   /* Get rlcId */
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = entCfg->rbId;
   rlcId.rbType = entCfg->rbType;

   kwDbmFetchDlRbCbByRbId(gCb,&rlcId, &rbCb);
   if (rbCb == NULLP)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rlcId.rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_RB_UNKWN);
      RLOG_ARG2(L_ERROR, DBG_UEID,ueId,
               "CellId[%u]:RbId[%d] not found",
               cellId,
               entCfg->rbId);
      RETVALUE(RFAILED);
   }

   rbCb->rlcId.ueId = ueId;

   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
         {
            kwDlTmmReEstablish(gCb,rbCb);
            break;
         }

      case CM_LTE_MODE_UM:
         {
            kwDlUmmReEstablish(gCb,rlcId,sndReEstInd,rbCb);
            break;
         }

      case CM_LTE_MODE_AM:
         {           
            kwAmmDlReEstablish(gCb, rlcId, rbCb);
            break;
         }
   }

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
      CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} 


/** 
 * @brief This primitive deletes the RBs in Ue Cb.
 *
 * @details
 *    - If UE ID is 0 then
 *      - Status Indication with Reason
 *    - Else,
 *      - Check for UE CB is present
 *      - If yes, Delete all RB CB in UE CB and Delete UE CB also.
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgDelDlUe
(
KwCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
CkwEntCfgInfo      *entCfg,
CkwEntCfgCfmInfo   *entCfm
)
#else
PUBLIC S16 kwCfgDelDlUe(ueId, cellId, entCfg, entCfm)
KwCb               *gCb;
CmLteRnti          ueId;
CmLteCellId        cellId;
CkwEntCfgInfo      *entCfg;
CkwEntCfgCfmInfo   *entCfm;
#endif
{
   S16        ret;     /* Return Value */
   KwDlUeCb   *ueCb;   /* UE Control Block */

   TRC3(kwCfgDelUe)

   RLOG_ARG3(L_DEBUG,DBG_RBID,entCfg->rbId,
             "kwCfgDelUe(ueId(%d), cellId(%d), cfgType(%d))",
            ueId, 
            cellId, 
            entCfg->cfgType);

   ret = ROK;

   /* Check for ueId is present or not */
   if ( ueId == 0 )
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
          CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
      RLOG_ARG2(L_ERROR,DBG_RBID,entCfg->rbId,
               "ueId(%d), cellId(%d)",
               ueId, 
               cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   ret = kwDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
         CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
      RLOG_ARG2(L_ERROR,DBG_CELLID, cellId,
               "UEID:%d UeCb not found RBID:%d",
               ueId,
               entCfg->rbId);
      RETVALUE(RFAILED);
   }

#ifdef LTE_L2_MEAS
   kwDelFrmDlL2Meas(gCb,cellId,ueId);
   kwDbmDelAllDlL2MeasTbFrmUe(gCb,ueCb);
#endif
   /* Delete Ue Cb */
   kwDbmDelDlUeCb(gCb,ueCb, FALSE);

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
      CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} 


/**
 * @brief This primitive deletes the RBs in Ue Cb.
 *
 * @details
 *    - If CELL ID is 0 then
 *      - Status Indication with Reason
 *    - Else,
 *      - Check for CELL CB is present
 *      - If yes, Delete all RB CB in CELL CB and Delete CELL CB also.
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return S16
 *    -#ROK
 *    -#RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgDelDlCell
(
KwCb               *gCb,
CmLteCellId        cellId,
CkwEntCfgInfo      *entCfg,
CkwEntCfgCfmInfo   *entCfm
)
#else
PUBLIC S16 kwCfgDelCell(gCb,cellId, entCfg, entCfm)
KwCb               *gCb;
CmLteCellId        cellId;
CkwEntCfgInfo      *entCfg;
CkwEntCfgCfmInfo   *entCfm;
#endif
{
   S16          ret;       /* Return Value */
   KwDlCellCb   *cellCb;   /* UE Control Block */
   U8           rbId;      /* RB Identifier */

   TRC3(kwCfgDelCell)

   RLOG_ARG2(L_DEBUG,DBG_RBID,entCfg->rbId,
         "kwCfgDelCell( cellId(%d), cfgType(%d)",
         cellId, 
         entCfg->cfgType);

   ret = ROK;
   cellCb = NULLP;
   rbId = entCfg->rbId;

   /* Check for ueId is present or not */
   if ( cellId == 0 )
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_CELL_UNKWN);
      RLOG_ARG1(L_DEBUG,DBG_RBID,entCfg->rbId,
               "cellId is 0 (%d) ",
               cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   kwDbmFetchDlCellCb(gCb,cellId, &cellCb);
   if (!cellCb)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_CELL_UNKWN);
      RLOG_ARG1(L_ERROR, DBG_CELLID,cellId,
               "CellCb not found for RBID:%d",
               entCfg->rbId);
      RETVALUE(RFAILED);
   }

   /* Delete Ue Cb */
   kwDbmDelDlCellCb(gCb,cellCb);

   /* Fill entCfm structure */
  /* kw005.201 added support for L2 Measurement */         
   KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_OK,
                       CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} 

/**
 * @brief This primitive changes the ueId of Ue Cb.
 *
 * @details
 *    - If oldUeId and newUeId are
 *      - Confirm the Status with Reason
 *    - If UeId not present
 *      - Confirm the Status with Reason
 *    - Create New UeCb
 *    - Copy rbCbs from old UeCb to new UeCb
 *    - Delete old UeCb
 *    - Fill entity confirmation
 *
 * @param [in]    ueInfo      -  Old UE Information
 * @param [in]    newUeInfo   -  New UE Information
 * @param [out]   status      -  Status
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgDlUeIdChng
(
KwCb        *gCb,
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo,
CmStatus    *status
)
#else
PUBLIC S16 kwCfgDlUeIdChng(gCb,ueInfo,newUeInfo,status)
KwCb        *gCb;
CkwUeInfo   *ueInfo;
CkwUeInfo   *newUeInfo;
CmStatus    *status;
#endif
{
   KwDlUeCb *ueCb;
/*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   TRC3(kwCfgUeIdChng)

   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      status->reason = CKW_CFG_REAS_SAME_UEID;
      status->status = CKW_CFG_CFM_NOK; 
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueInfo->cellId,
            "Old UeId[%d] same as new UeId[%d]",
            ueInfo->ueId,
            newUeInfo->ueId);
      RETVALUE(RFAILED);
   } 
   
   if(ROK == kwDbmFetchDlUeCb(gCb,newUeInfo->ueId, newUeInfo->cellId, &ueCb))
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, newUeInfo->cellId, 
            "NewUeId[%d]:ueCb already exists",
            newUeInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_EXISTS;
      status->status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   if(ROK != kwDbmFetchDlUeCb(gCb,ueInfo->ueId, ueInfo->cellId, &ueCb))
   {

      RLOG_ARG1(L_ERROR,DBG_CELLID,ueInfo->cellId,
            "UeId [%d]: UeCb not found",
            ueInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_UNKWN;
      status->status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
#ifdef LTE_L2_MEAS
   kwHdlMeasDlUeIdChg(gCb, ueInfo->cellId, ueInfo->ueId, newUeInfo->ueId);
#endif   
   if(ROK != cmHashListDelete(&(gCb->u.dlCb->ueLstCp), (PTR) ueCb))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueInfo->cellId,
            "UeId[%u] HashList Deletion Failed",
            ueInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_CREAT_FAIL;
      status->status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
   
   /* update the hash key with new values */ 
   ueCb->key.ueId = newUeInfo->ueId;
   ueCb->key.cellId = newUeInfo->cellId;

   if(ROK != cmHashListInsert(&(gCb->u.dlCb->ueLstCp), 
                              (PTR)ueCb, (U8 *)&(ueCb->key),
                              (U16) sizeof(KwUeKey)))

   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,newUeInfo->cellId,
            "UeId[%u] HashList Insertion Failed",
            newUeInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_CREAT_FAIL;
      status->status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }  
  
   RETVALUE(ROK);
} 

/********************************************************************30**

         End of file:     gp_cfg_dl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:59 2015

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
/main/1      ---       gk        1. Initial release.
*********************************************************************91*/



