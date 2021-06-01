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

     Name:

     Type:     C File.

     Desc:

     File:     ss_acc.h

     Sid:      ss_acc.h@@/main/TeNB_Main_BR/1 - Fri Aug 23 18:06:16 2013

     Prg:

************************************************************************/
/* header/extern include files (.h) */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "ss_dep.h"        /* implementation-specific */
#include "ss_err.h"        /* implementation-specific */
#include "ss_queue.h"      /* queues */
#include "ss_task.h"       /* tasking */
#include "ss_dep.h"       /* tasking */
#include "ss_msg.h"       /* tasking */
#include "ss_mem.h"       /* tasking */
#include "ss_gen.h"       /* tasking */
#include <assert.h>       /* tasking */
/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "ss_dep.x"        /* implementation-specific */
#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_drvr.x"       /* driver tasks */
#include "ss_gen.x"        /* general */


/* local defines */
#define PERMENT      6
#define PERMENT1     5
#define PERMENT2     6
#define MAXTSTCNT    0
#define TSTTIMES     10         /* number of test for certain function */
#define BSIZE        100
#define MAXMSGLEN    256        /* maximum message length */
#define MAXSMEMLEN   0x20       /* maximum static memory length */
#define TMR_TICKS    10
#define NMBTMRTSKS   2
#define TSKCNT       1000


#define SMSTASID     0
#define SMSTASPOOL   1
#define SMSTADPOOL   2
#define SMSTADQ      3
#define SMSTAENT     4
#define SMSTSLOOP    5
#define SMSTSDQ      6
#define SMSTSENT     7
#define NMBPRIOR     1
#define REFAILED     0
#define NMBTSKS      4

/* local typedefs */
#define MAX_TESTS   50
#define PASS        0
#define MAX_STR     64
#define MAX_STR_DIS 16
#define DEFAULT_TEST_INIT(tc, id, name, func, prefunc, pstFunc)   do {\
    (tc)->tcId = (U64) (id);                           \
    strncpy(&((tc)->tcName[0]), name, MAX_STR_DIS);    \
    (tc)->preReq = prefunc;                            \
    (tc)->testFunc = func;                             \
    (tc)->postReq = pstFunc;                           \
    (tc)->result = FAIL;                               \
} while (0)
#define RB_ACC_MAX_MSG    1000
#define MAX_QUEUE_LENGTH  50


/********************************************************************30**

         End of file:     ss_acc.h@@/main/TeNB_Main_BR/1 - Fri Aug 23 18:06:16 2013

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

   ver        pat        init                  description
---------  ---------  ----------  -------------------------------------
/main/TeNB_Main_BR/1    --------     ------    1. Initial release
*********************************************************************91*/
