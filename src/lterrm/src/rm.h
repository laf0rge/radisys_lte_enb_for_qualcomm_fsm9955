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

     Name:     RRM Application

     Type:     CPP include file

     Desc:     This file contains 
               

     File:     rm.h

     Sid:      rm.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:37 2013

     Prg:      hkrishna, jbm 

*********************************************************************21*/

#ifndef RM_H_
#define RM_H_

/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"


/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"
#include "rl_interface.h"

#define RM_MAX_UPPERSAP               1
#define RM_MAX_LOWERSAP               1 
#define RM_TQ_SIZE                    10   /* Timing Queue Size */
#define RM_MAX_TIMER     RM_MAX_LOWERSAP   /* MAX number of RRM timers */
#define RM_BNDREQ_TMR    1    /* Bind Request timer event value */
#define RM_MAX_BNDRETRY  2    /* Max number of Bind Retries for TFU SAP */

/* SSI related constants */
#define RM_MEM_PDU_SIZE   1024
#define RM_MEM_REGION     g_stRmCb.rmInit.region
#define RM_MEM_POOL       g_stRmCb.rmInit.pool
#define RM_PRNT_BUF       g_stRmCb.rmInit.prntBuf
#define RM_INST_ID        1

/** @brief Constant representing the number of bits per Byte
 */
#define RM_NUM_BITS_PER_BYTE        8

/** @brief This macro is used to get the ceil value of the quotient
 */
#define RM_CEIL(x, y) (((x) + (y)-1) / (y))

/** @brief The below Macros are used to return values in case of DRX */
#define RM_RCM_DRX_LONG_SHORT_CFG    1
#define RM_RCM_DRX_LONG_CFG          2
#define RM_RCM_DRX_NOT_CFG           0

/** @brief This Macro is used to configure Short DRX */
#define RM_RCM_RES_CFG_DRX_WITH_SHORT     0x40

/* Macros for Basic Algo */
#define RM_START_RI_IDX             161
#define RM_DFLT_CQI_PRD			      80
#define RM_DFLT_CQI_PMI_IDX		   78
#define RM_START_CQI_PMI_IDX		   77

/**
 * @details Macros for Static memory allocation/free
 */
/* Buf allocation */
#define RM_ALLOC(_buf, _size)                                     \
{                                                                 \
   if (SGetSBuf(RM_MEM_REGION, RM_MEM_POOL,                       \
            (Data **)(_buf), (_size)) == ROK)                     \
   {                                                              \
      RM_SET_ZERO((*(_buf)), (_size));                            \
   }                                                              \
   else                                                           \
   {                                                              \
      (*(_buf)) = NULLP;                                          \
   }                                                              \
}

#ifdef SS_LOCKLESS_MEMORY
#define RM_FREE_SHAREBLE_BUF(_buf, _size)   \
{\
      (Void) SPutStaticBuffer(RM_MEM_REGION, RM_MEM_POOL, (Data *) _buf, \
                                       (Size) _size, SS_SHARABLE_MEMORY); \
      (_buf) = NULLP;\
}
#else
#define RM_FREE_SHAREBLE_BUF(_buf, _size)   \
{\
         (Void) SPutSBuf(RM_MEM_REGION, RM_MEM_POOL, (Data *) _buf, (Size) _size); \
         (_buf) = NULLP;\
}

#endif

/* Buf de-allocation */
#define RM_FREE(_buf, _size)                                      \
{                                                                 \
   (Void) SPutSBuf(RM_MEM_REGION, RM_MEM_POOL,                    \
                   (Data *) (_buf), (Size) (_size));              \
   (_buf) = NULLP;                                                \
}

/**  
 * @details Macro for event related memory operations.
 */

/* initialize the memCp & allocate memory for the event structure */
#define RM_ALLOCEVNT(_evntPtr, _size)                             \
{                                                                 \
   (*_evntPtr) = NULLP;                                           \
   cmAllocEvnt((Size) (_size), RM_MEM_PDU_SIZE, &g_stRmCb.mem,    \
                     (Ptr *)(_evntPtr));                          \
}

/* allocate memory for members in the event structure */
#define RM_GET_MEM(_memPtr, _size, _allocPtr)                     \
{                                                                 \
   (*_allocPtr) = NULLP;                                          \
   cmGetMem((U8 *)(_memPtr), (_size),                             \
            (Ptr *)(_allocPtr));                                  \
}

/* free allocated memory for an event structure */
#define RM_FREEEVNT(_evntPtr)                                     \
{                                                                 \
   cmFreeMem((_evntPtr));                                         \
   (_evntPtr) = NULLP;                                            \
}

/* Macros for generic memory operations */
#define RM_MEM_COPY(_dst, _src, _len)                             \
{                                                                 \
   cmMemcpy((U8*) (_dst), (U8*) (_src), (_len));                  \
}

#define RM_SET_ZERO(_buf, _size)                                  \
{                                                                 \
   cmMemset((U8 *)(_buf), 0, (_size));                            \
}


#define RM_RCM_GET_HALF(val)  ((val)/2)

#ifdef DEBUGP
#define RM_LOGERROR(errCls, errCode, errVal, errDesc)             \
do                                                                \
{                                                                 \
   {                                                              \
    SLogError(g_stRmCb.rmInit.ent, g_stRmCb.rmInit.inst, g_stRmCb.rmInit.procId,    \
          (Txt *)__FILE__, __LINE__, errCls, errCode,             \
          (ErrVal)errVal, (Txt *)errDesc);                        \
   }                                                              \
}while(0)

#define RMLAYERNAME     "LTE RRM"
#define RM_DBGMASK_ERROR        1
#define RM_DBGMASK_WARNING      2
#define RM_DBGMASK_INFO         3

#define RM_DBG_ERROR(_args) \
{ \
   DBGP(&(g_stRmCb.rmInit), (Txt *)RMLAYERNAME, RM_DBGMASK_ERROR, _args); \
}

#define RM_DBG_WARNING(_args) \
{ \
   DBGP(&(g_stRmCb.rmInit), (Txt *)RMLAYERNAME, RM_DBGMASK_WARNING, _args); \
}

#define RM_DBG_INFO(_args) \
{ \
   DBGP(&(g_stRmCb.rmInit), (Txt *)RMLAYERNAME, RM_DBGMASK_INFO, _args); \
}
#else
#define RM_LOGERROR(errCls, errCode, errVal, errDesc)
#define RM_DBG_ERROR(_args)
#define RM_DBG_WARNING(_args)
#define RM_DBG_INFO(_args)
#endif


/** @enum RmUeTxnMode
 *
 * Represents Transmission modes defined in TS 36.213. This should be same
 * as defined for transmissionMode (AntennaInfoDedicated) in TS 36.331.
 *
 * - RM_UE_TXN_MODE1           0
 * - RM_UE_TXN_MODE2           1
 * - RM_UE_TXN_MODE3           2
 * - RM_UE_TXN_MODE4           3
 * - RM_UE_TXN_MODE5           4
 * - RM_UE_TXN_MODE6           5
 * - RM_UE_TXN_MODE7           6
 */
typedef enum RmUeTxnMode
{
   RM_UE_TXN_MODE1,
   RM_UE_TXN_MODE2,
   RM_UE_TXN_MODE3,
   RM_UE_TXN_MODE4,
   RM_UE_TXN_MODE5,
   RM_UE_TXN_MODE6,
   RM_UE_TXN_MODE7
} RmUeTxnMode;



/*****************************************************
**********        Data Structures          ***********
******************************************************/

/** @brief This structure contains information about the Application's lower
 * SAP.
 - Pst        pst          Post structure
 - SuId       suId         Service user ID
 - SpId       spId         Service provider ID
 - State      sapState     SAP State (Bound/Unbound)
 - Mem        mem          Region/Pool
 - CmTimer    timer        Timer for Bind
 - U8         bndRetryCnt  Number of Bind Retrys
 - U8         maxBndRetry  Max number of Bind Retrys
 - TmrCfg     bndTmr       Bind Timer configuration
 */
typedef struct rmUiSapCb
{
   Pst                       pst;
   SuId                      suId;
   SpId                      spId;
   State                     sapState;
   Mem                       mem;
   CmTimer                   timer;
   U8                        bndRetryCnt;
   U8                        maxBndRetry;
   U8                        numBndRetries;
   TmrCfg                    bndTmr;
} RmUiSapCb;

/** @brief This structure contains information about the Application's lower
 * SAP.
 - Pst        pst          Post structure
 - SuId       suId         Service user ID
 - SpId       spId         Service provider ID
 - State      sapState     SAP State (Bound/Unbound)
 - Mem        mem          Region/Pool
 - CmTimer    timer        Timer for Bind
 - U8         bndRetryCnt  Number of Bind Retrys
 - U8         maxBndRetry  Max number of Bind Retrys
 - TmrCfg     bndTmr       Bind Timer configuration
 */
typedef struct rmliSapCb
{
   Pst                       pst;
   SuId                      suId;
   SpId                      spId;
   State                     sapState;
   Mem                       mem;
   CmTimer                   timer;
   U8                        bndRetryCnt;
   U8                        maxBndRetry;
   U8                        numBndRetries;
   TmrCfg                    bndTmr;
} RmLiSapCb;


/** 
  * @brief
  * structure holding Layer Manager Response information cached from control
  * request.
  */
typedef struct rmLmResponse
{
   TranId transId;           /*!< Transaction ID */
   Resp   response;          /*!< Response */
}RmLmResponse;


/** 
  * @brief
  * Structure holding LTE RRM's General Configuration Information.
  */
typedef struct rmGenCb
{
   U8           tmrRes;           /*!< Timer resolution */
   RmLmResponse bndCfmResp;       /*!< Respones value for Bind Confirm */
   Status       memUpperThr;      /*!< upper threshold of the memory pool */
   Status       memLowerThr;      /*!< lower threshold of the memory pool */
}RmGenCb;


/** @brief This structure is the Control block for the application intialized at
 * the time of wrActvInit.
 - Mem         mem                  Memory Region and Pool information
 - CmTqCp      tqCp                 Timing Queue Control point
 - CmTqType    tq[RM_NUM_TQENTRY]   Timing Queue for Application
 - TskInit     rmInit                 Init structure
 - Header      lmHdr                Layer Manager Header
 - RmUiSapCb   rmuSap               FSM SAP
 - RmLiSapCb   rgmSap               RRM SAP
 */
typedef struct rmCb
{
   Mem                       mem; /* memory pool info */
   RmGenCb                   genCfg;
   CmTqCp                    tmrTqCp;           /* Timing Queue Control point    */
   CmTqType                  tmrTq[RM_TQ_SIZE]; /* Timing Queue for a resln. */
   TskInit                   rmInit;
   Header                    lmHdr;
   RmUiSapCb                 rmuSap;
   RmLiSapCb                 rgmSap;
   CmTimer                   tmrBlk[RM_MAX_TIMER];      /*!< Timer Block */
} RmCb;

/* APIs exposed by LIM */

EXTERN S16 rmLIMRgmBndReq ARGS((
         SuId                 suId,
         SpId                 spId));

EXTERN S16 rmLIMRgmUbndReq ARGS((
         SuId                 suId,
         Reason               reason));

/*
 * APIs exposed by Lrm
 */
EXTERN S16 rmLrmStartTmr ARGS((S16 tmrEvnt, U32 tmrVal, PTR cb));
EXTERN S16 rmLrmStopTmr ARGS((S16 tmrEvnt, PTR cb));
EXTERN S16 rmLrmTmrExpiry ARGS((PTR cb, S16 tmrEvnt));

/* LIM uses this to forward the BndCfm to Lrm. This function invokes a
 * Control Confirmation to the LM. */
EXTERN S16 rmLrmBndCfm ARGS((SuId suId, U8 status));
EXTERN S16 rmActvTmr ARGS((Ent ent, Inst inst ));



#endif /* RM_H_ */
/********************************************************************30**

           End of file:     rm.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:37 2013

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
*********************************************************************91*/
