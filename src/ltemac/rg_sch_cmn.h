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


/**********************************************************************
     Name:    LTE MAC SC1 scheduler 

     Type:    C header file

     Desc:    Defines required by SC1 scheduler

     File:    rg_sch_cmn.h

     Sid:      rg_sch_cmn.h@@/main/2 - Sat Jul 30 02:22:02 2011

     Prg:      

*********************************************************************21*/


#ifndef __RGSCHCMNH__
#define __RGSCHCMNH__ 
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/

/* Scheduler1 tunable params */
#define RG_SCH_CMN_GET_CELL(cell) ((RgSchCmnCell *)((cell)->sc.sch))
#define RG_SCH_CMN_GET_UL_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->ul)
#define RG_SCH_CMN_GET_DL_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->dl)
#define RG_SCH_CMN_GET_UE(ue) ((RgSchCmnUe *)((ue)->sch))
#define RG_SCH_CMN_GET_UL_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->ul)
#define RG_SCH_CMN_GET_DL_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->dl)
#define RG_SCH_CMN_GET_CMN_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->cmn)
#define RG_SCH_CMN_GET_DL_HQP(hqP) ((RgSchCmnDlHqProc *)((hqP)->sch))
#define RG_SCH_CMN_GET_DL_SVC(svc) ((RgSchCmnDlSvc *)((svc)->sch))
#define RG_SCH_CMN_GET_UL_LCG(lcg) ((RgSchCmnLcg *)((lcg)->sch))

/* Changing the check for retransmission - now checking if alloc
 * is non NULL instead of using the isRetx field of the harq Proc. 
 */
#define RG_SCH_CMN_IS_UL_UE_RETX(ue) ((&(ue)->ul.hqEnt.hqProcCb[\
 ((RgSchCmnCell *)(ue->cell->sc.sch))->ul.schdHqProcIdx])->alloc)

   /* Added support for SPS*/
#ifdef LTEMAC_SPS
#define RGSCH_SPS_MAX_SPS_ACTVN_BO                 400
#define RGSCH_SPS_MAX_DL_HQP_TX                    4
#define RGSCH_SPS_MAX_UL_ACT_CRC_FAIL              3
#define RGSCH_SPS_MAX_UL_SPS_OCC_CRC_FAIL          3
#define RGSCH_SPS_MAX_UL_REL_PDCCH                 2
#define RGSCH_SPS_ULBW_MASK_LEN                    4
#define RGSCH_SPS_MG_MASK_LEN                      10
#define RG_SCH_CMN_GET_UL_SPS_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->ul.ulSpsInfo)
#define RG_SCH_CMN_GET_UL_SPS_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->ul.ulSpsInfo)

#define RG_SCH_CMN_SID_PACKET_SIZE 10
#define RG_SCH_CMN_SPS_BSR_HEADROOM 6 /* PHR 2 bytes + Long BSR 4 bytes*/


#define RGSCH_CMN_MIN_SCALABLE_CQI  RGSCH_SPS_CQI_SCALE_FACTOR+1
/* Introduced timing delta for DL control in FDD */
#ifdef LTE_TDD
#define RGSCH_SPS_UL_LCM   40 /* Default lcm to start the spsSfTbl with */
#else
#define RGSCH_SPS_UL_DELTA (TFU_DLCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
#define RGSCH_SPS_UL_LCM   20 /* Default lcm to start the spsSfTbl with */
#endif


#define RG_SCH_CMN_IS_SPS_SCHD(ue) ((((&(ue)->ul.hqEnt.hqProcCb[\
 ((RgSchCmnCell *)(ue->cell->sc.sch))->ul.schdHqProcIdx])->alloc ? TRUE:FALSE)) || \
 (ue->ul.relPdcchSchdTime.sfn == cell->crntTime.sfn && ue->ul.relPdcchSchdTime.subframe == cell->crntTime.subframe))
#endif /* LTEMAC_SPS */

/* RRM_SP1_START */
#define RG_SCH_CALC_GBR_UTILIZATION(_cell, _lcCb, _prbUsed) {\
   if(_lcCb->qciCb->qci <= RGM_MAX_QCI_REPORTS)\
   {\
      _cell->prbUsage.qciPrbRpts[_lcCb->qciCb->qci-1].dlTotPrbUsed += _prbUsed;\
      _cell->prbUsage.qciPrbRpts[_lcCb->qciCb->qci-1].qci = _lcCb->qciCb->qci;\
   }\
}
/* RRM_SP1_END */


   /* This is added due to the limitation in BRDCM Platform 
    * BRDCM support only one Aperiodic Cqi reception in one UL Sf at lower MCS 
    * Note: Should be removed once BRDCM provides support for this */
#define RG_SCH_MAX_ACQI_PER_ULSF 1

#define RG_SCH_CMN_IS_UE_SCHDLD(ue) (((RgSchCmnUe *)((ue)->sch))->dl.proc != NULLP)
#define RG_SCH_CMN_PROC_SLCTD_FOR_RETX(proc) ((proc->tbInfo[0].txCntr!=0) ||\
                                               (proc->tbInfo[1].txCntr!=0))
#define RG_SCH_CMN_DL_IS_UE_ACTIVE(ue) (ue->dl.dlInactvMask == 0)
#define RG_SCH_CMN_UL_IS_UE_ACTIVE(ue) (ue->ul.ulInactvMask == 0) 
/* ulInactvMask and dlInactvMask are simulataneously set/reset
 * hence check against one suffices */
#define RG_SCH_CMN_IS_UE_PDCCHODR_INACTV(ue) (ue->ul.ulInactvMask & RG_PDCCHODR_INACTIVE) 

/* Adding routines to check Ue's Active state before triggering UE's Active transition */
#define RG_SCH_CMN_DL_UPDT_INACTV_MASK( cellCb, ue, maskElmnt) \
   if(RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))                          \
   {                                                           \
      ue->dl.dlInactvMask &= ~maskElmnt;                       \
   }                                                           \
   else                                                        \
   {                                                           \
      ue->dl.dlInactvMask &= ~maskElmnt;                       \
      if(RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))                       \
      {                                                        \
         rgSCHUtlDlActvtUe(cellCb, ue);                        \
      }                                                        \
   } 

#define RG_SCH_CMN_UL_UPDT_INACTV_MASK( cellCb, ue, maskElmnt) \
   if(RG_SCH_CMN_UL_IS_UE_ACTIVE(ue))                          \
   {                                                           \
      ue->ul.ulInactvMask &= ~maskElmnt;                       \
   }                                                           \
   else                                                        \
   {                                                           \
      ue->ul.ulInactvMask &= ~maskElmnt;                       \
      if(RG_SCH_CMN_UL_IS_UE_ACTIVE(ue))                       \
      {                                                        \
         rgSCHUtlUlActvtUe(cellCb, ue);                        \
      }                                                        \
   } 

#define RG_SCH_CMN_CSG_REFRESH_TIME 16
#define RG_SCH_CMN_OVRLDCTRL_REFRESH_TIME 50

/* totPrbCnt is set to 1 to avoid division by zero */
#define RG_SCH_RESET_HCSG_DL_PRB_CNTR(_cmnDlCell) \
{                                             \
   (_cmnDlCell)->ncsgPrbCnt = 0;                 \
   (_cmnDlCell)->totPrbCnt = 1;                  \
}

#define RG_SCH_RESET_HCSG_UL_PRB_CNTR(_cmnUlCell) \
{                                             \
   (_cmnUlCell)->ncsgPrbCnt = 0;                 \
   (_cmnUlCell)->totPrbCnt = 1;                  \
}

#define RG_SCH_CMN_DL_SVC_IS_GBR(svc) ((((RgSchCmnDlSvc*)(svc->sch))->qci >= RG_SCH_CMN_GBR_QCI_START) && \
                                (((RgSchCmnDlSvc*)(svc->sch))->qci <= RG_SCH_CMN_GBR_QCI_END))

/* Moved the below tables to rg_env_*.h files */


#define RG_SCH_CMN_SET_FORCE_TD(_ue, _event)\
         {\
            RgSchCmnDlUe *_ueDl = RG_SCH_CMN_GET_DL_UE((_ue));\
            _ueDl->mimoInfo.forceTD |= (_event);\
         }
#define RG_SCH_CMN_UNSET_FORCE_TD(_ue, _event)\
         {\
            RgSchCmnDlUe *_ueDl = RG_SCH_CMN_GET_DL_UE((_ue));\
            _ueDl->mimoInfo.forceTD &= ~(_event);\
         }
#define RG_SCH_CMN_INIT_FORCE_TD(_ue, _val)\
         {\
            RgSchCmnDlUe *_ueDl = RG_SCH_CMN_GET_DL_UE((_ue));\
            _ueDl->mimoInfo.forceTD = (_val);\
         }

#define RG_SCH_CMN_DL_TBS_TO_MCS(x, y) do {\
   if (x <= 9) y = x; \
   else if (x <= 15) y = x + 1; \
   else y = x + 2;\
} while(0)

#define RG_SCH_CMN_DL_MCS_TO_TBS(x, y) do {\
   if (x <= 9) y = x; \
   else if (x <= 16) y = x - 1; \
   else y = x - 2; \
}while(0)

#define RG_SCH_CMN_UL_TBS_TO_MCS(x, y) do {\
   if (x <= 10) y = x; \
   else if (x <= 19) y = x + 1; \
   else y = x + 2;\
} while(0)

#ifdef LTE_TDD
#define RG_SCH_CMN_CHK_DL_DATA_ALLOWED(_cell, _idx)\
(rgSchTddUlDlSubfrmTbl[_cell->ulDlCfgIdx][_idx] == RG_SCH_TDD_DL_SUBFRAME) ||\
((rgSchTddUlDlSubfrmTbl[_cell->ulDlCfgIdx][_idx] == RG_SCH_TDD_SPL_SUBFRAME) &&\
 (_cell->splSubfrmCfg.isDlDataAllowed == TRUE))
#define RG_SCH_CMN_ADJ_DWPTS_ITBS(_cellDl, _iTbs) \
{\
    if (_iTbs > 0)\
    {\
       _iTbs += rgSchCmnSplSfDeltaItbs[_cellDl->splSfCfg];\
       if ((S8)_iTbs < 0)\
       {\
          _iTbs = 0;\
       }\
    }\
    if (_iTbs > 26)\
    {\
       _iTbs = 26;\
    }\
}
#endif

/* RACHO start */
/* minimum grant, in bytes, to be given to HO and pdcchOrdered UEs */
#define RG_SCH_MIN_GRNT_HOPO 2
/* maximum dedPrmbls */
#define RG_SCH_MAX_DED_PRMBLS 60
/* is PDCCH order generation supported */
#define RG_SCH_CMN_IS_PO_SPRTD(cell) (cell->rachCfg.raOccasion.sfnEnum != RGR_SFN_NA)
/* Min gap value between crntTime and time of PRACH Oppurtunity */
/* RG_SCH_CMN_DL_DELTA is the number of SFs from crntTime at which
 * UE is expected to recieve PDCCH Order.
 * n+6 as per 213 6.1.1 last para */
#define RG_SCH_CMN_MIN_PRACH_OPPR_GAP (6+RG_SCH_CMN_DL_DELTA)
/* Idle time threshold in terms of subframes, implies
 * the max duration between a TA expiry and latest UL
 * data/Signal transmission time */
 /* Fix : syed Ignore if TaTmr is not configured */
#define RG_SCH_CMN_UE_IDLE_THRSLD(ue) (RG_SCH_CMN_UE_IDLETIME_FCTR * ue->dl.taCb.cfgTaTmr)
/* R8 Upgrade */
#define RG_SCH_CMN_GET_BI_VAL(prevVal,numUe) ( prevVal + ( numUe * RG_SCH_CMN_BI_NUMUE_FACTOR ))
#define RG_SCH_CMN_NUMUE_FACTOR    1
#define RG_SCH_CMN_BITBL_INDEX(x) ((x/RG_SCH_CMN_NUMUE_FACTOR)>=12)? 12:(x/RG_SCH_CMN_NUMUE_FACTOR)
#define RG_SCH_CMN_GET_BI(numUe)  rgSchCmnBiTbl[RG_SCH_CMN_BITBL_INDEX((numUe))]
/* RACHO end */

#define RG_SCH_CMN_SVC_IS_GBR(svc) ((((RgSchCmnDlSvc*)(svc->sch))->qci >= RG_SCH_CMN_GBR_QCI_START) && \
                                (((RgSchCmnDlSvc*)(svc->sch))->qci <= RG_SCH_CMN_GBR_QCI_END))

#define RG_SCH_CMN_SVC_IS_AMBR(svc) ((((RgSchCmnDlSvc*)(svc->sch))->qci > RG_SCH_CMN_GBR_QCI_END) && \
                                (((RgSchCmnDlSvc*)(svc->sch))->qci <= RG_SCH_CMN_MAX_QCI))

#define RG_SCH_CMN_TBS_TO_MODODR(x, y) do {\
   if (x <= 9) y = 2; \
   else if (x <= 15) y = 4; \
   else y = 6;\
} while(0)

/* To include the length and ModOrder in DataRecp Req. */
#define RG_SCH_UL_MCS_TO_MODODR(x, y) do {\
   RGSCH_ARRAY_BOUND_CHECK(0, rgUlIMcsTbl, x); \
   y = (TfuModScheme)rgUlIMcsTbl[x].qm;\
} while(0)

#define RG_SCH_CMN_ITBS_TO_RETX_IMCS(iTbs, iMcs) do {\
   if ((iTbs) <= 9) (iMcs) = 29; \
   else if ((iTbs) <= 15) (iMcs) = 30; \
   else (iMcs) = 31;\
} while(0)

/* Fix for ccpu00123919: In case of RETX TB scheduling avoiding recomputation of RB
 * and Tbs. Set all parameters same as Init TX except RV(only for NACKED) and
 * MCS.  */
#define RG_SCH_CMN_GET_MCS_FOR_RETX(tb, retxMcs) do {\
      if ((tb->isAckNackDtx == TFU_HQFDB_DTX)) { \
         retxMcs = tb->dlGrnt.iMcs; \
      } \
      else { \
         if (tb->dlGrnt.iMcs < 29) {\
            U8 _iTbs;\
            RG_SCH_CMN_DL_MCS_TO_TBS(tb->dlGrnt.iMcs, _iTbs);\
            RG_SCH_CMN_ITBS_TO_RETX_IMCS(_iTbs, retxMcs); \
         } \
         else {\
            retxMcs = tb->dlGrnt.iMcs; \
         }\
      }\
}while(0)

#define RG_SCH_CMN_DL_TBS_TO_MCS_DTX(proc, iTbs, imcs) do {\
      if ((proc->isAckNackDtx == TFU_HQFDB_DTX)) { \
         RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, imcs); \
      } \
      else { \
         RG_SCH_CMN_ITBS_TO_RETX_IMCS(iTbs, imcs); \
      } \
}while(0)

#define RG_SCH_CMN_UL_IS_CQI_VALID(cqi) ((cqi) > 0 && (cqi) < RG_SCH_CMN_UL_NUM_CQI)

#ifdef CCPU_OPT
#define RG_SCH_CMN_DL_GET_HDR_EST(svc, hdrEst) do {\
   hdrEst  = svc->estRlcHdrSz;\
   hdrEst += RG_SCH_CMN_DED_MAX_HDRSIZE * RG_SCH_CMN_MAX_DED_SDU;\
   if (svc->staPduPrsnt) \
   {\
      hdrEst += RG_SCH_CMN_DED_MAX_HDRSIZE;\
   }\
} while(0)
#else
#define RG_SCH_CMN_DL_GET_HDR_EST(svc, hdrEst) do {\
   hdrEst = RG_SCH_CMN_DED_MAX_HDRSIZE * RG_SCH_CMN_MAX_DED_SDU;\
} while(0)
#endif

#define RGSCH_CMN_MIN_GRNT_HDR (RG_SCH_CMN_DED_MAX_HDRSIZE * RG_SCH_CMN_MAX_DED_SDU + 1)

#define RG_SCH_CMN_MAX_UL_CONTRES_GRNT   4
#define RG_SCH_CMN_MAX_ALLOC_TRACK       10
/* Introduced timing delta for UL control in FDD*/
#define RG_SCH_CMN_MIN_BSR_RECP_INTRVL   (TFU_ULCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
#define RG_SCH_CMN_MIN_MSG3_RECP_INTRVL  RG_SCH_CMN_DL_DELTA + RGSCH_RARSP_MSG3_DELTA
/* Introduced timing delta for DL control in FDD */
/* This interval RG_SCH_CMN_MIN_RETXMSG3_RECP_INTRVL is used in FDD only */
#ifndef LTE_TDD
#define RG_SCH_CMN_MIN_RETXMSG3_RECP_INTRVL (TFU_DLCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
#endif
/* Fixing the priority table to be more in line with the spec 23.203,table 
 * 6.1.7 */
#define RG_SCH_CMN_QCI_TO_PRIO           {1, 3, 2, 4, 0, 5, 6, 7, 8}
#define RG_SCH_CMN_DCCH_PRIO             0

#define RG_SCH_CMN_GBR_QCI_START     1
#define RG_SCH_CMN_GBR_QCI_END       4
#define RG_SCH_CMN_NGBR_QCI_START    6
#define RG_SCH_CMN_NGBR_QCI_END      9

/* Introduced min & max qci for validation of qci */
#define RG_SCH_CMN_MIN_QCI               1
#define RG_SCH_CMN_MAX_QCI               9
#define RG_SCH_CMN_NUM_QCI               9
#define RG_SCH_CMN_MAX_CP                2
#define RG_SCH_CMN_NOR_CP                0
#define RG_SCH_CMN_EXT_CP                1
#define RG_SCH_CMN_NUM_TBS               27
#define RG_SCH_CMN_MAX_CQI               16
#define RB_SCH_CMN_NUM_SCS_PER_RB        12
#define RG_SCH_CMN_NUM_RBS               110
#define RG_SCH_CMN_UL_NUM_SF             RGSCH_NUM_UL_HQ_PROC+8
#define RG_SCH_CMN_UL_NUM_RE_PER_RB(cell) ((cell)->ulNumRePerRb)  
#ifdef LTE_TDD
#define RG_SCH_CMN_MAX_CMN_PDCCH 6
#else
#define RG_SCH_CMN_MAX_CMN_PDCCH 4
#endif
#define RG_SCH_CMN_UL_MAX_CQI            16
#define RG_SCH_CMN_UL_SR_BYTES           1
/* Refresh Timer Defines */
/* MS_WORKAROUND : syed tuning refresh time to 100ms for PFS */
#define RG_SCH_CMN_REFRESH_TIME 32 /* Refresh time/cycle in frames (10ms) */
/* Fix: syed align multiple UEs to refresh at same time */
#define RG_SCH_CMN_REFRESH_TIMERES 10
#define RG_SCH_CMN_NUM_REFRESH_Q    16
#define RG_SCH_CMN_EVNT_UE_REFRESH  1

#define RG_SCH_CMN_TPC_ACC_DEC_THRESHOLD  1
#define RG_SCH_CMN_TPC_ACC_INC_1DB_THRESHOLD  1
#define RG_SCH_CMN_TPC_ACC_INC_3DB_THRESHOLD  3
#define RG_SCH_CMN_TPC_ABS_DEC_4DB_THRESHOLD  4
#define RG_SCH_CMN_TPC_ABS_DEC_1DB_THRESHOLD  1
#define RG_SCH_CMN_TPC_ABS_INC_1DB_THRESHOLD  4
#define RG_SCH_CMN_TPC_ABS_INC_4DB_THRESHOLD  4

/* ccpu00117606 - ADD - Include CRS REs while calculating Efficiency */
#define RG_SCH_CMN_MAX_ANT_CONF               3
#define RG_SCH_CMN_NUM_SLOTS_PER_SF           2
/* ccpu00117606 - ADD - Defines for Effective Cell RS for different Tx Ant Ports */
#define RG_SCH_CMN_EFF_CRS_ONE_ANT_PORT       6
#define RG_SCH_CMN_EFF_CRS_TWO_ANT_PORT       12
#define RG_SCH_CMN_EFF_CRS_FOUR_ANT_PORT      16

/* ADD-new defines for Min & Max RI values */
#define RG_SCH_CMN_MIN_RI 1 
#define RG_SCH_CMN_MAX_RI 4 

#define RG_SCH_CMN_MAX_CW_PER_UE 2

#define RG_SCH_CMN_IS_RI_VALID(ri) \
   (ri >= RG_SCH_CMN_MIN_RI && ri <= RG_SCH_CMN_MAX_RI)

#define RGSCHCMNADDTOCRNTTIME(crntTime, toFill, incr) \
{\
   U32 absoluteTime;\
   absoluteTime = crntTime.sfn * RGSCH_NUM_SUB_FRAMES + crntTime.subframe;\
   absoluteTime += incr;\
   toFill.sfn = absoluteTime/RGSCH_NUM_SUB_FRAMES;\
   toFill.subframe = absoluteTime % RGSCH_NUM_SUB_FRAMES;\
}

#define RG_SCH_CMN_RARSP_WAIT_PRD   3

#define RG_SCH_CMN_MAX_SPL_CFI      2
#define RG_SCH_CMN_INVALID_INFO     0xff
#define RG_SCH_CMN_NUM_SUBCAR       12

#define RG_SCH_CMN_SUBFRM_0      0
#define RG_SCH_CMN_SPL_SUBFRM_1  1
#define RG_SCH_CMN_SUBFRM_5      5
#define RG_SCH_CMN_SPL_SUBFRM_6  6

#define RG_SCH_CMN_VALUE_ONE 1

#define RG_SCH_CMN_IS_ODD(x) ((x) & 0x01)

#define RG_SCH_CMN_MAX_NUM_OF_SFN 10240
#define RG_SCH_CMN_MAX_SFN_NUM 1023

#define RG_SCH_CMN_NUM_DL_AT_SWTCHPT   2

#define RG_SCH_CMN_10_MS_PRD      10
#define RG_SCH_CMN_5_MS_PRD       5

#define RG_SCH_CMN_DECR_FRAME(_x, _y) {\
   S16 _tmpNo = (_x) - (_y); \
   if(_tmpNo < 0) { \
      (_x) = _tmpNo + RGSCH_MAX_SFN; \
   }\
   else {\
      (_x) = _tmpNo; \
   }\
}
/* ADD-new hash define for max msg3 mcs val */
#define RG_SCH_CMN_MAX_MSG3_IMCS 15

#define RG_SCH_CMN_CALC_RARSPLST_SIZE(cell, raArrsz) {\
   S16 _sfNum=0; \
   /* Get the last UL subframe no */ \
   for(_sfNum=RGSCH_NUM_SUB_FRAMES-1; _sfNum >= 0; _sfNum--) \
   { \
      if(rgSchTddUlDlSubfrmTbl[(cell)->ulDlCfgIdx][_sfNum] == \
            RG_SCH_TDD_UL_SUBFRAME) \
      { \
         break; \
      } \
   } \
   (raArrSz) = (_sfNum + ((RgSchCmnCell *)(cell)->sc.sch)->dl.numRaSubFrms \
                + RG_SCH_CMN_RARSP_WAIT_PRD +  \
                (cell)->rachCfg.raWinSize - 1) / RGSCH_NUM_SUB_FRAMES + 1; \
}

/* Resource allocation type MACROs */
#define RG_SCH_CMN_RA_TYPE0 0
#define RG_SCH_CMN_RA_TYPE1 1
#define RG_SCH_CMN_RA_TYPE2 2
/* Added support for SPS*/
#ifdef LTEMAC_SPS
#define RG_SCH_SPS_CONS_DYN_SCHD 5
#define RG_SCH_SPS_CONS_RED_BO   5
#define RG_SCH_DL_SPS_ADDTL_BO   RGSCH_TA_SIZE /* 2 Bytes for TA */
/* RBG subset MACROs for RA type 1 */
#define RG_SCH_CMN_RBG_SUBSET0 0
#define RG_SCH_CMN_RBG_SUBSET1 1
#define RG_SCH_CMN_RBG_SUBSET2 2
#define RG_SCH_CMN_RBG_SUBSET3 3
#define RG_SCH_CMN_DL_NUM_ALLOCMASK 9
#define RG_SCH_CMN_SPS_MAX_PRD 640
#define RG_SCH_SPS_SID_INTERVAL 80 
#define RG_SCH_CMN_SPS_DL_ACTV   (1 << 0)
#define RG_SCH_CMN_SPS_DL_REACTV_FREQ (1 << 1)
#define RG_SCH_CMN_SPS_DL_REACTV_TIME (1 << 2)
#define RG_SCH_CMN_SPS_DL_REACTV \
(RG_SCH_CMN_SPS_DL_REACTV_FREQ | RG_SCH_CMN_SPS_DL_REACTV_TIME) 
#define RG_SCH_CMN_SPS_DL_REL    (1 << 3)
#define RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE 4
/* Number of 32 bit bitmasks for marking measurement gap for SPS */
#define RG_SCH_CMN_SPS_DL_MEASGAP_32BITMASK_SIZE 3  

/* 32 Bit mask size for n1Pucch: RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF/32 */
#define RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE \
   ((RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF + 31)/32)
#define RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL 20

/* Maximum number of feasible periodicity values for SPS, SRS, CQI and SR */
#define RG_SCH_CMN_SPS_MAX_NUM_PRD 21

/* Maximum value of iMcs for SPS UE */
#define RG_SCH_CMN_SPS_DL_MAX_MCS 15

/* Minimum gap between SPEECH_GOOD packet and SID packet */
#define RG_SCH_CMN_MIN_GAP_FOR_SID 60

/* DL SPS function MACROs */

#define RG_SCH_CMN_SPS_GET_DL_CELL(cell) &(((RgSchCmnCell *)((cell)->sc.sch))->dl.dlSpsInfo)
#define RG_SCH_CMN_SPS_GET_DL_UE(ue) &(((RgSchCmnUe *)((ue)->sch))->dl.dlSpsInfo)
#define RG_SCH_CMN_SPS_GET_DL_SVC(svc) &(((RgSchCmnDlSvc *)((svc)->sch))->dlSvcSpsInfo)
#define RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP) (((RgSchCmnDlHqProc *)((hqP)->sch))->isSpsSvcSchd)
#define RG_SCH_CMN_SPS_DL_IS_SPS_TX_HQP(hqP) (((RgSchCmnDlHqProc *)((hqP)->sch))->isSpsActv)
#define RG_SCH_CMN_IS_UE_SPS_SCHDLD(_ue, _schdTime)\
(((_ue)->dl.dlAllocCb.spsSchdTime.sfn == _schdTime.sfn) &&\
 ((_ue)->dl.dlAllocCb.spsSchdTime.subframe == _schdTime.subframe))


#define RG_SCH_CMN_DL_COUNT_ONES(_bitMask, _size, _numOnes)\
{\
   U8 _pos = 0;\
   *_numOnes = 0;\
   for (_pos = 0; _pos < _size; ++_pos)\
   {\
      *_numOnes += (_bitMask & (1 << (31 - _pos))) ? 1: 0;\
   }\
}

#define RG_SCH_CMN_DL_GET_START_POS(_allocedBitmask, _size, _startPos)\
{\
   U8 _pos = 0;\
   for (_pos = 0; _pos < _size; ++_pos)\
   {\
      if ((_allocedBitmask & (1 << (31 -_pos))))\
      {\
         continue;\
      }\
      else\
      {\
         *_startPos = _pos;\
         break;\
      }\
   }\
}

/* This macros returns position of idx in a 32 bit bitmask from LSB */
#define RG_SCH_CMN_DL_GET_POS_FRM_LSB(_idx) (31 - (_idx)) 
   
#define RG_SCH_CMN_SPS_GET_PRD_IDX(_prdVal, _prdIdx)\
{\
   switch (_prdVal)\
   {\
      case 2: *_prdIdx = RG_SCH_CMN_SPS_PRD_2SF; break;\
      case 5: *_prdIdx = RG_SCH_CMN_SPS_PRD_5SF; break;\
      case 10: *_prdIdx = RG_SCH_CMN_SPS_PRD_10SF; break;\
      case 20: *_prdIdx = RG_SCH_CMN_SPS_PRD_20SF; break;\
      case 30: *_prdIdx = RG_SCH_CMN_SPS_PRD_30SF; break;\
      case 32: *_prdIdx = RG_SCH_CMN_SPS_PRD_32SF; break;\
      case 40: *_prdIdx = RG_SCH_CMN_SPS_PRD_40SF; break;\
      case 60: *_prdIdx = RG_SCH_CMN_SPS_PRD_60SF; break;\
      case 64: *_prdIdx = RG_SCH_CMN_SPS_PRD_64SF; break;\
      case 80: *_prdIdx = RG_SCH_CMN_SPS_PRD_80SF; break;\
      case 120: *_prdIdx = RG_SCH_CMN_SPS_PRD_120SF; break;\
      case 128: *_prdIdx = RG_SCH_CMN_SPS_PRD_128SF; break;\
      case 160: *_prdIdx = RG_SCH_CMN_SPS_PRD_160SF; break;\
      case 256: *_prdIdx = RG_SCH_CMN_SPS_PRD_256SF; break;\
      case 320: *_prdIdx = RG_SCH_CMN_SPS_PRD_320SF; break;\
      case 512: *_prdIdx = RG_SCH_CMN_SPS_PRD_512SF; break;\
      case 640: *_prdIdx = RG_SCH_CMN_SPS_PRD_640SF; break;\
      case 1024: *_prdIdx = RG_SCH_CMN_SPS_PRD_1024SF; break;\
      case 1280: *_prdIdx = RG_SCH_CMN_SPS_PRD_1280SF; break;\
      case 2048: *_prdIdx = RG_SCH_CMN_SPS_PRD_2048SF; break;\
      case 2560: *_prdIdx = RG_SCH_CMN_SPS_PRD_2560SF; break;\
      default:   *_prdIdx = RG_SCH_CMN_SPS_PRD_INVALID;break;\
   }\
}
/* To be part of rg_env.h */
/* Maximum n1Pucch values per sub-frame: multiple of 32 */
#define RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF  96 
#define RG_SCH_SPS_DFLT_PRD 20
#define RG_SCH_SPS_CQI_DECR_VAL 2

#ifdef LTE_TDD
/* Added RgrSpsPrd casting to overcome G++ compilation warning*/
#define RGSCH_SPS_GET_PRDCTY(_prdEnum, _prd) \
{ \
   switch((_prdEnum)) \
   { \
      case RGR_SPS_PRD_10SF: \
         (_prd) = (RgrSpsPrd)10; \
         break; \
      case RGR_SPS_PRD_20SF: \
         (_prd) = (RgrSpsPrd)20; \
         break; \
      case RGR_SPS_PRD_32SF: \
         (_prd) = (RgrSpsPrd)30; \
         break; \
      case RGR_SPS_PRD_40SF: \
         (_prd) = (RgrSpsPrd)40; \
         break; \
      case RGR_SPS_PRD_64SF: \
         (_prd) =(RgrSpsPrd)60; \
         break; \
      case RGR_SPS_PRD_80SF: \
         (_prd) = (RgrSpsPrd)80; \
         break; \
      case RGR_SPS_PRD_128SF: \
         (_prd) = (RgrSpsPrd)120; \
         break; \
      case RGR_SPS_PRD_160SF: \
         (_prd) = (RgrSpsPrd)160; \
         break; \
      case RGR_SPS_PRD_320SF: \
         (_prd) = (RgrSpsPrd)320; \
         break; \
      case RGR_SPS_PRD_640SF: \
         (_prd) = (RgrSpsPrd)640; \
         break; \
      default:\
         (_prd) = RGR_SPS_PRD_INVALID;\
   } \
}
#endif /*LTE_TDD*/
/* ADD-hash define for actual transmission time */
/* Feedback for RelPdcch should be received at MAC by HARQ_INTERVAL+
 * RG_TFU_HQ_IND_DELTA time.*/
#define RG_SCH_CMN_SPS_TX_TIME (RG_SCH_CMN_HARQ_INTERVAL + TFU_HQFBKIND_ULDELTA)
#endif /* LTEMAC_SPS */

/* LTE_ADV_FLAG_REMOVED_START */
#define RG_SCH_CMN_IS_UE_CELL_EDGE(_ue) _ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge; 
#define RG_SCH_CMN_IS_SFR_ENB(_rgSchCellCb)  _rgSchCellCb->lteAdvCb.sfrCfg.status;
#define RG_SCH_CMN_SFR_POOL(_subFrm)  _subFrm->sfrTotalPoolInfo;

#define RG_SCH_MAX_RNTP_SAMPLES 10000
/* LTE_ADV_FLAG_REMOVED_END */



/* DL allocation MACROs */
#define RG_SCH_CMN_GET_ALLOCCB_FRM_UE(_ue) &((_ue)->dl.dlAllocCb); 
#define RG_SCH_CMN_GET_ALLOCCB_FRM_RACB(_raCb) &((_raCb)->rbAllocInfo); 
#define RG_SCH_CMN_INIT_SCHD_LNK(_schdLstLnk, _node)\
{\
      (_schdLstLnk)->node = (PTR)_node;\
      (_schdLstLnk)->prev = NULLP;\
      (_schdLstLnk)->next = NULLP;\
}

/* Changes for MIMO feature addition */
#define RG_SCH_CMN_FILL_DL_TXINFO(_allocInfo, _rb, _sFlg, _prcInf, _numLyr, _sf)\
{\
   _allocInfo->rbsReq                    = _rb;\
   _allocInfo->mimoAllocInfo.swpFlg      = _sFlg;\
   _allocInfo->mimoAllocInfo.precIdxInfo = _prcInf;\
   _allocInfo->mimoAllocInfo.numTxLyrs   = _numLyr;\
   _allocInfo->dlSf                      = _sf;\
}

#define RG_SCH_CMN_FILL_DL_TBINFO(_tbInfo, _bytsReq, _iTbs, _imcs, _tbCb, _noLyr)\
{\
   (_tbInfo)->schdlngForTb = TRUE;\
   (_tbInfo)->bytesReq     = _bytsReq;\
   (_tbInfo)->iTbs         = _iTbs;\
   (_tbInfo)->imcs         = _imcs;\
   (_tbInfo)->tbCb         = _tbCb;\
   (_tbInfo)->noLyr        = _noLyr;\
}

/* Bit Masks to Force Transmit diversity scheme */
#define RG_SCH_CMN_TD_RI_1         0x01 /* Transmit Diversity due to RI==1 in case
                                           of TM3 */
#define RG_SCH_CMN_TD_NO_PMI       0x02 /* Transmit Diversity due to No PMI */
#define RG_SCH_CMN_TD_TXMODE_RECFG 0x04 /* Transmit Diversity due to TXMODE ReCfg */
#define RG_SCH_CMN_TD_TXSCHEME_CHNG 0x08 /* Transmit Diversity due to TX scheme
                                            change */

#define RG_SCH_MAX_UL_TX_ANT 2

/*Maximum achievable code rate for non 64QAM UEs.
 *Value should NEVER be > than 93. Refer to 36.213, Table 7.2.3-1*/
#define RG_SCH_CMN_MAX_CODE_RATE_16QAM    85  /* 85% code rate*/
#define RG_SCH_CMN_MAX_EFF_BITS         4096

/* Refer BI table from 36.321 Table 7.2.1 */ 
#define RG_SCH_CMN_NUM_BI_VAL           13 

/*New macro to determine UE Category. We use the stored "ueCat" to 
 * index a UE category array. Therefore, the stored ueCat is 1 less
 * than actual UE cateogry.*/
#define RG_SCH_CMN_GET_UE_CTGY(ue) ((RG_SCH_CMN_GET_UE(ue))->cmn.ueCat + 1)
/*ccpu00117270-ADD-END*/
   
#define RG_SCH_CMN_UPD_RBS_TO_ADD(_CELL,_DLSF,_ALLCINFO,_RBSREQ,_RBSTOADD) \
{\
          U8  addtlRbsAvl;\
          addtlRbsAvl = rgSCHCmnFindNumAddtlRbsAvl(_CELL,_DLSF,_ALLCINFO);\
          if(_RBSREQ > addtlRbsAvl)\
          {\
              _RBSTOADD = addtlRbsAvl;\
          }\
          else\
          {\
              _RBSTOADD = _RBSREQ;\
          }\
}
   /* ccpu00126002 ADD macro added to check wrap around when index is reached 
   MAX_CQI_RI_RPT_BUFF*/
#define RG_SCH_INCR_CQIRI_INDEX(idx)\
{\
   (idx)++;\
   if(MAX_CQI_RI_RPT_BUFF == idx)\
   {\
      idx = 0;\
   }\
}

#define RG_SCH_CHK_ITBS_RANGE(_iTbs, _maxiTbs) \
{\
   if (_iTbs < 0) \
   {\
      _iTbs = 0;\
   }\
   else if (_iTbs > (_maxiTbs))\
   {\
      _iTbs = (_maxiTbs);\
   }\
}


/* LTE_ADV_FLAG_REMOVED_START */
#define RG_SCH_CMN_SFR_UPD_RBS_TO_ADD_IN_POOL(_CELL,_DLSF,_POOLINFO,_ALLCINFO,_RBSREQ,_RBSTOADD) \
{\
   U8  addtlRbsAvl;\
   addtlRbsAvl = rgSCHCmnSFRFindNumAddtlRbsAvl(_CELL,_DLSF,_POOLINFO,_ALLCINFO);\
   if(_RBSREQ > addtlRbsAvl)\
   {\
      _RBSTOADD = addtlRbsAvl;\
   }\
   else\
   {\
      _RBSTOADD = _RBSREQ;\
   }\
}
/* LTE_ADV_FLAG_REMOVED_END */

/* DELTA for CFI applying */
#define RG_SCH_CFI_APPLY_DELTA      4

#define RG_SCH_CFI_STEP_UP(_cell, _cellSch, _currCfi)\
{                                       \
   _cellSch->dl.newCfi = ((_currCfi) < _cell->dynCfiCb.maxCfi) ?  \
                          (_currCfi + 1):_cell->dynCfiCb.maxCfi;      \
   _cell->dynCfiCb.cfiIncr++;            \
}   

#define RG_SCH_CFI_STEP_DOWN(_cell, _cellSch, _currCfi)\
{                                       \
   _cellSch->dl.newCfi = _currCfi-1;      \
   _cell->dynCfiCb.cfiDecr++;            \
}   

#define RG_SCH_UPDT_CW2_CQI(_cqiCw1,_cqiCw2,_diffCqi)\
   if (_cqiCw1 > rgSchCmnDlCqiDiffOfst[_diffCqi]) \
   {\
      _cqiCw2 = _cqiCw1 - rgSchCmnDlCqiDiffOfst[_diffCqi]; \
   } \
   else \
   { \
      _cqiCw2 = 1; \
   }

#ifdef DL_LA
/* TM Mode Step Up/Down Factor macros */
#define RG_SCH_MODE_CHNG_STEPUP_FACTOR              1
#define RG_SCH_MODE_CHNG_STEPDOWN_FACTOR            1
#define RG_SCH_MODE_CHNG_STEPUP_THRSHD              150
#define RG_SCH_MODE_CHNG_STEPDOWN_CHECK_FACTOR      10
#define RG_SCH_MODE_CHNG_STEPDOWN_THRSHD            100

#define RG_SCH_TXSCHEME_CHNG_THRSHD                 5
#define RG_SCH_TXSCHEME_CHNG_ITBS_FACTOR            5

#define RG_SCH_FILL_RGM_TRANSMODE_IND(_ueId, _cellId, _mode, _txModChgInd)\
{\
  _txModChgInd->usCrnti = _ueId;\
  _txModChgInd->bCellId = _cellId;\
  _txModChgInd->eMode = _mode - 1;\
}
#endif
#ifdef __cplusplus
}
#endif
#endif /* __RGSCHCMNH__ */

/********************************************************************30**

         End of file:     rg_sch_cmn.h@@/main/2 - Sat Jul 30 02:22:02 2011

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
/main/1      ---       sm          1. LTE MAC 2.1 release
           rg001.201   sm          1. ccpu00106531: fixing incorrect 
                                      Imcs derivation. Inappropriate name 
                                      of CQI to ITbs table for DL has been 
                                      corrected.
                                   2. ccpu00107387: To include the length 
                                      and ModOrder in DataRecp Req.   
           rg004.201   sd          1. ccpu00105914: PHR handling for MSG3
           rg005.201   sm          1. Changed the check for uplink harq proc
                                      reTx. 
           rg006.201   sd          1. ccpu00112396: Removed dependency on 
                                      TFU_DELTA
           rg007.201   ap          1. Added support for MIMO
           rg008.201   sd          1. Removed dependency on MIMO compile-time flag
                       rsharon     2. Added support for SPS.
           rg009.201   avivek      1. Added RgrSpsPrd casting to overcome G++ 
                                      compilation warning      
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg001.301   asehgal      1.Added the definitions for maximum achievable non 64QAM UEs.
           rg002.301   avivek       1.changes for ccpu00109543
           rg001.301   asehgal      1.Added the definitions for maximum 
                                      achievable non 64QAM UEs. 
                                    2. ccpu00119494: added a define for delta 
                                       of actual TX time in SPS.
                                    3. ccpu00120274: added new hash defines 
                                       for min & max values of RI.
                                    4. ccpu00120703: added new hash define
                                       for max mcs for msg3
                                    5. ccpu00120621-MOD-fixing the priority 
                                       table to be more in line with the spec.
                                    6. ccpu00120621-hash defines for max & min
                                       values of qci.
                                    7. ccpu00118351-seperate deltas for UL & DL.
$SID$        ---     rt        1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
