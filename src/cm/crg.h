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

/**********************************************************************
     Name:     LTE-MAC layer 
  
     Type:     C include file 
  
     Desc:     Defines required by the LTE MAC-RRC control (CRG) interface

     File:     crg.h 

     Sid:      crg.h@@/main/8 - Wed Jul 27 12:23:28 2011

     Prg:      sdass

**********************************************************************/

#ifndef __CRG_H__
#define __CRG_H__

/** 
  @file crg.h 
  @brief Defines for CRG interface.
  */

/* Config/Reconfig/Delete MACROs */
#define CRG_CONFIG   1     /*!< Macro for action type Configuration */
#define CRG_RECONFIG 2     /*!< Macro for action type Re-Configuration */
#define CRG_DELETE   3     /*!< Macro for action type Delete */
#define CRG_RESET    4     /*!< Macro for action type Reset. Applicable
                            *   only for UE. */

/* Cell/Ue/Logical channel MACROs */
/* crg_h_001.main_6: Removed transport channel types from CRG. */
#define CRG_CELL_CFG 1     /*!< Macro for Cell Configuration type*/
#define CRG_UE_CFG   2     /*!< Macro for UE configuration type*/
#define CRG_LCH_CFG  3     /*!< Macro for Logical channel Configuration type*/

/* Transport channel types related MACROs */
/* CRG related MAX Size MACROs */
#define CRG_CFG_TRANSID_SIZE 12  /*!< Macro for Transaction ID size */

/* Transmit/Recieve direction */
#define CRG_DIR_TX_RX CRG_DIR_TX | CRG_DIR_RX /*!< Macro for Transmit and Receive
                                               *   direction Mask */
#define CRG_DIR_TX    (1<<0)                  /*!< Macro for Transmit 
                                               *   direction Mask*/
#define CRG_DIR_RX    (1<<1)                  /*!< Macro for Receive
                                               *   direction Mask*/

/* Confirm Status related MACROs */
#define CRG_CFG_CFM_OK  0x00 /*!< Macro for Positive confirmation */
#define CRG_CFG_CFM_NOK 0x0F /*!< Macro for Negative confirmation */

/* Event corresponding to each primitive at this interface */
#define EVTCRGBNDREQ   1     /*!< Macro for Bind Request */
#define EVTCRGBNDCFM   2     /*!< Macro for Bind Confirm */
#define EVTCRGUBNDREQ  3     /*!< Macro for Un-Bind Request */
#define EVTCRGCFGREQ   4     /*!< Macro for Configuration Request */
#define EVTCRGCFGCFM   5     /*!< Macro for Configuration Confirm */

/* Activation time limit in terms of number of frames */
#define CRG_ACTV_WIN_SIZE 20 /*!< Macro for Size of activation time window
                                (in terms of number of frames) */
/* selector(coupling) values */
#define CRG_SEL_TC 1     /*!< Macro for Tight Coupling */
#define CRG_SEL_LC 0     /*!< Macro for Light Coupling */

/* pack unpack error code */
#define ECRGXXX 0  /* reserved */
#define ERRCRG                  0

/* crg_h_001.main_7 - MODIFY - Added comments to these event definitions */
#define   ECRG001      (ERRCRG +    1)    /*        crg.c: 153 */
#define   ECRG002      (ERRCRG +    2)    /*        crg.c: 161 */
#define   ECRG003      (ERRCRG +    3)    /*        crg.c: 170 */
#define   ECRG004      (ERRCRG +    4)    /*        crg.c: 217 */
#define   ECRG005      (ERRCRG +    5)    /*        crg.c: 226 */
#define   ECRG006      (ERRCRG +    6)    /*        crg.c: 270 */
#define   ECRG007      (ERRCRG +    7)    /*        crg.c: 278 */
#define   ECRG008      (ERRCRG +    8)    /*        crg.c: 287 */
#define   ECRG009      (ERRCRG +    9)    /*        crg.c: 335 */
#define   ECRG010      (ERRCRG +   10)    /*        crg.c: 344 */
#define   ECRG011      (ERRCRG +   11)    /*        crg.c: 387 */
#define   ECRG012      (ERRCRG +   12)    /*        crg.c: 395 */
#define   ECRG013      (ERRCRG +   13)    /*        crg.c: 404 */
#define   ECRG014      (ERRCRG +   14)    /*        crg.c: 451 */
#define   ECRG015      (ERRCRG +   15)    /*        crg.c: 460 */
#define   ECRG016      (ERRCRG +   16)    /*        crg.c: 507 */
#define   ECRG017      (ERRCRG +   17)    /*        crg.c: 516 */
#define   ECRG018      (ERRCRG +   18)    /*        crg.c: 527 */
#define   ECRG019      (ERRCRG +   19)    /*        crg.c: 537 */
#define   ECRG020      (ERRCRG +   20)    /*        crg.c: 547 */
#define   ECRG021      (ERRCRG +   21)    /*        crg.c: 597 */
#define   ECRG022      (ERRCRG +   22)    /*        crg.c: 606 */
#define   ECRG023      (ERRCRG +   23)    /*        crg.c: 614 */
#define   ECRG024      (ERRCRG +   24)    /*        crg.c: 626 */
#define   ECRG025      (ERRCRG +   25)    /*        crg.c: 672 */
#define   ECRG026      (ERRCRG +   26)    /*        crg.c: 680 */
#define   ECRG027      (ERRCRG +   27)    /*        crg.c: 689 */
#define   ECRG028      (ERRCRG +   28)    /*        crg.c: 698 */
#define   ECRG029      (ERRCRG +   29)    /*        crg.c: 747 */
#define   ECRG030      (ERRCRG +   30)    /*        crg.c: 756 */
#define   ECRG031      (ERRCRG +   31)    /*        crg.c: 765 */

#endif /* __CRG_H__ */


/**********************************************************************
  
         End of file:     crg.h@@/main/8 - Wed Jul 27 12:23:28 2011
  
**********************************************************************/

/**********************************************************************
  
        Notes:
  
**********************************************************************/

/**********************************************************************

**********************************************************************/


/**********************************************************************
  
        Revision history:
  
**********************************************************************/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/2      ---   sdass 1. initial release.
/main/4      ---   sdass 1. initial release
/main/7      ---      crg_h_001.main_6 sm  1. Inclusion of Transport channels.
                       2. Removed Transport channel types from CRG.
/main/8      ---   crg_h_001.main_7   gvj  1. Updated for LTE MAC Rel 3.1
*********************************************************************91*/
