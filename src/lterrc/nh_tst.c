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
  
     Name:     RRC - test
  
     Type:     C source file
  
     Desc:     Sample C source code used to link RRC in a
               TRILLIUM development environment.

     File:     nh_tst.c
  
     Sid:      ni_tst.c@@/main/3 - Fri Jul  1 01:13:22 2011
  
     Prg:      st
  
**********************************************************************/
  
  
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=102;

#ifdef NH_PT_BUILD

#ifdef DOS                 /* DOS environment */
#include <process.h>       /* DOS process */  
#endif /* DOS */

/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hashlist Defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_pasn.h"       /* Common Pasn Defines */

#include "cm_lte.h"        /* Common LTE defines */
#include "kwu.h"           /* KWU Interface defines */
#include "ckw.h"           /* CKW Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "cpj.h"           /* CPJ Interface defines */

#include "nhu.h"           /* NHU Interface defines */
#include "nhu_asn.h"       /* NHU ASN defines */
#include "lnh.h"           /* LNH Interface defines */
#include "pju.h"           /* PJU Interface defines */
#include "nh.h"            /* NH defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* timer services */

#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_hash.x"       /* Common Hashlist Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_pasn.x"       /* Common Hashlist Definitions */


#include "cm_lte.x"        /* common LTE includes */
#include "kwu.x"           /* KWU Interface includes */
#include "ckw.x"           /* CKW Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CDM Interface includes */

#include "nhu.x"           /* NHU Interface includes */
#include "nhu_asn.x"       /* NHU ASN includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nh.x"            /* NH defines */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

EXTERN  int main ARGS((int argc,char **argv ));

EXTERN void exit ARGS((int status ));
  
#ifdef __cplusplus
}
#endif /* __cplusplus */
 
 
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
 *       File:  nh_tst.c
 *
 */
  
#ifdef ANSI
int main
(
int argc,                   /* argument count */
char **argv                 /* argument values */
)
#else /* ANSI */
int main(argc, argv)
int argc;                   /* argument count */
char **argv;                /* argument values */
#endif /* ANSI */
{
   TRC1(main)

   UNUSED(argc);
   UNUSED(argv);

   exit(0);

   return(OK);
} /* end of main */
#endif /* NH_PT_BUILD */

  
/**********************************************************************
  
         End of file:     ni_tst.c@@/main/3 - Fri Jul  1 01:13:22 2011
  
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
/main/1      ---      st               1. RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/
