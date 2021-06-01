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

        Name:    LTE PDCP - Configuration Manager file

        Type:    C source file

        Desc:    It contains the following configuraiton primtives
                    -- pjCfgAddRb 
                    -- pjCfgReCfgRb 
                    -- pjCfgDelRb 
                    -- pjCfgReEstRb 
                    -- pjCfgDelUe 
                    -- pjCfgPrcSecCfg 
                    -- pjCfgUeIdChng 
                    -- pjCfgPrcCount 
                    -- pjCfgPrcSduSta 

        File:     gp_pj_cfg.c

        Sid:      eq_cfg_dl.c@@/main/TeNB_Main_BR/8 - Mon Aug 11 16:42:44 2014

        Prg:      rbabu

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=212;
/** @file gp_pj_cfg.c
@brief LTE PDCP Configuration Module
**/


/* header (.h) include files */
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm5.h"                /* Timer Functions */
#include "cm_lte.h"             /* common LTE header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "cpj.h"                /* RRC layer */
#include "pju.h"                /* PDCP service user */
#include "lpj.h"                /* RRC layer */
#include "pj_env.h"             /* RLC environment options */
#include "pj.h"                 /* RLC layer */
#include "pj_dl.h"
#include "pj_udx.h"
#include "pj_err.h"



/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common LTE file */
#include "cm_llist.x"           /* common list header file */
#include "cpj.x"                /* RRC layer */
#include "pju.x"                /* PDCP service user */
#include "lpj.x"                /* LM Interface */
#include "pj.x"                 /* RLC layer */
#include "pj_udx.x"
#include "pj_dl.x"

/* Proto types declarations for local functions */
 
PRIVATE S16 pjCfgFillPjDlRbCb ARGS ((
PjCb                *gCb,
PjDlRbCb            *rbCb,
PjDlUeCb          *ueCb,
U8                 isHo,
UdxCfgEnt       *cfgEnt,
UdxCfmEnt       *cfmEnt
));

PRIVATE S16 pjCfgUpdDlCompInfo  ARGS ((
PjCb                   *gCb, 
PjDlRbCb               *rbCb,
PjDlUeCb               *ueCb,
UdxHdrCompCfg        *rohcCfg
));

#ifdef LTE_L2_MEAS
PRIVATE S16 pjAddToDlL2Meas ARGS ((
PjCb *gCb, 
PjDlRbCb *kwRbCb,
U8 cellId,
U8 ueId
));
#endif
/* Store the Profile ID into array index */
/* RRC is not sending the profile id directly. It will give
   the for all the profiles with True/False. PDCP will derive the profile
   based on this profile Id list */

PRIVATE U16  rohcProfId[CPJ_MAX_ROHC_PROF_LIST]=    { 0x0001, 0x0002, 0x0003, 0x0004,
                                              0x0006, 0x0101, 0x0102, 0x0103, 
                                              0x0104};

/*Counter incremented whenever there is key change*/
PUBLIC U8 keyChange;

/** @addtogroup cfgdata */
/*@{*/
/** 
* @brief
* 
*   Hanlder to process the configuration recieved from the upper layer and update the pdcp
*   data base.
*
* @b Description: 
*     1. This function create a new  PDCP RB Cb. @n
*     2. Newly created PDCP RB Cb will be placed in Data base with the  configuration
*     informtion received.  @n
*     3. Return Success or Failure to the caller with reason and status
*     information. @n
 
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation.
 * 
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 pjCfgAddDlRb
(
PjCb              *gCb,
CmLteRnti         ueId,
CmLteCellId       cellId,
U8                 isHo,
UdxCfgEnt         *entCfg,
PjDlUeCb          **ueCb,
UdxCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgAddDlRb (gCb, ueId, cellId,isHo, entCfg, ueCb, entCfm)
PjCb              *gCb;
CmLteRnti         ueId;
CmLteCellId       cellId;
U8                 isHo;
CpjCfgEnt         *entCfg;
PjDlUeCb          **ueCb; 
CpjCfmEnt         *entCfm;
#endif
{
   S16               ret;
   PjDlRbCb          *rbCb;  /* Rb Cb Pointer */

   TRC3(pjCfgAddDlRb);  

   /* Initialization */
   ret  = ROK;

   RLOG2(L_DEBUG, "pjCfgAddRb- Received with -- cellId(%d),ueId(%d)",
                   cellId, ueId);

   if( NULLP == *ueCb)
   { 
      /* Ue Cb is not present, Create Ue Cb */   
      if ( pjDbmCreateDlUeCb(gCb, ueId, cellId, ueCb) != ROK)
      {
         /* Ue Cb allocation failed  */   
         PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
               CPJ_CFG_REAS_UE_CREAT_FAIL);

         RLOG_ARG1(L_ERROR,DBG_UEID,ueId, "CellId[%d] Creating DL UeCb Failed",
                            cellId);
         RETVALUE(ret);
      } 
   }
 
   (*ueCb)->inst = gCb->init.inst;
#ifndef L2_L3_SPLIT
#ifdef TENB_STATS
   (*ueCb)->tenbStats = TSL2MapUeStatsBlk(ueId);
#endif
#endif

   /* Rb Id is allowed range or not */   
   PJ_VALIDATE_RBID(ueId, entCfg->rbId, entCfg->rbType, ret);
   if ( ROK != ret)
   { 
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
         CPJ_CFG_REAS_INVALID_RBID);

      RLOG_ARG2(L_ERROR,DBG_UEID,ueId, "CellId[%u]:RbId[%d] - "
            "Invalid RB ID,RbCb Creation Failed ", cellId, entCfg->rbId);
      RETVALUE(ret);
   }

   /* Find Rb Cb already exist or not */   
   PJ_DBM_FETCH_DL_RBCB(entCfg->rbId, entCfg->rbType, (*ueCb), rbCb);
   if (NULLP != rbCb)
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,ueId, "CellId [%u]:RbId[%d] already exists",
            cellId,entCfg->rbId);
      /* Rb Cb already exist for pdpcp */   
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_SAME_RBID);

      RLOG_ARG2(L_ERROR,DBG_UEID,ueId, "RB Id already present-- cellId(%d), rbId(%d)",
                      cellId, entCfg->rbId);
      RETVALUE(RFAILED);
   }
   else if (NULLP == rbCb)
   {
      /* Create Rb Cb and place in Ue Cb */   
      rbCb =   pjDbmCreateDlRbCb(gCb, entCfg->rbId, entCfg->rbType, *ueCb, PJ_CFG_PDCP); 
      if(NULLP == rbCb)
      {
         /* Create Rb Cb Failed*/   
         PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
               CPJ_CFG_REAS_ENT_CREAT_FAIL);

         RLOG_ARG2(L_ERROR,DBG_UEID,ueId, "Failed to Create RB CB -- cellId(%d) rbId(%d)",
                         cellId, entCfg->rbId);
         RETVALUE(RFAILED);
      } 
   }

   /* Copy the Recived information into RbCb */ 
   ret = pjCfgFillPjDlRbCb(gCb, rbCb, *ueCb,isHo, entCfg , entCfm);

   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);

   RETVALUE(ret);
} /* pjCfgAddRb */

/**
* @brief 
*
*   Hanlder to update the PJRB with the received infromation during
*   configuration or re configuration. 
*
* @b Description: 
*     1. update the data base with the  new configuration informtion received.  @n
*     2. update the data base with the  new re-configuration informtion received.  @n
*     3. If required Initiliaze the tx/rx Buffers and Timers. @n
*     4. Return Success or Failure to the caller with reason and status
*     information. @n
*
 * @param [out]   rbCb     -  RB Control Block
 * @param [out]   entCfm   -  PDCP Entiry Confirmation 
 * @param [in]    ueCb     -  UE Control Block
 * @param [in]    entCfg   -  PDCP Entity Configuration
 *
 * @return  ROK
 */
#ifdef ANSI
PRIVATE S16 pjCfgFillPjDlRbCb
(
PjCb            *gCb,
PjDlRbCb        *pjRbCb,
PjDlUeCb        *ueCb,
U8              isHo,
UdxCfgEnt       *cfgEnt,
UdxCfmEnt       *cfmEnt
)
#else
PRIVATE S16 pjCfgFillPjDlRbCb(gCb, pjRbCb, ueCb,isHo, cfgEnt, cfmEnt)
PjCb            *gCb;
PjDlRbCb        *pjRbCb;
PjDlUeCb        *ueCb;
U8              isHo,
CpjCfgEnt       *cfgEnt;
CpjCfmEnt       *cfmEnt;
#endif
{
   S16            ret;
   S16            tmrVal;    /* Timer Value */
   Buffer         *bMapBuf;  /* Bitmap status report buffer      */
   MsgLen         bitMapLen; /* Bit Map length */
   /* kw005.201 added support for L2 measurement */
   TRC3(pjCfgFillPjDlRbCb);

   /* Initialization */
   ret     = ROK;
   bMapBuf = NULLP;
   tmrVal  = 0; 
   bitMapLen = 0;

   RLOG_ARG3(L_DEBUG,DBG_UEID,ueCb->key.ueId, "Received Fill Req cellId(%d), rbId(%d),cfgType(%d)",
                           ueCb->key.cellId,
                       cfgEnt->rbId, cfgEnt->cfgType);

   switch (cfgEnt->cfgType)
   {
      /* Cfg Type received as ADD RB */
      case CPJ_CFG_ADD:
         {
            pjRbCb->state  =   PJ_STATE_NORMAL;
            pjRbCb->rbId   =   cfgEnt->rbId;
            pjRbCb->rbType =   cfgEnt->rbType;
            pjRbCb->mode   =   cfgEnt->m.addCfg.rlcInfo.mode;
            pjRbCb->dir    =   cfgEnt->m.addCfg.rlcInfo.dir;
            pjRbCb->ueCb   =   ueCb;

            if(PJ_SRB == pjRbCb->rbType)
            {
               /* SN length is fixed for SRB */
               pjRbCb->snLen  = PJ_SRB_SN_LEN;
               ueCb->numSrbs++;
            }
            else
            {
               if (CM_LTE_MODE_UM == pjRbCb->mode)
               {
                  /* SN length can be 7 bit or 12 bit in UM mode */
                  pjRbCb->snLen  =   cfgEnt->m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize;
               }
               else
               {
                  /* SN length can be 12/15 bit for AM mode */
                  pjRbCb->snLen  =   cfgEnt->m.addCfg.rlcInfo.m.rlcAm.pdcpSNSize;
                  if (isHo)
                  {
                     pjRbCb->state  =   PJ_STATE_HO;
                  }
               }
               ueCb->numDrbs++;
            }
            pjRbCb->maxTxSn = (1 << pjRbCb->snLen);
            /* Initialize all off board  timers */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
            cmInitTimers(&pjRbCb->dlCb.obdTmr,PJ_RB_MAX_TIMERS); 
#endif                  

            RLOG_ARG2(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
                  "Received RB Req with -- Dir(%d), mode(%d)",
                       pjRbCb->dir, pjRbCb->mode);

           
            if (PJ_DIR_DL &  pjRbCb->dir)
            {
               pjRbCb->dlCb.discTmrInt = cfgEnt->m.addCfg.discardTmr;
               pjRbCb->dlCb.cfmReqd    = cfgEnt->m.addCfg.cfmReqd;
               /* pjRbCb->dlCb.count      = 0xffffffff; */

               /* Discard Timer is Enabled */
               if(pjRbCb->dlCb.discTmrInt > 0 )
               {
                  cmInitTimers(&pjRbCb->dlCb.discTmr,PJ_RB_MAX_TIMERS); 
                  /* 1. The discTmr value is absolute value in ms units 
                     2. Store the DiscardTmr value in terms of Layer resolution
                     ie discTmrVal = dicTrmVal/(SS_TICKS*Layer Resolution) 
                     and round to next integer value.
                   */
                  tmrVal = (S16)(pjRbCb->dlCb.discTmrInt % (SS_TICKS_SEC
                        * gCb->pjGenCfg.timeRes));  /*KW_FIX*/
                  pjRbCb->dlCb.discTmrInt = (S16)(pjRbCb->dlCb.discTmrInt / 
                                            (SS_TICKS_SEC * gCb->pjGenCfg.timeRes));
                  if(tmrVal)
                     pjRbCb->dlCb.discTmrInt +=1; /* Round to next integer value if
                                                     required */          
               }

               /*Initialization for Tx Buf */
               gCb->pjGenCfg.nmbDlBins = PJ_MAX_HASH_BINS;
               pjDbmDlBufInit(gCb, &pjRbCb->dlCb.txBuf,gCb->pjGenCfg.nmbDlBins);
               cmLListInit(&pjRbCb->dlCb.dlPktQ);
            }

            /* Update the Compressor Informatio in Rb Cb */
            if(cfgEnt->rbType == CM_LTE_DRB)
            {
               pjCfgUpdDlCompInfo(gCb, pjRbCb, ueCb,&(cfgEnt->m.addCfg.hdrCompCfg));
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
               pjRbCb->qci =  cfgEnt->m.addCfg.qci;
               pjAddToDlL2Meas(gCb,pjRbCb,
                               ueCb->key.cellId,ueCb->key.ueId); 
#endif
            }

            break;
         }

         /* Cfg Type received as Modify RB */
      case CPJ_CFG_MODIFY:
         {
            if(CPJ_RECFG_CFMREQ & cfgEnt->m.modCfg.bitFlag)
               pjRbCb->dlCb.cfmReqd    = cfgEnt->m.modCfg.cfmReqd;

            /* Header Compression profile has been modified */

            if(TRUE == cfgEnt->m.modCfg.hdrCompCfg.hdrCompUsed)
            {
               /* Close All the channels */
               pjUtlDlCmpReset(gCb, pjRbCb); 
               pjCfgUpdDlCompInfo(gCb, pjRbCb, ueCb, &cfgEnt->m.modCfg.hdrCompCfg);
            }

            /* In case of Hand off scenario: At Target eNode-B, 
               Handoff information will send in re-cfg req */

            if(TRUE  & cfgEnt->m.modCfg.hoInfo.isHoPres)
            {
               /* Store the HO information  and call for Status Report
                * Generation also */
               if(CPJ_HO_DL & cfgEnt->m.modCfg.hoInfo.isHoPres)
               {
                  pjRbCb->dlCb.count = cfgEnt->m.modCfg.hoInfo.dlCount - 1;
                  if (pjRbCb->snLen != cfgEnt->m.modCfg.hoInfo.snLen)
                  {
                     pjRbCb->snLen = cfgEnt->m.modCfg.hoInfo.snLen;
                  }
                  PJ_GET_SN_HFN(cfgEnt->m.modCfg.hoInfo.dlCount, pjRbCb->snLen, 
                            pjRbCb->dlCb.nxtTxSn, pjRbCb->dlCb.txHfn);
               }
               pjDlmProcessDlPktQ(gCb, pjRbCb);
               pjRbCb->state = PJ_STATE_NORMAL;
            }

            break;
         }

         /* Un Expected cfg type */
      default:
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID, ueCb->key.cellId, 
                  "pjCfgFillRbCb Failed -- for the invalid cfgType(%d)",
                     cfgEnt->cfgType);
         }
   }
   RETVALUE(ROK);
} /* pjCfgFillPjRbCb */



/**
* @brief 
*
*   Hanlder to process the re-configuration for existing PJRB from the upper
*   layer.
*
* @b Description: 
*     1. This function fetch the PJRB from pdcp data base. @n
*     2. update the data base with the  new configuration informtion received.  @n
*     3. Return Success or Failure to the caller with reason and status
*     information. @n
*
* @param [in]    gCb      -  PDCP Instance Control block
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgReCfgDlRb
(
PjCb              *gCb,
PjDlUeCb          *ueCb,
UdxCfgEnt         *entCfg,
UdxCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgReCfgDlRb (gCb,ueCb, entCfg, entCfm)
PjCb              *gCb;
PjDlUeCb          *ueCb;
UdxCfgEnt         *entCfg;
UdxCfmEnt         *entCfm;
#endif
{
   S16       ret;
   PjDlRbCb     *rbCb;    /* Rb Cb */

   TRC3(pjCfgReCfgDlRb);

   /* Initialization */
   ret  = ROK;

   RLOG2(L_DEBUG, "pjCfgReCfgRb - Received for -- cellId(%d),ueId(%d)",
                          ueCb->key.cellId, ueCb->key.ueId);

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_DL_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);
   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == rbCb)
   { 
      RLOG_ARG2(L_ERROR, DBG_UEID,ueCb->key.ueId, "CellId [%u]:RbId[%d] already exists",
            ueCb->key.cellId,entCfg->rbId);
      /* Fill entCfm structure */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_PDCPID_UNKWN);

      RLOG_ARG2(L_ERROR,DBG_UEID,ueCb->key.ueId, 
            "RB Cb is not present -- cellId(%d), rbId(%d)",
                          ueCb->key.cellId, entCfg->rbId);
      RETVALUE(RFAILED);
   }

   /* Copy the received infromation in to Rb Cb */
   ret = pjCfgFillPjDlRbCb(gCb, rbCb, ueCb,FALSE, entCfg , entCfm);

   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
} /* pjCfgReCfgRb */


/**
* @brief
*  This function deletes the existing RB(PDCP and RLC) from  Ue Cb.
*
* @b Description:
*    1. Stop the timers and deallocate tx/rx Buffs for PJRB CB. @n
*    2. Delete the existing RB Cb(PDCP and RLC)  from Ue Cb. @n
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgDelDlRb
(
PjCb              *gCb,
PjDlUeCb          *ueCb,
UdxCfgEnt         *entCfg,
UdxCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgDelDlRb (gCb, ueCb, entCfg, entCfm)
PjCb              *gCb;
PjDlUeCb          *ueCb;
UdxCfgEnt         *entCfg;
UdxCfmEnt         *entCfm;
#endif
{
   S16      ret;
   PjDlRbCb     *rbCb;    /* Rb Cb */

   TRC3(pjCfgDelDlRb);

   /* Initialization */
   ret = ROK;

   RLOG2(L_DEBUG, "pjCfgDelRb- Received for -- cellId(%d),ueId(%d)",
                          ueCb->key.cellId,ueCb->key.ueId);

   /* Fetch RB Cb */
   PJ_DBM_FETCH_DL_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* RB Cb is not present   or not configured for PDCP */
   if (NULLP == rbCb) 
   { 
      RLOG_ARG2(L_ERROR, DBG_UEID,ueCb->key.ueId, "CellId [%u]:RbId[%d] already exists",
            ueCb->key.cellId,entCfg->rbId);
      /* Fill entCfm structure with invalid pdcp Id */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_PDCPID);

      RLOG_ARG2(L_ERROR,DBG_UEID,ueCb->key.ueId, 
            "RB Cb is not found, Deletion Failed -- cellId(%d), rbId(%d)",
                          ueCb->key.cellId, entCfg->rbId);
      RETVALUE(RFAILED);
   }
   /* Delete PDCP Related infromation from RB CB */
   ret= pjCfgDelPjDlRbCb(gCb, ueCb, rbCb); 

   if(ROK != ret)
   {
      /* Fill entCfm structure with invalid pdcp Id */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_PDCPID);

      RLOG_ARG2(L_ERROR,DBG_UEID,ueCb->key.ueId, 
            "RB Cb Deletion Failed -- cellId(%d), rbId(%d)",
                          ueCb->key.cellId, entCfg->rbId);
      RETVALUE(ret);
   }
   rbCb->pktAdmitCnt= 0;
   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
} /* pjCfgDelRb */




/**
*  @brief
*
*   This primitive is used to re-establish downlink SRB2 and/or DRB'S during RRC connection
*   re-configuration procedure.
*  
*  @b Description:
*  1. This function will trigger re-establishment for SRB2 and/or DRB's
*
* @param [in]    gCb      -  PDCP Instance Control block
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Configuration cfm.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgReEstDlRb
(
PjCb              *gCb,
PjDlUeCb          *ueCb,
UdxCfgEnt         *entCfg,
UdxCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgReEstDlRb (gCb,ueCb, entCfg, entCfm)
PjCb              *gCb;
PjDlUeCb          *ueCb;
UdxCfgEnt         *entCfg;
UdxCfmEnt         *entCfm;
#endif
{
   S16    ret;
   PjDlRbCb   *rbCb;   /* Rb Cb */

   TRC3(pjCfgReEstDlRb); 

   /* Initialization */
   ret  = ROK;

   RLOG2(L_DEBUG, "pjCfgReEstRb- Received for -- cellId(%d),ueId(%d)",
                         ueCb->key.cellId, ueCb->key.ueId);

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_DL_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);
   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == rbCb )
   { 
      RLOG_ARG2(L_ERROR, DBG_UEID,ueCb->key.ueId, "CellId [%u]:RbId[%d] already exists",
            ueCb->key.cellId,entCfg->rbId);
      /* Fill entCfm structure with invalid PDCP Id*/
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_PDCPID_UNKWN);

      RLOG_ARG1(L_ERROR,DBG_UEID,ueCb->key.ueId, 
            "Re-Est Failed : DL RB CB is not present -- cellId(%d)",
                          ueCb->key.cellId);

      RETVALUE(RFAILED);
   }

   /* A Cfm with failure is sent if the re-establishment comes when RBs are not in
      re-est state */
   if (ueCb->libInfo.state != PJ_STATE_REEST )
   {
      /* Fill entCfm structure with invalid PDCP Id*/
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_STATE);

      RLOG_ARG1(L_ERROR,DBG_UEID,ueCb->key.ueId, 
            "Re-Est Failed : RB State is Not REEST -- cellId(%d)",
                          ueCb->key.cellId);
      RETVALUE(RFAILED);
   }

   /* 5. Call the respective reestablishment functions for SRB */
   if(rbCb->rbType == PJ_SRB)
   {
      pjDlmReEstSrb(gCb,rbCb);
   }
   else /* PJ_DRBs */
   {
      /* During Re-Establishment DL RAB state is assumed to be in PJ_STATE_NORMAL
       * and the Status Report is sent only if this is the state.
       * But if the Reestablishment happens during HO(PJ_STATE_REEST_HO) 
       * due to the state change during HO no Status Report is sent after successful
       * Re-establishment
       */
      rbCb->state = PJ_STATE_NORMAL;
      rbCb->pktAdmitCnt = 0;
      if (rbCb->mode == PJ_DRB_UM)
      {
          pjDlmReEstDrbUm(gCb,rbCb);
      }
      else
      {
        pjDlmReEstDrbAm(gCb,rbCb);
      }
   }/* if(rbType == .... */

   RETVALUE(ret);
} /* pjCfgReEstRb */



/**
* @brief
* This primitive Delete Ue Cb from PDCP/RLC data base.
*
* @b Description:
*  1. This primitive Delete the Ue Cb from PDCP/RLC Data base.
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgDelDlUe
(
PjCb              *gCb,
PjDlUeCb          *ueCb,
UdxCfgEnt         *entCfg,
UdxCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgDelDlUe (gCb, ueCb, entCfg, entCfm)
PjCb              *gCb;
PjDlUeCb          *ueCb;
UdxCfgEnt         *entCfg;
UdxCfmEnt         *entCfm;
#endif
{
   TRC3(pjCfgDelDlUe);

   RLOG_ARG1(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
         "pjCfgDelUe- Received for -- cellId(%d)",
                          ueCb->key.cellId);

   /* Delete Ue Cb */
   pjDbmDelDlUeCb(gCb, ueCb, FALSE);

   /* Fill the Cfm */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* pjCfgDelUe */


/**
* @brief
* This primitive configure the security information of an Ue Cb.
*
* @b Description:
*  1. This primitive  configure the security information of an Ue Cb.
*  2. Initilize  the  Integrity/Ciphering Channels if required.
*
* @param [in]    secCfg   -  Entity Configuration to be done.
* @param [out]   secCfm   -  Entity Configuration Cfm done.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgPrcSecDlCfg
(
PjCb             *gCb,
UdxSecCfgReqInfo *secCfg,
UdxSecCfgCfmInfo *cfgCfm
)
#else
PUBLIC S16 pjCfgPrcSecDlCfg (gCb, secCfg, cfgCfm)
PjCb              *gCb;
UdxSecCfgReqInfo  *secCfg;
UdxSecCfgCfmInfo  *cfgCfm;
#endif
{
   S16     ret;
   PjDlUeCb *ueCb;

   TRC3(pjCfgPrcSecDlCfg);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;

   RLOG2(L_DEBUG, "pjCfgPrcSecCfg- Received for -- cellId(%d),ueId(%d)",
                          secCfg->cellId, secCfg->ueId);

   /* Fetch Ue Cb */
   ret = pjDbmFetchDlUeCb(gCb, secCfg->ueId, secCfg->cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      RLOG_ARG1(L_ERROR, DBG_CELLID, secCfg->cellId,"UeId[%u] not found",
            secCfg->ueId);
      /* Fill entCfm structure */
      PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
            CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UE_UNKWN);
      RETVALUE(ret);
   }

   ueCb->secInfo.selSecAct= secCfg->secCfg.selSecAct;

   /* Store Integrity Information */
   ueCb->secInfo.intInfo.algoType=secCfg->secCfg.integInfo.algoType;
   cmMemcpy(&ueCb->secInfo.intInfo.intKey[0], &secCfg->secCfg.integInfo.intKey[0], CPJ_INTG_KEY_LEN);

   /* Store Ciphering Informaiton */
   ueCb->secInfo.cipherInfo.algoType=secCfg->secCfg.cipherInfo.algoType; 

   cmMemcpy(ueCb->secInfo.cipherInfo.cpKey,
         &secCfg->secCfg.cipherInfo.ctrlplnKey[0],CPJ_CIPHER_KEY_LEN);

   cmMemcpy(ueCb->secInfo.cipherInfo.upKey,
         &secCfg->secCfg.cipherInfo.usrplnKey[0],CPJ_CIPHER_KEY_LEN);

   /* Security Activation Done */
   ueCb->secInfo.secAct = TRUE;

   keyChange++;

   if(TRUE == secCfg->secCfg.isHo)
   {
      ueCb->secInfo.firstMsg   =  FALSE;
   }
   else
   {
      ueCb->secInfo.firstMsg   =  TRUE;
   }

   ueCb->secInfo.secCxt.cellId = secCfg->cellId;
   ueCb->secInfo.secCxt.ueId   = secCfg->ueId;
   ueCb->secInfo.secCxt.dir    = PJ_SEC_DIR_DL;

   RLOG_ARG3(L_DEBUG,DBG_UEID,secCfg->ueId, 
         "Security Paramters stored in UE CB are -- cellId(%d)"  
         "Integrity Algo(%ld), cipherAlgoType(%ld) ",
         secCfg->cellId, ueCb->secInfo.intInfo.algoType,
         ueCb->secInfo.cipherInfo.algoType);

   /* Open the Security Channels */

   if(ueCb->secInfo.secAct)
   {

#ifdef PJ_SEC_ASYNC
      ueCb->secInfo.secCxt.cfgTxId = ueCb->libInfo.nxtAvlbTxId;
#endif 

      ret = pjUtlDlIntInit(gCb, ueCb);

      /* Integrity Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_INTINIT_FAILED);

         RETVALUE(ret);

      }
      ret = pjUtlDlCpInit(gCb, ueCb);
      /* Ciphering Control Plane Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_CCIPHR_FAILED);

         RETVALUE(ret);

      }
      ret = pjUtlDlUpInit(gCb, ueCb);

      /* Ciphering User Plane Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UCIPHR_FAILED);

         RETVALUE(ret);
      }
   }

   /* Fill Cfg Cfm with Status OK */
   PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
         CPJ_CFG_CFM_OK,CPJ_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* pjCfgPrcSecCfg */


/**
* @brief
* This primitive changes the ueId of existing Ue Cb.
* 
* @b Description:
*
*    1. This function delete the UeCb from hashList. @n
*    2. Update the Ue Cb with the new Keys. @n
*    3. Insert the UeCb in hash list with new Key.  @n
*
* @param [in]    ueInfo      -  Old UE Information
* @param [in]    newUeInfo   -  New UE Information
* @param [out]   status      -  Status
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgDlUeIdChng
(
PjCb      *gCb,
UdxUeInfo *ueInfo,
UdxUeInfo *newUeInfo,
CmStatus  *status
)
#else
PUBLIC S16 pjCfgDlUeIdChng (gCb, ueInfo, newUeInfo, status)
PjCb      *gCb;
UdxUeInfo *ueInfo;
UdxUeInfo *newUeInfo;
CmStatus  *status;
#endif
{
   PjDlUeCb *ueCb;
   S16    ret;

   TRC3(pjCfgDlUeIdChng)

   ueCb = NULLP;
   ret= ROK;

   RLOG2(L_DEBUG, "pjCfgUeIdChng- Recived for -- cellId(%d),ueId(%d)",
                          ueInfo->cellId, ueInfo->ueId);

   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      status->reason = CPJ_CFG_REAS_SAME_UEID;
      status->status = CPJ_CFG_CFM_NOK; 
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueInfo->cellId,"Old UeId[%d] same as new UeId[%d]",
            ueInfo->ueId,newUeInfo->ueId);
      RETVALUE(RFAILED);
   } 
   
   ret = pjDbmFetchDlUeCb(gCb, newUeInfo->ueId, newUeInfo->cellId, &ueCb);
   if (ret == ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, newUeInfo->cellId, "NewUeId[%d]:ueCb already exists",
            newUeInfo->ueId);
      status->reason = CPJ_CFG_REAS_SAME_UEID;
      status->status = CPJ_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   ret = pjDbmFetchDlUeCb(gCb, ueInfo->ueId, ueInfo->cellId, &ueCb);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueInfo->cellId,"UeId [%d]: UeCb not found",
            ueInfo->ueId);
      status->reason = CPJ_CFG_REAS_UE_UNKWN;
      status->status = CPJ_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   ret = cmHashListDelete(&(gCb->u.dlCb->ueLstCp), (PTR) ueCb);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueInfo->cellId,"UeId[%u] HashList Deletion Failed",
            ueInfo->ueId);
      status->reason = CPJ_CFG_REAS_UE_CREAT_FAIL;
      status->status = CPJ_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
   
   /* update the hash key with new values */ 
   ueCb->key.ueId = newUeInfo->ueId;
   ueCb->key.cellId = newUeInfo->cellId;

   ret = cmHashListInsert(&(gCb->u.dlCb->ueLstCp), (PTR)ueCb, 
                            (U8 *)&(ueCb->key), (U16) sizeof(PjUeKey));

   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,newUeInfo->cellId,"UeId[%u] HashList Insertion Failed",
            newUeInfo->ueId);
      status->reason = CPJ_CFG_REAS_UE_CREAT_FAIL;
      status->status = CPJ_CFG_CFM_NOK;
   }  
#ifndef L2_L3_SPLIT
#ifdef TENB_STATS
   ueCb->tenbStats = TSL2MapUeStatsBlk(ueCb->key.ueId);
#endif
#endif
  
   RETVALUE(ret);
} /* pjCfgUeIdChng */


/**
* @brief
* This primitive reports the Count informatin of all RBs of an Ue Cb.
*
* @b Description:
* 1. This function reports the count information of all the RBs of an UE. 
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [out]   countCfm -  Count Information
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgPrcDlCount
(
PjCb            *gCb,
UdxCountReqInfo *countReq,
UdxCountCfmInfo       *countCfm
)
#else
PUBLIC S16 pjCfgPrcDlCount (gCb, countReq, countCfm)
PjCb            *gCb;
UdxCountReqInfo *countReq;
UdxCountCfmInfo       *countCfm;
#endif
{
   S16     ret;
   PjDlUeCb  *ueCb;
   PjDlRbCb    *rbCb;
   U8      numRb;
   U8      idx;

   TRC3(pjCfgPrcDlCount); 

   /* Initialization */
   ueCb   = NULLP;
   rbCb   = NULLP;
   ret    = ROK;
   numRb  = 0;
   idx  = 0;

   RLOG2(L_DEBUG, "pjCfgPrcCount - Count Request Received for - cellId(%d),ueId(%d)",
                          countReq->cellId, countReq->ueId);

   /* Fetch  ue Cb */
   ret = pjDbmFetchDlUeCb(gCb, countReq->ueId, countReq->cellId, &ueCb);
   countCfm->ueId = countReq->ueId;
   countCfm->cellId = countReq->cellId;
   countCfm->transId = countReq->transId;

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      RLOG_ARG1(L_ERROR, DBG_CELLID, countCfm->cellId,"UeId[%u] not found",countCfm->ueId);
      /* Fill entCfm structure */
      countCfm->numRb  = numRb;
      countCfm->status = CPJ_CFG_CFM_NOK;
      countCfm->reason = CPJ_CFG_REAS_UE_UNKWN;
      RETVALUE(ret);
   }

   /* Loop through all DRBs  */
   for(numRb=0, idx=0; idx < PJ_MAX_DRB_PER_UE && numRb < UDX_MAX_DRB; idx++)  /* KW_FIX */
   {
      rbCb = ueCb->drbCb[idx];
      /* RB is present and configured for PDCP Layer */
      if(NULLP != rbCb)
      {
         countCfm->countInfo[numRb].rbId= rbCb->rbId; 
         countCfm->countInfo[numRb].dir = rbCb->dir; 
         countCfm->countInfo[numRb].dlCount= rbCb->dlCb.count; 
         countCfm->countInfo[numRb].ulCount= 0; 
         numRb++;
      }
   }

   /* Fill CountCfm with status and reason */
   countCfm->numRb  = numRb;
   if(numRb)
   {
      countCfm->status =CPJ_CFG_CFM_OK;
      countCfm->reason =CPJ_CFG_REAS_NONE;
   }
   else
   {
      countCfm->status =CPJ_CFG_CFM_NOK;
      countCfm->reason =CPJ_CFG_REAS_NO_DRBS;

   }

   RETVALUE(ret);
} /* pjCfgPrcCount */

/**
* @brief
* This primitive reports SDU status information of all RBs of an Ue Cb.
*
* @b Description:
*  1. This primitive Trigger the Handoff Request procedure 
*
* @param [in]    ueId      -  UE Identifier
* @param [in]    cellId    -  CELL Identifier
* @param [out]   sduStaCfm -  SDU Status Information
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgPrcDlSduSta
(
PjCb                 *gCb,
UdxSduStaReqInfo     *staReq
)
#else
PUBLIC S16 pjCfgPrcDlSduSta (gCb, staReq)
PjCb                 *gCb;
UdxSduStaReqInfo     *staReq;
#endif
{
   S16      ret;
   PjDlUeCb  *ueCb;
   U8 rbCnt;
   U8 numRb = 0;
   UdxSduStaInfo  *staInfo;
   PjDlHoCfmInfo *hoCfmInfo;
   UdxSduStaCfmInfo *cfmInfo;
   PjUdxDlSapCb      *udxSap; 

   TRC3(pjCfgPrcDlSduSta);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;

   RLOG2(L_DEBUG, "pjCfgPrcSduSta - Received for -- cellId(%d),ueId(%d)",
                          staReq->cellId, staReq->ueId);

   /* Memory leak fix ccpu00135359 */
   udxSap = &(gCb->u.dlCb->udxDlSap[0]);
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfmInfo, 
      sizeof (UdxSduStaCfmInfo)) != ROK)
   {  
#if (ERRCLASS & ERRCLS_DEBUG)    /*KW_FIX*/
      RLOG0(L_FATAL, "Memory Allocation failed.");
      /*ccpu00136858 */      
      PJ_PST_FREE(gCb->u.dlCb->udxDlSap[0].pst.region,
                  gCb->u.dlCb->udxDlSap[0].pst.pool, 
                   staReq, sizeof(UdxSduStaReqInfo));
#endif
         RETVALUE(RFAILED);
   }
   cfmInfo->ueId = staReq->ueId;
   cfmInfo->cellId = staReq->cellId;
   cfmInfo->transId = staReq->transId;
   cfmInfo->status = CPJ_CFG_CFM_OK; 

   /* Fetch Ue Cb */
   ret = pjDbmFetchDlUeCb(gCb, staReq->ueId, staReq->cellId, &ueCb);

   /* UeCb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      RLOG_ARG1(L_ERROR, DBG_CELLID, staReq->cellId,"UeId[%u] not found",staReq->ueId);
      cfmInfo->status = CPJ_CFG_CFM_NOK; 
      cfmInfo->reason = CPJ_CFG_REAS_UE_UNKWN;
      cfmInfo->numRb = numRb;

      /* Caller should return cfm with failure */
      RLOG_ARG1(L_ERROR,DBG_UEID,staReq->ueId,
                  "UE Cb is not found -- cellId(%d)",
                          staReq->cellId);

      /* Send Failure SduStaCfm */
      PjDlUdxSduStaCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
                        gCb->u.dlCb->udxDlSap[0].suId, cfmInfo);

      RETVALUE(RFAILED);
   }

   /* Call Ho Start handler for this UE */
   pjUtlDlReEstHO(gCb, ueCb);

   for(rbCnt=0, numRb=0; rbCnt < PJ_MAX_DRB_PER_UE && numRb < CPJ_MAX_DRB; rbCnt++)
   {
      hoCfmInfo   =  &ueCb->hoInfo->hoCfmInfo[rbCnt];
      staInfo     =  &cfmInfo->sduStaInfo[numRb];
      if(hoCfmInfo->pres != TRUE)
      {
         continue;
      }
      
      staInfo->rbId  = hoCfmInfo->rbId;
      staInfo->dir   =  hoCfmInfo->dir;
      staInfo->dlSduStaInfo.count     =  hoCfmInfo->count;
      staInfo->dlSduStaInfo.hoPres    =  TRUE;
      numRb++;
   } 
   cfmInfo->numRb = numRb;
   cfmInfo->status = ROK;
   cfmInfo->reason = CPJ_CFG_REAS_NONE; 
   PJ_FREE(gCb,ueCb->hoInfo->hoCfmInfo,
              (PJ_MAX_DRB_PER_UE * sizeof(PjDlHoCfmInfo)));
   PJ_FREE(gCb,ueCb->hoInfo, sizeof(PjDlHoInfo));             

   PjDlUdxSduStaCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
                     gCb->u.dlCb->udxDlSap[0].suId, cfmInfo);
   RETVALUE(ROK);
} /* pjCfgPrcSduSta */

   

/**
 * @brief
 * This primitive to update the ROHC Comp details in PJRB  Cb.
 *
 * @b Description:
 *  1. This primitive to update  the ROHC comp detils in PJRB Cb.
 *
 * @param [in]    pjRbCb    -  RB Control Block  
 * @param [in]    ueCb      -  Ue Control Block 
 * @param [in]    hdrCompCfg-  Comp Cfg Information 
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PRIVATE S16 pjCfgUpdDlCompInfo 
(
PjCb                   *gCb,
PjDlRbCb               *rbCb,
PjDlUeCb               *ueCb,
UdxHdrCompCfg        *rohcCfg
)
#else
PRIVATE S16 pjCfgUpdDlCompInfo(gCb, rbCb, ueCb, rohcCfg)
PjCb                   *gCb;
PjDlRbCb               *rbCb;
PjDlUeCb               *ueCb;
UdxHdrCompCfg        *rohcCfg;
#endif
{
   U16   profId;
   U8    idx;
   S16   ret;

   TRC3(pjCfgUpdDlCompInfo);

   /* Initialization */   
   ret    = ROK;
   profId = 0;
   idx  = 0; 

   RLOG2(L_DEBUG, "pjCfgUpdCompInfo - Received for -- cellId(%d),RbId(%d)",
                          rbCb->rbId, ueCb->key.ueId);

   /* Header Compresion information present */
   if(rohcCfg->hdrCompUsed)
   {
      /* Header Compression is enabled */ 
      rbCb->rohc.hdrCmpUsed = TRUE;

      /* User Can Configure multipel profiles. Maximum profile ID should be used */  
      for(; idx<CPJ_MAX_ROHC_PROF_LIST; idx++)
      {
         if((rohcCfg->rohcInfo.profLst[idx] )&&
               (profId < rohcProfId[idx]))
            profId = rohcProfId[idx];
      } 
   }
   else
   {
      /* Header Compression is disabled */
      rbCb->rohc.hdrCmpUsed = FALSE;
      profId = 0x0000;
   }

   /* Store the profile Id and MaxCid */

   rbCb->rohc.profId = profId;
   rbCb->rohc.maxCid = rohcCfg->rohcInfo.maxCid;

   /* Store the ue Details in Compressor Context */
   rbCb->cmpCxt.ueId   = ueCb->key.ueId;
   rbCb->cmpCxt.cellId = ueCb->key.cellId;
   rbCb->cmpCxt.rbId   = rbCb->rbId;
   rbCb->cmpCxt.rbType = rbCb->rbType;

   RLOG2(L_DEBUG, "pjCfgUpdCompInfo - Selected   -- profId(%d),maxCid(%d)",
                          rbCb->rohc.profId, rbCb->rohc.maxCid);


   /* Compress Init */
   if(rbCb->rohc.hdrCmpUsed)
   {
      ret=pjUtlDlCmpInit(gCb, rbCb); 
   }

   RETVALUE(ret);
} /* pjCfgUpdCompInfo */


/**
 * @brief
 * This primitive to delete RB from Ue Cb.
 *
 * @b Description:
 * 1. To delete the PJRB Cb from  UeCb.
 *
 * @param [in]    ueCb      -  Ue Control Block 
 * @param [in]    rbCb      -  Rb Control Block  
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PUBLIC S16  pjCfgDelPjDlRbCb 
(
PjCb                 *gCb,
PjDlUeCb             *ueCb,
PjDlRbCb             *rbCb
)
#else
PUBLIC S16 pjCfgDelPjDlRbCb(gCb,ueCb,rbCb)
PjCb                  *gCb;
PjDlUeCb              *ueCb;
PjDlRbCb              *rbCb;
#endif
{   
   S16  ret;
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
#endif
   TRC3(pjCfgDelPjDlRbCb);

   /* Initialization */
   ret = ROK;

   /* RbCb is present and Configured for PDCP */
   if(NULLP != rbCb) 
   {
      RLOG_ARG1(L_DEBUG, DBG_UEID,ueCb->key.ueId, 
            "Delete RB Req Received for -- rbId (%d)",rbCb->rbId);

#ifdef LTE_L2_MEAS
      /* Seatch if any measurement is in progress for this qci */
#if 0 /*PRERIT */
      if(gCb->pjL2Cb.measOn[pjRbCb->qci] !=  LPJ_L2MEAS_NONE)
      {
         PjL2MeasCb *measCb;
         for(evtIdx = 0; evtIdx < PJ_MAX_L2MEAS_EVT; evtIdx++)
         {
            if(gCb->pjL2Cb.pjMeasEvtCb[evtIdx] == NULLP) 
            {
               continue;
            }
            measCb = &gCb->pjL2Cb.pjMeasEvtCb[evtIdx]->measCb;
            for(qciIdx = 0; qciIdx < measCb->numQci; qciIdx++)
            {
               if(pjRbCb->qci == measCb->qci[qciIdx])
               {
                  break;
               }
            }
            if(qciIdx == measCb->numQci)
            {
               continue;
            }
            /* a mesurement running for that qci is found */
            switch(measCb->measType)
            {
               case LPJ_L2MEAS_DL_DELAY:
               {
                   PJ_RMV_RB_FRM_L2LIST(PJ_L2MEAS_DL_DELAY, measCb, pjRbCb);
                   /* TODO Clean up the TxEnt */
               }
               break;
               case LPJ_L2MEAS_DL_DISC:
               {
                   PJ_RMV_RB_FRM_L2LIST(PJ_L2MEAS_DL_DISC, measCb, pjRbCb);
               }
               break;
               default:
                  RETVALUE(RFAILED);
            }
            pjRbCb->rbL2Cb.measOn &= ~measCb->measType;
            pjLmmSendAlarm(LCM_CATEGORY_INTERFACE, LPJ_EVT_MEAS_HALT,\
            LPJ_CAUSE_RB_DELETION, 0, pjRbCb->ueCb->key.ueId, pjRbCb->qci);
         }
      }
#else
      PJ_UPD_L2_DECR_PER_QCI_RB_COUNT(gCb, rbCb); 
#endif
#endif
      rbCb->ueCb   = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
      /* Stop off board timers */
      if(PJ_DIR_DL & rbCb->dir)
         pjStopTmr(gCb, (PTR)rbCb, PJ_EVT_DL_OBD_TMR);  
#endif
      /* Deinit Tx Buffers */
      pjDbmTxDeInit(gCb, &rbCb->dlCb.txBuf);
      pjUtlEmptyDlPktList(gCb, rbCb);

      if (rbCb->dlCb.staRep != NULL)
      {
        PJ_FREE_BUF(rbCb->dlCb.staRep);
      }

      if (PJ_SRB == rbCb->rbType)
      {
         ueCb->numSrbs--;
      }
      else if(PJ_DRB == rbCb->rbType) 
      {
         ueCb->numDrbs--;
         if (rbCb->rohc.hdrCmpUsed)
         {
            pjUtlCmpClose(gCb,rbCb->cmpCxtId);
         }
      }

      /* Free the rbCb */
      pjDbmDelDlRbCb(gCb,rbCb->rbId, rbCb->rbType, &rbCb, PJ_CFG_PDCP,ueCb);
   } 
   RETVALUE(ret);
} /* pjCfgDelPjRBCb */


/**
 * @brief
 * Handler to process the downlink re-establishment Request for SRB1 during
 * RRC Re-Establishment request.
 *
 * @b Description:
 * 1. This primitive is used to re-establish SRB1 only during RRC Re-establishment 
 * 2. New Security Algo will applied 
 *
 * @param [in]    gCb        -  PDCP Instance Control block.
 * @param [in]    reEstReq   -  Entity Configuration to be done.
 * @param [out]   reEstCfm   -  Entity Configuration cfm.
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PUBLIC S16 pjCfgDlReEstReq
(
PjCb            *gCb,
UdxReEstReqInfo *reEstReq,
UdxReEstCfmInfo *reEstCfm
)
#else
PUBLIC S16 pjCfgDlReEstReq (gCb, reEstReq, reEstCfm)
PjCb            *gCb;
UdxReEstReqInfo *reEstReq;
UdxReEstCfmInfo *reEstCfm;
#endif
{
   S16     ret;
   PjDlRbCb    *rbCb;
   CmLteRlcId  rlcId;

   TRC3(pjCfgDlReEstReq);
 
   /* Initialization */
   rlcId.cellId  =  reEstReq->pdcpId.cellId;
   rlcId.ueId  =  reEstReq->pdcpId.ueId;
   rlcId.rbId  =  reEstReq->pdcpId.rbId;
   rlcId.rbType  =  reEstReq->pdcpId.rbType;

   RLOG2(L_DEBUG, "ReEst Req Received for -- cellId(%d),ueId(%d)",
                 reEstReq->pdcpId.cellId, reEstReq->pdcpId.ueId);

   ret = pjDbmFetchDlRbCb(gCb,rlcId,&rbCb);
   
   /* Ue Cb is not present */
   if( ROK != ret || NULLP == rbCb)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId.ueId, "CellId[%u]:RbId[%d] not found",
            rlcId.cellId, rlcId.rbId);
      /* Fill entCfm structure */
      PJ_CFG_FILL_REESTREQ_CFM(reEstCfm, reEstReq->pdcpId.ueId,reEstReq->pdcpId.cellId,
            reEstReq->transId, CPJ_CFG_CFM_NOK);
      RETVALUE(ret);
   }

   /* Copy Security Information */ 
   cmMemcpy(&rbCb->ueCb->secInfo.intInfo.intKey[0], &reEstReq->intKey[0], PJ_INTG_KEY_LEN);
   cmMemcpy(&rbCb->ueCb->secInfo.cipherInfo.cpKey[0], &reEstReq->ctrlplnKey[0], PJ_CIPHER_KEY_LEN);
   cmMemcpy(&rbCb->ueCb->secInfo.cipherInfo.upKey[0], &reEstReq->usrplnKey[0], PJ_CIPHER_KEY_LEN);

   /* Mark Security Action Done */
   rbCb->ueCb->secInfo.secAct = TRUE;
   
   /* update the status as success. Chances of failure after this is minimal*/
   PJ_CFG_FILL_REESTREQ_CFM(reEstCfm, reEstReq->pdcpId.ueId,reEstReq->pdcpId.cellId,
            reEstReq->transId, CPJ_CFG_CFM_OK);

#ifdef PJ_SEC_ASYNC
   /* In case of Async need to save the cfm information */
   pjUtlDlSaveCfmInfo(gCb, rbCb->ueCb, PJ_REEST_ASYNC_CFM, TRUE, ENTPJ, 
                                          (PTR)reEstCfm, NULLP);
   PJ_FREE(gCb,reEstCfm, sizeof(CpjReEstCfmInfo));
#endif

   /* Call ReEst for SRB1 */
   pjUtlDlReEstSrb1(gCb, rbCb);

   RETVALUE(ROK);
} /* pjCfgReEstReq */

#ifdef LTE_L2_MEAS

PRIVATE S16 pjAddToDlL2Meas(PjCb *gCb, PjDlRbCb *pjRbCb,U8 cellId,U8 ueId)
{
   PjL2MeasEvtCb *measEvtCb = NULLP;
   PjL2MeasCb    *measCb    = NULLP;
   U16           cntr;


   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &gCb->u.dlCb->pjL2Cb.pjL2EvtCb[cntr];
      measCb = &(measEvtCb->measCb);

      if (measCb->measType & LPJ_L2MEAS_DL_DISC)
      {
         pjUtlPlcMeasDatInDlL2Sts(&measCb->measData[pjRbCb->qci],
               &pjRbCb->rbL2Cb, measCb->measType);
         measCb->qci[pjRbCb->qci] = pjRbCb->qci;

         measCb->measData[pjRbCb->qci].totDrbsPerQci++;
      }
   }
   RETVALUE(ROK);
}/*pjAddToDlL2Meas*/ 
#endif /*LTE_L2_MEAS*/

/*@}*/

/********************************************************************30**

         End of file:     eq_cfg_dl.c@@/main/TeNB_Main_BR/8 - Mon Aug 11 16:42:44 2014

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
/main/1      ---      rbabu    1. LTERLC Release 2.1
/main/2      kw004.201 ap      1. Sending  status as CPJ_CFG_CFM_OK 
                                  in deleting unknown UE
/main/3      kw005.201 ap      1. Removed extra fetching of ueCb in the 
                                  function pjCfgUeIdChng.
                               2. Added support for L2 Measurement.
*********************************************************************91*/
