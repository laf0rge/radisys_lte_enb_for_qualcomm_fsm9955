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
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_dbm.c
  
     Sid:      gk_sch_dbm.c@@/main/2 - Sat Jul 30 02:21:38 2011
  
     Prg:      rr 
  
**********************************************************************/

/** @file rg_sch_dbm.c
@brief This file contains the APIs exposed for the database handling of the scheduler.
*/
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=236;
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_llist.x"      /* common linked list library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "rgr.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg_sch.x"
#include "rl_interface.h"
#include "rl_common.h"


/* local defines */
PRIVATE S16 rgSCHDbmInitUeCbLst ARGS(( RgSchCellCb *cellCb, U16 numBins));
#ifdef LTE_TDD
PRIVATE S16 rgSCHDbmInitUeTfuPendLst ARGS(( RgSchCellCb *cellCb, U16 numBins));
#endif
PRIVATE Void rgSCHDbmInitDedLcLst ARGS((RgSchUeCb *ueCb));
PRIVATE Void rgSCHDbmInitCmnLcLst ARGS((RgSchCellCb *cellCb));
#ifdef LTEMAC_SPS
PRIVATE S16 rgSCHDbmInitSpsUeCbLst ARGS((RgSchCellCb *cellCb,
                      U16 numBins));
#endif
PRIVATE Void rgSCHDbmInitRaCbLst ARGS(( RgSchCellCb *cellCb));
#ifndef LTE_TDD
PRIVATE Void rgSCHDbmInitRaReqLst ARGS(( RgSchCellCb *cellCb));
#endif
PRIVATE Void rgSCHDbmInitCrntRgrCfgLst ARGS(( RgSchCellCb *cellCb));
PRIVATE Void rgSCHDbmInitPndngRgrCfgLst ARGS(( RgSchCellCb *cellCb));

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 * @brief Handler for Initializing the cell.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCell
 *     
 *   Initializes the lists belonging to the cell.
 *     
 *           
 *  @param[in]  RgSchCellCb *cellCb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmInitCell
(
RgSchCellCb       *cellCb
)
#else
PUBLIC S16 rgSCHDbmInitCell(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   S16 ret;
   
   TRC2(rgSCHDbmInitCell);

   /* Initialize ue list */
   if ((ret = rgSCHDbmInitUeCbLst(cellCb, RGSCH_MAX_UE_BIN_PER_CELL)) != ROK)
      RETVALUE(ret);
#ifdef LTE_TDD
   if ((ret = rgSCHDbmInitUeTfuPendLst(cellCb, 
                                       RGSCH_MAX_UE_BIN_PER_CELL)) != ROK)
      RETVALUE(ret);
#endif

#ifdef LTEMAC_SPS
   /* Initialize SPS Ue list */
   if ((ret = rgSCHDbmInitSpsUeCbLst(cellCb, RGSCH_MAX_UE_BIN_PER_CELL)) != ROK)
      RETVALUE(ret);
#endif /* LTEMAC_SPS */

   /* Initialize BCCH/PCCH logical channels */
   rgSCHDbmInitCmnLcLst(cellCb);

   /* Initialize configuration lists */
   rgSCHDbmInitCrntRgrCfgLst(cellCb);
   rgSCHDbmInitPndngRgrCfgLst(cellCb);

#ifndef LTE_TDD
   /* Initialize raReq list */
   rgSCHDbmInitRaReqLst(cellCb);
#endif

   /* Initialize raCb list */
   rgSCHDbmInitRaCbLst(cellCb);

   /* Initialize l2mList */
#ifdef LTE_L2_MEAS 
   cmLListInit(&cellCb->l2mList);
#endif /* LTE_L2_MEAS */

   RETVALUE(ret);

} /* rgSCHDbmInitCell */

/**
 * @brief Handler for initializing the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInitUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDbmInitUeCbLst
(
RgSchCellCb    *cellCb,
U16            numBins
)
#else
PRIVATE S16 rgSCHDbmInitUeCbLst(cellCb, numBins)
RgSchCellCb    *cellCb;
U16            numBins;
#endif
{
   RgSchUeCb ue;  
   TRC2(rgSCHDbmInitUeCbLst)

   /* Fix: syed It is better to compute offset dynamically
    * rather than hardcoding it as 0 */      
   RETVALUE(cmHashListInit(&cellCb->ueLst, numBins, (U16)((PTR)&(ue.ueLstEnt) - (PTR)&ue), FALSE, 
               CM_HASH_KEYTYPE_CONID,
               rgSchCb[cellCb->instIdx].rgSchInit.region, 
               rgSchCb[cellCb->instIdx].rgSchInit.pool));

}  /* rgSCHDbmInitUeCbLst */

/**
 * @brief Handler for de-initializing the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDeInitUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmDeInitUeCbLst
(
RgSchCellCb       *cellCb
)
#else
PUBLIC S16 rgSCHDbmDeInitUeCbLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmDeInitUeCbLst)

   RETVALUE(cmHashListDeinit(&cellCb->ueLst));

}  /* rgSCHDbmDeInitUeCbLst */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for initializing the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInitSpsUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDbmInitSpsUeCbLst
(
RgSchCellCb       *cellCb,
U16               numBins
)
#else
PRIVATE S16 rgSCHDbmInitSpsUeCbLst(cellCb, numBins)
RgSchCellCb       *cellCb;
U16               numBins;
#endif
{
   RgSchUeCb ue;
   TRC2(rgSCHDbmInitSpsUeCbLst)

   RETVALUE(cmHashListInit(&cellCb->spsUeLst, numBins, (U16) ((PTR) &(ue.spsUeLstEnt) - (PTR) &ue), FALSE, 
               CM_HASH_KEYTYPE_CONID,
               rgSchCb[cellCb->instIdx].rgSchInit.region, 
               rgSchCb[cellCb->instIdx].rgSchInit.pool));

}  /* rgSCHDbmInitSpsUeCbLst */

/**
 * @brief Handler for de-initializing the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDeInitSpsUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmDeInitSpsUeCbLst
(
RgSchCellCb       *cellCb
)
#else
PUBLIC S16 rgSCHDbmDeInitSpsUeCbLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmDeInitSpsUeCbLst)

   RETVALUE(cmHashListDeinit(&cellCb->spsUeLst));

}  /* rgSCHDbmDeInitSpsUeCbLst */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for inserting the ueCb in to the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsUeCb
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmInsUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHDbmInsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHDbmInsUeCb)

   RETVALUE(cmHashListInsert(&cellCb->ueLst, (PTR)ueCb, 
      (U8 *)&ueCb->ueId, (U16)sizeof(ueCb->ueId)));

}  /* rgSCHDbmInsUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for inserting the ueCb in to the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsSpsUeCb
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmInsSpsUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHDbmInsSpsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHDbmInsSpsUeCb)

   RETVALUE(cmHashListInsert(&cellCb->spsUeLst, (PTR)ueCb, 
      (U8 *)&ueCb->spsRnti, (U16)sizeof(ueCb->spsRnti)));

}  /* end of rgSCHDbmInsSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for accessing the existing ueCb identified by the key ueId
 * in the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgSchUeCb*
 **/
#ifdef ANSI
PUBLIC RgSchUeCb* rgSCHDbmGetUeCb
(
RgSchCellCb       *cellCb, 
CmLteRnti      ueId
)
#else
PUBLIC RgSchUeCb* rgSCHDbmGetUeCb(cellCb, ueId)
RgSchCellCb       *cellCb;
CmLteRnti      ueId;
#endif
{
   RgSchUeCb *ueCb = NULLP; 

   TRC2(rgSCHDbmGetUeCb)

   cmHashListFind(&cellCb->ueLst, (U8 *)&ueId,
      sizeof(ueId), 0, (PTR *)&ueCb);
   RETVALUE(ueCb);
}  /* rgSCHDbmGetUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for accessing the existing ueCb identified by the key 
 * spsRnti in the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgSchUeCb*
 **/
#ifdef ANSI
PUBLIC RgSchUeCb* rgSCHDbmGetSpsUeCb
(
RgSchCellCb       *cellCb, 
CmLteRnti         spsRnti
)
#else
PUBLIC RgSchUeCb* rgSCHDbmGetSpsUeCb(cellCb, spsRnti)
RgSchCellCb       *cellCb;
CmLteRnti         spsRnti;
#endif
{
   RgSchUeCb *ueCb = NULLP; 

   TRC2(rgSCHDbmGetSpsUeCb)

   cmHashListFind(&cellCb->spsUeLst, (U8 *)&spsRnti,
      sizeof(spsRnti), 0, (PTR *)&ueCb);
   RETVALUE(ueCb);
}  /* rgSCHDbmGetSpsUeCb */
#endif

/**
 * @brief Handler for accessing the existing next ueCb in the ueCbLst under the
 *  cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgSchUeCb*
 **/
#ifdef ANSI
PUBLIC RgSchUeCb* rgSCHDbmGetNextUeCb
(
RgSchCellCb       *cellCb, 
RgSchUeCb         *ueCb
)
#else
PUBLIC RgSchUeCb* rgSCHDbmGetNextUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   RgSchUeCb *nextUeCb = NULLP; 

   TRC2(rgSCHDbmGetNextUeCb)

   cmHashListGetNext(&cellCb->ueLst, (PTR) ueCb, (PTR *)&nextUeCb);
   RETVALUE(nextUeCb);
}  /* rgSCHDbmGetNextUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for accessing the existing next ueCb stored in the spsUeCbLst
 *         using SPS-Rnti under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgSchUeCb*
 **/
#ifdef ANSI
PUBLIC RgSchUeCb* rgSCHDbmGetNextSpsUeCb
(
RgSchCellCb       *cellCb, 
RgSchUeCb         *ueCb
)
#else
PUBLIC RgSchUeCb* rgSCHDbmGetNextSpsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   RgSchUeCb *nextUeCb = NULLP; 

   TRC2(rgSCHDbmGetNextSpsUeCb)

   cmHashListGetNext(&cellCb->spsUeLst, (PTR) ueCb, (PTR *)&nextUeCb);
   RETVALUE(nextUeCb);
}  /* end of rgSCHDbmGetNextSpsUeCb */

#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
/**
 * @brief Handler for Cleaning up L2 Meas related Data in
 * cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDelL2MUe
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmDelL2MUe
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHDbmDelL2MUe(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   U8 lcCnt = 0;
   TRC2(rgSCHDbmDelL2MUe)

   ueCb->ul.hqEnt.numBusyHqProcs = 0;
   /* Clean cell level UE Active Count */
   for (lcCnt =0; lcCnt < RGSCH_MAX_LC_PER_UE; lcCnt++)
   {
      if (ueCb->ul.lcCb[lcCnt].isValid)
      {
         if((ueCb->ul.lcCb[lcCnt].qciCb->ulUeCount) &&
               (ueCb->ulActiveLCs &
                (1 << (ueCb->ul.lcCb[lcCnt].qciCb->qci -1))))
         {
            ueCb->ul.lcCb[lcCnt].qciCb->ulUeCount--;
            ueCb->ulActiveLCs &= ~(1 << 
                  (ueCb->ul.lcCb[lcCnt].qciCb->qci -1));
         }
      }

      if (ueCb->dl.lcCb[lcCnt])
      {
         if (ueCb->qciActiveLCs[ueCb->dl.lcCb[lcCnt]->qciCb->qci])
         {
            ueCb->dl.lcCb[lcCnt]->qciCb->dlUeCount--;
            ueCb->qciActiveLCs[ueCb->dl.lcCb[lcCnt]->qciCb->qci] = 0;
         }
      }
   }

   RETVALUE(ROK);
}  /* rgSCHDbmDelL2MUe */

#endif

/**
 * @brief Handler for deleting the existing ueCb from the ueCbLst under the
 * cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDelUeCb
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmDelUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHDbmDelUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHDbmDelUeCb)

   RETVALUE(cmHashListDelete(&cellCb->ueLst, (PTR)ueCb));
}  /* rgSCHDbmDelUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for deleting the existing ueCb from the spsUeCbLst under the
 * cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDelSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmDelSpsUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHDbmDelSpsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHDbmDelSpsUeCb)

   RETVALUE(cmHashListDelete(&cellCb->spsUeLst, (PTR)ueCb));
}  /* end of rgSCHDbmDelSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for Initializing the UE.
 *
 * @details
 *
 *     Function : rgSCHDbmInitUe
 *     
 *   Initializes the lists belonging to the UE.
 *     
 *           
 *  @param[in]  RgSchUeCb  *ueCb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmInitUe
(
RgSchUeCb       *ueCb
)
#else
PUBLIC S16 rgSCHDbmInitUe(ueCb)
RgSchUeCb       *ueCb;
#endif
{
   S16 ret = ROK;
   
   TRC2(rgSCHDbmInitUe);

   /* Initialize Dedicated logical channels */
   rgSCHDbmInitDedLcLst(ueCb);

   RETVALUE(ret);
} /* rgSCHDbmInitUe */

/**
 * @brief Handler for Initializing the dedicated logical channels.
 *
 * @details
 *
 *     Function : rgSCHDbmInitDedLcLst
 *     
 *   Initializes dedicated logical channels.
 *           
 *  @param[in]  RgSchUeCb *ueCb
 *  @return     Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHDbmInitDedLcLst
(
RgSchUeCb       *ueCb
)
#else
PRIVATE Void rgSCHDbmInitDedLcLst(ueCb)
RgSchUeCb       *ueCb;
#endif
{
   U8 idx;
   
   TRC2(rgSCHDbmInitDedLcLst);

   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      /* Set Dedicated LCs as not configured */
      ueCb->ul.lcCb[idx].isValid = FALSE;
      ueCb->dl.lcCb[idx] = NULLP;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;
  

} /* rgSCHDbmInitDedLcLst */

/**
 * @brief Handler for Initializing the common logical channel list of the cell.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCmnLcLst
 *     
 *   Initializes following common logical channels belonging to the cell.
 *     - BCCH on BCH
 *     - BCCH on DLSCH
 *     - PCCH
 *           
 *  @param[in]  RgSchCellCb *cellCb
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDbmInitCmnLcLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitCmnLcLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   U8 idx;
   
   TRC2(rgSCHDbmInitCmnLcLst);

   for (idx = 0; idx < RGSCH_MAX_CMN_LC_CB; idx++)
   {
      cellCb->cmnLcCb[idx].lcId = RGSCH_INVALID_LC_ID;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

} /* rgSCHDbmInitCmnLcLst */

/**
 * @brief Handler for inserting dedicated DL logical channel.
 *
 * @details
 *
 *     Function : rgSCHDbmInsDlDedLcCb
 *     
 *  @param[in]  RgSchUeCb *ueCb
 *  @param[in]  RgSchDlLcCb* dlLcCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsDlDedLcCb
(
RgSchUeCb         *ueCb, 
RgSchDlLcCb       *dlLcCb 
)
#else
PUBLIC Void rgSCHDbmInsDlDedLcCb(ueCb, dlLcCb)
RgSchUeCb         *ueCb; 
RgSchDlLcCb       *dlLcCb; 
#endif
{
   TRC2(rgSCHDbmInsDlDedLcCb);

   ueCb->dl.lcCb[dlLcCb->lcId - 1] = dlLcCb;

}  /* rgSCHDbmInsDlDedLcCb */

/**
 * @brief Handler for deleting dedicated DL logical channel.
 *
 * @details
 *
 *     Function : rgSCHDbmDelDlDedLcCb
 *     
 *  @param[in]  RgSchUeCb *ueCb
 *  @param[in]  RgSchDlLcCb* dlLcCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmDelDlDedLcCb
(
RgSchUeCb         *ueCb, 
RgSchDlLcCb       *dlLcCb 
)
#else
PUBLIC Void rgSCHDbmDelDlDedLcCb(ueCb, dlLcCb)
RgSchUeCb         *ueCb; 
RgSchDlLcCb       *dlLcCb; 
#endif
{
   TRC2(rgSCHDbmDelDlDedLcCb);

#ifdef LTE_L2_MEAS
   /* Clean cell level UE Active Count */

   if (ueCb->dl.lcCb[dlLcCb->lcId - 1])
   {
      if ((dlLcCb->qciCb)
            && (ueCb->qciActiveLCs[dlLcCb->qciCb->qci]))
      {
         ueCb->qciActiveLCs[dlLcCb->qciCb->qci]--;
         if (!(ueCb->qciActiveLCs[dlLcCb->qciCb->qci]))
         {
            dlLcCb->qciCb->dlUeCount--;
         }
      }
   }
#endif /* LTE_L2_MEAS */

   ueCb->dl.lcCb[dlLcCb->lcId - 1] = NULLP;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

}  /* rgSCHDbmDelDlDedLcCb */

/**
 * @brief Handler for accessing the existing dl dedicated lcCb at idx in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetDlDedLcCb
 *     
 *  @param[in]  *ueCb
 *  @param[in]  idx
 *  @return  RgSchDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchDlLcCb* rgSCHDbmGetDlDedLcCb
(
RgSchUeCb         *ueCb, 
CmLteLcId        idx
)
#else
PUBLIC RgSchDlLcCb* rgSCHDbmGetDlDedLcCb(ueCb, idx)
RgSchUeCb         *ueCb; 
CmLteLcId        idx;
#endif
{
   TRC2(rgSCHDbmGetDlDedLcCb);

   if (idx < RGSCH_DEDLC_MIN_LCID || idx > RGSCH_DEDLC_MAX_LCID)
   {
      RETVALUE(NULLP);
   }
   RETVALUE(ueCb->dl.lcCb[idx-1]);

}  /* rgSCHDbmGetDlDedLcCb */

/**
 * @brief Handler for accessing the existing first dl dedicated lcCb at idx 
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetFirstDlDedLcCb
 *     
 *           
 *  @param[in]  *ueCb
 *  @return  RgSchDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb
(
RgSchUeCb         *ueCb
)
#else
PUBLIC RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb(ueCb)
RgSchUeCb         *ueCb; 
#endif
{
   U8 idx;
   TRC2(rgSCHDbmGetFirstDlDedLcCb)
   
   for(idx = 0; idx < RGSCH_DEDLC_MAX_LCID; idx++)
   {
      if(ueCb->dl.lcCb[idx])
      {
         RETVALUE(ueCb->dl.lcCb[idx]);
      }
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetFirstDlDedLcCb */
/**
 * @brief Handler for accessing the existing next dl dedicated lcCb at idx 
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextDlDedLcCb
 *     
 *           
 *  @param[in]  *ueCb
 *  @param[in]  *lcCb
 *  @return  RgSchDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb
(
RgSchUeCb         *ueCb,
RgSchDlLcCb       *lcCb
)
#else
PUBLIC RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb(ueCb, lcCb)
RgSchUeCb         *ueCb; 
RgSchDlLcCb       *lcCb;
#endif
{
   U8 idx;
   TRC2(rgSCHDbmGetNextDlDedLcCb);

   if (!lcCb)
   {
      RETVALUE(rgSCHDbmGetFirstDlDedLcCb(ueCb));
   }

   for(idx = lcCb->lcId; idx < RGSCH_DEDLC_MAX_LCID; idx++)
   {
      if(ueCb->dl.lcCb[idx])
      {
         RETVALUE(ueCb->dl.lcCb[idx]);
      }
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetNextDlDedLcCb */

/**
 * @brief Handler for accessing the existing dl common lcCb identified by the key lcId
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetCmnLcCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  lcId
 *  @return  RgSchClcDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb
(
RgSchCellCb       *cellCb, 
CmLteLcId         lcId
)
#else
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb(cellCb, lcId)
RgSchCellCb       *cellCb;
CmLteLcId         lcId;
#endif
{
   U8 idx;

   TRC2(rgSCHDbmGetCmnLcCb)

   for(idx = 0; idx < RGSCH_MAX_CMN_LC_CB; idx++)
   {
      if(cellCb->cmnLcCb[idx].lcId == lcId)
      {
         RETVALUE(&(cellCb->cmnLcCb[idx]));
      }
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetCmnLcCb */

/**
 * @brief Handler for accessing the existing BCCH mapped on to BCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetBcchOnBch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch
(
RgSchCellCb       *cellCb 
)
#else
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetBcchOnBch)
 
   if(cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].lcId != RGSCH_INVALID_LC_ID)
   {
      RETVALUE(&(cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX]));
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetBcchOnBch */

/**
 * @brief Handler for accessing the existing BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetFirstBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch
(
RgSchCellCb       *cellCb
)
#else
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetFirstBcchOnDlsch)

   if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].lcId != RGSCH_INVALID_LC_ID)
   {
      RETVALUE(&(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1]));
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetFirstBcchOnDlsch */

/**
 * @brief Handler for accessing the existing BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetSecondBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch
(
RgSchCellCb       *cellCb
)
#else
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetSecondBcchOnDlsch)

   if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].lcId != RGSCH_INVALID_LC_ID)
   {
      RETVALUE(&(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2]));
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetSecondBcchOnDlsch */

/**
 * @brief Handler for accessing the existing PCCH in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetPcch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
#ifdef ANSI
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetPcch
(
RgSchCellCb       *cellCb
)
#else
PUBLIC RgSchClcDlLcCb* rgSCHDbmGetPcch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetPcch)
 
   if(cellCb->cmnLcCb[RGSCH_PCCH_IDX].lcId != RGSCH_INVALID_LC_ID)
   {
      RETVALUE(&(cellCb->cmnLcCb[RGSCH_PCCH_IDX]));
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetPcch */

/**
 * @brief Handler for inserting the BCCH mapped on to BCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsBcchOnBch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsBcchOnBch
(
RgSchCellCb       *cellCb, 
RgSchClcDlLcCb   *cmnDlLcCb
)
#else
PUBLIC Void rgSCHDbmInsBcchOnBch(cellCb, cmnDlLcCb)
RgSchCellCb       *cellCb;
RgSchClcDlLcCb   *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInsBcchOnBch)

   cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].lcId = cmnDlLcCb->lcId;
   cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].boLst = cmnDlLcCb->boLst;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

}  /* rgSCHDbmInsBcchOnBch */

/**
 * @brief Handler for inserting the BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsBcchOnDlsch
(
RgSchCellCb       *cellCb, 
RgSchClcDlLcCb   *cmnDlLcCb
)
#else
PUBLIC Void rgSCHDbmInsBcchOnDlsch(cellCb, cmnDlLcCb)
RgSchCellCb       *cellCb;
RgSchClcDlLcCb   *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInsBcchOnDlsch)

   if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].lcId == RGSCH_INVALID_LC_ID)   
   { 
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].lcId = cmnDlLcCb->lcId;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].boLst = cmnDlLcCb->boLst;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].si = FALSE;
   }
   else if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].lcId == RGSCH_INVALID_LC_ID)
   { 
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].lcId = cmnDlLcCb->lcId;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].boLst = cmnDlLcCb->boLst;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].si = TRUE;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

}  /* rgSCHDbmInsBcchOnDlsch */


/**
 * @brief Handler for inserting the PCCH in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsPcch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsPcch
(
RgSchCellCb       *cellCb, 
RgSchClcDlLcCb   *cmnDlLcCb
)
#else
PUBLIC Void rgSCHDbmInsPcch(cellCb, cmnDlLcCb)
RgSchCellCb       *cellCb;
RgSchClcDlLcCb   *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInsPcch)

   cellCb->cmnLcCb[RGSCH_PCCH_IDX].lcId = cmnDlLcCb->lcId;
   cellCb->cmnLcCb[RGSCH_PCCH_IDX].boLst = cmnDlLcCb->boLst;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

}  /* rgSCHDbmInsPcch */

/**
 * @brief Handler for initializing the boLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCmnLcBoLst
 *     
 *           
 *  @param[in] *cmnDlLcCb 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInitCmnLcBoLst
(
RgSchClcDlLcCb       *cmnDlLcCb
)
#else
PUBLIC Void rgSCHDbmInitCmnLcBoLst(cmnDlLcCb)
RgSchClcDlLcCb       *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInitCmnLcBoLst)

   cmLListInit(&cmnDlLcCb->boLst);
   RETVOID;
}  /* rgSCHDbmInitCmnLcBoLst */

/**
 * @brief Handler for inserting the bo report in to the boLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInsCmnLcBoRpt
 *     
 *           
 *  @param[in] *cmnDlLcCb 
 *  @param[in] *boRpt 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsCmnLcBoRpt
(
RgSchClcDlLcCb    *cmnDlLcCb,
RgSchClcBoRpt     *cmnBoRpt
)
#else
PUBLIC Void rgSCHDbmInsCmnLcBoRpt(cmnDlLcCb, cmnBoRpt)
RgSchClcDlLcCb    *cmnDlLcCb;
RgSchClcBoRpt     *cmnBoRpt;
#endif
{
   TRC2(rgSCHDbmInsCmnLcBoRpt)

   cmnBoRpt->boLstEnt.next = NULLP;
   cmnBoRpt->boLstEnt.prev = NULLP;
   cmnBoRpt->boLstEnt.node = (PTR)cmnBoRpt;
   cmLListAdd2Tail(&cmnDlLcCb->boLst, &cmnBoRpt->boLstEnt);
   RETVOID;
}  /* rgSCHDbmInsCmnLcBoRpt */


/**
 * @brief Handler for initializing the raCbLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitRaCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDbmInitRaCbLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitRaCbLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmInitRaCbLst)

   cmLListInit(&cellCb->raInfo.raCbLst);
   RETVOID;
}  /* rgSCHDbmInitRaCbLst */


/**
 * @brief Handler for accessing the existing raCb in the raCbLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetRaCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  key
 *  @return  RgSchRaCb*
 **/
#ifdef ANSI
PUBLIC RgSchRaCb* rgSCHDbmGetRaCb
(
RgSchCellCb       *cellCb, 
CmLteRnti      key
)
#else
PUBLIC RgSchRaCb* rgSCHDbmGetRaCb(cellCb, key)
RgSchCellCb       *cellCb; 
CmLteRnti      key;
#endif
{
   CmLList *tmpNode;

   TRC2(rgSCHDbmGetRaCb)

   CM_LLIST_FIRST_NODE(&cellCb->raInfo.raCbLst,tmpNode);
   while(tmpNode)
   {
      if(((RgSchRaCb *)tmpNode->node)->tmpCrnti == key)
      {
         RETVALUE((RgSchRaCb *)(tmpNode->node));
      }
      CM_LLIST_NEXT_NODE(&cellCb->raInfo.raCbLst,tmpNode);
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetRaCb */

#ifndef LTE_TDD
/**
 * @brief Handler for initializing the raReqLst.
 g
 * @details
 *
 *     Function : rgSCHDbmInitRaReqLst
 *     
 *           
 *  @param[in]  *cellCb 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDbmInitRaReqLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitRaReqLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   U8 idx;

   TRC2(rgSCHDbmInitRaReqLst)

   /* ccpu00133557- Memory Leak Fix- initializing for the all nodes 
    * in RAREQ list*/
   for(idx = 0; idx < RGSCH_RAREQ_ARRAY_SIZE; idx++)
   {
      cmLListInit(&cellCb->raInfo.raReqLst[idx]);
   }
   RETVOID;
}  /* rgSCHDbmInitRaReqLst */
#endif

/**
 * @brief Handler for initializing the crnt rgr cfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCrntRgrCfgLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDbmInitCrntRgrCfgLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitCrntRgrCfgLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmInitCrntRgrCfgLst)

   cmLListInit(&cellCb->rgCfgInfo.crntRgrCfgLst);
   RETVOID;
}  /* rgSCHDbmInitCrntRgrCfgLst */

/**
 * @brief Handler for initializing the pndng rgr cfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitPndngRgrCfgLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDbmInitPndngRgrCfgLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitPndngRgrCfgLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmInitPndngRgrCfgLst)

   cmLListInit(&cellCb->rgCfgInfo.pndngRgrCfgLst);
   RETVOID;
}  /* rgSCHDbmInitPndngRgrCfgLst */

/**
 * @brief Handler for inserting the cfgElem in to the crntRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInsCrntRgrCfgElem
 *           
 *
 *  @param[in] *cellCb 
 *  @param[in] *cfgElem 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsCrntRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
PUBLIC Void rgSCHDbmInsCrntRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmInsCrntRgrCfgElem)

   cfgElem->cfgReqLstEnt.next = NULLP;
   cfgElem->cfgReqLstEnt.prev = NULLP;
   cmLListAdd2Tail(&cellCb->rgCfgInfo.crntRgrCfgLst, &cfgElem->cfgReqLstEnt);
   RETVOID;
}  /* rgSCHDbmInsCrntRgrCfgElem */

/**
 * @brief Handler for inserting the cfgElem in to the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInsPndngRgrCfgElem
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *cfgElem 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmInsPndngRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
PUBLIC Void rgSCHDbmInsPndngRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmInsPndngRgrCfgElem)

   cfgElem->cfgReqLstEnt.next = NULLP;
   cfgElem->cfgReqLstEnt.prev = NULLP;
   cfgElem->cfgReqLstEnt.node = (PTR)cfgElem;
   cmLListAdd2Tail(&cellCb->rgCfgInfo.pndngRgrCfgLst, &cfgElem->cfgReqLstEnt);
   RETVOID;
}  /* rgSCHDbmInsPndngRgrCfgElem */

/**
 * @brief Handler for accessing the existing cfgElem in the crntRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextCrntRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *cfgElem
 *  @return  RgSchCfgElem*
 **/
#ifdef ANSI
PUBLIC RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
PUBLIC RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb; 
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmGetNextCrntRgrCfgElem)

   if(!cfgElem)
   {
      RETVALUE( cellCb->rgCfgInfo.crntRgrCfgLst.first ? 
               (RgSchCfgElem *)(cellCb->rgCfgInfo.crntRgrCfgLst.first->node) : NULLP );
   }
   RETVALUE( cfgElem->cfgReqLstEnt.next ? 
               (RgSchCfgElem *)(cfgElem->cfgReqLstEnt.next->node) : NULLP );
}  /* rgSCHDbmGetNextCrntRgrCfgElem */

/**
 * @brief Handler for accessing the existing cfgElem in the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextPndngRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *cfgElem
 *  @return  RgSchCfgElem*
 **/
#ifdef ANSI
PUBLIC RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
PUBLIC RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb; 
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmGetNextPndngRgrCfgElem)

   if(!cfgElem)
   {
      RETVALUE( cellCb->rgCfgInfo.pndngRgrCfgLst.first ? 
               (RgSchCfgElem *)(cellCb->rgCfgInfo.pndngRgrCfgLst.first->node) : NULLP );
   }
   RETVALUE( cfgElem->cfgReqLstEnt.next ? 
               (RgSchCfgElem *)(cfgElem->cfgReqLstEnt.next->node) : NULLP );
}  /* rgSCHDbmGetNextPndngRgrCfgElem */

/**
 * @brief Handler for extracting the existing cfgElem from the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetPndngRgrCfgElemByKey
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  key 
 *  @return  RgSchCfgElem*
 **/
#ifdef ANSI
PUBLIC RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey
(
RgSchCellCb          *cellCb,
CmLteTimingInfo   key
)
#else
PUBLIC RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey(cellCb, key)
RgSchCellCb          *cellCb;
CmLteTimingInfo   key;
#endif
{
   CmLList  *tmpNode;

   TRC2(rgSCHDbmGetPndngRgrCfgElemByKey)

   CM_LLIST_FIRST_NODE(&cellCb->rgCfgInfo.pndngRgrCfgLst,tmpNode);
   while(tmpNode)
   {
      if((((RgSchCfgElem *)tmpNode->node)->actvTime.sfn == key.sfn) &&
            (((RgSchCfgElem *)tmpNode->node)->actvTime.subframe == key.subframe))
      {
         RETVALUE((RgSchCfgElem *)(tmpNode->node));
      }
      CM_LLIST_NEXT_NODE(&cellCb->rgCfgInfo.pndngRgrCfgLst,tmpNode);
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmGetPndngRgrCfgElemByKey */

/**
 * @brief Handler for deleting the existing cfgElem from the crntRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmDelCrntRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *cfgElem 
 *  @return  RgSchCfgElem*
 **/
#ifdef ANSI
PUBLIC RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
PUBLIC RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmDelCrntRgrCfgElem)

   if(cmLListDelFrm(&cellCb->rgCfgInfo.crntRgrCfgLst,&cfgElem->cfgReqLstEnt))
   {
      RETVALUE((RgSchCfgElem *)(cfgElem->cfgReqLstEnt.node));
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmDelCrntRgrCfgElem */

/**
 * @brief Handler for deleting the existing cfgElem from the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmDelPndngRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *cfgElem 
 *  @return  RgSchCfgElem*
 **/
#ifdef ANSI
PUBLIC RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
PUBLIC RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmDelPndngRgrCfgElem)

   if(cmLListDelFrm(&cellCb->rgCfgInfo.pndngRgrCfgLst,&cfgElem->cfgReqLstEnt))
   {
      RETVALUE((RgSchCfgElem *)(cfgElem->cfgReqLstEnt.node));
   }
   RETVALUE(NULLP);
}  /* rgSCHDbmDelPndngRgrCfgElem */

/**
 * @brief Handler for initializing the rntiDb.
 *
 * @details
 *
 *     Function : rgSCHDbmRntiDbInit
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  rntiStart
 *  @param[in]  maxRntis
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmRntiDbInit
(
RgSchCellCb       *cellCb,
U16            rntiStart,
U16            maxRntis
)
#else
PUBLIC S16 rgSCHDbmRntiDbInit(cellCb, rntiStart, maxRntis)
RgSchCellCb       *cellCb;
U16            rntiStart;
U16            maxRntis;
#endif
{
   U16 rnti;
   RgSchRntiLnk   *rntiPool;

   TRC2(rgSCHDbmRntiDbInit)

      /* Fix for Change Request ccpu00099150 */
   if(rgSCHUtlAllocSBuf(cellCb->instIdx, 
       (Data **)&cellCb->rntiDb.rntiPool,maxRntis*sizeof(RgSchRntiLnk)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   cellCb->rntiDb.rntiStart = rntiStart;
   cellCb->rntiDb.maxRntis = maxRntis;

   cellCb->rntiDb.count = maxRntis;

   rnti = rntiStart;
   rntiPool = cellCb->rntiDb.rntiPool;
   if (maxRntis == 1)
   {
      rntiPool[0].rnti = rnti;
      rntiPool[0].prv = NULLP;
      rntiPool[0].nxt = NULLP;
      cellCb->rntiDb.lastRnti = &rntiPool[0];
   }
   else
   {
      U16 idx;
      rntiPool[0].rnti = rnti++;
      rntiPool[0].prv = NULLP;
      rntiPool[0].nxt = &rntiPool[1];
      for (idx = 1; idx < maxRntis - 1; ++idx)
      {
         rntiPool[idx].rnti = rnti++;
         rntiPool[idx].prv = &rntiPool[idx-1];
         rntiPool[idx].nxt = &rntiPool[idx+1];
      }
      rntiPool[idx].rnti = rnti++;
      rntiPool[idx].prv = &rntiPool[idx-1];
      rntiPool[idx].nxt = NULLP;
      cellCb->rntiDb.lastRnti = &rntiPool[idx];
   }
   cellCb->rntiDb.freeRnti = &rntiPool[0];
   RETVALUE(ROK);
} /* rgSCHDbmRntiDbInit */

/**
 * @brief Handler for de-initializing the rntiDb.
 *
 * @details
 *
 *     Function : rgSCHDbmRntiDbDeInit
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmRntiDbDeInit
(
RgSchCellCb       *cellCb
)
#else
PUBLIC Void rgSCHDbmRntiDbDeInit(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmRntiDbDeInit)

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cellCb->instIdx, (Data **)(&(cellCb->rntiDb.rntiPool)),
                 cellCb->rntiDb.maxRntis*sizeof(RgSchRntiLnk));
   cellCb->rntiDb.maxRntis = 0;
   cellCb->rntiDb.freeRnti = NULLP;
   cellCb->rntiDb.lastRnti = NULLP;
   cmLListInit(&cellCb->rntiDb.rntiGuardPool);

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

} /* rgSCHDbmRntiDbDeInit */

/**
 * @brief Handler for accessing the free RNTI.
 *
 * @details
 *
 *     Function : rgSCHDbmGetRnti
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchRntiLnk*
 **/
#ifdef ANSI
PUBLIC RgSchRntiLnk* rgSCHDbmGetRnti
(
RgSchCellCb       *cellCb
)
#else
PUBLIC RgSchRntiLnk* rgSCHDbmGetRnti(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   RgSchRntiLnk *rntiLnk;

   TRC2(rgSCHDbmGetRnti)

   if (!(cellCb->rntiDb.freeRnti))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"RNTI exhausted count:%d",
                cellCb->rntiDb.count);
      RETVALUE(NULLP);
   }

   rntiLnk = cellCb->rntiDb.freeRnti;
   cellCb->rntiDb.freeRnti = rntiLnk->nxt;

   /* setting prv and nxt to NULLP may not be needed */
   rntiLnk->prv = NULLP;
   rntiLnk->nxt = NULLP;

   if (cellCb->rntiDb.freeRnti != NULLP)
   {
      cellCb->rntiDb.freeRnti->prv = NULLP;
   }
   else
   {
      cellCb->rntiDb.lastRnti = NULLP;
   }

   cellCb->rntiDb.count--;

   RETVALUE(rntiLnk);
} /* rgSCHDbmGetRnti */

/**
 * @brief Handler for releasing the RNTI.
 *
 * @details
 *
 *     Function : rgSCHDbmRlsRnti
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  rntiLnk
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDbmRlsRnti
(
RgSchCellCb       *cellCb,
RgSchRntiLnk      *rntiLnk
)
#else
PUBLIC Void rgSCHDbmRlsRnti(cellCb, rntiLnk)
RgSchCellCb       *cellCb;
RgSchRntiLnk      *rntiLnk;
#endif
{
   TRC2(rgSCHDbmRlsRnti)

   rntiLnk->nxt = NULLP;
   if (cellCb->rntiDb.lastRnti)
   {
      cellCb->rntiDb.lastRnti->nxt = rntiLnk;
      rntiLnk->prv = cellCb->rntiDb.lastRnti;
   }
   else
   {
      rntiLnk->prv = NULLP;
      cellCb->rntiDb.freeRnti = rntiLnk;
   }
   cellCb->rntiDb.lastRnti = rntiLnk;

   cellCb->rntiDb.count++;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

} /* rgSCHDbmRlsRnti */

#ifdef LTE_TDD
/**
 * @brief Handler for initializing the ueTfuPendLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInitUeTfuPendLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDbmInitUeTfuPendLst
(
RgSchCellCb    *cellCb,
U16            numBins
)
#else
PRIVATE S16 rgSCHDbmInitUeTfuPendLst(cellCb, numBins)
RgSchCellCb    *cellCb;
U16            numBins;
#endif
{
   RgSchUePucchRecpInfo pucchInfo;
   TRC2(rgSCHDbmInitUeTfuPendLst)

   /* Fix: syed It is better to compute offset dynamically
    * rather than hardcoding it as 0 */      
   if(cmHashListInit(&cellCb->ueTfuPendLst, numBins, (U16)((PTR)&(pucchInfo.hashLstEnt) - (PTR)&pucchInfo), FALSE, 
               CM_HASH_KEYTYPE_CONID,
               rgSchCb[cellCb->instIdx].rgSchInit.region, 
               rgSchCb[cellCb->instIdx].rgSchInit.pool) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}  /* rgSCHDbmInitUeTfuPendLst */

/**
 * @brief Handler for de-initializing the ueTfuPendLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDeInitUeTfuPendLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDbmDeInitUeTfuPendLst
(
RgSchCellCb       *cellCb
)
#else
PUBLIC S16 rgSCHDbmDeInitUeTfuPendLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmDeInitUeTfuPendLst)

   cmHashListDeinit(&cellCb->ueTfuPendLst);
   
   RETVALUE(ROK);
}  /* rgSCHDbmDeInitUeTfuPendLst */
#endif

/**********************************************************************
 
         End of file:     gk_sch_dbm.c@@/main/2 - Sat Jul 30 02:21:38 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    rr                    1. LTE MAC 2.1 release
           rg007.201   ap  1. Added support for MIMO
           rg008.201 rsharon  1. Added support for SPS
           rg009.201 avivek   1. Changed data type from U8 to U16 in
                                 rgSCHDbmRntiDbInit 
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
$SID$        ---         rt  1. LTE MAC 4.1 release
*********************************************************************91*/
