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

     Name:     EnodeB Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_umm_csfb_geran.h

     Sid:      /main/

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_CSFB_GERAN_H__
#define __WR_UMM_CSFB_GERAN_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/*Transaction Choice*/
#define WR_CSFB_GERAN_REDIR    0x00
#define WR_CSFB_GERAN_CCO      0x01
#define WR_CSFB_GERAN_MEAS     0x10
#define WR_CSFB_GERAN_CCO_MEAS 0x11
/*trans IDs for DAT_CFM*/
#define WR_UMM_CSFB_GERAN_CCO_TRANSID          1


#if 0
#define WR_CSFB_GERAN_CONV_T304VAL_TO_ENUM(_val,_enum){\
   /*Need to sutract WR_TMR_T304_GERAN_DELAY from _val as this is the   
    * procedure delay*/                                \
   switch(_val)                                        \
   {                                                   \
      case 100:                                        \
         {                                             \
            _enum = 0;                                 \
            break;                                     \
         }                                             \
      case 200:                                        \
         {                                             \
            _enum = 1;                                 \
            break;                                     \
         }                                             \
      case 500:                                        \
         {                                             \
            _enum = 2;                                 \
            break;                                     \
         }                                             \
      case 1000:                                       \
         {                                             \
            _enum = 3;                                 \
            break;                                     \
         }                                             \
      case 2000:                                       \
         {                                             \
            _enum = 4;                                 \
            break;                                     \
         }                                             \
      case 4000:                                       \
         {                                             \
            _enum = 5;                                 \
            break;                                     \
         }                                             \
      case 8000:                                       \
         {                                             \
            _enum = 6;                                 \
            break;                                     \
         }                                             \
      default: _enum = 3;                              \
               break;                                  \
   }                                                   \
}                                                      
#endif
                                                       
#ifdef __cplusplus                                     
}                                                      
#endif /* __cplusplus */

#endif /*End of _WR_UMM_CSFB_GERAN_H__*/
/********************************************************************30**

           End of file:   /main/ 

*********************************************************************31*/

