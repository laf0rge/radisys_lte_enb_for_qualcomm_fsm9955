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

     Name:     Path MTU discovery for the SCTP Layer

     Type:     C header file

     Desc:     Structures required for path MTU discovery

     File:     sb_mtu.x

     Sid:      sb_mtu.x@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:16 2015

     Prg:      bk

*********************************************************************21*/

#ifndef __SB_MTUX__
#define __SB_MTUX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _SbMtuAddrCb
{
   CmHashListEnt     hlEnt;
   CmNetAddr         addr;
   U16               mtu;
   U16               mtuUpper;
   U16               mtuLower;
   U16               timeIdx;
   /* sb046.102: Multiple IP address per Endp */
   U8                nmbAssoc;
} SbMtuAddrCb;

typedef struct _SbMtuCp
{
   CmHashListCp      addrHl;
   U16               state;

#ifdef DEBUGP
   Txt               *prntBuf;          /* print buffer */
#endif

   U16               maxAddr;
   U16               maxMtu;
   U16               minMtu;
   U16               startMtu;
   U16               nextIdx;
   Pool              pool;
   Region            region;
   SbMtuAddrCb       **addrArray;    
   /* sb023.102  - addrArray Structure modified from being a static array to a
    * list of pointers to all the address control blocks */
} SbMtuCp;


EXTERN S16  sbMtuGetMemReq             ARGS((U16         maxAddr,
                                             U32         *memSz));

EXTERN S16  sbMtuInit                  ARGS((SbMtuCp     *mtuCp,
                                             U16         maxAddr,
                                             U16         mtuMin,
                                             U16         mtuMax,
                                             U16         mtuStart,
                                             Pool        pool,
                                             Region      region));


/* sb023.102 - Function added for getting a Address control block - 
Performance change */
EXTERN S16  sbMtuGetMtuCb              ARGS((SbMtuCp     *mtuCp,
                                             CmNetAddr   *addr,
                                             U16         *idx));
/* sb023.102 - Function added for deleting a Address control block - 
Performance change */
EXTERN S16  sbMtuDeleteMtuCb           ARGS((SbMtuCp     *mtuCp,
                                             U16          idx));

EXTERN S16  sbMtuQuerySz               ARGS((SbMtuCp     *mtuCp,
                                             CmNetAddr   *addr,
                                             U16         *mtu));

EXTERN S16  sbMtuQueryDf               ARGS((SbMtuCp     *mtuCp,
                                             CmNetAddr   *addr,
                                             U16         msgSz,
                                             Bool        *df));

EXTERN S16  sbMtuDestroy               ARGS((SbMtuCp     *mtuCp));

EXTERN S16  sbMtuIncMtu                ARGS((SbMtuCp     *mtuCp));

EXTERN S16  sbMtuIncMtuUpper           ARGS((SbMtuCp     *mtuCp));

EXTERN S16  sbMtuRcvIcmp               ARGS((SbMtuCp     *mtuCp,
                                             Buffer      *mBuf,
                                             U8          ver));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SB_MTUX__ */

/********************************************************************30**

         End of file:     sb_mtu.x@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:16 2015

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision History:

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
 /main/2     ---     bk     1. backbone draft.
 /main/2     ---     sb     1. Modified for SCTP release based on 
                               RFC-2960 'Oct 2000.
         sb023.102   sb     1. Changed done in Mtu for Performance.
         sb027.102   hm     1. Removed TAB
         sb046.102   hl     1. Multiple IP address per endpoint support
/main/3      ---   rsr/ag   1. Updated for Release of 1.3
*********************************************************************91*/


