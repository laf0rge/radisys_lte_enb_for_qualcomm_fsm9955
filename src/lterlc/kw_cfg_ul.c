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

        File:     gp_cfg_ul.c

        Sid:      gp_cfg_ul.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:14 2014

        Prg:      gk

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="CFG";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=192;
/** @file gp_cfg_ul.c
@brief RLC Uplink Configuration Module
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
#include "kw_ul.h"

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
#include "kw_ul.x"
#ifdef TENB_STATS
#ifdef L2_L3_SPLIT
#include "l2_tenb_stats.x"    /* Total EnodeB Stats declarations */
#endif
#endif
PUBLIC S16 kwValidateRbCfgParams (KwCb *gCb, CmLteRnti ueId, CmLteCellId cellId,
                                  CkwEntCfgInfo *cfgToValidate, CmStatus *status);
#define KW_MODULE KW_DBGMASK_CFG
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
PRIVATE S16 kwHdlMeasUlUeIdChg(KwCb *gCb, U8 cellId,U8 oldUeId, U8 newUeId)
{
   KwL2MeasCb    *measCb    = NULLP;
   U16           cntr;
   U16          ueIdx = 0;     

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measCb = &(gCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr].measCb);

      if( measCb->measType & LKW_L2MEAS_UL_IP)
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
PRIVATE S16 kwDelFrmUlL2Meas(KwCb *gCb, U8 cellId,U8 ueId)
{
   KwL2MeasCb    *measCb    = NULLP;
   U16           cntr;
   U16          ueIdx = 0;     

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measCb = &(gCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr].measCb);


      if( measCb->measType & LKW_L2MEAS_UL_IP)
      {

         for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
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


#if 1 /* loop for only debugging */

      {
         U32 myIdx =0;
         S16                  ret;           /* Return value */
            KwUlUeCb             *ueCb = NULL;  

            for (myIdx = 0; myIdx < measCb->val.ipThMeas.numUes; myIdx++)
            {
               ueCb = NULL;
               ret = kwDbmFetchUlUeCb(gCb, measCb->val.ipThMeas.ueInfoLst[myIdx].ueId, cellId, &ueCb);
            }
         }

#endif
      }
   }

   RETVALUE(ROK);
}


PRIVATE S16 kwAddToUlL2Meas(KwCb *gCb, KwUlRbCb *kwRbCb,U8 cellId,U8 ueId)
{
   KwL2MeasCb    *measCb    = NULLP;
   U16           cntr;
   U16           cntr1;
   U16            ueIdx = 0;
   U16            qciIdx = 0;
   U16           *numQci;
   U8             freeIdx = gCb->genCfg.maxUe;

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measCb = &(gCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr].measCb);

      freeIdx = gCb->genCfg.maxUe;

      if(measCb->measType & LKW_L2MEAS_ACT_UE )
      {
         for(cntr1 =0;cntr1<measCb->val.nonIpThMeas.numQci;cntr1++)
         {
            if(measCb->val.nonIpThMeas.qci[cntr1] != kwRbCb->qci)
            {
               measCb->val.nonIpThMeas.qci[cntr1]  = kwRbCb->qci;
               gCb->u.ulCb->kwL2Cb.measOn[kwRbCb->qci] |=measCb->measType;
               break;
            }
         }
      }

      if(((kwRbCb->rbL2Cb.measOn & measCb->measType) == LKW_L2MEAS_NONE))
      {
#ifdef LTE_L2_MEAS_RLC
         if (measCb->measType & LKW_L2MEAS_ACT_UE)
         {
            if((kwRbCb->mode == CM_LTE_MODE_UM) &&
                  (kwRbCb->dir & KW_DIR_DL ))
            {
               if (kwRbCb->m.um.umDl.sduQ.count)
               {
                  if (kwRbCb->ueCb->numActRb[kwRbCb->qci] == 0)
                  {
                     kwRbCb->ueCb->numActRb[kwRbCb->qci]++;
                     gCb->u.ulCb->kwL2Cb.numActUe[kwRbCb->qci]++;
                  }
               }
            }
            else if (kwRbCb->mode == CM_LTE_MODE_AM)
            {
               if ((kwRbCb->m.am.amDl.cntrlBo) ||
                     (kwRbCb->m.am.amDl.retxBo)  ||
                     (kwRbCb->m.am.amDl.bo))
               {
                  if (kwRbCb->ueCb->numActRb[kwRbCb->qci] == 0)
                  {
                     kwRbCb->ueCb->numActRb[kwRbCb->qci]++;
                     gCb->u.ulCb->kwL2Cb.numActUe[kwRbCb->qci]++;
                  }
               }
            }
         }
#endif
      }
      if((measCb->measType & LKW_L2MEAS_UL_IP))
      {

         for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
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

         for (qciIdx =0; qciIdx <  *numQci; qciIdx++)
         {
            if (measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] == kwRbCb->qci)
            {
               break;
            }
         }

         if (qciIdx == *numQci)
         {
            measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] = kwRbCb->qci;
            (*numQci)++;
         }

         kwUtlPlcMeasDatInL2Sts(&measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[kwRbCb->qci],
               &kwRbCb->rbL2Cb, measCb->measType);
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
PRIVATE S16 kwCfgFillUlRbCb
(
KwCb            *gCb,
KwUlRbCb        *rbCb,
KwUlUeCb        *ueCb,
CkwEntCfgInfo   *entCfg
)
#else
PRIVATE S16 kwCfgFillUlRbCb(gCb,rbCb, ueCb, entCfg)
KwCb            *gCb;
KwUlRbCb        *rbCb;
KwUlUeCb        *ueCb;
CkwEntCfgInfo   *entCfg;
#endif
{
   TRC3(kwCfgFillUlRbCb)

   RLOG_ARG3(L_DEBUG,DBG_UEID,rbCb->rlcId.ueId, 
                "kwCfgFillRbCb(cellId(%d), rbId(%d), rbType(%d))",
                rbCb->rlcId.cellId, 
                entCfg->rbId, 
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

         rbCb->m.umUl.snLen = entCfg->m.umInfo.ul.snLen;

         /* the bitmask for SN = 10 is 0x3ff and for SN = 5 is 0x1f */
         rbCb->m.umUl.modBitMask = (rbCb->m.umUl.umWinSz << 1) - 1; 

         rbCb->m.umUl.reOrdTmrInt = 
            entCfg->m.umInfo.ul.reOrdTmr;
         cmInitTimers(&(rbCb->m.umUl.reOrdTmr), 1);
         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = rbCb;
         break;
      }
      case CM_LTE_MODE_AM:
      {
         /* Down Link Information 
          * indx = 1 as Up Link   */
         rbCb->lch.lChId  = entCfg->lCh[1].lChId;
         rbCb->lch.lChType = entCfg->lCh[1].type;
         rbCb->dir = KW_DIR_BOTH;

         rbCb->m.amUl.staProhTmrInt = entCfg->m.amInfo.ul.staProhTmr;
         rbCb->m.amUl.reOrdTmrInt = entCfg->m.amInfo.ul.reOrdTmr;
         cmInitTimers(&(rbCb->m.amUl.reOrdTmr), 1);
         cmInitTimers(&(rbCb->m.amUl.staProhTmr), 1);

         rbCb->m.amUl.vrMr = rbCb->m.amUl.vrR + KW_AM_WIN_SZ;

         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = rbCb;

         break;
      }
      default:
      {
         RLOG_ARG2(L_ERROR,DBG_UEID,rbCb->rlcId.ueId, 
                "INVALID RB Mode cellId(%d), rbId(%d)",
                rbCb->rlcId.cellId, 
                entCfg->rbId); 
         RETVALUE(RFAILED);
      }
   }
   rbCb->mode = entCfg->entMode;
   
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
PRIVATE S16 kwCfgUpdateUlRb
(
KwCb            *gCb,
KwUlRbCb        *rbCb,
void            *ptr,
CkwEntCfgInfo   *entCfg
)
#else
PRIVATE S16 kwCfgUpdateUlRb(gCb,rbCb, ptr, entCfg)
KwCb            *gCb;
KwUlRbCb        *rbCb;
void            *ptr;
CkwEntCfgInfo   *entCfg;
#endif
{
   TRC3(kwCfgUpdateUlRb)
   
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
      {
         KwUlCellCb *cellCb = (KwUlCellCb *)ptr;
         rbCb->dir = entCfg->dir;
         rbCb->lch.lChId = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;

         cellCb->lCh[rbCb->lch.lChId - 1].ulRbCb = rbCb;
         break;
      }
      case CM_LTE_MODE_UM:
      {
         KwUlUeCb *ueCb = (KwUlUeCb *)ptr;
         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = NULLP;
         kwCfgFillUlRbCb(gCb,rbCb, ueCb, entCfg);
         break;
      }
      case CM_LTE_MODE_AM:
      {
         KwUlUeCb *ueCb = (KwUlUeCb *)ptr;

         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = NULLP;
         ueCb->lCh[entCfg->lCh[1].lChId - 1].ulRbCb = rbCb;
         /* Up Link */
         rbCb->lch.lChId = entCfg->lCh[1].lChId;
         rbCb->lch.lChType = entCfg->lCh[1].type;
         rbCb->m.amUl.staProhTmrInt = entCfg->m.amInfo.ul.staProhTmr;
         rbCb->m.amUl.reOrdTmrInt = entCfg->m.amInfo.ul.reOrdTmr;
         break;
      }
   }
   RETVALUE(CKW_CFG_REAS_NONE);
} 


/** 
 * @brief 
 *    This primitive validates uplink Add RB Configuration and reserve memory
 *    for RB creation.
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  ueId     -  UE Identifier
 * @param [in]  cellId   -  CELL Identifier
 * @param [in]  cfgToValidate   -  Entity Configuration needs to be validated. 
 * @param [out] status  -  status of configuration
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwValidateRbCfgParams
(
KwCb            *gCb,
CmLteRnti       ueId,
CmLteCellId     cellId,
CkwEntCfgInfo   *cfgToValidate,
CmStatus        *status
)
#else
PUBLIC S16 kwValidateRbCfgParams(gCb,ueId, cellId, cfgToValidate, status)
KwCb            *gCb;
CmLteRnti       ueId;
CmLteCellId     cellId;
CkwEntCfgInfo   *cfgToValidate;
CmStatus        *status;
#endif
{
   if (cellId == 0)
   {
      status->reason = CKW_CFG_REAS_CELL_UNKWN;
      RETVALUE (RFAILED);
   }
  
   if((CKW_CFG_ADD == cfgToValidate->cfgType) ||
      (CKW_CFG_MODIFY == cfgToValidate->cfgType))
   {
      /* Validate LChId for UM and AM modes */
      if ((cfgToValidate->lCh[0].lChId <= 0) ||
           ((cfgToValidate->entMode == CM_LTE_MODE_AM) &&
             (cfgToValidate->lCh[1].lChId <= 0)))
      {
         status->reason = CKW_CFG_REAS_INVALID_LCHID;
         RETVALUE(RFAILED); 
      }  
      if((cfgToValidate->entMode == CM_LTE_MODE_UM) &&
         (cfgToValidate->m.umInfo.ul.snLen != KW_UM_CFG_5BIT_SN_LEN) &&
         (cfgToValidate->m.umInfo.ul.snLen != KW_UM_CFG_10BIT_SN_LEN))
      {   
         RLOG_ARG2(L_ERROR,DBG_UEID,ueId,
               "CellId[%u]:UM Mode RB[%d],Invalid SN Len[%d]",
               cfgToValidate->rbId,
               cfgToValidate->m.umInfo.ul.snLen);
         status->reason = CKW_CFG_REAS_INVALID_SNLEN;
         RETVALUE(RFAILED); 
      }
      /* Process Adding new RB */
      if (ueId == 0)
      {
         if(cfgToValidate->rbId >= KW_MAX_RB_PER_CELL)
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            RETVALUE(RFAILED);
         }

         if ((cfgToValidate->lCh[0].type != CM_LTE_LCH_CCCH) &&
               (cfgToValidate->entMode != CM_LTE_MODE_TM))
         {
            status->reason= (cfgToValidate->entMode != CM_LTE_MODE_TM)? CKW_CFG_REAS_RB_MODE_MIS:
               CKW_CFG_REAS_LCHTYPE_MIS;
            RETVALUE(RFAILED);
         }
      }
      else
      {
         if (!(KW_VALIDATE_UE_RBID(cfgToValidate->rbType, cfgToValidate->rbId)))
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            RETVALUE(RFAILED);
         }

         if(cfgToValidate->entMode == CM_LTE_MODE_TM)
         {
            status->reason = CKW_CFG_REAS_LCHTYPE_MIS;
            RETVALUE(RFAILED);
         }
         if (!(((cfgToValidate->lCh[0].type == CM_LTE_LCH_DCCH) && 
               (cfgToValidate->entMode != CM_LTE_MODE_UM))|| 
               (cfgToValidate->lCh[0].type == CM_LTE_LCH_DTCH)) )
         {
            status->reason = CKW_CFG_REAS_RB_MODE_MIS;
            RETVALUE(RFAILED);
         }
      }
   }
   else /* cfgType is CKW_CFG_DELETE */
   {
      if (ueId == 0)
      {
         if(cfgToValidate->rbId >= KW_MAX_RB_PER_CELL)
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            RETVALUE(RFAILED);
         }

      }
      else
      {
         if (!(KW_VALIDATE_UE_RBID(cfgToValidate->rbType, cfgToValidate->rbId)))
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}


/** 
 * @brief 
 *    This primitive validates uplink Add RB Configuration and reserve memory
 *    for RB creation.
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  cfgToValidate   -  Entity Configuration needs to be validated. 
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 * @param [out] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgValidateUlRb
(
KwCb             *gCb,
CkwEntCfgInfo    *cfgToValidate,
KwUlEntTmpData   *cfgEntData,
KwUlCfgTmpData   *cfgTmpData 
)
#else
PUBLIC S16 kwCfgValidateUlRb(gCb,cfgToValidate, cfgEntData, cfgTmpData)
KwCb             *gCb;
CkwEntCfgInfo    *cfgToValidate;
KwUlEntTmpData   *cfgEntData;
KwUlCfgTmpData   *cfgTmpData; 
#endif
{
   TRC3(kwCfgValidateUlRb)

   RLOG_ARG2(L_DEBUG,DBG_UEID,cfgTmpData->ueId,
               "cellId(%d), cfgType(%d)",
                cfgTmpData->cellId,
                cfgToValidate->cfgType);

   if(ROK != kwValidateRbCfgParams(gCb, 
                                   cfgTmpData->ueId, 
                                   cfgTmpData->cellId, 
                                   cfgToValidate, 
                                   &cfgEntData->entUlCfgCfm.status))
   {
      RETVALUE(RFAILED);
   }
   
   cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_NONE;
   switch(cfgToValidate->cfgType)
   {
      case CKW_CFG_ADD:
      {
         if (cfgTmpData->ueId == 0)
         {
            /* Cell Cb is added if it not present , it is not roll backed if the
             * configuration fails */
            kwDbmFetchUlCellCb(gCb,cfgTmpData->cellId, &cfgTmpData->cellCb);
            if(!cfgTmpData->cellCb)
            {
               /* cell cb does not exist we need to create a new one */
               KW_ALLOC(gCb,cfgTmpData->cellCb, sizeof(KwUlCellCb));
               if(!cfgTmpData->cellCb)
               {
                  RLOG_ARG1(L_FATAL,DBG_UEID,cfgTmpData->ueId,
                        "Memory allocation failure CELLID:%d",
                        cfgTmpData->cellId);
                  cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_CELL_CREAT_FAIL;
                  RETVALUE(RFAILED);
               }
               kwDbmAddUlCellCb(gCb, cfgTmpData->cellId, cfgTmpData->cellCb);
            }
            else
            {
               if (( cfgTmpData->cellCb->rbCb[cfgToValidate->rbId] != NULLP))
               {
                  cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_RB_PRSNT;;
                  RETVALUE(RFAILED);
               }
            }

            KW_ALLOC(gCb,cfgEntData->rbCb, sizeof (KwUlRbCb));
            if (!cfgEntData->rbCb)
            {
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_CREAT_FAIL;
               RETVALUE(RFAILED);
            }
         }
         else
         {
            /* Ue Cb is added if it not present , it is not roll backed if the
             * configuration fails */
            kwDbmFetchUlUeCb(gCb,cfgTmpData->ueId, cfgTmpData->cellId, &cfgTmpData->ueCb);
            if(!cfgTmpData->ueCb)
            {
               KW_ALLOC(gCb,cfgTmpData->ueCb, sizeof(KwUlUeCb));
               if(!cfgTmpData->ueCb)
               {
                  RLOG_ARG1(L_FATAL,DBG_UEID,cfgTmpData->ueId,
                        "Memory allocation failure CELLID:%d",
                        cfgTmpData->cellId);
                  cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_UE_CREAT_FAIL;
                  RETVALUE(RFAILED);
               }
               kwDbmAddUlUeCb(gCb, cfgTmpData->ueId, cfgTmpData->cellId, cfgTmpData->ueCb);
            }
            else
            {
               KW_DBM_GET_RBCB_FROM_UECB(cfgToValidate->rbId, 
                     cfgToValidate->rbType,
                     cfgTmpData->ueCb,
                     cfgEntData->rbCb);
               if(cfgEntData->rbCb != NULLP)
               {
                  cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_RB_PRSNT;;
                  RETVALUE(RFAILED);
               }
            }
            KW_ALLOC(gCb,cfgEntData->rbCb, sizeof (KwUlRbCb));
            if (!cfgEntData->rbCb)
            {
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_CREAT_FAIL;
               RETVALUE(RFAILED);
            }
         }
         /*Allocating the memory for receive buffer */
         if(CM_LTE_MODE_UM == cfgToValidate->entMode)
         {
            U16 winLen;
        
            
            cfgEntData->rbCb->m.umUl.umWinSz = KW_POWER(2, 
                  ((cfgToValidate->m.umInfo.ul.snLen *5)-1));
            winLen =  cfgEntData->rbCb->m.umUl.umWinSz << 1;
            KW_ALLOC(gCb,
                     cfgEntData->rbCb->m.umUl.recBuf, 
                     (winLen * sizeof(KwUmRecBuf*)));
         }
         else if(CM_LTE_MODE_AM == cfgToValidate->entMode)
         {
            KW_ALLOC(gCb,
                     cfgEntData->rbCb->m.amUl.recBuf, 
                     ((KW_AM_WIN_SZ << 1) * sizeof(KwAmRecBuf *)));
         }
         break;
      }
      case CKW_CFG_MODIFY: 
      case CKW_CFG_DELETE:
      {
         if (cfgTmpData->ueId == 0)
         {
            /* Try to get the CellCb if it already exists */
            kwDbmFetchUlCellCb(gCb,cfgTmpData->cellId, &cfgTmpData->cellCb);
            if(!cfgTmpData->cellCb)
            {
                RLOG_ARG1(L_ERROR, DBG_CELLID,cfgTmpData->cellId, 
                      "CellCb not found UEID:%d",
                      cfgTmpData->ueId);
               /*how can a modify request come for a cell which does not exist*/
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_CELL_UNKWN;
               RETVALUE(RFAILED);
            }

            cfgEntData->rbCb = cfgTmpData->cellCb->rbCb[cfgToValidate->rbId];
            if (!cfgEntData->rbCb)
            {
               /* something is wrong the rbId for this cell does not exist */
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_UNKWN;
               RETVALUE(RFAILED);
            }
            RETVALUE(ROK);
         }
         else
         {
            kwDbmFetchUlUeCb(gCb,cfgTmpData->ueId, cfgTmpData->cellId, &cfgTmpData->ueCb);
            if(!cfgTmpData->ueCb)
            {   
               RLOG_ARG1(L_ERROR,DBG_CELLID, cfgTmpData->cellId,
                     "UeId [%d]: UeCb not found",
                     cfgTmpData->ueId);
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_UE_UNKWN;
               RETVALUE(RFAILED);
            }

            /* Get rbCb */
            KW_DBM_GET_RBCB_FROM_UECB(cfgToValidate->rbId, 
                                      cfgToValidate->rbType, 
                                      cfgTmpData->ueCb, 
                                      cfgEntData->rbCb);
            if ( cfgEntData->rbCb == NULLP)
            {
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_UNKWN;
               RETVALUE(RFAILED);
            }
            RETVALUE(ROK);
         }
         break;
      }
   }

   if(cfgToValidate->cfgType == CKW_CFG_MODIFY)
   {
      if(cfgToValidate->entMode != cfgEntData->rbCb->mode)
      {
         cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_MODE_MIS;
         RETVALUE(RFAILED);
      }

      if(cfgToValidate->m.umInfo.ul.snLen != cfgEntData->rbCb->m.umUl.snLen)
      {
         cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_SNLEN_MIS;
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}


/** 
 * @brief 
 *    This primitive roll back the RB Configuration
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  ueId     -  UE Identifier
 * @param [in]  cfg   -     Configuration entity. 
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgRollBackUlRb
(
KwCb             *gCb,
CmLteRnti        ueId,
CkwEntCfgInfo    *cfg,
KwUlEntTmpData   *cfgEntData
)
#else
PUBLIC S16 kwCfgRollBackUlRb(gCb, cfg, cfgEntData)
(
KwCb             *gCb;
CmLteRnti        ueId;
CkwEntCfgInfo    *cfg;
KwUlEntTmpData   *cfgEntData;
)
#endif
{
   TRC3(kwCfgRollBackUlRb)

   if(CKW_CFG_ADD == cfg->cfgType)
   {
      if(CM_LTE_MODE_UM == cfg->entMode)
      {
         KW_FREE(gCb,
               cfgEntData->rbCb->m.umUl.recBuf, 
               (cfgEntData->rbCb->m.umUl.umWinSz << 1) * sizeof(KwUmRecBuf*));
      }
      else if(CM_LTE_MODE_AM == cfg->entMode)
      {
         KW_FREE(gCb,
               cfgEntData->rbCb->m.amUl.recBuf,
               ((KW_AM_WIN_SZ << 1) * sizeof(KwAmRecBuf *)));
      }
      KW_FREE(gCb,cfgEntData->rbCb, sizeof(KwUlRbCb));
   }

   RETVALUE(ROK);
}


/** 
 * @brief 
 *    This primitive apply RB Configuration. 
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] cfgToAply  -   Configuration to be applied 
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 * @param [out] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC Void kwCfgApplyUlRb
(
KwCb             *gCb,
CkwEntCfgInfo    *cfgToAply,
KwUlEntTmpData   *cfgEntData,
KwUlCfgTmpData   *cfgTmpData
)
#else
PUBLIC Void kwCfgApplyUlRb(gCb, cfgToApply, cfgEntData, cfgTmpData)
(
KwCb             *gCb;
CkwEntCfgInfo    *cfgToAply;
KwUlEntTmpData   *cfgEntData;
KwUlCfgTmpData   *cfgTmpData;
)
#endif
{
   TRC3(kwCfgApplyUlRb)
  
   switch(cfgToAply->cfgType)
   {
      case CKW_CFG_ADD:
      {
         /* copy the RB Cb into UeCb */
         cfgEntData->rbCb->rlcId.rbId = cfgToAply->rbId;
         if (cfgTmpData->ueId == 0)
         {
            cfgTmpData->cellCb->rbCb[cfgToAply->rbId] = cfgEntData->rbCb;
            cfgTmpData->cellCb->lCh[cfgToAply->lCh[0].lChId -1].ulRbCb = cfgEntData->rbCb;
         }
         else
         {
            if(cfgToAply->rbType == CM_LTE_SRB)
            {
               cfgTmpData->ueCb->srbCb[cfgToAply->rbId] = cfgEntData->rbCb;
            }
            else
            {
               cfgTmpData->ueCb->drbCb[cfgToAply->rbId] = cfgEntData->rbCb;
            }
         }

         KW_LMM_RB_STS_INC(gCb);

         cfgEntData->rbCb->rlcId.cellId = cfgTmpData->cellId;
         cfgEntData->rbCb->rlcId.ueId   = cfgTmpData->ueId;
         cfgEntData->rbCb->rlcId.rbType = cfgToAply->rbType;
         cfgEntData->rbCb->inst         = gCb->init.inst;

         /* Fill RB CB */
         kwCfgFillUlRbCb(gCb,
                         cfgEntData->rbCb, 
                         cfgTmpData->ueCb,
                         cfgToAply);

#ifdef LTE_L2_MEAS
         cfgEntData->rbCb->qci = cfgToAply->qci;
         cfgEntData->rbCb->ueCb =  cfgTmpData->ueCb;
         if (cfgToAply->lCh[1].type == CM_LTE_LCH_DTCH)
         {
            /* ccpu00129778 */
            kwAddToUlL2Meas(gCb, cfgEntData->rbCb,
                  cfgTmpData->cellId,cfgTmpData->ueId); 
         }
#endif /* LTE_L2_MEAS */
         break;
      }
      case CKW_CFG_MODIFY:
      {
         if(cfgTmpData->ueId == 0)
         {
            kwCfgUpdateUlRb(gCb,
                            cfgEntData->rbCb, 
                            (void *)cfgTmpData->cellCb, 
                            cfgToAply);
         }
         else
         {
            kwCfgUpdateUlRb(gCb,
                            cfgEntData->rbCb, 
                            (void*)cfgTmpData->ueCb, 
                            cfgToAply);
         }
         break;
      }
      case CKW_CFG_DELETE:
      {
         if (cfgTmpData->ueId == 0)
         {
            cfgTmpData->cellCb->rbCb[cfgToAply->rbId] = NULLP;
            cfgTmpData->cellCb->lCh[cfgEntData->rbCb->lch.lChId - 1].ulRbCb = 
                                                                        NULLP;
         }
         else
         {
            cfgTmpData->ueCb->lCh[cfgEntData->rbCb->lch.lChId - 1].ulRbCb = 
                                                                       NULLP;

            /* Free the Buffers of RbCb */
            if( CM_LTE_MODE_UM == cfgEntData->rbCb->mode )
            {
               kwUmmFreeUlRbCb(gCb, cfgEntData->rbCb);
            }
            else if(CM_LTE_MODE_AM == cfgEntData->rbCb->mode)
            {
               kwAmmFreeUlRbCb(gCb,cfgEntData->rbCb);
            }

            /* Assign NULLP to rbCb in rbCbLst */
            if ( cfgToAply->rbType == CM_LTE_SRB )
            {
               cfgTmpData->ueCb->srbCb[cfgToAply->rbId] = NULLP;
            }
            else
            {
               cfgTmpData->ueCb->drbCb[cfgToAply->rbId] = NULLP;
            }
         }
         /* Delete RbCb  */
         KW_FREE(gCb,cfgEntData->rbCb, sizeof(KwUlRbCb));

         KW_LMM_RB_STS_DEC(gCb);
         break;
      }
   }
   RETVOID;
}


/** 
 * @brief 
 *    This primitive validates uplink Delete UE request 
 *    for RB creation.
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  cfgToValidate   -  Entity Configuration to be validated.
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 * @param [out] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgValidateDelUlUe
(
KwCb             *gCb,
CkwEntCfgInfo    *cfgToValidate,
KwUlEntTmpData   *cfgEntData,
KwUlCfgTmpData   *cfgTmpData
)
#else
PUBLIC S16 kwCfgValidateDelUlUe(gCb,cfgToValidate, cfgEntData,cfgTmpData)
KwCb             *gCb;
CkwEntCfgInfo    *cfgToValidate;
KwUlEntTmpData   *cfgEntData;
KwUlCfgTmpData   *cfgTmpData;
#endif
{
   TRC3(kwCfgValidateDelUlUe)

   RLOG_ARG2(L_DEBUG,DBG_UEID,cfgTmpData->ueId, 
             "cellId(%d), cfgType(%d)",
              cfgTmpData->cellId,cfgToValidate->cfgType);
   
   /* Check for ueId is present or not */
   if ( cfgTmpData->ueId == 0 )
   {
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_UE_UNKWN;;
      RLOG_ARG1(L_ERROR,DBG_UEID,cfgTmpData->ueId, 
            "UeId is 0 for CELLID;%d",
            cfgTmpData->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   if(ROK != kwDbmFetchUlUeCb(gCb,cfgTmpData->ueId, cfgTmpData->cellId, &cfgTmpData->ueCb))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID, cfgTmpData->cellId,
            "UeId [%d]: UeCb not found",
            cfgTmpData->ueId);
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_UE_UNKWN;;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/** 
 * @brief 
 *    This primitive apply Uplink Delete Ue request 
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 */
#ifdef ANSI
PUBLIC Void kwCfgApplyDelUlUe
(
KwCb             *gCb,
KwUlCfgTmpData   *cfgTmpData
)
#else
PUBLIC Void kwCfgApplyDelUlUe(gCb, cfgTmpData)
(
KwCb             *gCb;
KwUlCfgTmpData   *cfgTmpData;
)
#endif
{
   TRC3(kwCfgApplyDelUlUe)
   
#ifdef LTE_L2_MEAS
   kwDelFrmUlL2Meas(gCb,cfgTmpData->cellId,cfgTmpData->ueId);
#endif
   kwDbmDelUlUeCb(gCb,cfgTmpData->ueCb, FALSE);
   RETVOID;
}
  

/** 
 * @brief 
 *    This primitive validates uplink Delete UE request 
 *    for RB creation.
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] ueId     -  UE Identifier
 * @param [in] cfgToValidate   -  Entity Configuration to be done.
 * @param [in] cfgEntData    -  Configuration Temporary Data Entity
 * @param [in] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgValidateDelUlCell
(
KwCb             *gCb,
CmLteCellId      cellId,
CkwEntCfgInfo    *cfgToValidate,
KwUlEntTmpData   *cfgEntData,
KwUlCfgTmpData   *cfgTmpData
)
#else
PUBLIC S16 kwCfgValidateDelUlCell(gCb, cellId, cfgToValidate, cfgTmpData)
KwCb             *gCb;
CmLteCellId      cellId;
CkwEntCfgInfo    *cfgToValidate;
KwUlEntTmpData   *cfgEntData;
KwUlCfgTmpData   *cfgTmpData;
#endif
{
   TRC3(kwCfgValidateDelUlCell)

   RLOG_ARG1(L_DEBUG,DBG_CELLID,cellId ,"cfgType(%d)",
              cfgToValidate->cfgType);
   
   cfgTmpData->cellCb = NULLP;

   /* Check for cellId is present or not */
   if ( cellId == 0 )
   {
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_CELL_UNKWN;;
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId , "CellId is 0");
      RETVALUE(RFAILED);
   }

   /* Fetch Cell Cb */
   kwDbmFetchUlCellCb(gCb,cellId, &cfgTmpData->cellCb);
   if (!cfgTmpData->cellCb)
   {
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_CELL_UNKWN;;
      RLOG_ARG0(L_ERROR, DBG_CELLID,cellId, "CellCb not found");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/** 
 * @brief 
 *    This primitive apply Uplink Delete Ue request 
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] cfgEntData  - Temporary Data maintained for a transaction 
 *
 * @return  S16
 *    -# ROK
 */
#ifdef ANSI
PUBLIC Void kwCfgApplyDelUlCell
(
KwCb             *gCb,
KwUlCfgTmpData   *cfgInfo
)
#else
PUBLIC Void kwCfgApplyDelUlCell(gCb, cfgEntData)
(
KwCb             *gCb;
KwUlCfgTmpData   *cfgInfo;
)
#endif
{
   TRC3(kwCfgApplyDelUlCell)
   
   kwDbmDelUlCellCb(gCb,cfgInfo->cellCb);
   RETVOID;
}

/** 
 * @brief 
 *    This primitive validates reestablishment of RB.
 *  
 * @param [in]   gCb      -  RLC Instance Control Block
 * @param [in]   ueId     -  UE Identifier
 * @param [in]   cellId   -  CELL Identifier
 * @param [in]   cfgToValidate   -  Entity Configuration to be done.
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgValidateReEstRb
(
KwCb             *gCb,
CmLteRnti        ueId,
CmLteCellId      cellId,
CkwEntCfgInfo    *cfgToValidate,
KwUlEntTmpData   *cfgEntData
)
#else
PUBLIC S16 kwCfgValidateReEstRb(gCb,ueId, cellId, cfgToValidate, cfgEntData)
KwCb             *gCb;
CmLteRnti        ueId;
CmLteCellId      cellId;
CkwEntCfgInfo    *cfgToValidate;
KwUlEntTmpData   *cfgEntData;
#endif
{
   CmLteRlcId   rlcId;   /* RLC Identifier */
   TRC3(kwCfgValidateReEstRb)

   RLOG_ARG2(L_DEBUG, DBG_UEID,ueId,
         "cellId(%d) RBID:%d",
         cellId,
         cfgToValidate->rbId); 

   /* Get rlcId */
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = cfgToValidate->rbId;
   rlcId.rbType = cfgToValidate->rbType;

   kwDbmFetchUlRbCbByRbId(gCb,&rlcId, &cfgEntData->rbCb);
   if (cfgEntData->rbCb == NULLP)
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,ueId, 
            "CellId [%u]: rbId [%d] not found",
             cellId,
             cfgToValidate->rbId);
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_RB_UNKWN;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}



/** 
 * @brief 
 *    This primitive apply reestablishment of RB.
 *  
 * @param [in] gCb        -  RLC Instance Control Block
 * @param [in] ueId       -  UE Identifier
 * @param [in] cellId     -  CELL Identifier
 * @param [in] sndReEstInd  -  ReEstablish Indication Flag 
 * @param [in] cfgEntData -  Configuration Temporary Data Entity 
 *
 * @return  S16
 *    -# ROK
 */
#ifdef ANSI
PUBLIC Void kwCfgApplyReEstUlRb
(
KwCb             *gCb,
CmLteRnti        ueId,
CmLteCellId      cellId,
Bool             sndReEstInd,
KwUlEntTmpData   *cfgEntData
)
#else
PUBLIC Void kwCfgApplyReEstUlRb(gCb, ueId, cellId, sndReEstInd, cfgEntData)
(
KwCb             *gCb;
CmLteRnti        ueId;
CmLteCellId      cellId;
Bool             sndReEstInd;
KwUlEntTmpData   *cfgEntData;
)
#endif
{
   CmLteRlcId   rlcId;   /* RLC Identifier */
   TRC3(kwCfgApplyReEstUlRb)
   
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = cfgEntData->entUlCfgCfm.rbId;
   rlcId.rbType = cfgEntData->entUlCfgCfm.rbType;
   cfgEntData->rbCb->rlcId.ueId = ueId;
   switch (cfgEntData->rbCb->mode)
   {
      case CM_LTE_MODE_TM:
         {
            kwTmmUlReEstablish(gCb,cfgEntData->rbCb);
            break;
         }

      case CM_LTE_MODE_UM:
         {
            kwUmmUlReEstablish(gCb,&rlcId,cfgEntData->rbCb);
            break;
         }

      case CM_LTE_MODE_AM:
         {
            kwAmmUlReEstablish(gCb,rlcId,sndReEstInd,cfgEntData->rbCb);
            break;
         }
   }
   RETVOID;
}

/** 
 * @brief 
 *    This primitive validates reestablishment of RB.
 *  
 * @param [in] gCb        -  RLC Instance Control Block
 * @param [in] ueInfo     -  UE Identifier
 * @param [in] newUeInfo  -  CELL Identifier
 * @param [in] cfgTmpData -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwCfgValidateUeIdChng
(
KwCb             *gCb,
CkwUeInfo        *ueInfo,
CkwUeInfo        *newUeInfo,
KwUlCfgTmpData   *cfgTmpData
)
#else
PUBLIC S16 kwCfgValidateUeIdChng(gCb,ueInfo,newUeInfo,cfgTmpData)
KwCb             *gCb;
CkwUeInfo        *ueInfo;
CkwUeInfo        *newUeInfo;
KwUlCfgTmpData   *cfgTmpData;
#endif
{
   KwUlUeCb   *ueCb;
   TRC3(kwCfgValidateUeIdChng)

#define CFM_STATUS  cfgTmpData->cfgEntData[0].entUlCfgCfm.status
   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      CFM_STATUS.reason = CKW_CFG_REAS_SAME_UEID;
      CFM_STATUS.status = CKW_CFG_CFM_NOK; 
      RETVALUE(RFAILED);
   } 
   
   if(ROK == kwDbmFetchUlUeCb(gCb,newUeInfo->ueId, newUeInfo->cellId, &ueCb))
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, newUeInfo->cellId, 
            "NewUeId[%d]:ueCb already exists",
             newUeInfo->ueId);
      CFM_STATUS.reason = CKW_CFG_REAS_UE_EXISTS;
      CFM_STATUS.status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   if(ROK != kwDbmFetchUlUeCb(gCb,ueInfo->ueId, ueInfo->cellId, 
                              &cfgTmpData->ueCb))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID, ueInfo->cellId,
            "UeId [%d]: UeCb not found",
            ueInfo->ueId);
      CFM_STATUS.reason = CKW_CFG_REAS_UE_UNKWN;
      CFM_STATUS.status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
#undef CFM_STATUS
   RETVALUE(ROK);
}



/** 
 * @brief 
 *    This primitive apply reestablishment of RB.
 *  
 * @param [in] gCb        -  RLC Instance Control Block
 * @param [in] ueInfo     -  UE Identifier
 * @param [in] newUeInfo  -  CELL Identifier
 * @param [in] cfgTmpData -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 */
#ifdef ANSI
PUBLIC Void kwCfgApplyUlUeIdChng
(
KwCb             *gCb,
CkwUeInfo        *ueInfo,
CkwUeInfo        *newUeInfo,
KwUlCfgTmpData   *cfgTmpData
)
#else
PUBLIC Void kwCfgApplyUlUeIdChng(gCb, ueId, cellId, cfgTmpData)
(
KwCb             *gCb;
CkwUeInfo        *ueInfo,
CkwUeInfo        *newUeInfo,
KwUlCfgTmpData   *cfgTmpData;
)
#endif
{
   TRC3(kwCfgApplyUlUeIdChng)
   
#ifdef LTE_L2_MEAS
   kwHdlMeasUlUeIdChg(gCb, ueInfo->cellId, ueInfo->ueId, newUeInfo->ueId);
#endif   
   cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) cfgTmpData->ueCb);
   /* update the hash key with new values */ 
   cfgTmpData->ueCb->key.ueId = newUeInfo->ueId;
   cfgTmpData->ueCb->key.cellId =newUeInfo->cellId;
   if(ROK != cmHashListInsert(&(gCb->u.ulCb->ueLstCp),
                              (PTR)cfgTmpData->ueCb, 
                              (U8 *)&(cfgTmpData->ueCb->key),
                              (U16) sizeof(KwUeKey)))

   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,newUeInfo->cellId,
            "UeId[%u] HashList Insertion Failed",
            newUeInfo->ueId);
   }

   RETVOID;
}
/********************************************************************30**

         End of file:     gp_cfg_ul.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:14 2014

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

