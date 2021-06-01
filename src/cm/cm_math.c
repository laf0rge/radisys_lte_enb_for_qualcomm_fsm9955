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

     Name:     common MATH library functions
  
     Type:     C source file
  
     Desc:     Common functions that are implemented in
               a portable manner. These functions are selected 
               based on the operating system.

     File:     cm_math.c

     Sid:      cm_math.c@@/main/2 - Fri Dec 25 00:32:10 2009

     Prg:      ak

*********************************************************************21*/


/* header include files (.h) */
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */
#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */

#include "cm_math.h"
#include "cm_math.x"

#include <stdlib.h>

#include <math.h>


#ifdef SS_FLOAT
/*
*
*       Fun:   cmAbs
*
*       Desc:  common primitive for an absolute value of an integer
*
*       Ret:   absolute value of the operand 
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
#ifdef ANSI
PUBLIC U32 cmAbs
(
F64          val
)
#else
PUBLIC U32 cmAbs(val)
F64          val;
#endif
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   RETVALUE((U32) abs((int) val));/* G++ */
#endif
} /* end of cmAbs */
#endif /* SS_FLOAT */


#ifdef SS_FLOAT
/*
*
*       Fun:   cmPow
*
*       Desc:  common primitive for computing the power of x raised to y.
*
*       Ret:   value of x raised to the power of y.
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
#ifdef ANSI
PUBLIC F64 cmPow
(
F64          x,
F64          y
)
#else
PUBLIC F64 cmPow(x, y)
F64           x;
F64           y;
#endif
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   RETVALUE((F64) pow((F64)x, (F64)y));
#endif
} /* end of cmPow */
#endif /* SS_FLOAT */

#ifdef SS_FLOAT
/*
*
*       Fun:   cmFloor
*
*       Desc:  common primitive for computing the largest integral value 
*              not greater than x.
*
*       Ret:   floor value of x 
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
#ifdef ANSI
PUBLIC F64 cmFloor
(
F64          x
)
#else
PUBLIC F64 cmFloor(x)
F64           x;
#endif
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   RETVALUE((F64) floor((F64)x));
#endif

} /* end of cmFloor */
#endif /* SS_FLOAT */

#ifdef SS_FLOAT
/*
*
*       Fun:   cmLog
*
*       Desc:  common primitive for computing the natural logrithm of x.
*
*       Ret:   loge(x)
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
#ifdef ANSI
PUBLIC F64 cmLog
(
F64          x
)
#else
PUBLIC F64 cmLog(x)
F64           x;
#endif
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   RETVALUE((F64) log((F64)x));
#endif

} /* end of cmLog */
#endif /* SS_FLOAT */

#ifdef SS_FLOAT
/*
*
*       Fun:   cmLog10
*
*       Desc:  common primitive for computing the base 10 logrithm of x.
*
*       Ret:   log10(x)
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
#ifdef ANSI
PUBLIC F64 cmLog10
(
F64          x
)
#else
PUBLIC F64 cmLog10(x)
F64           x;
#endif
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   RETVALUE((F64) log10((F64)x));
#endif
} /* end of cmLog10 */
#endif /* SS_FLOAT */

 
/********************************************************************30**
  
         End of file:     cm_math.c@@/main/2 - Fri Dec 25 00:32:10 2009
  
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

*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      tsm  1. initial release.
/main/2      ---      sdass  1. Added float options for log10 and pow

*********************************************************************91*/
