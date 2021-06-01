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
  
     Name:     LTE RRC Hash Module 
 
     Type:     C source file
  
     Desc:     Contains Wrapper Functions for Existing Hash Functions.

     File:     nh_rhm.c
  
     Sid:      ni_rhm.c@@/main/3 - Fri Jul  1 01:13:22 2011

     Prg:      Reena
  
**********************************************************************/

/** @defgroup rhm_group Hash function Module
 *  @{
 */

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=95;
#include "envopt.h"         /* Environment options */
#include "envdep.h"         /* Environment dependent */
#include "envind.h"         /* Environment independent */
#include "gen.h"            /* General layer                */
#include "ssi.h"            /* System services              */
#include "cm5.h"            /* Common timer module          */
#include "cm_hash.h"        /* Common hashlist             */
#include "cm_tkns.h"        /* Common tokens                */
#include "cm_inet.h"        /* Inet header file             */
#include "cm_mblk.h"        /* Common memory allocation     */
#include "cm_llist.h"       /* Common link list */
#include "cm_lte.h"         /* Common LTE structure */
#include "ckw.h"            /* C-RLC interface */
#include "kwu.h"            /* D-RLC interface */
#include "crg.h"            /* C-MAC interface */
#include "cpj.h"            /* C-PDCP interface */
#include "pju.h"            /* D-PDCP interface */
#include "nhu.h"           /* RRC Upper Interface */
#include "nh.h"
#include "lnh.h"
/* external headers */

/* header/extern include files (.x) */
#include "gen.x"            /* General layer                */
#include "ssi.x"            /* System services              */
#include "cm5.x"            /* Common timer module          */
#include "cm_lib.x"         /* Common  library               */
#include "cm_hash.x"        /* Common hashlist             */
#include "cm_tkns.x"        /* Common tokens                */
#include "cm_mblk.x"        /* Common memory allocation     */
#include "cm_inet.x"        /* Inet header file             */
#include "cm_llist.x"       /* Common link list */
#include "cm_lte.x"         /* Common LTE structure */
#include "ckw.x"            /* C-RLC interface */
#include "kwu.x"            /* D-RLC interface */
#include "crg.x"            /* C-MAC interface */
#include "cpj.x"            /* C-PDCP interface */
#include "pju.x"            /* D-PDCP interface */
#include "nhu.x"           /* RRC Upper Interface */
#include "lnh.x"
#include "nh.x"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 *
 * @brief
 * 
 * @details
 * 
 * This function initializes the hash list.
 * 
 * @param[in] hashListCp     Hash list to be initialized
 * @param[in] nmbBins        Number of hash list bins
 * @param[in] offset         Offset of CmHashListEnt in entries 
 * @param[in] dupFlg         Allow duplicate keys 
 * @param[in] keyType        Key type for selecting hash fn  
 * @param[in] region         Memory region to allocate bins  
 * @param[in] pool           Memory pool to allocate bins 

 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhHashListInit
(
CmHashListCp *hashListCp,  
U16          nmbBins,      
U16          offset,      
Bool         dupFlg,     
U16          keyType,   
Region       region,   
Pool         pool     
)
#else /* ANSI */
PUBLIC S16 nhHashListInit(hashListCp, nmbBins, offset, dupFlg, keyType, 
                          region, pool)
CmHashListCp *hashListCp;
U16          nmbBins;   
U16          offset;   
Bool         dupFlg;  
U16          keyType;
Region       region;
Pool         pool; 
#endif /* ANSI */
{
   NhUstaDgn dgn;          /* diagnostics structure */
   S16       ret;
   
   /* Debug Trace */
   TRC2(nhHashListInit);
   
   /* initialize the hash list */
   ret = cmHashListInit(hashListCp, nmbBins, offset, dupFlg, keyType, 
                        region, pool);
   if(RFAILED == ret)
   {  
      RLOG0(L_ERROR, "cmHashListInit failure.");
      /* Raise Alarm */                
      dgn.type = LNH_USTA_DGNVAL_NONE;
      nhSendLmAlarm(LCM_CATEGORY_RESOURCE,LNH_EVENT_OPERATION_FAIL,
                    LCM_CAUSE_HASH_FAIL,&dgn);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* End of nhHashListInit */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function inserts an element into the hash list.
 * 
 * @param[in] hashListCp       Hash list to add to
 * @param[in] hashListEnt      Entry to add to hash list
 * @param[in] key              Key
 * @param[in] keyLen           Length of the key
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhHashListInsert
(
CmHashListCp *hashListCp,   
PTR          hashListEnt,  
U8           *key,        
U16          keyLen      
)
#else /* ANSI */
PUBLIC S16 nhHashListInsert (hashListCp, hashListEnt, key, keyLen)
CmHashListCp *hashListCp;
PTR          hashListEnt;
U8           *key;      
U16          keyLen;   
#endif /* ANSI */
{
   NhUstaDgn dgn;              /* diagnostics structure */
   S16       ret;

   /* Debug Trace */
   TRC2(nhHashListInsert);

   /* insert into the hash list */
   /* cmHashListInsert may return RFAILED or ROKDUP for failure */
   ret = cmHashListInsert(hashListCp,hashListEnt,key, keyLen);
   if(ROK != ret)
   {
      /* Log Error */
      RLOG0(L_ERROR, "cmHashListInsert failure.");
      /* Raise Alarm */
      dgn.type = LNH_USTA_DGNVAL_NONE;
      nhSendLmAlarm(LCM_CATEGORY_RESOURCE,LNH_EVENT_OPERATION_FAIL,
                    LCM_CAUSE_HASH_FAIL,&dgn);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* End of nhHashListInsert */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function removes specified element from the hash list.
 * 
 * @param[in] hashListCp       Hashlist to delete the entry 
 * @param[in] hashListEnt      Entry to delete
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhHashListDelete
(
CmHashListCp *hashListCp,  
PTR          hashListEnt  
)
#else /* ANSI */
PUBLIC S16 nhHashListDelete(hashListCp, hashListEnt)
CmHashListCp *hashListCp;
PTR           hashListEnt;
#endif /* ANSI */
{
   NhUstaDgn dgn;                /* diagnostics structure */
   S16       ret;

   /* Debug Trace */
   TRC2(nhHashListDelete);

   /* delete from the hash list */
   ret = cmHashListDelete(hashListCp,hashListEnt);
   if (RFAILED == ret)
   {
       /* Log Error */
      RLOG0(L_ERROR, "cmHashListDelete failure.");
      /* Raise Alarm */
      dgn.type = LNH_USTA_DGNVAL_NONE;
      nhSendLmAlarm(LCM_CATEGORY_RESOURCE, LNH_EVENT_OPERATION_FAIL,
                    LCM_CAUSE_HASH_FAIL, &dgn);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* End of nhHashListDelete */

/**
 *
 * @brief
 * 
 * @details
 * 
 * This function searches for particular element in hash list.
 * 
 * @param[in] hashListCp       Hash list to search the entry
 * @param[in] key              Pointer to the key
 * @param[in] keyLen           Length of the key
 * @param[in] data             Pointer to be returned
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhHashListFind
(
CmHashListCp *hashListCp,  
U8*          key,         
U16          keyLen,     
PTR          *data      
)
#else /* ANSI */
PUBLIC S16 nhHashListFind(hashListCp, key, keyLen, data)
CmHashListCp *hashListCp;
U8*          key;       
U16          keyLen;   
PTR          *data;   
#endif /* ANSI */
{
   /* Debug Trace */
   TRC2(nhHashListFind);

   RETVALUE(cmHashListFind(hashListCp, key, keyLen, 0, data));
} /* End of nhHashListFind */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function Gets next entry in the hash list with respect to specified previous entry.
 * @param[in] prevEnt        Previous entry
 * @param[in] entry          Entry to be returned
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 nhHashListGetNext
(
CmHashListCp *hashListCp,  
PTR          prevEnt,     
PTR          *entry      
)
#else /* ANSI */
PUBLIC S16 nhHashListGetNext(hashListCp, prevEnt, entry)
CmHashListCp *hashListCp;
PTR          prevEnt;   
PTR          *entry;   
#endif /* ANSI */
{
   /* Debug Trace */
   TRC2(nhHashListGetNext);

   /* cmHashListGetNext may return RFAILED or ROKDNA for failure */
   RETVALUE(cmHashListGetNext(hashListCp, prevEnt, entry));
} /* End of nhHashListGetNext */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function deinitializes hash list.
 * 
 * @param[in] hashListCp     Hash list to deinitialize
 * 
 * @return Void
 *  -# RETVOID
 **/
#ifdef ANSI
PUBLIC Void nhHashListDeInit
(
CmHashListCp *hashListCp  
)
#else /* ANSI */
PUBLIC Void nhHashListDeInit(hashListCp)
CmHashListCp *hashListCp;
#endif /* ANSI */
{
   NhUstaDgn dgn;         /* diagnostics structure */
   S16       ret;

   /* Debug Trace */
   TRC2(nhHashListDeInit)

   /* deinitialize */
   ret = cmHashListDeinit(hashListCp);
   if (RFAILED == ret)
   {
      /* Log Error */
      RLOG0(L_ERROR, "cmHashListDeinit failure.");
      /* Raise Alarm */
      dgn.type = LNH_USTA_DGNVAL_NONE;
      nhSendLmAlarm(LCM_CATEGORY_RESOURCE,LNH_EVENT_OPERATION_FAIL,
                    LCM_CAUSE_HASH_FAIL,&dgn);
   }
   RETVOID;
}/* End of nhHashListDeInit */

/** @} */
/**********************************************************************
  
         End of file:     ni_rhm.c@@/main/3 - Fri Jul  1 01:13:22 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/
