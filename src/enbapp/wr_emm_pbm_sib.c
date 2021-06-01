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
               

     File:     wr_emm_pbm_sib.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=93;

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_emm.h"        /* Environment options */
#include "nhu.x" 
#include "nhu_asn.x" 
#include "wr_emm_pbm.h"
#include "wr_emm_pws.h"
#include "wr_utils.h"
#include "wr_ifm_schd.h"
#define WR_EMM_RRC_CELL_ID_LEN         28
#define WR_EMM_RRC_MIB_SPARE_LEN       10
#define WR_EMM_RRC_SIB1_SUBFRM_NO       5 
#define WR_MAX_NUM_SIB1_PLMNS           6

/* PH04_CMAS */
PRIVATE S16 wrSibBldSIB11IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib11Sdu,
 NhuSysInfoBlockTyp11         *sib11Ie
);

PRIVATE S16 wrSibBldSIB10IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib10Sdu,
 NhuSysInfoBlockTyp10          *sib10Ie
);

PRIVATE S16 wrSibBldSIB12IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib12Sdu,
 NhuSysInfoBlockTyp12_r9         *sib12Ie
);

/* PH04_CMAS : END */

PUBLIC S16 wrSibBldCellAccInfo
(
 WrCellCb                                  *cellCb,
 NhuDatReqSdus                             *nhDatReqEvntSdu,
 NhuSysInfoBlockTyp1cellAccessRelatedInfo  *cellAcc
 );

EXTERN S16 wrSibBldSIB8IEs
(
  WrCellCb      *cellCb,
  NhuDatReqSdus *sib8Sdu,
  NhuSysInfoBlockTyp8  *Sib8Ie
);

PUBLIC S16 wrEmmPbmCellSchdReCfg ( U16 cellId);

   Buffer                    *sib10Pdu = NULLP;



/* 
 * Transport Block size table for BCCH and DLSCH  messages as per spec 36.213.
 * This table is in sorted order and duplicate entries has been removed.
 */
 PUBLIC U32 WrTbSzTbl[WR_TBSZTBL_SIZE] = 
{
   32, 56, 72, 88, 104, 120, 144, 176, 208, 224, 256, 296, 328, 376, 392, 440, 
   456, 488, 504, 552, 584, 600, 632, 680, 696, 744, 776, 840, 904, 968, 1000, 
   1064, 1128, 1160, 1192, 1256, 1288, 1384, 1480, 1608, 1736, 1800, 1864, 2216
};
PUBLIC S16 wrEmmPbmCellSchdReCfg(U16   cellId);
EXTERN S16 nhEDMEncReq ARGS((Void     *pdu,
                             U8       msgCategory,
                             Buffer   **mBuf,
                             U32      *length));


/** @brief This function fills MIB BCCH_BCH message (DL BCCH BCH). 
 *
 * @details
 *
 *     Function: wrSibBldMIBMsg
 *
 *         Processing steps:
 *         - This function build MIB message based on MIB parameters in Cell
 *           control block.
 *
 *
 * @param [in]           cellCb : cell control block.
 * @param [out] nhDatReqEvntSdu : MIB msg in RRC data request event SDU.
 * @param [in]         schedSFN : scheduled SFN.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PUBLIC S16 wrSibBldMIBMsg
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *nhDatReqEvntSdu,
U16                          schedSFN
)
{
   NhuBCCH_BCH_Msg           *bcchBch;
   NhuBCCH_BCH_MsgTyp        *bcchBchMsg = NULLP;
   NhuTimingInfo             *tmngInf = NULLP;
   WrMibParams               *mib = &cellCb->mib;

   TRC2(wrSibBldMIBMsg);

   bcchBch    = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchBchMsg);
   WR_FILL_TKN_UINT(bcchBch->pres, PRSNT_NODEF);
   bcchBchMsg = &(bcchBch->message);

   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = 0;

   WR_FILL_TKN_UINT(bcchBchMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(bcchBchMsg->dl_Bandwidth, mib->dlBw);
   WR_FILL_TKN_UINT(bcchBchMsg->phich_Config.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(bcchBchMsg->phich_Config.phich_Duration, mib->phichDur);
   WR_FILL_TKN_UINT(bcchBchMsg->phich_Config.phich_Resource, mib->phichGrps);

   /* systemFrameNumber */
   WR_GET_MEM(nhDatReqEvntSdu, sizeof(U8),
              &(bcchBchMsg->systemFrameNumber.val));
   bcchBchMsg->systemFrameNumber.pres = PRSNT_NODEF;
   bcchBchMsg->systemFrameNumber.len  = WR_BYTE_LEN;

   /* Assign the 8 most significant bits of SFN */
   WR_GET_SFN_MIB(bcchBchMsg->systemFrameNumber.val[0], schedSFN);

   /* Spare, since it is not optional */
   WR_GET_MEM(nhDatReqEvntSdu, (2 * sizeof(U8)), &(bcchBchMsg->spare.val));
   bcchBchMsg->spare.pres = PRSNT_NODEF;
   bcchBchMsg->spare.len  = WR_EMM_RRC_MIB_SPARE_LEN;

   RETVALUE(ROK);
} /* End of wrSibBldMIBMsg */

/** @brief This function fills cell access related information present in SIB1.
 *
 * @details
 *
 *     Function: wrSibBldCellAccInfo
 *
 *         Processing steps:
 *         - Fill the cell access related into cell access info IE in SIB1 IE
 *           based on SIB1 params in cell control block.
 *
 * @param [in]               cellCb : cell control block. 
 * @param [in, out] nhDatReqEvntSdu : RRC Data Req Event SDU.
 * @param [out]             cellAcc : cell Access related parameters.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PUBLIC S16 wrSibBldCellAccInfo
(
WrCellCb                                  *cellCb,
NhuDatReqSdus                             *nhDatReqEvntSdu,
NhuSysInfoBlockTyp1cellAccessRelatedInfo  *cellAcc
)
{
   WrSib1Params              *sib1 = &cellCb->sib1;
   U32                       cellIdShifted = (sib1->cellIdentity << 4);
   U32                       csgIdShifted = (sib1->csgId.val << 5);
   NhuPLMN_IdentityLst       *plmnLst = &(cellAcc->plmn_IdentityLst);
   U32                       idx = 0;
   U32                       plmnIdx = 0;
   U8                        digit = 0;
   U32                       numPlmnsActivated = 0;

   WR_FILL_TKN_UINT(cellAcc->pres, PRSNT_NODEF);

   for(idx = 0; idx < sib1->numPlmnIds; idx++)
   {
      if (sib1->plmns[idx].activated == TRUE)
      {
         numPlmnsActivated++;
      }
   }
   
   if (numPlmnsActivated > WR_MAX_NUM_SIB1_PLMNS)
   {
      numPlmnsActivated = WR_MAX_NUM_SIB1_PLMNS;
   } 
   /* Fill up all the broadcast PLMN IDs and the operator restriction     */
   WR_FILL_TKN_UINT(plmnLst->noComp, numPlmnsActivated);

   WR_GET_MEM(nhDatReqEvntSdu, 
              ((plmnLst->noComp.val) * (sizeof(NhuPLMN_IdentityInfo))), 
              &(plmnLst->member));
   for(idx = 0; ((idx < sib1->numPlmnIds) && (plmnIdx < numPlmnsActivated)); idx++)
   {
      NhuPLMN_Identity       *plmnIe = &(plmnLst->member[plmnIdx].plmn_Identity);
      WrPlmnId               *plmnId = &(sib1->plmns[idx].plmnId);

      if (sib1->plmns[idx].activated == FALSE)
      {
         continue;
      }
      WR_FILL_TKN_UINT(plmnLst->member[plmnIdx].pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(plmnIe->pres, PRSNT_NODEF);

      /* Fill in MCC digits for each PLMN ID                              */
      WR_FILL_TKN_UINT(plmnIe->mcc.noComp, WR_MCC_SIZE);
      WR_GET_MEM(nhDatReqEvntSdu,(WR_MCC_SIZE * (sizeof(NhuMCC_MNC_Digit))),
            &(plmnIe->mcc.member));
      for(digit = 0; digit < WR_MCC_SIZE; digit++)
      {
         WR_FILL_TKN_UINT(plmnIe->mcc.member[digit], plmnId->mcc[digit]);
      }

      /* Fill in MNC digits for each PLMN ID                              */
      WR_FILL_TKN_UINT(plmnIe->mnc.noComp, plmnId->numMncDigits);
      WR_GET_MEM(nhDatReqEvntSdu, 
                 (plmnId->numMncDigits * (sizeof(NhuMCC_MNC_Digit))),
                 &(plmnIe->mnc.member));
      for(digit = 0; digit < plmnId->numMncDigits; digit++)
      {
         WR_FILL_TKN_UINT(plmnIe->mnc.member[digit], plmnId->mnc[digit]);
      }

      /* Fill in if the cell is reserved for operator use   */ 
      WR_FILL_TKN_UINT(plmnLst->member[plmnIdx].cellReservedForOperatorUse,
                       sib1->plmns[idx].resvForOperUse);
      plmnIdx++;
   }

   /* Fill in the tracking area code                                      */
   cellAcc->trackingAreaCode.pres = PRSNT_NODEF;
   cellAcc->trackingAreaCode.len = WR_TRAC_LEN;
   WR_GET_MEM(nhDatReqEvntSdu, sizeof(U16), &(cellAcc->trackingAreaCode.val));
   cellAcc->trackingAreaCode.val[0] = (sib1->tac & 0xff00) >> 8;
   cellAcc->trackingAreaCode.val[1] = (sib1->tac & 0x00ff);

   /* Fill in CellIdentity  Bitstring of 28 bits len                      */
   cellAcc->cellIdentity.pres = PRSNT_NODEF;
   cellAcc->cellIdentity.len = WR_EMM_RRC_CELL_ID_LEN;
   WR_GET_MEM(nhDatReqEvntSdu, sizeof(U32), &(cellAcc->cellIdentity.val));
   cellAcc->cellIdentity.val[0] = (cellIdShifted & 0xff000000) >> 24;
   cellAcc->cellIdentity.val[1] = (cellIdShifted & 0x00ff0000) >> 16;
   cellAcc->cellIdentity.val[2] = (cellIdShifted & 0x0000ff00) >> 8;
   cellAcc->cellIdentity.val[3] = (cellIdShifted & 0x000000ff) >> 0;

   /* cellBarred */
   WR_FILL_TKN_UINT(cellAcc->cellBarred, sib1->cellBarred);

   /*  intraFreqReselection */
   WR_FILL_TKN_UINT(cellAcc->intraFreqReselection, sib1->intraFreqReselection);

   /* csg-Indication */
   WR_FILL_TKN_UINT(cellAcc->csg_Ind, sib1->csgInd);

   /* csg-Identity left empty*/
   /* if (cellAcc->csg_Ind.pres)*/
   if ( cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS ||
       cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS )
   {
      cellAcc->csg_Identity.pres = PRSNT_NODEF;
      cellAcc->csg_Identity.len  = WR_S1AP_CSG_ID_LEN;
      WR_GET_MEM(nhDatReqEvntSdu, sizeof(U32), &(cellAcc->csg_Identity.val));
      cellAcc->csg_Identity.val[0] = (csgIdShifted & 0xff000000) >> 24;
      cellAcc->csg_Identity.val[1] = (csgIdShifted & 0x00ff0000) >> 16;
      cellAcc->csg_Identity.val[2] = (csgIdShifted & 0x0000ff00) >> 8;
      cellAcc->csg_Identity.val[3] = (csgIdShifted & 0x000000ff) >> 0;
   }
   else
   {
      cellAcc->csg_Identity.pres = NOTPRSNT;
   }

   RETVALUE(ROK);
} /* end of wrSibBldCellAccInfo */

/** @brief This function fills System Info Block Type1 message (DL BCCH DLSCH).
 *
 * @details
 *
 *     Function: wrSibBldSIB1Msg
 *
 *         Processing steps:
 *         - This function is build the SIB1 IEs based on updated information in
 *           cell control block.
 *
 * @param [in]   cellCb : cell control block. 
 * @param [out] sib1Sdu : SIB1 SDU.
 * @param [in] schedSFN : schedfuled SFN number.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PUBLIC S16 wrSibBldSIB1Msg
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *sib1Sdu,
U16                          schedSFN
)
{
   NhuBCCH_DL_SCH_Msg        *bcchSchMsg = NULLP;
   NhuSysInfoBlockTyp1       *sibTyp1    = NULLP;
   NhuTimingInfo             *tmngInf    = NULLP;
   NhuSchedulingInfo         *schedInf   = NULLP;
   WrSib1Params              *sib1 = &cellCb->sib1;
   U8                        idx = 0;
   U8                        mapCnt = 0;
   U8                        sibCnt = 0;
   U8                        sib1SiCnt = 0;

   TRC2(wrSibBldSIB1Msg);

   bcchSchMsg = &(sib1Sdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);

   /* Filling Timing Info */
   tmngInf = &(sib1Sdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = WR_EMM_RRC_SIB1_SUBFRM_NO;

   WR_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   WR_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFOBLOCKTYP1);

   sibTyp1 = &(bcchSchMsg->message.val.c1.val.systemInfoBlockTyp1);
   WR_FILL_TKN_UINT(sibTyp1->pres, PRSNT_NODEF);

   /* cellAccessRelatedInfo */
   wrSibBldCellAccInfo(cellCb, sib1Sdu, 
                       &sibTyp1->cellAccessRelatedInfo);

   /* Q-RxLevMin */ 
   WR_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMin, sib1->qRxLevMin);
   if (sib1->qRxLevMinOffsetPres)
   {
      WR_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMinOffset,
                       sib1->qRxLevMinOffset);
   }
   else
   {
      sibTyp1->cellSelectionInfo.q_RxLevMinOffset.pres = NOTPRSNT;
   }

   /* Include P-MAX IE into SIB1 */ 
   WR_FILL_TKN_UINT(sibTyp1->p_Max, sib1->pMax);

   /* Include Frequency Band Indicator IE                                 */
   WR_FILL_TKN_UINT(sibTyp1->freqBandIndicator, sib1->freqBandIndicator);

   if((cellCb->eutranNbrFreqLst.count > 1))
   {
      WR_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, sib1->numSis);
   }
   else
   {
      /*SIB5 periodicity will not be added to SIB1 if no inter frequency configured*/
      WR_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, (sib1->numSis-1));
   }
   WR_GET_MEM(sib1Sdu, 
        (sizeof(NhuSchedulingInfo) * sibTyp1->schedulingInfoLst.noComp.val),
         &(sibTyp1->schedulingInfoLst.member));

   /* Filling Scheduling Periodicity info for SIB2 */
   /* Zero means only SIB2 by default */
   schedInf = &(sibTyp1->schedulingInfoLst.member[0]);
   WR_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(schedInf->si_Periodicity, sib1->siSchdInfo[0].period);

   if(sib1->siSchdInfo[idx].numSibs > 1)
   {
      WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, \
            (sib1->siSchdInfo[idx].numSibs -1));
      
      WR_GET_MEM(sib1Sdu, 
            (sizeof(NhuSIB_Typ) * schedInf->sib_MappingInfo.noComp.val),
            &(schedInf->sib_MappingInfo.member));
      for(mapCnt = 0,sibCnt = 0; sibCnt < (schedInf->sib_MappingInfo.noComp.val); \
            mapCnt++)
      {
         if(sib1->siSchdInfo[idx].sibs[mapCnt] != WR_SIB_2)
         {
            
            WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.member[sibCnt], \
               sib1->siSchdInfo[idx].sibs[mapCnt]);
            sibCnt++;
         }
      }
   }
   else
   {
      WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, 0);
   }
   sib1SiCnt = 1;
   /* Filling the Mapping info and for other SIBs */
   for(idx = 1; idx < (sib1->numSis); idx++)
   {
      /*SIB5 Scheduling info will be skipped if SIB5 is disabled*/
      if((sib1->siSchdInfo[idx].sibs[0] == WR_SIB_5)&& 
                  (cellCb->eutranNbrFreqLst.count  == 1))
      {
         RLOG0(L_INFO,"SIB5 info is skipped in SIB1");
         continue;
      }
      /*
       sib1SiCnt : this counter is used to fill the SIBs sheduling infomation in SIB1 
                   RRC message
       idx : this counter is used to get the SIBs shceduling information from 
                   cell CB
       These two counter are used to maintain the index when SIB5 is
        is not enabled
      */
      schedInf = &(sibTyp1->schedulingInfoLst.member[sib1SiCnt]);
      WR_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(schedInf->si_Periodicity, sib1->siSchdInfo[idx].period);
      WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, \
            sib1->siSchdInfo[idx].numSibs);
      WR_GET_MEM(sib1Sdu, 
            (sizeof(NhuSIB_Typ) * schedInf->sib_MappingInfo.noComp.val),
            &(schedInf->sib_MappingInfo.member));
      for(mapCnt = 0; mapCnt < (schedInf->sib_MappingInfo.noComp.val); \
            mapCnt++)
      {
         WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.member[mapCnt], \
               sib1->siSchdInfo[idx].sibs[mapCnt]);
      }
      sib1SiCnt++;
   }
/* ACC-TDD */ 
#ifdef LTE_TDD
   WR_FILL_TKN_UINT(sibTyp1->tdd_Config.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(sibTyp1->tdd_Config.subframeAssignment, sib1->tddCfg.subframeCfg);
   WR_FILL_TKN_UINT(sibTyp1->tdd_Config.specialSubfrmPatterns, sib1->tddCfg.specialSubframeCfg);

#else
   sibTyp1->tdd_Config.pres.pres = NOTPRSNT;
#endif

   /* Add SI Window size to the PDU                                       */
   WR_FILL_TKN_UINT(sibTyp1->si_WindowLength, sib1->siWindowLen);

   /* systemInfoValueTag */
   WR_FILL_TKN_UINT(sibTyp1->systemInfoValueTag, sib1->valueTag);

   /* nonCriticalExtension */
   sibTyp1->nonCriticalExtn.pres.pres = NOTPRSNT;

   RETVALUE(ROK);
} /* End of wrSibBldSIB1Msg */

/** @brief This function fills System Info message (DL BCCH DLSCH) header.
 *
 * @details
 *
 *     Function: wrSibBldSIHeader
 *
 *         Processing steps:
 *         - This function fills the SI header in RRC Data Req SDU.
 *
 * @param [in]   cellCb : cell control block.
 * @param [out]  sibSdu : SIB SDU to fill SI header.
 * @param [in] schedSFN : scheduled SFN.
 * @param [in] subframe : Sub frame number.
 * @param [in]  numSibs : Number of SIBs.
 * @return  S16
 *      -# Success : ROK
 */
PUBLIC S16 wrSibBldSIHeader
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *sibSdu,
U16                          schedSFN,
U8                           subframe,
U8                           numSibs
)
{
   NhuBCCH_DL_SCH_Msg             *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns        *critExt    = NULLP;
   NhuTimingInfo                  *tmngInf    = NULLP;

   bcchSchMsg = &(sibSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);

   /* Filling Timing Info */
   tmngInf = &(sibSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = subframe;

   WR_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   WR_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
   WR_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres, 
                    PRSNT_NODEF);

   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
   WR_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
   WR_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres, 
                    PRSNT_NODEF);

   /* Each SIB forms its own SI. There will be just one SIB per SI        */
   WR_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp, numSibs);
   WR_GET_MEM(sibSdu, 
              (sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)) * numSibs,
              &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

   RETVALUE(ROK);
} /* End of wrSibBldSIHeader */

/** @brief This function fills SIB2 IEs for a member in system information
 *
 * @details
 *
 *     Function: wrSibBldSIB2IEs
 *
 *         Processing steps:
 *         - This function is build the SIB2 IEs based on updated information in
 *           cell control block.
 *
 * @param [in]       cellCb : Cell control block
 * @param [in, out] sib2Sdu : RRC Data Request for SIB2 SDU.
 * @param [out]      sib2Ie : SIB2 IE.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrSibBldSIB2IEs
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *sib2Sdu,
NhuSysInfoBlockTyp2          *sib2Ie
)
{
   WrSib2Params                   *sib2       = &cellCb->sib2;
   NhuRACH_ConfigCmn              *rachCfg    = NULLP;
   NhuRACH_ConfigCmnpreambleInfo  *preAmbInf  = NULLP;
   NhuBCCH_Config                 *bcchCfg    = NULLP;
   NhuPCCH_Config                 *pcchCfg    = NULLP;
   NhuPRACH_ConfigSIB             *prachCfg   = NULLP;
   NhuPDSCH_ConfigCmn             *pdschCfg   = NULLP;
   NhuPUSCH_ConfigCmn             *puschCfg   = NULLP;
   NhuPUCCH_ConfigCmn             *pucchCfg   = NULLP;
   NhuSoundingRS_UL_ConfigCmn     *srsCfg     = NULLP;
   NhuUlnkPowerCntrlCmn           *ulPwrCntrl = NULLP;

   TRC2(wrSibBldSIB2IEs);

   WR_FILL_TKN_UINT(sib2Ie->pres, PRSNT_NODEF);

   /* Fill Access-barring information */
   if(sib2->barringInfoPres)
   {
      WR_FILL_TKN_UINT(sib2Ie->ac_BarringInfo.pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib2Ie->ac_BarringInfo.ac_BarringForEmergency,
            sib2->barringInfo.emergency);
      if (sib2->barringInfo.signalling.pres)
      {
         WR_FILL_TKN_UINT(sib2Ie->ac_BarringInfo.ac_BarringForMO_Signalling.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT((sib2Ie->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringFactor),
               sib2->barringInfo.signalling.acBarringFactor);
         WR_FILL_TKN_UINT((sib2Ie->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringTime),
               sib2->barringInfo.signalling.acBarringTime);
         wrFillTknStrBSXL(&(sib2Ie->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringForSpecialAC),
               5, &(sib2->barringInfo.signalling.acBarringForSpecialAC), &(sib2Sdu->memCp));
      }
      if (sib2->barringInfo.dataCall.pres)
      {
         WR_FILL_TKN_UINT(sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT((sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringFactor),
               sib2->barringInfo.dataCall.acBarringFactor);
         WR_FILL_TKN_UINT((sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringTime),
               sib2->barringInfo.dataCall.acBarringTime);
         wrFillTknStrBSXL(&(sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringForSpecialAC),
               5, &(sib2->barringInfo.dataCall.acBarringForSpecialAC), &(sib2Sdu->memCp));
      }
   }

   /*-Fill Radio Resource Config Common -*/
   WR_FILL_TKN_UINT(sib2Ie->radioResourceConfigCmn.pres, PRSNT_NODEF);

   /* RACH cfg */
   rachCfg   = &(sib2Ie->radioResourceConfigCmn.rach_ConfigCmn);
   preAmbInf = &(rachCfg->preambleInfo);
   WR_FILL_TKN_UINT(rachCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(preAmbInf->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(preAmbInf->numberOfRA_Preambles, 
                    sib2->rachCfg.noRaPreambles);
   WR_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.sizeOfRA_PreamblesGroupA,
                    sib2->rachCfg.noGrpAPreambles);
   WR_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messageSizeGroupA,
                    sib2->rachCfg.grpAMsgSize);
   WR_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messagePowerOffsetGroupB,
                    sib2->rachCfg.grpBPwrOffset);
   WR_FILL_TKN_UINT(rachCfg->powerRampingParams.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(rachCfg->powerRampingParams.powerRampingStep,
                    sib2->rachCfg.pwrRampStep);
   WR_FILL_TKN_UINT(rachCfg->powerRampingParams.preambleInitReceivedTgtPower,
                    sib2->rachCfg.prmbInitRcvdTrgtPwr);
   WR_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.preambleTransMax,
                    sib2->rachCfg.maxPrmbTrans);
   WR_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.ra_RspWindowSize,
                    sib2->rachCfg.raWindowSize);
   WR_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.mac_ContentionResolutionTimer,
                    sib2->rachCfg.contResTimer);
   WR_FILL_TKN_UINT(rachCfg->maxHARQ_Msg3Tx, sib2->rachCfg.maxMsg3HarqTrans);   

   /* BCCH CFG */
   bcchCfg = &(sib2Ie->radioResourceConfigCmn.bcch_Config);
   WR_FILL_TKN_UINT(bcchCfg->pres, PRSNT_NODEF);
   if (cellCb->pbmCb.pendReCalcAllPfsAndPos)
   {
      WR_FILL_TKN_UINT(bcchCfg->modificationPeriodCoeff, 
                       cellCb->pbmCb.pendModifPeriodCoeff);
   } 
   else 
   {
      WR_FILL_TKN_UINT(bcchCfg->modificationPeriodCoeff, 
                       sib2->bcchCfg.modifPeriodCoeff);
   }

   /* PCCH CFG */
   pcchCfg = &(sib2Ie->radioResourceConfigCmn.pcch_Config);
   WR_FILL_TKN_UINT(pcchCfg->pres, PRSNT_NODEF);
   if (cellCb->pbmCb.pendReCalcAllPfsAndPos)
   {
      WR_FILL_TKN_UINT(pcchCfg->defaultPagCycle, cellCb->pbmCb.pendDfltPagingCycle);
      WR_FILL_TKN_UINT(pcchCfg->nB, cellCb->pbmCb.pendnB);
   } 
   else 
   {
      WR_FILL_TKN_UINT(pcchCfg->defaultPagCycle, sib2->pcchCfg.defaultPagingCycle);
      WR_FILL_TKN_UINT(pcchCfg->nB, sib2->pcchCfg.nB);
   }
      
   /* PRACH Cfg */
   prachCfg = &(sib2Ie->radioResourceConfigCmn.prach_Config);
   WR_FILL_TKN_UINT(prachCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(prachCfg->rootSequenceIdx, sib2->prachCfg.rootSeqIdx);
   WR_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_ConfigIdx, 
                    sib2->prachCfg.prachCfgIdx);
   WR_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.highSpeedFlag,
                    sib2->prachCfg.highSpeedFlag);
   WR_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.zeroCorrelationZoneConfig,
                    sib2->prachCfg.zeroCorrZone);
   WR_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_FreqOffset,
                    sib2->prachCfg.prachFreqOffset);

    /* PDSCH Cfg */
   pdschCfg = &(sib2Ie->radioResourceConfigCmn.pdsch_ConfigCmn);
   WR_FILL_TKN_UINT(pdschCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(pdschCfg->referenceSignalPower, sib2->pdschCfg.refSigPower);
   WR_FILL_TKN_UINT(pdschCfg->p_b, sib2->pdschCfg.pB);

   /* PUSCH Cfg*/
   puschCfg = &(sib2Ie->radioResourceConfigCmn.pusch_ConfigCmn);
   WR_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.n_SB, sib2->puschCfg.nSB);
   WR_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.hoppingMode,
                    sib2->puschCfg.hoppingMode);
   WR_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pusch_HoppingOffset,
                    sib2->puschCfg.hoppingOffset);
   WR_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.enable64QAM,
                    sib2->puschCfg.qam64Enabled);
   WR_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupHoppingEnabled,
                    sib2->puschCfg.grpHoppingEnabled);
   WR_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH,
                    sib2->puschCfg.grpAssignPusch);
   WR_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled,
                    sib2->puschCfg.seqHoppingEnabled);
   WR_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.cyclicShift,
                    sib2->puschCfg.cyclicShift);

   /* PUCCH Cfg */
   pucchCfg = &(sib2Ie->radioResourceConfigCmn.pucch_ConfigCmn);
   WR_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(pucchCfg->deltaPUCCH_Shift, sib2->pucchCfg.deltaPucchShift);
   WR_FILL_TKN_UINT(pucchCfg->nRB_CQI, sib2->pucchCfg.nRBCqi);
   WR_FILL_TKN_UINT(pucchCfg->nCS_AN, sib2->pucchCfg.nCSAN);
   WR_FILL_TKN_UINT(pucchCfg->n1PUCCH_AN, sib2->pucchCfg.n1PucchAn);

   /* SoundingsRS UL Cfg */
   srsCfg = &(sib2Ie->radioResourceConfigCmn.soundingRS_UL_ConfigCmn);
   if (sib2->srsCfg.release == 1)
   {
      WR_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_RELEASE);
   }
   else
   {
      WR_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_SETUP);
      WR_FILL_TKN_UINT(srsCfg->val.setup.pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(srsCfg->val.setup.srs_BandwidthConfig,
                       sib2->srsCfg.srsBandwidthCfg);
      WR_FILL_TKN_UINT(srsCfg->val.setup.srs_SubfrmConfig,
                       sib2->srsCfg.srsSubfrmCfg);
      WR_FILL_TKN_UINT(srsCfg->val.setup.ackNackSRS_SimultaneousTransmission,
                       sib2->srsCfg.ackNackSrsSimulTx);
      srsCfg->val.setup.srs_MaxUpPts.pres = NOTPRSNT;
   }

   /* Uplink Power Control Config                                         */
   ulPwrCntrl = &(sib2Ie->radioResourceConfigCmn.uplinkPowerCntrlCmn);
   WR_FILL_TKN_UINT(ulPwrCntrl->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUSCH, 
                    sib2->ulPwrCtrlCfg.p0NominalPusch);
   WR_FILL_TKN_UINT(ulPwrCntrl->alpha, sib2->ulPwrCtrlCfg.alpha);
   WR_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUCCH, 
                    sib2->ulPwrCtrlCfg.p0NominlPucch);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1,
                    sib2->ulPwrCtrlCfg.deltaFPucchFormat1);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1b,
                    sib2->ulPwrCtrlCfg.deltaFPucchFormat1b);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2,
                    sib2->ulPwrCtrlCfg.deltaFPucchFormat2);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2a,
                    sib2->ulPwrCtrlCfg.deltaFPucchFormat2a);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2b,
                    sib2->ulPwrCtrlCfg.deltaFPucchFormat2b);
   WR_FILL_TKN_UINT(ulPwrCntrl->deltaPreambleMsg3, 
                    sib2->ulPwrCtrlCfg.deltaPeambleMsg3);

   /* Fill UL cyclic prefix                                               */
   WR_FILL_TKN_UINT(sib2Ie->radioResourceConfigCmn.ul_CyclicPrefixLength,
                    sib2->ulCyclicPrefix);

   /*-Fill ue Timers And Constants                                        */
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.t300, sib2->t300);
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.t301, sib2->t301);
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.t310, sib2->t310);
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.n310, sib2->n310);
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.t311, sib2->t311);
   WR_FILL_TKN_UINT(sib2Ie->ue_TmrsAndConsts.n311, sib2->n311);

   /*-Fill Freuency Infomration -*/
   WR_FILL_TKN_UINT(sib2Ie->freqInfo.pres, PRSNT_NODEF);
   /*ccpu00133012:The WR_NO_DFLT_UL_CARRIER_FREQ is taken as 0
     in order to support the default configuration.In that case 
     ulCarrierFrequency will not be fill in and taken as an offset
     from Dl Carrier Frequency as defined in 36.101
   */
   /* ccpu00134584 eNB broadcasting parameter "freqInfo-> ul-Bandwidth" for TDD mode.*/
#ifndef LTE_TDD
   if ( sib2->ulCarrierFreq != WR_NO_DFLT_UL_CARRIER_FREQ )
   {
       WR_FILL_TKN_UINT(sib2Ie->freqInfo.ul_CarrierFreq,sib2->ulCarrierFreq);
   }

   WR_FILL_TKN_UINT(sib2Ie->freqInfo.ul_Bandwidth, sib2->ulBw);
#endif
   WR_FILL_TKN_UINT(sib2Ie->freqInfo.additionalSpectrumEmission,
                    sib2->addlSpectrEmission);

   /* skipping MBSFN subframe config */
   sib2Ie->mbsfn_SubfrmConfigLst.noComp.pres = NOTPRSNT;

   /* TimeAlignmentTimer */
   WR_FILL_TKN_UINT(sib2Ie->timeAlignmentTimerCmn, sib2->timingAlignTimer);

   RETVALUE(ROK);
} /* End of wrSibBldSIB2IEs */

/** @brief This function fills SIB3 IE - Intra Frequency Cell Reselection 
 *
 * @details
 *
 *     Function: wrSibBldSib3IntraFreqCellReselIe
 *
 *         Processing steps:
 *         - Fill the RRC Intra freq cell reselection IE from the configured
 *           SIB3 parameters. 
 *
 * @param [in, out]  sib3Sdu : SIB3 SDU.
 * @param [in]       servingFreqInfo : Intra frequency cell reselcation information
 * @param [out]      intraIe : RRC Intra frequcy cel reslection IE.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrSibBldSib3IntraFreqCellReselIe
(
NhuDatReqSdus                                   *sib3Sdu,
WrEutranFreqCb                                  *servFreqInfo,
NhuSysInfoBlockTyp3intraFreqCellReselectionInfo *intraIe
)
{
   WR_FILL_TKN_UINT(intraIe->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(intraIe->q_RxLevMin, servFreqInfo->qRxLevMin);
   if (servFreqInfo->pMaxPres)
   {
      WR_FILL_TKN_UINT(intraIe->p_Max, servFreqInfo->pMax);
   }
   else
   {
      intraIe->p_Max.pres = NOTPRSNT;
   }

   if (servFreqInfo->sIntraSrch.pres)
   {
      WR_FILL_TKN_UINT(intraIe->s_IntraSearch, servFreqInfo->sIntraSrch.val);
   }
   else
   {
      intraIe->s_IntraSearch.pres = NOTPRSNT;
   }

   if (servFreqInfo->allowedMeasBW.pres)
   {
      WR_FILL_TKN_UINT(intraIe->allowedMeasBandwidth, servFreqInfo->allowedMeasBW.val);
   }
   else
   {
      intraIe->allowedMeasBandwidth.pres = NOTPRSNT;
   }

   WR_FILL_TKN_UINT(intraIe->presenceAntennaPort1, servFreqInfo->presenceAntPort1);
   WR_FILL_BIT_STR(intraIe->neighCellConfig, 2, servFreqInfo->neighCellConfig, sib3Sdu); 
   WR_FILL_TKN_UINT(intraIe->t_ReselectionEUTRA, servFreqInfo->tReselcxn);

   /* ccpu00133677: Added the pres filling as without present feild, this
    * IE is not sent out
    */
   WR_FILL_TKN_UINT(intraIe->t_ReselectionEUTRA_SF.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(intraIe->t_ReselectionEUTRA_SF.sf_Medium, 
                    servFreqInfo->sfMedium);
   WR_FILL_TKN_UINT(intraIe->t_ReselectionEUTRA_SF.sf_High, 
                    servFreqInfo->sfHigh);
   RETVALUE(ROK);
} /* end of wrSibBldSib3IntraFreqCellReselIe */

/** @brief This function fills SIB3 IEs for a member in system information
 *
 * @details
 *
 *     Function: wrSibBldSIB3IEs
 *
 *         Processing steps:
 *         - This function is build the SIB3 IEs based on updated information in
 *           cell control block.
 *
 * @param [in]       cellCb : Cell control block
 * @param [in, out] sib3Sdu : RRC Data Request for SIB3 SDU.
 * @param [out]      sib3Ie : SIB3 IE.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrSibBldSIB3IEs
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *sib3Sdu,
NhuSysInfoBlockTyp3          *sib3Ie
)
{
   WrSib3Params                              *sib3          = &(cellCb->sib3);
   WrEutranFreqCb                            *servFreqInfo = NULLP;
   CmLListCp                                 *freqInfoLstCp = NULLP;
   CmLList                                   *freqInfoLnk   = NULLP;
   NhuSysInfoBlockTyp3cellReselectionInfoCmn *cellResel     = NULLP;
   U8                                        dlFreqFlag = FALSE;

   WR_FILL_TKN_UINT(sib3Ie->pres, PRSNT_NODEF);

   freqInfoLstCp = &cellCb->eutranNbrFreqLst;
   /* Traverse the EUTRAN Frequncey list info List  get the Serving frequceny
    * information */ 
   CM_LLIST_FIRST_NODE(freqInfoLstCp, freqInfoLnk);
   while (NULLP != freqInfoLnk)
   {
      servFreqInfo = (WrEutranFreqCb *)(freqInfoLnk->node);

      if (servFreqInfo->dlEarfcn == cellCb->dlCarrierFreq)
      {
         dlFreqFlag = TRUE;
         /*-Fill Cell Reselection Information Common -*/
         cellResel = &(sib3Ie->cellReselectionInfoCmn);
         WR_FILL_TKN_UINT(cellResel->pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(cellResel->q_Hyst, sib3->qHyst);
         cellResel->speedStateReselectionPars.pres.pres = sib3->speedStateReselPres;
         if (sib3->speedStateReselPres)
         {
            NhuMobilityStateParams *msp;
            msp = &(cellResel->speedStateReselectionPars.mobilityStateParams);
            WR_FILL_TKN_UINT(msp->pres, PRSNT_NODEF);
            WR_FILL_TKN_UINT(msp->t_Evaluation, sib3->mobStateParams.tEvaluation);
            WR_FILL_TKN_UINT(msp->t_HystNormal, sib3->mobStateParams.tHyst);
            WR_FILL_TKN_UINT(msp->n_CellChangeMedium, 
                  sib3->mobStateParams.nCellChangeMed);
            WR_FILL_TKN_UINT(msp->n_CellChangeHigh, 
                  sib3->mobStateParams.nCellChangeHigh);
            WR_FILL_TKN_UINT(cellResel->speedStateReselectionPars.q_HystSF.pres, 
                  PRSNT_NODEF);
            WR_FILL_TKN_UINT(cellResel->speedStateReselectionPars.q_HystSF.sf_Medium,
                  sib3->sfMedium);
            WR_FILL_TKN_UINT(cellResel->speedStateReselectionPars.q_HystSF.sf_High, 
                  sib3->sfHigh);
         }

         /*-Fill Cell Reselection Serving Frequency Information -*/
         WR_FILL_TKN_UINT(sib3Ie->cellReselectionServingFreqInfo.pres,
               servFreqInfo->sNonIntraSrch.pres);
         if (servFreqInfo->sNonIntraSrch.pres)
         {
            WR_FILL_TKN_UINT(sib3Ie->cellReselectionServingFreqInfo.s_NonIntraSearch,
                  servFreqInfo->sNonIntraSrch.val);
         }
         WR_FILL_TKN_UINT(sib3Ie->cellReselectionServingFreqInfo.threshServingLow,
                          servFreqInfo->thSrvngLow);
         WR_FILL_TKN_UINT(sib3Ie->cellReselectionServingFreqInfo.cellReselectionPriority,
                          servFreqInfo->cellReSelPri);

         /*-Fill Intra-Frequency Cell Reselection Information -                 */
         wrSibBldSib3IntraFreqCellReselIe(sib3Sdu, servFreqInfo, \
               &sib3Ie->intraFreqCellReselectionInfo);
         break;
      }
      CM_LLIST_NEXT_NODE(freqInfoLstCp, freqInfoLnk);
   } /* end of while */
   if ( FALSE == dlFreqFlag)
   {
      RLOG0(L_ERROR,
              "wrSibBldSIB3IEs: Error No serving frequency available in FreqInfoLst");
     RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrSibBldSIB3IEs */

/** @brief This function fills SIB4 IEs for a member in system information 
 *
 * @details
 *
 *     Function: wrSibBldSIB4IEs
 *
 *         Processing steps:
 *         - Build the SIB4 IEs based on OAM configured information like CSG
 *           information of the current cell and ANR given inforamtion.
 *         - Get the Intra-frequecy Neighebour information from ANR module by
 *           using wrAnrGetWhiteAndBlackList API. 
 *
 * @param [in]       cellCb : Cell control block
 * @param [in, out] sib4Sdu : RRC Data Request for SIB4 SDU.
 * @param [out]      sib4Ie : SIB4 IE.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrSibBldSIB4IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib4Sdu,
 NhuSysInfoBlockTyp4          *sib4Ie
)
{
   WrSib4Params         *sib4 = &(cellCb->sib4);
   WrNrWlBlInfo         *nrWlBlInfo = NULLP;
   WrNrPerFreqWlInfo    *whiteLst = NULLP;
   WrNrPerFreqBlInfo    *blackLst = NULLP;
   U8                   idx       = 0;

   TRC2(wrSibBldSIB4IEs);

   WR_FILL_TKN_UINT(sib4Ie->pres, PRSNT_NODEF); 
   
/* Get Inta Freq neighbor cell list from ANR module */ 
   nrWlBlInfo = wrEmmAnrGetWhiteAndBlackList(cellCb->cellId, WR_ANR_LST_INTRA_FREQ, TRUE);

   if(!nrWlBlInfo)
   {
      RETVALUE(RFAILED);
   }
   whiteLst = &nrWlBlInfo->intra.wlData;
   if(whiteLst && whiteLst->numWlCells) 
   {
      if (whiteLst->numWlCells > 16 )
      {
         whiteLst->numWlCells = 16;
      }
      /*-Fill Intra-Frequency Neighbouring Cell List-*/
      WR_FILL_TKN_UINT(sib4Ie->intraFreqNeighCellLst.noComp, 
            whiteLst->numWlCells);
      WR_GET_MEM(sib4Sdu,
            (whiteLst->numWlCells)*(sizeof(NhuIntraFreqNeighCellInfo)),
            &(sib4Ie->intraFreqNeighCellLst.member));
      for ( idx = 0;
            idx < sib4Ie->intraFreqNeighCellLst.noComp.val;
            idx++
          )
      {
         WR_FILL_TKN_UINT(sib4Ie->intraFreqNeighCellLst.member[idx].pres, 
               PRSNT_NODEF);
         WR_FILL_TKN_UINT(sib4Ie->intraFreqNeighCellLst.member[idx].physCellId, 
               whiteLst->cellInfo[idx].phyCellId);
         /*RE_CHANGES*/ /* LTE_ADV starts */
         if (cellCb->lteAdvCb.lteAdvCfg.reCfg.status == RGR_ENABLE)
         {
            WR_FILL_TKN_UINT(sib4Ie->intraFreqNeighCellLst.member[idx].q_OffsetCell,
                  (U32)cellCb->lteAdvCb.lteAdvCfg.reCfg.neighQoffset);
         }
         else
         {
            WR_FILL_TKN_UINT(sib4Ie->intraFreqNeighCellLst.member[idx].q_OffsetCell,
                  whiteLst->cellInfo[idx].qoffset);
         }
         /* LTE_ADV ends */
      }
   }


   blackLst = &nrWlBlInfo->intra.blData;;
   if(blackLst && blackLst->numBlInfos) 
   {
      if (blackLst->numBlInfos > 16 )
      {
         blackLst->numBlInfos = 16;
      }
      /*-Fill Intra-Frequency Black Listed Cell List-*/
      WR_FILL_TKN_UINT(sib4Ie->intraFreqBlackCellLst.noComp, blackLst->numBlInfos);
      WR_GET_MEM(sib4Sdu,
            (sib4Ie->intraFreqBlackCellLst.noComp.val)
            *(sizeof(NhuPhysCellIdRange)),
            &(sib4Ie->intraFreqBlackCellLst.member));
      for (idx = 0;
            idx < sib4Ie->intraFreqBlackCellLst.noComp.val; idx++)
      {
         WR_FILL_TKN_UINT(sib4Ie->intraFreqBlackCellLst.member[idx].pres,
               PRSNT_NODEF);
         WR_FILL_TKN_UINT(sib4Ie->intraFreqBlackCellLst.member[idx].start,
               blackLst->blInfo[idx].startPhyCellID);
         if (blackLst->blInfo[idx].phyCellIdRangePres) 
         {
            WR_FILL_TKN_UINT(sib4Ie->intraFreqBlackCellLst.member[idx].range,
                  blackLst->blInfo[idx].phyCellIdRange);
         }
      }
   }
   if (sib4->csgCellInfoPres)
   {
      WR_FILL_TKN_UINT(sib4Ie->csg_PhysCellIdRange.pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib4Ie->csg_PhysCellIdRange.start,
                       sib4->csgPhyCellIdRange.startPhyCellID);
      if (sib4->csgPhyCellIdRange.phyCellIdRangePres) 
      {
         WR_FILL_TKN_UINT(sib4Ie->csg_PhysCellIdRange.range,
                          sib4->csgPhyCellIdRange.phyCellIdRange);
      }
   } 
   else 
   {
        sib4Ie->csg_PhysCellIdRange.pres.pres = NOTPRSNT;
   }

   RETVALUE(ROK);
} /* end of wrSibBldSIB4IEs */

/** @brief This function find the particular neighbour frequcy information from
 * all neighbour frequency informations.
 *
 * @details
 *
 *     Function: wrGetWlBlInfoPerDlFreq 
 *
 *         Processing steps:
 *         - This function traverse the list of neighbour frequency informations
 *           and if there is any match found in requested downlink frequency 
 *           then retuen that block of neigbour freqency information. 
 *
 * @param [in] dlCarrierFreq : Down link carrear frequency.
 * @param [in] nrAllWlBlInfo : White list and Black list for all frequencies
 *                           infromation.
 * @return WrNrPerFreqInfo *
 *    -# Success : Pointer to the WrNrPerFreqInfo.
 *    -# Failure : NULLP
 */
PRIVATE WrNrPerFreqInfo* wrGetWlBlInfoPerDlFreq
(
 U16             dlCarrierFreq, 
 WrNrWlBlInfo    *nrAllWlBlInfo
)
{
   WrNrInterFreqInfo *interFreqInfo = &nrAllWlBlInfo->inter;
   U8                nrInfoIndx = 0;

   for (nrInfoIndx = 0; nrInfoIndx < interFreqInfo->numFreq; nrInfoIndx++)
   {
      if(interFreqInfo->freqList[nrInfoIndx].dlCarrierFreq == dlCarrierFreq)
      {
         RETVALUE(&(interFreqInfo->freqList[nrInfoIndx]));
      }
   }/* traverse the list of neighbour frequencies */
   RETVALUE(NULLP);
} /* end of wrGetWlBlInfoPerDlFreq */

/** @brief This function fills SIB5 IEs for a member in system information.
 *
 * @details
 *
 *     Function: wrSibBldSIB5IEs
 *
 *         Processing steps:
 *         - Build the SIB5 IEs based on OAM configured information and ANR
 *           given inforamtion.
 *         - Get the Interfrequecy Neighebour information from ANR module by
 *           using wrAnrGetWhiteAndBlackList API. 
 *         - Get the per frequecy white list and Block list from ANR
 *           information by using the function wrGetWlBlInfoPerDlFreq.
 *
 * @param [in]       cellCb : Cell control block
 * @param [in, out] sib5Sdu : RRC Data Request for SIB5 SDU.
 * @param [out]      sib5Ie : SIB5 IE.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrSibBldSIB5IEs
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *sib5Sdu,
NhuSysInfoBlockTyp5          *sib5Ie
)
{
   WrSib3Params            *sib3     = &(cellCb->sib3);
   WrEutranFreqCb          *interFreqInfo = NULLP;
   CmLListCp               *interFreqInfoLstCp = NULLP;
   CmLList                 *freqInfoLnk   = NULLP;
   WrNrWlBlInfo            *nrAllWlBlInfo = NULLP;
   WrNrPerFreqInfo         *nrInfoPerFreq = NULLP; 
   WrNrPerFreqWlInfo       *wlData = NULLP; 
   WrNrPerFreqBlInfo       *blData =NULLP;
   U8                      idx1 = 0;
   U8                      idx2 = 0;

   TRC2(wrSibBldSIB5Ies);

   WR_FILL_TKN_UINT(sib5Ie->pres, PRSNT_NODEF);

   /* Get neighbor list info from ANR Module */
   nrAllWlBlInfo = wrEmmAnrGetWhiteAndBlackList(cellCb->cellId, WR_ANR_LST_INTER_FREQ, TRUE);
   if (!nrAllWlBlInfo)
   {
      RETVALUE(RFAILED);
   }

   /*-Fill Inter-Frequency Carrier Frequency List -*/
   interFreqInfoLstCp = &cellCb->eutranNbrFreqLst;
   WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.noComp, ((interFreqInfoLstCp->count)-1));
   WR_GET_MEM(sib5Sdu, (sib5Ie->interFreqCarrierFreqLst. noComp.val)
         *(sizeof(NhuInterFreqCarrierFreqInfo)),
         &(sib5Ie->interFreqCarrierFreqLst.member));
   idx1 = 0; 
   /* Traverse the EUTRAN NeghBOUR Frequncey list info List */ 
   CM_LLIST_FIRST_NODE(interFreqInfoLstCp, freqInfoLnk);
   while (NULLP != freqInfoLnk)
   {
      interFreqInfo = (WrEutranFreqCb *)(freqInfoLnk->node);

      if (interFreqInfo->dlEarfcn != cellCb->dlCarrierFreq)
      {
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].pres,
               PRSNT_NODEF);
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].dl_CarrierFreq,
               interFreqInfo->dlEarfcn);
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].q_RxLevMin,
               interFreqInfo->qRxLevMin);
         if (interFreqInfo->pMaxPres)
         {
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].p_Max,
                  interFreqInfo->pMax); 
         } 
         else
         {
            sib5Ie->interFreqCarrierFreqLst.member[idx1].p_Max.pres = NOTPRSNT;
         }

         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               t_ReselectionEUTRA, interFreqInfo->tReselcxn);
         if (interFreqInfo->sfPres)
         {
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  t_ReselectionEUTRA_SF.pres, PRSNT_NODEF);
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  t_ReselectionEUTRA_SF.sf_Medium,
                  interFreqInfo->sfMedium);
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  t_ReselectionEUTRA_SF.sf_High,
                  interFreqInfo->sfHigh);
         }
         else 
         {
            sib5Ie->interFreqCarrierFreqLst.member[idx1].t_ReselectionEUTRA_SF.
               pres.pres = NOTPRSNT;
         }
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               threshX_High, interFreqInfo->threshXHigh);
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               threshX_Low, interFreqInfo->threshXLow);
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               allowedMeasBandwidth, interFreqInfo->allowedMeasBW.val);
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               presenceAntennaPort1, interFreqInfo->presenceAntPort1);
         /* Cell Reselection Priority */
         /*  if (interFreqInfo->cellReSelPriPres) 
             { */
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               cellReselectionPriority, interFreqInfo->cellReSelPri);

         /*} else {
           sib5Ie->interFreqCarrierFreqLst.member[idx1].cellReselectionPriority.
           pres = NOTPRSNT;
           }*/
         /* Neighbor Cell Config  */
         WR_FILL_BIT_STR(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               neighCellConfig,2, interFreqInfo->neighCellConfig, sib5Sdu);
         /* Q-Offset Freq */
         WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
               q_OffsetFreq, interFreqInfo->qOffsetFreq);

         nrInfoPerFreq = NULLP;
         /* Get the white and black list neighbor info as per the Dl freq */
         nrInfoPerFreq = wrGetWlBlInfoPerDlFreq(interFreqInfo->dlEarfcn,
               nrAllWlBlInfo);
         if (NULLP == nrInfoPerFreq)
         {
            RLOG1(L_INFO,"wrSibBldSIB5IEs: there is no wl and bl"
                     "for this frequency:[%d]",
                     interFreqInfo->dlEarfcn);
         }

         /* For this DL freq, White List information is present */
         if (nrInfoPerFreq && (nrInfoPerFreq->wlData.numWlCells)) 
         {
            wlData = &(nrInfoPerFreq->wlData);

            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.noComp, wlData->numWlCells);
            WR_GET_MEM(sib5Sdu, (sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.noComp.val)
                  *(sizeof(NhuInterFreqNeighCellInfo)),
                  &(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.member));
            for (idx2 = 0;
                  idx2 < sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.noComp.val;
                  idx2++)
            {
               WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.member[idx2].pres,
                     PRSNT_NODEF);
               WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.member[idx2].physCellId,
                     wlData->cellInfo[idx2].phyCellId);
               WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqNeighCellLst.member[idx2].q_OffsetCell,
                     wlData->cellInfo[idx2].qoffset);
            }
         }

         blData = NULLP;

         if (nrInfoPerFreq && (nrInfoPerFreq->blData.numBlInfos)) 
         {
            blData = &(nrInfoPerFreq->blData);

            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  interFreqBlackCellLst.noComp, blData->numBlInfos);
            WR_GET_MEM(sib5Sdu, 
                  (sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqBlackCellLst.noComp.val)
                  *(sizeof(NhuPhysCellIdRange)),
                  &(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqBlackCellLst.member));

            /* blFreqMember = sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqBlackCellLst.member; */
            for (idx2 = 0;
                  idx2 < sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqBlackCellLst.noComp.val; idx2++)
            {
               WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                                interFreqBlackCellLst.member[idx2].pres, PRSNT_NODEF);
               WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqBlackCellLst.member[idx2].start, 
                     blData->blInfo[idx2].startPhyCellID);
               if (blData->blInfo[idx2].phyCellIdRangePres) {
                  WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].interFreqBlackCellLst.member[idx2].range,
                        blData->blInfo[idx2].phyCellIdRange);
               }
            }
         }
         /* R9 SIB5 IEs */
         if (interFreqInfo->qQualMinR9.pres)
         {
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                             extaddgrp_1.pres, PRSNT_NODEF);
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                             extaddgrp_1.q_QualMin_r9, 
                             interFreqInfo->qQualMinR9.val);
         } 
         else 
         {
            sib5Ie->interFreqCarrierFreqLst.member[idx1].extaddgrp_1.pres.pres = NOTPRSNT;
         }
         /* reSelcThreshLowR9 and reSelcThreshHighR9 fields are mandatory present 
          * if threshServingLowQ is present in systemInformationBlockType3; 
          * otherwise it is not present.
          */
         if (sib3->r9Params.threshServLowQRel9Pres)
         {
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  extaddgrp_1.threshX_Q_r9.pres, PRSNT_NODEF);
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  extaddgrp_1.threshX_Q_r9.threshX_HighQ_r9, 
                  interFreqInfo->threshXQR9.threshXHighQR9);
            WR_FILL_TKN_UINT(sib5Ie->interFreqCarrierFreqLst.member[idx1].
                  extaddgrp_1.threshX_Q_r9.threshX_LowQ_r9,
                  interFreqInfo->threshXQR9.threshXLowQR9);
         } 
         else 
         {
            sib5Ie->interFreqCarrierFreqLst.member[idx1].extaddgrp_1.
                                        threshX_Q_r9.pres.pres = NOTPRSNT;
         }
         /* move to the next index */
         idx1++;
         /* Move to the next node */
      }
      CM_LLIST_NEXT_NODE(interFreqInfoLstCp, freqInfoLnk);
   } /* end of while */
   RETVALUE(ROK);
} /* end of wrSibBldSIB5IEs */


/** @brief This function fills System Info Block Type6 message .
 *
 * @details
 *
 *     Function: wrSibBldSIB6IEs
 *
 *         Processing steps:
 *         - This function is build the SIB6 IEs based on updated information in
 *           cell control block.
 *
 * @param [in]   cellCb : cell control block. 
 * @param [out] sib1Sdu : SIB6 SDU.
 * @param [in] schedSFN : schedfuled SFN number.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PRIVATE S16 wrSibBldSIB6IEs
(
WrCellCb          *cellCb,
NhuDatReqSdus          *sib6Sdu,
NhuSysInfoBlockTyp6        *sib6Ie
)
{ 
   WrSib6Params              *sib6 = &cellCb->sib6;
   WrUtraFreqCb              *utraFreqInfo = NULLP;
   CmLListCp                 *utraFreqInfoLstCp = NULLP;
   CmLList                   *freqInfoLnk   = NULLP;
   U8                        cntTDD = 0;
   U8                       cntFDD = 0;

   TRC2(wrSibBldSIB6IEs);

   WR_FILL_TKN_UINT(sib6Ie->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(sib6Ie->t_ReselectionUTRA,sib6->tReselection);
   if (sib6->sfUsagePres)
   {
      WR_FILL_TKN_UINT(sib6Ie->t_ReselectionUTRA_SF.pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib6Ie->t_ReselectionUTRA_SF.sf_Medium,sib6->tReselectionSfMedium);
      WR_FILL_TKN_UINT(sib6Ie->t_ReselectionUTRA_SF.sf_High,sib6->tReselectionSfHigh);
   }
   else
   {
      sib6Ie->t_ReselectionUTRA_SF.pres.pres = NOTPRSNT;
   }

   /*-Fill Utra FDD-Frequency Carrier Frequency List -*/ 
   if(cellCb->utraFddNbrFreqLst.count != 0)
   {
      WR_GET_MEM(sib6Sdu,(cellCb->utraFddNbrFreqLst.count)*(sizeof(NhuCarrierFreqUTRA_FDD)),
            &(sib6Ie->carrierFreqLstUTRA_FDD.member));
   }

   /*-Fill Utra TDD-Frequency Carrier Frequency List -*/ 
   if(cellCb->utraTddNbrFreqLst.count != 0)
   {
      WR_GET_MEM(sib6Sdu,(cellCb->utraTddNbrFreqLst.count)*(sizeof(NhuCarrierFreqUTRA_TDD)),
            &(sib6Ie->carrierFreqLstUTRA_TDD.member));
   }

   /* Traverse the UTRAN FDD Neighbour Frequncey list info List */
   utraFreqInfoLstCp = &cellCb->utraFddNbrFreqLst;
   cntFDD = 0;

   CM_LLIST_FIRST_NODE(utraFreqInfoLstCp, freqInfoLnk);
   while(NULLP != freqInfoLnk)
   {
      utraFreqInfo = (WrUtraFreqCb *)(freqInfoLnk->node);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.noComp, 
            utraFreqInfoLstCp->count);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].pres,
            PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].carrierFreq,
            utraFreqInfo->t.utraFddFreq.arfcnDl);
      if(utraFreqInfo->priority.pres == PRSNT_NODEF)
      {
         WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].
               cellReselectionPriority, utraFreqInfo->priority.val);
      }
      else
      {
         sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].
            cellReselectionPriority.pres = NOTPRSNT;
      }
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].threshX_High,
            utraFreqInfo->threshXhigh);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].threshX_Low,
            utraFreqInfo->threshXlow);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].q_RxLevMin,
            utraFreqInfo->qRxLevMin);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].p_MaxUTRA,
            utraFreqInfo->pMaxUtra);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].q_QualMin,
            utraFreqInfo->t.utraFddFreq.pQualMin);

      if(utraFreqInfo->t.utraFddFreq.thresQR9Pres == PRSNT_NODEF)
      {
         WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].
               extaddgrp_1.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].
               extaddgrp_1.threshX_Q_r9.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].
               extaddgrp_1.threshX_Q_r9.threshX_HighQ_r9, utraFreqInfo->t.utraFddFreq.threshXhighQR9);
         WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].
               extaddgrp_1.threshX_Q_r9.threshX_LowQ_r9, utraFreqInfo->t.utraFddFreq.threshXlowQR9);
      }
      else 
      {
         sib6Ie->carrierFreqLstUTRA_FDD.member[cntFDD].extaddgrp_1.pres.pres = NOTPRSNT;
      }

      /* move to the next index */
      cntFDD++;
      CM_LLIST_NEXT_NODE(utraFreqInfoLstCp, freqInfoLnk);
   } /* end of while */

   /* Traverse the UTRAN TDD Neighbour Frequncey list info List */
   utraFreqInfoLstCp = &cellCb->utraTddNbrFreqLst;
   cntTDD = 0;
   CM_LLIST_FIRST_NODE(utraFreqInfoLstCp, freqInfoLnk);
   while(NULLP != freqInfoLnk)
   {
      utraFreqInfo = (WrUtraFreqCb *)(freqInfoLnk->node);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.noComp,
            utraFreqInfoLstCp->count);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].pres,
            PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].carrierFreq,
            utraFreqInfo->t.utraTddFreq.arfcn);
      if(utraFreqInfo->priority.pres == PRSNT_NODEF)
      {
         WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].
               cellReselectionPriority, utraFreqInfo->priority.val);
      }
      else
      {
         sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].
            cellReselectionPriority.pres = NOTPRSNT;
      }
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].threshX_High,
            utraFreqInfo->threshXhigh);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].threshX_Low,
            utraFreqInfo->threshXlow);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].q_RxLevMin,
            utraFreqInfo->qRxLevMin);
      WR_FILL_TKN_UINT(sib6Ie->carrierFreqLstUTRA_TDD.member[cntTDD].p_MaxUTRA,
            utraFreqInfo->pMaxUtra);

      /* move to the next index */
      cntTDD++;
      CM_LLIST_NEXT_NODE(utraFreqInfoLstCp, freqInfoLnk);
   }

   RETVALUE(ROK);
} /* End of wrSibBldSIB6Msg */

/** @brief This function fills System Info Block Type7 message .
 *
 * @details
 *
 *     Function: wrSibBldSIB7IEs
 *
 *         Processing steps:
 *         - This function is build the SIB7 IEs based on updated information in
 *           cell control block.
 *
 * @param [in]   cellCb : cell control block. 
 * @param [out] sib7Sdu : SIB7 SDU.
 * @param [out] sib7Ie  : SIB7 Ie list.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PRIVATE S16 wrSibBldSIB7IEs
(
WrCellCb			       *cellCb,
NhuDatReqSdus			 *sib7Sdu,
NhuSysInfoBlockTyp7   *sib7Ie
)
{ 
   WrSib7Params      *sib7 = &cellCb->sib7;
   WrGeranFreqCb     *geranFreqInfo = NULLP;
   CmLListCp         *geranFreqInfoLstCp = NULLP;
   CmLList           *freqInfoLnk   = NULLP;
   U8 			  		cntGeranFreq = 0;

   TRC2(wrSibBldSIB7IEs);

   WR_FILL_TKN_UINT(sib7Ie->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(sib7Ie->t_ReselectionGERAN,sib7->tReselectionGERAN);
   if (sib7->tReselSpeedSFPres)
   {
      WR_FILL_TKN_UINT(sib7Ie->t_ReselectionGERAN_SF.pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib7Ie->t_ReselectionGERAN_SF.sf_Medium,sib7->tReselGeranSFMedium);
      WR_FILL_TKN_UINT(sib7Ie->t_ReselectionGERAN_SF.sf_High,sib7->tReselGeranSFHigh);
   }
   else
   {
      sib7Ie->t_ReselectionGERAN_SF.pres.pres = NOTPRSNT; 
   }

   /*---------Fill GERAN-Frequency Carrier Frequency List -------*/ 
   geranFreqInfoLstCp = &cellCb->geranNbrFreqLst;
   if(geranFreqInfoLstCp->count != 0)
   {
      WR_GET_MEM(sib7Sdu,(geranFreqInfoLstCp->count)*(sizeof(NhuCarrierFreqsInfoGERAN)),
            &(sib7Ie->carrierFreqsInfoLst.member));
   }
   else
   {
      RLOG0(L_ERROR," Number of GERAN neighbor frequencies can't be zero ");
      RETVALUE(RFAILED);
   }

   /* Traverse the GERAN NeghBOUR Frequncey list info List */
   WR_FILL_TKN_UINT(sib7Ie->carrierFreqsInfoLst.noComp, 
         geranFreqInfoLstCp->count); /* Check shifting above */

   cntGeranFreq = 0;
   CM_LLIST_FIRST_NODE(geranFreqInfoLstCp, freqInfoLnk);
   while(NULLP != freqInfoLnk)
   {
      geranFreqInfo = (WrGeranFreqCb *)(freqInfoLnk->node);
      WR_FILL_TKN_UINT(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].carrierFreqs.pres,
            PRSNT_NODEF);
      if(WR_GERAN_PCS1900 == geranFreqInfo->bandInd)
      {
         wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].carrierFreqs.bandIndicator), 1);
      }
      else
         wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].carrierFreqs.bandIndicator), 0);
      /* --------------------------------------------------*/
      wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].carrierFreqs.startingARFCN), geranFreqInfo->arfcn);
      wrFillTknU8(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].carrierFreqs.followingARFCNs.choice), FOLLOWINGARFCNS_EXPLICITLSTOFARFCNS);
      wrFillTknU16(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].carrierFreqs.followingARFCNs.val.explicitLstOfARFCNs.noComp), 0);


      /* TODO: currently "carrierFreq.followingARFCNs" getting filled with one arfcn */
      /* --------------------------------------------------*/
      WR_FILL_TKN_UINT(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.pres, PRSNT_NODEF);
      wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.cellReselectionPriority), geranFreqInfo->cellReselPriority);
      wrFillTknStrBSXL(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.ncc_Permitted), 8,  &(geranFreqInfo->nccPermitted), &sib7Sdu->memCp);
      wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.q_RxLevMin), geranFreqInfo->qRxLevMin);
      if(geranFreqInfo->pMaxGeran.pres == TRUE)
      {
         wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.p_MaxGERAN), geranFreqInfo->pMaxGeran.val);
      }
      wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.threshX_High), geranFreqInfo->threshXHi);
      wrFillTknU32(&(sib7Ie->carrierFreqsInfoLst.member[cntGeranFreq].commonInfo.threshX_Low), geranFreqInfo->threshXLo);

      /* --------------------------------------------------*/
      /* moving on to next GERAN Freq index */
      cntGeranFreq++;

      CM_LLIST_NEXT_NODE(geranFreqInfoLstCp, freqInfoLnk);
   } /* end of while */
   RETVALUE(ROK);
} /* End of wrSibBldSIB7IEs */

/** @brief This function fills SIB10 IEs for a member in system information
 *
 * @details
 *
 *     Function: wrSibBldSIB10IEs
 *
 *         Processing steps:
 *         - Build the SIB10 IEs based on primary notification information
 *           received in write replacement warning request which was updated
 *           by PWS module in current cell control block.
 *
 * @param [in]        cellCb : cell control block
 * @param [in, out] sib10Sdu :  RRC data Request for SIB10 SDU.
 * @param [out]      sib10Ie : SIB10 IE which will be included in SI message.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrSibBldSIB10IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib10Sdu,
 NhuSysInfoBlockTyp10          *sib10Ie
 )
{
   WrPwsEtwsInfo *pri = NULLP;
   Data                  *flatBuf = NULLP;
   MsgLen                cnt = 0;
   U8                    idOctets[2];
   Buffer                *wtype = NULLP;

   TRC2(wrSibBldSIB10IEs);

   if (!cellCb)
   {
      return RFAILED;
   }

   pri = &(cellCb->warningInfo->etwsInfo);

   WR_FILL_TKN_UINT(sib10Ie->pres, PRSNT_NODEF);

   /* Fill the Message Identifier */
   idOctets[0] = (pri->wrngMsgId & 0xff00) >> 8;
   idOctets[1] = (pri->wrngMsgId & 0x00ff);


   wrFillTknStrBSXL(&(sib10Ie->messageId), 16,
        idOctets, &(sib10Sdu->memCp));

   /* Fill the Serail Number */
   idOctets[0] = (pri->wrngSerialNum & 0xff00) >> 8;
   idOctets[1] = (pri->wrngSerialNum & 0x00ff);

   wrFillTknStrBSXL(&(sib10Ie->serialNumber), 16,
         idOctets, &(sib10Sdu->memCp));

   /* Fill the Warning Type  */
    WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &wtype);
    if ( wtype == NULLP)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       RETVALUE(RFAILED);
    }

    SAddPstMsgMult((Data *)&(pri->primEtwsInfo.warningType),
              (MsgLen) sizeof(U16),
              (Buffer *)wtype);

    WR_ALLOC(&flatBuf, sizeof(U16));
    if (flatBuf == NULLP)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       WR_FREEMBUF(wtype);
       RETVALUE(RFAILED);
    }

    SCpyMsgFix(wtype, 0, sizeof(U16), flatBuf, &cnt);

    wrFillTknStrOSXL(&(sib10Ie->warningTyp),
         sizeof(U16),
         flatBuf,
         &(sib10Sdu->memCp));

    WR_FREE(flatBuf, sizeof(U16));

    /* Free the buffer. */
    WR_FREEMBUF(wtype);


   RETVALUE(ROK);
} /* end of wrSibBldSIB10IEs */

/** @brief This function fills SIB11 IEs for a member in system information.
 *
 * @details
 *
 *     Function: wrSibBldSIB11IEs
 *
 *         Processing steps:
 *         - Build the SIB11 IEs based on secondary notification information
 *           received in write replacement warning request which was updated
 *           by PWS module in current cell control block.
 *
 * @param [in]        cellCb : cell control block
 * @param [in, out] sib11Sdu : RRC Data Request for SIB11 SDU.
 * @param [out]      sib11Ie : SIB11 IE which will be included in SI message.
 * @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PRIVATE S16 wrSibBldSIB11IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib11Sdu,
 NhuSysInfoBlockTyp11         *sib11Ie
 )
{
   WrPwsWarningInfo      *sec = NULLP;
   Data                  *flatBuf = NULLP;
   Data                  *flatBufDCS = NULLP;
   MsgLen                cnt = 0;
   U8                    idOctets[2];
   Buffer                *dcs = NULLP;


   TRC2(wrSibBldSIB11IEs);

   if (!cellCb)
   {
      RETVALUE(RFAILED);
   }

   sec = (cellCb->warningInfo);

   WR_FILL_TKN_UINT(sib11Ie->pres, PRSNT_NODEF);

   /* Fill the Message Identifier */
   idOctets[0] = (sec->etwsInfo.wrngMsgId & 0xff00) >> 8;
   idOctets[1] = (sec->etwsInfo.wrngMsgId & 0x00ff);


   wrFillTknStrBSXL(&(sib11Ie->messageId), 16,
        idOctets, &(sib11Sdu->memCp));

   /* Fill the Serail Number */
   idOctets[0] = (sec->etwsInfo.wrngSerialNum & 0xff00) >> 8;
   idOctets[1] = (sec->etwsInfo.wrngSerialNum & 0x00ff);

   wrFillTknStrBSXL(&(sib11Ie->serialNumber), 16,
         idOctets, &(sib11Sdu->memCp));

   /* Fill the Segment type is last segment or not                           */
   WR_FILL_TKN_UINT(sib11Ie->warningMsgSegmentTyp, sec->siBldInfo.isLastSeg);
   /* Fill the Segment Number range is 0 to 64                               */
   WR_FILL_TKN_UINT(sib11Ie->warningMsgSegmentNumber, sec->siBldInfo.segNum);

   /* Fill the warning message Segment with the specified length */
   WR_ALLOC(&flatBuf, sec->siBldInfo.segLen);
   if (flatBuf == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

    SCpyMsgFix((sec->siBldInfo.segment), 0,
                  sec->siBldInfo.segLen, flatBuf, &cnt);

    wrFillTknStrOSXL(&(sib11Ie->warningMsgSegment),
         sec->siBldInfo.segLen,
         (Data *)flatBuf,
         &(sib11Sdu->memCp));

    WR_FREE(flatBuf, sec->siBldInfo.segLen);

    /* Fill the Data Codeing Scheme */
    WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &dcs);
    if ( dcs == NULLP)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       RETVALUE(RFAILED);
    }


    SAddPstMsgMult((Data *)&(sec->etwsInfo.secEtwsInfo.dataCodingScheme),
              (MsgLen) 1,
              (Buffer *)dcs);

    WR_ALLOC(&flatBufDCS, 1);
    if (flatBufDCS == NULLP)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       WR_FREEMBUF(dcs);
       RETVALUE(RFAILED);
    }

    SCpyMsgFix(dcs, 0, 1, flatBufDCS, &cnt);

    wrFillTknStrOSXL(&(sib11Ie->dataCodingScheme),
         1,
         flatBufDCS,
         &(sib11Sdu->memCp));

    WR_FREE(flatBufDCS, 1);

    /* Free the buffer. */
    WR_FREEMBUF(dcs);
    RETVALUE(ROK);
} /* end of wrSibBldSIB11IEs */




/* PH04_CMAS */
PRIVATE S16 wrSibBldSIB12IEs
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *sib12Sdu,
 NhuSysInfoBlockTyp12_r9         *sib12Ie
)
{
   WrPwsCmasInfo             *cmasInfo = NULLP;
   Data                      *flatBuf       = NULLP;
   Data                      *flatBufDCS    = NULLP;
   MsgLen                    cnt            = 0;
   U8                        idOctets[2];
   Buffer                    *dcs = NULLP;

   TRC2(wrSibBldSIB12IEs);

   if (!cellCb)
   {
      RETVALUE(RFAILED);
   }

   /* Get the CMAS message details. */
   cmasInfo = cellCb->warningInfo->siBldInfo.crntCmas;
   if (cmasInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }

   WR_FILL_TKN_UINT(sib12Ie->pres, PRSNT_NODEF);

   /* Fill the Message Identifier */
   idOctets[0] = (cmasInfo->wrngMsgId & 0xff00) >> 8;
   idOctets[1] = (cmasInfo->wrngMsgId & 0x00ff);

   wrFillTknStrBSXL(&(sib12Ie->messageId_r9), 16,
        idOctets, &(sib12Sdu->memCp));

   /* Fill the Serail Number */
   idOctets[0] = (cmasInfo->wrngSerialNum & 0xff00) >> 8;
   idOctets[1] = (cmasInfo->wrngSerialNum & 0x00ff);

   wrFillTknStrBSXL(&(sib12Ie->serialNumber_r9), 16,
         idOctets, &(sib12Sdu->memCp));

   /* Fill the Segment type is last segment or not */
   WR_FILL_TKN_UINT(sib12Ie->warningMsgSegmentTyp_r9,
                    cellCb->warningInfo->siBldInfo.isLastSeg);
   /* Fill the Segment Number range is 0 to 64 */
   WR_FILL_TKN_UINT(sib12Ie->warningMsgSegmentNumber_r9,
                           cellCb->warningInfo->siBldInfo.segNum);

   /* Fill the warning message Segment with the specified length */
   WR_ALLOC(&flatBuf, cellCb->warningInfo->siBldInfo.segLen);
   if (flatBuf == NULLP)
   { 
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
    
   SCpyMsgFix((cellCb->warningInfo->siBldInfo.segment), 0,
                  cellCb->warningInfo->siBldInfo.segLen, flatBuf, &cnt);

    wrFillTknStrOSXL(&(sib12Ie->warningMsgSegment_r9),
         cellCb->warningInfo->siBldInfo.segLen,
         (Data *)flatBuf,
         &(sib12Sdu->memCp));

    WR_FREE(flatBuf, cellCb->warningInfo->siBldInfo.segLen);


   /* Fill the Data Codeing Scheme */
    WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &dcs);
    if ( dcs == NULLP)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       RETVALUE(RFAILED);
    }

    SAddPstMsgMult((Data *)&(cmasInfo->wrngMsgInfo.dataCodingScheme),
              (MsgLen) 1,
              (Buffer *)dcs);

    WR_ALLOC(&flatBufDCS, 1);
    if (flatBufDCS == NULLP)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       WR_FREEMBUF(dcs);
       RETVALUE(RFAILED);
    }

    SCpyMsgFix(dcs, 0, 1, flatBufDCS, &cnt);

    wrFillTknStrOSXL(&(sib12Ie->dataCodingScheme_r9),
         1,
         flatBufDCS,
         &(sib12Sdu->memCp));

    WR_FREE(flatBufDCS, 1);

    /* Free the buffer. */
    WR_FREEMBUF(dcs);


   RETVALUE(ROK);
} /* end of wrSibBldSIB12IEs */



/** @brief This function builds SI based on the included SIBs
 *
 * @details
 *
 *     Function: wrEmmPbmBldSI 
 *
 *         Processing steps:
 *         - Build SI header for the current SI message.
 *         - Build the all the sibs mentioned in the siInfo of the current SI message.
 *
 * @param [in]   cellCb : cell control block
 * @param [in]    siSdu : System Information Message PDU.
 * @param [in]   siInfo : SI Information message.
 * @param [in] schedSFN : scheduled SFN for the SI message.
 * @param [in] subframe : subframe number.
 * @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPbmBldSI
(
 WrCellCb                     *cellCb,
 NhuDatReqSdus                *siSdu,
 WrSISchdInfo                 *siInfo,
 U16                          schedSFN,
 U8                           subframe,
 U8                           siIndx
 )
{
   NhuBCCH_DL_SCH_Msg        *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns   *critExt    = NULLP;
   U8                        idx         = 0;
   S16                       ret         = RFAILED;
   WrPbmInfo                 *pbmInfo    = NULLP;

   TRC2(wrEmmPbmBldSi);

   ret = wrSibBldSIHeader(cellCb, siSdu, schedSFN, subframe, siInfo->numSibs);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   bcchSchMsg = &(siSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);

   for(idx = 0; idx < siInfo->numSibs; idx++)
   {
      NhuSysInfo_r8_IEssib_TypAndInfoMember *member;

      member = &(critExt->val.systemInfo_r8.sib_TypAndInfo.member[idx]);
      /* TODO: Need to add other SIBs to this switch statement */
      switch(siInfo->sibs[idx])
      {
         case WR_SIB_2:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB2);
            ret = wrSibBldSIB2IEs(cellCb, siSdu, &(member->val.sib2));

            /* updating on-going si-index for SIB 2 */
            cellCb->sib2.siIndex = siIndx;
            break;
         case WR_SIB_3:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB3);
            ret = wrSibBldSIB3IEs(cellCb, siSdu, &(member->val.sib3));
            /* updating on-going si-index for SIB 3 */
            cellCb->sib3.siIndex = siIndx;
            break;
         case WR_SIB_4:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB4);
            ret = wrSibBldSIB4IEs(cellCb, siSdu, &(member->val.sib4));
            /* updating on-going si-index for SIB 4 */
            cellCb->sib4.siIndex = siIndx;
            break;
         case WR_SIB_5:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB5);
            ret = wrSibBldSIB5IEs(cellCb, siSdu, &(member->val.sib5));
            /* updating on-going si-index for SIB 5 */
            cellCb->sib5.siIndex = siIndx;
            break;
         case WR_SIB_6:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB6);
            ret = wrSibBldSIB6IEs(cellCb, siSdu, &(member->val.sib6));
            /* updating on-going si-index for SIB 6 */
            cellCb->sib6.siIndex = siIndx;
            break;
         case WR_SIB_7:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB7);
            ret = wrSibBldSIB7IEs(cellCb, siSdu, &(member->val.sib7));
            /* updating on-going si-index for SIB 7 */
            cellCb->sib7.siIndex = siIndx;
            break;
         case WR_SIB_8:
            {
               U16    sib8Prd = 0;
               RgrSiPeriodicity sib8Prdcty;
               cellCb->sib8.siIndex = siIndx;
               WR_FILL_TKN_UINT(member->choice, MEMBER_SIB8);
               ret = wrSibBldSIB8IEs(cellCb, siSdu, &(member->val.sib8));
               /* updating on-going si-index for SIB 8 */
               WR_ALLOC(&pbmInfo, sizeof(WrPbmInfo));
               if(NULLP == pbmInfo)
               {
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               WR_GET_SIB_PERIOD(cellCb, WR_SIB_8, sib8Prd);
               wrEmCellSchdSiPeriodMap(sib8Prd, &sib8Prdcty);
               /* fill the pbmInfo entry */
               pbmInfo->pbmInfoType = WR_PBM_TYPE_SIB8;
               pbmInfo->pendPagMsg.pf = 
                  (((cellCb->pbmCb.curSfn + sib8Prdcty) - WR_SFN_INCREMENT))%WR_MAX_SFN; 
      
               /* Insert to "pbmSchedInfo" table based on PF in pbmInfo */
               wrPbmSib8InsertRecord(cellCb, pbmInfo);    
            }
            break;
          case WR_SIB_9:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB9);
            WR_FILL_TKN_UINT(member->val.sib9.pres, PRSNT_NODEF);
            wrFillTknStrOSXL(&member->val.sib9.hnb_Name,(U16)strlen((S8 *)cellCb->sib9.henbName),
                                     (Data *)cellCb->sib9.henbName,(CmMemListCp *) siSdu);

            /* updating on-going si-index for SIB 9 */
            cellCb->sib9.siIndex = siIndx;
            break;
         case WR_SIB_10:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB10);
            ret = wrSibBldSIB10IEs(cellCb, siSdu, &(member->val.sib10));
            break;
         case WR_SIB_11:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB11);
            ret = wrSibBldSIB11IEs(cellCb, siSdu, &(member->val.sib11));
            break;
         /* PH04_CMAS */
         case WR_SIB_12:
            WR_FILL_TKN_UINT(member->choice, MEMBER_SIB12_V920);
            ret = wrSibBldSIB12IEs(cellCb, siSdu, &(member->val.sib12_v920));
               /* Fill The non critical section not present */
            critExt->val.systemInfo_r8.nonCriticalExtn.pres.pres = NOTPRSNT;
            break;
      }
   }
   RETVALUE(ret);
} /* end of wrEmmPbmBldSI */

/** @brief This function is used to build the DL BCCH PDU.
 *
 * @details
 *
 *     Function: wrBldDlBcchBchPdu
 *
 *         Processing steps:
 *         - Encode the RRC Data Request Event for DL BCCH BCH PDU in enocde buffer.
 *         - Free the allocated memory for RRC Data Request event for DL BCCH
 *           BCH PDU.
 *         - Post the encoded buffer to the RRC layer.
 *
 * @param [out] nhDatReqEvntSdu : RRC Data Req Event PDU.
 * @param [out]          encBuf : Encode buffer. 
 * @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PUBLIC S16 wrBldDlBcchBchPdu
(
 NhuDatReqSdus  *nhDatReqEvntSdu,
 Buffer         **encBuf
 )
{
   S16           ret = RFAILED;
   Void          *pdu;
   NhuCmnMsg     *cmnMsg;
   U32           length;
   
   TRC2(wrBldDlBcchBchPdu);

   cmnMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg);
   /* Encode the System info */
   {
      pdu = (Void *) &(cmnMsg->cmn.bcchBchMsg);
   }

   ret = nhEDMEncReq(pdu, NHU_MSG_DLBCCHBCH, encBuf, &length);
   WR_FREEEVNT(nhDatReqEvntSdu);
   if (ROK != ret)
   {
      /* Handle encode error here */
      RLOG0(L_ERROR,"wrBldDlBcchBchPdu: encode PDU failed.");
      RETVALUE(ret);
   }
   /*DEL ccpu00125691 : Padding is not required for MIB*/
   RETVALUE(ROK);
} /* end of wrBldDlBcchBchPdu */

/** @brief This function builds DL BCCH DLSCH PDU.
 *
 * @details
 *
 *     Function: wrBldDlBcchDlSchPdu
 *
 *         Processing steps:
 *         - Encode the RRC Data Request Event for DL BCCH DLSCH PDU in enocde buffer.
 *         - Free the allocated memory for RRC Data Request event for DL BCCH
 *           DLSCH PDU.
 *         - Post the encoded buffer to the RRC layer.
 *
 * @param [out] nhDatReqEvntSdu : RRC Data Req Event PDU.
 * @param [out]          encBuf : Encode buffer. 
 * @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
PUBLIC S16 wrBldDlBcchDlSchPdu
(
 NhuDatReqSdus  *nhDatReqEvntSdu,
 Buffer         **encBuf
 )
{
   S16           ret = RFAILED;
   Void          *pdu;
   NhuCmnMsg     *cmnMsg;
   U32           length;
   U32           idx;
   U32           padOctets;
   U32           msgOctets;
   U32           tbSize;

   TRC2(wrBldDlBcchDlSchPdu);

   cmnMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg);
   pdu    = (Void *) &(cmnMsg->cmn.bcchDlSchMsg); 
   ret = nhEDMEncReq(pdu, NHU_MSG_DLBCCHDLSCH, encBuf, &length);
   WR_FREEEVNT(nhDatReqEvntSdu);
   if (ROK != ret)
   {
      /* Handle encode error here */
      RLOG0(L_ERROR,"wrBldDlBcchBchPdu: encode PDU failed.");
      RETVALUE(ret);
   }
   if (NULLP == *encBuf)
   {
      /* Handle encode error here */
      RLOG0(L_ERROR,"wrBldDlBcchBchPdu: encBuf is NULLP.");
      RETVALUE(RFAILED);
   }
   /* Address padding requirement */
   msgOctets = (length/8) + ((length%8)==0? 0:1);

   /*MOD ccpu00125691 bytes and bits conversion & vice versa are taken care*/
   /*Get the TB SIZE for padding purpose*/
   WR_GET_TBSZ(msgOctets * 8, tbSize);
   padOctets = (tbSize / 8 - msgOctets);
   /* TODO : Derive a logic for this */
   if (padOctets < 0)
   {
      /* Handle the length error */
      RLOG0(L_ERROR,"wrBldDlBcchBchPdu: TBSz is smaller than encoded length.");
      WR_FREEMBUF(*encBuf);
      RETVALUE(RFAILED);
   }

   for (idx=0; idx < padOctets; idx++)
   {
      SAddPstMsg(0, *encBuf);
   }

   RETVALUE(ROK);
} /* end of wrBldDlBcchDlSchPdu*/

PUBLIC S16 wrEmmPbmCellSchdReCfg
(
U16                          cellId
)
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *cellReCfg = NULLP;
   WrCellCb                  *cellCb = NULLP;

   TRC2(wrEmmCellSchdCfg);

   RLOG0(L_DEBUG,
            "wrEmmPbmCellSchdReCfg: Filling Cell Cfg over RGR");

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;
   cellReCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);

   /* Populate RgrCellCfg */
   cellReCfg->cellId = (U16)cellId;
   cellReCfg->recfgTypes = RGR_CELL_SI_RECFG; 

   /* SI configuration */
   if (wrEmmCellSchdSiCfg(&cellReCfg->siReCfg, cellCb) != ROK)
   {
      RLOG1(L_ERROR,
               "wrEmmPbmCellSchdReCfg: wrEmmCellSchdSiCfg failed %d", cellId);
      RETVALUE(RFAILED);
   }

   if(ROK != WrIfmSchdCfgCell(cfgReq, (U32)cellId))
   {
      RLOG1(L_ERROR,
               "wrEmmPbmCellSchdReCfg: WrSndRgrCfgReq failed. cellId %d", cellId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}


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
