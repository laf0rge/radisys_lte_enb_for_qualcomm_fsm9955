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
  
        Name:     LTE RLC file for uplink and non real time tasks 
    
        Type:     C include file
  
        Desc:     This file contains helper macros for RLC uplink
                  and non real time tasks
 
        File:     kw_ul.h

        Sid:      kw_ul.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB_GA/5 - Fri Feb 12 12:09:16 2016
  
        Prg:      ag
  
*********************************************************************21*/
/** 
 * @file kw_ul.h
 * @brief RLC uplink helper macros
*/

/** @ingroup uplink_group
*/

#ifndef __KW_ULH__
#define __KW_ULH__

/**
 * @def KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB
 *
 *    This macro is used to check if UL IP throughput measurement is ON
 *    or off for the passed rb
 *
 *    Returns TRUE(non-zero) if measurement is ON else FALSE (zero)
 *
 * @param[in] _gCb     RLC UL Cb
 * @param[in] _rbCb    RLC uplink control block
 *
*/ 
#define KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
     (_gCb->u.ulCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UL_IP) && \
     (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_UL_IP))
#endif /* __KW_ULH__ */
  
/********************************************************************30**
  
         End of file:     kw_ul.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB_GA/5 - Fri Feb 12 12:09:16 2016
  
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
/main/1      ---       mn  1. Initial release.
*********************************************************************91*/
