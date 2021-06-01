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
 
     Name:     System Services -- implementation specific definitions
 
     Type:     C include file
 
     Desc:     This file conditionally includes various implementation
               specific files.
 
     File:     ss_dep.h
 
     Sid:      ss_dep.h@@/main/4 - Mon Nov 17 15:54:14 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSDEPH__
#define __SSDEPH__

#ifdef __cplusplus
extern "C" {
#endif

/*ss014.301 : 4GMX SSI specific includes*/
#ifdef SS_4GMX_LCORE
#include "rb_include.h"
#include <stdio.h>
#include <string.h>
#else
/*ss011.301 : RMI SSI specific includes*/
#ifdef SS_RMIOS
#include <stdio.h>
#include "uc_ss.h"
#include "string.h"
#include "byteorder.h"
#include "cm5.h"
#else
#ifdef SS_MT
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include "cm5.h"
#include "mt_ss.h"
  /* ss003.13: addition */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#else /* not SS_MT */

#ifdef SS_PS
#include <psos.h>
#include "cm5.h"
#include "ps.h"
#include "ps_ss.h"
  /* ss009.13: addition */
#include <string.h>
#else /* not SS_PS */

#ifdef SS_VW
  /* ss002.13: addition */
#include <string.h>
#include "vw_ss.h"
#include <sys/times.h>
#include <ioLib.h>
#include <sockLib.h>
#include <selectLib.h>
#include <hostLib.h>
#include "cm5.h"
#else /* not SS_VW */

#ifdef NS
#ifdef FAR
#undef FAR
#endif
#ifdef NEAR
#undef NEAR
#endif
#ifdef NU 
#include <stdio.h>
#include "windows.h"
#include "winbase.h"
#endif /* NU */

#ifdef NK
#include "ntddk.h"
#endif /* NK */
#include "ns_err.h"
#include "ns_task.h"
#include "ns_timer.h"
#include "ns_gen.h"
#else /* not NS */

#ifdef SS_SK
#include "sk_ss.h"
#else /* not SS_SK */

#error "Specify an implementation"

#endif /* SS_SK */
#endif /* NS */
#endif /* SS_VW */
#endif /* SS_PS */
#endif /* SS_MT */
#endif /* SS_RMIOS */
#endif /* SS_4GMX_LCORE */


#ifdef __cplusplus
}
#endif

#endif  /*  __SSDEPH__  */


  
/********************************************************************30**

         End of file:     ss_dep.h@@/main/4 - Mon Nov 17 15:54:14 2008

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
1.1          ---      kp   1. initial release
  
1.2          ---      kp   1. Added SS_SK includes

1.3          ---      kp   1. Added SS_MT and SS_PS includes
             ---      jn   2. String.h included
             ---      jn   3. String.h included for MT_SS
             ---      jn   3. String.h included for SS_PS 
/main/4      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/4+     ss001.301 schandra1 1. SSI phase2 value added feature additions
/main/4+     ss011.301 ssahniX   1. RMIOS related changes 
/main/4+     ss014.301 ssahniX   1. Added include's specific to SSI-4GMX 
*********************************************************************91*/
