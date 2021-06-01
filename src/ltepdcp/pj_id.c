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
  
        Name:     <name for the file for readability>
    
        Type:     <File type - C include file, C source file, make file>
  
        Desc:     <description of file contents>
 
        File:     kw_id.c

        Sid:      pj_id.c@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:48 2015
  
        Prg:      gk
  
*********************************************************************21*/
 

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *    
 */

  
/* header include files (.h) */
  
#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */

/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */

  
/* local defines */

#define KWSWMV 2             /* rlc - main version */
#define KWSWMR 1             /* rlc - main revision */
#define KWSWBV 0             /* rlc - branch version */
#define KWSWBR 5             /* rlc -  kw005.201 patch level */
#define KWSWPN "1000371"     /* rlc - part number */

/* local typedefs */

/* local externs */
  
/* forward references */

/* public variable declarations */

#ifdef __cplusplus
EXTERN "C" {
#endif /* CPLUSPLUS */

EXTERN S16 pjGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* CPLUSPLUS */

/* copyright banner */

PUBLIC CONSTANT Txt pjBan1[] =
   {"(c) COPYRIGHT 1989-2002, Trillium Digital Systems, Inc."};

PUBLIC CONSTANT Txt pjBan2[] =
   {"                 All rights reserved."};

/* system id */

PRIVATE CONSTANT SystemId sId ={
   KWSWMV,              /* rlc - main version */
   KWSWMR,              /* rlc - main revision */
   KWSWBV,              /* rlc - branch version */
   KWSWBR,              /* rlc - branch revision */
   KWSWPN               /* rlc - part number */
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
*       File:  kw_id.c
*
*/

#ifdef ANSI
PUBLIC S16 pjGetSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC S16 pjGetSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(pjGetSId);

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   s->ptNmb = sId.ptNmb;

   RETVALUE(TRUE);

} /* end of pjGetSId */

  
/********************************************************************30**
  
         End of file:     pj_id.c@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:48 2015
  
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
/main/1      ---         gk   1. initial release.
/main/2      ---         nm   1. LTERLC Release 2.1
/main/3      kw001.201   vp   1. Patch  for HENB, changes for max RBs
                               in cell and avoidance of float variables.
/main/4      kw002.201   vp   1. Patch for optimization,fixes for multi-region
                                 and retransmission.
/main/5      kw003.201 vp  1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/6      kw004.201   ap 1. Added traces.
                            2. Modified the trace Indication function
/main/7      kw005.201   ap 1. Support for R9 in RLC and PDCP
                            2. Support for L2 Measurement in RLC and PDCP
                            3. Fixes for internal CR.
                            4. Fixes for Customer raised defects.
*********************************************************************91*/
