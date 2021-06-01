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

     Name:     MD5, Checksum and Random Number Generator for the SCTP Layer

     Type:     C header file

     Desc:     Structures required for sb_port.c

     File:     sb_port.x

     Sid:      sb_port.x@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:17 2015

     Prg:      wvdl

*********************************************************************21*/

#ifndef __SB_PORTX__
#define __SB_PORTX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*sb084.102: changes for LONG_MSG*/
EXTERN U32 sbChecksum32       ARGS((U32 checksum, U8 *buf, MsgLen len,U8 checksumType));

EXTERN S16 sbRand32        ARGS((U32 type, U32 *v));

EXTERN S16 sbGen32MAC      ARGS((U8 *string, U32 len, U32 *val));

EXTERN S16 sbGen128MAC     ARGS((U8 *input, U32 len, U8 *signature));

/*sb007.103: 16 bit random number*/
EXTERN S16 sbRand16        ARGS(( U16 *v));

 /* sb059.102: Deleted to remove compilation error with g++ */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SB_PORTX__ */

/********************************************************************30**

         End of file:     sb_port.x@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:40:17 2015

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
 /main/2     ---    wvdl     1. Initial Release.
 /main/2     ---      sb     1. Modified for SCTP release based on 
                                RFC-2960 'Oct 2000.
 sb059.102   ---      pr     1. Deleted gethostid declaration.
 sb084.102   ---      kp     1. changes for LONG_MSG.
/main/3      ---      rsr/ag 1. Updated for Release of 1.3
           sb007.103  ag     1. 16 bit random generation for HBEAT procedure

*********************************************************************91*/


