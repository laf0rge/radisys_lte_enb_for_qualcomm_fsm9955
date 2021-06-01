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
 
     File:     mt_err.h

     Sid:      mt_err.h@@/main/3 - Mon Nov 17 15:54:10 2008
  
     Prg:      ada

*********************************************************************21*/


#ifndef __MTERRH__
#define __MTERRH__
  


/* log error macro */
#define MTLOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError( ENTNC, INSTNC, 0, __FILE__, __LINE__, \
             (ErrCls)errCls, (ErrVal)errCode, (ErrVal)errVal, errDesc )


/* defines */

#define   ERRMT       0
#define   EMTBASE     (ERRMT + 0)    /* reserved */
#define   EMTXXX      (EMTBASE)      /* reserved */

#define   EMT001      (ERRMT +    1)    /*      mt_ss.c:1310 */
#define   EMT002      (ERRMT +    2)    /*      mt_ss.c:1351 */
#define   EMT003      (ERRMT +    3)    /*      mt_ss.c:1448 */
#define   EMT004      (ERRMT +    4)    /*      mt_ss.c:1469 */
#define   EMT005      (ERRMT +    5)    /*      mt_ss.c:1518 */
#define   EMT006      (ERRMT +    6)    /*      mt_ss.c:1532 */
#define   EMT007      (ERRMT +    7)    /*      mt_ss.c:1969 */
#define   EMT008      (ERRMT +    8)    /*      mt_ss.c:1996 */
#define   EMT009      (ERRMT +    9)    /*      mt_ss.c:2098 */
#define   EMT010      (ERRMT +   10)    /*      mt_ss.c:2129 */
#define   EMT011      (ERRMT +   11)    /*      mt_ss.c:2160 */
#define   EMT012      (ERRMT +   12)    /*      mt_ss.c:2226 */
#define   EMT013      (ERRMT +   13)    /*      mt_ss.c:2302 */
#define   EMT014      (ERRMT +   14)    /*      mt_ss.c:2329 */
#define   EMT015      (ERRMT +   15)    /*      mt_ss.c:2370 */
#define   EMT016      (ERRMT +   16)    /*      mt_ss.c:2473 */
#define   EMT017      (ERRMT +   17)    /*      mt_ss.c:2498 */
#define   EMT018      (ERRMT +   18)    /*      mt_ss.c:2535 */
#define   EMT019      (ERRMT +   19)    /*      mt_ss.c:2578 */
#define   EMT020      (ERRMT +   20)    /*      mt_ss.c:2747 */
#define   EMT021      (ERRMT +   21)    /*      mt_ss.c:2772 */
#define   EMT022      (ERRMT +   22)    /*      mt_ss.c:2882 */
#define   EMT023      (ERRMT +   23)    /*      mt_ss.c:2972 */
#define   EMT024      (ERRMT +   24)    /*      mt_ss.c:3022 */
#define   EMT025      (ERRMT +   25)    /*      mt_ss.c:3086 */
#define   EMT026      (ERRMT +   26)    /*      mt_ss.c:3092 */
#define   EMT027      (ERRMT +   27)    /*      mt_ss.c:3141 */
#define   EMT028      (ERRMT +   28)    /*      mt_ss.c:3144 */
#define   EMT029      (ERRMT +   29)    /*      mt_ss.c:3453 */
#define   EMT030      (ERRMT +   30)    /*      mt_ss.c:3533 */
#define   EMT031      (ERRMT +   31)    /*      mt_ss.c:3613 */
#define   EMT032      (ERRMT +   32)    /*      mt_ss.c:3685 */
/* mt033.201 - addition of error codes */
#define   EMT033      (ERRMT +   33)    /*      mt_ss.c:3902 */
#define   EMT034      (ERRMT +   34)    /*      mt_ss.c:3984 */
#define   EMT035      (ERRMT +   35)    /*      mt_ss.c:4217 */
/* multi-core support */
#define   EMT036      (ERRMT +   36)    /*      mt_ss.c:1689 */
#define   EMT037      (ERRMT +   37)    /*      mt_ss.c:1709 */
#define   EMT038      (ERRMT +   38)    /*      mt_ss.c:1821 */
#define   EMT039      (ERRMT +   39)    /*      mt_ss.c:5085 */
#define   EMT040      (ERRMT +   40)    /*      mt_ss.c:5099 */
#define   EMT041      (ERRMT +   41)    /*      mt_ss.c:5106 */
#define   EMT042      (ERRMT +   42)    /*      mt_ss.c:5126 */
#define   EMT043      (ERRMT +   43)    /*      mt_ss.c:5133 */
#define   EMT044      (ERRMT +   44)    /*      mt_ss.c:5148 */
#define   EMT045      (ERRMT +   45)    /*      mt_ss.c:5155 */
#define   EMT046      (ERRMT +   46)    /*      mt_ss.c:5175 */



#endif

  
/********************************************************************30**
  
         End of file:     mt_err.h@@/main/3 - Mon Nov 17 15:54:10 2008
   
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
1.2+        mt028.201 kkj  1. Multiple procIds related changes
1.2+        mt033.201 vk   1. Addition of error code for SGetRegInfo
/main/3      ---      rp   1. SSI enhancements for Multi-core 
                              architecture support
*********************************************************************91*/
