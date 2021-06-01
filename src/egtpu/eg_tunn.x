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
  
     Name:     e-GTP layer
  
     Type:     C include file
  
     Desc:     Interface for tunnel management module.
  
     File:     eg_tunn.x
  
     Sid:      eg_tunn.x@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:18:52 2015
     
     Prg:      mm
  
*********************************************************************21*/

#ifndef __EG_TUNN_X__
#define __EG_TUNN_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**@file eg_tunn.x
    @brief Tunnel Management Module for EGTP
*/

/******************************************************************************
 *  Tunnel Manamgement local structures                                       *
 ******************************************************************************/
/**
  @ingroup TUNNMNGMT
    @struct _egTunnEvent
    @brief Enumerates Tunnel events
 */
typedef enum _egTunnEvent
{
   EG_TUN_EVENT_INCSESSREQ,  /**< A Create Session request is received */
   EG_TUN_EVENT_OUTSESSREQ,  /**< A Create Session Request is being sent */
   EG_TUN_EVENT_INCSESSRSP,  /**< A Create Session Response is received */
   EG_TUN_EVENT_OUTSESSRSP,  /**< A Create Session Response is being sent */
   EG_TUN_EVENT_ERR_IND,     /**< An Error Inidication is received from 
                                other module */
   EG_TUN_EVENT_INC_MSG,     /**< A GTP-C message is received on the tunnel */
   EG_TUN_EVENT_OUT_MSG,     /**< A GTP-C message is being sent on the tunnel */
   EG_TUN_EVENT_MAX
}EgTunnEvent;



#ifdef __cplusplus
}
#endif

#endif /* __EG_TUNN_X__ */


  
/********************************************************************30**
  
         End of file:     eg_tunn.x@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:18:52 2015
    
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
/main/1      ---      mm                1. Created for Initial Release 1.1
/main/2      ---      rsharon           1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3      eg004.201 magnihotri       1. Modified for eGTP-C PSF Upgarde:function prototype qualifier
                                           changed from PUBLIC to EXTERN
                                        2. Modified for eGTP-C PSF Upgarde:function prototype qualifier
                                           changed from PUBLIC to PRIVATE
                                        3. Modified for eGTP-C PSF Upgarde:function prototype qualifier
                                           changed from EXTERN to PUBLIC
*********************************************************************91*/
