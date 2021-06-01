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

     Type:     C include file

     Desc:     This file contains RIM protocol functionality, including
               encoding and decoding of RIM PDUs. 
               

     File:     wr_emm_rim.c

     Sid:      fc_emm_rim.c@@/main/Br_Tenb_Rim_Csfb_Intg/7 - Sun Nov  2 23:17:13 2014

     Prg:      nu 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_FILE_ID=33;
static int RLOG_MODULE_ID=1;

#include "wr.h"
#include "wr_emm.h"
#include "wr_emm_rim.h"
#include "wr_utils.h"
#include "cm_pasn.h"
#include "cm_pasn.x"

PUBLIC WrRimCb               rimCb;
EXTERN CmPAsnElmDef         *szMsgIEDb[][4];
EXTERN CmPAsnElmDef         *szMsg3gDbIe[][2];
PRIVATE S16 wrRimDecRirAppCont ( TknStrOSXL    *rimPdu, 
                                 WrRimEncDecCb *rimDecCb,
                                 S16           *msgIndx );

/** @brief This function Copy source routing address in to
*   destination routing address 
*
*     Function: wrRimCpyRoutInfo 
*
*         Processing steps:
*         -  Copy the routing address from destination to 
*            source based on the routing descriminator 
*
* @param[in]   src : Source Routing Address
* @param[out]  dst : Destination Routing Address
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/

PRIVATE S16 wrRimCpyRoutInfo(WrRimRoutingInfo *dst, WrRimRoutingInfo *src) 
{
   S16 ret = ROK;

   dst->ratChoice = src->ratChoice;
   switch(src->ratChoice)
   {
      case WR_RIM_GERAN_ROUT_DISC:
         {
            dst->t.geranCellId.ci     = src->t.geranCellId.ci;
            dst->t.geranCellId.lac    = src->t.geranCellId.lac;
            dst->t.geranCellId.rac    = src->t.geranCellId.rac;
            dst->t.geranCellId.plmnId = src->t.geranCellId.plmnId;
            break;
         }
      case WR_RIM_UTRA_ROUT_DISC:
         {
            dst->t.rncIdentity.lac    = src->t.rncIdentity.lac;
            dst->t.rncIdentity.rncId  = src->t.rncIdentity.rncId;
            dst->t.rncIdentity.rac    = src->t.rncIdentity.rac;
            dst->t.rncIdentity.plmnId = src->t.rncIdentity.plmnId;
            break;
         }
      case WR_RIM_EUTRA_ROUT_DISC:
         {
            dst->t.eutraIdentity.enbType = src->t.eutraIdentity.enbType;
            dst->t.eutraIdentity.enbId   = src->t.eutraIdentity.enbId;
            dst->t.eutraIdentity.tac     = src->t.eutraIdentity.tac;
            dst->t.eutraIdentity.plmnId  = src->t.eutraIdentity.plmnId;
            break;
         }
      default:
         {
            ret = RFAILED;
            RLOG0(L_ERROR, "wrRimCpyRoutInfo: Invalid RAT");
            break;
         }
   }
   RETVALUE(ret);
}



/** @brief This function is responsible for configuration of RIM Protocol
*
*     Function: wrRimCfgReq
*
*         Processing steps:
*         -  This function updates the rimCb with all the configuration
*            required for the RIM Protocol and initializes association CB
*            Hash list.
*
* @param[in] cfg : WrRimCfg
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PUBLIC S16 wrRimCfgReq
(
 WrRimCfg                    *cfg
)
{
   S16                       ret = RFAILED;

   if ( ( NULLP == cfg->rimStartTmr ) ||
        ( NULLP == cfg->rimStopTmr ))
   {
     RLOG0(L_ERROR, "wrRimCfgReq: rimStartTmr is NULL");
     RETVALUE(RFAILED);
   }
   rimCb.rirTmrVal    = cfg->tmrRirVal;
   rimCb.riTmrVal     = cfg->tmrRiVal;
   rimCb.riaeTmrVal   = cfg->tmrRiaeVal;
   rimCb.maxRetryCnt  = cfg->maxRetryCnt;
   rimCb.rimStartTmr  = cfg->rimStartTmr;
   rimCb.rimStopTmr   = cfg->rimStopTmr;
   /* Copy self Address */
   if ( ROK != wrRimCpyRoutInfo(&(rimCb.selfAddr), &(cfg->selfAddr)))
   {
      RETVALUE(RFAILED);
      RLOG0(L_ERROR, "wrRimCfgReq: wrRimCpyRoutInfo failed");
   }
   /* Initialize the associaition CB has list */
   ret = cmHashListInit(&(rimCb.assocCbHL), WR_RIM_MAX_ASSOCS, 0,
         FALSE, CM_HASH_KEYTYPE_ANY, wrEmmCb.init.region, wrEmmCb.init.pool);
   if ( ROK != ret)
   {
      RLOG0(L_ERROR, "wrRimCfgReq: cmHashListInit failed!!");
   }
   RETVALUE(ret);
}/* end of wrRimCfgReq */

/** @brief This function finds the cellIdx using reporting cellId 
*
*     Function: wrRimGetCellIdxFrmAssoc 
*
*         Processing steps:
*         - Go through the list of cell in the assocCb and 
*         - return the cellIdx if cellId matches else
*         - return failure. 
*
* @param[in]   assocCb   : Association Cb
* @param[in]   rptCellId : Reporting cellId
* @param[out]  cellIdx   : cell Index
* @return  S16
*          -# Success : TRUE
*          -# Failure : FALSE
*/
PRIVATE Bool wrRimGetCellIdxFrmAssoc
(
 WrRimAssocCb         *assocCb, 
 WrRimRptCell         *rptCellId, 
 U8                   *cellIdx
)
{
   U8                        idx;
   WrUtraCellId             *rptUtraCell;
   WrUtraCellId             *utraCell;
   WrGeranCellId            *rptGeranCell;
   WrGeranCellId            *geranCell;

#ifdef WR_RIM_FFP
   /* TODO_FFP: skip validation for EUTRAN as of now,
    * Hence validation required only for UTRA RAT and GERAN */
   if (  WR_RIM_EUTRA_RAT == rptCellId->ratChoice ) 
   {
      *cellIdx = 0;
      RETVALUE(TRUE);
   }
#endif
   for ( idx = 0; idx < WR_RIM_MAX_CELLS; idx++ )
   {
      if( TRUE == assocCb->cellLst[idx].isPres )
      {
         if (rptCellId->ratChoice != assocCb->cellLst[idx].rptCellId.ratChoice)
         {
            continue;
         }
         if ( WR_RIM_UTRA_RAT == rptCellId->ratChoice )
         {
            rptUtraCell = &(rptCellId->t.utraCell);
            utraCell = &(assocCb->cellLst[idx].rptCellId.t.utraCell);
            if( (utraCell->cId   == rptUtraCell->cId)   &&
                (utraCell->rncId == rptUtraCell->rncId) &&
                (wrEmmPlmnPlmnsEqual(&(utraCell->plmnId), &(rptUtraCell->plmnId))))
            {
               *cellIdx = idx;
               RETVALUE(TRUE);
            }
         }
         else /* WR_RIM_GERAN_RAT */
         {
            rptGeranCell = &(rptCellId->t.geranCell);
            geranCell = &(assocCb->cellLst[idx].rptCellId.t.geranCell);
            if(( geranCell->ci  == rptGeranCell->ci)  &&
                  ( geranCell->lac == rptGeranCell->lac) &&
                  ( geranCell->rac == rptGeranCell->rac) &&
                  ( wrEmmPlmnPlmnsEqual(&(geranCell->plmnId), &(rptGeranCell->plmnId))))
            {
               *cellIdx = idx;
               RETVALUE(TRUE);
            }
         }
      }
   }
   RETVALUE(FALSE);
}
      
/** @brief This function validates the inputs of the RIR procedure,
*
*     Function: wrRimValRirProcReqInfo
*
*         Processing steps:
*         -  Build config update PDU and dispatch the message to S1AP
*
* @param[in] rimReqInfo: contains required input parameters to trigger RIR. 
* @param[out] assocCb: RIM association Control Block
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimValRirProcReqInfo
(
  WrRimReqInfo               *rimReqInfo, 
  WrRimAssocCb               **assocCb
)
{
   WrRimAssocHashKey         assocKey;
   WrRimAssocCb             *tmpAssocCb = NULLP;
   S16                       ret;
   U8                        cellIdx;
   Bool                      cellPres = FALSE;
   
  
   if ( rimReqInfo->appId  >= WR_RIM_APP_MAX || 
                          rimReqInfo->appId <= WR_RIM_APP_RESVD ) 
   {
     RLOG1(L_ERROR,"wrRimValRirProcReqInfo: appId is invalid [appId : %d]",
              rimReqInfo->appId);
     RETVALUE(RFAILED);
   }

   if (rimReqInfo->reptType  >= WR_RIM_RIR_RESVD ) 
   {
     RLOG1(L_ERROR,"wrRimValRirProcReqInfo: reptType is invalid [reptType : %d]",
              rimReqInfo->reptType);
     RETVALUE(RFAILED);
   }

   WR_SET_ZERO((U8*)&(assocKey), sizeof(WrRimAssocHashKey));
   assocKey.appId   = rimReqInfo->appId;
   /* Copy source and destination address */
   ret = wrRimCpyRoutInfo(&(assocKey.srcAddr), &(rimReqInfo->srcAddr));
   if ( ROK == ret )
   {
      ret = wrRimCpyRoutInfo(&(assocKey.dstAddr), &(rimReqInfo->dstAddr));
   }
   if ( ret != ROK )
   {
      RLOG0(L_ERROR, "wrRimValRirProcReqInfo: wrRimCpyRoutInfo() failed");
      RETVALUE(RFAILED);
   }
   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey), 0,(PTR *)&tmpAssocCb);
   if ( ROK == ret) 
   {
      /* Check if RIM procedure is already triggered for Reporting cell Id */ 
      if ( TRUE == wrRimGetCellIdxFrmAssoc(tmpAssocCb, &(rimReqInfo->rptCellId), &cellIdx) )
      {
         cellPres = TRUE;
         if ( WR_RIM_RIR_STOP != rimReqInfo->reptType)
         {
            RLOG0(L_ERROR, "wrRimValRirProcReqInfo: \
                  Already an RIR procedure is ongoing");
            RETVALUE(RFAILED);
         }
      }
   }
   /* WR_RIM_RIR_STOP can be triggered for ongoing association */
   if ( (WR_RIM_RIR_STOP == rimReqInfo->reptType) &&
        ((ret != ROK) || (TRUE != cellPres))) 
   {
      RLOG0(L_ERROR, "wrRimValRirProcReqInfo: \
            RIR procedure is NOT ongoing");
      RETVALUE(RFAILED);
   }
   *assocCb = tmpAssocCb;
   RETVALUE(ROK);
}/* end of wrRimValRirProcReqInfo */

/** @brief This function is used to create RIM association
*
*     Function: wrRimCreateAssoc
*
*         Processing steps:
*         -  Creates a new Association with provided Hash key and
*            insert into the hash list 
*
* @param[in] assocCb  : RIM association Control Block 
* @param[in] assocKey : Hash key to interst assoc CB to HL. 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimCreateAssoc
(
 WrRimAssocCb                **assocCb,
 WrRimAssocHashKey           *assocKey
)
{
   S16                       ret = RFAILED;
   U8                        idx;
   WrRimAssocCb              *tmpAssocCb = NULLP;

   if ( rimCb.assocCount >= WR_RIM_MAX_ASSOCS )
   {
      RLOG0(L_ERROR, "wrRimCreateAssoc: rimCb.assocCount reached max");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&tmpAssocCb, sizeof(WrRimAssocCb))
   if ( NULLP == tmpAssocCb  )
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the hash Key */
   tmpAssocCb->hashKey.srcAddr = assocKey->srcAddr;
   tmpAssocCb->hashKey.dstAddr = assocKey->dstAddr;
   tmpAssocCb->hashKey.appId   = assocKey->appId;

   ret = cmHashListInsert(&(rimCb.assocCbHL), (PTR)tmpAssocCb,
                   (U8 *)&(tmpAssocCb->hashKey), (U16)sizeof(WrRimAssocHashKey));
   if (ROK != ret)
   {
      RLOG0(L_ERROR,"wrRimCreateAssoc: Failure in the Hash List Insertion ");
      WR_FREE(tmpAssocCb, sizeof(WrRimAssocCb));
      RETVALUE(RFAILED);
   }

   /* Initialize the association */
   for ( idx = 0; idx < WR_RIM_MAX_CELLS; idx++ ) 
   {
      cmInitTimers(&(tmpAssocCb->cellLst[idx].timer), 1);
   }
   /* Increment the number of assoc */
   rimCb.assocCount++;
   /* Initialize the cell count to 1 */
   tmpAssocCb->cellLst[0].isPres  = TRUE;
   tmpAssocCb->cellLst[0].cellIdx = 0;
   tmpAssocCb->cellLst[0].assocCbPtr = (PTR)tmpAssocCb;
   
   *assocCb = tmpAssocCb;
   RETVALUE(ROK);
}/* end of wrRimCreateAssoc */

/** @brief This function is used for deleting the existing RIM association
*
*     Function: wrRimDelAssoc
*
*         Processing steps:
*         - Stop the timer if running
*         - Delete the retransmission buffer, if it is non NULL
*         - Remove RIM association Cb from HL and free the memory  
*         - Frees the allocated memory 
*
* @param[in] assocCb  : RIM associaiton CB 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDelAssoc
(
 WrRimAssocCb                *assocCb,
 U8                           cellIdx,
 Bool                         delAll
)
{
   S16                       ret = RFAILED;
   WrRimDstCellInfo         *cellInfo;
   Bool                      delAssoc = TRUE;
   U8                        idx;
   
   if ( FALSE == delAll )
   {
      cellInfo = &(assocCb->cellLst[cellIdx]);
      /* Stop the timer if running */
      /* Need to deinit the Timer */
      if (( WR_TMR_RIM_TRIR  == cellInfo->timer.tmrEvnt) ||
            ( WR_TMR_RIM_TRIAE == cellInfo->timer.tmrEvnt))
      {
         RLOG1(L_INFO,"wrRimDelAssoc:Stopping timer :%d",  \
                                cellInfo->timer.tmrEvnt);
         rimCb.rimStopTmr((PTR)cellInfo, cellInfo->timer.tmrEvnt);
      } 
      /* Free the rtxPdu if it is non NULL*/
      if( NULLP != cellInfo->rtxPdu)
      {
         WR_FREEMBUF(cellInfo->rtxPdu);
      }
      /* Reset the cell parameters */
      WR_SET_ZERO((U8*) cellInfo, sizeof(WrRimDstCellInfo));
      /* Initialize the timer, for any future use */
      cmInitTimers(&(cellInfo->timer), 1);
      for ( idx = 0; idx < WR_RIM_MAX_CELLS; idx++)
      {
         if ( TRUE == assocCb->cellLst[idx].isPres )
         {
            delAssoc = FALSE;
            break;
         }
      }
   }
   else
   {
      for ( idx = 0; idx < WR_RIM_MAX_CELLS; idx++ )
      {
         cellInfo = &(assocCb->cellLst[idx]);
         if (( WR_TMR_RIM_TRIR  == cellInfo->timer.tmrEvnt) ||
               ( WR_TMR_RIM_TRIAE == cellInfo->timer.tmrEvnt))
         {
            RLOG1(L_INFO,"wrRimDelAssoc:Stopping timer :%d",  \
                                cellInfo->timer.tmrEvnt);
            rimCb.rimStopTmr((PTR)cellInfo, cellInfo->timer.tmrEvnt);
         } 
         /* Free the rtxPdu if it is non NULL*/
         if( NULLP != cellInfo->rtxPdu)
         {
            WR_FREEMBUF(cellInfo->rtxPdu);
         }
      }
   }

   if ( (TRUE == delAssoc ) || ( TRUE == delAll))
   { 
      ret = cmHashListDelete( &(rimCb.assocCbHL), (PTR) assocCb);
      WR_FREE(assocCb, sizeof(WrRimAssocCb));
      if ( ROK != ret)
      {
         RLOG0(L_ERROR,"wrRimDelAssoc: cmHashListDelete failed");
      }
      /* Decrement the number of assoc */
      rimCb.assocCount--;
   }
   RETVALUE(ret);
}/* end of wrRimDelAssoc */

/** @brief This function Fills the encode control block to
* to encode RIR PDU 
*
*     Function: wrRimFillRirEncCb
*
*         Processing steps:
*         - Fill encode CB by referring association CB. 
*
* @param[in]  assocCb: RIM association CB
* @param[out] encCb: encode control block
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE Void wrRimFillRirEncCb
(
 WrRimEncDecCb               *encCb,
 WrRimAssocCb                *assocCb,
 U8                           cellIdx
)
{

   WR_SET_ZERO((U8*) encCb, sizeof(WrRimEncDecCb));

   /* Fill PDU type */
   encCb->pduType               = WR_RIM_RIR_PDU;  

   /* Fill App Id */
   encCb->appId.pres            = PRSNT_NODEF; 
   encCb->appId.val             = assocCb->hashKey.appId; 
   
   /* Fill Protocol version */
   encCb->version.pres   = PRSNT_NODEF;
   encCb->version.val    = WR_RIM_PROT_VER1; 

   /* Fill RSN */
   encCb->rsn.pres              = PRSNT_NODEF;
   encCb->rsn.val               = assocCb->nextSndRsn++;

   /* Fill PDU indication */
   encCb->pduInd.pres           = PRSNT_NODEF;
   encCb->pduInd.isAckRequested = WR_RIM_NO_ACK_REQD; 
   encCb->pduInd.pduTypExt      = assocCb->cellLst[cellIdx].reptType; 

   /* Fill App Container */
   encCb->rirAppCont.pres          = PRSNT_NODEF;
   encCb->rirAppCont.rptCellId     = assocCb->cellLst[cellIdx].rptCellId; 

   /* Fill Source and Destination address */
   encCb->srcAddr               = assocCb->hashKey.srcAddr;
   encCb->dstAddr               = assocCb->hashKey.dstAddr;

   RETVOID;
}/* end of wrRimFillRirEncCb */
 
/** @brief This function fill the global enodeb Id 
*
*     Function: wrRimFillGbleNBId
*
*         Processing steps:
*         -  Build config update PDU and dispatch the message to S1AP
*
* @param[in]          iePdu: Pointer to S1AP IE PDU.
* @param[out] enbIdentifier: enodeb identifier 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillGbleNBId
(
 SzIePdu                      *iePdu,
 WrEnbIdentifier              *enbIdentifier
 )
{
   U32                       idx     = 0;
   SztGlobal_ENB_ID          *enbIe   = &(iePdu->u.globalEnbId);
   WrPlmnId                  *plmnId  = &(enbIdentifier->plmnId);
   U32                       enbType = enbIdentifier->enbType;
   U32                       enbId   = enbIdentifier->enbId;
   SztPLMNidentity           *plmnIe;

   /* Fill the pres IE */
   wrFillTknU8(&(enbIe->pres), PRSNT_NODEF);

   /* Fill PLMN id */
   plmnIe = &(enbIe->pLMNidentity);
   plmnIe->pres = PRSNT_NODEF;
   plmnIe->len  = WR_PLMNID_IE_LEN;

   if ( ROK != cmGetMem(&(iePdu->memCp), WR_RIM_LEN_PLMN, (Ptr *)&plmnIe->val))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   WR_RIM_PK_PLMN_ARR((plmnIe->val), plmnId, idx);
   wrFillTknU8(&(enbIe->eNB_ID.choice), enbType);

   switch(enbType)
   {
      case ENB_ID_HOMEENB_ID:
         {
            wrFillTknBStr32(&(enbIe->eNB_ID.val.homeENB_ID), 28,
                             (enbId & 0x0fffffff));
         }
         break;
      default:
         {
            wrFillTknBStr32(&(enbIe->eNB_ID.val.macroENB_ID), 20, 
                             (enbId & 0x000fffff));
         }
         break;
   }
   enbIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
} /* end of wrRimFillGbleNBId */

/** @brief This function  encode the enodeb ID using PASN encoder.
*
*     Function: wrRimFillEnbId
*
*         Processing steps:
*         -  Fill The global eNB Id and encode it using PASN encoder. 
*
* @param[in] enbIdentifier: eNB identifier
* @param[out] msgLen      : eNB identifier length
* @param[out] pkEnbId     : Encoded eNB id. 
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillEnbId
(
 WrEnbIdentifier             *enbIdentifier, 
 U8                          *pkEnbId, 
 MsgLen                      *msgLen
)
{

   WrPlmnId                  *plmnId = &(enbIdentifier->plmnId);
   SzIePdu                   *iePdu;
   CmPAsnErr                 asnErr;
   MsgLen                    cnt;
   U32                       len;
   U8                        idx = 0;
   U8                        dbgFlag = TRUE;
   Buffer                    *mBuf = NULLP;

   /* initialize the ASN error code */
   asnErr.errCode = 0;

  /* Use S1AP asn encoding to build global enbId */
    WR_ALLOCEVNT(&(iePdu), sizeof(SzIePdu));
    if( NULLP == iePdu )
    {
        RLOG0(L_FATAL,"Memory allocation failed.");
        RETVALUE(RFAILED);
    }

    if ( ROK != wrRimFillGbleNBId (iePdu, enbIdentifier))  
    {
       RLOG0(L_ERROR,"wrRimFillEnbId: wrRimFillGbleNBId failed!");
       WR_FREEEVNT(iePdu);
       RETVALUE(RFAILED);
    }

    SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &mBuf);
    if(NULLP == mBuf)
    {
       RLOG0(L_FATAL, "Memory allocation failed."); 
       WR_FREEEVNT(iePdu);
       RETVALUE(RFAILED);
    }
    /* TODO: define WR_UMM_S1AP_ENC_GLBL_ENB_ID  */
    if(ROK != cmPAsnEncMsg((Void *)&(iePdu->u.globalEnbId), CM_PASN_USE_MBUF,
                      (Void *)(mBuf), 0, 
                      (CmPAsnElmDef **)
                      szMsgIEDb[WR_UMM_S1AP_ENC_IE_IDX_GLBL_ENB_ID], &asnErr,
                      &len, dbgFlag, FALSE ))
    {
       RLOG1(L_ERROR,"wrRimFillEnbId:Failed To Encode enbId with errcode[%d]",
              asnErr.errCode);
       WR_FREEEVNT(iePdu);
       WR_FREEMBUF(mBuf);
       RETVALUE(RFAILED);
    }

    SFndLenMsg(mBuf, msgLen);
 
    /* Fill TAI */
    /* Fill PLMN */
    WR_RIM_TLV_PK_PLMN_ARR(pkEnbId, plmnId, idx );

    /* Fill TAC */                                                                          
    WR_RIM_PK_U16_ARR(pkEnbId, enbIdentifier->tac, idx);

   /* Fill Global enbId in pkArry */
    SCpyMsgFix(mBuf, 0, *msgLen,(Data *)(&(pkEnbId[idx])), &cnt);


   *msgLen += WR_RIM_LEN_TAI;  /* Add TAI length also */
    /* Free the allocated memory before returning */
    WR_FREEEVNT(iePdu);
    WR_FREEMBUF(mBuf);
    RETVALUE(ROK);
}/* end of wrRimFillEnbId */
 
/** @brief This function Fills the RNC identifier 
*
*     Function: wrRimFillRncId
*
*         Processing steps:
*         - Fill the PLMN ID, lac, rac and rnc Id in to pack array 
*
* @param[in] rncIdentifier: RNC identifier.
* @param[out] pkUtraCell:  Pointer ro pack array.
* @return  Void
*/
PRIVATE Void wrRimFillRncId
(
 WrRncIdentifier              *rncIdentifier,
 U8                           *pkUtraCell
)
{

   WrPlmnId                  *plmnId = &(rncIdentifier->plmnId);
   U8                        idx = 0; 

   /* Fill PLMN */
   WR_RIM_TLV_PK_PLMN_ARR(pkUtraCell, plmnId, idx );

   /* Fill LAC */
   WR_RIM_PK_U16_ARR(pkUtraCell, rncIdentifier->lac, idx );

   /* Fill rac */
   pkUtraCell[idx++] = rncIdentifier->rac; 

   WR_RIM_PK_U16_ARR(pkUtraCell, rncIdentifier->rncId, idx );

   RETVOID;
}/* end of wrRimFillRncId */

/** @brief This function is used to fill the Geran Identifier 
*
*     Function: wrRimFillGeranId
*
*         Processing steps:
*           - Fill PLMN id, lac, rac and cell identifier
*
* @param[in] geranId: Geran Identifier.
* @param[out] pkGeranCellId: Pointer to pack array
* @return  Void
*/
PRIVATE Void  wrRimFillGeranId
(
 WrGeranCellId                *geranId,
 U8                           *pkGeranCellId
)
{
   WrPlmnId                  *plmnId = &(geranId->plmnId);
   U8                        idx = 0; 

   /* Value part of Geran Cell Identifier - as per  48.018 - 11.3.9 */
   /* Fill PLMN */
   WR_RIM_TLV_PK_PLMN_ARR(pkGeranCellId, plmnId, idx );

   /* Fill LAC */
   WR_RIM_PK_U16_ARR(pkGeranCellId, geranId->lac, idx );

   /* Fill rac */
   pkGeranCellId[idx++] = geranId->rac; 

   /* Fill CI */
   WR_RIM_PK_U16_ARR(pkGeranCellId, geranId->ci, idx );

   RETVOID;
}/* end of wrRimFillGeranId */

/** @brief This function is used to fill routing info 
*
*     Function: wrRimFillRoutInfo
*
*         Processing steps:
*         -  Find the routing descriminator type
*         -  Fill enbId or utranId or geranId based on it.
*         -  Fill IEI and LI and the routing addressed prepared to PDU 
*
* @param[in] rimPdu: RIM pdu.
* @param[in] routAddr: Routing Address
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRoutInfo
(
 Buffer                      *rimPdu, 
 WrRimRoutingInfo            *routAddr
 )
{
   S16                       ret = RFAILED;
   U8                        pkRoutAddr[WR_RIM_ROUT_ADD_LEN] = {0};
   U8                        routAddDisc = 0;
   MsgLen                    len = 0;

   switch(routAddr->ratChoice)
   {
      case WR_RIM_EUTRA_RAT:
         {
            ret = wrRimFillEnbId(&(routAddr->t.eutraIdentity), 
                  &pkRoutAddr[0], &len);
            if ( ROK != ret)
            {
               RLOG0(L_ERROR, "wrRimFillRoutInfo:wrRimFillRncId Failed");
               RETVALUE(RFAILED);
            }
            routAddDisc = WR_RIM_EUTRA_ROUT_DISC;
         }
         break; 
      case WR_RIM_UTRA_RAT:
         {
            wrRimFillRncId(&(routAddr->t.rncIdentity), &(pkRoutAddr[0]));
            len = WR_RIM_LEN_RNC_ID; 
            routAddDisc = WR_RIM_UTRA_ROUT_DISC;
         }
         break; 
      case WR_RIM_GERAN_RAT:
         {
            wrRimFillGeranId(&(routAddr->t.geranCellId), &(pkRoutAddr[0]));
            routAddDisc = WR_RIM_GERAN_ROUT_DISC;
            len = WR_RIM_LEN_GERAN_CELL_ID;
         }
         break; 
      default:
         {
            RLOG0(L_ERROR, "wrRimFillRoutInfo: Invalid ratChoice");
            RETVALUE(RFAILED);
         }
   }

   /* Pack 
      1. IEI - RIM Routing Information
      2. LI
      3. Routing descriminator ( 4bit spare + 4 bit Rout disc )
      4. Cell Id */

   WR_RIM_PK_U8(rimPdu, WR_RIM_IEI_ROUT_INFO);
   WR_RIM_PK_LI(rimPdu, (len + 1));  /* len = len + 1 including routing disc */ 
   WR_RIM_PK_U8(rimPdu, routAddDisc);
   WR_RIM_PK_STR(rimPdu, (&(pkRoutAddr[0])), len);

   RETVALUE(ROK);
}/* end of wrRimFillRoutInfo */
 
/** @brief This function is used to fill the UTRAN source cell identifier 
*
*     Function: wrRimFillUtraSrcCellId
*
*         Processing steps:
*         - Fill the utran source cell identifier 
*
* @param[in] iePdu :S1AP PDU structure
* @param[in] utraCell: Utran cell identifier 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillUtraSrcCellId
(
 SzIePdu                      *iePdu, 
 WrUtraCellId                 *utraCell
)
{
   U32                       cellId;
   U8                        idx = 0;
   WrPlmnId                  *plmnId = &(utraCell->plmnId);
   U16                       rncId  = utraCell->rncId;
   U16                       cId    = utraCell->cId;
   SztSrcCellID              *utraCelIdIe = &(iePdu->u.utraSrcCellId);
   SztSrcUTRANCellID         *cellIdIe = &(utraCelIdIe->val.sourceUTRANCellID);
   SztPLMNidentity           *plmnIe;

   /* Fill Choice */
   utraCelIdIe->choice.pres  = PRSNT_NODEF;
   utraCelIdIe->choice.val   = SRCCELLID_SOURCEUTRANCELLID; 

   /* Fill the pres flag */
   wrFillTknU8 (&(cellIdIe->pres), PRSNT_NODEF);

   /* Fill PLMN id */
   plmnIe       = &(cellIdIe->pLMNidentity);
   plmnIe->pres = PRSNT_NODEF;
   plmnIe->len  = WR_PLMNID_IE_LEN;

   if ( ROK != cmGetMem(&(iePdu->memCp), WR_RIM_LEN_PLMN, (Ptr *)&plmnIe->val))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   WR_RIM_PK_PLMN_ARR((plmnIe->val), plmnId, idx);

   /* Fill cell Id */
   cellIdIe->uTRANcellID.pres = PRSNT_NODEF;

   if ( rncId < WR_EXT_RNC_START )
   {
      cellId =  (( 0xFFFF & cId ) | ( rncId << 16 ));
   }
   else
   {
      cellId =  (( 0x0FFF & cId ) | (rncId << 12));
   }
   cellIdIe->uTRANcellID.val = cellId;

   cellIdIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
}/* end of wrRimFillUtraSrcCellId */

/** @brief This function is used to fill the UTRAN source cell identifier 
*
*     Function: wrRimAsnEncUtraCellId 
*
*         Processing steps:
*         - Encodes the utran source cell identifier using PASN 
*
* @param[in] utraCellId: Utran Cell Identifier
* @param[in] utracellIe: Utran cell encode buffer 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimAsnEncUtraCellId
(
WrUtraCellId *utraCellId,
TknStrOSXL   *utracellIe
)
{
   SzIePdu                  *iePdu;
   CmPAsnErr                 asnErr;
   U32                       len = 0;
   MsgLen                    cnt;
   MsgLen                    msgLen;
   U8                        dbgFlag = TRUE;
   Buffer                   *mBuf = NULLP;

   /* initialize the ASN error code */
   asnErr.errCode = 0;

   /* Use S1AP asn encoding to build global enbId */
   WR_ALLOCEVNT(&(iePdu), sizeof(SzIePdu));
   if( NULLP == iePdu )
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   if ( ROK != wrRimFillUtraSrcCellId (iePdu, utraCellId))
   {
      RLOG0(L_ERROR, "wrRimFillUtraCellId: wrRimFillUtraSrcCellId failed!");
      WR_FREEEVNT(iePdu);
      RETVALUE(RFAILED);
   }

   SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &mBuf);
   if( NULLP == mBuf)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREEEVNT(iePdu);
      RETVALUE(RFAILED);
   }
   if(ROK != cmPAsnEncMsg((Void *)&(iePdu->u.utraSrcCellId), CM_PASN_USE_MBUF,
            (Void *)(mBuf), 0 ,
            (CmPAsnElmDef **) 
            szMsg3gDbIe[SZT_REQ_TYPE_ENCDEC_IE_UTRA_SRC_CELL_ID], &asnErr,
            &len, dbgFlag, FALSE ))
   {
      RLOG1(L_ERROR,"wrRimFillUtraCellId:Failed To Encode enbId with errcode[%d]",
            asnErr.errCode);
      WR_FREEEVNT(iePdu);
      WR_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   SFndLenMsg(mBuf, &msgLen);
   utracellIe->pres = PRSNT_NODEF;
   utracellIe->len  = msgLen;
   WR_ALLOC(&(utracellIe->val), utracellIe->len);
   if ( NULLP == utracellIe->val )
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      WR_FREEEVNT(iePdu);
      WR_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   /*  pack the UTRA cellId octet string */
   SCpyMsgFix(mBuf, 0, msgLen,(Data *)(utracellIe->val), &cnt);

   /* Free the allocated memory before returning */
   WR_FREEEVNT(iePdu);
   WR_FREEMBUF(mBuf);
   RETVALUE(ROK);
}
 
/** @brief This function is used to fill RIR Application container 
*
*     Function: wrRimFillRirAppCnt
*
*         Processing steps:
*        -  Fill RIR application container based on the RIM application 
*            identifier.
* @param[in] appId    : RIM application identifier 
* @param[out] appCont : Application container
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/

PRIVATE S16 wrRimFillRirAppCnt
(
 U8                           appId,
 WrRimRirAppCont              *appCont
)
{

   WrRimRptCell              *rptCell   = &(appCont->rptCellId);
   WrUtraCellId              *utraCell  = &(rptCell->t.utraCell);
   TknStrOSXL                *appCntIe  = &(appCont->appContIe);
   TknStrOSXL                 utraCellIe;

   switch (appId)
   {
      /* For SI3, NACC and MBMS RIM application - reporting Cell id is
         Geran cell Id hence, adding in fall through case */
      case WR_RIM_APP_SI3:
      case WR_RIM_APP_MBMS:
      case WR_RIM_APP_NACC:
         WR_ALLOC(&(appCntIe->val), WR_RIM_LEN_GERAN_CELL_ID);
         if( NULLP == appCntIe->val)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         wrRimFillGeranId(&(rptCell->t.geranCell), appCntIe->val);

         appCntIe->len  = WR_RIM_LEN_GERAN_CELL_ID;
         appCntIe->pres = PRSNT_NODEF;
         break;
      case WR_RIM_APP_UTRA_SI:
         {
            if( ROK != wrRimAsnEncUtraCellId(utraCell, &utraCellIe))
            {
               RLOG0(L_ERROR, "wrRimFillRirAppCnt: wrRimAsnEncUtraCellId failed!"); 
               RETVALUE(RFAILED);
            }
            /* Update the app contianer */
            *appCntIe = utraCellIe; 
            break;
         }
      default:
         {
            RLOG0(L_ERROR, "wrRimFillRirAppCnt: Invalid appId");
            RETVALUE(RFAILED);
         }
   }
  RETVALUE(ROK);
}

/** @brief This function is used to fill RI Application container 
*
*     Function: wrRimFillRirAppCnt
*
*         Processing steps:
*        -  Fill RI application container based on the RIM application 
*            identifier.
* @param[in] appId    : RIM application identifier 
* @param[out] appCont : Application container
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/

PRIVATE S16 wrRimFillRiAppCnt
(
 U8                           appId,
 WrRimRiAppCont              *appCont
)
{
   WrRimRptCell              *rptCell   = &(appCont->rptCellId);
   WrUtraCellId              *utraCell  = &(rptCell->t.utraCell);
   TknStrOSXL                 utraCellIe;
   MsgLen                     appContLen;
   WrRimNacc                 *nacc;

   switch (appId)
   {
      case WR_RIM_APP_NACC:
         {
            nacc = &(appCont->app.nacc);
            /* Length of RI application container IE for NACC application will be
             * SUM of Geran cell id + SI length + 1 ( number of SI/PSI and Type)
             */
            appContLen = WR_RIM_LEN_GERAN_CELL_ID + nacc->si.len + 1;
            appCont->appContIe.pres = PRSNT_NODEF;
            appCont->appContIe.len  = appContLen; 

            WR_ALLOC(&(appCont->appContIe.val), appContLen);
            if ( NULLP == appCont->appContIe.val )
            {
               RLOG0(L_FATAL,"Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            /* First 2 bytes for IEI and LI, hence pass &val[2] */
            wrRimFillGeranId(&(rptCell->t.geranCell), appCont->appContIe.val);

            /* Fill number of SI/PSI and Type, at index
             * WR_RIM_LEN_GERAN_CELL_ID */
            appCont->appContIe.val[WR_RIM_LEN_GERAN_CELL_ID] =   \
                            (((nacc->noOfSi) << 1) | ((nacc->siType) & 0x01));
            /* Copy SI, from index WR_RIM_LEN_GERAN_CELL_ID + 1 */
            cmMemcpy((U8*)(&(appCont->appContIe.val
                        [WR_RIM_LEN_GERAN_CELL_ID + 1])),
                        nacc->si.val, nacc->si.len);
            break;
         }
      case WR_RIM_APP_UTRA_SI:
         {
            if( ROK != wrRimAsnEncUtraCellId(utraCell, &utraCellIe))
            {
               RLOG0(L_ERROR, "wrRimFillRiAppCnt: wrRimAsnEncUtraCellId failed!"); 
               RETVALUE(RFAILED);
            }
            /* RI application container for UTRA includes reportCell Id
             * and UTRA SI */
            appContLen = utraCellIe.len + appCont->app.utraSI.len; 

            appCont->appContIe.pres = PRSNT_NODEF;
            appCont->appContIe.len  = appContLen; 
            WR_ALLOC(&(appCont->appContIe.val), appContLen);
            if ( NULLP == appCont->appContIe.val )
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            /* Copy Reporting Cell Id */
            cmMemcpy((appCont->appContIe.val),
                     (utraCellIe.val), utraCellIe.len);

            /* Copy Utra SI */
            cmMemcpy((U8*) &(appCont->appContIe.val[utraCellIe.len]),
                     appCont->app.utraSI.val, appCont->app.utraSI.len);

            /* Free the  utraCellIe */
            WR_FREE(utraCellIe.val, utraCellIe.len );
            break;
         }
      default:
         {
            /* Only NACC and UTRA  application container
             * filling is taken care as of now and rest will
             * be fall in default case */
            RLOG0(L_ERROR, "wrRimFillRiAppCnt: Unknow appId or Filling not \
                  supported");
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
}
/** @brief This function is used to fill the RIR RIM container 
*
*     Function: wrRimFillRirRimCont
*
*         Processing steps:
*          - Find the length of the container based on mandatory,
*            conditional and optional IEs.
*          - Fill the IEs
*
* @param[in] rimEncCb: RIM encode CB
* @param[out] rimPdu: RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRirRimCont
(
 WrRimEncDecCb                *rimEncCb,
 Buffer                       *rimPdu
 )
{
   U16                       rimContLen;  /* Max RIM rimCon len is 2 Byte */
   TknStrOSXL               *appContIe = &(rimEncCb->rirAppCont.appContIe); 
   

   /* Find the RIM container Len: Len of all the mandatory and conditional IEs
    * + length of IE and LI */ 
   rimContLen = WR_RIM_LEN_APP_ID + WR_RIM_LEN_RSN + WR_RIM_LEN_PDU_IND + 6;

   /* Fill the SON continer if present else, Fill application container */
   if ( PRSNT_NODEF != rimEncCb->sonAppId.pres )
   {
      /* Build the Rir App container and increment the length */
      wrRimFillRirAppCnt(rimEncCb->appId.val, &(rimEncCb->rirAppCont));

     /* Including IE and LI */
      rimContLen += rimEncCb->rirAppCont.appContIe.len + 2; 
   }
   else
   {
    /* Including IE and LI */
     rimContLen += rimEncCb->sonAppId.len + 2;
   }

   /* Check if optional IE is included */
   if ( PRSNT_NODEF == rimEncCb->version.pres ) 
   {
      rimContLen += ( WR_RIM_LEN_PROT_VER + 2 );/*including 1 
                                               byte IE and 1 bye LI */
   }
   /* Pack RIM container */
   WR_RIM_PK_U8(rimPdu, WR_RIM_IEI_RIR_CONT);
   WR_RIM_PK_LI(rimPdu, rimContLen);  
   WR_RIM_PK_APP_ID (rimEncCb->appId.val, rimPdu);

   /* Fill RSN number */
   WR_RIM_PK_RSN (rimEncCb->rsn.val, rimPdu); 
   /* Fill PDU indication IE */
   WR_RIM_PK_PDU_IND (rimEncCb->pduInd.isAckRequested, rimEncCb->pduInd.pduTypExt,
                      rimPdu);
   /* Fill Version */
   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      WR_RIM_PK_VER(rimEncCb->version.val, rimPdu);
   }


   /* Fill Son container or application container based on 
    * SON appId availability */

   if ( PRSNT_NODEF != rimEncCb->sonAppId.pres )
   {
      WR_RIM_PK_APP_CONT( appContIe, WR_RIM_IEI_RIR_APP_CONT, rimPdu);
      WR_FREE(rimEncCb->rirAppCont.appContIe.val, \
            rimEncCb->rirAppCont.appContIe.len);

   }
   else
   {
      WR_RIM_PK_SON_APP_ID((&(rimEncCb->sonAppId)), rimPdu );
      WR_FREE(rimEncCb->sonAppId.val, rimEncCb->sonAppId.len);
   }
   RETVALUE(ROK);
}/* end of wrRimFillRirRimCont */
 /** @brief This function fills the RI RIM contaier 
*
*
*     Function: wrRimFillRiRimCont
*
*         Processing steps:
*          - Find the length of the container based on mandatory,
*            conditional and optional IEs.
*          - Fill the IEs
*
* @param[in] rimEncCb: RIM encode CB
* @param[out] rimPdu: RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRiRimCont
(
 WrRimEncDecCb                *rimEncCb,
 Buffer                       *rimPdu
 )
{

   U16                       rimContLen = 0; /* Max RIM rimCon len is 2 Byte */
   TknStrOSXL                *appErrCntIe = &(rimEncCb->appErrCnt.appCntIe);

   /* Calucate the RIM container length. Including mandatory,
    * conditional and optional IE. Also include 2 byte extra for
    * each IE to include IE and LI */
   
   rimContLen = WR_RIM_LEN_APP_ID + WR_RIM_LEN_RSN + WR_RIM_LEN_PDU_IND + 6;

   /* Fill Application error container if present, else
    * fill application container */
   if ( PRSNT_NODEF == appErrCntIe->pres)
   {
      /* Add 2 bytes to length to include IEI and LI */
      rimContLen  += (appErrCntIe->len + WR_RIM_LEN_APP_ERR_CAUSE + 2);
      /* check if length exceeds 127 byte, need to allocated 2 bytes for
       * hence increment the Length by one */ 
      if ( ( appErrCntIe->len + WR_RIM_LEN_APP_ERR_CAUSE ) > 
             WR_RIM_MAX_LEN_ONE_BYTE)
      {
         rimContLen++; /* 2 Bytes used for Length indicator of App Err Cont */
      }
   }
   else
   {
      /* Fill application container and increment the length */
      if( ROK != wrRimFillRiAppCnt(rimEncCb->appId.val, &(rimEncCb->riAppCont)))
      {
         RLOG0(L_ERROR, "wrRimFillRiRimCont: wrRimFillRiAppCnt failed!");
         RETVALUE(RFAILED);
      }
      /* Add 2 bytes to length to include IEI and LI */
      rimContLen  += (rimEncCb->riAppCont.appContIe.len + 2);
      /* check if length exceeds 127 byte, need to allocated 2 bytes for
       * hence increment the Length by one */ 
      if (  rimEncCb->riAppCont.appContIe.len > WR_RIM_MAX_LEN_ONE_BYTE)
      {
         rimContLen++; /* 2 Bytes used for Length indicator of App Err Cont */
      }
   }
   if ( PRSNT_NODEF == rimEncCb->version.pres)
   {
      /*including 1 byte IE and 1 bye LI */
      rimContLen += ( WR_RIM_LEN_PROT_VER + 2 );
   }
   /* Pack RIM container */
   WR_RIM_PK_U8(rimPdu, WR_RIM_IEI_RI_CONT);
   /* Pack Length Indicator */
   WR_RIM_PK_LI(rimPdu, rimContLen);
   /* Fill Application Id */
   WR_RIM_PK_APP_ID ( rimEncCb->appId.val, rimPdu);
   /* Fill RSN number */
   WR_RIM_PK_RSN (rimEncCb->rsn.val, rimPdu); 
   /* Fill PDU indication IE */
   WR_RIM_PK_PDU_IND (rimEncCb->pduInd.isAckRequested, 
                      rimEncCb->pduInd.pduTypExt, rimPdu);
   /* Fill Version if present  */
   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      WR_RIM_PK_VER(rimEncCb->version.val, rimPdu);
   }
   /* Fill Application error container inf present, else
    * fill application container */
   if ( PRSNT_NODEF == appErrCntIe->pres )
   {
      WR_RIM_PK_APP_ERR_CONT(appErrCntIe, rimEncCb->appErrCnt.appCause,
                                                             rimPdu);
      WR_FREE(appErrCntIe->val, appErrCntIe->len);
   }
   else
   {
      WR_RIM_PK_APP_CONT((&(rimEncCb->riAppCont.appContIe)),
                                WR_RIM_IEI_RI_APP_CONT, rimPdu);
      WR_FREE(rimEncCb->riAppCont.appContIe.val, 
              rimEncCb->riAppCont.appContIe.len);
   }
   /* TODO-FFP: Filling of SON application identity */
   RETVALUE(ROK);
}/* end of wrRimFillRiRimCont */

/** @brief This function Fills the RIAE rim container 
*
*     Function: wrRimFillRiaeRimCont
*
*         Processing steps:
*          - Find the length of the container based on mandatory,
*            conditional and optional IEs.
*          - Fill the IEs
*
* @param[in] rimEncCb: RIM encode CB
* @param[out] rimPdu: RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRiaeRimCont
(
 WrRimEncDecCb                *rimEncCb,
 Buffer                       *rimPdu
 )
{

   U16                       rimContLen;  /* Max RIM rimCon len is 2 Byte */
   TknStrOSXL                *appCntIe = &(rimEncCb->appErrCnt.appCntIe);

   if ( ( PRSNT_NODEF != appCntIe->pres) || ( NULLP == appCntIe->val))
   {
      RLOG0(L_ERROR, "wrRimFillRiaeRimCont: Invalid Input");
      RETVALUE(RFAILED);
   }
   
   /* Find the RIM container Len: Len of all the mandatory and conditional IEs 
      included + 8. Adding 8 to include IEI and LI of 4 IEs.
      i.e. IEI = 1 byte and LI = 1 Byte, hence 4 * 2 = 8 */
   
   rimContLen = WR_RIM_LEN_APP_ID + WR_RIM_LEN_RSN + WR_RIM_LEN_PDU_IND + \
                + appCntIe->len + WR_RIM_LEN_APP_ERR_CAUSE + 8; 

   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      rimContLen += (WR_RIM_LEN_PROT_VER + 2);/*including 1 byte IE and 1 byte LI */
   }

   /* check if length exceeds 127 byte, need to allocated 2 bytes for
    *        * hence increment the Length by one */
   if ( appCntIe->len > WR_RIM_MAX_LEN_ONE_BYTE)
   {
      rimContLen++; /* 2 Bytes used for Length indicator of App Err Cont */
   }
   /* Pack RIM container */
   WR_RIM_PK_U8(rimPdu, WR_RIM_IEI_RIAE_CONT);
   WR_RIM_PK_LI(rimPdu, rimContLen);  
   WR_RIM_PK_APP_ID (rimEncCb->appId.val, rimPdu);

   /* Fill RSN number */
   WR_RIM_PK_RSN (rimEncCb->rsn.val, rimPdu); 

   /* Fill PDU indication IE */
   WR_RIM_PK_PDU_IND (rimEncCb->pduInd.isAckRequested, rimEncCb->pduInd.pduTypExt,\
                        rimPdu); 

   /* Fill Version */
   if ( PRSNT_NODEF == rimEncCb->version.pres ) 
   {
      WR_RIM_PK_VER(rimEncCb->version.val, rimPdu);
   }

   /* Pack App Error container container:
   */

   WR_RIM_PK_APP_ERR_CONT(appCntIe, rimEncCb->appErrCnt.appCause,
                                                      rimPdu);
   /* TODO-FFP: Filling of SON application identity */

   WR_FREE(appCntIe->val, appCntIe->len);
   RETVALUE(ROK);
}/* end of wrRimFillRiaeRimCont */

/** @brief This function fills the RI Ack container 
*
*     Function: wrRimFillRiAckRimCont
*
*         Processing steps:
*          - Find the length of the container based on mandatory,
*            conditional and optional IEs.
*          - Fill the IEs
*
* @param[in] rimEncCb: RIM encode CB
* @param[out] rimPdu: RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRiAckRimCont
(
 WrRimEncDecCb                *rimEncCb,
 Buffer                       *rimPdu
)
{
   U16                       rimContLen; /* Max RIM rimCon len is 2 Byte */

   /* Find the RIM container Len: Len of all the mandatory and conditional IEs 
      included + 4. Adding 8 to include IEI and LI of 2 IEs.
      i.e. IEI = 1 byte and LI = 1 Byte, hence 2 * 2 = 4 */

   rimContLen = WR_RIM_LEN_APP_ID + WR_RIM_LEN_RSN + 4;

   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      rimContLen += ( WR_RIM_LEN_PROT_VER + 2 );/*including 1 
                                               byte IE and 1 bye LI */
   }

   /* Pack RIM container */
   WR_RIM_PK_U8(rimPdu, WR_RIM_IEI_RIACK_CONT);
   WR_RIM_PK_LI(rimPdu, rimContLen);
   WR_RIM_PK_APP_ID (rimEncCb->appId.val, rimPdu);

   /* Fill RSN number */
   WR_RIM_PK_RSN (rimEncCb->rsn.val, rimPdu); 

   /* Fill Version */
   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      WR_RIM_PK_VER(rimEncCb->version.val, rimPdu);
   }

   /* TODO-FFP: Filling of SON application identity */
   RETVALUE(ROK);
} /* end of wrRimFillRiAckRimCont */
 
/** @brief This function fills the RI Error RIM container 
*
*     Function: wrRimFillRiErrRimCont
*
*     Processing steps:
*          - Find the length of the container based on mandatory,
*            conditional and optional IEs.
*          - Fill the IEs
*
* @param[in] rimEncCb: RIM encode CB
* @param[out] rimPdu: RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRiErrRimCont
(
 WrRimEncDecCb                *rimEncCb, 
 Buffer                       *rimPdu
)
{
   U16                       rimContLen;   /* Max RIM rimCon len is 2 Byte */
   U16                       pduInErrLen;
   TknStrOSXL               *pduInErr = &(rimEncCb->pduInErr); 


   if ( ( NULLP == pduInErr->val ) || ( PRSNT_NODEF != pduInErr->pres )) 
   {
      RLOG0(L_ERROR, "wrRimFillRiErrRimCont: Invalid Input");
      RETVALUE(RFAILED);
   }

   pduInErrLen = pduInErr->len;
   
   /* Find the RIM container Len: Len of all the mandatory and conditional IEs 
      included + 6. Adding 6 to include IEI and LI of 2 IEs.
      i.e. IEI = 1 byte and LI = 1 Byte, hence 3 * 2 = 6 */
   rimContLen = WR_RIM_LEN_APP_ID + WR_RIM_LEN_RIM_ERR_CAUSE 
                  + pduInErrLen +  6;

   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      rimContLen += ( WR_RIM_LEN_PROT_VER + 2 );/*including 1 
                                               byte IE and 1 bye LI */
   }
   /* check if 2 bytes is used for length */
   if ( pduInErrLen > WR_RIM_MAX_LEN_ONE_BYTE)
   {
     rimContLen++;
   }

   /* Pack RIM container */
   WR_RIM_PK_U8(rimPdu, WR_RIM_IEI_ERR_CONT);
   WR_RIM_PK_LI(rimPdu, rimContLen);

   /* Fill AppId */
   WR_RIM_PK_APP_ID(rimEncCb->appId.val, rimPdu);

   /* Fill cause */
   WR_RIM_PK_CAUSE(rimEncCb->rimCause.val, rimPdu);

   /* Fill Version */
   if ( PRSNT_NODEF == rimEncCb->version.pres) 
   {
      WR_RIM_PK_VER(rimEncCb->version.val, rimPdu);
   }

   /* Fill the pdu in Error  */
   WR_RIM_PK_PDU_IN_ERR(pduInErr, rimPdu);

   /* TODO-FFP: Filling of SON application identity */
   WR_FREE(pduInErr->val, pduInErr->len);
   RETVALUE(ROK);
} /* end of wrRimFillRiErrRimCont */

/** @brief This function encodes the RIM PDU 
*
*     Function: wrRimEncPdu
*
*         Processing steps:
*         -  Encode the PDU Tyoe
*         -  Encode the source and destination identifier
*         -  Encode the RIM container based on PDU Type
*
* @param[in] rimEncCb : RIM encode CB
* @param[out] rimPdu  : RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PUBLIC S16 wrRimEncPdu
(
 WrRimEncDecCb               *rimEncCb,
 Buffer                      *rimPdu
)
{

   S16                       ret;

     /* encode RIM PDU type */
   WR_RIM_PK_U8(rimPdu, rimEncCb->pduType);

   /* Fill the destination cell identifier */
   ret = wrRimFillRoutInfo(rimPdu, &(rimEncCb->dstAddr));  
   /* Fill the src cell identifier */
   if ( ROK == ret )
   {
      ret = wrRimFillRoutInfo(rimPdu, &(rimEncCb->srcAddr));  
   }
   else 
   {
      RLOG0(L_ERROR, "wrRimEncPdu: wrRimFillRoutInfo failed");
      RETVALUE(RFAILED);
   }

   /* Fill  Rim Container */
   switch(rimEncCb->pduType)
   {
      case WR_RIM_RIR_PDU:
         {
            ret =  wrRimFillRirRimCont(rimEncCb, rimPdu);
            if ( ROK != ret)
            {
               RLOG0(L_ERROR, "wrRimEncPdu: wrRimFillRirRimCont failed");
               if ( NULLP != rimEncCb->rirAppCont.appContIe.val )
               {
                  WR_FREE(rimEncCb->rirAppCont.appContIe.val, \
                        rimEncCb->rirAppCont.appContIe.len);
               }
            }
            break;
         }
      case WR_RIM_RI_PDU:
         {
            ret =  wrRimFillRiRimCont(rimEncCb, rimPdu);
            if ( ROK != ret)
            {
               RLOG0(L_ERROR, "wrRimEncPdu: wrRimFillRiRimCont failed");
               if ( NULLP != rimEncCb->appErrCnt.appCntIe.val )
               {
                  WR_FREE(rimEncCb->appErrCnt.appCntIe.val, \
                        rimEncCb->appErrCnt.appCntIe.len);
               }
            }
            break;
         }
      case WR_RIM_RIA_PDU:
         {
            ret =  wrRimFillRiAckRimCont(rimEncCb, rimPdu);
            if( ROK != ret)
            {
               RLOG0(L_ERROR, "wrRimEncPdu: wrRimFillRiAckRimCont failed");
            }
            break;
         }
      case WR_RIM_RIAE_PDU:
         {
            ret =  wrRimFillRiaeRimCont(rimEncCb, rimPdu);
            if ( ROK != ret)
            {
               RLOG0(L_ERROR, "wrRimEncPdu: wrRimFillRiaeRimCont failed");
               if ( NULLP != rimEncCb->appErrCnt.appCntIe.val )
               {
                  WR_FREE(rimEncCb->appErrCnt.appCntIe.val, \
                        rimEncCb->appErrCnt.appCntIe.len);
               }
            }
            break;
         }
      case WR_RIM_RIE_PDU:
         {
            ret =  wrRimFillRiErrRimCont(rimEncCb, rimPdu);
            if ( ROK != ret)
            {
               RLOG0(L_ERROR, "wrRimEncPdu: wrRimFillRiErrRimCont failed");
               if ( NULLP != rimEncCb->pduInErr.val )
               {
                  WR_FREE(rimEncCb->pduInErr.val, rimEncCb->pduInErr.len);
               }
            }
            break;
         }
      default:
         {
            ret = RFAILED;
            RLOG0(L_ERROR, "wrRimEncPdu: Invalid pduType");
            break;
         }
   }
   RETVALUE(ret);
}/* end of wrRimEncPdu */
/** @brief This function is an interface API to RIM application to 
 * trigger RIR procedure
*
*     Function: wrRimRirProcReq
*
*         Processing steps:
*         - Validate the inputs parameters
*         - Allocate the association CB
*         - Allocate the memory for RIM PDU and send it for encoding
*         - Update the association state
*         - Start the timer and update the output buffer. 
*
* @param[in] rimReqInfo:  RIM requence info.
* @param[out] pdu: RIM pdu  
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PUBLIC S16 wrRimRirProcReq
(
 WrRimReqInfo                *rimReqInfo,
 Buffer                      **pdu
)
{
  WrRimEncDecCb              encCb; 
  WrRimAssocCb               *assocCb = NULLP;
  WrRimAssocHashKey          assocKey;
  Buffer                     *rirPdu = NULLP;
  S16                        ret = RFAILED;
  U32                        cellIdx = 0;

  /* Validate the received input */
  ret = wrRimValRirProcReqInfo(rimReqInfo, &assocCb);
  if ( ROK != ret )
  {
     RLOG0(L_ERROR, "wrRimRirProcReq:Validation failed");
     RETVALUE(ret);
  }

  if ( NULLP == assocCb )
  {
     WR_SET_ZERO((U8*)&(assocKey), sizeof(WrRimAssocHashKey));

     assocKey.appId = rimReqInfo->appId;
     /* Copy source and destination address */
     ret = wrRimCpyRoutInfo(&(assocKey.srcAddr), &(rimReqInfo->srcAddr));
     if ( ROK == ret )
     {
        ret = wrRimCpyRoutInfo(&(assocKey.dstAddr), &(rimReqInfo->dstAddr));
     }
     if ( ret != ROK )
     {
        RLOG0(L_ERROR, "wrRimRirProcReq:wrRimCpyRoutInfo() failed");
        RETVALUE(RFAILED);
     }

     ret = wrRimCreateAssoc( &assocCb, &assocKey );
     if ( ROK != ret )
     {
        RLOG0(L_ERROR, "wrRimRirProcReq:Creation of RIM association failed");
        RETVALUE(RFAILED);
     }
  }
  else
  {
     WR_RIM_GET_FREE_CELL_IDX(assocCb, cellIdx);
     if ( WR_RIM_INVALID_CELL_ID == cellIdx )
     {
        RETVALUE(RFAILED);
     }
  }
  /* update the report type and cell Id */
  assocCb->cellLst[cellIdx].reptType  = rimReqInfo->reptType;
  assocCb->cellLst[cellIdx].rptCellId = rimReqInfo->rptCellId;

  /* Fill Rir EncCb and trigger RIR PDU encoding */
  wrRimFillRirEncCb(&encCb, assocCb, cellIdx);

  /* allocated memory for rimPdu */
  SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &rirPdu);
  if ( NULLP == rirPdu )
  {
     RLOG0(L_FATAL, "Memory allocation failed.");
     wrRimDelAssoc(assocCb, cellIdx, FALSE);
     RETVALUE(RFAILED);
  } 
  /* Send for encoding */ 
  ret = wrRimEncPdu(&encCb, rirPdu);
  if ( ROK == ret)
  {
     /* update the RSN in assocCell Cb and preserve a copy of
      * message for retransmission */
     assocCb->cellLst[cellIdx].lastSentRsn = encCb.rsn.val;
     ret = SAddMsgRef(rirPdu, WR_MEM_REGION, WR_MEM_POOL, \
                    &(assocCb->cellLst[cellIdx].rtxPdu));
     if ( ROK != ret)
     {
        WR_FREEMBUF(rirPdu);
        wrRimDelAssoc(assocCb,cellIdx,FALSE);
        RLOG0(L_ERROR, "wrRimRirProcReq:SAddMsgRef Failed!");
        RETVALUE(RFAILED);
     }
     /* Start T-RIR */
     rimCb.rimStartTmr((PTR)&(assocCb->cellLst[cellIdx]), \
                       WR_TMR_RIM_TRIR, rimCb.rirTmrVal); 
     RLOG0(L_INFO, "wrRimRirProcReq: started WR_TMR_RIM_TRIR timer ");

     /* Set the association state */
     switch(rimReqInfo->reptType)
     {
#ifdef WR_RIM_FFP
        case WR_RIM_RIR_SINGLE:
           {
              assocCb->cellLst[cellIdx].state = \
                          WR_RIM_RIR_SINGLE_REP_TRIGGERED;
           }
           break;
#endif
        case WR_RIM_RIR_MULTI:
           {
              assocCb->cellLst[cellIdx].state  = \
                          WR_RIM_RIR_MULTI_REP_TRIGGERED;
           }
           break;
        case WR_RIM_RIR_STOP:
           {
              assocCb->cellLst[cellIdx].state = \
                         WR_RIM_RIR_STOP_TRIGGERED;
           }
           break;
        default:
           {
              WR_FREEMBUF(rirPdu);
              wrRimDelAssoc(assocCb, cellIdx, FALSE);
              RLOG0(L_ERROR, "wrRimRirProcReq:Invalid reptType");
              RETVALUE(RFAILED);
           }
     }
  }
  else
  {
     WR_RIM_ENC_FREE_UNUSED_MEM((&encCb));
     WR_FREEMBUF(rirPdu);
     wrRimDelAssoc(assocCb, cellIdx, FALSE);
     RLOG0(L_ERROR, "wrRimRirProcReq:Encoding of the PDU failed");
     RETVALUE(RFAILED);
  }
  *pdu = rirPdu;
  RETVALUE(ROK);
}/* end of wrRimRirProcReq */

/** @brief This function aborts the onging RIM procedure
*
*     Function: wrRimAbortRirProc
*
*         Processing steps:
*         -  if abortaAll is set to TRUE, then delete all the association 
*         -  Otherwise delete the assciation based on rimReqInfo
*
* @param[in] abortAll: Flag to indicate all the association at once.
* @param[out] rimReqInfo: RIM request Info 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/ 
PUBLIC S16 wrRimAbortRirProc
(
 Bool                        abortAll,
 WrRimReqInfo               *rimReqInfo
)
{
   WrRimAssocCb              *prev = NULLP;
   WrRimAssocCb              *assocCb = NULLP;
   WrRimAssocHashKey          assocKey;
   S16                        ret;
   U8                         cellIdx = 0;
   /* if abortAll flag is TRUE then delete all the ongoing RIM 
    * Associations */
   if (TRUE == abortAll)
   { 
      /* Delete all the association  */
      while (ROK == cmHashListGetNext(&(rimCb.assocCbHL),
               (PTR)prev,
               (PTR *)&assocCb))
      {
         wrRimDelAssoc(assocCb, cellIdx, TRUE);
         prev = NULLP;
         assocCb = NULLP;
      }
   }
   else
   {
      /* Delete the associaition based on the second argument */

      WR_SET_ZERO((U8*)&(assocKey), sizeof(WrRimAssocHashKey));

      assocKey.appId   = rimReqInfo->appId;
      /* Copy source and destination address */
      ret = wrRimCpyRoutInfo(&(assocKey.srcAddr), &(rimReqInfo->srcAddr));
      if ( ROK == ret )
      {
         ret = wrRimCpyRoutInfo(&(assocKey.dstAddr), &(rimReqInfo->dstAddr));
      }
      if ( ret != ROK )
      {
         RLOG0(L_ERROR, "wrRimAbortRirProc: wrRimCpyRoutInfo() failed");
         RETVALUE(RFAILED);
      }

      ret = cmHashListFind(&(rimCb.assocCbHL),
            (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);
      if( (NULLP == assocCb) || (FALSE == wrRimGetCellIdxFrmAssoc \
               (assocCb, &(rimReqInfo->rptCellId), &cellIdx)))
      {
         /* Did we find the Association */
         RLOG0(L_ERROR, "wrRimAbortRirProc:No Association found");
         RETVALUE(RFAILED);
      }    
      wrRimDelAssoc(assocCb, cellIdx, FALSE);
   }
   RETVALUE(ROK);
}/* end of wrRimAbortRirProc */

/** @brief This function Fills the RIAE enc Cb. 
 *
 *     Function: wrRimFillRiaeEncCb
 *
 *         Processing steps:
 *          - Fill the encode cb using assocCb and appErrInfo
 *
 * @param[in] assocCb: RIM association CB
 * @param[in] appErrInfo: Application error info
 * @param[out] encCb: Encode CB.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */ 
PRIVATE Void wrRimFillRiaeEncCb
(
 WrRimEncDecCb               *encCb,
 WrRimAssocCb                *assocCb,
 WrRimAppErr                 *appErrInfo
)
{

   WR_SET_ZERO((U8*) encCb, sizeof(WrRimEncDecCb));

   /* Fill PDU type */
   encCb->pduType               = WR_RIM_RIAE_PDU;  

   /* Fill App Id */
   encCb->appId.pres            = PRSNT_NODEF; 
   encCb->appId.val             = assocCb->hashKey.appId; 
   
   /* Fill Protocol version */
   encCb->version.pres          = PRSNT_NODEF;
   encCb->version.val           = WR_RIM_PROT_VER1; 

   /* Fill RSN */
   encCb->rsn.pres              = PRSNT_NODEF;
   encCb->rsn.val               = assocCb->nextSndRsn++;

   /* Fill PDU indication */
   encCb->pduInd.pres           = PRSNT_NODEF;
   encCb->pduInd.isAckRequested = WR_RIM_ACK_REQD; 

   /* Fill App Error Container */
   encCb->appErrCnt.appCntIe    = appErrInfo->appCont;
   encCb->appErrCnt.appCause    = appErrInfo->cause; 

     /* Fill Source and Destination address */
   encCb->srcAddr               = assocCb->hashKey.srcAddr;
   encCb->dstAddr               = assocCb->hashKey.dstAddr;

   RETVOID;
}/* end of wrRimFillRiaeEncCb */

 /** @brief This function is an interface API to RIM application to trigger RIAE
  * procedure 
 *
 *     Function: wrRimRiaeProcReq
 *
 *         Processing steps:
 *         -  Validate teh input
 *         -  Create RIM association if doesnt exists.
 *         -  Encode the PDU 
 *         -  Start the timer and update the RIM PDU
 *
 * @param[in] appErrInfo: Application error Info.
 * @param[out] pdu: RIM pdu.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */ 
PUBLIC S16 wrRimRiaeProcReq
(
 WrRimAppErr                 *appErrInfo,
 Buffer                      **pdu
 )
{           

   WrRimEncDecCb             encCb; 
   WrRimAssocCb              *assocCb = NULLP;
   WrRimAssocHashKey         assocKey;
   Buffer                    *riaePdu = NULLP;
   S16                       ret = RFAILED;
   U8                        cellIdx = 0;

   /* Validate the APP ID: */ 
   if ( appErrInfo->appId  >= WR_RIM_APP_MAX || 
         appErrInfo->appId <= WR_RIM_APP_RESVD ) 
   {
      RLOG1(L_ERROR,"wrRimRiaeProcReq: appId is invalid [appId : %d]",
            appErrInfo->appId);
      RETVALUE(RFAILED);
   }

   WR_SET_ZERO((U8*)&(assocKey), sizeof(WrRimAssocHashKey));

   assocKey.appId   = appErrInfo->appId;
   /* Copy source and destination address */
   ret = wrRimCpyRoutInfo(&(assocKey.srcAddr), &(appErrInfo->srcAddr));
   if ( ROK == ret )
   {
      ret = wrRimCpyRoutInfo(&(assocKey.dstAddr), &(appErrInfo->dstAddr));
   }
   if ( ret != ROK )
   {
      RLOG0(L_ERROR, "wrRimRiaeProcReq: wrRimCpyRoutInfo() failed");
      RETVALUE(RFAILED);
   }
   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);
   if ( NULLP == assocCb ) 
   {
      /* In case of single Report we do not have Association so 
       * we have to create the Association Cb */
      ret = wrRimCreateAssoc( &assocCb, &assocKey);
      if ( ROK != ret )
      {
         RLOG0(L_ERROR, "wrRimRirProcReq:Creation of RIM association failed");
         RETVALUE(RFAILED);
      }
   }
   else if ( FALSE == wrRimGetCellIdxFrmAssoc(assocCb, &(appErrInfo->rptCellId), &cellIdx))
   {
      /* get free cell Index */
     WR_RIM_GET_FREE_CELL_IDX(assocCb, cellIdx);
     if ( WR_RIM_INVALID_CELL_ID == cellIdx )
     {
        RETVALUE(RFAILED);
     }
   }
   wrRimFillRiaeEncCb(&encCb, assocCb, appErrInfo);

   /* allocated memory for rimPdu */
   SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &riaePdu);
   if ( NULLP == riaePdu )
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrRimDelAssoc(assocCb, cellIdx, FALSE);
      RETVALUE(RFAILED);
   } 
   /* Send for encoding */ 
   ret = wrRimEncPdu(&encCb, riaePdu);
   if ( ROK != ret) 
   {
      wrRimDelAssoc(assocCb, cellIdx, FALSE);
      WR_FREEMBUF(riaePdu);
      RLOG0(L_ERROR, "wrRimRirProcReq:Encoding of the PDU failed");
      RETVALUE(RFAILED);
   }
   else
   {
      /* Store the RSN in to cellInfo, to validate the received ACK */ 
      assocCb->cellLst[cellIdx].lastSentRsn = encCb.rsn.val;
      /* preserve the copy of message for re-transmission purpose */
      ret = SAddMsgRef(riaePdu, WR_MEM_REGION, WR_MEM_POOL, 
                       &(assocCb->cellLst[cellIdx].rtxPdu));
      if ( ROK != ret)
      {
         wrRimDelAssoc(assocCb, cellIdx, FALSE);
         WR_FREEMBUF(riaePdu);
         RLOG0(L_ERROR, "wrRimRirProcReq:SAddMsgRef Failed!");
         RETVALUE(RFAILED);
      }
      /* Start T-RIAE */
      rimCb.rimStartTmr((PTR)&(assocCb->cellLst[cellIdx]), WR_TMR_RIM_TRIAE, \
                                                          rimCb.riaeTmrVal);
     RLOG0(L_INFO, "wrRimRiaeProcReq: started WR_TMR_RIM_TRIAE timer");
      /* Set the association state */
      assocCb->cellLst[cellIdx].state = WR_RIM_RIAE_TRIGGERED;
   }
   *pdu = riaePdu;
   RETVALUE(ROK);
}/* end of wrRimRiaeProcReq */

/** @brief This function Fills the RI encode Cb 
 *
 *     Function: wrRimFillRiEncCb
 *
 *         Processing steps:
 *         - Fill the encode CB 
 *
 * @param[in] assocCb    : RIM association CB.
 * @param[in] appErrInfo : Application error info.
 * @param[out] encCb     : encode CB 
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */ 
PRIVATE Void wrRimFillRiEncCb
(
 WrRimEncDecCb               *encCb, 
 WrRimAssocCb                *assocCb, 
 WrRimAppErr                 *appErrInfo,
 U8                           reptType
)
{

   WR_SET_ZERO((U8*) encCb, sizeof(WrRimEncDecCb));

   /* Fill PDU type */
   encCb->pduType                 = WR_RIM_RI_PDU;  

   /* Fill App Id */
   encCb->appId.pres              = PRSNT_NODEF; 
   encCb->appId.val              = assocCb->hashKey.appId; 

   /* Fill Protocol version */
   encCb->version.pres            = PRSNT_NODEF;
   encCb->version.val             = WR_RIM_PROT_VER1; 

   /* Fill RSN */
   encCb->rsn.pres                = PRSNT_NODEF;
   encCb->rsn.val                 = assocCb->nextSndRsn++;

   /* Fill PDU indication */
   encCb->pduInd.pres             = PRSNT_NODEF;
   encCb->pduInd.isAckRequested   = WR_RIM_NO_ACK_REQD; 
   encCb->pduInd.pduTypExt        = reptType; 

   /* Fill RI with App Error Container */
   encCb->appErrCnt.appCntIe      = appErrInfo->appCont;
   encCb->appErrCnt.appCause      = appErrInfo->cause; 

      /* Fill Source and Destination address */
   encCb->srcAddr                 = assocCb->hashKey.srcAddr;
   encCb->dstAddr                 = assocCb->hashKey.dstAddr;

   RETVOID;
}/* end of wrRimFillRiEncCb */

/** @brief This function is an inter API to RIM application to trigger
 *  RI with application container.
 *
 *     Function: wrRimRiErrReq
 *
 *         Processing steps:
 *         - Validate the input and return failure if invalid 
 *         - Enocde the RI PDU
 *         - Update the PDU 
 *
 * @param[in] appErrInfo: Application error info.
 * @param[out] pdu: RIM pdu
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */ 
PUBLIC S16 wrRimRiErrReq
(
 WrRimAppErr                 *appErrInfo, 
 Buffer                      **pdu
)
{           
   WrRimEncDecCb             encCb; 
   WrRimAssocCb             *assocCb = NULLP;
   WrRimAssocHashKey         assocKey;
   Buffer                   *riPdu = NULLP;
   S16                       ret = RFAILED;
   U8                        cellIdx;
   U8                        reptType;

   /* Validate the APP ID: TODO - validation can be done for errCont  */ 
   if ( ( appErrInfo->appId  >= WR_RIM_APP_MAX) || 
         ( appErrInfo->appId <= WR_RIM_APP_RESVD )) 
   {
      RLOG1(L_ERROR,":wrRimRiErrReq appId is invalid [appId : %d]",
            appErrInfo->appId);
      RETVALUE(RFAILED);
   }

   WR_SET_ZERO((U8*)&(assocKey), sizeof(WrRimAssocHashKey));

   assocKey.appId   = appErrInfo->appId;
   /* Copy source and destination address */
   ret = wrRimCpyRoutInfo(&(assocKey.srcAddr), &(appErrInfo->srcAddr));
   if ( ROK == ret )
   {
      ret = wrRimCpyRoutInfo(&(assocKey.dstAddr), &(appErrInfo->dstAddr));
   }
   if ( ret != ROK )
   {
      RLOG0(L_ERROR, "wrRimRiaeProcReq: wrRimCpyRoutInfo() failed");
      RETVALUE(RFAILED);
   }

   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);
   if( (NULLP == assocCb) || (FALSE == wrRimGetCellIdxFrmAssoc(assocCb, \
               &(appErrInfo->rptCellId), &cellIdx)))
   {
      RLOG0(L_ERROR, "wrRimRiErrReq:RIM association is not found");
      RETVALUE(RFAILED);
   }
   reptType = assocCb->cellLst[cellIdx].reptType;
   wrRimFillRiEncCb(&encCb, assocCb, appErrInfo, reptType);

   /* allocated memory for rimPdu */
   SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &riPdu);
   if ( NULLP == riPdu )
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } 

   /* Send for encoding */ 
   ret = wrRimEncPdu(&encCb, riPdu);
   if ( ROK != ret) 
   {
      WR_FREEMBUF(riPdu);
      RLOG0(L_ERROR, "wrRimRiErrReq:Encoding of the PDU failed");
   }
   else 
   {
       assocCb->cellLst[cellIdx].lastSentRsn = encCb.rsn.val;
      *pdu = riPdu;
   }

   wrRimDelAssoc(assocCb, cellIdx, FALSE);
   RETVALUE(ret);
}/* end of wrRimRiErrReq */ 

/** @brief This function is used for deleting all the RIM associations
*
*     Function: wrRimShutdown
*
*         Processing steps:
*         -  Traverse through all the nodes of the hashlist
*         -  Stop the RIM timers if running
*         -  Delete the Association
*         -  Deinitialise the Hashlist in rimCb
*
* @param[in] Void
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PUBLIC S16 wrRimShutdown
(
 Void
 ) 
{
   WrRimAssocCb              *prev    = NULLP;
   WrRimAssocCb              *assocCb = NULLP;
   U8                         cellIdx = 0;

   /* Delete all the association one after another */
   while (ROK == cmHashListGetNext(&(rimCb.assocCbHL),
              (PTR)prev, (PTR *)&assocCb))
   {
      wrRimDelAssoc(assocCb, cellIdx, TRUE);
      prev = NULLP;
      assocCb = NULLP;
   }
   /* DeInit the hashlists in rimCb */
   cmHashListDeinit(&(rimCb.assocCbHL));
   RETVALUE(ROK);
} /* end of wrRimShutdown */
 
/** @brief This function handles the time out of any RIM timer 
*
*     Function: wrRimTimoutHdl
*
*         Processing steps:
*         -  Trigger Re-transmission of the PDU if max retransmission 
*             is not reached.
*         - Send indication if maximum retransmission is reached. 
*
* @param[in] rimTO         : RIM time out 
* @param[out] timeOutAction: Time out action 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PUBLIC S16 wrRimTimoutHdl
(
 WrRimTimeOut                *rimTO,
 WrRimToAction               *timeOutAction
 )
{
   S16                       ret;
   U32                       tmrVal;
   WrRimDstCellInfo         *assocCellCb = (WrRimDstCellInfo*) rimTO->cb;
   WrRimAssocCb             *assocCb = (WrRimAssocCb *)assocCellCb->assocCbPtr;
 

   RLOG1(L_INFO,"wrRimTimoutHdl:TMR:%d expired", rimTO->event);
   WR_SET_ZERO((U8*) timeOutAction, sizeof(WrRimToAction));
   /* Check whether Maximum number of transmission 
    * has been reached or not */
   if(assocCellCb->rtxCount < rimCb.maxRetryCnt)
   {
      /* As the maximum transmission is not reached yet 
       * retransmit the PDU again and increment the maxRtxCnt */ 
      timeOutAction->isMaxRtxReached = FALSE;
      ret = SAddMsgRef(assocCellCb->rtxPdu, WR_MEM_REGION, WR_MEM_POOL,
            &(timeOutAction->rtxPdu));
      if ( ROK != ret)
      {
         wrRimDelAssoc(assocCb,assocCellCb->cellIdx, FALSE);
         RLOG0(L_ERROR, "wrRimRirProcReq:SAddMsgRef Failed!");
         RETVALUE(RFAILED);
      }  
      timeOutAction->dstAddr = assocCb->hashKey.dstAddr;
      assocCellCb->rtxCount++;
      tmrVal = (( WR_TMR_RIM_TRIR == rimTO->event)?rimCb.rirTmrVal:
                                                   rimCb.riaeTmrVal);  
      rimCb.rimStartTmr((PTR)assocCellCb, rimTO->event, tmrVal); 
      RLOG1(L_INFO,"wrRimTimoutHdl:Restarted timer :%d", rimTO->event);
   }
   else
   { 
     /* As the maximum transmission is reached 
      * set the isMaxRtxReached to TRUE  and delete the 
      * association in RIM Protocol */
      timeOutAction->isMaxRtxReached = TRUE;
      timeOutAction->appId = assocCb->hashKey.appId;
      timeOutAction->rptCellId = assocCellCb->rptCellId;
      wrRimDelAssoc(assocCb,assocCellCb->cellIdx, FALSE);
   }
   RETVALUE(ROK);
}/* end of wrRimTimoutHdl */

/** @brief This function fills the RI Error encode CB 
*
*     Function: wrRimFillRiErrEncCb
*
*         Processing steps:
*         -  Fill the RI error encodeCB 
*
* @param[in] decCb: Decode CB, which contains all the decoded parameters.
* @param[in] encCb: Encode CB - ri error 
* @param[in] rimPdu: erroneous RIM PDU
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimFillRiErrEncCb
(
 WrRimEncDecCb               *encCb,
 WrRimEncDecCb               *decCb,
 TknStrOSXL                  *rimPdu
)
{

    if (( NULLP == rimPdu) || ( NULLP  == rimPdu->val) || 
        ( PRSNT_NODEF != decCb->appId.pres )           ||
        ( PRSNT_NODEF != decCb->decFailCause.pres ))
    {
       RLOG0(L_ERROR, "wrRimFillRiErrEncCb: Invalid Input");
       RETVALUE(RFAILED);
    }
    else if ( WR_RIM_RIE_PDU == decCb->pduType) 
    {
       /* No need to send Error PDU for received Error PDU */
       RLOG0(L_INFO, "wrRimFillRiErrEncCb: no need to send RIE for RIE PDU ");
       RETVALUE(RFAILED);
    }
    
    encCb->pduType      = WR_RIM_RIE_PDU; 
    encCb->appId        = decCb->appId;

    encCb->version.pres = PRSNT_NODEF;
    encCb->version.val  = WR_RIM_PROT_VER1;

    encCb->rimCause     = decCb->decFailCause;
    /* Fill the pduInErr */
    encCb->pduInErr.pres = PRSNT_NODEF;
    encCb->pduInErr.len = rimPdu->len;
    WR_ALLOC(&(encCb->pduInErr.val), rimPdu->len);
    if ( NULLP == encCb->pduInErr.val )
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       RETVALUE(RFAILED);
    }
    cmMemcpy((encCb->pduInErr.val), rimPdu->val, rimPdu->len); 

     /* Fill Source and Destination address: Mirror the received
     * source and destination address */
    encCb->srcAddr      = decCb->dstAddr;
    encCb->dstAddr      = decCb->srcAddr;

    RETVALUE(ROK);   
} /* end of wrRimFillRiErrEncCb */
 
/** @brief This function fills RI ACK encodeCB. 
*
*     Function: wrRimFillRiAckEncCb
*
*         Processing steps:
*         -  Fill RI ACK encode CB  using decCb.
*
* @param[in] decCb : Deocde Cb
* @param[out] encCb: Encode CB to be filled.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE Void wrRimFillRiAckEncCb
(
 WrRimEncDecCb           *encCb,
 WrRimEncDecCb           *decCb
 )
{

    encCb->pduType      = WR_RIM_RIA_PDU; 
    encCb->appId        = decCb->appId;

    encCb->version.pres = PRSNT_NODEF;
    encCb->version.val  = WR_RIM_PROT_VER1;

    encCb->rsn          = decCb->rsn;
    /* Mirror the Source and destination address received */
    encCb->srcAddr      = decCb->dstAddr;
    encCb->dstAddr      = decCb->srcAddr;

    RETVOID;   
}/* end of wrRimFillRiAckEncCb */

/** @brief This function validate the received RI and update the assoc State. 
*
*     Function: wrRimRiValAndUpdtState
*
*         Processing steps:
*         -  Validate the RI based on PDU extention and present association
*            state. 
*         - Fill semantic error and return failure in case of error.
*         - Update association state  
*
* @param[in] assocCb: Association CB.
* @param[in] decCb  : Decode Cb
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimRiValAndUpdtState
(
 WrRimDstCellInfo            *assocCellCb, 
 WrRimEncDecCb               *decCb
)
{
   S16                       ret = ROK;
   Bool                      isSemErr = FALSE;

   switch(assocCellCb->state)
   {
      case WR_RIM_RIR_STOP_TRIGGERED:
         {
            if ( WR_RIM_RI_STOP != decCb->pduInd.pduTypExt)
            {
               ret = RFAILED;
            }
            /* else - No need to update the state as the assocCb 
             * will be deleted */
            break;
         }
      case WR_RIM_RIAE_TRIGGERED:
         {
            /* Discard the RI if STOP/RIAE triggered */
            ret = RFAILED;
            /* No need to update the state as the PDU will be discarded */
            break;
         }
#ifdef WR_RIM_FPP
      case WR_RIM_RIR_SINGLE_REP_TRIGGERED:
         {
            if ( WR_RIM_RI_SINGLE != decCb->pduInd.pduTypExt)
            {
               /* expecting WR_RIM_RI_SINGLE, send semantic error for any
                * other PDU ext  */ 
               ret      = RFAILED;
               isSemErr = TRUE;
            }
            else
            {
               /* Update teh associaition state */
               assocCellCb->state = WR_RIM_RI_SINGLE_REP_RCVD; 
            }
            break;
         }
#endif
      case WR_RIM_RIR_MULTI_REP_TRIGGERED:
         {
            if ( WR_RIM_RI_INIT_MULTI != decCb->pduInd.pduTypExt) 
            {
               /* expecting WR_RIM_RI_INIT_MULTI report, send Semantic
                * error if anything else received */
               ret      = RFAILED;
               isSemErr = TRUE;
            }
            else
            {
               /* Update teh associaition state */
               assocCellCb->state = WR_RIM_RI_MULTI_INIT_REP_RCVD; 
            }
            break;
         }
      case WR_RIM_RI_MULTI_INIT_REP_RCVD:
         {
            if (( WR_RIM_RI_SINGLE == decCb->pduInd.pduTypExt) ||
                  ( WR_RIM_RI_STOP   == decCb->pduInd.pduTypExt)) 
            {
               /* expecting WR_RIM_RI_MULTI report  */
               ret = RFAILED;
               isSemErr = TRUE;
            }
            else if ( WR_RIM_RI_INIT_MULTI == decCb->pduInd.pduTypExt)
            {
               /* Duplicate PDU, discarding it */
               ret = RFAILED;
            }
            else 
            {
               /* update the assoc state */
               assocCellCb->state = WR_RIM_RI_MULTI_REP_RCVD; 
            }
            break;
         }
      case WR_RIM_RI_MULTI_REP_RCVD:
         {
            if (( WR_RIM_RI_SINGLE     == decCb->pduInd.pduTypExt) ||
                  ( WR_RIM_RI_INIT_MULTI == decCb->pduInd.pduTypExt) || 
                  ( WR_RIM_RI_STOP       == decCb->pduInd.pduTypExt)) 
            {
               /* expecting WR_RIM_RI_MULTI report  */
               ret = RFAILED;
               isSemErr = TRUE;
            }
            break;
         }
         /* No need to update the state, it will remain as is */
      default:
         {
            ret = RFAILED;
            RLOG0(L_ERROR, "wrRimRiValAndUpdtState:Invalid state");
         }
   }

   if ( ( ROK != ret) && (TRUE == isSemErr) )
   {
      WR_RIM_FILL_RIM_CAUSE((&(decCb->decFailCause)), \
            WR_RIM_SEMANT_ERR );
   }
   RETVALUE(ret);
} /* wrRimRiValAndUpdtState */
/** @brief This function validates the RSN 
*
*     Function: wrRimValRcvdRiRsn
*
*         Processing steps:
*         -  validate the last received RSN with the received RSN, 
*            and return failure if outside the window and return ROK
*            if valid. 
*
* @param[in] lastRsnRcvd: Last received RSN.
* @param[in] newRsn     : Recived RSN 
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimValRcvdRiRsn
(
 U32                         lastRsnRcvd, 
 U32                         newRsn 
 )
{

   if ( newRsn >= lastRsnRcvd )
   {
      if ( (newRsn - lastRsnRcvd)  < WR_RIM_RSN_MID )
      {
          /* caller will process the PDU */
          RETVALUE(ROK);
      }
      else
      {
          RETVALUE(RFAILED);
      }
   }
   else if ( newRsn < lastRsnRcvd )
   {
      if ( ( WR_RIM_RSN_MAX  - (lastRsnRcvd - newRsn )) < WR_RIM_RSN_MID )
      {
          /* caller will process the PDU */
          RETVALUE(ROK);
      }
      else
      {
          /* caller will ignore the PDU */
          RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* end of wrRimValRcvdRiRsn */
 
/** @brief This function process the received RI PDU 
*
*     Function: wrRimProcessRi
*
*         Processing steps:
*         -  Find the RIM association
*         -  Validate and update the RIM association state.
*         -  Build and send RAN error PDU in case of error.
*         -  Validate the RSN value received with the last received RSN
*         -  Update the output structure
*
* @param[in] decCb  : Decode CB
* @param[in] rimPdu : rimPdu
* @param[out] rimInfo: RIM response structure
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimProcessRi
(
 WrRimEncDecCb               *decCb, 
 WrRimDecRspInfo             *rimInfo, 
 TknStrOSXL                  *rimPdu 
)
{
   WrRimEncDecCb             encCb;
   WrRimAssocCb              *assocCb = NULLP;
   WrRimAssocHashKey         assocKey;
   Buffer                    *pdu = NULLP;
   S16                       ret;
   U8                        cellIdx = 0;
   WrRimDstCellInfo         *assocCellCb;
   U8                        appContErrPres = decCb->appErrCnt.appCntIe.pres;
   S16                       msgIndx = 1; /* To decoding from Length field */

   /* Get the association */
   assocKey.srcAddr = decCb->dstAddr;
   assocKey.dstAddr = decCb->srcAddr;
   assocKey.appId   = decCb->appId.val;

   /* If appContErrPres is TRUE then riAppCont will be absent,
    * hence need to decode erroneous application container to 
    * decode the reporting cell identfier. Update the reporting
    * cellIdx in decCb->riAppCont.rptCellId */
   if ( TRUE == appContErrPres ) 
   {
      wrRimDecRirAppCont( &(decCb->appErrCnt.appCntIe), decCb, &msgIndx ); 
      /* Store rptCell id in riAppCont.rptCellId, so that we
       * can avoid different cases below */
      decCb->riAppCont.rptCellId = decCb->rirAppCont.rptCellId;
      /* Release the memory allocated for appContIE */
      if ( NULLP != decCb->rirAppCont.appContIe.val )
      {
         WR_FREE(decCb->rirAppCont.appContIe.val, \
               decCb->rirAppCont.appContIe.len);
      }
   }
   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);
   
   if( (NULLP == assocCb) || (FALSE == wrRimGetCellIdxFrmAssoc \
               (assocCb, &(decCb->riAppCont.rptCellId), &cellIdx)))
   {
      RLOG0(L_ERROR, "wrRimProcessRi: AssocCb doesnt exists"); 
      RETVALUE(RFAILED);
   }
   assocCellCb = &(assocCb->cellLst[cellIdx]);
   /* validate the association states */
   if (ROK != wrRimRiValAndUpdtState ( assocCellCb, decCb))
   {
      if ( PRSNT_NODEF != decCb->decFailCause.pres )
      {
         /* Discarding the PDU without sending any RIE PDU */
         RETVALUE(RFAILED);
      }
      /* Build RIError PDU */
      if (ROK != wrRimFillRiErrEncCb(&encCb, decCb, rimPdu))
      {
         RLOG0(L_ERROR, "wrRimProcessRi:wrRimFillRiErrEncCb failed!");
         RETVALUE(RFAILED);
      }
      /* allocated memory for rimPdu */
      SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &pdu);
      if ( NULLP == pdu )
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      } 
      if ( ROK  != wrRimEncPdu(&encCb,pdu))
      {
         RLOG0(L_ERROR, "wrRimProcessRi:wrRimEncPdu failed!");
         WR_FREEMBUF(pdu);
         RETVALUE(RFAILED);
      }

      rimInfo->isDecSucc = FALSE;
      rimInfo->dstAddr   = decCb->srcAddr;
      rimInfo->srcAddr   = decCb->dstAddr;
      rimInfo->rptCellId = decCb->riAppCont.rptCellId;
      rimInfo->rimErrPdu = pdu;
      /* Free the memory as the validation failed */
      WR_RIM_DEC_FREE_UNUSED_MEM(decCb);
      RETVALUE(ROK);
   }
   /* Stop the T-RIR timer */
   if (( WR_RIM_RI_INIT_MULTI == decCb->pduInd.pduTypExt) ||
       ( WR_RIM_RI_SINGLE     == decCb->pduInd.pduTypExt)) 
   {
      RLOG0(L_INFO,"wrRimProcessRi:Stopping WR_TMR_RIM_TRIR timer");
      rimCb.rimStopTmr((PTR)assocCellCb, WR_TMR_RIM_TRIR);
      /* Free the Rtx buffer */
      if ( NULLP != assocCellCb->rtxPdu )
      {
         WR_FREEMBUF(assocCellCb->rtxPdu);
      }
   }
   /* validate RSN : RSN validation is done for pdu ext type WR_RIM_RI_MULTI,
    WR_RIM_RI_END and WR_RIM_RI_STOP */
   else
   {
      if ( ROK != wrRimValRcvdRiRsn (assocCellCb->lastRcvdRsn, decCb->rsn.val))
      {
         /* Discard the RI pdu if RSN is outside the window */
         RLOG0(L_ERROR, "wrRimProcessRi:wrRimValRcvdRiRsn failed!");
         RETVALUE(RFAILED);
      }             
   }
   /* Update the received RSN */
   assocCellCb->lastRcvdRsn = decCb->rsn.val;
   /* Check if Ack requested */
   if (( WR_RIM_RI_END    == decCb->pduInd.pduTypExt) ||
       (( WR_RIM_RI_MULTI == decCb->pduInd.pduTypExt) &&
        ( TRUE == decCb->pduInd.isAckRequested ))) 
   {
      wrRimFillRiAckEncCb(&encCb, decCb);
      /* allocated memory for rimPdu */
      SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &pdu);
      if ( NULLP == pdu )
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      } 
      if ( ROK  != wrRimEncPdu(&encCb,pdu))
      {
         RLOG0(L_ERROR, "wrRimProcessRi:wrRimEncPdu failed!");
         WR_FREEMBUF(pdu);
         RETVALUE(RFAILED);
      }
      assocCellCb->lastSentRsn = encCb.rsn.val;
      rimInfo->rimAckPdu      = pdu;
      rimInfo->isAckRequested = TRUE;
   }

   /* Update the output structure */
   rimInfo->isDecSucc    = TRUE;
   rimInfo->rimPduType   = decCb->pduType;
   rimInfo->appId        = decCb->appId.val;
   rimInfo->pdu.riDecInfo.rptType = decCb->pduInd.pduTypExt;

   if ( PRSNT_NODEF == appContErrPres )
   {
     rimInfo->pdu.riDecInfo.appErrCause.pres = PRSNT_NODEF;
     rimInfo->pdu.riDecInfo.appErrCause.val  = decCb->appErrCnt.appCause;
   }
   else if (( WR_RIM_RI_STOP != decCb->pduInd.pduTypExt) &&
            ( WR_RIM_RI_END  != decCb->pduInd.pduTypExt))
   {
      if ( WR_RIM_APP_UTRA_SI == decCb->appId.val )
      {
        rimInfo->pdu.riDecInfo.app.utraSI = decCb->riAppCont.app.utraSI; 
      }
      else if ( WR_RIM_APP_NACC == decCb->appId.val )
      {
        rimInfo->pdu.riDecInfo.app.nacc.noOfSi = decCb->riAppCont.app.nacc.noOfSi; 
        rimInfo->pdu.riDecInfo.app.nacc.siType = decCb->riAppCont.app.nacc.siType; 
        rimInfo->pdu.riDecInfo.app.nacc.si     = decCb->riAppCont.app.nacc.si; 
      }
      /* Update the App Container IE, which is required in case of
       * reporting any application error container */
        rimInfo->pdu.riDecInfo.appCont        = decCb->riAppCont.appContIe;
   }
   rimInfo->rptCellId = decCb->riAppCont.rptCellId; 
   rimInfo->srcAddr   = decCb->dstAddr;
   rimInfo->dstAddr   = decCb->srcAddr; 

   /* Delete the association if RI/END or RI/Stop received */
   if (( WR_RIM_RI_END  == decCb->pduInd.pduTypExt)  ||
       ( WR_RIM_RI_STOP == decCb->pduInd.pduTypExt)||
       ( PRSNT_NODEF == appContErrPres))
   { 
     /* Free the memory allocated for SI and app container IE
        as it is not used in case of END/STOP */
      WR_RIM_DEC_FREE_UNUSED_MEM(decCb);
      wrRimDelAssoc(assocCb, cellIdx, FALSE);
   }
   RETVALUE(ROK);
}/* end of wrRimProcessRi */

/** @brief This function Validates the Destination address received in 
* RIM PDU 
*
*     Function: wrRimValDstAddr 
*
*         Processing steps:
*         - Validate the Destination address received in RIM
*         PDU. If the address is same as the self node address
*         return ROK. 
*
* @param[out]  dstAddr : Destination Routing Address
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimValDstAddr(WrRimRoutingInfo *dstAddr)
{
   S16 ret = ROK;

   switch(dstAddr->ratChoice)
   {
      case WR_RIM_EUTRA_ROUT_DISC:
         {
             WrEnbIdentifier   *enbAddr = &(rimCb.selfAddr.t.eutraIdentity); 
            /* Check enbId, enbType and tac */
            if ((dstAddr->t.eutraIdentity.enbType != enbAddr->enbType) ||
                (dstAddr->t.eutraIdentity.enbId   != enbAddr->enbId)   ||
                (dstAddr->t.eutraIdentity.tac     != enbAddr->tac)     || 
                (FALSE == wrEmmPlmnPlmnsEqual(&(dstAddr->t.eutraIdentity.plmnId),
                                                         &(enbAddr->plmnId))))
            {
                ret = RFAILED;
                RLOG0(L_ERROR, "wrRimValDstAddr: Invalid Addr");
            }
            /* compare the PLMN ID */
            break;
         }
      /* TODO-FFP: Need to enhance the API for the other RATs, if
       * the RIM protocol has to be used in other nodes ( GERAN or UTRAN)
       */
      case WR_RIM_GERAN_ROUT_DISC:
      case WR_RIM_UTRA_ROUT_DISC:
      /* fall through */
      default:
         {
            ret = RFAILED;
            RLOG0(L_ERROR, "wrRimValDstAddr: Invalid Addr");
            break;
         }
   }
   RETVALUE(ret);
}
/** @brief This function handles the receibed RIR PDU 
*
*     Function: wrRimProcessRir
*
*         Processing steps:
*         -  Create RIM assocaition CB and initialize it.
*         -  Update the output strcuture  
*
* @param[in] decCb: decode Cb.
* @param[out] rimInfo: RIM info response, output structure.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimProcessRir
( 
 WrRimEncDecCb               *decCb, 
 WrRimDecRspInfo             *rimInfo 
 )
{
   S16                       ret;
   WrRimAssocCb             *assocCb = NULLP;
   WrRimAssocHashKey         assocKey;
   U8                        cellIdx = 0; 
#ifdef WR_RIM_FFP
   if ( WR_RIM_RIR_STOP == decCb->pduInd.pduTypExt )
   {
       /* TODO-FFP: As of now discard the PDU as eNB will
        * not behave as serving BSS */
      RLOG0(L_INFO, "wrRimProcessRir: RIR received with\
                          PDU ext  WR_RIM_RIR_STOP"); 
      RETVALUE(RFAILED);
   }
#endif

   /* Check if Association already present */
   /* Destination address will the source and source address would be 
    * the destination address for further communication */
   assocKey.srcAddr = decCb->dstAddr;
   assocKey.dstAddr = decCb->srcAddr;
   assocKey.appId   = decCb->appId.val;

   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);

   if ( NULLP != assocCb ) 
   {
      /* Continue if RIR received for existing association, This may be the
       * scenario where report has been lost and RIR re-transmitted by the 
       * controlling BSS, This scenario should not hit as of now, because eNB will
       * not behave as Serving BSS in present implementation  */
      RLOG0(L_INFO, "wrRimProcessRir: RIR received for exisitng association "); 
   }
   else
   {
      /* check if the Destination address received in the RIR PDU is
       * same as enbId of self node */
      if( ROK != wrRimValDstAddr(&(decCb->dstAddr)))
      {
        RLOG0(L_INFO, "wrRimProcessRir: Destination address received in RIR PDU \
                                        does not match the self node address"); 
        RETVALUE(RFAILED);
      }
      ret = wrRimCreateAssoc( &assocCb, &assocKey );
      if ( ROK != ret )
      {
         RLOG0(L_ERROR, "wrRimProcessRir: Creation of RIM association failed");
         RETVALUE(RFAILED);
      }
   }
   /* Update the report type and rptCellId */
   /* update the report type and cell Id */
   assocCb->cellLst[cellIdx].reptType  = decCb->pduInd.pduTypExt;
   assocCb->cellLst[cellIdx].rptCellId = decCb->rirAppCont.rptCellId; 

   /* TODO-FFP: handling of multiple request, as of now
      eNB will always be controlling BSS, hence we keep this 
      for -FFP */

   /* Fill Output structure */
   rimInfo->isDecSucc                = TRUE; 
   rimInfo->rimPduType               = WR_RIM_RIR_PDU; 
   rimInfo->appId                    = decCb->appId.val;
   rimInfo->rptCellId                = decCb->rirAppCont.rptCellId;
   rimInfo->pdu.rirDecInfo.appCont   = decCb->rirAppCont.appContIe;
   rimInfo->pdu.rirDecInfo.reptType  = decCb->pduInd.pduTypExt; 
   rimInfo->dstAddr                  = decCb->srcAddr;
   rimInfo->srcAddr                  = decCb->dstAddr;
   rimInfo->rptCellId                = decCb->rirAppCont.rptCellId; 

   RETVALUE(ROK);
}/* end of wrRimProcessRir*/

/** @brief This function hanlder the received RIR ACK PDU 
*
*     Function: wrRimProcessRiAck
*
*         Processing steps:
*         -  Check if RI ACK is valid.
*         -  If valid, stop the timer and delete the retransmission PDU.
*         -  Update the output structure. 
*
* @param[in] decCb : decode CB
* @param[out] rimInfo : RIM Info repsonse, output structure.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimProcessRiAck
( 
 WrRimEncDecCb               *decCb, 
 WrRimDecRspInfo             *rimInfo 
 )
{
   S16                       ret;
   WrRimAssocCb             *assocCb = NULLP;
   WrRimAssocHashKey         assocKey;
   WrRimDstCellInfo         *assocCellCb;
   U8                        cellIdx;

   /* Get the association */
   assocKey.srcAddr = decCb->dstAddr;
   assocKey.dstAddr = decCb->srcAddr;
   assocKey.appId   = decCb->appId.val;

   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);
   if (( ROK != ret) || ( NULLP == assocCb)) 
   {
      RLOG0(L_ERROR, "wrRimProcessRiAck: AssocCb doesnt exists"); 
      RETVALUE(RFAILED);
   }
   /* Compare the received RSN with last sent RSN */
   WR_RIM_FIND_CELL_IDX_USING_RSN(assocCb, decCb->rsn.val, cellIdx);
   if ( WR_RIM_INVALID_CELL_ID == cellIdx ) 
   {
      /* Discard the ACK PDU */
      RLOG0(L_ERROR, "wrRimProcessRiAck: rsn doent match, discarding the PDU"); 
      RETVALUE(RFAILED);
   }
   assocCellCb = &(assocCb->cellLst[cellIdx]);
   /* As of now ACK can be received only for T-RIAE,
    TODO-FFP: Add handling for RI - FFP */
   RLOG0(L_INFO,"wrRimProcessRiAck:Stopping WR_TMR_RIM_TRIAE timer");
   rimCb.rimStopTmr((PTR)assocCellCb, WR_TMR_RIM_TRIAE);
   /* Free the Rtx buffer */
   if ( NULLP != assocCellCb->rtxPdu )
   {
      WR_FREEMBUF(assocCellCb->rtxPdu);
   }
   rimInfo->pdu.riaDecInfo.pduType = WR_RIM_RIAE_PDU; 

   /* update the output structure */
   rimInfo->isDecSucc = TRUE;
   rimInfo->rimPduType   = decCb->pduType;

   rimInfo->rptCellId = assocCellCb->rptCellId; 
   rimInfo->srcAddr   = decCb->srcAddr; 
   rimInfo->dstAddr   = decCb->dstAddr; 
   rimInfo->appId     = assocKey.appId;

   /* delete the Assoc cb, if ACK received for RIAE */
   if (  WR_RIM_RIAE_PDU == rimInfo->pdu.riaDecInfo.pduType)
   {
      wrRimDelAssoc(assocCb, cellIdx, FALSE);
   }

   RETVALUE(ROK);
}/* end of wrRimProcessRiAck */

/** @brief This function hanldes the received RI ERR PDU 
*
*     Function: wrRimProcessRiErr
*
*         Processing steps:
*         -  Find the association, stop the timer and remove the retransmission PDU
*         -  Delete the association.
*         -  Update the output structure 
*
* @param[in] decCb  : Decode Cb
* @param[in] rimInfo: RIM response Info, output structure.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimProcessRiErr
( 
 WrRimEncDecCb               *decCb, 
 WrRimDecRspInfo             *rimInfo 
)
{
   S16                       ret;
   WrRimAssocCb             *assocCb = NULLP;
   WrRimDstCellInfo         *assocCellCb = NULLP;
   WrRimAssocHashKey         assocKey;
   U8                        cellIdx;
   WrRimEncDecCb             riErrDecCb; 

   /* Get the association */
   assocKey.srcAddr = decCb->dstAddr;
   assocKey.dstAddr = decCb->srcAddr;
   assocKey.appId   = decCb->appId.val;

   ret = cmHashListFind(&(rimCb.assocCbHL),
         (U8 *)&assocKey, sizeof(WrRimAssocHashKey),0,(PTR *)&assocCb);
   if ( NULLP == assocCb ) 
   {
      RLOG0(L_ERROR, "wrRimProcessRiErr: AssocCb doesnt exists"); 
      RETVALUE(RFAILED);
   }

   WR_SET_ZERO((U8*)(&riErrDecCb), sizeof(WrRimEncDecCb));
   /* Decode the PDU in error to fetch the RSN value */
   wrRimDecPdu(&(decCb->pduInErr), &riErrDecCb);
   /* Free unused memory, which is allocated by the decoder 
    * while decoding pduInErr and not used further */
   WR_RIM_DEC_FREE_UNUSED_MEM((&riErrDecCb));
   /* Free Unused memory allocated by decCb */
   WR_RIM_DEC_FREE_UNUSED_MEM((decCb));
   /* Find the assocCellCb based on last sent RSN  */
   if ( PRSNT_NODEF == riErrDecCb.rsn.pres )
   {
      WR_RIM_FIND_CELL_IDX_USING_RSN(assocCb, riErrDecCb.rsn.val, cellIdx);
      if ( WR_RIM_INVALID_CELL_ID != cellIdx ) 
      {
         assocCellCb = &(assocCb->cellLst[cellIdx]);
      }
   }
   if ( NULLP != assocCellCb )
   {
      /*  Stop the timer  */
      RLOG1(L_INFO,"wrRimProcessRiErr:Stopping timer :%d",  \
                                assocCellCb->timer.tmrEvnt);
      rimCb.rimStopTmr((PTR)assocCellCb, assocCellCb->timer.tmrEvnt);
      /* Free the Rtx buffer */
      if ( NULLP != assocCellCb->rtxPdu )
      {
         WR_FREEMBUF(assocCellCb->rtxPdu);
      }

      /* Update the output strcuture */
      rimInfo->isDecSucc    = TRUE;
      rimInfo->rimPduType   = decCb->pduType;

      rimInfo->srcAddr   = decCb->dstAddr;
      rimInfo->dstAddr   = decCb->srcAddr; 
      rimInfo->appId     = decCb->appId.val; 
      rimInfo->rptCellId = assocCellCb->rptCellId; 
      rimInfo->pdu.riErrDecInfo.rimErrCause = decCb->rimCause.val;

      /* Association shall be deleted after receiving RI Err PDU  */
      wrRimDelAssoc(assocCb, cellIdx, FALSE);
      RETVALUE(ROK);
   }
   else
   {
      RLOG0(L_ERROR, "wrRimProcessRiErr: Not able to find cellIndx discarding \
                      the error PDU"); 
   }
   /* No association found, return failure */
   RETVALUE(RFAILED);
}/* end of wrRimProcessRiErr */

/*============================================================= END of Outgoing PATH =================================================================*/

/*============================================================= Start of incoming PATH =================================================================*/

/** @brief This function handle the received RIM PDU - interface API to RIM App 
 *
 *     Function: wrRimPrcRcvdPdu
 *
 *         Processing steps:
 *         -  
 *
 * @param[in] rimPdu  : RIM PDU
* @param[out] rimInfo: RIM response Info
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/  
/* After receiving RI or any RIM PDU, RIM app shall send it to RIM protocol for decoding */
PUBLIC S16 wrRimPrcRcvdPdu
(
 TknStrOSXL                  *rimPdu,
 WrRimDecRspInfo             *rimInfo 
)
{
    Buffer                   *rirErrPdu;
    WrRimEncDecCb            decCb;
    WrRimEncDecCb            encCb;
    S16                      ret = RFAILED;

    if ( (NULLP == rimPdu) || (NULLP == rimPdu->val) || (NULLP == rimInfo))
    {
      RLOG0(L_ERROR, "wrRimPrcRcvdPdu: Invalid Input!");
      RETVALUE(RFAILED);
    }
     
    WR_SET_ZERO((U8*)&decCb, sizeof(WrRimEncDecCb));

    /* Set the output structure values to zero */
    WR_SET_ZERO((U8*) rimInfo,  sizeof(WrRimDecRspInfo));

    ret = wrRimDecPdu(rimPdu, &decCb);
    /* Free unused memory, which is allocated by the decoder */
    if ( ROK != ret)
    {
      WR_RIM_DEC_FREE_UNUSED_MEM((&decCb));
      RLOG0(L_ERROR, "wrRimPrcRcvdPdu:wrRimDecPdu failed!");
      if ( PRSNT_NODEF == decCb.decFailCause.pres ) 
      {
         if (ROK != wrRimFillRiErrEncCb(&encCb, &decCb, rimPdu))
         {
            RLOG0(L_ERROR, "wrRimPrcRcvdPdu:wrRimFillRiErrEncCb failed!");
            RETVALUE(RFAILED);
         }
         /* allocated memory for rimPdu */
         SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &rirErrPdu);
         if ( NULLP == rirErrPdu )
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         } 
         if ( ROK  != wrRimEncPdu(&encCb, rirErrPdu))
         {
            RLOG0(L_ERROR, "wrRimPrcRcvdPdu:wrRimEncPdu failed!");
            WR_FREEMBUF(rirErrPdu);
            RETVALUE(RFAILED);
         }
         rimInfo->isDecSucc = FALSE;
         rimInfo->rimErrPdu = rirErrPdu;
         /* Update the destination address, which is required to
          * send in rIMRoutingAdress IE of eNB Direct Information
          * transfer IE */
         rimInfo->dstAddr   = decCb.srcAddr;
         rimInfo->srcAddr   = decCb.dstAddr;
         RETVALUE(ROK);
      }
      else
      {
        RETVALUE(RFAILED);
      }
    }
    switch(decCb.pduType)
    {
       case WR_RIM_RIR_PDU:
          {
             if ( ROK != wrRimProcessRir(&decCb, rimInfo))
             {
                RLOG0(L_ERROR, "wrRimPrcRcvdPdu: wrRimProcessRir failed");
                WR_RIM_DEC_FREE_UNUSED_MEM((&decCb));
                RETVALUE(RFAILED);
             }
             break;
          }
       case WR_RIM_RI_PDU:
          {
             if ( ROK != wrRimProcessRi(&decCb, rimInfo, rimPdu))
             {
                RLOG0(L_ERROR, "wrRimPrcRcvdPdu: wrRimProcessRi failed");
                WR_RIM_DEC_FREE_UNUSED_MEM((&decCb));
                RETVALUE(RFAILED);
             }
             break;
          }
       case WR_RIM_RIA_PDU:
          {
             if ( ROK != wrRimProcessRiAck(&decCb, rimInfo))
             {
                RLOG0(L_ERROR, "wrRimPrcRcvdPdu: wrRimProcessRiAck failed");
                RETVALUE(RFAILED);
             }
             break;
          }
#ifdef WR_RIM_FFP
       case WR_RIM_RIAE_PDU:
          {
             /* TODO-FFP: As per present scope, eNB will always behave as controlling
                BSS and hence no RIAE will be received. Need to take care of this
                once we support eNB as serving BSS, as of now printing error log 
                and returning RFAILED */ 

             RLOG0(L_ERROR, "wrRimPrcRcvdPdu: Received WR_RIM_RIAE_PDU"); 
             RETVALUE(RFAILED);
          }
#endif
       case WR_RIM_RIE_PDU:
          {
             if ( ROK != wrRimProcessRiErr(&decCb, rimInfo))
             {
                RLOG0(L_ERROR, "wrRimPrcRcvdPdu: wrRimProcessRie failed");
                RETVALUE(RFAILED);
             }
             break;
          }
       default:
          {
             RLOG0(L_ERROR, "wrRimPrcRcvdPdu: Invalid pduType");
             RETVALUE(RFAILED);
          }
    } /* switch case end */  

    RETVALUE(ROK);
}  /* end of wrRimPrcRcvdPdu */

/*================================================ RIM DECODE APIs ========================================= */
 /** @brief This function Unpacks the Geran Id 
*
*     Function: wrRimUnpkGeranId
*
*         Processing steps:
*         -  Unpack the Geran Identifier 
*
* @param[in] rimPdu : RIM PDU.
* @param[in] msgIndx: message index.
* @param[out] geranCellId: Geran Cell Identifier.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimUnpkGeranId
(
 TknStrOSXL                  *rimPdu, 
 S16                         *msgIdx,
 WrGeranCellId               *geranCellId
)
{

    WrPlmnId                 *plmnId = &(geranCellId->plmnId);
   /* Value part of Geran Cell Identifier - as per  48.018 - 11.3.9 */

   /* Unpack PLMN id */
    WR_RIM_TLV_UNPK_PLMN_ID(rimPdu, plmnId, (*msgIdx));

   /* Unpack LAC */
    WR_RIM_UNPK_U16(rimPdu->val, (geranCellId->lac), (*msgIdx));

   /* Unpack RAC */
    WR_RIM_UNPK_U8(rimPdu->val, (geranCellId->rac), (*msgIdx));

   /* Unpack cell id */
    WR_RIM_UNPK_U16(rimPdu->val, (geranCellId->ci), (*msgIdx));

    RETVALUE(ROK);

}/* end of wrRimUnpkGeranId */ 

/** @brief This function unpacks the Utran Identifier 
*
*     Function: wrRimUnpkUtranId
*
*         Processing steps:
*         - Unpack the Utran Identifier 
*
* @param[in]  rimPdu     : RIM PDU.
* @param[in]  msgIndx    : message index.
* @param[out] geranCellId: Utran Cell Identifier.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimUnpkUtranId
(
 TknStrOSXL                  *rimPdu, 
 S16                         *msgIdx,
 WrRncIdentifier             *utraCellId
 )
{

    WrPlmnId                 *plmnId = &(utraCellId->plmnId); 
   /* Value part of Geran Cell Identifier - as per  48.018 - 11.3.9 */

   /* Unpack PLMN id */
    WR_RIM_TLV_UNPK_PLMN_ID(rimPdu, plmnId, (*msgIdx));

   /* Unpack LAC */
    WR_RIM_UNPK_U16(rimPdu->val, utraCellId->lac, (*msgIdx));

   /* Unpack RAC */
    WR_RIM_UNPK_U8(rimPdu->val, utraCellId->rac, (*msgIdx));

   /* Unpack RNC  id */
    WR_RIM_UNPK_U16(rimPdu->val, utraCellId->rncId, (*msgIdx));

    RETVALUE(ROK);

} /* end of wrRimUnpkUtranId */
 /** @brief This function Unpacks the eNb identifier 
*
*     Function: wrRimUnpkEnbId
*
*         Processing steps:
*         -  Unpack the Global identifier using PASN decoder. 
*         -  Unpack the TAI
*
* @param[in]  rimPdu         : RIM PDU.
* @param[in]  msgIndx        : message index.
* @param[in]  routInfoLen    : Routing Info Length.
* @param[out] eutranId       : eNB Identifier.
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimUnpkEnbId
(
 TknStrOSXL                  *rimPdu, 
 S16                         *msgIdx,
 WrEnbIdentifier             *eutranId, 
 MsgLen                      routInfoLen
 )
{
   S16                       gblEnbIdLen;
   SzIePdu                   *iePdu; 
   U32                       numDecOct = 0; /* number of octets decoded */
   S16                       ret;       /* return value from different procedures */
   U8                        dbgFlag = TRUE;
   Buffer                    *mBuf;
   CmPAsnErr                 asnErr;
   TknStr4                   *enbIdIe;
   U32                       msgLen;


   /* Unpack the length */
   WrPlmnId                  *plmnId = &(eutranId->plmnId); 

   /* Decode TAI */ 

   WR_RIM_TLV_UNPK_PLMN_ID(rimPdu, plmnId, (*msgIdx));

   WR_RIM_UNPK_U16(rimPdu->val, (eutranId->tac), (*msgIdx));

   gblEnbIdLen = routInfoLen - WR_RIM_LEN_TAI; 
   if ( gblEnbIdLen <= 0 )
   {
      RLOG0(L_ERROR,"wrRimUnpkEnbId: Invalid Length");
      RETVALUE(RFAILED);
   }
   /* Initialize memCp */
   WR_ALLOCEVNT(&(iePdu), sizeof(SzIePdu));
   if (NULLP == iePdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Alloc memory for mBuf */
   SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &mBuf);
   if ( NULLP == mBuf )
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREEEVNT(iePdu);
      RETVALUE(RFAILED);
   }
   /* convert the OSXL to Buffer */
   SAddPstMsgMult( &(rimPdu->val[*msgIdx]), gblEnbIdLen, mBuf );

   asnErr.errCode = 0;
   msgLen = 0;
   ret = (cmPAsnDecMsg((Void *)&(iePdu->u.globalEnbId), CM_PASN_USE_MBUF, 
            (Void **)&(mBuf), &numDecOct, 0, 
            ((CmPAsnElmDef **)
             szMsgIEDb[WR_UMM_S1AP_ENC_IE_IDX_GLBL_ENB_ID]),
            CM_PASN_DROP_ELMNTS, 
            (CmPAsnErr*) &(asnErr), msgLen,
            (CmMemListCp *)&(iePdu->memCp), dbgFlag));

   if ( ROK != ret )
   {
      RLOG1(L_ERROR,"wrRimUnpkEnbId: cmPAsnDecMsg failed, with cause :%d", asnErr.errCode);
      WR_FREEEVNT(iePdu);
      WR_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Unpack PLMN id */
   /* TODO-FFP: Assuming PLMN-Id in TAI and enbId are same and unpkacing PLMNid present in 
      TAI only */
   /* TODO: validate choice */

   /* Decode enb type and enbId */
   if ( ENB_ID_MACROENB_ID == iePdu->u.globalEnbId.eNB_ID.choice.val)
   {
      eutranId->enbType = ENB_ID_MACROENB_ID;
      enbIdIe = &(iePdu->u.globalEnbId.eNB_ID.val.macroENB_ID);
   }
   else 
   {
      /* Home ENB ID */
      eutranId->enbType = ENB_ID_HOMEENB_ID;
      enbIdIe = &(iePdu->u.globalEnbId.eNB_ID.val.homeENB_ID);
   }
   WR_RIM_UNPK_TKN_STR4(enbIdIe, (eutranId->enbId)); 

   /* Update the msg Index and free the allocated memory before returning */
   *msgIdx+= gblEnbIdLen; 

   WR_FREEEVNT(iePdu);
   WR_FREEMBUF(mBuf);
   RETVALUE(ROK);

}/* end of wrRimUnpkEnbId */
 
/** @brief This function decode the Routing Info. 
*
*     Function: wrRimDecRoutInfo
*
*         Processing steps:
*         - Decode the routing info based on Routing Discriminator. 
*
* @param[in] rimDecCb  : RIM decode Cb. 
* @param[in] rimPdu    : RIM Pdu.
* @param[in] msgIndx   : Message Index.
* @param[out] routInfo : Routing Info.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16  wrRimDecRoutInfo
(
 WrRimEncDecCb               *rimDecCb, 
 TknStrOSXL                  *rimPdu, 
 S16                         *msgIndx, 
 WrRimRoutingInfo            *routInfo
)
{
     U8                      routInfoIei;  
     MsgLen                  len;
     U8                      routDisc;

     if ( (rimPdu->len - *msgIndx ) <  WR_RIM_MIN_ROUT_INFO_LEN )
     {
        RLOG0(L_ERROR, "wrRimDecRoutInfo: Len Invalid, Discarding the PDU");
        RETVALUE(RFAILED);
     }
     /* unpk Routing Info IEI */
     WR_RIM_UNPK_U8(rimPdu->val, routInfoIei, (*msgIndx));

     if ( WR_RIM_IEI_ROUT_INFO != routInfoIei )
     {
       RLOG1(L_ERROR,"wrRimDecRoutInfo: Invalid IEI : %d]",
            routInfoIei);
       RETVALUE(RFAILED);
     } 
      
     WR_RIM_UNPK_LEN(rimPdu->val, len, (*msgIndx));
     if ( ( rimPdu->len - *msgIndx ) < len )
     {
        RLOG0(L_ERROR, "wrRimDecRoutInfo:Invalid len, Discarding the PDU");
        RETVALUE(RFAILED);
     }
     /* Unpack Routing Address Descriminator */
     WR_RIM_UNPK_ROUT_DISC(rimPdu->val, routDisc, (*msgIndx));
     
     switch(routDisc)
     {
        case WR_RIM_GERAN_ROUT_DISC:
           if( ( WR_RIM_LEN_GERAN_CELL_ID != (len - 1 ))||
               ( ROK != wrRimUnpkGeranId(rimPdu, msgIndx, 
                                  &(routInfo->t.geranCellId))))
           {
              RLOG0(L_ERROR, "wrRimDecRoutInfo:Invalid len - Discarding the PDU");
              RETVALUE(RFAILED);
           }
           break;
        case WR_RIM_UTRA_ROUT_DISC:
           if( ( WR_RIM_LEN_RNC_ID != (len - 1)) ||
               ( ROK != wrRimUnpkUtranId(rimPdu, msgIndx, 
                                           &(routInfo->t.rncIdentity))))
           {
              RLOG0(L_ERROR, "wrRimDecRoutInfo:Invalid len - Discarding the PDU");
              RETVALUE(RFAILED);
           }
           break;
        case WR_RIM_EUTRA_ROUT_DISC:
           if ( ROK != wrRimUnpkEnbId(rimPdu, msgIndx,
                              &(routInfo->t.eutraIdentity), (len - 1)))
           {
              RLOG0(L_ERROR, "wrRimDecRoutInfo:EnbId Unpacking failed\n");
              RETVALUE(RFAILED);
           }
        break;
       default:
        {
           RLOG1(L_ERROR,"wrRimDecRoutInfo: Invalid routing Disc \
                 Discarding PDU: [%d]", routDisc);
           RETVALUE(RFAILED);
        }
     }
     /* Update the routing Disc */
     routInfo->ratChoice = routDisc; 
     RETVALUE(ROK);
} /* end of wrRimDecRoutInfo */

/** @brief This function decodes the Utra CellId 
*
*     Function: wrRimDecUtraCellId 
*
*         Processing steps:
*         - Decode the UTRA cellId using PASN decoder. 
*
* @param[in] rimPdu    : RIM PDU. 
* @param[in] msgIndx   : Index to the PDU to start decoding.
* @param[in] isExtRncId: presence of extented RNC.
* @param[out] utraCel  : UTRA cell Identifer.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecUtraCellId
(
 TknStrOSXL            *rimPdu,
 WrUtraCellId          *utraCell,
 Bool                   isExtRncId,
 S16                   *msgIndx
 )
{
   SzIePdu                  *iePdu; 
   U32                       numDecOct = 0; /* number of octets decoded */
   S16                       ret; 
   U8                        dbgFlag = TRUE;
   U32                       msgLen = 0;
   Buffer                    *mBuf;
   CmPAsnErr                 asnErr;
   U8                        tmpIndx = 0;
   U32                       tgtCellId;
   TknStrOSXL                *plmnId;


   /* Initialize memCp */
   WR_ALLOCEVNT(&(iePdu), sizeof(SzIePdu));
   if ( NULLP == iePdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Alloc memory for mBuf */
   SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &mBuf);
   if ( NULLP == mBuf)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREEEVNT(iePdu);
      RETVALUE(RFAILED);
   }
   /* convert the OSXL to Buffer */
   SAddPstMsgMult( &(rimPdu->val[*msgIndx]), WR_RIM_LEN_UTRAN_CELL_ID, mBuf );

   asnErr.errCode = 0;
   msgLen = 0;
   ret = (cmPAsnDecMsg((Void *)&(iePdu->u.utraSrcCellId), CM_PASN_USE_MBUF,
            (Void **)&(mBuf), &numDecOct, 0, 
            ((CmPAsnElmDef **)
             szMsg3gDbIe[SZT_REQ_TYPE_ENCDEC_IE_UTRA_SRC_CELL_ID]),
            CM_PASN_DROP_ELMNTS, 
            (CmPAsnErr*) &(asnErr), msgLen,
            (CmMemListCp *)&(iePdu->memCp), dbgFlag));

   if ( ROK != ret )
   {
      RLOG1(L_ERROR,"wrRimDecUtraSiApp: cmPAsnDecMsg failed, with cause :%d",\
            asnErr.errCode);
      WR_FREEEVNT(iePdu);
      WR_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Unpack PLMN id */
   plmnId = &(iePdu->u.utraSrcCellId.val.sourceUTRANCellID. pLMNidentity);
   WR_RIM_UNPK_PLMN_ID(plmnId, (&(utraCell->plmnId)), tmpIndx);

   /* Deocde CI and RNC ID from from target celld */
   /* Check if RNC id  is extended */
   tgtCellId = iePdu->u.utraSrcCellId.val.sourceUTRANCellID.uTRANcellID.val; 
   if ( TRUE != isExtRncId ) 
   {
      utraCell->rncId  =  ( tgtCellId >> 16 );
      utraCell->cId    =  ( tgtCellId & 0x0000FFFF);
   }
   else
   {
      utraCell->rncId  = ( tgtCellId >> 12 );  
      utraCell->cId    =  ( tgtCellId & 0x00000FFF); 
   }

   /* Update the message index */
   *msgIndx += numDecOct;
   /* Free the memory allocated for iePdu and mBuf */
   WR_FREEEVNT(iePdu);
   WR_FREEMBUF(mBuf);

   RETVALUE(ROK);
}


/** @brief This function decode the RIR App Container 
*
*     Function: wrRimDecRirAppCont
*
*         Processing steps:
*         -  Decode the Application container based on application identity. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecRirAppCont
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb,
 S16                         *msgIndx
)
{

   TknStrOSXL                *appContIe = &(rimDecCb->rirAppCont.appContIe);  
   U16                       len;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == rimDecCb->rirAppCont.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecRirAppCont: Multi Instance of App Cont IE, \
                                                       discarding PDU");
      RETVALUE(RFAILED);
   }

   if ( (rimPdu->len - *msgIndx) < WR_RIM_MIN_LEN_IEI_LI )
   {
     RLOG0(L_ERROR,"wrRimDecRirAppCont: Inavlid length");
     WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR );
     RETVALUE(RFAILED);
   }
   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, len, (*msgIndx));

   if ( (rimPdu->len - *msgIndx) < len )
   {
      RLOG0(L_ERROR,"wrRimDecRirAppCont: Invalid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }

  /* Copy the complete Application container including
     IE and Len, in case if required for reporting any
     Application error */

  /* Assign appCont len = len +  2  (2  to incldue IEI and LI )*/
  appContIe->len = len + 2;

  WR_ALLOC(&(appContIe->val), (appContIe->len));
  if ( NULLP == appContIe->val )
  {
     RLOG0(L_FATAL, "Memory allocation failed.");
     RETVALUE(RFAILED);
  }
  appContIe->pres = PRSNT_NODEF;
  cmMemcpy((appContIe->val), &(rimPdu->val[*msgIndx - 2]), \
                                             appContIe->len); 
  switch (rimDecCb->appId.val) 
  {
     case WR_RIM_APP_NACC:
     case WR_RIM_APP_SI3:
     case WR_RIM_APP_MBMS:
        {
           /* Update the RAT choice to GERAN in reporting cell Id and
            * Unpack the cell Id  */
           rimDecCb->rirAppCont.rptCellId.ratChoice = WR_RIM_GERAN_RAT;
           if( ROK != wrRimUnpkGeranId(rimPdu, msgIndx, 
                    &(rimDecCb->rirAppCont.rptCellId.t.geranCell)))
           {
              RLOG0(L_ERROR,"wrRimDecNaccApp: wrRimUnpkGeranId failed");
              WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
              RETVALUE(RFAILED);
           }
        }
        break;
     case WR_RIM_APP_UTRA_SI:
        {

           WrUtraCellId  *utraCell = &(rimDecCb->rirAppCont.rptCellId.t.utraCell);
           Bool  isExtRncId = FALSE;
           /* Decode the reporting Utran cell Id */

           /* Check if extended RNC id is present */
           if (rimDecCb->srcAddr.t.rncIdentity.rncId >= WR_EXT_RNC_START)
           {
              isExtRncId = TRUE;
           }
           /* Update the RAT choice to UTRA in reporting cell Id */
           rimDecCb->rirAppCont.rptCellId.ratChoice = WR_RIM_UTRA_RAT;
           if( ROK != wrRimDecUtraCellId(rimPdu, utraCell, isExtRncId, msgIndx))
           {
              RLOG0(L_ERROR,"wrRimDecUtraSiApp: wrRimDecUtraCellId Failed");
              WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
              RETVALUE(RFAILED);
           }
        }
        break;
     case WR_RIM_APP_SON_TRFR:  
        {
           /* TODO-FFP: reporting cell Id type is based on self node, either 
              EUTRA or UTRA: in this case it is EUTRA always, but
              Need take a parameter as part of RIM configuration to 
              Identify the node, for generic implementation of RIM protoc-FFP */

           /* Move the msgIndx as reportCell Id is not decoded 
              as of now */
           *msgIndx += len;
        }
        break;
     default:
        {
           /* It should never hit here, as the validation of
              AppId would have been done before invoking this API */
           RLOG0(L_ERROR,"wrRimDecRirAppCont: Invalid app Id");
           RETVALUE(RFAILED);
        }
  }

   rimDecCb->rirAppCont.pres = PRSNT_NODEF;
   RETVALUE(ROK); 
}/* end of wrRimDecRirAppCont */

/** @brief This function Deocdes the NACC Application container
*
*     Function: wrRimDecNaccApp
*
*         Processing steps:
*               - Decode the NACC application Container  
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*  
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecNaccApp
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx,
 S16                          len
)
{
    S16                      siLen;
    WrRimNacc                *nacc    = &(rimDecCb->riAppCont.app.nacc);
    TknU8                    *cause   = &(rimDecCb->decFailCause);

   /* Unpack the length */

   siLen = len - (WR_RIM_LEN_GERAN_CELL_ID + 1); /* one byte will be for number
                                                   of SI and type  of S1 */ 
   if (( (rimPdu->len - *msgIndx) < len))
   {
      RLOG0(L_ERROR,"wrRimDecNaccApp: Inavlid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   /* Update the RAT choice to GERAN in reporting cell Id and
    * Unpack the cell Id  */
   rimDecCb->riAppCont.rptCellId.ratChoice = WR_RIM_GERAN_RAT;
   if( ROK != wrRimUnpkGeranId(rimPdu, msgIndx, 
            &(rimDecCb->riAppCont.rptCellId.t.geranCell)))
   {
      RLOG0(L_ERROR,"wrRimDecNaccApp: wrRimUnpkGeranId failed");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   
   /* Check if SI is present */
   if ( (len - (WR_RIM_LEN_GERAN_CELL_ID + 1) ) <= 0 )
   {
       /* RI-End and RI-Stop, will contain only ReportCellId in APP cont */
      if (( WR_RIM_RI_STOP == rimDecCb->pduInd.pduTypExt) ||
          ( WR_RIM_RI_END  == rimDecCb->pduInd.pduTypExt))
      {
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_ERROR,"wrRimDecNaccApp: Inavlid length");
         WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
         RETVALUE(RFAILED);
      }
   }
  
   /* Decode SI */
   WR_RIM_UNPK_NUM_SI_PSI_TYPE(rimPdu->val, nacc->noOfSi, nacc->siType, (*msgIndx));
   WR_ALLOC(&(nacc->si.val), siLen);
   if ( NULLP == nacc->si.val )
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   nacc->si.pres = PRSNT_NODEF;
   nacc->si.len  = siLen;
   WR_RIM_UNPK_STR(rimPdu->val, (&(nacc->si)), (*msgIndx));
   RETVALUE(ROK);
}/* end of wrRimDecNaccApp */

/** @brief This function Decode the UTRA SI application 
*
*     Function: wrRimDecUtraSiApp
*
*         Processing steps:
*         - Decode the the UTRA SI application 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecUtraSiApp
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx,
 S16                          len
)
{
   TknStrOSXL                *utraSi = &(rimDecCb->riAppCont.app.utraSI);
   WrUtraCellId              *utraCell = &(rimDecCb->riAppCont.rptCellId.t.utraCell);
   TknU8                     *cause = &(rimDecCb->decFailCause);
   U16                        siLen;
   S16                        tmpIndx = 0;
   Bool                       isExtRncId = FALSE;

                                             
   if ( (rimPdu->len - *msgIndx) < len ) 
   {
      RLOG0(L_ERROR,"wrRimDecUtraSiApp: Inavlid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   /* Store the msgIndx value */
   tmpIndx = *msgIndx;
   /* Update the RAT choice to UTRA in reporting cell Id */
   rimDecCb->riAppCont.rptCellId.ratChoice = WR_RIM_UTRA_RAT;
   /* Check if extended RNC id is present */
   if (rimDecCb->srcAddr.t.rncIdentity.rncId >= WR_EXT_RNC_START)
   {
      isExtRncId = TRUE;
   }
   if( ROK != wrRimDecUtraCellId(rimPdu, utraCell, isExtRncId, msgIndx))
   {
      RLOG0(L_ERROR,"wrRimDecUtraSiApp: wrRimDecUtraCellId Failed");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   /* Total length - length of UTRA cell Idx */
   siLen = len - (*msgIndx - tmpIndx);

   /* Check if SI is present */
   if ( siLen  <= 0 )
   {
       /* RI-End and RI-Stop, will contain only ReportCellId in APP cont */
      if (( WR_RIM_RI_STOP == rimDecCb->pduInd.pduTypExt) ||
          ( WR_RIM_RI_END  == rimDecCb->pduInd.pduTypExt))
      {
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_ERROR,"wrRimDecUtraSiApp: Inavlid length");
         WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
         RETVALUE(RFAILED);
      }
   }
   /* Decode SI */
   WR_ALLOC(&(utraSi->val), siLen);
   if ( NULLP == utraSi->val )
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   utraSi->pres   = PRSNT_NODEF;
   utraSi->len    = siLen;
   WR_RIM_UNPK_STR(rimPdu->val, utraSi, (*msgIndx));

   RETVALUE(ROK); 
}/* end of wrRimDecUtraSiApp */

/** @brief This function Unpacks RI Application container 
*
*     Function: wrRimDecRiAppCont
*
*         Processing steps:
*         -  Decode the RI application container based on Application identity. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecRiAppCont
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb,
 S16                         *msgIndx
 )
{
   TknU8                     *cause   = &(rimDecCb->decFailCause);
   S16                       len;
   TknStrOSXL               *appContIe = &(rimDecCb->riAppCont.appContIe);
   U8                        apContHeaderLen;

   if ( PRSNT_NODEF == rimDecCb->riAppCont.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecRiAppCont: Multi instances of AppContIE, \
                                                    discarding PDU");
      RETVALUE(RFAILED);
   } 
   if ( (rimPdu->len - *msgIndx) < WR_RIM_MIN_LEN_IEI_LI )
   {
     RLOG0(L_ERROR,"wrRimDecRiAppCont: Inavlid length");
     WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR );
     RETVALUE(RFAILED);
   }
   if ( PRSNT_NODEF != rimDecCb->appId.pres )
   {
      RLOG0(L_ERROR,"wrRimDecRiAppCont: APP Id is not present");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE );
      RETVALUE(RFAILED);
   }
   /* Decode the length IE */
   WR_RIM_UNPK_LEN(rimPdu->val, len, (*msgIndx));
   if (( (rimPdu->len - *msgIndx) < len))
   {
      RLOG0(L_ERROR,"wrRimDecRiAppCont: Invalid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   /* Copy and keep the application container IE including length 
    * in case if required for reporting any Application Error */
   /* Assign appCont len = len +  2 or 3 (to incldue IEI and LI )
    * LI is 2 byte if length is greaten that 127 */
   appContIe->pres = PRSNT_NODEF;
   if( len  > WR_RIM_MAX_LEN_ONE_BYTE )
   {
      apContHeaderLen = 3;
   }
   else
   {
      apContHeaderLen = 2;
   }
   appContIe->len = len + apContHeaderLen;
   WR_ALLOC(&(appContIe->val), (appContIe->len));
   if ( NULLP == appContIe->val )
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cmMemcpy((appContIe->val), &(rimPdu->val[*msgIndx - apContHeaderLen]), 
                                                      appContIe->len); 

   switch ( rimDecCb->appId.val )
   {
      case WR_RIM_APP_NACC:
         if ( ROK != wrRimDecNaccApp(rimPdu, rimDecCb, msgIndx, len))
         {
            RLOG0(L_ERROR,"wrRimDecRiAppCont: wrRimDecNaccApp Failed");
            RETVALUE(RFAILED);
         }
         break;
      case WR_RIM_APP_UTRA_SI:
         if ( ROK != wrRimDecUtraSiApp(rimPdu, rimDecCb, msgIndx,len))
         {
            RLOG0(L_ERROR,"wrRimDecRiAppCont: wrRimDecUtraSiApp Failed");
            RETVALUE(RFAILED);
         }
         break;
      case WR_RIM_APP_SI3:
      case WR_RIM_APP_MBMS:
      case WR_RIM_APP_SON_TRFR: 
         { 
            /* TODO-FFP: Not handled as of now, 
               move the message index */
            /* Unpack the length */
            *msgIndx += len;
         }
         break;
      default:
         {
            /* It should never hit here, as the validation of
               AppId would have been done before invoking this API */
            RLOG0(L_ERROR,"wrRimDecRiAppCont: Invalid appid");
            RETVALUE(RFAILED);
         }
   }
   rimDecCb->riAppCont.pres = PRSNT_NODEF;
   RETVALUE(ROK); 
}/* end of wrRimDecRiAppCont */

/** @brief This function Decodes the application Identity from the 
* received RIM PDU 
*
*     Function: wrRimDecAppId
*
*         Processing steps:
*         - Deocde the applcation identity if length is valid. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecAppId
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx
)
{
   U16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == rimDecCb->appId.pres)
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecAppId: AppId present twice, discarding PDU");
      RETVALUE(RFAILED);
   } 
   if ( (rimPdu->len - *msgIndx) < WR_RIM_MIN_LEN_IEI_LI )
   {
      RLOG0(L_ERROR,"wrRimDecAppId: Invalid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   /* Validate the Application ID IE len */
   if ( WR_RIM_LEN_APP_ID != tmpLen)
   {
      RLOG0(L_ERROR,"wrRimDecAppId: Invalid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_U8(rimPdu->val, rimDecCb->appId.val, (*msgIndx));

   rimDecCb->appId.pres = PRSNT_NODEF;

   if ( ( rimDecCb->appId.val < WR_RIM_APP_NACC ) || 
        ( rimDecCb->appId.val > WR_RIM_APP_UTRA_SI))
   {
      RLOG0(L_ERROR,"wrRimDecAppId: Unknown appId");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_UNKNOWN_RIM_APP);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);  
}/* end of wrRimDecAppId */

/** @brief This function unpacks the RIM cause from the received RIM PDU 
*
*     Function: wrRimDecRimCause
*
*         Processing steps:
*         - Deocde the RIM cause if length is valid. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecRimCause
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx
)
{
   U16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == rimDecCb->rimCause.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecRimCause: Multi Instances of RIM Cause,\
                                                   discarding PDU");
      RETVALUE(RFAILED);
   } 
   if ( (rimPdu->len - *msgIndx) <  WR_RIM_MIN_LEN_IEI_LI ) 
   {
      RLOG0(L_ERROR,"wrRimDecRimCause: Invalid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   /* Validate the Application ID IE len */
   if ( WR_RIM_LEN_RIM_ERR_CAUSE != tmpLen)
   {
      RLOG0(L_ERROR,"wrRimDecRimCause: Invalid length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_U8(rimPdu->val, rimDecCb->rimCause.val, (*msgIndx));

   rimDecCb->rimCause.pres = PRSNT_NODEF;
   RETVALUE(ROK);  
}/* end of wrRimDecRimCause */

/** @brief This function unpacks the RSN from the received RIM PDU 
*
*     Function: wrRimDecRsn
*
*        Processing steps:
*         - Deocde the RSN if length is valid. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecRsn
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx
)
{

   U16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF  == rimDecCb->rsn.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecRsn: Multiple instaces of RSN IE, discarding PDU");
      RETVALUE(RFAILED);
   } 
   if ( (rimPdu->len - *msgIndx) < WR_RIM_MIN_LEN_IEI_LI) 
   {
      RLOG0(L_ERROR,"wrRimDecRsn: Invalid Len");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   /* Validate the Application ID IE len */
   if ( WR_RIM_LEN_RSN != tmpLen)
   {
      RLOG0(L_ERROR,"wrRimDecRsn: Invalid Len");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }
   WR_RIM_UNPK_U32(rimPdu->val, rimDecCb->rsn.val, (*msgIndx));
   rimDecCb->rsn.pres = PRSNT_NODEF;
   RETVALUE(ROK);
}/* end of wrRimDecRsn */

/** @brief This function unpakcs the PDU indication from the received RIM PDU. 
*
*     Function: wrRimDecPduInd
*
*         Processing steps:
*         - Deocde the PDU indication. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecPduInd
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx
)
{
   U16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == rimDecCb->pduInd.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecPduInd: Multiple instances of PDU IND IE, \
                                                    discarding PDU");
      RETVALUE(RFAILED);
   } 
   if ( (rimPdu->len - *msgIndx) < WR_RIM_MIN_LEN_IEI_LI) 
   {
      RLOG0(L_ERROR,"wrRimDecPduInd: Invalid length, discarding PDU");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   /* Validate the Application ID IE len */
   if ( WR_RIM_LEN_PDU_IND != tmpLen)
   {
      RLOG0(L_ERROR,"wrRimDecPduInd: Invalid length, discarding PDU");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_PDU_IND(rimPdu->val, rimDecCb->pduInd.isAckRequested, 
         rimDecCb->pduInd.pduTypExt, (*msgIndx));

   rimDecCb->pduInd.pres = PRSNT_NODEF;
   RETVALUE(ROK);
}/* end of wrRimDecPduInd */
 
/** @brief This function Decode the RIM protocol version 
*
*     Function: wrRimDecVersion
*
*         Processing steps:
*            - Deocde RIM protocol version. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecVersion
(
 TknStrOSXL                  *rimPdu,
 WrRimEncDecCb               *rimDecCb, 
 S16                         *msgIndx
)
{
   U16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF  == rimDecCb->version.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecVersion: Multiple instaces of Version IE, \
                                                      discarding PDU");
      RETVALUE(RFAILED);
   } 
   if ( (rimPdu->len - *msgIndx) <  WR_RIM_MIN_LEN_IEI_LI )
   {
      RLOG0(L_ERROR,"wrRimDecVersion: Invalid Length \n"); 
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_SEMANT_ERR);
      RETVALUE(RFAILED);
   }
   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   /* Validate the Application ID IE len */
   if ( WR_RIM_LEN_PROT_VER != tmpLen)
   {
      RLOG0(L_ERROR,"wrRimDecVersion: Invalid Length \n"); 
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_SEMANT_ERR);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_U8(rimPdu->val, rimDecCb->version.val, (*msgIndx));
   rimDecCb->version.pres = PRSNT_NODEF;

   RETVALUE(ROK);

}/* end of wrRimDecVersion */

/** @brief This function decodes the SON application identity. 
*
*     Function: wrRimDecSonAppId
*
*         Processing steps:
*            - Deocde SON application identity. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecSonAppId
(
 TknStrOSXL                  *rimPdu,
 WrRimEncDecCb               *rimDecCb,
 S16                         *msgIndx
) 
{
   U16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == rimDecCb->sonAppId.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecSonAppId: Multiple instances of SON AppId \
                                              present, discarding PDU");
      RETVALUE(RFAILED);
   }
   if ( (rimPdu->len - *msgIndx) <  WR_RIM_MIN_LEN_IEI_LI )
   {
      RLOG0(L_ERROR,"wrRimDecSonAppId: Invalid Length");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));
   
   if ( ( rimPdu->len - *msgIndx ) < tmpLen )
   {
      RLOG0(L_ERROR,"wrRimDecSonAppId: Invalid Len while decoding appId");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_COND_IE_ERR);
      RETVALUE(RFAILED);
   }
   /* TODO-FFP: As the SON ID is not decoded as of now move the
      msg Index */
   *msgIndx += tmpLen;

   rimDecCb->sonAppId.pres = PRSNT_NODEF;
   RETVALUE(ROK);
}/* end of wrRimDecSonAppId */

/** @brief This function decodes PDU in error IE. 
*
*     Function: wrRimDecPduInErr
*
*         Processing steps:
*            - Deocde PDU in error IE. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecPduInErr
(
 TknStrOSXL                  *rimPdu,
 WrRimEncDecCb               *rimDecCb,
 S16                         *msgIndx
) 
{
   S16                       tmpLen;
   TknU8                     *cause = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == rimDecCb->pduInErr.pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR,"wrRimDecPduInErr: Multiple instances of PDU inErr IE \
                                              present, discarding PDU");
      RETVALUE(RFAILED);
   }
   if ( (rimPdu->len - *msgIndx) <  WR_RIM_MIN_LEN_IEI_LI )
   {
      RLOG0(L_ERROR, "wrRimDecPduInErr: Invalid Length"); 
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }
   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   if ( ( rimPdu->len - *msgIndx ) < tmpLen )
   {
      RLOG0(L_ERROR, "wrRimDecPduInErr: Invalid Length"); 
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   rimDecCb->pduInErr.pres = PRSNT_NODEF; 
   rimDecCb->pduInErr.len  = tmpLen;

   WR_ALLOC(&(rimDecCb->pduInErr.val), tmpLen);
   if ( NULLP == rimDecCb->pduInErr.val  ) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   WR_RIM_UNPK_STR (rimPdu->val, (&(rimDecCb->pduInErr)), (*msgIndx));
 
   RETVALUE(ROK);
}/* end of wrRimDecPduInErr */

/** @brief This function decodes the application error container IE. 
*
*     Function: wrRimDecAppErrCont
*
*         Processing steps:
*            - Deocde Application error container. 
*
* @param[in] rimPdu     : RIM PDU
* @param[in] msgIndx    : Message Index
* @param[out] rimDecCb  : Decode Cb
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecAppErrCont
(
 TknStrOSXL                  *rimPdu,
 WrRimEncDecCb               *rimDecCb,
 S16                         *msgIndx
) 
{
   U16                       tmpLen;
   TknStrOSXL                *appCntIe = &(rimDecCb->appErrCnt.appCntIe);
   TknU8                     *cause    = &(rimDecCb->decFailCause);

   if ( PRSNT_NODEF == appCntIe->pres )
   {
      /* Discard the PDU, if same IE present twice */
      RLOG0(L_ERROR, "wrRimDecAppErrCont: Multi instances of the  errCnt IE "); 
      RETVALUE(RFAILED);
   }
   if ( (rimPdu->len - *msgIndx) <  WR_RIM_MIN_LEN_IEI_LI )
   {
      RLOG0(L_ERROR, "wrRimDecAppErrCont: Invalid Length"); 
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }
   /* Unpack the length */
   WR_RIM_UNPK_LEN(rimPdu->val, tmpLen, (*msgIndx));

   if ( ( rimPdu->len - *msgIndx ) < tmpLen )
   {
      RLOG0(L_ERROR, "wrRimDecAppErrCont : Invalid Length"); 
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_INVLD_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   /* Decode Application error cause */
   WR_RIM_UNPK_U8(rimPdu->val, rimDecCb->appErrCnt.appCause, (*msgIndx));

   appCntIe->pres = PRSNT_NODEF; 
   appCntIe->len = tmpLen - 1; 
                              
   WR_ALLOC(&(appCntIe->val), appCntIe->len);
   if ( NULLP == appCntIe->val ) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   WR_RIM_UNPK_STR (rimPdu->val, appCntIe, (*msgIndx));
 
   RETVALUE(ROK);
}/* end of wrRimDecAppErrCont */

/** @brief This function validates the RIM container IE 
*
*     Function: wrRimValRimContIe
*
*         Processing steps:
*            - Validate the RIM container and return failure if invalid 
*
* @param[in] rimContIe     : RIM container IE
* @param[in] pduType       : PDU type
*
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimValRimContIe
(
 U8                          rimContIe,  
 U8                          pduType 
)
{
   S16                       ret = ROK;
   switch(pduType)
   {
      case WR_RIM_RIR_PDU:
         {
            if ( WR_RIM_IEI_RIR_CONT != rimContIe) 
            {
               ret = RFAILED;
            }
            break;
         }
      case WR_RIM_RI_PDU:
         {
            if ( WR_RIM_IEI_RI_CONT != rimContIe) 
            {
               ret = RFAILED;
            }
            break;
         }
      case WR_RIM_RIA_PDU:
         {
            if ( WR_RIM_IEI_RIACK_CONT != rimContIe) 
            {
               ret = RFAILED;
            }
            break;
         }
      case WR_RIM_RIAE_PDU:
         {
            if ( WR_RIM_IEI_RIAE_CONT != rimContIe) 
            {
               ret = RFAILED;
            }
            break;
         }
      case WR_RIM_RIE_PDU:
         {
            if ( WR_RIM_IEI_ERR_CONT != rimContIe) 
            {
               ret = RFAILED;
            }
            break;
         }
      default:
         {
            ret = RFAILED;
            RLOG0(L_ERROR,"wrRimValRimContIe: Inavlid pduType");
            break;
         }
   }
   RETVALUE(ret);
}/* end of wrRimValRimContIe */

/** @brief This function Decodes the RIM container. 
*
*     Function: wrRimDecRimCont
*
*         Processing steps:
*         - Validate the length and decode the IEs one by one based on the IE type. 
*
* @param[in] rimPdu   : RIM pdu
* @param[in] msgIndx  : Message Index
* @param[out]rimDecCb : RIM decode CB
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimDecRimCont
(
 WrRimEncDecCb               *rimDecCb, 
 TknStrOSXL                  *rimPdu, 
 S16                         *msgIndx
)
{
   S16                       ret = RFAILED;
   U8                        rimContIEi;
   S32                       len;
   U8                        IEi; 
   TknU8                     *cause = &(rimDecCb->decFailCause);


   if ( (rimPdu->len - *msgIndx) < WR_RIM_MIN_LEN_IEI_LI )
   {
      RLOG0(L_ERROR,"wrRimDecRimCont: Mandatory parameter is missing");

      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_U8(rimPdu->val, rimContIEi, (*msgIndx));
   if ( ROK != wrRimValRimContIe(rimContIEi, rimDecCb->pduType))
   {
      RLOG0(L_ERROR,"wrRimDecRimCont: Invalid rim container IEI");
      RETVALUE(RFAILED);
   }

   WR_RIM_UNPK_LEN(rimPdu->val, len, (*msgIndx));

   if ( (rimPdu->len - *msgIndx) < len )
   {
      RLOG0(L_ERROR,"wrRimDecRimCont: Invalid rim Length");
      RETVALUE(RFAILED);
   }

   while( (rimPdu->len - *msgIndx) > 0 ) 
   {
      WR_RIM_UNPK_U8(rimPdu->val, IEi, (*msgIndx));
      switch(IEi)
      {
         case WR_RIM_IEI_APP_ID: 
            {
               if ( ROK != wrRimDecAppId(rimPdu, rimDecCb, msgIndx ))
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecAppId failed!");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case WR_RIM_IEI_RSN:
            {
               if ( ROK != wrRimDecRsn(rimPdu, rimDecCb, msgIndx))
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecRsn failed!");
                  RETVALUE(RFAILED);
               }
               break;         
            }
         case WR_RIM_IEI_PDU_IND:
            {
               if ( ROK != wrRimDecPduInd(rimPdu, rimDecCb, msgIndx))
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecPduInd failed!");
                  RETVALUE(RFAILED);
               }
               break;         
            }
         case WR_RIM_IEI_PROT_VER:
            {
               if ( ROK != wrRimDecVersion(rimPdu, rimDecCb, msgIndx))
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecVersion failed!");
                  RETVALUE(RFAILED);
               }
               break;      
            }
         case WR_RIM_IEI_RIR_APP_CONT: 
            {
               ret = wrRimDecRirAppCont(rimPdu, rimDecCb, msgIndx );
               if ( ROK != ret )
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecRirAppCont failed!");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case WR_RIM_IEI_RI_APP_CONT: 
            {
               ret = wrRimDecRiAppCont(rimPdu, rimDecCb, msgIndx );
               if ( ROK != ret )
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecRiAppCont failed!");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case WR_RIM_IEI_SON_TRFR_APP_ID: 
            {
               ret = wrRimDecSonAppId(rimPdu, rimDecCb, msgIndx); 
                                         
               if ( ROK != ret )
               {
                  RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecSonAppId - failed!");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case WR_RIM_IEI_PDU_IN_ERR: 
           {
              ret = wrRimDecPduInErr(rimPdu, rimDecCb, msgIndx); 
              if ( ROK != ret )
              {
                 RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecRirPduInErr - failed!");
                 RETVALUE(RFAILED);
              }
              break;
           }
        case WR_RIM_IEI_APP_ERR_CONT:
           {
              ret = wrRimDecAppErrCont(rimPdu, rimDecCb, msgIndx); 
              if ( ROK != ret )
              {
                 RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecAppErrCont - failed!");
                 RETVALUE(RFAILED);
              }
              break;
           }
        case WR_RIM_IEI_CAUSE:
           {
              ret = wrRimDecRimCause(rimPdu, rimDecCb, msgIndx); 
              if ( ROK != ret )
              {
                 RLOG0(L_ERROR,"wrRimDecRimCont:wrRimDecRimCause- failed!");
                 RETVALUE(RFAILED);
              }
              break;
           }
        default:
           {
              RLOG1(L_ERROR,"wrRimDecRimCont: Invalid IEI \
                    Discarding PDU: %d", IEi);
              WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_SEMANT_ERR);
              RETVALUE(RFAILED);
           }
      }
   }
   RETVALUE(ROK);
}/* end of wrRimDecRimCont */

/** @brief This function validates the RIM container IE 
*
*     Function: wrRimValRimContIEs
*
*         Processing steps:
*         - Validate the mandatory and conditional 
*            IEs based on the PDU Type 
*
* @param[in] decCb: RIM Decode Cb.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PRIVATE S16 wrRimValRimContIEs
(
 WrRimEncDecCb               *decCb
)
{

   TknU8                     *cause = &(decCb->decFailCause);
   /* App Id is mandatory in all the RIM PDU,
    * Validate the presence of this IE */
   if ( PRSNT_NODEF != decCb->appId.pres )
   {
      RLOG1(L_ERROR,"wrRimValRimContIEs: Mandatory IE is missing for PDU:%d",
            decCb->pduType);
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
      RETVALUE(RFAILED);
   }
   /*  sonAppId is Conditional in all the RIM PDU -
    *  If appId is SON Transfer Application and SON Transfer
    *  Application Identity IE is not present
    */
   else if (( WR_RIM_APP_SON_TRFR == decCb->appId.val) && 
            ( PRSNT_NODEF != decCb->sonAppId.pres ))
   {
      RLOG0(L_ERROR,"wrRimValRimContIEs: conditional IE is missing");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_COND_IE);
      RETVALUE(RFAILED);
   }
   /* Unexpected conditional IE is received in following cases -
    * - If appId is other than SON Transfer and SON Transfer 
    *   Application Identity IE is present 
    */
   else if (( WR_RIM_APP_SON_TRFR != decCb->appId.val ) &&
            ( PRSNT_NODEF == decCb->sonAppId.pres ))
   {
      RLOG0(L_ERROR,"wrRimValRimContIEs: Unexpected conditional IE");
      WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_UNEXPCD_COND_IE);
      RETVALUE(RFAILED);
   }
   /* Now validate the remaining - mandatory and conditional IEs based on
    * received PDU type from peer */
   switch(decCb->pduType)
   {
      case WR_RIM_RIR_PDU:
         {
            /* Check if any RIR mandatory parameters is missing */
            if ( ( PRSNT_NODEF != decCb->rsn.pres ) ||
                 ( PRSNT_NODEF != decCb->pduInd.pres ) )
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Mandatory IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
               RETVALUE(RFAILED);
            }
            /* Check if Application container IE is missing, when app Id is not
             *  SON transfer  */
            else if (( WR_RIM_APP_SON_TRFR != decCb->appId.val ) &&
                     ( PRSNT_NODEF != decCb->rirAppCont.pres ))
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: conditional IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_COND_IE);
               RETVALUE(RFAILED);
            }
            /* Check if Application container IE is present, when app Id is
             *  SON transfer  */
            else if (( WR_RIM_APP_SON_TRFR == decCb->appId.val) &&
                     ( PRSNT_NODEF == decCb->rirAppCont.pres ))
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Unexpected conditional IE");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_UNEXPCD_COND_IE);
               RETVALUE(RFAILED);
            }

            /* validate PDU extention */
            if ( decCb->pduInd.pduTypExt >= WR_RIM_RIR_RESVD )
            {
               RLOG1(L_ERROR,"wrRimValRimContIEs: Invalid PDU extention:%d",
                     decCb->pduInd.pduTypExt);
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_PDU_NOT_COMPTBL);
               RETVALUE(RFAILED);
            }
         }
         break;
      case WR_RIM_RI_PDU:
         {
            /* Check if any RI mandatory parameters is missing */
            if (( PRSNT_NODEF != decCb->rsn.pres )|| 
                ( PRSNT_NODEF != decCb->pduInd.pres ))
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Mandatory IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
               RETVALUE(RFAILED);
            }
            /* Conditional IE is missing in following cases -
             * If Application ID is not SON transfer then either
             * RI APP container or App error container should be presnet.
             * i.e Both should not be presnet or abset. 
             */
            else if (( WR_RIM_APP_SON_TRFR != decCb->appId.val) &&
                     ((( PRSNT_NODEF != decCb->riAppCont.pres) &&
                       ( PRSNT_NODEF != decCb->appErrCnt.appCntIe.pres)) || 
                      (( PRSNT_NODEF == decCb->riAppCont.pres) &&
                       ( PRSNT_NODEF == decCb->appErrCnt.appCntIe.pres))))
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: conditional IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_COND_IE);
               RETVALUE(RFAILED);
            }
             /* Unexpected Conditional IE 
              * If Application ID is SON transfer then neither
              * RI APP container or App error container should not be presnet 
              */
            else if (( WR_RIM_APP_SON_TRFR == decCb->appId.val) &&
                     (( PRSNT_NODEF == decCb->riAppCont.pres) ||
                      ( PRSNT_NODEF == decCb->appErrCnt.appCntIe.pres)))

            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Unexpected conditional IE");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_UNEXPCD_COND_IE);
               RETVALUE(RFAILED);
            }
            /* validate PDU extention */
            if ( decCb->pduInd.pduTypExt >= WR_RIM_RI_RESVD )
            {
               RLOG1(L_ERROR,"wrRimValRimContIEs: Invalid PDU extention:%d",
                     decCb->pduInd.pduTypExt);
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_PDU_NOT_COMPTBL);
               RETVALUE(RFAILED);
            }
         }
         break;
      case WR_RIM_RIA_PDU: 
         {
            /* Check if any RI mandatory parameters is missing */
            if ( PRSNT_NODEF != decCb->rsn.pres )
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Mandatory IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
               RETVALUE(RFAILED);
            }
         }
         break;
      case WR_RIM_RIE_PDU:
         {
            /* Check if any RI mandatory parameters is missing */
            if (( PRSNT_NODEF != decCb->rimCause.pres ) ||
                ( PRSNT_NODEF != decCb->pduInErr.pres ))
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Mandatory IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
               RETVALUE(RFAILED);
            }
         }
         break;
      case WR_RIM_RIAE_PDU:
         {
            /* Check if any RI mandatory parameters is missing */
            if ( ( PRSNT_NODEF != decCb->rsn.pres )|| 
                 ( PRSNT_NODEF != decCb->pduInd.pres )||
                 ( PRSNT_NODEF != decCb->appErrCnt.appCntIe.pres ))
            {
               RLOG0(L_ERROR,"wrRimValRimContIEs: Mandatory IE is missing");
               WR_RIM_FILL_RIM_CAUSE(cause, WR_RIM_MSING_MNDTRY_IE);
               RETVALUE(RFAILED);
            }
            /* No need of validation of PDU extntion in RIAE, as it is spare */
         }
         break;
      default:
         {
            RLOG0(L_ERROR, " wrRimValRimContIEs: Invalid PDU type is received \
                  for decoding ");
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
} /* end of wrRimValRimContIEs */

/** @brief This function decodes the RIM PDU. 
*
*     Function: wrRimDecPdu
*
*         Processing steps:
*         -  Decode the PDU Type
*         -  Deocde the source and destination cell identifier
*         -  Deocde the RIM container 
*
* @param[in] rimPdu  : RIM Pdu.
* @param[out] rimDecCb: RIM Decode Cb.
* @return  S16
*          -# Success : ROK
*          -# Failure : RFAILED
*/
PUBLIC S16 wrRimDecPdu
(
 TknStrOSXL                  *rimPdu, 
 WrRimEncDecCb               *rimDecCb
)
{
    S16                      ret = RFAILED;
    S16                      msgIndx = 0;

    if ( 0 == rimPdu->len )
    {
      RLOG0(L_ERROR, "wrRimDecPdu: rimPdu->len is zero-Discarding the PDU");
      RETVALUE(RFAILED);
    }

    WR_RIM_UNPK_U8(rimPdu->val, rimDecCb->pduType, msgIndx);
    WR_RIM_VAL_PDU_TYPE(rimDecCb->pduType, ret);
    if ( ROK != ret)
    {
       RLOG1(L_ERROR,"wrRimDecPdu: Invalid PDU type[pduType : %d]",
            rimDecCb->pduType);
       RETVALUE(RFAILED);
    }
    /* Decode Destination cell Identifier */
    if ( ROK != wrRimDecRoutInfo(rimDecCb, rimPdu, &msgIndx, &(rimDecCb->dstAddr)))
    { 
       /* Decode Destination cell Identifier */
       RLOG0(L_ERROR, "wrRimDecPdu: wrRimDecRoutInfo failed-Discarding the PDU");
       RETVALUE(RFAILED);
    }
    /* Decode Source cell Identifier */
    if ( ROK != wrRimDecRoutInfo(rimDecCb, rimPdu, &msgIndx, &(rimDecCb->srcAddr)))
    { 
       /* Decode Source cell Identifier */
       RLOG0(L_ERROR, "wrRimDecPdu: wrRimDecRoutInfo failed-Discarding the PDU");
       RETVALUE(RFAILED);
    }

   /* Decode  Rim Container */
    ret =  wrRimDecRimCont(rimDecCb, rimPdu, &msgIndx);
    if ( ROK != ret)
    {
       /* Decode Destination cell Identifier */
       RLOG0(L_ERROR, "wrRimDecPdu: Rim container decoding failed");
       RETVALUE(RFAILED);
    }
    ret = wrRimValRimContIEs(rimDecCb);
    if ( ROK != ret)
    {
       /* Decode Destination cell Identifier */
       RLOG0(L_ERROR, "wrRimDecPdu: Rim container decoding failed");
       RETVALUE(RFAILED);
    }
    RETVALUE(ROK);
} /* end of wrRimDecPdu */

/********************************************************************30**

           End of file:     fc_emm_rim.c@@/main/Br_Tenb_Rim_Csfb_Intg/7 - Sun Nov  2 23:17:13 2014

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
/main/1        ---      nu         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

