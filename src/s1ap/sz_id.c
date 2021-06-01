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
  
     Name:     s1ap - id
  
     Type:     C source file
  
     Desc:     Structure for software part number, version and
               revision.

     File:     sq_id.c
  
     Sid:      sq_id.c@@/main/4 - Mon Jan 10 22:16:58 2011
 
     Prg:      sn
  
*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */

#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */

/* header/extern include files (.x) */

#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */

/* defines */

#define SZSWMV 3             /* s1ap - main version */
#define SZSWMR 1             /* s1ap - main revision */
#define SZSWBV 0             /* s1ap - branch version */
#define SZSWBR 8             /* s1ap - sz008.301 */
#define SZSWPN "1000361"     /* s1ap - part number */

/* public variable declarations */

#ifdef __cplusplus
extern "C" {
#endif

EXTERN S16 szGetSId ARGS((SystemId *s));

#ifdef __cplusplus
}
#endif

/* copyright banner */

PUBLIC CONSTANT Txt szBan1[] =
   {"(c) COPYRIGHT 1989-2002, Trillium Digital Systems, Inc."};

PUBLIC CONSTANT Txt szBan2[] =
   {"                 All rights reserved."};

/* system id */

PRIVATE CONSTANT SystemId sId ={
   SZSWMV,              /* s1ap - main version */
   SZSWMR,              /* s1ap - main revision */
   SZSWBV,              /* s1ap - branch version */
   SZSWBR,              /* s1ap - branch revision */
   SZSWPN               /* s1ap - part number */
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
*       File:  sq_id.c
*
*/

#ifdef ANSI
PUBLIC S16 szGetSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC S16 szGetSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(szGetSId);

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   s->ptNmb = sId.ptNmb;

   RETVALUE(TRUE);

} /* end of szGetSid */

/*********************************************************************
  
         End of file:     sq_id.c@@/main/4 - Mon Jan 10 22:16:58 2011
 
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
/main/4      ----      pkaX  Updated for release 3.1.
/main/4     sz001.301  pkaX  1. Updated the S1AP specification release 9.6.0.
                             2. Fix for compilation warning when LSZV1 disabled.
/main/4     sz002.301  pkaX  1. Updated the destinaion port and suConnId value
                                in the trace indication.
                             2. Handled the unexpected event and audit statistics
                                correctly.
                             3. Added the new macro to update the PDU statistics.
                                The PDU statistics will be incremented once the PDU
                                is received.
                             4. Removed the unused members in the structures.
/main/4    sz003.301   pka   1. Updated for PSF-S1AP 1.1 release.                             
/main/4    sz004.301   va    1. Updated for Transparent Container Dec/Enc                     
/main/4    sz005.301   va    1. Updated for Sz005.301 patch                      
/main/4    sz006.301   ve    1. Updated for sz006.301 patch             
/main/4    sz007.301   ve    1. Updated for sz007.301 patch             
/main/4    sz008.301   akaranth 1. Updated for sz008.301 patch             
*********************************************************************91*/
