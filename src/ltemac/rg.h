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
 
    File:   rg.h
 
    Sid:      rg.h@@/main/3 - Sat Jul 30 02:22:00 2011
 
    Prg:    st
 
**********************************************************************/
 
/** @file rg.h
@brief This file contains definitons for Mac.
*/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/
 
#ifndef __RGH__
#define __RGH__

#include "rl_interface.h"
#include "rl_common.h"

#define RGLAYERNAME     "MAC"
#define RG_MAX_TB_PER_UE 2
#define RG_MAX_BCCH_DLSCH 2
#define RG_INVALID_RNTI   0x0000

#define RG_MAX_NUM_DED_LC 10       /* maximum dedicated logical channels in a UE */
#define RG_MAX_NUM_CMN_LC 5       /* maximum number of common logical 
                                     channels in a cell */
#define RG_MAX_LCG_PER_UE 4
#define RG_CON_RES_ID_SZ  6
#define RG_MAX_RA_RNTI   60

/* Added support for SPS*/
#ifdef LTEMAC_SPS
#define RG_ADDL_SPS_OCC_FOR_EXP_REL 2

#define RG_SPS_SID_PACKET_SIZE 10
#define RG_SPS_LCG_ID  1

#endif

/* Well known RNTIS */
#define RG_SI_RNTI        0xffff
#define RG_P_RNTI         0xfffe
#if 0
#define RG_RGU_UL_SAP     0
#define RG_RGU_DL_SAP     1
#endif

#define RG_MAX_UPPERSAP               3
#define RG_MAX_LOWERSAP               1 
#define RG_TQ_SIZE                    10   /* Timing Queue Size */
#define RG_MAX_TIMER     RG_MAX_LOWERSAP   /* MAX number of MAC timers */
#define RG_NMB_CELL_HASHBIN           10   /* Number of Hash Bins for cell hash
                                              table */
#define RG_NMB_UE_HASHBIN             10   /* Number of Hash Bins for UE hash
                                              table */        
#define RG_BNDREQ_TMR    1    /* Bind Request timer event value */
#define RG_MAX_BNDRETRY  2    /* Max number of Bind Retries for TFU SAP */


#define RG_UE_TQ_SIZE    10   /* Timing Queue Size */

#define RG_INVALID_SCHD_TYPE 255
#define RG_MAX_UE_PER_CELL 0xFFFF /*!< Maximum number of UEs per cell */
#define RG_MAX_UE_BIN_PER_CELL 128 /*!< Maximum number of UE bins per cell */
#define RG_MIN_DL_BW       6    /*!< Minimum Downlink bandwidth in RBs */
#define RG_MAX_DL_BW       110  /*!< Maximum Downlink bandwidth in RBs */
#define RG_MIN_UL_BW       6    /*!< Minimum Uplink bandwidth in RBs */
#define RG_MAX_UL_BW       110  /*!< Maximum Uplink bandwidth in RBs */
#define RG_MIN_CFI_VAL     1    /*!< Minimum value for CFI */
#define RG_MAX_CFI_VAL     3    /*!< Maximum value for CFI */
#define RG_QM_BPSK         2    /*!< Qm value for BPSK */
#define RG_QM_QPSK         4    /*!< Qm value for QPSK */
#define RG_QM_64QAM        6    /*!< Qm value for 64QAM */
#define RG_MIN_SRS_SFCFG_IDX 0  /*!< Minimum value for SRS subframe 
                                     configurtion index */
#define RG_MAX_SRS_SFCFG_IDX 15  /*!< Maximum value for SRS subframe 
                                     configurtion index */
#define RG_MAX_SRS_TX_OFFSET 8    /*!< Maximum number of SRS transmission 
                                    offsets per cell */
#define RG_MIN_MAC_RNTI      10   /*!< Minimum value of RNTI to be managed by 
                                    MAC */

/* HARQ related MACROs */
#define RG_NUM_DL_HQ_PROC 8
#define RG_NUM_UL_HQ_PROC 8
#define RG_MIN_HQ_TX 1

/* Group power related MACROs */
#define RG_MAX_GRP_PWR_FMT3_IDX  15   /*!< Maximum index value for group power format 3 */
#define RG_MAX_GRP_PWR_FMT3A_IDX 31   /*!< Maximum index value for group power format 3A */

/* MACROs to indicate cell specific config for cell to be active */
#define RG_BCCH_BCH_CFG_DONE      (1<<0)
#define RG_BCCH_DLSCH_CFG1_DONE    (1<<1)
#define RG_BCCH_DLSCH_CFG2_DONE    (1<<2)
#define RG_PCCH_CFG_DONE          (1<<3)
#define RG_UL_CCCH_CFG_DONE       (1<<4)
#define RG_DL_CCCH_CFG_DONE       (1<<5)
#define RG_SCHD_CFG_DONE          (1<<6)
#define RG_CELL_ACTIVE (RG_BCCH_BCH_CFG_DONE | RG_BCCH_DLSCH_CFG1_DONE |  RG_BCCH_DLSCH_CFG2_DONE | RG_PCCH_CFG_DONE | RG_UL_CCCH_CFG_DONE | RG_DL_CCCH_CFG_DONE)
/* Logical channel realated MACROs */
#define RG_INVALID_LCG_ID  255
#define RG_INVALID_LC_ID   255
#define RG_BCCH_BCH_IDX   0
#define RG_BCCH_DLSCH_IDX 1
#define RG_PCCH_IDX 2

/* constants for MIN & MAX qci*/
#define RG_QCI_MIN 1
#define RG_QCI_MAX 9

/* PUCCH related macros */
#define RG_PUCCH_MAXVAL_CS    7   /*!< Maximum value for cyclic shift of PUCCH */
#define RG_PUCCH_MINVAL_DS    1   /*!< Mininmum value for delta shift of PUCCH */
#define RG_PUCCH_MAXVAL_DS    3   /*!< Maximum value for delta shift of PUCCH */

/* DUX related macros */
#define RG_LCID_MASK 0x1F
#define RG_LCID_LEN 0x5
#define RG_CCCH_LCID 0x00
#define RG_DEDLC_MIN_LCID 0x01
#define RG_DEDLC_MAX_LCID 0x0A
#define RG_RES_MIN_LCID 0x0B
#define RG_RES_MAX_LCID 0x19
#define RG_PHR_LCID 0x1A
#define RG_CRNTI_LCID 0X1B
#define RG_TRUNC_BSR_LCID 0X1C
#define RG_SHORT_BSR_LCID 0X1D
#define RG_LONG_BSR_LCID  0X1E
#define RG_PAD_LCID 0x1F
#define RG_MAX_EXTN_PAD_SUBHDRS 0x02

#define RG_CCCH_SDU_PRSNT     (1<<0)
#define RG_CRNTI_CE_PRSNT     (1<<1)
#define RG_PHR_CE_PRSNT       (1<<2)
#define RG_TRUNC_BSR_CE_PRSNT (1<<3)
#define RG_SHORT_BSR_CE_PRSNT (1<<4)
#define RG_LONG_BSR_CE_PRSNT  (1<<5)
/* L2_COUNTERS */
#define RG_ACTIVE_LC_PRSNT  (1<<6)

#ifdef LTEMAC_SPS
#define RG_UL_SPS_ACT_PRSNT (1<<7)
#endif

/* LOGICAL CHANNEL */
#define RG_MAX_LC_PER_UE   10
/* Maximum number of common logical channel control blocks */
#define RG_MAX_CMN_LC_CB   3
#define RG_MAX_BCCH   2

#define RG_OPTM_NUM_DED_LC    3

/* Random access related MACROs */
#define RG_MAX_RA_PREAMBLE_FMT 3 /*!< Maximun value of Random access preamble 
                                      format */
#define RG_MAX_RA_WINSIZE    10  /*!< Maximum size of Random access response 
                                      window in subframes */
#define RG_MIN_RA_WINSIZE    2   /*!< Minimum size of Random access response 
                                      window in subframes */
#define RG_MIN_NUM_RA_PREAMBLE 4 /*!< Minimum number of Random access 
                                      preambles */
#define RG_MAX_NUM_RA_PREAMBLE 64 /*!< Maximim number of Random access 
                                      preambles */
#define RG_NUM_RA_RB     6

#define RG_MAX_RA_RSP_ALLOC    4 /*!< Maximum number of Random access
                                      allocations */
#define RG_CRG_CFG 1          /* CRG configuration element */
#define RG_RGR_CFG 2          /* RGR configuration element */

#define RG_NUM_ITBS         27
#define RG_MAX_NUM_RB       110
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
#define RG_MAX_LYR_PERCW    2

#ifdef LTE_TDD
#define RG_MAX_DL_HARQ_NUM   15
#else
#define RG_MAX_DL_HARQ_NUM   8 
#endif

#define RG_LCG_ISCFGD(lcg) ((lcg)->lcgId != RG_INVALID_LCG_ID)
/* Corrected the check for dlCcchId */
#define RG_DLCCCH_ISCFGD(cell) ((cell)->dlCcchId != RG_INVALID_LC_ID)
#define RG_ULCCCH_ISCFGD(cell) ((cell)->ulCcchId != RG_INVALID_LC_ID)
/* After merging from 2.1 to 2.2 */
#define RG_CALC_SF_DIFF(_time1, _time2)\
   (_time1.sfn*10+_time1.subframe) < (_time2.sfn*10+_time2.subframe)?\
     ((_time1.sfn+RG_MAX_SFN)*10+_time1.subframe) -\
       (_time2.sfn*10+_time2.subframe) : \
     (_time1.sfn*10+_time1.subframe) - (_time2.sfn*10+_time2.subframe)
/*LTE_L2_MEAS_PHASE2*/
#define RG_CALC_SFN_SF_DIFF(_time1,_sfnCycle, _time2)\
(((_time1.sfn+RG_MAX_SFN * _sfnCycle)*10) + _time1.subframe -\
(_time2.sfn*10 + _time2.subframe))


#define RGADDTOCRNTTIME(crntTime, toFill, incr)          \
   if ((crntTime.subframe + incr) > (RG_NUM_SUB_FRAMES - 1))   \
      toFill.sfn = (crntTime.sfn + 1) % RG_MAX_SFN;      \
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.subframe = (crntTime.subframe + incr) % RG_NUM_SUB_FRAMES;

#define RGSUBFRMCRNTTIME(crntTime, toFill, dcr)    \
{                                                  \
   if (crntTime.sfn == 0)                          \
{                                                  \
   if ((crntTime.subframe - (dcr)) < 0)              \
   {                                               \
      toFill.sfn = RG_MAX_SFN - 1;                 \
   }                                               \
   else                                            \
   {                                               \
   toFill.sfn = crntTime.sfn;                      \
   }                                               \
}                                                  \
else                                               \
{                                                  \
   if ((crntTime.subframe - (dcr)) < 0)              \
   {                                               \
   toFill.sfn = crntTime.sfn - 1;                  \
   }                                               \
   else                                            \
   toFill.sfn = crntTime.sfn;                      \
}                                                  \
toFill.subframe = (RG_NUM_SUB_FRAMES + crntTime.subframe - (dcr)) % (RG_NUM_SUB_FRAMES);                                   \
}

#define RGCPYTIMEINFO(src, dst)  \
   dst.sfn        = src.sfn;     \
   dst.subframe   = src.subframe;
#define RG_TIMEINFO_SAME(x, y) ((x.sfn == y.sfn) && (x.subframe == y.subframe))


#define rgPBuf  rgCb.rgInit.prntBuf

#if 0
#define RGDBGPRM(_args)          \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_PRM, _args)
#define RGDBGERR(_args)          \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_ERR, _args)
#define RGDBGINFO(_args)         \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_INFO, _args)
#endif
/* Debug Prints for MAC */
#ifdef DEBUGP
#define RGDBGERRNEW(_args)          \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_ERR, _args)
#define RGDBGINFONEW(_args)         \
                  DBGP(&rgCb.rgInit, RGLAYERNAME, DBGMASK_INFO, _args)
#else
#define RGDBGERRNEW(_args) 
#define RGDBGINFONEW(_args)
#endif /* #ifdef DEBUGP */
#define RGDBGPRM(_args) 
#define RGDBGERR(_args) 
#define RGDBGINFO(_args)

#ifdef ERRCLS_KW
#define RG_NULL_CHECK(_inst, _ptr )     \
   if((_ptr) == NULLP)  \
   {                                               \
      RGDBGERRNEW((rgPBuf, "Null Pointer detected"));\
      SExit();\
   }
#define RG_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)     \
   if((_idxVal) >= (sizeof(_array)/sizeof(_array[0]))) \
   {                                               \
      RGDBGERRNEW((rgPBuf, "Array Bound Check Failed"));\
      SExit();\
   }
#else
#define RG_NULL_CHECK(_inst, _ptr )     
#define RG_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)     
#endif

/* Macro to free the message buffer and initialize it to zero */
/***********************************************************
 *
 *     Name : RG_FREE_MSG
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
#define RG_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      SPutMsg((_buf)); \
      _buf = NULLP; \
   } \
}

#define RG_FREE_MEM(_mem)\
{\
   if (NULLP != (_mem)) \
   { \
      cmFreeMem((_mem)); \
      _mem = NULLP; \
   } \
}

#ifdef L2_OPTMZ

#define RG_FREE_TB(_tb)\
{\
   U32 lchIdx, pduIdx;\
   SResetMBuf(_tb->macHdr);\
   SResetMBuf(_tb->macCes);\
   _tb->tbPres = FALSE;\
   _tb->tbSize = 0;\
   _tb->padSize = 0;\
   for(lchIdx = 0; lchIdx < 10; lchIdx++) \
   {\
      for(pduIdx = 0; pduIdx < 4; pduIdx++)\
      {\
          _tb->lchInfo[lchIdx].mBuf[pduIdx] = NULL;\
      }\
      _tb->lchInfo[lchIdx].numPdu = 0;\
   }\
   _tb->numLch = 0;\
}

#endif

#if 0

         RG_FREE_MSG(_tb->lchInfo[lchIdx].mBuf[pduIdx]);
   for(lchIdx = 0; lchIdx < 10; lchIdx++) 
   {
      for(pduIdx = 0; pduIdx < 4; pduIdx++)
      {
      }
      _tb->lchInfo[lchIdx].numPdu = 0;
   }

#endif

/***********************************************************
 *
 *     Name : RG_DROP_RGUDDATREQ_MBUF
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
#if 0
#define RG_DROP_RGUDDATREQ_MBUF(_datReq)\
{\
   U32 idx1,idx2,idx3;\
   U32 idx;\
   if (_datReq != NULLP)\
   {\
      for(idx = 0; idx < _datReq->nmbOfUeGrantPerTti; idx++)\
      {\
         for (idx1=0; idx1 < _datReq->datReq[idx].nmbOfTbs; idx1++)\
         {\
            for (idx2=0; idx2 < _datReq->datReq[idx].datReqTb[idx1].nmbLch; idx2++)\
            {\
               for (idx3=0; \
                     idx3 < _datReq->datReq[idx].datReqTb[idx1].lchData[idx2].pdu.numPdu; \
                     idx3++)\
               {\
                  RG_FREE_MSG(_datReq->datReq[idx].datReqTb[idx1].\
                        lchData[idx2].pdu.mBuf[idx3]);\
               }\
            }\
         }\
      }\
   }\
}
#else
#define RG_DROP_RGUDDATREQ_MBUF(_datReq)\
{\
   U32 idx5,idx6,idx7;\
   for (idx5=0; idx5 < _datReq.nmbOfTbs; idx5++)\
   {\
      for (idx6=0; idx6 < _datReq.datReqTb[idx5].nmbLch; idx6++)\
      {\
         for (idx7=0; \
               idx7 < _datReq.datReqTb[idx5].lchData[idx6].pdu.numPdu; \
               idx7++)\
         {\
            RG_FREE_MSG(_datReq.datReqTb[idx5].\
                  lchData[idx6].pdu.mBuf[idx7]);\
         }\
      }\
   }\
}

#endif
/***********************************************************
 *
 *     Name : RG_DROP_RGUCDATREQ_MBUF
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
#define RG_DROP_RGUCDATREQ_MBUF(_datReq)\
{\
   if (_datReq != NULLP)\
   {\
      RG_FREE_MSG(_datReq->pdu);\
   }\
}

/* RRM_SP1_START */
#define RG_UPD_GBR_PRB(_cellCb, _qci, _prbUsed) {\
   if(_qci <= RG_MAX_QCI_REPORTS)\
   {\
      _cellCb->qcisUlPrbCnt[_qci-1] += _prbUsed;\
   }\
}
/* RRM_SP1_END */

/* Macros for memory region and pool determination */
#define RG_GET_MEM_REGION(rgCb)  (rgCb.rgInit.region)
#define RG_GET_MEM_POOL(rgCb)    (rgCb.rgInit.pool)


/* MUX related macros */
#define RG_RAR_SHDR_LEN                1
#define RG_RAR_ELEM_LEN                6
#define RG_MAX_SDU_SUB_HDR_LEN         3
#define RG_MAX_PAD_ARR_SZ              4096 /* Changing from 400 to 4096 */
#define RG_PAD_BYTE                    0x00

#define RG_HDR_TYPE_CRES                1
#define RG_HDR_TYPE_TA                  2

#define RG_SDU_SHDR_LEN 1
#define RG_CE_SHDR_LEN 1
#define RG_CRES_LEN    6
#define RG_TA_LEN      1
#define RG_CRES_ELM_LEN (RG_CE_SHDR_LEN+RG_CRES_LEN)
#define RG_TA_ELM_LEN   (RG_CE_SHDR_LEN+RG_TA_LEN)

#define RG_CRES_LCID_IDX               0x1C
#define RG_TA_LCID_IDX                 0x1D

/* Structure member offset computation macro    */
#define OffsetOf(type, field)                                                 \
         (PTR) (&(((type *) NULLP)->field))

#define RG_MAX_SUBFRAMES_IN_SFN        9
#define RG_MAX_SFN                     1024
#define RG_NUM_SUB_FRAMES              10
/* RRM_SP1_START */
#define RG_MAX_QCI_REPORTS         4
/* RRM_SP1_END */

#ifdef LTE_L2_MEAS
#define RG_NUM_UL_SUB_FRAMES              16
#define RG_MAX_QCI_VALUE                  10

#define RG_CALC_SF_DIFF(_time1, _time2)\
      (_time1.sfn*10+_time1.subframe) < (_time2.sfn*10+_time2.subframe)?\
     ((_time1.sfn+RG_MAX_SFN)*10+_time1.subframe) -\
       (_time2.sfn*10+_time2.subframe) : \
     (_time1.sfn*10+_time1.subframe) - (_time2.sfn*10+_time2.subframe)

#define RG_TTI_CYCLE_INVLD                0xFFFFFFFF     
#define RG_CALC_TTI_CNT(_cellCb, _ttiCnt)\
     _ttiCnt = (10 * (_cellCb->crntTime.sfn + (_cellCb->ttiCycle * 1024)) )+\
               _cellCb->crntTime.subframe;
#endif /* LTE_L2_MEAS */

/* Tuned according to TDD Cfg Mode2 and 2UE/TTI.
 * Need to tune if NumUE/TTI is increased */
#define RG_MAX_DFRD_FREE_BUFS             32 /* 16- 2 UE per TTI 32 - 4UE/TTI */
#define RG_MAX_FREE_BUFS_PERTTI           8 /* 4 - 2 Ue per TTI, 8 - 4UE/TTI */


/* Define for the block size for memory allocation */
/* RG_BLKSZ changed from 2048 to 1500*/
#define RG_BLKSZ                       1500

/* Defines for RGU Statistics types */
#define RG_RGU_SDU_DROP 1
#define RG_RGU_SDU_RCVD 2

/* MACROS for General Statistics */
#define RG_CFG_ADD      1
#define RG_CFG_DEL      2

#define RG_HQ_FDB_IND_CB_TYPE_HQ_ENT      1
#define RG_HQ_FDB_IND_CB_TYPE_RA_CB       2

/* 
 * Removed unused hash-define which defines the
 * index for releasing the subframe.
 */

/* Value used to set nDmrs in uplink grant if nDmrs is not applicable */
#define RG_INVALID_NDMRS  10

#define RG_SEND_TRC_IND(_mBuf, _event) rgLMMTrcInd(_mBuf, _event)

/* Note: Any changes to these enums should reflect to */
/** @details Enums for special argument
 *
*/
typedef enum
{
   RG_DIAG_NA
} RgDiagSplArg;
 
#ifdef SS_DIAG 
#define RG_DIAG_LVL0(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                        \
{                                                                                                \
   if(rgCb.rgInit.logMask & SS_DIAG_LVL0)                                                         \
   {                                                                                             \
      ssDiagFix(_tknId, _splArgEnum, ENTRG, rgCb.rgInit.inst, SS_DIAG_LVL0, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                             \
}

/** @details Macro definition for LTE-MAC error logs
 *  
*/
#define RG_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \
{                                                                 \
   if(rgCb.rgInit.logMask & SS_DIAG_LVL1)                             \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTRG, rgCb.rgInit.inst, SS_DIAG_LVL1, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for LTE-MAC critical logs
 *  
*/
#define RG_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
   if(rgCb.rgInit.logMask & SS_DIAG_LVL2)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTRG, rgCb.rgInit.inst, SS_DIAG_LVL2, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for LTE-MAC logs 
 *  
*/
#define RG_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(rgCb.rgInit.logMask & SS_DIAG_LVL3)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTRG, rgCb.rgInit.inst, SS_DIAG_LVL3, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for LTE-MAC logs
 *  
*/
#define RG_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(rgCb.rgInit.logMask & SS_DIAG_LVL4)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTRG, rgCb.rgInit.inst, SS_DIAG_LVL4, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}
  
#else

#define RG_DIAG_LVL0(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                        \
{                                                                                                \
}

/** @details Macro definition for LTE-MAC error logs
 *  
*/
#define RG_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \
{                                                                 \
}

/** @details Macro definition for LTE-MAC critical logs
 *  
*/
#define RG_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
}

/** @details Macro definition for LTE-MAC logs 
 *  
*/
#define RG_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
}

/** @details Macro definition for LTE-MAC logs
 *  
*/
#define RG_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
}
#endif

#endif /* __RGH__ */

/**********************************************************************
         End of file:     rg.h@@/main/3 - Sat Jul 30 02:22:00 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       st  1. Initial Release
/main/2      ---       st  1. LTE MAC 2.1 release
             rg005.201 sm  1. Modified values for optimization.
             rg007.201 gvj/ap  1. Added support for MIMO
             rg008.201 sd  1.Removed dependency on MIMO compile-time flag
                       rsharon 2.Added support for SPS.
             rg009.201 dv  1.ccpu00116498- Change in RG_BLKSZ
                           2.ccpu00116700- changed from U32 to PTR
/main/3      ---     gj                1. LTE MAC 3.1 Release.                           
           rg002.301  asehgal 1.ccpu0010621:hash defines for min & max qci 
                               defined.
                              2. ccpu00118351:seperate deltas for UL & DL.
$SID$      ---         rt     1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
