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
               

     File:     nl_earfcn_sel.c

     Sid:      bk_earfcn_sel.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 19:05:35 2014

     Prg:       

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=246;
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

PRIVATE S16 nlCellSelectEarfcnByDSon ARGS((
NlCellCb      *cellCb,   /* cell control block */
LnlCellCfg    *cellCfg  /* cell config structure  */
));


/**
 * @brief validate the EARFCN configuration received from SM
 *
 * @details
 *
 *     Function : nlValidateCellEarfcnCfg
 *
 *     This function will validate EARFCN configuration
 *     received from SM
 *
 *  @param[in]  cellCfg   : cell configuration parameter structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlValidateCellEarfcnCfg
(
LnlCellCfg    *cellCfg  /* cell config structure  */
)
#else
PUBLIC S16 nlValidateCellEarfcnCfg(cellCfg)
(
LnlCellCfg   *cellCfg;    /* cell config structure */
)
#endif
{
   S16 ret = ROK;
   TRC3(nlValidateCellEarfcnCfg);
   do
   {
      if(cellCfg->numDlEarfcn == 0) 
      {
         RLOG0(L_ERROR, "Empty DL-Earfcn list received");
         ret = RFAILED;
         break;
      }
      if(cellCfg->numUlEarfcn == 0) 
      {
         RLOG0(L_ERROR, "Empty UL-Earfcn list received");
         ret = RFAILED;
         break;
      }
      if(cellCfg->numDlEarfcn != cellCfg->numUlEarfcn)
      {
         RLOG0(L_ERROR, "number of DL-Earfcn and UL-Earfcn mismatched");
         ret = RFAILED;
         break;
      }
   }while(0);

   RETVALUE(ret);
}

/**
 * @brief select a EARFCN from EARFCN List.
 *
 * @details
 *
 *     Function : nlCellSelectEarfcnByDSon
 *
 *     This function selects a unique EARFCN from EARFCN list
 *     received from SM.
 *
 *  @param[in]  cellCb    : cell control block
 *  @param[in]  cellCfg   : cell configuration parameter structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCellSelectEarfcnByDSon
(
NlCellCb      *cellCb,   /* cell control block */
LnlCellCfg    *cellCfg  /* cell config structure  */
)
#else
PRIVATE S16 nlCellSelectEarfcnByDSon(cellCb, cellCfg)
(
NlCellCb     *cellCb;     /* cell control block*/
LnlCellCfg   *cellCfg;    /* cell config structure */
)
#endif
{
   Bool                 earfcnSel = FALSE;
   U32                  earfcnCount;
   U32                  remEarfcnCount;
   NlRemScanEarfcnSel   remScanEarfcnSel[LNL_MAX_NUM_EARFCN];
   NlRemScanEarfcnSel   *dlEarfcnSel = NULLP;
   U8                   earfcnSelCount = 0;
   U8                   count;

   TRC3(nlCellSelectEarfcnByDSon);

   for(earfcnCount = 0; earfcnCount < cellCfg->numDlEarfcn; earfcnCount ++)
   {
      earfcnSel = TRUE;
      for(remEarfcnCount = 0; remEarfcnCount <
                  cellCfg->numRemScanEarfcn; remEarfcnCount ++)
      {
         if(cellCfg->dlEarfcnList[earfcnCount] ==
                     cellCfg->remScanEarfcn[remEarfcnCount].dlEarfcn)
         {
            remScanEarfcnSel[earfcnSelCount].rssi =
                     cellCfg->remScanEarfcn[remEarfcnCount].rssi;
            remScanEarfcnSel[earfcnSelCount].dlEarfcnIdx = earfcnCount;
            earfcnSelCount ++;
            earfcnSel = FALSE;
            break;
         }
      }
      if(FALSE !=earfcnSel)
      {
         cellCb->cellInfo.dlEarfcn = cellCfg->dlEarfcnList[earfcnCount];
         cellCb->cellInfo.ulEarfcn = cellCfg->ulEarfcnList[earfcnCount];
         break;
      }
   }
   if(TRUE != earfcnSel)
   {
      dlEarfcnSel = &remScanEarfcnSel[0];
      /*select the EARFCN from REMSCAN list with least RSSI value*/
      for(count = 1; count < earfcnSelCount; count ++)
      {
         if(dlEarfcnSel->rssi > remScanEarfcnSel[count].rssi)
         {
            dlEarfcnSel = &(remScanEarfcnSel[count]);
         }
      }
      /*Update the DL/UL EARFCN in cell control block*/
      cellCb->cellInfo.dlEarfcn = cellCfg->dlEarfcnList[dlEarfcnSel->dlEarfcnIdx];
      cellCb->cellInfo.ulEarfcn = cellCfg->ulEarfcnList[dlEarfcnSel->dlEarfcnIdx];
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager cell Earfcn Configuration request handler.
 *
 * @details
 *
 *     Function : nlCellSelectEarfcn
 *
 *     This function handles the SON cell Earfcn configuration
 *     request received from the Layer Manager.
 *
 *  @param[in]  cellCb    : cell control block
 *  @param[in]  cellCfg   : cell configuration parameter structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlCellSelectEarfcn
(
NlCellCb      *cellCb,   /* cell control block */
LnlCellCfg    *cellCfg  /* cell config structure  */
)
#else
PUBLIC S16 nlCellSelectEarfcn(cellCb, cellCfg)
(
NlCellCb     *cellCb;     /* cell control block*/
LnlCellCfg   *cellCfg;    /* cell config structure */
)
#endif
{
   TRC3(nlCellSelectEarfcn);

   switch(nlCb.genCfg.earfcnSelType)
   {
      case NL_PCI_SEL_DISABLE :
      {
         cellCb->cellInfo.dlEarfcn = cellCfg->dlEarfcnList[0]; 
         cellCb->cellInfo.ulEarfcn = cellCfg->ulEarfcnList[0];
         RLOG2(L_INFO, "SON selected DL-Earfcn : %d UL-earfcn : %d",
           cellCb->cellInfo.dlEarfcn, cellCb->cellInfo.ulEarfcn);
         /*send selected ERAFCN Indication to SM */
         nlSendEarfcnUpdateInd(cellCb);
      }
      break;
      case NL_PCI_SEL_DSON :
      {
         nlCellSelectEarfcnByDSon(cellCb, cellCfg);
         RLOG2(L_INFO, "SON selected DL-Earfcn : %d UL-earfcn : %d",
           cellCb->cellInfo.dlEarfcn, cellCb->cellInfo.ulEarfcn);
         /*send selected ERAFCN Indication to SM */
         nlSendEarfcnUpdateInd(cellCb);
      }
      break;
      case NL_PCI_SEL_CSON :
      {

      }
      break;
      default :
      break;
   }
   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:     bk_earfcn_sel.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 19:05:35 2014

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

