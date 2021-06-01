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
  
        Name:    UDX  Control Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures for and prototypes
                  UDX Interface
 
        File:    udx.h

        Sid:      kw_udx.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:23:18 2015
  
        Prg:     chakrapani
  
*********************************************************************21*/

#ifndef __KW_UDX_H__
#define __KW_UDX_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @file udx.h
    @brief UDX Interface File (ckw.h)
*/
 
/* UDX Interface Hash Defines */

/* Loose Coupling define */
#define UDX_SEL_LC            0     /*!< Loose Coupling Option */
#define UDX_SEL_LWLC          1
#define KW_MAX_UDX 3


/* CKW Interface defines */
#define UDX_MAX_ENT_CFG       24    /*!< Maximum number of entities to configure */

#define   ERRUDX 0
#define   EUDXXXX 0
/* ckw_h_001.main_3 added support for L2 Measurement */   
#ifdef LTE_L2_MEAS
#define UDX_MAX_QCI           10
#endif
/* CKW Interface configuration type */
#define UDX_CFG_ADD           1     /*!< Add RLC Entity */
#define UDX_CFG_MODIFY        2     /*!< Modify RLC Entity */
#define UDX_CFG_DELETE        3     /*!< Delete RLC entity */
#define UDX_CFG_REESTABLISH   4     /*!< Re-establish RLC entity */
#define UDX_CFG_DELETE_UE     5     /*!< Release RLC entities per UE */
#define UDX_CFG_DELETE_CELL   6     /*!< Release RLC entities per Cell */

/* CKW RLC entity direction configuration */
#define UDX_CFG_DIR_UL        1     /*!< Unlink direction */
#define UDX_CFG_DIR_DL        2     /*!< Downlink direction */
#define UDX_CFG_DIR_BOTH      3     /*!< Both Downlink and Unlink */

/* CKW Configuration confirmations */
#define UDX_CFG_CFM_OK        1     /*!< Configuration confirmation success */
#define UDX_CFG_CFM_NOK       2     /*!< Configuration confirmation failed */

#if 0
/* CKW Configuration Failure Reasons */
#define CKW_CFG_REAS_NONE              0  /*!< No Failure */
#define CKW_CFG_REAS_RB_CREAT_FAIL     1  /*!< RB CB creation Failure */
#define CKW_CFG_REAS_UE_CREAT_FAIL     2  /*!< UE CB creation Failure */
#define CKW_CFG_REAS_CELL_CREAT_FAIL   3  /*!< CELL CB creation Failure */
#define CKW_CFG_REAS_RB_PRSNT          4  /*!< RB CB already present */
#define CKW_CFG_REAS_LCHTYPE_MIS       5  /*!< Logical channel type mismatch
                                           with mode */
#define CKW_CFG_REAS_RB_UNKWN          6  /*!< RB Cb unknown */
#define CKW_CFG_REAS_UE_UNKWN          7  /*!< UE Cb unknown */
#define CKW_CFG_REAS_CELL_UNKWN        8  /*!< Cell Cb unknown */
#define CKW_CFG_REAS_RB_DEL_FAIL       9  /*!< RB Cb Deletion Failure */
#define CKW_CFG_REAS_UE_DEL_FAIL      10  /*!< UE Cb Deletion Failure */
#define CKW_CFG_REAS_RB_REEST_FAIL    11  /*!< RB Cb Re establish Failure */
#define CKW_CFG_REAS_RB_MODE_MIS      12  /*!< RB Cb Mode mismatch */
#define CKW_CFG_REAS_REEST_FAIL       13  /*!< RB Cb Re-establishment Failed */
#define CKW_CFG_REAS_RECFG_FAIL       14  /*!< RB Cb Re-configuration Failed */
#define CKW_CFG_REAS_INVALID_CFG      15  /*!< Invalid Configuration Type */
#define CKW_CFG_REAS_SAME_UEID        16  /*!< UE Ids match in ChngUeId Request */
#define CKW_CFG_REAS_CELL_DEL_FAIL    17  /*!< Cell Cb Deletion Failure */
#define CKW_CFG_REAS_INVALID_LCHID    18  /*!< Invalid LChId */
#define CKW_CFG_REAS_INVALID_DIR      19  /*!< Invalid Direction */
#define CKW_CFG_REAS_UE_EXISTS        20  /*!< UE ID already exists */
#endif

/***********************************************************************
        Defines for  CKW Interface Events
 ***********************************************************************/
#define UDX_EVT_BND_REQ              0x01   /*!< Bind Request */
#define UDX_EVT_BND_CFM              0x02   /*!< Bind Confirm */
#define UDX_EVT_UBND_REQ             0x03   /*!< Unbind Request */
#define UDX_EVT_CFG_REQ              0x04   /*!< Config Request */
#define UDX_EVT_CFG_CFM              0x05   /*!< Config Confirm */
#define UDX_EVT_UEIDCHG_REQ          0x06   /*!< UE Id Change Request */
#define UDX_EVT_UEIDCHG_CFM          0x07   /*!< UE Id Change Confirm */
#define UDX_EVT_STA_UPD_REQ          0x08
#define UDX_EVT_STA_UPD_CFM          0x09
#define UDX_EVT_STA_PDU_REQ          0x0A
#define UDX_EVT_STA_PHBT_TMR_START   0x0B
#define UDX_EVT_DL_CLEANUP_MEM       0x10   /*!< To cleanup memory in DL inst */

/***********************************************************************
        Defines for Measurements 
 ***********************************************************************/
#define UDX_EVT_L2MEAS_REQ           0x11 
#define UDX_EVT_L2MEAS_SEND_REQ      0x12 
#define UDX_EVT_L2MEAS_STOP_REQ      0x13 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

  
/********************************************************************30**
         End of file:     kw_udx.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:23:18 2015
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
/main/1      ---      cp   1. initial release.
*********************************************************************91*/
