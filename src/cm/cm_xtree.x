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
  
     Name:    Common Multiway Radix Tree
  
     Type:    C include file 
  
     Desc:    Structures, variables and typedefs required by radix tree
 
     File:    cm_xtree.x

     Sid:      cm_xtree.x@@/main/4 - Tue May 20 15:52:28 2003
  
     Prg:     na
  
*********************************************************************21*/


#ifndef __CMXTREEX__
#define __CMXTREEX__

#ifdef __cplusplus
extern "C" {
#endif


/* typedefs */

/* cm_xtree_x_001.main_3 : Changed CmRdx to cmXRdx throughout the file */

/* forward definitions */
#ifdef CM_XTREE
typedef struct cmXRdxListEnt CmXRdxListEnt;
typedef struct cmXRdxTreeNodeEnt CmXRdxTreeNodeEnt;
typedef struct cmXRdxTreeNode CmXRdxTreeNode;

typedef struct cmXRdxTreeEnt CmXRdxTreeEnt;

/* List pointer */
struct cmXRdxListEnt
{
   PTR prev;
   PTR next;
};


/* Radix tree node */
struct cmXRdxTreeNode
{
   CmXRdxTreeNodeEnt **entries;
   CmXRdxTreeNode    *parent;
   U16              treeLevel;
};

/* Radix tree entry  */
struct cmXRdxTreeEnt
{
   /* List.prev/next points to CmXRdxTreeEnt structure */
   CmXRdxListEnt  list;    
   U8            *key;
   U16           len;
   U8            wildCard;
   /* searchResult.prev/next points to user prot. data record */
   CmXRdxListEnt  searchResult;
};

/* Radix tree node entry */
struct cmXRdxTreeNodeEnt
{
   CmXRdxListEnt  treeData;
   CmXRdxTreeNode *nextNode;
};

/* Radix tree control point */
typedef struct cmXRdxTreeCp
{
   CmXRdxTreeNode *root;
   Region        region;
   Pool          pool;
   U16           entriesPerNode;
   Bool          reverseKeys;
   U16           offset;
/* 001.main_2 - Added to maintain memory used in radix tree */
   U32           memUsed;
} CmXRdxTreeCp;


/* functions prototypes */

/* Initialize radix tree */
EXTERN S16 cmXRdxTreeInit ARGS((
              CmXRdxTreeCp *cp, 
              Region region, 
              Pool pool, 
              U16 entriesPerNode,
              U16 offset,
              Bool reverseKeys));

/* Deinitialize radix tree */
EXTERN S16 cmXRdxTreeDeinit ARGS((
              CmXRdxTreeCp *cp));

/* Initialize new entry into radix tree */
EXTERN S16 cmXRdxTreeInsert ARGS((
              CmXRdxTreeCp *cp,
              PTR entry,
              U8 *key,
              U16 keyLength,
              U8 wildCard));

/* Find best match in radix tree */
EXTERN S16 cmXRdxTreeFind ARGS((
              CmXRdxTreeCp *cp,
              U8 *key,
              U16 keyLength,
              PTR *entry));

/* Find all matching entries in radix tree */
EXTERN S16 cmXRdxTreeFindAll ARGS((
              CmXRdxTreeCp *cp,
              U8 *key,
              U16 keyLength,
              PTR *entry,
              U16 *count));

/* Delete entry from radix tree (specify prot. data rec) */
EXTERN S16 cmXRdxTreeDeletePtr ARGS((
              CmXRdxTreeCp *cp,
              PTR entry));

/* Delete entry from radix tree (specify key) */
EXTERN S16 cmXRdxTreeDeleteKey ARGS((
              CmXRdxTreeCp *cp,
              U8 *key,
              U16 keyLength,
              Bool wildCard));



#define CM_XTREE_BINSIZE (sizeof(CmXRdxTreeEnt))
#else /* CM_XTREE */
typedef struct cmRdxListEnt CmRdxListEnt;
typedef struct cmRdxTreeNodeEnt CmRdxTreeNodeEnt;
typedef struct cmRdxTreeNode CmRdxTreeNode;

typedef struct cmRdxTreeEnt CmRdxTreeEnt;

/* List pointer */
struct cmRdxListEnt
{
   PTR prev;
   PTR next;
};


/* Radix tree node */
struct cmRdxTreeNode
{
   CmRdxTreeNodeEnt **entries;
   CmRdxTreeNode    *parent;
   U16              treeLevel;
};

/* Radix tree entry  */
struct cmRdxTreeEnt
{
   /* List.prev/next points to CmRdxTreeEnt structure */
   CmRdxListEnt  list;    
   U8            *key;
   U16           len;
   U8            wildCard;
   /* searchResult.prev/next points to user prot. data record */
   CmRdxListEnt  searchResult;
};

/* Radix tree node entry */
struct cmRdxTreeNodeEnt
{
   CmRdxListEnt  treeData;
   CmRdxTreeNode *nextNode;
};

/* Radix tree control point */
typedef struct cmRdxTreeCp
{
   CmRdxTreeNode *root;
   Region        region;
   Pool          pool;
   U16           entriesPerNode;
   Bool          reverseKeys;
   U16           offset;
/* 001.main_2 - Added to maintain memory used in radix tree */
   U32           memUsed;
} CmRdxTreeCp;


/* functions prototypes */

/* Initialize radix tree */
EXTERN S16 cmRdxTreeInit ARGS((
              CmRdxTreeCp *cp, 
              Region region, 
              Pool pool, 
              U16 entriesPerNode,
              U16 offset,
              Bool reverseKeys));

/* Deinitialize radix tree */
EXTERN S16 cmRdxTreeDeinit ARGS((
              CmRdxTreeCp *cp));

/* Initialize new entry into radix tree */
EXTERN S16 cmRdxTreeInsert ARGS((
              CmRdxTreeCp *cp,
              PTR entry,
              U8 *key,
              U16 keyLength,
              U8 wildCard));

/* Find best match in radix tree */
EXTERN S16 cmRdxTreeFind ARGS((
              CmRdxTreeCp *cp,
              U8 *key,
              U16 keyLength,
              PTR *entry));

/* Find all matching entries in radix tree */
EXTERN S16 cmRdxTreeFindAll ARGS((
              CmRdxTreeCp *cp,
              U8 *key,
              U16 keyLength,
              PTR *entry,
              U16 *count));

/* Delete entry from radix tree (specify prot. data rec) */
EXTERN S16 cmRdxTreeDeletePtr ARGS((
              CmRdxTreeCp *cp,
              PTR entry));

/* Delete entry from radix tree (specify key) */
EXTERN S16 cmRdxTreeDeleteKey ARGS((
              CmRdxTreeCp *cp,
              U8 *key,
              U16 keyLength,
              Bool wildCard));



#define CM_XTREE_BINSIZE (sizeof(CmRdxTreeEnt))
#endif /* CM_XTREE */

#ifdef __cplusplus
}
#endif

#endif  /* __CMXTREEX__ */


/********************************************************************30**
  
         End of file:     cm_xtree.x@@/main/4 - Tue May 20 15:52:28 2003
  
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
/main/2      ---       ms                   1. Initial release
/main/3      ---       ms                   1. Added to maintain memory used in radix tree
/main/4      ---       pg                   1. CmRdx has been changed to CmXRdx throughout the code
*********************************************************************91*/
