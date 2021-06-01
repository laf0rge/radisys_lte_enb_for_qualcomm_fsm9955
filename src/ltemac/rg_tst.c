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
  
     Name:     LTE-MAC - test
  
     Type:     C source file
  
     Desc:     Sample C source code used to link LTE-MAC in a
               TRILLIUM development environment.

     File:     rg_tst.c
  
     Sid:      gk_tst.c@@/main/2 - Sat Jul 30 02:21:56 2011
  
     Prg:      st
  
**********************************************************************/
  
  
/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#ifdef RG_PT_BUILD
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#ifdef DOS                 /* dos environment */
#include <process.h>       /* dos - process */  
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXTERN  int main ARGS((int argc,char **argv ));
EXTERN  void exit ARGS((int status ));
  
#ifdef __cplusplus
}
#endif
 
 
/*
 *
 *       Fun:   Main
 *
 *       Desc:  Program execution starts here.
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  rg_tst.c
 *
 */
  
#ifdef ANSI
int main
(
int argc,                   /* argument count */
char **argv                 /* argument values */
)
#else
int main(argc, argv)
int argc;                   /* argument count */
char **argv;                /* argument values */
#endif
{
   TRC1(main)

   UNUSED(argc);
   UNUSED(argv);

   exit(0);
  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVALUE(OK);
} /* end of main */
#endif /* RG_PT_BUILD */

  
/**********************************************************************
  
         End of file:     gk_tst.c@@/main/2 - Sat Jul 30 02:21:56 2011
  
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
/main/1      ---     st   1. LTE-MAC Initial Release.
/main/1      ---     st   1. LTE MAC 2.1 release
/main/2   rg008.201  rsharon   1.Fixed TRACE5 issue.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---     rt   1. LTE MAC 4.1 release
*********************************************************************91*/
