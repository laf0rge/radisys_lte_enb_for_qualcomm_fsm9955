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
     Name:    Upper Layer Interface - RLC

     Type:    C header file

     Desc:    Structures, variables and typedefs required by 
              RGM interface

     File:    rgm.h

     Sid:      rgm.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:54:00 2013

     Prg:      

*********************************************************************21*/


#ifndef __RGMH__
#define __RGMH__
#ifdef __cplusplus
extern "C" {
#endif

/** 
  @file rgm.h 
  @brief Defines for RGM interface.
 */


/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/

/* Event corresponding to each primitive at this interface */
#define EVTRGMBASEEVT      0     /*!< Bind Request */
#define EVTRGMBNDREQ       1     /*!< Bind Request */
#define EVTRGMUBNDREQ      2     /*!< Un-Bind Request */
#define EVTRGMBNDCFM       3     /*!< Bind Confirm */
#define EVTRGMCFGPRBRPRT   4     /*!< PRB Report Configuration */
#define EVTRGMPRBRPRTIND   5     /*!< PRB report Indication */
#define EVTRGMTRANSMODEIND 6     /*!< Trans Mode Chg Indication */
#define EVTRGMMAX          7


/* selector(coupling) values */
#define RGM_SEL_LC              0
#define RGM_SEL_LWLC            1
#define RGM_SEL_TC              2

#define RGM_UBNDREQ_MNGMT       1    /*!< TFU sap unbind reason */
/* RRM_SP1_START */
/* Rgm Prb Usage Info */
#define RGM_PRB_USAGE_UL        1    /* Only UL - Mode TDD */ 
#define RGM_PRB_USAGE_DL        2    /* Only DL - Mode TDD */
/* RRM_SP1_END */


#ifdef __cplusplus
}
#endif
#endif /* __RGMH__ */

/********************************************************************30**

         End of file:     rgm.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:54:00 2013

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
/main/1      ---     sm  1. initial release.
/main/2      ---     st    1. Added Error Codes.
/main/4      ---     rgm_h_001.main_3   sv  1. Changing number of PDUs and LCs
                                              to minimal value for optimization
/main/5      ---      rgm_h_001.main_4   ap 1. Added RGM_MAX_TB macro
/main/6      ---      rgm_h_001.main_5  gvj 1. Updated for LTE MAC Rel 3.1
*********************************************************************91*/
