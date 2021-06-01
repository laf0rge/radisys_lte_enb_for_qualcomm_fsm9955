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

     Desc:     This file contains 
               

     File:     wr_cmn.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* This file includes the definitions that are common to multiple modules */

#ifndef __WR_CMN_H__
#define __WR_CMN_H__

#include "wr.h"
#include "wr_alarm.h"
#include "rmu.x"
#include "rmu.h"
#include "lnl.h"
#include "lnl.x"
#include "lnl.h"
#include "nlu.x"
#include "leg.x"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/* 
 * Message building related macros 
 * */
/* Macros to get NCC and BCC from BSIC */
#define WR_GET_NCC_FRM_BSIC(bsic) (bsic & 0x38)>>3  
#define WR_GET_BCC_FRM_BSIC(bsic) (bsic & 0x07)

/* Get absolute value */
#define WR_ABS_VAL(_x,_y) ((_x > _y)?((_x) - (_y)):((_y) - (_x)))

/**
 * @def WR_GET_NUM_ENBIP_CFGD 
 *
 *    Retrieve number of eNB IP Addresses configured from Tpt address array 
 *
 * @param[in] _srcCmtptArr   Source ENB IP address 
 * @param[in] _dstCount   Dst counter
 *
*/
#define WR_GET_NUM_ENBIP_CFGD(_srcCmtptArr, _dstCount) \
{\
   _dstCount = 0;\
   if(_srcCmtptArr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)\
   {\
     _dstCount++;\
   }\
   if(_srcCmtptArr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)\
   {\
     _dstCount++;\
   }\
}   
   
/**
 * @def WR_GET_IDX_FROM_ENBIP_ARR 
 *
 *   Retrieve array index for given eNB IP Address type 
 *
 * @param[in] _srcType   Source IP address type
 * @param[in] _dstIdx    IPV4/IPV6 index
 *
*/
#define WR_GET_IDX_FROM_ENBIP_ARR(_srcType, _dstIdx)   \
{\
   if(_srcType == smCfgCb.enbIpAddr[WR_IPV4_IDX].type)\
   {\
     _dstIdx = WR_IPV4_IDX;\
   }\
   else\
   if(_srcType == smCfgCb.enbIpAddr[WR_IPV6_IDX].type)\
   {\
     _dstIdx = WR_IPV6_IDX;\
   }\
}   
   
/**
 * @def WR_COPY_TPT_TO_NETADDR_ARR 
 *
 *  Copy source Tpt address array to destination net address array
 *  and update number of addresses copied. 
 *
 * @param[in] _dstNetArr        destination IP address 
 * @param[in] _dstCount         destination Count
 * @param[in] _srcCmtptArr      source IP address
 *
*/
#define WR_COPY_TPT_TO_NETADDR_ARR(_dstNetArr, _dstCount, _srcCmtptArr) \
{\
   S32 _ipIdx = 0;\
   _dstCount = 0;\
   if(_srcCmtptArr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)\
   {\
     wrCpyTptToNetAddr(&_dstNetArr[_ipIdx], &_srcCmtptArr[WR_IPV4_IDX]);\
     _ipIdx++;\
     _dstCount++;\
   }\
   if(_srcCmtptArr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)\
   {\
     wrCpyTptToNetAddr(&_dstNetArr[_ipIdx], &_srcCmtptArr[WR_IPV6_IDX]);\
     _ipIdx++;\
     _dstCount++;\
   }\
}
   
/**
 * @def WR_COPY_TPT_TO_TPT_ARR 
 *
 *  Copy source Tpt address array to destination net address array
 *  and update number of addresses copied.
 *
 * @param[in] _dstCmtptArr      destination IP address
 * @param[in] _dstCount         destination Count
 * @param[in] _srcCmtptArr      source IP address
 *
*/
#define WR_COPY_TPT_TO_TPT_ARR(_dstCmtptArr, _dstCount, _srcCmtptArr) \
{\
   S32 _tptIdx = 0;\
   _dstCount = 0;\
   if(_srcCmtptArr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)\
   {\
     wrCpyCmTptAddr(&_dstCmtptArr[_tptIdx], &_srcCmtptArr[WR_IPV4_IDX]);\
     _tptIdx++;\
     _dstCount++;\
   }\
   if(_srcCmtptArr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)\
   {\
     wrCpyCmTptAddr(&_dstCmtptArr[_tptIdx], &_srcCmtptArr[WR_IPV6_IDX]);\
     _tptIdx++;\
     _dstCount++;\
   }\
}
   
/* Extract the bits from SFN */
#define WR_GET_SFN_MIB(_var, _sfnInt)  \
{\
   U8 _byte = 0;    \
   /* antz - corrected the shifting */ \
   _byte = (U8)((_sfnInt & 0x0003FC) >> 2);    \
   _var = _byte; \
}

/**  
 * @details Macro for SGetMsg.
 */
#define WR_ALLOCMBUF(_region, _pool, _buf)            \
do                                                    \
{                                                     \
   if (SGetMsg((_region), (_pool),                    \
                        (Buffer **)(_buf)) != ROK)    \
   {                                                  \
      (*(_buf)) = NULLP;                              \
   }                                                  \
}                                                     \
while(0)

/**  
 * @details Macro for SPutMsg.
 */
#define WR_FREEMBUF(_mBuf)                            \
do{                                                   \
   if (_mBuf != NULLP)                                \
   {                                                  \
      (Void)SPutMsg((_mBuf));                            \
      (_mBuf) = NULLP;                                   \
   }                                                  \
}while(0)

/* SSI_CHANGES */


/* initialize the memCp & allocate memory for the event structure */
#define WR_ALLOCEVNT(_evntPtr, _size)\
{\
   (*_evntPtr) = NULLP; \
   cmAllocEvnt((Size) _size, WR_MEM_PDU_SIZE, &wrCb.mem,   \
                     (Ptr *)(_evntPtr));  \
}

#define WR_ALLOCEGEVNT(_evntPtr, _size)\
{\
   (*_evntPtr) = NULLP; \
   if(cmAllocEvnt((Size) _size, 2048, &wrCb.mem,   \
                     (Ptr *)(_evntPtr)) != ROK)  \
   {                                            \
      RETVALUE(RFAILED);                         \
   }                         \
}

/* allocate memory for members in the event structure */
#define WR_GET_MEM(_memPtr, _size, _allocPtr)\
{\
   (*_allocPtr) = NULLP;    \
   if(cmGetMem((U8 *)_memPtr, _size, (Ptr *)_allocPtr) != ROK)    \
   {                                                              \
       RETVALUE(RFAILED);                                         \
   }                                                              \
}

/* free allocated memory for an event structure */
#define WR_FREE_EVNT(_evntPtr)  \
{   \
   cmFreeMem((Ptr *)(_evntPtr));   \
   (_evntPtr) = NULLP;  \
}

/* macro to fill TknUInt */ 
#define WR_FILL_TKN_UINT(_ptr, _val) {\
   _ptr.pres = PRSNT_NODEF;\
   _ptr.val = _val;\
}

/* macro to fill TknStrOSXL */ 
#define WR_FILL_TKN_STR_UINT(_ptr, _val, _len) {\
   _ptr->pres = PRSNT_NODEF;\
   _ptr->val = _val;\
   _ptr->len = _len;\
}

#define WR_GET_S1AP_CON_ID(_suConId,_ptr) {\
    _suConId = ((_ptr->cellId & 0xFFFF) << 16)\
               |(_ptr->ueIdx & 0xFFFF);\
}

#define WR_GET_CELLID_FRM_SUCONID(_suConId, _cellId, _ueId) {\
   _cellId = ((_suConId >> 16) & 0xFFFF); \
   _ueId   = ((_suConId & 0xFF)); \
}

#define WR_MEM_COPY(_dst, _src, _len) \
   cmMemcpy((U8*) (_dst), (U8*) (_src), _len);


#define WR_FREEEVNT(_evntPtr)       \
{                                      \
   cmFreeMem((_evntPtr));              \
   (_evntPtr) = NULLP;                 \
}

#define WR_ALLOC(_buf, _size)         \
{\
   if (SGetSBuf(WR_MEM_REGION, WR_MEM_POOL, (Data **)(_buf), _size) == ROK)\
   {                                                         \
      WR_SET_ZERO((*_buf), _size);                           \
   }                                                         \
   else                                                      \
   {                                                         \
      WR_DIAG_LVL0(0x0ff00001, WR_DIAG_NA, SS_DIAG_INV_ARG,\
            "SSI memory allocation of %d \
            bytes %d failed in :%d",_size, __LINE__, 0, 0)   \
      (*_buf) = NULLP;                                       \
   }                                                         \
}

#ifdef SS_LOCKLESS_MEMORY
#define WR_ALLOC_SHAREBLE_BUF(_buf, _size)         \
{\
   if (SGetStaticBuffer(WR_MEM_REGION, WR_MEM_POOL, (Data **)(_buf), \
       _size, SS_SHARABLE_MEMORY) == ROK)\
   {                                                         \
      WR_SET_ZERO((*_buf), _size);                           \
   }                                                         \
   else                                                      \
   {                                                         \
      WR_DIAG_LVL0(0x0ff00001, WR_DIAG_NA, SS_DIAG_INV_ARG,\
            "SSI memory allocation of %d \
            bytes %d failed in :%d",_size, __LINE__, 0, 0)   \
      (*_buf) = NULLP;                                       \
   }                                                         \
}
#else
#define WR_ALLOC_SHAREBLE_BUF(_buf, _size)         \
         {\
         if (SGetSBuf(WR_MEM_REGION, WR_MEM_POOL, (Data **)(_buf), \
               _size) == ROK)\
         {                                                         \
         WR_SET_ZERO((*_buf), _size);                           \
         }                                                         \
         else                                                      \
         {                                                         \
         WR_DIAG_LVL0(0x0ff00001, WR_DIAG_NA, SS_DIAG_INV_ARG,\
            "SSI memory allocation of %d \
            bytes %d failed in :%d",_size, __LINE__, 0, 0)   \
         (*_buf) = NULLP;                                       \
            }                                                         \
            }

#endif


#define WR_SET_ZERO(_buf, _size) \
{\
   cmMemset((U8 *)(_buf), 0, _size);\
}


#define WR_FREE(_buf, _size)          \
{\
   (Void) SPutSBuf(WR_MEM_REGION, WR_MEM_POOL, (Data *) _buf, (Size) _size); \
   (_buf) = NULLP;\
}
#define WR_GET_VALS_RGR_TRANS(_arr, _cfgTyp, _lcId, _crnti)          \
{\
   U16 _tempByte = 0;\
   _cfgTyp = _arr[0];\
   _lcId = _arr[1];\
   _tempByte = _arr[3];\
   _tempByte = ((_tempByte << 8) | _arr[2]);\
   _crnti = _tempByte;\
}

#define WR_SET_TRANS(_arr, _transId)          \
{\
   _arr[0] = (U8) (_transId & 0xff);\
   _arr[1] = (U8) ((_transId & 0x0000ff00) >> 8) ;\
   _arr[2] = (U8) ((_transId & 0x00ff0000) >> 16);\
   _arr[3] = (U8) ((_transId & 0xff000000) >> 24);\
}

#define WR_GET_U32_FRM_OSXL(_var, _tkn) \
{ \
   U16 _len = 0;\
   U16 _idx = 0;\
   U32 _finalVal = 0;\
   _len = _tkn.len;\
   if(_len > 4)\
   _len = 4;\
   for(_idx = 0; _idx < _len; _idx++) \
   {\
      _finalVal = ((_finalVal << 8) | (_tkn.val[_idx])); \
   }\
   _var = _finalVal; \
}

#define WR_GET_UE_X2_IDX(_ueX2apId) (_ueX2apId % WR_MAX_X2AP_CON)
#define WR_CALC_COUNT(_snLen, _sn, _txHfn, _count)         \
{                                                          \
   _count = _sn;                                           \
   _txHfn = _txHfn << _snLen;                               \
   _count |= _txHfn;                                       \
} 

#define WR_LOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   { \
    SLogError(wrCb.init.ent, wrCb.init.inst, wrCb.init.procId,     \
          (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
          (Txt *)errDesc); \
   } \
}while(0)

#define WR_FILL_BIT_STR(_ptr, _len, _val, _pdu) {\
   U16     _tmpLen;                             \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   WR_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      *_ptr.val  =  _val;           \
      *_ptr.val    =   (*_ptr.val)  <<    (_tmpLen*8 - _len);  \
   } \
}

#define WR_FILL_BIT_STR_VAL(_ptr, _len, _val, _pdu) {\
   S16      i;                                    \
   U16     _tmpLen;                             \
   U32    _val2= _val; \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   WR_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = _tmpLen - 1; i >= 0; i--)                  \
      {                                          \
         _ptr.val[i] = (U8)(0x000000ff & _val2);                  \
         _val2 = _val2>>8;\
      }                                          \
   }                                             \
}

/*!< Fill bit string array based on length.
 * Here len is in bits.
 */
#define WR_FILL_BIT_STR_ARR(_ptr, _len, _val, _pdu) {\
   S16      i;                                    \
   U16     _tmpLen;                             \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   WR_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = _tmpLen - 1; i >= 0; i--)                  \
      {                                          \
         _ptr.val[i] = _val[i];                  \
      }                                          \
   }                                             \
}					        		

#ifdef FLAT_BUFFER_OPT
#define WR_FREEMFLATBUF(_buf) \
{ \
   (Void) SPutStaticBuffer(WR_MEM_REGION, WR_MEM_POOL,   \
            (Data *) (_buf)->startAddr, (Size)((_buf)->ptr - (_buf)->startAddr + (_buf)->len) , 0); \
}
#endif
 
#define WR_ANTENA_TX_MODE_1 NhuAntennaInfoDedicatedtransmissionModetm1Enum 
#define WR_ANTENA_TX_MODE_2 NhuAntennaInfoDedicatedtransmissionModetm2Enum 
#define WR_ANTENA_TX_MODE_3 NhuAntennaInfoDedicatedtransmissionModetm3Enum 
#define WR_ANTENA_TX_MODE_4 NhuAntennaInfoDedicatedtransmissionModetm4Enum 
#define WR_ANTENA_TX_MODE_5 NhuAntennaInfoDedicatedtransmissionModetm5Enum 
#define WR_ANTENA_TX_MODE_6 NhuAntennaInfoDedicatedtransmissionModetm6Enum 
#define WR_ANTENA_TX_MODE_7 NhuAntennaInfoDedicatedtransmissionModetm7Enum 
#define WR_ANTENA_TX_MODE_8 NhuAntennaInfoDedicatedtransmissionModetm8_v920Enum 

#define FILL_ANT_TX_MODE(_txmode,_cfg) {\
switch(_txmode)\
{\
   case WR_ANTENA_TX_MODE_1:                \
      _cfg->txModeEnum   = RGR_UE_TM_1;       \
      break;                    \
   case WR_ANTENA_TX_MODE_2:                \
      _cfg->txModeEnum   = RGR_UE_TM_2;       \
      break;                     \
   case WR_ANTENA_TX_MODE_3:                \
      _cfg->txModeEnum   = RGR_UE_TM_3;       \
      break;                     \
   case WR_ANTENA_TX_MODE_4:                \
      _cfg->txModeEnum   = RGR_UE_TM_4;       \
      break;                      \
   case WR_ANTENA_TX_MODE_5:                \
      _cfg->txModeEnum   = RGR_UE_TM_5;       \
      break;                       \
   case WR_ANTENA_TX_MODE_6:                \
      _cfg->txModeEnum   = RGR_UE_TM_6;       \
      break;    \
   case WR_ANTENA_TX_MODE_7:                \
      _cfg->txModeEnum   = RGR_UE_TM_7;       \
      break;    \
}\
}
/*TM4 changes start*/

#define WR_GET_UMM_TM(_txmode,_mode) {\
switch(_txmode-1)\
{\
   case WR_ANTENA_TX_MODE_1:                \
      _mode   = CTF_UE_TX_ANT_MODE_TM1;       \
      break;                    \
   case WR_ANTENA_TX_MODE_2:                \
       _mode   = CTF_UE_TX_ANT_MODE_TM2;       \
      break;                     \
   case WR_ANTENA_TX_MODE_3:                \
       _mode   = CTF_UE_TX_ANT_MODE_TM3;       \
      break;                     \
   case WR_ANTENA_TX_MODE_4:                \
       _mode   = CTF_UE_TX_ANT_MODE_TM4;       \
      break;                      \
   case WR_ANTENA_TX_MODE_5:                \
       _mode   = CTF_UE_TX_ANT_MODE_TM5;       \
      break;                       \
   case WR_ANTENA_TX_MODE_6:                \
       _mode  = CTF_UE_TX_ANT_MODE_TM6;       \
      break;    \
   case WR_ANTENA_TX_MODE_7:                \
       _mode   = CTF_UE_TX_ANT_MODE_TM7;       \
      break;    \
}\
}

#define WR_GET_RGR_TM(_txmode,_mode) {\
switch(_txmode)\
{\
   case WR_ANTENA_TX_MODE_1:                \
      _mode   = RGR_UE_TM_1;       \
      break;                    \
   case WR_ANTENA_TX_MODE_2:                \
      _mode = RGR_UE_TM_2;       \
      break;                     \
   case WR_ANTENA_TX_MODE_3:                \
      _mode   = RGR_UE_TM_3;       \
      break;                     \
   case WR_ANTENA_TX_MODE_4:                \
      _mode   = RGR_UE_TM_4;       \
      break;                      \
   case WR_ANTENA_TX_MODE_5:                \
      _mode   = RGR_UE_TM_5;       \
      break;                       \
   case WR_ANTENA_TX_MODE_6:                \
      _mode   = RGR_UE_TM_6;       \
      break;    \
   case WR_ANTENA_TX_MODE_7:                \
      _mode   = RGR_UE_TM_7;       \
      break;    \
}\
}
#if 0

#define WR_GET_RGR_TM_UE(_txmode,_mode) {\
switch(_txmode)\
{\
   case RGR_UE_TM_1:                \
      _mode  = RGR_UE_TM_1;       \
      break;                    \
   case RGR_UE_TM_2:                \
      _mode  = RGR_UE_TM_2;       \
      break;                     \
   case RGR_UE_TM_3:                \
      _mode   = RGR_UE_TM_3;       \
      break;                     \
   case RGR_UE_TM_4:                \
      _mode    = RGR_UE_TM_4;       \
      break;                      \
   case RGR_UE_TM_5:                \
      _mode    = RGR_UE_TM_5;       \
      break;                       \
   case RGR_UE_TM_6:                \
      _mode    = RGR_UE_TM_6;       \
      break;    \
   case RGR_UE_TM_7:                \
      _mode    = RGR_UE_TM_7;       \
      break;    \
}\
}
#endif
#define WR_GET_NHU_TM(_txMode,_mode) {\
switch(_txMode-1)\
{\
   case NhuAntennaInfoDedicatedtransmissionModetm1Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm1Enum;       \
      break;                    \
   case NhuAntennaInfoDedicatedtransmissionModetm2Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm2Enum;       \
      break;                     \
   case NhuAntennaInfoDedicatedtransmissionModetm3Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm3Enum;       \
      break;                     \
   case NhuAntennaInfoDedicatedtransmissionModetm4Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm4Enum;       \
      break;                      \
   case NhuAntennaInfoDedicatedtransmissionModetm5Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm5Enum;       \
      break;                       \
   case NhuAntennaInfoDedicatedtransmissionModetm6Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm6Enum;       \
      break;    \
   case NhuAntennaInfoDedicatedtransmissionModetm7Enum:                \
      _mode   = NhuAntennaInfoDedicatedtransmissionModetm7Enum;       \
      break;    \
}\
}
/*TM4 changes end*/

/* CSG_DEV start */
EXTERN U16  wrPciRange[14];
#define WR_EMM_GET_PCI_RANGE(pciRangeEnum, range) \
{\
   if(pciRangeEnum < 13)\
   {\
      range = wrPciRange[pciRangeEnum];\
   }\
   else\
   {\
      range = wrPciRange[0];\
   }\
}\

#define WR_S1AP_CSG_ID_LEN   27        /* Bits */
#define WR_CSG_ID_MAX_VAL    134217727 /* max 27 bit value (7FFFFFF) */
#define WR_PCI_MIN_VAL       0
#define WR_PCI_MAX_VAL       503
/* CSG_DEV end */

typedef U8                   WrCellBw;

#define WR_BANDWIDTH_6RBS    0
#define WR_BANDWIDTH_15RBS   1
#define WR_BANDWIDTH_25RBS   2
#define WR_BANDWIDTH_50RBS   3
#define WR_BANDWIDTH_75RBS   4
#define WR_BANDWIDTH_100RBS  5

#define WR_CAUSE_RADIONW     0
#define WR_CAUSE_TRANSPORT   1
/* ccpu00127852,ccpu00128532-spec compliant order */
#define WR_CAUSE_NAS         2
#define WR_CAUSE_PROTOCOL    3
#define WR_CAUSE_MISC        4

/*!< Max of IPv4 and IPv6 address length in string notation:
  As per internet protocol standard in in.h 
  INET_ADDRSTRLEN is defined as 16 
  INET6_ADDRSTRLEN is defined as 46
 */
#define MAX_IPADDR_LEN 46

/* max 32 UTRA Cells per Freq (Max Freq can be 16)*/ 
#define MAX_NBR_UTRA_CELL    32 
#define MAX_NBR_EUTRA_FREQ   16
#define MAX_NBR_UTRA_FREQ    16
#define WR_SMM_MAX_CDMA_1X_BAND_CLASS 32
#define WR_SMM_MAX_CDMA_1X_NEIGH_FREQ 16
#define WR_SMM_MAX_CDMA_1X_NEIGH_CELL 40

/* MAX Broadcast PLMNs */
#define WR_MAX_BRDCST_PLMN  6    /* Max. nos of PLMNs that can be broadcasted by the eNodeB in SIB 1 */

/*#ifdef TENB_AS_SECURITY*/
#define WR_MAX_AS_SEC_ALGOS   4
/*#endif*/

/* LTE_ADV starts */
#define WR_LTE_ADV_PICO_ENB   1
#define WR_LTE_ADV_MACRO_ENB  0
/* LTE_ADV ends */

/*rel10*/
#define WR_MAX_UTRA_CELLS_RPTD_PER_RPT 8
#define WR_MAX_U16_VALUE               65535
#define WR_MAX_U8_VALUE                0xFF

/** @brief Enum holds the value of the Cell States
*/
typedef enum WrCellState
{
   WR_CELL_STATE_INIT = 1,
   WR_CELL_STATE_RESET,
   WR_CELL_STATE_UP,
   WR_CELL_STATE_PWR_DOWN,
   WR_CELL_STATE_RECFG
}WrCellState;

/** @brief Enum holds the value for different Rat TYPES
*/
typedef enum
{
   UTRA_FDD,
   UTRA_TDD,
   GERAN,
   CDMA2000_HRPD,
   CDMA2000_1XRTT,
   MAX_RAT_TYPE
}wrCsfbRatType;

/* Enum to identify the Type of CS fallback triggered */
typedef enum
{
   CDMA2000_CSFB_WITHOUT_REDIRECTION = 1,
   CDMA2000_CSFB_WITH_REDIRECTION,
   CDMA2000_CSFB_WITHOUT_PS_HO,
   CDMA2000_CSFB,
   CDMA2000_CSFB_WO_PSHO_FAIL_REDIR_SUCC,
   CDMA2000_CSFB_DUALRXTX_REDIR,        
   CDMA2000_CSFB_DUALRXTX_DLNAS,        
   GERAN_CSFB,
   GERAN_CSFB_CCO, 
   GERAN_CSFB_CCO_WITH_NACC, 
   GERAN_CSFB_WITH_REDIRECTION, 
   GERAN_CSFB_WITHOUT_REDIRECTION, 
   GERAN_CSFB_WITH_REDIRECTION_WITH_SI,
   UTRA_CSFB,
   UTRA_CSFB_WITHOUT_REDIRECTION, 
   UTRA_CSFB_WITH_REDIRECTION_WITH_SI,
   UNKNOWN_CSFB_REDIRECTION_TYPE
}wrCsfbReDirType;

/* ZUC_ALGO - start */
#define WR_UMM_SNOW3G_ALGO_BITMASK 0x80
#define WR_UMM_AES_ALGO_BITMASK    0x40
#define WR_UMM_ZUC_ALGO_BITMASK    0x20
/* ZUC_ALGO -  end */

typedef enum
{
  WR_UMM_NULL_ALGO,
  WR_UMM_SNOW3G_ALGO,
  WR_UMM_AES_ALGO,
  WR_UMM_ZUC_ALGO
} WrUmmAsSecAlgo;

typedef struct wrRatPriorityList{
   U8          numberOfRats;
   wrCsfbRatType  rat[MAX_RAT_TYPE];
}WrRatPriorityList;

/** @brief This Enum used for measurement Cell Type.
 */
typedef enum wrUmmMeasCellTyp
{
  WR_UMM_MEAS_INTRA_FREQ,          /*!< Intra */
  WR_UMM_MEAS_INTER_FREQ,          /*!< Inter Frequency*/
  WR_UMM_MEAS_1XCDMA,              /*!< 1xCdma */
  WR_UMM_MEAS_CDMA_HRPD,           /*!< Cdma HRPD */
  WR_UMM_MEAS_UTRA_FDD_FREQ,       /*!< Utra FDD Frequency */
  WR_UMM_MEAS_UTRA_TDD_FREQ,       /*!< Utra TDD Frequency */
  WR_UMM_MEAS_GERAN_FREQ,          /*!< GERAN Frequency */
  WR_UMM_MEAS_NONE                 /*!< NONE this is for handling failure condition */
}WrUmmMeasCellTyp;

#define WR_UMM_MEAS_MAX_GERAN_ARFCN 16 
/* @brief This structure used to maintain GERAN meas obj
 * */
typedef struct wrUmmMeasGeranMeasInfo
{
   U16     arfcn;/*BCCH ARFCN */
   U8      bsic;/*BSIC */
   U8      nccPermitted; /* OR nccPermitted of all the cells*/
   U8      bandIndicator; /*BCCH ARFCN bandindicator*/
   S8      qOffset;       /*Q-offset GERAN*/
}WrUmmMeasGeranMeasInfo;
/* @brief This Enum used for identifying
 * mobility type and RAT type
 * */
typedef enum wrMobilityType 
{
   WR_INTRA_SYS_MEAS,
   WR_INTRA_SYS_MBLTY,
   WR_INTER_SYS_UTRA_MBLTY,
   WR_INTER_SYS_GERAN_MBLTY
}WrMobilityType;

/* @brief This Enum is to identify the 
 * decision taken by the HRL algorithm
 * */
typedef enum wrHrlDecisionRet
{
   WR_MEAS_MBLTY_APPL_TO_CELL = 0,  /* Cell is not forbidden/restricted for mobility */
   WR_MEAS_MBLTY_NOT_APPL_TO_CELL,  /* Cell is forbidden/restricted for mobility */
   WR_MEAS_MBLTY_APPL_TO_RAT,       /* RAT is not forbidden/restricted for mobility */
   WR_MEAS_MBLTY_NOT_APPL_TO_RAT    /* RAT is forbidden/restricted for mobility */
}WrHrlDecisionRet;

/* @brief This Enum is to identify the
 * RAT type.
 * WR_RAT_EUTRA  -  To check whether EUTRA is forbidden or not.
 * WR_FORB_RAT_UTRA - To check whether UTRA is forbidden or not.
 * WR_FORB_RAT_CDMA - To check whether CDMA is forbidden or not.
 * WR_FORB_RAT_GERAN - To chekc whether GERAN is forbidden or is forbidden or not.
 * */
typedef enum wrForbRatTyp
{
   WR_RAT_EUTRA = 0,
   WR_FORB_RAT_UTRA,
   WR_FORB_RAT_CDMA,   
   WR_FORB_RAT_GERAN  
}WrForbRatTyp;

/** @brief This structure contains the cause value and type that is to be
 *         in S1AP and X2AP
 *
-  U32   causeTyp       Cause Type
-  U32   causeVal       Cause Value
*/
typedef struct wrUmmMsgCause
{
   U32                       causeTyp;
   U32                       causeVal;
} WrUmmMsgCause;

/* MCC and MNC are digits taking values 0 to 9. Each element represents */
/* a digit that takes the same range                                    */

/** @brief This structure contains PLMN ID 
-  U8    numMncDigits   Number of Digits in MNC
-  U8    mcc[3]         3 Octets of MCC value
-  U8    mnc[3]         2/3 Octets of MNC value
*/
typedef struct wrPlmnId
{
   U8                        numMncDigits;
   U8                        mcc[3];
   U8                        mnc[3];
} WrPlmnId;

/** @brief This structure contains EUTRAN Global Cell Identity
-  WrPlmnId  plmnId         PLMN Identity
-  U32       eutranCellId   EUTRAN Cell Identity
*/
typedef struct wrEcgi
{
   WrPlmnId                  plmnId; /* 24 bit long */
   U32                       eutranCellId; /* 28 bit long */
} WrEcgi;

/** @brief This structure holds the value of S-TMSI for a UE.
-  Bool    pres      Presence field
-  U8      mmec      MME code
-  U32     mTMSI     m-TMSI value
*/
typedef struct wrSTmsi
{
   Bool                      pres;
   U8                        mmec;
   U32                       mTMSI;
} WrSTmsi;

/** @brief This structure holds the value of GUMMIE.
-  Bool      pres       Presence field
-  WrPlmnId  plmnId     PLMN ID
-  U16       mmeGrpId   MME Group Id
-  U8        mmec       MME Group Code
 */
typedef struct  wrGummei 
{
   Bool                      pres;
/*ccpu00125227: Adding Presenceflag for plmn-identity also*/
   Bool                      plmn_identity_pres;
   WrPlmnId                  plmnId;
   U16                       mmeGrpId; 
   U8                        mmec;
}WrGummei;  

/** @brief This structure holds the value of the Bit rates for DL and UL
-  U32  dl     Downlink Bit rate
-  U32  ul     Uplink Bit rate
*/
typedef struct wrBitRate
{
   U32                       dl;
   U32                       ul;
} WrBitRate;

#define WR_PLMNID_IE_LEN     3          /* Octets */
#define WR_LAC_IE_LEN        2          /* Octets */
#define WR_RAC_IE_LEN        1          /* Octets */
#define WR_CI_IE_LEN         2          /* Octets */

#define WR_EXT_RNC_START     4096      /* start Ext RNC id */

#define RB_DIR_UL            1
#define RB_DIR_DL            2
#define RB_DIR_BI            RB_DIR_UL | RB_DIR_DL
typedef U32                  WrRbDir;

typedef U32                  WrMmeId;
typedef CmLteCellId          WrCellId;

/** @brief This structure holds the value of Cell Global Identity
-  WrPlmnId  plmnId     PLMN Identity
-  U32       cellId     Cell Identity
*/
typedef struct wrCgi
{
   WrPlmnId                  plmnId;
   U32                       cellId;
} WrCgi;

/** @brief This structure holds the value of the Tracking Area Identity
-  WrPlmnId plmnId   PLMN Identity
-  U16      tac      Tracking Area Code
*/
typedef struct wrTai
{
   WrPlmnId                  plmnId;
   U16                       tac;
} WrTai;

/* Integrity and Ciphering Key Length */
#define WR_INTG_KEY_LEN                16
#define WR_CIPH_KEY_LEN                16
#define WR_KENB_KEY_LEN                32

/** @brief This structure holds the value of Logical channel configuration
-  U8    priority       Priority of the Logical Channel
-  U32   pbr            Prioritized bit rate
-  U32   bucketSzDur    Bucket size duration
-  U8    lcg            Logical channel group 
*/
typedef struct wrLchChnlCfg
{
   U8                        priority;
   U32                       pbr;
   U32                       bucketSzDur;
   U8                        lcg;
} WrLchChnlCfg;

/** @brief This structure holds the UE's capability information
-  NhuUE_EUTRA_Cap eUTRA_Cap       Decoded E-UTRA Capability Information
-  TknStrOSXL      ueEutraOctStr   E-UTRA Capability Octet String 
-  TknStrOSXL      ueUtraOctStr    UTRA Capability Octet String 
-  TknStrOSXL      ueGeranCsOctStr GERAN CS Capability Octet String 
-  TknStrOSXL      ueGeranPsOctStr GERAN PS Capability Octet String
-  TknStrOSXL      ueCdmaOctStr    CDMA 2000 Capability String
*/
typedef struct wrUeCapInfo
{
   CmMemListCp               memCp;
   NhuUE_EUTRA_Cap           eUTRA_Cap; /* decocde eUTra Cap Info */
   TknStrOSXL                ueEutraOctStr; /* eUtra cap Oct Str */
   TknStrOSXL                ueUtraOctStr; /* Utra cap oct str */
   TknStrOSXL                ueGeranCsOctStr; /* GERAN CS cap oct str */
   TknStrOSXL                ueGeranPsOctStr; /* GERAN PS cap oct str */
   TknStrOSXL                ueCdmaOctStr; /* CDMA 2000 cap str */
   /* SPS changes starts */
   /* This is encoded separately. 
    * Will be present only if NhuUE_EUTRA_Cap_v940_IEslateNonCriticalExtn is present */
   NhuUE_EUTRA_Cap_v9a0_IEs  eUTRA_Cap_v9a0;
   /* SPS changes ends */
} WrUeCapInfo;

typedef U16                  WrTac;
typedef U16                  WrLac;
/** @brief This structure contains the Forbidden Tracing Area Infomation */
typedef struct wrForbiddenTAs
{
   WrPlmnId    plmnId;   /*!< PLMN ID */
   U16         noOfTac;  /*!< Number of Tracking Area Codes */
   WrTac       *tac;    /*!< Tracking Area Code */
} WrForbiddenTAs;

/** @brief This structure contains the Forbidden Location Area Information */
typedef struct wrForbiddenLAs
{
   WrPlmnId    plmnId;   /*!< PLMN ID */
   U16         noOfLac;  /*!< Number of Location Area Codes */
   WrLac       *lac;    /*!< Location Area Code */
} WrForbiddenLAs;

typedef struct wrHoRstrctInfo
{
   CmMemListCp        memCp;         /*!< Added for chained allocation */
   WrPlmnId          srvngPlmn;      /*!< Serving PLMN */
   TknU32            frbdnInterRATs; /*!< Forbidden Inter RATs */
   U8                noOfEplmn;      /*!< Number of Equivalent PLMNs */
   U8                noOfTAs;        /*!< Number of forbidden Tracking Areas */
   U8                noOfLAs;        /*!< Number of forbidden Location Areas */
   WrPlmnId          *ePlmn;         /*!< Equivalent PLMNs */
   WrForbiddenTAs    *frbdnTAs;      /*!< Forbidden Tracking Areas */
   WrForbiddenLAs    *frbdnLAs;      /*!< Forbidden Location Areas */
} WrHoRstrctInfo;

typedef struct wrPLMNs
{
   Bool     isPLMNForb;       /*!< Identify whether the PLMN os forbidden or not */
   WrPlmnId    plmnId;   /*!< PLMN ID */
} WrPLMNs;

typedef struct wrSelctdPLMN
{
   U8          nosOfPlmns;                      /*!< Nos. of selected PLMNs */
   WrPLMNs     selectdPLMN[WR_MAX_BRDCST_PLMN]; /*!< PLMNs selected for the UE */
}WrSelctdPLMN;

typedef struct wrHrlCb
{
   U8          nosOfPlmns;                      /*!< Nos. of PLMNs to validate */
   WrPlmnId    plmnToVldt[WR_MAX_BRDCST_PLMN];  /*!< PLMNs to be validated for HRL */
   U16         tacOrLacToVldt;                  /*!< TAC or LAC to validate for HRL */
}WrHrlCb;

/* RLC Mode macros */
#define WR_RLC_AM           0
#define WR_RLC_UM_BI_DIR    1
#define WR_RLC_UM_UL        2
#define WR_RLC_UM_DL        3

/** @brief This structure holds the value of RLC's Acknowledged mode
 * configuration 
-  U32  tPollRetransmit       Poll Retransmit timer value
-  U32  pollPdu               Number of Poll PDU
-  U32  pollByte              Number of Poll Byte
-  U32  maxRetxThreshold      Maximum Retransmission Threshold
-  U32  tReordering           Reordering timer  value
-  U32  tStatusProhibit       Status prohibit timer value
*/
typedef struct wrRlcAmCfg
{
   U32                  tPollRetransmit;
   U32                  pollPdu;
   U32                  pollByte;
   U32                  maxRetxThreshold;
   U32                  tReordering;
   U32                  tStatusProhibit;
} WrRlcAmCfg;

/** @brief This structure holds the value of the RLC's Unacknowledged mode
 * configuration
-  U32  dlSnLen      Downlink Sequence number length 
-  U32  ulSnLen      Uplink Sequence number length
-  U32  tReordering  Reordering timer value
*/
typedef struct wrRlcUmCfg
{
   U32                  dlSnLen;
   U32                  ulSnLen;
   U32                  tReordering;
} WrRlcUmCfg;

/** @brief This structure holds the value of the RLC configuration
-  U8             mode     Mode of the entity i.e. UM/AM
-  union
-  {
-     WrRlcAmCfg  amCfg    AM mode configuration
-     WrRlcUmCfg  umCfg    UM mode configuration
-  } u                     Union of AM/UM
*/
typedef struct wrRlcCfg
{
   U8                        mode;
   union
   {
      WrRlcAmCfg             amCfg;
      WrRlcUmCfg             umCfg;
   } u;
} WrRlcCfg;

/** @brief This structure holds the PDCP configuration
-  Bool   statusRptReq   TRUE if status Report is needed
-  Bool   hdrCmpReq      TRUE if Header compression is needed
-  U32    discardTmr     Data discard timer
-  U32    snSize         Sequence number size
-  U16    maxCid         Maximum CID
-  U16    rohcProfid     ROHC profile ID
*/
typedef struct wrPdcpCfg
{
   Bool                      statusRptReq;
   Bool                      hdrCmpReq;
   U32                       discardTmr;
   U32                       snSize;
   U16                       maxCid;
   U16                       rohcProfid;
} WrPdcpCfg;

/**
 * @brief This structure contains connId and Type of connId 
 * @details These are the structure members
 * -U8                         type      WR_S1AP_ENB_ID or WR_S1AP_MME_ID 
 *                                         
 * -U32                        ConnId    S1AP MME/ENB ID 
 */
typedef struct wrEmmS1ConId
{
   U8      type;
   U32     conId;
}WrEmmS1ConId;

/** @brief This structure contains the Location Area Identity
 * - Bool       isLaiPres whether LAI field is received or not
 *  - WrPlmnId  plmnId  PLMN Identity
 *   - U16       lac     Location Area Code
 *    */
typedef struct wrRegLai
{
      Bool                      isLaiPres;
      WrPlmnId                  plmnId;
      U16                       lac;
} WrRegLai;

/* IRAT_HO */
typedef struct wrUmmIntraLteHoNghInfo
{
   U16        pci;
   U32        earfcn;
   U32        eutranCellId;
   CmTptAddr  neighEnbAddr;
   U8         isX2Connected;
   U8         enbType;
   U16        tac;
   PTR        neighEnbCb;
} WrUmmIntraLteHoNghInfo;

typedef struct wrUmmIRatUtraHoNghInfo
{
  U16                           psc;
  U32                           arfcn;
  U16                           rncId;
  U16                           cId;
  U16                           lac;
  U8                            rac;
  WrPlmnId                      plmnId;
  TknU32                        srvccHoInd;
} WrUmmIRatUtraHoNghInfo;

typedef struct wrUmmIRatGeranHoNghInfo
{
  Bool                          psSrvcNotAvlbl;
  U32                           rssi;
  TknU32                        srvccHoInd;
} WrUmmIRatGeranHoNghInfo;
/**
 * @brief This structure contains information needed to be fetched from
 *        EMM, to fill Handover Request. 
 * @details These are the structure members
 * - U16        pci            Physical Channel Identity of the Neighbour
 * - U32        earfcn         Frequency of the Neighbour 
 * - U32        peerId         Id of X2 connection with the Neighbour
 * - U32        eutranCellId   ENB ID of the Neighbour
 * - WrPlmnId   plmnId         PLMN ID of the Neighbour
 * - CmTptAddr  neighEnbAddr   IP Address of the Neighbour
 * - PTR        neighEnbCb     Neighbour Control Block
 * - PTR        neighCellCb    Neighbour Cell Control Block
 */
typedef struct wrHoNghInfo
{
   U32        peerId;
   WrPlmnId   plmnId;
   PTR        neighCellCb;
   U8         hoType;
/* CSG_DEV HO Start */
   U8         isCsgInfoDiscovered;
   U32        csgId;
   U8         csgCellAccessMode;
   /* CSG_DEV HO Ends */
/* IRAT_HO */
   union
   {
     WrUmmIntraLteHoNghInfo intraLteNghInfo;
     WrUmmIRatUtraHoNghInfo iRatUtraNghInfo;
     WrUmmIRatGeranHoNghInfo iRatGeranNghInfo;
   }t;
} WrHoNghInfo;

#define WR_UMM_MAX_INTER_ARFCN 32
#define WR_UMM_MAX_UTRAN_ARFCN 32
#define WR_UMM_MAX_GERAN_ARFCN 16 
#define WR_MAX_GERAN_SI        10 
/** @brief This structure used to store Anr measurement Frequency in Cell
 * Control block.

 - TknU32                      intraFreq         Intra frequency  
 - U8                          interArfcnCnt     Inter Frequency Arfcn count 
 - U32                         interArfcn[WR_UMM_MAX_INTER_ARFCN] inter frequency Arfcn list
 - U8                          utranArfcnCnt     Utra Frequency Arfcn count
 - U32                         utranArfcn[WR_UMM_MAX_INTER_ARFCN] utra Frequency list
*/ 
typedef struct wrUmmAnrMeasInfo
{
   TknU32                      intraFreq;
   U8                          interArfcnCnt;
   U32                         interArfcn[WR_UMM_MAX_INTER_ARFCN];
   U8                          utranArfcnCnt;
   U32                         utranArfcn[WR_UMM_MAX_INTER_ARFCN];
/* Inter-Freq ANR: Start */
   WrUmmMeasCellTyp            currAnrCfgType;
   U8                          currAnrInterFreqCnt;
/* Inter-Freq ANR: End */
} WrUmmAnrMeasInfo;

#define WR_UMM_MEAS_INVALID_MEAS_ID  0xFF

/* LTE_ADV starts */
typedef struct _wrEmmlteAdvFeatureActDeact
{
    RgrFeature   feature;          /*! < Feature to enable (ABS, SFR, DSFR, RE) */
    RgrFeatureStatus flag;           /*! < To enable or disable the feature */
} WrEmmlteAdvFeatureActDeact;
/* LTE_ADV ends */


 /** @brief This structure used to store UTRA FDD cell information like Psc Id and
 * Power related information.

  -  U16        pscId      PSC
  -  S16        rscp       RSCP power
  -  U8         ecno       ECNO power
*/ 
typedef struct wrUmmUtraCellRedirInfo
{
    U16                       pscId;  /* PSC */
    S16                       rscp;   /* CPICH_RSCP */
    U8                        ecno;   /* CPICH_ECNO */
    TknStrOSXL                utraSI; /* SI information from NRT */
} WrUmmUtraCellRedirInfo;

 /** @brief This structure used to store UTRA TDD cell information like Psc Id and
 * Power related information.

  -  U16        pscId      PSC
  -  S16        rscp       RSCP power
  -  U16        arfcn      ARFCN of the cell
*/ 
typedef struct wrUmmUtraTddCellRedirInfo
{
    U16                       pscId;  /* PSC */
    S16                       rscp;   /* CPICH_RSCP */
    U16                       arfcn;  /* ARFCN */
    TknStrOSXL                utraSI; /* SI information from NRT */
} WrUmmUtraTddCellRedirInfo;

#define WR_UMM_MAX_UTRA_TDD_ARFCN_REL10  6
#define WR_UMM_MAX_UTRA_CELLS_WITH_SI    16
/** @brief This structure used to store candidate cell information in UE control
 *  *  block.
 *
 *  -  U16                 arfcn          Arfcn
 *  -  U8                  noOfPscs       Number of PSC's
 *  -  WrUmmUtraCellInfo   utraCellInfo  Utra cell power information
 **/
typedef struct wrUmmUtraRedirectInfo
{
   U8                        duplexTyp;
   U16                       arfcn;
   U8                        noOfPscs;
   TknPres                   pres;
   WrUmmUtraCellRedirInfo    utraCellRedirInfo[WR_UMM_MAX_UTRA_CELLS_WITH_SI];
} WrUmmUtraRedirectInfo;


/** @brief This structure used to store candidate cell information in UE control
 *  *  block.
 *
 *  -  U16                 arfcn          Arfcn
 *  -  U8                  noOfPscs       Number of PSC's
 *  -  WrUmmUtraCellInfo   utraCellInfo  Utra cell power information
 **/
typedef struct wrUmmUtraTddRedirectInfo
{
   Bool                      eRedirUtraR10Sup;
   U8                        noOfPscs;
   U8                        noOfTddArfcn;
   U32                       arfcnTddLst[WR_UMM_MAX_UTRA_TDD_ARFCN_REL10];
   WrUmmUtraTddCellRedirInfo    utraCellRedirInfo[WR_UMM_MAX_UTRA_CELLS_WITH_SI];
} WrUmmUtraTddRedirectInfo;

/** @brief This structure used to store candidate cell information in UE control
 *  *  block.
 *
 *  -  U16                 arfcn          Arfcn
 *  -  U8                  noOfPscs       Number of PSC's
 *  -  WrUmmUtraCellInfo   utraCellInfo  Utra cell power information
 **/
typedef struct wrUmmEutraRedirectInfo
{
   U16                       arfcn;
} WrUmmEutraRedirectInfo;

/** @brief This structure used to store candidate cell information in UE control
 *  *  block.
 *
 *  -  U16                        arfcn          Arfcn
 *  -  LwrCdma2kBandClassType     bandClass
 **/
typedef struct wrUmmCdmaRedirectInfo
{
   U16                       arfcn;
   LwrCdma2kBandClassType    bandClass;
} WrUmmCdmaRedirectInfo;
typedef  TknStrOSXL* WrSiInfoGeran[WR_MAX_GERAN_SI]; /*octet string of length 23 and a max of 10 Geran SIs*/

typedef struct wrCellInfoListGeran
{

   U8              bsic;      /* PhysCellIdGeran/Base Station Identifier Code: 
                                         bits 7 & 6 = 0, bits 5 4 3 = NCC, 
                                         bits 2 1 0 = BCC*/
   
   U16            arfcn;              /*BCCH ARFCN*/
   U8             noOfsiPsi;/*no. of SIs or PSIs*/
   WrSiInfoGeran  siListGeran; /*octet string of length 23 and a max of 10 Geran SIs*/ 
}WrCellInfoListGeran;
/** @brief This structure is used to store the target frequency information
 *  - LwrGeranBand      bandIndicator : GERAN frequency band indicator
 *  - U16               arfcn[WR_UMM_MAX_GERAN_ARFCN] : 16 BCCH ARFCNs
 * */
typedef struct wrUmmGeranRedirectInfo
{
   WrCellInfoListGeran cellInfoList[WR_UMM_MAX_GERAN_ARFCN];/*16 cell Infos*/ 
   U8                  cellInfoCnt;   
   U8                  bandInd;            /*Band Indicator: GSM 850, GSM 900, 
                                           DCS 1800, PCS 1900*/
}WrUmmGeranRedirectInfo;

/** @enum wrUmmTmrVal
 *
 *  Different types of Redirection possible. If it is CSFB redirection or just
 * release due to redirection 
   WR_UMM_REDIRECTION,
   WR_UMM_CSFB
 */
typedef enum wrUmmRedirectType
{
   WR_UMM_REDIRECTION,
   WR_UMM_CSFB
}WrUmmRedirectType;

/** @brief This structure used to store candidate cell information in UE control
 *  *  block.
 *
 *   -  WrUmmMeasCellTyp      cellTyp      Cell type
 *   -  U8                    duplexTyp    Duplex Type
 *   -  WrUmmEutraRedirectInfo  eutraInfo  eutra Redirect information
 *   -  WrUmmUtraRedirectInfo   utraInfo   Utra Redire information
 *      WrUmmGeranRedirectInfo  geranInfo  GERAN Recidrection information 
 **/
typedef struct wrUmmRedirectInfo
{
   U16                       cellId;
   U32                       eRedirGeranR9Sup;
   Bool                      eRedirUtraR9Sup;
   WrUmmMeasCellTyp          ratTyp;
   WrUmmRedirectType         redirType;
   union
   {
      WrUmmEutraRedirectInfo  eutraInfo;
      WrUmmUtraRedirectInfo   utraInfo;
      WrUmmUtraTddRedirectInfo   utraTddInfo;
      WrUmmCdmaRedirectInfo   cdmaInfo;
      WrUmmGeranRedirectInfo  geranInfo;
   }val;
} WrUmmRedirectInfo;

 /** @enum WrUmmUeEstablishCause
  *
  * Establishment cause received in the RRC connection setup request.
  * This should be same as defined for establishmentCause (RRCConnectionRequest
  * IE) in TS 36.331.
  *
  * - WR_UMM_EMERGENCY         
  * - WR_UMM_HIGHPRIORACCESS   
  * - WR_UMM_MTACCESS          
  * - WR_UMM_MOSIGNALLING      
  * - WR_UMM_MODATA            
  */
  typedef enum wrUmmUeEstablishCause
  {
     WR_UMM_EMERGENCY,
     WR_UMM_HIGHPRIORACCESS,
     WR_UMM_MTACCESS,
     WR_UMM_MOSIGNALLING,
     WR_UMM_MODATA,
     WR_UMM_HO_CALL
  }WrUmmUeEstablishCause;

/** @enum wrUmmMeasRptCfgType
 *
 *  Report Configuration types used for ANR and HO alorithms
 * - WR_UMM_NO_ANR_CFG
 * - WR_UMM_EVENT_A3_CFG
 * - WR_UMM_EVENT_A5_CFG
 * - WR_UMM_STRONG_CELL_CFG
 * - WR_UMM_EVENT_A5_INTER_CFG  
 */
typedef enum wrUmmMeasRptCfgType
{
   WR_UMM_NO_ANR_CFG,
   WR_UMM_EVENT_A3_CFG,
   WR_UMM_EVENT_A5_CFG,
   WR_UMM_STRONG_CELL_CFG,      /* perodic ANR*/ /* changed as part of Inter-Freq ANR */
   WR_UMM_EVENT_A5_INTER_CFG, /* Inter-Freq ANR: This Algo has A5 meas for inter-freq and A3 meas for Intra-freq */
   WR_UMM_PERD_STRONG_CELL_CFG /* periodic strong cell configuration as part of ICIC */
}WrUmmMeasRptCfgType;

/** @enum wrUmmTmrVal
 *
 *  Different timers used to fetch the timer values 
 * - WR_UMM_INT_LTE_S1_PREP_TMR
 * - WR_UMM_IRAT_S1_PREP_TMR
 * - WR_UMM_INT_LTE_S1_OVRL_TMR
 * - WR_UMM_IRAT_S1_OVRL_TMR
 */
typedef enum wrUmmTmrVal 
{
   WR_UMM_INT_LTE_S1_PREP_TMR,
   WR_UMM_IRAT_S1_PREP_TMR,
   WR_UMM_INT_LTE_S1_OVRL_TMR,
   WR_UMM_IRAT_S1_OVRL_TMR
}wrUmmTmrVal;

/** @brief This structure 
 - U16  longDrxCycle   This parameter specifies the Long DRX cycle
 - U16  drxStartOffst  This parameter specifies the DRX Start offset
 */
typedef struct WrDrxLongCycleOffst
{
   U16      longDrxCycle;   
   U16      drxStartOffst;  
} WrDrxLongCycleOffst;

/** @brief This structure 
 - Bool    pres        This paramter indicates the presence of short
                       DRX
 - U16     shortDrxCycle This parameter specifies the short DRX cycle
 - U8      drxShortCycleTmr This parameter specifies the short DRX
                            timer value
 */
typedef struct wrDrxShortDrx
{
   Bool    pres;             /*!< Short cycle is configured or not */
   U16     shortDrxCycle;    /*!< DRX Short Cycle value in sub-frames*/
   U8      drxShortCycleTmr; /*!< Value in multiples of Short DRX Cycles*/
} WrDrxShortDrx;

/** @brief This structure holds the Drx Configuration
 - Bool                   isDrxEnabled       Specifies whethere DRX is enabled or not
 - TknS32                 cqiMask            Specifies the CQI mask
 - U8                     drxOnDurTmr        Specifies the Duration of Drx On timer
 - U16                    drxInactvTmr       Specifies the Drx Inactivity timer
 - U8                     drxRetxTmr         Specifies the DRX Retransmission timer
 - WrDrxLongCycleOffst    drxLongCycleOffst  Specifies the long cycle Offset
 - WrDrxShortDrx          drxShortDrx        Specifies the short cycle information
 */
typedef struct wrDrxCfg
{
   Bool                  isDrxEnabled;
   TknS32                cqiMask;
   U8                    drxOnDurTmr;
   U16                   drxInactvTmr; 
   U8                    drxRetxTmr; 
   WrDrxLongCycleOffst   drxLongCycleOffst;
   WrDrxShortDrx         drxShortDrx;
} WrDrxCfg;

/* GA3.0 */
typedef struct wr1xRttTgtCell
{
   U32         bandClass;
   U32         arfcn;
   U16         pnOffset;
   U16         pilotPnPhase;
   U8          pilotStrength;
}WrCdmaRttPilotLst;

typedef struct wrCdma1xRttPnOffset
{
   U16         pnOffset;
   U32         bandClass;
   U32         arfcn;
}WrCdma1xRttPnOffset;

typedef struct wrCdma1xRttPilotInfo
{
   U8          systemType;
   U8          pilotOneWay;
   U16         channelNum;
   TknStrOSXL  *sectorId;
   U8          bandClass;
}WrCdma1xRttPilotInfo;
/* SPS changes starts */
typedef struct wrSpsCfg
{
   U16              spsRnti;
   RmuSpsConfigDl   spsCfgDl;
   RmuSpsConfigUl   spsCfgUl;
} WrSpsCfg;
/* SPS changes ends */

/* Structure for CDMA target cell information */
typedef struct wrcdmatgtcell
{
   U16                   marketId;
   U16                   switchNum;
   U16                   cellId;
   U16                   sectorNum;
   U32                   cdmaType;
}WrCdmaCell;
typedef struct wrcdmacell
{
   WrCdmaCell       cdmaCell;
   U8               priority;
}WrCdmaTgtCell;


/* Structure for CPU and memory information */
typedef struct wrMemCpuMeasCb
{
   U16           memUtilizationMask;
   U16           cpuUtilizationMask;
   CmTimer       cpuMemUtilTmr;
   CmLteMemInfo  memInfo;
   CmLteMemInfo  memCfmInfo;
   CmCpuStatsInfo cpuInfo;
   CmLteCpuInfo  cpuCfmInfo;
}WrMemCpuMeasCb;


#define WR_MAX_CELLS                   2
#define WR_MAX_MMES                    17 /*Supported MMEs are 16. 
                                            17 is to handle peer IDs as they start from 1*/
#define WR_CELL_CFG_DFLT_RBS           6

/* The definitions below are meant for SIB 1                              */
#define WR_MAX_PLMN_IDS                6

#define WR_MCC_SIZE                    3

/* Currently enodeb shall support 16 active UE's, more than 16 Ue's shall be
 *  redirected and 20 MO signaling UE's shall be supported.*/
#define WR_INVALID_SR_INDEX            0xFFFF 
#define WR_INVALID_SRCFG               0xFF 
#define WR_MAX_SR_CQI_CONFIGURED_UES        TENB_MAX_UE_SUPPORTED
#define WR_MAX_SR_CQI_NOT_CONFIGURED_UES    20
#define WR_MAX_UES_PER_CELL            (WR_MAX_SR_CQI_CONFIGURED_UES + \
                                        WR_MAX_SR_CQI_NOT_CONFIGURED_UES)
#define WR_NUM_SR_CQI_RES               WR_MAX_SR_CQI_CONFIGURED_UES
#define WR_VAL_ZERO                    0
#define WR_VAL_ONE                     1
/* EUTRA, CDMA1xRTT, GERAN-CS and GERAN-PS rats */
#define WR_NO_COMP_IN_UECAP             4 
#define WR_VAL_TWO_CELL_RELEASE         2
#define WR_MAX_SFN                     1024

/* Some of the interface configuration status is translated to this type  */
#define WR_CFG_OK                      1
#define WR_CFG_NOK                     2

/* These macros are used to configure SRB1 and SRB2. They are used as LCH */
/* IDs and in the configuration of lower layers                           */
#define WR_PDCP_ID_SRB1                1
#define WR_PDCP_ID_SRB2                2
#define WR_PDCP_ID_DRB                 3

#define WR_RLC_TM_MODE                 1
#define WR_RLC_UM_MODE                 2
#define WR_RLC_AM_MODE                 3

/* The following two macros are defined for data type WrEnbType which     */
/* classifies local eNB and the neighbour eNBs as MACRO or HOME eNBs      */
#define  WR_MACRO_ENB                  ENB_ID_MACROENB_ID
#define  WR_HOME_ENB                   ENB_ID_HOMEENB_ID

#define WR_S1AP_ENB_ID     1
#define WR_S1AP_MME_ID     2
typedef U8                             WrEnbType;

#define MAX_CDMA2OOO_PARAMS_LEN      50

/* Following MACROS used for maximum number of indices for DCI format 3/3a */
#define WR_MAX_FMT3_INDX   16 
#define WR_MAX_FMT3A_INDX  32

/* All the APIs provided by EMM to other modules                          */
EXTERN S16 wrEmCellSchdSiWinSzMap(U16 window, U8 *siWindowSize);
EXTERN S16 wrEmmInit(Void);
EXTERN S16 wrEmmGetCgi(WrCellId cellId, WrCgi *cgi);
EXTERN S16 wrEmmSchdTtiInd(RgrTtiIndInfo *ttiInd);
EXTERN S16 wrEmmSchdCellCfgCfm(U32 transId, U8 status);
EXTERN S16 wrEmmS1apProcPdu(SztUDatEvnt *uDatEvnt);
EXTERN S16 wrEmmPrcPhyCfgCfm(U32 transId, U8 status);
EXTERN S16 wrEmmSetupS1(WrMmeId mmeId);
EXTERN S16 wrEmmRrcCellCfgCfm(U16 cellId);
EXTERN S16 wrEmmAddCell(WrCellId cellId);
EXTERN S16 wrEmmSchdSiCfgCfm(U32 transId, U8 status);
EXTERN S16 wrEmmGetCellType(WrCellId cellId, U8 *cellType);
EXTERN S16 wrEmmSendEnbConfigTransfer(Void);

EXTERN S16 wrEmmSchdWrngSiCfgCfm
(
U32                          transId,
U8                           status
);

EXTERN S16 wrEmmSchdStopWrngSiCfgCfm
(
U32                          transId,
U8                           status
);

/* PH04_CMAS : end */

EXTERN S16 wrEmmPrcS1StaInd(SztStaInd *sztSta);
EXTERN S16 wrEmmMmeSetupWaitTmrExpiry(PTR mmeCb);
EXTERN S16 wrEmmAddMme(WrMmeId mmeId);
EXTERN S16 wrEmmGetMmeByPlmn(WrPlmnId *plmnId, WrMmeId *mmeId);
EXTERN S16 wrEmmGetMmeByMmec(U8 mmec, WrMmeId *mmeId);
EXTERN S16 wrEmmGetMme(WrPlmnId *plmnId, U16 grpId, U8 mmeCode, WrMmeId *mmeId);
EXTERN S16 wrEmmGetSelPlmn(WrCellId cellId, U8 selPlmnId, WrPlmnId **plmn);
EXTERN S16 wrEmmGetDatAppAddr(CmTptAddr **addr, U8 type);
EXTERN S16 wrEmmUmmMeasReCfg(WrCellId cellId);
EXTERN S16 wrEmmUpdateCellState(U16 cellId,WrCellState cellState);
EXTERN S16 wrEmmMmeBrngCellPwrDown(Void);
/*ccpu00142992 fix*/
EXTERN S16 wrEmmCallMmeSetup(Void);
/* All the APIs provided by UMM to other modules                          */
EXTERN Void wrUmmInit(Void);
EXTERN S16  wrUmmAddCell(WrCellId cellId, LwrAddCellCfg *cfgAdd);
/* SPS changes strarts */
EXTERN S16  wrUmmCreateSpsRntiDb(WrCellId cellId, LwrAddCellCfg *cfgAdd, U32 grpIndx);
/* SPS changes ends */
EXTERN S16  wrUmmDelMme(WrMmeId mmeId);
EXTERN S16  wrUmmAddMme(WrMmeId mmeId);
EXTERN S16  wrUmmPrcRrcCfgCfm(NhuCfgCfmSdus *cfgCfm);
EXTERN S16  wrUmmPrcPhyCfgCfm(U32 transId, U8 status);
EXTERN S16  wrUmmPrcSchdCfgCfm(U32 transId, U8 status);
EXTERN S16  wrUmmPrcUuDatInd(NhuDatIndSdus *uuDatInd);
EXTERN S16  wrUmmPrcS1RelCfm(SztRelCfm *relCfm);
EXTERN S16  wrUmmPrcS1RelInd(SztRelInd *relInd);
EXTERN S16  wrUmmPrcS1ErrInd(SztLclErrInd *relInd);
EXTERN S16  wrUmmPrcS1apConCfm(SztConCfm *conCfm);
EXTERN S16  wrUmmPrcS1DatInd(SztDatEvntInd *s1DatInd);
EXTERN S16  wrUmmPrcRrcDatCfm(NhuDatCfmSdus *rrcDatCfm);
EXTERN S16  wrUmmPrcRrcErrInd(NhuErrIndSdus *rrcErrInd);
EXTERN S16  wrUmmPrcRrcEncodeCfm(NhuEncCfmSdus *rrcEncCfmSdus);
EXTERN S16  wrUmmPrcRrcDecodeCfm(NhuDecodeSduCfm *rrcDecCfmSdus);
EXTERN S16  wrUmmPrcTimerExp(WrUmmTimer *cb, U8 wrUmmTmrExpEvnt);
EXTERN S16  wrUmmMmeFullRst(WrMmeId mmeId);
EXTERN S16  wrUmmMmePartInRst(WrMmeId mmeId, SztResetTyp *tknIE);
EXTERN S16  wrUmmMmePartRst(WrMmeId mmeId, WrEmmS1ConId *conId, U32 nmbCons);
EXTERN S16  wrUmmPrcReestabCfm(U32 transId, U8 status);
EXTERN S16  wrUmmUpdateCellState(U16 cellId,WrCellState cellState);
EXTERN S16  wrUmmPrcRrmUeAdmitResp(RmuUeAdmitRsp *ueAdmitResp);
EXTERN S16  wrUmmPrcRrmUeRelInd(RmuUeRelInd *ueRelInd);
EXTERN S16  wrUmmPrcRrmUeRelResp(RmuUeRelRsp *ueRelResp);
EXTERN S16  wrUmmProcHoAdmitRsp(RmuUeHoRsp* hoResp);
#ifdef TENB_AS_SECURITY
EXTERN S16  wrUmmPrcKdfCfm(U32 trasnId, U8 status, CtfKdfCfmInfo *kdfCfmInf);
#endif
EXTERN S16 wrUmmGetUeCntxToRel(WrCellId cellId);

/* All the APIs provided by DMM to other modules                          */
EXTERN S16  wrDamInit(Void);
EXTERN Void  wrDamTnlMgmtCfm(EgtUEvnt *eguMsg);
EXTERN S16  wrDamErrInd(EgtUEvnt *eguMsg);
#ifdef TENB_FP
EXTERN S16  wrDamEgtpFpDatInd(U32 lclTeid, Buffer* mBuf);
#endif
EXTERN S16  wrDamEgtpDatInd(Pst *pst, EgtUEvnt *eguMsg);
EXTERN S16  wrDamPdcpDatInd(CmLtePdcpId *pdcpId, Buffer *mBuf);
EXTERN S16  wrDamPdcpDatFwdInd(Pst *pst, CmLtePdcpId *pdcpId,
      PjuDatFwdIndInfo *datFwdInd);
EXTERN Bool wrDamIsUeInactv(Void *cb);
EXTERN U16 wrEmmGetServCarrierFreq(U16 cellId);
EXTERN U16 wrEmmGetPhyCellId(U16 cellId);
/* SPS changes starts */
EXTERN U16 wrEmmGetDuplexMode(U16 cellId);
EXTERN S16 wrEmmGetMaxUeSupport(U16 cellId, U16 *pMaxUeSupp);
/* SPS changes ends */
EXTERN U16 wrEmmGetNeighIpFrmPeerId(U32 peerId, U8 cellId, CmTptAddr *peerAddr);

/* API provided by parameter module                                       */
EXTERN Void wrEnbParamsInit(Void);
EXTERN U8 wrEmmGetNumTxAnt(Void);
EXTERN U32 wrEmmGetRrcUeCapTmrVal(U16 cellId);

EXTERN U32 wrEmmGetRrcSecModTmrVal(U16 cellId);
EXTERN U32 wrEmmGetRrcConTmrVal(U16 cellId);
EXTERN U32 wrEmmGetRrcReestabTmrVal(U16 cellId);
EXTERN U32 wrEmmGetRrcConRelTmrVal(U16 cellId);
EXTERN U32 wrEmmGetRrcReCfgTmrVal(U16 cellId);
EXTERN S16 wrEmmGetT304IRATTmrVal(U16 cellId, U32     *val);
EXTERN S16 wrEmmGetT301TmrVal(U16 cellId, U32     *val);
EXTERN S16 wrEmmGetTmrVal(U16 cellId, U32 type,U32 *tmrVal); 

EXTERN S16 wrUmmMmeGetFirstUeS1apId(WrMmeId mmeId, U32 *ue_enb_s1apId, 
                                                     U32 *ue_mme_s1apId);
EXTERN S16 wrUmmAnrMeasInitCfg(U16 cellId, WrUmmAnrMeasInfo *anrMeasCfgInfo);

EXTERN S16 wrUmmAnrStartMeasurements(U16 cellId);
EXTERN Bool wrEmmAnrUeConnected(U16  cellId);
EXTERN Bool wrEmmAnrIsTriceTmrRunning(U16   cellId);

EXTERN S16 wrUmmAnrDelMeasCfgForAllUe(U16  cellId);
EXTERN S16 wrEmmMeasReportCgiHdlr(U16 cellId,U8 nrType,U16 carrFreq,
      NhuMeasResults *measRprt, U8 ueAccessStratRel);
EXTERN S16 wrUmmAddNrFreq(U16 cellId, U8 numFreq, LwrNeighFreqAddModCfg *nrFreqCfg);

EXTERN S16 wrUmmModNrFreq(U16 cellId,U8 numFreq,LwrNeighFreqAddModCfg *nrFreqCfg);
EXTERN S16 wrUmmDelNrFreq(U16 cellId,U16 numFreq,LwrNeighFreqDelCfg *nrFreqCfg);
EXTERN S16 wrEmmGetOffsetFreqFrmArfcn(U16 cellId,U16 arfcn,U8 *offsetFreq);
EXTERN S16 wrUmmAddNrGeranObj(U16 cellId,U8 numFreq, WrUmmMeasGeranMeasInfo *nrFreqCfg);
EXTERN Void* wrEmmAnrGetGeranNbr(U16 cellId, U16 arfcn, U8 bandInd, U8 ncc, U8 bcc);
EXTERN U8 wrEmmGetAnrDrxInActivTmrVal(WrCellId cellId);
EXTERN PUBLIC Void wrEmmAnrPrintCellInfo(Void);
EXTERN Void wrEmmAnrPrintUtraFddCellInfo (U8 cellId);
EXTERN Void wrEmmAnrPrintUtraTddCellInfo (U8 cellId);
EXTERN S16 wrSendNghCellInfo(LwrMngmt  *lwrMng);
EXTERN S16 wrEmmDynCfgReq(LwrDynMngmt *dynCfg);
EXTERN U16 wrEmmGetAnrDrxOnDur(WrCellId  cellId);
EXTERN U16 wrEmmGetAnrDrxLongCycle(WrCellId  cellId);
EXTERN Void wrPlatRgrTtiHndl(U32 tticnt);
EXTERN Bool wrUmmChkGeranBandindEql(U8  cfgdBand, U32  nhuBand);
EXTERN S16 wrUmmCsfbCdma2k1xMobParams(U16 cellId, LwrCdma2k1xMobParams *mobParams);

EXTERN U32 wrUmmGetRrcDatCfmTmrVal(U32     cellId);
EXTERN S16 smBuildNlGenCfg(NlGenCfg   *genCfg);
EXTERN S16 smBuildNlNghCellCfg(LnlNghCellCfg   *nghCellCfg);
EXTERN S16 smBuildNlCellCfg(LnlCellCfg   *cellCfg);
EXTERN S16 smBuildNlCellReCfg(LnlCellReCfg   *cellReCfg);
EXTERN S16 smUpdateNlPciVal(U16   pci);
EXTERN S16 smUpdateNlEarfcnVal(NlUpdateEarfcnCfgInd *earfcnCfgInd);
EXTERN S16 smSendNlCellReCfg(Void);
EXTERN S16 smUpdateNlPrachVal(NlUpdatePrachCfgInd   *prachCfgInd);
EXTERN S16 wrEmmGetPlmnRedirSupport(WrPlmnId  *plmn,Bool *isRedirSupport,WrCellId cellId);
EXTERN Void smFillEgIpInfo(SpId spId,CmTptAddr *srcAddr,EgIpInfo *ipInfo);
EXTERN S16 smBuildTServerCfg(S32 idx);
EXTERN S16 smUpdateNlTxPwrVal(NlUpdateTxPwrCfgInd *txPwrCfgInd);
EXTERN S16 smUpdateNlMeasRptInterval(U32   measInterval);
EXTERN S16 wrEmmCheckPRem(void);

/* HCSG DEV */
#ifdef HCSG_DBG
EXTERN S16 wrUmmHcsgPrintUeStats(Void);
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

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
