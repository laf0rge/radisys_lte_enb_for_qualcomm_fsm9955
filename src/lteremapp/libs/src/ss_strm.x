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
 
     Desc:     Data structure definitions for STREAMS messages.
 
     File:     ss_strm.x
 
     Sid:      ss_strm.x@@/main/3 - Mon Nov 17 15:54:23 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSSTRMX__
#define __SSSTRMX__

#ifdef __cplusplus
extern "C" {
#endif


/* message block is defined in ssi.x */
typedef struct ssmsgb    SsMblk;



/* data block */
struct ssdatab
{
   U8                   db_ref;                 /* reference count */
   U8                   db_type;                /* type of data block */
	/* ss006.301 */
   Bool                 shared;                 /* is this data block shared ? */
   U8                   *db_base;               /* base of data buffer */
   U8                   *db_lim;                /* limit of data buffer */
	/* ss008.301 - deprecating for normal OS, useful for Cavium/RMI OS*/
#ifdef SS_DBLK_FREE_RTN
   struct ssfree_rtn    *db_frtnp;             /* free routine */
#endif /* SS_DBLK_FREE_RTN */
};

typedef struct ssdatab   SsDblk;



/* to free memory used with esballoc() */
struct ssfree_rtn
{
   void                 (*free_func)(char *);   /* free function */
   char                 *free_arg;              /* argument to function */
};

typedef struct ssfree_rtn SsFrtn;



/* STREAMS functions prototypes */
PUBLIC S16      ssStrmCfg ARGS((Region, Region));
PUBLIC S32      ssAdjMsg ARGS((SsMblk *, S32));
PUBLIC SsMblk   *ssAllocB ARGS((S32, U32));
PUBLIC SsMblk   *ssCopyB ARGS((SsMblk *));
PUBLIC SsMblk   *ssCopyMsg ARGS((SsMblk *));
PUBLIC SsMblk   *ssDupB ARGS((SsMblk *));
PUBLIC SsMblk   *ssDupMsg ARGS((SsMblk *));
PUBLIC SsMblk   *ssESBAlloc ARGS((U8 *, S32, S32, SsFrtn *));
PUBLIC void     ssFreeB ARGS((SsMblk *));
PUBLIC void     ssFreeMsg ARGS((SsMblk *));
PUBLIC void     ssLinkB ARGS((SsMblk *, SsMblk *));
PUBLIC S32      ssMsgDSize ARGS((SsMblk *));
PUBLIC S32      ssPullupMsg ARGS((SsMblk *, S32));
PUBLIC SsMblk   *ssRmvB ARGS((SsMblk *, SsMblk *));
PUBLIC S32      ssTestB ARGS((S32, U32));
PUBLIC SsMblk   *ssUnlinkB ARGS((SsMblk *));


#ifdef __cplusplus
}
#endif

#endif  /*  __SSSTRMX__  */


  
/********************************************************************30**

         End of file:     ss_strm.x@@/main/3 - Mon Nov 17 15:54:23 2008

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
/main/3+   ss006.301  vsrao 1. Optimized the Dblk sharing in mBuf
/main/3+   ss008.301 srinivas  1. DBlks Free Routine guarded under
                                  Flag SS_DBLK_FREE_RTN
                               2. Optimization of New buffer management API's
*********************************************************************91*/
