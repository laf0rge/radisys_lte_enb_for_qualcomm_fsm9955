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
               

     File:     nl_lmm.c

     Sid:      bk_pci_sel.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/4 - Mon Jan  5 16:16:12 2015

     Prg:      subhamay 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=250;
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
#include "rl_common.h"     /* RLOG defines */   
#include "rl_interface.h"   
#include "lnl.h"           /* NL Interface defines */
#include "nlu.h"
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

#include "lnl.x"           /* NL Interface includes */
#include "nlu.x"
#include "nl.x"            /* SON includes */

/* local defines */

/* local typedefs */
 
/* local externs */
/* forward references */
PRIVATE Void nlStoreNghCellInfo ARGS ((
NlCellCb             *cellCb,      /* serv cell control block*/
NlNghServCellCb      *nghCellCb,   /* neighbor cell information in SON*/
NlCellInfo           *tgtCellInfo, /* neighbor cell information from SM/eNB-APP*/
Bool                 isNewNgh,     /* whether this is new neighbor or not*/
NlAlarm              *alarm        /* alarm structure*/
));

PRIVATE Void nlStoreNghNghCellInfo ARGS ((
NlNghNghCellCb      *nghNghCellCb, /* neighbor's neighbor cell information in SON*/
NlCellInfo          *tgtCellInfo,  /* neighbor cell information from SM/eNB-APP*/
Bool                 isNewNgh,     /* whether this is new neighbor or not*/
NlAlarm             *alarm         /* alarm structure*/
));

PRIVATE S16 nlFindNghServingCell ARGS ((
CmLListCp         *servCellList,
NlCellInfo        *nghCellInfo,
NlNghServCellCb   **nghServCell
));

PRIVATE S16 nlFindNghNghCell  ARGS((
CmLListCp         *nghCellList,
NlCellInfo        *nghCellInfo,
NlNghNghCellCb    **nghNghCell
));

PRIVATE S16 nlNghNghPciInfoModify ARGS((
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
));

PRIVATE Void nlUpdateNghCellInfo   ARGS((
NlNghServCellCb     *nghCellCb,
NlCellInfo          *srcCellInfo,  /* neighbor cell configuration from SM*/
NlAlarm             *alarm
));

PRIVATE Void nlUpdateNghNghCellInfo   ARGS((
NlNghNghCellCb      *nghNghCellCb,
NlCellInfo          *srcCellInfo,  /* neighbor cell configuration from SM*/
NlAlarm             *alarm
));

PRIVATE Bool nlNghAddPciCollisionDetect ARGS ((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell config structure*/
NlTmpNghCellAdd   *nghCellAdd, /* neighbor cell configuration add structure*/
U32                msgType,    /* message type*/
NlAlarm           *alarm       /* alarm structure*/
));

PRIVATE S16 nlSelectRandomPciFromPciList  ARGS((
NlCellCb   *cellCb
));

PRIVATE S16 nlValidatePciWithNghCellModInfo ARGS ((
U16               newPci,      /* newly selected PCI*/
U16               dlEarfcn,    /* downlink carrier freq*/
NlCellPciType     modType,
NlTmpNghCellMod   *nghCellMod  /* neighbor cell configuration from SM/eNB-APP*/
));

PRIVATE S16 nlValidatePciWithNghCellAddInfo ARGS ((
U16               newPci,      /* newly selected PCI*/
U16               dlEarfcn,    /* downlink carrier freq*/
NlCellPciType     modType,     /* mod3, mod6, mod30 PCI*/
NlTmpNghCellAdd   *nghCellAdd  /* neighbor cell configuration from SM/eNB-APP*/
));

PRIVATE S16 nlSelectNewPciFromPciListByPrevSelPciIdx ARGS((
NlCellCb          *cellCb,     /* cell control block*/
U32               startIdx,    /* PCI start index*/
U32               endIdx,       /*PCI end index */
NlTmpNghCellCfg   *nghCellCfg,  /*neighbor cell configuration*/
NlAlarm           *alarm,       /*alarm information*/
NlCellPciType     modType      /*PCI mod type*/
));

PRIVATE S16 nlNghPciConfusionDetectWithAddNewNghCfg ARGS((
NlNghServCellCb   *nghServCb,  /* neighbor cell cb*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM*/
U32                nghEnbIdx,  /* neighbor enodeB count index in new neighbor cfg from SM*/
U32                nghCellIdx, /* neighbor cell count index in new neighbor cfg from SM*/
NlAlarm           *alarm       /* alarm structure*/
));

PRIVATE S16 nlNghNghPciConfusionDetectWithAddNewNghCfg ARGS((
NlNghNghCellCb    *nghNghCb,   /* neighbor's neighbor cell cb*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM*/
U32                nghEnbIdx,  /* neighbor enodeB count index in new neighbor cfg from SM*/
U32                nghCellIdx, /* neighbor cell count index in new neighbor cfg from SM*/
U32                nghNghCellIdx,/*neighbor's neighbor cell count index*/
NlAlarm           *alarm       /* alarm structure*/
));

PRIVATE S16 nlSelectServCellPciByDSon ARGS((
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
U32               msgType,
NlAlarm           *alarm
));

PRIVATE S16 nlNghNghPciInfoDelete ARGS ((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure */
));

PRIVATE S16 nlNghNghInfoUpdate ARGS ((
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
));

PRIVATE S16 nlNghNghInfoDelete ARGS ((
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
));

PRIVATE S16 nlNghNghInfoAdd ARGS ((
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
));

PRIVATE S16 nlValidatePciModRules ARGS((
U16             newPci,
U16             nghPci,
NlCellPciType   modType,
U16             selEarfcn,
U16             nghEarfcn
));

PRIVATE S16 NlValidateInitPciModRule   ARGS((
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
NlAlarm           *alarm
));

PRIVATE S16 nlValidateInitPciModRuleWithNgh   ARGS((
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
U32                pciCount,
NlCellPciType      modType
));

NlCellPciType   pciModType[NL_MAX_PCI_MOD_VAL] = {NL_SELECTED_PCI_MOD3,
                             NL_SELECTED_PCI_MOD6, NL_SELECTED_PCI_MOD30,  NL_SELECTED_DEFAULT_PCI};

/**
 * @brief validate the  PCI list configuration received from SM
 *
 * @details
 *
 *     Function : nlValidateCellPciCfg
 *
 *     This function will validate the cell PCI list configuration
 *     received from SM.
 *
 *  @param[in]  cellCfg   : cell configuration
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlValidateCellPciCfg
(
LnlCellCfg    *cellCfg
)
#else
PUBLIC S16 nlValidateCellPciCfg(cellCfg)
(
LnlCellCfg    *cellCfg;
)
#endif
{
   S16   ret = ROK;
   TRC3(nlValidateCellPciCfg);

   do
   {
      if(cellCfg->numPci == 0)
      {
         RLOG0(L_ERROR, "Empty PCI List received from SM");
         ret = RFAILED;
         break;
      }
   }while(0);
   
   RETVALUE(ret);
}

/**
 * @brief Validate selected PCI as per MOD3 MOD6 MOD30 rule with neighbor PCI
 *
 * @details
 *
 *  TODO
 *
 *  @param[in]  cellCb       : cell control block
 *  @param[in]  nghCellCfg   : neighbor cell configuration
 *  @param[in]  pciCount     : Index of Pci List
 *  @param[in]  modType      : Indicate whether Mod3 Mod6 or Mod30 PCI selection
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlValidateInitPciModRuleWithNgh
(
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
U32                pciCount,
NlCellPciType      modType
)
#else
PRIVATE S16 nlValidateInitPciModRuleWithNgh(cellCb, nghCellCfg, pciCount, modType)
(
NlCellCb        *cellCb;
NlTmpNghCellCfg *nghCellCfg;
U32              pciCount;
NlCellPciType    modType;
)
#endif
{
   U16   newPci;
   U32   numEnb;
   newPci = cellCb->pciList[pciCount];
   for(numEnb = 0; numEnb < nghCellCfg->numNghEnb; numEnb ++)
   {
      if(NL_NGH_ADD == nghCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType)
      {
         /*check whether selected PCI received as part of new neighbor information from OAM*/
         if(ROK != nlValidatePciWithNghCellAddInfo(newPci, cellCb->
                      cellInfo.dlEarfcn, modType,
                      &(nghCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellAdd)))
         {
            RETVALUE(RFAILED);
         }
      }
   }
   cellCb->cellInfo.pci = newPci;
   cellCb->selPciIdx = pciCount;
   RETVALUE(ROK);
}

/**
 * @brief Validate selected PCI as per MOD3 MOD6 MOD30 rule
 *
 * @details
 *
 *  TODO
 *
 *  @param[in]  cellCb       : cell control block
 *  @param[in]  nghCellCfg   : neighbor cell configuration
 *  @param[in]  alarm        : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 NlValidateInitPciModRule
(
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
NlAlarm           *alarm
)
#else
PRIVATE S16 NlValidateInitPciModRule(cellCb, nghCellCfg, alarm)
(
NlCellCb        *cellCb;
NlTmpNghCellCfg *nghCellCfg;
NlAlarm         *alarm;
)
#endif
{
   U32             startIdx;
   U32             endIdx;
   U32             count;
   U32             pciCount;

   for(count = 0; count < NL_MAX_PCI_MOD_VAL; count ++)
   {
      NlCellPciType   modType;
      modType = pciModType[count];
      /*first validate with already selected PCI*/
      startIdx = cellCb->selPciIdx;
      endIdx   = cellCb->numPci;
      for(pciCount = startIdx; pciCount < endIdx; pciCount ++)
      {
         if(ROK == nlValidateInitPciModRuleWithNgh(cellCb,
                   nghCellCfg, pciCount, modType))
         {
            RETVALUE(ROK);
         }
      }
      startIdx = 0;
      endIdx = cellCb->selPciIdx;
      for(pciCount = startIdx; pciCount < endIdx; pciCount ++)
      {
         if(ROK == nlValidateInitPciModRuleWithNgh(cellCb,
                   nghCellCfg, pciCount, modType))
         {
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);
}
/**
 * @brief select the PCI for serving cell
 *
 * @details
 *
 *     Function : nlSelectServCellPciByDSon
 *
 *     This function will select a PCI for serving cell which is
 *     collision free with respect to neighbor cell configurations.
 *     received from SM when distributed SON PCI selection is configured
 *
 *  @param[in]  cellCb       : cell control block
 *  @param[in]  nghCellCfg   : neighbor cell configuration
 *  @param[in]  msgType      : message type
 *  @param[in]  alarm        : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlSelectServCellPciByDSon
(
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
U32               msgType,
NlAlarm           *alarm
)
#else
PRIVATE S16 nlSelectServCellPciByDSon(cellCb, nghCellCfg, msgType, alarm)
(
NlCellCb        *cellCb;
NlTmpNghCellCfg *nghCellCfg;
U32              msgType;
NlAlarm         *alarm;
)
#endif
{
   U32    pciCount;
   Bool   pciFound = FALSE;
   S16    ret      = ROK;
   TRC3(nlSelectServCellPciByDSon);
   /*check whether the PCI received from OAM is valid or not*/
   /*If eNB restarted and SON already informed selected PCI to OAM previously 
     then a valid PCI will be received from OAM*/
   if(cellCb->cellInfo.pci <= NL_MAX_PCI_VAL)
   {
      /*check whether this PCI present in PCI list or not*/
      for(pciCount = 0; pciCount < cellCb->numPci; pciCount ++)
      {
         if(cellCb->pciList[pciCount] == cellCb->cellInfo.pci)
         {
            cellCb->selPciIdx = pciCount;
            pciFound = TRUE;
            break;
         }
      }
      if(TRUE != pciFound)
      {
         nlSelectRandomPciFromPciList(cellCb);
      }
   }
   else
   {
      nlSelectRandomPciFromPciList(cellCb);
   }
   /*check whether the serv cell PCI creating any PCI collision with 
     neighbor cell information received from SM*/
   if(nghCellCfg->numNghEnb > 0)
   {

      /*still now no neighbor information is stored in SON module*/
      /*check whether received neighbor information creates any problem for selected PCI*/
      if(ROK != NlValidateInitPciModRule(cellCb, nghCellCfg, alarm))
      {
         RLOG0(L_ERROR,
                 "Failed to select PCI for INIT state due to Mod problem");
         ret = RFAILED;
      }
      if(TRUE == nlNghPciCollisionDetect(cellCb, nghCellCfg, msgType, alarm))
      {
         RLOG0(L_ERROR,
                 "PCI collision detected for serving cell PCI selection");
         ret = RFAILED;
      }
      if(ROK != nlNghPciInfoAdd(cellCb, nghCellCfg, alarm))
      {
         RLOG0(L_ERROR,
               "Failed to insertneighbor config into SON NRT table");
         ret = RFAILED;
      }
   }
   RETVALUE(ret);
}

/**
 * @brief configure the PCI list received from SM
 *
 * @details
 *
 *     Function : nlCellSelectPci
 *
 *     This function will select a PCI for serving cell which is 
 *     collision free with respect to neighbor cell configurations.
 *     received from SM.
 *
 *  @param[in]  cellCb       : cell control block
 *  @param[in]  nghCellCfg   : neighbor cell configuration
 *  @param[in]  alarm        : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlCellSelectPci
(
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
U32               msgType,
NlAlarm           *alarm
)
#else
PUBLIC S16 nlCellSelectPci(cellCb, nghCellCfg, msgType, alarm)
(
NlCellCb        *cellCb;
NlTmpNghCellCfg *nghCellCfg;
U32              msgType;
NlAlarm         *alarm;
)
#endif
{
   TRC3(nlCellSelectPci);
   switch(nlCb.genCfg.pciSelType)
   {
      case NL_PCI_SEL_DISABLE :
      {
         cellCb->cellInfo.pci = cellCb->pciList[0];
         RLOG1(L_INFO, "SON selected Serv cell PCI : %d", 
                cellCb->cellInfo.pci);
         cellCb->isSmSonCfgDone = TRUE;
         /*send PCI update to SM*/
         nlSendPciUpdateInd(cellCb);
         RETVALUE(ROK);
      }
      break;
      case NL_PCI_SEL_DSON :
      {
         if(ROK == nlSelectServCellPciByDSon(cellCb, nghCellCfg, msgType, alarm))
         {
            RLOG1(L_INFO, "SON selected Serv cell PCI : %d", 
                    cellCb->cellInfo.pci);
            cellCb->isSmSonCfgDone = TRUE;
            /*send PCI update to SM*/
            nlSendPciUpdateInd(cellCb);
            RETVALUE(ROK);
         }
      }
      break;
      case NL_PCI_SEL_CSON :
      {


      }
   }
   RETVALUE(RFAILED);
}
/**
 * @brief configure the PCI list received from SM
 *
 * @details
 *
 *     Function : nlCellStorePciCfg
 *
 *     This function will configure the PCI list in cell control block
 *     received from SM.
 *
 *  @param[in]  cellCb    : cell control block
 *  @param[in]  cellCfg   : cell configuration
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlCellStorePciCfg
(
NlCellCb     *cellCb,
LnlCellCfg   *cellCfg
)
#else
PUBLIC S16 nlCellStorePciCfg(cellCb, cellCfg)
(
NlCellCb     *cellCb;
LnlCellCfg   *cellCfg;
)
#endif
{
   U16        count;
   U16        numPci = 0;
   U16        maxCsgPci;
   TRC3(nlCellStorePciCfg);
   cellCb->cellInfo.pci  = cellCfg->pci;
   cellCb->cellInfo.isCsgCell = cellCfg->csgPciCfg.isCsgPciCfg;
   /* we always consider PCI selection only for HOME femto*/
   /* we are considering only open and close access mode
   if((TRUE == cellCfg->csgPciCfg.isCsgPciCfg) &&
      (LNL_ENB_HYBRID_ACCESS == cellCfg->csgPciCfg.accessMode))
   {
      RLOG0(L_ERROR, "HYBRID CSG access mode is not supported in SON");
      RETVALUE(RFAILED);
   }*/

   cellCb->cellInfo.isCsgPciCfg = cellCfg->csgPciCfg.isCsgPciCfg;
   cellCb->cellInfo.accessMode = cellCfg->csgPciCfg.accessMode;
   cellCb->cellInfo.csgStartPci = cellCfg->csgPciCfg.csgStartPci;
   cellCb->cellInfo.csgPciRange = cellCfg->csgPciCfg.csgPciRange;
   cellCb->cellInfo.isSmallCellPciCfg = cellCfg->smallCellPciCfg.isSmallCellPciCfg;
   cellCb->cellInfo.smallCellStartPci = cellCfg->smallCellPciCfg.smallCellStartPci;   
   cellCb->cellInfo.maxSmallCellPci = cellCfg->smallCellPciCfg.maxSmallCellPci;   
   maxCsgPci = cellCb->cellInfo.csgStartPci + cellCb->cellInfo.csgPciRange;
   if(NL_PCI_SEL_DSON == nlCb.genCfg.pciSelType) 
   {
      if((TRUE == cellCfg->csgPciCfg.isCsgPciCfg) && 
         (LNL_ENB_CLOSED_ACCESS == cellCfg->csgPciCfg.accessMode))
      {
         if((LNL_MAX_PCI_CFG < cellCfg->csgPciCfg.csgPciRange) || 
              (LNL_MAX_PCI_CFG < cellCfg->numPci))
         {
            RLOG0(L_ERROR, "Invalid PCI range received for CSG PCI");
            RETVALUE(RFAILED);
         }
         if(NL_MAX_PCI_VAL < cellCfg->csgPciCfg.csgStartPci)
         {
            RLOG1(L_ERROR, "Invalid start CSG PCI value[%d] received", 
                  cellCfg->csgPciCfg.csgStartPci);
            RETVALUE(RFAILED);
         }
         for(count = 0; count < cellCfg->numPci; count ++)
         {
            if((cellCfg->pciList[count] >= cellCfg->csgPciCfg.csgStartPci) && 
             (( cellCfg->pciList[count] < maxCsgPci)))
            {
               cellCb->pciList[numPci] = cellCfg->pciList[count];
               numPci ++;
            }
         }
         if(numPci < 1)
         {
            RLOG0(L_ERROR, "No Valid PCI selected in  CSG PCI Range config");
            RETVALUE(RFAILED);
         }
         cellCb->numPci = numPci;
      }
      else if(TRUE == cellCfg->smallCellPciCfg.isSmallCellPciCfg)
      {
         for(count = 0; count < cellCfg->numPci; count ++)
         {
            if((cellCfg->pciList[count] >= cellCfg->smallCellPciCfg.smallCellStartPci) && 
             (cellCfg->pciList[count] <= cellCfg->smallCellPciCfg.maxSmallCellPci))
            {
               if(TRUE == cellCfg->csgPciCfg.isCsgPciCfg)
               {
                  if((cellCfg->pciList[count] < cellCfg->csgPciCfg.csgStartPci) || 
                          (cellCfg->pciList[count] >= maxCsgPci))
                  {
                     cellCb->pciList[numPci] = cellCfg->pciList[count];
                     numPci ++;
                  }
               }
               else
               {
                  cellCb->pciList[numPci] = cellCfg->pciList[count];
                  numPci ++;
               }
            }
         }
         if(numPci < 1)
         {
            RLOG0(L_ERROR, "No Valid PCI selected in  Small Cell PCI Range config");
            RETVALUE(RFAILED);
         }
         cellCb->numPci = numPci;
      }
      /*store the PCI list in SON cell control block*/
      else  
      {
         if(TRUE == cellCfg->csgPciCfg.isCsgPciCfg)
         {
            for(count = 0; count < cellCfg->numPci; count ++)
            {
               if((cellCfg->pciList[count] < cellCfg->csgPciCfg.csgStartPci) ||
                          (cellCfg->pciList[count] >= maxCsgPci))
               {
                  cellCb->pciList[numPci] = cellCfg->pciList[count];
                  numPci ++;
               }
            }
            cellCb->numPci = numPci;
         }
         else
         {
            cellCb->numPci = cellCfg->numPci;
            cmMemcpy((U8 *)&cellCb->pciList[0], (U8 *)&cellCfg->pciList[0],
               sizeof(cellCfg->pciList[0]) * cellCfg->numPci);
         }
         if(cellCb->numPci < 1)
         {
            RLOG0(L_ERROR, "No Valid PCI selected in  Small Cell PCI Range config");
            RETVALUE(RFAILED);
         }
      }
   }
   else
   {
      cellCb->numPci = cellCfg->numPci;
      cmMemcpy((U8 *)&cellCb->pciList[0], (U8 *)&cellCfg->pciList[0],
            sizeof(cellCfg->pciList[0]) * cellCfg->numPci);
      if(cellCb->numPci < 1)
      {
         RLOG0(L_ERROR, "PCI list is empty");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief select a PCI from PCI list and use ecgi as key for selection
 *
 * @details
 *
 *     Function : nlSelectRandomPciFromPciList
 *
 *     This function will generate a random key from ECGI and select a PCI
 *     from PCI list
 *
 *  @param[in]  cellCb    : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlSelectRandomPciFromPciList
(
NlCellCb   *cellCb
)
#else
PRIVATE S16 nlSelectRandomPciFromPciList(cellCb)
(
NlCellCb   *cellCb;
)
#endif
{
   U8     key[NL_MAX_PCI_KEY_LEN] = {0};
   U32    count;
   U32    sum;
   U16    idx;
   TRC3(nlSelectRandomPciFromPciList);
   cmMemcpy(&key[0], (U8 *)&(cellCb->cellInfo.ecgi.plmn.mcc[0]), 3);
   cmMemcpy(&key[3], (U8 *)&(cellCb->cellInfo.ecgi.plmn.mnc[0]),
                         cellCb->cellInfo.ecgi.plmn.numMncDigits);
   cmMemcpy(&key[6], (U8 *)&(cellCb->cellInfo.ecgi.cellId), 4);
   sum = 0;
   for (count = 0; count < NL_MAX_PCI_KEY_LEN; count++)
   {
      /*this wouldn't help much to generate random number. will implement something better TODO*/
      sum = (CM_STR_HASHFUNC_CONSTANT * sum) + key[count];
   }
   idx = (U16) (sum % cellCb->numPci);
   cellCb->selPciIdx = idx;
   cellCb->cellInfo.pci = cellCb->pciList[idx];
   RETVALUE(ROK);
}

/**
 * @brief To update the selected PCI as the first element of the PCI List
 *
 * @details
 *
 *     Function : nlUpdatePciListWithSelPci
 *
 *     This function will update the PCI list. It will make the first element 
 *     of PCI list as same as selected PCI.
 *
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlUpdatePciListWithSelPci
(
NlCellCb       *cellCb
)
#else
PUBLIC S16 nlUpdatePciListWithSelPci(cellCb)
(
NlCellCb       *cellCb;
)
#endif
{
   //U32   pciCount;
   //U32   idx;
   TRC3(nlUpdatePciListWithSelPci);
RETVALUE(ROK);
}
/**
 * @brief To update the PCI as well as PCI list for the eNB
 *
 * @details
 *
 *     Function : nlStoreCellReCfgPciInfo
 *
 *     This function will update the PCI as well as PCI list. This function
 *     also clear alarm if raised earlier for the old pci.
 *
 *  @param[in]  cellCb    : cell control block
 *  @param[in]  cellReCfg : cell re config information
 *  @param[in]  selPciIdx : pci selected from PCI list
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlStoreCellReCfgPciInfo
(
NlCellCb       *cellCb,
LnlCellReCfg   *cellReCfg,
U32            selPciIdx
)
#else
PUBLIC S16 nlStoreCellReCfgPciInfo(cellCb, cellreCfg, selPciIdx)
(
NlCellCb       *cellCb;
LnlCellReCfg   *cellReCfg;
U32            selPciIdx;
)
#endif
{
   NlAlarm  alarmInfo;
   NlAlarm  *alarm;
   U32      alarmCount;
   TRC3(nlStoreCellReCfgPciInfo);
   cmMemset((U8 *)&alarmInfo, 0, sizeof(NlAlarm));
   alarm = &alarmInfo;

   /*clear any ALARM if raised earlier for this PCI*/
   NL_FILL_CLEAR_ALARM(cellCb->cellInfo, alarm);
   nlServCellPeerAlarmClear(cellCb);
   cmMemset((U8 *)&cellCb->pciList[0], 0, sizeof(cellCb->pciList[0]) * cellCb->numPci);
   cmMemcpy((U8 *)&cellCb->pciList[0], (U8 *)&(cellReCfg->pciList[0]),
         sizeof(cellReCfg->pciList[0]) * cellReCfg->numPci);
   cellCb->numPci = cellReCfg->numPci;
   RLOG2(L_INFO, "PCI modification Success: Old PCI val %d New PCI val %d",
         cellCb->cellInfo.pci, cellCb->pciList[selPciIdx]);
   cellCb->cellInfo.pci = cellCb->pciList[selPciIdx];
   cellCb->selPciIdx    = selPciIdx;
   nlUpdatePciListWithSelPci(cellCb);
   /*clear the alarm if it raised earlier*/
   if(TRUE == alarm->isAlarmCleared)
   {
      for(alarmCount = 0; alarmCount < alarm->numAlarmCleared; alarmCount ++)
      {
         nlSendLnlAlarm(&alarm->alarmCleared[alarmCount]);
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief check whether any neighbor cell information present with
 *  provided pci, ecgi and frequencies
 *
 * @details
 *
 *     Function : nlValidatePciEcgiWithNghCellInfo
 *
 *     This function will validate whether any neighbor cell information
 *     exist with provided pci,ecgi and dl/ul carrier frequencies
 *
 *  @param[in]  cellCb   : cell control block
 *  @param[in]  pci      : physical cell Id
 *  @param[in]  ecgi     : cell id and plmn id
 *  @param[in]  dlEarfcn : downlink carrier frequency
 *  @param[in]  alarm    : alarm Information
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlValidatePciEcgiWithNghCellInfo
(
NlCellCb     *cellCb,  /* cell control block*/
U16          pci,      /* physical cell Id*/
LnlEcgi      *ecgi,    /* cell id and plmn id*/
U16          dlEarfcn, /* downlink carrier frequency*/
NlAlarm      *alarm    /* alarm information*/
)
#else
PUBLIC S16 nlValidatePciEcgiWithNghCellInfo(cellCb, pci, 
           ecgi, dlEarfcn, alarm)
(
NlCellCb     *cellCb;  /* cell control block*/
LnlEcgi      *ecgi;    /* cell id and plmn id*/
U16          pci;      /* physical cell Id*/
U16          dlEarfcn; /* downlink carrier frequency*/
NlAlarm      *alarm;    /* alarm information*/
)
#endif
{
   PTR               prevEntry = NULLP;
   NlNeighCellCb     *nghCellCb = NULLP;
   CmLListCp         *nghCellList = NULLP;
   CmLListCp         *servCellList = NULLP;
   CmLList           *nghLink  = NULLP;
   CmLList           *servLink = NULLP;
   NlNghServCellCb   *nghServCellCb = NULLP;
   NlNghNghCellCb    *nghNghCellCb = NULLP;

   TRC3(nlValidatePciEcgiWithNghCellInfo);

   while(ROK == cmHashListGetNext(&(cellCb->neighEnbCb.nghEnbListCp), 
         prevEntry, (PTR *) &nghCellCb))
   {
      /*check whether direct neighber PCI information matching or not*/
      servCellList = &(nghCellCb->servCellLst);
      CM_LLIST_FIRST_NODE(servCellList, servLink);
      while(NULLP != servLink)
      {
         nghServCellCb = (NlNghServCellCb *)servLink->node;
         if((pci == nghServCellCb->servCellInfo.pci) &&
         (dlEarfcn == nghServCellCb->servCellInfo.dlEarfcn))
         {
            if(FALSE != cmMemcmp((U8 *)ecgi, (U8 *) &nghServCellCb->servCellInfo.ecgi,
                sizeof(LnlEcgi)))
            {
               NL_FILL_NGH_CELL_PCI_CONFUSION_ALARM(pci, ecgi->cellId, nghServCellCb, alarm);
               RLOG3(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d CGI %lu ",
                   nghServCellCb->servCellInfo.pci, nghServCellCb->servCellInfo.dlEarfcn,
                   nghServCellCb->servCellInfo.ecgi.cellId);
               RLOG3(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     pci, dlEarfcn, ecgi->cellId); 
               RETVALUE(RFAILED);
            }
         }
         /*check whether any neighbor's neighbor PCI information matching or not*/
         nghCellList = &(nghServCellCb->nghCellLst);
         CM_LLIST_FIRST_NODE(nghCellList, nghLink);
         while(NULLP != nghLink)
         {
            nghNghCellCb = (NlNghNghCellCb *)nghLink->node;
            if((pci == nghNghCellCb->nghNghCellInfo.pci) &&
              (dlEarfcn == nghNghCellCb->nghNghCellInfo.dlEarfcn))
            {
               if((ecgi->cellId > 0) && (nghNghCellCb->
                                   nghNghCellInfo.ecgi.cellId > 0))
               {
                  if(FALSE != cmMemcmp((U8 *)ecgi, 
                              (U8 *)&nghNghCellCb->nghNghCellInfo.ecgi,
                     sizeof(LnlEcgi)))
                  {
                     NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(pci, 
                                     ecgi->cellId, nghNghCellCb, alarm);
                     RLOG3(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d CGI %lu ",
                     nghNghCellCb->nghNghCellInfo.pci, nghNghCellCb->nghNghCellInfo.dlEarfcn,
                     nghNghCellCb->nghNghCellInfo.ecgi.cellId);
                     RLOG3(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     pci, dlEarfcn, ecgi->cellId);
                     RETVALUE(RFAILED);
                  }
               }
               else
               {
                  NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(pci, 
                                      ecgi->cellId, nghNghCellCb, alarm);
                  RLOG2(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d ",
                     nghNghCellCb->nghNghCellInfo.pci, nghNghCellCb->nghNghCellInfo.dlEarfcn);
                  RLOG2(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     pci, dlEarfcn);
                  RETVALUE(RFAILED);
               }               
            }
            nghLink = NULLP;
            CM_LLIST_NEXT_NODE(nghCellList, nghLink);
         }
         servLink = NULLP;
         CM_LLIST_NEXT_NODE(servCellList, servLink);
      }
      prevEntry = (PTR)nghCellCb;
   }
   RETVALUE(ROK);
}

/**
 * @brief check whether any neighbor cell information present with
 *  provided pci and frequencies
 *
 * @details
 *
 *     Function : nlValidatePciWithNghCellInfo
 *
 *     This function will validate whether any neighbor cell information
 *     exist with provided pci and dl/ul carrier frequencies
 *
 *  @param[in]  cellCb   : cell control block
 *  @param[in]  pci      : physical cell Id
 *  @param[in]  dlEarfcn : downlink carrier frequency
 *  @param[in]  modType  : whether Mod3 , Mod6, Mod30 PCI
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlValidatePciWithNghCellInfo
(
NlCellCb        *cellCb,
U16             pci,      /* physical cell Id*/
U16             dlEarfcn, /* downlink carrier frequency*/
NlCellPciType   modType
)
#else
PUBLIC S16 nlValidatePciWithNghCellInfo(cellCb, pci, dlEarfcn, modType)
(
NlCellCb        *cellCb;
U16             pci;      /* physical cell Id*/
U16             dlEarfcn; /* downlink carrier frequency*/
NlCellPciType   modType;
)
#endif
{
   PTR               prevEntry = NULLP;
   NlNeighCellCb     *nghCellCb = NULLP;
   CmLListCp         *nghCellList = NULLP;
   CmLListCp         *servCellList = NULLP;
   CmLList           *nghLink  = NULLP;
   CmLList           *servLink = NULLP;
   NlNghServCellCb   *nghServCellCb = NULLP;
   NlNghNghCellCb    *nghNghCellCb = NULLP;
   TRC3(nlValidatePciWithNghCellInfo);
   while(ROK == cmHashListGetNext(&(cellCb->neighEnbCb.nghEnbListCp), prevEntry, (PTR *) &nghCellCb))
   {
      /*check whether direct neighber PCI information matching or not*/
      servCellList = &(nghCellCb->servCellLst);
      CM_LLIST_FIRST_NODE(servCellList, servLink);
      while(NULLP != servLink)
      {
         nghServCellCb = (NlNghServCellCb *)servLink->node; 
         if((pci == nghServCellCb->servCellInfo.pci) && 
         (dlEarfcn == nghServCellCb->servCellInfo.dlEarfcn))
         {
            RETVALUE(RFAILED);
         }
         if(ROK != nlValidatePciModRules(pci, nghServCellCb->servCellInfo.pci, modType, 
                        dlEarfcn, nghServCellCb->servCellInfo.dlEarfcn))
         {
            RETVALUE(RFAILED);
         }
         /*check whether any neighbor's neighbor PCI information matching or not*/
         nghCellList = &(nghServCellCb->nghCellLst);
         CM_LLIST_FIRST_NODE(nghCellList, nghLink);
         while(NULLP != nghLink)
         {
            nghNghCellCb = (NlNghNghCellCb *)nghLink->node; 
            if((pci == nghNghCellCb->nghNghCellInfo.pci) &&
              (dlEarfcn == nghNghCellCb->nghNghCellInfo.dlEarfcn))
            {
               RETVALUE(RFAILED);
            }
            nghLink = NULLP;
            CM_LLIST_NEXT_NODE(nghCellList, nghLink);
         }
         servLink = NULLP;
         CM_LLIST_NEXT_NODE(servCellList, servLink);
      } 
      prevEntry = (PTR)nghCellCb;
   }
   RETVALUE(ROK);
}

/**
 * @brief check whether PCI matching with neighbor cell modify info
 *
 * @details
 *
 *     Function : nlValidatePciWithNghCellModInfo
 *
 *     This function will validate whether the newly selected PCI 
 *     matching with neighbor cell modification information received from SM
 *
 *  @param[in]  newPci     : newly selected PCI
 *  @param[in]  dlEarfcn   : downlink carrier frequency
 *  @param[in]  nghCellMod : neighbor cell modify info
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlValidatePciWithNghCellModInfo
(
U16               newPci,      /* newly selected PCI*/
U16               dlEarfcn,    /* downlink carrier freq*/
NlCellPciType     modType,
NlTmpNghCellMod   *nghCellMod  /* neighbor cell configuration from SM/eNB-APP*/
)
#else
PRIVATE S16 nlValidatePciWithNghCellModInfo(newPci, dlEarfcn, modType, nghCellMod)
(
U16               newPci;      /* newly selected PCI*/
U16               dlEarfcn;    /* downlink carrier freq*/
NlCellPciType     modType;
NlTmpNghCellMod   *nghCellMod; /* neighbor cell configuration from SM/eNB-APP*/
)
#endif
{
   U32                 nghCount;
   U32                 nghNghCount;
   NlTmpNghCellModInfo *cellModInfo = NULLP;
   for(nghCount = 0; nghCount < nghCellMod->numNgh; nghCount ++)
   {
      cellModInfo = &(nghCellMod->nghCellModInfo[nghCount]);
      if((newPci == cellModInfo->nghCellInfo.pci) &&
        (dlEarfcn == cellModInfo->nghCellInfo.dlEarfcn))
      {
         RETVALUE(RFAILED);
      }
      if(ROK != nlValidatePciModRules(newPci, cellModInfo->nghCellInfo.pci,
                     modType, dlEarfcn, cellModInfo->nghCellInfo.dlEarfcn))
      {
         RETVALUE(RFAILED);
      } 
      /*check for neighbor's neighbor*/
      for(nghNghCount = 0; nghNghCount < cellModInfo->numNghNgh; nghNghCount ++)
      {
         if((newPci == cellModInfo->nghNghCellInfo[nghNghCount].pci) &&
           (dlEarfcn == cellModInfo->nghNghCellInfo[nghNghCount].dlEarfcn))
         {
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief check whether PCI matching with neighbor cell add info
 *
 * @details
 *
 *     Function : nlValidatePciWithNghCellAddInfo
 *
 *     This function will validate whether the newly selected PCI
 *     matching with neighbor cell add information received from SM
 *
 *  @param[in]  newPci     : newly selected PCI
 *  @param[in]  dlEarfcn   : downlink carrier frequency
 *  @param[in]  nghCellAdd : neighbor cell add info
 *  @param[in]  modType    : whether Mod3, Mod6, Mod30 PCI
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlValidatePciWithNghCellAddInfo
(
U16               newPci,      /* newly selected PCI*/
U16               dlEarfcn,    /* downlink carrier freq*/
NlCellPciType     modType,     /* Mod3, Mod6, Mod30 PCI*/
NlTmpNghCellAdd   *nghCellAdd  /* neighbor cell configuration from SM/eNB-APP*/
)
#else
PRIVATE S16 nlValidatePciWithNghCellAddInfo(newPci, dlEarfcn, modType,nghCellAdd)
(
U16               newPci;      /* newly selected PCI*/
U16               dlEarfcn;    /* downlink carrier freq*/
NlCellPciType     modType;     /* Mod3, Mod6, Mod30 PCI*/
NlTmpNghCellAdd   *nghCellAdd; /* neighbor cell configuration from SM/eNB-APP*/
)
#endif
{
   U32                 nghCount;
   U32                 nghNghCount;
   NlTmpNghCellAddInfo *cellAddInfo = NULLP;
   for(nghCount = 0; nghCount < nghCellAdd->numNgh; nghCount ++)
   {
      cellAddInfo = &(nghCellAdd->nghCellAddInfo[nghCount]);
      if((newPci == cellAddInfo->nghCellInfo.pci) &&
        (dlEarfcn == cellAddInfo->nghCellInfo.dlEarfcn))
      {
         RETVALUE(RFAILED);
      }
      if(ROK != nlValidatePciModRules(newPci, cellAddInfo->nghCellInfo.pci, modType, 
                  dlEarfcn, cellAddInfo->nghCellInfo.dlEarfcn))
      {
         RETVALUE(RFAILED);
      }      
      /*check for neighbor's neighbor*/
      for(nghNghCount = 0; nghNghCount < cellAddInfo->numNghNgh; nghNghCount ++)
      {
         if((newPci == cellAddInfo->nghNghCellInfo[nghNghCount].pci) &&
           (dlEarfcn == cellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn))
         {
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief validation of newly selected PCI 
 *
 * @details
 *
 *     This function will validate the newly selected PCI %3, %6 and %30
 *     value with neighbor PCI. If newly selected PCI %3, %6, %30 values are
 *     same as neighbor PCI then another new PCI should be selected
 *
 *  @param[in]  newPci     : newly selected PCI
 *  @param[in]  nghPci     : neighbor PCI
 *  @param[in]  modType    : Indicate whether %3 %6 or %30
 *  @param[in]  selEarfcn  : serving cell EARFCN
 *  @param[in]  nghEarfcn  : neighbor Earfcn
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlValidatePciModRules
(
U16             newPci,     
U16             nghPci,
NlCellPciType   modType,
U16             selEarfcn,
U16             nghEarfcn
)
#else
PRIVATE S16 nlValidatePciModRules(newPci, nghPci, modType, selEarfcn, nghEarfcn)
(
U16             newPci;      
U16             nghPci;
NlCellPciType   modType;
U16             selEarfcn;
U16             nghEarfcn;
)
#endif
{
   /*validate PCI MOD rule only for MOD3, MOD6 and MOD30*/
   if(modType != NL_SELECTED_DEFAULT_PCI)
   {
      /*do mod value validation only if frequency is same*/
      if(selEarfcn == nghEarfcn)
      {
         if(newPci % modType == nghPci % modType)
         {
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief select a PCI from pci list which is collision free
 *
 * @details
 *
 *     Function : nlSelectNewPciFromPciListByPrevSelPciIdx
 *
 *     This function will select a PCI from PCI list by considering 
 *     the previously seletced PCI Index and should be collision free
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  startIdx   : start index for PCI list
 *  @param[in]  endIdx     : end Index for PCI list
 *  @param[in]  nghCellCfg : neighbor cell info
 *  @param[in]  alarm      : alarm info
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlSelectNewPciFromPciListByPrevSelPciIdx
(
NlCellCb          *cellCb,     /* cell control block*/
U32               startIdx,    /* */
U32               endIdx,       /* */
NlTmpNghCellCfg   *nghCellCfg,
NlAlarm           *alarm,
NlCellPciType     modType
)
#else
PRIVATE S16 nlSelectNewPciFromPciListByPrevSelPciIdx(cellCb, startIdx, endIdx, 
                       nghCellCfg, alarm, modType)
(
NlCellCb          *cellCb;     /* cell control block*/
U32               startIdx; /* */
U32               endIdx;      /* */
NlTmpNghCellCfg   *nghCellCfg;
NlAlarm           *alarm;
NlCellPciType     modType;
)
#endif
{
   U16        newPci = 0;
   U32        pciCount;
   U8         numEnb;
   Bool       validPci;
   TRC3(nlSelectNewPciFromPciListByPrevSelPciIdx);

   for(pciCount = startIdx; pciCount < endIdx; pciCount ++)
   {
      newPci = cellCb->pciList[pciCount];
      if((newPci <= NL_MAX_PCI_VAL) &&
          (newPci != cellCb->cellInfo.pci))
      {
         validPci = TRUE;
         /*check whether the selected PCI already present in SON neighbor information*/
         if(ROK == nlValidatePciWithNghCellInfo(cellCb, newPci,
                   cellCb->cellInfo.dlEarfcn, modType))
         {
            for(numEnb = 0; numEnb < nghCellCfg->numNghEnb; numEnb ++)
            {
               if(NL_NGH_ADD == nghCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType)
               {
                  /*check whether selected PCI received as part of new neighbor information from OAM*/
                  if(ROK != nlValidatePciWithNghCellAddInfo(newPci, cellCb->
                      cellInfo.dlEarfcn, modType,
                      &(nghCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellAdd)))
                  {
                     validPci = FALSE;
                     break;
                  }
               }
               else if(NL_NGH_MOD == nghCellCfg->nghEnbCellCfg[numEnb].nghCellParam.cfgType)
               {
                  /*check whether selected PCI received as part of new neighbor modification information from OAM*/
                  if(ROK != nlValidatePciWithNghCellModInfo(newPci, cellCb->
                      cellInfo.dlEarfcn, modType,
                      &(nghCellCfg->nghEnbCellCfg[numEnb].nghCellParam.u.nghCellMod)))
                  {
                     validPci = FALSE;
                     break;
                  }
               }
            }
            if(TRUE == validPci)
            {
               /*clear any ALARM if raised earlier for this PCI*/
               NL_FILL_CLEAR_ALARM(cellCb->cellInfo, alarm);
               nlServCellPeerAlarmClear(cellCb);
               /*update the alarm about PCI modification*/
               NL_FILL_PCI_UPDATE_ALARM(cellCb, alarm);
               /*update the cell control block with new PCI*/
               cellCb->cellInfo.pci = newPci;
               cellCb->selPciIdx = pciCount;
               /* update cell state as configured*/
               NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_CONFIGURED);
               /*update PCI list with first element as selected PCI of this Cell*/
               nlUpdatePciListWithSelPci(cellCb);
               RETVALUE(ROK);
            }
         }
      }
   }
   RETVALUE(RFAILED);
}
  
/**
 * @brief resolve the PCI collison detected through
 *  neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghPciCollisionResolve
 *
 *     This function will resolve the PCI collision
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration from SM/eNB-APP
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlNghPciCollisionResolve
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC S16 nlNghPciCollisionResolve(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32             count;
   U32   startIdx;
   U32   endIdx;
   TRC3(nlNghPciCollisionResolve);
   for(count = 0; count < NL_MAX_PCI_MOD_VAL; count ++)
   {
      NlCellPciType   modType;
      modType = pciModType[count];
      startIdx = cellCb->selPciIdx + 1;
      endIdx   = cellCb->numPci;
      /*check the new PCI selection based on mod*/
      if(ROK != nlSelectNewPciFromPciListByPrevSelPciIdx(cellCb,
                               startIdx, endIdx, nghCellCfg, alarm, modType))
      {
         startIdx = 0;
         endIdx   = cellCb->selPciIdx;
         if(ROK != nlSelectNewPciFromPciListByPrevSelPciIdx(cellCb,
                              startIdx, endIdx, nghCellCfg, alarm, modType))
         {
            continue;
         }
         else
         {
            RETVALUE(ROK);
         }
      }
      else
      {
         RETVALUE(ROK);
      }
   }
   RETVALUE (RFAILED);
}

/**
 * @brief PCI collison detection from neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghModPciCollisionDetect
 *
 *     This function will validate whether pci received from SM/eNB-APP
 *     is matching with serving cell
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellMod : neighbor cell configuration structure
 *  @param[in]  msgType    : message type
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : FALSE
 *     -# Failure       : TRUE
 *
 **/
#ifdef ANSI
PRIVATE Bool nlNghModPciCollisionDetect
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell config structure*/
NlTmpNghCellMod   *nghCellMod, /* neighbor cell mod configuration structure*/
U32                msgType,    /* message type*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PRIVATE Bool nlNghModPciCollisionDetect(cellCb, nghCellCfg, nghCellMod, msgType, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell config structure*/
NlTmpNghCellMod   *nghCellMod; /* neighbor cell mod configuration*/
U32                msgType;    /* message type*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                   nghCount;
   U32                   nghNghCount;
   Bool                  pciCollision = FALSE;
   NlTmpNghCellModInfo   *cellModInfo  = NULLP;
   LnlEcgi               *ecgi         = NULLP;
   S16                   ret           = ROK;
   TRC3(nlNghModPciCollisionDetect);

   for(nghCount = 0; nghCount < nghCellMod->numNgh; nghCount ++)
   {
      cellModInfo = &(nghCellMod->nghCellModInfo[nghCount]);
      ecgi = &(cellModInfo->nghCellInfo.ecgi);
      NL_NGH_CELL_COLLISION_DETECT(cellCb, ecgi, cellModInfo->nghCellInfo.pci,
         cellModInfo->nghCellInfo.dlEarfcn, pciCollision);
      if(TRUE == pciCollision)
      {
         ret = RFAILED;
         /*PCI COLLISION OCCURED*/
          RLOG3(L_INFO, "PCI collison detected: serving cell PCI %d DL-Earfcn %d CGI %lu",
              cellCb->cellInfo.pci, 
              cellCb->cellInfo.dlEarfcn, cellCb->cellInfo.ecgi.cellId);
          RLOG3(L_INFO, "Neighbor cell PCI %d DL-Earfcn %d CGI %lu",
               cellModInfo->nghCellInfo.pci,
               cellModInfo->nghCellInfo.dlEarfcn, ecgi->cellId);

         /*update PCI value for raising alarm*/
         NL_FILL_PCI_COLLISON_ALARM(cellModInfo->nghCellInfo.ecgi.cellId, 
            cellCb, alarm);
         if((SON_ENB_CFG_UPD_NGH_CFG != msgType) &&
            (SON_X2SETUP_REQ_NGH_CFG != msgType))
         {
            ret = nlNghPciCollisionResolve(cellCb, nghCellCfg, alarm);
            alarm->resolveDeferred = TRUE;
         }
      }
      for(nghNghCount = 0; nghNghCount < cellModInfo->numNghNgh; nghNghCount ++)
      {
         ecgi = &(cellModInfo->nghNghCellInfo[nghNghCount].ecgi);
         NL_NGH_CELL_COLLISION_DETECT(cellCb, ecgi, cellModInfo->
            nghNghCellInfo[nghNghCount].pci,
            cellModInfo->nghNghCellInfo[nghNghCount].dlEarfcn, pciCollision)
         if(TRUE == pciCollision)
         {
            ret = RFAILED;
            /*PCI COLLISION OCCURED*/
            RLOG3(L_INFO, "PCI confusion detected: serving cell PCI %d DL-Earfcn %d CGI %lu",
              cellCb->cellInfo.pci, cellCb->cellInfo.dlEarfcn, cellCb->cellInfo.ecgi.cellId);
            RLOG3(L_INFO, "Neighbor's Neighbor cell PCI %d DL-Earfcn %d CGI %lu",
               cellModInfo->nghNghCellInfo[nghNghCount].pci, 
               cellModInfo->nghNghCellInfo[nghNghCount].dlEarfcn, ecgi->cellId);
            /*When PCI collision occured with neighbor's neighbor, 
              PCI confusion alarm will be raised*/
            /*update PCI value for raising alarm*/
            NL_FILL_PCI_COLLISON_ALARM(cellModInfo->nghNghCellInfo[nghNghCount].pci,
               cellCb, alarm);
            /*kwork fix*/
            if(alarm->numAlarmRaised <= LNL_MAX_NGH_CELL_INFO)
            {
               alarm->alarmRaised[alarm->numAlarmRaised -1].dgn.type = LNL_PCI_CONFUSION;
               alarm->alarmRaised[alarm->numAlarmRaised -1].dgn.alarmId = LNL_ALARM_PCI_CONFUSION;
            }
            if((SON_ENB_CFG_UPD_NGH_CFG != msgType) &&
            (SON_X2SETUP_REQ_NGH_CFG != msgType))
            {
               ret = nlNghPciCollisionResolve(cellCb, nghCellCfg, alarm);
               alarm->resolveDeferred = TRUE;
            }
         }
      }
   }
   if(ROK == ret)
   {
      RETVALUE(FALSE);
   }
   RETVALUE(TRUE);
}

/**
 * @brief PCI collison detection from neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghAddPciCollisionDetect
 *
 *     This function will validate whether pci received from SM/eNB-APP
 *     is matching with serving cell
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell config structure
 *  @param[in]  nghCellAdd : neighbor cell add configuration structure
 *  @param[in]  msgType    : message type
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : FALSE
 *     -# Failure       : TRUE
 *
 **/
#ifdef ANSI
PRIVATE Bool nlNghAddPciCollisionDetect
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell config structure*/
NlTmpNghCellAdd   *nghCellAdd, /* neighbor cell configuration add structure*/
U32                msgType,    /* message type*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PRIVATE Bool nlNghAddPciCollisionDetect(cellCb, nghCellCfg, nghCellAdd, msgType, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell config structure*/
NlTmpNghCellAdd   *nghCellAdd; /* neighbor cell add configuration*/
U32                msgType;    /* message type*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                   nghCount;
   U32                   nghNghCount;
   Bool                  pciCollision  = FALSE;
   NlTmpNghCellAddInfo   *cellAddInfo  = NULLP;
   LnlEcgi               *ecgi         = NULLP;
   S16                   ret = ROK;
   TRC3(nlNghAddPciCollisionDetect);

   for(nghCount = 0; nghCount < nghCellAdd->numNgh; nghCount ++)
   {
      cellAddInfo = &(nghCellAdd->nghCellAddInfo[nghCount]);
      ecgi = &(cellAddInfo->nghCellInfo.ecgi);
      NL_NGH_CELL_COLLISION_DETECT(cellCb, ecgi, cellAddInfo->nghCellInfo.pci,
         cellAddInfo->nghCellInfo.dlEarfcn, pciCollision);
      if(TRUE == pciCollision)
      {
         ret = RFAILED;
         /*PCI COLLISION OCCURED*/
         RLOG3(L_INFO, "PCI collison detected: serving cell PCI %d DL-Earfcn %d CGI %lu",
              cellCb->cellInfo.pci, 
              cellCb->cellInfo.dlEarfcn, cellCb->cellInfo.ecgi.cellId);
         RLOG3(L_INFO, "Neighbor cell PCI %d DL-Earfcn %d CGI %lu",
               cellAddInfo->nghCellInfo.pci,
               cellAddInfo->nghCellInfo.dlEarfcn, ecgi->cellId);

         /*update PCI value for raising alarm*/
         NL_FILL_PCI_COLLISON_ALARM(cellAddInfo->nghCellInfo.ecgi.cellId, 
               cellCb, alarm);
         if((SON_ENB_CFG_UPD_NGH_CFG != msgType) &&
            (SON_X2SETUP_REQ_NGH_CFG != msgType))
         {
            ret = nlNghPciCollisionResolve(cellCb, nghCellCfg, alarm);
            alarm->resolveDeferred = TRUE;
         }
      }
      for(nghNghCount = 0; nghNghCount < cellAddInfo->numNghNgh; nghNghCount ++)
      {
         ecgi = &(cellAddInfo->nghNghCellInfo[nghNghCount].ecgi);
         NL_NGH_CELL_COLLISION_DETECT(cellCb, ecgi, cellAddInfo->
            nghNghCellInfo[nghNghCount].pci,
            cellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn, pciCollision)
         if(TRUE == pciCollision)
         {
            ret = RFAILED;
            /*PCI COLLISION OCCURED*/
            RLOG3(L_INFO, "PCI confusion detected: serving cell PCI %d DL-Earfcn %d CGI %lu",
              cellCb->cellInfo.pci, cellCb->cellInfo.dlEarfcn, cellCb->cellInfo.ecgi.cellId);
            RLOG3(L_INFO, "Neighbor's Neighbor cell PCI %d DL-Earfcn %d CGI %lu",
               cellAddInfo->nghNghCellInfo[nghNghCount].pci,
               cellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn, ecgi->cellId);
            /*when PCI collision occured with neighbor's neighbor PCI, 
              PCI confusion alarm will be raised*/
            /*update PCI value for raising alarm*/
            NL_FILL_PCI_COLLISON_ALARM(cellAddInfo->
               nghNghCellInfo[nghNghCount].ecgi.cellId, cellCb, alarm);
            /*kwork fixes*/
            if(alarm->numAlarmRaised <= LNL_MAX_NGH_CELL_INFO)
            {
               alarm->alarmRaised[alarm->numAlarmRaised -1].dgn.type = LNL_PCI_CONFUSION;       
               alarm->alarmRaised[alarm->numAlarmRaised -1].dgn.alarmId = LNL_ALARM_PCI_CONFUSION;
            }
            if((SON_ENB_CFG_UPD_NGH_CFG != msgType) &&
            (SON_X2SETUP_REQ_NGH_CFG != msgType))
            {
               ret = nlNghPciCollisionResolve(cellCb, nghCellCfg, alarm);
               alarm->resolveDeferred = TRUE;
            }
         }
      }
   }
   if(ROK == ret)
   {
      RETVALUE(FALSE);
   }
   RETVALUE(TRUE);
}
/**
 * @brief PCI collison detection from neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghPciCollisionDetect
 *
 *     This function will validate whether pci received from SM/eNB-APP
 *     is matching with serving cell
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration structure
 *  @param[in]  msgType    : message type
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : FALSE
 *     -# Failure       : TRUE
 *
 **/
#ifdef ANSI
PUBLIC Bool nlNghPciCollisionDetect
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration structure*/
U32                msgType,    /* type of message*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC Bool nlNghPciCollisionDetect(cellCb, nghCellCfg, msgType, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
U32                msgType;    /* type of message*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32   numNghEnb=0;
   Bool  pciConflict = FALSE;
   TRC3(nlNghPciCollisionDetect);
   for(numNghEnb = 0; numNghEnb < nghCellCfg->numNghEnb; numNghEnb ++)
   {
      switch(nghCellCfg->nghEnbCellCfg[numNghEnb].nghCellParam.cfgType)
      {
         case NL_NGH_ADD :
         {
            if(TRUE == nlNghAddPciCollisionDetect(cellCb, nghCellCfg,
                       &(nghCellCfg->nghEnbCellCfg[numNghEnb].
                       nghCellParam.u.nghCellAdd), msgType, alarm))
            {
               RLOG0(L_ERROR, "PCI collison detected for new PCI addition");
               pciConflict = TRUE;
            }  
            break; 
         }
         case NL_NGH_MOD :
         {
            if(TRUE == nlNghModPciCollisionDetect(cellCb, nghCellCfg,
                       &(nghCellCfg->nghEnbCellCfg[numNghEnb].
                       nghCellParam.u.nghCellMod), msgType, alarm))
            {
               RLOG0(L_ERROR, "PCI collison detected for PCI modification");
               pciConflict = TRUE;
            }
            break;
         }
         case NL_NGH_DEL :
         {
            break;
         }
         default :
         {
            RLOG0(L_ERROR, "Inavlid neighbor cfg type received");
            RETVALUE(TRUE);
         }
      }
   }
   if(TRUE == pciConflict)
   {
      RETVALUE(TRUE);
   }
   RETVALUE(FALSE);
}

/**
 * @brief PCI confusion detection from neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghModPciConfusionDetect
 *
 *     This function will validate whether pci received from SM/eNB-APP
 *     is matching with serving cell
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellMod : neighbor cell configuration from SM
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : FALSE
 *     -# Failure       : TRUE
 *
 **/
#ifdef ANSI
PUBLIC Bool nlNghModPciConfusionDetect
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellMod   *nghCellMod, /* neighbor cell configuration structure*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC Bool nlNghModPciConfusionDetect(cellCb, nghCellMod, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellMod   *nghCellMod; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                   nghCount;
   U32                   nghNghCount;
   NlTmpNghCellModInfo  *cellModInfo = NULLP;
   TRC3(nlNghAddPciConfusionDetect);

   for(nghCount = 0; nghCount < nghCellMod->numNgh; nghCount ++)
   {
      /*check whether any direct neighbor creating PCI confusion*/
      cellModInfo = &(nghCellMod->nghCellModInfo[nghCount]);

      if(ROK != nlValidatePciEcgiWithNghCellInfo(cellCb, cellModInfo->
        nghCellInfo.pci, &cellModInfo->nghCellInfo.ecgi,
        cellModInfo->nghCellInfo.dlEarfcn, alarm))
      {
         /*PCI CONFUSION OCCURED*/
         RLOG3(L_INFO, "PCI confusion detected with PCI mod val %d Ngh Ecgi CellId %d "
               "DL-Earfcn %d", cellModInfo->nghCellInfo.pci, 
                cellModInfo->nghCellInfo.ecgi.cellId, cellModInfo->nghCellInfo.dlEarfcn);
      }
      /*check for neighbor's neighbor creating PCI confusion*/
      for(nghNghCount = 0; nghNghCount < cellModInfo->numNghNgh; nghNghCount++)
      {
         if(ROK != nlValidatePciEcgiWithNghCellInfo(cellCb, cellModInfo->
            nghNghCellInfo[nghNghCount].pci, 
            &cellModInfo->nghNghCellInfo[nghNghCount].ecgi, 
            cellModInfo->nghNghCellInfo[nghNghCount].dlEarfcn, alarm))
         {
            /*PCI CONFUSION OCCURED*/
            RLOG3(L_INFO, "PCI confusion detected with PCI mod val %d Ngh Ngh Ecgi CellId %d"
               "DL-Earfcn %d", cellModInfo->nghNghCellInfo[nghNghCount].pci,
               cellModInfo->nghNghCellInfo[nghNghCount].ecgi.cellId, 
               cellModInfo->nghNghCellInfo[nghNghCount].dlEarfcn);
         }
      }
   }
   RETVALUE(FALSE);
}
/**
 * @brief PCI confusion detection from neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghAddPciConfusionDetect
 *
 *     This function will validate whether pci received from SM/eNB-APP
 *     is matching with serving cell
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellAdd : neighbor cell configuration from SM
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : FALSE
 *     -# Failure       : TRUE
 *
 **/
#ifdef ANSI
PUBLIC Bool nlNghAddPciConfusionDetect
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellAdd   *nghCellAdd, /* neighbor cell configuration structure*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC Bool nlNghAddPciConfusionDetect(cellCb, nghCellAdd, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellAdd   *nghCellAdd; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                   nghCount;
   U32                   nghNghCount;
   NlTmpNghCellAddInfo  *cellAddInfo = NULLP;
   TRC3(nlNghAddPciConfusionDetect);

   for(nghCount = 0; nghCount < nghCellAdd->numNgh; nghCount ++)
   {
      /*check whether any direct neighbor creating PCI confusion*/
      cellAddInfo = &(nghCellAdd->nghCellAddInfo[nghCount]);

      if(ROK != nlValidatePciEcgiWithNghCellInfo(cellCb, cellAddInfo->
        nghCellInfo.pci, &cellAddInfo->nghCellInfo.ecgi,
        cellAddInfo->nghCellInfo.dlEarfcn, alarm))
      {
         /*PCI CONFUSION OCCURED*/
         RLOG3(L_INFO, "PCI confusion detected with PCI val %d New Ngh Ecgi CellId %d "
              "DL-Earfcn %d", cellAddInfo->nghCellInfo.pci, 
               cellAddInfo->nghCellInfo.ecgi.cellId, cellAddInfo->nghCellInfo.dlEarfcn);
      }
      /*check for neighbor's neighbor creating PCI confusion*/
      for(nghNghCount = 0; nghNghCount < cellAddInfo->numNghNgh; nghNghCount++)
      {
         if(ROK != nlValidatePciEcgiWithNghCellInfo(cellCb, cellAddInfo->
            nghNghCellInfo[nghNghCount].pci, 
            &cellAddInfo->nghNghCellInfo[nghNghCount].ecgi, 
            cellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn, alarm))
         {
            /*PCI CONFUSION OCCURED*/
            RLOG3(L_INFO, "PCI confusion detected with PCI val %d new Ngh Ngh CellId %d "
               "DL-Earfcn %d", cellAddInfo->nghNghCellInfo[nghNghCount].pci, 
               cellAddInfo->nghNghCellInfo[nghNghCount].ecgi.cellId,
               cellAddInfo->nghNghCellInfo[nghNghCount].dlEarfcn);
         }
      }
   }
   RETVALUE(FALSE);
}

/**
 * @brief PCI confusion detection from neighbor cell Information
 *
 * @details
 *
 *     Function : nlNghPciConfusionDetect
 *
 *     This function will validate whether pci received from SM/eNB-APP
 *     is matching with serving cell
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  lnlNghCell : neighbor cell configuration from SM
 *  @param[in]  nluNghCell : neighbor cell configuration from eNB-APP
 *  @param[in]  alarm      : Indicate alarm raised or not
 *  @return  S16
 *     -# Success       : FALSE
 *     -# Failure       : TRUE
 *
 **/
#ifdef ANSI
PUBLIC Bool nlNghPciConfusionDetect
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration structure*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC Bool nlNghPciConfusionDetect(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32   nghEnbCount=0;
   TRC3(nlNghPciConfusionDetect);
   /*considering there is no duplicate value in neighbor 
     information what ever received from SM/eNB-APP*/
   /*If there is any duplicate neighbor information is present corresponding 
     neighbor information will not be inserted into hash list if it is a 
     direct neighbor*/
   /*will add duplicate neighbor information validation later TODO*/
   for(nghEnbCount = 0; nghEnbCount < nghCellCfg->numNghEnb; nghEnbCount ++)
   {
      switch(nghCellCfg->nghEnbCellCfg[nghEnbCount].nghCellParam.cfgType)
      {
         case NL_NGH_ADD :
         {
            if(TRUE == nlNghAddPciConfusionDetect(cellCb, 
              &(nghCellCfg->nghEnbCellCfg[nghEnbCount].
               nghCellParam.u.nghCellAdd), alarm))
            {
               RETVALUE(TRUE);
            }
            break;
         }
         case NL_NGH_MOD :
         {
            if(TRUE == nlNghModPciConfusionDetect(cellCb, 
              &(nghCellCfg->nghEnbCellCfg[nghEnbCount].
              nghCellParam.u.nghCellMod), alarm))
            {
               RETVALUE(TRUE);
            }
            break;
         }
         case NL_NGH_DEL :
         {
            break;
         }
         default :
         {
            RETVALUE(TRUE);
         }
      }
   }
   RETVALUE(FALSE);
}

/**
 * @brief neighbor cell information updation as part of PCI modification
 *
 * @details
 *
 *     Function : nlUpdateNghCellInfo
 *
 *     This function will modify the pci of neighbor cell.
 *     It will clear the alarm if raised for the previous PCI
 *
 *  @param[in]  nghcellCb     : neighbor cell control block
 *  @param[in]  srcCellInfo   : cell information which will be updated in neighbor cell information
 *  @param[in]  alarm         : Indicate alarm raised or not
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void nlUpdateNghCellInfo
(
NlNghServCellCb     *nghCellCb,
NlCellInfo          *srcCellInfo,  /* neighbor cell configuration from SM*/
NlAlarm             *alarm
)
#else
PRIVATE Void nlUpdateNghCellInfo(nghCellCb, srcCellInfo, alarm)
(
NlNghServCellCb     *nghCellCb;
NlCellInfo          *srcCellInfo;  /* neighbor cell configuration from SM*/
NlAlarm             *alarm;
)
#endif
{
   NlCellInfo *tgtCellInfo = NULL;
   TRC3(nlUpdateNghCellInfo);
   tgtCellInfo = &(nghCellCb->servCellInfo);
   tgtCellInfo->dlEarfcn = srcCellInfo->dlEarfcn;
   tgtCellInfo->ulEarfcn = srcCellInfo->ulEarfcn;
   cmMemcpy((U8 *)&tgtCellInfo->ecgi, (U8 *)&srcCellInfo->ecgi, sizeof(LnlEcgi));
   if(tgtCellInfo->pci != srcCellInfo->pci)
   {
      /*check whether this PCI modification clearing any existing Alarm*/
      NL_FILL_CLEAR_ALARM(nghCellCb->servCellInfo, alarm);
      nlNghPeerAlarmClear(nghCellCb);
      tgtCellInfo->pci = srcCellInfo->pci;
   }
   RETVOID;
}
/**
 * @brief neighbor's neighbor cell information updation as part of PCI modification
 *
 * @details
 *
 *     Function : nlUpdateNghNghCellInfo
 *
 *     This function will modify the pci of neighbor's neighbor cell.
 *     It will clear the alarm if raised for the previous PCI
 *
 *  @param[in]  nghNghcellCb  : neighbor's neighbor cell control block
 *  @param[in]  srcCellInfo   : cell information which will be updated in neighbor cell information
 *  @param[in]  alarm         : Indicate alarm raised or not
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void nlUpdateNghNghCellInfo
(
NlNghNghCellCb      *nghNghCellCb,
NlCellInfo          *srcCellInfo,  /* neighbor cell configuration from SM*/
NlAlarm             *alarm
)
#else
PRIVATE Void nlUpdateNghNghCellInfo(nghNghCellCb, srcCellInfo, alarm)
(
NlNghNghCellCb      *nghNghCellCb;
NlCellInfo          *srcCellInfo;  /* neighbor cell configuration from SM*/
NlAlarm             *alarm;
)
#endif
{
   NlCellInfo *tgtCellInfo = NULL;
   TRC3(nlUpdateNghNghCellInfo);
   tgtCellInfo = &(nghNghCellCb->nghNghCellInfo);
   tgtCellInfo->dlEarfcn = srcCellInfo->dlEarfcn;
   tgtCellInfo->ulEarfcn = srcCellInfo->ulEarfcn;
   cmMemcpy((U8 *)&tgtCellInfo->ecgi, (U8 *)&srcCellInfo->ecgi, sizeof(LnlEcgi));
   if(tgtCellInfo->pci != srcCellInfo->pci)
   {
      /*check whether this PCI modification clearing any existing Alarm*/
      NL_FILL_CLEAR_ALARM(nghNghCellCb->nghNghCellInfo, alarm);
      nlNghNghPeerAlarmClear(nghNghCellCb);
      tgtCellInfo->pci = srcCellInfo->pci;
   }
   RETVOID;
}

/**
 * @brief insert the neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlStoreNghNghCellInfo
 *
 *     This function will insert neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  nghCellCb  : neighbor cell information in SON
 *  @param[in]  srcCellInfo: neighbor cell information from SM/eNB-APP 
 *  @param[in]  isNewNgh   : Indicate whether this is new neighbor or existing one 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void nlStoreNghNghCellInfo
(
NlNghNghCellCb      *nghNghCellCb,  /* neighbor's neighbor cell information in SON*/
NlCellInfo          *tgtCellInfo,  /* neighbor cell configuration from SM*/
Bool                 isNewNgh,     /* whether this is new neighbor or not*/
NlAlarm             *alarm         /* alarm strcuture*/
)
#else
PRIVATE Void nlStoreNghNghCellInfo(nghNghCellCb, tgtCellInfo, isNewNgh, alarm)
(
NlNghNghCellCb      *nghNghCellCb; /* neighbor's neighbor cell information in SON*/
NlCellInfo          *tgtCellInfo;  /* neighbor cell configuration*/
Bool                 isNewNgh;     /* whether this is new neighbor or not*/
NlAlarm             *alarm;        /* alarm structure*/
)
#endif
{
   TRC3(nlStoreNghNghCellInfo);
   if(TRUE != isNewNgh)
   {
      if(nghNghCellCb->nghNghCellInfo.pci != tgtCellInfo->pci)
      {
         /*check whether this PCI modification clearing any existing Alarm*/
         NL_FILL_CLEAR_ALARM(nghNghCellCb->nghNghCellInfo, alarm);
         nlNghNghPeerAlarmClear(nghNghCellCb);
      }
      nghNghCellCb->nghNghCellInfo.pci = tgtCellInfo->pci;
   }
   else
   {
      nghNghCellCb->nghNghCellInfo.dlEarfcn = tgtCellInfo->dlEarfcn;
      nghNghCellCb->nghNghCellInfo.ulEarfcn = tgtCellInfo->ulEarfcn;
      cmMemcpy((U8 *)&nghNghCellCb->nghNghCellInfo.ecgi, 
        (U8 *)&tgtCellInfo->ecgi, sizeof(LnlEcgi));
      nghNghCellCb->nghNghCellInfo.pci = tgtCellInfo->pci;
   }
   RETVOID;
}

/**
 * @brief insert the neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlStoreNghCellInfo
 *
 *     This function will insert neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  cellCb     : Serv cell information in SON
 *  @param[in]  nghCellCb  : neighbor cell information in SON
 *  @param[in]  srcCellInfo: neighbor cell information from SM/eNB-APP 
 *  @param[in]  isNewNgh   : Indicate whether this is new neighbor or existing one 
 *  @param[in]  alarm      : alarm structure 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void nlStoreNghCellInfo
(
NlCellCb            *cellCb,     /* serv cell information in SON*/
NlNghServCellCb     *nghCellCb,  /* neighbor cell information in SON*/
NlCellInfo          *tgtCellInfo,/* neighbor cell configuration from SM*/
Bool                 isNewNgh,   /* whether this is new neighbor or not*/
NlAlarm             *alarm       /* alarm structure*/
)
#else
PRIVATE Void nlStoreNghCellInfo(cellCb, nghCellCb, tgtCellInfo, isNewNgh, alarm)
(
NlCellCb            *cellCb,       /* serv cell information in SON*/
NlNghServCellCb     *nghCellCb;    /* neighbor cell information in SON*/
NlCellInfo          *tgtCellInfo;  /* neighbor cell configuration*/
Bool                 isNewNgh;     /* whether this is new neighbor or not*/
NlAlarm             *alarm;        /* alarm structure*/
)
#endif
{
   TRC3(nlStoreNghCellInfo);
   if(TRUE != isNewNgh)
   {
      if(nghCellCb->servCellInfo.pci != tgtCellInfo->pci)
      {
         /*check whether this PCI modification clearing any existing Alarm*/
         NL_FILL_CLEAR_ALARM(nghCellCb->servCellInfo, alarm);
         nlNghPeerAlarmClear(nghCellCb);
      }
      nghCellCb->servCellInfo.pci = tgtCellInfo->pci;
   }
   else
   {
      nghCellCb->servCellInfo.dlEarfcn = tgtCellInfo->dlEarfcn;
      nghCellCb->servCellInfo.ulEarfcn = tgtCellInfo->ulEarfcn;
      cmMemcpy((U8 *)&nghCellCb->servCellInfo.ecgi, 
        (U8 *)&tgtCellInfo->ecgi, sizeof(LnlEcgi));
      nghCellCb->servCellInfo.pci = tgtCellInfo->pci;
      nghCellCb->servCellInfo.csgAccessMode = tgtCellInfo->csgAccessMode;
      nghCellCb->servCellInfo.csgId = tgtCellInfo->csgId;
      nghCellCb->servCellInfo.lteBand = tgtCellInfo->lteBand;
      nghCellCb->servCellInfo.tac = tgtCellInfo->tac;
      nghCellCb->servCellInfo.ulBw = tgtCellInfo->ulBw;
      nghCellCb->servCellInfo.dlBw = tgtCellInfo->dlBw;
      nghCellCb->servCellInfo.RSTxPower = tgtCellInfo->RSTxPower;
      nghCellCb->servCellInfo.rsrp = tgtCellInfo->rsrp;
      nghCellCb->servCellInfo.rssi = tgtCellInfo->rssi;
      nghCellCb->servCellInfo.isCellBarred = tgtCellInfo->isCellBarred;
      /*check if neighbor PLMN  is same as serving cell PLMN then only increment the number of neighbor*/
      if(FALSE == cmMemcmp((U8 *)&(cellCb->cellInfo.ecgi.plmn),
                (U8 *) &(tgtCellInfo->ecgi.plmn), sizeof(LnlPlmnId)))
      {
         cellCb->numNgh ++;
         if(NL_VENDOR_DSON_STATE_CONFIGURED == cellCb->dSonState)
         {
            nlReportNumNghToSon(cellCb->numNgh);
         }
      }
   }
   RETVOID;
}

/**
 * @brief find the neighbor serving cell control block
 *
 * @details
 *
 *     Function : nlFindNghServingCell 
 *
 *     This function will find the neighbor servcell control block as per ECGI
 *
 *  @param[in]  servCellList  : neighbor serv  cell list
 *  @param[in]  nghCellInfo   : ecgi value
 *  @param[in]  **nghServCell : update the neighbor serv cell if found in 
 *                              neighbor serv cell list
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED 
 *
 **/
#ifdef ANSI
PRIVATE S16 nlFindNghServingCell
(
CmLListCp         *servCellList, 
NlCellInfo        *nghCellInfo,
NlNghServCellCb   **nghServCell
)
#else
PRIVATE S16 nlFindNghServingCell(servCellList, nghCellInfo, nghServCell)
(
CmLListCp         *servCellList;
NlCellInfo        *nghCellInfo; 
NlNghServCellCb   **nghServCell;
)
#endif
{
   NlNghServCellCb   *servCellCb;
   CmLList*          link = NULLP;
   TRC3(nlFindNghServingCell);

   CM_LLIST_FIRST_NODE(servCellList, link);
   while (NULLP != link)
   {
      servCellCb = (NlNghServCellCb *)link->node;
      if(FALSE == cmMemcmp((U8 *)&servCellCb->servCellInfo.ecgi, (U8 *)&nghCellInfo->ecgi, 
          sizeof(LnlEcgi)))
      {
         *nghServCell = servCellCb;
         RETVALUE(ROK);
      }
      link = NULLP;
      CM_LLIST_NEXT_NODE(servCellList, link);
   }
   RETVALUE(RFAILED);
}
/**
 * @brief find the neighbor's neoghbor cell control block with PCI 
 *
 * @details
 *
 *     Function : nlFindNghNghCell
 *
 *     This function will validate whether a neighbor's neighbor cell 
 *     control block present with a certail PCI value
 *
 *  @param[in]  nghCellList  : neighbor's neighbor cell List
 *  @param[in]  nghCellInfo  : neighbor cell information
 *  @param[in]  nghNghCell   : neighbor's neighbor cellInfo
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 **/
#ifdef ANSI
PRIVATE S16 nlFindNghNghCell
(
CmLListCp         *nghCellList,
NlCellInfo        *nghCellInfo,
NlNghNghCellCb    **nghNghCell
)
#else
PRIVATE S16 nlFindNghNghCell(nghCellList, nghCellInfo, nghNghCell)
(
CmLListCp         *nghCellList;
NlCellInfo        *nghCellInfo;
NlNghNghCellCb    **nghNghCell;
)
#endif
{
   NlNghNghCellCb   *nghNghCellCb = NULLP;
   CmLList*          link = NULLP;

   TRC3(nlFindNghNghCell);
   
   CM_LLIST_FIRST_NODE(nghCellList, link);
   while (NULLP != link)
   {
      nghNghCellCb = (NlNghNghCellCb *)link->node;
      if((nghNghCellCb->nghNghCellInfo.ecgi.cellId > 0) &&
        (nghCellInfo->ecgi.cellId > 0))
      {
         if(FALSE == cmMemcmp((U8 *)&nghNghCellCb->nghNghCellInfo.ecgi, 
              (U8 *)&nghCellInfo->ecgi, sizeof(LnlEcgi)))
         {
            *nghNghCell = nghNghCellCb;
            RETVALUE(ROK);
         }
      }
      else if(nghNghCellCb->nghNghCellInfo.pci == nghCellInfo->pci)
      {
         *nghNghCell = nghNghCellCb;
         RETVALUE(ROK);
      }
      link = NULLP;
      CM_LLIST_NEXT_NODE(nghCellList, link);
   }
   RETVALUE(RFAILED);
}

/**
 * @brief detect PCI confusion with new neighbor add config received from SM.
 *
 * @details
 *
 *     Function : nlNghNghPciConfusionDetectWithAddNewNghCfg 
 *
 *     This function will detect the PCI confusion with new neighbor
 *     add config received from SM.
 *
 *  @param[in]  nghNghCb      : new neighbor's neighbor info already store in ngh table
 *  @param[in]  nghCellCfg    : neighbor cell configuration
 *  @param[in]  nghEnbIdx     : neighbor eNB configuration Idx
 *  @param[in]  nghCelIdx     : neighbor cell configuration Idx
 *  @param[in]  nghNghCelIdx  : neighbor's neighbor cell configuration Idx
 *  @param[in]  alarm         : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghNghPciConfusionDetectWithAddNewNghCfg
(
NlNghNghCellCb    *nghNghCb,   /* neighbor's neighbor cell cb*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM*/
U32                nghEnbIdx,  /* neighbor enodeB count index in new neighbor cfg from SM*/
U32                nghCellIdx, /* neighbor cell count index in new neighbor cfg from SM*/
U32                nghNghCellIdx,/*neighbor's neighbor cell count index*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PRIVATE S16 nlNghNghPciConfusionDetectWithAddNewNghCfg(nghNghCb, nghCellCfg,
                                 nghEnbIdx, nghCellIdx, alarm)
(
NlNghNghCellCb    *nghNghCb,   /* neighbor's neighbor cell cb*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
U32                nghEnbIdx;  /* neighbor enodeB count index in new neighbor cfg from SM*/
U32                nghCellIdx; /* neighbor count index in new neighbor cfg from SM*/
U32                nghNghCellIdx;/*neighbor's neighbor cell count index*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32               nghEnbCount;
   U32               nghCount;
   U32               nghNghCount;

   NlTmpNghCellAddInfo *cellAddInfo = NULLP;
   NlCellInfo          *cellAdd = NULLP;
   NlTmpNghCellAdd     *nghCellAdd = NULLP;
   NlCellInfo          *nghNghCellAdd = NULLP;
   Bool                flag           = FALSE;

   TRC3(nlNghNghPciConfusionDetectWithAddNewNghCfg);

   for(nghEnbCount = nghEnbIdx; nghEnbCount < nghCellCfg->
                                numNghEnb; nghEnbCount ++)
   {
      nghCellAdd = &(nghCellCfg->nghEnbCellCfg[nghEnbCount].
                      nghCellParam.u.nghCellAdd);
      for(nghCount = nghCellIdx; nghCount < nghCellAdd->numNgh; nghCount ++)
      {
         cellAddInfo = &(nghCellAdd->nghCellAddInfo[nghCount]);
         cellAdd = &(cellAddInfo->nghCellInfo);
         if(TRUE == flag)
         {
            /*check whether any direct neighbor creating PCI confusion with
              this neighbor's neighbor*/
            if((cellAdd->pci == nghNghCb->nghNghCellInfo.pci) &&
              (cellAdd->dlEarfcn == nghNghCb->nghNghCellInfo.dlEarfcn))
            {
               if(nghNghCb->nghNghCellInfo.ecgi.cellId > 0)
               {
                  if(FALSE != cmMemcmp((U8 *)&(cellAdd->ecgi),
                    (U8 *) &(nghNghCb->nghNghCellInfo.ecgi), sizeof(LnlEcgi)))
                  {
                     NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(cellAdd->pci,
                                   cellAdd->ecgi.cellId, nghNghCb, alarm);
                     RLOG3(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d CGI %lu ",
                          nghNghCb->nghNghCellInfo.pci, nghNghCb->nghNghCellInfo.dlEarfcn,
                          nghNghCb->nghNghCellInfo.ecgi.cellId);
                     RLOG3(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     cellAdd->pci, cellAdd->dlEarfcn, cellAdd->ecgi.cellId);
                  }
               }   
               else
               {
                  NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(cellAdd->pci,
                                   cellAdd->ecgi.cellId, nghNghCb, alarm);
                  RLOG2(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d ",
                       nghNghCb->nghNghCellInfo.pci, nghNghCb->nghNghCellInfo.dlEarfcn);
                  RLOG2(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d",
                     cellAdd->pci, cellAdd->dlEarfcn);
               }
            }
         }
         for(nghNghCount = nghNghCellIdx; nghNghCount < cellAddInfo->numNghNgh; nghNghCount ++)
         {
            flag = TRUE;
            nghNghCellAdd = &(cellAddInfo->nghNghCellInfo[nghNghCount]);
            if((nghNghCellAdd->pci == nghNghCb->nghNghCellInfo.pci) &&
              (nghNghCellAdd->dlEarfcn == nghNghCb->nghNghCellInfo.dlEarfcn))
            {
               if((nghNghCellAdd->ecgi.cellId > 0) && 
                  (nghNghCb->nghNghCellInfo.ecgi.cellId > 0))
               {
                  if(FALSE != cmMemcmp((U8 *)&(nghNghCellAdd->ecgi),
                   (U8 *) &(nghNghCb->nghNghCellInfo.ecgi), sizeof(LnlEcgi)))
                  {
                     NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(nghNghCellAdd->pci,
                                   nghNghCellAdd->ecgi.cellId, nghNghCb, alarm);
                     RLOG3(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d CGI %lu ",
                          nghNghCb->nghNghCellInfo.pci, nghNghCb->nghNghCellInfo.dlEarfcn,
                          nghNghCb->nghNghCellInfo.ecgi.cellId);
                     RLOG3(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     nghNghCellAdd->pci, nghNghCellAdd->dlEarfcn, nghNghCellAdd->ecgi.cellId);
                  }
               }
               else
               {
                  NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(nghNghCellAdd->pci,
                                   nghNghCellAdd->ecgi.cellId, nghNghCb, alarm);
                  RLOG2(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d ",
                       nghNghCb->nghNghCellInfo.pci, nghNghCb->nghNghCellInfo.dlEarfcn);
                  RLOG2(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d",
                     nghNghCellAdd->pci, nghNghCellAdd->dlEarfcn);
               }
            }
         }
         nghNghCellIdx = 0;
      }
      nghCellIdx = 0;
   }
   RETVALUE(ROK);
}

/**
 * @brief detect PCI confusion with new neighbor add config received from SM.
 *
 * @details
 *
 *     Function : nlNghPciConfusionDetectWithAddNewNghCfg
 *
 *     This function will detect the PCI confusion with new neighbor
 *     add config received from SM.
 *
 *  @param[in]  nghServCb  : new neighbor info already store in ngh table
 *  @param[in]  nghCellCfg : neighbor cell configuration
 *  @param[in]  nghEnbidx  : neighbor eNB configuration Idx
 *  @param[in]  nghCelIdx  : neighbor cell configuration Idx
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghPciConfusionDetectWithAddNewNghCfg
(
NlNghServCellCb   *nghServCb,  /* neighbor cell cb*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM*/
U32                nghEnbIdx,  /* neighbor enodeB count index in new neighbor cfg from SM*/
U32                nghCellIdx, /* neighbor cell count index in new neighbor cfg from SM*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PRIVATE S16 nlNghPciConfusionDetectWithAddNewNghCfg(nghServCb, nghCellCfg, 
                                 nghEnbIdx, nghCellIdx, alarm)
(
NlNghServCellCb   *nghServCb;  /* neighbor cell cb*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
U32                nghEnbIdx;  /* neighbor enodeB count index in new neighbor cfg from SM*/
U32                nghCellIdx; /* neighbor count index in new neighbor cfg from SM*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                 nghEnbCount;
   U32                 nghCount;
   U32                 nghNghCount;
   NlTmpNghCellAddInfo *cellAddInfo = NULLP;
   NlCellInfo          *cellAdd = NULLP;
   NlTmpNghCellAdd     *nghCellAdd = NULLP;
   TRC3(nlNghPciConfusionDetectWithAddNewNghCfg);

   for(nghEnbCount = nghEnbIdx; nghEnbCount < nghCellCfg->
                                numNghEnb; nghEnbCount ++)
   {
      nghCellAdd = &(nghCellCfg->
            nghEnbCellCfg[nghEnbCount].nghCellParam.u.nghCellAdd);
      for(nghCount = nghCellIdx; nghCount < nghCellAdd->numNgh; nghCount ++)
      {
         cellAddInfo = &(nghCellAdd->nghCellAddInfo[nghCount]);
         cellAdd = &(cellAddInfo->nghCellInfo);
         /*check whether any direct neighbor creating PCI confusion with
           this neighbor*/
         if((cellAdd->pci == nghServCb->servCellInfo.pci) &&
            (cellAdd->dlEarfcn == nghServCb->servCellInfo.dlEarfcn))
         {
            if(FALSE != cmMemcmp((U8 *)&(cellAdd->ecgi), 
                (U8 *) &(nghServCb->servCellInfo.ecgi), sizeof(LnlEcgi)))
            {
               NL_FILL_NGH_CELL_PCI_CONFUSION_ALARM(cellAdd->pci, 
                                   cellAdd->ecgi.cellId, nghServCb, alarm);
               RLOG3(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d CGI %lu ",
                              nghServCb->servCellInfo.pci, nghServCb->servCellInfo.dlEarfcn,
                              nghServCb->servCellInfo.ecgi.cellId);
               RLOG3(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     cellAdd->pci, cellAdd->dlEarfcn, cellAdd->ecgi.cellId);
            }
         }
         /*check whether any neighbor's neighbor creating PCI confusion with
           this neighbor*/
         for(nghNghCount = 0; nghNghCount < cellAddInfo->numNghNgh; nghNghCount ++)
         {
            cellAdd = &(cellAddInfo->nghNghCellInfo[nghNghCount]);
            if((cellAdd->pci == nghServCb->servCellInfo.pci) &&
              (cellAdd->dlEarfcn == nghServCb->servCellInfo.dlEarfcn))
            {
               if(FALSE != cmMemcmp((U8 *)&(cellAdd->ecgi),
                (U8 *) &(nghServCb->servCellInfo.ecgi), sizeof(LnlEcgi)))
               {
                  NL_FILL_NGH_CELL_PCI_CONFUSION_ALARM(cellAdd->pci,
                                   cellAdd->ecgi.cellId, nghServCb, alarm);
                  RLOG3(L_INFO, "PCI confusion detected: Neighbor1 PCI %d DL-Earfcn %d CGI %lu ",
                              nghServCb->servCellInfo.pci, nghServCb->servCellInfo.dlEarfcn,
                              nghServCb->servCellInfo.ecgi.cellId);
                  RLOG3(L_INFO, "Neighbor2 PCI %d DL-Earfcn %d CGI %lu",
                     cellAdd->pci, cellAdd->dlEarfcn, cellAdd->ecgi.cellId);
               }
            }
         }
      }
      nghCellIdx = 0;
   }
   RETVALUE(ROK);
}

/**
 * @brief insert the neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlNghPciInfoAdd
 *
 *     This function will insert neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration 
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PUBLIC S16 nlNghPciInfoAdd
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC S16 nlNghPciInfoAdd(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                   numEnbCount;
   U32                   nghCount;
   U32                   nghNghCount;
   NlNeighCellCb         *nghCellCb = NULLP;
   NlTmpNghCellAddInfo   *cellAddInfo = NULLP;
   NlNghServCellCb       *nghServCell = NULL;
   NlNghNghCellCb        *nghNghCell  = NULL;
   U32                    hashKey;
   TRC3(nlNghPciInfoAdd);

   for(numEnbCount = 0; numEnbCount < nghCellCfg->numNghEnb; numEnbCount ++)
   {
      hashKey = nghCellCfg->nghEnbCellCfg[numEnbCount].nghEnbId;
      if(ROK != cmHashListFind(&(cellCb->neighEnbCb.nghEnbListCp),
                (U8 *)&hashKey, sizeof(hashKey), 0, (PTR *)&nghCellCb))
      {
         NL_ALLOC(NL_REGION, NL_POOL, &nghCellCb, sizeof(NlNeighCellCb));
         if(NULLP == nghCellCb)
         {
            RLOG0(L_ERROR, "Failed to allocate memory for neighbor cells");
            RETVALUE(RFAILED);
         }
         nghCellCb->enbId = hashKey;
         if(ROK != cmHashListInsert(&(cellCb->neighEnbCb.nghEnbListCp),
            (PTR)nghCellCb,(U8 *)&nghCellCb->enbId, sizeof(nghCellCb->enbId)))
         {
            RLOG1(L_ERROR, "failed to insert neighbor cell into hash table "
             "with eNBId %d", hashKey);
            NL_FREE(NL_REGION, NL_POOL, nghCellCb, sizeof(NlNeighCellCb));
            RETVALUE(RFAILED);
         }
      }
      /*Insert the neighbor serv cells information*/
      for(nghCount = 0; nghCount < nghCellCfg->nghEnbCellCfg[numEnbCount].
               nghCellParam.u.nghCellAdd.numNgh; nghCount ++)
      {
         cellAddInfo = &(nghCellCfg->nghEnbCellCfg[numEnbCount].
              nghCellParam.u.nghCellAdd.nghCellAddInfo[nghCount]);
         /*check whether corresponding serving cell already present or not*/
         if(ROK != nlFindNghServingCell(&nghCellCb->servCellLst, 
                  &cellAddInfo->nghCellInfo, &nghServCell))
         {
            NL_ALLOC(NL_REGION, NL_POOL, &nghServCell, sizeof(NlNghServCellCb));
            if(NULLP == nghServCell)
            {
               RLOG0(L_ERROR, "Failed to allocate memory for neighbor cells");
               RETVALUE(RFAILED);
            }
            nlStoreNghCellInfo(cellCb, nghServCell, &cellAddInfo->nghCellInfo, TRUE, alarm);
            nlUpdateNghCellAlarmInfo(nghServCell, alarm);
            nghServCell->lnk.node = (PTR)nghServCell;
            cmLListAdd2Tail(&nghCellCb->servCellLst, &nghServCell->lnk);
            /*There is a possibility of getting neighbor information with same PCI
              SO we should detect PCI confusion for those neighbor*/
            nlNghPciConfusionDetectWithAddNewNghCfg(nghServCell, nghCellCfg, 
                            numEnbCount, nghCount+1, alarm); 
            /*check whether any neighbor's neighbor information needs to be added or not*/
            for(nghNghCount = 0; nghNghCount < cellAddInfo->numNghNgh; nghNghCount ++)
            {
               /*add all the neighbor's neighbor cell information*/
               NL_ALLOC(NL_REGION, NL_POOL, &nghNghCell, sizeof(NlNghNghCellCb));
               if(NULLP == nghNghCell)
               {
                  RLOG0(L_ERROR, "Failed to allocate memory for neighbor cells");
                  RETVALUE(RFAILED);
               }
               nlStoreNghNghCellInfo(nghNghCell, 
                      &cellAddInfo->nghNghCellInfo[nghNghCount], TRUE, alarm);
               nlUpdateNghNghCellAlarmInfo(nghNghCell, alarm);
               nghNghCell->lnk.node = (PTR)nghNghCell;
               cmLListAdd2Tail(&nghServCell->nghCellLst, &nghNghCell->lnk);
               /*There is a possibility of getting neighbor's neighbor information with same PCI
                 SO we should detect PCI confusion for those neighbor*/
               nlNghNghPciConfusionDetectWithAddNewNghCfg(nghNghCell, nghCellCfg,
                            numEnbCount, nghCount, nghNghCount +1, alarm);
            }
         }
         else
         {
            /*update the serving cell info with current values*/
            nlStoreNghCellInfo(cellCb, nghServCell, &cellAddInfo->nghCellInfo, FALSE, alarm);
            nlUpdateNghCellAlarmInfo(nghServCell, alarm);

            /*There is a possibility of getting neighbor information with same PCI
              SO we should detect PCI confusion for those neighbor*/
            nlNghPciConfusionDetectWithAddNewNghCfg(nghServCell, nghCellCfg,
                            numEnbCount, nghCount+1, alarm);

            for(nghNghCount = 0; nghNghCount < cellAddInfo->numNghNgh; nghNghCount ++)
            {
               /*check whether corresponding neighbor's neighbor cell already present or not*/
               if(ROK != nlFindNghNghCell(&nghServCell->nghCellLst,
                         &cellAddInfo->nghNghCellInfo[nghNghCount], &nghNghCell))
               {
                  /*add all the neighbor's neighbor cell information*/
                  NL_ALLOC(NL_REGION, NL_POOL, &nghNghCell, sizeof(NlNghNghCellCb));
                  if(NULLP == nghNghCell)
                  {
                     RLOG0(L_ERROR, "Failed to allocate memory for neighbor cells");
                     RETVALUE(RFAILED);
                  }
                  nlStoreNghNghCellInfo(nghNghCell,
                      &cellAddInfo->nghNghCellInfo[nghNghCount], TRUE, alarm);
                  nlUpdateNghNghCellAlarmInfo(nghNghCell, alarm);
                  nghNghCell->lnk.node = (PTR)nghNghCell;
                  cmLListAdd2Tail(&nghServCell->nghCellLst, &nghNghCell->lnk);
                  /*There is a possibility of getting neighbor's neighbor information with same PCI
                  SO we should detect PCI confusion for those neighbor*/
                  nlNghNghPciConfusionDetectWithAddNewNghCfg(nghNghCell, nghCellCfg,
                            numEnbCount, nghCount, nghNghCount +1, alarm);
               }
               else
               {
                  nlStoreNghNghCellInfo(nghNghCell,
                      &cellAddInfo->nghNghCellInfo[nghNghCount], FALSE, alarm);
                  nlUpdateNghNghCellAlarmInfo(nghNghCell, alarm);
                  /*There is a possibility of getting neighbor's neighbor information with same PCI
                  SO we should detect PCI confusion for those neighbor*/
                  nlNghNghPciConfusionDetectWithAddNewNghCfg(nghNghCell, nghCellCfg,
                            numEnbCount, nghCount, nghNghCount +1, alarm);
               }
            }
         }
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief delete the neighbor's neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlNghNghPciInfoDelete
 *
 *     This function will delete neighbor's neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghNghPciInfoDelete
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm
)
#else
PRIVATE S16 nlNghNghPciInfoDelete(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;
)
#endif
{
   PTR                     prevEntry      = NULLP;
   U32                     nghEnbCount;
   U32                     nghCount;
   NlTmpNghCellDel         *nghDel        = NULLP;
   NlNeighCellCb           *nghCellCb     = NULLP;
   CmLList                 *nghLink       = NULLP;
   CmLListCp               *servCellList  = NULLP;
   CmLListCp               *nghCellList   = NULLP;
   CmLList                 *servLink      = NULLP;
   NlNghServCellCb         *nghServCellCb = NULLP;
   NlNghNghCellCb          *nghNghCellCb  = NULLP;
   Bool                     nghNghDel     = FALSE;

   TRC3(nlNghNghPciInfoDelete);

   for(nghEnbCount = 0; nghEnbCount < nghCellCfg->numNghEnb; nghEnbCount ++)
   {
      nghDel  = &(nghCellCfg->nghEnbCellCfg[nghEnbCount].
               nghCellParam.u.nghCellDel);
      for(nghCount = 0; nghCount < nghDel->numNgh; nghCount ++)
      {
         nghNghDel = FALSE;
         while(ROK == cmHashListGetNext(&(cellCb->neighEnbCb.nghEnbListCp),
            prevEntry, (PTR *) &nghCellCb))
         {
            /*check whether direct neighber PCI information matching or not*/
            servCellList = &(nghCellCb->servCellLst);
            CM_LLIST_FIRST_NODE(servCellList, servLink);
            while(NULLP != servLink)
            {
               nghServCellCb = (NlNghServCellCb *)servLink->node;
               nghCellList = &(nghServCellCb->nghCellLst);
               CM_LLIST_FIRST_NODE(nghCellList, nghLink);
               while(NULLP != nghLink)
               {
                  nghNghCellCb = (NlNghNghCellCb *)nghLink->node;
                  if(FALSE == cmMemcmp((U8 *) &nghDel->ecgi[nghCount], 
                    (U8 *)&nghNghCellCb->nghNghCellInfo.ecgi, sizeof(LnlEcgi)))
                  {
                     NL_FILL_CLEAR_ALARM(nghNghCellCb->nghNghCellInfo, alarm);
                     nlNghNghPeerAlarmClear(nghNghCellCb);
                     cmLListDelFrm(nghCellList, nghLink);
                     NL_FREE(NL_REGION, NL_POOL, nghNghCellCb, sizeof(NlNghNghCellCb)); 
                     nghNghDel = TRUE;
                     break;
                  }
                  nghLink = NULLP;
                  CM_LLIST_NEXT_NODE(nghCellList, nghLink);
               }
               if(TRUE == nghNghDel)
               {
                  break;
               }
               servLink = NULLP;
               CM_LLIST_NEXT_NODE(servCellList, servLink);
            }
            if(TRUE == nghNghDel)
            {
               break;
            }
            prevEntry = (PTR)nghCellCb;
         }
      }  
   }
   RETVALUE(ROK);
}


/**
 * @brief delete the neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlNghPciInfoDelete
 *
 *     This function will delete neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PUBLIC S16 nlNghPciInfoDelete
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC S16 nlNghPciInfoDelete(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                     nghEnbCount;
   U32                     nghCount;
   CmLList                 *link       = NULLP;
   CmLList                 *nghLink    = NULLP;
   NlNeighCellCb           *nghCellCb  = NULLP;
   NlTmpNghCellDel         *nghDel     = NULLP;
   NlNghServCellCb         *nghServCell= NULLP;
   NlNghNghCellCb          *nghNghCell = NULLP;
   U32                     hashKey;
  
   TRC3(nlNghPciInfoDelete);

   for(nghEnbCount = 0; nghEnbCount < nghCellCfg->numNghEnb; nghEnbCount ++)
   {
      hashKey = nghCellCfg->nghEnbCellCfg[nghEnbCount].nghEnbId;
      nghDel  = &(nghCellCfg->nghEnbCellCfg[nghEnbCount].
               nghCellParam.u.nghCellDel);
      if(ROK == cmHashListFind(&(cellCb->neighEnbCb.nghEnbListCp),
                (U8 *)&hashKey, sizeof(hashKey),
                0, (PTR *)&nghCellCb))
      {
         for(nghCount = 0; nghCount < nghDel->numNgh; nghCount ++)
         {
            CM_LLIST_FIRST_NODE(&nghCellCb->servCellLst, link);
            while(NULLP != link)
            {
               nghServCell = (NlNghServCellCb *)link->node;
               if(FALSE == cmMemcmp((U8 *)&nghServCell->servCellInfo.ecgi, 
                   (U8 *)&nghDel->ecgi[nghCount], sizeof(LnlEcgi)))
               {
                  NL_FILL_CLEAR_ALARM(nghServCell->servCellInfo, alarm);
                  nlNghPeerAlarmClear(nghServCell);
                  /*delete all the neighbor's neighbor cell information*/
                  CM_LLIST_FIRST_NODE(&nghServCell->nghCellLst, nghLink);
                  while(NULLP != nghLink)
                  {
                     nghNghCell = (NlNghNghCellCb *)nghLink->node;
                     NL_FILL_CLEAR_ALARM(nghNghCell->nghNghCellInfo, alarm);
                     nlNghNghPeerAlarmClear(nghNghCell);
                     cmLListDelFrm(&nghServCell->nghCellLst, nghLink);
                     NL_FREE(NL_REGION, NL_POOL, nghNghCell, sizeof(NlNghNghCellCb));
                     nghLink = NULLP;
                     CM_LLIST_FIRST_NODE(&nghServCell->nghCellLst, nghLink);
                  }
                  /*check if serv cell plmn matching with neighbor cell then decrement the number of neighbor*/
                  if(FALSE == cmMemcmp((U8 *)&nghServCell->servCellInfo.ecgi.plmn,
                   (U8 *)&cellCb->cellInfo.ecgi.plmn, sizeof(LnlPlmnId)))
                  {
                     cellCb->numNgh --;
                     /*Inform to BRDCM SON*/
                     if(NL_VENDOR_DSON_STATE_CONFIGURED == cellCb->dSonState)
                     {
                        nlReportNumNghToSon(cellCb->numNgh);
                     }                     
                  }
                  cmLListDelFrm(&nghCellCb->servCellLst, link);
                  NL_FREE(NL_REGION, NL_POOL, nghServCell, sizeof(NlNghServCellCb));
                  break;
               }
               link = NULLP;
               CM_LLIST_NEXT_NODE(&nghCellCb->servCellLst, link);
            }
         }
         /*delete the neighbor eNodeB information if no serving cells are present*/
         if(nghCellCb->servCellLst.count == 0)
         {
            cmHashListDelete(&(cellCb->neighEnbCb.nghEnbListCp), (PTR)nghCellCb);
            NL_FREE(NL_REGION, NL_POOL, nghCellCb, sizeof(NlNeighCellCb));
         }
      }
   }
   /*delete all the direct neighbor information if it is present as 
     neighbor's neighbor table*/
   nlNghNghPciInfoDelete(cellCb, nghCellCfg, alarm);
   RETVALUE(ROK);
}

/**
 * @brief update the neighbor's neighborcell Information
 *
 * @details
 *
 *     Function : nlNghNghInfoUpdate
 *
 *     This function will modify neighbor's neighbir
 *     cell config received from SM/eNB-APP in SON neighbor table
 *
 *  @param[in]  nghNghList : neighbor's neighbor cell inormation list
 *  @param[in]  nghModInfo : neighbor information to modify
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghNghInfoUpdate
(
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
)
#else
PRIVATE S16 nlNghNghInfoUpdate(nghNghList, nghModInfo, alarm)
(
CmLListCp             *nghNghList;  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo;  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm;       /* alarm structure*/
)
#endif
{
   U32                     nghNghCount;
   CmLList                 *nghLink    = NULLP;
   NlNghNghCellCb          *nghNghCell = NULLP;

   TRC3(nlNghNghInfoUpdate);
   /*neighbor's neighbor to modify*/
   for(nghNghCount = 0; nghNghCount < nghModInfo->numNghNgh; nghNghCount ++)
   {
      CM_LLIST_FIRST_NODE(nghNghList, nghLink);
      while(NULLP != nghLink)
      {
         nghNghCell = (NlNghNghCellCb *)nghLink->node;
         if(FALSE == cmMemcmp((U8 *)&nghNghCell->nghNghCellInfo.ecgi,
              (U8 *)&nghModInfo->nghNghCellInfo[nghNghCount].ecgi,
              sizeof(LnlEcgi)))
         {
            nlUpdateNghNghCellInfo(nghNghCell,
                   &nghModInfo->nghNghCellInfo[nghNghCount], alarm);
            nlUpdateNghNghCellAlarmInfo(nghNghCell, alarm);
            break;
         }
         nghLink = NULLP;
         CM_LLIST_NEXT_NODE(nghNghList, nghLink);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief delete the neighbor's neighborcell Information
 *
 * @details
 *
 *     Function : nlNghNghInfoDelete
 *
 *     This function will delete neighbor's neighbir
 *     cell config received from SM/eNB-APP in SON neighbor table
 *
 *  @param[in]  nghNghList : neighbor's neighbor cell inormation list
 *  @param[in]  nghModInfo : neighbor information to modify
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghNghInfoDelete
(
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
)
#else
PRIVATE S16 nlNghNghInfoDelete(nghNghList, nghModInfo, alarm)
(
CmLListCp             *nghNghList;  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo;  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm;       /* alarm structure*/
)
#endif
{
   U32                     nghNghCount;
   CmLList                 *nghLink    = NULLP;
   NlNghNghCellCb          *nghNghCell = NULLP;
   Bool                    nghNghDel   = TRUE;
   
   TRC3(nlNghNghInfoDelete);

   /*new neighbor's neighbor to delete*/
   CM_LLIST_FIRST_NODE(nghNghList, nghLink);
   while(NULLP != nghLink)
   {
      nghNghCell = (NlNghNghCellCb *)nghLink->node;
      for(nghNghCount = 0; nghNghCount < nghModInfo->numNghNgh; nghNghCount ++)
      {
         nghNghDel = TRUE;
         if(FALSE == cmMemcmp((U8 *)&nghNghCell->nghNghCellInfo.ecgi,
              (U8 *)&nghModInfo->nghNghCellInfo[nghNghCount].ecgi,
              sizeof(LnlEcgi)))
         {
            nghNghDel = FALSE;
            break;
         }
      }
      if(TRUE == nghNghDel)
      {
         NL_FILL_CLEAR_ALARM(nghNghCell->nghNghCellInfo, alarm);
         nlNghNghPeerAlarmClear(nghNghCell);
         cmLListDelFrm(nghNghList, nghLink);
         NL_FREE(NL_REGION, NL_POOL, nghNghCell, sizeof(NlNghNghCellCb));
      }
      nghLink = NULLP;
      CM_LLIST_NEXT_NODE(nghNghList, nghLink);
   }
   RETVALUE(ROK);
}

/**
 * @brief add the neighbor's neighborcell Information
 *
 * @details
 *
 *     Function : nlNghNghInfoAdd
 *
 *     This function will add neighbor's neighbir
 *     cell config received from SM/eNB-APP in SON neighbor table
 *
 *  @param[in]  nghNghList : neighbor's neighbor cell inormation list
 *  @param[in]  nghModInfo : neighbor information to modify
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghNghInfoAdd
(
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
)
#else
PRIVATE S16 nlNghNghInfoAdd(nghNghList, nghModInfo, alarm)
(
CmLListCp             *nghNghList;  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo;  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm;       /* alarm structure*/
)
#endif
{
   U32                     nghNghCount;
   CmLList                 *nghLink    = NULLP;
   NlNghNghCellCb          *nghNghCell = NULLP;
   Bool                    nghNghAdd   = TRUE;
   TRC3(nlNghNghInfoAdd);

   for(nghNghCount = 0; nghNghCount < nghModInfo->numNghNgh; nghNghCount ++)
   {
      nghNghAdd = TRUE;
      CM_LLIST_FIRST_NODE(nghNghList, nghLink);
      while(NULLP != nghLink)
      {
         nghNghCell = (NlNghNghCellCb *)nghLink->node;
         if(FALSE == cmMemcmp((U8 *)&nghNghCell->nghNghCellInfo.ecgi,
           (U8 *)&nghModInfo->nghNghCellInfo[nghNghCount].ecgi,
           sizeof(LnlEcgi)))
         {
            nghNghAdd = FALSE;
            break;
         }
         nghLink = NULLP;
         CM_LLIST_NEXT_NODE(nghNghList, nghLink);
      }
      if(TRUE == nghNghAdd)
      {
         /*add all the neighbor's neighbor cell information*/
         NL_ALLOC(NL_REGION, NL_POOL, &nghNghCell, sizeof(NlNghNghCellCb));
         if(NULLP == nghNghCell)
         {
            RLOG0(L_ERROR, "Failed to allocate memory for neighbor cells");
            RETVALUE(RFAILED);
         }
         nlStoreNghNghCellInfo(nghNghCell,
             &nghModInfo->nghNghCellInfo[nghNghCount], TRUE, alarm);
         nlUpdateNghNghCellAlarmInfo(nghNghCell, alarm);
         nghNghCell->lnk.node = (PTR)nghNghCell;
         cmLListAdd2Tail(nghNghList, &nghNghCell->lnk);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief modify the neighbor's neighborcell Information 
 *
 * @details
 *
 *     Function : nlNghNghPciInfoModify
 *
 *     This function will insert/modify/delete neighbor's neighbir 
 *     cell config received from SM/eNB-APP in SON neighbor table
 *
 *  @param[in]  nghNghList : neighbor's neighbor cell inormation list
 *  @param[in]  nghModInfo : neighbor information to modify
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlNghNghPciInfoModify
(
CmLListCp             *nghNghList,  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo,  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm        /* alarm structure*/
)
#else
PRIVATE S16 nlNghPciInfoModify(nghNghList, nghModInfo, alarm)
(
CmLListCp             *nghNghList;  /* neighbor's neighbor list*/
NlTmpNghCellModInfo   *nghModInfo;  /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm               *alarm;       /* alarm structure*/
)
#endif
{
   TRC3(nlNghNghPciInfoModify);
   
   /*check whether neighbor's neighbor information must be add/del/modify*/
   if(nghNghList->count == nghModInfo->numNghNgh)
   {
      nlNghNghInfoUpdate(nghNghList, nghModInfo, alarm);
   }
   else if(nghNghList->count > nghModInfo->numNghNgh)
   {
      nlNghNghInfoDelete(nghNghList, nghModInfo, alarm);
   }
   else if(nghNghList->count < nghModInfo->numNghNgh)
   {
      nlNghNghInfoAdd(nghNghList, nghModInfo, alarm);
   }
   RETVALUE(ROK);
}

/**
 * @brief modify the neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlNghPciInfoModify
 *
 *     This function will insert neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlNghPciInfoModify
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC S16 nlNghPciInfoModify(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32                     nghCount;
   U32                     nghEnbCount;
   CmLList                 *link       = NULLP;
   NlNeighCellCb           *nghCellCb  = NULLP;
   NlTmpNghCellModInfo     *nghModInfo = NULLP;
   NlTmpNghCellMod         *nghMod     = NULLP;
   NlNghServCellCb         *nghServCell= NULLP;
   U32                     hashKey;

   TRC3(nlNghPciInfoModify);
   
   for(nghEnbCount = 0; nghEnbCount < nghCellCfg->numNghEnb; nghEnbCount ++)
   {
      hashKey  = nghCellCfg->nghEnbCellCfg[nghEnbCount].nghEnbId;
      nghMod   = &(nghCellCfg->nghEnbCellCfg[nghEnbCount].nghCellParam.u.nghCellMod);
      if(ROK == cmHashListFind(&(cellCb->neighEnbCb.nghEnbListCp),
                (U8 *)&hashKey, sizeof(hashKey),
                0, (PTR *)&nghCellCb))
      {
         /*find the serving neighbor enb cell*/
         for(nghCount = 0; nghCount < nghMod->numNgh; nghCount ++)
         {
            nghModInfo = &(nghMod->nghCellModInfo[nghCount]);
            CM_LLIST_FIRST_NODE(&nghCellCb->servCellLst, link);
            while(NULLP != link)
            {
               nghServCell = (NlNghServCellCb *)link->node;
               if(FALSE == cmMemcmp((U8 *)&nghServCell->servCellInfo.ecgi, 
                  (U8 *)&nghModInfo->oldEcgi, sizeof(LnlEcgi)))
               {
                  nlUpdateNghCellInfo(nghServCell, &nghModInfo->nghCellInfo, alarm);
                  /*PCI confusion detection when duplicate PCI received 
                    in neighbor PCI list modification TODO*/
                  nlUpdateNghCellAlarmInfo(nghServCell, alarm);
                  nlNghNghPciInfoModify(&nghServCell->nghCellLst, nghModInfo, alarm);
                  break;
               }
               link = NULLP;
               CM_LLIST_NEXT_NODE(&nghCellCb->servCellLst, link);
            }
         }
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief modify the neighbor cell Information in SON neighbor table
 *
 * @details
 *
 *     Function : nlNghPciInfoUpdateInNghTbl
 *
 *     This function will insert neighbor cell config received from SM/eNB-APP
 *     in SON neighbor table
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlNghPciInfoUpdateInNghTbl
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC S16 nlNghPciInfoUpdateInNghTbl(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   TRC3(nlNghPciInfoUpdateInNghTbl);
   /*As there is no possiblity of add/del/mod of cells can come one at time
        taking the config type from first enbcellcfg*/
   switch(nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType)
   {
      case NL_NGH_ADD :
      {
         if(ROK != nlNghPciInfoAdd(cellCb, nghCellCfg, alarm))
         {
            RLOG0(L_ERROR,
               "Failed to insertneighbor config into SON NRT table");
            RETVALUE(RFAILED);
         }
         break;
      }
      case NL_NGH_DEL :
      {
         nlNghPciInfoDelete(cellCb, nghCellCfg, alarm);
         break;
      }
      case NL_NGH_MOD :
      {
         nlNghPciInfoModify(cellCb, nghCellCfg, alarm);
         break;
      }
      default :
      {
         RLOG1(L_ERROR, "Invalid ngh cell config type %d received",
             nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief PCI collision resolve for neighbor deletion or modification
 *
 * @details
 *
 *     Function : nlNghPciCollisionResolveWithNghDelOrMod
 *
 *     This function will resolve PCI collision or confusion after 
 *     deleting or modifying neighbor cell 
 *
 *  @param[in]  cellCb     : cell control block
 *  @param[in]  nghCellCfg : neighbor cell configuration
 *  @param[in]  alarm      : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlNghPciCollisionResolveWithNghDelOrMod
(
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
)
#else
PUBLIC S16 nlNghPciCollisionResolveWithNghDelOrMod(cellCb, nghCellCfg, alarm)
(
NlCellCb          *cellCb;     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg; /* neighbor cell configuration*/
NlAlarm           *alarm;      /* alarm structure*/
)
#endif
{
   U32   alarmCount;

   TRC3(nlNghPciCollisionResolveWithNghDelOrMod);

   if((NL_NGH_DEL == nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType) ||
      ((NL_NGH_MOD == nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType) &&
      (TRUE == alarm->resolveDeferred)))
   {
      if(ROK == nlNghPciCollisionResolve(cellCb, nghCellCfg, alarm))
      {
         /*clear all the alarm raised earlier*/
         for(alarmCount = 0; alarmCount < alarm->numAlarmCleared; alarmCount ++)
         {
            nlSendLnlAlarm(&alarm->alarmCleared[alarmCount]);
         }
         if(TRUE == alarm->isPciUpdated)
         {
            NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_PCI_MODIFY);
            cellCb->pciModAlarmInfo.isAlarmSend = TRUE;
         }
         nlSendPciUpdateInd(cellCb);
      }
   }
   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:     bk_pci_sel.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/4 - Mon Jan  5 16:16:12 2015

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

