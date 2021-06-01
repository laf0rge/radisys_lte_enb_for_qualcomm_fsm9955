/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
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

     Name:     System Services -- identification file

     Type:     C source file

     Desc:     Version information

     File:     ss_id.c

     Sid:      ss_id.c@@/main/3 - Mon Nov 17 15:54:17 2008

     Prg:      kp

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

#define SSSWMV 3              /* main version */
#define SSSWMR 1              /* main revision */
#define SSSWBV 0              /* branch version */
/* ss017.301 : Modified for patch 17 */
#define SSSWBR 17            /* branch revision */
#define SSSWPN "1078001"      /* part number */


/* public variable declarations */


/* copyright banner */

CONSTANT PUBLIC Txt ssBan1[] ={"(c) COPYRIGHT 1989-1998, Trillium Digital Systems, Inc."};
CONSTANT PUBLIC Txt ssBan2[] ={"                 All rights reserved."};

/* system id */

/* ss041.103 */
PRIVATE CONSTANT SystemId ssSId =  
{
   SSSWMV,                    /* main version */
   SSSWMR,                    /* main revision */
   SSSWBV,                    /* branch version */
   SSSWBR,                    /* branch revision */
   SSSWPN,                    /* part number */
};

/* ss041.103 */
#ifdef SS_SID_CHANGE
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
*       File:  ss_id.c
*
*/

#ifdef ANSI
PUBLIC S16 SGetInDepSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC S16 SGetInDepSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(ssGetSId)
   s->mVer = ssSId.mVer;
   s->mRev = ssSId.mRev;
   s->bVer = ssSId.bVer;
   s->bRev = ssSId.bRev;
   s->ptNmb = ssSId.ptNmb;
   RETVALUE(TRUE);
} /* end of SGetInDepSId */
#endif /* SS_SID_CHANGE */

/* ss038.103 */

/********************************************************************30**

         End of file:     ss_id.c@@/main/3 - Mon Nov 17 15:54:17 2008

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
/main/3      ---      ch   1. initial release
/main/3+  ss001.301 schandra1 1. initial release
/main/3+  ss002.301 pradeep Updated brach revision 
/main/3+  ss003.301 mgupta Updated brach revision 
/main/3+  ss004.301 pmacharla Updated brach revision 
/main/3+  ss005.301 pmacharla Updated brach revision 
/main/3+  ss006.301 pmacharla Updated brach revision 
/main/3+  ss007.301 pmacharla Updated brach revision 
/main/3+  ss008.301 srinivas  Updated brach revision 
/main/3+  ss009.301 ssahniX   Updated brach revision 
/main/3+  ss010.301 rk        1. Updated brach revision 
/main/3+  ss011.301 ssahniX   Updated brach revision 
/main/3+  ss012.301 ssahniX   Updated brach revision 
/main/3+  ss013.301 mkumar    Updated brach revision 
/main/3+  ss014.301 ssahniX   Updated branch revision 
/main/3+  ss015.301 akapoor   Updated branch revision 
/main/3+  ss016.301 akapoor   Updated branch revision 
/main/3+  ss017.301 akapoor   Updated branch revision 
*********************************************************************91*/
