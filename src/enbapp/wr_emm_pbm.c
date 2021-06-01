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
               

     File:     wr_emm_pbm.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=29;

/* header include files (.h) */
#include "wr.h"            /* Common headers and Data Structures */
#include "wr_utils.h"
#include "wr_emm.h"        /* Environment options */
#include "wr_emm_pbm.h"
#include "wr_utils.h"
#include "wr_ifm_rrc.h"
#include "szt_asn.x"
#include "wr_ifm_schd.h"
#include "wr_emm_mme.h"
#include "wr_emm_cfg.h"

PRIVATE S16 wrPbmInsertPagingRecord
(
 U8                     count,
 WrCellCb               *cellCb, 
 WrUePagingMsgInfo      *uePagingInfo
 );

EXTERN U32 numTtiTicks;

EXTERN Void wrPbmPwsStopPagingEtws (WrCellCb *cellCb, WrEtwsPagInfo *pagInfo);

/* Paging Occusions array as per TS 36.304v970 FDD sub frames */
U16 wrPbmPoArr[3][4] = {
   {9, WR_PBM_INVALID_PO, WR_PBM_INVALID_PO, WR_PBM_INVALID_PO},
   {4, 9, WR_PBM_INVALID_PO, WR_PBM_INVALID_PO},
   {0, 4, 5, 9}
};
PRIVATE U16 wrPbmGetSiIndex
(
 U16           siPeriod,
 U8            numSIs,
 WrSISchdInfo  *siSchdInfo
);

PRIVATE U16 wrEmmPbmGetSibPeriod
(
 WrCellCb   *cellCb,
 U8         sibNum
);

PRIVATE U16 wrPbmCalculateTransmitSFN
(
U16          csfn,
U32          siPeriodicity
);


/** @brief This function is used to Modify the PLMN information SIB1
 *
 * @details
 *
 *     Function: wrEmmPbmModPlmnInSib1
 *
 *         Processing steps:
 *         - Traverse the list of PLMNs in SIB1 and activate or deactivate the
 *           PLMN based on the requested PLMN matches perform the requested
 *           action.   
 *
 *
 * @param [in, out] cellCb : cell control block
 * @param [in]      plmnId : PLMN ID  
 * @param [in]    activate : PLMN ID is need to change active or not. 
 * @return S16 
 *    -# Success : ROK 
 */
PRIVATE S16 wrEmmPbmModPlmnInSib1
(
WrCellCb                     *cellCb,
WrPlmnId                     *plmnId,
Bool                         activate
)
{
   U32                       plmnIdx;

   for(plmnIdx = 0; plmnIdx < cellCb->sib1.numPlmnIds; plmnIdx++)
   {
      if (wrUtlPlmnsEqual(&cellCb->sib1.plmns[plmnIdx].plmnId, plmnId) == TRUE)
      {
         cellCb->sib1.plmns[plmnIdx].activated = activate;
         cellCb->sib1.modified = TRUE;
         /*
           MME_LDBAL: set this flag to true.
           Same shall be checked while sending eNB configuration update message
         */
         cellCb->sib1Modified = TRUE;
         if((cellCb->cellState == WR_CELL_STATE_UP)&&(wrEmmCb.adminState == TRUE))
         {
            /* As the plmn is added or deleted we need to update 
             * the system value tag */
            cellCb->sib1.valueTag++;
            cellCb->sib1.valueTag &= WR_EMM_PBM_SIB1_MAX_VALUE_TAG;
            /* As we have modified the Sib1 we are giving the updated sib1 
             * to SCH for broadcasting */
            if(ROK != wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_TRANS_ID))

            {
               RLOG0(L_ERROR, "PBM SIB1 updation to scheduler failed");
               RETVALUE(LCM_REASON_INVALID_PAR_VAL);
            }
            /* Whenever contents of SIB1 changes other than addition/deletion of 
             * SIB10 /SIB11 /SIB12 we need to do Paging for the UEs in order to 
             * read the updated SIB1 */
            if(ROK != wrPbmStartPagingMsg(cellCb,WR_PAGE_SI_TYPE1))
            {
               RLOG0(L_WARNING, "Failed to send Paging for SIB1 Modification");
               RETVALUE(RFAILED);
            }

         }

         break;
      }
   }

   RETVALUE(ROK);
} /* end of wrEmmPbmModPlmnInSib1 */

/** @brief This function will add or delete the PLMN information in
 *         SIB1 for all cell control blocks.
 *
 * @details
 *
 *     Function:wrEmmPbmModifyPlmn 
 *
 *         Processing steps:
 *         - Activate or deactivate the PLMN information in
 *           SIB1 for all cell control blocks.
 *
 * @param [in]   plmnId : PLMN ID.
 * @param [in] activate : specified PLMN ID is change to active or not.
 * @return S16
 *     -# Success : ROK
 */
PUBLIC S16 wrEmmPbmModifyPlmn
(
WrPlmnId                     *plmnId,
Bool                         activate
)
{
   U32                       cellIdx;

   for(cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
   {
      WrCellCb               *cellCb = wrEmmCb.cellCb[cellIdx];
      wrEmmPbmModPlmnInSib1(cellCb, plmnId, activate);
   }

   RETVALUE(ROK);
}

/** @brief This function 
 *
 * @details
 *
 *     Function: wrEmmPbmMibConfig
 *
 *         Processing steps:
 *         - Allocate the memory for RRC Data Request event SDU.
 *         - Build and encode the MIB message.
 *         - Allocate the memory for MAC SI config Request and set the header
 *           fields for the configuration request to scheduler.
 *         - Configure the MAC layer with encoded MIB message by using the
 *           function WrIfmSchdSiCfgReq.
 *
 * @param [in] cellId : cell identity.
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmMibConfig
(
WrCellId                     cellId
)
{
   RgrSiCfgReqInfo           *siCfgReq         = NULLP;
   NhuDatReqSdus             *nhuDatReqEvntSdu = NULLP;
   WrCellCb                  *cellCb;
   U32                       transId;
   WrMibParams               *mib;// = &cellCb->mib;

   WR_GET_CELLCB(cellCb,cellId);
   
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
      RETVALUE(RFAILED);
   }
 
   mib = &(cellCb->mib);
   WR_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
   if(NULLP == nhuDatReqEvntSdu)
   {
      RLOG0(L_FATAL,"Memory Allocation failed");
      RETVALUE(RFAILED);
   }
   WR_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
   if(NULLP == siCfgReq)
   {
      RLOG0(L_FATAL, "Memory Allocation failed");
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   siCfgReq->cellId = cellId;
   siCfgReq->cfgType = RGR_SI_CFG_TYPE_MIB;

   /* Build MIB pdu */
   if (wrSibBldMIBMsg(cellCb, nhuDatReqEvntSdu, 0) != ROK)
   {
      RLOG0(L_ERROR,"Builiding MIB Message failed");
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }   

   /*Encoding of BCCH Message*/
   if (wrBldDlBcchBchPdu(nhuDatReqEvntSdu,&siCfgReq->pdu) != ROK)
   {
      RLOG0(L_ERROR,"Encoding failed for BCCH Message");
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }   

   transId = (cellId << 16 | WR_MIB_TRANS_ID);
   if (WrIfmSchdSiCfgReq(transId, siCfgReq) != ROK)
   {
      RLOG0(L_ERROR,"Failed to send SI-Configuration-Req to Scheduler for MIB Message");
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrEmmPbmMibConfig */

/** @brief This function is used to send/configure the SIB1 message 
 *         to the MAC layer. 
 *
 * @details
 *
 *     Function:wrEmmPbmSib1Config 
 *
 *         Processing steps:
 *         - Allocate the memory for RRC Data Request event SDU.
 *         - Build and encode the SIB1 message.
 *         - Allocate the memory for MAC SI config Request and set the header
 *           fields for the configuration request to scheduler.
 *         - Configure the MAC layer with encoded SIB message by using the
 *           function WrIfmSchdSiCfgReq.
 *
 * @param [in] cellId : cell identity.
 * @return S16
 *     -# Success  : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmSib1Config
(
WrCellId                     cellId,
U16                          cfgType /* PH04_CMAS */
)
{
   RgrSiCfgReqInfo           *siCfgReq         = NULLP;
   NhuDatReqSdus             *nhuDatReqEvntSdu = NULLP;
   WrCellCb                  *cellCb = NULLP;
   U32                       transId = 0;
   MsgLen                    msgLen;

   WR_GET_CELLCB(cellCb,cellId);

   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
      RETVALUE(RFAILED);
   }

   WR_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
   if(NULLP == nhuDatReqEvntSdu)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
   if(NULLP == siCfgReq)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   siCfgReq->cellId = cellId;
   siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1;

   /* Build SIB1 pdu */
   if (wrSibBldSIB1Msg(cellCb, nhuDatReqEvntSdu, 0) != ROK)
   {
      RLOG0(L_ERROR,"Building SIB1 Message failed");
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }   
  
   /*Encoding of DLSCH Message*/
   if (wrBldDlBcchDlSchPdu(nhuDatReqEvntSdu, &siCfgReq->pdu) != ROK)
   {
      RLOG0(L_ERROR,"Encoding failed for DLSCH Message");
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }

   switch(cfgType)
   {
      case WR_SIB1_TRANS_ID:
      {
         transId = (cellId << 16 | WR_SIB1_TRANS_ID);
         SPrntMsg(siCfgReq->pdu,0,0);
         if (SFndLenMsg(siCfgReq->pdu, &msgLen) != ROK)
                   RETVALUE(RFAILED);
         RLOG1(L_INFO,"Configuring SIB1 to MAC(Msg-Len:[%d])", msgLen);

      }
      break;
      case WR_SIB1_PRI_ETWS_TRANS_ID:
      {
         transId = (cellId << 16 | WR_SIB1_PRI_ETWS_TRANS_ID);
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1_PWS; 
         SPrntMsg(siCfgReq->pdu,0,0);
         if (SFndLenMsg(siCfgReq->pdu, &msgLen) != ROK)
                     RETVALUE(RFAILED);
         RLOG1(L_INFO,"Configuring SIB1 with SIB10 to MAC(Msg-Len:[%d])", msgLen);
      }
      break;
      case WR_SIB1_SEC_ETWS_TRANS_ID:
      {
         transId = (cellId << 16 | WR_SIB1_SEC_ETWS_TRANS_ID);
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1_PWS;
         SPrntMsg(siCfgReq->pdu,0,0);
         if (SFndLenMsg(siCfgReq->pdu, &msgLen) != ROK)
                 RETVALUE(RFAILED);
         RLOG1(L_INFO,"Configuring SIB1 with SIB11 to MAC(Msg-Len:[%d])",msgLen);
      }
      break;
      case WR_SIB1_BOTH_ETWS_TRANS_ID:
      {
         transId = (cellId << 16 | WR_SIB1_BOTH_ETWS_TRANS_ID);
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1_PWS;
         SPrntMsg(siCfgReq->pdu,0,0);
         if (SFndLenMsg(siCfgReq->pdu, &msgLen) != ROK)
               RETVALUE(RFAILED);
         RLOG1(L_INFO,"Configuring SIB1 with SIB10 and SIB11 to MAC(Msg-Len:[%d])", msgLen);
      }
      break;
      case WR_SIB1_CMAS_TRANS_ID:
      {
         transId = (cellId << 16 | WR_SIB1_CMAS_TRANS_ID);
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1_PWS;

         /* This code is for debugging purpose only. */
         SPrntMsg(siCfgReq->pdu,0,0);
         if (SFndLenMsg(siCfgReq->pdu, &msgLen) != ROK)
         {
            RLOG1(L_INFO, "Configuring SIB1 with SIB12 to MAC(Msg-Len:[%d])", msgLen);
            RETVALUE(RFAILED);
         }
      }
      break;
      case WR_SIB1_KILL_TRANS_ID:
      {
         transId = (cellId << 16 | WR_SIB1_KILL_TRANS_ID);
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1_PWS;
      }
      break;
      default:
      {
         RLOG1(L_ERROR,"Invalid SIB Configuration type [%d]",cfgType);
      }
      break;
   }

   if (WrIfmSchdSiCfgReq(transId, siCfgReq) != ROK)
   {
      RLOG0(L_ERROR,"Failed to send SI-Configuration-Req to Scheduler for SIB-1 Message");      
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmPbmSib1Config */

/** @brief This function is used to send SI message Stop indication to MAC
 *
 * @details
 *
 *     Function:wrEmmPbmStopSI 
 *
 *         Processing steps:
 *         - Allocate the memory for MAC SI config Request and set the header
 *           fields for the configuration request to scheduler.
 *         - Configure the MAC layer with encoded SI message by using the
 *           function WrIfmSchdSiCfgReq.
 *
 * @param [in] cellId : cell ID.
 * @param [in]  siIdx : SI Index.
 * @return S16
 *     -# Success  : ROK
 *     -# Failure : RFAILED
 */

PRIVATE S16 wrEmmPbmStopSI
(
WrCellId                     cellId,
U8                           siIdx
)
{
   
   RgrSiCfgReqInfo           *siCfgReq         = NULLP;
   WrCellCb                  *cellCb;
   U32                       transId;
   
   
   WR_GET_CELLCB(cellCb,cellId);
   
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
      RETVALUE(RFAILED);
   }
   WR_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
   if(NULLP == siCfgReq)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set the header fields for the configuration request to scheduler    */
   siCfgReq->cellId  = cellId;
   siCfgReq->cfgType = RGR_SI_STOP;
   /*siId = 1 is changed to siId=siIdx+1 due to failure is MAC*/
   siCfgReq->siId    = siIdx +1;

   transId = (cellId << 16 | siIdx);
   
   if (WrIfmSchdSiCfgReq(transId, siCfgReq) != ROK)
   { 
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RLOG0(L_ERROR,"Failed to send SI-Configuration-Req to Scheduler to stop SI");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
   

/** @brief This function is used to send or configure the Particular SI message in SIB1
 *         scheduling information to MAC layer.
 *
 * @details
 *
 *     Function:wrEmmPbmSiConfig 
 *
 *         Processing steps:
 *         - Allocate the memory for RRC Data Request event SDU and build the 
 *           SI message pointed by siIdx in SIB1 Scheduling information and
 *           encode the SI request.
 *         - Allocate the memory for MAC SI config Request and set the header
 *           fields for the configuration request to scheduler.
 *         - Configure the MAC layer with encoded SI message by using the
 *           function WrIfmSchdSiCfgReq.
 *
 * @param [in] cellId : cell ID.
 * @param [in]  siIdx : SI Index.
 * @return S16
 *     -# Success  : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmPbmSiConfig
(
WrCellId                     cellId,
U8                           siIdx
)
{
   RgrSiCfgReqInfo           *siCfgReq         = NULLP;
   NhuDatReqSdus             *nhuDatReqEvntSdu = NULLP;
   WrCellCb                  *cellCb;
   U32                       transId;


   WR_GET_CELLCB(cellCb,cellId);

   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
      RETVALUE(RFAILED);
   }

   WR_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
   if(NULLP == nhuDatReqEvntSdu)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
   if(NULLP == siCfgReq)
   {
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }


   /* Set the header fields for the configuration request to scheduler    */
   siCfgReq->cellId  = cellId;
   siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
   /*siId = 1 is changed to siId=siIdx+1 due to failure is MAC*/
   siCfgReq->siId    = siIdx +1;

   /* Invoke buidling of SI referenced by the siIdx passed                */
   if (wrEmmPbmBldSI(cellCb, nhuDatReqEvntSdu,
                     &(cellCb->sib1.siSchdInfo[siIdx]), 0, 0, siIdx) != ROK)
   {
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RLOG1(L_ERROR,"Builiding SI Message failed for SI-INDEX[%d]",siIdx);
      RETVALUE(RFAILED);
   }   

   /* Encode the SI build                                                 */
   if (wrBldDlBcchDlSchPdu(nhuDatReqEvntSdu, &siCfgReq->pdu) != ROK)
   {
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RLOG1(L_ERROR,"Encoding SI Message failed for for SI-INDEX[%d]",siIdx);
      RETVALUE(RFAILED);
   }

   transId = (cellId << 16 | siIdx);

   if (WrIfmSchdSiCfgReq(transId, siCfgReq) != ROK)
   {
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RLOG1(L_ERROR,"Failed to send SI-Configuration-Req to Scheduler for SI-INDEX[%d]",siIdx);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmPbmSiConfig */


/* PH04_CMAS : Added for CMAS feature. */
/** @brief This function is used to send or configure the Particular SI message in SIB1
 *         scheduling information to MAC layer.
 *
 * @details
 *
 *     Function:wrEmmPbmWrngSiConfig
 *
 *         Processing steps:
 *         - Allocate the memory for RRC Data Request event SDU and build the
 *           SI message pointed by siIdx in SIB1 Scheduling information and
 *           encode the SI request.
 *         - Allocate the memory for MAC SI config Request and set the header
 *           fields for the configuration request to scheduler.
 *         - Configure the MAC layer with encoded SI message by using the
 *           function WrIfmSchdSiCfgReq.
 *
 * @param [in] cellId : cell ID.
 * @param [in]  siIdx : SI Index.
 * @return S16
 *     -# Success  : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmWrngSiConfig
(
WrCellId                     cellId,
U8                           siIdx
)
{
   RgrWarningSiCfgReqInfo    *siCfgReq = NULLP;
   WrCellCb                  *cellCb;
   U32                       transId;
   WrPwsSegmentInfo          *segInfo = NULLP;
   /* ccpu00143127 */
   U8                        addIdx = 0;
   U8                        idx;

   WR_GET_CELLCB(cellCb,cellId);
   if (cellCb == NULLP)
   {
       RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
       RETVALUE(RFAILED);
   }

   if(cellCb->warningInfo == NULLP)
   {
       RLOG0(L_ERROR, "Warning info not present in the cellCb");
       RETVALUE(RFAILED);

   }

   WR_ALLOC(&siCfgReq, sizeof(RgrWarningSiCfgReqInfo));
   if(NULLP == siCfgReq)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* ccpu00143127 */
   for (idx = 0; idx < cellCb->sib1.numSis; idx++)
   {
      if((cellCb->eutranNbrFreqLst.count > 1) && 
            (cellCb->sib1.siSchdInfo[idx].sibs[0] == WR_SIB_5))
      {
         addIdx = 1;
         break;
      }
   }

   /* Set the header fields for the configuration request to scheduler    */
   siCfgReq->cellId  = cellId;
   if (siIdx == WR_PBM_SIB10_SI_INDEX)
   {
      /* ccpu00143127 */
      siCfgReq->siId    = cellCb->warningInfo->sib10SiIdx + addIdx;
      cmLListInit(&(siCfgReq->siPduLst));

      /* Allocate memory for new message segment. */
       WR_ALLOC(&segInfo, sizeof(WrPwsSegmentInfo));
       if (segInfo == NULLP)
       {
          RLOG0(L_FATAL,"Memory allocation failed.");
          WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
          RETVALUE(RFAILED);
       }
       /* Allocate memory for pdu in the new message segment. */
       WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &(segInfo->pdu));
       if (segInfo->pdu == NULLP)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
          WR_FREE(segInfo, sizeof(WrPwsSegmentInfo));
          RETVALUE(RFAILED);
       }

       if(ROK != SCpyMsgMsg(cellCb->warningInfo->etwsInfo.primEtwsInfo.pdu, wrCb.mem.region, wrCb.mem.pool ,
                         &(segInfo->pdu)))
       {
          WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
          WR_FREE(segInfo, sizeof(WrPwsSegmentInfo));
          RLOG0(L_ERROR,"Failed to copy warning message pdu");
          RETVALUE(RFAILED);
       }

       /* Adjust the link node. */
       segInfo->cmasSegPduLstLnk.node = (PTR)segInfo;

       cmLListAdd2Tail(&(siCfgReq->siPduLst),
                                 &(segInfo->cmasSegPduLstLnk));
   }

   if (siIdx == WR_PBM_SIB11_SI_INDEX)
   {
      /* ccpu00143127 */
      siCfgReq->siId    = cellCb->warningInfo->sib11SiIdx + addIdx;
      cmLListInit(&(siCfgReq->siPduLst));
      wrPwsCpySegLst(siCfgReq, &(cellCb->warningInfo->etwsInfo.secEtwsInfo.segPduLst));
   }

   if (siIdx == WR_PBM_SIB12_SI_INDEX)
   {
      /* ccpu00143127 */
      siCfgReq->siId    = cellCb->warningInfo->sib12SiIdx + addIdx;
      cmLListInit(&(siCfgReq->siPduLst));
      wrPwsCpySegLst(siCfgReq, &(cellCb->warningInfo->currentCmas->wrngMsgInfo.segPduLst));
   }

   transId = (cellId << 16 | siIdx);

   if (WrIfmSchdWrngSiCfgReq(transId, siCfgReq) != ROK)
   {
      WR_FREE(siCfgReq, sizeof(RgrWarningSiCfgReqInfo));
      WR_FREE(segInfo, sizeof(WrPwsSegmentInfo));
      RLOG1(L_ERROR,"Failed to send SI-Configuration-Req to Scheduler for SI-INDEX[%d]",siIdx);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmPbmWrngSiConfig */



/* PH04_CMAS : This function is added for CMAS feature. */
PUBLIC S16 wrEmmPbmWrngStopSi
(
WrCellId                     cellId,
U8                           siIdx
)
{
   U32                       transId;

   transId = (cellId << 16 | siIdx);

   if (WrIfmSchdWrngSiStopReq(transId, siIdx) != ROK)
   {
      RLOG1(L_ERROR,"Failed to send Warning SI-Stop-Req to Scheduler for SI-INDEX[%d]",siIdx);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}



/* PH04_CMAS : This function is added for warning messages. */
/** @brief This function is used to send or configure the Particular SI message in SIB1
 *         scheduling information to MAC layer.
 *
 * @details
 *
 *     Function: wrEmmPbmBldEncWrngSeg
 *
 *         Processing steps:
 *         - Allocate the memory for RRC Data Request event SDU and build the
 *           SI message pointed by siIdx in SIB1 Scheduling information and
 *           encode the SI request.
 *         - Allocate the memory for MAC SI config Request and set the header
 *           fields for the configuration request to scheduler.
 *         - Configure the MAC layer with encoded SI message by using the
 *           function WrIfmSchdSiCfgReq.
 *
 * @param [in] cellId : cell ID.
 * @param [in]  siIdx : SI Index.
 * @return S16
 *     -# Success  : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmBldEncWrngSeg
(
WrCellId                     cellId,
U8                           siIdx,
Buffer                       **pdu
)
{
   NhuDatReqSdus             *nhuDatReqEvntSdu = NULLP;
   WrCellCb                  *cellCb;
   WrSISchdInfo              schdInfo;

   WR_GET_CELLCB(cellCb,cellId);
   if (cellCb == NULLP)
   {
       RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
       RETVALUE(RFAILED);
   }

   WR_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
   if(NULLP == nhuDatReqEvntSdu)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* As per design periodicity of warning SI is equal to SIB2 periodicity. */
   schdInfo.period = WR_SIB2_PERIOD;
   schdInfo.numSibs = 1;

   if(siIdx == WR_PBM_SIB12_SI_INDEX)
   {
      schdInfo.sibs[0] = WR_SIB_12;
   }
   else if(siIdx == WR_PBM_SIB11_SI_INDEX)
   {
      schdInfo.sibs[0] = WR_SIB_11;
   }
   else if(siIdx == WR_PBM_SIB10_SI_INDEX)
   {
      schdInfo.sibs[0] = WR_SIB_10;
   }


   /* Invoke buidling of SI referenced by the siIdx passed                */
   if (wrEmmPbmBldSI(cellCb, nhuDatReqEvntSdu,
                     &(schdInfo), 0, 0, siIdx) != ROK)
   {
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      RLOG1(L_ERROR,"Builiding SI Message failed for SI-INDEX[%d]",siIdx);

      RETVALUE(RFAILED);
   }

   /* Encode the SI build                                                 */
   if (wrBldDlBcchDlSchPdu(nhuDatReqEvntSdu, pdu) != ROK)
   {
      RLOG1(L_ERROR,"Encoding SI Message failed for for SI-INDEX[%d]",siIdx);
      /*
       * WR_FREE_EVNT(nhuDatReqEvntSdu); is not required 
       * since nhuDatReqEvntSdu is already freed in wrBldDlBcchBchPdu 
       */
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrEmmPbmBldEncWrngSeg */
                                                                 
/** @brief This function prepares the MIB, SIB1 and SI messages with one or set
 *         of SIBs configures the MAC.
 *
 * @details
 *
 *     Function:wrEmmPbmSibConfig 
 *
 *         Processing steps:
 *         - Prepares the MIB and configures the MAC IM by using 
 *           the function wrEmmPbmMibConfig.
 *         - Prepares the SIB1 message and configures the MAC IM 
 *           by using the function wrEmmPbmSib1Config.
 *         - Prepares the each SI message with one or set of SIBs based on the
 *           updated scheduling information in SIB1 and configures the MAC IM.
 *         - This function invoked by EMM module.
 *
 * @param [in] cellId : cell identity.
 * @return  S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmSibConfig
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;
   U32                       idx;
 
   TRC2(wrEmmPbmSibConfig);

   RLOG0(L_DEBUG,"Filling SIB Config");

   WR_GET_CELLCB(cellCb,cellId);

   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
      RETVALUE(RFAILED);
   }
   /* The first broadcast configureation is MIB.                          */
   if (wrEmmPbmMibConfig(cellId) != ROK)
   {
      RLOG0(L_ERROR,"Not able to configure MIB");
      RETVALUE(RFAILED);
   }
   /* PH04_CMAS : Added last parameter. */
   if (wrEmmPbmSib1Config(cellId, WR_SIB1_TRANS_ID) != ROK)
   {
      RLOG0(L_ERROR,"Not able to configure SIB1");
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < cellCb->sib1.numSis; idx++)
   {
      if((cellCb->eutranNbrFreqLst.count == 1) && 
         (cellCb->sib1.siSchdInfo[idx].sibs[0] == WR_SIB_5))
      {
         RLOG0(L_INFO,"Skipped SIB5 Configuration to MAC as no"
                        "inter frequency configuration exists");
         continue;
      }
      if (wrEmmPbmSiConfig(cellId, idx) != ROK)
      {
         RLOG1(L_ERROR,"Not able to configure SI for SIB [%d]",cellCb->sib1.siSchdInfo[idx].sibs[0]);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is used to fills RRC(NHU) Event Header 
 *
 * @details
 *
 *     Function: wrEmmFillRrcEvntHdr 
 *
 *         Processing steps:
 *         - This function fills the RRC Event header.
 *
 * @param [out] nhuHdr : RRC event header.
 * @param [in]  cellId : cell identiry. 
 * @param [in]   crnti : CRNTI.
 * @param [in] transId : transaction identity.
 * @return S16
 *    -# Success : ROK
 */
PUBLIC S16 wrEmmFillRrcEvntHdr
(
NhuHdr      *nhuHdr,
U16         cellId,
U16         crnti,
U32         transId
)
{

   TRC2(wrEmmFillRrcEvntHdr);
   
   nhuHdr->cellId  = cellId;
   nhuHdr->ueId    = crnti;
   nhuHdr->transId = transId;

   RETVALUE(ROK);
} /* End of wrEmmFillRrcEvntHdr */
/** @brief This function is used to calculate the all possible PFs and POs for
 *         the given T and Nb values. 
 *
 * @details
 *
 *     Function: wrPbmUpdDfltPagCycNb 
 *
 *         Processing steps:
 *         - This function is used to calculate the all possible PFs and POs 
 *           for all UEs for the configured default DRX cycle(T) and Nb.
 *
 * @param [in]           cellCb : cell control block
 * @param [in] dfltPagCycFactor : default paging cycle factor. 
 * @param [in]         nBFactor : nB factor. 
 * @return void 
 */
PRIVATE void wrPbmUpdDfltPagCycNb
(
 WrCellCb    *cellCb, 
 U16         dfltPagCycFactor, 
 U8          nBFactor
 )
{
   U16 dfltPagCycle, nBValue=0;
   U16 nbByT, ueIdByN;
   U16 sfn, ueId;
   U16 N, Ns, i_s;
  

   /* Conver the enums to exact values in ms/radio farmes */  
   WR_GET_PAGING_CYCLE(dfltPagCycFactor, dfltPagCycle);
   WR_GET_NB_VALUE(nBFactor, dfltPagCycle, nBValue);

   /* calculate N */
   N = (dfltPagCycle < nBValue)?(dfltPagCycle):(nBValue);

   /* calculate Ns */
   nbByT = nBValue/dfltPagCycle;
   Ns = (nbByT > 1)?(nbByT):(1);

   for (sfn = 0; sfn < WR_MAX_SFN; sfn++)
   { 
     
      /* Mark the all POs are INVALID_PO */
      cmMemset((U8*)(cellCb->pbmCb.allPfsAndPos[sfn]), 
            WR_PBM_INVALID_PO, 
            (MAX_POSSIBLE_POS * sizeof(U8)));
      for (ueId = 0; ueId < WR_PBM_MAX_UEID; ueId++) 
      {
         /* calculate i_s */
         ueIdByN = ueId/N;
         i_s = ueIdByN % Ns;

#ifndef LTE_TDD
        /* Updating hte PO based on FDD table */
        cellCb->pbmCb.allPfsAndPos[sfn][i_s] = wrPbmPoArr[Ns / 2][i_s];
#else
        /* Following are the values of the paging occasion
         *         for TDD as per 36.304 880 specification */
        if(( i_s == 0))
        {
           cellCb->pbmCb.allPfsAndPos[sfn][i_s] = 0;
        }
        else if(((i_s == 1) && (Ns == 2)) || ((Ns == 4) && (i_s == 2)))
        {
           cellCb->pbmCb.allPfsAndPos[sfn][i_s] = 5;
        }
        else if ((i_s == 1) && (Ns == 4))
        {
           cellCb->pbmCb.allPfsAndPos[sfn][i_s] = 1;
        }
        else if ((i_s == 3) && (Ns == 4))
        {
           cellCb->pbmCb.allPfsAndPos[sfn][i_s] = 6;
        }
#endif

      } /* end of UE_ID loop */
   }/* end of SFN for llop */
   RETVOID;
}

/** @brief This function is used to find SFN for the next modification period.
 *
 * @details
 *
 *     Function: wrCalcNexModPeriodSFN
 *
 *         Processing steps:
 *         - Calculate the next nearest modification period SFN based on the 
 *           modification period co-efficient and Default Paging Cycle in SIB2.
 *
 * @param [in] cellCb : cell control block.  
 * @param [in] curSfn : current SFN 
 * @return U16
 *     # Success : SFN number for the next modification period.
 */
PRIVATE U16 wrCalcNexModPeriodSFN
(
 WrCellCb   *cellCb, 
 U16        curSfn 
)
{
   U8       modifPerdCoeff;
   U16      numOfSfnsPerModifPerd;
   U16      dfltDrxCycle;
   U16      lastModifPerdSfnByNumOfSfnsPerModifPerd;
   
   WR_GET_MODIFI_COEFF(cellCb->sib2.bcchCfg.modifPeriodCoeff, modifPerdCoeff);
   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle, dfltDrxCycle);
      
   numOfSfnsPerModifPerd = (modifPerdCoeff * dfltDrxCycle);
   lastModifPerdSfnByNumOfSfnsPerModifPerd = curSfn/numOfSfnsPerModifPerd;
   RETVALUE ((lastModifPerdSfnByNumOfSfnsPerModifPerd+1)*numOfSfnsPerModifPerd);
} /* end of wrCalcNexModPeriodSFN */ 

/** @brief This function is used to validate the one complete DRX cycle can
 *         fit or not with in the current modification period. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Calculate the next nearest modification period SFN.
 *         - If the difference between current SFN and next nearest modification
 *           period SFN difference is less than default paging cycle value, then 
 *           returns FALSE. Otherwise returns TRUE.
 *
 * @param [in] cellCb : cell control block.
 * @param [in] curSfn : current SFN.
 * @return Bool
 *        TRUE  : if at least one DRX cycle can fit with in the current
 *                modification period.
 *        FALSE : Otherwise.
 *          
 */
PRIVATE Bool wrPbmIsPagDrxCycleFit
(
 WrCellCb    *cellCb, 
 U16         curSfn
 )
{
   U16      dfltDrxCycle;
   U16      nxtModifPerdSfn;

   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle, dfltDrxCycle);
      
   nxtModifPerdSfn = wrCalcNexModPeriodSFN(cellCb, curSfn);
   RETVALUE(((nxtModifPerdSfn-(curSfn + WR_SFN_INCREMENT)) > dfltDrxCycle)?(TRUE):(FALSE));
} /* end of wrPbmIsPagDrxCycleFit */ 

/** @brief This function is used to get the PBM info entry list. 
 *
 * @details
 *
 *     Function:wrPbmGetPendPagMsgLst 
 *
 *         Processing steps:
 *         - Get the Pending Paging message List based on the SFN.
 *
 * @param [in]    sfn : System Frame Number
 * @param [in] cellCb : cell control block. 
 * @return CmLListCp * :Header of the linked list PBM info entries. 
 */
PRIVATE CmLListCp * wrPbmGetPendPagMsgLst
(
 U16         sfn, 
 WrCellCb    *cellCb
)
{
   RETVALUE(cellCb->pbmCb.pbmSchedInfo[sfn]);
}

/** @brief This function is used to validate the pending pbm info events present
 *         or not in pbmSchedInfo table based on pf(sfn), pbmCycleCnt and
 *         PBM inoramation type.
 *
 * @details
 *
 *     Function: wrPbmIsPendingPbmInfoPres
 *
 *         Processing steps:
 *         - Get the Pending Paging Message Lst based on PF.
 *         - Traverse the pbmInfoLst and find the proper entry based on the PF
 *           and pbmCycleCnt and pbmInfoType matches, then returns TRUE. 
 *           Otherwise returns FALSE.
 *
 * @param [in]          pf : Paging Frame.
 * @param [in]      cellCb : Cell Control Block 
 * @param [in] pbmInfoType : PBM Information type.
 * @return Bool
 *          TRUE : If PBM info entry is found in the PBM scheduling table.
 *         FALSE : Not found the PBM info entry.
 */
PRIVATE Bool  wrPbmIsPendingPbmInfoPres
(
 U16            pf, 
 WrCellCb       *cellCb,
 WrPbmInfoType  pbmInfoType
)
{
   CmLListCp    *pbmInfoLstCp = NULLP;
   CmLList      *pbmInfoLnk   = NULLP; 
   WrPbmInfo    *tempPbmInfo  = NULLP;

   /* Get the Pending Paging Message Lst based on PF */
   pbmInfoLstCp = wrPbmGetPendPagMsgLst(pf, cellCb);

   CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);
   /* Traverse the pbmInfoLst and find the proper entry. */
   while (pbmInfoLnk)
   {
      tempPbmInfo = (WrPbmInfo *)(pbmInfoLnk->node);

      if ((tempPbmInfo->pbmInfoType == pbmInfoType) &&
          (tempPbmInfo->pendPagMsg.pf == pf))
      {
         RETVALUE(TRUE);
      }
      /* Move to the next node */
      CM_LLIST_NEXT_NODE(pbmInfoLstCp, pbmInfoLnk);
   } /* end of while */ 
   RETVALUE(FALSE);
} /* end of wrPbmIsPendingPbmInfoPres */

/** @brief This function is used to calculate or get the next appropriate
 *         Paging frame and Paging cycle count.
 *
 * @details
 *
 *     Function: wrPbmGetNxtPfAndPbmCycCnt
 *
 *         Processing steps:
 *         - This function is invoked during at least one paging cycle is not
 *           fits with in the current modification period to trigger SI
 *           information change for broadcasting the SIBs.
 *         - Set the Pf and pbmCycleCnt in next modification period .
 *
 *
 * @param [in]       cellCb : cell control block. 
 * @param [in]       curSfn : current SFN. 
 * @param [out]          pf : paging frame number. 
 * @param [out] pbmCycleCnt : PBM cycle count. 
 * @return void 
 */
PRIVATE void wrPbmGetNxtPfAndPbmCycCnt
(
 WrCellCb   *cellCb,
 U16        curSfn, 
 U16        *pf, 
 Bool       isNxtModPrd
)
{
   U8       modifPerdCoeff;
   
   U16      dfltDrxCycle;
   U16      nxtModifPerdSfn;
   U16      nxtPageSfn;
   U8       repeatNumPagCycles;

   WR_GET_MODIFI_COEFF(cellCb->sib2.bcchCfg.modifPeriodCoeff, modifPerdCoeff);
   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle, dfltDrxCycle);
   repeatNumPagCycles = cellCb->pbmCb.repeatNumPagCycles;

  
   nxtModifPerdSfn = wrCalcNexModPeriodSFN(cellCb, curSfn);
   
   if (isNxtModPrd == TRUE)
   {
      nxtPageSfn = nxtModifPerdSfn + 
         ((modifPerdCoeff - repeatNumPagCycles) * dfltDrxCycle);
      (*pf) = nxtPageSfn % WR_MAX_SFN;
     nxtPageSfn = (nxtPageSfn < curSfn)? (nxtPageSfn + WR_MAX_SFN):
         nxtPageSfn;
   }
   else
   {
      nxtPageSfn = nxtModifPerdSfn - 
          (repeatNumPagCycles * dfltDrxCycle);
      (*pf) = nxtPageSfn % WR_MAX_SFN;
     nxtPageSfn = (nxtPageSfn < curSfn)? (nxtPageSfn + WR_MAX_SFN):
         nxtPageSfn;
   }

   RETVOID;
} /* end of wrPbmGetNxtPfAndPbmCycCnt */ 

/** @brief This function is used to Add the pending PBM Info entry to the
 *         "pbmSchedInfo" table based on the PF for pending paging msg and SFN
 *         for SI change trigger.
 *
 * @details
 *
 *     Function: wrPbmAddPendPbmInfoEntry
 *
 *         Processing steps:
 *         - This functions is used to add the new pending Paging message entry
 *           into one of entry in the pending pbm info table. 
 *
 * @param [in, out] cellCb : cell control block.
 * @param [in]    pbmEntry : PBM info entry.
 * @return void 
 */
PRIVATE void wrPbmAddPendPbmInfoEntry
(
 WrCellCb      *cellCb, 
 WrPbmInfo     *pbmEntry
)
{
   CmLListCp *pbmInfoLstCp = NULLP;
   CmLList      *pbmInfoLnk   = NULLP; 
   WrPbmInfo    *tempPbmInfo  = NULLP;

   TRC2(wrPbmAddPendPbmInfoEntry);

   pbmEntry->lnk.next = NULLP;
   pbmEntry->lnk.prev = NULLP;
   pbmEntry->lnk.node = (PTR)pbmEntry;

   /* Get the Pending PBM Info list for that PF/sfn in "pbmSchedInfo" table */
   pbmInfoLstCp = wrPbmGetPendPagMsgLst(pbmEntry->pendPagMsg.pf, cellCb);
   RLOG2(L_DEBUG,"Adding Paging Entry for Message [%d]" 
         "at Paging Frame [%d]",pbmEntry->pbmInfoType,pbmEntry->pendPagMsg.pf);

   switch (pbmEntry->pbmInfoType)
   {
      /* PH04_CMAS : Added two new cases for CMAS feature.*/
      case WR_PBM_TYPE_START_PAG_SIMOD:
      case WR_PBM_TYPE_START_PAG_ETWS:
      case WR_PBM_TYPE_START_PAG_CMAS:
      case WR_PBM_TYPE_STOP_PAG_SIMOD:
      case WR_PBM_TYPE_STOP_PAG_ETWS:
      case WR_PBM_TYPE_STOP_PAG_CMAS:
      case WR_PBM_TYPE_START_SI_MOD:
      case WR_PBM_TYPE_NORMAL_PAG:
      case WR_PBM_TYPE_CMAS:
      case WR_PBM_TYPE_PRI_ETWS:
      case WR_PBM_TYPE_SEC_ETWS:
      case WR_PBM_TYPE_SIB8:
         {
            /* Insert in sorted order of the Linked List for S1-AP paging msg info */
            /* Also for other case like start or stop paging with 
             * si-change and etws indication (or)
             * pbmInfo belongs to broadcast information */ 
            /* ccpu00142909 */ 
            CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);
            while (pbmInfoLnk)
            {
               tempPbmInfo = (WrPbmInfo *)(pbmInfoLnk->node);

               if ((pbmEntry->pendPagMsg.po <= tempPbmInfo->pendPagMsg.po ))
               {
                  cmLListInsCrnt(pbmInfoLstCp,&pbmEntry->lnk);
                  break;
               }
               else
               {
                  CM_LLIST_NEXT_NODE(pbmInfoLstCp, pbmInfoLnk);
               }
            } /* end of while */ 
            if(NULLP == pbmInfoLnk)
            {
               cmLListAdd2Tail(pbmInfoLstCp, &pbmEntry->lnk);
            }
            break;
         }
   } /* end of switch */
   RETVOID;
} /* end of wrPbmAddPendPbmInfoEntry */

/* Fix for ccpu00139096 */
/** @brief This function is used store paging info of ETWS message 
 *
 * @details
 *
 *     Function: wrPbmSetWaringPagingInfo
 *
 *         Processing steps:
 *         - update pf and po value in warningInfo 
 *
 * @param [in, out] cellCb : cell control block.
 * @param [in]    pbmInfo  : PBM info entry.
 * @param [in]    Type     : ETWS Message Type.
 * @return void 
 */

PRIVATE Void wrPbmSetWaringPagingInfo
(        
 WrCellCb              *cellCb,
 WrPbmInfo             *pbmInfo,
 WrWaringPagingType    type
)
{
   WrEtwsPagInfo   *pagingInfo;

   pagingInfo = &cellCb->warningInfo->etwsInfo.waringPagingInfo[type];
   pagingInfo->pf = pbmInfo->pendPagMsg.pf;
   pagingInfo->po = pbmInfo->pendPagMsg.po;

   RETVOID;
} /*end of wrPbmSetWaringPagingInfo*/

/** @brief This function is used to Start RRC:Paging message for
 *          special event(etws,si-change,etc.,) processing.
 *
 * @details
 *
 *     Function: wrPbmStartPagingMsg
 *
 *         Processing steps:
 *         - Create a "wrPbmInfo" and add it to the "pbmSchInfo" table 
 *           based on current sfn stored in cell control block.
 *         - If the trigger is belongs to etws-indication,then set the 
 *           "pbmInfoType" to WR_PBM_TYPE_START_PAG_ETWS and cycle count
 *           set to 0(i.e,for immiadiate process the paging message as soon
 *           as hit this entry).
 *         - This will be processed in next TTI indication processed in 
 *           PBM module. @sa wrPbmTTIInd.
 *         - If the trigger is belongs to system information change,then 
 *           set the "pbmInfoType" to WR_PBM_TYPE_START_PAG_SIMOD.
 *         - Validate the One complete DRX cycle is possible or not in this
 *           modificaion period. According to that only set the values of set 
 *           the PF and pbmCycleCnt based on curSfn in pbmCb.
 *@code
 *    switch (pageType)
 *    {
 *      case WR_PAGE_ETWS_IND:
 *      {
 *        pbmInfo->pbmInfoType = WR_PBM_TYPE_START_PAG_ETWS;
 *        pbmInfo->pbmCycleCnt = 0;
 *        pbmInfo->pendPagMsg.pf = cellCb->pbmCb.currSfn + WR_SFN_INCREMENT;
 *        break;
 *      }
 *      case WR_PAGE_SYS_SI_CHAGE:
 *      {
 *        pbmInfo->pbmInfoType = WR_PBM_TYPE_START_PAG_SIMOD;
 *        //validate the One complete DRX cycle is possible or not in this
 *        // modificaion period. Based on that set the values "pbmCycleCnt" and
 *        // "PF".
 *        break;
 *      }
 *    }
 *@endcode
 *
 * @param [in, out] cellCb : cell  control block.
 * @param [in]    pageType : Paging message event type like etws-ind or si-chage.
 * @return S16 
 *    -# Ssucces : ROK
 *    -# Failure : RFAILED 
 */
PUBLIC S16 wrPbmStartPagingMsg
(
 WrCellCb      *cellCb, 
 WrPageType    pageType
)
{
   WrPbmInfo                 *pbmInfo      = NULLP;
   U16                       pf;
   U16                       T_sfn = 0;
   U16                       csfn = 0;
   U32                       defaultPagingCycle = 0;
   RgrSiPeriodicity          pws_msg_periodicity = 0;
   U8                        sib2Period = 0;
      
   
   TRC2(wrPbmStartPagingMsg);

   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle,defaultPagingCycle); 
   WR_GET_SIB_PERIOD(cellCb, WR_SIB_2, sib2Period);
   wrEmCellSchdSiPeriodMap(sib2Period,&pws_msg_periodicity);
   
   RLOG1(L_DEBUG,"Start Paging Messagge for Page Type [%d] received",pageType);
   switch (pageType)
   {
      case WR_PAGE_ETWS_IND:
      {
         /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL," Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pbmInfoType = WR_PBM_TYPE_START_PAG_ETWS;
         csfn = cellCb->pbmCb.curSfn;
         T_sfn = wrPbmCalculateTransmitSFN(csfn,pws_msg_periodicity); 
         pbmInfo->pendPagMsg.pf = T_sfn % WR_MAX_SFN;

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
          
         /* Fix for ccpu00139096 */
         wrPbmSetWaringPagingInfo(cellCb, pbmInfo, WR_START_PAG_ETWS);

         /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_ETWS;
         pbmInfo->pendPagMsg.pf = (( T_sfn + defaultPagingCycle ) % 
                                  WR_MAX_SFN);

         /* Fix for ccpu00139096 */
         wrPbmSetWaringPagingInfo(cellCb, pbmInfo, WR_STOP_PAG_ETWS);

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo); 
         
         break;
      }
      /* PH04_CMAS */
      case WR_PAGE_CMAS_IND:
      {
         /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pbmInfoType = WR_PBM_TYPE_START_PAG_CMAS;
         csfn = cellCb->pbmCb.curSfn;
         T_sfn = wrPbmCalculateTransmitSFN(csfn, pws_msg_periodicity); 
         pbmInfo->pendPagMsg.pf = T_sfn % WR_MAX_SFN;

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
          
         /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_CMAS;
         pbmInfo->pendPagMsg.pf = ((T_sfn + defaultPagingCycle) % 
                                  WR_MAX_SFN);
             
         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
         break;
      }
      case WR_PAGE_SYS_SI_CHAGE:
      {
         /* validate the One complete DRX cycle is possible or not
          * in this modificaion period and also configured repeat "n" DRX
          * cycles. Based on these set the value of "pbmCycleCnt" and "PF".
          */
         if ((TRUE == wrPbmIsPagDrxCycleFit(cellCb, cellCb->pbmCb.curSfn)))
         {
            pf = cellCb->pbmCb.curSfn;
            wrPbmGetNxtPfAndPbmCycCnt(cellCb, cellCb->pbmCb.curSfn, 
                                      &pf, FALSE);
         } 
         else 
         {
            wrPbmGetNxtPfAndPbmCycCnt(cellCb, cellCb->pbmCb.curSfn, 
                                      &pf, TRUE);
         }
         /* Perform the lukp in pbmSchedInfo table based on pf as key */
         if ((FALSE == wrPbmIsPendingPbmInfoPres(pf, cellCb,
                                                 WR_PBM_TYPE_START_PAG_SIMOD)))
         {
            WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
            if(NULLP == pbmInfo)
            {
               RLOG0(L_FATAL,"Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            /* fill the pbmInfo entry */
            pbmInfo->pbmInfoType = WR_PBM_TYPE_START_PAG_SIMOD;
            pbmInfo->pendPagMsg.pf = pf; 
            
            /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
            wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
         }
         break;
      }
      case WR_PAGE_SI_TYPE1:
      {  
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         /* fill the pbmInfo entry */
         pbmInfo->pbmInfoType = WR_PBM_TYPE_START_PAG_SIMOD;
         pbmInfo->pendPagMsg.pf = ((cellCb->pbmCb.curSfn + WR_SFN_INCREMENT + 1)
                                   % WR_MAX_SFN); 

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);    
         break;
      }
      default:
      {
        RLOG1(L_ERROR,"Unknown Paging Indication is received:[%d]", 
           pageType);
        RETVALUE(RFAILED);
      }
   } /* end of switch */
   
   RETVALUE(ROK);
} /* end of wrPbmStartPagingMsg */

/** @brief This function is used to calculate or get the next appropriate
 *         Paging frame for Warning messages.
 *
 * @details
 *
 *     Function: wrPbmCalculateTransmitSFN
 *
 *         Processing steps:
 *         - This function is invoked to calculate at SFN at which 
 *           paging should start for Warning messages
 *         - Set the Paging frame in sync with SIB transmission .
 *
 *
 * @param [in]       curSfn           : current SFN.
 * @param [in]       siPeriodicity    : Periodicity of SIB10/11/12.
 * @return           T_sfn            : SFN at which paging should begin. 
 */

PRIVATE U16 wrPbmCalculateTransmitSFN
(
U16         csfn, 
U32         siPeriodicity
)
{
    U16 x     = 0;
    U16 T_sfn = 0;
    
    TRC2(wrPbmCalculateTransmitSFN);


    if( csfn > 0)
    {
        x  = (csfn/siPeriodicity)+1;
    }
    else
    {
       x  = 1;
    }
    T_sfn = siPeriodicity * x;
    if( T_sfn > csfn + WR_PBM_PWS_5SFN_OFFSET)
    {
        T_sfn = T_sfn;
    }
    else
    {
        T_sfn = T_sfn + siPeriodicity;
    }
    RLOG1(L_DEBUG,"Calculated TransmitSFN [%d]",T_sfn);
    RETVALUE(T_sfn);     
}/* end of wrPbmCalculateTransmitSFN */


/* PH04_CMAS : This function is added for CMAS feature. */
PUBLIC S16 wrPbmStopPagingMsg
(
 WrCellCb      *cellCb,
 WrPageType    pageType
)
{
   WrPbmInfo *pbmInfo      = NULLP;


   TRC2(wrPbmStopPagingMsg);

   /* Add a new entry in the frame table for stoping the paging. */
   RLOG1(L_DEBUG,"Stop Paging Messagge for Page Type [%d] received",pageType);

   switch (pageType)
   {
      case WR_PAGE_ETWS_IND:
      {
          /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_ETWS;
         pbmInfo->pendPagMsg.pf = ( cellCb->pbmCb.curSfn + 5) % WR_MAX_SFN;

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
        break;
      }
      /* PH04_CMAS */
      case WR_PAGE_CMAS_IND:
      {
          /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL," Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_CMAS;
         pbmInfo->pendPagMsg.pf = (cellCb->pbmCb.curSfn + 
                                    WR_PBM_PWS_5SFN_OFFSET) % WR_MAX_SFN;

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
         break;
      }
      default:
      {
        RLOG1(L_ERROR,"Unknown Paging Indication is received:[%d]",
           pageType);
        RETVALUE(RFAILED);
      }
   } /* end of switch */

   RETVALUE(ROK);
} /* end of wrPbmStopPagingMsg */

 /* @details
 *
 *     Function: wrPbmSib8InsertRecord
 *
 *         Processing steps:
 *         - This functions is used to add the sib8 Paging message entry
 *           into one of entry in the pending pbm info table. 
 *
 * @param [in, out] cellCb : cell control block.
 * @param [in]    pbmEntry : PBM info entry.
 * @return void 
 */
PUBLIC S16 wrPbmSib8InsertRecord
(
 WrCellCb       *cellCb, 
 WrPbmInfo      *pbmInfo
)
{
   /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
   wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);    
   RETVALUE(ROK);
}

/* PH04_CMAS : This function is added as part of CMAS design change. */
PUBLIC S16 wrPbmStartWarningMsg
(
 WrCellCb      *cellCb, 
 WrWarningType    pageType
)
{
   WrPbmInfo                 *pbmInfo      = NULLP;
   U16                       T_sfn=0,x=0,csfn=0;
   RgrSiPeriodicity          siPeriodicity= 0;
   U32                       defaultValue = 0;
   U8                        sib2Period = 0;
      
   
   TRC2(wrPbmStartWarningMsg);
   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle,defaultValue); 

   WR_GET_SIB_PERIOD(cellCb, WR_SIB_2, sib2Period);
   wrEmCellSchdSiPeriodMap(sib2Period,&siPeriodicity);
   /* allocate memory pbmInfo entry */
   WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
   if(NULLP == pbmInfo)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* this x is multiplication factor to find the nearest SFN */
   csfn = cellCb->pbmCb.curSfn;
   if( csfn > 0)
   {
      x  = (csfn/siPeriodicity)+1;
   }
   else
   {
      x  = 1;
   }
   T_sfn = siPeriodicity*x;
   if( T_sfn > csfn + 5)
   {
      T_sfn = T_sfn;
   }
   else
   {
      T_sfn = T_sfn + siPeriodicity;
   }
   pbmInfo->pendPagMsg.pf = ((T_sfn + defaultValue)% WR_MAX_SFN);

   switch (pageType)
   {
      case WR_START_PRI_ETWS_IND:
      {
         pbmInfo->pbmInfoType = WR_PBM_TYPE_PRI_ETWS;

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         /* Fix for ccpu00139096 */
         wrPbmSetWaringPagingInfo(cellCb, pbmInfo, WR_START_PAG_PRI_ETWS);

         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
         break;
      }
      case WR_START_SEC_ETWS_IND:
      {
         pbmInfo->pbmInfoType = WR_PBM_TYPE_SEC_ETWS;

         /* Fix for ccpu00139096 */
         wrPbmSetWaringPagingInfo(cellCb, pbmInfo, WR_START_PAG_SEC_ETWS);

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
         break;
      }
      case WR_START_BOTH_ETWS_IND:
      {
         /* Fix for ccpu00139096 */
         wrPbmSetWaringPagingInfo(cellCb, pbmInfo, WR_START_PAG_PRI_ETWS);

         pbmInfo->pbmInfoType = WR_PBM_TYPE_PRI_ETWS;
         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);

         /* allocate memory pbmInfo entry */
         WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo)); 
         if(NULLP == pbmInfo)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         pbmInfo->pendPagMsg.pf = ((T_sfn + defaultValue)% WR_MAX_SFN );
         /* Fix for ccpu00139096 */
         wrPbmSetWaringPagingInfo(cellCb, pbmInfo, WR_START_PAG_SEC_ETWS);

         pbmInfo->pbmInfoType = WR_PBM_TYPE_SEC_ETWS; 
         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
 
         break;
      }
      /* PH04_CMAS */
      case WR_START_CMAS_IND:
      {
         pbmInfo->pbmInfoType = WR_PBM_TYPE_CMAS;

         /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
         wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
         break;
      }
      default:
      {
        RLOG1(L_ERROR,"Unknown Paging Indication is received:[%d]",
           pageType);
        WR_FREE(pbmInfo, sizeof(WrPbmInfo));
        RETVALUE(RFAILED);
      }
   } /* end of switch */
   RETVALUE(ROK);
} /*wrPbmStartWarningMsg*/



/** @brief This function is used to calculate and fill the PF and PO 
 *         based on uePagingInfo. 
 *
 * @details
 *
 *     Function:wrPbmCalcPfAndPo 
 *
 *         Processing steps:
 *         - This function calculate and fill the paging fram and paging
 *           occusion in UEPagingInfo.
 *
 * @param [in]            cellCb : cell control block.
 * @param [in, out] uePagingInfo : UE specific paging inforamtion.
 * @return void 
 */
PRIVATE void wrPbmCalcPfAndPo 
(
 WrCellCb               *cellCb,
 WrUePagingMsgInfo      *uePagingInfo
)
{
   U32         ueSpecificValue = 0;
   U32         defaultValue = 0;
   U16         T = 0, N = 0, Ns = 0, Nb = 0;
   U16         pf = 0, i_s = 0, po = 0, x = 0 , csfn, sfn = 0, T_sfn;

   TRC2(wrPbmCalcPfAndPo);
   /*
    * Fill the Value of T (DRX cycle of the UE)
    * T = min(UE Specific DRX value allocated by upper layers, default DRX
    * broadcast in System Information) i.e. min( ueSpcPagDrx, defaultValue)
    */
   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle, defaultValue);
   if(uePagingInfo->pagingDrxPres == FALSE)
   {
      uePagingInfo->T = T =  defaultValue;
   }
   else
   {
      WR_GET_PAGING_CYCLE(uePagingInfo->ueSpcPagDrx, ueSpecificValue);
      T = (ueSpecificValue < defaultValue) ? ueSpecificValue : defaultValue;
      uePagingInfo->T = T;
   }

   /* 
    *  Fill the Value of N 
    *  N = min( T, nB)
    *  i.e. min(T, wrDfltPCCHCfgNB)
    */
   WR_GET_NB_VALUE(cellCb->sib2.pcchCfg.nB, T, Nb);
   N = ( T < Nb) ? T : Nb;  
   uePagingInfo->N = N;

   /* 
    * Fill the Value of Ns 
    * Ns = max(1, nB/T)
    */
   Ns  = ((Nb / T) > 1 ) ? (Nb / T) : 1;

   /* Fill the Value of pf */
   pf = ( T / N ) * ((uePagingInfo->ue_Id) % N);
   /*
    * if T is 128, PF 20 and current system frame number is 200
    * next system frame where eNb can broadcast the paging records is 276
    * PF is when (SFN % T) == (T/N)*(UE_ID%N)
    */
   csfn = cellCb->pbmCb.curSfn;
   /* here is the boundary condition
    * even if CSFN is 200, PF is 204 then postpone to next DRX cycle 
    * because we are already processing for PF 204 and eNB got paging 
    * request whose PF is 204 thats the reason always doing +1
    */
    /* this x is multiplication factor to find the nearest SFN */
   if( csfn > pf)
   {
      x  = ((csfn - pf)/T)+1;
   }
   else
   {
      x  = ((pf - csfn)/T)+1;
   }
   T_sfn = pf + T*x;
   /* if csfn is less than pf
    * for example csfn is 2, pf is 100 and T is 128
    * sfn should be 100, but T_sfn is 228 */
   if(csfn <= pf)
   {
      if(pf > csfn + WR_SFN_INCREMENT)
      {
          sfn = pf;
      }
     else
     {
          sfn = T_sfn;
     }
   }
   /* if csfn between 224 - 227, pf is 100 and T is 128
    * sfn should be 356, but T_sfn is 228
    * so, make sfn as T_sfn + T */
   else
   {
      if( T_sfn > csfn + WR_SFN_INCREMENT)
      {
         sfn = T_sfn;
      }
      else
      {
         T_sfn = T_sfn + T;
        sfn = T_sfn;
      }
   }
   /* wrap around for 1024 */
   uePagingInfo->pf = (sfn % 1024);

   /* Fill the Value of po */
   i_s = (((uePagingInfo->ue_Id/ N)) % Ns);
#ifndef LTE_TDD
   /* Updating hte PO based on FDD table */
   po = wrPbmPoArr[Ns / 2][i_s];
#else
   /* Following are the values of the paging occasion
    *         for TDD as per 36.304 880 specification */
   if(( i_s == 0))
   {
      po = 0;
   }
   else if(((i_s == 1) && (Ns == 2)) || ((Ns == 4) && (i_s == 2)))
   {
      po = 5;
   }
   else if ((i_s == 1) && (Ns == 4))
   {
      po = 1;
   }
   else if ((i_s == 3) && (Ns == 4))
   {
      po = 6;
   }
#endif
   uePagingInfo->po = po;

   RETVOID;
} /* end of wrPbmCalcPfAndPo */

/** @brief This function is used to update the Paging Record infor in pending
 *         paging message depending on Priority and Time of arrival. 
 *
 * @details
 *
 *     Function: wrPbmInsertPagingRecOnPriBased 
 *
 *         Processing steps:
 *          - This function will check if there are any entry which has less
 *          paging priority than the received one. If yes, move the same into
 *          next paging occasion. 
 *          - It will also considers the time of arrival for moving the 
 *          paging message to next occasion
 *          - Store the new paging record in the place from which paging record
 *          is moved to next paging occasion
 *
 * @param [in] uePagingInfo : UE Paging information.
 * @param [out]     pbmInfo : PBM information.
 * @return S16 
 *    ROK : Successfully updated the paging record information.
 */
PRIVATE S16 wrPbmInsertPagingRecOnPriBased
(
 U8                     count,
 WrCellCb               *cellCb,
 WrPbmInfo              *pbmInfo,
 WrUePagingMsgInfo      *uePagingInfo
)
{
   U8               numPagRcrds = 0;
   U32              selPagIndex = 0;
   U32              loopIndx;
   U32              selPagTimeDiff = WR_MAX_PAGING_TTI_TIME;
   U32              timeDiff;
   U32              selPagPriority = 0;
   U32              isPagingRecSlectd = FALSE;
   U32              ret = ROK;
   WrPagingRecord   tmpPagingRecord;

   TRC2(wrPbmInsertPagingRecOnPriBased);

   numPagRcrds = pbmInfo->pendPagMsg.numPagRcrds;

   for(loopIndx = 0; loopIndx < numPagRcrds; loopIndx++)
   {
      /* Check if the paging record has the low priority than the one received
       * Also, check if already selected record has the high priority than the
       * paging record which is present. If so, check if this record is
       * recently received, if yes, select this record for moving it to
       * next paging cycle.
       */
      if((pbmInfo->pendPagMsg.pagRec[loopIndx].pagPriority > 
                                           uePagingInfo->pagPriority) &&
         (pbmInfo->pendPagMsg.pagRec[loopIndx].pagPriority >= selPagPriority))
      {
         if(numTtiTicks >= pbmInfo->pendPagMsg.pagRec[loopIndx].arrvTimeInfo)
         {
            timeDiff = numTtiTicks - 
                           pbmInfo->pendPagMsg.pagRec[loopIndx].arrvTimeInfo;
         }
         else
         {
            timeDiff = ((numTtiTicks - 
                         pbmInfo->pendPagMsg.pagRec[loopIndx].arrvTimeInfo) +
                         WR_MAX_PAGING_TTI_TIME);
         }
         /* If the time difference is less, means the paging record is received
          * recently than the already selected one. If it is equal, as we
          * insert in the 0 to max, select the last one. It might be possible
          * that the last one is received earlier than the one before, but
          * we are considering 10MS window and will treat all the paging 
          * messags received within that window are received at same time
          */
         if(timeDiff <= selPagTimeDiff)
         {
            selPagIndex = loopIndx;
            selPagTimeDiff = timeDiff;
            selPagPriority = pbmInfo->pendPagMsg.pagRec[loopIndx].pagPriority;
            isPagingRecSlectd = TRUE;
         }
      }
   }

   /* Calculate the next paging cycle for moving the paging into next 
    * cycle
    */
   uePagingInfo->pf = ((uePagingInfo->pf + uePagingInfo->T)%1024);

   /* Check if we have selected the paging record which can be moved to next 
    * cycle. If so, move it to next cycle and keep the newly came record in 
    * the index place. If we have not found any, pass this new record to
    * next cycle and increment the count
    */
   if(isPagingRecSlectd)
   {
      /* Store the exiting paging record information in the temp variable from
       * the selected index which needs to be moved to next paging cycle
       */
      cmMemcpy((U8*) &tmpPagingRecord, 
               (U8*)&pbmInfo->pendPagMsg.pagRec[selPagIndex],
               sizeof(WrPagingRecord));

      /* Store the newly received paging information in the index selected
       * as the received is high priority than one received earlier
       */
      /* Update Ue Identity type in Paging Record */
      pbmInfo->pendPagMsg.pagRec[selPagIndex].ueIdType = 
         uePagingInfo->ueIdenType;
      /* Update Domain indication in Paging Record */
      pbmInfo->pendPagMsg.pagRec[selPagIndex].domIndType = 
         uePagingInfo->domIndType;
      /* copy the UE Identity in to Paging Record */   
      cmMemcpy((U8 *)&(pbmInfo->pendPagMsg.pagRec[selPagIndex].ueId),
            (U8 *)&(uePagingInfo->ueIden), 
            sizeof(uePagingInfo->ueIden));
      /* Store the time when eNB received the paging message from MME. This
       * is used to preempt the paging record when high priority pag message
       * are received
       */
      pbmInfo->pendPagMsg.pagRec[selPagIndex].arrvTimeInfo = 
         (numTtiTicks % WR_MAX_PAGING_TTI_TIME);
      /* Store the paging priority received from MME */
      pbmInfo->pendPagMsg.pagRec[selPagIndex].pagPriority = 
         uePagingInfo->pagPriority;

      /* Copy back the information of paging which needs to be moved out from
       * the paging record to next cycle 
       */
      uePagingInfo->ueIdenType = tmpPagingRecord.ueIdType;
      uePagingInfo->domIndType = tmpPagingRecord.domIndType;
      uePagingInfo->arrvTimeInfo = tmpPagingRecord.arrvTimeInfo;
      uePagingInfo->pagPriority = tmpPagingRecord.pagPriority;

      cmMemcpy((U8 *)&(uePagingInfo->ueIden), (U8*) &tmpPagingRecord.ueId, 
               sizeof(uePagingInfo->ueIden));
      /* Call the insert function for placing the paging record into next 
       * available paging cycle
       */
      ret = wrPbmInsertPagingRecord(count, cellCb, uePagingInfo);
   }
   else
   {
      count++;
      ret = wrPbmInsertPagingRecord(count, cellCb, uePagingInfo);
   }

   RETVALUE(ret);
} /* end of wrPbmInsertPagingRecOnPriBased */



/** @brief This function is used to update the Paging Record infor in pending
 *         paging message. 
 *
 * @details
 *
 *     Function: wrPbmUpdatePagingRecord 
 *
 *         Processing steps:
 *         - This function will add the a new UE paging record to the pending
 *           Paging message information.
 *
 * @param [in] uePagingInfo : UE Paging information.
 * @param [out]     pbmInfo : PBM information.
 * @return S16 
 *    ROK : Successfully updated the paging record information.
 */
PRIVATE S16 wrPbmUpdatePagingRecord
(
 WrUePagingMsgInfo      *uePagingInfo,
 WrPbmInfo              *pbmInfo
)
{
   U8     numPagRcrds = 0;

   TRC2(wrPbmUpdatePagingRecord);

   numPagRcrds = pbmInfo->pendPagMsg.numPagRcrds;
   /* Update Ue Identity type in Paging Record */
   pbmInfo->pendPagMsg.pagRec[numPagRcrds].ueIdType = 
                              uePagingInfo->ueIdenType;
   /* Update Domain indication in Paging Record */
   pbmInfo->pendPagMsg.pagRec[numPagRcrds].domIndType = 
                  uePagingInfo->domIndType;
   /* copy the UE Identity in to Paging Record */   
   cmMemcpy((U8 *)&(pbmInfo->pendPagMsg.pagRec[numPagRcrds].ueId),
            (U8 *)&(uePagingInfo->ueIden), 
            sizeof(uePagingInfo->ueIden));
   /* Store the time when eNB received the paging message from MME. This
    * is used to preempt the paging record when high priority pag message
    * are received
    */
   pbmInfo->pendPagMsg.pagRec[numPagRcrds].arrvTimeInfo = 
                                    (numTtiTicks % WR_MAX_PAGING_TTI_TIME);
   /* Store the paging priority received from MME */
   pbmInfo->pendPagMsg.pagRec[numPagRcrds].pagPriority = 
                                           uePagingInfo->pagPriority;
   /* Increase the num of paging records in this message */
   (pbmInfo->pendPagMsg.numPagRcrds)++;

   RETVALUE(ROK);
} /* end of wrPbmUpdatePagingRecord */

/** @brief This function is used to insert a pageing record in already existing
 *         entry or in new entry.
 *
 * @details
 *
 *     Function:wrPbmInsertPagingRecord 
 *
 *         Processing steps:
 *         - This function calls the function "wrPbmCreatePagingRecord"  or calls recursively
 *           max records exeeds in a PF. this postpones to next DRX cycle
 *           to find the appropriate Pending PBM Info Lst is present or not for the
 *           given PF.
 *           @sa "wrPbmCreatePagingRecord/wrPbmInsertPagingRecord"
 *         - If the Pending PBM Info list is present in the "pbmSchInfo" table,
 *           then traverse the list find that is entry present or not with same
 *           PF and PO.
 *@code
 *            - If "pendPagMsg" entry is present 
 *               - if  "numPagRcrds" < WR_PBM_MAX_PAGING_RECORDS 
 *                    then update the already existed entry with new paging record information. 
 *                 else 
 *                    postpone to next DRX Cycle and call wrPbmInsertPagingRecord. Because next
 *                    SFN might be already having MAX PAGING RECORDs
 *@endcode
 *         - Ohterwise, allocate memory for pending Paging message entry 
 *           "WrPbmInfo" and insert to the "pbmSchInfo" table in the appropriate 
 *           position by using the function wrPbmAddPendPbmInfoEntry.
 *           @sa wrPbmAddPendPbmInfoEntry
 *
 * @param [in, out]    cellCb : cell control block  
 * @param [in]   uePagingInfo : Ue specific Paging information.
 * @return S16
 *      ROK : Succfully created a paging Record.
 *  RFAILED : Not able to create a Paging record 
 *           due to memory failure.
 */

PRIVATE S16 wrPbmInsertPagingRecord
(
 U8                     count,
 WrCellCb               *cellCb, 
 WrUePagingMsgInfo      *uePagingInfo
 )
{

   CmLListCp *pbmInfoLstCp = NULLP;
   CmLList   *pbmInfoLnk   = NULLP; 
   WrPbmInfo *pbmInfo      = NULLP;
   U8         max_iterations = 1024/uePagingInfo->T;

   TRC2(wrPbmInsertPagingRecord);

   RLOG_ARG1(L_INFO,DBG_CELLID, cellCb->cellId,
      "Received Paging for MT-Access, S-TMSI = [0x%x]",
      uePagingInfo->ueIden.sTMSI.mTMSI);
   /* Get the Pending Paging Message Lst based on PF */
   if(count >= max_iterations)
   {
      RLOG0(L_ERROR,
      "wrPbmInsertPagingRecord is failed to find a free slot");
      RETVALUE(RFAILED);
   }
   pbmInfoLstCp = wrPbmGetPendPagMsgLst(uePagingInfo->pf, cellCb);

   CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);
   /* Traverse the pbmInfoLst and update the paging record based on PF and PO
   */
   while (pbmInfoLnk)
   {
      pbmInfo = (WrPbmInfo *)(pbmInfoLnk->node);

      if (pbmInfo && 
            (pbmInfo->pbmInfoType == WR_PBM_TYPE_NORMAL_PAG) &&
            (pbmInfo->pendPagMsg.pf == uePagingInfo->pf) &&
            (pbmInfo->pendPagMsg.po == uePagingInfo->po))
      {
         if(pbmInfo->pendPagMsg.numPagRcrds < WR_PBM_MAX_PAGING_RECORDS)
         {
            /* Update the Paging Record information */
            wrPbmUpdatePagingRecord(uePagingInfo, pbmInfo);
            RETVALUE(ROK);
         }
         else 
         {
            /* create another entry and insert into the pbmSchTable 
             * at approprioate position is in the next DRX cycle (PF+T)
             */
            if(ROK == wrPbmInsertPagingRecOnPriBased(count, cellCb, pbmInfo, 
                                                     uePagingInfo))
            {
               RETVALUE(ROK);
            }
            else
            {  
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

         } /* end of else part */
      } /* end of if */
      /* Move to the next node */
      CM_LLIST_NEXT_NODE(pbmInfoLstCp, pbmInfoLnk);
   } /* end of while, to traverse the pbmInfoLst */

   /* if there is no match of PF and PO in the List,then allocate,update and 
    * insert the new pending paging message to the current List
    */
   WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
   if(NULLP == pbmInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   pbmInfo->pbmInfoType = WR_PBM_TYPE_NORMAL_PAG;
   pbmInfo->pendPagMsg.pf = uePagingInfo->pf;
   pbmInfo->pendPagMsg.po = uePagingInfo->po;

   /* Initilize the num of paging records to 0 */
   pbmInfo->pendPagMsg.numPagRcrds = 0;
   /* Update the Paging Record information in pending paging msg */
   wrPbmUpdatePagingRecord(uePagingInfo, pbmInfo);

   /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
   wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);

   RETVALUE(ROK);
}

/** @brief This function is used to create a pageing record in already existing
 *         entry or in new entry.
 *
 * @details
 *
 *     Function:wrPbmCreatePagingRecord 
 *
 *         Processing steps:
 *         - This function calls the function "wrPbmGetPendPagMsgLst". 
 *           to find the appropriate Pending PBM Info Lst is present or not for the
 *           given PF.
 *           @sa "wrPbmGetPendPagMsgLst"
 *         - If the Pending PBM Info list is present in the "pbmSchInfo" table,
 *           then traverse the list find that is entry present or not with same
 *           PF and PO.
 *@code
 *            - If "pendPagMsg" entry is present 
 *               - if  "numPagRcrds" < WR_PBM_MAX_PAGING_RECORDS 
 *                    then update the already existed entry with new paging record information. 
 *                 else 
 *                    create a new entry with PF = (SFN + T) % MAX_TABLE_ETRYS; 
 *                    PO is same as the calculated one.
 *                    i.e.,adjust the "pageCycleCnt" to 
 *                    pageCycleCnt += ((SFN + GET_NUM_SFNS_PER_T(T))/MAX_TABLE_ETRYS);
 *@endcode
 *         - Ohterwise, allocate memory for pending Paging message entry 
 *           "WrPbmInfo" and insert to the "pbmSchInfo" table in the appropriate 
 *           position by using the function wrPbmAddPendPbmInfoEntry.
 *           @sa wrPbmAddPendPbmInfoEntry
 *
 * @param [in, out]    cellCb : cell control block  
 * @param [in]   uePagingInfo : Ue specific Paging information.
 * @return S16
 *      ROK : Succfully created a paging Record.
 *  RFAILED : Not able to create a Paging record 
 *           due to memory failure.
 */
PRIVATE S16 wrPbmCreatePagingRecord
(
 WrCellCb               *cellCb, 
 WrUePagingMsgInfo      *uePagingInfo
 )
{
   U8  count = 0;

   TRC2(wrPbmCreatePagingRecord);

   /* calcuate and update the PF and PO in Ue Paging info*/
   wrPbmCalcPfAndPo(cellCb, uePagingInfo);

   wrPbmInsertPagingRecord(count,cellCb,uePagingInfo);

   RETVALUE(ROK);
}/* end of wrPbmCreatePagingRecord */

/** @brief This function is used to fill the RRC paging message. 
 *
 * @details
 *
 *     Function: wrFillRRCPagingMsg
 *
 *         Processing steps:
 *         - This function prepares the RRC: Paging message with one or multiple
 *           paging records which belongs same PF and PO.
 *
 *
 * @param [in]       cellCb : cell control block. 
 * @param [in]    pagMsgEnt : paging message info. 
 * @param [out]  datReqSdus : RRC paging message.
 * @return S16 
 *   -# Success : ROK
 *   -# FAilure : RFAILED
 */
PRIVATE S16 wrFillRRCPagingMsg
(
 WrCellCb         *cellCb,
 WrPagingMsgInfo  *pagMsgEnt,
 NhuDatReqSdus    *datReqSdus 
)
{
   NhuPCCH_Msg *pcchMsg = NULLP;
   NhuPag      *page    = NULLP;
   U8          idx, idx1;


   TRC2(wrFillRRCPagingMsg);

   pcchMsg = &(datReqSdus->sdu.ueMsg.msg.cmnMsg.cmn.pcchMsg);

   WR_FILL_TKN_UINT(pcchMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(pcchMsg->message.choice,0);

   if(!(pcchMsg->message.choice.val))
   {
      WR_FILL_TKN_UINT(pcchMsg->message.val.c1.choice,0);
   
      page = &(pcchMsg->message.val.c1.val.paging);

      WR_FILL_TKN_UINT(page->pres, PRSNT_NODEF);
      
      /* Fill is there any SI change is present                              */
      if (cellCb->pbmCb.siModPres)
      {
         WR_FILL_TKN_UINT(page->systemInfoModfn,
                          NhuPagsystemInfoModfntrueEnum);
      } else {
         page->systemInfoModfn.pres = NOTPRSNT; 
      }
     
      /* Fill is there any ETWS-indication is present in this Cell            */
      if (cellCb->pbmCb.etwsIndPres)
      {
         WR_FILL_TKN_UINT(page->etws_Ind, NhuPagetws_IndtrueEnum);
      } else {
         page->etws_Ind.pres = NOTPRSNT;
      }
     /* PH04_CMAS */
      /* Fill is there any ETWS-indication is present in this Cell            */
      if (cellCb->pbmCb.cmasIndPres)
      {
         WR_FILL_TKN_UINT(page->nonCriticalExtn.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(page->nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);

         WR_FILL_TKN_UINT(page->nonCriticalExtn.nonCriticalExtn.cmas_Ind_r9,
                                NhuPag_v920_IEscmas_Ind_r9trueEnum);
      } else {
           page->nonCriticalExtn.pres.val = NOTPRSNT;
      }
      /* PH04_CMAS end */

      /* Fill the Paging Records                                              */
      if (pagMsgEnt->numPagRcrds)
      {
         WR_FILL_TKN_UINT(page->pagingRecordLst.noComp,
                          pagMsgEnt->numPagRcrds);

         WR_GET_MEM(datReqSdus, 
               (page->pagingRecordLst.noComp.val)*(sizeof(NhuPagRecord)),
               &(page->pagingRecordLst.member));

         /* Fill one or multiple Paging Records                               */
         for(idx = 0; idx < (page->pagingRecordLst.noComp.val); idx++)
         {
            WR_FILL_TKN_UINT(page->pagingRecordLst.member[idx].pres, 
                             PRSNT_NODEF);
            WR_FILL_TKN_UINT(page->pagingRecordLst.member[idx].cn_Domain,
                             pagMsgEnt->pagRec[idx].domIndType);
            WR_FILL_TKN_UINT(page->pagingRecordLst.member[idx].
                             ue_Identity.choice,
                             pagMsgEnt->pagRec[idx].ueIdType);
            switch(pagMsgEnt->pagRec[idx].ueIdType)
            {
               case WR_PBM_UE_PAGEID_S_TMSI: 
               {
                  WR_FILL_TKN_UINT(page->pagingRecordLst.member[idx].
                                   ue_Identity.val.s_TMSI.pres, PRSNT_NODEF);
                  WR_FILL_BIT_STR(page->pagingRecordLst.member[idx].
                                  ue_Identity.val.s_TMSI.mmec, 8,
                                  (pagMsgEnt->pagRec[idx].ueId.sTMSI.mmec),
                                   datReqSdus);
                  WR_FILL_BIT_STR_VAL(page->pagingRecordLst.member[idx].ue_Identity.val.s_TMSI.m_TMSI,
                                      32,
                                      (pagMsgEnt->pagRec[idx].ueId.sTMSI.mTMSI),
                                      datReqSdus);
                  break;
               } /* end of case UEPAGID_S_TMSI */
               case WR_PBM_UE_PAGEID_IMSI: /* IMSI based */
               {
                  /* Number of digits in IMSI is stored in first byte */
                  WR_FILL_TKN_UINT(page->pagingRecordLst.member[idx].
                                   ue_Identity.val.imsi.noComp, 
                                   pagMsgEnt->pagRec[idx].ueId.imsi[0]);
                  WR_GET_MEM(datReqSdus, 
                             (page->pagingRecordLst.member[idx].ue_Identity.val.
                              imsi.noComp.val) * (sizeof(NhuIMSI_Digit)), 
                              &(page->pagingRecordLst.member[idx].ue_Identity.val.
                              imsi.member));
                  /* Fill the Digits of the IMSI */
                  for(idx1 = 0;  
                      idx1 < pagMsgEnt->pagRec[idx].ueId.imsi[0]; 
                      idx1++)
                  {
                     WR_FILL_TKN_UINT(page->pagingRecordLst.member[idx].
                                      ue_Identity.val.imsi.member[idx1],
                                      pagMsgEnt->pagRec[idx].ueId.imsi[idx1+1]); 
                  } /* end of for loop */
                  break;
                } /* end of case UEPAGID_IMSI */
            } /* end of switch statement */
         } /* end of for loop */
      } else {
         page->pagingRecordLst.noComp.pres = NOTPRSNT;
      }
   } /* end of if statement */
   RETVALUE(ROK);
} /* end of wrFillRRCPagingMsg */

/** @brief This function is used to fill and send RRC Paging message.
 *
 * @details
 *
 *     Function: wrPbmFillAndSndPagingMsg
 *
 *         Processing steps:
 *           - Allocates the memory for RRC:Paging message and 
 *             to fill the RRC:paging message call the following 
 *             func wrFillRRCPagingMsg.@sa wrFillRRCPagingMsg
 *           - Finally calls the RRC IM modile function wrIfmRrcSndReqToUe
 *             to send the prepared RRC:Paging message.
 *
 * @param [in] pageMsgInfo : Paging message information which is used to build
 *                           the RRC:Paging message
 * @param [in]      cellCb : cell control block
 * @return S16 
 *    -# Sucecess : ROK
 *    -# Failure : RFAILED.
 */
PRIVATE S16 wrPbmFillAndSndPagingMsg
(
 WrPagingMsgInfo     *pageMsgInfo, 
 WrCellCb            *cellCb
 )
{
   NhuDatReqSdus *pagMsg = NULLP;

   WR_ALLOCEVNT(&pagMsg, sizeof(NhuDatReqSdus));
   if(NULLP == pagMsg)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   
   /* Fill HDR Info      TODO:verify cellId in CellCb                         */
   wrEmmFillRrcEvntHdr(&(pagMsg->hdr), cellCb->cellId, WR_VAL_ZERO, C1_PAGING);
   pagMsg->sdu.ueMsg.isMsgDed = FALSE;

   /* Fill Common Message Info                                                */
   pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = NHU_MSG_DLPCCH;
   pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = WR_PCCH_ID;

   /* Fill Timing Info                                                        */
   pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.sfn = pageMsgInfo->pf;
   pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.subframe = 
                                                 pageMsgInfo->po;
   /* Fill the RRC paging Message Info                                        */
   wrFillRRCPagingMsg(cellCb, pageMsgInfo, pagMsg);
  
   /* Call the RRC-IM module function to send prepared RRC:Paging msg         */
   wrIfmRrcSndReqToUe(pagMsg);
   RETVALUE(ROK);
} /* end of wrPbmFillAndSndPagingMsg */

/** @brief This function is used to find the possible paging occusions for
 *         the curent paging frame(sfn)
 *
 * @details This functions performs the following activities.
 *
 *     Function: wrPbmCalcNextPFandSetOfPOs 
 *
 *         Processing steps:
 *            - Based on the current SFN, get the next paging frame with
 *              all possible paging occusions from the pre calculated table
 *              "allPfsAndPos" in PBM control block based on Default paging
 *              cycle and nB which are in SIB2.
 *
 *
 * @param [in]    cellCb : cell control block
 * @param [in]       sfn : system frame number 
 * @param [in]        pf : paging frame
 * @param [out] setOfPos : Paging occusions with in the paging frame.  
 * @return void
 */
PRIVATE void wrPbmCalcNextPFandSetOfPOs
(
 WrCellCb   *cellCb,
 U16        sfn, 
 U16        *pf, 
 U8         **setOfPos
)
{
   *pf = sfn;
   *setOfPos = cellCb->pbmCb.allPfsAndPos[sfn];
   RETVOID;
} /* end of wrPbmCalcNextPFandSetOfPOs */

/** @brief This function is used to trigger multiple RRC:Paging messages with
 *         paging events like etws-id and/or Si-change for all possible 
 *         occusions in one radio(paging) frame.
 * 
 * @details
 *
 *     Function: wrPbmProcessPagingEvents
 *
 *         Processing steps:
 *         - Calculate the next PF and appropriate POs based on current SFN.
 *         - Trigger paging msg with etws-ind and/or si-change event for 
 *           all POs except which POs aleady included Paging msg events 
 *           for this PF.  
 *         - Skip to send the page msg with etws/si-change event for the 
 *           invalid POs and already paging event is included for that PO
 *           in earliear paging message.
 *         - Prepare and send RRC:Paging message with specified event in
 *           PBM control block.
 *
 * @param [in]    cellCb : cell control block 
 * @param [in]       sfn : system frame number
 * @param [in] poBitMask : Paging Occusions bit mask
 * @return void
 */
PRIVATE void wrPbmProcessPagingEvents
(
 WrCellCb   *cellCb,
 U16        sfn,
 U16         poBitMask
)
{
   WrPagingMsgInfo pagMsgInfo;
   U16             idx, pf;
   U8              *setOfPos = NULLP;

   TRC2(wrPbmProcessPagingEvents);

   /* Calculate the next PF and appropriate POs */
   wrPbmCalcNextPFandSetOfPOs(cellCb, sfn, &pf, &setOfPos);

   /* Trigger paging msg with etws-ind and/or si-change event for all POs 
    * except which POs aleady included Paging msg events for this PF.
    */
   for(idx=0; (idx < MAX_POSSIBLE_POS); idx++) 
   {
      /* Skip to send the page msg with etws/si-change event for the invalid POs
       * and already paging event is included for that PO in earliear paging message */ 
      if((WR_PBM_INVALID_PO == setOfPos[idx]) ||
            ((1<<setOfPos[idx]) & poBitMask))
      {
         continue;
      }
      /* clear the pagMsgInfo */
      cmMemset((U8*)&pagMsgInfo, 0, sizeof(pagMsgInfo));

      /* fill the PF and PO */
      pagMsgInfo.pf = pf;
      pagMsgInfo.po = setOfPos[idx];
      pagMsgInfo.numPagRcrds = 0;

      /* Prepare and send RRC:Paging message with event */
      wrPbmFillAndSndPagingMsg(&pagMsgInfo, cellCb);
   }
   RETVOID;
} /* end of wrPbmProcessPagingEvents */

/** @brief This function is used to update the PBMCB's paging events.
 *
 * @details
 *
 *     Function: wrPbmUpdPbmCbPagEvents
 *
 *         Processing steps:
 *         - Based on the pbmInfoType in pbmInfo, set the paging events like
 *           "etwsIndPres" or "siModPres" in PBM control block is set 
 *           TRUE or FALSE.
 *
 * @param [in, out] pbmCb : PBM Control block
 * @param [in]    pbmInfo : PBM Information
 * @return void 
 */
PRIVATE void wrPbmUpdPbmCbPagEvents
(
 WrPbmCb       *pbmCb, 
 WrPbmInfo     *pbmInfo
) 
{
   TRC2(wrPbmUpdPbmCbPagEvents);

   switch (pbmInfo->pbmInfoType)
   {
      case WR_PBM_TYPE_START_PAG_ETWS:
         {
            pbmCb->etwsIndPres = TRUE;
            break;
         }
      /* PH04_CMAS */
      case WR_PBM_TYPE_START_PAG_CMAS:
         {
            pbmCb->cmasIndPres = TRUE;
            break;
         }
      /* PH04_CMAS end */

      case WR_PBM_TYPE_START_PAG_SIMOD:
         {
            pbmCb->siModPres = TRUE;
            break;
         }
      case WR_PBM_TYPE_STOP_PAG_SIMOD:
         {
            pbmCb->siModPres = FALSE;
            break;
         }
      case WR_PBM_TYPE_STOP_PAG_ETWS:
         {
            pbmCb->etwsIndPres = FALSE;
            break;
         }
      /* PH04_CMAS */
      case WR_PBM_TYPE_STOP_PAG_CMAS:
         {
            pbmCb->cmasIndPres = FALSE;
            break;
         }
      /* PH04_CMAS end */

      case WR_PBM_TYPE_START_SI_MOD:
      case WR_PBM_TYPE_NORMAL_PAG:
      case WR_PBM_TYPE_CMAS:
      case WR_PBM_TYPE_PRI_ETWS:
      case WR_PBM_TYPE_SEC_ETWS:
      case WR_PBM_TYPE_SIB8:
         {
            break;
         }
      default:
         {
            RLOG1(L_ERROR,"Unknown Paging Indication is processing:[%d]", 
               pbmInfo->pbmInfoType);
            break;
         }
   } /* end of switch */
   RETVOID;
} /* end of wrPbmUpdPbmCbPagEvents */

/** @brief This function parse the S1-AP Paging msg and update the uePagingInfo.
 *
 * @details
 *
 *     Function: wrPbmUpdateUePagInfo
 *
 *         Processing steps:
 *         - Update all the parameters of UePagingInfo which we received from 
 *           S1-AP:Paging message.
 *         - Calculates the Paging Frame(PF) and Paging Occusion(PO) based on
 *           UePagingInfo parameters and upadte those values in UePagingInfo.
 *         - Update the output parameters pagMsgTAILst, pagMsgCSG_IdLst if the 
 *           TAI and CSG IDs are present in the received S1-AP paging message.
 *           If any list is not present set the oupur parameter to NULLP.
 *
 * @param [in]       s1apPagMsg : S1-AP paging message PDU
 * @param [out]    uePagingInfo : UE specific paging info updated based on S1-AP PDU.
 * @param [out]    pagMsgTAILst : TAI list if TAI Lst present in Paging Msg or NULLP.  
 * @param [out] pagMsgCSG_IdLst : CSG ID List if CSG Lst present in paging msg or NULLP 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPbmUpdateUePagInfo
(
 S1apPdu             *s1apPagMsg,
 WrUePagingMsgInfo   *uePagingInfo,
 SztTAILst           **pagMsgTAILst,
 SztCSG_IdLst        **pagMsgCSG_IdLst
)
{
   SztProtIE_Field_PagIEs *pagMember = NULLP;
   SztPag                 *sztPag     = NULLP;
   U16                    memberIdx = 0;
   TknBStr32              *ueIdxBitStr = NULLP;
   U16                    numCompPagMsg = 0;
   U16                    ueId = 0;
   U8                     idx1, idx2;
   U16                    memberId;

   TRC2(wrPbmUpdateUePagInfo);

   /* Get the reference of the Paging Message */
   sztPag = &(s1apPagMsg->pdu.val.initiatingMsg.value.u.sztPag);
   numCompPagMsg = sztPag->protocolIEs.noComp.val;

   /* Accessing the IEs within the S1AP pdu */
   for(memberIdx = 0; memberIdx < numCompPagMsg; memberIdx++)
   {
      pagMember = &(sztPag->protocolIEs.member[memberIdx]);
      memberId = pagMember->id.val;

      switch(pagMember->id.val)
      {
         case Sztid_TAILst:
         {
            *pagMsgTAILst = &(pagMember->value.u.sztTAILst);
            break;
         }
         case Sztid_CSG_IdLst:
         {
            *pagMsgCSG_IdLst = &(pagMember->value.u.sztCSG_IdLst);
            break;
         }
         case Sztid_UEIdentityIdxValue:
         {
            ueIdxBitStr = &(pagMember->value.u.sztUEIdentityIdxValue);
            /* UEID is only 10 bits so it has been shifted for 6 bits */
            ueId = 0;
            ueId =((ueIdxBitStr->val[0] << 2) | 
                   ((ueIdxBitStr->val[1] & 0xC0) >> 6));
            uePagingInfo->ue_Id = ueId;
            break;
         }
         case Sztid_UEPagID:
         {
            if(pagMember->value.u.sztUEPagID.choice.val == UEPAGID_IMSI)
            {
               uePagingInfo->ueIdenType = UEPAGID_IMSI;
               /* Number of IMSI Digits(0-9 digits are 
                  only valid) = 2 * IMSI legth in bytes                   */
               uePagingInfo->ueIden.imsi[0] = 
                       2*(pagMember->value.u.sztUEPagID.val.iMSI.len);

               /* Converting IMSI in byte format to IMSI in digit format    */
               for(idx1 = 0, idx2 = 1; 
                   idx1 < pagMember->value.u.sztUEPagID.val.iMSI.len; 
                   idx1++, idx2++)
               {
                  uePagingInfo->ueIden.imsi[idx2]  = 
                     pagMember->value.u.sztUEPagID.val.iMSI.val[idx1] & 0x0f;
                   uePagingInfo->ueIden.imsi[++idx2]  =
                      ((pagMember->value.u.sztUEPagID.val.iMSI.val[idx1] & 0xf0) >> 4);
               } /* end of for loop */

               /*   When IMSI has odd number of digits,the filler
                *   digit will  be added at the end to make an even number
                *   of digits of length 2N. The filler digit must be ignorded
                *   while sending to RRC and the length should be decremented
                * */
                if((uePagingInfo->ueIden.imsi[idx2-1] == 0xf))
                {
                   uePagingInfo->ueIden.imsi[0] =
                      uePagingInfo->ueIden.imsi[0] - 1;
                }
            }
            else if (pagMember->value.u.sztUEPagID.choice.val == UEPAGID_S_TMSI)
            {
               uePagingInfo->ueIdenType = UEPAGID_S_TMSI;
               uePagingInfo->ueIden.sTMSI.pres = TRUE;
               uePagingInfo->ueIden.sTMSI.mmec = pagMember->value.u.
                                   sztUEPagID.val.s_TMSI.mMEC.val[0];

               for(idx1 = 0; idx1 < 4; idx1++)
               {
                  uePagingInfo->ueIden.sTMSI.mTMSI <<= 8;
                  uePagingInfo->ueIden.sTMSI.mTMSI |= 
                                  (U8)(pagMember->value.u.sztUEPagID.
                                       val.s_TMSI.m_TMSI.val[idx1]);
               } /* end of for loop */
            }
            else
            {
               RLOG0(L_ERROR,"Valid UE Identity should be provided");
               RETVALUE(RFAILED);
            } /* end of else statement */
            break;
         } /* end of Sztid_UEPagID */
         case Sztid_pagingDRX:
         {
            uePagingInfo->pagingDrxPres = TRUE;
            uePagingInfo->ueSpcPagDrx = pagMember->value.u.sztPagDRX.val;
            break;
         } /* end of Sztid_pagingDRX */
         case Sztid_CNDomain:
         {
            uePagingInfo->domIndType = pagMember->value.u.sztCNDomain.val;
            break;
         }
         case Sztid_PagPriority:
         {
            uePagingInfo->pagPriority = pagMember->value.u.sztPagPriority.val;
         }
         break;
         default:
         {
            RLOG1(L_ERROR,"Unknown IE or Option received:[%d]", 
               memberId);
            break;
         }
      } /* end of switch statement */
   } /* end of for loop */
   
   RETVALUE(ROK);
} /* end of wrPbmUpdateUePagInfo */

/** @brief This function is used to process the Pending SI messages.
 *
 * @details
 *
 *     Function:wrPbmProcPendSiMsg 
 *
 *         Processing steps:
 *         - Update the SI Scheduling Information in SIB1 with the Pending SI
 *           Information.
 *         - If the numSis are changed i.e., either new SI is added or removed the 
 *           existed SI message then reconfigure the MAC with all SIs
 *           Otherwise configure the MAC with changed SIs only.
 *
 * @param [in, out] cellCb : cell control block
 * @return S16
 *             ROK : successfully reconfigured the MAC with the required SI info.
 *         RFAILED : Not able to configure the MAC with updated SI info.
 */
PRIVATE S16 wrPbmProcPendSiMsg
(
 WrCellCb *cellCb
)
{
   WrSISchdInfo  *pendSiInfo     = &(cellCb->pbmCb.pendSiInfo[0]); 
   RgrCellRecfg  *reCfg          = &(cellCb->pendRecfg.schReCfg);
   WrSISchdInfo  *siSchdInfo     = &(cellCb->sib1.siSchdInfo[0]);
   U8            onGoingNumSIs   = cellCb->sib1.numSis;
   U8            pendingNumSIs   = cellCb->pbmCb.numSIs;
   Bool          newSiMsg        = TRUE;
   Bool          allSisReConfig  = FALSE;
   U8            pendSiIdx, siIdx, copySiIdx, sibIdx  = 0;
   U8            updatedSiIdxs[WR_PBM_MAX_NUM_SIS];  
   U8            updSiIdx, numUpdatedSis = 0;  
   WrSISchdInfo  tmpSiSchdInfo;
   U8            sib2SIIdx, sib2Period;

   /* traverse the on-going SI msgs and update it with pending SI-msg */ 
   for (pendSiIdx = 0; pendSiIdx < pendingNumSIs; pendSiIdx++)
   {
      newSiMsg = TRUE;
      for(siIdx = 0; siIdx < onGoingNumSIs; siIdx++)
      {
         /* is periodicities are matched, then update the pending 
          * SI's sibs to the ongoing SI's sibs */
         if (siSchdInfo[siIdx].period == pendSiInfo[pendSiIdx].period)
         {
            newSiMsg = FALSE;

            /* After any SIB stop/SIB periodicity cahnges may 
             * lead to the num SIBS is 0 in the SI msg */
            if (pendSiInfo[pendSiIdx].numSibs == 0)
            {
               /* remove the SI msg because there is no SIBs in SI msg */
               for (copySiIdx = siIdx; copySiIdx < onGoingNumSIs; copySiIdx++)
               {
                  cmMemcpy((U8 *)&siSchdInfo[copySiIdx], 
                           (U8 *)&siSchdInfo[copySiIdx+1], sizeof(WrSISchdInfo));
               }
               onGoingNumSIs--;
               siIdx--;
               cellCb->sib1.numSis--;
               allSisReConfig = TRUE;
            } 
            else 
            { 
               /* updating the SI msg */
               siSchdInfo[siIdx].numSibs = 0;
               /* overwrite all sibs in ongoing Si msg 
                * with the SIB numbers in pending SI msg */
               for (sibIdx = 0; 
                    sibIdx < pendSiInfo[pendSiIdx].numSibs;
                    sibIdx++)
               {
                  siSchdInfo[siIdx].sibs[sibIdx] = 
                            pendSiInfo[pendSiIdx].sibs[sibIdx];
                  siSchdInfo[siIdx].numSibs++;
               }/* end of for to copy all sibs in pending
                   si msg to ongoing si msg */

               /* store the updated SI indx number for to reconfigure MAC */
               updatedSiIdxs[numUpdatedSis++] = siIdx; 
            }
            break;
         } /* end of if to SIB periodicity matches */
      } /* end of for to traverse all on-going SIs */

      /* new SI msg which periodicity is not matched with any ongoing Si msgs
       * periodicity. */
      if (newSiMsg == TRUE)
      {
         siSchdInfo[cellCb->sib1.numSis].numSibs = 0;
         for (sibIdx = 0; sibIdx < pendSiInfo[pendSiIdx].numSibs; sibIdx++)
         {
            siSchdInfo[cellCb->sib1.numSis].sibs[sibIdx] = 
                                         pendSiInfo[pendSiIdx].sibs[sibIdx];
            siSchdInfo[cellCb->sib1.numSis].period = 
                                         pendSiInfo[pendSiIdx].period;
            siSchdInfo[cellCb->sib1.numSis].numSibs++;
         } /* end of for to copy all sibs in pending si msg to ongoing si msg */

         /* Increment number of ongoing SI msgs */
         cellCb->sib1.numSis++; 
         allSisReConfig = TRUE;
      } /* end of if */
     
   } /* end of for to traverse the all pending Si msgs */
   
   /* Get pending SI Info with updated SIB2 Periodicity and 
      update the SIB2 included SI message  as SI-0 */
      
   sib2Period = wrEmmPbmGetSibPeriod(cellCb, WR_SIB_2);
   sib2SIIdx = wrPbmGetSiIndex(sib2Period, 
                               cellCb->sib1.numSis, 
                               siSchdInfo);
   if (sib2SIIdx >= WR_PBM_MAX_NUM_SIS)
   {
      RLOG0(L_ERROR,"Invalid Number of SIS.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   /* Thw swapping of two SIs because SIB2 should be in SI-0.*/
   if (sib2SIIdx != 0)
   {
      cmMemcpy((U8*)&tmpSiSchdInfo, (U8 *)&siSchdInfo[0], sizeof(WrSISchdInfo));
      cmMemcpy((U8*)&siSchdInfo[0], (U8 *)&siSchdInfo[sib2SIIdx], sizeof(WrSISchdInfo));
      cmMemcpy( (U8 *)&siSchdInfo[sib2SIIdx], (U8*)&tmpSiSchdInfo, sizeof(WrSISchdInfo));
   }


   if(numUpdatedSis)
   {
      cellCb->sib1.valueTag++;
      cellCb->sib1.valueTag &= WR_EMM_PBM_SIB1_MAX_VALUE_TAG;
         /*127802*/       
         /* Enable this Macro when the logging-levels are implemented and
            increse the appropriate logging level for this below print. 
            RLOG0(L_INFO,"***SIB1 VALUE TAG %d",cellCb->sib1.valueTag));
         */
      if(!allSisReConfig)
      {
                   /* Ph04_CMAS : Added last parameter. */
                   wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_TRANS_ID);
      }
   }


   if (allSisReConfig)
   {
#if WR_RECFG
      cellCb->pendRecfg.cellReCfgBitMsk |= WR_CELL_SCHED_RECFG_PEND; 
      reCfg->recfgTypes |= RGR_CELL_SI_RECFG;
      cellCb->pendReCfgPres = TRUE;
      wrEmmCellSchdSiCfg(&reCfg->siReCfg, cellCb);
      if (TRUE == cellCb->pendReCfgPres)
      {
         wrEmmCellSchdReCfg(cellCb->cellId);
      }
#endif               
   } else {
      /* reconfigure all SIs to MAC, because numSIs are changed */
      for (updSiIdx = 0; updSiIdx < numUpdatedSis; updSiIdx++)
      {
         siIdx = updatedSiIdxs[updSiIdx];
         if((cellCb->sibsCfg[siIdx].sibId== WR_SIB_5) &&  
                              (cellCb->eutranNbrFreqLst.count == 1))
         {
            RETVALUE(wrEmmPbmStopSI(cellCb->cellId, siIdx));   
         }
         else
         {
            if (wrEmmPbmSiConfig(cellCb->cellId, siIdx) != ROK)
            {
               RETVALUE(RFAILED);
            }
         }
      }
   }
      RETVALUE(ROK);
} /* end of wrPbmProcPendSiMsg */


/** @brief This function is used to validate the cell TAI is present in the TAI
 *         list received in the S1AP: Paging message.
 *
 * @details
 *
 *     Function: wrPbmIsTaiPresent 
 *
 *         Processing steps:
 *         - Get the Tracking Area Code, num of PLMNs and PLMN IDs from SIB1. 

 *         - Traverse all TAIs in S1-AP paging msg and compare with cell TAI.
 *         - First compare the TAC in TAI of the cell is matched with the
 *           received TAC in TAI.Then compare the received PLMN with the 
 *           cell PLMN list.If there is any match found then returns TRUE.
 *           Otherwise returns FALSE.
 *
 * @param [in] cellCb : cell control block
 * @param [in] taiLst : TAI list 
 * @return Bool 
 *         TRUE  : If cell TAI is matched with at least one TAI in the received
 *                 TAI list.
 *         FALSE : cell TAI is not present in the received TAI list.
 */
PRIVATE Bool wrPbmIsTaiPresent
(
 WrCellCb    *cellCb, 
 SztTAILst   *taiLst
) 
{
   WrPlmnId                          *cellPlmnId   = NULLP;
   WrPlmnId                          plmnId;
   SztProtIE_SingleCont_TAIItemIEs   *taiMbr       = NULLP;
   SztPLMNidentity                   *plmnIe       = NULLP;
   U8                                cellTac[2]    = {0};
   U8                                numPlmnIds    = 0;
   U8                                plmnIndex, taiIndex;

   /* Get the Tracking Area Code,num of PLMNs and PLMN IDs from SIB1 */
   cellTac[1] = (U8)(cellCb->sib1.tac & 0xff);
   cellTac[0] = (U8)((cellCb->sib1.tac & 0xff00)>>8);

   numPlmnIds = cellCb->sib1.numPlmnIds;

   /* traverse all TAIs in S1-AP paging msg and compare with cell TAI */
   for (taiIndex = 0; (taiIndex < taiLst->noComp.val); taiIndex++)
   {
      taiMbr = &taiLst->member[taiIndex];

      if ((cellTac[0] == taiMbr->value.u.sztTAIItem.tAI.tAC.val[0]) && 
            (cellTac[1] == taiMbr->value.u.sztTAIItem.tAI.tAC.val[1]))
      {
         /* Parse S1AP PLMN ID IE */
         plmnIe = &taiMbr->value.u.sztTAIItem.tAI.pLMNidentity;
         wrEmmMmeParsePlmnIe(plmnIe, &plmnId);

         /* compare the received PLMNID with the cell PLMNIDs */
         for (plmnIndex = 0; plmnIndex < numPlmnIds; plmnIndex++)
         {
            cellPlmnId = &cellCb->sib1.plmns[plmnIndex].plmnId;
            if(wrEmmPlmnPlmnsEqual(&plmnId, cellPlmnId)) 
            {
               RETVALUE(TRUE);
            }
         } /* end of for loop for PLMN Ids comparison */
      }  /* end of if for TAC comparison */
   } /* end of TAI list for loop */

   RETVALUE(FALSE);
} /* end of wrPbmIsTaiPresent */

/** @brief This function is used to validate the cell CSG-ID is matched with the
 *         any one of the CSG-ID  of the S1-AP: Paging message.
 *
 * @details
 *
 *     Function: wrPbmIsCsgIdPresent
 *
 *         Processing steps:
 *         - Traverse the received TAI list and compare the each
 *           received TAI with the cell control block TAI.If the match found 
 *           return TRUE. Otherwise return FALSE.
 *
 * @param [in, out] cellCb : cell control block
 * @param [in]    csgIdLst : CSG ID list which is received in S1AP:Pageing message. 
 * @return Bool
 *         TRUE : cell CSG id is matched with one of the CSG id received Paging
 *                message.
 *        FALSE : cell CSG ID is not matched.
 */
PRIVATE Bool wrPbmIsCsgIdPresent
(
 WrCellCb        *cellCb, 
 SztCSG_IdLst    *csgIdLst
) 
{
   SztCSG_IdLst_Item               *csgMbr       = NULLP;
   U32                             csgIdConfig = 0; 
   U32                             csgIdRcvd   = 0; 
   U8                              csgIdPres    = FALSE;
   U8                              csgIndex; 

      /* 
         Please note the CSG ID configured for the UE is transmitted in the SIB1.
         So taking the configured values taken from SIB1
      */
      csgIdPres   = cellCb->sib1.csgInd;
      csgIdConfig = cellCb->sib1.csgId.val;
   
      if (csgIdPres == TRUE)
      {
         for (csgIndex = 0;  csgIndex < csgIdLst->noComp.val; csgIndex++)
         {
            csgMbr = &csgIdLst->member[csgIndex];
            wrUtlGetU32FrmBStr32(&csgIdRcvd,&(csgMbr->cSG_Id));
            /* CSG ID size is 27 bits */
            if (csgIdConfig == csgIdRcvd )
            {
               RETVALUE(TRUE);
            }
         } /* end of CSG Lst for loop */
      } /* end of if the serving cell is CSG cell */
   
   RETVALUE(FALSE);
} /* end of wrPbmIsCsgIdPresent */

/** @brief This function is used to process the S1-AP Paging message.
 *
 * @details
 *
 *     Function:wrEmmPbmProcPagingMsg 
 *
 *         Processing steps:
 *         - This function is called EMM module on reception trigger from the 
 *           S1-AP IM module upon receiving S1-AP:Paging Message from MME.
 *         - Identify in which Cells the RRC:Paging message needs to triger 
 *           based on TAI and or CSGID for each cell and received List in 
 *           S1-AP Pagigng Message.
 *           - Update the UePagingInfo(which includes all information to
 *             calculate PF and PO and paging record) from S1-AP:Paging Message.
 *           - Traverse all Cells supported by ENB to validate paging msg
 *             trigger is required or not.
 *           - If the cell's TAI is present in the received S1-AP Paging
 *             Request TAI list.Then trigger the RRC:Paging message in this
 *             Cell.
 *           - Otherwise Validate the CSG id of the cell is present or not
 *             in the received S1-AP paging Req csg id list.If CSG ID is 
 *             present then trgger the RRC:paging message in the current cell.
 *             Otherwise skip cell to process the paging request.
 *
 * @param [in] s1apPagMsg : S1-AP paging message PDU.
 * @return S16
 *       ROK     : succefully processed the S1-AP paging message.
 *       RFAILED : not able to process the paging message due to memory lack.
 */
PUBLIC S16 wrEmmPbmProcPagingMsg
(
 S1apPdu *s1apPagMsg 
) 
{
   SztTAILst                       *taiLst    = NULLP;
   SztCSG_IdLst                    *csgIdLst = NULLP;
   WrCellCb                        *cellCb   = NULLP;
   WrUePagingMsgInfo               uePagingInfo;
   U16                             cellIdx = 0; 

   TRC2(wrEmmPbmPagingMsg);

   /* Initialize the default paging priority to 255 and the same will be 
    * updated if it is received from MME. 
    */
   uePagingInfo.pagPriority = WR_DEFAULT_PAGING_PRI;

   /* Update the UePagingInfo from S1-AP:Paging Message */
  wrPbmUpdateUePagInfo(s1apPagMsg, &uePagingInfo, &taiLst, &csgIdLst);

  RLOG2(L_DEBUG,"Received PAGING, UE Index = [%d], CN Domain = [%d]",
     uePagingInfo.ue_Id, uePagingInfo.domIndType);
   
  /* traverse all Cells supported by ENB to validate paging msg trigger is
   * required or not */ 
  for (cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
  {
     cellCb = wrEmmCb.cellCb[cellIdx];

     if(cellCb->cellState != WR_CELL_STATE_UP)
     {
          RETVALUE(ROK);
     }

     /* cell's TAI is present in the received S1-AP paging Req TAI list */
     if ((NULLP != taiLst) &&
         (TRUE == wrPbmIsTaiPresent(cellCb, taiLst)))
     {
        /* CSG_DEV Start */
        /* CSG_DEV:In a particular TA if the CSG Id List is present than needs to page only those Cells
         */
         if (NULLP != csgIdLst) 
         { 
            /* CSG_DEV:CSG id of the cell is present or not in the received
             * S1-AP paging Req csg id list */
            if ( cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS || cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
            {
               if ((TRUE == wrPbmIsCsgIdPresent(cellCb, csgIdLst)))
               {
                  if (RFAILED == wrPbmCreatePagingRecord(cellCb, &uePagingInfo))
                  {
                     RLOG0(L_ERROR, "Paging Record Creation failed");
                  }
               }
            continue;
            }/*WR_ENB_CLOSED_ACCESS and WR_ENB_HYBRID_ACCESS*/
            else
            {
               if (RFAILED == wrPbmCreatePagingRecord(cellCb, &uePagingInfo))
               {
                  RLOG0(L_ERROR, "Paging Record Creation failed");
               }
            }/* This covers Open Access Mode */
         }
         else
         {
            if (RFAILED == wrPbmCreatePagingRecord(cellCb, &uePagingInfo))                       
            {
               RLOG0(L_ERROR, "Paging Record Creation failed");
            }
            continue;
         }  
     }

  } /* end of for to traverse all the cells in ENB */
   RETVALUE(ROK);
} /* end of wrEmmPbmPagingMsg */

/** @brief This function is used to process the PBM scheduleing info table for
 * every SFN and free the PBmInfo if any node is present */
PUBLIC Void wrEmmPbmFreePendMsgLst
(
 U16         sfn,
 WrCellCb    *cellCb
 )
{
   CmLListCp       *pbmInfoLstCp = NULLP;
   CmLList         *pbmInfoLnk   = NULLP; 
   CmLList         *temp         = NULLP; 
   WrPbmInfo       *pbmInfo      = NULLP;
   /* Get the Pending PBM Info list to be processed at this 
    * sfn from the "pbmSchedInfo" table */
   pbmInfoLstCp = wrPbmGetPendPagMsgLst(sfn, cellCb);
   if(NULLP == pbmInfoLstCp)
   {
      RETVOID;
   }

   /* Traverse the Pending PBM info List */ 
   CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);

   temp = pbmInfoLnk;
   while (NULL != pbmInfoLnk) 
   {
      temp = temp->next;
      pbmInfo = (WrPbmInfo *)(pbmInfoLnk->node);
      /* Remove the entry from the pend PBM info Lst */
      cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
      /* Free the memory for that pending PBM Info entry */
      WR_FREE(pbmInfo, sizeof(WrPbmInfo));
      /* Move to the next node */
      pbmInfoLnk = temp;
   }

   RETVOID;
} /* wrEmmPbmFreePendMsgLst */
/** @brief This function is used to process the PBM scheduleing info table for
 * every SFN.
 *
 * @details
 *
 *     Function: wrPbmTTIInd
 *
 *         Processing steps:
 *             - Update the temp SFN with current SFN + offset value(WR_SFN_INCREMENT)
 *             - Use the updated sfn to calculate the SIB11 periodicty is reached or not.
 *             - If SIB11 periodicity is reached, then build and send the SIB11 message.
 *               Build the SIB11 by using this function @sa wrSibBldSIB11Msg.
 *               and send to RGR IM module by using this function @sa wrEmmPbmSiConfig
 *               and updated SIB1 also should send to RGR IM.
 *             - Use the Updated SFN, perform the lookup into the "pbmSchInfo" table,
 *               update "sysInfoChange" and "etwsInd" based on the "pbmInfoType".
 *               by using the wrPbmUpdPbmInfoPagEvents @sa wrPbmUpdPbmInfoPagEvents
 *@code
 *               wrPbmUpdPbmCbPagEvents(pbmCb, pbmInfo);
 *@endcode 
 *             - In PbmInfo entry, if the "pbmInfoType" is equal to 
 *               "WR_PBM_TYPE_START_SI_MOD" and "pbmCycleCnt" is reacehed to 0.
 *             - Then group the multiple SIBs in one SI message based on the same 
 *               SI-Periodicity by using this function @sa wrPbmProcPendSiMsg.
 *             - If successfully brodcasted one or multiple SIs, then finally 
 *               remove the current entry from the list and free it.
 *             - Set the bitmask to indicate which Paging Occusions in one paging
 *               frame are successfully broadcasted paging message with ETWS 
 *               and SI-change.
 *             - Get the Pending Paging message list based on the SFN/PF from 
 *               the pending PBM Info tabl by using the following function 
 *               "wrPbmGetPendPagMsgLst". @sa wrPbmGetPendPagMsgLst. 
 *             - Temparerly take a "poBitMask" on stack, set the Paging occusion bit 
 *               for which Paging ocussion,when we trigggered paging message with
 *               etws/si change icluding. The prepared "poBitMask" is used 
 *               while processing the  paging message with etws and 
 *               si-modification events for rest of the Paging occusions.
 *             - In PbmInfo entry, if the "pbmInfoType" is set to 
 *               "WR_PBM_TYPE_NORMAL_PAG".Performe the following steps for 
 *                all elements in the List.
 *                - If the  "pbmCycleCnt" in PageMsgEntry is not ZERO,
 *                  then "pageCycleCnt" decrment it by ONE.
 *                - Otherwise("pageCycleCnt" is 0), prepare and send the paging
 *                  message with all paging records by using this func 
 *                  wrPbmFillAndSndPagingMsg  @sa wrPbmFillAndSndPagingMsg
 *                   - set the bit "poBitMask" for which PO the current 
 *                     paging message is triggered.
 *                - Then finally remove the all the processed ("pendPaeMsg") 
 *                  from the "pendPagMsgLst". Then free the element.
 *             - In PbmInfo entry, if the "pbmInfoType" is equal to either 
 *               "WR_PBM_TYPE_START_PAG_ETWS" or "WR_PBM_TYPE_START_PAG_SIMOD"
 *               and "pbmCycleCnt" is reacehed to 0.
 *             - Call the function wrPbmProcessPagingEvents for to process the 
 *               paging events like ETWS-indication(etwsInd) and/or system 
 *               information change(sysInfoChange) is enabled or not with in 
 *               this cell. @sa wrPbmProcessPagingEvents
 *@code
 *            if ((pbmInfo->pbmInfoType == WR_PBM_TYPE_START_PAG_ETWS) || 
 *                (pbmInfo->pbmInfoType == WR_PBM_TYPE_START_SI_MOD))
 *            {
 *             wrPbmProcessPagingEvents(cellCb, sfn, poBitMask);
 *            }
 *@endcode
 *         - In PbmInfo entry, if the "pbmInfoType" is equal to either
 *           "WR_PBM_TYPE_START_PAG_SIMOD" or "WR_PBM_TYPE_START_PAG_ETWS"
 *           and  "pbmCycleCnt" is reacehed to 0.
 *        - Remove from the curent List and update the same entry to stop Paging 
 *          and adjust the "pbmCycleCnt" and "pbmInfoType" and insert in to the
 *          "pbmSchInfo" table.
 *@code
 *        if (pbmInfo->pbmInfoType == WR_PBM_TYPE_START_PAG_SIMOD)
 *        {
 *           pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_SIMOD;
 *        } else if (pbmInfo->pbmInfoType == WR_PBM_TYPE_START_ETWS)
 *        {
 *           pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_ETWS;
 *        }
 *       // based on the availablity of Default DRX cycle with in this
 *       // modification period and also configured Repetation DRX cycle.
 *       pbmInfo->pbmCycleCnt = atLeast (nT,T);
 *       // insert it to the pbmSchInfo table at proper position.
 *@endcode
 *     - In PbmInfo entry, if the "pbmInfoType" is equal to either 
 *       "WR_PBM_TYPE_STOP_PAG_SIMOD" or "WR_PBM_TYPE_STOP_PAG_ETWS"
 *       and  "pbmCycleCnt" is reacehed to 0.
 *     - Remove the element from the list and free it.
 * @param [in] tmngInfo : timing info  
 * @return S16
 *    -# Success : ROK 
 */
PUBLIC S16 wrPbmTTIInd
(
 RgrTtiIndInfo *tmngInfo
) 
{
   CmLListCp       *pbmInfoLstCp = NULLP;
   CmLList         *pbmInfoLnk   = NULLP; 
   CmLList         *temp         = NULLP; 
   WrPbmInfo       *pbmInfo      = NULLP;
   WrCellCb        *cellCb       = NULLP; 
   U8              modifPerdCoeff;
   U16             numOfSfnsPerModifPerd;
   U16             dfltDrxCycle;
   U16              poBitMask     = 0;
   U8              cellIdx       = 0;
   U16             sfn;
   static U16      numofPagingEvntForSiMod = 0;
   
   TRC2(wrPbmTTIInd);
   
  for (cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
  {
     cellCb = wrEmmCb.cellCb[cellIdx];
     /* Updating SFN */
     cellCb->pbmCb.curSfn = tmngInfo->sfn;

     /* update the temp SFN with currnet SFN + offset */ 
     sfn = (tmngInfo->sfn + WR_SFN_INCREMENT) % WR_MAX_SFN;
     WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle, dfltDrxCycle);
#ifdef WR_RECFG
  if (TRUE == cellCb->pbmCb.pendReCalcAllPfsAndPos) 
  {
    WR_GET_MODIFI_COEFF(cellCb->sib2.bcchCfg.modifPeriodCoeff, modifPerdCoeff);
      
    numOfSfnsPerModifPerd = (modifPerdCoeff * dfltDrxCycle);
    /* configure the Lower layers if pending Reconfiguratiion present */
   if (sfn % numOfSfnsPerModifPerd == 0)
   {
      /* Recalculate ALL Pfs And POs when modification coeff or nB or
       * defaultPagingFactor changes */
      if (TRUE == cellCb->pbmCb.pendReCalcAllPfsAndPos)
      {
         /* calculate and update the table "allPfsAndPos" which is used to send
          * RRC:Paging message with SI-change or etws indication for all PFs 
          * and POs */
         wrPbmUpdDfltPagCycNb(cellCb, cellCb->pbmCb.pendDfltPagingCycle ,
               cellCb->pbmCb.pendnB);
         cellCb->sib2.pcchCfg.defaultPagingCycle = cellCb->pbmCb.pendDfltPagingCycle;
         cellCb->sib2.pcchCfg.nB = cellCb->pbmCb.pendnB;
         cellCb->sib2.bcchCfg.modifPeriodCoeff = cellCb->pbmCb.pendModifPeriodCoeff;
         cellCb->pbmCb.pendReCalcAllPfsAndPos = FALSE;  
      }
   }
  }
#endif
     /* TODO: is SIB11 Periodicity reached then build and send it to MAC */

     /* Get the Pending PBM Info list to be processed at this 
      * sfn from the "pbmSchedInfo" table */
     pbmInfoLstCp = wrPbmGetPendPagMsgLst(sfn, cellCb);

     /* Traverse the Pending PBM info List */ 
     CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);

     temp = pbmInfoLnk;
     while (NULL != pbmInfoLnk) 
     {
        temp = temp->next;
        pbmInfo = (WrPbmInfo *)(pbmInfoLnk->node);
        /* Update the PBM Control Block's paging Events based on 
         * the value of pbmInfoType in  pbmInfo.
         */
        wrPbmUpdPbmCbPagEvents(&cellCb->pbmCb, pbmInfo);
        switch (pbmInfo->pbmInfoType)
        {
           RLOG2(L_DEBUG,"Processing PbmInfoLst for SFN [%d], PBM-INFO-TYPE [%d]",sfn,pbmInfo->pbmInfoType);
           case WR_PBM_TYPE_START_SI_MOD:
              {
                 /* Group the multiple SIBs in one SI message based on the same
                  * SI-Periodicity 
                  */
                 wrPbmProcPendSiMsg(cellCb);

                 /* Reset the pending Si info in pbmCb .*/
                 cellCb->pbmCb.numSIs = 0;
                 cmMemset((U8*)&(cellCb->pbmCb.pendSiInfo[0]),
                           0, (WR_PBM_MAX_NUM_SIS* sizeof(WrSISchdInfo)));

                 /* Remove the entry from the pend PBM info Lst */
                 cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                 /* Free the memory for that pending PBM Info entry */
                 WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                 break;
              }
           case WR_PBM_TYPE_NORMAL_PAG:
              {
                 /* set the PO bit mask of PO in which included the paging event */
                 WR_SET_PO_BIT_MASK(poBitMask, pbmInfo->pendPagMsg.po); 

                 /* Fill and send the RRC:paging message in which contains the one or
                  * multiple paging records 
                  */
                 wrPbmFillAndSndPagingMsg(&pbmInfo->pendPagMsg, cellCb);

                 /* Remove the entry from the pend PBM info Lst */
                 cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                 /* Free the memory for that pending PBM Info entry */
                 WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                 break;
              }
           case WR_PBM_TYPE_START_PAG_SIMOD:
              {
                 /* To Reuse the same entry, remove the entry from 
                  * the pendPbmInfo Lst */
                 cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                 numofPagingEvntForSiMod++;
                 pbmInfo->pendPagMsg.pf = (sfn + 1)%WR_MAX_SFN;
                 /* When ever SI Modification happens we need to Page
                  * in dfltDrxCycle number of radio frames in order
                  * to accommodate all the connected UEs
                  */
                 if(numofPagingEvntForSiMod == dfltDrxCycle)
                 {
                    numofPagingEvntForSiMod = 0;
                    pbmInfo->pbmInfoType = WR_PBM_TYPE_STOP_PAG_SIMOD;
                 }
                 /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
                 wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
                 break;
              }
           case WR_PBM_TYPE_STOP_PAG_ETWS:
           case WR_PBM_TYPE_STOP_PAG_SIMOD:
           case WR_PBM_TYPE_STOP_PAG_CMAS: /* PH04_CMAS */
              {
                 /* Remove the entry from the pend PBM info Lst */
                 cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                 /* Free the memory for that pending PBM Info entry */
                 WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                 break;
              }
            /* PH04_CMAS */
            case WR_PBM_TYPE_START_PAG_ETWS:
            case WR_PBM_TYPE_START_PAG_CMAS:
               {
                   /*  Reuse the same entry */
                   /* To Reuse the same entry, remove the entry from
                   *  the pendPbmInfo Lst */
                   cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                   /* Free the memory for that pending PBM Info entry */
                   WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                   break;
               }
           /* PH04_CMAS - end */
           /* PH04_CMAS : design change */
           case WR_PBM_TYPE_PRI_ETWS:
               {
                   /* Remove the entry from the pend PBM info Lst */
                   cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                   /* Free the memory for that pending PBM Info entry */
                   WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                   wrPwsStartPrimaryEtws(cellCb);
                   break;
               }
               case WR_PBM_TYPE_SEC_ETWS:
               {
                   /* Remove the entry from the pend PBM info Lst */
                   cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                   /* Free the memory for that pending PBM Info entry */
                   WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                   wrPwsStartSecondaryEtws(cellCb);
                   break;
               }
               case WR_PBM_TYPE_CMAS:
               {
                   /* Remove the entry from the pend PBM info Lst */
                   cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                   /* Free the memory for that pending PBM Info entry */
                   WR_FREE(pbmInfo, sizeof(WrPbmInfo));
                   wrPwsStartCmas(cellCb);
                   break;
               }
               case WR_PBM_TYPE_SIB8:
               {
                  U8   sib8Period = 0;
                  RgrSiPeriodicity sib8Prdcty;
                  WR_GET_SIB_PERIOD(cellCb, WR_SIB_8, sib8Period);
                   /* Remove the entry from the pend PBM info Lst */
                  wrEmCellSchdSiPeriodMap(sib8Period, &sib8Prdcty);
                  cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
                  pbmInfo->pendPagMsg.pf = 
                           (sfn + sib8Prdcty) % WR_MAX_SFN;
                  wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
                  wrEmmPrcSib8SchdEvnt(cellCb);
                  break;
               }
           default:
              {
                 RLOG1(L_ERROR,"Unknown PbmInfoType:[%d] is in pbmSchedInfo"
                    " Table.", pbmInfo->pbmInfoType);
                 break;
              }
        } /*end of switch */
        /* Move to the next node */
         pbmInfoLnk = temp;
     } /* end of while */

     /* pending paging mesg event present in Lst then start transmitting
      * RRC:Paging message with SI-change and/or etws-indication for all PFs and
      * POs execept some POs whcih we already included the paging event.
      */
     /* PH04_CMAS : added last condition in the if check. */
     if (cellCb->pbmCb.etwsIndPres || cellCb->pbmCb.siModPres || cellCb->pbmCb.cmasIndPres)
     {
        wrPbmProcessPagingEvents(cellCb, sfn, poBitMask);
     }
  } /* end of for to loop Cells in enb */

   RETVALUE(ROK);
} /* end of wrPbmTTIInd */

/** @brief This function is used to get the SI index for the specified SI
 *         periodicity in the specified set of SI scheduling information.
 *
 * @details
 *
 *     Function: wrPbmGetSiIndex 
 *
 *         Processing steps:
 *         - This function traverse the set of SIs(siSchdInfo) and returns the
 *           SI index when the requested SI-Periodicity matches.
 *           - If there is no match of SI-Periodicity, then returns the new SI
 *             message index.
 *           - If the numSis is 0, then returns the first SI index value is 0.
 *           
 * @param [in]   siPeriod : SI-Periodicty.
 * @param [in]     numSIs : number of SIs 
 * @param [in] siSchdInfo : SI scheduling info. 
 * @return U16
 *      -# Success : SI-index is 0 if number of ongoing SI is 0.
 *                   else matched SI index or new Si-index value.
 *      -# Failure : RFAILED
 */
PRIVATE U16 wrPbmGetSiIndex
(
 U16           siPeriod,
 U8            numSIs,
 WrSISchdInfo  *siSchdInfo
)
{
   U8 siIdx = 0;
  
   /* KW_0610 */
   if ((numSIs == 0) || (numSIs >= WR_PBM_MAX_NUM_SIS))
   {
      RLOG1(L_ERROR, "Invalid numSIs[%d] value", numSIs);
      RETVALUE(0); 
   } 
   for (siIdx = 0; siIdx < numSIs; siIdx++) /* KW Fix */ 
   { 
      if(siSchdInfo[siIdx].period == siPeriod)
      { 
         RETVALUE(siIdx); 
      } 
   } 
   siSchdInfo[numSIs].numSibs=0;
   siSchdInfo[siIdx].period = siPeriod;
   RETVALUE(numSIs);
}

/** @brief This function is used to get the SI-index of transmitting SIB.
 *   Which is used to when periodicity of the SIB changed scenario.
 *
 * @details
 *
 *     Function: wrPbmGetOnGoingSiIndex
 *
 *         Processing steps:
 *         - This function will returns the current on-going SI index which is
 *           included the requested SIB.
 *
 * @param [in] cellCb : cell control block.
 * @param [in] sibNum : SIB number
 * @return U16 
 *      -# Success : SI index of the SIB is transmitting.
 *      -# Failure : RINVALID
 */
PRIVATE U16 wrPbmGetOnGoingSiIndex
(
 WrCellCb      *cellCb, 
 U8            sibNum
)
{
   switch(sibNum)
   {
      case WR_SIB_2:
      {
        RETVALUE(cellCb->sib2.siIndex);
      }
      case WR_SIB_3:
      {
        RETVALUE(cellCb->sib3.siIndex);
      }
      case WR_SIB_4:
      {
        RETVALUE(cellCb->sib4.siIndex);
      }
      case WR_SIB_5:
      {
        RETVALUE(cellCb->sib5.siIndex);
      }
      case WR_SIB_6:
      {
       RETVALUE(cellCb->sib6.siIndex);
      }
      case WR_SIB_9:
      {
        RETVALUE(cellCb->sib9.siIndex);
      }
      /* PH04_CMAS */
      case WR_SIB_10:
      {
         if(cellCb->warningInfo)
         {
           RETVALUE(cellCb->warningInfo->sib10SiIdx);
         }
      }
      case WR_SIB_11:
      {
         if(cellCb->warningInfo)
         {
            RETVALUE(cellCb->warningInfo->sib11SiIdx);
         }
      }
      case WR_SIB_12:
      {
         if(cellCb->warningInfo)
         {
            RETVALUE(cellCb->warningInfo->sib12SiIdx);
         }
      }
      /* PH04_CMAS : end */
    
      default:
      {
        RLOG1(L_ERROR,"Unknown sib num[%d] Index checking ", 
           sibNum);
      }
   }/* end of switch */
   RETVALUE(RINVALID);
}/* end of wrPbmGetOnGoingSiIndex */

/* ccpu00144744 */
/** @brief This function is used to update the SI index of a transmitting SIB.
 *   Which is used to when a SIB is removed from transmission and the next
 *   SIB takes the index.
 *
 * @details
 *
 *     Function: wrPbmUpdateSiIndex
 *
 *         Processing steps:
 *         - This function will update the SI index
 *
 * @param [in] cellCb  : Cell control block.
 * @param [in] sibNum  : SIB number
 * @param [in] siIndex : SI index
 * @return void 
 */
PRIVATE Void wrPbmUpdateSiIndex
(
WrCellCb                     *cellCb, 
U8                           sibNum,
U8                           siIndex
)
{
   switch(sibNum)
   {
      case WR_SIB_2:
      {
         cellCb->sib2.siIndex = siIndex;
      }
      break;
      case WR_SIB_3:
      {
         cellCb->sib3.siIndex = siIndex;
      }
      break;
      case WR_SIB_4:
      {
         cellCb->sib4.siIndex = siIndex;
      }
      break;
      case WR_SIB_5:
      {
         cellCb->sib5.siIndex = siIndex;
      }
      break;
      case WR_SIB_6:
      {
         cellCb->sib6.siIndex = siIndex;
      }
      break;
      case WR_SIB_9:
      {
         cellCb->sib9.siIndex = siIndex;
      }
      break;
      /* PH04_CMAS */
      case WR_SIB_10:
      {
         if(cellCb->warningInfo)
         {
            cellCb->warningInfo->sib10SiIdx = siIndex;
         }
      }
      break;
      case WR_SIB_11:
      {
         if(cellCb->warningInfo)
         {
            cellCb->warningInfo->sib11SiIdx = siIndex;
         }
      }
      break;
      case WR_SIB_12:
      {
         if(cellCb->warningInfo)
         {
            cellCb->warningInfo->sib12SiIdx = siIndex;
         }
      }
      break;
      /* PH04_CMAS : end */
    
      default:
      {
         RLOG1(L_ERROR,"Unknown sib num[%d]", sibNum);
      }
      break;
   }
   RETVOID;
}/* end of wrPbmUpdateSiIndex */

/** @brief This function is used to get the SIB periodicity.
 *
 * @details
 *
 *     Function: wrEmmPbmGetSibPeriod
 *
 *         Processing steps:
 *         - This function returns the configured SIB periodicity dtored in cell
 *           control block.
 *
 * @param [in] cellCb : cell Control block 
 * @param [in] sibNum : SIB number 
 * @return U16 
 *      -# Success : SIB Periodicity
 *      -# Failure : RSKIP 
 *  
 *  Notes: SIB periodicity max enum value is 6 as per 36.311.
 *         Here the RSKIP is used 13 to catch failure case.
 */
PRIVATE U16 wrEmmPbmGetSibPeriod
(
 WrCellCb   *cellCb,
 U8         sibNum
)
{
   U8   sibIndex;   

   /* PH04_CMAS : For warning messages periodicity is equal to the SIB2 periodicity*/
   if( (sibNum == WR_SIB_10) || (sibNum == WR_SIB_11) || (sibNum == WR_SIB_12))
   {
        for(sibIndex = 0;sibIndex < cellCb->numSibsCfg; sibIndex++)
      {
         if (cellCb->sibsCfg[sibIndex].sibId == WR_SIB_2)
             RETVALUE(cellCb->sibsCfg[sibIndex].period);
      }
   }
   /* PH04_CMAS : end */

   for(sibIndex = 0;sibIndex < cellCb->numSibsCfg; sibIndex++)
   {
      if (cellCb->sibsCfg[sibIndex].sibId == sibNum)
      {
         RETVALUE(cellCb->sibsCfg[sibIndex].period);
      } /* end of is sib num validatation */
   } /* end of  for to traverse OAM configured sibs */
   
   RETVALUE(RSKIP);
} /* end of wrEmmPbmGetSiPeriodicity */

/** @brief This function is used to remove the SIB from ongoing SI message. 
 *
 * @details
 *
 *     Function: wrPbmRemoveSibNumFromOnGoingSIMsg
 *
 *         Processing steps:
 *         - This function will update the pending SI information with 
 *           ongoing SI message all SIBs except the remove SIB num.
 *            - Get the Si index from the pending SI msgs based on periodicity
 *              of ongoing SI
 *            - Traverse sibs of ongoing SI message which is includeded
 *              earliear the current sib and remove the current sib from 
 *              ongoing SI message i.e., update the pending SI info without 
 *              the current sib.
 *
 * @param [in, out] cellCb : cell control block
 * @param [in]      sibNum : sib number
 * @param [in]   oldSiIndx : SI index of the SIB is included in on-goiong SI
 *                           message. 
 * @return Void
 */
PRIVATE void wrPbmRemoveSibNumFromOnGoingSIMsg
(
 WrCellCb *cellCb,
 U8       sibNum,
 U8       oldSiIndx
)
{
   WrSISchdInfo  *siSchdInfo        = &(cellCb->sib1.siSchdInfo[0]);
   WrSISchdInfo  *pendSiInfo        = &(cellCb->pbmCb.pendSiInfo[0]);
   U8            pendNumSIs;
   U8            numSIs;
   U8            pendSiIdx;
   U8            idx; 
   U8            ovrIdx = 0;

   /* pendNumSIs need to updated because we may need to add 
    * new SI information for removing the SIB from another
    * SI message incase of SI-perodicity chages.*/
   pendNumSIs = cellCb->pbmCb.numSIs; 
   numSIs = cellCb->sib1.numSis;

   /* Get the Si index from the pending SI msgs based on periodicity
    * of ongoing SI */
   pendSiIdx = wrPbmGetSiIndex(siSchdInfo[oldSiIndx].period,
                               pendNumSIs, 
                               pendSiInfo); 

   if (pendSiIdx >= WR_PBM_MAX_NUM_SIS)
   {
      RLOG0(L_ERROR,"Invalid Number of SIS.");
      RETVOID;
   }  /* End of if condition.*/

   /* Traverse sibs of ongoing SI message which is includeded
    * earliear the current sib and remove the current sib from ongoing
    * SI message i.e., update the pending SI info with out the current
    * sib */
      if (pendSiIdx <  pendNumSIs)
      {
       /*  pendSiInfo[pendSiIdx].period = siSchdInfo[oldSiIndx].period; */
         for(idx = 0; idx < pendSiInfo[pendSiIdx].numSibs; idx++)
         {
            if (sibNum == pendSiInfo[pendSiIdx].sibs[idx])
            {
               for (ovrIdx = idx; ovrIdx < pendSiInfo[pendSiIdx].numSibs; ovrIdx++)
               {
                  pendSiInfo[pendSiIdx].sibs[ovrIdx] = pendSiInfo[pendSiIdx].sibs[ovrIdx+1];
                  pendSiInfo[pendSiIdx].numSibs--;
               }
            }
         }
      }
   RETVOID;
} /* end of wrPbmRemoveSibNumFromOnGoingSIMsg */
PRIVATE Bool wrPbmIsSibUpdateInPendingSiInfo
(
 WrCellCb      *cellCb,
 U8            sibNum
)
{
   WrSISchdInfo  *pendSiInfo = &(cellCb->pbmCb.pendSiInfo[0]);
   U8            pendNumSIs = cellCb->pbmCb.numSIs;
   U8            sibsIdx, pendSiIdx;
   U8            sibPeriod;

   sibPeriod = wrEmmPbmGetSibPeriod(cellCb, sibNum);
   pendSiIdx = wrPbmGetSiIndex(sibPeriod, 
                               pendNumSIs, 
                               pendSiInfo);
   if (pendSiIdx >= WR_PBM_MAX_NUM_SIS)
   {
      RLOG0(L_ERROR,"Invalid Number of SIS.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   for(sibsIdx = 0; sibsIdx < pendNumSIs; sibsIdx++)
   {
      if (sibNum == pendSiInfo[pendSiIdx].sibs[sibsIdx])
      {
         RETVALUE(TRUE);
      }
   }
   RETVALUE(FALSE);
}

/** @brief This function is used to add the new SIB or modifying the periodicity
 *         of the existed SIB to pending SI inforamtion in PBM control block.
 *
 * @details
 *
 *     Function:wrPbmIsExistedPendSiMsgUpdated 
 *
 *         Processing steps:
 *         - This function will update the pending SI Information in PBM control
 *           block with the updated SI message with all SIBs in that SI message.
 *         - If the SIB periodicity changed this function will update the
 *           pending SI information in PBM control block with two SI messages.
 *           One SI message old SI periodicity with all SIBs except removed SIB.
 *           another SI message with new SI periodicty with all existed SIB and
 *           new SIB also.
 *
 * @param [in, out] cellCb : cell control block
 * @param [in]      sibNum : SIB Number  
 * @param [in]   sibPeriod : SIB Periodicity 
 * @return Bool 
 *         TRUE : SIB is successfully added/updated to or removed from 
 *                the existed SI Msg. 
 *        FALSE : There is no match with Si periodicity.hence the new SIB is not
 *                added to any one of the on-going SI msg.
 */
PRIVATE Bool wrPbmIsExistedPendSiMsgUpdated
(
 WrCellCb      *cellCb,
 U8            sibNum,
 U16           sibPeriod
 )
{
   WrSISchdInfo  *siSchdInfo        = &(cellCb->sib1.siSchdInfo[0]);
   WrSISchdInfo  *pendSiInfo        = &(cellCb->pbmCb.pendSiInfo[0]);
   U8            pendNumSIs, numSIs = cellCb->sib1.numSis;
   Bool          alreadySibPres     = FALSE;
   Bool          pendSiInfoUpdated  = FALSE;
   Bool          sibPeriodChanged   = FALSE;
   U8            sibsIdx, siIdx, pendSiIdx, pendSibsIdx;
   U8            oldSiIndx, idx;
   
   /* Get the SI index which is mapped to which SI message earliear */
   oldSiIndx    = wrPbmGetOnGoingSiIndex(cellCb, sibNum);
   if (oldSiIndx >= WR_PBM_MAX_NUM_SIS)
   {
      RLOG0(L_ERROR,"Invalid Number of SIS");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   pendNumSIs = cellCb->pbmCb.numSIs;

   if (sibPeriod != siSchdInfo[oldSiIndx].period)
   {
      sibPeriodChanged = TRUE;
      wrPbmRemoveSibNumFromOnGoingSIMsg(cellCb, sibNum, oldSiIndx);
      /* Get the Pending SI index based on periodicity matches */
      pendSiIdx = wrPbmGetSiIndex(siSchdInfo[oldSiIndx].period, 
            pendNumSIs, 
            pendSiInfo);
      if (pendSiIdx >= WR_PBM_MAX_NUM_SIS)
      {
         RLOG0(L_ERROR,"Invalid Number of SIS.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/

      /* For updating new pending SI message, pendSiIdx should equal to pendNumSIs */
      if ((0 == pendSiInfo[pendSiIdx].numSibs) && (pendSiIdx == pendNumSIs))
      {
            cellCb->pbmCb.numSIs++;
            pendSiInfo[pendSiIdx].period = siSchdInfo[oldSiIndx].period; 
         for(pendSibsIdx = 0, idx = 0; (idx < siSchdInfo[oldSiIndx].numSibs); idx++)
         {
            if (sibNum != siSchdInfo[oldSiIndx].sibs[idx])
            {
               pendSiInfo[pendSiIdx].sibs[pendSibsIdx++] = siSchdInfo[oldSiIndx].sibs[idx]; 
               pendSiInfo[pendSiIdx].numSibs++;
            }
         } 
      }
      else
      {
      }
   }
   pendNumSIs = cellCb->pbmCb.numSIs;
   /* Get the Pending SI index based on periodicity matches */
   pendSiIdx = wrPbmGetSiIndex(sibPeriod, pendNumSIs, pendSiInfo); 
   if (pendSiIdx >= WR_PBM_MAX_NUM_SIS)
   {
      RLOG0(L_ERROR,"Invalid Number of SIS.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   
   /* Loop to traverse all SI msgs in Schduling info list in SIB1 */
   for (siIdx = 0; siIdx < numSIs; siIdx++)
   {
      if (sibPeriod == siSchdInfo[siIdx].period)
      {
         if ((0 == pendSiInfo[pendSiIdx].numSibs) &&
              (pendNumSIs == pendSiIdx) )
         {
            cellCb->pbmCb.numSIs++;
            pendSiInfo[pendSiIdx].period = sibPeriod; 
         } else {
            for( pendSibsIdx=0; pendSibsIdx < pendSiInfo[pendSiIdx].numSibs; pendSibsIdx++)
            {
               if(sibNum == pendSiInfo[pendSiIdx].sibs[pendSibsIdx])
               {
                  alreadySibPres = TRUE;
                  pendSiInfoUpdated = TRUE;
               }
            } 

         }
          
         /* Loop to travers all sibs in this SI msg */
         for (sibsIdx = 0; sibsIdx < siSchdInfo[siIdx].numSibs; sibsIdx++)
         {
            pendSibsIdx = pendSiInfo[pendSiIdx].numSibs;
            {
               if (pendSiInfo[pendSiIdx].sibs[pendSibsIdx] != 
                     siSchdInfo[siIdx].sibs[sibsIdx])
               {
                  if (sibNum == siSchdInfo[siIdx].sibs[sibsIdx])
                  {
                     alreadySibPres = TRUE;
                     pendSiInfoUpdated = TRUE;
                  } 
                  /* copy the sib num from ongoing SI msg to pending SI msg */ 
                  if (FALSE == sibPeriodChanged)
                  {
                     if (FALSE  == wrPbmIsSibUpdateInPendingSiInfo(cellCb,
                                                siSchdInfo[siIdx].sibs[sibsIdx]))
                     { 
                        pendSiInfo[pendSiIdx].sibs[pendSibsIdx] = 
                                            siSchdInfo[siIdx].sibs[sibsIdx];
                        pendSiInfo[pendSiIdx].numSibs++;
                     }
                  }
                  else
                  {
                     RLOG0(L_INFO, "Updating the pending si info with same periodicity"
                          "if there is already ongoing SIBs");
                     pendSiInfo[pendSiIdx].sibs[pendSiInfo[pendSiIdx].numSibs] = 
                                                   siSchdInfo[siIdx].sibs[sibsIdx];
                     pendSiInfo[pendSiIdx].numSibs++;
                  }
               }
            }
         } /* end of for to traverse the sibs on going in this SI msg */

         /* current SIB is not an ongoing sib i.e.,new SIB to add this SI msg */
         if (FALSE == alreadySibPres)
         {
            pendSiInfo[pendSiIdx].sibs[pendSiInfo[pendSiIdx].numSibs] = sibNum;
            pendSiInfo[pendSiIdx].numSibs++;
            pendSiInfoUpdated = TRUE;
         }
      } /* end of if the sib periodicity matches with ongoing SI periodicity */
   } /* end of for loop for all SI's schduling info in SIB1 */
  RETVALUE(pendSiInfoUpdated);
} /* end of wrPbmIsExistedPendSiMsgUpdated */

/** @brief This function is used to update the pending SIBs info in PBMCB.
 *
 * @details
 *
 *     Function: wrPbmUpdatePendSiInfo
 *
 *         Processing steps:
 *         - This function is invoked during adding new SIB or modifying
 *           the already existed SIB periodicty.
 *
 * @param [in, out] cellCb : cell control block
 * @param [in]      sibNum : SIB number. 
 * @return S16 
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PRIVATE S16 wrPbmUpdatePendSiInfo
(
 WrCellCb *cellCb,
 U8        sibNum
)
{
   WrSISchdInfo  *pendSiInfo = &(cellCb->pbmCb.pendSiInfo[0]);
   U8            pendNumSIs;
   U16           sibPeriod;
   U8            pendSiIdx;

   sibPeriod = wrEmmPbmGetSibPeriod(cellCb, sibNum);
   if (sibPeriod == RSKIP)
   {
      RLOG1(L_ERROR,"Peridicity is unknown for this SIB: [%d]", sibNum);
      RETVALUE(RFAILED);
   }
   
   /* new Sib with new periodicity which is not match with any ongoing 
    * SI-periodidicy then create a new SI msg */
   if (FALSE == wrPbmIsExistedPendSiMsgUpdated(cellCb, sibNum, sibPeriod))
   {
      pendNumSIs = cellCb->pbmCb.numSIs; 
     
      /* Get the Pending SI index based on if periodicity matches. 
       * if not matches get new SI index */
      pendSiIdx = wrPbmGetSiIndex(sibPeriod, pendNumSIs, pendSiInfo);
      if (pendSiIdx >= WR_PBM_MAX_NUM_SIS)
      {
         RLOG0(L_ERROR,"Invalid Number of SIS.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/

      /* "numSibs" in pendSI message is 0,i.e.,pending si msgs will increment */
      if ((0 == pendSiInfo[pendSiIdx].numSibs) &&
          (pendNumSIs == pendSiIdx))
      {
         /*127802*/
         RLOG2(L_INFO,"sibNum:[%d], Pending NumSIs:[%d]",sibNum, cellCb->pbmCb.numSIs);
          cellCb->pbmCb.numSIs++;
          pendSiInfo[pendSiIdx].period = sibPeriod; 
      }
      pendSiInfo[pendSiIdx].sibs[pendSiInfo[pendSiIdx].numSibs] = sibNum;
      pendSiInfo[pendSiIdx].numSibs++;
   } /* end of if for new sib wih new periodicity */

  RETVALUE(ROK);
} /* end of wrPbmUpdatePendSiInfo */

/** @brief This function is used to add/del to new SIB to SI scheduling info in
 *         SIB1.
 *
 * @details
 *
 *     Function: wrPbmUpdSiSchedInfoLstInSIB1
 *
 *         Processing steps:
 *         - This function is invoked during cell configuration and ETWS SIBs
 *           are directly adding/removeing to SIB1 scheduling info. 
 *         - Based on the configured SIB periodicity, update the SI SIB num to
 *           the already existed SI mapping info in SIB1 and update the SI 
 *           mesage Index through output parameter "retransSiIndx".
 *         - Based on the "action" parameter, add/remove the SIB number to the
 *           already existed SI message if SIB periodicity and SI message
 *           periodicy matches.If there is no match of SI periodicty, trying to
 *           add new SIB new SI periodicty creates new SI message.
 *
 * @param [in, out]     cellCb : cell control block 
 * @param [in]          action : Action is either ADD/DEL on the specified SIB.  
 * @param [in]          sibNum : sib number 
 * @param [out]  retransSiIndx : Modified SI index is valid if number of 
 *                             transmit SIs not changed.
 * @return S16 
 *               WR_PBM_SEND_NONE : Failure case.
 *         WR_PBM_SEND_ONE_SI_MSG : SIB is added to  or delted from already
 *                                  existed SI messsage.reconfigur to MAC only
 *                                  one SI msg which is mentioned in
 *                                  retransSiIndx output parameter.
 *         WR_PBM_SEND_ALL_SI_MSG : SIB Add or delete causes to change numSIs,
 *                                  i.e.,new SI msg is created or stopped
 *                                  already transmitting SI because of no SIBs.
 *                                  Hence re-configure all SI msgs to MAC.
 *         
 */
PUBLIC S16 wrPbmUpdSiSchedInfoLstInSIB1
(
 WrCellCb      *cellCb,
 WrSibAction   action,
 U8            sibNum,
 U8            *retransSiIndx
)
{
   U16           sibPeriod;
   U8            sibsIdx;
   U8            siIdx = WR_PBM_SEND_NONE; 
   U8            sibNumCrnt;      

   RLOG2(L_DEBUG,"Received SIB1-Schd-Info-Lst Update for Sib-Number[%d]" 
                  " Action[%d]",sibNum,action);
   switch(action)
   {
      case WR_SIB_ACTION_ADD:
      {    
           sibPeriod = wrEmmPbmGetSibPeriod(cellCb, sibNum); 
           if (sibPeriod == RSKIP)
           {
              RLOG1(L_ERROR,"Peridicity is unknown for this SIB:[%d]",sibNum);
              RETVALUE(WR_PBM_SEND_NONE);
           }
          /* PH04_CMAS : Added the if check*/
           if((sibNum != WR_SIB_10) && (sibNum != WR_SIB_11) && (sibNum != WR_SIB_12))
           {
             for (siIdx = 0; siIdx < cellCb->sib1.numSis; siIdx++)
             {
                if (sibPeriod == cellCb->sib1.siSchdInfo[siIdx].period)
                {
                  /* periodicity matches with any one of the on-going SI msg */
                  /* traverse the SIBs in SI msg */
                  for (sibsIdx = 0; sibsIdx < cellCb->sib1.siSchdInfo[siIdx].numSibs; sibsIdx++)
                  {
                      sibNumCrnt = cellCb->sib1.siSchdInfo[siIdx].sibs[sibsIdx];

                      if((sibNumCrnt != WR_SIB_10) && (sibNumCrnt != WR_SIB_11) && (sibNumCrnt != WR_SIB_12))
                      {
                        if (sibNum == cellCb->sib1.siSchdInfo[siIdx].sibs[sibsIdx])
                        {
                           *retransSiIndx = siIdx;
                           RETVALUE(WR_PBM_SEND_ONE_SI_MSG);
                        }
                      }
                  } /* end of for to traverse the Sibs in SI msg */

                  /* sibNum is not present in the SI msg, then add the sibNum to
                  * this SI msg*/
                  cellCb->sib1.siSchdInfo[siIdx].sibs[cellCb->sib1.siSchdInfo[siIdx].numSibs++] = sibNum;
                  *retransSiIndx = siIdx;
                  RETVALUE(WR_PBM_SEND_ONE_SI_MSG);
                }/* end of if */
             }/* end of for to traverse all SI msgs */
          } /* PH04_CMAS */


           /* new Sib periodicity not matched with any one of the 
            * on-going SI msg periodicty. Hence we are creating new SI msg */
           cellCb->sib1.siSchdInfo[cellCb->sib1.numSis].numSibs = 0;
           cellCb->sib1.siSchdInfo[cellCb->sib1.numSis].period = sibPeriod;
           *retransSiIndx = cellCb->sib1.numSis;
           cellCb->sib1.siSchdInfo[cellCb->sib1.numSis].
               sibs[cellCb->sib1.siSchdInfo[cellCb->sib1.numSis].numSibs++] = sibNum;
           cellCb->sib1.numSis++;
           RETVALUE(WR_PBM_SEND_ALL_SI_MSGS);
      }
      case WR_SIB_ACTION_REMOVE:
      {
         siIdx = wrPbmGetOnGoingSiIndex(cellCb, sibNum);
         if (siIdx >= WR_PBM_MAX_NUM_SIS)
         {
            RLOG0(L_ERROR,"Invalid Number of SIS.");
            RETVALUE(RFAILED);
         }/* End of if condition.*/

         /* remove the SIB num frm SI msg if Si index is valid */
         if (RINVALID != siIdx)
         {
            /* traverse all sibs in the SI msg and remove the sibNum from SI
             * msg */
            for (sibsIdx = 0; 
                  sibsIdx < cellCb->sib1.siSchdInfo[siIdx].numSibs; 
                  sibsIdx++)
            {
               if(sibNum != cellCb->sib1.siSchdInfo[siIdx].sibs[sibsIdx]) 
               {
                  continue;
               } /* end of if to skip all the sibNum doesn't matches */

               /* skipped the sibNum when it matches the sibnum in SI-msg */
               cellCb->sib1.siSchdInfo[siIdx].sibs[sibsIdx] = 
                  cellCb->sib1.siSchdInfo[siIdx].sibs[sibsIdx+1];
            } /* end of for to traverse all SIBs*/

            /* decrement the numSibs in that SI msg */
            /* ccpu00144744 */
            if(cellCb->sib1.siSchdInfo[siIdx].numSibs)
            {
               cellCb->sib1.siSchdInfo[siIdx].numSibs--;
            }
            else
            {
               RLOG2(L_ERROR,"Invalid SIB removal for Sib-Number[%d]"
                     "Sib-Index[%d]",sibNum, siIdx);

               RETVALUE(RFAILED);
            }

            /* Validate that need to cfg MAC with 
             * only one SI msg or all SI Msgs */
            if (0 != cellCb->sib1.siSchdInfo[siIdx].numSibs)
            {
               *retransSiIndx = siIdx;
               RETVALUE(WR_PBM_SEND_ONE_SI_MSG);
            } 
            else 
            {
               /* remove the SI msg from the list of SI msgs in SIB1 */
               for (; siIdx < cellCb->sib1.numSis; siIdx++)
               {
                  cmMemcpy((U8 *) &(cellCb->sib1.siSchdInfo[siIdx]), 
                        (U8 *) &(cellCb->sib1.siSchdInfo[siIdx+1]), 
                        sizeof(WrSISchdInfo));
                  /* ccpu00144744 */
                  for (sibsIdx = 0; 
                        sibsIdx < cellCb->sib1.siSchdInfo[siIdx].numSibs; 
                        sibsIdx++)
                  {
                     wrPbmUpdateSiIndex(cellCb, 
                           cellCb->sib1.siSchdInfo[siIdx].sibs[sibsIdx], siIdx);
                  }
               }
               /* Decrement Num of valid Sis */
               cellCb->sib1.numSis--;
               RETVALUE(WR_PBM_SEND_ALL_SI_MSGS);
            } /* end of else part for Num od SIBs in SI msg is 0 */
         }
         break;
      }
      default:
           RLOG1(L_ERROR,"Unknown action: [%d] trying to "
              "perform on the SIB", action);
           break;
   } /* end of switch to perform sib action */
   RETVALUE(WR_PBM_SEND_NONE);
} /* end of wrPbmUpdSiSchedInfoLstInSIB1 */

/** @brief This function is used to process the ETWS SIBS trgger during TTI
 *         indication reached to SIB10 and/or SIB11 periodicty reached.
 *
 * @details
 *
 *     Function: wrEmmPbmProcEtwsSibs 
 *
 *         Processing steps:
 *         - Based on the etwsSibMappingInfo, configure the MAC with all SI
 *           messages or ETWS included specific SI messages only.
 *         - Build and configure the SI message to MAC layer by using the
 *           function wrEmmPbmSiConfig.
 *
 * @param [in]             cellCb : cell control block
 * @param [in]        numEtwsSibs : number of ETWS SIBS. 
 * @param [in]  etwsSibMapingInfo : ETWS SIBs to SI mapping inforamtion. 
 * @return S16 
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PRIVATE S16 wrEmmPbmProcEtwsSibs
(
 WrCellCb    *cellCb, 
 U8          numEtwsSibs, 
 WrSibMaping *etwsSibMapingInfo
)
{
   U8 siIdx = 0;

   if (WR_MAX_NUM_ETWS_SIBS == numEtwsSibs)
   {
      if ((WR_PBM_SEND_ONE_SI_MSG == etwsSibMapingInfo[0].reCfgOneOrAllSis) && 
          (WR_PBM_SEND_ONE_SI_MSG == etwsSibMapingInfo[1].reCfgOneOrAllSis))
      {
         if (etwsSibMapingInfo[0].siIndx == etwsSibMapingInfo[1].siIndx)
         {
            /* transmit SI msg which is included both SIB10 and SIB11 sibs */
            if (wrEmmPbmSiConfig(cellCb->cellId, 
                     etwsSibMapingInfo[0].siIndx) != ROK)
            {
               RETVALUE(RFAILED);
            }
         } 
         else 
         {
            if ((etwsSibMapingInfo[0].sibAction == WR_SIB_ACTION_ADD) || 
                (etwsSibMapingInfo[1].sibAction == WR_SIB_ACTION_ADD))
            {
               for (siIdx = 0; siIdx < numEtwsSibs; siIdx++)
               {
                  /* Reconfigureing the MAC with one SI msg at one time */
                  if (wrEmmPbmSiConfig(cellCb->cellId, 
                           etwsSibMapingInfo[siIdx].siIndx) != ROK)
                  {
                     RETVALUE(RFAILED);
                  }
               }
            }
            else if((etwsSibMapingInfo[0].sibAction == WR_SIB_ACTION_REMOVE) ||
                    (etwsSibMapingInfo[1].sibAction == WR_SIB_ACTION_REMOVE))
            {
               wrEmmPbmCellSchdReCfg(cellCb->cellId);
               /* TODO: SICFG with NULL PDU is requred */
            }
         }
      }
      /* re-configure MAC with all SIs. because of either SIB10 or SIB11 
       * inclding in SI msgs causes numSIs changed */
      else if ((WR_PBM_SEND_ALL_SI_MSGS == 
                                   etwsSibMapingInfo[0].reCfgOneOrAllSis) ||
               (WR_PBM_SEND_ALL_SI_MSGS == 
                                   etwsSibMapingInfo[1].reCfgOneOrAllSis))
      {
         if ((etwsSibMapingInfo[0].sibAction == WR_SIB_ACTION_ADD) || 
               (etwsSibMapingInfo[1].sibAction == WR_SIB_ACTION_ADD))
         {
            /* reconfigure all SIs to MAC, because numSIs are changed */
            for (siIdx = 0; siIdx < cellCb->sib1.numSis; siIdx++)
            {
               if (wrEmmPbmSiConfig(cellCb->cellId, siIdx) != ROK)
               {
                  RETVALUE(RFAILED);
               }
            } /* end of for */
         } 
         else if((etwsSibMapingInfo[0].sibAction == WR_SIB_ACTION_REMOVE) ||
                    (etwsSibMapingInfo[1].sibAction == WR_SIB_ACTION_REMOVE))
         {
              wrEmmPbmCellSchdReCfg(cellCb->cellId);
               /* TODO: SICFG with NULL PDU is requred */
         }
      } 
      else 
      {
         RETVALUE(RFAILED);
      }
   } 
   else  /* only one ETWS sib is triggered */
   {
      if ((WR_PBM_SEND_ONE_SI_MSG == etwsSibMapingInfo[0].reCfgOneOrAllSis))
      {
         if (wrEmmPbmSiConfig(cellCb->cellId, siIdx) != ROK)
         {
            RETVALUE(RFAILED);
         }
      }
      else if (WR_PBM_SEND_ALL_SI_MSGS == etwsSibMapingInfo[0].reCfgOneOrAllSis)
      {
         if (etwsSibMapingInfo[0].sibAction == WR_SIB_ACTION_ADD)
         {
            /* reconfigure all SIs to MAC, because numSIs are changed */
            for (siIdx = 0; siIdx < cellCb->sib1.numSis; siIdx++)
            {
               if (wrEmmPbmSiConfig(cellCb->cellId, siIdx) != ROK)
               {
                  RETVALUE(RFAILED);
               }
            } /* end of for */
         }
         else if(etwsSibMapingInfo[0].sibAction == WR_SIB_ACTION_REMOVE) 
         {
              wrEmmPbmCellSchdReCfg(cellCb->cellId);
               /* TODO: SICFG with NULL PDU is requred */
         }
      } /* else part for re-configuring all SI msgs */
      else 
      {
         RETVALUE(RFAILED);
      }
   } /* end of else part for processing one etws SIB trigger */

   RETVALUE(ROK);
} /* end of wrEmmPbmProcEtwsSibs */

/** @brief This function is used to add or update the sibs which needs to
 *         broadcast.
 *
 * @details
 *
 *     Function: wrPbmAddOrUpdateSibs
 *
 *         Processing steps:
 *         - Trigger the RRC:Paging message with SI-change for only frst SIB in
 *           list of sibs and also except for SIB10 and SIB11.
 *         - Start RRC:Paging message with etws-ind for only frst SIB in 
 *           list of sibs SIB10 and SIB11 iff already is etws-ind is not enbaled
 *         - In case of SIB10 and SIB11 only, update the SIB1 Scheduling info 
 *           SIB10 and/or SIB11 periodicity. Immediately (re)configure  the MAC
 *           either with etws sibs carried SI msgs only or all SI msgs.
 *         - In case of other SIBs, Update Pending SI information with the
 *           newly triggered SIBs included SI meessags.
 *         - Validate the One complete DRX cycle is possible or not in the
 *           current modificaion period and also validate paging message can
 *           retransmit with the configured number of "n" DRX cycles with in 
 *           the Modification Perioed. Based on these set the value of
 *           "pbmCycleCnt" and "PF".
 *         - Perform the lookup in pbmSchedInfo table with in the cell based on
 *           PF, pbmCycleCnt and PBM entry type is WR_PBM_TYPE_START_SI_MOD.
 *           If the pbmInfo entry is not present, then create an entry and
 *           update the all values and insert into the pbmSchedInfo table.
 *         - This pbmInfo entry will be processed just before the next
 *           modification period to configure the MAC with all SIs or SIBs 
 *           includde SI message only.
 *         - This function will be invoked by following modules.
 *            -# ANR
 *            -# EnodeB Overload Module
 *            -# PWS
 *            -# EMM during cell deleting.
 * @param [in] numSibs : number of SIBs need to add or update. 
 * @param [in]    sibs : set of SIB numbers. 
 * @param [in]  cellId : cell Id. 
 * @return S16 
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PUBLIC S16 wrPbmAddOrUpdateSibs
(
 U8   numSibs,
 U8   *sibs,  
 U16  cellId
)  
{
   WrCellCb      *cellCb    = NULLP;
   WrPbmInfo     *pbmInfo   = NULLP;
   U16           nxtModifPerdSfn;
   U16           modifPerdCoeff;
   U16           bestModifPerdSfn;
   U16           dfltDrxCycle;
   U16           curSfn;
   U8            sibIndx;
   Bool          skipPagingTrigger = FALSE;
   U8            sibNum;
    
   /* Get the cellCb  based on CellId */
   WR_GET_CELLCB(cellCb, cellId);

   /* KW_0610 */
   if ((NULLP == cellCb) || (numSibs >= WR_MAX_SIBS))
   {
      RLOG1(L_ERROR, "Invalid numSibs[%d] Value", numSibs);
      RETVALUE(RFAILED);
   }
   /*Fix for ccpu00131680*/
   if (cellCb->cellState != WR_CELL_STATE_UP  && cellCb->cellState != WR_CELL_STATE_PWR_DOWN)
   {
      RETVALUE(RFAILED);
   }
   /* get the currnet Sfn */
   curSfn = cellCb->pbmCb.curSfn;
   WR_GET_PAGING_CYCLE(cellCb->sib2.pcchCfg.defaultPagingCycle,
         dfltDrxCycle);

 
   RLOG_ARG0(L_INFO,DBG_CELLID, cellId,
      "Received Paging for BCCH modification");

   for (sibIndx = 0; sibIndx < numSibs; sibIndx++) /* KW Fix */
   {
      /* get the first sib and trgger the paging msg with si-change */
      sibNum = sibs[sibIndx];

      /* PH04_CMAS */
      if ((sibNum != WR_SIB_10) && (sibNum != WR_SIB_11) && (sibNum != WR_SIB_12))
      {
         /* Start paging message with SI-change for only frst SIB in 
          * list of sibs and also except for SIB10 and SIB11 */
         if ((FALSE == cellCb->pbmCb.siModPres) && 
             (FALSE == skipPagingTrigger))
         {
            skipPagingTrigger = TRUE;
            if(ROK != wrPbmStartPagingMsg(cellCb, WR_PAGE_SYS_SI_CHAGE))
            {
               RETVALUE(RFAILED);
            }
         }
        
         /* Update Pending SI information */
         wrPbmUpdatePendSiInfo(cellCb, sibNum);
      } 
   } /* end of for to travers all sibs */

   /* get the sfn of the next modification period */ 
   nxtModifPerdSfn = wrCalcNexModPeriodSFN(cellCb, curSfn);
   
   /* validate the One complete DRX cycle is possible or not
    * in this modificaion period and also configured repeat "n" DRX
    * cycles. Based on these set the value of "pbmCycleCnt" and "PF".
    */
    if ((TRUE == wrPbmIsPagDrxCycleFit(cellCb, curSfn)))
    { 
       bestModifPerdSfn = nxtModifPerdSfn;
     nxtModifPerdSfn = (nxtModifPerdSfn < curSfn)? (nxtModifPerdSfn + WR_MAX_SFN):
         nxtModifPerdSfn;
    } 
    else 
    {/* Get next next modification period SFN */
       WR_GET_MODIFI_COEFF(cellCb->sib2.bcchCfg.modifPeriodCoeff, 
                           modifPerdCoeff);
       
       bestModifPerdSfn = ((nxtModifPerdSfn + (modifPerdCoeff*dfltDrxCycle)) %
                                                              WR_MAX_SFN);
     bestModifPerdSfn = (bestModifPerdSfn < curSfn)? (bestModifPerdSfn + WR_MAX_SFN):
         bestModifPerdSfn;
   }
   /* Fix for the cr ccpu00125100 */
   bestModifPerdSfn = bestModifPerdSfn % WR_MAX_SFN;
   /* Perform the lukp in pbmSchedInfo table based on pf as key */
   if ((FALSE == wrPbmIsPendingPbmInfoPres(bestModifPerdSfn, cellCb,
                                           WR_PBM_TYPE_START_SI_MOD)))
   {
      WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
      if(NULLP == pbmInfo)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      } 

      /* fill the pbmInfo entry */
      pbmInfo->pbmInfoType = WR_PBM_TYPE_START_SI_MOD;
      /* Updating SCH with new SI info .
       * dfltDrxCycle SFNs in-advance to the Modification boundary
       * For eg dfltDrxCycle is 64 rf and bestModifPerdSfn is 100
       * updating the SCH with new SI info at 100-64 = 36 SFN
       * */
      pbmInfo->pendPagMsg.pf = (((bestModifPerdSfn - dfltDrxCycle) + WR_MAX_SFN) % WR_MAX_SFN);

      /* Insert to "pbmSchedInfo" table based on SFN or PF in pbmInfo */
      wrPbmAddPendPbmInfoEntry(cellCb, pbmInfo);
   }
   
   RETVALUE(ROK);
} /* end of wrPbmAddOrUpdateSibs */

PUBLIC S16 wrPbmDelSibs
(
 U8   numSibs,
 U8   *sibs,  
 U16  cellId
)  
{
   WrCellCb      *cellCb = NULLP;
   WrSibMaping   etwsSibMapingInfo[2];
   U8            sibIndx = 0;
   U8            sibNum = 0, numEtwsSibs = 0;
   U8            reCfgOneOrAllSis;
   WrSISchdInfo  *pendSiInfo = &(cellCb->pbmCb.pendSiInfo[0]);
   U8            pendNumSIs;
   U16           sibPeriod;
   U8            pendSiIdx, oldSiIndx;

   /* Get the cellCb  based on CellId */
   WR_GET_CELLCB(cellCb, cellId);

   if (cellCb == NULL)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"Could not find Cell Control Block");
      RETVALUE(RFAILED);
   }
  
   /* Get SIB period */ 
   sibPeriod = wrEmmPbmGetSibPeriod(cellCb, sibNum);
   if (sibPeriod == RSKIP)
   {
      RLOG1(L_ERROR,"Peridicity is unknown "
         "for this SIB: [%d]", sibNum);
      RETVALUE(RFAILED);
   }

   for (sibIndx = 0; sibIndx < numSibs; sibIndx++)
   {
      sibNum = sibs[sibIndx];

      if ((sibNum == WR_SIB_10) || (sibNum == WR_SIB_11))
      {
         etwsSibMapingInfo[numEtwsSibs].sibNum = sibNum;
         reCfgOneOrAllSis = wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
                                      WR_SIB_ACTION_REMOVE,
                                      sibNum,
                                      &etwsSibMapingInfo[numEtwsSibs].siIndx);
         
         etwsSibMapingInfo[numEtwsSibs].reCfgOneOrAllSis = reCfgOneOrAllSis;
         etwsSibMapingInfo[numEtwsSibs].sibAction = WR_SIB_ACTION_REMOVE;
         numEtwsSibs++;
        
         
         pendNumSIs = cellCb->pbmCb.numSIs;

         /* Get the Pending SI index based on periodicity matches */
         pendSiIdx = wrPbmGetSiIndex(sibPeriod, pendNumSIs, pendSiInfo); 

         /* TODO: Update the removed SIBs in Pending SI information in PBM 
          * Control Block iff the same SI is present in pending SI info only.
          */
         if (pendSiIdx < pendNumSIs)
         {
            /* Get the SI index which is mapped to which SI message earliear */
            oldSiIndx    = wrPbmGetOnGoingSiIndex(cellCb, sibNum);
            if (oldSiIndx >= WR_PBM_MAX_NUM_SIS)
            {
               RLOG0(L_ERROR,"Invalid Number of SIS.");
               RETVALUE(RFAILED);
            }/* End of if condition.*/

            wrPbmRemoveSibNumFromOnGoingSIMsg(cellCb, sibNum, oldSiIndx);
         }
      } /* end of if block for only on removal of etws-SIBs processing.*/
   } /* end of for to traverse all deleting Sibs */
 
   /* Immediately (re)configure  the MAC either with etws sibs carried SI
    * msgs only or all SI msgs 
    */
   if (numEtwsSibs > 0)
   {
      wrEmmPbmProcEtwsSibs(cellCb, numEtwsSibs, etwsSibMapingInfo);
      RETVALUE(ROK);
   }
   
   RETVALUE(ROK);
} /* end of wrPbmDelSibs */

/** @brief This function is used to trigger the dynamic neighbor update.
 * @details 
 *
 *     Function: wrEmmPbmTrgNrUpdt 
 *
 *         Processing steps:
 *         -  This functions group the SIB4/SIB5/SIB8 update trigger from
 *            EMM ANR Module.
 *         - Validate that is Intra Frequency Neighbour is updated, then add
 *           the pending sibs with SIB4
 *         - Validate that is Inter Frequency Neighbour is updated, then add
 *           the pending sibs with SIB5
 *         - Validate that is Inter RAT  Neighbour is updated, then add
 *           the pending sibs with SIB8
 *         - Finally call the grouped SIBs into pending Sibs in PBMCb in one
 *           call by using function @sa wrPbmAddOrUpdateSibs
 *
 *
 * @param[in] cellId       : Cell Id 
 * @param[in] updtNrBitMap : Bit Map for Inter, Intra and CDMA 2K Neighbour
 *                           update from ANR module.
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmTrgNrUpdt(U16 cellId, U8 updtNrBitMap)
{
   U8                        numSibs = 0;
   U8                        sibs[WR_MAX_SIBS];

   /* is Intra Frequency Neighbour is updated */
   if (WR_ANR_INTRA_FREQ_BIT & updtNrBitMap)
   {
      sibs[numSibs++] = WR_SIB_4; 
   }

   /* is Inter Frequency Neighbour is updated */
   if (WR_ANR_INTER_FREQ_BIT & updtNrBitMap)
   {
      sibs[numSibs++] = WR_SIB_5; 
   }

   /* is CDMA 2K Neighbour is updated */
   if((WR_ANR_CDMA_1XRTT_FREQ_BIT & updtNrBitMap) || 
      (WR_ANR_CDMA_HRPD_FREQ_BIT & updtNrBitMap) || 
      (WR_ANR_CDMA_1XRTT_BAND_BIT & updtNrBitMap))
   {
      sibs[numSibs++] = WR_SIB_8; 
   }

   if(WR_ANR_INTRA_FREQ_SIB3_BIT & updtNrBitMap)
   {
      sibs[numSibs++] = WR_SIB_3;
   }

   /* is UTRA Neighbour updated */
   if (WR_ANR_UTRA_FREQ_BIT & updtNrBitMap)
   {
      sibs[numSibs++] = WR_SIB_6;
   }

  /* is GERA NNeighbour updated 
   * but currently not supporting dynamic updates of GERAN rat neighbors */
   if (WR_ANR_GERAN_FREQ_BIT & updtNrBitMap)
   {
      sibs[numSibs++] = WR_SIB_7;
   }

   /* Update the Pending SIBs Information */ 
   if (RFAILED == wrPbmAddOrUpdateSibs(numSibs, sibs, cellId))
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrEmmPbmTrgNrUpdt */
/** @brief This function Initialize the PBM Module by EMM module after MIB,
 *         SIBs parameters configured.
 * @details
 *
 *     Function : wrEmmPbmCellCfg
 *
 *         Processing steps:
 *         - Intialise all SIs numSibs to 0.
 *         - Allocate memory for all elements pbmSchedInfo table.
 *         - Allocate memory for allPfsAndPos
 *         - Calculate and update the table "allPfsAndPos" which is used 
 *           to send RRC:Paging message with SI-change or etws indication 
 *           for all PFs and POs.
 *         - Update SIB1 SI Scheduling Mapping info for all configured Sibs.
 *         - This function will be invoked by following modules.
 *            -# EMM during cell creating
 *
 * @param [in, out] cellCb : cell control block
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmCellCfg
(
 WrCellCb *cellCb
)
{
   U16          dfltPagCycFactor = cellCb->sib2.pcchCfg.defaultPagingCycle;
   U8           nBFactor         = cellCb->sib2.pcchCfg.nB; 
   U8           *poArray         = NULLP;
   U8           numSibs          = cellCb->numSibsCfg;
   WrSibCfg     *sibsCfg         = &cellCb->sibsCfg[0];
   U8           sibIndex         = 0;
   U8           siIndex          = 0;
   S16          pbmInfoIndex, sfnIndex = 0;
   U8           ignoreSiIndex;
   U8           modifCoeff;

   
   cellCb->sib1.numSis = 0;

   /* Validate the Repeat number of paging cycles */
   cellCb->pbmCb.repeatNumPagCycles = 1;
   WR_GET_MODIFI_COEFF(cellCb->sib2.bcchCfg.modifPeriodCoeff, modifCoeff);
   if (cellCb->pbmCb.repeatNumPagCycles > modifCoeff)
   {
      RETVALUE(RFAILED);
   }
   /* TODO: We need to identify similar validations for PBM and PWS modules used
    * configuration varibles */

   /* Intialise all SIs numSibs to 0. */
   for(siIndex = 0; siIndex < WR_PBM_MAX_NUM_SIS; siIndex++)
   {
      cellCb->sib1.siSchdInfo[siIndex].numSibs = 0;
   }
   
   /* allocate memory for all elements pbmSchedInfo table */ 
   for (pbmInfoIndex = 0;
        pbmInfoIndex < WR_PBM_MAX_SCHED_ENTRYS; 
        pbmInfoIndex++)
   {
      WR_ALLOC(&(cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex]), sizeof(CmLListCp));
      if(NULLP == cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex])
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         /* Free the allocated memory */
         for(pbmInfoIndex=pbmInfoIndex-1; (pbmInfoIndex >= 0); pbmInfoIndex--)
            WR_FREE(cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex], sizeof(CmLListCp));

         RETVALUE(RFAILED);
      }

      /* initilize the CmLListCp with NULL values. */
      cmLListInit(cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex]);
   } /* end of for to initialize the pbm schduling info table */

   /* Allocate memory for allPfsAndPos */ 
   WR_ALLOC(&cellCb->pbmCb.allPfsAndPos, (WR_MAX_SFN * sizeof(U8 *)));
   if(NULLP == cellCb->pbmCb.allPfsAndPos)
   {
      RLOG0(L_FATAL," Memory allocation failed.");
      /* Freeing the above allocated memory */
      for(pbmInfoIndex=0; (pbmInfoIndex < WR_PBM_MAX_SCHED_ENTRYS); pbmInfoIndex++)
          WR_FREE(cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex], sizeof(CmLListCp));
      RETVALUE(RFAILED);
   }

   for (sfnIndex = 0; sfnIndex < WR_MAX_SFN; sfnIndex++)  
   {
      WR_ALLOC(&poArray, (MAX_POSSIBLE_POS * sizeof(U8)));
      if(NULLP == poArray)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         
         /* Freeing the above allocated memory */
         for (pbmInfoIndex=0; (pbmInfoIndex < WR_PBM_MAX_SCHED_ENTRYS); pbmInfoIndex++)
              WR_FREE(cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex], sizeof(CmLListCp));
         
         /* Freeing the meomry which allocated in this loop */
         for (sfnIndex=sfnIndex-1; sfnIndex >= 0; sfnIndex--)
              WR_FREE((cellCb->pbmCb.allPfsAndPos[sfnIndex]), (MAX_POSSIBLE_POS * sizeof(U8)));
         
         /* Freeing the memory for which is allocated for allPfsAndPos */ 
         WR_FREE(cellCb->pbmCb.allPfsAndPos, (WR_MAX_SFN * sizeof(U8 *)));
         RETVALUE(RFAILED);
      }
      cellCb->pbmCb.allPfsAndPos[sfnIndex] = poArray;
   }
   /* calculate and update the table "allPfsAndPos" which is used to send
    * RRC:Paging message with SI-change or etws indication for all PFs 
    * and POs */
   wrPbmUpdDfltPagCycNb(cellCb, dfltPagCycFactor, nBFactor);

   /* update SIb1 SI Scheduling info for all configured Sibs */
   for (sibIndex = 0; sibIndex < numSibs; sibIndex++)
   {
      if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
               WR_SIB_ACTION_ADD,
               sibsCfg[sibIndex].sibId,
               &ignoreSiIndex))
      {
         RLOG0(L_ERROR,"Updating SI-SCHD-INFO-LST is Failed");
         
         /* Freeing the above allocated memory */
         for(pbmInfoIndex=0; (pbmInfoIndex < WR_PBM_MAX_SCHED_ENTRYS); pbmInfoIndex++)
             WR_FREE(cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex], sizeof(CmLListCp));
         
         /* Freeing the Memory which is allocated for all POs in one PF */ 
         for (sfnIndex = 0; sfnIndex < WR_MAX_SFN; sfnIndex++)  
              WR_FREE(cellCb->pbmCb.allPfsAndPos, (WR_MAX_SFN * sizeof(U8 *)));

         /* Freeing the memory for which is allocated for all PFs */ 
         WR_FREE(cellCb->pbmCb.allPfsAndPos, (WR_MAX_SFN * sizeof(U8 *)));
         RETVALUE(RFAILED);
      }
   } /* end of for to initialize the si scheduling info in SIB1 */
   RETVALUE(ROK);
} /* end of wrEmmPbmCellCfg */

/** @brief This function will cleanup the PBM related information during Cell
 *         deletion.
 *
 * @details
 *
 *     Function: wrEmmPbmCellDel
 *
 *         Processing steps:
 *         - This function is invoked by EMM module during Cell Deletion. 
 *         - This function will delete the any pending Paging message and
 *           cleanup the PBM Scheduling Information table. 
 *         - Cleanup the Memory allocated for all PFs and POs table for the
 *           given Nb and T. 
 *         - This function will be invoked by following modules.
 *            -# EMM during cell deleting. 
 *
 * @param [in] cellCb : cell control block
 * @return S16 
 *       -# Success : ROK 
 */
PUBLIC S16 wrEmmPbmCellDel
(
 WrCellCb *cellCb
 )
{
   CmLListCp       *pbmInfoLstCp = NULLP;
   CmLList         *pbmInfoLnk   = NULLP; 
   U8              *poArray      = NULLP;
   U16             pbmInfoIndex, sfnIndex = 0;

   /* Free memory for all elements pbmSchedInfo table */ 
   for (pbmInfoIndex = 0;
        pbmInfoIndex < WR_PBM_MAX_SCHED_ENTRYS; 
        pbmInfoIndex++)
   {
      pbmInfoLstCp = cellCb->pbmCb.pbmSchedInfo[pbmInfoIndex];

      if (0 != pbmInfoLstCp->count)
      {
         /* Free all elements in the pending PBM Info list */
         CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);
         while (NULL != pbmInfoLnk)
         {
            /* Remove the entry from the pend PBM info Lst */
            cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);

            /* Free the memory for that pending PBM Info entry */
            WR_FREE(pbmInfoLnk->node, sizeof(WrPbmInfo));

            CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);
         } /* end of while */
      }
      /* Free the PBM ListCp memory */
      WR_FREE(pbmInfoLstCp, sizeof(CmLListCp));
   } /* end of for all elements pbmSchedInfo table */

   /* Free the memory for all paging frames */
   for (sfnIndex = 0; sfnIndex < WR_MAX_SFN; sfnIndex++)  
   {
      poArray = cellCb->pbmCb.allPfsAndPos[sfnIndex];

      WR_FREE(poArray, (MAX_POSSIBLE_POS * sizeof(U8)));
   } /*end of for to traverse all paging frames */
   
   /* Free memory for allPfsAndPos */ 
   WR_FREE(cellCb->pbmCb.allPfsAndPos, (WR_MAX_SFN * sizeof(U8 *)));

   RETVALUE(ROK);
} /* end of wrEmmPbmCellDel */

/*Fix for ccpu00139096*/
/** @brief This function will cleanup the ETWS PBM related information during
 *  Concurrent ETWS
 *
 * @details
 *
 *     Function: wrPbmPwsStopPagingEtws 
 *
 *         Processing steps:
 *         - This function is invoked by PWS module during Deletion paging entry. 
 *         - This function will delete the any pending Paging message and
 *           cleanup the PBM Scheduling Information table. 
 *         - Cleanup the Memory allocated for given pf
 *         - This function will be invoked by following modules.
 *            -# PWS during concurrent ETWS. 
 *
 * @param [in] cellCb    : cell control block
 * @param [in] pagInfo   : paging Info
 * @return Void 
 */

PUBLIC Void wrPbmPwsStopPagingEtws 
(        
WrCellCb              *cellCb,
WrEtwsPagInfo         *pagInfo
)
{
  CmLListCp       *pbmInfoLstCp = NULLP;
  CmLList         *pbmInfoLnk   = NULLP; 
  CmLList         *temp         = NULLP; 
  WrPbmInfo       *pbmInfo      = NULLP;

  pbmInfoLstCp = wrPbmGetPendPagMsgLst(pagInfo->pf, cellCb);
  CM_LLIST_FIRST_NODE(pbmInfoLstCp, pbmInfoLnk);
  temp = pbmInfoLnk;

  while (NULL != pbmInfoLnk) 
  {
     temp = temp->next;
     pbmInfo = (WrPbmInfo *)(pbmInfoLnk->node);
     RLOG1(L_DEBUG,"Removing PBM Message pbmType [%d]",pbmInfo->pbmInfoType);

     /* Update the PBM Control Block's paging Events based on 
      * the value of pbmInfoType in  pbmInfo.
      */
     wrPbmUpdPbmCbPagEvents(&cellCb->pbmCb, pbmInfo);
     switch (pbmInfo->pbmInfoType)
     {
         case WR_PBM_TYPE_START_PAG_ETWS:
         case WR_PBM_TYPE_STOP_PAG_ETWS:
         case WR_PBM_TYPE_PRI_ETWS:
         case WR_PBM_TYPE_SEC_ETWS:
         {
           /* Remove the entry from the pend PBM info Lst */
           cmLListDelFrm(pbmInfoLstCp, pbmInfoLnk);
           /* Free the memory for that pending PBM Info entry */
           WR_FREE(pbmInfo, sizeof(WrPbmInfo));
           break;
         }
         default:
           break;
     }
     pbmInfoLnk = temp;
  }

  RETVOID;
}/*end of wrPwsStopPagingEtws*/

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
