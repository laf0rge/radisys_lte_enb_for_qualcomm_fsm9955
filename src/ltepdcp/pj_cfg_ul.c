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

        Sid:      eq_cfg_ul.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:47 2015

        Prg:      rbabu

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=213;

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
#include  "pj_ul.h"
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
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x"

/* Proto types declarations for local functions */
 
PRIVATE S16 pjCfgFillPjUlRbCb ARGS ((
PjCb            *gCb,
PjUlRbCb        *rbCb,
PjUlUeCb        *ueCb,
U8              isHo,
CpjCfgEnt       *cfgEnt,
CpjCfmEnt       *cfmEnt
));

PRIVATE S16 pjCfgUpdUlCompInfo  ARGS ((
PjCb                   *gCb,
PjUlRbCb               *rbCb,
PjUlUeCb               *ueCb,
CpjHdrCompCfg        *rohcCfg
));

#ifdef LTE_L2_MEAS
PRIVATE S16 pjAddToUlL2Meas ARGS ((
PjCb *gCb, 
PjUlRbCb *kwRbCb,
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
PUBLIC S16 pjCfgAddUlRb
(
PjCb              *gCb,
CmLteRnti         ueId,
CmLteCellId       cellId,
U8                isHo,
CpjCfgEnt         *entCfg,
PjUlUeCb          **ueCb,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgAddUlRb (gCb, ueId, cellId,isHo, entCfg, entCfm)
PjCb              *gCb;
CmLteRnti         ueId;
CmLteCellId       cellId;
U8                isHo;
CpjCfgEnt         *entCfg;
PjUlUeCb          **ueCb;
CpjCfmEnt         *entCfm;
#endif
{
   S16              ret;
   PjUlRbCb         *rbCb;  /* Rb Cb Pointer */

   TRC3(pjCfgAddUlRb);  

   /* Initialization */
   ret  = ROK;
   rbCb = NULLP;

   RLOG2(L_DEBUG, "pjCfgAddRb- Received with -- cellId(%d),ueId(%d)",
                     cellId, ueId);

   if( NULLP == *ueCb)
   { 
      /* Ue Cb is not present, Create Ue Cb */   
      if ( pjDbmCreateUlUeCb(gCb, ueId, cellId, ueCb) != ROK)
      {
         /* Ue Cb allocation failed  */   
         PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
               CPJ_CFG_REAS_UE_CREAT_FAIL);

         RLOG_ARG1(L_ERROR,DBG_UEID,ueId, "Creating UE Cb Failed -- cellId(%d)",
                     cellId);
         RETVALUE(ret);
      } 
   }

   (*ueCb)->inst = gCb->init.inst;
   /* Rb Id is allowed range or not */   
   PJ_VALIDATE_RBID(ueId, entCfg->rbId, entCfg->rbType, ret);
   if ( ROK != ret)
   { 
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
         CPJ_CFG_REAS_INVALID_RBID);

      RLOG_ARG2(L_ERROR,DBG_UEID,ueId, "Invalid RB ID-- cellId(%d),RB Id(%d)",
                     cellId, entCfg->rbId);
      RETVALUE(ret);
   }


   /* Find Rb Cb already exist or not */   
   PJ_DBM_FETCH_UL_RBCB(entCfg->rbId, entCfg->rbType, (*ueCb), rbCb);

   if (NULLP != rbCb)
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,ueId, "CellId [%u]:RbId[%d] already exists",
            cellId,entCfg->rbId);
      /* Rb Cb already exist for pdpcp */   
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_SAME_RBID);
      RETVALUE(RFAILED);
   }
   else if (NULLP == rbCb)
   {
      /* Create Rb Cb and place in Ue Cb */   
      rbCb =   pjDbmCreateUlRbCb(gCb, entCfg->rbId, entCfg->rbType, *ueCb, PJ_CFG_PDCP); 
      if(NULLP == rbCb)
      {
         RLOG_ARG2(L_ERROR, DBG_UEID, ueId,"CellId [%u]:Creation of rbCb with id [%d] failed", 
               cellId, entCfg->rbId);
         /* Create Rb Cb Failed*/   
         PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
               CPJ_CFG_REAS_ENT_CREAT_FAIL);
         RETVALUE(RFAILED);
      } 
   }

   /* Copy the Recived information into RbCb */ 
   ret = pjCfgFillPjUlRbCb(gCb, rbCb, *ueCb,isHo, entCfg , entCfm);

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
PRIVATE S16 pjCfgFillPjUlRbCb
(
PjCb            *gCb,
PjUlRbCb        *pjRbCb,
PjUlUeCb        *ueCb,
U8               isHo,
CpjCfgEnt       *cfgEnt,
CpjCfmEnt       *cfmEnt
)
#else
PRIVATE S16 pjCfgFillPjUlRbCb(gCb, pjRbCb, ueCb, isHo, cfgEnt, cfmEnt)
PjCb            *gCb;
PjUlRbCb        *pjRbCb;
PjUlUeCb        *ueCb;
U8               isHo;
CpjCfgEnt       *cfgEnt;
CpjCfmEnt       *cfmEnt;
#endif
{
   S16            ret;
   S16            tmrVal;    /* Timer Value */
   U8             *bitMap;  /* Bitmap status report buffer      */
   U16            bMapLen; /* Bit Map length */
   /* kw005.201 added support for L2 measurement */
   
   TRC3(pjCfgFillPjUlRbCb);

   /* Initialization */
   ret     = ROK;
   tmrVal  = 0; 
   bMapLen = 0;

   RLOG_ARG3(L_DEBUG,DBG_UEID,ueCb->key.ueId,"Received Fill RB Req For "
                 "cellId(%d), rbId(%d), cfgType(%d)",
                 ueCb->key.cellId, cfgEnt->rbId, cfgEnt->cfgType);

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
                  /* SN length can be 7 bit or 12 bit in UM mode */
                  pjRbCb->snLen  =   cfgEnt->m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize;
               else
               {
                  /* SN length can be 12/15 bit for AM mode */
                  pjRbCb->snLen  =   cfgEnt->m.addCfg.rlcInfo.m.rlcAm.pdcpSNSize;
                  if (isHo)
                  {
                     pjRbCb->state = PJ_STATE_HO;
                  }
               }
               ueCb->numDrbs++;
            }
            /* Initialize all off board  timers */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
            cmInitTimers(&pjRbCb->ulCb.obdTmr,PJ_RB_MAX_TIMERS); 
#endif                  

            RLOG_ARG2(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
                  "RB Add Req Received with - Dir(%d),mode(%d)",
                         pjRbCb->dir, pjRbCb->mode);

            pjRbCb->ulCb.discReqd   = cfgEnt->m.addCfg.discReqd;
            pjRbCb->ulCb.staRepReqd = cfgEnt->m.addCfg.rlcInfo.m.rlcAm.statusRepReqd;

            /*Initialization for Rx Buf */
            gCb->pjGenCfg.nmbUlBins = PJ_MAX_HASH_BINS;
            pjDbmBufInit(gCb, &pjRbCb->ulCb.recBuf,gCb->pjGenCfg.nmbUlBins);
            cmLListInit(&pjRbCb->ulCb.ulPktLst);
            pjRbCb->ulCb.lastSubCnt = 0xffffffff;
            

            /* Update the Compressor Informatio in Rb Cb */
            if(cfgEnt->rbType == CM_LTE_DRB)
            {
               ret=pjCfgUpdUlCompInfo(gCb, pjRbCb, ueCb,&cfgEnt->m.addCfg.hdrCompCfg);
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
               pjRbCb->qci =  cfgEnt->m.addCfg.qci;
               pjAddToUlL2Meas(gCb,pjRbCb,
                               ueCb->key.cellId,ueCb->key.ueId); 
#endif
            }

            break;
         }

         /* Cfg Type received as Modify RB */
      case CPJ_CFG_MODIFY:
         {
            /* Store the received Information */
            if(CPJ_RECFG_DISRQD & cfgEnt->m.modCfg.bitFlag)
               pjRbCb->ulCb.discReqd   = cfgEnt->m.modCfg.discReqd;

            /* Header Compression profile has been modified */

            if(TRUE == cfgEnt->m.modCfg.hdrCompCfg.hdrCompUsed)
            {
               /* Close All the channels */
               pjUtlUlCmpReset(gCb, pjRbCb); 
               pjCfgUpdUlCompInfo(gCb, pjRbCb, ueCb, &cfgEnt->m.modCfg.hdrCompCfg);
            }

            /* In case of Hand off scenario: At Target eNode-B, 
               Handoff information will send in re-cfg req */

            if(TRUE  & cfgEnt->m.modCfg.hoInfo.isHoPres)
            {
               /* Store the HO information  and call for Status Report
                * Generation also */
              
               /* Down Up Lin Information present */
               if(CPJ_HO_UL & cfgEnt->m.modCfg.hoInfo.isHoPres)
               {
                  RLOG_ARG2(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
                        "Received PDCP Update from Application for dir(%d), mode(%d)",
                               pjRbCb->dir, pjRbCb->mode);

                  pjRbCb->ulCb.lastSubCnt = cfgEnt->m.modCfg.hoInfo.ulCount - 1;
                  if (pjRbCb->snLen != cfgEnt->m.modCfg.hoInfo.snLen)
                  {
                     pjRbCb->snLen = cfgEnt->m.modCfg.hoInfo.snLen;
                  }
                  PJ_GET_SN_HFN(cfgEnt->m.modCfg.hoInfo.ulCount , pjRbCb->snLen, \
                        (pjRbCb->ulCb.nxtRxSn), (pjRbCb->ulCb.rxHfn));
                  /* Store the Count in ulCb, this is needed while sending PDCP Status Report */
                  bMapLen = cfgEnt->m.modCfg.hoInfo.numBits;
                  bitMap = cfgEnt->m.modCfg.hoInfo.ulBitMap;

                  if(((pjRbCb->ulCb.nxtRxSn + bMapLen) >= (1 << (pjRbCb->snLen))) || (pjRbCb->ulCb.rxHfn != 0))
                  {
                     /* If the Forwarded Packets were received before SN Status from Source, then the 
                      * COUNT Value would have been mapped with HFN = 0.
                      * But if HFN in SN Status is non-zero or if FMS + Bitmap Len exceeds 4096/16384,
                      * then the COUNT of the Buffered packets should be re-computed. */
                     pjUtlUlResetBuffrdPkts(gCb, pjRbCb);
                  }

                  /* Build and send the Status Report to RLC */
                  /* Send the FMS & bitmap retrieved now from App */
                  RLOG_ARG0(L_DEBUG,DBG_UEID,ueCb->key.ueId,
                          "Sending PDCP Status Report to UE ");

                  pjUlmBldStaRep(gCb, pjRbCb, pjRbCb->ulCb.nxtRxSn, bitMap, bMapLen);

                  pjUlmProcessUlPktQ(gCb, pjRbCb);

                  pjRbCb->state = PJ_STATE_NORMAL;

                  if(bMapLen != 0)
                  {
                     PJ_FREE(gCb, bitMap, (bMapLen / 8) * sizeof(U8));
                  }
               } 
            }

            break;
         }

         /* Un Expected cfg type */
      default:
         {
            RLOG1(L_ERROR, "Invalid cfgType [%d]", cfgEnt->cfgType);
         }
   }
   RETVALUE(ret);
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
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgReCfgUlRb
(
PjCb              *gCb,
PjUlUeCb          *ueCb,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgReCfgUlRb (gCb, ueCb, entCfg, entCfm)
PjCb              *gCb;
PjUlUeCb          *ueCb;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16       ret;
   PjUlRbCb     *rbCb;    /* Rb Cb */

   TRC3(pjCfgReCfgUlRb);

   /* Initialization */
   ret  = ROK;
   rbCb = NULLP;

   RLOG2(L_DEBUG, "Re-Cfg UL RB Req Received for -- cellId(%d),ueId(%d)",
                     ueCb->key.cellId,ueCb->key.ueId);

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_UL_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == rbCb)
   { 
      RLOG_ARG2(L_ERROR, DBG_UEID,ueCb->key.ueId, "CellId[%u]:RbId[%d] not found",
            ueCb->key.cellId, entCfg->rbId);
      /* Fill entCfm structure */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_PDCPID_UNKWN);

      RETVALUE(RFAILED);
   }

   /* Copy the received infromation in to Rb Cb */
   ret = pjCfgFillPjUlRbCb(gCb, rbCb, ueCb,FALSE, entCfg , entCfm);

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
PUBLIC S16 pjCfgDelUlRb
(
PjCb              *gCb,
PjUlUeCb          *ueCb,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgDelUlRb (gCb, ueId, cellId, entCfg, entCfm)
PjCb              *gCb;
PjUlUeCb          *ueCb;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16      ret;
   PjUlRbCb *rbCb = NULLP; 

   TRC3(pjCfgDelUlRb);

   /* Initialization */
   ret = ROK;

   RLOG2(L_DEBUG, "Del UL RB Req Received for -- cellId(%d),ueId(%d)",
                     ueCb->key.cellId, ueCb->key.ueId);

   /* Fetch RB Cb */
   PJ_DBM_FETCH_UL_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* RB Cb is not present   or not configured for PDCP */
   if (NULLP == rbCb) 
   { 
      RLOG_ARG2(L_ERROR, DBG_UEID,ueCb->key.ueId, "CellId[%u]:RbId[%d] not found",
            ueCb->key.cellId, entCfg->rbId);
      /* Fill entCfm structure with invalid pdcp Id */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_PDCPID);

      RETVALUE(RFAILED);
   }
   /* Delete PDCP Related infromation from RB CB */
   ret= pjCfgDelPjUlRbCb(gCb, ueCb, rbCb); 

   if(ROK != ret)
   {
      /* Fill entCfm structure with invalid pdcp Id */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_PDCPID);

      RLOG_ARG2(L_ERROR, DBG_UEID, ueCb->key.ueId, "CellId[%u]:RbId[%d] not deleted",
            ueCb->key.cellId, entCfg->rbId);
      RETVALUE(ret);
   }
   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
} /* pjCfgDelRb */




/**
*  @brief
*
*   This primitive is used to re-establish Uplink SRB2 and/or DRB'S during RRC connection
*   re-configuration procedure.
*  
*  @b Description:
*  1. This function will trigger re-establishment for SRB2 and/or DRB's
*
* @param [in]    gCB      -  Pdcp Instance Control block
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Configuration cfm.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgReEstUlRb
(
PjCb              *gCb,
PjUlUeCb          *ueCb, 
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm,
UdxCfmEnt         *entDlCfgCfm 
)
#else
PUBLIC S16 pjCfgReEstUlRb (gCb, *ueCb, entCfg, entCfm, entDlCfgCfm)
PjCb          *gCb;
PjUlUeCb      *ueCb;
CpjCfgEnt     *entCfg;
CpjCfmEnt     *entCfm;
UdxCfmEnt     *entDlCfgCfm; 
#endif
{
   PjUlRbCb *rbCb = NULLP;

   TRC3(pjCfgReEstUlRb); 

   RLOG2(L_DEBUG, "Config ReEst Req Received for UL RB -- cellId(%d),ueId(%d)",
                     ueCb->key.cellId, ueCb->key.ueId);

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_UL_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == rbCb )
   {
      RLOG_ARG2(L_ERROR, DBG_UEID, ueCb->key.ueId, "CellId[%u]:RbId[%d] not found",
            ueCb->key.cellId, entCfg->rbId);
      /*This RB might be configured at PDCP-DL not at PDCP-UL (Rlc-UM RB with DL direction)*/
      PJ_MEM_CPY(entCfm, entDlCfgCfm, sizeof(CpjCfmEnt)); /* KW_FIX */

      RETVALUE(RFAILED);
   }
   /* A Cfm with failure is sent if the re-establishment comes when RBs are not in
      re-est state */
   if (ueCb->libInfo.state != PJ_STATE_REEST )
   {
      /* Fill entCfm structure with invalid PDCP Id*/
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_STATE);
      RLOG_ARG2(L_ERROR, DBG_UEID, ueCb->key.ueId, "CellId[%u]:W/o REEST ,RbId[%d] not reestblshd",
            ueCb->key.cellId, entCfg->rbId);
      RETVALUE(RFAILED);
   }

   if(rbCb->rbType == PJ_SRB)
   {
      pjUlmReEstSrb(gCb,rbCb);
   }
   else
   {
      ueCb->libInfo.numReEstDrb++;
      rbCb->state = PJ_STATE_REEST;
     
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
       rbCb->ulCb.obdCnt   = 0;
       rbCb->ulCb.transCmp = FALSE;
       if(rbCb->mode == PJ_DRB_AM)
       {
          rbCb->ulCb.firstReEstCnt   =   rbCb->ulCb.lastSubCnt + 1;
       }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */    
   }/* if(rbType == .... */
 
   if (entDlCfgCfm->status != CPJ_CFG_CFM_OK)
   {
      PJ_MEM_CPY(entCfm, entDlCfgCfm, 
                       sizeof(CpjCfmEnt)); /* KW_FIX */
   }

   RETVALUE(ROK);
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
PUBLIC S16 pjCfgDelUlUe
(
PjCb              *gCb,
PjUlUeCb          *ueCb,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgDelUlUe (gCb, ueId, cellId, entCfg, entCfm)
PjCb              *gCb;
PjUlUeCb          *ueCb,
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16     ret;
   U8      idx;    /* index for Rb Cb */

   TRC3(pjCfgDelUlUe);

   /* Initialization */
   ret    = ROK;
   idx    = 0;

   RLOG2(L_DEBUG, "pjCfgDelUe- Received for -- cellId(%d),ueId(%d)",
                     ueCb->key.cellId, ueCb->key.ueId);


   /* Delete Ue Cb */
   ret=pjDbmDelUlUeCb(gCb, ueCb, FALSE);

   /* Fill the Cfm */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
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
PUBLIC S16 pjCfgPrcSecUlCfg
(
PjCb             *gCb,
CpjSecCfgReqInfo *secCfg,
CpjSecCfgCfmInfo *cfgCfm
)
#else
PUBLIC S16 pjCfgPrcSecUlCfg (gCb,secCfg, cfgCfm)
PjCb              *gCb;
CpjSecCfgReqInfo  *secCfg;
CpjSecCfgCfmInfo  *cfgCfm;
#endif
{
   S16     ret;
   PjUlUeCb *ueCb;

   TRC3(pjCfgPrcSecUlCfg);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;

   RLOG2(L_DEBUG, "pjCfgPrcSecCfg- Received for -- cellId(%d),ueId(%d)",
                     secCfg->cellId, secCfg->ueId);

   /* Fetch Ue Cb */
   ret = pjDbmFetchUlUeCb(gCb, secCfg->ueId, secCfg->cellId, &ueCb);

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
   ueCb->secInfo.secCxt.dir    = PJ_SEC_DIR_UL;

   RLOG_ARG3(L_DEBUG,DBG_UEID,secCfg->ueId, 
         "Security Paramters stored in UE CB are -- cellId(%d),"
          " Integrity Algo(%ld), cipherAlgoType(%ld) ",
           secCfg->cellId, ueCb->secInfo.intInfo.algoType,
           ueCb->secInfo.cipherInfo.algoType);

   /* Open the Security Channels */

   if(ueCb->secInfo.secAct)
   {

#ifdef PJ_SEC_ASYNC
      ueCb->secInfo.secCxt.cfgTxId = ueCb->libInfo.nxtAvlbTxId;
#endif 

      ret = pjUtlUlIntInit(gCb,ueCb);

      /* Integrity Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_INTINIT_FAILED);

         RETVALUE(ret);

      }
      ret = pjUtlUlCpInit(gCb,ueCb);
      /* Ciphering Control Plane Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_CCIPHR_FAILED);

         RETVALUE(ret);

      }
      ret = pjUtlUlUpInit(gCb,ueCb);

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
PUBLIC S16 pjCfgUlUeIdChng
(
PjCb      *gCb,
CpjUeInfo *ueInfo,
CpjUeInfo *newUeInfo,
CmStatus  *status
)
#else
PUBLIC S16 pjCfgUlUeIdChng (gCb, ueInfo, newUeInfo, status)
PjCb      *gCb;
CpjUeInfo *ueInfo;
CpjUeInfo *newUeInfo;
CmStatus  *status;
#endif
{
   PjUlUeCb *ueCb;
   S16    ret;

   TRC3(pjCfgUlUeIdChng)

   ueCb = NULLP;
   ret= ROK;

   RLOG2(L_DEBUG, "pjCfgUeIdChng- Recived for -- cellId(%d),ueId(%d)",
                     ueInfo->cellId, ueInfo->ueId);

   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      status->reason = CPJ_CFG_REAS_SAME_UEID;
      status->status = CPJ_CFG_CFM_NOK; 

#ifdef DEBUGP
      RLOG_ARG3(L_ERROR,DBG_UEID,ueInfo->ueId, "Changing UE Id -- Failed :"
           "old and New UeId are Same - cellId(%d),newCellId(%d), newUeId(%d)",
                ueInfo->cellId, newUeInfo->cellId, newUeInfo->ueId);
#endif
      RETVALUE(RFAILED);
   } 
   
   ret = pjDbmFetchUlUeCb(gCb, newUeInfo->ueId, newUeInfo->cellId, &ueCb);
   if (ret == ROK)
   {

#ifdef DEBUGP
      RLOG_ARG1(L_ERROR,DBG_UEID,ueInfo->ueId,
            "Changing Ue ID Failed :New UeId already Exists -- cellId(%d)",
                     newUeInfo->cellId );
#endif
      status->reason = CPJ_CFG_REAS_SAME_UEID;
      status->status = CPJ_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   ret = pjDbmFetchUlUeCb(gCb, ueInfo->ueId, ueInfo->cellId, &ueCb);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, ueInfo->cellId,"UeId[%u] not found",ueInfo->ueId);
      status->reason = CPJ_CFG_REAS_UE_UNKWN;
      status->status = CPJ_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   ret = cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb);
   if (ret != ROK)
   {

#ifdef DEBUGP
      RLOG_ARG1(L_ERROR,DBG_UEID,ueInfo->ueId, "cmHashListDelete Failed -- cellId(%d)",
                     ueInfo->cellId);
#endif
      status->reason = CPJ_CFG_REAS_UE_CREAT_FAIL;
      status->status = CPJ_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
   
   /* update the hash key with new values */ 
   ueCb->key.ueId = newUeInfo->ueId;
   ueCb->key.cellId = newUeInfo->cellId;

    ret = cmHashListInsert(&(gCb->u.ulCb->ueLstCp), (PTR)ueCb, 
          (U8 *)&(ueCb->key), (U16) sizeof(PjUeKey));

  if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG2(L_ERROR,DBG_UEID,ueInfo->ueId, 
            "cmHashListInsert Failed -- cellId(%d),New ueId(%d)",
                     newUeInfo->cellId, newUeInfo->ueId);
#endif
      status->reason = CPJ_CFG_REAS_UE_CREAT_FAIL;
      status->status = CPJ_CFG_CFM_NOK;
   }  
  
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
PUBLIC S16 pjCfgPrcUlCount
(
PjCb            *gCb,
CpjCountReqInfo *countReq,
CpjCountCfmInfo       *countCfm,
UdxCountCfmInfo *cntCfm
)
#else
PUBLIC S16 pjCfgPrcUlCount (gCb, countReq, countCfm)
PjCb            *gCb;
CpjCountReqInfo *countReq;
CpjCountCfmInfo       *countCfm;
UdxCountCfmInfo *cntCfm;
#endif
{
   S16     ret;
   PjUlUeCb  *ueCb;
   PjUlRbCb    *rbCb;
   U8      numdlRb;
   U8      idx;
   U8      idx1;

   TRC3(pjCfgPrcUlCount); 

   /* Initialization */
   ueCb   = NULLP;
   rbCb   = NULLP;
   ret    = ROK;
   idx  = 0;
   idx1  = 0;
   numdlRb = cntCfm->numRb;

   RLOG2(L_DEBUG, "pjCfgPrcCount - Count Request Received for - cellId(%d),ueId(%d)",
                     countReq->cellId, countReq->ueId);

   /* Fetch  ue Cb */
   ret = pjDbmFetchUlUeCb(gCb, countReq->ueId, countReq->cellId, &ueCb);
   countCfm->ueId = countReq->ueId;
   countCfm->cellId = countReq->cellId;
   countCfm->transId = countReq->transId;

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      RLOG_ARG1(L_ERROR, DBG_CELLID, countReq->cellId,"UeId[%u] not found",countReq->ueId);
      /* Fill entCfm structure */
      countCfm->numRb  = cntCfm->numRb;
      countCfm->status = CPJ_CFG_CFM_NOK;
      countCfm->reason = CPJ_CFG_REAS_UE_UNKWN;
      RETVALUE(ret);
   }

   /* Loop through all DRBs  */
   for(idx=0; idx < PJ_MAX_DRB_PER_UE; idx++)     /* KW_FIX */
   {
      rbCb = ueCb->drbCb[idx];
      /* RB is present and configured for PDCP Layer */
      if(NULLP != rbCb)
      {
         for(idx1=0; idx1 < numdlRb; idx1++)
         {
            if(rbCb->rbId == countCfm->countInfo[idx1].rbId)
            {
               countCfm->countInfo[idx1].ulCount= (( rbCb->ulCb.rxHfn << rbCb->snLen) | rbCb->ulCb.nxtRxSn); 
               break;
            }   
            else
            {    
               countCfm->countInfo[countCfm->numRb].rbId= rbCb->rbId; 
               countCfm->countInfo[countCfm->numRb].dir = rbCb->dir; 
               countCfm->countInfo[countCfm->numRb].ulCount= (( rbCb->ulCb.rxHfn << rbCb->snLen) | rbCb->ulCb.nxtRxSn); 
               countCfm->countInfo[countCfm->numRb].dlCount= 0; 
               countCfm->numRb++;  
            }
         } 
         if(numdlRb == 0)
         {  
            countCfm->countInfo[countCfm->numRb].rbId= rbCb->rbId; 
            countCfm->countInfo[countCfm->numRb].dir = rbCb->dir; 
            countCfm->countInfo[countCfm->numRb].ulCount= (( rbCb->ulCb.rxHfn << rbCb->snLen) | rbCb->ulCb.nxtRxSn); 
            countCfm->countInfo[countCfm->numRb].dlCount= 0; 
            countCfm->numRb++;  

         }

      }

   }

   /* Fill CountCfm with status and reason */
   if(countCfm->numRb)
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
PUBLIC S16 pjCfgPrcUlSduSta
(
PjCb                 *gCb,
CpjSduStaReqInfo     *staReq,
UdxSduStaCfmInfo     *staCfm
)
#else
PUBLIC S16 pjCfgPrcUlSduSta (gCb, staReq, staCfm)
PjCb                 *gCb;
CpjSduStaReqInfo     *staReq;
UdxSduStaCfmInfo     *staCfm;
#endif
{
   S16                ret;
   PjUlUeCb          *ueCb;
   CpjSduStaCfmInfo  *cfmInfo;

   TRC3(pjCfgPrcUlSduSta);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;

   RLOG2(L_DEBUG, "pjCfgPrcSduSta - Received for -- cellId(%d),ueId(%d)",
                     staReq->cellId, staReq->ueId);


   /* Fetch Ue Cb */
   ret = pjDbmFetchUlUeCb(gCb, staReq->ueId, staReq->cellId, &ueCb);

   /* UeCb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      RLOG_ARG1(L_ERROR, DBG_CELLID, staReq->cellId,"UeId[%u] not found",staReq->ueId);
      /* Memory leak fix ccpu00135359 */
      PjCpjSapCb        *cpjSap;
      cpjSap = &gCb->u.ulCb->cpjSap;
      if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cfmInfo, 
         sizeof (CpjSduStaCfmInfo)) != ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed");
         RETVALUE(RFAILED);
      }

      cfmInfo->ueId = staReq->ueId;
      cfmInfo->cellId = staReq->cellId;
      cfmInfo->transId = staReq->transId;
      cfmInfo->status = CPJ_CFG_CFM_NOK; 
      cfmInfo->reason = CPJ_CFG_REAS_UE_UNKWN;

      /* Caller should return cfm with failure */
      RLOG_ARG1(L_DEBUG,DBG_UEID,staReq->ueId, "UE Cb is not found -- cellId(%d)",
                     staReq->cellId);

      /* Send Failure SduStaCfm */
      PjUiCpjSduStaCfm(&gCb->u.ulCb->cpjSap.pst, 
                        gCb->u.ulCb->cpjSap.suId, cfmInfo);

      
      RETVALUE(RFAILED);
   }

   /* Call Ho Start handler for this UE */
   pjUtlUlReEstHO(gCb, ueCb);

   ueCb->hoInfo->transId = staReq->transId;
   ueCb->hoInfo->staCfm = staCfm;

   if (ueCb->libInfo.numReEstCmp > 0)
   {
      if (ueCb->libInfo.numReEstCmp > ueCb->libInfo.numReEstDrb)
      {
         /* Error Case */
         ueCb->libInfo.numReEstDrb = 0;
         ueCb->libInfo.numReEstCmp = 0;
      } 
      else
      {
         ueCb->libInfo.numReEstDrb -= ueCb->libInfo.numReEstCmp;
         ueCb->libInfo.numReEstCmp = 0;
      }
   }

   if (ueCb->libInfo.numReEstDrb == 0)
   {
      pjUtlUlSndSduStaCfm(gCb,ueCb);
   }

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
PRIVATE S16 pjCfgUpdUlCompInfo 
(
PjCb                   *gCb,
PjUlRbCb               *rbCb,
PjUlUeCb               *ueCb,
CpjHdrCompCfg        *rohcCfg
)
#else
PRIVATE S16 pjCfgUpdUlCompInfo(gCb, rbCb, ueCb, rohcCfg)
PjCb                   *gCb; 
PjUlRbCb               *rbCb;
PjUlUeCb               *ueCb;
CpjHdrCompCfg        *rohcCfg;
#endif
{
   U16   profId;
   U8    idx;
   S16   ret;

   TRC3(pjCfgUpdUlCompInfo);

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
      ret=pjUtlUlCmpInit(gCb, rbCb); 
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
PUBLIC S16  pjCfgDelPjUlRbCb 
(
PjCb                 *gCb,
PjUlUeCb             *ueCb,
PjUlRbCb             *rbCb
)
#else
PUBLIC S16 pjCfgDelPjUlRbCb(gCb,ueCb,rbCb)
PjCb                  *gCb;
PjUlUeCb              *ueCb;
PjUlRbCb              *rbCb;
#endif
{   
   S16  ret;
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS_COMMENT
   U8   evtIdx;
   U8   qciIdx;
#endif
   TRC3(pjCfgDelPjUlRbCb);

   /* Initialization */
   ret = ROK;

   RLOG0(L_DEBUG,"pjCfgDelPjRBCb- Recieved -- rbId() CfgState() ");

   /* RbCb is present and Configured for PDCP */
   if(NULLP != rbCb) 
   {
      RLOG1(L_DEBUG, "pjCfgDelPjRBCb- Received for -- rbId (%d)",
               rbCb->rbId);

#ifdef LTE_L2_MEAS
      /* This should be done for every counter added for UL */
      if((rbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS] != NULLP) &&
          (rbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->totDrbsPerQci > 0))
      {
         rbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->totDrbsPerQci--;
      }
#endif
      rbCb->ueCb   = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
      /* Stop off board timers */  

      if(PJ_DIR_UL & rbCb->dir)
          if(rbCb->ulCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
          {
                pjStopTmr(gCb, (PTR)&rbCb, PJ_EVT_UL_OBD_TMR); 
          }
#endif
     
      /* Deinit Rx Buffers */
      pjDbmRxDeInit(gCb,&rbCb->ulCb.recBuf);
      pjUtlEmptyUlPktList(gCb, rbCb);

      /* Close the Comp channels */  
      if (PJ_SRB == rbCb->rbType)
      {
         ueCb->numSrbs--;
      }
      else if (PJ_DRB == rbCb->rbType)
      {
         ueCb->numDrbs--;
         if (rbCb->rohc.hdrCmpUsed)
         {
            pjUtlCmpClose(gCb,rbCb->cmpCxtId);
         }
      }

      /* Free the rbCb */
      pjDbmDelUlRbCb(gCb,rbCb->rbId, rbCb->rbType, &rbCb, PJ_CFG_PDCP, ueCb);

   } 
   RETVALUE(ret);
} /* pjCfgDelPjRBCb */


/**
 * @brief
 * Handler to process the re-establishment Request for SRB1 during
 * RRC Re-Establishment request.
 *
 * @b Description:
 * 1. This primitive is used to re-establish SRB1 only during RRC Re-establishment 
 * 2. New Security Algo will applied 
 *
 * @param [in]    reEstReq   -  Entity Configuration to be done.
 * @param [out]   reEstCfm   -  Entity Configuration cfm.
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PUBLIC S16 pjCfgUlReEstReq
(
PjCb              *gCb,
CpjReEstReqInfo   *reEstReq,
CpjReEstCfmInfo   *reEstCfm
)
#else
PUBLIC S16 pjCfgUlReEstReq (gCb, reEstReq, reEstCfm)
PjCb              *gCb;
CpjReEstReqInfo   *reEstReq;
CpjReEstCfmInfo   *reEstCfm;
#endif
{
   S16     ret;
   PjUlUeCb  *ueCb;
   PjUlRbCb    *rbCb;
   CmLteRlcId rlcId;

   TRC3(pjCfgUlReEstReq);
 
   /* Initialization */
   ret = ROK;
   rlcId.ueId   =  reEstReq->pdcpId.ueId;
   rlcId.cellId   =  reEstReq->pdcpId.cellId;
   rlcId.rbId   =  reEstReq->pdcpId.rbId;
   rlcId.rbType   =  reEstReq->pdcpId.rbType;

   RLOG2(L_DEBUG, "pjCfgReEstReq- Received for -- cellId(%d),ueId(%d)",
                     rlcId.cellId, rlcId.ueId);

   ret = pjDbmFetchUlRbCb(gCb, rlcId,&rbCb);
   
   
   /* Ue Cb is not present */
   if( ROK != ret || NULLP == rbCb)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId.ueId, "CellId[%u]:RbId[%d] not found",
            rlcId.cellId, rlcId.rbId);
      /* Fill entCfm structure */
      PJ_CFG_FILL_REESTREQ_CFM(reEstCfm, rlcId.ueId,rlcId.cellId,
            reEstReq->transId, LCM_PRIM_NOK);
      RETVALUE(ret);
   }
  
   ueCb = rbCb->ueCb;

   /* Copy Security Information */ 
   cmMemcpy(&ueCb->secInfo.intInfo.intKey[0], &reEstReq->intKey[0], PJ_INTG_KEY_LEN);
   cmMemcpy(&ueCb->secInfo.cipherInfo.cpKey[0], &reEstReq->ctrlplnKey[0], PJ_CIPHER_KEY_LEN);
   cmMemcpy(&ueCb->secInfo.cipherInfo.upKey[0], &reEstReq->usrplnKey[0], PJ_CIPHER_KEY_LEN);

   /* Mark Security Action Done */
   ueCb->secInfo.secAct = TRUE;

   reEstCfm->transId = reEstReq->transId;
   reEstCfm->ueId    = reEstReq->pdcpId.ueId;
   reEstCfm->cellId  = reEstReq->pdcpId.cellId;

   /* Save the information */
#ifdef PJ_SEC_ASYNC
   pjUtlUlSaveCfmInfo(gCb,rbCb->ueCb, PJ_REEST_ASYNC_CFM, FALSE, ENTPJ, 
                                          (PTR)reEstCfm, NULLP);
   PJ_FREE(gCb,reEstCfm, sizeof(CpjReEstCfmInfo));
#endif /* PJ_SEC_ASYNC */

   /* Call ReEst for SRB1 */
   pjUtlUlReEstSrb1(gCb,rbCb);

   RETVALUE(ROK);
} /* pjCfgReEstReq */

#ifdef LTE_L2_MEAS
/**
 * @brief
 * Handler to add measCb to rbCb.
 *
 * @b Description:
 * Handler to add measCb to rbCb.
 *
 * 
 * @return  ROK
 *
 */
PRIVATE S16 pjAddToUlL2Meas(PjCb *tPjCb, PjUlRbCb *pjRbCb,U8 cellId, U8 ueId)
{
   U32 cntr;

   RLOG1(L_DEBUG, "L2_MEAS_ADD Adding UE ueid %d ", ueId);

   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      PjL2MeasCb *measCb = &tPjCb->u.ulCb->pjL2Cb.pjL2EvtCb[cntr].measCb;

      if (measCb->measType & LPJ_L2MEAS_UL_LOSS)
      {
         pjUtlPlcMeasDatInUlL2Sts(&measCb->measData[pjRbCb->qci],
               &pjRbCb->rbL2Cb, measCb->measType);
         measCb->qci[pjRbCb->qci] = pjRbCb->qci;
         measCb->measData[pjRbCb->qci].totDrbsPerQci++;
         measCb->numQci++;
      }
   }
   RETVALUE(ROK);
}/*pjAddToDlL2Meas*/ 
#endif

/*@}*/

/********************************************************************30**

         End of file:     eq_cfg_ul.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:47 2015

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
