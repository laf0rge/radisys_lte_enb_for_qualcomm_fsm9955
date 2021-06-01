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
  
     Name:     general layer
  
     Type:     C include file
  
     Desc:     defines , required by cm_tkns.c and cm_tkns.h
   
     File:     cm_tkns.h
  
     Sid:      cm_tkns.h@@/main/5 - Thu Oct 27 00:36:59 2005
  
     Prg:      mb
  
*********************************************************************21*/

#ifndef __CMTKNSH
#define __CMTKNSH

  
/*
*     The structures and variables declared in this file
*     correspond to structures and variables used by
*     the following TRILLIUM software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*
*/
  
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*
*/


/* defines */
#define cmPkTknPres(x, mBuf)       cmPkTknU8(x, mBuf)

#define cmUnpkTknPres(x, mBuf)     cmUnpkTknU8(x, mBuf)


/* packing and unpacking for token bit strings */

#define CMPKTKNBSTR(tknStr, mBuf)                   \
{                                                  \
   Cntr i;                                         \
   Cntr len;                                       \
                                                   \
   if(tknStr->pres)                                \
   {                                               \
      if (tknStr->len % 8)                         \
         len = (tknStr->len/8 ) + 1;               \
      else                                         \
         len = (tknStr->len/8 );                   \
                                                   \
      /* Value */                                  \
      for (i = 0; i < (S16) len; i++)              \
      {                                            \
         CMCHKPK(SPkU8, tknStr->val[i], mBuf);     \
      }                                            \
                                                   \
      /* Length */                                 \
      CMCHKPK(SPkU8, tknStr->len, mBuf);           \
   }                                               \
                                                   \
   /* Token Header */                              \
   CMCHKPK(SPkU8, tknStr->pres, mBuf);             \
}

#define CMUNPKTKNBSTR(tknStr, mBuf)                                 \
{                                                                  \
   Cntr i;                                                         \
   Cntr len;                                                       \
                                                                   \
   /* Token Header */                                              \
   CMCHKUNPK(SUnpkU8, &tknStr->pres, mBuf);                        \
                                                                   \
   if(tknStr->pres)                                                \
   {                                                               \
     /* Length */                                                  \
      CMCHKUNPK(SUnpkU8, &tknStr->len, mBuf);                      \
                                                                   \
      if (tknStr->len % 8)                                         \
         len = (tknStr->len/8 ) + 1;                               \
      else                                                         \
         len = (tknStr->len/8 );                                   \
                                                                   \
      /* Value */                                                  \
      for (i = 1; i <= (S16) len; i++)                             \
      {                                                            \
         CMCHKUNPK(SUnpkU8, &tknStr->val[len - i], mBuf);          \
      }                                                            \
   }                                                               \
                                                                   \
}

#endif /* __CMTKNSH */

  
/********************************************************************30**
  
         End of file:     cm_tkns.h@@/main/5 - Thu Oct 27 00:36:59 2005
    
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************70**
  
    
*********************************************************************71*/

/********************************************************************80**


********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
  1.1        ---      mb   1. Initial Release
  /main/3    ---      dw   1. Added pk/unpk macros for TKNBSTR
/main/4      ---      cvp  1. Merging with latest branch.
/main/5      ---      rbabu 1. Updated for NBAP software release 1.2
*********************************************************************91*/
