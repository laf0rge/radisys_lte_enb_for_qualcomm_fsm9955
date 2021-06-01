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

     Name:     RRM Interface Layer

     Type:     defination  file

     Desc:     

     File:     rmu.h

     Sid:      rmu.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:44:23 2013

     Prg:      rb

*********************************************************************21*/

#ifndef __RMUH__
#define __RMUH__

#ifdef __cplusplus
EXTERN "C" {
#endif

/*!< Maximum number of ERABs allowed per UE */
#define RMU_MAX_NUM_ERABS   8

/*!< UE Inactivity timer expired at FSM since UE doesn't have any data */
#define RMU_UE_NO_DATA 0

/*!< UE Inactivity timer s re-started at FSM since UE have data */
#define RMU_UE_DATA 1 

#define RMU_RECFG_REQ_ERAB_CONFIG         1
#define RMU_RECFG_REQ_UE_CAP              2
#define RMU_RECFG_REQ_NEW_CRNTI           4
#define RMU_RECFG_REQ_CSG_MEMBERSHIP_MOD  8

#define RMU_RECFG_RSP_ERAB_CONFIG      1
#define RMU_RECFG_RSP_UE_RSRC_CONFIG   2
/*Events in RMU interface*/
#define EVTRMUBASEEVT      0 
#define EVTRMUOVLDSTRTIND  1 
#define EVTRMUOVLDSTPIND   2 
#define EVTRMUUEADMITREQ   3 
#define EVTRMUUEHOREQ      4 
#define EVTRMUUERELREQ     5 
#define EVTRMUUERECFGREQ   6 


#define EVTRMUCELLRECFGIND 7 
#define EVTRMUUEADMITRSP   8 
#define EVTRMUUEHORSP      9 
#define EVTRMUUERELRSP     10 
#define EVTRMUUERELIND     11 
#define EVTRMUUERECFGRSP   12
	
#define EVTRMUBNDREQ       13
#define EVTRMUBNDCFM       14
	
#define EVTRMUFSMSTAIND    15
/* SPS changes starts */
#define EVTRMUUESPSDISIND  16
#define EVTRMUUEERABRELIND 17 /*!< Event UE ERAB Release indication */
#define EVTRMUUEINACTIND   18 /*!< Event UE inactivity indication*/
#define EVTRMUUERECFGIND   19 /*!< Event UE Reconfiguration Indication */
#define EVTRMUMAX          20 /*!< Maximum RMU interface Event */
/* SPS changes ends */

#define RMU_SEL_LC     0
#define RMU_SEL_LWLC   1 
#define RMU_SEL_TC     2 
#define RMU_SEL_NPKLC  3

#if 0
#ifdef RMU_LWLC
#define RMU_SEL_LWLC   1 
#define RMU_SEL_TC     2 
#ifdef RMU_NPLC
#define RMU_SEL_NPKLC  2
#define RMU_SEL_TC     3 
#endif /*RMU_NPLC*/
#else  /*RMU_LWLC*/
#define RMU_SEL_TC     1 
#ifdef RMU_NPLC
#define RMU_SEL_NPKLC  1 
#define RMU_SEL_TC     2 
#endif /*RMU_NPLC*/
#endif /*RMU_LWLC*/
#endif


/* Bitmap values for different PHY dedicated resources */
#define RM_RCM_PHY_DED_RES_CFG_SR   1
#define RM_RCM_PHY_DED_RES_CFG_CQI  2
#define RM_RCM_PHY_DED_RES_CFG_SRS  4
#define RM_RCM_PHY_DED_RES_CFG_ANT  8

/* Bitmap values for the different MAC Res  */
/* Data Structures Need to be updated to use these values */
#define RM_RCM_MAC_MAIN_CFG_ULSCH    1
#define RM_RCM_MAC_MAIN_CFG_DRX      1<<1
#define RM_RCM_MAC_MAIN_CFG_TA       1<<2
#define RM_RCM_MAC_MAIN_CFG_PHR      1<<3

/* Bitmap values for different RadioResource types */
#define RM_RCM_MAC_MAIN_CFG      1
#define RM_RCM_PHY_DED_RES_CFG  	2

/* SPS changes starts */
#define RM_RCM_DL_SPS_CFG    4
#define RM_RCM_UL_SPS_CFG    8
/* SPS changes ends */
/* Bitmap values for the type of resources configured to the UE */
#define RM_RCM_UE_RADIO_RES_SR_BITMAP_VAL 	1
#define RM_RCM_UE_RADIO_RES_CQI_BITMAP_VAL  2
#define RM_RCM_UE_RADIO_RES_SRS_BITMAP_VAL 	4
#define RM_RCM_UE_RADIO_RES_DRX_BITMAP_VAL 	8
#define RM_RCM_UE_RADIO_RES_DRX_REL_BITMAP_VAL 	(1<<7)
/* SPS changes starts */
#define RM_RCM_UE_RADIO_RES_SPS_BITMAP_VAL   16   
/* SPS changes ends */

/* Macro for defing Max no. of N1 PUCCH resources allocated for a UE*/   
#define RM_RCM_SPS_MAX_N1_PUCCH     4   

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __RMUH__ */

/********************************************************************30**

         End of file:     rmu.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:44:23 2013

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
*********************************************************************91*/
