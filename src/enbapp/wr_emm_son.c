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
               

     File:     wr_emm_son.c

     Sid:      fc_emm_son.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:17:00 2014

     Prg:      subhamay 

*********************************************************************21*/

#include "wr.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_utils.h"
#include "czt.h"
#include "wr_ifm_x2ap.h"
#include "wr_ifm_son.h"
#include "wr_emm_neighenb.h"
#include "wr_emm_mme.h"
#include "nlu.h"
#include "czt.x"
#include "nlu.x"


static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=140;

PRIVATE S16 wrEmmCopyNghEnbCellAddInfo   ARGS((
CztSrvdCells          *nghCellIe, /*cell information received on X2/Config Update*/
NluNghCellCfgParam    *nluNghCell /*cell information to be sent to SON*/
));

PRIVATE S16 wrEmmCopyNghEnbCellModInfo   ARGS((
CztSrvdCellsToMdfy    *nghCellModIe, /*cell information received on Config Update*/
NluNghCellCfgParam    *nluNghCell    /*cell information to be sent to SON*/
));

PRIVATE S16 wrEmmCopyNghEnbCellDelInfo   ARGS((
CztOld_ECGIs          *delCell,   /*cell information received on Config Update*/
NluNghCellCfgParam    *nluNghCell /*cell information to be sent to SON*/
));

/**
 * @brief send neighbor eNB cell information received in X2 setup response
      message to SON
 *
 * @details
 *
 *     Function : wrEmmSendSetupRspNghEnbInfoToSon
 *
 *     This function will inform all the neighbor information received in X2
 *     setup response to SON module for PCI collision/confusion detection
 *
 *  @param[in]  cellCb       : serving cell control block
 *  @param[in]  enbNghCb     : neighbor eNB control block
 *  @param[in]  setupRsp     : X2 setup response message structure
 *
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 wrEmmSendSetupRspNghEnbInfoToSon
(
WrCellCb              *cellCb,       /* cell control block*/
WrNeighEnbCb          *enbNghCb,     /* neighbor eNB control block*/
CztX2SetupResp        *setupRsp      /* X2 setup response message */
)
#else
PUBLIC S16 wrEmmSendSetupRspNghEnbInfoToSon(cellCb, enbNghCb, setupRsp)
(
WrCellCb              *cellCb;       /* cell control block*/
WrNeighEnbCb          *enbNghCb;     /* neighbor eNB control block*/
CztX2SetupResp        *setupRsp;     /* X2 setup response message */
)
#endif
{
   NluNeighborReport               *newNgh = NULLP;
   NluNghCellCfgParam              *nghCellParam = NULLP;
   U8                              idx;
   TRC3(wrEmmSendSetupRspNghEnbInfoToSon);
   CztProtIE_Field_X2SetupResp_IEs *IEs = setupRsp->protocolIEs.member;
   WR_ALLOC(&newNgh, sizeof(NluNeighborReport));
   if(NULLP == newNgh)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   newNgh->msgType = NLU_X2SETUP_RSP_NGH_RPT;
   newNgh->servCellId = cellCb->cellId;
   newNgh->numNghEnb = 1;
   newNgh->nghEnbCellCfg[0].nghEnbId = enbNghCb->enbId;
   nghCellParam = &(newNgh->nghEnbCellCfg[0].nghCellParam);

   for(idx = 0; idx < setupRsp->protocolIEs.noComp.val; idx++)
   {
      CztProtIE_Field_X2SetupResp_IEs *ie = IEs + idx;

      switch(ie->id.val)
      {
         case Cztid_GlobalENB_ID:
            break;
         case Cztid_SrvdCells:
            wrEmmCopyNghEnbCellAddInfo(&ie->value.u.cztSrvdCells, nghCellParam);
            break;
         case Cztid_GUGroupIDLst:
         default :
            break;
      }
   }
   wrIfmSonSendNeighborReq(newNgh);
   RETVALUE(ROK);
}

/**
 * @brief send neighbor eNB cell information received in X2 setup message to SON
 *
 * @details
 *
 *     Function : wrEmmSendSetupReqNghEnbInfoToSon
 *
 *     This function will inform all the neighbor information received in X2 
 *     setup request to SON module for PCI collision/confusion detection
 *
 *  @param[in]  cellCb       : serving cell control block
 *  @param[in]  enbNghCb     : neighbor eNB control block
 *  @param[in]  setupReq     : X2 setup req message structure
 * 
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PUBLIC S16 wrEmmSendSetupReqNghEnbInfoToSon
(
WrCellCb              *cellCb,       /* cell control block*/
WrNeighEnbCb          *enbNghCb,     /* neighbor eNB control block*/
CztX2SetupRqst        *setupReq      /* X2 setup request message */
)
#else
PUBLIC S16 wrEmmSendSetupReqNghEnbInfoToSon(cellCb, enbNghCb, setupReq)
(
WrCellCb              *cellCb;       /* cell control block*/
WrNeighEnbCb          *enbNghCb;     /* neighbor eNB control block*/
CztX2SetupRqst        *setupReq;     /* X2 setup request message */
)
#endif
{ 
   NluNeighborReport               *newNgh = NULLP;
   CztProtIE_Field_X2SetupRqst_IEs *IEs    = NULLP;
   CztProtIE_Field_X2SetupRqst_IEs *ie     = NULLP;
   U8                              idx;
   NluNghCellCfgParam              *nghCellParam = NULLP;
   TRC3(wrEmmSendSetupReqNghEnbInfoToSon);
   WR_ALLOC(&newNgh, sizeof(NluNeighborReport));
   if(NULLP == newNgh)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   newNgh->msgType = NLU_X2SETUP_REQ_NGH_RPT;
   newNgh->servCellId = cellCb->cellId;
   newNgh->numNghEnb = 1;
   newNgh->nghEnbCellCfg[0].nghEnbId = enbNghCb->enbId;
   nghCellParam = &(newNgh->nghEnbCellCfg[0].nghCellParam);

   IEs = setupReq->protocolIEs.member;
   for(idx = 0; idx < setupReq->protocolIEs.noComp.val; idx++)
   {
      ie = IEs + idx;

      switch(ie->id.val)
      {
         case Cztid_GlobalENB_ID:
            /*Neighbor eNB ID got modified need to handle later TODO*/
            break;
         case Cztid_SrvdCells:
            wrEmmCopyNghEnbCellAddInfo(&ie->value.u.cztSrvdCells, nghCellParam);
            break;
         case Cztid_GUGroupIDLst:
         default :
            break;
      }
   }
   /*send the X2 setup request cell information to SON*/
   wrIfmSonSendNeighborReq(newNgh); 
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 wrEmmCopyNghEnbCellDelInfo
(
CztOld_ECGIs          *nghCellDel, /*cell information received on eNB config update*/
NluNghCellCfgParam    *nluNghCell /*cell information to be sent to SON*/
)
#else
PRIVATE S16 wrEmmCopyNghEnbCellDelInfo(nghCellDel, nluNghCell)
(
CztOld_ECGIs          *nghCellDel; /*cell information received on eNB config update*/
NluNghCellCfgParam    *nluNghCell; /*cell information to be sent to SON*/
)
#endif
{
   U8                        noComp = 0;
   U8                        count;
   TRC3(wrEmmCopyNghEnbCellDelInfo);
   noComp = nghCellDel->noComp.val;
   nluNghCell->cfgType = NLU_NGH_DEL;
   nluNghCell->u.nghCellDel.numNgh = noComp;
   for(count = 0; count < noComp; count ++)
   {
      wrEmmNghEnbParseTknStr4Ie(&(nghCellDel->member[count].eUTRANcellIdentifier),
                 &(nluNghCell->u.nghCellDel.ecgi[count].cellId));
      wrEmmNghEnbParsePlmnIe(&(nghCellDel->member[count].pLMN_Identity),
                 (WrPlmnId *)&(nluNghCell->u.nghCellDel.ecgi[count].plmn));
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 wrEmmCopyNghEnbCellModInfo
(
CztSrvdCellsToMdfy    *nghCellMod, /*cell information received on eNB config update*/
NluNghCellCfgParam    *nluNghCell /*cell information to be sent to SON*/
)
#else
PRIVATE S16 wrEmmCopyNghEnbCellModInfo(nghCellMod, nluNghCell)
(
CztSrvdCellsToMdfy    *nghCellMod; /*cell information received on eNB config update*/
NluNghCellCfgParam    *nluNghCell; /*cell information to be sent to SON*/
)
#endif
{
   U8                        numModCells;
   U8                        cellCount;
   U8                        nghNghCount;
   NluNghCellModInfo         *nluNghModInfo = NULLP;
   CztSrvdCell_Inform        *nghSrvdCellInfo =NULLP;
   CztNeighbour_InformMember *srvNeigInfo = NULLP;
   TRC3(wrEmmCopyNghEnbCellModInfo);
   numModCells = nghCellMod->noComp.val;
   nluNghCell->cfgType = NLU_NGH_MOD;
   nluNghCell->u.nghCellMod.numNgh = numModCells;
   for(cellCount = 0; cellCount < numModCells; cellCount ++)
   {
      nluNghModInfo = &(nluNghCell->u.nghCellMod.nghCellModInfo[cellCount]);
      if(nghCellMod->member[cellCount].old_ecgi.pres.pres == TRUE)
      {
         wrEmmNghEnbParseTknStr4Ie(&(nghCellMod->member[cellCount].old_ecgi.
                     eUTRANcellIdentifier), &(nluNghModInfo->oldEcgi.cellId));
         wrEmmNghEnbParsePlmnIe(&(nghCellMod->member[cellCount].old_ecgi.pLMN_Identity),
                                            (WrPlmnId *)&(nluNghModInfo->oldEcgi.plmn));
         /*Handling served CellInfo IE*/
         if(nghCellMod->member[cellCount].servedCellInfo.pres.pres == TRUE)
         {
            nghSrvdCellInfo = &(nghCellMod->member[cellCount].servedCellInfo);
            wrEmmNghEnbParseTknStr4Ie(&(nghSrvdCellInfo->cellId.eUTRANcellIdentifier),
               &(nluNghModInfo->nghCellInfo.ecgi.cellId));
            wrEmmNghEnbParsePlmnIe(&(nghSrvdCellInfo->cellId.pLMN_Identity),
               (WrPlmnId *)&(nluNghModInfo->nghCellInfo.ecgi.plmn));

            nluNghModInfo->nghCellInfo.ulEarfcn =
               nghSrvdCellInfo->eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
            nluNghModInfo->nghCellInfo.dlEarfcn =
               nghSrvdCellInfo->eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;
            nluNghModInfo->nghCellInfo.pci = nghSrvdCellInfo->pCI.val;
         }
         nluNghModInfo->numNghNgh = 
           nghCellMod->member[cellCount].neighbour_Info.noComp.val;
         for(nghNghCount = 0; nghNghCount < nluNghModInfo->
                        numNghNgh; nghNghCount ++)
         {
            srvNeigInfo =
                &(nghCellMod->member[cellCount].neighbour_Info.member[nghNghCount]);
            wrEmmNghEnbParseTknStr4Ie(&(srvNeigInfo->eCGI.eUTRANcellIdentifier),
                     &(nluNghModInfo->nghNghCellInfo[nghNghCount].ecgi.cellId));
            wrEmmNghEnbParsePlmnIe(&(srvNeigInfo->eCGI.pLMN_Identity),
                     (WrPlmnId *)&(nluNghModInfo->nghNghCellInfo[nghNghCount].ecgi.plmn));
            nluNghModInfo->nghNghCellInfo[nghNghCount].pci = srvNeigInfo->pCI.val;
            nluNghModInfo->nghNghCellInfo[nghNghCount].dlEarfcn = 
                        srvNeigInfo->eARFCN.val;
            nluNghModInfo->nghNghCellInfo[nghNghCount].ulEarfcn = 
                        srvNeigInfo->eARFCN.val;
            
         } 
      }
   }
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 wrEmmCopyNghEnbCellAddInfo
(
CztSrvdCells          *nghCellIe, /*cell information received on X2*/
NluNghCellCfgParam    *nluNghCell /*cell information to be sent to SON*/
)
#else
PRIVATE S16 wrEmmCopyNghEnbCellAddInfo(nghCellIe, nluNghCell)
(
CztSrvdCells          *nghCellIe; /*cell information received on X2*/
NluNghCellCfgParam    *nluNghCell; /*cell information to be sent to SON*/
)
#endif
{
   U8                         numSrvdCells;
   U8                         idx;
   U8                         nghIdx;
   CztSrvdCellsMember         *srvdCell = NULLP;
   CztNeighbour_InformMember  *nghCell = NULLP;
   NluNghCellAddInfo          *nluCellAdd = NULLP;
   TRC3(wrEmmCopyNghEnbCellAddInfo);

   numSrvdCells = nghCellIe->noComp.val;
   nluNghCell->cfgType = NLU_NGH_ADD;
   nluNghCell->u.nghCellAdd.numNgh = numSrvdCells;
   for(idx = 0;idx < numSrvdCells;idx++)
   {
      srvdCell = &nghCellIe->member[idx];
      nluCellAdd = &(nluNghCell->u.nghCellAdd.nghCellAddInfo[idx]);
      nluCellAdd->nghCellInfo.pci = srvdCell->servedCellInfo.pCI.val;
      if(EUTRA_MODE_INFO_FDD == srvdCell->servedCellInfo.eUTRA_Mode_Info.choice.val)
      {
         nluCellAdd->nghCellInfo.dlEarfcn = 
            srvdCell->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;
         nluCellAdd->nghCellInfo.ulEarfcn = 
            srvdCell->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
      }
      else if (EUTRA_MODE_INFO_TDD == srvdCell->servedCellInfo.eUTRA_Mode_Info.choice.val)
      {
          nluCellAdd->nghCellInfo.dlEarfcn =
            srvdCell->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN.val;
         nluCellAdd->nghCellInfo.ulEarfcn =
            srvdCell->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN.val;
      }
      wrEmmNghEnbParseTknStr4Ie(&srvdCell->servedCellInfo.
            cellId.eUTRANcellIdentifier, &nluCellAdd->nghCellInfo.ecgi.cellId);
      wrEmmNghEnbParsePlmnIe(&srvdCell->servedCellInfo.cellId.pLMN_Identity,
            (WrPlmnId *)&nluCellAdd->nghCellInfo.ecgi.plmn);
      
      /*copy all the neighbor's neighbor information*/
      nluCellAdd->numNghNgh = srvdCell->neighbour_Info.noComp.val;
      for(nghIdx = 0; nghIdx < srvdCell->neighbour_Info.noComp.val; nghIdx ++)
      {
         nghCell = &(srvdCell->neighbour_Info.member[nghIdx]);
         nluCellAdd->nghNghCellInfo[nghIdx].pci = nghCell->pCI.val;
         nluCellAdd->nghNghCellInfo[nghIdx].dlEarfcn = nghCell->eARFCN.val;
         /*AS UL EARFCN is not part of X2 neighbor's information, 
             so setting ULEARFCN same as DLEARFCN*/
         nluCellAdd->nghNghCellInfo[nghIdx].ulEarfcn = nghCell->eARFCN.val;
         wrEmmNghEnbParseTknStr4Ie(&nghCell->eCGI.eUTRANcellIdentifier,
               &nluCellAdd->nghNghCellInfo[nghIdx].ecgi.cellId);
         wrEmmNghEnbParsePlmnIe(&nghCell->eCGI.pLMN_Identity,
               (WrPlmnId *)&nluCellAdd->nghNghCellInfo[nghIdx].ecgi.plmn);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief send neighbor eNB cell information received in eNB config update
      message to SON
 *
 * @details
 *
 *     Function : wrEmmSendCfgUpdateNghEnbInfoToSon
 *
 *     This function will inform all the neighbor information received in X2
 *     setup response to SON module for PCI collision/confusion detection
 *
 *  @param[in]  cellCb       : serving cell control block
 *  @param[in]  enbNghCb     : neighbor eNB control block
 *  @param[in]  cfgUpdate    : eNB config update request message structure
 *
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 wrEmmSendCfgUpdateNghEnbInfoToSon
(
WrCellCb              *cellCb,       /* cell control block*/   
WrNeighEnbCb          *enbNghCb,     /* neighbor eNB control block*/
CztENBConfigUpd       *cfgUpdate     /* eNB config update request message */
)
#else
PUBLIC S16 wrEmmSendCfgUpdateNghEnbInfoToSon(cellCb, enbNghCb, cfgUpdate)
(
WrCellCb              *cellCb;       /* cell control block*/
WrNeighEnbCb          *enbNghCb;     /* neighbor eNB control block*/
CztENBConfigUpd       *cfgUpdate;    /* eNB config update request message */
)
#endif
{
   NluNeighborReport         *nghUpdate = NULLP;
   NluNghCellCfgParam        *nghCellParam = NULLP;
   U8                        noComp;
   U8                        idx1;
   TRC3(wrEmmSendCfgUpdateNghEnbInfoToSon);
   noComp = cfgUpdate->protocolIEs.noComp.val;
   WR_ALLOC(&nghUpdate, sizeof(NluNeighborReport));
   if(NULLP == nghUpdate)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   nghUpdate->msgType = NLU_ENB_CFG_UPD_NGH_RPT;
   nghUpdate->servCellId = cellCb->cellId;
   nghUpdate->numNghEnb = 1;
   nghUpdate->nghEnbCellCfg[0].nghEnbId = enbNghCb->enbId;
   nghCellParam = &(nghUpdate->nghEnbCellCfg[0].nghCellParam);

   for (idx1 = 0; idx1 < noComp; idx1++)
   {
      switch (cfgUpdate->protocolIEs.member[idx1].id.val)
      {
         case Cztid_SrvdCellsToAdd:
            wrEmmCopyNghEnbCellAddInfo(&(cfgUpdate->protocolIEs.member[idx1].
                             value.u.cztSrvdCells), nghCellParam);
            break;
         case Cztid_SrvdCellsToMdfy:
            wrEmmCopyNghEnbCellModInfo(&(cfgUpdate->protocolIEs.member[idx1].
                             value.u.cztSrvdCellsToMdfy), nghCellParam);
            break;
         case Cztid_SrvdCellsToDelete:
            wrEmmCopyNghEnbCellDelInfo(&(cfgUpdate->protocolIEs.member[idx1].
                             value.u.cztOld_ECGIs), nghCellParam);
            break;
         case Cztid_GUGroupIDToAddLst:
         case Cztid_GUGroupIDToDeleteLst:
         default:
            break;
      }
   }

   wrIfmSonSendNeighborReq(nghUpdate); 
   RETVALUE(ROK);
}

/**
 * @brief send neighbor eNB delete message to SON
 *
 * @details
 *
 *     Function : wrEmmSendNghEnbDelToSon
 *
 *     This function will inform all the neighbor information to be deleted at SON module
 *
 *  @param[in]  cellCb       : serving cell control block
 *  @param[in]  enbNghCb     : neighbor eNB control block
 *
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 wrEmmSendNghEnbDelToSon
(
WrCellCb              *cellCb,       /* cell control block*/
WrNeighEnbCb          *enbNghCb      /* neighbor eNB control block*/
)
#else
PUBLIC S16 wrEmmSendNghEnbDelToSon(cellCb, enbNghCb)
(
WrCellCb              *cellCb;       /* cell control block*/
WrNeighEnbCb          *enbNghCb;     /* neighbor eNB control block*/
)
#endif
{
   NluNeighborReport         *nghDel = NULLP;
   NluNghCellCfgParam        *nghCellParam = NULLP;
   CmLListCp                 *servCellList = NULLP;
   WrEutranNeighCellCb       *servCellNode = NULLP;
   CmLList                   *link = NULLP;
   U32                       count = 0;
   TRC3(wrEmmSendNghEnbDelToSon);

   WR_ALLOC(&nghDel, sizeof(NluNeighborReport));
   if(NULLP == nghDel)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   nghDel->msgType = NLU_ENB_DEL_NGH_RPT;
   nghDel->servCellId = cellCb->cellId;
   nghDel->numNghEnb = 1;
   nghDel->nghEnbCellCfg[0].nghEnbId = enbNghCb->enbId;
   nghCellParam = &(nghDel->nghEnbCellCfg[0].nghCellParam);
   nghCellParam->cfgType = NLU_NGH_DEL;
   /*get all the serving cell identity of the neighbor eNodeB*/
   servCellList = &enbNghCb->srvdCellLst;
   CM_LLIST_FIRST_NODE(servCellList, link);
   while (NULLP != link)
   {
      servCellNode = (WrEutranNeighCellCb *)link->node;
      cmMemcpy((U8 *)&(nghCellParam->u.nghCellDel.ecgi[count]), 
          (U8 *)&(servCellNode->eCgi), sizeof(WrEcgi));
      count ++;
      link = NULLP;
      CM_LLIST_NEXT_NODE(servCellList, link);
   }
   nghCellParam->u.nghCellDel.numNgh = count; 
   
   wrIfmSonSendNeighborReq(nghDel); 
   
   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:     fc_emm_son.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:17:00 2014

*********************************************************************31*/

