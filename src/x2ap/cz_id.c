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
  
     Name:     x2ap - id
  
     Type:     C source file
  
     Desc:     Structure for software part number, version and
               revision.

     File:     cz_id.c
  
     Sid:      tr_id.c@@/main/2 - Tue Aug 30 18:36:12 2011
 
     Prg:      sy
  
*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */

#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm5.h"              /* Common timer library            */
#include "lcz.h"              /* layer manager macros */

/* header/extern include files (.x) */

#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */

/* defines */

#define CZSWMV 3             /* x2ap - main version */
#define CZSWMR 1             /* x2ap - main revision */
#define CZSWBV 0             /* x2ap - branch version */
/*cz001.301:Modified GPR label*/
#define CZSWBR 1             /* x2ap - cz001.301 */
#define CZSWPN "1000373"     /* x2ap - part number */

/* public variable declarations */

#ifdef __cplusplus
extern "C" {
#endif

EXTERN S16 czGetSId ARGS((SystemId *s));

#ifdef __cplusplus
}
#endif

/* copyright banner */

PUBLIC CONSTANT Txt czBan1[] =
   {"(c) COPYRIGHT 1989-2002, Trillium Digital Systems, Inc."};

PUBLIC CONSTANT Txt czBan2[] =
   {"                 All rights reserved."};

/* system id */

PRIVATE CONSTANT SystemId sId ={
   CZSWMV,              /* x2ap - main version */
   CZSWMR,              /* x2ap - main revision */
   CZSWBV,              /* x2ap - branch version */
   CZSWBR,              /* x2ap - branch revision */
   CZSWPN               /* x2ap - part number */
};

/*
*     support functions
*/

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
*       File:  cz_id.c
*
*/

#ifdef ANSI
PUBLIC S16 czGetSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC S16 czGetSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(czGetSId);

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   cmMemcpy((U8 *)s->ptNmb, (U8 *)sId.ptNmb, LCZ_MAX_PT_NUM_SIZE);

   RETVALUE(TRUE);

} /* end of czGetSid */

/*********************************************************************
  
         End of file:     tr_id.c@@/main/2 - Tue Aug 30 18:36:12 2011
 
**********************************************************************/

/**********************************************************************
  
        Notes:
  
**********************************************************************/

/**********************************************************************

**********************************************************************/

/********************************************************************
  
        Revision history:
  
**********************************************************************/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sy             1. LTE-X2AP Initial Release.
            cz001.101   mm             1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt             1. LTE-X2AP 3.1 release.
/main/3     cz001.301  akaranth        1. Updated for cz001.301 patch.
*********************************************************************91*/
