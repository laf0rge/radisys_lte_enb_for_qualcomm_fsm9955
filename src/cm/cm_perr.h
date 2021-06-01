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
  
     Name:     PER ASN.1 error header file
  
     Type:     C include file
  
     Desc:     Error Defines used by encoding/decoding routines
  
     File:     cm_perr.h
  
     Sid:      cm_perr.h@@/main/8 - Thu Sep 25 19:36:36 2008
  
     Prg:      mp
  
*********************************************************************21*/

#ifndef __CMPERRH__
#define __CMPERRH__


#define SLOGERROR(ent, inst, procId, __FILE__, __LINE__,  \
                  errCls, errCode, errVal, errDesc)        \
                       SLogError(ent, inst, procId, __FILE__, __LINE__, \
                                  errCls, errCode, errVal, errDesc)
/* cm_perr_h_001.main_7 Changes to support Multiple ProcIds */ 
#ifdef SS_MULTIPLE_PROCS
#define CMPASNLOGERROR(errCls, errCode, errVal, errDesc) \
        SLOGERROR(ENTNC, INSTNC, 0, \
                  __FILE__, __LINE__, errCls, errCode, errVal, errDesc)
#else
#define CMPASNLOGERROR(errCls, errCode, errVal, errDesc) \
        SLOGERROR(ENTNC, INSTNC, SFndProcId(), \
                  __FILE__, __LINE__, errCls, errCode, errVal, errDesc)
#endif /* SS_MULTIPLE_PROCS */

#define ECMPASNBASE 0
#define ECMPASNXXX ECMPASNBASE
#define ERRCMPASN  ECMPASNBASE

#define   ECMPASN001      (ERRCMPASN +    1)    /*   cm_pasn1.c: 243 */
#define   ECMPASN002      (ERRCMPASN +    2)    /*   cm_pasn1.c: 350 */
#define   ECMPASN003      (ERRCMPASN +    3)    /*   cm_pasn1.c: 694 */
#define   ECMPASN004      (ERRCMPASN +    4)    /*   cm_pasn1.c: 929 */
#define   ECMPASN005      (ERRCMPASN +    5)    /*   cm_pasn1.c:1231 */
#define   ECMPASN006      (ERRCMPASN +    6)    /*   cm_pasn1.c:1821 */
#define   ECMPASN007      (ERRCMPASN +    7)    /*   cm_pasn1.c:1831 */
#define   ECMPASN008      (ERRCMPASN +    8)    /*   cm_pasn1.c:1933 */
#define   ECMPASN009      (ERRCMPASN +    9)    /*   cm_pasn1.c:1943 */
#define   ECMPASN010      (ERRCMPASN +   10)    /*   cm_pasn1.c:2248 */

#define   ECMPASN011      (ERRCMPASN +   11)    /*   cm_pasn2.c: 481 */
#define   ECMPASN012      (ERRCMPASN +   12)    /*   cm_pasn2.c: 720 */
#define   ECMPASN013      (ERRCMPASN +   13)    /*   cm_pasn2.c: 906 */
#define   ECMPASN014      (ERRCMPASN +   14)    /*   cm_pasn2.c:1813 */
#define   ECMPASN015      (ERRCMPASN +   15)    /*   cm_pasn2.c:2137 */

#define   ECMPASN016      (ERRCMPASN +   16)    /*   cm_pasn3.c: 246 */
#define   ECMPASN017      (ERRCMPASN +   17)    /*   cm_pasn3.c: 276 */
#define   ECMPASN018      (ERRCMPASN +   18)    /*   cm_pasn3.c: 418 */
#define   ECMPASN019      (ERRCMPASN +   19)    /*   cm_pasn3.c:2090 */


#endif /* __CMPERRH__ */
 
  
/********************************************************************30**
  
         End of file:     cm_perr.h@@/main/8 - Thu Sep 25 19:36:36 2008
   
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
 
 
*********************************************************************81*/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.1          ---      mp   1. initial release.

/main/2        ---      bsr  1. New error codes added
 

/main/4      ---      pk   1. Updated for rel 1.3
/main/5      ---      rbabu 1. updated for NBAP software release 1.2
/main/7      ---      rbhat 1. RRC initial release.
/main/8      cm_perr_h_001.main_7  hsingh  1. Changes to support Multiple ProcIds.
*********************************************************************91*/
