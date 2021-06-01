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
 
        File:     pj_lib.x

        Sid:      eq_lib.x@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:42:49 2014
  
        Prg:      na 
  
*********************************************************************21*/

#ifndef __PJ_LIB_X__
#define __PJ_LIB_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef PJ_CMP_ASYNC

typedef S16 (*ObdCmpInitCfm) ARGS((
   Pst *                pst,
   PjCmpCxt             cmpCxt,
   PTR                  ctxId));

typedef S16 (*ObdCmpCfm) ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu));

typedef S16 (*ObdDecmpCfm) ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu,
   Buffer *             fbPkt
));

/* Library function externs */
EXTERN S16 PjLibObdCmpInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PTR      ctxId                    /* Compression Context ID */
));
EXTERN S16 PjLibObdCmpCfm ARGS((
Pst        *pst,        /* post structure             */
PTR         cmpCxtId,   /* Context Id for compression */
PjLibTrans  libTrans,   /* Transaction Id             */
Buffer     *opSdu       /* compressed SDU             */
));
EXTERN S16 PjLibObdDecmpCfm ARGS((
Pst         *pst,                 /* post structure */
PTR         cmpCxtId,             /* Context Id for decompression */
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opSdu,               /* decompressed SDU */
Buffer      *fbPkt                /* Feedback pkt */
));
EXTERN S16 PjLibObdCmpInitReq ARGS((
Pst * pst,
PjCmpCxt cmpCxt,
PjRohc rohc
));
EXTERN S16 PjLibObdCmpReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdDecmpReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdCmpCloseReq ARGS((
Pst * pst,
PTR cmpCxtId
));
EXTERN S16 PjLibObdCmpResetReq ARGS((
Pst * pst,
PTR cmpCxtId
));
EXTERN S16 PjLibObdCmpFbReq  ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans libTrans,             /* Transaction Id */
Buffer   *feedback                /* SDU to be decompressed */
));

EXTERN S16 cmPkLibObdCmpInitCfm ARGS((
Pst * pst,
PjCmpCxt cmpCxt,
PTR ctxId
));

EXTERN S16 cmUnpkLibObdCmpInitCfm ARGS((
ObdCmpInitCfm func,
Pst *pst,
Buffer *mBuf
));

EXTERN S16 cmPkLibObdCmpCfm ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdCmpCfm ARGS((
   ObdCmpCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdDecmpCfm ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu,
   Buffer *             fbPkt
));

EXTERN S16 cmUnpkLibObdDecmpCfm ARGS((
   ObdDecmpCfm          func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkPjRohc ARGS((
   PjRohc               *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjRohc ARGS((
   PjRohc               *param,
   Buffer               *mBuf
));
EXTERN S16 cmPkPjCmpCxt ARGS((
   PjCmpCxt             *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjCmpCxt ARGS((
   PjCmpCxt             *param,
   Buffer               *mBuf
));

#else

EXTERN S16 pjLibCmpInitReq  ARGS((
PjCmpCxt cmpCxt,                  
PjRohc   rohc,                    
PTR      *cmpCxtId                
));
EXTERN S16 pjLibCmpReq  ARGS((
PTR      cmpCxtId,                
Buffer   *mBuf,                   
Buffer   **opSdu                  
));
EXTERN S16 pjLibDecmpReq  ARGS((
PTR      cmpCxtId,                
Buffer   *mBuf,                   
Buffer   **opSdu,                 
Buffer   **feedback               
));
EXTERN S16 pjLibCmpCloseReq ARGS((
PTR      cmpCxtId                 
));
EXTERN S16 pjLibCmpResetReq ARGS((
PTR      cmpCxtId                 
));
EXTERN S16 pjLibCmpFbReq ARGS((
PTR      cmpCxtId,
Buffer   *feedback
));

#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC

typedef S16 (*ObdIntInitCfm) ARGS((
   Pst *                pst,
   PjSecCxt             secCtx,
   PTR                  ctxId));

typedef S16 (*ObdCpInitCfm) ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId));

typedef S16 (*ObdUpInitCfm) ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId));


typedef S16 (*ObdCipherCfm) ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu));

typedef S16 (*ObdDecipherCfm) ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu));


EXTERN S16 PjLibObdIntInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Integrity Context ID */
));
EXTERN S16 PjLibObdCpInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
));
EXTERN S16 PjLibObdUpInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
));
EXTERN S16 PjLibObdIntInitReq ARGS((
Pst * pst,
PjSecCxt secCtx,
PjIntInfo intInfo
));
EXTERN S16 PjLibObdCpInitReq ARGS((
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *cpKey
));
EXTERN S16 PjLibObdUpInitReq ARGS((
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *upKey
));
EXTERN S16 PjLibObdIntProtReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer ** mBuf
));
EXTERN S16 PjLibObdCipherReq ARGS((
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdIntVerReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32 macI
));
EXTERN S16 PjLibObdDecipherReq ARGS((
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdIntCloseReq ARGS((
Pst * pst,
PTR intCxtId
));
EXTERN S16 PjLibObdCipherCloseReq ARGS((
Pst * pst,
PTR cpCxtId
));

EXTERN S16 cmPkLibObdIntInitCfm ARGS((
   Pst *                pst,
   PjSecCxt             secCtx,
   PTR                  ctxId
));

EXTERN S16 cmUnpkLibObdIntInitCfm ARGS((
   ObdIntInitCfm        func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdCpInitCfm ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId
));

EXTERN S16 cmUnpkLibObdCpInitCfm ARGS((
   ObdCpInitCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdUpInitCfm ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId
));

EXTERN S16 cmUnpkLibObdUpInitCfm ARGS((
   ObdUpInitCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdIntProtCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   U32                  macI
));

EXTERN S16 cmUnpkLibObdIntProtCfm ARGS((
   ObdIntProtCfm        func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdCipherCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdCipherCfm ARGS((
   ObdCipherCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdDecipherCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdDecipherCfm ARGS((
   ObdDecipherCfm       func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdIntVerCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Status               status
));

EXTERN S16 cmUnpkLibObdIntVerCfm ARGS((
   ObdIntVerCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkPjSecCxt ARGS((
   PjSecCxt             *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjSecCxt ARGS((
   PjSecCxt             *param,
   Buffer               *mBuf
));
EXTERN S16 cmPkPjIntInfo ARGS((
   PjIntInfo            *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjIntInfo ARGS((
   PjIntInfo            *param,
   Buffer               *mBuf
));
EXTERN S16 cmPkPjSecInp ARGS((
   PjSecInp             *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjSecInp ARGS((
   PjSecInp             *param,
   Buffer               *mBuf
));

#else

EXTERN S16 PjLibObdUlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 PjLibObdDlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 pjLibObdPrcKenbCfg ARGS ((
PjCb                   *gCb,
CpjAsKeyInfo*       kenbInf,
CpjAskeyCfmInfo     *kdfCfm
));

EXTERN S16 pjLibObdPrcKenbStarCfg ARGS ((
PjCb                   *gCb,
CpjKenbStarInfo*       kenbInf,
CpjKenbStarCfmInfo     *kdfCfm
));

EXTERN S16 PjLibObdDlCloseReq ARGS ((Void));

EXTERN S16 PjLibObdUlCloseReq ARGS ((Void));

EXTERN S16 PjLibObdIntInitReq ARGS ((
PjSec *secInp
));

EXTERN Void PjUlSpaccTrigger ARGS ((Void));

EXTERN Void PjDlSpaccTrigger ARGS ((Void));


EXTERN S16 PjLibObdIntProtReq ARGS ((
PjLibTrans  *libTrans, 
Buffer **mBuf
));


#ifdef TENB_AS_SECURITY
EXTERN S16 PjLibObdDlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 PjLibObdUlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 PjLibObdDlCloseReq ARGS ((Void));

EXTERN S16 PjLibObdUlCloseReq ARGS ((Void));
#endif

#ifdef TENB_AS_SECURITY
EXTERN S16 PjLibObdIntVerReq ARGS ((
PjLibTrans  *libTrans,
Buffer * mBuf,
U32   macI
));
#else
EXTERN S16 PjLibObdIntVerReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32 macI
));

#endif

EXTERN S16 PjLibObdIntCloseReq ARGS ((
Void *intCxtId
));


EXTERN S16 PjLibObdCpInitReq ARGS ((
PjSec *secInp
));

EXTERN S16 PjLibObdUpInitReq ARGS ((
PjSec *secInp
));


EXTERN S16 PjLibObdCipherReq ARGS ((
PjLibTrans  *libTrans,
Buffer      *mBuf,
Buffer      **opSdu
));

#ifdef FLAT_BUFFER_OPT
EXTERN S16 PjLibObdCipherReqFB ARGS ((
PjLibTrans  *libTrans,
FlatBuffer  *mBuf,
Buffer      **opSdu
));
#endif

EXTERN S16 PjLibObdDecipherReq ARGS ((
PjLibTrans  *libTrans,
Buffer      *mBuf,
Buffer      **opSdu
));


EXTERN S16 PjLibObdCipherCloseReq ARGS ((
Void *cpCxtId
));

EXTERN S16 pjLibIntInitReq ARGS (( 
PjSecCxt secCxt,                  /* Context for Intg protection/verification */
PjIntInfo intInfo,                /* Integrity Cfg Info */
Void      **cxtId                  /* FD */
));

EXTERN S16 pjLibCpInitReq ARGS ((
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                  /* Control plane ciphering key */
Void     **cxtId                   /* FD */
));

EXTERN S16 pjLibUpInitReq ARGS ((
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                  /* User plane ciphering key */
Void     **cxtId                   /* FD */
));
EXTERN S16 pjLibDecipherReq ARGS((
PTR         intCxtId,           /* Context Id for compression */
PjSecInp    secInp,             /* Input for Deciphering */
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
));
EXTERN S16 pjLibIntVerReq  ARGS((
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
Buffer     *mBuf,                   /* SDU to be compressed */
U32        macI,                    /* Message authentication code for the SDU */
Status     *status                  /* Status of the Integrity verification */
));
#endif

#if defined(PJ_CMP_ASYNC) || defined(PJ_SEC_ASYNC)
EXTERN S16 cmPkPjLibTrans ARGS((
   PjLibTrans           *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjLibTrans ARGS((
   PjLibTrans           *param,
   Buffer               *mBuf
));

#endif /* PJ_CMP_ASYNC || PJ_SEC_ASYNC */
/* kw003.201 */

/* kw004.201 Guarded under PTPJLIB */
#ifdef PTPJLIB 
EXTERN S16 pjLibPktAdmCtrl ARGS((
   PjDlRbCb             *pjRbCb,  
   Buffer               *pkt
));
#endif

typedef S16 (*ObdIntProtCfm) ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   U32                  macI));

typedef S16 (*ObdIntVerCfm) ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   Status               status));

EXTERN S16 PjLibObdCipherCfm ARGS((
Pst      *pst,                    /* post structure */
PjLibTrans *libTrans,              /* Transaction Id */
Buffer   *opSdu                   /* ciphered SDU */
));
EXTERN S16 PjLibObdDecipherCfm ARGS((
Pst         *pst,                 /* post structure */
PjLibTrans  *libTrans,             /* Transaction Id */
Buffer      *opPdu                /* deciphered SDU */
));
EXTERN S16 PjLibObdIntProtCfm ARGS((
Pst      *pst,                    /* post structure */
PjLibTrans *libTrans,              /* Transaction Id */
U32      macI                     /* Message authentication code */
));

EXTERN S16 PjLibObdIntVerCfm ARGS((
Pst      *pst,                    /* post structure */
PjLibTrans *libTrans,              /* Transaction Id */
Status   status                   /* Integrity verification status */
));
EXTERN S16 cmPkLibObdIntProtCfm ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   U32                  macI
));
EXTERN S16 cmPkLibObdIntVerCfm ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   Status               status
));
EXTERN S16 cmPkLibObdCipherCfm ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   Buffer *             opSdu
));
typedef S16 (*ObdCipherCfm) ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   Buffer *             opSdu
));
typedef S16 (*ObdDecipherCfm) ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   Buffer *             opSdu));

EXTERN S16 cmUnpkLibObdCipherCfm ARGS((
   ObdCipherCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmUnpkLibObdIntVerCfm ARGS((
   ObdIntVerCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmUnpkLibObdIntProtCfm ARGS((
   ObdIntProtCfm        func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdDecipherCfm ARGS((
   Pst *                pst,
   PjLibTrans           *libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdDecipherCfm ARGS((
   ObdDecipherCfm       func,
   Pst *                pst,
   Buffer               *mBuf
));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PJ_LIB_X__ */
  
/********************************************************************30**
  
         End of file:     eq_lib.x@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:42:49 2014
  
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
/main/2      kw003.201 vp  1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/3      kw004.201 ap  1. Function pjLibPktAdmCtrl have been guarded under
                              PTPJLIB flag
*********************************************************************91*/
