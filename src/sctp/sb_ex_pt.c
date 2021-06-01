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

     Name:    SCTP - external interface - portable

     Type:    C source file

     Desc:    Portable unpacking routines.

     File:    sb_ex_pt.c

     Sid:      sb_ex_pt.c@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:11 2015

     Prg:     bk

*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
/* sb010.102: Include file added for portable upper layer */
#include "cm_inet.h"
#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* common timer */
#include "cm_err.h"        /* common error */
#include "sb_err.h"        /* TUCL error defines */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"        /* common DNS */
#include "sct.h"           /* upper interface */
#include "lsb.h"           /* TUCL layer mgt defines */
/* sb010.102: Include file added for portable upper layer */
#include "sb_mtu.h"
#include "sb.h"            /* TUCL internal defines */

/* header/extern include files (.x) */

#include "gen.x"           /* general */
#include "cm_inet.x"       /* common sockets */
#include "ssi.x"           /* system services */
/* sb010.102: Include file added for portable upper layer */
#include "cm_inet.x"
#include "cm_hash.x"       /* common hash */
#include "cm_llist.x"      /* common linked list */
#include "cm5.x"           /* common timer */
#include "cm_tpt.x"        /* common transport typedefs */
#include "cm_dns.x"        /* common DNS */
#include "sct.x"           /* upper interface */
#include "lsb.x"           /* TUCL layer mgt structures */
/* sb011.102: Include file added for portable upper layer */
#include "sb_mtu.x"
#include "sb.x"            /* TUCL internal */


/* local defines */

/* local typedefs */

/* local externs */

/* forward references */

/* functions in other modules */

/* public variable declarations */

/* private variable declarations */


/*
*     support functions
*/


/*
*
*       Fun:    initialize external
*
*       Desc:   Initializes variables used to interface with Upper/Lower
*               Layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   sb_ex_pt.c
*
*/

#ifdef ANSI
PUBLIC S16 sbInitExt
(
Void
)
#else
PUBLIC S16 sbInitExt()
#endif
{
   TRC2(sbInitExt)

   RETVALUE(ROK);
} /* end of sbInitExt */



/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from Upper/Lower Layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   sb_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC S16 sbActvTsk
(
Pst      *pst,              /* post */
Buffer   *mBuf              /* message buffer */
)
#else
PUBLIC S16 sbActvTsk(pst, mBuf)
Pst      *pst;              /* post */
Buffer   *mBuf;             /* message buffer */
#endif
{
   TRC3(sbActvTsk)

   UNUSED(pst);
   UNUSED(mBuf);

   SExitTsk();

   RETVALUE(ROK);
} /* end of sbActvTsk */


/********************************************************************30**

         End of file:     sb_ex_pt.c@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:11 2015

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
 /main/2     ---    asa     1. initial release.
 /main/2     ---     sb     1. Modified for SCTP release based on 
                               RFC-2960 'Oct 2000.
          sb010.102  rs     1. Include file added for portability
          sb011.102  rs     1. Include file added for portability
/main/3      ---   rsr/ag   1. Updated for Release of 1.3
*********************************************************************91*/
