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



/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code to provide software part number, version and
               revision. 
  
     File:     rg_id.c 
  
     Sid:      rg_id.c@@/main/3 - Sat Jul 30 02:22:00 2011
  
     Prg:      sm
  
**********************************************************************/

/** @file rg_id.c
@brief This file contains the definition of this software's part number, 
       version and revision. Also provides an API to access the same.
*/

/* header include files (.h) */
#include "envopt.h"           /* environment options */  
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "lrg.h"              /* layer manager */
   
/* header/extern include files (.x) */
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm_lib.x"           /* common library */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
EXTERN Void rgGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* defines */
#define RGSWMV 3             /* LTE MAC- main version */
#define RGSWMR 1             /* LTE MAC - main revision */
#define RGSWBV 0             /* LTE MAC - branch version */
/* Updated the branch revision Number */
#define RGSWBR 2             /* LTE MAC - branch revision */
#define RGSWPN "1000372"     /* LTE MAC - part number */

/* system id */
PRIVATE CONSTANT SystemId sId ={
   RGSWMV,              /* LTE MAC - main version */
   RGSWMR,              /* LTE MAC - main revision */
   RGSWBV,              /* LTE MAC - branch version */
   RGSWBR,              /* LTE MAC - branch revision */
   (Txt *)RGSWPN               /* LTE MAC - part number */
};


/**
 * @brief Retrieve system id. 
 *
 * @details
 *
 *     Function : rgGetSId 
 *    
 *     Get system id consisting of part number, main version and
 *     revision and branch version and branch.
 *     
 *     
 *  @param[out] SystemId *s  
 *  @return  void
 **/
/*
*
*       Fun:   get system id
*
*       Desc:  Get system id consisting of part number, main version and
*              revision and branch version and branch.
*
*       Ret:   TRUE      - ok
*
*       Notes: None
*
*       File:  rg_id.c
*
*/
 
#ifdef ANSI
PUBLIC Void rgGetSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC Void rgGetSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(rgGetSId);

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   cmMemcpy((U8 *)s->ptNmb, (U8 *)sId.ptNmb, LRG_MAX_PT_NUM_SIZE); 

   /* Stack Crash Problem for TRACE5 Changes. Added the return below */
  RETVOID; 
  
} /* end of rgGetSid */



/**********************************************************************
 
         End of file:     rg_id.c@@/main/3 - Sat Jul 30 02:22:00 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       sm                1. Initial Release.
/main/1+     rg001.201 sd                1. Patch 1 for LTE MAC 2.1 
/main/1+     rg002.201 sm                1. Patch 2 for LTE MAC 2.1 
/main/1+     rg003.201 pk                1. Patch 3 for LTE MAC 2.1 
/main/1+     rg004.201 sd                1. Patch 4 for LTE MAC 2.1
/main/1+     rg005.201 sm                1. Patch 5 for LTE MAC 2.1
/main/1+     rg006.201 sm                1. Patch 6 for LTE MAC 2.1
/main/1+     rg007.201 gjahagirdar       1. Patch 7 for LTE MAC 2.1
/main/1+     rg008.201 rsharon           1. Patch 8 for LTE MAC 2.1
/main/1+     rg009.201 dv                1. Patch 9 for LTE MAC 2.1
/main/1+     rg010.201 av                1. Patch 10 for LTE MAC 2.1
/main/3      ---       gvj               1. LTE MAC 3.1 Release
/main/3      rg001.301 nudupi            1. Patch 1 for LTE MAC 3.1. 
/main/3      rg002.301 asehgal           1. Patch 2 for LTE MAC 3.1. 
$SID$        ---       rt                1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
