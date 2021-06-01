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

     Name:     LTE-X2AP Layer 
  
     Type:     C include file
  
     Desc:     Defines required by LTE X2AP.

     File:     cz.h

     Sid:      cz.h@@/main/2 - Tue Aug 30 18:36:04 2011

     Prg:      

**********************************************************************/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000373        LTE X2AP
*
*/


#ifndef __CZH__
#define __CZH__

#ifdef __cplusplus           
extern "C" {
#endif /* __cplusplus */

/** 
 * Macro for X2AP layer name
 */
#define CZLAYERNAME  "LTE-X2AP"

/**@ingroup utlmodule 
 * This macro represents the maximum procedure codes supported for
          X2AP. */
#define CZ_MAX_MSG_PROC_CODE  16

/**@ingroup utlmodule 
 * This macro represents maximum message type choice values supported
          for X2AP. */
#define CZ_MAX_MSG_CHOICE_VAL  3

/** @ingroup bmpmodule */
#define CZ_MAX_MSG_DIRECTN     2
/** @ingroup bmpmodule */
#define CZ_UNKNOWN_UE_X2AP_ID  0xFFFF
/** @ingroup ldmmodule */
#define CZ_UNKNOWN_MEAS_ID     0xFFFF
/** @ingroup bmpmodule */
#define CZ_MAX_UE_X2AP_ID      4096
/** @ingroup utlmodule */
#define CZ_MAX_IDX             5
/** @ingroup edmmodule */
#define CZ_PASN_VER_ZERO       0


/*******************************************************/
/* Basic Mobility Management Module */
/*******************************************************/
/** This macro represents Maximum number of timers required in basic
         mobility management. */
/** @ingroup bmpmodule */
#define CZ_BMP_MAX_TIMERS      2   /**< maximum Nuber of timers used in BMP*/
/** @ingroup tpmmodule */
#define CZ_MAX_SCTSAP_TMRS     2   /**<Sctp sap timers */

/** @ingroup bmpmodule */
#define CZ_BMP_HO_MAX_STATES   3   /**<  This macro represents Maximum number of handover states in basic
                                           mobility management. */

/** @ingroup bmpmodule 
    This macro gets the keyPrefix[eNodeB Type] from input key. */
#define CZ_BMP_GET_UECB_KEY_PREFIX(_key, _keyPrefix)  \
      _keyPrefix = GetHiWord(_key); \
      _keyPrefix = GetLoByte(_keyPrefix); 

/*******************************************************/
/* End Of Basic Mobility Management Module */
/*******************************************************/

/** @ingroup tmrmodule */
#define CZ_TMR_START           1
/** @ingroup tmrmodule */
#define CZ_TMR_STOP            2

/** @ingroup tpmmodule */
#define CZ_MAX_SCT_SAP_TMRS    5

/** @ingroup tmrmodule */
#define CZ_TQSIZE              64

/** @ingroup tmrmodule */
#define CZ_TQNUMENT            64
/* cz001.301: SCTP Protocl id added*/
/** @ingroup tpmmodule */
#define CZ_SCT_PROTID_X2AP     27

/**  
 @ingroup tmrmodule 
 * Timer for time to wait IE value */
#define CZ_GP_TMR_WAIT_ENB_UPD 0

/** @ingroup tmrmodule */
#define CZ_GP_TMR_WAIT_SETUP   1

/** @ingroup */
#define CZ_LCL_SU_ENDPID       1

/** @ingroup tpmmodule */
#define CZ_ENDP_CFG            1

/** @ingroup tpmmodule */
#define CZ_ENDP_OPEN_IN_PRG    2

/** @ingroup tpmmodule */
#define CZ_ENDP_OPEN           3

/** @ingroup tpmmodule */
#define CZ_ASSOC_DOWN          1

/** @ingroup tpmmodule */
#define CZ_ASSOC_IN_PRG        2

/** @ingroup tpmmodule */
#define CZ_ASSOC_UP            3

/** @ingroup */
#define CZ_HASH_LIST_SIZE 1024
#define CZ_HASH_KEYTYPE CM_HASH_KEYTYPE_U32MOD
#define CZ_MEM_PDU_SIZE   2048

/** @ingroup lmmmodule */
#define CZ_DEL_SAP  1
/** @ingroup lmmmodule */
#define CZ_UBND_SAP 2
/** @ingroup lmmmodule */
#define CZ_SHUT_DOWN 3
#define CZ_SCT_MAX_SEL 4

/** @ingroup lmmmodule */
#ifndef CZ_MAX_INSTANCES
#define CZ_MAX_INSTANCES 65535
#endif /*endif CZ_MAX_INSTANCES */

/** @ingroup lmmmodule */
#define CZ_MIN_OUT_STREAMS 3
#ifdef SS_MULTIPLE_PROCS
#define czCb (*czCbPtr) /* Avtec error fixing for 1.2*/
#endif /* SS_MULTIPLE_PROCS */


/* avtecs fix for 1.2*/

#define MAX_MSG_CHOICE_VAL 3

/** @def CZ_UPD_STS
 * The macro CZ_UPD_STS is used to print successful processing of a message */
#define CZ_UPD_STS(_peerCb, _msgType, _dir)  \
{                                            \
 if(_dir == CZ_DIR_IN)                       \
 {                                           \
    CZ_DBG_INFO((CZ_PRNT_BUF,                \
     "Message %d received successfully.\n", msgType)); \
 }                                            \
 else                                         \
 {                                            \
    CZ_DBG_INFO((CZ_PRNT_BUF,                  \
        " Message %d sent successfully.\n", msgType)); \
 }                                             \
} /* CZ_UPD_STS */

/** @def CZ_GP_ERR_IND_USR
 * The macro CZ_GP_ERR_IND_USR is used to give an error indication to the
 * user. */
#define CZ_GP_ERR_IND_USR(_peerCb, _event, _msgType, _prevState)\
{\
   S16 _ret = ROK; \
   _ret = czUimSndGpErrInd(_peerCb, _event);\
   if(_ret != ROK)\
   {\
      _peerCb->state = _prevState;\
      _ret = RFAILED;\
      CZ_FREEEVNT(_event); \
      czUtlUpdInErrSts(_peerCb, _msgType); \
      CZ_DBG_FATAL((CZ_PRNT_BUF, \
         "Indication of the received message %d failed.\
               Reverting the state to %d.\n", \
               _msgType, _prevState));\
   }\
   else\
   {\
      CZ_DBG_INFO((CZ_PRNT_BUF, \
          "Indication of the received message %d successful.\n", _msgType));\
   }\
} /* CZ_GP_ERR_IND_USR */


/** @def CZ_GET_EVENT_MSG_MAP 
    This macro gets the mapped Trillium X2AP event using input Trillium
         X2AP message type. Trillium X2AP event is an input to different FSMs
         implemented for:
    - Global Procedure.
    - Basic Mobility Management Procedure. 
    @note For a list of definitions of Trillium's X2AP message types refer
          to czt.h
*/
#define CZ_GET_EVENT_MSG_MAP(_msg, _evnt) \
   _evnt = ((_msg < CZT_MSG_MAX) ? \
           (czEvntMap[_msg]) : (CZT_MSG_UNKNOWN));


/**  
 * @details Macro for setting memory to zero
 * using cmMemset.
 */
#define CZ_SET_ZERO(_memPtr, _size)                   \
do                                                    \
{                                                     \
      /* cz001.301 : removing null check for _memPtr \
       * as it is not required. build warning fix.
       * Keeping do while loop, eventhough only one statement
       * present. Because, if code gets added here, 
       * should not give any problem*/                   \
      (Void)cmMemset((U8*)(_memPtr), 0, (Size)_size); \
}                                                     \
while(0)
      

/**  
 * This section defines the macros used for
 * Memory allocation in X2AP.  
 */

/**  
 * @details Macro for SGetSBuf.
 */
#define CZ_ALLOC(_datPtr, _size)                        \
{                                                       \
   if (SGetSBuf(czCb.init.region,                       \
                  czCb.init.pool,                       \
                  (Data**)&_datPtr, _size) == ROK)      \
   {                                                    \
      cmMemset((U8*)_datPtr, 0, _size);                 \
      /* add to general status */                       \
      czCb.genSta.memAlloc += SBUFSIZE(_size);          \
   }                                                    \
   else                                                 \
   {                                                    \
      CzUstaDgn _dgn; \
      _datPtr = NULLP;                                  \
                                                            \
      /* Inform the Layer Manager of resource crunch */ \
      _dgn.alarmType = LCZ_USTA_DGNVAL_MEM;                  \
      _dgn.u.mem.region = czCb.init.region;             \
      _dgn.u.mem.pool = czCb.init.pool;                 \
      czLmmSendLmAlarm (LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,     \
                     LCM_CAUSE_MEM_ALLOC_FAIL, &_dgn);                     \
   }                                                   \
} /* end of CZ_ALLOC */


/**  
 * @details Macro for SPutSBuf.
 */
#define CZ_FREE(_datPtr, _size)                       \
{                                                     \
   if (_datPtr != NULLP)                              \
   {                                                  \
      (Void)SPutSBuf(czCb.init.region,                \
                     czCb.init.pool,                  \
                     (Data*)_datPtr, _size);          \
      /* subtract from general status */              \
      czCb.genSta.memAlloc -= SBUFSIZE(_size);        \
      _datPtr = NULLP;                                \
   }                                                  \
}


/**  
 * @details Macro for SGetMsg.
 */
#define CZ_ALLOCMBUF(_buf)            \
do                                                    \
{                                                     \
   if (SGetMsg(czCb.init.region, czCb.init.pool,      \
                        (Buffer **) _buf) != ROK)     \
   {                                                  \
      (*_buf) = NULLP;                                \
   }                                                  \
}                                                     \
while(0)

/**  
 * @details Macro for SPutMsg.
 */
#define CZ_FREEMBUF(_mBuf)                            \
do                                                    \
{                                                     \
   if(NULLP != _mBuf)                                 \
   {                                                  \
     (Void)SPutMsg(_mBuf);                            \
     _mBuf = NULLP;                                   \
   }                                                  \
}                                                     \
while(0)

/**  
 * @details Macro for allocating an Event structure
 * using cmAllocEvnt.
 */
#define CZ_ALLOCEVNT(_evntPtr, _size, _mem)           \
do                                                    \
{                                                     \
   (*_evntPtr) = NULLP;                               \
   (Void)cmAllocEvnt((Size) _size, CZ_MEM_PDU_SIZE,   \
                     _mem, (Ptr *)_evntPtr);          \
}                                                     \
while(0)

/**  
 * @details Macro for allocating in an event structure
 * using cmGetMem.
 */
#define CZ_GET_MEM(_memPtr, _size, _allocPtr)         \
do                                                    \
{                                                     \
   (*_allocPtr) = NULLP;                              \
   (Void)cmGetMem(_memPtr, _size, (Ptr *)_allocPtr);  \
}                                                     \
while(0)

/**  
 * @details Macro for freeing an event structure
 * using cmFreeMem.
 */
#define CZ_FREEEVNT(_evnt)                            \
do                                                    \
{                                                     \
   cmFreeMem((_evnt));                                \
   (_evnt) = NULLP;                                   \
}                                                     \
while(0)

/**  
 * @details Macro for copying memory from source to target
 * using cmMemcpy.
 */
#define CZ_MEM_COPY(_tgtPtr,_srcPtr, _size)                              \
do                                                                       \
{                                                                        \
   (Void)cmMemcpy((U8*)_tgtPtr,(CONSTANT U8*)_srcPtr, (Size)_size);      \
}                                                                        \
while(0)

/**
 * @details Macro for memory.
 */
#define CZ_PRNT_BUF  czCb.init.prntBuf
#define CZ_INIT_ENT  czCb.init.ent
#define CZ_INIT_INST czCb.init.inst
#define CZ_REG       czCb.init.region
#define CZ_POOL      czCb.init.pool
/**  
 * @details Macros in X2AP for debug logging.
 */
#ifdef DEBUGP
#define CZP                       czCb.init.prntBuf /* Avtecs error fixing for 1.2*/

/**
 * @detail Macro for printing.
 */
#define CZPRNT(_prntbuf)   \
{                          \
   sprintf _prntbuf;       \
   SPrint(CZP);            \
}

#ifdef CZ_PRNT_TIMESTAMP
#define CZ_PRNTTIMESTAMP                                                \
{                                                                       \
   S8 _buf[60];                                                    \
   (Void) SGetTimeStamp(_buf);                                          \
   CZPRNT((CZP, "[%s]:", _buf));                                        \
}
#else /*else CZ_PRNTTIMESTAMP */
#define CZ_PRNTTIMESTAMP
#endif /* endif CZ_PRNTTIMESTAMP */

/**
 * @detail Macro for printing layer details.
 */
#define CZ_PRNT_LAYER                                                   \
{                                                                       \
   CZPRNT((CZP, " [%s 0x%x:%x]", CZLAYERNAME,                           \
         czCb.init.ent,czCb.init.inst));                                \
}

/**
  * @detail Macro for printing file name and line number.
 */
#define CZ_PRNT_FILE_LINE                                               \
{                                                                       \
   CZPRNT((CZP, " [%s:%d:%s]",__FILE__,__LINE__, __tapa_fun));          \
}


/* trace macro*/
#define CZ_TRC2(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (czCb.init.dbgMask & (LCZ_DBGMASK_TRC))                           \
   {                                                                    \
      CZ_PRNTTIMESTAMP                                                  \
      CZ_PRNT_LAYER                                                     \
      CZ_PRNT_FILE_LINE                                                 \
      CZPRNT((CZP," Entering\n"));                                       \
      TRC2(_arg);                                                       \
   }                                                                    \
}

/*-- Added for SS_MULTIPLE_PROCS support --*/
#ifdef SS_MULTIPLE_PROCS
#define CZ_TRC_ACTINIT(_arg)                                            \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              
#endif /* endif SS_MULTIPLE_PROCS */

/* trace macro */
#define CZ_TRC3(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (czCb.init.dbgMask & (LCZ_DBGMASK_TRC))                           \
   {                                                                    \
      CZ_PRNTTIMESTAMP                                                  \
      CZ_PRNT_LAYER                                                     \
      CZ_PRNT_FILE_LINE                                                 \
      CZPRNT((CZP," Entering\n"));                                       \
      TRC3(_arg);                                                       \
   }                                                                    \
}

/* @detail Macro for VOID return */
#define CZ_RETVOID                                                      \
do                                                                      \
{                                                                       \
   if (czCb.init.dbgMask & (LCZ_DBGMASK_TRC))                           \
   {                                                                    \
      CZ_PRNTTIMESTAMP                                                  \
      CZ_PRNT_LAYER                                                     \
      CZ_PRNT_FILE_LINE                                                 \
      CZPRNT((CZP, " Exiting\n"));                                       \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

/**
 * @def CZ_RETVALUE macro to return a value from any X2AP function
 */
#define CZ_RETVALUE(_arg)                                               \
do                                                                      \
{                                                                       \
   if (czCb.init.dbgMask & (LCZ_DBGMASK_TRC))                           \
   {                                                                    \
      CZ_PRNTTIMESTAMP                                                  \
      CZ_PRNT_LAYER                                                     \
      CZ_PRNT_FILE_LINE                                                 \
      CZPRNT((CZP, " Exiting\n"));                                       \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}while(0)

/* @detail Macro for DEBUG prints. */
#define CZ_DBG(_mask, _maskStr, _arg)                \
do                                                                      \
{                                                                       \
   if (czCb.init.dbgMask & (_mask))                                     \
   {                                                                    \
      CZ_PRNTTIMESTAMP                                                  \
      CZ_PRNT_LAYER                                                     \
      CZ_PRNT_FILE_LINE                                                 \
      CZPRNT((CZP, " [" #_maskStr "]" ));                               \
      CZPRNT((CZP, ":"));                                                \
      CZPRNT(_arg);                                                     \
      CZPRNT((CZP, "\n"));                                              \
   }                                                                    \
}while(0)



/* cz001.101 : Improve Debug Prints */

#define CZ_DBG_PARAM(_arg)   CZ_DBG(LCZ_DBGMASK_PARAM, "PARAM", _arg)
#define CZ_DBGP(_arg)         CZ_DBG(LCZ_DBGMASK_DBG,"DEBUG", _arg)
#define CZ_DBG_INFO(_arg)    CZ_DBG(LCZ_DBGMASK_INFO,"INFO", _arg)
#define CZ_DBG_WARN(_arg)    CZ_DBG(LCZ_DBGMASK_WARN,"WARNING", _arg)
#define CZ_DBG_ERROR(_arg)     CZ_DBG(LCZ_DBGMASK_ERROR,"ERROR", _arg)
#define CZ_DBG_FATAL(_arg)   CZ_DBG(LCZ_DBGMASK_FATAL,"FATAL", _arg)
#define CZ_DBG_BUF(_arg)      CZ_DBG(LCZ_DBGMASK_BUF,"BUFFER", _arg)

#else /* DEBUGP */

#define CZ_RETVOID            do{RETVOID;}while(0)
#define CZ_RETVALUE(_arg)     do{RETVALUE(_arg);}while(0)
/* Trace */
#define CZ_TRC2(_arg)
#define CZ_TRC3(_arg)

#ifdef SS_MULTIPLE_PROCS
#define CZ_TRC_ACTINIT(_arg);
#endif /* endif SS_MULTIPLE_PROCS */


/* @detail Macro for DEBUG prints. */
#define CZ_DBG_PARAM(_arg)
#define CZ_DBGP(_arg)
#define CZ_DBG_INFO(_arg)
#define CZ_DBG_FLOW(_arg2)
#define CZ_DBG_WARN(_arg)
#define CZ_DBG_ERR(_arg)
#define CZ_DBG_FATAL(_arg)
#define CZ_DBG_ERROR(_args)
#endif /* DEBUGP */



/**  
 * @details Assertions in X2AP for debug
 * assertions are available by enabling ERRCLASS
 * & ERRCLS_DEBUG and DEBUGP (for log).
 */
#if (ERRCLASS & ERRCLS_DEBUG)

#ifdef DEBUGP
#define CZ_PRNT_ASSERT(_arg)                             \
{                                                        \
   sprintf(CZ_PRNT_BUF, "[%s 0x%x:%x] %s:%d ",           \
            CZLAYERNAME, CZ_INIT_ENT, CZ_INIT_INST,      \
            __FILE__, __LINE__);                         \
   SPrint(CZ_PRNT_BUF);                                  \
}
#else /* else CZ_PRNT_ASSERT */
#define CZ_PRNT_ASSERT(_arg)
#endif /* endif CZ_PRNT_ASSERT */
   

/**  
 * Assert that expression is TRUE (non-zero).
 */
#define CZ_ASSERT(_cond)                                    \
do                                                          \
{                                                           \
   if (!(_cond))                                            \
   {                                                        \
      CZ_PRNT_ASSERT((CZ_PRNT_BUF,                          \
            "Assertion Failed condition %s \n", #_cond));   \
      exit(0);                                              \
   }                                                        \
}                                                           \
while(0)

/**  
 * Assert that value is TRUE (non-zero).
 */
#define CZ_ASSERT_TRUE(_val)                                   \
do                                                             \
{                                                              \
   if (!_val)                                                  \
   {                                                           \
      CZ_PRNT_ASSERT((CZ_PRNT_BUF,                             \
         "Assertion Failed, value %s : Not TRUE \n", #_val));  \
      exit(0);                                                 \
   }                                                           \
}                                                              \
while(0)

/**  
 * Assert that value is FALSE (zero). 
 */
#define CZ_ASSERT_FALSE(_val)                                  \
do                                                             \
{                                                              \
   if (_val)                                                   \
   {                                                           \
      CZ_PRNT_ASSERT((CZ_PRNT_BUF,                             \
         "Assertion Failed, value %s : Not FALSE \n", #_val)); \
      exit(0);                                                 \
   }                                                           \
}                                                              \
while(0)

/**  
 * Assert that val1 != val2 
 */
#define CZ_ASSERT_NOT_EQUAL(_val1, _val2)                      \
do                                                             \
{                                                              \
   if (_val1 != _val2)                                         \
   {                                                           \
    CZ_PRNT_ASSERT((CZ_PRNT_BUF,                             \
    "Assertion Failed %s & %s : Not Equal \n",#_val1, #_val2));\
     exit(0);                                                 \
   }                                                           \
}                                                              \
while(0)

/**  
 * Assert that pointer ptr != NULL 
 */
#define CZ_ASSERT_PTR_NON_NULL(_ptr)                           \
do                                                             \
{                                                              \
   if (_ptr == NULLP)                                          \
   {                                                           \
      CZ_PRNT_ASSERT((CZ_PRNT_BUF,                             \
         "Assertion Failed, Pointer %s : Null \n", #_ptr));    \
      exit(0);                                                 \
   }                                                           \
}while(0)

#else /* (ERRCLASS & ERRCLS_DEBUG) */

/* @detail Macro for ASSERT. */
#define CZ_ASSERT(_cond)
#define CZ_ASSERT_TRUE(_val)
#define CZ_ASSERT_FALSE(_val)
#define CZ_ASSERT_NOT_EQUAL(_val1, _val2)
#define CZ_ASSERT_PTR_NON_NULL(_ptr)

#endif /* (ERRCLASS & ERRCLS_DEBUG) */

/* @detail Macro for sending local error to upper layer. */
#define CZ_SEND_LCL_GEN_ERR(_peerCb, _cType, _cVal, _msgTyp)              \
{                                                                         \
   CztLclErrInd _errInd;                                                  \
   _errInd.peerId = _peerCb->peerId;                                      \
   _errInd.causeType  = _cType;                                           \
   _errInd.causeValue = (U8)_cVal;                                        \
   _errInd.msgType    = _msgTyp;                                          \
   czUimSndErrInd (_peerCb->cztSapCb, &_errInd, NULLP, NULLP);            \
}                                              

/* @detail Macro for sending local error to upper layer. */
#define CZ_SEND_LCL_UI_ERR(_cztSapCb, _peerId, _cType, _cVal, _event)       \
{                                                                           \
   CztLclErrInd _errInd;                                                    \
   _errInd.peerId = _peerId;                                                \
   _errInd.causeType  = _cType;                                             \
   _errInd.causeValue = (U8)_cVal;                                          \
   czUimSndErrInd (_cztSapCb, &_errInd, _event, NULLP);                     \
}

/* @detail Macro for sending local resource error to upper layer. */
#define CZ_SEND_LCL_RSRCPROC_ERR(_peerCb, _cType, _cVal, _msgTyp,          \
                                 _measId1, _measId2 , _event,  _mBuf)      \
{                                                                          \
   CztLclErrInd _errInd;                                                   \
   S16          _ret;                                                      \
   CztEvnt      *_tmpEvnt = _event;                                        \
   _errInd.peerId = _peerCb->peerId;                                       \
   _errInd.causeType  = _cType;                                            \
   _errInd.causeValue = (U8)_cVal;                                         \
   _errInd.msgType    = _msgTyp;                                           \
   CZ_SET_ZERO(&_errInd.u.rsrcInfo, sizeof(CzRsrcInfo));                   \
   _errInd.u.rsrcInfo.eNb1MeasId = _measId1;                               \
   _errInd.u.rsrcInfo.eNb2MeasId = _measId2;                               \
   _ret = czUimSndErrInd (_peerCb->cztSapCb, &_errInd, _event, _mBuf);     \
   if(_ret != ROK)                                                         \
   {                                                                       \
      if(_tmpEvnt != NULLP)                                                \
      {                                                                    \
         CZ_FREEEVNT(_tmpEvnt);                                            \
      }                                                                    \
   }                                                                       \
} /* end CZ_SEND_LCL_RSRCPROC_ERR */                                              

#define CZ_SEND_LCL_ERR_ALL(_peerCb, _cType, _cVal, _msgTyp,_event,         \
                                                            _mBuf)          \
{                                                                           \
   S16          _ret = ROK;                                                 \
   CztLclErrInd _errInd;                                                    \
   _errInd.peerId = _peerCb->peerId;                                        \
   _errInd.causeType  = _cType;                                             \
   _errInd.causeValue = (U8)_cVal;                                          \
   _errInd.msgType    = _msgTyp;                                            \
   _ret = czUimSndErrInd (_peerCb->cztSapCb, &_errInd, _event, _mBuf);      \
   if(_ret != ROK)                                                          \
   {                                                                        \
      if(_event != NULLP)                                                   \
      {                                                                     \
         CZ_FREEEVNT(_event);                                               \
      }                                                                     \
   }                                                                        \
} /* end CZ_SEND_LCL_ERR_ALL */     

                                         
#define CZ_SND_RES_ALARM(_region, _pool)                               \
{                                                                      \
   CzUstaDgn _dgn;                                                     \
   CZ_SET_ZERO(_dgn, sizeof(CzUstaDgn));                               \
   _dgn.alarmType = LCZ_USTA_DGNVAL_MEM;                               \
   _dgn.u.mem.region = _region;                                        \
   _dgn.u.mem.pool =_pool;                                             \
   czLmmSendLmAlarm (LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL, \
                    LCM_CAUSE_MEM_ALLOC_FAIL, &_dgn);                  \
}


/* Send an alarm for sapId events */
#define CZ_SEND_SAPID_ALARM(_sapId, _evnt, _cause) \
{ \
   CzUstaDgn _dgn; \
   cmMemset((U8*)&_dgn, 0, sizeof(CzUstaDgn)); \
   _dgn.alarmType = LCZ_USTA_DGNVAL_SAPID; \
   _dgn.u.sapId = _sapId; \
   czLmmSendLmAlarm(LCM_CATEGORY_INTERFACE, _evnt, _cause, &_dgn); \
}


/* Send an alarm for UE related events */
#define CZ_SEND_UE_ALARM(_ueId,_peerId, _evnt, _cause) \
{ \
   CzUstaDgn _dgn; \
   cmMemset((U8*)&_dgn, 0, sizeof(CzUstaDgn)); \
   _dgn.alarmType = LCZ_USTA_DGNVAL_UE; \
   _dgn.u.ueInfo.peerId = _peerId; \
   _dgn.u.ueInfo.ueId = _ueId; \
   czLmmSendLmAlarm(LCM_CATEGORY_PROTOCOL, _evnt, _cause, &_dgn); \
}


/* Send an alarm for peer events */
#define CZ_SEND_PEER_ALARM(_peerId, _dstNAddr, _evnt, _cause) \
{ \
   CzUstaDgn _dgn; \
   cmMemset((U8*)&_dgn, 0, sizeof(CzUstaDgn)); \
   _dgn.alarmType = LCZ_USTA_DGNVAL_PEER; \
   _dgn.u.peerInfo.peerId = _peerId; \
   if ( _dstNAddr != NULLP) \
   {\
    cmMemcpy((U8 *)&(_dgn.u.peerInfo.dstNAddr), (U8 *)_dstNAddr, \
                                               sizeof(CmNetAddr));\
   }\
   else \
   {\
      _dgn.u.peerInfo.dstNAddr.type = CM_NETADDR_NOTPRSNT;\
   }\
   czLmmSendLmAlarm(LCM_CATEGORY_PROTOCOL, _evnt, _cause, &_dgn); \
}

/** @def CZ_FILL_TKN 
  *  Fills the U8/U32/U16 values in the IE .
  */
#define CZ_FILL_TKN(_ie,_val)      \
{                                  \
   _ie.pres = PRSNT_NODEF;         \
   _ie.val  = _val;                \
}

/** @def CZ_FILL_IE_ID 
  *  Fills the ie cause, id, critcicality.
  */
#define CZ_FILL_IE_ID(_ie,_id, _crit,_typ)   \
{                                            \
   _ie.pres = TRUE;                          \
   _ie.ieId  = _id;                          \
   _ie.crit  = _crit;                        \
   _ie.typOfErr  = _typ;                     \
}



#define CZ_FILL_TKN_STR_OSXL(_ie , _len, _val)                  \
{                                                               \
  _ie.pres = TRUE;                                              \
  _ie.len =  _len;                                              \
  _ie.val = _val;                                               \
}   

/* ccpu00129775 */
/* The value of SS_TICKS_SEC is 10 if SS_4GMX_LCORE is defined
 * So CZ_TMR_RES is set to 10, so that ms is converted to secs  */
#define CZ_TMR_RES 10
#define CZ_CALC_DELAY_TIME(_delay) (_delay/((1000/SS_TICKS_SEC) * CZ_TMR_RES))
/* ccpu00129775 */

/* To convert from seconds to milliseconds */
#define CZ_CONV_SEC_TO_MSEC(_sec)  \
{                                  \
   _sec *= 1000;                   \
}             

/**
 * @def CZ_CHK_LYR_CFG Macro to check if General configuration and Protocol configuration is done 
 */
#define CZ_CHK_LYR_CFG(_fn, ret) \
   {\
      ret = ROK;\
      if (czCb.init.cfgDone != TRUE) \
      { \
         CZ_DBG_ERROR((czCb.init.prntBuf, "_fn: General Configuration not" \
                       "done")); \
         ret = LCM_REASON_GENCFG_NOT_DONE;\
      }\
      if((ret == ROK) && (czCb.nodeCb.cfgDone != TRUE))\
      {\
         CZ_DBG_ERROR((czCb.init.prntBuf, "_fn: Protocol Configuration not"\
                       "done")); \
         ret = LCZ_REASON_PROTCFG_NOT_DONE;\
      }\
   }

#ifdef ALIGN_64BIT
#define CZ_PRNT_PEERID CZ_DBG_INFO((CZ_PRNT_BUF, "For peer with peerId (%d)", \
                                                            peerCb->peerId));
#else /* else ALIGN_64BIT */
#define CZ_PRNT_PEERID CZ_DBG_INFO((CZ_PRNT_BUF, "For peer with peerId (%ld)",\
                                                             peerCb->peerId));
#endif /* ALIGN_64BIT */
/* cz001.301: Diagnostics changes */
#ifdef SS_DIAG
/* Note: Any changes to these enums should reflect to */
/** @details Enums for special argument
 *
*/
typedef enum
{
   CZ_DIAG_PEERID,
   CZ_DIAG_SUASSOCID,
   CZ_DIAG_NA
} CzDiagSplArg;
 
#define CZ_DIAG_LVL0(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                                             \
   if(czCb.init.logMask & SS_DIAG_LVL0)                             \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTCZ, czCb.init.inst, SS_DIAG_LVL0, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }\
}

/** @details Macro definition for X2AP error logs
 *  
*/
#define CZ_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \
{                                                                 \
   if(czCb.init.logMask & SS_DIAG_LVL1)                             \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTCZ, czCb.init.inst, SS_DIAG_LVL1, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for X2AP critical logs
 *  
*/
#define CZ_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
   if(czCb.init.logMask & SS_DIAG_LVL2)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTCZ, czCb.init.inst, SS_DIAG_LVL2, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for X2AP logs 
 *  
*/
#define CZ_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(czCb.init.logMask & SS_DIAG_LVL3)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTCZ, czCb.init.inst, SS_DIAG_LVL3, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for X2AP logs
 *  
*/
#define CZ_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(czCb.init.logMask & SS_DIAG_LVL4)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTCZ, czCb.init.inst, SS_DIAG_LVL4, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}
 
/** 
 * @details Macro definition for X2AP Hex-Dump
 */
#define CZ_DIAG_MSG(_mBuf, _str)                                                                  \
{                                                                                                \
   if(czCb.init.logMask & SS_DIAG_MSG)                                                      \
   {                                                                                             \
      Buffer  *_newBuf;                                                                          \
      SAddMsgRef(_mBuf, czCb.init.region, czCb.init.pool, &_newBuf);                             \
      ssDiagFix(0, CZ_DIAG_NA, ENTCZ, czCb.init.inst, SS_DIAG_MSG, SS_DIAG_MSG_TYPE_HEXDUMP,(PTR)_newBuf, 0, 0, 0,0, _str);\
   }                                                                                             \
}

/** @details Macro definition for TeNB APP Varible length logs
 *  
*/
#define CZ_DIAG_VAR(_tknId, _splArgEnum, _splArg, _string, _stringPtr)                                   \
{                                                                                                        \
   if(czCb.init.logMask & SS_DIAG_VAR)                                                              \
   {                                                                                                     \
      ssDiagFix(_tknId, _splArgEnum, ENTCZ, czCb.init.inst, SS_DIAG_VAR, SS_DIAG_MSG_TYPE_FIXED, _splArg, 0, 0, 0, 0, _string);\
      ssDiagVarible(ENTCZ, czCb.init.inst, SS_DIAG_VAR, SS_DIAG_MSG_TYPE_VAR, _tknId, _splArg, _stringPtr);\
   }                                                                                                     \
}

#else

#define CZ_DIAG_LVL0(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \

/** @details Macro definition for X2AP error logs
 *  
*/
#define CZ_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \

/** @details Macro definition for X2AP critical logs
 *  
*/
#define CZ_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \

/** @details Macro definition for X2AP logs 
 *  
*/
#define CZ_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \

/** @details Macro definition for X2AP logs
 *  
*/
#define CZ_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
 
/** 
 * @details Macro definition for X2AP Hex-Dump
 */
#define CZ_DIAG_MSG(_mBuf, _str)                                                                  \

/** @details Macro definition for TeNB APP Varible length logs
 *  
*/
#define CZ_DIAG_VAR(_tknId, _splArgEnum, _splArg, _string, _stringPtr)                                   \

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SS_DIAG */

#endif /* __CZH__ */


/**********************************************************************
  
         End of file:     cz.h@@/main/2 - Tue Aug 30 18:36:04 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
$SID$       cz001.301   akaranth        1. SCTP Protocol id added
                                        2. Diagnostics changes. 
                                        3. removed Null check for the CZ_SET_ZERO macro.
*********************************************************************91*/
