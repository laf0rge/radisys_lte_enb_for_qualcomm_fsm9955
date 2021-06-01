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
 
    Name:   RRC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE-MAC
 
    File:   rg_sch.h
 
    Sid:      rg_sch.h@@/main/2 - Sat Jul 30 02:22:04 2011
 
    Prg:    st
 
**********************************************************************/
 


/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/
 
#ifndef __RGSCH__
#define __RGSCH__

#define RGSCHLAYERNAME     "MAC Scheduler"

/* This variables must moved into cell context finally. Since this requires replacing
 *  all macros in the code with these cell specific variables, this will taken as part of 
 *  delta cleanup activity in MAC. Untill then, since we only support one cell, this should be fine
 */
#ifdef RG_SCH_DYNDLDELTA
#define RGSCH_UPDATE_DELTA(delta)\
{\
rgSchCb[0].rgSchDlDelta    = RG_ENV_DL_DELTA + delta;\
rgSchCb[0].rgSchCmnDlDelta = RG_ENV_SCH_CMN_DL_DELTA + delta;\
rgSchCb[0].rgSchUlDelta    = TFU_ENV_HQFBKIND_ULDELTA + delta;\
}
#else
#define RGSCH_UPDATE_DELTA(delta)
#endif

#ifdef RG_SCH_DYNDLDELTA
/* 4UE_TTI_DELTA */
#define RG_DL_DELTA rgSchCb[0].rgSchDlDelta
#define RG_SCH_CMN_DL_DELTA (rgSchCb[0].rgSchCmnDlDelta)
#define TFU_HQFBKIND_ULDELTA (rgSchCb[0].rgSchUlDelta)
#else
#ifdef  SPLIT_RLC_DL_TASK
#define RG_DL_DELTA (RG_ENV_DL_DELTA + 1 )
#define RG_SCH_CMN_DL_DELTA (RG_ENV_SCH_CMN_DL_DELTA + 1)
#define TFU_HQFBKIND_ULDELTA (TFU_ENV_HQFBKIND_ULDELTA + 1)
#else 
#define RG_DL_DELTA RG_ENV_DL_DELTA 
#define RG_SCH_CMN_DL_DELTA RG_ENV_SCH_CMN_DL_DELTA 
#define TFU_HQFBKIND_ULDELTA TFU_ENV_HQFBKIND_ULDELTA 
#endif
#endif

/* Tunable parameter */
#define RGSCH_MAX_NUM_DED_LC 10       /* maximum dedicated logical channels in a UE */
#define RGSCH_MAX_NUM_CMN_LC 5        /* maximum number of common logical 
                                         channels in a cell */
#define RG_SCH_LCG0 0
#define RG_SCH_QCI5 5
#define RGSCH_MAX_LCG_PER_UE 4
#define RGSCH_CON_RES_ID_SZ  6
#ifdef LTE_TDD
#define RGSCH_MAX_RA_RNTI   60
#else
#define RGSCH_MAX_RA_RNTI   10
#define RGSCH_RAREQ_ARRAY_SIZE (2*RGSCH_MAX_RA_RNTI)
#endif

#define RGSCH_MAX_RAPID 64 /* XXX: For RACH Module */
/* Below two temps added by nagaraja S */
#define RG_MEAS_GAPPRD_40 40
#define RG_MEAS_GAPPRD_80 80
/* Added support for SPS*/

/* LTEMAC_SPS changes */
#define RG_SCH_NUM_RATYPE1_SUBSETS    4
#define RG_SCH_NUM_RATYPE2_32BIT_MASK 4
#define RG_SCH_NUM_RATYPE1_32BIT_MASK RG_SCH_NUM_RATYPE1_SUBSETS

/* MAX number of feedbacks in case of LTE_TDD */
#ifdef LTEMAC_SPS
#ifdef LTE_TDD
#define RGSCH_TDD_MAX_FDBK 4
#endif /* LTE_TDD */
/* DL SPS States of UE */
#define RGSCH_DL_SPS_IDLE                    0x00
#define RGSCH_DL_SPS_ACTVTN_PENDING          0x01
#define RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM    0x02
#define RGSCH_DL_SPS_ACTV                    0x03
#define RGSCH_DL_SPS_RELEASE_TRIGGERED       0x04
#define RGSCH_DL_SPS_REACTVTN_PENDING        0x05  

/* UL SPS States of UE */
#define RGSCH_SPS_IDLE              0x00
#define RGSCH_SPS_TO_BE_ACTV        0x01
#define RGSCH_SPS_TO_BE_REACTV      0x02
#define RGSCH_SPS_TO_BE_REL         0x03
#define RGSCH_SPS_ACTV_PDCCH_SENT   0x04
#define RGSCH_SPS_REL_SENT          0x05
#define RGSCH_SPS_ACTV              0x06

#define RG_SCH_SPS_LCG_ID 1
#define RG_SCH_MAX_NUM_N1PUCCH_PER_UE 4
#define RG_SCH_MAX_N1PUCCH_VAL 2047
#define RGSCH_INVALID_SUBFRAME 0xFF
#define RGSCH_INVALID_SFN 0xFFFF
#endif

#define RGSCH_MAX_DL_HQ_PROC 8

/* This is the delta that MAC works on Schedule's DELTA should be a function of
 * this.
 */

/* Well known RNTIS */
#define RGSCH_SI_RNTI        0xffff
#define RGSCH_P_RNTI         0xfffe

#define RGSCH_MAX_UPPERSAP               3
#define RGSCH_MAX_LOWERSAP               1 
#define RGSCH_TQ_SIZE                    100   /* Timing Queue Size */ 
#define RGSCH_MAX_TIMER     RGSCH_MAX_LOWERSAP   /* MAX number of MAC timers */
#define RGSCH_NMB_CELL_HASHBIN           10   /* Number of Hash Bins for cell hash
                                              table */
#define RGSCH_NMB_UE_HASHBIN             10   /* Number of Hash Bins for UE hash
                                              table */        
#define RGSCH_BNDREQ_TMR    1    /* Bind Request timer event value */
#define RGSCH_MAX_BNDRETRY  2    /* Max number of Bind Retries for TFU SAP */


#define RGSCH_UE_TQ_SIZE    10   /* Timing Queue Size */

#define RGSCH_INVALID_SCHD_TYPE 255
#define RGSCH_MAX_UE_PER_CELL 0xFFFF /*!< Maximum number of UEs per cell */
#define RGSCH_MAX_UE_BIN_PER_CELL 128 /*!< Maximum number of UE bins per cell */
#define RGSCH_MIN_DL_BW       6    /*!< Minimum Downlink bandwidth in RBs */
#define RGSCH_MAX_DL_BW       110  /*!< Maximum Downlink bandwidth in RBs */
#define RGSCH_MIN_UL_BW       6    /*!< Minimum Uplink bandwidth in RBs */
#define RGSCH_MAX_UL_BW       110  /*!< Maximum Uplink bandwidth in RBs */
#define RGSCH_NUM_SC_IN_RB     12  /*!< Number of sub carriers in a RB */
#define RGSCH_NUM_PBCH_SYM      4  /*!< Number of PBCH symbols in subframe0.Section 6.6,36.211 */
#define RGSCH_NUM_PSS_SSS_SYM   2  /*!< Number of primary and secondary Synchronization symbols in                                        subframe 0 and subframe 5 */
#define RGSCH_NUM_CELL_RS_ONE_ANT_PORT  8   /*!< Number of cell specific reference symbols in a Subframe */
#define RGSCH_NUM_CELL_RS_TWO_ANT_PORT  16  /*!< Number of cell specific reference symbols in a Subframe */
#define RGSCH_NUM_CELL_RS_FOUR_ANT_PORT  24   /*!< Number of cell specific reference symbols in a Subframe */
#define RGSCH_TOT_NUM_SYM_NORCP 168 /*!< Total number of symbols in a RB in case of Normal CP */
#define RGSCH_TOT_NUM_SYM_EXTCP 144 /*!< Total number of symbols in a RB in case of Extended CP */

#define RGSCH_MAX_NUM_CSRS_ONE_SYMBOL_RB  4
#define RGSCH_NUM_ANT_PORT_FOUR 4
#define RGSCH_MIN_NUM_ANT_PORT 1
#define RGSCH_NUM_REGS_1ST_SYM 2
#define RGSCH_NUM_REGS_2ND_SYM_FOUR_ANT_PORT 2
#define RGSCH_NUM_REGS_2ND_SYM_1OR2_ANT_PORT 3
#define RGSCH_NUM_REGS_3RD_SYM 3
#define RGSCH_NUM_REGS_4TH_SYM_NOR_CP 3
#define RGSCH_NUM_REGS_4TH_SYM_EXT_CP 2

#define RGSCH_MIN_CFI_VAL     1    /*!< Minimum value for CFI */
#define RGSCH_MAX_CFI_VAL     3    /*!< Maximum value for CFI */
#define RGSCH_QM_BPSK         2    /*!< Qm value for BPSK */
#define RGSCH_QM_QPSK         4    /*!< Qm value for QPSK */
#define RGSCH_QM_64QAM        6    /*!< Qm value for 64QAM */
#define RGSCH_MIN_SRS_SFCFG_IDX 0  /*!< Minimum value for SRS subframe 
                                     configurtion index */
#define RGSCH_MAX_SRS_SFCFG_IDX 15  /*!< Maximum value for SRS subframe 
                                     configurtion index */
#define RGSCH_MAX_SRS_TX_OFFSET 8    /*!< Maximum number of SRS transmission 
                                    offsets per cell */
#ifdef LTE_TDD
#define RGSCH_MIN_MAC_RNTI      61   /*!< Minimum value of RNTI to be managed by 
                                    MAC */
#else
#define RGSCH_MIN_MAC_RNTI      11   /*!< Minimum value of RNTI to be managed by 
                                    MAC */
#endif

#define RG_SCH_DL_MAX_ITBS 26
#define RG_SCH_UL_MAX_ITBS 26

#define RG_TIME_DIFF(_currTime,_prevTime)                  \
   (_currTime < _prevTime ? ((0xffffffff - _prevTime) + _currTime ): (_currTime - _prevTime))

#define RGSCH_TYPE_SC1    RGR_SCH_TYPE_SC1 
#define RGSCH_TYPE_PFS    RGR_SCH_TYPE_PFS
#define RGSCH_TYPE_RR     RGR_SCH_TYPE_RR
#define RGSCH_TYPE_MAXCI  RGR_SCH_TYPE_MAXCI 
/* HARQ related MACROs */
#ifdef TFU_TDD
#define RGSCH_NUM_DL_HQ_PROC 15
#else
#define RGSCH_NUM_DL_HQ_PROC 8
#endif
#define RGSCH_NUM_UL_HQ_PROC 8
#define RGSCH_MIN_HQ_TX 1

/* Group power related MACROs */
#define RGSCH_MAX_GRP_PWR_FMT3_IDX  15   /*!< Maximum index value for group power format 3 */
#define RGSCH_MAX_GRP_PWR_FMT3A_IDX 31   /*!< Maximum index value for group power format 3A */

/* MACROs to indicate cell specific config for cell to be active */
#define RGSCH_BCCH_BCH_CFG      (1<<0)
#define RGSCH_BCCH_DLSCH_CFG1    (1<<1)
#define RGSCH_BCCH_DLSCH_CFG2    (1<<2)
#define RGSCH_PCCH_CFG          (1<<3)
#define RGSCH_UL_CCCH_CFG       (1<<4)
#define RGSCH_DL_CCCH_CFG       (1<<5)
#define RGSCH_SCHD_CFG          (1<<6)

#define RGSCH_CELL_ACTIVE_CFG (RGSCH_BCCH_BCH_CFG | RGSCH_BCCH_DLSCH_CFG1 | RGSCH_BCCH_DLSCH_CFG2 | RGSCH_PCCH_CFG | RGSCH_UL_CCCH_CFG | RGSCH_DL_CCCH_CFG)

/* Logical channel realated MACROs */
#define RGSCH_INVALID_LCG_ID  255
#define RGSCH_INVALID_LC_ID   255
#define RGSCH_BCCH_BCH_IDX   0
#define RGSCH_BCCH_DLSCH_IDX1 1
#define RGSCH_BCCH_DLSCH_IDX2 2
#define RGSCH_PCCH_IDX 3

/* PUCCH related macros */
#define RGSCH_PUCCH_MAXVAL_CS    7   /*!< Maximum value for cyclic shift of PUCCH */
#define RGSCH_PUCCH_MINVAL_DS    1   /*!< Mininmum value for delta shift of PUCCH */
#define RGSCH_PUCCH_MAXVAL_DS    3   /*!< Maximum value for delta shift of PUCCH */

/* DUX related macros */
#define RGSCH_LCID_MASK 0x1F
#define RGSCH_LCID_LEN 0x5
#define RGSCH_CCCH_LCID 0x00
#define RGSCH_DEDLC_MIN_LCID 0x01
#define RGSCH_DEDLC_MAX_LCID 0x0A
#define RGSCH_RES_MIN_LCID 0x0B
#define RGSCH_RES_MAX_LCID 0x19
#define RGSCH_PHR_LCID 0x1A
#define RGSCH_CRNTI_LCID 0X1B
#define RGSCH_TRUNC_BSR_LCID 0X1C
#define RGSCH_SHORT_BSR_LCID 0X1D
#define RGSCH_LONG_BSR_LCID  0X1E
#define RGSCH_PAD_LCID 0x1F
/* Fix: If only TA is scheduled, use some dummy LCID */
#define RG_TA_LCID 0x20
#define RGSCH_MAX_EXTN_PAD_SUBHDRS 0x02

#define RGSCH_CCCH_SDU_PRSNT     (1<<0)
#define RGSCH_CRNTI_CE_PRSNT     (1<<1)
#define RGSCH_PHR_CE_PRSNT       (1<<2)
#define RGSCH_TRUNC_BSR_CE_PRSNT (1<<3)
#define RGSCH_SHORT_BSR_CE_PRSNT (1<<4)
#define RGSCH_LONG_BSR_CE_PRSNT  (1<<5)
/* L2_COUNTERS */
#define RGSCH_ACTIVE_LC_PRSNT  (1<<6)
#ifdef LTEMAC_SPS
#define RGSCH_UL_SPS_ACT_PRSENT (1<<7)
#endif

/* LOGICAL CHANNEL */
#define RGSCH_MAX_LC_PER_UE   10
/* Maximum number of common logical channel control blocks */
#define RGSCH_MAX_CMN_LC_CB   4

/* Random access related MACROs */
#define RGSCH_MAX_RA_PREAMBLE_FMT 3 /*!< Maximun value of Random access preamble 
                                      format */
#define RGSCH_MAX_RA_WINSIZE    10  /*!< Maximum size of Random access response 
                                      window in subframes */
#define RGSCH_MIN_RA_WINSIZE    2   /*!< Minimum size of Random access response 
                                      window in subframes */
#define RGSCH_MIN_NUM_RA_PREAMBLE 4 /*!< Minimum number of Random access 
                                      preambles */
#define RGSCH_MAX_NUM_RA_PREAMBLE 64 /*!< Maximim number of Random access 
                                      preambles */
#define RGSCH_NUM_RA_RB     6
                                      
#define RGSCH_MAX_UL_RB     110     /*!< MAX Uplink RBs */

#define RGSCH_MAX_RA_RSP_ALLOC    4 /*!< Maximum number of Random access
                                      allocations */

#define RGSCH_MAX_RA_RNTI_PER_SUBFRM   6
#define RGSCH_MAX_TDD_RA_RSP_ALLOC     6 
#define RGSCH_MAX_TDD_RA_PREAMBLE_FMT  4
#define RGSCH_MAX_TDD_UL_DL_CFG        7
#define RGSCH_MAX_TDD_SPL_SUBFRM_CFG   9
#define RGSCH_INVALID_INFO             0xff

#define RGSCH_RGR_CFG 1          /* RGR configuration element */

/* RACHO */
#define RGSCH_MIN_MSG3_GRNT_SZ 7  /*!< Minimum msg3 grant size in bytes */

#define RGSCH_NUM_ITBS         27
#define RGSCH_MAX_NUM_RB       110
/* Max number of layers per CW */
#define RGSCH_MAX_NUM_LYR_PERCW   2

/* New hash define for TA. Refer to 36.213, 4.2.3*/
#define RGSCH_NO_TA_RQD  31  

/* ccpu00133680: Expcted alloc per LC at the end of scheduling
 * 2 bytes for minimum RLC Header+ 1 byte for minimal MAC header
 * + 1 byte for minimal RLC SDU*/
#define RGSCH_MIN_ALLOC_PER_LC 4
#define RGSCH_MAX_REFRESH_GRPSZ   4 /*! Max number of UEs refreshed per subframe */
#define RGSCH_MAX_REFRESH_OFFSET 16 /*! Max number of subframes used for UE refresh */

#define RGSCH_NUM_PDB_SEV_LEVELS 4 /*Number of Severity Levels the PDB can be classified as */
#define RG_SCH_MAX_PDB_TIME      280 /*Maximum PDB time */
#define RGSCH_LCG_ISCFGD(lcg) ((lcg)->lcgId != RGSCH_INVALID_LCG_ID)
/* Corrected the check for dlCcchId */
#define RGSCH_DLCCCH_ISCFGD(cell) ((cell)->dlCcchId != RGSCH_INVALID_LC_ID)
#define RGSCH_ULCCCH_ISCFGD(cell) ((cell)->ulCcchId != RGSCH_INVALID_LC_ID)
/* Note: In RGSCH_CALC_SF_DIFF, _time1 should be the latest */
#define RGSCH_CALC_SF_DIFF(_time1, _time2)\
   (_time1.sfn*10+_time1.subframe) < (_time2.sfn*10+_time2.subframe)?\
     ((_time1.sfn+RGSCH_MAX_SFN)*10+_time1.subframe) -\
       (_time2.sfn*10+_time2.subframe) : \
     (_time1.sfn*10+_time1.subframe) - (_time2.sfn*10+_time2.subframe)\

/*Addef for L2Meas*/
/*LTE_L2_MEAS_PHASE2*/
#define RGSCH_CALC_SFN_SF_DIFF(_time1,_sfnCycle, _time2)\
(((_time1.sfn+RGSCH_MAX_SFN * _sfnCycle)*10) + _time1.subframe -\
(_time2.sfn*10 + _time2.subframe))

#define RG_SCH_ADD_TO_CRNT_TIME(crntTime, toFill, incr)          \
   if ((crntTime.subframe + (incr)) >= RGSCH_NUM_SUB_FRAMES)   \
      toFill.sfn = (crntTime.sfn + \
            (crntTime.subframe + (incr))/RGSCH_NUM_SUB_FRAMES) \
                                                % RGSCH_MAX_SFN;\
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.subframe = (crntTime.subframe + (incr)) % RGSCH_NUM_SUB_FRAMES;

#define RGSCHDECRFRMCRNTTIME(_crntDl, _prevDl, decr)    \
do \
{                                                  \
   S32  _subframe;\
   _subframe = _crntDl.sfn * RGSCH_NUM_SUB_FRAMES + _crntDl.subframe; \
   _subframe = _subframe - decr; \
   if(_subframe < 0) \
   { \
      _subframe = (RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES) + _subframe; \
   } \
   _prevDl.sfn = _subframe / RGSCH_NUM_SUB_FRAMES; \
   _prevDl.subframe = _subframe % RGSCH_NUM_SUB_FRAMES; \
} while(0)

/* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
 * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
 * serving the purpose */

#define RGSCHCPYTIMEINFO(src, dst)  \
   dst.sfn        = src.sfn;     \
   dst.subframe   = src.subframe;


#define RGSCH_TIMEINFO_SAME(x, y) (((x).sfn == (y).sfn) && ((x).subframe == (y).subframe))
/* Added support for SPS*/

#ifdef LTEMAC_SPS
#define RGSCH_TIMEINFO_CMP(_x, _y, _ret)\
{\
   if (RGSCH_TIMEINFO_SAME(_x, _y))\
   {*_ret = 0;}\
   else if ((_x).sfn > (_y).sfn)\
   {*_ret = 1;}\
   else if ((_x).sfn == (_y).sfn)\
   {\
      if ((_x).subframe > (_y).subframe)\
      { *_ret = 1; }\
      else\
      {*_ret = -1; }\
   }\
   else\
   { *_ret = -1; }\
}
#endif
/* RACHO : TRUE if rapId is a ded preamble */
#define RGSCH_IS_DEDPRM(cell, rapId) ((rapId) >= (cell->rachCfg.numRaPreamble))

#define rgSchPBuf(inst)  rgSchCb[inst].rgSchInit.prntBuf

#if 0
#define RGSCHDBGPRM(inst, _args)          \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_PRM, _args)
#define RGSCHDBGERR(inst, _args)          \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_ERR, _args)
#define RGSCHDBGINFO(inst, _args)         \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_INFO, _args)
#endif
/* Debug Prints for MAC */
#ifdef DEBUGP
#define RGSCHDBGERRNEW(inst, _args)          \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_ERR, _args)
#define RGSCHDBGINFONEW(inst, _args)         \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_INFO, _args)
#define RGSCHDBGPRM(inst, _args)   UNUSED(inst);
#define RGSCHDBGERR(inst, _args)  \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_ERR, _args)
#define RGSCHDBGINFO(inst, _args)  UNUSED(inst); 
#else
#define RGSCHDBGERRNEW(inst, _args) 
#define RGSCHDBGINFONEW(inst, _args)
#define RGSCHDBGPRM(inst, _args)
#define RGSCHDBGERR(inst, _args)
#define RGSCHDBGINFO(inst, _args)
#endif  /* #ifdef DEBUGP */

#define RGSCH_IS_GBR_BEARER(cfgdGbr) ((cfgdGbr) != 0)

#ifdef ERRCLS_KW
#define RGSCH_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)     \
   if((_idxVal) >= (sizeof(_array)/sizeof(_array[0]))) \
   {                                               \
      RGSCHDBGERRNEW((_inst), (rgSchPBuf(_inst), "Array Bound Check Failed"));\
      SExit();\
   }\

#define RGSCH_NULL_CHECK(_inst, _ptr )     \
   if((_ptr) == NULLP)  \
   {                                               \
      RGSCHDBGERRNEW((_inst), (rgSchPBuf(_inst), "Null Pointer detected"));\
      SExit();\
   }
#define RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(_inst, _array, _idxVal)     \
   if(((_idxVal) >= (sizeof(_array)/sizeof(_array[0])))|| (_idxVal < 0)) \
   {                                               \
      RGSCHDBGERRNEW((_inst), (rgSchPBuf(_inst), "Array Bound Check Failed"));\
      SExit();\
   }
   
#define RGSCH_PFS_AMBR_ARRAY_BOUND_CHECK(_inst, _pfsCell, _qId, _ue ) \
{\
   if (_ue->csgMmbrSta == TRUE)\
   {\
      RGSCH_ARRAY_BOUND_CHECK(_inst, _pfsCell->txQueues.prioAmbrLst, _qId);\
   }\
   else\
   {\
      RGSCH_ARRAY_BOUND_CHECK(_inst, _pfsCell->txQueues.normAmbrLst, _qId);\
   }\
}
#else
#define RGSCH_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)  
#define RGSCH_NULL_CHECK( _inst, _ptr )     
#define RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(_inst, _array, _idxVal)
#define RGSCH_PFS_AMBR_ARRAY_BOUND_CHECK(_inst, _pfsCell, _qId, _ue )
#endif

/* Macro to free the message buffer and initialize it to zero */
/***********************************************************
 *
 *     Name : RGSCH_FREE_MSG
 *
 *     Desc : Macro to free the message buffer and initialize it to zero
 *            
 *     Input  : mBuf - message buffer pointer to be retunrned
 *
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/
#define RGSCH_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      SPutMsg((_buf)); \
      _buf = NULLP; \
   } \
}

#define RGSCH_FREE_MEM(_mem)\
{\
   if (NULLP != (_mem)) \
   { \
      cmFreeMem((Ptr)(_mem)); \
      _mem = NULLP; \
   } \
}
/***********************************************************
 *
 *     Name : RGSCH_DROP_RGUDDATREQ_MBUF
 *
 *     Desc : Macro to free the message buffers and initialize them to zero
 *            
 *     Input  : _datreq - Dedicated Data Request pointer which has mBufs
 *              to be freed
 *     
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/

#define RGSCH_DROP_RGUDDATREQ_MBUF(_datReq)\
{\
   U32 idx1,idx2,idx;\
   if (_datReq != NULLP)\
   {\
      for (idx=0; idx < _datReq->numLch; idx++)\
      {\
         for (idx1=0; idx1 < RGU_MAX_PDUSET; idx1++)\
         {\
            for (idx2=0; idx2 < _datReq->lchData[idx].pdu[idx1].numPdu; idx2++)\
            {\
               RGSCH_FREE_MSG(_datReq->lchData[idx].pdu[idx1].mBuf[idx2]);\
            }\
         }\
      }\
   }\
}
/***********************************************************
 *
 *     Name : RGSCH_DROP_RGUCDATREQ_MBUF
 *
 *     Desc : Macro to free the message buffers and initialize them to zero
 *            
 *     Input  : _datreq - Common Data Request pointer which has mBufs
 *              to be freed
 *     
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/
#define RGSCH_DROP_RGUCDATREQ_MBUF(_datReq)\
{\
   U32 idx1;\
   if (_datReq != NULLP)\
   {\
      for (idx1 = 0; idx1 < RGU_MAX_PDUSET; idx1++)\
      {\
         if (_datReq->pdu != NULLP)\
         {\
            RGSCH_FREE_MSG(_datReq->pdu[idx1]);\
         }\
      }\
   }\
}


/* Macros for memory region and pool determination */
#define RGSCH_GET_MEM_REGION(rgCb)  (rgCb.rgInit.region)
#define RGSCH_GET_MEM_POOL(rgCb)    (rgCb.rgInit.pool)

#ifdef LTE_TDD
#define RGSCH_UPD_HQAN_FDBKTIME(_tbInfo, _dlSf, _timingInfo) \
do \
{  \
   (_tbInfo)->m = 0;   \
   (_tbInfo)->fdbkTime.sfn = (_timingInfo.sfn + \
         _dlSf->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN; \
   (_tbInfo)->fdbkTime.subframe = _dlSf->dlFdbkInfo.subframe; \
   (_tbInfo)->timingInfo = _timingInfo; \
} while(0) 

#define RGSCH_UPD_ANINFO_WITH_HQ(_anInfo, _tbInfo) \
do \
{  \
   _anInfo->sfn = (_tbInfo)->fdbkTime.sfn; \
   _anInfo->subframe = (_tbInfo)->fdbkTime.subframe; \
   _anInfo->latestMIdx = (_tbInfo)->m; \
} while(0) 
/* Support for iPhich=1 for TDD*/

#define RGSCH_UPD_PHICH(_ulDlCfgIdx, _subframe, _hqProc) \
   if(_ulDlCfgIdx == 0 && ((_subframe == 4) ||( _subframe == 9)))\
   {\
      _hqProc->iPhich = 1;\
   }\
   else\
   {\
      _hqProc->iPhich = 0;\
   }
#endif


/* MUX related macros */
#define RGSCH_HDR_TYPE_CRES                1
#define RGSCH_HDR_TYPE_TA                  2

#define RGSCH_SDU_SHDR_LEN 1
#define RGSCH_CE_SHDR_LEN 1
#define RGSCH_CRES_LEN    6
#define RGSCH_TA_LEN      1
#define RGSCH_CRES_ELM_LEN RGSCH_CE_SHDR_LEN+RGSCH_CRES_LEN
#define RGSCH_TA_ELM_LEN RGSCH_CE_SHDR_LEN+RGSCH_TA_LEN

#define RGSCH_CRES_LCID_IDX               0x1C
#define RGSCH_TA_LCID_IDX                 0x1D

#define RGSCH_MAX_SUBFRAMES_IN_SFN        9
#define RGSCH_MAX_SFN                     1024
#define RGSCH_NUM_SUB_FRAMES              10
/* Definig the Macro for the Size SF alloc Info Structure
 * The Value is defines the number of subframes the Alloc Info is maintained
 * The SF alloc Info is used to send the scheduled Allocation Info 
 * to MAC from Scheduler
 */
#define RGSCH_SF_ALLOC_SIZE                4

/* Defining new MACRO for DL subframes */
#define RGSCH_NUM_DL_SUBFRAMES            20
/* Define for the block size for memory allocation */
#define RGSCH_BLKSZ                       2048

/* Defines for RGU Statistics types */
#define RGSCH_RGU_SDU_DROP 1
#define RGSCH_RGU_SDU_RCVD 2

/* MACROS for General Statistics */
#define RGSCH_CFG_ADD      1
#define RGSCH_CFG_DEL      2

#define RGSCH_HQ_FDB_IND_CB_TYPE_HQ_ENT      1
#define RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB       2

/* The value of RGSCH_TDD_MAX_P_VAL is 4, since we need the table for 
 * P+1 value the below Macro is defined and used 
 */
#define RGSCH_TDD_MAX_P_PLUS_ONE_VAL      5
#define RGSCH_TDD_MAX_FREQ_RSRC  6

/* 
 * RG_TFU_HQ_IND_DELTA is replaced with
 * TFU_HQFBKIND_ULDELTA and moved into envopt.h.
 */

/* Macro for selecting the subframe index to be deleted */

#define RGSCH_RLS_SF_IDX   (RG_SCH_CMN_HARQ_INTERVAL + TFU_HQFBKIND_ULDELTA)

/* Macro for max random access responses */
#define RGSCH_MAX_RA_RSP_ALLOC   4

/* Value used to set nDmrs in uplink grant if nDmrs is not applicable */
#define RGSCH_INVALID_NDMRS  10

/* comcodsepa : start */
#define RGSCH_MIN(x, y) ((x) <= (y) ? (x) : (y))
#define RGSCH_MAX(x, y) ((x) >= (y) ? (x) : (y))
#define RGSCH_CEIL(x, y) (((x) + (y)-1) / (y))
#define RGSCH_CEILING(x) (((x) - (U8)(x)) ? (U8)(x+1) : (U8)x)  
#define RGSCH_DIV_ROUND(x, y) (((x) + (y)/2) / (y))

#define RGSCH_RARSP_WAIT_PERIOD 3 
#define RGSCH_INCR_FRAME(x) x = ((x) + 1) & 0x03ff
#define RGSCH_INCR_SUB_FRAME(x, y) do {\
   if ((x.subframe += y) > 9) {\
      RGSCH_INCR_FRAME(x.sfn); x.subframe -= 10;\
   }\
} while(0)

#define RG_SCH_TDD_DL_SUBFRAME     1
#define RG_SCH_TDD_UL_SUBFRAME     2
#define RG_SCH_TDD_SPL_SUBFRAME    3

#define RG_SCH_MAX_DAI_IDX         4
#define RG_SCH_DAI_MAX_BIT_VAL     3
/* Since DAI bit value is stored and its range is 0-3 in SCH, 
 * So Invalid is Set to 0xFF */
/* ccpu00139414 */
#define RG_SCH_INVALID_DAI_VAL     0x00
/* Valid Range is 0-8, So Invalid is Set to 99 */
#define RG_SCH_INVALID_M_VAL       0xFF

/* Below macro used to get the DAI value (1 -4) for the total DL transmission
 * made
 */
#define RG_SCH_GET_DAI_VALUE(dai) ((((dai) - 1) & RG_SCH_DAI_MAX_BIT_VAL) + 1)
/* below Macro indicates the max number of DL transmissions that can be made in
 * a single FeedBak period
 */
#define RG_SCH_MAX_NUM_EXPECTED_ACKS      9
/* The below macro indicates the max number of feedback report 
 */
#define RG_SCH_NUM_FDBK_VALUE             3

#define RG_SCH_ULIDX_LSB           1
#define RG_SCH_ULIDX_MSB           2
#define RG_SCH_ULIDX_BOTH          3

#define RGSCH_ONE_RAR_SIZE          7
#define RGSCH_MSG4_HDRSIZE          1
#define RGSCH_CONT_RESID_SIZE       7
/* CR timer change*/
#ifdef RGR_V1
#define RGSCH_CCCH_SDU_HDRSIZE      1
#define RGSCH_CONTRES_EXP      0xffff
#endif
/* Corrected allocation for common channels */
#define RGSCH_ONE_BIHDR_SIZE        1                                             

#define RGSCH_TA_SIZE               2

#define RGSCH_GET_RAR_BYTES(x)   ((x) * RGSCH_ONE_RAR_SIZE)

#define RG_SCH_CMN_MEAS_GAPPRD40 40
#define RG_SCH_CMN_MEAS_GAPPRD80 80
#define RG_MEAS_GAPPRD_40           40
#define RG_MEAS_GAPPRD_80           80
#define RG_MAX_NUM_DLSF             10 /* Maximum number of Subframes */
#define RG_MEASGAP_INACTIVE         0x01
#define RG_ACKNAKREP_INACTIVE       0x02
#define RG_PDCCHODR_INACTIVE        0x04
#define RG_DRX_INACTIVE             0x08    /*UE is DRX inactive */

#define RG_SCH_DRX_UL               0      /*UE is DRX active in uplink*/
#define RG_SCH_DRX_DL               1     /*UE is DRX active in downlink*/
/*Fix:Inform UE delete to scheduler*/
#define RG_MACUEDEL_INACTIVE        0x10  
/* Fix : syed set UE inactive for scheduling if it is not completely 
 * initialized */
#define RG_HQENT_INACTIVE           0x20
/* Timer events */
#define RG_SCH_TMR_ACKNACK_REP      1  /* Timer event for ACK NACK Rep */
#define RG_SCH_TMR_MEASGAP          2  /* Timer event for Measurement gap */
#define RG_SCH_TMR_UL_ACKNACK       3  /* Timer event for Ul Inactivity due to ack-nack */
#define RG_SCH_TMR_DL_ACKNACK       4  /* Timer event for Dl Inactivity due to ack-nack */
#define RG_SCH_TMR_UL_MEASGAP       5  /* Timer event for Ul Inactivity due to Measurement gap */
#define RG_SCH_TMR_DL_MEASGAP       6  /* Timer event for Dl Inactivity due to Measurement gap */
#define RG_SCH_TMR_TA               7  /* Timer event for Timing Alignment */
/* Added periodic BSR timer */
#ifndef RGR_V1
#define RG_SCH_TMR_BSR              8  /* Timer event for Periodic BSR */
#else
#define RG_SCH_TMR_BSR              8  /* Timer event for Periodic BSR */
#endif

#define RG_SCH_TMR_TXMODE_TRNSTN    9  /* Timer event for TX Mode Transition */
#define RG_SCH_TXMODE_TRANS_TIMER   1000  /* Timer length for Tx Mode Transition
                                             Completion */
                                                         
#define RG_SCH_TMR_DLINACTV         2  /* Timer event for DL Inactivity */
#define RG_SCH_MEAS_GAP_LEN         6  /* Timer event for measurement GAP */

                                                         

#define RG_SCH_DRX_DL_DELTA     (RG_DL_DELTA) /*!< look at timer queue 
                                                           for DL
                                                           RG_SCH_DRX_DL_DELTA 
                                                           in advance */

/* Introduced UL control timing delta in FDD */
#define RG_SCH_DRX_UL_DELTA     (TFU_ULCNTRL_DLDELTA)
/*Assigning RG_SCH_DRX_MAX_DELTA to the higher delta of uplink delta and
 * downlink delta.*/
#define RG_SCH_DRX_MAX_DELTA ((RG_SCH_DRX_DL_DELTA < RG_SCH_DRX_UL_DELTA)?\
RG_SCH_DRX_UL_DELTA:RG_SCH_DRX_DL_DELTA)

#define RG_SCH_NO_DELTA 0  /*Consider no delta */
/** @brief The lenght of array we maintain to track DRX Cycles.
 * @details
 * The periodicity of LONG DRX Cycle has a maximum value of 2560. The ideal and
 * the fastest way would be to have an array of this size. However having an
 * Array this large would make the cellCb a huge structure and may have
 * performance implications as well. 
 * Hence the defined size and lower values are a multiple of the larger sizes. 
 * A distance based approach is used to get to the correct value. 
 * @sa RgSchDRXCellCb.
 * For decreased processing hit this value maybe increased to 
 * [320, 640, 1024, 2560]
 */
#define RG_SCH_MAX_DRXQ_SIZE 256        /*!< Length of the DRQ queue maintained 
                                       at the cell level. */
#define RG_SCH_MIN_HARQ_RTT       8      /*!< Minimum round trip time for Harq
                                              feedback*/

/* MASKs for tracking DRX activity */

#define DRX_UE_INACTIVE          0xFFFFFFFF
#define DRX_INVALID              0xFFFF
#define DRX_TMR_EXPRD            -1 

#define RG_SCH_DRX_SR_BITMASK        0x00000001  /*DRX SR Bitmask*/
#define RG_SCH_DRX_RA_BITMASK        0x00000002  /*DRX RA Bitmask*/
#define RG_SCH_DRX_ONDUR_BITMASK     0x00000004  /*DRX on-duration bitmask */
#define RG_SCH_DRX_INACTVTMR_BITMASK 0x00000008  /*DRX inactive timer bitmask */
/* DLHQ Bitmask should always be the last */
#define RG_SCH_DRX_DLHQ_BITMASK      0x00000010  /*DRX DL harq bitmask*/
                                                    

/*if any bit in the mask is 0, ue is active */
#define RG_SCH_DRX_DL_IS_UE_ACTIVE(drxCb)\
   (drxCb->drxDlInactvMask ^ DRX_UE_INACTIVE) 

/*if any bit in the mask is 0, ue is active */
#define RG_SCH_DRX_UL_IS_UE_ACTIVE(drxCb)\
   (drxCb->drxUlInactvMask ^ DRX_UE_INACTIVE)     
/*if UE is in DL DRX on-duration */                                                     
#define RG_SCH_DRX_DL_IS_UE_ONDUR_INACTIVE(drxCb) \
   (drxCb->drxDlInactvMask & RG_SCH_DRX_ONDUR_BITMASK)
/*if UE is in UL DRX onduration */
#define RG_SCH_DRX_UL_IS_UE_ONDUR_INACTIVE(drxCb) \
   (drxCb->drxUlInactvMask & RG_SCH_DRX_ONDUR_BITMASK)
/*if ue is in DL DRX Inactive timer period */
#define RG_SCH_DRX_DL_IS_UE_INACTVTMR_INACTIVE(drxCb) \
   (drxCb->drxDlInactvMask & RG_SCH_DRX_INACTVTMR_BITMASK)

/*if ue is in UL DRX Inactive timer period */
#define RG_SCH_DRX_UL_IS_UE_INACTVTMR_INACTIVE(drxCb) \
   (drxCb->drxUlInactvMask & RG_SCH_DRX_INACTVTMR_BITMASK)
/*get DRX cell */
#define RG_SCH_DRX_GET_CELL(_cell) ((_cell)->drxCb) 
/*get DRX UE */
#define RG_SCH_DRX_GET_UE(_ue) ((_ue)->drxCb) 
/*get DRX DL HARQ */
#define RG_SCH_DRX_GET_DL_HQ(_hqProc) &((_hqProc)->drxCb)



#ifdef LTE_TDD

#define RGSCH_MAX_SFCFG    2   /* refer to rgSchDrxDlSfTddcfg in rg_sch.h to 
                                  understand why this is 2 */
#define RGSCH_MAX_TDD_CFG  7   
                                                     
#define RGSCH_NUM_SFRAMES 10   /* number of subframes in a RF */
  
#endif /* LTE_TDD */                                                    

#ifndef LTE_TDD                                                         
/* Introduced UL control timing delta in FDD */
/* Number of subframes in advance UL control (DCI/PHICH) should be sent from SCH */
#define TFU_ULCNTRL_DLDELTA  (RG_SCH_CMN_HARQ_INTERVAL - TFU_CRCIND_ULDELTA)
#endif/*ndef LTE_TDD*/                                                         

/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/* SI Re-configuration related bit masks */
#define RGSCH_SI_DFLT            0     /* Default */
#define RGSCH_SI_SICFG_UPD   (1<<0) /* SI CFG Updated */
#define RGSCH_SI_MIB_UPD   (1<<1) /* MIB CFG Updated */
#define RGSCH_SI_SIB1_UPD   (1<<2) /* SIB1 CFG Updated */
#define RGSCH_SI_SI_UPD           (1<<3) /* SI PDU CFG Updated */
#define RGSCH_SI_SIB1_PWS_UPD   (1<<4) /* SIB1 PWS CFG Updated */
/*SI Scheduling Specific */
#define RGSCH_MIB_PERIODICITY        4 /*!< MIB Periodicity */ 
#define RGSCH_SIB1_PERIODICITY       8 /*!< SIB1 Periodicity */
#define RGSCH_SIB1_RPT_PERIODICITY   2 /*!< SIB1 Repeat Periodicity */
#define RGSCH_MIB_TX_SF_NUM          0 /*!< MIB TX SF Number */
#define RGSCH_SIB1_TX_SF_NUM         5 /*!< SIB1 TX SF Number */
/*rg009.lpr-ccpu00116647 -  Added siId validation for lower limit */
#define RGSCH_SI_SIID_LOWER_LMT      1 /*!< Least usabel siId number */
#define RGR_MAX_NUM_WARNING_SI       3 /*!< Max no. of Warning SI*/
#define RGSCHCHKNUPDSIPDU(_CRNTINFO,_NEWINFO,_NEWPDU,_BITMSK, _BITFLG) \
do\
{\
      if(NULLP == _CRNTINFO)\
      {\
         _CRNTINFO = _NEWPDU; \
      }\
      else\
      {\
         if(NULLP != _NEWINFO)\
         {\
            RGSCH_FREE_MSG(_NEWINFO);\
            _NEWINFO = NULLP;\
         }\
         _NEWINFO = _NEWPDU;\
         _BITMSK |= _BITFLG;\
      }\
}while(0)

#define RGSCH_SET_SI_INFO(_CRNTPTR,_NEWPTR)\
do\
{\
     if(NULLP != _CRNTPTR)\
        RGSCH_FREE_MSG(_CRNTPTR);\
     _CRNTPTR = _NEWPTR;\
     _NEWPTR = NULLP;\
}while(0)
#endif /*RGR_SI_SCH */
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/
 /* Array Based List size to store next active Periodic CQI, SRS and SR 
    tranmission instances at cell Cb  */

 /* Periodic CQI  max periodicity =   160 Ref: 36.213 (Table 7.2.2-1A For FDD 
                                        and 1C for TDD) */
 /* SRS      max periodicity =   320 Ref: 36.213 (Table 8.2-1) */
 /* SR       max periodicity =   80  Ref: 36.213 (Table 10.1-5 ) */
 /* Modifying Queue Size from 320 to 321. 320 is a possible periodicity for RI
  * and SRS. If it is 320, then while moving to next occassion it is being added
  * same queue. If more than one node is available in the queue then same UE's 
  * node is adding to the same queue and processed in same TTI. This is happening
  * in infinite loop. Hence modifying it to 321, which is not a possible periodicity.
  */
/* Changing this back to 320 as it was causing 
 * problem in TDD attach and stability
 */
#define  RG_SCH_PCQI_SRS_SR_TRINS_SIZE 320 
                                           

#ifdef LTE_TDD

/* Ref 36.213 Table 8.2-2:  */
#define  RG_SCH_ISRS_MAX_SUPP       644  
/* Note: <= 10 ms periodicity is not supported */ 
#define  RG_SCH_ISRS_MIN_SUPP        10 

/* Ref 36.213 Table 7.2.2-1C */

#define RG_SCH_ICQI_MAX_SUPP     315 
#define RG_SCH_ICQI_MIN_SUPP    0 

#else
/* Ref 36.213 Table 8.2-1:  */
#define  RG_SCH_ISRS_MAX_SUPP       636  
/* Note: <= 10 ms periodicity is not supported */ 
#define  RG_SCH_ISRS_MIN_SUPP        0 


#define RG_SCH_ICQI_MAX_SUPP    541 
#define RG_SCH_ICQI_MIN_SUPP    0 

#define RG_SCH_ICQI_RESV_FDD  317

#endif

/*Refer Table 7.2.1-5: for max BW configuration M=6*/
#define RG_SCH_MAX_NUM_UE_SEL_SUBBANDS 6

/*Refer Table 7.2.1-5: for max BW configuration k =4, ceil(110/4) = 28*/
#define RG_SCH_MAX_TOT_NUM_SUBBANDS 28

/* 36.213 Table 7.2.2-1B:  */
#define RG_SCH_IRI_MAX_SUPP   965
#define RG_SCH_IRI_MIN_SUPP   0

/*K value in CQI table min and max values*/
#define RG_SCH_CQI_K_MAX    4
#define RG_SCH_CQI_K_MIN    1

#define RG_SCH_PUCCH_RES_MAX_SUPP  1185 /* TODO: Need to check value */

#define RG_SCH_IRI_MAX_SUPP   965 


#define RG_SCH_ISR_MAX_SUPP 154
#define RG_SCH_ISR_MIN_SUPP 0

#define RG_SCH_SRS_FREQDOM_POS_MIN    0
#define RG_SCH_SRS_FREQDOM_POS_MAX    23

#define RG_SCH_SRS_TXCOMB_MIN    0
#define RG_SCH_SRS_TXCOMB_MAX    1

#define RG_SCH_SR_RES_IDX    2047   /* TODO: Need to check */
/*Reference: 36.213 Table:7.2.2-1A */
#define RG_SCH_CQIPMI_CFGIDX_MAX_FDD  10

/* Reference: 36.213 Table:7.2.2-1C */
#define RG_SCH_CQIPMI_CFGIDX_MAX_TDD  7


/* Note: RI table is same for FDD and TDD */
/*Reference: 36.213 Table:7.2.2-1B */
#define RG_SCH_RI_CFGIDX_MAX  6


/*Reference: 36.213 Table:7.2.2-2 */
#define RG_SCH_BW_SUBSZ_BWPARTS_MAX   5

/* Reference : 36.213 Table 8.2-1 */
#define RG_SCH_SRS_ISRS_INDX_MAX_FDD  8

/* Reference : 36.213 Table 8.2-2 */
#define RG_SCH_SRS_ISRS_INDX_MAX_TDD  7

/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
#define RG_SCH_ISR_INDX_MAX  5


/* This use used to mark as invalid index value */
#define RG_SCH_INVALID_IDX 0xffff

/*ccpu00116923 - ADD - SRS present support*/
#define RGSCH_CELLSP_SRS_SF_CONFIGS 16

/*Used for Periodic CQI. */
#define RG_SCH_GET_CQI_J_VAL(_dlBw, _j) \
{\
 if(_dlBw >= 64)\
   {\
   _j = 4;\
   }\
 else if(_dlBw >= 27)\
   {\
   _j = 3;\
   }\
 else if(_dlBw >= 11 )\
   {\
   _j = 2;\
   }\
 else\
   {\
   _j = 1;\
   }\
}

#define RG_SCH_GET_CQI_K_VAL(_dlBw,  _k) \
{\
 if(_dlBw >= 64)\
   {\
   _k = 8;\
   }\
 else if(_dlBw >= 27)\
   {\
   _k = 6;\
   }\
 else if(_dlBw >= 11 )\
   {\
   _k = 4;\
   }\
 else\
   {\
   _k = 4;\
   }\
}


#define RG_SCH_GET_SBCQI_M_K_VAL(_dlBw, _m, _k) \
{\
 if(_dlBw >= 64)\
   {\
   _m = 6;\
   _k = 4;\
   }\
 else if(_dlBw >= 27)\
   {\
   _m = 5;\
   _k = 3;\
   }\
 else if(_dlBw >= 11 )\
   {\
   _m = 3;\
   _k = 2;\
   }\
 else\
   {\
   _m= 1;\
   _k = 2;\
   }\
}


/* To Get the Idx to pCqiSrsSrLst in RgSchCellCb*/
#define RG_SCH_GET_IDX_PCQISRSSR(_time, _indexId)\
{\
   (_indexId) = (_time.sfn)*10+ (_time.subframe); \
   (_indexId) = (_indexId)%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;\
} 

/* To Get UE Next Active Periodic RI Idx to the table pCqiSrsSrLst */
/* Caller should check the periodic RI is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_RI(_ueCb, _riIdx)\
{\
   (_riIdx) = (_ueCb)->cqiCb.nRiTrIdx;\
}

/* To Get UE Next Active SR Idx to the table pCqiSrsSrLst */
/* Caller should check the periodic SR is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_SR(_ueCb, _srIdx)\
{\
   (_srIdx) = (_ueCb)->srCb..nSrTrIdx;\
}

/* To Get UE Next Active SRS Idx to the table pCqiSrsSrLst */
/* Caller should check the SRS is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_SRS(_ueCb, _ssrIdx)\
{\
   (_srsIdx) = (_ueCb)->srsCb..nSrsTrIdx;\
}



/* To Get UE Next Active Periodic CQI Idx to the table pCqiSrsSrLst */
/* Caller should check the periodic CQI is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_PCQI(_ueCb, _pCqiIdx)\
{\
   (_pCqiIdx) = (_ueCb)->cqiCb.nCqiTrIdx;\
}

/* To Move Next Occurance for periodic CQI Idx */
/* Caller should not call this Macro if PCQI is disabled */
#define RG_SCH_MOVE_PCQI_UE(_cellCb, _ueCb)\
{\
  U16   _nCqiIdx;\
  /* Compute Next Transmission Instance */ \
  _nCqiIdx = (_ueCb)->cqiCb.cqiPeri + (_ueCb)->cqiCb.nCqiTrIdx;\
  _nCqiIdx = _nCqiIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  \
  /* Delete from current List and move to new list */ \
   cmLListDelFrm(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->cqiCb.nCqiTrIdx].cqiLst),\
                                            &((_ueCb)->cqiCb.cqiLstEnt));\
   (_ueCb)->cqiCb.nCqiTrIdx = _nCqiIdx;\
   cmLListAdd2Tail(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->cqiCb.nCqiTrIdx].cqiLst),\
                                               &((_ueCb)->cqiCb.cqiLstEnt));\
}


/* To Move Next Occurance for periodic RI Idx */
/* Caller should not call this Macro if RI is disabled */
#define RG_SCH_MOVE_RI_UE(_cellCb, _ueCb)\
{\
  U16   _nRiIdx;\
  U16   _nRiDist;\
  U16   _effPeriodicity;\
  /* Compute Next Transmission Instance */ \
  if ((_ueCb)->cqiCb.cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_WB_REP)\
   {\
     _effPeriodicity = (_ueCb)->cqiCb.cqiPeri * ((_ueCb)->cqiCb.riPeri);\
     _nRiIdx = _effPeriodicity + (_ueCb)->cqiCb.nRiTrIdx;\
   }\
  else \
   {\
     _effPeriodicity = ((_ueCb)->cqiCb.h * (_ueCb)->cqiCb.cqiPeri * \
                           (_ueCb)->cqiCb.riPeri);\
     _nRiIdx = _effPeriodicity + (_ueCb)->cqiCb.nRiTrIdx;\
   }\
  _nRiIdx = _nRiIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  \
  _nRiDist = _nRiIdx/RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  \
  /* Delete from current List and move to new list */ \
   cmLListDelFrm(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->cqiCb.nRiTrIdx].riLst),\
                         &((_ueCb)->cqiCb.riLstEnt));\
   (_ueCb)->cqiCb.nRiTrIdx = _nRiIdx;\
   (_ueCb)->cqiCb.riDist = _nRiDist;\
   cmLListAdd2Tail(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->cqiCb.nRiTrIdx].riLst), \
                          &((_ueCb)->cqiCb.riLstEnt));\
}

/* To Move Next Occurance for periodic SRS Idx */
/* Caller should not call this Macro if SRS is disabled */
#define RG_SCH_MOVE_SRS_UE(_cellCb, _ueCb)\
{\
  U16   _nSrsIdx;\
  /* Compute Next Transmission Instance */ \
  _nSrsIdx = (_ueCb)->srsCb.peri + (_ueCb)->srsCb.nSrsTrIdx;\
  _nSrsIdx = _nSrsIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  \
  /* Delete from current List and move to new list */ \
   cmLListDelFrm(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->srsCb.nSrsTrIdx].srsLst),\
                                  &((_ueCb)->srsCb.srsLstEnt));\
   (_ueCb)->srsCb.nSrsTrIdx = _nSrsIdx;\
   cmLListAdd2Tail(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->srsCb.nSrsTrIdx].srsLst),\
                           &((_ueCb)->srsCb.srsLstEnt));\
} 

/* To Move Next Occurance for periodic SR Idx */
/* Caller should not call this Macro if SR is disabled */
#define RG_SCH_MOVE_SR_UE(_cellCb, _ueCb)\
{\
  U16   _nSrIdx;\
  /* Compute Next Transmission Instance */ \
  _nSrIdx = (_ueCb)->srCb.peri + (_ueCb)->srCb.nSrTrIdx;\
  _nSrIdx = _nSrIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  \
  /* Delete from current List and move to new list */ \
   cmLListDelFrm(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->srCb.nSrTrIdx].srLst),\
                                           &((_ueCb)->srCb.srLstEnt));\
   (_ueCb)->srCb.nSrTrIdx = _nSrIdx;\
   cmLListAdd2Tail(&((_cellCb)->pCqiSrsSrLst[(_ueCb)->srCb.nSrTrIdx].srLst),\
                                           &((_ueCb)->srCb.srLstEnt));\
}

#define  RG_SCH_PARSE_MULTI_PMI(_bitLen, _totPmiBitLen, _psMode12,\
                                   _ueCb, _revArray,_sbOffst )\
{\
   U8 _loop =0;\
   for(_loop =0; _loop<(_totPmiBitLen/_bitLen); _loop++)\
   {\
       (_psMode12)->subbandArr[_loop].pmi = (U8)rgSCHUtlParse(_revArray,\
               _sbOffst,(U8)(_sbOffst+_bitLen), (U8)TFU_MAX_CQI_BYTES);\
       _sbOffst+=_bitLen;\
       (_psMode12)->subbandArr[_loop].subBand.numRb = (_ueCb)->acqiCb.k;\
       (_psMode12)->subbandArr[_loop].subBand.rbStart = \
                                   (U8) ((_ueCb)->acqiCb.k * (_loop));\
   }\
}

/*  Removed the MACRO RG_SCH_GET_PERIODICITY_TBL
 *  The function rgSCHUtlGetPcqiSrsSrRiTbl will do the same task as this 
 *  MACRO*/

#endif 




#ifdef LTEMAC_HDFDD
/* Half Duplex Specific defines */
/* Number of subframes information managed */
#define RG_SCH_HDFDD_NUMSFINFO   20
/* Subframe States */
#define RG_SCH_HDFDD_NOSCHD       0x00
#define RG_SCH_HDFDD_DLDATA       0x01
#define RG_SCH_HDFDD_DLCNTRL      0x02
#define RG_SCH_HDFDD_UL           0x04

#define RG_SCH_HDFDD_INVSFN       (RGSCH_MAX_SFN + 100) /* Add some arbitrary number to make it invalide */

#define RG_SCH_HDFDD_GRDTIM_DUR  1
#define RG_SCH_HDFDD_DELTA   10

/* To get the BCH is present or not at subframe */
#define RG_SCH_BCCH_TRUE_FALSE( _time, _bchTrue)\
{\
  _bchTrue = FALSE;\
  /* Call the API is provided by SI module */ \
}

/* Mark the subframe */
#define RG_SCH_HDFDD_MARKSTATE(_ueCb, _state, _sfn, _sfi)\
{\
   (_ueCb)->hdFddCb->subfrm[(_sfi)].subFrmDir = _state;\
   (_ueCb)->hdFddCb->subfrm[(_sfi)].sfn = _sfn;\
}

/* validate the ueCb and mark */
#define RG_SCH_HDFDD_VLDTANDMARK(_ueCb, _state, _sfn, _sfi)\
{\
   if( (_ueCb) != NULLP && (_ueCb)->hdFddCb)\
   {\
      RG_SCH_HDFDD_MARKSTATE(_ueCb, _state, _sfn, _sfi);\
   }\
}\

/* Get SFN and SFI from tti numbers */
#define RG_SCH_HDFDD_GETPTI(_time) ((((_time).sfn * RGSCH_NUM_SUB_FRAMES + \
                 (_time).subframe) + RG_SCH_HDFDD_DELTA) % RG_SCH_PCQI_SRS_SR_TRINS_SIZE)

/* Get SFI and SFN from given time and subframe offset */
#define RG_SCH_HDFDD_GETSFN(_sfn, _time, _offset)\
                     rgSCHHdFddGetSfn(&(_sfn), (_time), (_offset))

/* Get SFI and SFN from given time and subframe offset */
#define RG_SCH_HDFDD_GETSFI(_sfi, _time, _offset)\
          (_sfi) = (((_time).sfn * RGSCH_NUM_SUB_FRAMES) + \
                  ((_time).subframe + _offset))% RG_SCH_HDFDD_NUMSFINFO

/*If UE is HDFDD enabled */
#define RG_SCH_HDFDD_UE_ENBLD(_ue) ((_ue)->hdFddEnbld)
/*If HDFDD UE is scheduled */
#define RG_SCH_HDFDD_ISCMN_SCHED(_dlSf) ((_dlSf)->bch.tbSize || \
                    (_dlSf)->bcch.tbSize || (_dlSf)->pcch.tbSize)

#define RG_SCH_HDFDD_ROLLSFN(_sfCount, _sfn)\
do{\
   if (_sfCount < -RGSCH_NUM_SUB_FRAMES) \
   {\
      _sfn = (_sfn + (_sfCount/RGSCH_NUM_SUB_FRAMES)) & (RGSCH_MAX_SFN - 1);\
   } \
}while(0);

#endif /* LTEMAC_HDFDD */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT

/* Macro to check if expected CQI report collation has been done */
#define RG_SCH_CQIR_IS_TIMTOSEND_CQIREPT(_ue)\
     ((_ue)->schCqiInfo.cqiCount >= \
     (_ue)->cqiReptCfgInfo.numColltdCqiRept )

/* Macro to check if PUSH N CQI Reporting is still active */
#define RG_SCH_CQIR_IS_PUSHNCQI_ENBLE(_ue)\
   ((_ue)->cqiReptCfgInfo.numColltdCqiRept != 0)
#endif /* ifdef RGR_CQI_REPT */

/* Macro to check if Pa has valid value */
#define RG_SCH_IS_PAPRSNT(_ue) ((_ue)->pA.pres == TRUE)

/* Macro to check if Pa has been configured by RRM */
#define RG_SCH_UE_CFG_ISPAPRSNT(_info) ((_info).pAPrsnt == TRUE)

/* Macro to get absolute difference between two INT */
#define RG_ABSLT_DIFF(x, y) ((x > y)? (x-y):(y-x)) 

/* MIB transmission Count for DL TB counter */
#ifdef LTE_L2_MEAS
#define RG_SCH_MIB_CNT       4
#endif 

#define RG_SCH_PFS_FLW_CTRL_TRIG_TIME 64 

/* Interval between Transmission and feedback */
#define RG_SCH_CMN_HARQ_INTERVAL  4   
#define RG_SCH_MAX_MPHICH         3
#define RG_SCH_CMN_MAX_CFI        4

#define RGSCH_GET_SPS_SF_CFI(_dlTotalBw, _cfi)\
{\
   if(_dlTotalBw <= 10)\
   {                              \
      _cfi= 2;                    \
   }                              \
   else                           \
   {                              \
      _cfi = RGSCH_MIN(2, _cfi);  \
   }                              \
}      

#ifdef TFU_UPGRADE
#define RG_UPD_ACQI_TRIG_WT(_ue, _isAck)\
{\
   if(_ue->dl.ueDlCqiCfg.aprdCqiCfg.pres)\
   {\
      rgSCHUtlUpdACqiTrigWt(_ue, _isAck);\
   }\
}
#else
#define RG_UPD_ACQI_TRIG_WT(_ue, _isAck){}
#endif

/* SR_RACH_STATS */
EXTERN U32 rgNumPrachRecvd;       /* Num of Rach Req received including dedicated preambles */
EXTERN U32 rgNumRarSched;         /* Num of RARs sent */
EXTERN U32 rgNumBI;               /* Num of BackOff Ind sent */
EXTERN U32 rgNumMsg3CrcPassed;    /* Num of CRC success for Msg3 */
EXTERN U32 rgNumMsg3CrcFailed;    /* Num of CRC fail for Msg 3 */
EXTERN U32 rgNumMsg3FailMaxRetx;  /* Num of Msg3 fail after Max Retx attempts */
EXTERN U32 rgNumMsg4Ack;          /* Num of Acks for Msg4 Tx */
EXTERN U32 rgNumMsg4Nack; 
       /* Num of Nacks for Msg4 Tx */
EXTERN U32 rgNumMsg4FailMaxRetx;  /* Num of Msg4 Tx failed after Max Retx attempts */
EXTERN U32 rgNumSrRecvd;          /* Num of Sched Req received */
EXTERN U32 rgNumSrGrant;          /* Num of Sched Req Grants sent */
EXTERN U32 rgNumMsg3CrntiCE;      /* Num of Msg 3 CRNTI CE received */
EXTERN U32 rgNumDedPream;         /* Num of Dedicated Preambles recvd */
EXTERN U32 rgNumMsg3CCCHSdu;      /* Num of Msg 3 CCCH Sdus recvd */
EXTERN U32 rgNumCCCHSduCrntiNotFound;  /*UE Ctx not found for CCCH SDU Msg 3 */
EXTERN U32 rgNumCrntiCeCrntiNotFound;  /*UE Ctx not found for CRNTI CE Msg 3 */
EXTERN U32 rgNumMsg4WithCCCHSdu;       /* Num of Msg4 with CCCH Sdu */
EXTERN U32 rgNumMsg4WoCCCHSdu;         /* Num of Msg4 without CCCH Sdu */
EXTERN U32 rgNumMsg4Dtx;               /* Num of DTX received for Msg 4 */
EXTERN U32 rgNumMsg3AckSent;           /* Num of PHICH Ack sent for Msg 3 */
EXTERN U32 rgNumMsg3NackSent;          /* Num of PHICH Nack sent for Msg 3 */
EXTERN U32 rgNumMsg4PdcchWithCrnti;    /* Num of PDCCH for CRNTI based contention resolution */
EXTERN U32 rgNumRarFailDuetoRntiExhaustion; /* Num of RACH Failures due to RNTI pool exhaution */
EXTERN U32 rgNumTAModified;            /* Num of times TA received is different from prev value */
EXTERN U32 rgNumTASent;               /* Num of TA Command sent */
EXTERN U32 rgNumMsg4ToBeTx;           /* Num of times MSG4 that should be sent */
EXTERN U32 rgNumMsg4Txed;             /* Num of MSG4 actually sent *//* ysNumMsg4ToBeTx -ysNumMsg4Txed == Failed MSG4 TX */
EXTERN U32 rgNumMsg3DtxRcvd;         /* CRC Fail with SINR < 0 */

EXTERN U32 rgNumDedPreamUECtxtFound;
#endif /* __RGSCH__ */

/**********************************************************************
         End of file:     rg_sch.h@@/main/2 - Sat Jul 30 02:22:04 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       st  1. LTE MAC 2.1 release
/main/1+     rg002.201 sd  1. ccpu00108815: Corrected timing for SIB2 handling.
                           2. ccpu00108811: Corrected allocation for common
                              channels.
            rg004.201  pk  1. ccpu00109921: PBCH allocation 
             rg004.201 rnt 1. Modified for SI Enhancement
             rg005.201 sm  1. Changed marco to use U8 instead of int
                           2. Changed values for optimization
             rg006.201 sd  1. ccpu00112398: Added periodic BSR timer
                           2. ccpu000111706, ccpu00112394:Added MACRO
                              RG_TFU_HQ_IND_DELTA  
             rg007.201   ap  1. Added support for MIMO
             rg008.201 sd  1.Removed dependency on MIMO compile-time flag
                  rsharon 2.Added support for SPS.
             rg009.201 rsharon  1.[ccpu00112372]Changes for CR timer.
             rg009.201 dv  1.Added support for TFU_UPGRADE.
                           2.ccpu00116486- Macro made independent of RG_UL_DELTA
                           3.ccpu00116309 - Support for iPhich=1.
             rg009.201 gj  1. [ccpu00116647]- Added Lower limit for siId
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
             rg002.301 asehgal 1. ccpu00118468 - introduced a define for deflt
                                                 value of TA
                               2.Removed the MACRO RG_SCH_GET_PERIODICITY_TBL.
                               3.ccpu00119331: Changing variable type.
                               4. ccpu00118351:Introduced seperate deltas for
                                  uplink & downlink control.
$SID$        ---       rt      1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
