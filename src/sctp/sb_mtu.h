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

     Type:     C include file

     Desc:     Defines required for path MTU discovery

     File:     sb_mtu.h

     Sid:      sb_mtu.h@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:16 2015

     Prg:      bk

*********************************************************************21*/

#ifndef __SB_MTUH__
#define __SB_MTUH__
/* sb005.103: removing unused  hashdefs */
/*
#define SB_MTU_MAX_ADDR          100
#define SB_MTU_MIN_ADDR          1
#define SB_MTU_ARRAY_SIZE        (2*SB_MTU_MAX_ADDR+1)
*/

#define SB_MTU_STATE_INACTIVE    0
#define SB_MTU_STATE_ACTIVE      1

#define SB_MTU_INC_MTU           36000
#define SB_MTU_INC_MTU_UPPER     6000

#define SB_MTU_PROT_ICMP4        1
#define SB_MTU_PROT_ICMP6        58

#define SB_MTU_ICMP4_DST_UNREACH 3
#define SB_MTU_ICMP4_NEED_FRAG   4
#define SB_MTU_ICMP6_TOO_BIG_TYPE 2
#define SB_MTU_ICMP6_TOO_BIG_CODE 0

#define SB_MTU_ALLOC(_mtuCp, _size, _datPtr, _ret)     \
{                                                      \
   _ret = SGetSBuf(_mtuCp->region,                     \
                   _mtuCp->pool,                       \
                   (Data**)&_datPtr, _size);           \
   if ( _ret == ROK )                                  \
   {                                                   \
      cmMemset((U8*)_datPtr, 0, _size);                \
   }                                                   \
   else                                                \
   {                                                   \
      _datPtr = NULLP;                                 \
   }                                                   \
}

/* zero out and deallocate a static buffer */
#define SB_MTU_FREE(_mtuCp, _size, _datPtr)             \
{                                                       \
   (Void) SPutSBuf(_mtuCp->region,                      \
                   _mtuCp->pool,                        \
                   (Data*)_datPtr, _size);              \
}

/* copy a network address */
#define SB_MTU_CPY_NADDR(_tgtPtr, _srcPtr)                              \
{                                                                       \
   S16 _i;                                                              \
                                                                        \
   SB_ZERO((_tgtPtr), sizeof(CmNetAddr));                               \
                                                                        \
   if ((_srcPtr) == (CmNetAddr *)NULLP)                                 \
   {                                                                    \
      (_tgtPtr)->type = CM_NETADDR_IPV4;                                \
      (_tgtPtr)->u.ipv4NetAddr = 0;                                     \
   }                                                                    \
   else                                                                 \
   {                                                                    \
      if ((_srcPtr)->type == CM_NETADDR_IPV4)                           \
      {                                                                 \
         (_tgtPtr)->type = CM_NETADDR_IPV4;                             \
         (_tgtPtr)->u.ipv4NetAddr = (_srcPtr)->u.ipv4NetAddr;           \
      }                                                                 \
      else if ((_srcPtr)->type == CM_NETADDR_IPV6)                      \
      {                                                                 \
         (_tgtPtr)->type = CM_NETADDR_IPV6;                             \
         for (_i = 0; _i < CM_IPV6ADDR_SIZE; _i++)                      \
         {                                                              \
            (_tgtPtr)->u.ipv6NetAddr[_i] = (_srcPtr)->u.ipv6NetAddr[_i];\
         }                                                              \
      }                                                                 \
      else                                                              \
      {                                                                 \
         (_tgtPtr)->type = CM_NETADDR_IPV4;                             \
         (_tgtPtr)->u.ipv4NetAddr = 0;                                  \
      }                                                                 \
   }                                                                    \
} 


#endif /* __SB_MTUH__ */

/********************************************************************30**

         End of file:     sb_mtu.h@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:16 2015

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
 /main/2     ---    bk      1. backbone draft.
 /main/2     ---     sb     1. Modified for SCTP release based on 
                               RFC-2960 'Oct 2000.
         sb005.103   ag     1. Removed hash defines(SB_MTU_MIN_ADDR, SB_MTU_ARRAY_SIZE 
                               and SB_MTU_MAX_ADDR).

*********************************************************************91*/


