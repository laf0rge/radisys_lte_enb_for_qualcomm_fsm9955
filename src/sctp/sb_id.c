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

     Name:    SCTP Layer

     Type:    C source file

     Desc:    product id file

     File:    sb_id.c

     Sid:      sb_id.c@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:15 2015

     Prg:     wvdl

*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */

#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "lsb.h"              /* layer management */

/* header/extern include files (.x) */

#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */

/* defines */

#define SBSWMV 1            /* SCTP - main version */
#define SBSWMR 3            /* SCTP - main revision */
#define SBSWBV 0            /* SCTP - branch version */
/* sb036.103: modification - branch revision modified from 35 to 36 */
#define SBSWBR 36            /* SCTP - branch revision */
#define SBSWPN "1000191"    /* SCTP - part number */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* forward references */

PUBLIC S16 sbGetSid ARGS((SystemId *sid));

/* public variable declarations */

/* copyright banner */

CONSTANT PUBLIC Txt sbBan1[] =
      {"(c) COPYRIGHT 1989-1999, Trillium Digital Systems, Inc."};
CONSTANT PUBLIC Txt sbBan2[] =
      {"                 All rights reserved."};

/* system id */

PRIVATE CONSTANT SystemId sId ={
   SBSWMV,                    /* main version */
   SBSWMR,                    /* main revision */
   SBSWBV,                    /* branch version */
   SBSWBR,                    /* branch revision */
   SBSWPN,                    /* part number */
};


/*
*     support functions
*/


/*
*
*       Fun:   sbGetSid
*
*       Desc:  Get system id consisting of part number, main version and
*              revision and branch version and branch.
*
*       Ret:   TRUE      - ok
*
*       Notes: None
*
*       File:  sb_id.c
*
*/
#ifdef ANSI
PUBLIC S16 sbGetSid
(
SystemId *s                 /* system id */
)
#else
PUBLIC S16 sbGetSid(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(sbGetSid)

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   s->ptNmb = sId.ptNmb;

   RETVALUE(TRUE);

} /* sbGetSid */

#ifdef __cplusplus
}
#endif /* __cplusplus */



/********************************************************************30**

         End of file:     sb_id.c@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:15 2015

*********************************************************************31*/
/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************70**

  version    initials                   description
-----------  ---------  ------------------------------------------------

*********************************************************************71*/

/********************************************************************80**

*********************************************************************81*/
/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
 /main/2     ---    wvdl    1. initial release.
 /main/2     ---     sb     1. Modified for SCTP release based on 
                               RFC-2960 'Oct 2000.
           sb001.12  sb     1. Branch revision modified from 0 to 1.
           sb004.12  sb     1. Branch revision modified from 1 to 2.
           sb005.12  ap     1. Branch revision modified from 2 to 3.
           sb006.102 rs     1. Branch revision modified from 3 to 4.
           sb007.102 ap     1. Branch revision modified from 4 to 5.
           sb008.12  sb     1. Branch revision modified from 5 to 6.
           sb009.102 rs     1. Branch revision modified from 6 to 7.
           sb010.102 rs     1. Branch revision modified from 7 to 8.
           sb011.102 rs     1. Branch revision modified from 8 to 9.
           sb012.102 rs     1. Branch revision modified from 9 to 10.
           sb013.102 rs     1. Branch revision modified from 10 to 11.
           sb014.102 ap     1. Branch revision modified from 11 to 12.
           sb015.102 rs     1. Branch revision modified from 12 to 13.
           sb016.102 ap/rs  1. Branch revision modified from 13 to 14.
           sb017.102 rs     1. Branch revision modified from 14 to 15.
           sb018.102 ab     1. Branch revision modified from 15 to 16.
           sb020.102 rs/ab  1. Branch revision modified from 16 to 17.
           sb021.102 rs     1. Branch revision modified from 17 to 18.
           sb022.102 ab     1. Branch revision modified from 18 to 19.
           sb023.102 sb     1. Branch revision modified from 19 to 20.
           sb024.102 kk     1. Branch revision modified from 20 to 21.
           sb025.102 kk     1. Branch revision modified from 21 to 22.
           sb026.102 kk     1. Branch revision modified from 22 to 23.
           sb027.102 hm     1. Branch revision modified from 23 to 24.
           sb028.102 hm     1. Branch revision modified from 24 to 25.
           sb029.102 hm     1. Branch revision modified from 25 to 26.
           sb030.102 kk     1. Branch revision modified from 26 to 27.
           sb031.102 hm     1. Branch revision modified from 27 to 28.
           sb032.102 hm     1. Branch revision modified from 28 to 29.
           sb033.102 hm     1. Branch revision modified from 29 to 30.
           sb034.102 hm     1. Branch revision modified from 30 to 31.
           sb035.102 hm     1. Branch revision modified from 34 to 35.
           sb036.102 rs     1. Branch revision modified from 35 to 36.
           sb037.102 rs     1. Branch revision modified from 36 to 37.
           sb038.102 hl     1. Branch revision modified from 37 to 38.
           sb039.102 rs     1. Branch revision modified from 38 to 39.
           sb040.102 rs     1. Branch revision modified from 39 to 40.
           sb041.102 hl     1. Branch revision modified from 40 to 41. 
           sb042.102 hl     1. Branch revision modified from 41 to 42.
           sb043.102 hl     1. Branch revision modified from 42 to 43.
           sb044.102 rs     1. Branch revision modified from 43 to 44.
           sb045.102 rs     1. Branch revision modified from 44 to 45.
           sb046.102 hl     1. Branch revision modified from 45 to 46.
           sb047.102 hl     1. Branch revision modified from 46 to 47.
           sb048.102 hl     1. Branch revision modified from 47 to 48.
           sb049.102 rs     1. Branch revision modified from 48 to 49.
           sb050.102 sg     1. Branch revision modified from 49 to 50.
           sb051.102 ag     1. Branch revision modified from 50 to 51.
           sb052.102 ag     1. Branch revision modified from 51 to 52.
           sb053.102 rk     1. Branch revision modified from 52 to 53.
           sb054.102 rk     1. Branch revision modified from 53 to 54.
           sb055.102 ag     1. Branch revision modified from 54 to 55.
           sb056.102 rk     1. Branch revision modified from 55 to 56.
           sb057.102 pr     1. Branch revision modified from 56 to 57.
           sb058.102 pr     1. Branch revision modified from 57 to 58.
           sb059.102 pr     1. Branch revision modified from 58 to 59.
           sb060.102 pr     1. Branch revision modified from 59 to 60.
           sb061.102 pr     1. Branch revision modified from 60 to 61.
           sb062.102 pr     1. Branch revision modified from 61 to 62.
           sb063.102 pr     1. Branch revision modified from 62 to 63.
           sb064.102 pr     1. Branch revision modified from 63 to 64.
           sb065.102 kp     1. Branch revision modified from 64 to 65.
           sb066.102 kp     1. Branch revision modified from 65 to 66.
           sb067.102 kp     1. Branch revision modified from 66 to 67.
           sb068.102 kp     1. Branch revision modified from 67 to 68.
           sb069.102 kp     1. Branch revision modified from 68 to 69.
           sb070.102 kp     1. Branch revision modified from 69 to 70.
           sb071.102 kp     1. Branch revision modified from 70 to 71.
           sb072.102 kp     1. Branch revision modified from 71 to 72.
           sb073.102 kp     1. Branch revision modified from 72 to 73.
           sb074.102 kp     1. Branch revision modified from 73 to 74.
           sb075.102 pc     1. Branch revision modified from 74 to 75.
           sb076.102 kp     1. Branch revision modified from 75 to 76.
           sb077.102 kp     1. Branch revision modified from 76 to 77.
           sb078.102 kp     1. Branch revision modified from 77 to 78.
           sb079.102 kp     1. Branch revision modified from 78 to 79.
           sb080.102 kp     1. Branch revision modified from 79 to 80.
           sb081.102 kp     1. Branch revision modified from 80 to 81.
           sb082.102 kp     1. Branch revision modified from 81 to 82.
           sb083.102 kp     1. Branch revision modified from 82 to 83.
           sb084.102 kp     1. Branch revision modified from 83 to 84.
           sb085.102 kp     1. Branch revision modified from 84 to 85.
           sb086.102 rsr    1. Branch revision modified from 85 to 86.
/main/3      ---     rsr/ag 1. Updated for Release of 1.3
           sb001.103 ag     1. Branch revision modified from 0 to 1.
           sb002.103 rsr    1. Branch revision modified from 1 to 2.
           sb003.103 ag     1. Branch revision modified from 2 to 3.
           sb004.103 ag     1. Branch revision modified from 3 to 4.
           sb005.103 ag     1. Branch revision modified from 4 to 5.
           sb006.103 ag     1. Branch revision modified from 5 to 6.
           sb007.103 ag     1. Branch revision modified from 6 to 7.
           sb008.103 nuX    1. Branch revision modified from 7 to 8.
           sb009.103 nuX    1. Branch revision modified from 8 to 9.
           sb010.103 nuX    1. Branch revision modified from 9 to 10.
           sb011.103 nuX    1. Branch revision modified from 10 to 11.
           sb012.103 pkaX   1. Branch revision modified from 11 to 12.
           sb013.103 pkaX   1. Branch revision modified from 12 to 13.
           sb014.103 pkaX   1. Branch revision modified from 13 to 14.
           sb015.103 pkaX   1. Branch revision modified from 14 to 15.
           sb016.103 hsingh   1. Branch revision modified from 15 to 16.
           sb017.103 pkaX   1. Branch revision modified from 16 to 17.
           sb018.103 pkaX   1. Branch revision modified from 17 to 18.
           sb019.103 krkX   1. Branch revision modified from 18 to 19.
/main/3    sb020.103 krkX   1. Branch revision modified from 19 to 20.
/main/3    sb021.103 krkX   1. Branch revision modified from 20 to 21.
/main/3    sb022.103 pkaX   1. Branch revision modified from 21 to 22.
/main/3    sb023.103 pkaX   1. Branch revision modified from 22 to 23.
/main/3    sb024.103 ajainx 1. Branch revision modified from 23 to 24.
/main/3    sb025.103 mm     1. Branch revision modified from 24 to 25.
/main/3    sb026.103 mm     1. Branch revision modified from 25 to 26.
/main/3    sb027.103 ajainx 1. Branch revision modified from 26 to 27.
/main/3    sb028.103 ajainx 1. Branch revision modified from 27 to 28.
/main/3    sb029.103 krkX.  1.Branch revision modified from 28 to 29.
/main/3    sb030.103 krkX.  1.Branch revision modified from 29 to 30.
/main/3    sb031.103 vsk .  1.Branch revision modified from 30 to 31.
/main/3    sb032.103 vsk .  1.Branch revision modified from 31 to 32.
/main/3    sb033.103 sdey   1.Branch revision modified from 32 to 33.
/main/3    sb034.103 vsk    1.Branch revision modified from 33 to 34.
/main/3    sb035.103 vsk    1.Branch revision modified from 34 to 35.
/main/3    sb036.103 mm     1.Branch revision modified from 35 to 36.
**********************************************************************/
