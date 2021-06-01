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
               

     File:     wr_umm_rrc_msg.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=66;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_utils.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_drx.h"
#include "wr_umm_meas_fun.h"
#include "wr_umm_ecsfb_utils.h"

EXTERN S16 wrSibBldCellAccInfo(WrCellCb  *cellCb,NhuDatReqSdus  *nhDatReqEvntSdu,
          NhuSysInfoBlockTyp1cellAccessRelatedInfo  *cellAcc);

EXTERN S16 wrEmmGetNeighUtraSIInfo
(
WrUmmRedirectInfo       *redirectInfo
);

EXTERN S16 wrIncKpiCsfbWithSi
(
U8                      redirType
);

EXTERN WrNrUtranFreqCb* wrEmmAnrGetFreqNode
(
U16                     arfcnDl,
WrNrUtranCb             *utranCb
);

PRIVATE U16 wrUmmRrcGetCfgPollRetxTmr2
(
  U32                          pollRetxTmr
);

PRIVATE U16 wrUmmRrcGetCfgStatusProhTmr2
(
  U32                          statusProhTmr
);

PRIVATE U16 wrUmmRrcGetCfgReorderingTmr2
(
  U32                          reorderingTmr
);


/**
 *  @brief This function is used to fill the MAC Main Config IE in the
 *         RRC Message
 *         
 *      Function: wrUmmRrcFillMacCfgIe
 *
 *          Processing steps:
 *          - Fill the IE elements
 *
 *
 *  @param [in/out] macCfg : MAC Main Config IE
 *  @return S16
 */
PUBLIC S16 wrUmmRrcFillMacCfgIe
(
NhuMAC_MainConfig            *macCfg,
WrUeCb                       *ueCb
)
{
   /* MAC-MainConfig  */
   WR_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

   /* ul-SCH-Config */
   WR_FILL_TKN_UINT(macCfg->ul_SCH_Config.pres, PRSNT_NODEF);
   macCfg->ul_SCH_Config.maxHARQ_Tx.pres = PRSNT_NODEF;
   macCfg->ul_SCH_Config.maxHARQ_Tx.val  = ueCb->ueUlHqCfg.maxUlHqTx; 
#ifdef WR_RSYS_OAM
   WR_FILL_TKN_UINT(macCfg->ul_SCH_Config.periodicBSR_Timer,
         ueCb->uePerBsrTmrCfg.prdBsrTmr);
   WR_FILL_TKN_UINT(macCfg->ul_SCH_Config.retxBSR_Timer,
         ueCb->uePerBsrTmrCfg.retxBsrTmr);
#else
   WR_FILL_TKN_UINT(macCfg->ul_SCH_Config.periodicBSR_Timer,
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.periodicBSR_Timer);
   WR_FILL_TKN_UINT(macCfg->ul_SCH_Config.retxBSR_Timer,
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.retxBSR_Timer);
#endif
   WR_FILL_TKN_UINT(macCfg->ul_SCH_Config.ttiBundling,
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.ttiBundling);

   /* The cell wide broadcast parameter sets the value at each UE */
   WR_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated,
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.timeAlignmentTimerDedicated);

   /* phr-Config */ 
   WR_FILL_TKN_UINT(macCfg->phr_Config.choice, PHR_CONFIG_SETUP);
   WR_FILL_TKN_UINT(macCfg->phr_Config.val.setup.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(macCfg->phr_Config.val.setup.periodicPHR_Timer,\
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.phr_Config.periodicPHR_Timer);
   WR_FILL_TKN_UINT(macCfg->phr_Config.val.setup.prohibitPHR_Timer,\
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.phr_Config.prohibitPHR_Timer);
   WR_FILL_TKN_UINT(macCfg->phr_Config.val.setup.dl_PathlossChange,\
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.phr_Config.dl_PathlossChange);

   RETVALUE(ROK);
}
/** @brief This function is used to Initized Nhu Measuremnt object 
 *
 * @details
 *
 *     Function: wrUmmInitNhuMeasCfg
 *
 *         Processing steps:
 *         - Initialize  measurement configuration
 *
 * @param[in] measCfg  : Measurement configuration
 * @return Void
 */
PRIVATE Void wrUmmInitNhuMeasCfg
(
  NhuMeasConfig                *measCfg
  )
{
   measCfg->measObjectToRemoveLst.noComp.pres = NOTPRSNT;
   measCfg->reportConfigToRemoveLst.noComp.pres = NOTPRSNT;
   measCfg->measIdToRemoveLst.noComp.pres = NOTPRSNT;
   measCfg->quantityConfig.pres.pres = NOTPRSNT;
   measCfg->measGapConfig.choice.pres = NOTPRSNT;
   measCfg->preRegistrationInfoHRPD.pres.pres = NOTPRSNT;
   measCfg->speedStatePars.choice.pres = NOTPRSNT;
   measCfg->s_Measure.pres = NOTPRSNT;
}
/**
 *  @brief This function returns the MAX retransmission Threshold value for 
 *         the given RLC max threshold enum
 *         
 *      Function: wrUmmRrcGetCfgMaxRetxThrsh
 *
 *          Processing steps:
 *          - switch to the given RLC max threshold enum
 *          - return the max retransmission  Threshold value
 *
 *
 *  @param [in] maxRetxThrsh : RLC max threshold enum
 *  @return U16
 */
PRIVATE U16 wrUmmRrcGetCfgMaxRetxThrsh
(
U32                          maxRetxThrsh
)
{
   switch(maxRetxThrsh)
   {
      case NhuUL_AM_RLCmaxRetxThresholdt1Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T1);
      case NhuUL_AM_RLCmaxRetxThresholdt2Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T2);
      case NhuUL_AM_RLCmaxRetxThresholdt3Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T3);
      case NhuUL_AM_RLCmaxRetxThresholdt4Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T4);
      case NhuUL_AM_RLCmaxRetxThresholdt6Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T6);
      case NhuUL_AM_RLCmaxRetxThresholdt8Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T8);
      case NhuUL_AM_RLCmaxRetxThresholdt16Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T16);
      case NhuUL_AM_RLCmaxRetxThresholdt32Enum:
         RETVALUE(NHU_MAX_RETX_THRES_T32);
      default:
         RETVALUE(NHU_MAX_RETX_THRES_T32);
   }
}

/**
 *  @brief This function returns the Poll Byte value for 
 *         the given Poll Byte enum
 *         
 *      Function: wrUmmRrcGetCfgPollByte
 *
 *          Processing steps:
 *          - switch to the given Poll Byte
 *          - return the Poll Byte value
 *
 *
 *  @param [in] pollByte : Poll Byte enum
 *  @return U16
 */

PRIVATE U16 wrUmmRrcGetCfgPollByte
(
U32                          pollByte
)
{
   switch(pollByte)
   {
      case NhuPollBytekB25Enum:
         RETVALUE(NHU_POLL_BYTE_KB25);
      case NhuPollBytekB50Enum:
         RETVALUE(NHU_POLL_BYTE_KB50);
      case NhuPollBytekB75Enum:
         RETVALUE(NHU_POLL_BYTE_KB75);
      case NhuPollBytekB100Enum:
         RETVALUE(NHU_POLL_BYTE_KB100);
      case NhuPollBytekB125Enum:
         RETVALUE(NHU_POLL_BYTE_KB125);
      case NhuPollBytekB250Enum:
         RETVALUE(NHU_POLL_BYTE_KB250);
      case NhuPollBytekB375Enum:
         RETVALUE(NHU_POLL_BYTE_KB375);
      case NhuPollBytekB500Enum:
         RETVALUE(NHU_POLL_BYTE_KB500);
      case NhuPollBytekB750Enum:
         RETVALUE(NHU_POLL_BYTE_KB750);
      case NhuPollBytekB1000Enum:
         RETVALUE(NHU_POLL_BYTE_KB1000);
      case NhuPollBytekB1250Enum:
         RETVALUE(NHU_POLL_BYTE_KB1250);
      case NhuPollBytekB1500Enum:
         RETVALUE(NHU_POLL_BYTE_KB1500);
      case NhuPollBytekB2000Enum:
         RETVALUE(NHU_POLL_BYTE_KB2000);
      case NhuPollBytekB3000Enum:
         RETVALUE(NHU_POLL_BYTE_KB3000);
      case NhuPollBytekBinfinityEnum:
         RETVALUE(NHU_POLL_BYTE_INF);
      default:
         RETVALUE(NHU_POLL_BYTE_INF);
   }
}

/**
 *  @brief This function returns the Poll PDU count for the enum provided
 *
 *      Function: wrUmmRrcGetCfgPollPdu
 *
 *          Processing steps:
 *          - switch to the given Poll PDU enum
 *          - return the Poll Pdu count
 *
 *
 *  @param [in] pollPdu
 *  @return U16
 */

PRIVATE U16 wrUmmRrcGetCfgPollPdu
(
U32                          pollPdu
)
{
   switch(pollPdu)
   {
      case NhuPollPDUp4Enum:
         RETVALUE(NHU_POLL_PDU_P4);
      case NhuPollPDUp8Enum:
         RETVALUE(NHU_POLL_PDU_P8);
      case NhuPollPDUp16Enum:
         RETVALUE(NHU_POLL_PDU_P16);
      case NhuPollPDUp32Enum:
         RETVALUE(NHU_POLL_PDU_P32);
      case NhuPollPDUp64Enum:
         /* TODO: Need to check why 64 is not defined in RRC */
         RETVALUE(NHU_POLL_PDU_P128);
      case NhuPollPDUp128Enum:
         RETVALUE(NHU_POLL_PDU_P128);
      case NhuPollPDUp256Enum:
         RETVALUE(NHU_POLL_PDU_P256);
      case NhuPollPDUpInfinityEnum:
         RETVALUE(NHU_POLL_PDU_INF);
      default:
         RETVALUE(NHU_POLL_PDU_INF);
   }
}

/**
 *  @brief This function returns th retransmission timer value for the enum
 *
 *      Function:  wrUmmRrcGetCfgPollRetxTmr
 *
 *          Processing steps:
 *          - switch to the given poll retransmission timer enum
 *          - return the poll retransmission timer value
 *
 *  @param [in] pollRetxTmr : Enum for Poll retransmission timer value
 *  @return U16
 */

PRIVATE U16 wrUmmRrcGetCfgPollRetxTmr
(
U32                          pollRetxTmr
)
{
    U16 ret;
   switch(pollRetxTmr)
   {
      case NhuT_PollRetransmitms5Enum:
         RETVALUE(NHU_TPOLL_RETX_MS5);
      case NhuT_PollRetransmitms10Enum:
         RETVALUE(NHU_TPOLL_RETX_MS10);
      case NhuT_PollRetransmitms15Enum:
         RETVALUE(NHU_TPOLL_RETX_MS15);
      case NhuT_PollRetransmitms20Enum:
         RETVALUE(NHU_TPOLL_RETX_MS20);
      case NhuT_PollRetransmitms25Enum:
         RETVALUE(NHU_TPOLL_RETX_MS25);
      case NhuT_PollRetransmitms30Enum:
         RETVALUE(NHU_TPOLL_RETX_MS30);
      case NhuT_PollRetransmitms35Enum:
         RETVALUE(NHU_TPOLL_RETX_MS35);
      case NhuT_PollRetransmitms40Enum:
         RETVALUE(NHU_TPOLL_RETX_MS40);
      case NhuT_PollRetransmitms45Enum:
         RETVALUE(NHU_TPOLL_RETX_MS45);
      case NhuT_PollRetransmitms50Enum:
         RETVALUE(NHU_TPOLL_RETX_MS50);
      case NhuT_PollRetransmitms55Enum:
         RETVALUE(NHU_TPOLL_RETX_MS55);
      case NhuT_PollRetransmitms60Enum:
         RETVALUE(NHU_TPOLL_RETX_MS60);
      case NhuT_PollRetransmitms65Enum:
         RETVALUE(NHU_TPOLL_RETX_MS65);
      case NhuT_PollRetransmitms70Enum:
         RETVALUE(NHU_TPOLL_RETX_MS70);
      case NhuT_PollRetransmitms75Enum:
         RETVALUE(NHU_TPOLL_RETX_MS75);
      case NhuT_PollRetransmitms80Enum:
         RETVALUE(NHU_TPOLL_RETX_MS80);
      case NhuT_PollRetransmitms85Enum:
         RETVALUE(NHU_TPOLL_RETX_MS85);
      case NhuT_PollRetransmitms90Enum:
         RETVALUE(NHU_TPOLL_RETX_MS90);
      case NhuT_PollRetransmitms95Enum:
         RETVALUE(NHU_TPOLL_RETX_MS95);
      case NhuT_PollRetransmitms100Enum:
         RETVALUE(NHU_TPOLL_RETX_MS100);
      case NhuT_PollRetransmitms105Enum:
         RETVALUE(NHU_TPOLL_RETX_MS105);
      case NhuT_PollRetransmitms110Enum:
         RETVALUE(NHU_TPOLL_RETX_MS110);
      case NhuT_PollRetransmitms115Enum:
         RETVALUE(NHU_TPOLL_RETX_MS115);
      case NhuT_PollRetransmitms120Enum:
         RETVALUE(NHU_TPOLL_RETX_MS120);
      case NhuT_PollRetransmitms125Enum:
         RETVALUE(NHU_TPOLL_RETX_MS125);
      case NhuT_PollRetransmitms130Enum:
         RETVALUE(NHU_TPOLL_RETX_MS130);
      case NhuT_PollRetransmitms135Enum:
         RETVALUE(NHU_TPOLL_RETX_MS135);
        default:
            ret = wrUmmRrcGetCfgPollRetxTmr2(pollRetxTmr);
            RETVALUE(ret);

    }
}

/**
 *  @brief This function returns the Status Prohibit timer value for the enum
 *
 *      Function:  wrUmmRrcGetCfgStatusProhTmr
 *
 *          Processing steps:
 *          - switch to the given Status Prohibit timer enum
 *          - return the Status Prohibit timer value
 *
 *  @param [in] statusProhTmr : Status Prohibit timer enum
 *  @return U16
 */

PRIVATE U16 wrUmmRrcGetCfgStatusProhTmr
(
U32                          statusProhTmr
)
{
    U16 ret;
   switch(statusProhTmr)
   {
      case NhuT_StatusProhibitms0Enum:
         RETVALUE(NHU_STATUS_PROH_MS5);
      case NhuT_StatusProhibitms5Enum:
         RETVALUE(NHU_STATUS_PROH_MS5);
      case NhuT_StatusProhibitms10Enum:
         RETVALUE(NHU_STATUS_PROH_MS10);
      case NhuT_StatusProhibitms15Enum:
         RETVALUE(NHU_STATUS_PROH_MS15);
      case NhuT_StatusProhibitms20Enum:
         RETVALUE(NHU_STATUS_PROH_MS20);
      case NhuT_StatusProhibitms25Enum:
         RETVALUE(NHU_STATUS_PROH_MS25);
      case NhuT_StatusProhibitms30Enum:
         RETVALUE(NHU_STATUS_PROH_MS30);
      case NhuT_StatusProhibitms35Enum:
         RETVALUE(NHU_STATUS_PROH_MS35);
      case NhuT_StatusProhibitms40Enum:
         RETVALUE(NHU_STATUS_PROH_MS40);
      case NhuT_StatusProhibitms45Enum:
         RETVALUE(NHU_STATUS_PROH_MS45);
      case NhuT_StatusProhibitms50Enum:
         RETVALUE(NHU_STATUS_PROH_MS50);
      case NhuT_StatusProhibitms55Enum:
         RETVALUE(NHU_STATUS_PROH_MS55);
      case NhuT_StatusProhibitms60Enum:
         RETVALUE(NHU_STATUS_PROH_MS60);
      case NhuT_StatusProhibitms65Enum:
         RETVALUE(NHU_STATUS_PROH_MS65);
      case NhuT_StatusProhibitms70Enum:
         RETVALUE(NHU_STATUS_PROH_MS70);
      case NhuT_StatusProhibitms75Enum:
         RETVALUE(NHU_STATUS_PROH_MS75);
      case NhuT_StatusProhibitms80Enum:
         RETVALUE(NHU_STATUS_PROH_MS80);
      case NhuT_StatusProhibitms85Enum:
         RETVALUE(NHU_STATUS_PROH_MS85);
      case NhuT_StatusProhibitms90Enum:
         RETVALUE(NHU_STATUS_PROH_MS90);
      case NhuT_StatusProhibitms95Enum:
         RETVALUE(NHU_STATUS_PROH_MS95);
      case NhuT_StatusProhibitms100Enum:
         RETVALUE(NHU_STATUS_PROH_MS100);
      case NhuT_StatusProhibitms105Enum:
         RETVALUE(NHU_STATUS_PROH_MS105);
      case NhuT_StatusProhibitms110Enum:
         RETVALUE(NHU_STATUS_PROH_MS110);
      case NhuT_StatusProhibitms115Enum:
         RETVALUE(NHU_STATUS_PROH_MS115);
      case NhuT_StatusProhibitms120Enum:
         RETVALUE(NHU_STATUS_PROH_MS120);
      case NhuT_StatusProhibitms125Enum:
         RETVALUE(NHU_STATUS_PROH_MS125);
      case NhuT_StatusProhibitms130Enum:
         RETVALUE(NHU_STATUS_PROH_MS130);
      case NhuT_StatusProhibitms135Enum:
         RETVALUE(NHU_STATUS_PROH_MS135);
      case NhuT_StatusProhibitms140Enum:
         RETVALUE(NHU_STATUS_PROH_MS140);
        default:
            ret = wrUmmRrcGetCfgStatusProhTmr2(statusProhTmr);
            RETVALUE(ret);

   }
}

/**
 *  @brief This function returns the PDCP discard timer  value for the enum
 *
 *      Function:  wrUmmGetCfgPdcpDiscardTmrVal 
 *
 *          Processing steps:
 *          - switch to the given PDCP discard Timer enum
 *          - return the Discard Timer value NHU eNum
 *
 *  @param [in] discardTmr : Discard timer enum
 *  @return NhuPdcpDiscardTmr
 */
PRIVATE NhuPdcpDiscardTmr  wrUmmGetCfgPdcpDiscardTmrVal 
(
U32                          discardTmr
)
{
   switch(discardTmr)
   {
      case NhuPDCP_ConfigdiscardTimerms50Enum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MS50);
      case NhuPDCP_ConfigdiscardTimerms100Enum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MS100);
      case NhuPDCP_ConfigdiscardTimerms150Enum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MS150);
      case NhuPDCP_ConfigdiscardTimerms300Enum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MS300);
      case NhuPDCP_ConfigdiscardTimerms500Enum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MS500);
      case NhuPDCP_ConfigdiscardTimerms1500Enum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MS1500);
      case NhuPDCP_ConfigdiscardTimerinfinityEnum:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MSINF);
      default:
         RETVALUE(NHU_PDCP_DISCARD_TMR_MSINF);
   }
}
/**
 *  @brief This function returns the SN Field length value for the enum
 *
 *      Function:  wrUmmGetCfgPdcpSnLen 
 *
 *          Processing steps:
 *          - switch to the given SN Filed Length enum
 *          - return the SN Field Length  value
 *
 *  @param [in] snLen : SN Field Length enum
 *  @return U16
 */
PRIVATE U16 wrUmmGetCfgPdcpSnLen 
(
U32                          snLen
)
{
   switch(snLen)
   {
      case NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum:
         RETVALUE(NHU_PDCP_SN_SIZE_7BITS);
      case NhuPDCP_Configrlc_UMpdcp_SN_Sizelen12bitsEnum:
         RETVALUE(NHU_PDCP_SN_SIZE_12BITS);
      default:
         RETVALUE(NHU_PDCP_SN_SIZE_12BITS);
   }
}

/**
 *  @brief This function returns the Reordering timer value for the enum
 *
 *      Function:  wrUmmRrcGetCfgSnLen
 *
 *          Processing steps:
 *          - switch to the given SN Filed Length enum
 *          - return the SN Field Length timer value
 *
 *  @param [in] snLen : SN Field Length enum
 *  @return U16
 */
PRIVATE U16 wrUmmRrcGetCfgSnLen
(
U8                          snLen
)
{
   switch(snLen)
   {
      case NhuSN_FieldLengthsize5Enum:
/* MURTUZA : changes for split arch */
         RETVALUE(1);
      case NhuSN_FieldLengthsize10Enum:
         RETVALUE(2);
      default:
         RETVALUE(2);
   }
}

/**
 *  @brief This function returns the Reordering timer value for the enum
 *
 *      Function:  wrUmmRrcGetCfgReorderingTmr
 *
 *          Processing steps:
 *          - switch to the given Reordering timer timer enum
 *          - return the Reordering timer timer value
 *
 *  @param [in] reorderingTmr : Reordering timer timer enum
 *  @return U16
 */

PRIVATE U16 wrUmmRrcGetCfgReorderingTmr
(
U32                          reorderingTmr
)
{
    U16 ret;
   switch(reorderingTmr)
   {
      case NhuT_Reorderingms0Enum:
         RETVALUE(NHU_TREORD_MS0);
      case NhuT_Reorderingms5Enum:
         RETVALUE(NHU_TREORD_MS5);
      case NhuT_Reorderingms10Enum:
         RETVALUE(NHU_TREORD_MS10);
      case NhuT_Reorderingms15Enum:
         RETVALUE(NHU_TREORD_MS15);
      case NhuT_Reorderingms20Enum:
         RETVALUE(NHU_TREORD_MS20);
      case NhuT_Reorderingms25Enum:
         RETVALUE(NHU_TREORD_MS25);
      case NhuT_Reorderingms30Enum:
         RETVALUE(NHU_TREORD_MS30);
      case NhuT_Reorderingms35Enum:
         RETVALUE(NHU_TREORD_MS35);
      case NhuT_Reorderingms40Enum:
         RETVALUE(NHU_TREORD_MS40);
      case NhuT_Reorderingms45Enum:
         RETVALUE(NHU_TREORD_MS45);
      case NhuT_Reorderingms50Enum:
         RETVALUE(NHU_TREORD_MS50);
      case NhuT_Reorderingms55Enum:
         RETVALUE(NHU_TREORD_MS55);
      case NhuT_Reorderingms60Enum:
         RETVALUE(NHU_TREORD_MS60);
      case NhuT_Reorderingms65Enum:
         RETVALUE(NHU_TREORD_MS65);
      case NhuT_Reorderingms70Enum:
         RETVALUE(NHU_TREORD_MS70);
      case NhuT_Reorderingms75Enum:
         RETVALUE(NHU_TREORD_MS75);
      case NhuT_Reorderingms80Enum:
         RETVALUE(NHU_TREORD_MS80);
      case NhuT_Reorderingms85Enum:
         RETVALUE(NHU_TREORD_MS85);
      case NhuT_Reorderingms90Enum:
         RETVALUE(NHU_TREORD_MS90);
      case NhuT_Reorderingms95Enum:
         RETVALUE(NHU_TREORD_MS95);
      case NhuT_Reorderingms100Enum:
         RETVALUE(NHU_TREORD_MS100);
      case NhuT_Reorderingms110Enum:
         RETVALUE(NHU_TREORD_MS110);
      case NhuT_Reorderingms120Enum:
         RETVALUE(NHU_TREORD_MS120);
      case NhuT_Reorderingms130Enum:
         RETVALUE(NHU_TREORD_MS130);
        default:
            ret = wrUmmRrcGetCfgReorderingTmr2(reorderingTmr);
            RETVALUE(ret);

    }
}

/**
 *  @brief This function fill the UL RLC config info for DRB config
 *
 *      Function: wrUmmRrcFillUlRbRlcCfg
 *
 *          Processing steps:
 *          - update the UL RLC cfg info
 *
 *
 *  @param [out] ulRlcMode : pointer to DRB RLC info, to be updated
 *  @param [in] cfgParams : pointer to cgf params containing the RLC info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PRIVATE S16 wrUmmRrcFillUlRbRlcCfg
(
NhuUlRlcMode                 *ulRlcMode,
WrRabCfgParams               *cfgParams,
Bool                         isUlRbSpsEnabled
)
{
   ulRlcMode->rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
   if (cfgParams->rlcMode == WR_RLC_AM_MODE)
   {
      WrRlcAmModeUl          *ul = &cfgParams->rlcParams.u.am.ul;
      NhuUlAmRlcModeInfo     *amRlc = &(ulRlcMode->m.amRlc);

      ulRlcMode->mode    = CM_LTE_MODE_AM;
      amRlc->tStatusProh = wrUmmRrcGetCfgStatusProhTmr(ul->statusProhTmr);
      if (isUlRbSpsEnabled == TRUE)
      {
         amRlc->tReordring  = wrUmmRrcGetCfgReorderingTmr(NhuT_Reorderingms20Enum);
      }
      else
      {
         amRlc->tReordring  = wrUmmRrcGetCfgReorderingTmr(ul->reorderingTmr);
      }
   }
   else if (cfgParams->rlcMode == WR_RLC_UM_MODE) 
   {
      ulRlcMode->mode               = CM_LTE_MODE_UM;
      WrRlcUmModeUl          *ul = &cfgParams->rlcParams.u.um.ul;
      ulRlcMode->m.umRlc.snFieldLen = wrUmmRrcGetCfgSnLen(ul->snFldLen);
      if (isUlRbSpsEnabled == TRUE)
      {
         ulRlcMode->m.umRlc.tReordring = wrUmmRrcGetCfgReorderingTmr(NhuT_Reorderingms20Enum);
      }
      else
      {
         ulRlcMode->m.umRlc.tReordring = wrUmmRrcGetCfgReorderingTmr(ul->tReordering);
      }
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function fill the DL RLC config info for DRB config
 *
 *      Function: wrUmmRrcFillDlRbRlcCfg
 *
 *          Processing steps:
 *          - update the DL RLC cfg info
 *
 *
 *  @param [out] dlRlcMode : pointer to DRB RLC info, to be updated
 *  @param [in]  cfgParams : pointer to cgf params containing the RLC info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */


PRIVATE S16 wrUmmRrcFillDlRbRlcCfg
(
NhuDlRlcMode                 *dlRlcMode,
WrRabCfgParams               *cfgParams
)
{
   dlRlcMode->rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
   if (cfgParams->rlcMode == WR_RLC_AM_MODE)
   {
      WrRlcAmModeDl          *dl = &cfgParams->rlcParams.u.am.dl;
      NhuDlAmRlcModeInfo     *amRlc = &(dlRlcMode->m.amRlc);

      dlRlcMode->mode      = CM_LTE_MODE_AM;
      amRlc->maxRetxThresh = wrUmmRrcGetCfgMaxRetxThrsh(dl->maxRetxThrsh);
      amRlc->pollByte      = wrUmmRrcGetCfgPollByte(dl->pollByte);
      amRlc->pollPdu       = wrUmmRrcGetCfgPollPdu(dl->pollPdu);
      amRlc->tPollRetx     = wrUmmRrcGetCfgPollRetxTmr(dl->pollRetxTmr);
   }
   else
   {
      WrRlcUmModeDl          *dl = &cfgParams->rlcParams.u.um.dl;

      dlRlcMode->mode       = CM_LTE_MODE_UM;
      dlRlcMode->m.umRlc.snFieldLen = wrUmmRrcGetCfgSnLen(dl->snFldLen);

   }

   RETVALUE(ROK);
}


/**
 *  @brief This function fill the PDCP config info for DRB config
 *
 *      Function: wrUmmRrcFillUlRbPdcpCfg
 *
 *          Processing steps:
 *          - update the Pdcp cfg info
 *
 *
 *  @param [out] pdcpCfg : pointer to DRB PDCP info, to be updated
 *  @param [in]  pdcpParams : pointer to cgf params containing the PDCP info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */


PRIVATE S16 wrUmmRrcFillRbPdcpCfg
(
NhuPdcpCfg                   *pdcpCfg,
WrPdcpCfgParams              *pdcpParams
)
{
   pdcpCfg->pdcpCfgBitMsk = 0;
   if(pdcpParams->discardTimer <= NhuPDCP_ConfigdiscardTimerinfinityEnum)
   {
      pdcpCfg->pdcpCfgBitMsk |= NHU_PDCP_DISCARD_TMR;
      pdcpCfg->discardTmr = wrUmmGetCfgPdcpDiscardTmrVal(pdcpParams->discardTimer);
   }
   else
   {
        pdcpCfg->pdcpCfgBitMsk &= ~NHU_PDCP_DISCARD_TMR;
   }
   if(pdcpParams->statusRprtReqd)
   {
      pdcpCfg->pdcpCfgBitMsk |= NHU_PDCP_STATUS_RPT;
      pdcpCfg->statusRptReq   = pdcpParams->statusRprtReqd;
   }

   pdcpCfg->cfmReqd = pdcpParams->cfmReqrd;
   pdcpCfg->pdcpCfgBitMsk |= NHU_PDCP_CFM_REQD;
   RETVALUE(ROK);
}

/**
 *  @brief This function builds the DRB Mod config Info
 *
 *      Function: wrUmmRrcModDrbCfg 
 *
 *          Processing steps:
 *          - Into the given drbCfg pointer
 *          - update the DL RLC cfg info
 *          - update the UL RLC cfg info
 *          - update the PDCP cfg info
 *
 *
 *  @param [out] drbCfg  : pointer for the updating the DRB config Info
 *  @param [in]  drbId   : DRB ID
 *  @param [in]  lcId    : logical channel ID
 *  @param [in]  qci     : QCI of the bearer
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcModDrbCfg 
(
U16                          cellId,
NhuRbCfgInfo                 *drbCfg,
U8                           drbId,
U8                           lcId,
U8                           qci,
Bool                         isUlSpsEnabled
)
{
   WrRabCfgParams            *params;
   WrPdcpCfgParams           *pdcp; 
   WrRlcCfgParams            *rlc; 
   WrUmmCellCb               *ummCellCb = NULLP;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);

   }

   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);

   /* DRB related inforation that is set at RLC and PDCP layers           */
   drbCfg->rbCfgBitMsk                 = (NHU_RB_CFG_UL | NHU_RB_CFG_DL); 
   drbCfg->rbId.rbId                   = drbId;
   drbCfg->rbId.rbTyp                  = CM_LTE_DRB;
   drbCfg->lcId.lcId                   = lcId;
   drbCfg->lcId.logChType              = (CmLteLcType)CM_LTE_LCH_DTCH;
   
   /* Fix for the CR ccpu00128224 : Updating with correct qci for L2_MEAS */
#ifdef LTE_L2_MEAS
   drbCfg->qci                         = qci;
#endif

   /* UL related configuration - logical channel mapping and RLC info     */
   drbCfg->ulRbCfg.ulRbBitMsk         = NHU_RB_CFG_RLC_MODE_UL;
   drbCfg->ulRbCfg.ulRbBitMsk        |= (NHU_RB_CFG_LOGCH_MAP_UL);
   drbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;
   drbCfg->ulRbCfg.ulLogChMap.lcCfgType = NHU_CFG_TYPE_EXPLICIT;
   drbCfg->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres = NHU_UL_LOGCH_MAP_GRP;
   if (isUlSpsEnabled == TRUE)
   {
      /* LCG group 1 assigned for UL SPS */
      drbCfg->ulRbCfg.ulLogChMap.lcCfg.logChGrp = 1;
   }
   else
   {
      drbCfg->ulRbCfg.ulLogChMap.lcCfg.logChGrp = params->lchParams.lchGrp;
   }
   wrUmmRrcFillUlRbRlcCfg(&(drbCfg->ulRbCfg.ulRlcMode), params, isUlSpsEnabled);

   /* DL related configuration - logical channel mapping and RLC info     */
   drbCfg->dlRbCfg.dlRbBitMsk         = NHU_RB_CFG_RLC_MODE_DL;
   drbCfg->dlRbCfg.dlRbBitMsk        |= (NHU_RB_CFG_LOGCH_MAP_DL); 
   drbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
   wrUmmRrcFillDlRbRlcCfg(&(drbCfg->dlRbCfg.dlRlcMode), params);

   /* PDCP Cfg */
   drbCfg->rbCfgBitMsk                |= (NHU_RB_CFG_PDCP_INFO);
   wrUmmRrcFillRbPdcpCfg(&(drbCfg->pdcpCfg), pdcp);
   if(params->rlcMode == WR_RLC_UM_MODE)
   {
      drbCfg->pdcpCfg.pdcpCfgBitMsk |= NHU_PDCP_SN_SIZE;
      drbCfg->pdcpCfg.snSize         = wrUmmGetCfgPdcpSnLen(pdcp->pdcpSnSize);
   }
   if(params->rlcMode == WR_RLC_AM_MODE)
   {
      drbCfg->pdcpCfg.pdcpCfgBitMsk |= NHU_PDCP_SN_SIZE;
      drbCfg->pdcpCfg.snSize         = NHU_PDCP_SN_SIZE_12BITS;
   }
 
   RETVALUE(ROK);
}


/**
 *  @brief This function builds the DRB config Info
 *
 *      Function: wrUmmRrcAddDrbCfg
 *
 *          Processing steps:
 *          - Into the given drbCfg pointer
 *          - update the logical channel info
 *          - update the DL RLC cfg info
 *          - update the UL RLC cfg info
 *          - update the PDCP cfg info
 *
 *
 *  @param [out] drbCfg  : pointer for the updating the DRB config Info
 *  @param [in]  drbId   : DRB ID
 *  @param [in]  lcId    : logical channel ID
 *  @param [in]  qci     : QCI of the bearer
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcAddDrbCfg
(
U16                          cellId,
NhuRbCfgInfo                 *drbCfg,
U8                           drbId,
U8                           lcId,
U8                           qci,
/* SPS changes starts */
Bool                         isUlSpsEnabled
/* SPS changes starts */
)
{
   WrRabCfgParams            *params;
   WrPdcpCfgParams           *pdcp; 
   WrRlcCfgParams            *rlc; 
   WrUmmCellCb               *ummCellCb = NULLP;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);

   }

   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);

   /* DRB related inforation that is set at RLC and PDCP layers           */
   drbCfg->rbCfgBitMsk                 = (NHU_RB_CFG_UL | NHU_RB_CFG_DL); 
   drbCfg->rbId.rbId                   = drbId;
   drbCfg->rbId.rbTyp                  = CM_LTE_DRB;
   drbCfg->lcId.lcId                   = lcId;
   drbCfg->lcId.logChType              = (CmLteLcType)CM_LTE_LCH_DTCH;

   /* Fix for the CR ccpu00128243 : Updating with correct qci for L2_MEAS */
#ifdef LTE_L2_MEAS
   drbCfg->qci                         = qci;
#endif

   /* UL related configuration - logical channel mapping and RLC info     */
   drbCfg->ulRbCfg.ulRbBitMsk          = (NHU_RB_CFG_LOGCH_MAP_UL);
   drbCfg->ulRbCfg.ulRbBitMsk         |= (NHU_RB_CFG_RLC_MODE_UL);
   drbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;
   drbCfg->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres = NHU_UL_LOGCH_MAP_GRP;
   drbCfg->ulRbCfg.ulLogChMap.lcCfgType = NHU_CFG_TYPE_EXPLICIT;
   /* SPS changes starts */
   if (isUlSpsEnabled == TRUE)
   {
      /* LCG group 1 assigned for UL SPS */
      drbCfg->ulRbCfg.ulLogChMap.lcCfg.logChGrp = 1;
   }
   else
   {
      drbCfg->ulRbCfg.ulLogChMap.lcCfg.logChGrp = params->lchParams.lchGrp;
   }
   /* SPS changes ends */
   wrUmmRrcFillUlRbRlcCfg(&(drbCfg->ulRbCfg.ulRlcMode), params, isUlSpsEnabled);

   /* DL related configuration - logical channel mapping and RLC info     */
   drbCfg->dlRbCfg.dlRbBitMsk          = (NHU_RB_CFG_LOGCH_MAP_DL); 
   drbCfg->dlRbCfg.dlRbBitMsk         |= (NHU_RB_CFG_RLC_MODE_DL);
   drbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
   wrUmmRrcFillDlRbRlcCfg(&(drbCfg->dlRbCfg.dlRlcMode), params);

   /* PDCP Cfg */
   drbCfg->rbCfgBitMsk                |= (NHU_RB_CFG_PDCP_INFO);
   wrUmmRrcFillRbPdcpCfg(&(drbCfg->pdcpCfg), pdcp);
   if(params->rlcMode == WR_RLC_UM_MODE)
   {
      drbCfg->pdcpCfg.pdcpCfgBitMsk |= NHU_PDCP_SN_SIZE;
      drbCfg->pdcpCfg.snSize         = wrUmmGetCfgPdcpSnLen(pdcp->pdcpSnSize);
   }
   if(params->rlcMode == WR_RLC_AM_MODE)
   {
      drbCfg->pdcpCfg.pdcpCfgBitMsk |= NHU_PDCP_SN_SIZE;
      drbCfg->pdcpCfg.snSize         = NHU_PDCP_SN_SIZE_12BITS;
   }
   RETVALUE(ROK);
}

/**
 * @brief This function updates the SRB infoin the UE Control Block
 *
 * @details
 *    Function: wrUmmRrcFillUeSrbInfo
 *
 *    Processing steps:
 *       - set the srb parameters to the input value
 *
 *  @param [in]  srbCfg     : SRB Information
 *  @param [in]  rbId       : RB ID
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcFillUeSrbInfo
(
WrUeRBInfo *srbCfg,
U8         rbId
)
{
   U8 idx = 0;
   
   TRC2(wrUmmRrcFillUeSrbInfo);

   idx = srbCfg->numOfSrbCfgd++;
   srbCfg->srb[idx].inUse  = TRUE;
   srbCfg->srb[idx].rbType = CM_LTE_SRB;
   srbCfg->srb[idx].rbId   = rbId;
   srbCfg->srb[idx].pdcpId = rbId;

   RETVALUE(ROK);

} /* wrUmmRrcFillUeSrbInfo */

/**
 *  @brief This function fills Physical Configuration for UE
 *
 *      Function: wrFillRadResPhyCfgDed
 *
 *          Processing steps:
 *          - fill the PDSCH dedicated info for the UE
 *
 *  @param [out] radioResCfg : pointer to radio resource config dedication info
 *  @param [in]  evnt        : pointer to NHU stucture
 *  @param [in]  rbId        : Radio bearer ID
 *  @param [in]  ueCb        : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrFillRadResPhyCfgDed
(
NhuRadioResourceConfigDedicated *radioResCfg,
Ptr                             evnt,
U8                              rbId,
WrUeCb                          *ueCb
)
{
   NhuPDSCH_ConfigDedicated        *pdschCfg = NULLP;
   NhuPUCCH_ConfigDedicated        *pucchCfg = NULLP;
   NhuPUSCH_ConfigDedicated        *puschCfg = NULLP;
   NhuUlnkPowerCntrlDedicated      *uplnkPwrCntrl = NULLP;
   NhuTPC_PDCCH_Config             *tpcPdcchCfgPucch = NULLP;
   NhuTPC_PDCCH_Config             *tpcPdcchCfgPusch = NULLP;
   NhuCQI_ReportConfig             *cqiRprtng = NULLP;
   NhuSoundingRS_UL_ConfigDedicated *soundngRsUlCfg = NULLP;
   NhuSchedulingRqstConfig          *schedulingRqstConfig = NULLP;
   NhuPhysicalConfigDedicatedantennaInfo *antennaInfo = NULLP;
   NhuCQI_ReportPeriodicsetup       *cqiRepPerSetup = NULLP;
   Data                             *pucchTpcRnti =  NULLP;
   Data                             *puschTpcRnti =  NULLP;
   U16                              duplexMode;

   TRC2(wrFillRadResPhyCfgDed);

   RLOG0(L_DEBUG, "Filling Dedicated Phy Cfg");

   WR_FILL_TKN_UINT(radioResCfg->physicalConfigDedicated.pres,PRSNT_NODEF);

   if(rbId == WR_PDCP_ID_SRB1)
   {
      /* PDSCH Configuration */
      pdschCfg = &(radioResCfg->physicalConfigDedicated.\
            pdsch_ConfigDedicated);

      WR_FILL_TKN_UINT(pdschCfg->pres,PRSNT_NODEF);
      WR_FILL_TKN_UINT(pdschCfg->p_a, ueCb->pdschDedCfg.p_a); /*4 MIMO*/

      /* PUCCH Configuration */
      pucchCfg = &(radioResCfg->physicalConfigDedicated.pucch_ConfigDedicated);
      WR_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
      /* Default Release - 36.508 */
      WR_FILL_TKN_UINT(pucchCfg->ackNackRepetition.choice, ACKNACKREPETITION_RELEASE);
      /* absent for FDD & TDD use bundling */
      /* TODO Fill the Value of tddAckNackFcbkMode only for
       * TDD - default is bundling for FDD its not Present 
       * for this we need to add this parameter in UeCb */
      duplexMode = wrEmmGetDuplexMode(ueCb->cellId);
      if(duplexMode == NHU_MODE_TDD)
      {
         WR_FILL_TKN_UINT(pucchCfg->tdd_AckNackFeedbackMode,\
               ueCb->pucchDedCfg.tddAckNackFdbkMode);
      }

      /* PUSCH ConfigDedicated */
      puschCfg = &(radioResCfg->physicalConfigDedicated.pusch_ConfigDedicated); 
      WR_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(puschCfg->betaOffset_ACK_Idx, ueCb->puschDedCfg.ackIdx);  /*10 MIMO*/
      WR_FILL_TKN_UINT(puschCfg->betaOffset_RI_Idx, ueCb->puschDedCfg.riIdx); /*12 MIMO*/
      WR_FILL_TKN_UINT(puschCfg->betaOffset_CQI_Idx, ueCb->puschDedCfg.cqiIdx); /*15 MIMO*/ 

      /* Uplink Power Control Configuration */
      uplnkPwrCntrl = &(radioResCfg->physicalConfigDedicated.\
            uplinkPowerCntrlDedicated); 
      WR_FILL_TKN_UINT(uplnkPwrCntrl->pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUSCH,\
            ueCb->ulPwrDedCtrl.p0UePusch);   /*0, MIMO*/
      WR_FILL_TKN_UINT(uplnkPwrCntrl->deltaMCS_Enabled,\
            ueCb->ulPwrDedCtrl.deltaMcsEnabled); /*0, MIMO*/
      WR_FILL_TKN_UINT(uplnkPwrCntrl->accumulationEnabled,\
            ueCb->ulPwrDedCtrl.accumlationEnabled); /*0, MIMO*/
      WR_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUCCH,\
            ueCb->ulPwrDedCtrl.p0UePucch); /*0, MIMO*/
      WR_FILL_TKN_UINT(uplnkPwrCntrl->pSRS_Offset,\
            ueCb->ulPwrDedCtrl.pSrsOff);  /*7, MIMO*/
      WR_FILL_TKN_UINT(uplnkPwrCntrl->filterCoefficient,\
            ueCb->ulPwrDedCtrl.filterCoeff);   /*4, MIMO*/

      /* CQI Reporting Related Information */
      cqiRprtng = &(radioResCfg->physicalConfigDedicated.cqi_ReportConfig);

      /*If the Maximum number of UE's reached, CQI shall not be configured*/
      if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
      {
         WR_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);

         if (ueCb->cqiRptCfg.aperCqiEnbld)
         {
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic, \
                  ueCb->cqiRptCfg.cqiAperiodicMode); /* 3, MIMO*/
         }
         WR_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, \
               ueCb->cqiRptCfg.nomPdschRsEpreOff); /* 0, MIMO */

         /* NhuCQI_ReportPeriodicsetup - empty */
         cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;

         /* Dynamic CQI has been disabled*/      
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
               CQI_REPORTPERIODIC_SETUP);
         WR_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset,\
               ueCb->cqiRptCfg.nomPdschRsEpreOff);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.pres, \
               PRSNT_NODEF);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_PUCCH_ResourceIdx, \
               ueCb->cqiRptCfg.cqiPucchRsrcIdx);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_pmi_ConfigIdx, \
               ueCb->cqiRptCfg.cqipmiCfgIdx);
         if(ueCb->cqiRptCfg.fmtInd ==  WR_UE_CQI_FMT_SB)
         {
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                  cqi_FormatIndicatorPeriodic.choice, 
                  CQI_FORMATINDICATORPERIODIC_SUBBANDCQI);
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                  cqi_FormatIndicatorPeriodic.val.subbandCQI.pres, PRSNT_NODEF);
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                  cqi_FormatIndicatorPeriodic.val.subbandCQI.k,\
                  ueCb->cqiRptCfg.subbndCqik);
         }
         else
         {
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                  cqi_FormatIndicatorPeriodic.choice, 
                  CQI_FORMATINDICATORPERIODIC_WIDEBANDCQI);
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                  cqi_FormatIndicatorPeriodic.val.subbandCQI.pres, NOTPRSNT);
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                  cqi_FormatIndicatorPeriodic.val.subbandCQI.k,
                  ueCb->cqiRptCfg.subbndCqik);
         }
         /* HO_REEST: Filling RI Index */
         if(ueCb->cqiRptCfg.riPres == TRUE)
         {
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.
                  ri_ConfigIdx, ueCb->cqiRptCfg.riCfgIdx);
         }
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.simultaneousAckNackAndCQI, \
               ueCb->cqiRptCfg.simulAckNackAndCQI);
      }
      else
      {
         cqiRprtng->pres.pres = NOTPRSNT;
      }

      /* Scheduling Request Configuration */
      schedulingRqstConfig = &(radioResCfg->physicalConfigDedicated.\
            schedulingRqstConfig);

      if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_SR)
      {
         WR_FILL_TKN_UINT(schedulingRqstConfig->choice,\
               SCHEDULINGRQSTCONFIG_SETUP);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,\
               ueCb->srCfg.srPUCCHResIdx);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
               ueCb->srCfg.srCfgIdx);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
               ueCb->srCfg.enDsrTransMax);
      }
      else
      {
         schedulingRqstConfig->choice.pres = NOTPRSNT;
      }

      /*PDCCH-PUCCH Configuration */
      tpcPdcchCfgPucch = &(radioResCfg->physicalConfigDedicated.\
            tpc_PDCCH_ConfigPUCCH);
      tpcPdcchCfgPucch->choice.pres = NOTPRSNT;
      if (ueCb->uePucchPwr.pres)
      {
         WR_FILL_TKN_UINT(tpcPdcchCfgPucch->choice, TPC_PDCCH_CONFIG_SETUP);
         WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.pres, PRSNT_NODEF);       
         WR_GET_MEM(evnt, 2, &pucchTpcRnti);

         pucchTpcRnti[1] = (U8)(0x00ff & ueCb->uePucchPwr.tpcRnti);
         pucchTpcRnti[0] = (U8)(0x00ff & (ueCb->uePucchPwr.tpcRnti>>8));

         WR_FILL_TKN_STR_UINT((&tpcPdcchCfgPucch->val.setup.tpc_RNTI),pucchTpcRnti , 16);
         if (ueCb->uePucchPwr.isFmt3a)
         {
            WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.choice,1);
            WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.val.indexOfFormat3A, ueCb->uePucchPwr.idx);          
         }
         else
         {
            WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.choice,0);
            WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.val.indexOfFormat3, ueCb->uePucchPwr.idx);
         }   
         /*127802*/    
         RLOG_ARG2(L_INFO, DBG_CRNTI, ueCb->crnti,"****PUCCH POWER CONFIG****"
            "[%u][%u]", ueCb->uePucchPwr.idx, ueCb->uePucchPwr.tpcRnti);
      }
      else
      {
         tpcPdcchCfgPucch->choice.pres = NOTPRSNT;
      }
      /* PDCCH-PUSCH Configuration */
      tpcPdcchCfgPusch = &(radioResCfg->physicalConfigDedicated.\
            tpc_PDCCH_ConfigPUSCH);
      tpcPdcchCfgPusch->choice.pres = NOTPRSNT;
      if (ueCb->uePuschPwr.pres)
      {
         WR_FILL_TKN_UINT(tpcPdcchCfgPusch->choice, TPC_PDCCH_CONFIG_SETUP);
         WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.pres, PRSNT_NODEF);       
         WR_GET_MEM(evnt, 2, &puschTpcRnti);
         puschTpcRnti[1] = (U8)(0x00ff & ueCb->uePuschPwr.tpcRnti);
         puschTpcRnti[0] = (U8)(0x00ff & (ueCb->uePuschPwr.tpcRnti>>8));

         WR_FILL_TKN_STR_UINT((&tpcPdcchCfgPusch->val.setup.tpc_RNTI), puschTpcRnti, 16);
         if (ueCb->uePuschPwr.isFmt3a)
         {
            WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.choice,1);
            WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.val.indexOfFormat3A, ueCb->uePuschPwr.idx);          
         }
         else
         {
            WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.choice,0);
            WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.val.indexOfFormat3, ueCb->uePuschPwr.idx);
         }   
         /*127802*/ 
         RLOG_ARG2(L_INFO, DBG_CRNTI, ueCb->crnti,"****PUSCH POWER CONFIG****"
            "[%u] [%u]", ueCb->uePuschPwr.idx, ueCb->uePuschPwr.tpcRnti);
      }
      else
      {
         tpcPdcchCfgPusch->choice.pres = NOTPRSNT;
      }
      /* Sounding Rs Uplink Configuration */
      soundngRsUlCfg = &(radioResCfg->physicalConfigDedicated.\
            soundingRS_UL_ConfigDedicated);
      if(wrSmEnableSrs == TRUE)
      {
         soundngRsUlCfg->choice.pres = TRUE;
         WR_FILL_TKN_UINT(soundngRsUlCfg->choice, SOUNDINGRS_UL_CONFIGDEDICATED_SETUP);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.srs_Bandwidth,ueCb->srsUlDedCfg.srsBw);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.srs_HoppingBandwidth,ueCb->srsUlDedCfg.srsHoppingBw);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.freqDomainPosition,ueCb->srsUlDedCfg.freqDomPos);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.duration,ueCb->srsUlDedCfg.duration);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.srs_ConfigIdx,ueCb->srsUlDedCfg.srsCfgIdx);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.transmissionComb,ueCb->srsUlDedCfg.transComb);
         WR_FILL_TKN_UINT(soundngRsUlCfg->val.setup.cyclicShift,ueCb->srsUlDedCfg.cyclicshift);
      }
      else
      {
         soundngRsUlCfg->choice.pres = TRUE;
         WR_FILL_TKN_UINT(soundngRsUlCfg->choice, SOUNDINGRS_UL_CONFIGDEDICATED_RELEASE);
      }
   }
   else
   {
      /*   tpc-PDCCH-ConfigPUCCH  empty 
       *   PDCCH-PUCCH Configuration */
      tpcPdcchCfgPucch = &(radioResCfg->physicalConfigDedicated.\
            tpc_PDCCH_ConfigPUCCH);
      tpcPdcchCfgPucch->choice.pres = NOTPRSNT;


      /* tpc-PDCCH-ConfigPUSCH 
       * PDCCH-PUSCH Configuration */
      tpcPdcchCfgPusch = &(radioResCfg->physicalConfigDedicated.\
            tpc_PDCCH_ConfigPUSCH);
      tpcPdcchCfgPusch->choice.pres = NOTPRSNT;


      /* CQI Reporting Related Information */
      cqiRprtng = &(radioResCfg->physicalConfigDedicated.cqi_ReportConfig);
      /*If the Maximum number of UE's reached, CQI shall not be configured*/
      if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
      {
         WR_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);
         if (ueCb->cqiRptCfg.aperCqiEnbld)
         {
            WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic,\
                  ueCb->cqiRptCfg.cqiAperiodicMode);
         }
         WR_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset,\
               ueCb->cqiRptCfg.nomPdschRsEpreOff);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
               CQI_REPORTPERIODIC_SETUP);

         /* NhuCQI_ReportPeriodicsetup */ 
         cqiRepPerSetup = &(cqiRprtng->cqi_ReportPeriodic.val.setup);

         WR_FILL_TKN_UINT(cqiRepPerSetup->pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_PUCCH_ResourceIdx,\
               ueCb->cqiRptCfg.cqiPucchRsrcIdx);

         WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_pmi_ConfigIdx,\
               ueCb->cqiRptCfg.cqipmiCfgIdx);
         /*129311: CQI format check*/
         if(ueCb->cqiRptCfg.fmtInd ==  WR_UE_CQI_FMT_WB)
         {
            WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_FormatIndicatorPeriodic.choice,\
                  CQI_FORMATINDICATORPERIODIC_WIDEBANDCQI);
         }
         else
         {
            WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_FormatIndicatorPeriodic.choice,\
                  CQI_FORMATINDICATORPERIODIC_SUBBANDCQI);
            WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_FormatIndicatorPeriodic.val.subbandCQI.pres, PRSNT_NODEF);
            /*129311:subband value k assigned*/
            WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_FormatIndicatorPeriodic.val.subbandCQI.k,\
                  ueCb->cqiRptCfg.subbndCqik);

         }
         /*129311:check for rank indication*/
         if(ueCb->cqiRptCfg.riPres == TRUE)
         {
            WR_FILL_TKN_UINT(cqiRepPerSetup->ri_ConfigIdx,\
                  ueCb->cqiRptCfg.riCfgIdx);

         }
         WR_FILL_TKN_UINT(cqiRepPerSetup->simultaneousAckNackAndCQI,\
               ueCb->cqiRptCfg.simulAckNackAndCQI);
      }
      else
      {
         cqiRprtng->pres.pres = NOTPRSNT;
      }
      /* Sounding Rs Uplink Configuration */
      soundngRsUlCfg = &(radioResCfg->physicalConfigDedicated.\
            soundingRS_UL_ConfigDedicated);

      soundngRsUlCfg->choice.pres = NOTPRSNT;

      /* Scheduling Request Configuration */
      schedulingRqstConfig = &(radioResCfg->physicalConfigDedicated.\
            schedulingRqstConfig);

      if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_SR)
      {
         WR_FILL_TKN_UINT(schedulingRqstConfig->choice,\
               SCHEDULINGRQSTCONFIG_SETUP);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,\
                ueCb->srCfg.srPUCCHResIdx);

         /* This function is called during Ctf Cfg Req 
          * veGetSRCfgIdx(&schedulingRqstConfig->val.setup.sr_ConfigIdx);
          * and the cfg idx is derived then */
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
               ueCb->srCfg.srCfgIdx);
         WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
               ueCb->srCfg.enDsrTransMax);
      }
      else
      {
         schedulingRqstConfig->choice.pres = NOTPRSNT;
      }
   }

   /*  Antenna Configuration */
   antennaInfo = &(radioResCfg->physicalConfigDedicated.antennaInfo);
   WR_FILL_TKN_UINT(antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
   WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.transmissionMode,\
         ueCb->antennaInfo.txmode);

   if  (ueCb->antennaInfo.txmode == CTF_UE_TX_ANT_MODE_TM3)
   { 
      WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3);
      WR_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm3,\
            2, 3, evnt); /* bit string 11 */
      /* ReEst Fix */
     
   }
   else if (ueCb->antennaInfo.txmode == CTF_UE_TX_ANT_MODE_TM4)
   { 
      WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM4);
      WR_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm4,\
            6, 63, evnt); 
   }

   WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.\
         ue_TransmitAntennaSelection.choice,\
         UE_TRANSMITANTENNASELECTION_RELEASE);

   RETVALUE(ROK);
}

PRIVATE Void wrUmmRrcFillMacCfg
(
NhuRadioResourceConfigDedicatedmac_MainConfig *macIe,
WrUeCb                       *ueCb
)
{
   NhuMAC_MainConfig              *expVal;
   NhuMAC_MainConfigul_SCH_Config *ulSchCfg;
   NhuMAC_MainConfigphr_Config    *phrCfg;

   wrFillTknU8(&macIe->choice, MAC_MAINCONFIG_EXPLICITVALUE);
   wrFillTknU8(&(macIe->val.explicitValue.pres), PRSNT_NODEF);
   expVal = &(macIe->val.explicitValue);

   /* Fill UL shared channel configuration */
   ulSchCfg = &(expVal->ul_SCH_Config);
   wrFillTknU8(&ulSchCfg->pres, PRSNT_NODEF);
   wrFillTknU32(&ulSchCfg->maxHARQ_Tx, ueCb->ueUlHqCfg.maxUlHqTx);
#ifdef WR_RSYS_OAM
   wrFillTknU32(&ulSchCfg->periodicBSR_Timer, ueCb->uePerBsrTmrCfg.prdBsrTmr);
   wrFillTknU32(&ulSchCfg->retxBSR_Timer, ueCb->uePerBsrTmrCfg.retxBsrTmr);
#else
   wrFillTknU32(&ulSchCfg->periodicBSR_Timer, 1);
   wrFillTknU32(&ulSchCfg->retxBSR_Timer, 0);
#endif
   wrFillTknU8(&(ulSchCfg->ttiBundling), 0);

   /* Timing Alignment Timer set to infinity */
   wrFillTknU32(&expVal->timeAlignmentTimerDedicated,
         wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.timeAlignmentTimerDedicated);

   /* PHR configuration */
   phrCfg = &(expVal->phr_Config);
   wrFillTknU8(&phrCfg->choice, 1); /* Setting it up as SETUP */
   wrFillTknU8(&phrCfg->val.setup.pres, PRSNT_NODEF);
   wrFillTknU32(&phrCfg->val.setup.periodicPHR_Timer, 3); /* Check the value and set it to default one */
   wrFillTknU32(&phrCfg->val.setup.prohibitPHR_Timer, 0); /* Check the value and set it to default one */
   wrFillTknU32(&phrCfg->val.setup.dl_PathlossChange, 1); /* Check the value and set it to default one */
   /* Set the R9 parameters are not present */
   wrFillTknU8(&expVal->extaddgrp_1.pres, NOTPRSNT);
   RETVOID;
}


/**
 *  @brief This function fills the RRC connection setup message
 *
 *          Processing steps:
 *          - Obtain the pointer to ccch message
 *          - fill the RRc Connection setup message
 *
 *  @param [out] nhuDatRspSdu  : pointer to structure containing RRC connection
 *                               setup message
 *  @param [in]  ueCb          : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcFillConSetup
(
NhuDatRspSdus                *nhuDatRspSdu,
WrUeCb                       *ueCb
)
{
   U8                              isSrb1 = TRUE;
   NhuDL_CCCH_Msg                  *dlMsg;
   NhuRRCConSetup                  *conSetUp;
   NhuRRCConSetup_r8_IEs           *r8_IEs;
   NhuRadioResourceConfigDedicated *resCfgDed;

   RLOG0(L_DEBUG, "Building  Connection Setup Msg");
   /*127802-start*/
   RLOG_ARG1(L_INFO, DBG_CRNTI, ueCb->crnti,"Sending RRC-Connection Setup "
      "cellId[%d]",ueCb->cellId);
   /*127802-end*/
   nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);
   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_CCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_RRCCONSETUP);

   conSetUp = &(dlMsg->message.val.c1.val.rrcConSetup);
   wrFillTknU8(&conSetUp->pres, PRSNT_NODEF);
   wrFillTknU32(&conSetUp->rrc_TrnsxnId, 0);
   wrFillTknU8(&conSetUp->criticalExtns.choice, CRITICALEXTNS_C1);

   /* RRCConnectionSetup-r8-IEs*/
   r8_IEs = &(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8);
   wrFillTknU8(&conSetUp->criticalExtns.val.c1.choice, C1_RRCCONSETUP_R8);
   wrFillTknU8(&r8_IEs->pres, PRSNT_NODEF);
   wrFillTknU8(&r8_IEs->nonCriticalExtn.pres, NOTPRSNT); 

   /* RadioResourceConfigDedicated */
   resCfgDed = &(r8_IEs->radioResourceConfigDedicated);
   wrFillTknU8(&resCfgDed->pres, PRSNT_NODEF);

   WR_GET_MEM(nhuDatRspSdu, sizeof(NhuSRB_ToAddMod), 
                                        &resCfgDed->srb_ToAddModLst.member);
   wrFillTknU16(&(resCfgDed->srb_ToAddModLst.noComp), 1);
   wrUmmRrcFillSrbIE(ueCb->cellId, &resCfgDed->srb_ToAddModLst.member[0], isSrb1);
#if 0
#else
   wrUmmRrcFillMacCfg(&resCfgDed->mac_MainConfig, ueCb);
#endif
   wrFillRadResPhyCfgDed(resCfgDed, (Ptr)nhuDatRspSdu, WR_PDCP_ID_SRB1, ueCb);
    /* RRC-REL10-Upgrade */
   wrFillTknU8(&(resCfgDed->extaddgrp_1.pres), PRSNT_NODEF);
   wrFillTknU8(&(resCfgDed->extaddgrp_1.rlf_TmrsAndConsts_r9.choice),
         RLF_TMRSANDCONSTS_R9_RELEASE); 
   resCfgDed->extaddgrp_1.rlf_TmrsAndConsts_r9.val.setup.pres.pres = NOTPRSNT;

   RETVALUE(ROK);
} /* End of wrUmmRrcFillConSetup */

 /*  @brief This function  builds UTRA redirection connection release message
 *
 *      Function: wrUmmFillUtraRedirInfo
 *
 *          Processing steps:
 *          - update the redirect info into connection release messge
 *
 *  @param [in]   accessStratumRls : Access Strtutam Release 
 *  @param [in]   conRlsR8 : pointer to NHU Redirection information 
 *  @param [out]  redirectInfo : pointer to Redirect information 
 *  @param [in]  NhuDatReqSdus: pointer to RRC Con Release Message
 *  @return 
 */
PRIVATE S16 wrUmmFillUtraFddRedirInfo
(
 WrUeCb                      *ueCb,
 U8                          accessStratumRls,
 NhuRRCConRls_r8_IEs         *conRlsR8,
 WrUmmRedirectInfo           *redirectInfo,
 NhuDatReqSdus               *nhuDatReqSdu
 )
{
   NhuRRCConRls_v890_IEs       *nonCritExtn_v890 = NULLP;
   NhuRRCConRls_v920_IEs       *nonCritExtn_v920 = NULLP;
   NhuCellInfoLstUTRA_FDD_r9   *utraFddR9     = NULLP;
   NhuRedirectedCarrierInfo    *redirInfo = NULLP;
   WrUmmUtraRedirectInfo       *utraRedirectInfo = &redirectInfo->val.utraInfo;
   NhuRRCConRls_v920_IEscellInfoLst_r9 *cellInfoLstR9 = NULLP;
   U8 index1 = 0, idx = 0;

   TRC2(wrUmmFillUtraFddRedirInfo)

   redirInfo = &conRlsR8->redirectedCarrierInfo;

   if(redirectInfo->val.utraInfo.noOfPscs != 0)
   {
         wrFillTknU8(&redirInfo->choice, REDIRECTEDCARRIERINFO_UTRA_FDD);
         wrFillTknU32(&redirInfo->val.utra_FDD, redirectInfo->val.utraInfo.arfcn);
         if(accessStratumRls >= NhuAccessStratumRlsrel9Enum)
         {
            RLOG0(L_INFO, "UE Supports Rel 9 and Above"); 
            /*Check fo UE Supports eRidirection and Access SI for 
             * selected UTRA Neighbours 
             * */
            if(TRUE == redirectInfo->eRedirUtraR9Sup)
            {
               RLOG0(L_INFO, "UE Supports eRedirUtraR9Sup"); 
               /*Access UTRA NGH for SI Information, this function also
                * updates the redirection info which has PSC for the 
                * selcted UTRA Cells contains SI so that while filling 
                * redirection with SI only those NGH will be selected
                * */
               if(ROK == (wrEmmGetNeighUtraSIInfo(redirectInfo)) 
                                       && (utraRedirectInfo->noOfPscs != 0))
               {
                  RLOG0(L_INFO, "UE and UTRA NGH has SI information"); 
                  nonCritExtn_v890 = &conRlsR8->nonCriticalExtn;
                  wrFillTknU8(&nonCritExtn_v890->pres,PRSNT_NODEF);

                  nonCritExtn_v920 = &nonCritExtn_v890->nonCriticalExtn;
                  wrFillTknU8(&nonCritExtn_v920->pres, PRSNT_NODEF);

                  cellInfoLstR9 = &nonCritExtn_v920->cellInfoLst_r9;
                  wrFillTknU8(&cellInfoLstR9->choice, CELLINFOLST_R9_UTRA_FDD_R9);
               
                  utraFddR9 = &cellInfoLstR9->val.utra_FDD_r9;

                  WR_GET_MEM(&nhuDatReqSdu->memCp,
                        utraRedirectInfo->noOfPscs * sizeof(NhuCellInfoUTRA_FDD_r9), &utraFddR9->member);
                  for(index1 = 0; 
                        index1 < utraRedirectInfo->noOfPscs; index1++)
                  {
                     if(utraRedirectInfo->utraCellRedirInfo[index1].utraSI.pres == PRSNT_NODEF)
                     {
                        RLOG0(L_INFO, "FILLING UTRA with SI Information"); 
                        /* Fill PhysCellIdUTRA-FDD and utra-BCCH-Container-r9 
                           From utraRedirectInfo */ 
                        wrFillTknU8(&(utraFddR9->member[idx].pres), PRSNT_NODEF);
                        
                        wrFillTknU32(&utraFddR9->member[idx].physCellId_r9, 
                              utraRedirectInfo->utraCellRedirInfo[index1].pscId);

                        wrFillTknStrOSXL(&utraFddR9->member[idx].utra_BCCH_Cont_r9,
                              utraRedirectInfo->utraCellRedirInfo[index1].utraSI.len,
                              utraRedirectInfo->utraCellRedirInfo[index1].utraSI.val,
                              &nhuDatReqSdu->memCp);
                        idx++;
                     }/*end of CellInfoList Filling */
                  } /* end of FOR loop */
                  wrFillTknU16(&utraFddR9->noComp, idx);
               }/* end of EMM UTRA NGH Function call*/
            }/*end of UE supp of e-Redirection check*/
         }/* end of accessStratumRls Check */
   }/*end of First if loop*/
   else
   {
      if ((redirectInfo->redirType == WR_UMM_CSFB) && (ueCb != NULLP))
      {
         /*Increment UTRA UTRA_CSFB_WITHOUT_REDIRECTION */
            ueCb->csfbInfo.redirType = UTRA_CSFB_WITHOUT_REDIRECTION;
      }
      RLOG0(L_WARNING, "redirectInfo->val.utraInfo.noOfPscs are ZERO, Sending Without Redirection"); 
   }

   if ((redirectInfo->redirType == WR_UMM_CSFB) &&  (idx > 0))
   {
		/*Increment UTRA KPI_ID_LTE_CSFB_UTRA_WITH_REDIR_WITH_SI */
	   if (ueCb != NULLP)
      {
         ueCb->csfbInfo.redirType = UTRA_CSFB_WITH_REDIRECTION_WITH_SI;
      }
   }
   RETVALUE(ROK);
}/*wrUmmFillUtraReDirectInfo*/

 /*  @brief This function  builds UTRA TDD redirection connection release message
 *
 *      Function: wrUmmFillUtraTddRedirInfo
 *
 *          Processing steps:
 *          - update the redirect info into connection release messge
 *
 *  @param [in]   accessStratumRls : Access Strtutam Release 
 *  @param [in]   conRlsR8 : pointer to NHU Redirection information 
 *  @param [out]  redirectInfo : pointer to Redirect information 
 *  @param [in]  NhuDatReqSdus: pointer to RRC Con Release Message
 *  @return 
 */
PRIVATE S16 wrUmmFillUtraTddRedirInfo
(
 WrUeCb                      *ueCb,
 U8                          accessStratumRls,
 NhuRRCConRls_r8_IEs         *conRlsR8,
 WrUmmRedirectInfo           *redirectInfo,
 NhuDatReqSdus               *nhuDatReqSdu
 )
{
   U8                          idx = 0;
   U32	noOfPscsWithSi =0;
   WrUmmUtraTddRedirectInfo   *utraTddRedirInfo = &redirectInfo->val.utraTddInfo;

   /* Check if UE supports eRedirUtraR10Sup and the nos of 
    * UTRA TDD ARFCN are more than 1. Then choice is 
    * REDIRECTEDCARRIERINFO_UTRA_TDD_R10. */
   if(utraTddRedirInfo->eRedirUtraR10Sup == TRUE) 
   {
      NhuCarrierFreqLstUTRA_TDD_r10    *utraTddR10 = NULLP;
      utraTddR10 = &conRlsR8->redirectedCarrierInfo.val.utra_TDD_r10;

      wrFillTknU8(&conRlsR8->redirectedCarrierInfo.choice, 
            REDIRECTEDCARRIERINFO_UTRA_TDD_R10);
      wrFillTknU16(&utraTddR10->noComp, utraTddRedirInfo->noOfTddArfcn);

      WR_GET_MEM(&nhuDatReqSdu->memCp,
            utraTddRedirInfo->noOfTddArfcn * sizeof(NhuARFCN_ValueUTRA), &utraTddR10->member);
      for(idx = 0; idx < utraTddRedirInfo->noOfTddArfcn; idx++)
      {
         wrFillTknU32(&utraTddR10->member[idx], 
               utraTddRedirInfo->arfcnTddLst[idx]);
      }
   }
   else
   {
      wrFillTknU8(&conRlsR8->redirectedCarrierInfo.choice, 
            REDIRECTEDCARRIERINFO_UTRA_TDD);
      wrFillTknU32(&conRlsR8->redirectedCarrierInfo.val.utra_TDD, 
            utraTddRedirInfo->arfcnTddLst[0]);
   }

   /* Fill the Cell Info Lst with UTRA TDD SI. */
   if(utraTddRedirInfo->noOfPscs != 0)
   {
      if((utraTddRedirInfo->eRedirUtraR10Sup == TRUE) || (redirectInfo->eRedirUtraR9Sup == TRUE)) 
      {
         for (idx=0; idx < utraTddRedirInfo->noOfPscs; idx++)
         {
            if (utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.pres == PRSNT_NODEF)	
            {
               noOfPscsWithSi++;
            }
         }

         if (noOfPscsWithSi == 0)
         {
            RLOG0(L_WARNING, "redirectInfo->val.utraInfo.noOfPscs with SI is ZERO"); 
            RETVALUE(ROK);
         }
      }

      NhuRRCConRls_v920_IEscellInfoLst_r9   *cellInfoLst = 
         &conRlsR8->nonCriticalExtn.nonCriticalExtn.cellInfoLst_r9;

      /* If UE supports eRedirR9 fill the Cell Info Lst R9 */
      if((redirectInfo->eRedirUtraR9Sup == TRUE) && 
            (utraTddRedirInfo->eRedirUtraR10Sup == FALSE))
      {
         NhuCellInfoUTRA_TDD_r9   *cellInfoTdd = NULLP;

         wrFillTknU8(&conRlsR8->nonCriticalExtn.pres, PRSNT_NODEF);
         wrFillTknU8(&conRlsR8->nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);
         wrFillTknU8(&cellInfoLst->choice, CELLINFOLST_R9_UTRA_TDD_R9);
         wrFillTknU16(&cellInfoLst->val.utra_TDD_r9.noComp, utraTddRedirInfo->noOfPscs);
         WR_GET_MEM(&nhuDatReqSdu->memCp,
               noOfPscsWithSi * sizeof(NhuCellInfoUTRA_TDD_r9), &cellInfoLst->val.utra_TDD_r9.member);
         for(idx = 0; idx < utraTddRedirInfo->noOfPscs; idx++)
         {         
            if (utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.pres == PRSNT_NODEF)	
            {
               cellInfoTdd = &cellInfoLst->val.utra_TDD_r9.member[idx];

               wrFillTknU8(&cellInfoTdd->pres, PRSNT_NODEF);
               wrFillTknU32(&cellInfoTdd->physCellId_r9, utraTddRedirInfo->utraCellRedirInfo[idx].pscId);
               wrFillTknStrOSXL(&cellInfoTdd->utra_BCCH_Cont_r9,
                     utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.len,
                     utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.val,
                     &nhuDatReqSdu->memCp);
            }
         }
      }
      /* If UE supports eRedirR10 fill the Cell Info Lst R10 */
      if(utraTddRedirInfo->eRedirUtraR10Sup == TRUE) 
      {
         NhuCellInfoUTRA_TDD_r10    *cellInfoTdd = NULLP;

         wrFillTknU8(&conRlsR8->nonCriticalExtn.pres, PRSNT_NODEF);
         wrFillTknU8(&conRlsR8->nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);
         wrFillTknU8(&cellInfoLst->choice, CELLINFOLST_R9_UTRA_TDD_R10);
         wrFillTknU16(&cellInfoLst->val.utra_TDD_r10.noComp, utraTddRedirInfo->noOfPscs);
         WR_GET_MEM(&nhuDatReqSdu->memCp,
               noOfPscsWithSi * sizeof(NhuCellInfoUTRA_TDD_r10), &cellInfoLst->val.utra_TDD_r10.member);
         for(idx = 0; idx < utraTddRedirInfo->noOfPscs; idx++)
         {
            if (utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.pres == PRSNT_NODEF)	
            {
               cellInfoTdd = &cellInfoLst->val.utra_TDD_r10.member[idx];

               wrFillTknU8(&cellInfoTdd->pres, PRSNT_NODEF);
               wrFillTknU32(&cellInfoTdd->physCellId_r10, utraTddRedirInfo->utraCellRedirInfo[idx].pscId);
               wrFillTknU32(&cellInfoTdd->carrierFreq_r10, utraTddRedirInfo->utraCellRedirInfo[idx].arfcn);
               wrFillTknStrOSXL(&cellInfoTdd->utra_BCCH_Cont_r10,
                     utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.len,
                     utraTddRedirInfo->utraCellRedirInfo[idx].utraSI.val,
                     &nhuDatReqSdu->memCp);
            }
         }
      }
   }
   else
   {
      RLOG0(L_WARNING, "redirectInfo->val.utraInfo.noOfPscs are ZERO"); 
   }

   if ((redirectInfo->redirType == WR_UMM_CSFB) && (noOfPscsWithSi > 0))
   {
		/*Increment UTRA KPI_ID_LTE_CSFB_UTRA_WITH_REDIR_WITH_SI */
		if (ueCb != NULLP)
      {
         ueCb->csfbInfo.redirType = UTRA_CSFB_WITH_REDIRECTION_WITH_SI;
      }
   }
   RETVALUE(ROK);
}

/**
 *  @brief This function  builds redirection connection release message
 *
 *      Function: wrUmmFillReDirectInfoToRrcRelMsg
 *
 *          Processing steps:
 *          - update the redirect info into connection release messge
 *
 *  @param [in]   nhuDatReqSdu     : SDU to allocate memory
 *  @param [in]   accessStratumRls : Access Strtutam Release 
 *  @param [in]   conRlsR8 : pointer to NHU Redirection information 
 *  @param [out]  redirectInfo : pointer to Redirect information 
 *  @param [in]  NhuDatReqSdus: pointer to RRC Con Release Message
 *  @return 
 */
PRIVATE Void wrUmmFillReDirectInfoToRrcRelMsg
(
 WrUeCb                      *ueCb,
 U8                          accessStratumRls,
 NhuRRCConRls_r8_IEs         *conRlsR8,
 WrUmmRedirectInfo           *redirectInfo,
 NhuDatReqSdus               *nhuDatReqSdu
 )
{
   NhuRedirectedCarrierInfo    *redirInfo = NULLP;

   TRC2(wrUmmFillReDirectInfoToRrcRelMsg)

   redirInfo = &conRlsR8->redirectedCarrierInfo;

   RLOG1(L_DEBUG, "Function wrUmmFillReDirectInfoToRrcRelMsg: RATTYPE:=%d", 
                                            redirectInfo->ratTyp);
   /* Re-direction Info for Release 9 and above UE */
   /* When Rat Type is CDMA(1xRTT or HRPD) */
   switch(redirectInfo->ratTyp)
   {
      case WR_UMM_MEAS_INTRA_FREQ:
      case WR_UMM_MEAS_INTER_FREQ:
         {
            wrFillTknU8(&redirInfo->choice,
                                REDIRECTEDCARRIERINFO_EUTRA);
            wrFillTknU32(&redirInfo->val.eutra, 
                           redirectInfo->val.eutraInfo.arfcn);
         }
         break;
      case WR_UMM_MEAS_1XCDMA:
      case WR_UMM_MEAS_CDMA_HRPD:
         {
            wrUmmFillCdmaRedirectionInfo(redirInfo,  redirectInfo);
         }
         break;
      case WR_UMM_MEAS_UTRA_FDD_FREQ:
         {
            wrUmmFillUtraFddRedirInfo(ueCb, accessStratumRls, 
                         conRlsR8, redirectInfo, nhuDatReqSdu);
         }
         break;
      case WR_UMM_MEAS_UTRA_TDD_FREQ:
         {
            wrUmmFillUtraTddRedirInfo(ueCb, accessStratumRls,
                  conRlsR8, redirectInfo, nhuDatReqSdu);
         }
         break;
      case WR_UMM_MEAS_GERAN_FREQ:
         {
            wrFillTknU8(&redirInfo->choice, REDIRECTEDCARRIERINFO_GERAN);
            wrUmmFillNhuGeranRedirectInfo(redirectInfo, conRlsR8, 
                                           nhuDatReqSdu, accessStratumRls);
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "RAT Type is not Supported for CSFB"); 
         }
         break;
   }
   RETVOID;
} /* wrUmmFillReDirectInfoToRrcRelMsg*/
/**
 *  @brief This function fills Downlink Information Transfer
 *
 *      Function: wrUmmRrcFillDlInfTfr
 *
 *          Processing steps:
 *          - obtain the DL DCCH message pointer 
 *          - set the presence of the message
 *          - fill the Downlink Information Transfer
 *
 *
 *  @param [out] dlInfoTfrPdu : pointer to DL info transfer PDU 
 *  @param [in]  nasPdu       : pointer to NAS pdu
 *  @param [in]  pdcpId       : bearer PDCP ID 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcFillDlInfTfr
(
NhuDatReqSdus                *dlInfoTfrPdu,
TknStrOSXL                   *nasPdu,
U8                           pdcpId
)
{
   NhuDL_DCCH_Msg            *dlMsg = NULLP;
   NhuDLInfoTfr              *infoTfrMsg = NULLP;
   NhuDLInfoTfr_r8_IEs       *msgIe = NULLP;
   U16                       idx = 0;
   
   RLOG0(L_DEBUG, "Building DL Info Transfer");

   dlInfoTfrPdu->sdu.isUeCfgPres    = FALSE;
   dlInfoTfrPdu->sdu.ueMsg.isMsgDed = TRUE;
   dlInfoTfrPdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = pdcpId;
   dlMsg = &(dlInfoTfrPdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_DLINFOTFR);

   /* DLInformationTransfer */
   infoTfrMsg = &(dlMsg->message.val.c1.val.dlInfoTfr);
   wrFillTknU8(&infoTfrMsg->pres, PRSNT_NODEF);
   /* TODO: Determine how to set the transaction id appropriately         */
   wrFillTknU32(&infoTfrMsg->rrc_TrnsxnId, 0);
   wrFillTknU8(&infoTfrMsg->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&infoTfrMsg->criticalExtns.val.c1.choice, C1_DLINFOTFR_R8);

   /* DLInformationTransfer-r8-IEs */ 
   msgIe = &(infoTfrMsg->criticalExtns.val.c1.val.dlInfoTfr_r8);
   wrFillTknU8(&msgIe->pres, PRSNT_NODEF);
   wrFillTknU8(&msgIe->dedicatedInfoTyp.choice,
                                        DEDICATEDINFOTYP_DEDICATEDINFONAS);

   /* NAS PDU */
   WR_GET_MEM(dlInfoTfrPdu, nasPdu->len, 
                        &(msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.val));
   msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.pres = PRSNT_NODEF;
   msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.len = nasPdu->len;
   /* copy each byte */
   for(idx = 0; idx < nasPdu->len; idx++)
   {
      msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.val[idx] = nasPdu->val[idx];
   }

   /* criticalExtensionsFuture */
   wrFillTknU8(&msgIe->nonCriticalExtn.pres, NOTPRSNT);

   RETVALUE(ROK);
} /* End of veFillRRC_DlInfTr */



/**
 *  @brief This function fills PDCP Configuration
 *
 *      Function: wrFillPdcpCfg
 *
 *          Processing steps:
 *          - fill the PDCP  configuration
 *          - update whether confirmation is required or not
 *
 *
 *  @param [out] pdcpCfg : pointer to PDCP config message
 *  @param [in]  rbId     : Radio Beare Id
 *  @param [in]  cfmReqd  : whetehr the confirmation is required
 *  @param [in]  ueCb    : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrFillPdcpCfg
(
NhuPdcpCfg    *pdcpCfg,
U8            rbId,
U8            cfmReqd,
WrUeCb        *ueCb
)
#else
PUBLIC S16 wrFillPdcpCfg(pdcpCfg, rbId, cfmReqd,ueCb)
NhuPdcpCfg    *pdcpCfg;
U8            rbId;
U8            cfmReqd;
WrUeCb        *ueCb;
#endif
{
   TRC2(wrFillPdcpCfg);

   RLOG0(L_DEBUG, "Fill PDCP Cfg");

   pdcpCfg->pdcpCfgBitMsk = 
      (NHU_PDCP_DISCARD_TMR | NHU_PDCP_STATUS_RPT | NHU_PDCP_CFM_REQD);

   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_DISCARD_TMR)
   {
      pdcpCfg->discardTmr = ueCb->pdcpCfg.discardTmr;/*NHU_PDCP_DISCARD_TMR_MSINF;*/
   }

   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_STATUS_RPT)
   {
      pdcpCfg->statusRptReq = ueCb->pdcpCfg.statusRptReq;
   }

   if (rbId == 3)
   {
      pdcpCfg->snSize = ueCb->pdcpCfg.snSize;/*NHU_PDCP_SN_SIZE_7BITS;*/
      pdcpCfg->pdcpCfgBitMsk |=  NHU_PDCP_SN_SIZE;
      pdcpCfg->statusRptReq = ueCb->pdcpCfg.statusRptReq/*FALSE*/;
   }
   /* Updating cfmreqd for SRB channels */
   pdcpCfg->cfmReqd = cfmReqd;

   /* 00124655: to make the PDCP discard timer infinite
    * Removed the hardcoded part at application configured at the layer.
    */
   RETVALUE(ROK);
} /* End of wrFillPdcpCfg */


/**
 *  @brief This function fills Transport Channel Configuration
 *
 *      Function: wrUmmRrcFillTrchCfg
 *
 *          Processing steps:
 /          - update the Transpot channel config Info
 *
 *
 *  @param [out] trChCfg : ponter to Transport changnel message to be updated
 *  @param [in]  ueCb    : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcFillTrchCfg
(
NhuTrChCfg                   *trChCfg,
WrUeCb                       *ueCb
)
{
   U8                        trChCfgType;

   RLOG0(L_DEBUG, "Fill Transport Channel Cfg.");

   trChCfgType = NHU_CFG_TYPE_EXPLICIT;

   trChCfg->trChCfgType = trChCfgType;
   if(trChCfg->trChCfgType == NHU_CFG_TYPE_EXPLICIT)
   {
      trChCfg->trChBitMsk                = (NHU_TRCH_CFG_UL_HARQ);
      trChCfg->ulHarqCfg.ulMaxHarqTx     = ueCb->harqinfo.ulMaxHarqTx;
      trChCfg->ulHarqCfg.deltaHarqOffset = ueCb->harqinfo.deltaHarqOffset;
   } 
   RETVALUE(ROK);
} /* End of wrUmmRrcFillTrchCfg */

/**
 *  @brief This function fills Security Configuration
 *
 *      Function: wrFillSecCfg
 *
 *          Processing steps:
 *          - update the RRc security configuration info
 *
 *
 *  @param [out] rrcSecCfg : pointer to security config info to be updated
 *  @param [in] secCfg    : UE security Info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmRrcFillSecCfg
(
NhuSecurityCfg               *rrcSecCfg,
WrUeSecCfg                   *secCfg
)
{
   U8                        idx = 0;

   RLOG0(L_DEBUG, "Fill Security Related Cfg Info");

   rrcSecCfg->secCfgBitMsk = (NHU_SEC_CIPHER_ALGO | NHU_SEC_INTG_ALGO);
   rrcSecCfg->intgCfgInfo.action = NHU_ACTION_ADD;

   /* Intg Cfg */
   switch(secCfg->intgAlgo)
   {
      case 1:
      {
         rrcSecCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA1;
         break;
      }
      case 2:
      {
         rrcSecCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA2;
         break;
      }
/* ZUC_ALGO - start */
      case 3:
      {
         rrcSecCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA3;
         break;
      }
/* ZUC_ALGO - end */      
   }
   for(idx = 0; idx < WR_INTG_KEY_LEN; idx++)
   {
      rrcSecCfg->intgCfgInfo.intgKey[idx] = secCfg->intgKey[idx];
   }

   /* Ciph Cfg */
   switch(secCfg->ciphAlgo)
   {
      case 0:
      {
         rrcSecCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA0;
         break;
      }
      case 1:
      {
         rrcSecCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA1;
         break;
      }
      case 2:
      {
         rrcSecCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA2;
         break;
      }
/* ZUC_ALGO - start */
      case 3:
      {
         rrcSecCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA3;
         break;
      }
/* ZUC_ALGO - end */
   }

   for(idx = 0; idx < WR_CIPH_KEY_LEN; idx++)
   {
      rrcSecCfg->cipherCfgInfo.cipherCpKey[idx] = secCfg->cpCiphKey[idx];
      rrcSecCfg->cipherCfgInfo.cipherUpKey[idx] = secCfg->upCiphKey[idx];
   }

   RETVALUE(ROK);
} /* End of wrFillSecCfg */
   /* ccpu00129736 removing duplicate function wrUmmFillNhuUeCfgInfo and 
         renamed properly */
#ifdef TENB_AS_SECURITY
PUBLIC S16 wrUmmFillNhuUeCfgInfo
(
NhuUeCfgInfo                 *ueCfg,
WrUeCb                       *ueCb,
U16                          newCrnti
)
{
   U8                        expCfgBitMask = 0;

   RLOG0(L_DEBUG, "Construct UE Cfg Related Info");

   /* MS_FIX: security configuration must exist once the complete security */
   /*         is validated. For now, we will check if the security config  */
   /*         on the ueCb is not NULL                                      */
   if (ueCb->secCfg != NULLP)
   {
      expCfgBitMask |= NHU_EXP_CFG_SEC_CFG;
   }

   expCfgBitMask |= (NHU_EXP_CFG_UE_CFG | NHU_EXP_CFG_TRCH_CFG);
   ueCfg->conReCfgCause = NHU_CONN_RESTB_EVNT;

   /* filling UeCfg type*/
   ueCfg->ueCfgBitMsk = expCfgBitMask;


   /* Transport Channel Configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_TRCH_CFG)
   {
      wrUmmRrcFillTrchCfg(&(ueCfg->trChCfg),ueCb);
   }
   /* Security Channel Configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_SEC_CFG)
   {
      if(ueCb->hzSecCfg != NULLP) 
      {
         wrUmmRrcFillSecCfg(&(ueCfg->secCfg), ueCb->hzSecCfg);
      }
      else
      {
         /*127802*/
         /* ccpu00129736 fix for NHU error indication print 
                  during re-establishment */ 
         wrUmmRrcFillSecCfg(&(ueCfg->secCfg), ueCb->secCfg);
      }
   }

   /* UE Info Configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
   {
      ueCfg->ueInfo.newCrnti = newCrnti;
   }

   RETVALUE(ROK);
} /*-- End of wrUmmFillNhuUeCfgInfo --*/
#endif


/**
 *  @brief This function builds the SRB config Info
 *
 *      Function: wrUmmRrcAddSrbCfg
 *
 *          Processing steps:
 *          - Into the given SrbCfg pointer
 *          - update the logical channel info
 *          - update the DL RLC cfg info
 *          - update the UL RLC cfg info
 *          - update the PDCP cfg info
 *
 *
 *  @param [out] srbCfg  : pointer for the updating the DRB config Info
 *  @param [in]  isSrb1  : is the configuration to be done for SRB1 or SRB2
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */


PUBLIC S16 wrUmmRrcAddSrbCfg
(
U16                          cellId,
NhuRbCfgInfo                 *srbCfg,
U8                           isSrb1
)
{
   U8                        qci;
   U8                        lcId;
   U8                        srbId;
   WrRabCfgParams            *params;
   WrPdcpCfgParams           *pdcp; 
   WrRlcCfgParams            *rlc;
   WrUmmCellCb               *ummCellCb = NULLP;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   if (isSrb1 == TRUE)
   {
      qci   = WR_SRB1_QCI;
      lcId  = WR_PDCP_ID_SRB1;
      srbId = WR_PDCP_ID_SRB1;
   }
   else
   {
      qci   = WR_SRB2_QCI;
      lcId  = WR_PDCP_ID_SRB2;
      srbId = WR_PDCP_ID_SRB2;
   }
   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);

   /* DRB related inforation that is set at RLC and PDCP layers           */
   srbCfg->rbCfgBitMsk                 = (NHU_RB_CFG_UL | NHU_RB_CFG_DL); 
   srbCfg->rbId.rbId                   = srbId;
   srbCfg->rbId.rbTyp                  = CM_LTE_SRB;
   srbCfg->lcId.lcId                   = lcId;
   srbCfg->lcId.logChType              = (CmLteLcType)CM_LTE_LCH_DCCH;
   /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
   srbCfg->qci                         = 1;
#endif

   /* UL related configuration - logical channel mapping and RLC info     */
   srbCfg->ulRbCfg.ulRbBitMsk          = (NHU_RB_CFG_LOGCH_MAP_UL);
   srbCfg->ulRbCfg.ulRbBitMsk         |= (NHU_RB_CFG_RLC_MODE_UL);
   srbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;
   wrUmmRrcFillUlRbRlcCfg(&(srbCfg->ulRbCfg.ulRlcMode), params, FALSE);

   /* DL related configuration - logical channel mapping and RLC info     */
   srbCfg->dlRbCfg.dlRbBitMsk          = (NHU_RB_CFG_LOGCH_MAP_DL); 
   srbCfg->dlRbCfg.dlRbBitMsk         |= (NHU_RB_CFG_RLC_MODE_DL);
   srbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
   wrUmmRrcFillDlRbRlcCfg(&(srbCfg->dlRbCfg.dlRlcMode), params);

   /* PDCP Cfg */
   srbCfg->rbCfgBitMsk                |= (NHU_RB_CFG_PDCP_INFO);
   wrUmmRrcFillRbPdcpCfg(&(srbCfg->pdcpCfg), pdcp);

   RETVALUE(ROK);
} /* wrUmmRrcAddSrbCfg */

/**
 *  @brief This function fills UeCfgInfo in event structure
 *
 *      Function: wrUmmRrcAddSrb1Cfg
 *
 *          Processing steps:
 *          - fill up the ue configuraiton info for SRB connection 
 *            establishment
 *          - allocate memory for SRB config info
 *          - fill the SRB config info
 *
 *
 *  @param [in]  evnt  : pointer to memCb
 *  @param [out] ueCfg : pointer to UE config info 
 *  @param [in]  ueCb  : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */ 
PUBLIC S16 wrUmmRrcAddSrb1Cfg
(
Ptr                          evnt,
NhuUeCfgInfo                 *ueCfg,
WrUeCb                       *ueCb
)
{
   NhuRbCfgList              *rbCfgList = NULLP;

   RLOG0(L_DEBUG, "Construct UE Cfg Related Info");

   ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
   rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
   rbCfgList->numOfRbCfgInfo = 1;
   WR_GET_MEM(evnt, sizeof(NhuRbCfgInfo), &(rbCfgList->rbCfgInfoList));
   wrUmmRrcAddSrbCfg(ueCb->cellId, rbCfgList->rbCfgInfoList, TRUE);

   /* Fill up the UE configuration. This is part of conn establishment    */
   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_UE_CFG;
   ueCfg->ueInfo.newCrnti = ueCb->crnti;

   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_TRCH_CFG;
   wrUmmRrcFillTrchCfg(&(ueCfg->trChCfg),ueCb);
   RETVALUE(ROK);
}


/**
 *  @brief This function fills NHU Event Header
 *
 *      Function: wrUmmFillRrcEvntHdr
 *
 *          Processing steps:
 *          - update teh NHU header
 *
 *
 *  @param [out] nhuHdr  : pointer to NHU header
 *  @param [in]  cellId  : cell ID
 *  @param [in]  crnti   : RNTI of the UE
 *  @param [in]  transId : Transaction ID
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmFillRrcEvntHdr
(
NhuHdr      *nhuHdr,
U16         cellId,
U16         crnti,
U32         transId
)
{

   TRC2(wrUmmFillRrcEvntHdr);

   RLOG0(L_DEBUG, "Populate the NHU header info");

   nhuHdr->cellId  = cellId;
   nhuHdr->ueId    = crnti;
   nhuHdr->transId = transId;

   RETVALUE(ROK);
} /* End of wrUmmFillRrcEvntHdr */


/* ************************************************************************* */
/*               UE Capability Related  Filling Functions                    */
/* ************************************************************************* */

/**
 *  @brief This function fills Ue Capability Enquiry info
 *
 *      Function: wrUmmRrcFillUeCapEnq
 *
 *          Processing steps:
 *          - obtain the piointer for the UE cap enquiry message
 *          - update teh required Information
 *
 *
 *  @param [out] rrcDatReqSdu : pointer to DATA Req Sdu
 *  @param [in]  transId      : transaction ID
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmRrcFillUeCapEnq
(
NhuDatReqSdus                *rrcDatReqSdu,
WrUmmRatLst                  *ratLst,
U32                          transId
)
{
   NhuDL_DCCH_Msg            *dlMsg     = NULLP;
   NhuUECapEnquiry           *ueCapEnq  = NULLP;
   NhuUE_CapRqst             *ueCapReq  = NULLP;
   U8                         ratIdx    = 0;

   RLOG0(L_DEBUG, "Building UE Capability Enquiry");

   rrcDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   rrcDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

   dlMsg = &(rrcDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_UECAPENQUIRY);

   /* UeCapabilityEnquiry */
   ueCapEnq = &(dlMsg->message.val.c1.val.ueCapEnquiry);
   wrFillTknU8(&ueCapEnq->pres, PRSNT_NODEF);
   wrFillTknU32(&ueCapEnq->rrc_TrnsxnId, transId);
   wrFillTknU8(&ueCapEnq->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&ueCapEnq->criticalExtns.val.c1.choice, C1_UECAPENQUIRY_R8);
   wrFillTknU8(&ueCapEnq->criticalExtns.val.c1.val.ueCapEnquiry_r8.pres,
                                                                PRSNT_NODEF);

   /* UeCapInfo */
   ueCapReq = &(ueCapEnq->criticalExtns.val.c1.val.ueCapEnquiry_r8.ue_CapRqst);

   /* Get the numComp form ratLst */
   wrFillTknU16(&ueCapReq->noComp, ratLst->noComp);

   /* Allocate memory required for num components */
   WR_GET_MEM(rrcDatReqSdu, sizeof(NhuRAT_Typ)*ueCapReq->noComp.val,
                                                         &(ueCapReq->member));
   if (ueCapReq->member == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* fill ratType */
   for ( ratIdx = 0; ratIdx < ratLst->noComp; ratIdx++ ) 
   {
     wrFillTknU32(&ueCapReq->member[ratIdx], ratLst->ratType[ratIdx]);
   }
   RETVALUE(ROK);
} /* End of wrUmmRrcFillUeCapEnq */

/**
 *  @brief This function fills Ue RadioAccess Cap info for RRC Encoding.
 *
 *      Function: wrUmmRrcFillUeRadAccCapInfo
 *
 *          Processing steps:
 *          - fill the UE radio Access Capability info
 *
 *
 *  @param [in]  encodedInfo : encoded capability info
 *  @param [out] encSdu      : pointer in which the info to be encoded
 *  @param [in]  ueCb        : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcFillUeRadAccCapInfo
(
TknStrOSXL                   *encodedInfo,
NhuEncReqSdus                *encSdu,
WrUeCb                       *ueCb
)
{
   NhuUERadioAccessCapInfo         *ueRACapInfo     = NULLP;
   NhuUERadioAccessCapInfo_r8_IEs  *radioCap  = NULLP;

   RLOG0(L_DEBUG, "Building Radio Access Capability Info");
   
   ueRACapInfo = &encSdu->sdu.u.ueRadioCapInfo;
   wrFillTknU8(&ueRACapInfo->pres,PRSNT_NODEF);
   wrFillTknU8(&ueRACapInfo->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&ueRACapInfo->criticalExtns.val.c1.choice,
                                            C1_UERADIOACCESSCAPINFO_R8);
   radioCap = &(ueRACapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8);
   wrFillTknU8(&radioCap->pres,PRSNT_NODEF);
   wrFillTknStrOSXL(&(radioCap->ue_RadioAccessCapInfo), encodedInfo->len,
                                           encodedInfo->val, &(encSdu->memCp)); 

   RETVALUE(ROK);
}

/**
 *  @brief This function fills Security Mode Command message
 *
 *      Function: wrUmmRrcFillSecModeCmd
 *
 *          Processing steps:
 *          - obtain the pointer of the dlCcch Message
 *          - update the security Mode command Info
 *
 *
 *  @param [out] transId      : Transaction ID
 *  @param [in]  secCfg       : pointer to UE securityu Info
 *  @param [in]  rrcDatReqSdu : RRC Data Req Messge, to be updated with security
 *                             info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcFillSecModeCmd
(
U32                          transId,
WrUeSecCfg                   *secCfg,
NhuDatReqSdus                *rrcDatReqSdu,
U32                          ueRlsVer
)
{
   NhuDL_DCCH_Msg            *dlMsg;
   NhuSecModeCmd             *secModeCmd;
   NhuSecConfigSMC           *securityCfg;
   NhuSecModeCmd_r8_IEs      *secModeR8;
   NhuSecAlgorithmConfig     *secAlgo;

   RLOG0(L_DEBUG, "Building Security Mode Command.");

   if (NULLP == rrcDatReqSdu)
   {
      RLOG0(L_ERROR, "invalid parameters");
      RETVALUE(RFAILED);
   }

   rrcDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   rrcDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

   dlMsg = &(rrcDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   WR_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   WR_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C1_SECURITYMODECMD);

   /* SecurityModeCommand  */
   secModeCmd = &(dlMsg->message.val.c1.val.securityModeCmd);
   WR_FILL_TKN_UINT(secModeCmd->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(secModeCmd->rrc_TrnsxnId, transId);
   WR_FILL_TKN_UINT(secModeCmd->criticalExtns.choice, CRITICALEXTNS_C1);
   WR_FILL_TKN_UINT(secModeCmd->criticalExtns.val.c1.choice,
                                                 C1_SECURITYMODECMD_R8);

   /* Security mode release 8 parameters */
   secModeR8 = &secModeCmd->criticalExtns.val.c1.val.securityModeCmd_r8;
   WR_FILL_TKN_UINT(secModeR8->pres, PRSNT_NODEF);

   /* SecurityConfigSMC */
   securityCfg = &(secModeR8->securityConfigSMC);
   secAlgo = &(securityCfg->securityAlgorithmConfig);
   WR_FILL_TKN_UINT(securityCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(secAlgo->pres, PRSNT_NODEF);
   
   WR_FILL_TKN_UINT(secAlgo->cipheringAlgorithm, secCfg->ciphAlgo);
   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO && ueRlsVer == NhuAccessStratumRlsrel8Enum)
   {
      WR_FILL_TKN_UINT(secAlgo->integrityProtAlgorithm, NhuSecAlgorithmConfigintegrityProtAlgorithmspare1Enum);
   }
   else
   {
      WR_FILL_TKN_UINT(secAlgo->integrityProtAlgorithm, secCfg->intgAlgo);
   }

   RETVALUE(ROK);
} /* End of wrUmmRrcFillSecModeCmd */

/**
 *  @brief This function buids the RRC connection reconfgiuration message 
 *         of the UE
 *
 *      Function: wrUmmRrcCreateRrcReconfigMsg
 *
 *          Processing steps:
 *          - fill the reconfig info to the given RRC data request pointer
 *
 *
 *  @param [out] recfgMsg     : RRC data request message to be updated with
 *                             reconfig Info
 *  @param [in]  recfgTransId : transaction Id
 *  @return  ROK
 */
PUBLIC S16 wrUmmRrcCreateRrcReconfigMsg
(
NhuDatReqSdus                *recfgMsg,
U8                           recfgTransId
)
{
   NhuDL_DCCH_Msg            *dcchMsg = NULLP;
   NhuRRCConRecfgn           *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs    *recfgIEs = NULLP;

   /* Populating NHU Dat Req                                              */
   recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   dcchMsg = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   recfgMsg->sdu.ueMsg.isMsgDed = TRUE;
   wrFillTknU8(&dcchMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dcchMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dcchMsg->message.val.c1.choice, C1_RRCCONRECFGN);

   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   wrFillTknU8(&rrcConReCfg->pres, PRSNT_NODEF);
   wrFillTknU32(&rrcConReCfg->rrc_TrnsxnId, recfgTransId);
   wrFillTknU8(&rrcConReCfg->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&rrcConReCfg->criticalExtns.val.c1.choice, C1_RRCCONRECFGN_R8);
   recfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   wrFillTknU8(&recfgIEs->pres, PRSNT_NODEF);

   /* Set all the IEs as not present. Based on the need individual ones   */
   /* IEs will be populated by subsequent functions                       */
   recfgIEs->measConfig.pres.pres                     = NOTPRSNT;
   recfgIEs->mobilityCntrlInfo.pres. pres             = NOTPRSNT;
   recfgIEs->dedicatedInfoNASLst.noComp.pres          = NOTPRSNT;
   recfgIEs->radioResourceConfigDedicated.pres.pres   = NOTPRSNT;
   recfgIEs->securityConfigHO.pres.pres               = NOTPRSNT;
   recfgIEs->nonCriticalExtn.pres.pres                = NOTPRSNT;

   RETVALUE(ROK);
}


/**
 *  @brief This function initializes the NHU datd req message for the RRC Radio
 *         Resource dedicated message
 *
 *      Function: wrUmmRrcPrepRadioResourceDed
 *
 *          Processing steps:
 *          - Initialize allm teh related params to present fieb to NOTPRSNT 
 *
 *
 *  @param [out] radioRes : pointer to Radio resource config dedicated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcPrepRadioResourceDed
(
NhuRadioResourceConfigDedicated *radioRes
)
{
   wrFillTknU8(&radioRes->pres, PRSNT_NODEF);
   radioRes->srb_ToAddModLst.noComp.pres = NOTPRSNT;
   radioRes->drb_ToAddModLst.noComp.pres = NOTPRSNT;
   radioRes->drb_ToRlsLst.noComp.pres = NOTPRSNT;
   radioRes->mac_MainConfig.choice.pres = NOTPRSNT;
   radioRes->mac_MainConfig.val.explicitValue.pres.pres = NOTPRSNT;
   radioRes->sps_Config.pres.pres = NOTPRSNT;
   radioRes->physicalConfigDedicated.pres.pres = NOTPRSNT;
   RETVALUE(ROK);
}

/**
 *  @brief This function fill the SRB IE
 *
 *      Function: wrUmmRrcFillSrbIE
 *
 *          Processing steps:
 *          - update the SRB IE presence flag
 *          - fill the SRB IE fields
 *
 *  @param [out] srbIe  : pointer to SRB IE
 *  @param [in]  isSrb1 : whether the IE is for SRB1 or SRB2
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmRrcFillSrbIE
(
U16                          cellId,
NhuSRB_ToAddMod              *srbIe,
U8                           isSrb1
)
{
   U8                                   lchId;
   U8                                   rbId;
   U8                                   qci;
   WrRabCfgParams                       *params;
   WrPdcpCfgParams                      *pdcp;
   WrRlcCfgParams                       *rlc;
   WrLchCfgParams                       *lch;
   NhuLogChannelConfigul_SpecificParams *chCfgParams = NULLP;
   NhuRLC_Configam                      *rlcAm;
   NhuLogChannelConfig                  *srbLchCfg;
   WrUmmCellCb                          *ummCellCb = NULLP;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }


   if (isSrb1 == TRUE)
   {
      lchId = WR_PDCP_ID_SRB1;
      rbId  = WR_PDCP_ID_SRB1;
      qci   = WR_SRB1_QCI;
   }
   else
   {
      lchId = WR_PDCP_ID_SRB2;
      rbId  = WR_PDCP_ID_SRB2;
      qci   = WR_SRB2_QCI;
   }
   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);
   lch    = &(params->lchParams);
   wrFillTknU8(&srbIe->pres, PRSNT_NODEF);
   wrFillTknU32(&srbIe->srb_Identity, rbId);

   /* Fill RLC related params */
   wrFillTknU8(&srbIe->rlc_Config.choice, 0);
   wrFillTknU8(&srbIe->rlc_Config.val.explicitValue.choice, 0);
   rlcAm  = &srbIe->rlc_Config.val.explicitValue.val.am;
   wrFillTknU8(&rlcAm->pres, PRSNT_NODEF);
   wrFillTknU8(&rlcAm->ul_AM_RLC.pres, PRSNT_NODEF);
   wrFillTknU32(&rlcAm->ul_AM_RLC.t_PollRetransmit, 
         rlc->u.am.ul.pollRetxTmr);
   wrFillTknU32(&rlcAm->ul_AM_RLC.pollPDU, rlc->u.am.ul.pollPdu);
   wrFillTknU32(&rlcAm->ul_AM_RLC.pollByte, rlc->u.am.ul.pollByte);
   wrFillTknU32(&rlcAm->ul_AM_RLC.maxRetxThreshold, 
         rlc->u.am.ul.maxRetxThrsh);
   wrFillTknU8(&rlcAm->dl_AM_RLC.pres, PRSNT_NODEF);
   wrFillTknU32(&rlcAm->dl_AM_RLC.t_Reordering, rlc->u.am.dl.reorderingTmr);
   wrFillTknU32(&rlcAm->dl_AM_RLC.t_StatusProhibit, 
         rlc->u.am.dl.statusProhTmr);

   /* TODO: THe following portion of the code needs correction            */
   srbLchCfg = &(srbIe->logicalChannelConfig.val.explicitValue);
   wrFillTknU8(&srbIe->logicalChannelConfig.choice, 0);
   wrFillTknU8(&srbLchCfg->pres, PRSNT_NODEF);
   chCfgParams = &(srbLchCfg->ul_SpecificParams);
   wrFillTknU8(&chCfgParams->pres, PRSNT_NODEF);
   wrFillTknU32(&chCfgParams->priority, lch->priority);
   wrFillTknU32(&chCfgParams->prioritisedBitRate, lch->bitRate);
   wrFillTknU32(&chCfgParams->bucketSizeDuration, lch->bucketSizeDur);
   wrFillTknU32(&chCfgParams->logicalChannelGroup, lch->lchGrp);

   RETVALUE(ROK);
} /* end of wrUmmRrcFillSrbIE */ 

/** @brief This function fills DRB information for 
 *         reconfiguration to RRC layer. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It prepares RRC reconfigution message
 *           with all the information required to
 *           reconfigure DRB based on new QCI. 
 *
 *
 * @param NhuDRB_ToAddMod          *drbIe 
 * @param WrUmmRabModifyRabInfo    *rabInfo
 * @return ROK/RFAILED 
 */ 
PUBLIC S16 wrUmmRrcReconfigFillModDrb
(
 WrUeCb                       *ueCb,
 Bool                         isUeUlSpsEnabled,
 NhuDRB_ToAddMod              *drbIe,
 U8                           erabId,
 U8                           drbId,
 U8                           lchId,
 U8                           qci
)
{
   WrRabCfgParams              *params;
   WrPdcpCfgParams             *pdcp; 
   WrRlcCfgParams              *rlc; 
   WrLchCfgParams              *lch;
   NhuLogChannelConfigul_SpecificParams *chCfgParams = NULLP;
   WrUmmCellCb                          *ummCellCb = NULLP;

   WR_UMM_GET_CELLCB(ummCellCb, ueCb->cellId);

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);
   lch    = &(params->lchParams);
   wrFillTknU8(&drbIe->pres, PRSNT_NODEF);
  // wrFillTknU32(&drbIe->eps_BrIdentity, erabId);
   wrFillTknU32(&drbIe->drb_Identity, drbId);


   /* Fill RLC related params */
   if (params->rlcMode == WR_RLC_AM_MODE)
   {
      NhuRLC_Configam        *rlcAm = &drbIe->rlc_Config.val.am;
      wrFillTknU8(&drbIe->rlc_Config.choice, RLC_CONFIG_AM);
      wrFillTknU8(&rlcAm->pres, PRSNT_NODEF);
      wrFillTknU8(&rlcAm->ul_AM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcAm->ul_AM_RLC.t_PollRetransmit, 
                                                   rlc->u.am.ul.pollRetxTmr);
      wrFillTknU32(&rlcAm->ul_AM_RLC.pollPDU, rlc->u.am.ul.pollPdu);
      wrFillTknU32(&rlcAm->ul_AM_RLC.pollByte, rlc->u.am.ul.pollByte);
      wrFillTknU32(&rlcAm->ul_AM_RLC.maxRetxThreshold, 
                                                  rlc->u.am.ul.maxRetxThrsh);
      wrFillTknU8(&rlcAm->dl_AM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcAm->dl_AM_RLC.t_Reordering, rlc->u.am.dl.reorderingTmr);
      wrFillTknU32(&rlcAm->dl_AM_RLC.t_StatusProhibit, rlc->u.am.dl.statusProhTmr);
      RLOG2(L_DEBUG, "RLC AM mode is being configured statusProh[%ld] "
         "reordering[%ld]", rlcAm->dl_AM_RLC.t_StatusProhibit.val,
         rlcAm->dl_AM_RLC.t_Reordering.val);
   }
   else
   {
      NhuRLC_Configum_Bi_Directional *rlcUmBiDir = &drbIe->rlc_Config.
         val.um_Bi_Directional;

      wrFillTknU8(&drbIe->rlc_Config.choice, RLC_CONFIG_UM_BI_DIRECTIONAL);
      wrFillTknU8(&rlcUmBiDir->pres, PRSNT_NODEF);
      wrFillTknU8(&rlcUmBiDir->ul_UM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcUmBiDir->ul_UM_RLC.sn_FieldLength, rlc->u.um.ul.snFldLen);
      wrFillTknU8(&rlcUmBiDir->dl_UM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.sn_FieldLength, rlc->u.um.dl.snFldLen );
      wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.t_Reordering, rlc->u.um.dl.tReordering);
      RLOG0(L_DEBUG, "RLC UM mode is being configured");
   }

   /* TODO: THe following portion of the code needs correction            */
 //  wrFillTknU32(&drbIe->logicalChannelIdentity, lchId);
   wrFillTknU8(&drbIe->logicalChannelConfig.pres, PRSNT_NODEF);
   chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
   wrFillTknU8(&chCfgParams->pres, PRSNT_NODEF);
   wrFillTknU32(&chCfgParams->priority, lch->priority);
   wrFillTknU32(&chCfgParams->bucketSizeDuration, lch->bucketSizeDur);

   RLOG4(L_DEBUG, "LCID [%u], UE SR MASK[%u], isUeUlSpsEnabled [%u], UE Rel [%u]", \
         lchId, ueCb->isSrMaskEnabled, isUeUlSpsEnabled, \
         ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
   if ((ueCb->isSrMaskEnabled == TRUE) && (isUeUlSpsEnabled == TRUE) && \
        (ueCb->ueCap->eUTRA_Cap.accessStratumRls.val >= NhuAccessStratumRlsrel9Enum))
   {
      RLOG1(L_DEBUG, "Setting Logical Channel SR mask Logical Channel Id [%u]", lchId);
      wrFillTknU8(&drbIe->logicalChannelConfig.extaddgrp_1.pres, PRSNT_NODEF);
      wrFillTknU32(&drbIe->logicalChannelConfig.extaddgrp_1.logicalChannelSR_Mask_r9, \
            NhuLogChannelConfigextaddgrp_1logicalChannelSR_Mask_r9setupEnum);
   }

   wrFillTknU32(&chCfgParams->prioritisedBitRate, lch->bitRate);
   wrFillTknU32(&chCfgParams->logicalChannelGroup, lch->lchGrp);

   RETVALUE(ROK);
}


/**
 *  @brief This function fill UL prioritisedBitRate info in the Ue reconfiguration 
 *         information message for the UE for the GBR services based on the GBR
 *         data rate configuration of the service.PBR will be selected such that it
 *         is nearest to the configured GBR values.
 *
 *      Function: wrUmmRrcReconfigFillulPbr
 *
 *
 *
 *  @param [out] drbIe            : pointer to the DRB addMod list in reconfigurationm
 *                                  message
 *  @param [in]  ulGbr            : Configured Ul GBR value
 *  @param [in]  qci              : QCI configuration.
 *  @param [in]  isUlSpsEnabled   : UL SPS enabled flag
 *  @return 
 */
PUBLIC Void wrUmmRrcReconfigFillulPbr
(
NhuDRB_ToAddMod            *drbIe,
U32                         ulGbr,
U32                         qci,
Bool                        isUlSpsEnabled
)
{
   NhuLogChannelConfigul_SpecificParams *chCfgParams = NULLP;
   LwrPrioritisedBitRate  prioBitRate = LTE_PBR_KBPS8;

   
   if (qci <= 4  && isUlSpsEnabled == FALSE)
   {
      chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
      wrFillTknU8(&chCfgParams->pres, PRSNT_NODEF);
      /* We are CEILING of the date rate to next higher PBR if the 
       * ulGbr value is more than the mid value of the standard defined
       * PBR values. If it is less then the mid value we FLOOR it 
       * to the next lower PBR rate.
       * ie: Mid value of the PBR 256_kBps and 128_kBps is 192_kBps
       * if the ulGbr is less than 192_kBps and more than 96_kBps
       * we configure PBR as 128_kBps and so on.
       */
      if(ulGbr >= 192000) 
      {
          prioBitRate = LTE_PBR_KBPS256; 
      }
      else if(ulGbr >= 96000 && ulGbr < 192000)
      {
          prioBitRate = LTE_PBR_KBPS128; 
      }
      else if(ulGbr >= 48000 && ulGbr < 96000 )
      {
          prioBitRate = LTE_PBR_KBPS64; 
      }
      else if(ulGbr >= 24000 && ulGbr < 48000 )
      {
          prioBitRate = LTE_PBR_KBPS32; 
      }
      else if(ulGbr >= 12000 && ulGbr < 24000 )
      {
          prioBitRate = LTE_PBR_KBPS16; 
      }
      else
      {
          prioBitRate = LTE_PBR_KBPS8; 
      }
      wrFillTknU32(&chCfgParams->prioritisedBitRate, prioBitRate);
   }

}
/**
 *  @brief This function fill DRB info in the Ue reconfiguration 
 *         information message for the UE
 *
 *      Function: wrUmmRrcReconfigFillDrb
 *
 *          Processing steps:
 *          - fill the PDCP config message
 *          - fill the RLC config Info
 *          - fill the logical channel info
 *
 *
 *  @param [out] drbIe   : pointer to the DRB addMod list in reconfigurationm
 *                        message
 *  @param [in]  erabId  : Radio bearer Id
 *  @param [in]  drbId   : DRB Id
 *  @param [in]  lchId   : logical Channel Id
 *  @param [in]  qci     : QCI for the bearer
 *  @return 
 */
PUBLIC S16 wrUmmRrcReconfigFillDrb
(
/* SPS changes starts */
WrUeCb                       *ueCb,
Bool                         isUeUlSpsEnabled,
/* SPS changes ends */
NhuDRB_ToAddMod              *drbIe,
U8                           erabId,
U8                           drbId,
U8                           lchId,
U8                           qci,
/* SPS changes starts */
Bool                         isDlSpsEnabled,
Bool                         isUlSpsEnabled
/* SPS changes ends */
)
{
   WrRabCfgParams                       *params;
   WrPdcpCfgParams                      *pdcp; 
   WrRlcCfgParams                       *rlc; 
   WrLchCfgParams                       *lch;
   NhuLogChannelConfigul_SpecificParams *chCfgParams = NULLP;
   WrUmmCellCb                          *ummCellCb = NULLP;

   /* SPS changes starts */
   WR_UMM_GET_CELLCB(ummCellCb, ueCb->cellId);
   /* SPS changes ends */

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);
   lch    = &(params->lchParams);
   wrFillTknU8(&drbIe->pres, PRSNT_NODEF);
   wrFillTknU32(&drbIe->eps_BrIdentity, erabId);
   wrFillTknU32(&drbIe->drb_Identity, drbId);

   /* PDCP-Config */
   wrFillTknU8(&drbIe->pdcp_Config.pres, PRSNT_NODEF);
   wrFillTknU32(&drbIe->pdcp_Config.discardTimer, pdcp->discardTimer);
   if (!pdcp->hdrCompress)
   {
      wrFillTknU8(&drbIe->pdcp_Config.headerCompression.choice,
                                             HEADERCOMPRESSION_NOTUSED);
   }
   else
   { 
      /*127802*/
      RLOG0(L_INFO, "Header compression is turned on");
      RETVALUE(RFAILED);
   }

   if (params->rlcMode == WR_RLC_AM_MODE)
   {
      wrFillTknU8(&drbIe->pdcp_Config.rlc_AM.pres, PRSNT_NODEF);
      wrFillTknU8(&drbIe->pdcp_Config.rlc_AM.statusReportReqd, 
                                                  pdcp->statusRprtReqd);
   }
   else
   {
      wrFillTknU8(&drbIe->pdcp_Config.rlc_UM.pres, PRSNT_NODEF);
      wrFillTknU32(&drbIe->pdcp_Config.rlc_UM.pdcp_SN_Size, pdcp->pdcpSnSize);      
   }

   /* Fill RLC related params */
   if (params->rlcMode == WR_RLC_AM_MODE)
   {
      NhuRLC_Configam        *rlcAm = &drbIe->rlc_Config.val.am;
      wrFillTknU8(&drbIe->rlc_Config.choice, RLC_CONFIG_AM);
      wrFillTknU8(&rlcAm->pres, PRSNT_NODEF);
      wrFillTknU8(&rlcAm->ul_AM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcAm->ul_AM_RLC.t_PollRetransmit, 
                                                   rlc->u.am.ul.pollRetxTmr);
      wrFillTknU32(&rlcAm->ul_AM_RLC.pollPDU, rlc->u.am.ul.pollPdu);
      wrFillTknU32(&rlcAm->ul_AM_RLC.pollByte, rlc->u.am.ul.pollByte);
      wrFillTknU32(&rlcAm->ul_AM_RLC.maxRetxThreshold, 
                                                  rlc->u.am.ul.maxRetxThrsh);
      wrFillTknU8(&rlcAm->dl_AM_RLC.pres, PRSNT_NODEF);
      if (isDlSpsEnabled == TRUE)
      {
         wrFillTknU32(&rlcAm->dl_AM_RLC.t_Reordering, NhuT_Reorderingms20Enum);
      }
      else
      {
         wrFillTknU32(&rlcAm->dl_AM_RLC.t_Reordering, rlc->u.am.dl.reorderingTmr);
      }
      wrFillTknU32(&rlcAm->dl_AM_RLC.t_StatusProhibit, rlc->u.am.dl.statusProhTmr);
      /*127802*/
      RLOG2(L_DEBUG, "RLC AM mode is being configured. statusProh[%ld] "
          "reordering[%ld]", rlcAm->dl_AM_RLC.t_StatusProhibit.val,
          rlcAm->dl_AM_RLC.t_Reordering.val);
   }
   else
   {
      NhuRLC_Configum_Bi_Directional *rlcUmBiDir = &drbIe->rlc_Config.
         val.um_Bi_Directional;

      wrFillTknU8(&drbIe->rlc_Config.choice, RLC_CONFIG_UM_BI_DIRECTIONAL);
      wrFillTknU8(&rlcUmBiDir->pres, PRSNT_NODEF);
      wrFillTknU8(&rlcUmBiDir->ul_UM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcUmBiDir->ul_UM_RLC.sn_FieldLength, rlc->u.um.ul.snFldLen);
      wrFillTknU8(&rlcUmBiDir->dl_UM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.sn_FieldLength, rlc->u.um.dl.snFldLen );
      if (isDlSpsEnabled == TRUE)
      {
         wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.t_Reordering, NhuT_Reorderingms20Enum);
      }
      else
      {
         wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.t_Reordering, rlc->u.um.dl.tReordering);
      }
      /*127802*/
      RLOG0(L_DEBUG, "RLC UM mode is being configured");
   }

   /* TODO: THe following portion of the code needs correction            */
   wrFillTknU32(&drbIe->logicalChannelIdentity, lchId);
   wrFillTknU8(&drbIe->logicalChannelConfig.pres, PRSNT_NODEF);
   chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
   wrFillTknU8(&chCfgParams->pres, PRSNT_NODEF);
   wrFillTknU32(&chCfgParams->priority, lch->priority);
   wrFillTknU32(&chCfgParams->prioritisedBitRate, lch->bitRate);
   wrFillTknU32(&chCfgParams->bucketSizeDuration, lch->bucketSizeDur);
   /* SPS changes starts */
   RLOG4(L_DEBUG, "wrUmmRrcReconfigFillDrb: lchId [%u] ueCb->isSrMaskEnabled [%u], isUeUlSpsEnabled [%u], accessStratumRls [%u]", \
         lchId, ueCb->isSrMaskEnabled, isUeUlSpsEnabled, ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
   if ((ueCb->isSrMaskEnabled == TRUE) && (isUeUlSpsEnabled == TRUE) && \
         (ueCb->ueCap->eUTRA_Cap.accessStratumRls.val >= NhuAccessStratumRlsrel9Enum))
   {
      wrFillTknU8(&drbIe->logicalChannelConfig.extaddgrp_1.pres, PRSNT_NODEF);
      wrFillTknU32(&drbIe->logicalChannelConfig.extaddgrp_1.logicalChannelSR_Mask_r9, \
            NhuLogChannelConfigextaddgrp_1logicalChannelSR_Mask_r9setupEnum);
   }
   if (isUlSpsEnabled == TRUE)
   {
      wrFillTknU32(&chCfgParams->prioritisedBitRate, \
            NhuLogChannelConfigul_SpecificParamsprioritisedBitRateinfinityEnum);
      wrFillTknU32(&chCfgParams->logicalChannelGroup, 1);
   }
   else
   {
      wrFillTknU32(&chCfgParams->prioritisedBitRate, lch->bitRate);
      wrFillTknU32(&chCfgParams->logicalChannelGroup, lch->lchGrp);
   }
   /* SPS changes ends */

   RETVALUE(ROK);
}

/**
 *  @brief This function build the dedicate NAS Info
 *
 *      Function: wrUmmRrcDedNASInfo
 *
 *          Processing steps:
 *          - Allocate memory for the Dedicated NAS Info
 *          - update the NAS infornmation
 *
 *
 *  @param [in] evnt     : pointer to memCb
 *  @param [in] reCfgIes : pointer to reconfig IEs
 *  @param [in] nasPdu   : pointer to the nasPdu
 *  @return 
 */
PUBLIC S16 wrUmmRrcDedNASInfo
(
NhuDatReqSdus                *evnt,
NhuRRCConRecfgn_r8_IEs       *reCfgIes,
TknStrOSXL                   *nasPdu
)
{
   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *nasLst = NULLP;
   U8                                        nasIdx = 0;

   if(NULLP == nasPdu)
   {
      RETVALUE(ROK);
   }   
   RLOG0(L_DEBUG, "Building RRC ReCfg Msg - ERAB Rel");
      /* NAS PDU list; Only NAS PDU would exist */
   nasLst = &(reCfgIes->dedicatedInfoNASLst);

   nasLst->noComp.pres = PRSNT_NODEF;
   nasLst->noComp.val = nasIdx + 1;
   /* Allocating memory for TknStrOSXL */
   WR_GET_MEM(evnt, (sizeof(NhuDedicatedInfoNAS)),&nasLst->member);

   /* Allocate memory for the octect string */
   WR_GET_MEM(evnt, nasPdu->len,&(nasLst->member[nasIdx].val));
   nasLst->member[nasIdx].pres = PRSNT_NODEF;
   nasLst->member[nasIdx].len  = nasPdu->len;
   /* Copy Byte by Byte */
   cmMemcpy((U8 *)nasLst->member[nasIdx].val, (U8 *)nasPdu->val, (nasPdu->len));
   RETVALUE(ROK);
}/*wrUmmRrcDedNASFromERABRelCmd*/

/**
 *  @brief This function fills the Idle mode mobility config Info
 *
 *      Function: wrUmmRrcFillIdleModeMobility
 *
 *          Processing steps:
 *          - fills the enum as TknU32 in the idle mobility control info 
 *            for the timer value received
 *
 *
 *  @param [out] idleModeMobility : Idle mode Mobility Control Info
 *  @param [in]  t320InMin        : t320 min value 
 *  @return None
 */
PRIVATE Void wrUmmRrcFillIdleModeMobility
(
NhuIdleModeMobilityCntrlInfo *idleModeMobility,
U32                          t320InMin
)
{
   if (t320InMin != 0)
   {
      wrFillTknU8(&idleModeMobility->pres, PRSNT_NODEF);
      if (t320InMin <= 5)
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min5Enum);
      }
      else if (t320InMin <= 10)
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min10Enum);
      }
      else if (t320InMin <= 20)
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min20Enum);
      }
      else if (t320InMin <= 30)
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min30Enum);
      }
      else if (t320InMin <= 60)
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min60Enum);
      }
      else if (t320InMin <= 120)
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min120Enum);
      }
      else
      {
         wrFillTknU32(&idleModeMobility->t320, 
                           NhuIdleModeMobilityCntrlInfot320min180Enum);
      }
      RETVOID;
   }
   else
   {
      idleModeMobility->pres.pres = NOTPRSNT;
   }

   RETVOID;
}

/**
 *  @brief This function  builds connection release message
 *
 *      Function: wrUmmRrcBldConRls
 *
 *          Processing steps:
 *          - obtain the pointer to connection release msg 
 *          - update the connection release messge
 *
 *  @param [in]   accessStratumRls : Access Strtutam Release 
 *  @param [in]   redirectInfo : Redirection information 
 *  @param [out]  nhuDatReqSdu : pointer to Data Request Sdu 
 *  @param [in]   relCause     : Connection release Cause
 *  @param [in]   t320InMin    : t320 timer value
 *  @return 
 */
PUBLIC S16 wrUmmRrcBldConRls
(
WrUeCb                       *ueCb,
U8                           accessStratumRls,
WrUmmRedirectInfo            *redirectInfo,
NhuDatReqSdus                *nhuDatReqSdu,
U32                          relCause,
U32                          t320InMin
)
{
   NhuDL_DCCH_Msg            *dlMsg;
   NhuRRCConRls              *conRls;
   NhuRRCConRls_r8_IEs       *conRlsR8;
   NhuIdleModeMobilityCntrlInfo *idleModeMobility;

   RLOG0(L_DEBUG, "Building RRC Connection Release");

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_RRCCONRLS);

   /* RRCConnectionRelease */
   conRls = &(dlMsg->message.val.c1.val.rrcConRls);
   wrFillTknU8(&conRls->pres, PRSNT_NODEF);
   wrFillTknU32(&conRls->rrc_TrnsxnId, 0);
   wrFillTknU8(&conRls->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&conRls->criticalExtns.val.c1.choice, C1_RRCCONRLS_R8);

   /* RRCConnectionRelease-r8-IEs */
   conRlsR8 = &(conRls->criticalExtns.val.c1.val.rrcConRls_r8);
   wrFillTknU8(&conRlsR8->pres, PRSNT_NODEF);
   wrFillTknU32(&conRlsR8->releaseCause, relCause);

   if(redirectInfo != NULLP)
   {
      RLOG_ARG1(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionRelease"
            " with Redirection & cause [%d]", relCause);
      /* RRCConnectionRelease-r8-IEs Redirect information*/
      wrUmmFillReDirectInfoToRrcRelMsg(ueCb, accessStratumRls, conRlsR8, 
                                               redirectInfo, nhuDatReqSdu);
   }
    /* IdleModeMobilityCntrlInfo */
   if(redirectInfo == NULLP)
   {
      RLOG_ARG1(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionRelease"
            " without Redirection & cause [%lu]", relCause);
      idleModeMobility = &(conRlsR8->idleModeMobilityCntrlInfo);
      wrUmmRrcFillIdleModeMobility(idleModeMobility, t320InMin);
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function builds the RRC Connection Release message
 *
 *      Function: wrUmmRrcBldReestabReject
 *
 *          Processing steps:
 *          - update the pointer to reestablishment reject message
 *
 *
 *  @param [out] reestabRejSdu : pointer to reestablishment reject message
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcBldReestabReject
(
NhuDatRspSdus                *reestabRejSdu
)
{
   NhuDL_CCCH_Msg            *dlMsg;
   NhuRRCConReestbRej        *reestabRej;
   NhuRRCConReestbRej_r8_IEs *r8_IE;

   RLOG0(L_DEBUG, "Building RRC Reestablishment Reject");

   reestabRejSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(reestabRejSdu->sdu.m.dlCcchMsg.ccchMsg);
   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_CCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_RRCCONREESTBREJ);

   /* RRC Reestablishment Reject */
   reestabRej = &(dlMsg->message.val.c1.val.rrcConReestbRej);
   wrFillTknU8(&reestabRej->pres, PRSNT_NODEF);
   wrFillTknU8(&reestabRej->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&reestabRej->criticalExtns.choice, C1_RRCCONREESTB_R8);

   /* RRC Reestablishment-r8-IEs */
   r8_IE = &(reestabRej->criticalExtns.val.rrcConReestbRej_r8);
   r8_IE->pres.pres = PRSNT_NODEF;
   r8_IE->nonCriticalExtn.pres.pres = PRSNT_NODEF;


   RETVALUE(ROK);
} /* end of wrUmmRrcBldReestabReject */ 

/**
 *  @brief This function fills RRC Connection Reestablish Msg
 *
 *      Function: wrUmmRrcBldReestabMsg
 *
 *          Processing steps:
 *          - update the presence flag for Connection Reestablish Msg
 *          - fill the Re-establishment parameters
 *          - update the fields to activate SRB1
 *
 *  @param [out] reestabSdu : pointer to Connection Reestablish Msg
 *  @param [in]  ueCb       : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcBldReestabMsg
(
NhuDatRspSdus                *reestabSdu,
WrUeCb                       *ueCb
)
{
   NhuDL_CCCH_Msg                  *dlMsg     = NULLP;
   NhuRRCConReestb                 *conReestb = NULLP;
   NhuRRCConReestb_r8_IEs          *reestabIe = NULLP;
   NhuRadioResourceConfigDedicated *dedRadRes = NULLP;
   U8                              isSrb1     = TRUE;
   NhuUeCfgInfo           *ueCfg;
/*Removed DRX flag*/
   NhuDRX_Config        *drxCfgIe

   TRC2(wrUmmFillRrcReEstabSetup);

   RLOG0(L_DEBUG, "Building RRC Reestablishment MSg");

   reestabSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(reestabSdu->sdu.m.dlCcchMsg.ccchMsg);
   WR_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C1);
   WR_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C1_RRCCONREESTB);

   conReestb = &(dlMsg->message.val.c1.val.rrcConReestb);
   WR_FILL_TKN_UINT(conReestb->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(conReestb->rrc_TrnsxnId, WR_VAL_ONE);
   WR_FILL_TKN_UINT(conReestb->criticalExtns.choice, CRITICALEXTNS_C1);

   WR_FILL_TKN_UINT(conReestb->criticalExtns.val.c1.choice, 
                                                   C1_RRCCONREESTB_R8);

   /*Fill Re-establishment parameters*/
   reestabIe = &conReestb->criticalExtns.val.c1.val.rrcConReestb_r8;
   wrFillTknU8(&reestabIe->pres, PRSNT_NODEF);
   wrFillTknU8(&reestabIe->radioResourceConfigDedicated.pres, PRSNT_NODEF);

   /*Activate SRB1 */
   dedRadRes = &(reestabIe->radioResourceConfigDedicated);
   WR_GET_MEM(reestabSdu, sizeof(NhuSRB_ToAddMod), 
                                        &dedRadRes->srb_ToAddModLst.member);
   wrFillTknU16(&(dedRadRes->srb_ToAddModLst.noComp), 1);
   wrUmmRrcFillSrbIE(ueCb->cellId, &dedRadRes->srb_ToAddModLst.member[0], isSrb1);
#if 0
#else
   wrUmmRrcFillMacCfg(&dedRadRes->mac_MainConfig, ueCb);
#endif

/*Removed DRX flag*/
   if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_DRX)
   {   
      drxCfgIe = &dedRadRes->mac_MainConfig.val.explicitValue.drx_Config;
      wrUmmRrcFillDrxCfgIe(drxCfgIe, &ueCb->drxCfg);
   }
/*Removed DRX flag*/

   wrFillRadResPhyCfgDed(dedRadRes, (Ptr)reestabSdu, WR_PDCP_ID_SRB1, ueCb);

   wrFillTknU32(&reestabIe->nextHopChainingCount, ueCb->nhChainCnt);
   wrFillTknU8(&reestabIe->nonCriticalExtn.pres, NOTPRSNT);
      /* Fill Config values */
   reestabSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = &(reestabSdu->sdu.ueCfg);
#ifdef TENB_AS_SECURITY
   /* ccpu00129736 removing duplicate function wrUmmFillNhuUeCfgInfo and 
         renamed properly */
   if(wrUmmFillNhuUeCfgInfo(ueCfg, ueCb,
         reestabSdu->sdu.m.dlCcchMsg.tmpCrnti) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif

   /* RRC-REL10-Upgrade */
   wrFillTknU8(&(dedRadRes->extaddgrp_1.pres), PRSNT_NODEF);
   wrFillTknU8(&(dedRadRes->extaddgrp_1.rlf_TmrsAndConsts_r9.choice),
         RLF_TMRSANDCONSTS_R9_RELEASE);
   dedRadRes->extaddgrp_1.rlf_TmrsAndConsts_r9.val.setup.pres.pres = NOTPRSNT;

  RETVALUE(ROK);
} /* end of wrUmmRrcBldReestabMsg */ 

/** @brief This function is used to Fill paramters for Connection Reject
 *
 * @details
 *
 *     Function: wrUmmRrcFillConRej
 *
 *         Processing steps:
 *         - This function fills RRC parameters for the Reject Message 
 *
 * @param[in, out] nhuDatRspEvntSdu: Incoming RRC Structure 
 * @param[in] waitTime: Wait time before UE can retry for Connection Request 
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrUmmRrcFillConRej
(
 NhuDatRspSdus *nhuDatRspEvntSdu,
 U32           waitTime
)
{
   NhuDL_CCCH_Msg *dlMsg = NULLP;
   NhuRRCConRej *conRej = NULLP;

   TRC2(wrUmmRrcFillConRej);

   RLOG0(L_DEBUG, "Building Connection Reject MSg");

   if(NULLP == nhuDatRspEvntSdu)
   {
      RLOG0(L_ERROR, "nhuDatRspEvntSdu is NULL");

      RETVALUE(RFAILED);
   }

   nhuDatRspEvntSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(nhuDatRspEvntSdu->sdu.m.dlCcchMsg.ccchMsg);

   WR_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C1);
   WR_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C1_RRCCONREJ);

   /*---- RRCConnectionReject --*/
   conRej = &(dlMsg->message.val.c1.val.rrcConRej);
   WR_FILL_TKN_UINT(conRej->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(conRej->criticalExtns.choice, CRITICALEXTNS_C1);

   /*---- RRCConnectionReject-r8-IEs  --*/
   WR_FILL_TKN_UINT(conRej->criticalExtns.val.c1.choice, C1_RRCCONREJ_R8);
   WR_FILL_TKN_UINT(conRej->criticalExtns.val.c1.val.rrcConRej_r8.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(conRej->criticalExtns.val.c1.val.rrcConRej_r8.waitTime,
         waitTime);
   WR_FILL_TKN_UINT(conRej->criticalExtns.val.c1.val.rrcConRej_r8.\
         nonCriticalExtn.pres, NOTPRSNT);

   RETVALUE(ROK);
}

/** @brief This function is used to fill the Dedicated Radio Resource IE
 *
 * @details
 *
 *     Function: wrUmmRrcFillDedRadioCfg
 *
 *         Processing steps:
 *         - fill the srb1 information
 *         - fill the srb2 information
 *         - fill the drb information
 *
 * @param[in, out] ded: Incoming RRC Structure
 * @param[in] ueCb    : UE Control Block 
 * @param[in] memCp   : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrUmmRrcFillDedRadioCfg
(
NhuRadioResourceConfigDedicated *ded,
WrUeCb                          *ueCb,
CmMemListCp                     *memCp,
/* SPS changes starts */
Bool                            isUeUlSpsEnabled
/* SPS changes ends */
)
{
   NhuDRB_ToAddMod              *drbIe = NULLP;
   NhuSRB_ToAddMod              *srbIe = NULLP;
   U8                           idx =0;
   U8                           rbIdx = 0;

   wrUmmRrcPrepRadioResourceDed(ded);

   wrFillTknU16(&ded->srb_ToAddModLst.noComp, ueCb->rbInfo.numOfSrbCfgd);
   WR_GET_MEM(memCp, (sizeof(NhuSRB_ToAddMod) * ueCb->rbInfo.numOfSrbCfgd),
                          &(ded->srb_ToAddModLst.member));

   if (ded->srb_ToAddModLst.member == NULLP)
   {
      RETVALUE(RFAILED);
   }
   for( idx = 0; idx < ueCb->rbInfo.numOfSrbCfgd; idx++)
   {
      srbIe   = ded->srb_ToAddModLst.member + idx;
      if( idx == 0)
      {
         wrUmmRrcFillSrbIE(ueCb->cellId, srbIe, TRUE);       
      }
      else
      {
         wrUmmRrcFillSrbIE(ueCb->cellId, srbIe, FALSE);
      }
   }

   wrFillTknU16(&ded->drb_ToAddModLst.noComp, ueCb->rbInfo.numOfRbCfgd);
   /* Allocate memory to include each RAB into the message towards the UE */
   WR_GET_MEM(memCp, (sizeof(NhuDRB_ToAddMod) * ueCb->rbInfo.numOfRbCfgd),
                          &(ded->drb_ToAddModLst.member));
   if (ded->drb_ToAddModLst.member == NULLP)
   {
      RETVALUE(RFAILED);
   }
   for (idx = 0; (idx < WR_MAX_DRBS) && (rbIdx < ueCb->rbInfo.numOfRbCfgd); idx++)
   {
      if(ueCb->rbInfo.rab[idx].inUse == TRUE)
      {
         /*127802*/
         RLOG3(L_DEBUG, "rbInfo info present:: idx [%d] erabId [%lu] rbId [%d]",
            idx, ueCb->rbInfo.rab[idx].eRABId, ueCb->rbInfo.rab[idx].rbId);
         drbIe   = ded->drb_ToAddModLst.member + rbIdx;
         /* SPS changes starts */
         wrUmmRrcReconfigFillDrb(ueCb, isUeUlSpsEnabled, drbIe, ueCb->rbInfo.rab[idx].eRABId,
               ueCb->rbInfo.rab[idx].rbId ,ueCb->rbInfo.rab[idx].lchId ,
               ueCb->rbInfo.rab[idx].qci, ueCb->rbInfo.rab[idx].isDlSpsEnabled, 
               ueCb->rbInfo.rab[idx].isUlSpsEnabled);
         /* SPS changes ends */
         /* UL PBR changes gbr.ul is in kbps so gbr.ul/8 is done here to get kBps*/
         wrUmmRrcReconfigFillulPbr(drbIe, ueCb->rbInfo.rab[idx].gbr.ul/8,
               ueCb->rbInfo.rab[idx].qci, ueCb->rbInfo.rab[idx].isUlSpsEnabled); 
         rbIdx++;
      }
   }

   /* ccpu00128534 */
   /* MAC Main Config */
   wrFillTknU8(&(ded->mac_MainConfig.choice), 0);
   wrUmmRrcFillMacCfgIe(&(ded->mac_MainConfig.val.explicitValue), ueCb);

   /* PHY Config */
   wrFillRadResPhyCfgDed(ded, (Ptr)memCp, WR_PDCP_ID_SRB1, ueCb);

   /* RRC-REL10-Upgrade */
   wrFillTknU8(&(ded->extaddgrp_1.pres), PRSNT_NODEF);
   wrFillTknU8(&(ded->extaddgrp_1.rlf_TmrsAndConsts_r9.choice),
         RLF_TMRSANDCONSTS_R9_RELEASE);
   ded->extaddgrp_1.rlf_TmrsAndConsts_r9.val.setup.pres.pres = NOTPRSNT;

   RETVALUE(ROK);
}

/** @brief This function is used to fill the AS Context information in
 *         Source to target container.
 *
 * @details
 *
 *     Function: wrUmmRrcFillAsCtxt
 *
 *         Processing steps:
 *         - fill the reestablishment information
 *
 * @param[in] ueCb         : UE Control Block
 * @param[in, out] asCtxt  : Incoming RRC Structure
 * @param[in] memCp        : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PRIVATE S16 wrUmmRrcFillAsCtxt
(
WrUeCb                       *ueCb,
NhuAS_Ctxt                   *asCtxt,
CmMemListCp                  *memCp
)
{
   WrCellCb                  *cellCb;
   U16                       len = 16;
//   U8                        val[2] = {0x01, 0x02};
   U16                      val = 0x0201;

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&asCtxt->pres, PRSNT_NODEF);
   wrFillTknU8(&asCtxt->reestablishmentInfo.pres, PRSNT_NODEF);
   wrFillTknU32(&asCtxt->reestablishmentInfo.sourcePhysCellId,
                        cellCb->physCellId);
   WR_FILL_BIT_STR_VAL(asCtxt->reestablishmentInfo.targetCellShortMAC_I,
                        len, val, memCp);
   asCtxt->reestablishmentInfo.additionalReestabInfoLst.noComp.pres = NOTPRSNT;

   RETVALUE(ROK);
}

/** @brief This function fills MIB IE in Source to Target Container. 
 *
 * @details
 *
 *     Function: wrUmmRrcFillMibForHo
 *
 *         Processing steps:
 *         - This function build MIB message based on MIB parameters in Cell
 *           control block.
 *
 *
 * @param [in]  cellCb   : cell control block.
 * @param [out] memCp    : MIB msg in RRC data request event SDU.
 * @param [in]  schedSFN : scheduled SFN.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PRIVATE S16 wrUmmRrcFillMibForHo
(
WrCellCb                     *cellCb,
NhuMasterInfoBlock           *bcchBchMsg,
CmMemListCp                  *memCp,
U16                          schedSFN
)
{
   WrMibParams               *mib = &cellCb->mib;

   WR_FILL_TKN_UINT(bcchBchMsg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(bcchBchMsg->dl_Bandwidth, mib->dlBw);
   WR_FILL_TKN_UINT(bcchBchMsg->phich_Config.pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(bcchBchMsg->phich_Config.phich_Duration, mib->phichDur);
   WR_FILL_TKN_UINT(bcchBchMsg->phich_Config.phich_Resource, mib->phichGrps);

   /* systemFrameNumber */
   WR_GET_MEM(memCp, sizeof(U8),
              &(bcchBchMsg->systemFrameNumber.val));
   bcchBchMsg->systemFrameNumber.pres = PRSNT_NODEF;
   bcchBchMsg->systemFrameNumber.len  = WR_BYTE_LEN;

   /* Assign the 8 most significant bits of SFN */
   WR_GET_SFN_MIB(bcchBchMsg->systemFrameNumber.val[0], schedSFN);

   /* Spare, since it is not optional */
   WR_GET_MEM(memCp, (2 * sizeof(U8)), &(bcchBchMsg->spare.val));
   bcchBchMsg->spare.pres = PRSNT_NODEF;
   bcchBchMsg->spare.len  = 10; /* value same as WR_EMM_RRC_MIB_SPARE_LEN */

   RETVALUE(ROK);
}

/** @brief This function fills SIB1 IE in Source to Target Container.
 *
 * @details
 *
 *     Function: wrUmmRrcFillSib1ForHo
 *
 *         Processing steps:
 *         - This function build SIB message based on updated information in
 *           cell control block.
 *
 *
 * @param [in]  cellCb   : cell control block.
 * @param [out] memCp    : MIB msg in RRC data request event SDU.
 * @param [in]  schedSFN : scheduled SFN.
 * @return  S16
 *      -# Success : ROK.
 *      -# Failure : RFAILED. 
 */
PRIVATE S16 wrUmmRrcFillSib1ForHo
(
WrCellCb                     *cellCb,
CmMemListCp                  *memCp,
NhuSysInfoBlockTyp1          *sibTyp1,
U16                          schedSFN
)
{
   NhuSchedulingInfo         *schedInf   = NULLP;
   WrSib1Params              *sib1 = &cellCb->sib1;
   U8                        idx = 0;
   U8                        mapCnt = 0;

   TRC2(wrSibBldSIB1Msg);

   WR_FILL_TKN_UINT(sibTyp1->pres, PRSNT_NODEF);

   /* cellAccessRelatedInfo */
   wrSibBldCellAccInfo(cellCb,(NhuDatReqSdus *)memCp, 
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

   /* Include P-MAX IE into SIB1                                          */
   {
      WR_FILL_TKN_UINT(sibTyp1->p_Max, sib1->pMax);
   }
   /* Include Frequency Band Indicator IE                                 */
   WR_FILL_TKN_UINT(sibTyp1->freqBandIndicator, sib1->freqBandIndicator);

   /*TODO Sriky : Determine logic for this section                        */
   if((cellCb->eutranNbrFreqLst.count > 1))
   {
   WR_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, sib1->numSis);
   }
   else
   {
      /*SIB5 periodicity will not be added to SIB1 if no inter frequency configured*/
      WR_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, (sib1->numSis-1));
   }
   WR_GET_MEM(memCp, 
        (sizeof(NhuSchedulingInfo) * sibTyp1->schedulingInfoLst.noComp.val),
         &(sibTyp1->schedulingInfoLst.member));

   /* Filling Scheduling Periodicity info for SIB2 */
   /* Zero means only SIB2 by default */
   schedInf = &(sibTyp1->schedulingInfoLst.member[0]);
   WR_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(schedInf->si_Periodicity, sib1->siSchdInfo[0].period);
   WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, 0);

   /* Filling the Mapping info and for other SIBs */
   for(idx = 1; idx < (sibTyp1->schedulingInfoLst.noComp.val); idx++)
   {
      /*SIB5 Scheduling info will be skipped if SIB5 is disabled*/
      if((sib1->siSchdInfo[idx].sibs[0] == WR_SIB_5)&&
                  (cellCb->eutranNbrFreqLst.count  == 1))
      {
         RLOG0(L_INFO,"SIB5 info is skipped in SIB1");
         continue;
      }

      schedInf = &(sibTyp1->schedulingInfoLst.member[idx]);
      WR_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
      WR_FILL_TKN_UINT(schedInf->si_Periodicity, sib1->siSchdInfo[idx].period);
      WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, \
            sib1->siSchdInfo[idx].numSibs);
      WR_GET_MEM(memCp, 
            (sizeof(NhuSIB_Typ) * schedInf->sib_MappingInfo.noComp.val),
            &(schedInf->sib_MappingInfo.member));
      for(mapCnt = 0; mapCnt < (schedInf->sib_MappingInfo.noComp.val); \
            mapCnt++)
      {
         WR_FILL_TKN_UINT(schedInf->sib_MappingInfo.member[mapCnt], \
               sib1->siSchdInfo[idx].sibs[mapCnt]);
      }
   }

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
}

/** @brief This function fills SIB2 IEs for a member in system information
 *
 * @details
 *
 *     Function: wrUmmRrcFillSib2ForHo
 *
 *         Processing steps:
 *         - This function is build the SIB2 IEs based on updated information in
 *           cell control block.
 *
 * @param [in]       cellCb : Cell control block
 * @param [in]       memCp  : Memory Pointer
 * @param [out]      sib2Ie : SIB2 IE.
 * @return  S16
 *      -# Success : ROK
 */
PRIVATE S16 wrUmmRrcFillSib2ForHo
(
WrCellCb                     *cellCb,
CmMemListCp                  *memCp,
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
               5, &(sib2->barringInfo.signalling.acBarringForSpecialAC), memCp);
      }
      if (sib2->barringInfo.dataCall.pres)
      {
         WR_FILL_TKN_UINT(sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.pres, PRSNT_NODEF);
         WR_FILL_TKN_UINT((sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringFactor),
               sib2->barringInfo.dataCall.acBarringFactor);
         WR_FILL_TKN_UINT((sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringTime),
               sib2->barringInfo.dataCall.acBarringTime);
         wrFillTknStrBSXL(&(sib2Ie->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringForSpecialAC),
               5, &(sib2->barringInfo.dataCall.acBarringForSpecialAC), memCp);
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
   /* ccpu00134584 eNB broadcasting parameter "freqInfo-> ul-Bandwidth" for TDD mode.*/
#ifndef LTE_TDD
   WR_FILL_TKN_UINT(sib2Ie->freqInfo.ul_CarrierFreq, sib2->ulCarrierFreq);
   WR_FILL_TKN_UINT(sib2Ie->freqInfo.ul_Bandwidth, sib2->ulBw);
#endif

   WR_FILL_TKN_UINT(sib2Ie->freqInfo.additionalSpectrumEmission,
                    sib2->addlSpectrEmission);

   /* skipping MBSFN subframe config */
   sib2Ie->mbsfn_SubfrmConfigLst.noComp.pres = NOTPRSNT;

   /* TimeAlignmentTimer */
   WR_FILL_TKN_UINT(sib2Ie->timeAlignmentTimerCmn, sib2->timingAlignTimer);

   RETVALUE(ROK);
}

/** @brief This function is used to fill the Dedicated Radio Resource IE
 *
 * @details
 *
 *     Function: wrUmmRrcFillSrcMeasCfgInfo
 *
 *         Processing steps:
 *         - fill the current measurement configuration
 *
 * @param[in, out] measCfg : RRC Structure to be filled
 * @param[in] ueCb         : UE Control Block 
 * @param[in] memCp        : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrUmmRrcFillSrcMeasCfgInfo
(
NhuMeasConfig                *measCfg,
WrUeCb                       *ueCb,
CmMemListCp                  *memCp
)
{
   /* ccpu00128534 */
   WrUmmMeasTransInfo         measTransCb;
   WrMeasUeCb                 *ueMeas = ueCb->measCfg;
   WrMeasUeCbEutraMeasObjNode *eutraMeasNode;
   WrMeasIdNode               *measIdNode;
   WrMeasRptCfgNode           *eutraRptCfg;
   CmLList                    *eutraMeasLnk;
   U16                        idx = 0;
   WrUmmCellCb                *cellCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(measCfg->pres), PRSNT_NODEF);

   wrUmmInitNhuMeasCfg(measCfg);
   cmMemset((U8*)&measTransCb, 0, sizeof(WrUmmMeasTransInfo));

   /* filling the measObjId */
   measTransCb.addMeasInfo.measObjIdCnt = ueMeas->interMeasObjLst.count + 1; /*1-intra*/
   measTransCb.addMeasInfo.measObjLst[idx++] = ueMeas->intraMeasObj.measObjId;
   eutraMeasLnk = ueMeas->interMeasObjLst.first;
   while(NULLP != eutraMeasLnk)
   {
      eutraMeasNode = (WrMeasUeCbEutraMeasObjNode*)eutraMeasLnk->node;
      measTransCb.addMeasInfo.measObjLst[idx++] = eutraMeasNode->measObjId;
      eutraMeasLnk = eutraMeasLnk->next;
   }

   /* filling the measId */
   idx = 0;
   measTransCb.addMeasInfo.measIdCnt = ueMeas->measIdLst.count;
   eutraMeasLnk = ueMeas->measIdLst.first;
   while(NULLP != eutraMeasLnk)
   {
      measIdNode = (WrMeasIdNode*)eutraMeasLnk->node;
      measTransCb.addMeasInfo.measIdLst[idx++] = measIdNode->measID;
      eutraMeasLnk = eutraMeasLnk->next;
   }

   /* filling the rptCfgLst */
   idx = 0;
   measTransCb.addMeasInfo.rptCfgIdCnt = ueMeas->rptCfgLst.count;
   eutraMeasLnk = ueMeas->rptCfgLst.first;
   while(NULLP != eutraMeasLnk)
   {
      eutraRptCfg = (WrMeasRptCfgNode*)eutraMeasLnk->node;
      measTransCb.addMeasInfo.rptCfgLst[idx++] = eutraRptCfg->rptCfgId;
      eutraMeasLnk = eutraMeasLnk->next;
   }

   /* Configuring S-Measure, Quantity and Mobility Params */
   ueMeas->isQuantityCfged =  FALSE;

   /* Configuring Meas Objects and report configuration List*/
   if(ROK != wrMeasFillNhuMsg (memCp, measCfg, ueCb, &measTransCb, cellCb->measCellCfg, 0xFFFFFFFF))
   {
      RLOG0(L_ERROR, "wrMeasFillNhuMsg Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is used to fill the integrity protection algorithm IE
 *         for a UE based on the UEs 3GPP release information. 
 * 
 * @details
 *
 *     Function: wrUmmRrcFillIntgAlgo 
 *
 *         Processing steps:
 *         - If the UE is release 8 and null algorithm is to be configured, then we 
 *          fill as spare1. 
 *         - If the UE is release 9 and null algorithm is to be configured, then we 
 *          fill as null algorithm.
 *         - In other cases we fill the actual algorithm to be configured.
 *
 * @param[out] asCfg       : Incoming RRC Structure
 * @param[in] ueCb         : UE Control Block
 * @return Void 
 *        -# None 
 */
PUBLIC Void wrUmmRrcFillIntgAlgo
(
NhuSecAlgorithmConfig        *secAlgCfg,
WrUeCb                       *ueCb
)
{
   if(ueCb->secCfg->intgAlgo == WR_UMM_NULL_ALGO)  
   {
      if(NULLP == ueCb->ueCap)
      {
         /* Ue capability information is mandatory field */ 
         RLOG0(L_ERROR, "ueCb->ueCap is NULL");
      }
      else if(ueCb->ueCap->eUTRA_Cap.accessStratumRls.val == NhuAccessStratumRlsrel8Enum)  
      {
         wrFillTknU32(&(secAlgCfg->integrityProtAlgorithm), 
                      NhuSecAlgorithmConfigintegrityProtAlgorithmspare1Enum); 
      }
      else 
      {
         /* If release 9 and other then fill the actual algorithm to be 
          * configured.
          */
         wrFillTknU32(&(secAlgCfg->integrityProtAlgorithm), 
                   ueCb->secCfg->intgAlgo); 
      }
   } 
   else 
   { 
      wrFillTknU32(&(secAlgCfg->integrityProtAlgorithm), 
                   ueCb->secCfg->intgAlgo); 
   }
   RETVOID; 
} /* end of wrUmmRrcFillIntgAlgo */

/** @brief This function is used to fill the AS Configuration IE in 
 *         Source to Target Container IE
 *
 * @details
 *
 *     Function: wrUmmRrcFillAsCfg
 *
 *         Processing steps:
 *         - fill the Measurement Configuration
 *         - fill the bearer configuration
 *         - fill the broadcase information
 *
 * @param[in] ueCb         : UE Control Block
 * @param[in, out] asCfg   : Incoming RRC Structure
 * @param[in] memCp        : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PRIVATE S16 wrUmmRrcFillAsCfg
(
WrUeCb                       *ueCb,
NhuAS_Config                 *asCfg,
CmMemListCp                  *memCp
)
{
   U16                       len;
   WrCellCb                  *cellCb;
   /* SPS changes starts */
   U8                        bSpsBitMap = 0;
   /* SPS changes ends */

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(asCfg->pres), PRSNT_NODEF);

   /* filling measurement information */
   wrUmmRrcFillSrcMeasCfgInfo(&asCfg->sourceMeasConfig, ueCb, memCp);

   /* filling bearer information */
   /* SPS changes starts */
   wrUmmRrcFillDedRadioCfg(&(asCfg->sourceRadioResourceConfig), ueCb, memCp, ueCb->spsCfg.spsCfgUl.bIsSpsEnabled);

   /* If SPS is configured, fill in the source to target container */
   if (ueCb->spsCfg.spsCfgDl.bIsSpsEnabled == TRUE)
   {
      bSpsBitMap = RM_RCM_DL_SPS_CFG;
   }
   if (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled == TRUE)
   {
      bSpsBitMap |= RM_RCM_UL_SPS_CFG;
   }
   wrUmmRrcFillSpsCfgIe(memCp, bSpsBitMap, ueCb->spsCfg.spsRnti, &ueCb->spsCfg.spsCfgDl, \
         &ueCb->spsCfg.spsCfgUl, &asCfg->sourceRadioResourceConfig.sps_Config, \
         wrEmmGetDuplexMode(ueCb->cellId));
   /* SPS changes ends */

   /* filling security information */
   wrFillTknU8(&asCfg->sourceSecAlgorithmConfig.pres, PRSNT_NODEF);

   /*ccpu00129900*/
   wrFillTknU32(&asCfg->sourceSecAlgorithmConfig.cipheringAlgorithm,
         ueCb->secCfg->ciphAlgo);
   wrUmmRrcFillIntgAlgo(&asCfg->sourceSecAlgorithmConfig, ueCb);
   /*ccpu00129900*/

   /* filling crnti */
   len = 16;
   WR_FILL_BIT_STR_VAL(asCfg->sourceUE_Identity, len, ueCb->crnti, memCp);

   /* filling MIB information */
   /* Fix for ccpu00127546  */
   wrUmmRrcFillMibForHo(cellCb, &asCfg->sourceMasterInfoBlock, memCp, cellCb->pbmCb.curSfn);

   /* filling SIB1 information */
   wrUmmRrcFillSib1ForHo(cellCb, memCp, &asCfg->sourceSysInfoBlockTyp1, 0);

   /* filling SIB2 information */
   wrUmmRrcFillSib2ForHo(cellCb, memCp, &asCfg->sourceSysInfoBlockTyp2);

   /* filling antenna information */
   wrFillTknU8(&asCfg->antennaInfoCmn.pres, PRSNT_NODEF);
   wrFillTknU32(&asCfg->antennaInfoCmn.antennaPortsCount, cellCb->numAntPorts);

   /* filling DL carrier freq information */
   wrFillTknU32(&asCfg->sourceDl_CarrierFreq, cellCb->dlCarrierFreq);

   asCfg->extaddgrp_1.pres.pres = NOTPRSNT;
   RETVALUE(ROK);
}

/** @brief This function is used to fill the RAT information in the container.
 *
 * @details
 *
 *     Function: wrUmmRrcFillRatContnr
 *
 *         Processing steps:
 *         - fill the RAT information from UE CB to RRC IE.
 *
 * @param[in] ueCb         : UE Control Block
 * @param[in, out] asCfg   : Incoming RRC Structure
 * @param[in] memCp        : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PRIVATE S16 wrUmmRrcFillRatContnr
(
CmMemListCp                  *memCp,
NhuUE_CapRAT_Cont            *rrcIe,
TknStrOSXL                   *ueRatInfo,
U32                          ratType
)
{
   wrFillTknU8(&(rrcIe->pres), PRSNT_NODEF);
   wrFillTknU32(&(rrcIe->rat_Typ), ratType);
   rrcIe->ueCapRAT_Cont.pres = PRSNT_NODEF;
   rrcIe->ueCapRAT_Cont.len  = ueRatInfo->len;
   WR_GET_MEM(memCp, ueRatInfo->len, &rrcIe->ueCapRAT_Cont.val);
   cmMemcpy(rrcIe->ueCapRAT_Cont.val, ueRatInfo->val, ueRatInfo->len);
   RETVALUE(ROK);
}

/** @brief This function is used to fill the UE Capability information in
 *         Source to Target Container IE
 *
 * @details
 *
 *     Function: wrUmmRrcFillUeCapInfo
 *
 *         Processing steps:
 *         - fill the information received as part of UE 
 *           Capability Information
 *
 * @param[in] ueCb         : UE Control Block
 * @param[in, out] asCfg   : Incoming RRC Structure
 * @param[in] memCp        : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PRIVATE S16 wrUmmRrcFillUeCapInfo
(
WrUeCb                       *ueCb,
NhuUE_CapRAT_ContLst         *ueRadAccCapInfo,
CmMemListCp                  *memCp
)
{
   WrUeCapInfo               *ueCap = ueCb->ueCap;
   U16                       numComp = 0;

   if(ueCap == NULLP)
   {
      ueRadAccCapInfo->noComp.pres = NOTPRSNT;
      RETVALUE(ROK);
   }

   /* find the number of components to be filled */
   if(ueCap->ueEutraOctStr.pres == PRSNT_NODEF)
   {
      numComp++;
   }
   if(ueCap->ueUtraOctStr.pres == PRSNT_NODEF)
   {
      numComp++;
   }
   if(ueCap->ueGeranCsOctStr.pres == PRSNT_NODEF)
   {
      numComp++;
   }
   if(ueCap->ueGeranPsOctStr.pres == PRSNT_NODEF)
   {
      numComp++;
   }
   if(ueCap->ueCdmaOctStr.pres == PRSNT_NODEF)
   {
      numComp++;
   }

   /* fill the RRC IE */
   wrFillTknU16(&(ueRadAccCapInfo->noComp), numComp);
   WR_GET_MEM(memCp, numComp * sizeof(NhuUE_CapRAT_Cont),
                   &(ueRadAccCapInfo->member));
   numComp = 0;
   if(ueCap->ueEutraOctStr.pres == PRSNT_NODEF)
   {
      wrUmmRrcFillRatContnr(memCp, &ueRadAccCapInfo->member[numComp++],
                             &ueCap->ueEutraOctStr, NhuRAT_TypeutraEnum);
   }
   if(ueCap->ueUtraOctStr.pres == PRSNT_NODEF)
   {
      wrUmmRrcFillRatContnr(memCp, &ueRadAccCapInfo->member[numComp++],
                             &ueCap->ueUtraOctStr, NhuRAT_TyputraEnum);
   }
   if(ueCap->ueGeranCsOctStr.pres == PRSNT_NODEF)
   {
      wrUmmRrcFillRatContnr(memCp, &ueRadAccCapInfo->member[numComp++],
                             &ueCap->ueGeranCsOctStr, NhuRAT_Typgeran_csEnum);
   }
   if(ueCap->ueGeranPsOctStr.pres == PRSNT_NODEF)
   {
      wrUmmRrcFillRatContnr(memCp, &ueRadAccCapInfo->member[numComp++],
                             &ueCap->ueGeranPsOctStr, NhuRAT_Typgeran_psEnum);
   }
   if(ueCap->ueCdmaOctStr.pres == PRSNT_NODEF)
   {
      wrUmmRrcFillRatContnr(memCp, &ueRadAccCapInfo->member[numComp],
                             &ueCap->ueCdmaOctStr, NhuRAT_Typcdma2000_1XRTTEnum);
   }

   RETVALUE(ROK);
}

/** @brief This function is used to fill the Source to Target Container IE
 *
 * @details
 *
 *     Function: wrUmmRrcFillSrcToTgetIe
 *
 *         Processing steps:
 *         - fill the necessary information from UE CB and Cell CB.
 *
 * @param[in, out] ded: Incoming RRC Structure
 * @param[in] ueCb    : UE Control Block 
 * @param[in] memCp   : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrUmmRrcFillSrcToTgetIe
(
WrUeCb                       *ueCb,
NhuEncReqSdus                *hoPrepMsgEnc
)
{
   NhuHovrPrepInfo           *hoPrep;
   NhuHovrPrepInfo_r8_IEs    *hoPrepInfoR8;
   NhuUE_CapRAT_ContLst      *ueRadAccCapInfo;
   NhuAS_Config              *asCfg;
   NhuRRM_Config             *rrmCfg;
   NhuAS_Ctxt                *asCtxt;

   hoPrep = &(hoPrepMsgEnc->sdu.u.hovrPrepInfo);
   wrFillTknU8(&(hoPrep->pres), PRSNT_NODEF);
   wrFillTknU8(&(hoPrep->criticalExtns.choice), CRITICALEXTNS_C1);
   wrFillTknU8(&(hoPrep->criticalExtns.val.c1.choice), C1_HANDOVERPREPINFO_R8);

   hoPrepInfoR8 = &(hoPrep->criticalExtns.val.c1.val.handoverPrepInfo_r8);
   wrFillTknU8(&(hoPrepInfoR8->pres), PRSNT_NODEF);

   /* fill UE Capability information */
   ueRadAccCapInfo = &(hoPrepInfoR8->ue_RadioAccessCapInfo);
   wrUmmRrcFillUeCapInfo(ueCb, ueRadAccCapInfo, &(hoPrepMsgEnc->memCp));

   /* fill AS Config information */
   asCfg = &(hoPrepInfoR8->as_Config);
   wrUmmRrcFillAsCfg(ueCb, asCfg, &(hoPrepMsgEnc->memCp));

   /* fill RRM Config information */
   rrmCfg = &(hoPrepInfoR8->rrm_Config);
   wrFillTknU8(&(rrmCfg->pres), NOTPRSNT);

   /* fill AS Context information */
   asCtxt = &(hoPrepInfoR8->as_Ctxt);
   wrUmmRrcFillAsCtxt(ueCb, asCtxt, &(hoPrepMsgEnc->memCp));

   /* fill critical extension */
   /* RRC-REL10-Upgrade start */
   /* The source eNodeB should consider filling of ue-ConfigRelease-r9 IE 
    * by taking the minimum value of access stratum release version of 
    * release 10 eNodeB  and to that of UE's access stratum release value.*/
   if(NhuAccessStratumRlsrel8Enum == ueCb->ueCap->eUTRA_Cap.accessStratumRls.val)
   {
      hoPrepInfoR8->nonCriticalExtn.pres.pres = NOTPRSNT;
   }
   else 
   {     
      wrFillTknU8(&(hoPrepInfoR8->nonCriticalExtn.pres), PRSNT_NODEF);
      if(NhuAccessStratumRlsrel10Enum <= ueCb->ueCap->eUTRA_Cap.accessStratumRls.val)
      {
         wrFillTknU32(&(hoPrepInfoR8->nonCriticalExtn.ue_ConfigRls_r9),
               NhuHovrPrepInfo_v920_IEsue_ConfigRls_r9rel10Enum);
      }
      else
      {
         wrFillTknU32(&(hoPrepInfoR8->nonCriticalExtn.ue_ConfigRls_r9),
               NhuHovrPrepInfo_v920_IEsue_ConfigRls_r9rel9Enum);
      }
   }
   /* RRC-REL10-Upgrade end*/

   RETVALUE(ROK);
}

/* SPS changes starts */
/**
 *  @brief This function converts UL Implicit Release After from RMU to NHU
 *
 *      Function: wrUmmSpsConvertImplicitRlsRmuToNhu
 *
 *          Processing steps:
 *          - Assign corresponding values
 *
 *
 *  @param [in] rmuSpsIntervalUl    : RMU UL SPS Interval
 *  @return EnumNhuSPS_ConfigULsetupimplicitRlsAfter
 */
PRIVATE EnumNhuSPS_ConfigULsetupimplicitRlsAfter wrUmmSpsConvertImplicitRlsRmuToNhu
(
 RmuSpsImplctRelAftr   eImplctRelAftr
)
{
   switch(eImplctRelAftr)
   {
      case RMU_SPS_E2:
         RETVALUE(NhuSPS_ConfigULsetupimplicitRlsAftere2Enum);
      case RMU_SPS_E3:
         RETVALUE(NhuSPS_ConfigULsetupimplicitRlsAftere3Enum);
      case RMU_SPS_E4:
         RETVALUE(NhuSPS_ConfigULsetupimplicitRlsAftere4Enum);
      case RMU_SPS_E8:
         RETVALUE(NhuSPS_ConfigULsetupimplicitRlsAftere8Enum);
      default:
         RETVALUE(NhuSPS_ConfigULsetupimplicitRlsAftere2Enum);
   }
}

/**
 *  @brief This function converts UL Interval from RMU to NHU
 *
 *      Function: wrUmmSpsConvertIntervalUlRmuToNhu
 *
 *          Processing steps:
 *          - Assign corresponding values
 *
 *
 *  @param [in] rmuSpsIntervalUl    : RMU UL SPS Interval
 *  @return EnumNhuSPS_ConfigULsetupsemiPersistSchedIntervalUL
 */
PRIVATE EnumNhuSPS_ConfigULsetupsemiPersistSchedIntervalUL wrUmmSpsConvertIntervalUlRmuToNhu
(
 RmuSpsIntervalUl   rmuSpsIntervalUl
)
{
   switch(rmuSpsIntervalUl)
   {
      case RMU_SPS_UL_INTERVAL_E10:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf10Enum);
      case RMU_SPS_UL_INTERVAL_E20:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf20Enum);
      case RMU_SPS_UL_INTERVAL_E32:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf32Enum);
      case RMU_SPS_UL_INTERVAL_E40:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf40Enum);
      case RMU_SPS_UL_INTERVAL_E64:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf64Enum);
      case RMU_SPS_UL_INTERVAL_E80:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf80Enum);
      case RMU_SPS_UL_INTERVAL_E128:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf128Enum);
      case RMU_SPS_UL_INTERVAL_E160:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf160Enum);
      case RMU_SPS_UL_INTERVAL_E320:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf320Enum);
      case RMU_SPS_UL_INTERVAL_E640:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf640Enum);
      default:
         RETVALUE(NhuSPS_ConfigULsetupsemiPersistSchedIntervalULsf20Enum);
   }
}

/**
 *  @brief This function converts DL Interval from RMU to NHU
 *
 *      Function: wrUmmSpsConvertIntervalDlRmuToNhu
 *
 *          Processing steps:
 *          - Assign corresponding values
 *
 *
 *  @param [in] rmuSpsIntervalDl    : RMU DL SPS Interval
 *  @return EnumNhuSPS_ConfigDLsetupsemiPersistSchedIntervalDL
 */
PRIVATE EnumNhuSPS_ConfigDLsetupsemiPersistSchedIntervalDL wrUmmSpsConvertIntervalDlRmuToNhu
(
 RmuSpsIntervalDl   rmuSpsIntervalDl
)
{
   switch(rmuSpsIntervalDl)
   {
      case RMU_SPS_DL_INTERVAL_E10:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf10Enum);
      case RMU_SPS_DL_INTERVAL_E20:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf20Enum);
      case RMU_SPS_DL_INTERVAL_E32:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf32Enum);
      case RMU_SPS_DL_INTERVAL_E40:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf40Enum);
      case RMU_SPS_DL_INTERVAL_E64:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf64Enum);
      case RMU_SPS_DL_INTERVAL_E80:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf80Enum);
      case RMU_SPS_DL_INTERVAL_E128:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf128Enum);
      case RMU_SPS_DL_INTERVAL_E160:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf160Enum);
      case RMU_SPS_DL_INTERVAL_E320:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf320Enum);
      case RMU_SPS_DL_INTERVAL_E640:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf640Enum);
      default:
         RETVALUE(NhuSPS_ConfigDLsetupsemiPersistSchedIntervalDLsf20Enum);
   }
}

/**
 *  @brief This function is used to fill the SPS Config IE in the
 *         RRC Message
 *
 *      Function: wrUmmRrcFillSpsCfgIe
 *
 *          Processing steps:
 *          - Fill the IE elements
 *
 *
 *  @param [in] memCp        : Pointer to mem list
 *  @param [in] bSpsBitMap   : SPS bitmap
 *  @param [in] spsRnti      : SPS RNTI
 *  @param [in] pDlSpsCfg    : Pointer to DL SPS
 *  @param [in] pUlSpsCfg    : Pointer to UL SPS
 *  @param [out] spsCfgIe    : Pointer to SPS IE
 *  @return S16
 */
PUBLIC S16 wrUmmRrcFillSpsCfgIe
(
 CmMemListCp        *memCp,
 U8                 bSpsBitMap,
 U16                spsRnti,
 RmuSpsConfigDl     *pDlSpsCfg,
 RmuSpsConfigUl     *pUlSpsCfg,
 NhuSPS_Config      *spsCfgIe,
 U16                duplexMode
)
{
   Bool  isFillSpsRnti = FALSE;
   Data  *tempSpsRnti;
   U16   count;
   EnumNhuSPS_ConfigDLsetupsemiPersistSchedIntervalDL eSpsDlInterval;
   EnumNhuSPS_ConfigULsetupsemiPersistSchedIntervalUL eSpsUlInterval;
   EnumNhuSPS_ConfigULsetupimplicitRlsAfter           eImplicitRlsAfter;

   spsCfgIe->pres.pres = NOTPRSNT;
   if (bSpsBitMap & RM_RCM_DL_SPS_CFG)
   {
      /* SPS configuration has been configured/released in DL */
      WR_FILL_TKN_UINT(spsCfgIe->pres, PRSNT_NODEF);
      if (pDlSpsCfg->bIsSpsEnabled == TRUE)
      {
         /* SPS has been configured in DL */
         /* Fill SPS DL configurations */
         isFillSpsRnti = TRUE;
         wrFillTknU8(&spsCfgIe->sps_ConfigDL.choice, 1);
         WR_FILL_TKN_UINT(spsCfgIe->sps_ConfigDL.val.setup.pres, PRSNT_NODEF);
         eSpsDlInterval = wrUmmSpsConvertIntervalDlRmuToNhu(pDlSpsCfg->eSpsIntervalDl);
         wrFillTknU32(&spsCfgIe->sps_ConfigDL.val.setup.semiPersistSchedIntervalDL, \
               (U32)eSpsDlInterval);
         wrFillTknU32(&spsCfgIe->sps_ConfigDL.val.setup.numberOfConfSPS_Processes, \
               (U32)pDlSpsCfg->usNumOfConfSpsProc);
         wrFillTknU16(&spsCfgIe->sps_ConfigDL.val.setup.n1PUCCH_AN_PersistentLst.noComp, \
               (U16)pDlSpsCfg->usNumN1Pucch);
         if (pDlSpsCfg->usNumN1Pucch > 0)
         {
            WR_GET_MEM(memCp, (sizeof(NhuN1PUCCH_AN_PersistentLstMember) * pDlSpsCfg->usNumN1Pucch), 
                  &(spsCfgIe->sps_ConfigDL.val.setup.n1PUCCH_AN_PersistentLst.member));
            if (spsCfgIe->sps_ConfigDL.val.setup.n1PUCCH_AN_PersistentLst.member == NULLP)
            {
               RETVALUE(RFAILED);
            }
            for (count = 0; count < spsCfgIe->sps_ConfigDL.val.setup.n1PUCCH_AN_PersistentLst.noComp.val; count++)
            {
               wrFillTknU32(&spsCfgIe->sps_ConfigDL.val.setup.n1PUCCH_AN_PersistentLst.member[count],\
                     (U32)pDlSpsCfg->usN1Pucch[count]);
            }
         }
      }
      else
      {
         RLOG0(L_DEBUG, "Releasing DL SPS");
         /* Release DL SPS Configuration */
         wrFillTknU8(&spsCfgIe->sps_ConfigDL.choice, 0);
      }
   }
   if (bSpsBitMap & RM_RCM_UL_SPS_CFG)
   {
      /* SPS configuration has been configured/released in UL */
      /* Fill SPS UL configurations */
      WR_FILL_TKN_UINT(spsCfgIe->pres, PRSNT_NODEF);
      if (pUlSpsCfg->bIsSpsEnabled == TRUE)
      {
         /* Fill SPS UL configurations */
         isFillSpsRnti = TRUE;
         wrFillTknU8(&spsCfgIe->sps_ConfigUL.choice, 1);
         WR_FILL_TKN_UINT(spsCfgIe->sps_ConfigUL.val.setup.pres, PRSNT_NODEF);
         eSpsUlInterval = wrUmmSpsConvertIntervalUlRmuToNhu(pUlSpsCfg->eSpsIntervalUl);
         wrFillTknU32(&spsCfgIe->sps_ConfigUL.val.setup.semiPersistSchedIntervalUL, (U32)eSpsUlInterval);
         eImplicitRlsAfter = wrUmmSpsConvertImplicitRlsRmuToNhu(pUlSpsCfg->eImplctRelAftr);
         wrFillTknU32(&spsCfgIe->sps_ConfigUL.val.setup.implicitRlsAfter, (U32)eImplicitRlsAfter);
         if (pUlSpsCfg->bP0PersistentPres == TRUE)
         {
            wrFillTknU8(&spsCfgIe->sps_ConfigUL.val.setup.p0_Persistent.pres, PRSNT_NODEF);
            wrFillTknS32(&spsCfgIe->sps_ConfigUL.val.setup.p0_Persistent.p0_NominalPUSCH_Persistent, \
                  (S32)pUlSpsCfg->stP0Persistent.sP0NominalPUSCHPersistent);
            wrFillTknS32(&spsCfgIe->sps_ConfigUL.val.setup.p0_Persistent.p0_UE_PUSCH_Persistent, \
                  (S32)pUlSpsCfg->stP0Persistent.sP0UEPuschPersistent);
         }
         else
         {
            spsCfgIe->sps_ConfigUL.val.setup.p0_Persistent.pres.pres = NOTPRSNT;
         }
#ifdef LTE_TDD
         if ((duplexMode == NHU_MODE_TDD) && (pUlSpsCfg->bTwoIntervalCfg))
         {
            wrFillTknU32(&spsCfgIe->sps_ConfigUL.val.setup.twoIntervalsConfig, 0);
         }
         else
         {
            spsCfgIe->sps_ConfigUL.val.setup.twoIntervalsConfig.pres = NOTPRSNT;
         }
#else
         spsCfgIe->sps_ConfigUL.val.setup.twoIntervalsConfig.pres = NOTPRSNT;
#endif
      }
      else
      {
         RLOG0(L_DEBUG, "Releasing UL SPS");
         /* Release UL SPS Configuration */
         wrFillTknU8(&spsCfgIe->sps_ConfigUL.choice, 0);
      }
   }
   if (isFillSpsRnti == TRUE)
   {
      WR_GET_MEM(memCp, 2, &tempSpsRnti);
      if (NULLP == tempSpsRnti)
      {
         /* The return failure is not been handled due to this since
          * all the places in the code we don't handle failures due to this,
          * if it has to be done, we will have to see what all can be done
          * since schedular is already configured. Also we will have to handle
          * failures in calling caller too. */
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      tempSpsRnti[1] = (U8)(0x00ff & spsRnti);
      tempSpsRnti[0] = (U8)(0x00ff & (spsRnti >> 8));

      WR_FILL_TKN_STR_UINT((&spsCfgIe->semiPersistSchedC_RNTI), tempSpsRnti, 16);
   }
   else
   {
      /* Mark SPS as not present */
      spsCfgIe->semiPersistSchedC_RNTI.pres = NOTPRSNT;
   }
   RETVALUE(ROK);
}
/* SPS changes ends */

/** @brief This function is used to fill the IRAT Utra Source to Target
 *  Container IE
 *
 * @details
 *
 *     Function: wrUmmUtraRrcFillSrcToTgetIe
 *
 *         Processing steps:
 *         - fill the necessary information from UE CB and Cell CB.
 *
 * @param[in, out] ded: Incoming RRC Structure
 * @param[in] ueCb    : UE Control Block 
 * @param[in] memCp   : memory control pointer
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrUmmUtraRrcFillSrcToTgetIe
(
WrUeCb                       *ueCb,
NhuEncReqSdus                *hoPrepMsgEnc,
TknStrOSXL                   *utraUeCapContainer
)
{
   NhuToTgtRNC_Cont                                   *rncCont;
   NhuInterRATHovrInfoWithInterRATCapabilities_r3r3   *r3;
   NhuInterRATHovrInfoWithInterRATCapabilities_r3_IEs *iRatHOInfoIe;
   NhuInterRATHovrInfoWithInterRATCapabilities_r3r3v390NonCriticalExtns 
                                                      *v390NonCritExtIe;
   NhuInterRATHovrInfoWithInterRATCapabilities_v860ext_IEs *iRatEutraCapIe;
   NhuEUTRA_RadioAccessCapue_EUTRA_Cap                     *eUtraCapIe;

   rncCont = &(hoPrepMsgEnc->sdu.u.nhuIntRatHoInfoWithUeCap);
   wrFillTknU8(&(rncCont->choice), TOTGTRNC_CONT_INTERRATHOVRINFO);
   wrFillTknU8(&(rncCont->val.interRATHovrInfo.choice), 
         INTERRATHOVRINFOWITHINTERRATCAPABILITIES_R3_R3);

   r3 = &(rncCont->val.interRATHovrInfo.val.r3);
   iRatHOInfoIe = &(r3->interRATHovrInfo_r3);
   v390NonCritExtIe = &(r3->v390NonCriticalExtns);
   wrFillTknU8(&(r3->pres), PRSNT_NODEF);
   wrFillTknU8(&(iRatHOInfoIe->pres), PRSNT_NODEF);

   /* Fill IRAT HO INFO - UTRAN UE cap received from UE */
   wrFillTknStrOSXL(&(iRatHOInfoIe->interRATHovrInfo), utraUeCapContainer->len, 
         utraUeCapContainer->val, &(hoPrepMsgEnc->memCp)); 

      /* Fill EUTRA UE capability */
   wrFillTknU8(&(v390NonCritExtIe->pres), PRSNT_NODEF);
   wrFillTknU8(&(v390NonCritExtIe->
                interRATHovrInfoWithInterRATCapabilities_v390ext.pres), PRSNT_NODEF);

   wrFillTknU8(&(v390NonCritExtIe->v690NonCriticalExtns.pres), PRSNT_NODEF);
   wrFillTknU8(&(v390NonCritExtIe->v690NonCriticalExtns.
        interRATHovrInfoWithInterRATCapabilities_v690ext.pres),PRSNT_NODEF);

   wrFillTknU8(&(v390NonCritExtIe->v690NonCriticalExtns.v860NonCriticalExtns.pres), 
         PRSNT_NODEF);

   iRatEutraCapIe = &(v390NonCritExtIe->v690NonCriticalExtns.v860NonCriticalExtns.
         interRATHovrInfoWithInterRATCapabilities_v860ext);
   wrFillTknU8(&(iRatEutraCapIe->pres), PRSNT_NODEF);
   wrFillTknU8(&(iRatEutraCapIe->ue_RATSpecificCap.pres), PRSNT_NODEF);
   wrFillTknU8(&(iRatEutraCapIe->ue_RATSpecificCap.eutra_RadioAccessCap.pres), 
         PRSNT_NODEF);

   eUtraCapIe = &(iRatEutraCapIe->ue_RATSpecificCap.eutra_RadioAccessCap.ue_EUTRA_Cap);
   wrFillTknStrOSXL(eUtraCapIe, ueCb->ueCap->ueEutraOctStr.len, 
         ueCb->ueCap->ueEutraOctStr.val, &(hoPrepMsgEnc->memCp)); 

   RETVALUE(ROK);
}

/**
 *  @brief This function fills the UE Recfg Info into the NHU structures
 *
 *  @details This function will encode the UE recfg info based on the RRM
 *           configuraiton
 *
 *      Function: wrUmmRrcFillUePhyCfgDedInfo
 *
 *          Processing steps:
 *          - if flag phyCfgDedResBitMap for CQI update is set
 *             - update the ueReCfg with CQI info
 *
 *
 *  @param [in] rrcCfgDed      : pointer to PHY Config deidicated info of the RRC
 *                               reconfiguration info
 *  @param [in] rrmUeRecfgResp : pointer to Ue Control block of the UE for which UE
 *                               Recfg need to be done
 *  @return 
 */
PUBLIC Void wrUmmRrcFillUePhyCfgDedInfo
(
   NhuRadioResourceConfigDedicated  *rrcCfgDed,
   RmuUeRecfgRsp                    *rrmUeRecfgResp
)
{
   NhuCQI_ReportConfig              *cqiRprtng = NULLP;
   RmuUeCqiReportCfg                *cqiRepCfg = NULLP;
   NhuSchedulingRqstConfig          *schedulingRqstConfig = NULLP;
   
   WR_FILL_TKN_UINT(rrcCfgDed->physicalConfigDedicated.pres,PRSNT_NODEF);

   /* Configuring the CQI report configuration */

   if((rrmUeRecfgResp->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap)
         & (U8)RM_RCM_PHY_DED_RES_CFG_CQI)
   {
      /* Pointing RRC CQI config Into */
      cqiRprtng = &rrcCfgDed->physicalConfigDedicated.cqi_ReportConfig;
      schedulingRqstConfig = &(rrcCfgDed->physicalConfigDedicated.\
            schedulingRqstConfig);

      /* Pointing to the CQI Info Configured from RRM */
      cqiRepCfg = &rrmUeRecfgResp->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg;

      WR_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);
      if (cqiRepCfg->stAPrdCqiRepCfg.enbld)
      {
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic, \
            cqiRepCfg->stAPrdCqiRepCfg.enPuschReportMode);
      }
      WR_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, \
               wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.nomPdschRsEpreOff); /* 0, MIMO */

      /* NhuCQI_ReportPeriodicsetup - empty */
      cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;

      wrFillTknS32(&(cqiRprtng->nomPDSCH_RS_EPRE_Offset),\
            (S32)(wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.nomPdschRsEpreOff));

      WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
            CQI_REPORTPERIODIC_SETUP);
      WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.pres, \
            PRSNT_NODEF);
      WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_PUCCH_ResourceIdx, \
            cqiRepCfg->stPrdCqiRepCfg.usCqiResourceIndex);

      RLOG1(L_DEBUG, "CQI IDX AT RRC RECONFIG[%d]",
         cqiRepCfg->stPrdCqiRepCfg.usCqiConfigIndex);

      WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_pmi_ConfigIdx, \
             cqiRepCfg->stPrdCqiRepCfg.usCqiConfigIndex);

      if(RMU_PCQI_FMT_WIDEBAND == cqiRepCfg->stPrdCqiRepCfg.enCqiFormatInd)
      {
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
               cqi_FormatIndicatorPeriodic.choice, 
               0);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
               cqi_FormatIndicatorPeriodic.val.subbandCQI.pres,
               NOTPRSNT);
      }
      else 
      {
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
               cqi_FormatIndicatorPeriodic.choice, 
               PRSNT_NODEF);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
               cqi_FormatIndicatorPeriodic.val.subbandCQI.pres,
               PRSNT_NODEF);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
               cqi_FormatIndicatorPeriodic.val.subbandCQI.k,
               cqiRepCfg->stPrdCqiRepCfg.bFactorK);
      }

      RLOG1(L_DEBUG, "SIMULACKCQI IDX AT RRC RECONFIG[%d]",
         cqiRepCfg->stPrdCqiRepCfg.bSimulAckNackCqi);
      WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.simultaneousAckNackAndCQI, \
                cqiRepCfg->stPrdCqiRepCfg.bSimulAckNackCqi);

      if(cqiRepCfg->stPrdCqiRepCfg.riConfigIndex.bPres == TRUE)
      {
         RLOG1(L_DEBUG, "RI IDX AT RRC RECONFIG[%d]",
            cqiRepCfg->stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex);
         WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.ri_ConfigIdx, \
         cqiRepCfg->stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex);
      }
#ifndef WR_DL_CQI
      cqiRprtng->pres.pres = NOTPRSNT; /* Disable CQI Reporting */
#endif
   }
 if(schedulingRqstConfig != NULLP)
 {
    if((rrmUeRecfgResp->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap)
          & (U8)RM_RCM_PHY_DED_RES_CFG_SR)
    {
       WR_FILL_TKN_UINT(schedulingRqstConfig->choice,\
             SCHEDULINGRQSTCONFIG_SETUP);
       WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
       WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,\
             wrSmDfltSrPUCCHResIdx);
       WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
             cqiRepCfg->stPrdCqiRepCfg.srCfgIdx);
       /* Macro RGR_DSR_TXMAX_4 is defined for only MSPD.
        * Hence for avoiding the PAL compilation error
        * hard coded to equvalent value to 4 
        */
#if TFU_UPGRADE
       WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
             RGR_DSR_TXMAX_4);
#else
       WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
             4);
#endif
       RLOG1(L_DEBUG, "SR CFG IDX AT RRC RECONFIG[%d]",
             cqiRepCfg->stPrdCqiRepCfg.srCfgIdx);

    }
 }
}

/**
 *  @brief This function fills the UE Recfg Info into the NHU structures
 *
 *  @details This function will encode the UE recfg info based on the RRM
 *           configuraiton
 *
 *      Function: wrUmmRrcFillUeRadioResDedInfo
 *
 *          Processing steps:
 *          - if flag phyCfgDedResBitMap for CQI update is set
 *             - update the ueReCfg with CQI info
 *
 *
 *  @param [in] rrcCfgDed      : pointer to PHY Config deidicated info of the RRC
 *                               reconfiguration info
 *  @param [in] rrmUeRecfgResp : pointer to Ue Control block of the UE for which UE
 *                               Recfg need to be done
 *  @return 
 */
PUBLIC Void wrUmmRrcFillUeRadioResDedInfo
(
   /* SPS changes starts */
   CmMemListCp                      *memCp,
   WrUeCb                           *ueCb,
   WrSpsCfg                         *pSpsCfg,
   /* SPS changes ends */
   NhuRadioResourceConfigDedicated  *rrcCfgDed,
   RmuUeRecfgRsp                    *rrmUeRecfgResp
)
{
/*Removed DRX flag*/
   NhuMAC_MainConfig         *macCfg;
   /* SPS changes starts */
   U8                        bSpsBitMap = 0;
   /* SPS changes ends */

   if(rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_MAC_MAIN_CFG)
   {  
      macCfg = &(rrcCfgDed->mac_MainConfig.val.explicitValue);

      wrFillTknU8(&(rrcCfgDed->mac_MainConfig.choice), 0); 

      /* MAC-MainConfig  */
      WR_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

      /* The cell wide broadcast parameter sets the value at each UE */
      WR_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated,
            wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.timeAlignmentTimerDedicated);

      if(rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
            RM_RCM_MAC_MAIN_CFG_DRX)
      {
         wrUmmRrcFillDrxCfgIe(&macCfg->drx_Config, 
               (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);     
      }
   }
   if(rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap &
      RM_RCM_PHY_DED_RES_CFG)
   {   
      wrUmmRrcFillUePhyCfgDedInfo(rrcCfgDed, rrmUeRecfgResp);
   }
   /* SPS changes starts */
   if ((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) && \
      (ueCb->spsCfg.spsCfgDl.bIsSpsEnabled != rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigDl.bIsSpsEnabled))
   {
      bSpsBitMap = RM_RCM_DL_SPS_CFG;
   }
   if ((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG) && \
      (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled != rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl.bIsSpsEnabled))
   {
      bSpsBitMap |= RM_RCM_UL_SPS_CFG;
   }
   wrUmmRrcFillSpsCfgIe(memCp, bSpsBitMap, pSpsCfg->spsRnti, \
         &rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigDl, &rrmUeRecfgResp->stUeRadioResCfg.\
         stSpsCfg.stSpsConfigUl, &rrcCfgDed->sps_Config, wrEmmGetDuplexMode(ueCb->cellId));
   /* SPS changes ends */
   /* RRC-REL10-Upgrade */
   wrFillTknU8(&(rrcCfgDed->extaddgrp_1.pres), PRSNT_NODEF);
   wrFillTknU8(&(rrcCfgDed->extaddgrp_1.rlf_TmrsAndConsts_r9.choice),
                        RLF_TMRSANDCONSTS_R9_RELEASE);
   rrcCfgDed->extaddgrp_1.rlf_TmrsAndConsts_r9.val.setup.pres.pres = NOTPRSNT;

}

/** @brief This function is responsible for to Update Nhu MacMain config
 * from reconfig message.
 * 
 *  @details
 * Function:wrUmmRrcGetNhuMacMainCfgFrmReCfg
 *
 *   Processing steps:
 *    - Update Nhu Mac main config
 *     
 * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
 * @param[in]  nhuMeasCfg: pointer to Nhu measurement object Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC Void wrUmmRrcGetNhuMacMainCfgFrmReCfg
(
 NhuDatReqSdus                *rrcReCfgMsg,
 NhuMAC_MainConfig            **macCfg
)
{
   NhuDL_DCCH_Msg                   *dcchMsg;
   NhuRRCConRecfgn                  *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs           *recfgIEs;
   NhuRadioResourceConfigDedicated  *rrcCfgDed;

   TRC2(wrUmmRrcGetNhuMacMainCfgFrmReCfg)

   dcchMsg     = &(rrcReCfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
   *macCfg      = &(rrcCfgDed->mac_MainConfig.val.explicitValue);

   rrcCfgDed->srb_ToAddModLst.noComp.pres = NOTPRSNT;
   rrcCfgDed->drb_ToAddModLst.noComp.pres = NOTPRSNT;
   rrcCfgDed->drb_ToRlsLst.noComp.pres = NOTPRSNT;
   rrcCfgDed->sps_Config.pres.pres = NOTPRSNT;
   rrcCfgDed->physicalConfigDedicated.pres.pres = NOTPRSNT;

   wrFillTknU8(&(rrcCfgDed->pres), PRSNT_NODEF);
   wrFillTknU8(&(rrcCfgDed->mac_MainConfig.choice), 0);
}

/** @brief This function is responsible for to Get cfg Poll Retransmission Timer
 * 
 *  @details
 * Function:wrUmmRrcGetCfgPollRetxTmr2
 *
 *   Processing steps:
 *    - Get cfg Poll Retransmission Timer
 *     
 * @param[in]  rrcReCfgMsg: poll retransmission timer
 * @return U16
 *       RETVALUE:enum
**/
PRIVATE U16 wrUmmRrcGetCfgPollRetxTmr2
(
  U32                          pollRetxTmr
   )
{
    switch(pollRetxTmr)
    {
        case NhuT_PollRetransmitms140Enum:
            RETVALUE(NHU_TPOLL_RETX_MS140);
        case NhuT_PollRetransmitms145Enum:
            RETVALUE(NHU_TPOLL_RETX_MS145);
        case NhuT_PollRetransmitms150Enum:
            RETVALUE(NHU_TPOLL_RETX_MS150);
        case NhuT_PollRetransmitms155Enum:
            RETVALUE(NHU_TPOLL_RETX_MS155);
        case NhuT_PollRetransmitms160Enum:
            RETVALUE(NHU_TPOLL_RETX_MS160);
        case NhuT_PollRetransmitms165Enum:
            RETVALUE(NHU_TPOLL_RETX_MS165);
        case NhuT_PollRetransmitms170Enum:
            RETVALUE(NHU_TPOLL_RETX_MS170);
        case NhuT_PollRetransmitms175Enum:
            RETVALUE(NHU_TPOLL_RETX_MS175);
        case NhuT_PollRetransmitms180Enum:
            RETVALUE(NHU_TPOLL_RETX_MS180);
        case NhuT_PollRetransmitms185Enum:
            RETVALUE(NHU_TPOLL_RETX_MS185);
        case NhuT_PollRetransmitms190Enum:
            RETVALUE(NHU_TPOLL_RETX_MS190);
        case NhuT_PollRetransmitms195Enum:
            RETVALUE(NHU_TPOLL_RETX_MS195);
        case NhuT_PollRetransmitms200Enum:
            RETVALUE(NHU_TPOLL_RETX_MS200);
        case NhuT_PollRetransmitms205Enum:
            RETVALUE(NHU_TPOLL_RETX_MS205);
        case NhuT_PollRetransmitms210Enum:
            RETVALUE(NHU_TPOLL_RETX_MS210);
        case NhuT_PollRetransmitms215Enum:
            RETVALUE(NHU_TPOLL_RETX_MS215);
        case NhuT_PollRetransmitms220Enum:
            RETVALUE(NHU_TPOLL_RETX_MS220);
        case NhuT_PollRetransmitms225Enum:
            RETVALUE(NHU_TPOLL_RETX_MS225);
        case NhuT_PollRetransmitms230Enum:
            RETVALUE(NHU_TPOLL_RETX_MS230);
        case NhuT_PollRetransmitms235Enum:
            RETVALUE(NHU_TPOLL_RETX_MS235);
        case NhuT_PollRetransmitms240Enum:
            RETVALUE(NHU_TPOLL_RETX_MS240);
        case NhuT_PollRetransmitms245Enum:
            RETVALUE(NHU_TPOLL_RETX_MS245);
        case NhuT_PollRetransmitms250Enum:
            RETVALUE(NHU_TPOLL_RETX_MS250);
        case NhuT_PollRetransmitms300Enum:
            RETVALUE(NHU_TPOLL_RETX_MS300);
        case NhuT_PollRetransmitms350Enum:
            RETVALUE(NHU_TPOLL_RETX_MS350);
        case NhuT_PollRetransmitms400Enum:
            RETVALUE(NHU_TPOLL_RETX_MS400);
        case NhuT_PollRetransmitms450Enum:
            RETVALUE(NHU_TPOLL_RETX_MS450);
        case NhuT_PollRetransmitms500Enum:
            RETVALUE(NHU_TPOLL_RETX_MS500);
        default:
            RETVALUE(NHU_TPOLL_RETX_MS500);
    }

}


/** @brief This function is responsible for to Get cfg status from proh timer
 * 
 *  @details
 * Function:wrUmmRrcGetCfgStatusProhTmr2
 *
 *   Processing steps:
 *    - Get cfg status from proh timer
 *     
 * @param[in]  statusProhTmr: status Proh Timer
 * @return U16
 *       RETVALUE:enum
**/
PRIVATE U16 wrUmmRrcGetCfgStatusProhTmr2
(
  U32                          statusProhTmr
  )
{
    switch(statusProhTmr)
    {
        case NhuT_StatusProhibitms145Enum:
            RETVALUE(NHU_STATUS_PROH_MS145);
        case NhuT_StatusProhibitms150Enum:
            RETVALUE(NHU_STATUS_PROH_MS150);
        case NhuT_StatusProhibitms155Enum:
            RETVALUE(NHU_STATUS_PROH_MS155);
        case NhuT_StatusProhibitms160Enum:
            RETVALUE(NHU_STATUS_PROH_MS160);
        case NhuT_StatusProhibitms165Enum:
            RETVALUE(NHU_STATUS_PROH_MS165);
        case NhuT_StatusProhibitms170Enum:
            RETVALUE(NHU_STATUS_PROH_MS170);
        case NhuT_StatusProhibitms175Enum:
            RETVALUE(NHU_STATUS_PROH_MS175);
        case NhuT_StatusProhibitms180Enum:
            RETVALUE(NHU_STATUS_PROH_MS180);
        case NhuT_StatusProhibitms185Enum:
            RETVALUE(NHU_STATUS_PROH_MS185);
        case NhuT_StatusProhibitms190Enum:
            RETVALUE(NHU_STATUS_PROH_MS190);
        case NhuT_StatusProhibitms195Enum:
            RETVALUE(NHU_STATUS_PROH_MS195);
        case NhuT_StatusProhibitms200Enum:
            RETVALUE(NHU_STATUS_PROH_MS200);
        case NhuT_StatusProhibitms205Enum:
            RETVALUE(NHU_STATUS_PROH_MS205);
        case NhuT_StatusProhibitms210Enum:
            RETVALUE(NHU_STATUS_PROH_MS210);
        case NhuT_StatusProhibitms215Enum:
            RETVALUE(NHU_STATUS_PROH_MS215);
        case NhuT_StatusProhibitms220Enum:
            RETVALUE(NHU_STATUS_PROH_MS220);
        case NhuT_StatusProhibitms225Enum:
            RETVALUE(NHU_STATUS_PROH_MS225);
        case NhuT_StatusProhibitms230Enum:
            RETVALUE(NHU_STATUS_PROH_MS230);
        case NhuT_StatusProhibitms235Enum:
            RETVALUE(NHU_STATUS_PROH_MS235);
        case NhuT_StatusProhibitms240Enum:
            RETVALUE(NHU_STATUS_PROH_MS240);
        case NhuT_StatusProhibitms245Enum:
            RETVALUE(NHU_STATUS_PROH_MS245);
        case NhuT_StatusProhibitms250Enum:
            RETVALUE(NHU_STATUS_PROH_MS250);
        case NhuT_StatusProhibitms300Enum:
            RETVALUE(NHU_STATUS_PROH_MS300);
        case NhuT_StatusProhibitms350Enum:
            RETVALUE(NHU_STATUS_PROH_MS350);
        case NhuT_StatusProhibitms400Enum:
            RETVALUE(NHU_STATUS_PROH_MS400);
        case NhuT_StatusProhibitms450Enum:
            RETVALUE(NHU_STATUS_PROH_MS450);
        case NhuT_StatusProhibitms500Enum:
            RETVALUE(NHU_STATUS_PROH_MS500);
        default:
            RETVALUE(NHU_STATUS_PROH_MS500);

    }
}

/** @brief This function is responsible for to Get cfg Reordering Timer
 * 
 *  @details
 * Function:wrUmmRrcGetCfgReorderingTmr2
 *
 *   Processing steps:
 *    - Get cfg Reordering Timer
 *     
 * @param[in]  reorderingTmr: Reordering Timer
 * @return U16
 *       RETVALUE:enum
**/
PRIVATE U16 wrUmmRrcGetCfgReorderingTmr2
(
  U32                          reorderingTmr
  )
{
    switch(reorderingTmr)
    {
        case NhuT_Reorderingms140Enum:
            RETVALUE(NHU_TREORD_MS140);
        case NhuT_Reorderingms150Enum:
            RETVALUE(NHU_TREORD_MS150);
        case NhuT_Reorderingms160Enum:
            RETVALUE(NHU_TREORD_MS160);
        case NhuT_Reorderingms170Enum:
            RETVALUE(NHU_TREORD_MS170);
        case NhuT_Reorderingms180Enum:
            RETVALUE(NHU_TREORD_MS180);
        case NhuT_Reorderingms190Enum:
            RETVALUE(NHU_TREORD_MS190);
        case NhuT_Reorderingms200Enum:
            RETVALUE(NHU_TREORD_MS200);
        default:
            RETVALUE(NHU_TREORD_MS200);
    }
}

PUBLIC U32  wrGetRptInvlValFromEnum
(
 U32 enumVal
) 
{
   U32 val;
   switch(enumVal)
   {
      case NhuReportIntervalms120Enum:
         val = 120;
         break;
      case NhuReportIntervalms240Enum:
         val = 240;
         break;
      case NhuReportIntervalms480Enum:
         val = 480;
         break;
      case NhuReportIntervalms640Enum:
         val = 640;
         break;
      case NhuReportIntervalms1024Enum:
         val = 1024;
         break;
      case NhuReportIntervalms2048Enum:
         val = 2048;
         break;
      case NhuReportIntervalms5120Enum:
         val = 5120;
         break;
      case NhuReportIntervalms10240Enum:
         val = 10240;
         break;
      case NhuReportIntervalmin1Enum:
         val = 60000;
         break;
      case NhuReportIntervalmin6Enum:
         val = 360000;
         break;
      case NhuReportIntervalmin12Enum:
         val = 720000;
         break;
      case NhuReportIntervalmin30Enum:
         val = 1800000;
         break;
      case NhuReportIntervalmin60Enum:
         val = 3600000;
         break;
      default:
         val = 240;
         break;
   }
   RETVALUE(val);
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
