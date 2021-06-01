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

     Name:     common hash functions

     Type:     C include file

     Desc:     Structures, variables and typedefs required by common
               functions.
               (Newer version of functions in cm_bdy1)

     File:     cm_hash.x

     Sid:      cm_hash.x@@/main/13 - Thu Jul 10 17:27:41 2008

     Prg:      na

*********************************************************************21*/

#ifndef __CMHASHX__
#define __CMHASHX__

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

/* forward definitions */

typedef struct cmHashListCp CmHashListCp;     /*  hash list control point */


/* hash function */

typedef S16 (* CmHashFunc) ARGS((CmHashListCp *hashListCp, U8 *key, 
                                  U16 keyLen, U16 *idx));

/* list entry */
typedef struct cmListEnt CmListEnt;      /* list entry */

struct cmListEnt                         /* list entry */
{
   CmListEnt *next;                /* next entry in list */
   CmListEnt *prev;                /* prev entry in list */
};

#ifdef CM_MT_HASH_BIN
typedef struct cmListBinEnt CmListBinEnt;/* Bin entry */

struct cmListBinEnt                /* Bin entry */
{
   CmListEnt *next;                /* next entry in list */
   CmListEnt *prev;                /* prev entry in list */
   U16 nmbEnt;                     /* current number of entries */
};
#endif


/* hash list entry */
   
typedef struct cmHashListEnt       /* hash list entry */
{
   CmListEnt list;                 /* list pointers */
   U8  *key;                       /* pointer to key */
   U16 keyLen;                     /* length of key */
   U16 hashVal;                    /* computed hash value */
} CmHashListEnt;

/* hash list control point */

struct cmHashListCp        /*  hash list control point */
{
#ifndef CM_MT_HASH_BIN
   CmListEnt *hl;                  /* pointer to hash list bins */
#else
   CmListBinEnt *hl;               /* pointer to hash list bins */
#endif
   Region region;                  /* memory region to allocate bins */
   Pool pool;                      /* memory pool to allocate bins */
   U16 nmbBins;                    /* number of hash list bins */
   U16 binBitMask;                 /* number of bits if nmbBins is power of 2 */
   U8  nmbBinBits;                 /* number of bits to represent nmbBins */
#ifndef CM_MT_HASH_BIN
   U16 nmbEnt;                     /* current number of entries */
#endif
   U16 offset;                     /* offset of CmHashListEnt in entries */
   Bool dupFlg;                    /* allow duplicate keys */
   U16  keyType;                   /* key type for selecting hash functions */
   CmHashFunc hashFunc;            /* hash function for this key type */
};


/* functions prototypes */

EXTERN S16 cmHashListInit ARGS((
              CmHashListCp *hashListCp,  /* hash list to initialize */
              U16          nmbBins,      /* number of hash list bins */
              U16          offset,       /* offset of CmHashListEnt in entries */
              Bool         dupFlg,       /* allow duplicate keys */
              U16          keyType,      /* key type for selecting hash fn */
              Region       region,       /* memory region to allocate bins */
              Pool         pool));       /* memory pool to allocate bins */

EXTERN S16 cmHashListDeinit ARGS((
              CmHashListCp *hashListCp));/* hash list to initialize */

EXTERN S16 cmHashListInsert ARGS((
              CmHashListCp *hashListCp,  /* hash list to add to */
              PTR          entry,        /* entry to add */
              U8           *key,         /* pointer to key */
              U16          keyLen));     /* length of key */

EXTERN S16 cmHashListDelete ARGS((
              CmHashListCp *hashListCp,  /* hash list to delete from */
              PTR          entry));      /* entry to delete */

EXTERN S16 cmHashListFind ARGS((
              CmHashListCp *hashListCp,  /* hash list to search */
              U8           *key,         /* pointer to key */
              U16          keyLen,       /* length of key */
              U16          seqNmb,       /* used in case of duplicate keys */
              PTR          *entry));     /* entry to be returned */

EXTERN S16 cmHashListGetNext ARGS((
              CmHashListCp *hashListCp,    /* hash list to get from */
              PTR          prevEnt,        /* previous entry */
              PTR          *entry));       /* entry to be returned */

#ifdef CM_MT_HASH_BIN
EXTERN S16 cmHashListBinGetNextEntry ARGS((
              CmHashListCp *hashListCp,    /* hash list to get from */
              U16          binIdx,         /* Index of the bin */
              PTR          prevEnt,        /* previous entry */
              PTR          *entry));       /* entry to be returned */
#endif

/* This function is obsoleted! Use macros defined in cm_hash.h instead
 */
EXTERN S16 cmHashListQuery ARGS((
              CmHashListCp *hashListCp,    /* hash list to query */
              U8           queryType,      /* type of query */
              U16          *result));      /* result of query */

/* Hash list with open addressing
 */
EXTERN S16 cmHashListOAInsert ARGS((
              CmHashListCp *hashListCp,  /* hash list to add to */
              PTR          entry,        /* entry to add */
              U8           *key,         /* pointer to key */
              U16          keyLen));     /* length of key */

#ifdef __cplusplus
}
#endif

#endif /* __CMHASHX__ */

  
/********************************************************************30**
  
         End of file:     cm_hash.x@@/main/13 - Thu Jul 10 17:27:41 2008
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************80**

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
1.1          ---  rg    1. initial release.

1.2          ---  bw    1. add one more field for hash value in struct
                           CmHashListEnt.
             ---  bw    2. change copyright header

1.3          ---  ak    1. added prototype for cmHashListOAInsert
*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
1.4          ---      ak   1. added comment to indicate obsolescence of
                             cmHashListQuery
             ---      rg   2. added binBitMask and hashFunc to CmHashListCp.
             ---      rg   3. added prototype for CmHashFunc.
             ---      kvm  4. added nmbBinBits to CmHashListCp.
             ---      kvm  5. added forward definition of CmHashListCp.

1.5          ---      mk   6. corrected structure name "cmHashListEnt" 
                             from "CmHashListEnt". 
1.6          ---      bbk  1. Changed copyright header date.

1.7          ---      ash  1. Added support for c++ compiler

1.8          ---      tej  1. Changed copyright header date.
1.9          ---      bbk  1. Removed the use of variable index as 
                              "index" is a VxWorks keyword
/main/11     ---      cvp  1. Changed the copyright header date.
/main/12     ---      rbabu 1. Updated for NBAP software release 1.2
/main/13     ---      st   1. MT-Safe implementation under 
*********************************************************************91*/
