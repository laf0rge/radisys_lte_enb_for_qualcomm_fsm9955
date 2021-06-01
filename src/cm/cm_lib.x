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

     Name:     common library functions
  
     Type:     C include file
  
     Desc:     Prototypes for common functions that are implemented in
               both a portable and a performance-efficient manner. These
               functions are selected based on the operating system.

     File:     cm_lib.x

     Sid:      cm_lib.x@@/main/9 - Wed Jan 27 20:18:04 2010

     Prg:      ak

*********************************************************************21*/



#ifndef __CMLIBX__
#define __CMLIBX__

#ifdef __cplusplus
extern "C" {
#endif

PUBLIC U8 *cmMemcpy      ARGS((U8 *tgt, CONSTANT U8 *src, PTR len));
PUBLIC S16 cmMemcmp      ARGS((CONSTANT U8 *s1, CONSTANT U8 *s2, PTR len));
PUBLIC U8 *cmMemset      ARGS((U8 *src, U8 val, PTR len));

PUBLIC S16 cmStrcmp      ARGS((CONSTANT U8 *s1, CONSTANT U8 *s2));
/* cm_lib_x_001.main_8:Changing from S16 to MsgLen.*/
PUBLIC S16 cmStrncmp     ARGS((CONSTANT U8 *s1, CONSTANT U8 *s2, MsgLen len));
PUBLIC MsgLen cmStrlen   ARGS((CONSTANT U8 *s));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif   /* __CMLIBX__ */

  
/********************************************************************30**
  
         End of file:     cm_lib.x@@/main/9 - Wed Jan 27 20:18:04 2010
  
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
1.1          ---      ak   1. initial release
 
1.2          ---      mk   2. Modified the function prototype declaration
                              for cmMemcpy, cmMemcmp, cmMemset, for the
                              data type of parameter 'len' from S16 to PTR
             ---      mg   3. Changes to pass through chksrc.
1.3          ---      bbk  1. Changed copyright header date.

1.4          ---      ns   1. Added extern "C" for making this file C++
                              compatible

1.5          ---      tej  1. Change in header
/main/7      ---       mj   1. MPLS release 1.2 changes.
/main/8      ---      st   1. Update for MAP Release 2.3
/main/9      ---  cm_lib_x_001.main_8    ve   1. Changing from S16 to MsgLen.
*********************************************************************91*/
