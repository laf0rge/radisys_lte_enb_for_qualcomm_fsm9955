/********************************************************************16**

                         (c) COPYRIGHT 1989-2005 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

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
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/********************************************************************20**
  
        Name:    Common Umts  
    
        Type:     C include file
  
        Desc:    This file Contains the Data structures for Common UMTS
 
        File:   cm_umts.x 

        Sid:      cm_umts.x@@/main/12 - Fri Sep 12 13:47:59 2008
  
        Prg:     Saurabh 
  
*********************************************************************21*/

#ifndef __CM_UMTS_X__
#define __CM_UMTS_X__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*__cplusplus*/

#include "cm_llist.h"
#include "cm_tkns.h"

#include "cm_llist.x"
#include "cm_tkns.x"

/* defination for Common UMTS */
typedef U8 UmtsRbId;          /* Radio Bearer ID */
typedef U32 UmtsUrnti;        /* Urnti UE Id */
typedef U16 UmtsCrnti;        /* Crnti UE Id */
typedef U8 UmtsLogChType;     /* Logical Channel Type */
typedef U8 UmtsRlcSvcType;    /* Rlc Service Type */
typedef U8 UmtsOperatingMode; /* UMTS Operating Mode */
typedef U8 UmtsUeType;        /* UE type */
typedef U16 UmtsHrnti;
typedef U16 UmtsErnti;        /* ERNTI */
typedef U32 UmtsCellId;
typedef U32 UmtsCRNCCtxId;
/*  Common UMTS CPICH Info List */
typedef struct umtsCPICHInfo
{
   Bool     barredStatus;           /* indicates if the cell is barred */
   U16      primScramblingCode;     /* scrambling code of Primary CPICH */
} UmtsCPICHInfo;

/*  Common UMTS Freq. Info List */
typedef struct umtsFreqInfo
{
   Bool               barredStatus; /* indicates if the Frequency is barred */
   U16                uarfcnUl;
   U16                uarfcnDl;
   U8                 numCells;
   UmtsCPICHInfo      *primCPICHInfo;
} UmtsFreqInfo;

/*  Common UMTS cell Info List */
typedef struct umtsCellInfoLst
{
   U8                numFddCell;
   UmtsFreqInfo      *cellInfo;
} UmtsCellInfoLst;

typedef struct umtsCctrchId
{
   U8 cctrchIdentity;
   U8 tFlag;
   union {
      UmtsCellId cellId;
      U32 ueId;
   } t;
}UmtsCctrchId;

typedef struct umtsTrChId
{
   UmtsCctrchId cctrchId;   /* CCTrCh Id */
   U8 chType;      /* Transport Channel Type */
   U16 chIdentity; /* Transport Channel identity */
#ifdef NODEB
   U8   rlId;     /* Radio Link Id */
#endif /* NODEB */
}UmtsTrChId;

typedef struct umtsRlcId    /* Umts Rlc Id */
{
   U8 tFlag;
   union {
   U32      ueId;       /* UE ID */
   U32      cellId;    /* Cell ID */
   } t;
   U8 rbId;
}UmtsRlcId;


typedef struct umtsPdcpId
{
   U8        cmnOrDed;
   union {
      UmtsUrnti ueId; /* ue ID information*/
      U32       cmnId; /* For MTCH RBs */
   } u;
   UmtsRbId rbId;  /* radio bearer ID*/
} UmtsPdcpId;

typedef struct umtsLogChId   /* Umts Logical Channel Id */
{
   UmtsLogChType logCh;   /* Channel Type */
   U8 inst;               /* Logical Channel Instance */
   union {
      U32 ueId;
      U32 cellId;
   } type;
   U8 logChId;
}UmtsLogChId;

typedef struct umtsPiBitMap
{
   U8 len;
   U8 map[UMTS_MAX_PI_BITMAP_LEN];
} UmtsPiBitMap;
/* CMK Interface Structure */


/* cm_umts_x_001.main_11
   Code cleanup : removed RRC related structures from common file */

/* Packing and Unpacking function prototype */
EXTERN S16 cmUnpkUmtsRlcId ARGS((UmtsRlcId *rlcId, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsUrnti ARGS((UmtsUrnti *ueId, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsCrnti  ARGS((UmtsCrnti *ueId, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsRbId ARGS((UmtsRbId *rbId,  Buffer *mBuf));
EXTERN S16 cmUnpkUmtsLogChId ARGS((UmtsLogChId *logChId, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsLogChType ARGS((UmtsLogChType *logCh, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsRlcSvcType ARGS((UmtsRlcSvcType *svc, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsOperatingMode ARGS((UmtsOperatingMode *mode,Buffer *mBuf));
EXTERN S16 cmUnpkUmtsTrChId ARGS((UmtsTrChId *trCh, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsPiBitMap ARGS((UmtsPiBitMap *pi, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsCPICHInfo ARGS (( UmtsCPICHInfo *param, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsFreqInfo ARGS ((UmtsFreqInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkUmtsCellInfoLst ARGS ((UmtsCellInfoLst *param,Ptr ptr,Buffer *mBuf));

EXTERN S16 cmPkUmtsTrChId ARGS((UmtsTrChId *trCh, Buffer *mBuf));
EXTERN S16 cmPkUmtsOperatingMode ARGS((UmtsOperatingMode *mode, Buffer *mBuf));
EXTERN S16 cmPkUmtsRlcSvcType ARGS((UmtsRlcSvcType *svc, Buffer *mBuf));
EXTERN S16 cmPkUmtsLogChId ARGS((UmtsLogChId *logChId, Buffer *mBuf));
EXTERN S16 cmPkUmtsLogChType ARGS((UmtsLogChType *logCh, Buffer *mBuf));
EXTERN S16 cmPkUmtsRlcId ARGS((UmtsRlcId *rlcId, Buffer *mBuf));
EXTERN S16 cmPkUmtsCrnti ARGS((UmtsCrnti *ueId, Buffer *mBuf));
EXTERN S16 cmPkUmtsUrnti ARGS((UmtsUrnti *ueId, Buffer *mBuf));
EXTERN S16 cmPkUmtsRbId ARGS((UmtsRbId *rbId,  Buffer *mBuf));
EXTERN S16 cmPkUmtsPiBitMap ARGS((UmtsPiBitMap *pi, Buffer *mBuf));
EXTERN S16 cmPkUmtsCPICHInfo ARGS (( UmtsCPICHInfo *param, Buffer *mBuf));
EXTERN S16 cmPkUmtsFreqInfo ARGS (( UmtsFreqInfo *param, Buffer *mBuf));
EXTERN S16 cmPkUmtsCellInfoLst ARGS (( UmtsCellInfoLst *param, Buffer *mBuf));

EXTERN S16 cmPkUmtsCellId ARGS((UmtsCellId *param,Buffer *mBuf));
EXTERN S16 cmPkUmtsHrnti ARGS((UmtsHrnti *param,Buffer *mBuf));
EXTERN S16 cmPkUmtsCRNCCtxId ARGS((UmtsCRNCCtxId *ctxId,Buffer *mBuf));

EXTERN S16 cmUnpkUmtsCellId ARGS((UmtsCellId *param,Buffer *mBuf));
EXTERN S16 cmUnpkUmtsHrnti ARGS((UmtsHrnti *param,Buffer *mBuf));
EXTERN S16 cmUnpkUmtsCRNCCtxId ARGS((UmtsCRNCCtxId *param,Buffer *mBuf));

/* Pack-unpack prototypes for CMK interface */

EXTERN S16 cmUnpkUmtsCctrchId ARGS ((
UmtsCctrchId *ccTrCh,
Buffer *mBuf
));

EXTERN S16 cmPkUmtsCctrchId ARGS ((
UmtsCctrchId *ccTrCh,
Buffer *mBuf
));

EXTERN S16 cmPkUmtsPdcpId ARGS((UmtsPdcpId *PdcpId, Buffer *mBuf));
EXTERN S16 cmUnpkUmtsPdcpId ARGS((UmtsPdcpId *pdcpId, Buffer *mBuf));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CM_UMTS_X__ */

/********************************************************************30**
  
         End of file:     cm_umts.x@@/main/12 - Fri Sep 12 13:47:59 2008
  
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
----------- -------- ---- -----------------------------------------------
/main/2      ---      na   1. initial release.
/main/2      ---      tsm  2. Modifications for RRC release
/main/3      ---   jz    3. RLC Release 1.2.
/main/4      ---   vt 1. MAC Release 1.2
/main/5      ---    rbabu 1. Updated for NBAP software release 1.2
/main/6      ---    rbabu 1. Updated UMTS Transport Channel Id.
                             a)Radio Link Id.
                             b) Cell Id.
                             c) Cell Id.
/main/7      ---   gs 1. Updated for MAC-HS software release 1.1
/main/8      ---   rbabu 1. Update for PDCP Software release 1.1
/main/9      ---   apany  1. UmtsLogChId change
                          2. UmtsPdcpId change
                          3. Removed MkCipherElm
                          4. Removed UmtsMkuId
/main/10     ---   ap     1. RLC Release 2.1
/main/11     ---   sn     1. MAC-HS Release 2.1
/main/12     ---   sg     1. cm_umts_x_001.main_11 Code cleanup :
                             removed RRC related structures from common file
*********************************************************************91*/
