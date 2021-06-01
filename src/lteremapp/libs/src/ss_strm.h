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
 
     Name:     System Services -- STREAMS
 
     Type:     C include file
 
     Desc:     Various macro definitions required for STREAMS.
 
     File:     ss_strm.h
 
     Sid:      ss_strm.h@@/main/2 - Mon Apr  5 18:01:20 2010
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSSTRMH__
#define __SSSTRMH__

#ifdef __cplusplus
extern "C" {
#endif



/* STREAMS message types */
#define SS_M_DATA       0
#define SS_M_PROTO      1
#define SS_M_BREAK      2
#define SS_M_CTL        3
#define SS_M_DELAY      4
#define SS_M_IOCTL      5
#define SS_M_PASSFP     6
#define SS_M_RSE        7
#define SS_M_SETOPTS    8
#define SS_M_SIG        9


/* message initialization macro */
/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN
#define SS_STRM_INITB(mp, dp, dat, size, frtn) \
   (mp)->b_next = NULLP; \
   (mp)->b_prev = NULLP; \
   (mp)->b_cont = NULLP; \
   (mp)->b_rptr = (dat); \
   (mp)->b_wptr = (dat); \
   (mp)->b_datap = (dp); \
   (dp)->db_frtnp = (frtn); \
   (dp)->db_base = (dat); \
   (dp)->db_lim = (dat) == NULLP ? (dat) : ((dat) + (size)); \
   (dp)->db_ref = 1; \
   (dp)->db_type = SS_M_DATA; \
 	(dp)->shared = FALSE;
  
#else
#define SS_STRM_INITB(mp, dp, dat, size, frtn) \
   (mp)->b_next = NULLP; \
   (mp)->b_prev = NULLP; \
   (mp)->b_cont = NULLP; \
   (mp)->b_rptr = (dat); \
   (mp)->b_wptr = (dat); \
   (mp)->b_datap = (dp); \
   (dp)->db_base = (dat); \
   (dp)->db_lim = (dat) == NULLP ? (dat) : ((dat) + (size)); \
   (dp)->db_ref = 1; \
   (dp)->db_type = SS_M_DATA; \
	(dp)->shared = FALSE;
#endif /* SS_DBLK_FREE_RTN */


#ifdef __cplusplus
}
#endif

#endif  /*  __SSSTRMH__  */


  
/********************************************************************30**

         End of file:     ss_strm.h@@/main/2 - Mon Apr  5 18:01:20 2010

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

1.2          ---      kp   1. C++ compilable, cosmetic changes

/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/3  ss008.301 srinivas  1. DBlks Free Routine guarded under 
                                Flag SS_DBLK_FREE_RTN
                             2. Optimization of New buffer management API's
*********************************************************************91*/
