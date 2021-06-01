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
  
        Name:     LTE RLC file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for RLC 
 
        File:     kw.h

        Sid:      pj.h@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:47:44 2014
        Sid:      pj.h@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:47:44 2014
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw.h
@brief RLC Hash definitions
*/

#ifndef __PJH__
#define __PJH__

#include "rl_interface.h"
#include "rl_common.h"
#include "rl_rlog.h"

 
#define PJLAYERNAME   "LTE PDCP"     /* Layer Name */

#define EPJxxx 1
#define PJ_TMR_LEN                     10
#define PJ_MAX_KWUSAPS        2     /*!< Maximum number of KWU Saps. */
#define PJ_MAX_UDXSAPS        1     
#define PJ_MAX_RB_PER_CELL          10
#define PJ_MAX_SRB_PER_UE           3
#define PJ_MAX_DRB_PER_UE           32
#define PJ_MAX_LCH_PER_UE           12
#define PJ_MAX_LCH_PER_CELL         6
#define PJ_MAX_NUM_RB               24
#define PJ_MAX_UE                   0xffffffff 
#define PJ_UE_LIST_BUCKET_SIZE      128 
#define PJ_CELL_LIST_BUCKET_SIZE    10 
#define PJ_MAX_RB                   32
#define PJ_DEF_SEQ_NUM 0
#define PJ_ASYNC_WIN                 16

#define PJ_MAX_CPJSAPS        1     /*!< Maximum number of CPJ Saps. */
#define PJ_MAX_PJUSAPS        2     /*!< Maximum number of PJU Saps. */

#define PJ_LMM_GENCFG_DONE    2     /*!< PDCP LM general configuraiton done. */

#define PJ_PDCP_MODE_DL     1  
#define PJ_PDCP_MODE_UL     2

/* Direction defines */
#define PJ_DIR_UL        1     /*!< Unlink direction */
#define PJ_DIR_DL        2     /*!< Downlink direction */
#define PJ_DIR_BOTH      3     /*!< Both Downlink and Unlink */

#define PJ_DBG_SUB_MASK   DBGMASK_MI               /* Use for sub-mask */
#define PJ_DBGMASK_DETAIL (PJ_DBG_SUB_MASK << 0)   /* Parameters, It will give
                                                      in depth info */
#define PJ_DBGMASK_BRIEF  (PJ_DBG_SUB_MASK << 1)   /* Info, It will give info at
                                                      entry and exit places along
                                                      with certain state changes */
#define PJ_DBGMASK_ERROR  (PJ_DBG_SUB_MASK << 2)   /* Error information */
#define PJ_DBGMASK_FATAL  (PJ_DBG_SUB_MASK << 3)   /* FATAL errors like memory
                                                      resource failure etc., */


#define PJ_DBG_MDL_MASK   (PJ_DBG_SUB_MASK << 4) 

#define PJ_DBGMASK_DLM         (PJ_DBG_MDL_MASK << 0) /* DL */
#define PJ_DBGMASK_ULM         (PJ_DBG_MDL_MASK << 1) /* UL */
#define PJ_DBGMASK_UTL         (PJ_DBG_MDL_MASK << 2) /* DBM, UTL, TMR */
#define PJ_DBGMASK_CFG         (PJ_DBG_MDL_MASK << 3) /* LMM and CFG */
#define PJ_DBGMASK_LIB         (PJ_DBG_MDL_MASK << 4) /* LIB - means software
                                                               or offboard hooks */
#define PJ_DBGMASK_INF         (PJ_DBG_MDL_MASK << 5)
#define PJ_DBGMASK_LMM         (PJ_DBG_MDL_MASK << 6)
#define PJ_DBGMASK_DL         (PJ_DBG_MDL_MASK << 7)
#define PJ_DBGMASK_UL         (PJ_DBG_MDL_MASK << 8)


#ifdef TENB_T2K3K_SPECIFIC_CHANGES

#define ICC_POOL_ZERO_SIZE 384
#define ICC_POOL_ONE_SIZE  1920
#define ICC_POOL_TWO_SIZE  3968
#define ICC_POOL_THREE_SIZE  16256
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */

#ifdef DEBUGP
#define _pjPBuf   pjCb.init.prntBuf

#define PJ_PRNT(_cb,_prntbuf)      \
{                              \
   sprintf _prntbuf;           \
   SPrint(_cb->init.prntBuf);            \
}

#else  /* DEBUGP */ 
#define PJ_PRNT_HLINE(_pMsg)
#define PJ_PRNT(_cb,_prntbuf)
#define PJ_PRNT_TSTAMP
#define PJ_PRNT_MBUF(_mBufPtr)
#define PJ_PRNT_MEMINFO
#endif /* DEBUGP */

#ifdef DEBUGP
#define PJ_PRNT_HLINE(_pMsg)                                        \
{                                                                   \
   sprintf(_pjPBuf, "[PDCP_LAYER: %s:%d]::", __FILE__, __LINE__);  \
   SPrint(_pjPBuf);                                                 \
   sprintf(_pjPBuf, _pMsg);                                         \
   SPrint(_pjPBuf);                                                 \
}

#define PJDBGP(_cb,_mask, _arg)                               \
do                                                            \
{                                                             \
   if (!((_cb->init.dbgMask & _mask) ^ _mask))                \
   {                                                          \
      PJ_PRNT(_cb,_arg);                                          \
   }                                                          \
}while(0)
#else /* DEBUGP */
#define PJ_PJ_PRNT_HLINE(_pMsg)
#define PJDBGP(_cb,_mask, _arg) 
#endif /* DEBUGP */


 
/************************************************************************
 *                            SAP States
 ************************************************************************/
 
/* ############################PDCP DEFINES ############################# */

#define PJ_SAP_NOT_CFG        0     /*!< SAP Not Configured */
#define PJ_SAP_CFG            1     /*!< SAP Configured but not not bound */
#define PJ_SAP_BND            2     /*!< SAP Bound */
#define PJ_SAP_BINDING        3     /*!< SAP Bind initiated */
#define PJ_SAP_UBND           4     /*!< SAP Unbind */
#define PJ_MAX_SAP_BND_RETRY  3     /*!< Maximum SAP Bin Retries */

/* Times related */
#define PJ_RB_MAX_TIMERS       1       /* No of timers */
#define PJ_MAX_OBD_TMR         3       /* Number of OBD Timers */
#define PJ_MAX_DISC_TMR        1       /* Number of Discard Timers */
#define PJ_MAX_KWUSAP_TMR      1     /*!< Maximum number of KWU SAP Timers. */
#define PJ_EVT_WAIT_BNDCFM     0x01    /* Bind confirm timer */
#define PJ_MAX_UDXSAP_TMR      1     /*!< Maximum number of UDX SAP Timers. */
#define PJ_EVT_DL_OBD_TMR      0x11
#define PJ_EVT_UL_OBD_TMR      0x12
#define PJ_EVT_OBD_TMR         0x13
#define PJ_EVT_TMR_NONE        0x14
#define PJ_EVT_OBD_WAIT_TMR    0x15
#define PJ_EVT_UE_DEL_WAIT_TMR 0x16
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define PJ_EVT_L2_TMR          0x20
#define PJ_EVT_L2_MEAS_TMR     0x17
#endif /* LTE_L2_MEAS */
#define PJ_EVT_WAIT_KWU_BNDCFM 0x18    
#define PJ_EVT_WAIT_UDX_BNDCFM 0x19
#define PJ_DRB_7BIT_SN_HDR     0x0080
#define PJ_DRB_12BIT_SN_HDR    0x8000
#define PJ_DRB_15BIT_SN_HDR    0x8000 /*!< 15 bit SN setting the hdr for DRB */
#define PJ_ROHC_FEEDBACK_HDR   0x0010
#define PJ_FIRST_BIT           0x80

#define PJ_DATA_PDU            0x01
#define PJ_ROHC_FEEDBACK       0x02
#define PJ_STA_PDU             0x03

#define PJ_INTG_KEY_LEN        16
#define PJ_CIPHER_KEY_LEN      16

/*#define PJ_MAX_DAT_CFM         512 */
/* changed from 512 to 1024 */
#define PJ_MAX_DAT_CFM         1024

#define PJ_SDU_RECEIVED        0x01
#define PJ_WAIT_FOR_INTPROT    0x02
#define PJ_WAIT_FOR_CMP        0x03
#define PJ_WAIT_FOR_CIPHER     0x04
#define PJ_PDU_CONSTRUCTED     0x05
#define PJ_PDU_SUBMITTED       0x06
#define PJ_TX_ENT_TO_DEL       0x07
#define PJ_PDU_TO_DELETE       0x08

#define PJ_7_BIT_SN            7
#define PJ_12_BIT_SN           12
#define PJ_15_BIT_SN           15

#define PJ_TX_BUF_LEN          256
#define PJ_MAX_SDU_SIZE        8188

#define PJ_SRB                 PJ_SRB_SAP
#define PJ_DRB                 PJ_DRB_SAP

#define PJ_SRB_SAP             0
#define PJ_DRB_SAP             1

#define PJ_DRB_UM              2
#define PJ_DRB_AM              3

#define PJ_CFM_NOK             1
#define PJ_CFM_OK              0

#define PJ_GET_PJCB(_inst) pjCb[_inst];  
#define PJ_BYTE_LEN            8


#define PJ_SEC_DIR_UL          0
#define PJ_SEC_DIR_DL          1

#define PJ_PDU_CONSTRUCT_FAILED  1
#define PJ_HDR_COMP_FAILED       2
#define PJ_INTG_PROT_FAILED      3
#define PJ_CIPHER_FAILED         4
#define PJ_CPY_MSG_FAILED        5
#define PJ_REM_MSG_FAILED        6
#define PJ_DISC_TMR_EXP          7
#define PJ_OBD_TIMEOUT           8

#define PJ_CFG_RLC             1
#define PJ_CFG_PDCP            2
#define PJ_CFG_BOTH            3
#define PJ_TRANS_ID_LST_BKT_SIZE   10  

/* PJ_MS_KENB_LEN: Its the max lenght of the key for derivation of CK and IK */
/* PJ_MS_STR_S_LEN: Length of "STR S" which is one of the inputs for CK and IK dervation */ 
#define PJ_MS_KENB_LEN  32
#define PJ_MS_STR_S_LEN 9
#define PJ_NUM_BYTES_FRESH   4      /*<!Number of Bytes for FRESH variable */

#define PJ_KDF_SAP      0       /*!<Dummy SAP for KDF functionality*/


#define PJ_VALIDATE_RBID(_ueId, _rbId, _rbType, _ret)          \
{                                                              \
   _ret = ROK;                                                 \
   if (_ueId == 0)                                             \
   {                                                           \
      if (_rbId >= PJ_MAX_RB_PER_CELL)                         \
      {                                                        \
         _ret = RFAILED;                                       \
      }                                                        \
   }                                                           \
   else                                                        \
   {                                                           \
      if ((_rbType != CM_LTE_SRB) && (_rbType != CM_LTE_DRB))  \
      {                                                        \
         _ret = RFAILED;                                       \
      }                                                        \
      else if (_rbType == CM_LTE_SRB &&_rbId >= PJ_MAX_SRB_PER_UE)  \
      {                                                        \
         _ret = RFAILED;                                       \
      }                                                        \
      else if (_rbType == CM_LTE_DRB &&_rbId >= PJ_MAX_DRB_PER_UE)  \
      {                                                        \
         _ret = RFAILED;                                       \
      }                                                        \
   }                                                           \
}

#define PJ_DBM_FETCH_RBCB(_rbId, _rbType, _ueCb, _rbCb)     \
{                                                           \
  PjRbCb  **_rbCbLst;                                         \
   _rbCbLst = (_rbType==CM_LTE_SRB)?_ueCb->srbCb:_ueCb->drbCb;\
   if (_rbCbLst[_rbId] != NULLP)                            \
   {                                                        \
      _rbCb = _rbCbLst[_rbId];                              \
   }                                                        \
   else                                                     \
   {                                                        \
      _rbCb = NULLP;                                        \
   }                                                        \
}
#define PJ_DBM_FETCH_CELL_RBCB(_rbId, _rbCbLst, _rbCb)     \
{                                                           \
   if (_rbCbLst[_rbId] != NULLP)                            \
   {                                                        \
      _rbCb = _rbCbLst[_rbId];                              \
   }                                                        \
   else                                                     \
   {                                                        \
      _rbCb = NULLP;                                        \
   }                                                        \
}

#define PJ_GET_UL_UDX_SAP(_cb) (&(_cb->u.ulCb->udxUlSap[0])) 

/* LMM Module Macros */
#define PJ_LMM_RB_STS_INC(_cb)                                    \
{                                                              \
   _cb->pjGenSts.numOfRb++;                                      \
}

#define PJ_LMM_RB_STS_DEC(_cb)                                    \
{                                                              \
   _cb->pjGenSts.numOfRb--;                                      \
}




/* kw005.201 moved the definition from kw_pj_dlm.c file */
#define PJ_PJ_ASYNC_WIN        16
#ifdef LTE_L2_MEAS
/* L2 Measurement index to be used in rbCb to store measData */                                       
#define PJ_L2MEAS_UL_LOSS 0
#define PJ_L2CPU_PERCORE_STATS 1
#define PJ_L2MEM_PERPOOL_STATS 2
#define PJ_L2MEAS_ACT_UE        3
#define PJ_L2MEAS_DL_DELAY      4
#define PJ_L2MEAS_DL_DISC       5
#define PJ_MAX_L2MEAS_INRB      6
#define PJ_L2MEAS_UU_LOSS      7
#define PJ_L2MEAS_DL_IP        8
#define PJ_L2MEAS_UL_IP        9

#define PJ_L2_MAX_TIMERS        1

#define PJ_MAX_L2MEAS_EVT       10
#define PJ_INIT_RB_L2CB(_measVal, _measCb, _pjRbCb)                       \
{                                                                         \
   cmLListAdd2Tail(&_measCb->rbList, &_pjRbCb->rbL2Cb.listNode[_measVal]);\
   _pjRbCb->rbL2Cb.l2Sts[_measVal] = &_measCb->measData[_pjRbCb->qci];    \
}

#define PJ_RMV_RB_FRM_L2LIST(_measVal, _measCb, _pjRbCb)          \
cmLListDelFrm(&_measCb->rbList,                                    \
     &_pjRbCb->rbL2Cb.listNode[_measVal]);

#define PJ_UPD_L2_DLDELAY_STS(_pjRbCb, _val)                      \
{                                                                 \
   _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DELAY]->dlDelay.numPkts++;          \
   _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DELAY]->dlDelay.val += _val;        \
}                                          
#if 0
#define PJ_UPD_L2_ULLOSS_PKT_STS(_pjRbCb)                         \
{                                                                 \
   if(_pjRbCb->rbL2Cb.measOn & LPJ_L2MEAS_UL_LOSS)                \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->ulLoss.numPkts++;     \
}                                              

#define PJ_UPD_L2_ULLOSS_STS(_pjRbCb)                             \
{                                                                 \
   if(_pjRbCb->rbL2Cb.measOn & LPJ_L2MEAS_UL_LOSS)                \
   {                                                              \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->ulLoss.numPkts++;     \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->ulLoss.val++;      \
   }                                                              \
}                                              
#endif
#define PJ_UPD_L2_DLDISC_PKT_STS(_pjCb, _pjRbCb)                         \
{                                                                 \
   if((_pjRbCb->rbType == PJ_DRB) &&                              \
      ((_pjCb)->u.dlCb->pjL2Cb.measOn[_pjRbCb->qci] & LPJ_L2MEAS_DL_DISC))   \
   {                                                              \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DISC]->dlDisc.numPkts++;     \
   }                                                              \
}                                              

#define PJ_UPD_L2_DLDISC_STS(_pjCb, _pjRbCb)                             \
{                                                                 \
   if((_pjRbCb->rbType == PJ_DRB) &&                              \
      ((_pjCb)->u.dlCb->pjL2Cb.measOn[_pjRbCb->qci] & LPJ_L2MEAS_DL_DISC))   \
   {                                                              \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DISC]->dlDisc.val++;      \
   }                                                              \
}                                      

#define PJ_SEND_SAPID_ALARM(_cb,_sapId, _evnt, _cause) \
{ \
   pjLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, _sapId, 0, 0); \
}
#define PJ_SEND_UEID_ALARM(_ueId, _qci, _evnt, _cause) \
{ \
   pjLmmSendAlarm(LCM_CATEGORY_INTERFACE, _evnt, _cause, 0, _ueId, _qci); \
}

#define PJ_UPD_L2_DECR_PER_QCI_RB_COUNT(_pjCb, _pjRbCb)                 \
{                                                                 \
   if(((_pjRbCb)->rbType == CM_LTE_DRB) &&                    \
      ((_pjCb)->u.dlCb->pjL2Cb.measOn[_pjRbCb->qci] & LPJ_L2MEAS_DL_DISC))   \
   {                                                              \
      U32 idx1;                                                    \
      for (idx1 = 0; idx1 < LPJ_MAX_L2MEAS; idx1++)                  \
      {                                                           \
         if(_pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.measType & LPJ_L2MEAS_DL_DISC)                 \
         {                                                        \
            if(_pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.measData[(_pjRbCb)->qci].totDrbsPerQci > 0) \
            {                                                                                               \
               _pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.measData[(_pjRbCb)->qci].totDrbsPerQci--;   \
               if (_pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.measData[(_pjRbCb)->qci].totDrbsPerQci == 0) \
               {                                                                                                 \
                  _pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.qci[(_pjRbCb)->qci] = 0;                      \
                  cmMemset((U8 *)&_pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.measData[(_pjRbCb)->qci], 0,  \
                     sizeof(_pjCb->u.dlCb->pjL2Cb.pjL2EvtCb[idx1].measCb.measData[(_pjRbCb)->qci]));            \
               }                                                                                                 \
            }                                                                                                    \
         }                                                                                                       \
      }                                                                                                          \
   }                                                                                                             \
}

#define PJ_GET_SN_DIFF(_expSn, _rcvdSn, _snLen, _diff)  \
{                                                       \
   if(_expSn > _rcvdSn)                                 \
   {                                                    \
      _diff = _rcvdSn + (1 << _snLen) - _expSn;         \
   }                                                    \
   else                                                 \
   {                                                    \
      _diff = _rcvdSn - _expSn;                         \
   }                                                    \
}

#define PJ_UPD_L2_ULLOSS_CNTRS(_cb,_count, _pjRbCb)                                  \
{                                                                                \
   PjSn _sn;                                                                     \
   U32 _hfn;                                                                     \
   PJ_GET_SN_HFN(_count, _pjRbCb->snLen, _sn, _hfn)                              \
   if(_cb->u.ulCb->pjL2Cb.measOn[_pjRbCb->qci] & LPJ_L2MEAS_UL_LOSS)                               \
   {                                                                             \
       U32 _diff;                                                                \
       PJ_GET_SN_DIFF(_pjRbCb->ulCb.nxtSubSn, _sn, _pjRbCb->snLen, _diff)        \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->ulLoss.numPkts += _diff + 1;     \
      _pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]->ulLoss.val += _diff;             \
   }                                                                             \
   _pjRbCb->ulCb.nxtSubSn = (_sn + 1) % (1 << _pjRbCb->snLen);                   \
}

#else /* LTE_L2_MEAS */
#define PJ_UPD_L2_DLDELAY_STS(_pjRbCb, _val)
#define PJ_UPD_L2_ULLOSS_PKT_STS(_pjRbCb) 
#define PJ_UPD_L2_ULLOSS_STS(_pjRbCb)
#define PJ_UPD_L2_DLDISC_PKT_STS(_pjCb,_pjRbCb)  
#define PJ_UPD_L2_DLDISC_STS(_pjCb,_pjRbCb)      
#define PJ_SEND_SAPID_ALARM(_cb,_sapId, _evnt, _cause)
#define PJ_SEND_UEID_ALARM(_ueId, _qci, _evnt, _cause)
#define PJ_UPD_L2_DECR_PER_QCI_RB_COUNT(_pjCb, _pjRbCb)
#define PJ_UPD_L2_ULLOSS_CNTRS(_cb, _count, _pjRbCb)
#endif /* LTE_L2_MEAS */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS 
#if (ERRCLASS & ERRCLS_INT_PAR)
#define PJ_VALDATE_SAP(_cb, _chkSpId, _sap, _ret)                    \
{                                                                    \
   if (_chkSpId != _sap->spId)                                       \
   {                                                                 \
      RLOG0(L_ERROR,"Sap Id Validation Failed.");                    \
      _ret = RFAILED;                                                \
   }                                                                 \
   /* SAP state validation */                                        \
   if(_sap->state != PJ_SAP_BND)                                     \
   {                                                                 \
      RLOG0(L_ERROR,"Sap State Invalid.");                           \
      pjLmmSendAlarm(_cb, LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,   \
            LCM_CAUSE_INV_STATE, 0, 0, 0);                           \
      _ret = RFAILED;                                                \
   }                                                                 \
}
#else /* ERRCLASS & ERRCLS_INT_PAR */
#define PJ_VALDATE_SAP(_cb, _chkSpId, _sap, _ret)                    \
{                                                                    \
   /* SAP state validation */                                        \
   if(_sap->state != PJ_SAP_BND)                                     \
   {                                                                 \
      RLOG0(L_ERROR,"Sap State Invalid.");                            \
      pjLmmSendAlarm(_cb, LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,   \
            LCM_CAUSE_INV_STATE, 0, 0, 0);                           \
      _ret = RFAILED;                                                \
   }                                                                 \
}
#endif /* ERRCLASS & ERRCLS_INT_PAR */                              
#else /* LTE_L2_MEAS */
#if (ERRCLASS & ERRCLS_INT_PAR)
#define PJ_VALDATE_SAP(_cb, _chkSpId, _sap, _ret)                    \
{                                                                    \
   if (_chkSpId != _sap->spId)                                       \
   {                                                                 \
      RLOG0(L_ERROR,"Sap Id Validation Failed.");                     \
      _ret = RFAILED;                                                \
   }                                                                 \
   /* SAP state validation */                                        \
   if(_sap->state != PJ_SAP_BND)                                     \
   {                                                                 \
      RLOG0(L_ERROR,"Sap State Invalid.");                            \
      pjLmmSendAlarm(_cb, LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,   \
            LCM_CAUSE_INV_STATE, 0, 0);                              \
      _ret = RFAILED;                                                \
   }                                                                 \
}
#else /* ERRCLASS & ERRCLS_INT_PAR */
#define PJ_VALDATE_SAP(_cb, _chkSpId, _sap, _ret)                         \
{                                                                    \
   /* SAP state validation */                                        \
   if(_sap->state != PJ_SAP_BND)                                     \
   {                                                                 \
      RLOG0(L_ERROR,"Sap State Invalid.");                            \
      pjLmmSendAlarm(_cb, LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,   \
            LCM_CAUSE_INV_STATE, 0, 0);                              \
      _ret = RFAILED;                                                \
   }                                                                 \
}
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#endif/* LTE_L2_MEAS */

#ifdef FLAT_BUFFER_OPT
#define PJ_FREE_FLAT_BUF(_cb, _buf) \
{ \
   if(((_buf)->len != 0) && ((_buf)->startAddr != NULLP))\
   {\
      (Void) SPutStaticBuffer(_cb->init.region, _cb->init.pool,   \
            (Data *) (_buf)->startAddr, (Size)((_buf)->ptr - (_buf)->startAddr + (_buf)->len) , 0);                 \
   }\
}
#endif

#define PJ_FREE_SHRABL_BUF(_cb,_buf, _size)                          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutStaticBuffer(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define PJ_ALLOC_SHRABL_BUF(_cb,_buf, _size)                                    \
{                                                                    \
 if (SGetStaticBuffer(_cb->init.region, _cb->init.pool, (Data **)&_buf,      \
                (Size) _size, 0) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define PJ_ALLOC_SHRABL_BUF_WM(_cb,_buf, _size)                                    \
{                                                                    \
 SGetStaticBuffer(_cb->init.region, _cb->init.pool, (Data **)&_buf,      \
                (Size) _size, 0);                                \
}


#define PJ_FREE_SHRABL_BUF_PST(_region, _pool, _buf, _size)                          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutStaticBuffer(_region, _pool,   \
            (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define PJ_ALLOC_SHRABL_BUF_PST(_region, _pool, _buf, _size)                                    \
{                                                                    \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,      \
                (Size) _size, 0);                                \
}

#define PJ_PJDBM_CREATE_RBCB(_rbCb,_rbCbLst, _rbId, _rbType)    \
{                                                               \
    /* Allocate memory for rbCb */                              \
   PJ_ALLOC(_rbCb, sizeof(RbCb));                               \
                                                                \
   if ( rbCb != NULLP)                                          \
   {                                                            \
      _rbCbLst[_rbId] = (_rbCb);                                \
      _rbCb->kwRbCb.rlcId.rbId = _rbId;                         \
   }                                                            \
}                                                               

/* Macros for configuration module */
#define PJ_CFG_FILL_CFG_CFM(_entCfm, _rbId, _rbType, _status, _reason)   \
{                                                                        \
   _entCfm->rbId  = _rbId;                                               \
   _entCfm->rbType  = _rbType;                                           \
   _entCfm->status = _status;                                            \
   _entCfm->reason = _reason;                                            \
}

#define PJ_CFG_FILL_SECCFG_CFM(_entCfm, _ueId, _cellId, _transId, _status, _reason)  \
{                                                               \
   _entCfm->ueId   = _ueId;                                     \
   _entCfm->cellId = _cellId;                                   \
   _entCfm->transId = _transId;                                 \
   _entCfm->status = _status;                                   \
   _entCfm->reason = _reason;                                   \
}

#define PJ_CFG_FILL_SDUSTA_CFM(_entCfm, _ueId, _cellId, _transId, _status, _reason)  \
{                                                               \
   _entCfm->ueId   = _ueId;                                     \
   _entCfm->cellId = _cellId;                                   \
   _entCfm->transId = _transId;                                 \
   _entCfm->status = _status;                                   \
   _entCfm->reason = _reason;                                   \
}

#define PJ_CFG_FILL_REESTREQ_CFM(_entCfm, _ueId, _cellId, _transId, _status)  \
{                                                               \
   _entCfm->ueId   = _ueId;                                     \
   _entCfm->cellId = _cellId;                                   \
   _entCfm->transId = _transId;                                 \
   _entCfm->status = _status;                                   \
}

#define PJ_UPD_COUNT(_pjRbCb, _sn, _txHfn, _count)         \
{                                                          \
   _count = _txHfn << (_pjRbCb->snLen);                    \
   _count |= _sn;                                       \
} 
#define PJ_ADD_PST_MSG(_pdu, _hdr, _size, _ret)            \
{                                                          \
   Data _liByte;                                           \
   S16  _bytes;                                            \
   _bytes = _size - 1;                                     \
   while (_bytes >= 0 )                                    \
   {                                                       \
      _liByte = (U8) (_hdr >> (8*_bytes));                 \
      _ret = SAddPstMsg((Data )_liByte, _pdu);             \
      if ( _ret != ROK )                                   \
      {                                                    \
         break;                                            \
      }                                                    \
      _bytes--;                                            \
   }                                                       \
}

#define PJ_RESET_ALL_VAR(_pjRbCb)                          \
{                                                          \
   _pjRbCb->dlCb.nxtTxSn  = 0;                             \
   _pjRbCb->dlCb.txHfn    = 0;                             \
   _pjRbCb->dlCb.cfmExp   = 0;                             \
   _pjRbCb->dlCb.count    = 0;                             \
}

#define PJ_ALLOC(_cb, _buf, _size)                         \
{                                                          \
 if (SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf,\
                (Size) _size) == ROK)                      \
   {                                                       \
      cmMemset((U8 *)(_buf), 0, _size);                    \
   }                                                       \
   else                                                    \
   {                                                       \
      (_buf) = NULLP;                                      \
   }                                                       \
}

#define PJ_ALLOC_WC(_cb, _buf, _size)\
{ \
 if (SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf,\
                (Size) _size) != ROK)                      \
   {                                                       \
      (_buf) = NULLP;                                      \
   }                                                       \
}


#define PJ_FREE_BUF(_buf)                                  \
{                                                          \
   if (_buf != NULLP)                                      \
   {                                                       \
      SPutMsg(_buf);                                       \
   }                                                       \
   _buf = NULLP;                                           \
}

#define PJ_FREE(_cb, _buf, _size)                               \
{                                                          \
   if (_buf != NULLP)                                      \
   {                                                       \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,    \
                         (Data *) _buf, (Size) _size);     \
      _buf = NULLP;                                        \
   }                                                       \
}     

#define PJ_PST_FREE(_region, _pool, _buf, _size)           \
{                                                          \
   if (_buf != NULLP)                                      \
   {                                                       \
      (Void) SPutSBuf(_region, _pool,                      \
                      (Data *) _buf, (Size) _size);        \
      _buf = NULLP;                                        \
   }                                                       \
}     
                                                                                        
#define PJ_MEM_CPY(_dst, _src, _size)                      \
{                                                          \
   cmMemcpy((U8*)_dst, (U8 *)_src, _size);                 \
}

#define PJ_DLM_UPD_VAR(_pjRbCb, _sn, _txHfn)               \
{                                                          \
   PjDlCb    *_dlCb;                                       \
                                                           \
   _dlCb   = &_pjRbCb->dlCb;                                \
   _sn    = _dlCb->nxtTxSn;                                 \
   _txHfn = _dlCb->txHfn;                                   \
   if ( (++_dlCb->nxtTxSn) ==  _pjRbCb->maxTxSn)                 \
   {                                                       \
      _dlCb->txHfn++;                                       \
      _dlCb->nxtTxSn = 0;                                   \
   }                                                       \
}

#define PJ_FILL_CFM_OK(_cb,_pjRbCb, _fms, _datCfm)             \
{                                                          \
   U32     _startCount = _pjRbCb->dlCb.cfmExp;             \
   U32     _stopCount  = _pjRbCb->dlCb.cfmExp;             \
   PjTxEnt *_ent;                                          \
   _ent = (PjTxEnt *)pjDbmGetTxEntSn(_cb, &(_pjRbCb->dlCb.txBuf), _fms); \
   if ( _ent != NULLP )                                    \
   {                                                       \
      _stopCount = _ent->count;                            \
      _pjRbCb->dlCb.cfmExp = _ent->count;                  \
      _pjRbCb->dlCb.nxtToSub = _ent->count;                \
   }                                                       \
   while ( _startCount < _stopCount )                      \
   {                                                       \
      _ent = (PjTxEnt *)pjDbmGetTxEnt(_cb, &(_pjRbCb->dlCb.txBuf), _startCount);\
      if ( _ent != NULLP )                                 \
      {                                                    \
         if ( _pjRbCb->dlCb.cfmReqd)                       \
         {                                                 \
            _datCfm->cfmSta[_datCfm->numSdus].sduId  = _ent->sduId;\
            _datCfm->cfmSta[_datCfm->numSdus].status = PJ_CFM_OK;\
            _datCfm->numSdus++;                            \
         }                                                 \
         pjDbmDelTxEnt(_cb, &(pjRbCb->dlCb.txBuf), _ent->count);\
      }                                                    \
      _startCount++;                                       \
   }                                                       \
}

#define PJ_UPD_CFM_EXP(_cb,_dlCb)                              \
{                                                          \
   U32 _curCount = _dlCb->count;                            \
   U32 _count    = _dlCb->cfmExp;                           \
   PjTxEnt *_txEnt = NULLP;                                \
   while ( _count <= _curCount )                           \
   {                                                       \
      _txEnt = (PjTxEnt *)pjDbmGetTxEnt(_cb,&(_dlCb->txBuf), _count);\
      if ( _txEnt != NULLP )                               \
      {                                                    \
         break;                                            \
      }                                                    \
      _count++;                                            \
   }                                                       \
   _dlCb->cfmExp = _count;                                  \
}

#define PJ_UPD_DL_VAR(_cb, _pjRbCb, _curCnt)                    \
{                                                          \
   U32   _count = _curCnt + 1;                             \
   if ( _pjRbCb->dlCb.nxtToSub == _curCnt )                \
   {                                                       \
      _pjRbCb->dlCb.nxtToSub = _count;                     \
   }                                                       \
   if ( _pjRbCb->dlCb.cfmExp  == _curCnt )                 \
   {                                                       \
      _pjRbCb->dlCb.cfmExp = _count;                       \
   }                                                       \
}

#define PJ_CREATE_NEW_LIST(_cb,_pjRbCb, _tmpTxBuf, _ret)       \
{                                                          \
   _tmpTxBuf.datQ  = _pjRbCb->dlCb.txBuf.datQ;             \
   _tmpTxBuf.numEntries =  pjRbCb->dlCb.txBuf.numEntries;  \
   _tmpTxBuf.numBins =  pjRbCb->dlCb.txBuf.numBins;        \
   _tmpTxBuf.sduSubmitQ = pjRbCb->dlCb.txBuf.sduSubmitQ;   \
   pjDbmDlBufInit(_cb,&pjRbCb->dlCb.txBuf,_cb->pjGenCfg.nmbDlBins);\
   if ( _pjRbCb->dlCb.txBuf.datQ == NULLP )                \
   {                                                       \
      RLOG0(L_ERROR,"PJ_CREATE_NEW_LIST: PJ_ALLOC Failed.");          \
      _ret = RFAILED;                                      \
   }                                                       \
}

#define PJ_SND_ROHC_FDB(_cb,_pjRbCb, _rohcFdbPdu, _ret)        \
{                                                          \
   Data _hdr = 0;                                          \
   U32  _count = 0;                                        \
   _hdr |= PJ_ROHC_FEEDBACK_HDR;                           \
   _ret = SAddPreMsg((Data) _hdr, _rohcFdbPdu);            \
   if ( _ret != ROK )                                      \
   {                                                       \
      RLOG0(L_ERROR,"PJ_SND__ROHC_FDB: SAddPreMsg failed for pdu ");\
      PJ_FREE_BUF(_rohcFdbPdu);                            \
      _ret  =  RFAILED;                                    \
   }                                                       \
   if ( _pjRbCb->mode == PJ_DRB_UM )                       \
   {                                                       \
      _count = _pjRbCb->dlCb.nxtToSub - 1;                 \
   }                                                       \
   else                                                    \
   {                                                       \
      _count = _pjRbCb->dlCb.cfmExp - 1;                   \
   }                                                       \
   if ( _ret == ROK )                                      \
   {                                                       \
      pjDlmSendDatReq(_cb, _pjRbCb, _count, _rohcFdbPdu);           \
   }                                                       \
}

#define PJ_MAX_HASH_BINS 255

/* Defines used in gp_pj_dbm.c */
#define PJ_HASH_FN(_buf, _cnt) (_cnt % PJ_MAX_HASH_BINS)

/* Defines used in gp_pj_ulm.c */
#define MAX_BMAP_SIZE         2048
#define PJ_DRB_AM_WINSIZE(_snLen) (1 << (_snLen - 1))     
#define PJ_U16_BIT_SIZE       16
#define PJ_BYTE_SIZE          8
#define PJ_SRB_SN_LEN         5
#define PJ_DRBAM_SN_LEN       12
#define PJ_TYPE_DRB_DATA      0x80
#define PJ_TYPE_STATUS_REPORT  0x00
#define PJ_TYPE_ROHC_FEEDBACK 0x01
#define PJ_PDU_BIT_MASK       0x70
#define PJ_12_BIT_SN_MSB_MASK 0x0F
#define PJ_15_BIT_SN_MSB_MASK 0x7F
#define PJ_CPDU_RES_VAL_MASK       0x0F
#define PJ_MACI_LEN           4

#define PJ_RDY_TO_DCIPHER      1
#define PJ_SENT_TO_DCIPHER     2
#define PJ_RDY_TO_INTVER       3
#define PJ_SENT_TO_INTVER      4
#define PJ_RDY_TO_DCOMP        5
#define PJ_SENT_TO_DCOMP       6
#define PJ_RDY_TO_SUBMIT       7
#define PJ_RX_ENT_TO_DEL       8
#define PJ_RX_INTVER_FAIL      9

#define PJ_STATE_NORMAL       1
#define PJ_STATE_REEST        2
#define PJ_STATE_REEST_HO     3
#define PJ_STATE_HO           4
#define PJ_TMR_ULM_OBD        10

/* Defines used in gp_pj_utl.c */
#define PJ_LIB_COMP_BIT_MASK     0x00000001
#define PJ_LIB_INT_BIT_MASK      0x00000002
#define PJ_LIB_CP_CIP_BIT_MASK   0x00000004
#define PJ_LIB_UP_CIP_BIT_MASK   0x00000008

#define PJ_MAX_SDU_IN_DAT_FWD_IND      5
/* Defines used for multi cfg confirms */
#define PJ_MAX_ASYNC_CFM   2

#define PJ_SEC_ASYNC_CFM         0x01
#define PJ_CFG_ASYNC_CFM         0x02
#define PJ_CFG_REEST_ASYNC_CFM   0x04
#define PJ_CFG_UEDEL_ASYNC_CFM   0x08
#define PJ_REEST_ASYNC_CFM       0x10

#define  PJ_SEC_INIT_CFM   1
#define  PJ_CMP_INIT_CFM   2

#define PJ_GET_MEM_REGION(_cb) (_cb->init.region)

#define PJ_GET_MEM_POOL(_cb) (_cb->init.pool)

#define PJ_GET_MEM_POOL_ADDRESS(_cb) (&_cb->init.pool)

#define PJ_GET_DBG_MASK(_cb) (_cb->init.dbgMask)

/* Memset to value */
#define PJ_MEM_SET(_arg, _val, _size) \
{ \
  cmMemset((U8 *)_arg, (U8)_val, _size); \
}

/* Set the unsolictated Status flag */
#define PJ_SET_USTA_FLAG(_cb, _value) \
{ \
   _cb->init.usta = _value; \
}

#define PJ_DATA_FWD_PKT   1
#define PJ_DATA_NRM_PKT   2

#define PJ_CHK_RESTART_OBD_TIMER(_cb,_ueCb, _cfgTxId)                    \
{                                                                             \
   U16 _idx;                                                                  \
   if(_ueCb->libInfo.crntTmrTxId == _cfgTxId)                                 \
   {                                                                          \
      pjStopTmr(_cb,(PTR)_ueCb, PJ_EVT_OBD_WAIT_TMR);                         \
   }                                                                          \
                                                                              \
   if(pjChkTmr(_cb,(PTR)_ueCb, PJ_EVT_OBD_WAIT_TMR) == FALSE)                 \
   {                                                                          \
      for(_idx = 0; _idx < (PJ_MAX_ASYNC_CFM - 1) ; _idx++)                   \
      {                                                                       \
         _ueCb->libInfo.crntTmrTxId =                                         \
               (U16)((_ueCb->libInfo.crntTmrTxId + 1) % PJ_MAX_ASYNC_CFM);          \
         if((_ueCb->libInfo.asyncCfm[_ueCb->libInfo.crntTmrTxId] != NULLP) && \
            (_ueCb->libInfo.asyncCfm[_ueCb->libInfo.crntTmrTxId]->startTmr    \
                                                      == TRUE))               \
         {                                                                    \
            pjStartTmr(_cb,(PTR)_ueCb, PJ_EVT_OBD_WAIT_TMR);                 \
            break;                                                            \
         }                                                                    \
      }                                                                       \
   }                                                                          \
}                                                                             \

#define PJ_CLEAN_AND_UPD_ASYNCINFO(_cb,_ueCb, _txIdx)                 \
{                                                                 \
   PJ_FREE(_cb,_ueCb->libInfo.asyncCfm[_txIdx], sizeof(PjAsyncCfm));  \
   _ueCb->libInfo.asyncCfm[_txIdx]=NULLP;                         \
   if(_ueCb->libInfo.nxtAvlbTxId  == PJ_MAX_ASYNC_CFM)            \
   {                                                              \
      _ueCb->libInfo.nxtAvlbTxId  = _txIdx;                       \
   }                                                              \
}                                                                 \

#define PJ_FILL_TIMEOUT_CFG_CFM_INFO(_cb,_cfgCfm, _asyncCfm)               \
{                                                                            \
      U16 _idx;                                                              \
      PJ_ALLOC(_cb,_cfgCfm, sizeof(CpjCfgCfmInfo));                         \
      if (_cfgCfm != NULLP)                                                  \
      {                                                                      \
         for ( _idx = 0; _idx < _asyncCfm->numEnt ; _idx++)                  \
         {                                                                   \
            if ( _asyncCfm->cmpInitBitMask &                                 \
                           (1 << (_asyncCfm->cfmEnt[_idx].rbId - 1)))        \
            {                                                                \
               _cfgCfm->cfmEnt[_idx].status =  CPJ_CFG_CFM_NOK;              \
               _cfgCfm->cfmEnt[_idx].reason = CPJ_CFG_REAS_OBD_TIMEOUT;      \
            }                                                                \
            else                                                             \
            {                                                                \
               _cfgCfm->cfmEnt[_idx].status = _asyncCfm->cfmEnt[_idx].status;\
               _cfgCfm->cfmEnt[_idx].reason = _asyncCfm->cfmEnt[_idx].reason;\
            }                                                                \
            _cfgCfm->cfmEnt[_idx].rbId   = _asyncCfm->cfmEnt[_idx].rbId;     \
            _cfgCfm->cfmEnt[_idx].rbType = _asyncCfm->cfmEnt[_idx].rbType;   \
         }                                                                   \
         _cfgCfm->ueId    = _asyncCfm->ueId;                                 \
         _cfgCfm->cellId  = _asyncCfm->cellId;                               \
         _cfgCfm->transId = _asyncCfm->transId;                              \
         _cfgCfm->numEnt  = _asyncCfm->numEnt;                               \
      }                                                                      \
}                                                                            \

#ifdef PJ_SEC_ASYNC
/* In the case when all the elements in the reception buffer have already
 * been sent to the dcomp unit or sent up, our nxtSubDcomp will be pointing to 
 * NEXT_PDCP_RX_SN. So if there is a jump, this has to be updated accordingly */
#define PJ_ULM_UPD_NXT2DCOMP(_ulCb, _snLen, _sn, _oldRxCnt)        \
{                                                                  \
   if(_oldRxCnt == _ulCb->nxtSubDeCmp)                             \
   {                                                               \
      PJ_GET_COUNT(_ulCb->nxtSubCnt, _snLen, _sn, _ulCb->rxHfn);   \
   }                                                               \
}
#else  /* !PJ_SEC_ASYNC */
#define PJ_ULM_UPD_NXT2DCOMP(_ulCb, _snLen, _sn, _oldRxCnt) 
#endif /* PJ_SEC_ASYNC */

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
/* There can be a case where the first PDU that you receive *
 * does not match with the NEXT_PDCP_RX_SN. If nxtSubCnt is also
 * pointing to NETX_PDCP_RX_SN, it has to be updated. */
#define PJ_ULM_UPD_NXT2SUB(_ulCb, _cnt)                      \
{                                                            \
   if(_ulCb->recBuf.numEntries == 0)                         \
   {                                                         \
      _ulCb->nxtSubCnt = _cnt;                               \
      _ulCb->nxtSubDeCmp = _cnt;                             \
   }                                                         \
}
#define PJ_INC_OBD_COUNT(_pjRbCb, _count)                                \
{                                                                        \
   if(_pjRbCb->state == PJ_STATE_REEST ||                                \
         _pjRbCb->state == PJ_STATE_REEST_HO)                            \
   {                                                                     \
      _pjRbCb->ulCb.obdCnt ++;                                           \
   }                                                                     \
}

/* Notes: This will be called only for DRBs */
#define PJ_DEC_OBD_COUNT(_cb, _pjRbCb, _count)                                \
{                                                                        \
   if(_pjRbCb->state == PJ_STATE_REEST ||                                \
         _pjRbCb->state == PJ_STATE_REEST_HO)                            \
   {                                                                     \
      if((_count >= _pjRbCb->ulCb.firstReEstCnt)&&                       \
         (_pjRbCb->mode = PJ_DRB_AM))                                    \
      {                                                                  \
         _pjRbCb->ulCb.obdCnt --;                                        \
      }                                                                  \
   }                                                                     \
}
#else /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
/* Dummy defines */
#define PJ_ULM_UPD_NXT2SUB(_ulCb, _cnt)    
#define PJ_INC_OBD_COUNT(_pjRbCb, _count)  
#define PJ_DEC_OBD_COUNT(_cb,_pjRbCb, _count)  
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

#define PJ_ULM_UPD_RX_VAR(_sn, _pjRbCb)                          \
{                                                                \
   if(_sn < _pjRbCb->ulCb.nxtRxSn)                               \
   {                                                             \
      _pjRbCb->ulCb.rxHfn++;                                     \
   }                                                             \
                                                                 \
   _pjRbCb->ulCb.nxtRxSn = (U16)((_sn + 1) % (1 << _pjRbCb->snLen));    \
   if(_pjRbCb->ulCb.nxtRxSn == 0)                                \
   {                                                             \
      _pjRbCb->ulCb.rxHfn ++;                                    \
   }                                                             \
}

#define PJ_INC_SN(_sn, _snLen) ((++_sn) % (1 << _snLen))                      \

#define PJ_GET_SN_HFN(_count, _snLen, _sn, _hfn)             \
{                                                            \
   _sn   = (U16)(_count % (1 << _snLen));                          \
   _hfn  =  (_count >> _snLen);                              \
}                                                            \

#define PJ_GET_SN(_count, _snLen, _sn)  (_sn   =  _count % (1 << _snLen));

#define PJ_ULM_REEST_DRB_AM(_pjRbCb)                         \
{                                                            \
   _pjRbCb->frstReEstCnt =  _pjRbCb->lastSubCnt + 1;         \
   _pjRbCb->obdCnt  =  0;                                    \
                                                             \
}                                                            

#define PJ_ULM_UPD_HFN(_sn, _pjRbCb, _ulCb, _hfn)              \
{                                                              \
   if(_sn < _pjRbCb->ulCb.nxtRxSn)                             \
   {                                                           \
      _hfn  = (_ulCb->rxHfn + 1 );                             \
   }                                                           \
   else                                                        \
   {                                                           \
      _hfn  = _ulCb->rxHfn;                                    \
   }                                                           \
}

#define PJ_ULM_SRB_UPD_COUNT(_sn, _pjRbCb, _ulCb, _count)      \
{                                                              \
   if(_sn < _ulCb->nxtRxSn)                                    \
   {                                                           \
      _count  = ((_ulCb->rxHfn + 1 ) << _pjRbCb->snLen) | _sn; \
   }                                                           \
   else                                                        \
   {                                                           \
      _count  = (_ulCb->rxHfn  << _pjRbCb->snLen) | _sn;       \
   }                                                           \
}

/* Check for wrap around */                                      
#define PJ_ULM_UPD_RXHFN(_ulCb, _sn)                             \
{                                                                \
   if(_sn < _ulCb->nxtRxSn)  ulCb->rxHfn ++;                     \
}
/* kw005.201,replaced macro with function */
#define PJ_SND_PJU_STA_IND(_cb,_pjRbCb, _rxEnt)              \
{                                                        \
   CmLtePdcpId  _pdcpId;                                \
   Buffer   *_mBuf = NULLP;                              \
   PjuStaIndInfo  _staInd;                              \
   PjPjuSapCb  *_pjuSap;                                 \
                                                         \
   if (_pjRbCb->rbType == PJ_SRB)                        \
      _pjuSap   = &(_cb->u.ulCb->pjuSap[PJ_SRB_SAP]);                \
   else                                                  \
      _pjuSap   = &(_cb->u.ulCb->pjuSap[PJ_DRB_SAP]); \
                                                                     \
                                                         \
                                                         \
      _pdcpId.ueId   =  _pjRbCb->ueCb->key.ueId;        \
      _pdcpId.cellId =  _pjRbCb->ueCb->key.cellId;      \
      _pdcpId.rbId   =  _pjRbCb->rbId;                  \
      _pdcpId.rbType =  _pjRbCb->rbType;                \
                                                         \
      switch (_rxEnt->state)                             \
      {                                                  \
         case PJ_RDY_TO_DCIPHER:                         \
         case PJ_SENT_TO_DCIPHER:                        \
         {                                               \
            _staInd.cause  =  PJU_DCIP_FAILED;          \
            break;                                       \
         }                                               \
         case PJ_RDY_TO_INTVER:                          \
         case PJ_SENT_TO_INTVER:                         \
         {                                               \
            _staInd.cause  =  PJU_INTVER_FAILED;        \
               _mBuf          =  _rxEnt->mBuf;           \
               _rxEnt->mBuf    =  NULLP;                 \
            break;                                       \
         }                                               \
         case PJ_RDY_TO_DCOMP:                           \
         case PJ_SENT_TO_DCOMP:                          \
         {                                               \
            _staInd.cause  =  PJU_DCOMP_FAILED;         \
            if(_pjRbCb->ulCb.discReqd == FALSE)          \
            {                                            \
               _mBuf          =  _rxEnt->mBuf;           \
               _rxEnt->mBuf    =  NULLP;                 \
            }                                            \
            break;                                       \
         }                                               \
         default:                                        \
            break;                                       \
      }                                                  \
      PjUiPjuStaInd(&(_pjuSap->pst), _pjuSap->suId,      \
                    &_pdcpId, &_staInd, _mBuf);            \
}



#define PJ_SND_PJU_DAT_IND(_cb,_pjRbCb, _rxEnt)         \
{                                                   \
   CmLtePdcpId  _pdcpId;                   \
   Buffer        *_mBuf;                            \
   PjPjuSapCb  *_pjuSap;                            \
                                                    \
   if (_pjRbCb->rbType == PJ_SRB)                   \
   {                                                \
      _pjuSap   = &(_cb->u.ulCb->pjuSap[PJ_SRB_SAP]);       \
   }                                                \
   else                                             \
   {                                                \
      PJ_UPD_L2_ULLOSS_CNTRS(_cb,_rxEnt->count, _pjRbCb)        \
      _pjuSap   = &(_cb->u.ulCb->pjuSap[PJ_DRB_SAP]);       \
   }						    \
                                                    \
      _pdcpId.ueId   =  _pjRbCb->ueCb->key.ueId;   \
      _pdcpId.cellId =  _pjRbCb->ueCb->key.cellId; \
      _pdcpId.rbType =  _pjRbCb->rbType;           \
      _pdcpId.rbId   =  _pjRbCb->rbId;             \
      _mBuf       =  _rxEnt->mBuf;                  \
      _rxEnt->mBuf =  NULLP;                        \
                                                    \
      /* If trace flag is enabled send the trace indication */\
      if(_cb->init.trc == TRUE)                     \
      {                                             \
         /* Populate the trace params */            \
         pjLmmSendTrc(_cb,EVTPJUDATIND, _mBuf);         \
      }                                             \
                                                    \
      PjUiPjuDatInd(&(_pjuSap->pst), _pjuSap->suId, \
             &_pdcpId, _mBuf);                       \
      PJ_STS_INC_PJU_CNT(_cb,_pjRbCb->rbType, txSdus)   \
}


#define  PJ_FILL_REEST_CFM_INFO(_reEstCfm, _asyncCfm, _status)  \
{                                                               \
   _reEstCfm->ueId    =  _asyncCfm->ueId;                       \
   _reEstCfm->cellId  =  _asyncCfm->cellId;                     \
   _reEstCfm->transId =  _asyncCfm->transId;                    \
   _reEstCfm->status  =  _status;                               \
}

#define  PJ_FILL_SEC_CFM_INFO(_secCfgCfm, _asyncCfm)  \
{                                                     \
   _secCfgCfm->ueId    =  _asyncCfm->ueId;            \
   _secCfgCfm->cellId  =  _asyncCfm->cellId;          \
   _secCfgCfm->transId =  _asyncCfm->transId;         \
   _secCfgCfm->status  =  UDX_CFG_CFM_OK;             \
   _secCfgCfm->reason  =  UDX_CFG_REAS_NONE;          \
}

#define  PJ_FILL_REEST_CFG_CFM_INFO(_cfmInfo, _asyncCfm)               \
{                                                                      \
   U8 _idx;                                                            \
   for (_idx = 0; _idx < _asyncCfm->numEnt ; _idx++)                   \
   {                                                                   \
      if (_asyncCfm->cfmEnt[_idx].cfgType == CPJ_CFG_REESTABLISH)      \
      {                                                                \
         _cfmInfo->cfmEnt[_idx].status = CPJ_CFG_CFM_OK;               \
         _cfmInfo->cfmEnt[_idx].reason = LCM_REASON_NOT_APPL;          \
      }                                                                \
      else                                                             \
      {                                                                \
         _cfmInfo->cfmEnt[_idx].status = _asyncCfm->cfmEnt[_idx].status;\
         _cfmInfo->cfmEnt[_idx].reason = _asyncCfm->cfmEnt[_idx].reason;\
      }                                                                \
      _cfmInfo->cfmEnt[_idx].rbId  = _asyncCfm->cfmEnt[_idx].rbId;      \
      _cfmInfo->cfmEnt[_idx].rbType  = _asyncCfm->cfmEnt[_idx].rbType; \
   }                                                                   \
   _cfmInfo->ueId    = _asyncCfm->ueId;                                \
   _cfmInfo->cellId  = _asyncCfm->cellId;                              \
   _cfmInfo->transId = _asyncCfm->transId;                             \
   _cfmInfo->numEnt  = _asyncCfm->numEnt;                              \
}


#define PJ_PERFORM_ERR_UPDATES(_cb, _pjRbCb, _count) \
{                                               \
   PjUlCb   *_upCb;                             \
                                                \
   _upCb =  &_pjRbCb->ulCb;                  \
   if(_upCb->recBuf.numEntries != 0)            \
   {                                            \
      PJ_CALC_NXT2SUB(_cb, _upCb);        \
   }                                            \
   PJ_CHK_RDY_TO_DELIVER(_cb, _pjRbCb);              \
}


#define PJ_CALC_NXT2SUB(_cb, __ulCb)                                             \
{                                                                          \
   PjRxEnt  *_rxEnt;                                                       \
   U32  _nxtToSub;                                                         \
   _nxtToSub = __ulCb->nxtSubCnt;                                           \
   if(__ulCb->recBuf.numEntries != 0)                                       \
   {                                                                       \
      _nxtToSub ++;                                                        \
      while( (_rxEnt = (PjRxEnt *)pjDbmGetRxEnt(_cb, &ulCb->recBuf, _nxtToSub)) != NULLP)\
      {                                                                 \
         __ulCb->nxtSubCnt++;                                            \
      }                                                                 \
   }                                                                       \
   __ulCb->nxtSubCnt  =  _nxtToSub;                                         \
}

#define PJ_RECALC_RX_VARS(_cb, _pjRbCb, _cnt )                                \
{                                                                        \
   PjUlCb   *_ulCb;                                                      \
   U32      _nxtToSub;                                                   \
   U32      _nxtToDeCmp;                                                 \
   U32      _nxtRxCnt;                                                   \
   PjRxEnt  *_rxEnt;                                                     \
                                                                         \
   _ulCb =  &_pjRbCb->ulCb;                                              \
   _nxtToSub   =  _ulCb->nxtSubCnt + 1;                                  \
   _nxtRxCnt   =  (_ulCb->nxtRxSn << (_pjRbCb->snLen)) | _ulCb->rxHfn;   \
   while(_nxtToSub < _nxtRxCnt)                                          \
   {                                                                     \
      if(pjDbmGetRxEnt(_cb, &ulCb->recBuf, _nxtToSub) != NULLP)               \
      {                                                                  \
         break;                                                          \
      }                                                                  \
      _nxtToSub++;                                                       \
   }                                                                     \
}

#define PJ_CHK_RDY_TO_DELIVER(_cb, _pjRbCb)                     \
{                                                          \
   PjUlCb   *_ulCb;                                       \
   PjRxEnt  *_rxEnt;                                      \
                                                           \
   _ulCb  =  &_pjRbCb->ulCb;                                     \
   if(_ulCb->recBuf.numEntries == 0)                       \
   {                                                       \
      if((_pjRbCb->rbType   ==  PJ_DRB)&&                  \
         (_pjRbCb->state != PJ_STATE_NORMAL))              \
      {                                                    \
            if((_ulCb->transCmp)                          \
                  && (_ulCb->obdCnt == 0)                \
              )                                            \
            {                                              \
               pjUlmReEstDrbAm(_cb, _pjRbCb);                     \
            }                                              \
      }                                                    \
   }                                                       \
   else                                                    \
   {                                                       \
     _rxEnt  =  (PjRxEnt *)pjDbmGetRxEnt(_cb, &_ulCb->recBuf, ulCb->nxtSubCnt);\
     if( _rxEnt && _rxEnt->state == PJ_RDY_TO_SUBMIT)      \
     {                                                     \
        if(_pjRbCb->rbType == PJ_SRB)                      \
        {                                                  \
           pjUlmDeliverSrb(_cb, _pjRbCb, _rxEnt);                \
        }                                                  \
        else                                               \
        {                                                  \
           if(_pjRbCb->mode   == PJ_DRB_UM)                \
           {                                               \
              pjUlmDeliverDrbUm(_cb, _pjRbCb, _rxEnt);           \
           }                                               \
           else                                            \
           {                                               \
              pjUlmDeliverDrbAm(_cb, _pjRbCb, _rxEnt);           \
           }                                               \
        }                                                  \
     }                                                     \
   }                                                       \
}                                                          

#define PJ_GET_COUNT(_count, _snLen, _sn, _hfn)              \
{                                                            \
   _count   = ((_hfn << _snLen) | _sn);                       \
}                                                            

#define PJ_GET_DATFWD_CNT(_pjRbCb, _datFwdSn, _count)                               \
{                                                                                   \
   if((_pjRbCb->ulCb.nxtRxSn == 0) && (_pjRbCb->ulCb.rxHfn == 0))                   \
   {                                                                                \
      PJ_GET_COUNT(_count, _pjRbCb->snLen, _datFwdSn, 0);                          \
   }                                                                                \
   else if(_datFwdSn >= _pjRbCb->ulCb.nxtRxSn)                                      \
   {                                                                                \
      PJ_GET_COUNT(_count, _pjRbCb->snLen, _datFwdSn, (_pjRbCb->ulCb.rxHfn));      \
   }                                                                                \
   else                                                                             \
   {                                                                                \
      PJ_GET_COUNT(_count, _pjRbCb->snLen, _datFwdSn, (_pjRbCb->ulCb.rxHfn + 1));  \
   }                                                                                \
}

#ifdef LTE_L2_MEAS
#define PJ_ULM_DRBUM_REEST(_cb,_pjRbCb)       \
{                                         \
   _pjRbCb->ulCb.nxtRxSn   =  0;          \
   _pjRbCb->ulCb.nxtSubSn  =  0;          \
   _pjRbCb->ulCb.rxHfn    =  0;           \
   pjUtlUlHdlRbReEstComplete(_cb, _pjRbCb);         \
}
#else
#define PJ_ULM_DRBUM_REEST(_cb,_pjRbCb)       \
{                                         \
   _pjRbCb->ulCb.nxtRxSn   =  0;          \
   _pjRbCb->ulCb.rxHfn    =  0;           \
   pjUtlUlHdlRbReEstComplete(_cb, _pjRbCb);         \
}
#endif

#define PJ_CHK_SN_OUTSIDE_WIN(_ulCb, _sn, _snLen)                                       \
(                                                                               \
 (((_sn - (_ulCb->lastSubCnt % (1 << _snLen ))) % (1 << _snLen)) > PJ_DRB_AM_WINSIZE(_snLen)) || \
 ((((_ulCb->lastSubCnt % (1 << _snLen)) - _sn) >= 0) &&               \
  (((_ulCb->lastSubCnt % (1 << _snLen)) - _sn) < PJ_DRB_AM_WINSIZE(_snLen)))  \
)   

/* Update state and call the respective delivery 
 * function */
#define PJ_ULM_DELIVER_DRB(_cb,_pjRbCb, _rxEnt)  \
{                                            \
   if(_pjRbCb->mode == PJ_DRB_AM)            \
   {                                         \
      pjUlmDeliverDrbAm(_cb,_pjRbCb, _rxEnt);    \
   }                                         \
   else                                      \
   {                                         \
      pjUlmDeliverDrbUm(_cb,_pjRbCb, _rxEnt);    \
   }                                         \
}
 
/* Update state and call the respective delivery 
 * function */
#define PJ_ULM_DELIVER_PDU(_cb,_pjRbCb, _rxEnt)  \
{                                            \
                                             \
   if(_pjRbCb->rbType == PJ_SRB)             \
   {                                         \
      pjUlmDeliverSrb(_cb,_pjRbCb, _rxEnt);      \
   }                                         \
   else                                      \
   {                                         \
      PJ_ULM_DELIVER_DRB(_cb,_pjRbCb, _rxEnt);   \
   }                                         \
}
 
/* There is case that all PDUs in the recv Buf can be back from off board 
 * but not sent up. In that case, though the the recvBuf has entries the 
 * obd timer should not be restarted. This works with the PJ_ULM_GET_DRBAM_SUBCNT
 * macro to acheive this. */
#define PJ_DRBAM_ALL_PDU_RECVD(_pjRbCb)\
((pjRbCb->rbType == PJ_DRB) &&         \
 (pjRbCb->mode == PJ_DRB_AM) &&        \
 (ulCb->lastSubCnt == ulCb->obdPdu))   \


#define PJ_ULM_GET_DRBAM_SUBCNT(_pjUlRbCb, _nxtSubCnt, _pjCb)                          \
{                                                                             \
   U32 _count    = _pjUlRbCb->ulCb.lastSubCnt;                                  \
   U32 _nxtRxCnt  =  (_pjUlRbCb->ulCb.nxtRxSn << _pjUlRbCb->snLen) | _pjUlRbCb->ulCb.rxHfn ;\
   PjRxEnt *_rxEnt = NULLP;                                                   \
   _nxtSubCnt      = _count;                                                  \
   if(((_pjUlRbCb->state != PJ_STATE_NORMAL) &&                                 \
       (_pjUlRbCb->ulCb.obdCnt != 0)) ||                                        \
      ((_pjUlRbCb->state == PJ_STATE_NORMAL) &&                                 \
       (_pjUlRbCb->ulCb.recBuf.numEntries != 0)))                               \
   {                                                                          \
      _count ++;                                                              \
      for(;_count < _nxtRxCnt; _count ++)                                     \
      {                                                                       \
         _rxEnt   = (PjRxEnt *)pjDbmGetRxEnt(_pjCb, &_pjUlRbCb->ulCb.recBuf, _count);  \
         if((_rxEnt != NULLP) && (_rxEnt->state != PJ_RDY_TO_SUBMIT))         \
         {                                                                    \
            _nxtSubCnt  =  _count;                                            \
            break;                                                            \
         }                                                                    \
      }                                                                       \
   }                                                                          \
}                                                                                   


#define PJ_ULM_GET_SUBCNT(_pjUlRbCb, _subPdu, _pjCb)                        \
{                                                                  \
   if((_pjUlRbCb->rbType == PJ_DRB) && (_pjUlRbCb->mode  == PJ_DRB_AM))\
   {                                                               \
      PJ_ULM_GET_DRBAM_SUBCNT(_pjUlRbCb, _subPdu, _pjCb);                   \
   }                                                               \
   else                                                            \
   {                                                               \
      _subPdu =  _pjUlRbCb->ulCb.nxtSubCnt;                           \
   }                                                               \
}


#define PJ_STS_INC_GEN_CNT(_cb, _cntr)       \
{                                            \
   _cb->pjGenSts._cntr++;                    \
}                                            \

#define PJ_STS_INC_PJU_CNT(_cb, _rbType, _cntr)   \
{                                            \
   if (_rbType == PJ_SRB)                    \
      _cb->u.ulCb->pjuSap[PJ_SRB_SAP].sts._cntr++;   \
   else                                      \
      _cb->u.ulCb->pjuSap[PJ_DRB_SAP].sts._cntr++;   \
}                                            \

#define PJ_PK_U8(_msg, _val, _ret)                                     \
{                                                                      \
   (_ret) = SAddPstMsg((Data) (_val), (_msg));                         \
}

#define PJ_PK_U16(_msg, _val, _ret)                                    \
{                                                                      \
   PJ_PK_U8(_msg, (U8) GetHiByte(_val), _ret)                          \
   if ((_ret) == ROK)                                                  \
      PJ_PK_U8(_msg, (U8) GetLoByte(_val), _ret)                       \
}

#define PJ_PK_U32(_msg, _val, _ret)                                    \
{                                                                      \
   U16 _tmp16;                                                         \
   _tmp16 = (U16) GetHiWord(_val);                                     \
   PJ_PK_U16(_msg, _tmp16, _ret)                                       \
   if ((_ret) == ROK)                                                  \
   {                                                                   \
      _tmp16 = (U16) GetLoWord(_val);                                  \
      PJ_PK_U16(_msg, _tmp16, _ret)                                    \
   }                                                                   \
}

#define PJ_PACK_MACI(_mBuf, _macI)              \
{                                               \
   U16 _tmp;                                    \
   Data _pkArray[4];                            \
   _tmp = (U16) GetHiWord(_macI);               \
   _pkArray[0] = (Data) GetHiByte(_tmp);        \
   _pkArray[1] = (Data) GetLoByte(_tmp);        \
   _tmp = (U16) GetLoWord(_macI);               \
   _pkArray[2] = (Data) GetHiByte(_tmp);        \
   _pkArray[3] = (Data) GetLoByte(_tmp);        \
   SAddPstMsgMult(_pkArray, (MsgLen) 4, _mBuf); \
}

#define PJ_UNPK_MACI(_mBuf, _macI)                          \
{                                                           \
   U16 _tmp16 = 0;                                          \
   Data _unPkArray[4] = {0};                                      \
   SRemPstMsgMult(_unPkArray, (MsgLen)4, _mBuf);            \
   _tmp16 = (U16) PutHiByte(_tmp16, (U8) _unPkArray[3]);    \
   _tmp16 = (U16) PutLoByte(_tmp16, (U8) _unPkArray[2]);    \
   _macI  = (U32) PutHiWord(_macI, (U16) _tmp16);           \
   _tmp16 = (U16) PutHiByte(_tmp16, (U8) _unPkArray[1]);    \
   _tmp16 = (U16) PutLoByte(_tmp16, (U8) _unPkArray[0]);    \
   _macI  = (U32) PutLoWord(_macI, (U16) _tmp16);           \
}

/*@def PJ_SEC_FILL_COUNT
*
*  This macro fills the COUNT value. COUNT value is composed 
*  of a HFN and the PDCP SN. Refer to 36.323 for details.
* 
*@param[in]_secInp      Count Value as Input 
*@param[in]_pSecInfo    Variable into which count value needs to be populated
*
*/
#define PJ_SEC_FILL_COUNT(_secInp, _pSecInfo)              \
{                                               \
   U16 _tmp;                                    \
   _tmp = (U16) GetHiWord(_secInp);               \
   _pSecInfo[0] = (Data) GetHiByte(_tmp);        \
   _pSecInfo[1] = (Data) GetLoByte(_tmp);        \
   _tmp = (U16) GetLoWord(_secInp);               \
   _pSecInfo[2] = (Data) GetHiByte(_tmp);        \
   _pSecInfo[3] = (Data) GetLoByte(_tmp);        \
}

/*@def PJ_SEC_FILL_FRESH
*
*  This macro fills the FRESH value. FRESH [0], . FRESH [3]
*  is set as BEARER[0] . BEARER[4] | 0 pow(27). Refer to 33.401 for details.
* 
*@param[in]_fresh   Variable into which o/p needs to be set.
*@param[in]_rbId    RbId of the RB to be filled in FRESH
*
*/
#define PJ_SEC_FILL_FRESH(_fresh, _rbId)              \
{                                               \
   _fresh[0] = 0;        \
   _fresh[1] = 0;        \
   _fresh[2] = 0;        \
   _fresh[3] = _rbId;        \
}

#define PJ_TIME_DIFF(cur,prev)                  \
   (cur<prev ? ((0xffffffff - prev) + cur ): (cur - prev)) 
#define PJ_MAX_CNT_RB_OVERLOAD 10
#endif /* __PJH__ */
  
/********************************************************************30**
  
         End of file:     pj.h@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:47:44 2014
         End of file:     pj.h@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:47:44 2014
  
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
/main/1      ---       mn  1. Initial release.
/main/2      ---       nm  1. LTERLC Release 2.1
*********************************************************************91*/
