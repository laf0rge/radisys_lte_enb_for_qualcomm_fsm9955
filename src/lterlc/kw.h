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

        Sid:      kw.h@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:05 2015
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw.h
@brief RLC Hash definitions
*/

#ifndef __KWH__
#define __KWH__
#include "rl_interface.h"
#include "rl_common.h"

 
#define KWLAYERNAME   "LTE RLC"     /* Layer Name */

#define EKWxxx 1
#define EMG099 1
#define EMG102 2
#define EMG103 3
#define EMG104 4

/* RLC-SPLIT Activity */
#define KW_ONE       1
#define KW_BIT0      0x01
#define KW_BIT1      0x02
#define KW_BIT2      0x04
#define KW_BIT3      0x08

#define KW_2K_BYTE   2048

/* RLC RB flag bits */
#define KW_RB_REESTABLISH_DL  KW_BIT0
#define KW_RB_REESTABLISH_UL  KW_BIT1
#define KW_RB_DELETE_DL       KW_BIT2
#define KW_RB_DELETE_UL       KW_BIT3


#define KW_MOD_1024           0x3FF  /* used for MOD 1024 */


 
/************************************************************************
 *                            SAP States
 ************************************************************************/

#define KW_SAP_NOT_CFG        0     /*!< SAP Not Configured */
#define KW_SAP_CFG            1     /*!< SAP Configured but not not bound */
#define KW_SAP_BND            2     /*!< SAP Bound */
#define KW_SAP_BINDING        3     /*!< SAP Bind initiated */
#define KW_SAP_UBND           4     /*!< SAP Unbind */

#define KW_MAX_SAP_BND_RETRY  3     /*!< Maximum SAP Bin Retries */

#define KW_MAX_UE             0xffffffff  /*!< Maximum number of UEs. */

/* Maximum number of Saps */
#define KW_MAX_UDXSAPS        1     /*!< Maximum number of UDX Saps */
#define KW_MAX_KWUSAPS        2     /*!< Maximum number of KWU Saps. */
#define KW_MAX_CKWSAPS        1     /*!< Maximum number of CKW Saps. */
#define KW_MAX_RGUSAPS        2     /*!< Maximum number of RGU Saps. */

#define KW_MAX_RGUSAP_TMR     1     /*!< Maximum number of RGU SAP Timers. */

#define KW_UI_RRC             0     /*!< Upper interface RRC sap Id. */
#define KW_UI_PDCP            1     /*!< Upper interface PDCP sap Id. */

#ifdef LTE_L2_MEAS
/* TODO. This works for FDD only. For TDD the array dimension
 * should be changed according to the number of Harq Procs */
#define KW_MAX_TB_PER_UE      64    /*!< Maximum number of tbCb for UE */
#define KW_INVALID_TBID       KW_MAX_TB_PER_UE
#endif
/*******************************************************************************
 *                              Memory related Defines 
 ******************************************************************************/
#ifdef MCCABE_COV
/* Allocate function */
#define KW_ALLOC(_cb,_buf, _size)                                   \
{                                                                   \
   SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf,       \
                (Size) _size);                                     \
      cmMemset((U8 *)(_buf), 0, _size);                             \
}

#define KW_RMV_SDU(_cb,_sduQ,_sdu)                    \
{                                                     \
   SPutMsg(_sdu->mBuf);                               \
   cmLListDelFrm(_sduQ,&_sdu->lstEnt);                \
   KW_FREE_WC(_cb,_sdu, sizeof(KwSdu));               \
}

#define KW_FREE(_cb,_buf, _size)                          \
{                                                         \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size);                 \
      _buf = NULLP;                                       \
}

#define KW_FREE_BUF(_buf)           \
{                                   \
      SPutMsg(_buf);                \
      _buf = NULLP;                 \
}

#else

#define KW_FREE_SHRABL_BUF(_region, _pool,_buf, _size)    \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutStaticBuffer(_region, _pool,             \
            (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define KW_FREE_SHRABL_BUF_WC(_region, _pool,_buf, _size) \
{                                                         \
  (Void) SPutStaticBuffer(_region, _pool,                 \
        (Data *) _buf, (Size) _size, 0);                 \
  _buf = NULLP;                                       \
}

#define KW_ALLOC_SHRABL_BUF_WC(_region, _pool,_buf, _size)           \
{                                                                    \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,                    \
                (Size) _size, 0);                                    \
}

#define KW_ALLOC_SHRABL_BUF(_region, _pool,_buf, _size)              \
{                                                                    \
 if (SGetStaticBuffer(_region, _pool, (Data **)&_buf,                \
                (Size) _size, 0) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}
/* Allocate function */
#define KW_ALLOC(_cb,_buf, _size)                                    \
{                                                                    \
 if (SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf,      \
                (Size) _size) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define KW_ALLOC_WC(_cb,_buf, _size)  \
           SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf, (Size) _size)     

#define KW_RMV_SDU(_cb,_sduQ,_sdu)              \
{                                               \
   if(_sdu->mBuf)                               \
   {                                            \
      SPutMsg(_sdu->mBuf);    \
   }                                            \
   cmLListDelFrm(_sduQ,&_sdu->lstEnt);          \
   KW_FREE(_cb,_sdu, sizeof(KwSdu));            \
}

#define KW_FREE(_cb,_buf, _size)                          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define KW_FREE_BUF(_buf)           \
{                                   \
   if (_buf != NULLP)               \
   {                                \
      SPutMsg(_buf);                \
   }                                \
      _buf = NULLP;                 \
}
#endif




#define KW_FREE_WC(_cb,_buf, _size)                       \
{                                                         \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size);                 \
}

/* kw002.201 Freeing from region of pst */
#define KW_PST_FREE(_region, _pool, _buf, _size)          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutSBuf(_region, _pool,                     \
                      (Data *) _buf, (Size) _size);       \
      _buf = NULLP;                                       \
   }                                                      \
}

#define KW_SHRABL_STATIC_BUF_FREE(_region, _pool, _buf, _size)     \
{                                                                  \
   if (_buf != NULLP)                                              \
   {                                                               \
      (Void) SPutStaticBuffer(_region, _pool,                      \
                      (Data *) _buf, (Size) _size, 0);             \
      _buf = NULLP;                                                \
   }                                                               \
}

#define KW_SHRABL_STATIC_BUF_ALLOC(_region, _pool, _buf, _size)     \
{                                                                        \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,      \
                (Size) _size, 0);                                        \
}
#define KW_FREE_BUF_WC(_buf)   SPutMsg((_buf));

#define KW_MEM_CPY(_dst, _src, _size)  cmMemcpy((U8*)_dst, (U8 *)_src, _size); 

#define KW_MEM_ZERO(_buf, _size) cmMemset((U8 *)(_buf), 0, _size);

#define KW_GET_MEM_REGION(_cb) (_cb->init.region)

#define KW_GET_MEM_POOL(_cb) (_cb->init.pool)

#define KW_GET_MEM_POOL_ADDRESS(_cb) (&_cb->init.pool)

/* Memset to value */
#define KW_MEM_SET(_arg, _val, _size) cmMemset((U8 *)_arg, (U8)_val, _size); 

/* Alarms */
/* Send an alarm for sapId events */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define KW_GETSDUIDX(_sduIdx) \
{\
   _sduIdx = (((_sduIdx)+1) % KW_L2MEAS_SDUIDX);\
}
#define KW_SEND_SAPID_ALARM(_cb,_sapId, _evnt, _cause) \
{ \
   kwLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, _sapId, 0, 0); \
}
#define KW_SEND_UEID_ALARM(_cb,_ueId, _qci, _evnt, _cause) \
{ \
   kwLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, 0, _ueId, _qci); \
}
#else /* LTE_L2_MEAS */
#define KW_SEND_SAPID_ALARM(_cb,_sapId, _evnt, _cause) \
{ \
   kwLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, _sapId, 0); \
}
#define KW_SEND_UEID_ALARM(_cb,_ueId, _evnt, _cause) \
{ \
   kwLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, 0, _ueId); \
}
#endif /* LTE_L2_MEAS */

/*******************************************************************************
 *                              Common Defines 
 ******************************************************************************/

/* RLC Configuration parameters */
#define KW_MAX_UL_LI                (2 * KW_MAX_LI)
/*macro KW_MAX_DL_LI is moved to kw_env.h file */
#define KW_MAX_DAT                  KW_MAXIMUM_DAT
/*macro KW_MAX_PDU is moved to kw_env.h file */
#define KW_MAX_RB_PER_CELL          10
#define KW_MAX_SRB_PER_UE           3
#define KW_MAX_DRB_PER_UE           32
#define KW_MAX_LCH_PER_UE           12
#define KW_MAX_LCH_PER_CELL         6
#define KW_MAX_NUM_RB               24
#define KW_MAX_UE                   0xffffffff 
#define KW_UE_LIST_BUCKET_SIZE      128 
#define KW_CELL_LIST_BUCKET_SIZE    10 
#define KW_TRANS_ID_LST_BKT_SIZE    10 
#define KW_MAX_RB                   32

/* Direction defines */
#define KW_DIR_UL        1     /*!< Unlink direction */
#define KW_DIR_DL        2     /*!< Downlink direction */
#define KW_DIR_BOTH      3     /*!< Both Downlink and Unlink */

#define KW_DEF_SEQ_NUM 0 /**< Sequence number to pick in case of duplicate
                              entries in hash list searches*/

/**
 * @def KW_MIN
 *
 *    Macro to find the miniumum of two numbers
 *
 * @param[in] x    First number
 * @param[in] y    Second number
 *
*/
#define KW_MIN(x,y) (x) < (y) ? (x) : (y)

/**
 * @def KW_GET_KWCB
 *
 *    Macro to the RLC instance
 *
 * @param[in] _inst    Instance Id
 *
*/
#define KW_GET_KWCB(_inst) kwCb[_inst]                              

#define KW_ADD_SDU            1     /*!< Add SDU. */
#define KW_DEL_SDU            2     /*!< Delete SDU. */

#define KW_CFM_NOK            0     /*!< Do not send DatCfm */
#define KW_CFM_OK             1     /*!< Send DatCfm */

/* Set the unsolictated Status flag */
#define KW_SET_USTA_FLAG(_kwCb, _value) \
{ \
   _kwCb->init.usta = _value; \
}

/* Macros to get the init parameters */
#define KW_GET_DBG_MASK(_kwCb) (_kwCb->init.dbgMask)
#define KW_GET_LMPST_MEM_POOL(_kwCb) (_kwCb->init.lmPst.pool)
#define KW_GET_LMPST_MEM_REGION(_kwCb) (_kwCb->init.lmPst.region)

/* Macros for configuration module */
#define KW_CFG_FILL_CFG_CFM(_entCfm, _rbId, _rbType, _status, _reason)  \
{                                                              \
   _entCfm->rbId  = _rbId;                                     \
   _entCfm->rbType = _rbType;                                  \
   _entCfm->status.status = _status;                           \
   _entCfm->status.reason = _reason;                           \
} 

/**
 * @def KW_VALIDATE_UE_RBID
 *
 *    This macro validates whether the _rbId passed is valid or not. It checks
 *    if the _rbId is within the maximum value depending on the _rbType.
 *    Returns TRUE if valid else FALSE
 *
 * @param[in] _rbType    Type of the Radio Bearer; SRB or DRB
 * @param[in] _rbId      RB Id of the RB to be validated
 *
*/ 
#define KW_VALIDATE_UE_RBID(_rbType, _rbId)                     \
       ((_rbType == CM_LTE_SRB && _rbId < KW_MAX_SRB_PER_UE) || \
       (_rbType == CM_LTE_DRB && _rbId < KW_MAX_DRB_PER_UE))

/*******************************************************************************
 *                              UIM Defines 
 ******************************************************************************/
#if (ERRCLASS & ERRCLS_INT_PAR)
#define KW_VALDATE_SAP(_cb,_chkSpId, _sap, _ret)                             \
{                                                                            \
   if (_chkSpId != _sap->spId)                                               \
   {                                                                         \
      KWLOGERROR(_cb,ERRCLS_DEBUG, EKWxxx, (ErrVal) RFAILED,                 \
            "Sap Id Validation Failed.");                                    \
      _ret = RFAILED;                                                        \
   }                                                                         \
   /* SAP state validation */                                                \
   if(_sap->state != KW_SAP_BND)                                             \
   {                                                                         \
      KWLOGERROR(_cb,ERRCLS_INT_PAR, EKWXXX, (ErrVal) RFAILED,               \
            "Sap State Invalid.");                                           \
      KW_SEND_SAPID_ALARM(_cb,0, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_STATE); \
      _ret = RFAILED;                                                        \
   }                                                                         \
}
#else /* ERRCLASS & ERRCLS_INT_PAR */
#define KW_VALDATE_SAP(_cb,_chkSpId, _sap, _ret)                             \
{                                                                            \
   /* SAP state validation */                                                \
   if(_sap->state != KW_SAP_BND)                                             \
   {                                                                         \
      KWLOGERROR(_cb,ERRCLS_INT_PAR, EKWXXX, (ErrVal) RFAILED,               \
            "Sap State Invalid.");                                           \
      KW_SEND_SAPID_ALARM(_cb,0, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_STATE); \
      _ret = RFAILED;                                                        \
   }                                                                         \
}
#endif /* ERRCLASS & ERRCLS_INT_PAR */

/*******************************************************************************
 *                              Timer Defines 
 ******************************************************************************/
#define KW_TMR_LEN                     10
#define KW_MAX_UM_TMR                  1
#define KW_MAX_AM_TMR                  3
#define KW_EVT_UMUL_REORD_TMR          1
#define KW_EVT_AMUL_REORD_TMR          2
#define KW_EVT_AMUL_STA_PROH_TMR       3
#define KW_EVT_AMDL_POLL_RETX_TMR      4
#define KW_EVT_WAIT_BNDCFM             5
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define KW_EVT_L2_TMR                  6
#endif /* LTE_L2_MEAS */

/*******************************************************************************
 *                              DBM Defines 
 ******************************************************************************/
/**
 * @def KW_DBM_GET_RBCB_FROM_UECB
 *
 *    This macro makes _rbCb point to the RB in _ueCb based on the passed 
 *    _rbId and _rbType. _rbCb can point to NULLP
 *
 * @param[in] _rbId      RB Id of the RB to be fetched
 * @param[in] _rbType    Type of the RB, SRB or DRB
 * @param[in] _ueCb      Pointer to the UECb for which to get the RB
 * @param[out] _rbCb     Pointer to the found RbCb
 *
*/
#define KW_DBM_GET_RBCB_FROM_UECB(_rbId, _rbType, _ueCb, _rbCb)            \
           (_rbCb) = ((_rbType) == CM_LTE_SRB) ? (_ueCb)->srbCb[(_rbId)] : \
                                                 (_ueCb)->drbCb[(_rbId)];     
/**
 * @def KW_DBM_GET_CELL_RBCB
 *
 *    This macro makes _rbCb point to the RB in the _rbCbLst. 
 *    _rbCb can point to NULLP
 *
 * @param[in] _rbId         RB Id of the RB to be fetched
 * @param[in] _rbCbLst      Pointer to array of RBCbs
 * @param[out] _rbCb        Pointer to the found RbCb
 *
*/
#define KW_DBM_GET_CELL_RBCB(_rbId, _rbCbLst, _rbCb) \
           (_rbCb) = (_rbCbLst)[(_rbId)]; 

/*******************************************************************************
 *                              UMM Defines 
 ******************************************************************************/
#define KW_UMDL     rbCb->m.umDl 
#define KW_UMUL     rbCb->m.umUl 

/* Sequence Number length defines */
#define KW_UM_CFG_5BIT_SN_LEN      1 /**< UM 5-bit Sequence number length 
                                          in bytes*/   
#define KW_UM_CFG_10BIT_SN_LEN     2 /**< UM 10-bit Sequence number length 
                                          in bytes*/

/**
 * @def KW_RMV_MAC_HDR_SZ
 *
 *    If PDU size is greater than 127, MAC header would be 3 bytes else 2 bytes
 *
 * @param[in,out] _pduSz        Size of the pdu 
 *
*/
#define KW_RMV_MAC_HDR_SZ(_pduSz) (_pduSz) -= ((_pduSz) > 127) ? 3 : 2;

/**
 * @def KW_UM_GET_VALUE
 *
 *    This macro is used to calculate the value of UM state variables used 
 *    in comparisons.  VR(UH) - UM Window Size is taken as the base modulus.
 *    Returns the modifed value
 *
 * @param[in] _val               Value of the state variable 
 * @param[in] _kwUmUl        Um Uplink control block
 *
*/ 
#define KW_UM_GET_VALUE(_val,_kwUmUl)  \
     (((_val) - ((_kwUmUl).vrUh - (_kwUmUl).umWinSz)) & ((_kwUmUl).modBitMask))
     
/*******************************************************************************
 *                              AMM Defines 
 ******************************************************************************/ 
#define AMDL                           rbCb->m.amDl 
#define AMUL                           rbCb->m.amUl

/* PDU Types */
#define KW_DATA_PDU  1
#define KW_CNTRL_PDU 0

#define KW_FI_FIRST_SEG                0x02
#define KW_FI_LAST_SEG                 0x01

#define KW_POLL_SET                    0x20 /* 00100000 */
#define KW_POLL_UNSET                  0xdf /* 11011111 */
#define KW_AM_WIN_SZ                   512
#define KW_MAX_NACK_CNT                100
/*KW_MAX_CNTRL_FIELDS (Maximum size of Status Pdu) 
 *  = MAX_NACK_CNT * sizeof(NACK,E1,E2,soStart,soEnd)+ Header*/ 
#define KW_MAX_CNTRL_FIELDS            ((KW_MAX_NACK_CNT*42+ 14 +7)/8)  

/* Each LI(Length Indicator) holds approx 1+1/2 byte and some other fields thus keeping Header Size equal to twice of MAX LI */
#define KW_MAX_HDRSZ                   (2*KW_MAX_LI) 
#define KW_AM_PDU_FIXED_HDRSZ          2
#define KW_AM_SEG_FIXED_HDRSZ          4
#define KW_EXTN_HDRSZ                  2
#define KW_CNTRL_PDU_FIXED_HDRSZ       2
#define KW_MAC_HDR_SZ2                 2
#define KW_MAC_HDR_SZ3                 3
#define KW_BYTE_LEN                    8
#define KW_2BYTE_LEN                   16
#define KW_E1_LEN                      1
#define KW_NACK_E1E2_LEN               12
#define KW_SO_LEN                      15
#define KW_DC_LEN                      1
#define KW_CPT_LEN                     3
#define KW_RF_LEN                      1
#define KW_P_LEN                       1
#define KW_FI_LEN                      2
#define KW_E_LEN                       1
#define KW_SN_LEN                      10
#define KW_LSF_LEN                     1
#define KW_LI_LEN                      11

#define KW_DC_POS                      0x80
#define KW_DC_SHT                      7      
#define KW_RF_POS                      0x40
#define KW_RF_SHT                      6 
#define KW_POLL_POS                    0x20
#define KW_POLL_SHT                    5 
#define KW_FI_POS                      0x18
#define KW_FI_SHT                      3
#define KW_E_POS                       0x04
#define KW_E_SHT                       2 
#define KW_SN_POS                      0x03

#define KW_SDU_LST                     1
#define KW_SEG_LST                     2
#define KW_RETX_LST                    3
#define KW_ALL_BYTES_MISSING           0x7fff

#define KW_MAX_PDU_MAP                 30       /*!< Maximum PDU Map. */

#define KW_LLIST_FIRST_SDU(lstCp, nod)          \
{                                               \
   CmLList *tmpNode;                            \
   /*CM_LLIST_FIRST_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP)*/                        \
   if((tmpNode=cmLListFirst(&lstCp)))            \
      nod = (KwSdu *)tmpNode->node;             \
   else                                         \
      nod = NULLP;                              \
}                                                          

                                                           
#define KW_LLIST_FIRST_SEG(lstCp, nod)         \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_FIRST_NODE(&(lstCp), tmpNode);*/     \
   /*if (tmpNode != NULLP)*/                       \
   if((tmpNode=cmLListFirst(&lstCp)))            \
      nod = (KwSeg *)tmpNode->node;            \
   else                                        \
      nod = NULLP;                             \
}                                                          

                                                           
#define KW_LLIST_FIRST_RETX(lstCp, nod)        \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_FIRST_NODE(&(lstCp), tmpNode);*/     \
   /*if (tmpNode != NULLP)*/                       \
   if((tmpNode=cmLListFirst(&lstCp)))            \
      nod = (KwRetx *)tmpNode->node;           \
   else                                        \
      nod = NULLP;                             \
}

#define KW_LLIST_NEXT_SDU(lstCp, nod)          \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_NEXT_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP)  */                     \
   if((tmpNode = cmLListNext(&lstCp)))          \
      nod = (KwSdu *)tmpNode->node;            \
   else                                        \
      nod = NULLP;                             \
}                                              


#define KW_LLIST_NEXT_SEG(lstCp, nod)          \
{                                              \
   CmLList *tmpNode;                           \
   (lstCp).crnt = &((nod)->lstEnt);            \
   /*CM_LLIST_NEXT_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP)*/                       \
   if((tmpNode = cmLListNext(&lstCp)))           \
      nod = (KwSeg *)tmpNode->node;            \
   else                                        \
      nod = NULLP;                             \
}      

                                        
#define KW_LLIST_NEXT_RETX(lstCp, nod)         \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_NEXT_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP) */                      \
   if ((tmpNode = cmLListNext(&lstCp)))          \
      nod = (KwRetx *)tmpNode->node;           \
   else                                        \
      nod = NULLP;                             \
}


#define KW_LLIST_LAST_RETX(lstCp, nod)         \
{                                              \
   CmLList *tempNode = NULLP;                  \
   cmLListLast(&lstCp);                        \
   tempNode = cmLListCrnt(&lstCp);             \
   if (tempNode != NULLP)                      \
      nod = (KwRetx *)tempNode->node;          \
   else                                        \
      nod = NULLP;                             \
}

#define KW_LLIST_LAST_SEG(lstCp, nod)          \
{                                              \
   CmLList *tempNode = NULLP;                  \
   cmLListLast(&lstCp);                        \
   tempNode = cmLListCrnt(&lstCp);             \
   if (tempNode != NULLP)                      \
      nod = (KwSeg *)tempNode->node;           \
   else                                        \
      nod = NULLP;                             \
}

#define KW_LLIST_LAST_SDU(lstCp, nod)          \
{                                              \
   CmLList *tempNode = NULLP;                  \
   cmLListLast(&lstCp);                        \
   tempNode = cmLListCrnt(&lstCp);             \
   if (tempNode != NULLP)                      \
      nod = (KwSdu *)tempNode->node;           \
   else                                        \
      nod = NULLP;                             \
}

#define CM_LLIST_INS_AFT_CRNT(lstCp, nod)      \
{                                              \
   CmLList *nodeToIns = &nod->lstEnt;\
   nodeToIns->node = (PTR) nod;\
   cmLListInsAfterCrnt(&lstCp, nodeToIns);     \
}

#define CM_LLIST_INS_BEF_CRNT(lstCp, nod)      \
{                                              \
   CmLList *nodeToIns = &nod->lstEnt;          \
   nodeToIns->node = (PTR) nod;                \
   cmLListInsCrnt(&lstCp, nodeToIns);          \
}

#define KW_LLIST_DEL_RECBUF(_recBuf)                       \
{                                                          \
   KwSeg  *_seg = NULLP;                                   \
   KW_LLIST_FIRST_SEG(_recBuf->segLst, _seg);              \
   while (_seg)                                            \
   {                                                       \
      cmLListDelFrm(&_recBuf->segLst, &_seg->lstEnt);      \
      KW_FREE(_seg, sizeof(KwSeg));                        \
      KW_LLIST_NEXT_SEG(_recBuf->segLst, _seg);            \
   }                                                       \
}

#define MODAMT(x, y, z)   \
{                         \
   y = (x - z) & 0x3ff;   \
}                                                         

#define MODAMR(x, y, z)   \
{                         \
   y = (x - z) & 0x3ff;   \
}

/**
 * @def KW_AM_IS_TRANS_WIN_STALLED
 *
 *    This macro is used to check if the AM transmit window is stalled or not.
 *    The tramist window is stalled when the distance between VT(S) and VT(A)
 *    is greater than equal to 512. Actually it should never be greater 
 *    than 512.
 *    Returns TRUE if the window is stalled else FALSE
 *
 * @param[in] _amDl     AM Downlink control block
 *
*/ 
#define KW_AM_IS_TRANS_WIN_STALLED(_amDl)  \
     ((((_amDl)->vtS - (_amDl)->vtA) & 0x3ff) >= KW_AM_WIN_SZ)

#ifdef TENB_STATS
#define KW_AM_TRANS_WIN_SIZE(_amDl)  \
     (((_amDl)->vtS - (_amDl)->vtA) & 0x3ff)
#endif

#define KW_AM_IS_POLL_BIT_SET(_amDl) \
  (AMDL.pollSn == ((AMDL.vtS - 1) & 0x3ff))

#define KW_FILL_CNTRL_INFO(cntrlInfo, _val, _len, _idx, _eb)\
{                                                           \
   cntrlInfo.val = _val;                                    \
   cntrlInfo.len = _len;                                    \
   cntrlInfo.idx = _idx;                                    \
   cntrlInfo.emtBits = _eb;                                 \
}
#define KW_FILL_PREV_IDX(cntrlInfo, _e1Idx, _e1eb, _idx, _eb) \
{                                                                     \
  _e1Idx = cntrlInfo.e1Idx;                                           \
  _e1eb  = cntrlInfo.e1eb;                                            \
  _idx   = cntrlInfo.idx;                                             \
  _eb    = cntrlInfo.emtBits;                                         \
}

#define KW_FILL_HDR_ARGS(hdrInfo, _val, _len)  \
{                                              \
   hdrInfo.val = _val;                         \
   hdrInfo.len = _len;                         \
}

/* kw003.201 - This macro provides the header size other than the */
/*             fixed header of 2 bytes for each AMD PDU or 4 bytes*/
/*             for an AM PDU segment                              */
#define KW_AM_EXTN_HDRSZ(_numLi, _eHdrSz)       \
{                                               \
   if ((_numLi - 1) % 2)                        \
   {                                            \
      _eHdrSz = ((3 * (_numLi - 2)) >> 1) + 2;  \
   }                                            \
   else                                         \
   {                                            \
      _eHdrSz = (3 * (_numLi - 1)) >> 1;        \
   }                                            \
}

/* Update poll bit in the buffer */
#define KW_UPD_POLL_BIT(_gCb, _retx, _poll)                \
{                                                          \
   U8 fHdr;                                                \
                                                           \
   if (_poll != _retx->amHdr.p)                            \
   {                                                       \
      /* Get the first byte of the buffer */               \
      SRemPreMsg((Data *)&fHdr, _retx->seg);               \
      if (_poll == TRUE)                                   \
      {                                                    \
         fHdr = fHdr | KW_POLL_SET;                        \
      }                                                    \
      else                                                 \
      {                                                    \
         fHdr = fHdr & KW_POLL_UNSET;                      \
      }                                                    \
      /* Concatenated updated hdr to the mBuf */           \
      SAddPreMsg ((Data)fHdr, _retx->seg);                 \
   }                                                       \
   /* Update poll bit in the hdrInfo */                    \
   _retx->amHdr.p = _poll;                                 \
}

#define KW_AM_ELIMINATE_EXTN_HDR(_pduSz, _sduSz, _numLi)   \
{                                                          \
   if ( (_pduSz > _sduSz) && (_sduSz < 2048) )             \
   {                                                       \
      _pduSz -= (_numLi % 2) ? 1 : 2;                      \
   }                                                       \
}
/**
 * @def KW_AM_CHK_SN_WITHIN_RECV_WINDOW
 *
 *    This macro is used to check if a Sequence Number falls within the AM
 *    reception window or not.
 *    The condition is VR(R) <= SN < VR(MR), which are subtracting the base
 *    modulus becomes 0 <= (SN - VR(R)) % 1024 < (VR(MR) - VR(R)) % 1024
 *    NOTE: Careful with the parantheses
 *
 *    Returns TRUE if within the window; FALSE otherwise
 *
 * @param[in] _sn       The sequence number to be checked
 * @param[in] _amUl     AM Uplink control block
 *
*/
#define KW_AM_CHK_SN_WITHIN_RECV_WINDOW(_sn, _amUl)          \
  ((((_sn) - (_amUl->vrR)) & 0x3ff) < (((_amUl->vrMr) - (_amUl->vrR)) & 0x3ff)) 

#define KW_POWER(x, y)  x << (y-1); 

#ifndef L2_OPTMZ
#define kwCpyMsg(_cb,x, y) \
      (SAddMsgRef((x), KW_GET_MEM_REGION(_cb), KW_GET_MEM_POOL(_cb), (y)))
#else
/* L2 optimization for mUe/Tti: Removing dup buf*/
#define kwCpyMsg(_cb,x, y) \
      (SIncMsgRef((x), KW_GET_MEM_REGION(_cb), KW_GET_MEM_POOL(_cb), (y)))
#endif

//      printf("Copy Msg %x \n",x);

/*******************************************************************************
 *                              Debugging Defines 
 ******************************************************************************/
#define KW_DBG_SUB_MASK   DBGMASK_MI             /**< Use for sub-mask */
#define KW_DBGMASK_DETAIL (KW_DBG_SUB_MASK << 0) /**< Parameters, It will give
                                                      in depth info */
#define KW_DBGMASK_BRIEF  (KW_DBG_SUB_MASK << 1) /**< Info, It will give info at
                                                    entry and exit places along
                                                   with certain state changes */
#define KW_DBGMASK_ERROR  (KW_DBG_SUB_MASK << 2) /**< Error information */
#define KW_DBGMASK_FATAL  (KW_DBG_SUB_MASK << 3) /**< FATAL errors like memory
                                                    resource failure etc., */

#define KW_DBG_MDL_MASK (KW_DBG_SUB_MASK << 4)

#define KW_DBGMASK_TM         (KW_DBG_MDL_MASK << 0)    /**< TM */
#define KW_DBGMASK_UM         (KW_DBG_MDL_MASK << 1)    /**< UM */
#define KW_DBGMASK_AM         (KW_DBG_MDL_MASK << 2)    /**< AM */
#define KW_DBGMASK_DL         (KW_DBG_MDL_MASK << 3)    /**< DL */
#define KW_DBGMASK_UL         (KW_DBG_MDL_MASK << 4)    /**< UL */
#define KW_DBGMASK_CFG        (KW_DBG_MDL_MASK << 5)    /**< CFG */
#define KW_DBGMASK_LMM        (KW_DBG_MDL_MASK << 6)    /**< LMM */
#define KW_DBGMASK_INF        (KW_DBG_MDL_MASK << 7)    /**< UI, LI */
#define KW_DBGMASK_DUT        (KW_DBG_MDL_MASK << 8)    /**< DBM, UTL, TMR */
#define KW_DBGMASK_MBUF_PRNT  (KW_DBG_MDL_MASK << 9)    /**< MBUF, useful in
                                                             integrated 
                                                             testing */
#define KW_DBGMASK_MEM_INFO   (KW_DBG_MDL_MASK << 10)   /**< Print SSI memory
                                                             information*/
#define KW_DBGMASK_UDX        (KW_DBG_MDL_MASK << 11)   /**< UDX interface */

#ifdef DEBUGP
#define KW_PRNT_BORDER                                   \
do                                                       \
{                                                        \
   KW_PRNT((_kwPBuf, "\n==========================\n")); \
}while(0)

#define KW_PRNT_HLINE(_cb,_pMsg)                                              \
{                                                                             \
   sprintf((_cb)->init.prntBuf, "[RLC_LAYER: %s:%d]::", __FILE__, __LINE__);  \
   SPrint((_cb)->init.prntBuf);                                               \
   KW_PRNT_TSTAMP(_cb);                                                       \
   sprintf((_cb)->init.prntBuf, _pMsg);                                       \
   SPrint((_cb)->init.prntBuf);                                               \
}

#define KW_PRNT(_cb,_prntbuf)  \
{                              \
   sprintf _prntbuf;           \
   SPrint(_cb->init.prntBuf);  \
}

#define KW_PRINT_TO_BUFFER(_cb,...)                             \
{                                                               \
   snprintf((_cb)->init.prntBuf, PRNTSZE, "[%s]::", __func__);  \
   SPrint((_cb)->init.prntBuf);                                 \
   snprintf(_cb->init.prntBuf, PRNTSZE,__VA_ARGS__);            \
   SPrint(_cb->init.prntBuf);                                   \
}

#define KW_PRNT_TSTAMP(_cb)                                   \
{                                                             \
   S8 _buf[60];                                               \
   DateTime dt;                                               \
   cmMemset((U8 *)(&dt), 0, sizeof(DateTime));                \
   SGetDateTime(&dt);                                         \
   sprintf(_buf, "date: %02d/%02d/%04d time: %02d:%02d:%02d", \
     (int)dt.month,(int)dt.day,(int)dt.year + 1900,           \
     (int)dt.hour,(int)dt.min,(int)dt.sec);                   \
   KW_PRNT(_cb,(_cb->init.prntBuf,("[%s]", _buf)));           \
}

#define KW_PRNT_MBUF(_cb,_mBufPtr)                          \
do                                                          \
{                                                           \
   if(_cb->init.dbgMask & (KW_DBGMASK_MBUF_PRNT))           \
   {                                                        \
     KW_PRNT_HLINE(_cb,("\nMessage Buffer Contents:\n"));   \
     SPrntMsg ((Buffer *)_mBufPtr, 0, 0);                   \
   }                                                        \
}while(0)

#define KW_PRNT_MEMINFO(_cb)                                  \
do                                                            \
{                                                             \
   U32  _memInfo;                                             \
   if(_cb->init.dbgMask & (KW_DBGMASK_MEM_INFO))              \
   {                                                          \
     KW_PRNT_HLINE(_cb,("\nMemory Information:\n"));          \
     SRegInfoShow(0, &_memInfo);                              \
   }                                                          \
}while(0)

#define KWDBGP_INTERNAL(_cb,_mask,...)           \
do                                               \
{                                                \
   if (!((_cb->init.dbgMask & _mask) ^ _mask))   \
   {                                             \
      KW_PRINT_TO_BUFFER(_cb, __VA_ARGS__);      \
   }                                             \
}while(0)

#define KWDBGP_ERROR(_cb, ...) \
   KWDBGP_INTERNAL(_cb,(KW_DBGMASK_ERROR | KW_MODULE),__VA_ARGS__)

#define KWDBGP_DETAIL(_cb, ...) \
   KWDBGP_INTERNAL(_cb,(KW_DBGMASK_DETAIL | KW_MODULE),__VA_ARGS__)

#define KWDBGP_BRIEF(_cb, ...) \
   KWDBGP_INTERNAL(_cb,(KW_DBGMASK_BRIEF | KW_MODULE),__VA_ARGS__)   
   
#else  /* DEBUGP */ 
#define KW_PRNT_HLINE(_cb,_pMsg)
#define KW_PRNT(_cb,_prntbuf)
#define KW_PRNT_TSTAMP(_cb)
#define KW_PRNT_MBUF(_cb,_mBufPtr)
#define KW_PRNT_MEMINFO(_cb)
#define KWDBGP(_cb,_mask, _arg)
#define KWDBGP_ERROR(_cb, ...) 
#define KWDBGP_DETAIL(_cb, ...)
#define KWDBGP_BRIEF(_cb, ...)
#endif /* DEBUGP */

/*******************************************************************************
 *                              LMM Defines 
 ******************************************************************************/
#define KW_LMM_RB_STS_INC(_cb)    (_cb)->genSts.numOfRb++;

#define KW_LMM_RB_STS_DEC(_cb)    (_cb)->genSts.numOfRb--;

#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
#define KW_FILL_SAP_HELPER(_Sap, _cfg, _gCb)\
{\
   _Sap->pst.selector = _cfg->selector; \
   _Sap->pst.route = _cfg->route; \
   _Sap->pst.prior =  _cfg->priority; \
   _Sap->pst.region = _gCb->init.region;\
   _Sap->pst.pool = _gCb->init.pool;\
   _Sap->pst.dstProcId = _cfg->procId; \
   _Sap->pst.dstEnt = _cfg->ent; \
   _Sap->pst.dstInst = _cfg->inst; \
   _Sap->pst.srcProcId = _gCb->init.procId; \
   _Sap->pst.srcEnt = _gCb->init.ent; \
   _Sap->pst.srcInst = _gCb->init.inst; \
   _Sap->pst.event = EVTNONE; \
   _Sap->spId = _cfg->sapId; \
   _Sap->state = KW_SAP_CFG; \
}
#else /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
#define KW_FILL_SAP_HELPER(_Sap, _cfg, _gCb)\
{\
   _Sap->pst.selector = _cfg->selector; \
   _Sap->pst.route = _cfg->route; \
   _Sap->pst.prior =  _cfg->priority; \
   _Sap->pst.region = _cfg->mem.region;\
   _Sap->pst.pool = _cfg->mem.pool;\
   _Sap->pst.dstProcId = _cfg->procId;\
   _Sap->pst.dstEnt = _cfg->ent;\
   _Sap->pst.dstInst = _cfg->inst;\
   _Sap->pst.srcProcId = _gCb->init.procId;\
   _Sap->pst.srcEnt = _gCb->init.ent;\
   _Sap->pst.srcInst = _gCb->init.inst;\
   _Sap->pst.event = EVTNONE;\
   _Sap->spId = _cfg->sapId;\
   _Sap->state = KW_SAP_CFG;\
}
#endif

/*******************************************************************************
 *                              UDX Defines 
 ******************************************************************************/
#define KW_GET_DL_SAPCB(_cb, _rbCb) (_cb->u.dlCb->udxDlSap + _rbCb->udxSapId)
#if 0
#define KW_GET_UL_SAPCB(_cb, _rbCb) (_cb->u.ulCb->udxUlSap + _rbCb->udxSapId)
#endif
#define KW_GET_UDX_SAP(_cb) (_cb->u.ulCb->udxUlSap)

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define KW_L2_MAX_TIMERS        1
#define KW_QCI_LIST_BUCKET_SIZE 10
#define KW_TB_LIST_BUCKET_SIZE  10
#define KW_MAX_L2MEAS_EVT       10
/* L2 Measurement index to be used in rbCb to store measData */                                       
#define KW_L2MEAS_ACT_UE       0                                       
#define KW_L2MEAS_DL_DELAY     1                                       
#define KW_L2MEAS_DL_DISC      2
#define KW_L2MEAS_UU_LOSS      3
#define KW_L2MEAS_DL_IP        4
#define KW_L2MEAS_UL_IP        5
#endif /* LTE_L2_MEAS */

#define KW_RDWR_LOCK(_lockPtr)
#define KW_RDWR_UNLOCK(_lockPtr)
#define KW_TIME_DIFF(t1,t2)                  \
   (t1<t2 ? ((0xffffffff - t2) + t1 ): (t1 - t2)) 

#endif /* __KWH__ */
  
/********************************************************************30**
  
         End of file:     kw.h@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:05 2015
  
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
*********************************************************************91*/
