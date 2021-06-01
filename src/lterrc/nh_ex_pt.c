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
  
     Name:     RRC
  
     Type:     C file
  
     Desc:     Functions required for scheduling and initialization.
  
     File:     nh_ex_pt.c
  
     Sid:      ni_ex_pt.c@@/main/4 - Fri Jul  1 01:13:18 2011
  
     Prg:      st
  
**********************************************************************/


/* Header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=101;

#include "envopt.h"           /* Environment options      */
#ifdef NH_PT_BUILD
#include "envdep.h"           /* Environment dependent    */
#include "envind.h"           /* Environment independent  */
#include "gen.h"              /* General layer            */
#include "ssi.h"              /* System service interface */
 
/* Header/extern include files (.x)     */
#include "gen.x"              /* General layer            */
#include "ssi.x"              /* System service interface */
   
  
 
/* Local externs */
EXTERN S16 nhActvTsk  ARGS((Pst *pst, Buffer *mBuf));
EXTERN S16 nhInitExt  ARGS((Void));
  



/*
*
*       Fun:    Initialize external
*
*       Desc:   Initializes variables used to interface with Upper/Lower
*               Layer.  
*
*       Ret:    ROK - OK
*
*       Notes:  None
*
*       File:   nh_ex_pt.c
*
*/
  
#ifdef ANSI
PUBLIC S16 nhInitExt
(
void
)
#else /* ANSI */
PUBLIC S16 nhInitExt()
#endif /* ANSI */
{
   TRC2(nhInitExt)
   RETVALUE(ROK);
} /* end of nhInitExt */



/*
*
*       Fun:    Activate task
*
*       Desc:   Processes received event from Upper/Lower Layer.
*
*       Ret:    ROK - OK
*
*       Notes:  None
*
*       File:   nh_ex_pt.c
*
*/
  
#ifdef ANSI
PUBLIC S16 nhActvTsk
(
Pst *pst,                   /* Post */
Buffer *mBuf                /* Message buffer */
)
#else /* ANSI */
PUBLIC S16 nhActvTsk(pst, mBuf)
Pst *pst;                   /* Post */
Buffer *mBuf;               /* Message buffer */
#endif /* ANSI */
{
   TRC3(nhActvTsk)
   UNUSED(pst);
   UNUSED(mBuf);
   SExitTsk();
   RETVALUE(ROK);
} /* end of nhActvTsk */
#endif /* NH_PT_BUILD */

  
/**********************************************************************
  
         End of file:     ni_ex_pt.c@@/main/4 - Fri Jul  1 01:13:18 2011
  
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
/main/2      ---      st               1. RRC Initial Release.
/main/3      ---      ch               1. RRC Release 2.1.
/main/4      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/
