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

     Name:     MTSS
  
     Type:     C source file
  
     Desc:     Version information
 
     File:     mt_id.c

     Sid:      mt_id.c@@/main/3 - Mon Nov 17 15:54:11 2008
  
     Prg:      ada

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
  
#define MTSWMV 3              /* MTSS - main version */
#define MTSWMR 1              /* MTSS - main revision */
#define MTSWBV 0              /* MTSS - branch version */
/* mt015.301 - modified */
#define MTSWBR 15            /* MTSS - branch revision*/
#define MTSWPN "1078006"      /* MTSS - part number */


/* public variable declarations */


/* copyright banner */
  
CONSTANT PUBLIC Txt mtBan1[] ={"(c) COPYRIGHT 1989-1998, Trillium Digital Systems, Inc."};
CONSTANT PUBLIC Txt mtBan2[] ={"                 All rights reserved."};


/* system id */

/* mt040.201 */
/* mt003.301 : Compilation warning */
/*mt013.301 : fix for Compilation warning due to type casting mismatch*/
PRIVATE CONSTANT SystemId mtSId ={
   MTSWMV,                    /* main version */
   MTSWMR,                    /* main revision */
   MTSWBV,                    /* branch version */
   MTSWBR,                    /* branch revision */
  (Txt *)MTSWPN,             /* part number */
};

/* mt040.201 */
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
*       File:  mt_id.c
*
*/

#ifdef ANSI
PUBLIC S16 SGetDepSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC S16 SGetDepSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(SGetDepSId)
   s->mVer = mtSId.mVer;
   s->mRev = mtSId.mRev;
   s->bVer = mtSId.bVer;
   s->bRev = mtSId.bRev;
   s->ptNmb = mtSId.ptNmb;
   RETVALUE(TRUE);
} /* end of mtGetSid */
#endif /* SS_SID_CHANGE */
/* mt034.201 */

/********************************************************************30**

         End of file:     mt_id.c@@/main/3 - Mon Nov 17 15:54:11 2008

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
1.1          ---  ada   1. initial release

*********************************************************************81*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.2          ---      kp   1. MTSS-Solaris release 2.1
1.2+      mt018.201  bdu   1. change the revision number.
          mt019.201  bdu   2. change the revision number.
          mt020.201  bjp   3. change the revision number.
          mt021.201  bjp   4. change the revision number.
          mt022.201  bjp   5. change the revision number.
          mt023.201  sdg   6. change the revision number.
          mt024.201  fa    7. change the revision number.
          mt025.201  bjp   8. change the revision number.
          mt026.201  bjp   1. change the revision number.
          mt027.201  bjp   1. change the revision number.
          mt028.201  kkj   1. change the revision number.
          mt029.201  bn    12. change the revision number.
          mt030.201  bn    13. change the revision number.
          mt031.201  bn    14. change the revision number.
          mt032.201  bn    15. change the revision number.
          mt033.201  vk    1. change the revision number.           
          mt034.201  gs    1. Revision number change
          mt035.201  ss    1. Revision number change
          mt036.201  ss    1. Revision number change
          mt037.201  ss    1. Revision number change
          mt038.201  bn    1. Revision number change
          mt039.201  bn    1. Revision number change
/main/3      ---       rp    1. Revision number change.
/main/3+  mt001.301 schandra1 1. Revision number change.
/main/3+  mt002.301 schandra1 1. Revision number change.
/main/3+  mt003.301 pmacharla 1. Revision number change.
/main/3+  mt004.301 mgupta 1. Revision number change.
/main/3+  mt005.301 pmacharla 1. Revision number change.
/main/3+  mt006.301 ajha      1. Adding the initialization of tmrActvFnMt 
/main/3+  mt007.301 hsingh    1. Fix related to file based mem config on 64 bit machine
/main/3+  mt008.301 pmacharla 1.Thread cleanup on SDeInit 
/main/3+  mt009.301 ssahniX   1. Revision number change 
/main/3+  mt010.301 ssahniX   1. Revision number change 
/main/3+  mt011.301 rk        1. Revision number change 
/main/3+  mt012.301 ssahniX   1. Revision number change 
/main/3+  mt013.301 mkumar    1. Revision number change 
/main/3+  mt014.301 ssahniX    1. Revision number change 
/main/3+  mt015.301 akapoor    1. Revision number change 
*********************************************************************91*/
