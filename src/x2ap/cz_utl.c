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

     Name:     LTE-X2AP Layer 
  
     Type:     C Source file
  
     Desc:     This file contains the common utility functions used across the
               X2AP layer.

     File:     cz_utl.c

     Sid:      tr_utl.c@@/main/2 - Tue Aug 30 18:36:17 2011

     Prg:       

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_pasn.h"       /* common per asn.1                */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "czt.h"           /* CZT interface defines           */
#include "lcz.h"           /* X2AP Layer management           */
#include "cz.h"            /* X2AP layer defines              */
#include "cz_err.h"        /* X2AP - error */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* common per asn.1                */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT interface structures        */
#include "lcz.x"           /* X2AP Layer management           */
#include "cz.x"            /* X2AP layer structures           */

/* local defines */



PRIVATE S16 czUtlFillCritDiagIe ARGS((CztEvnt    *ptr,
                                      CztCriticalityDiag *critIe,
                                      CztX2AP_PDU *pdu,
                                      CzIeInfo    *idlst,
                                      Bool         isErrInd));

PRIVATE Void czMapPasnErrToCauseIe ARGS((S16   errCode,
                                         U8   *causeType,
                                         U8   *causeVal));
PRIVATE CztEvnt* czUtlX2RsrcStsDecErr ARGS((CztX2AP_PDU  *pdu,
                                            U8           causeType,
                                            U8           causeVal));
PRIVATE CztEvnt* czUtlX2SetupReqDecErr ARGS((CztX2AP_PDU  *pdu,
                                             U8           causeType,
                                             U8           causeval));

PRIVATE CztEvnt* czUtlMobChgReqDecErr ARGS((CztX2AP_PDU  *pdu,
                                             U8           causeType,
                                             U8           causeval));

PRIVATE CztEvnt* czUtlCellActReqDecErr ARGS((CztX2AP_PDU  *pdu,
                                             U8           causeType,
                                             U8           causeval));

PUBLIC S16 czUtlFillTknBStr32 ARGS(
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val      /* value to be filled in */
));


PRIVATE CztEvnt* czUtlHoReqDecErr ARGS((CztX2AP_PDU   *pdu,
                                         U8      causeType,
                                         U8      causeVal));
PRIVATE S16 czUtlGetRsrcErrIdLst ARGS((CztX2AP_PDU   *pdu,
                                       U8            msgId,
                                       CzIeInfo      *ieId,
                                       CztUeInfo     *ueInfo));
PRIVATE S16 czUtlGetBmpErrIdLst ARGS((CztX2AP_PDU    *pdu,
                                       U8            msgId,
                                       CzIeInfo      *ieId,
                                       CztUeInfo     *ueInfo));

/** X2AP Procedure Code IE Values:

-# Handover Preparation = "0"
-# Handover Cancel = "1"
-# Load Indication = "2"
-# Error Indication = "3"
-# SN Status Transfer = "4"
-# UE Context Release = "5"
-# X2 Setup = "6"
-# Reset = "7"
-# eNB Configuration Update = "8"
-# Resource Status Reporting Initiation = "9" 
-# Resource Status Reporting = "10" 
-# Mobility Settings Change = "11"   
-# RLF Indication = "12"
-# Handover Report = "13"
-# Cell Activation Request = "14"
-# Private Message ="15"            

Type of Message M CHOICE:
-# Initiating Message = 0
-# Successful Outcome = 1
-# Unsuccessful Outcome = 2
*/

/** @defgroup utlmodule X2AP Utilities Module
 *  @{
 */

/** @var czX2MsgMap 
    This static table gives the mapping of X2AP message type's procedure code 
    and choice values to Trillium's X2AP message type representation.
    @note For a list of definitions of Trillium's X2AP message types refer
          to czt.h
 */
/*kworks fix : changed type S16 to U8*/
PUBLIC U8 czX2MsgMap[CZ_MAX_MSG_PROC_CODE][CZ_MAX_MSG_CHOICE_VAL] = 
{
  /******Message Type IE Value********************/       
  /* Message Procedure Code */  /**Message Type Choice*/   

  /* HO Preparation */
   {
   CZT_MSG_HO_REQ,           /* Initiating Message */
   CZT_MSG_HO_ACK,           /* Successful Outcome */ 
   CZT_MSG_HO_FAIL          /* Unsuccessful Outcome */  
   },

   /* HO Cancel */
   {
   CZT_MSG_HO_CANCEL,        /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */ 
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */  
   },

  /* Load Indication */
   {
   CZT_MSG_LOAD_IND,         /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */ 
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */  
   },

  /* Error Indication */
   {
   CZT_MSG_ERR_IND,          /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */ 
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */     
   },

  /* SN Status Transfer */
   {
   CZT_MSG_SN_STATUS_TRFR,  /* Initiating Message */
   CZT_MSG_UNKNOWN,         /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */   
   },

  /* UE Ctxt Release */
   {
   CZT_MSG_UE_CXT_RELEASE,   /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */   
   },

   /* X2 Setup */
   {
   CZT_MSG_X2_SETUP,        /* Initiating Message */
   CZT_MSG_X2_SETUP_RSP,    /* Successful Outcome */
   CZT_MSG_X2_SETUP_FAIL    /* Unsuccessful Outcome */   
   },

   /* Reset */
   {
   CZT_MSG_RESET,           /* Initiating Message */
   CZT_MSG_RESET_RSP,       /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */ 
   },

   /* Config Update */
   {
   CZT_MSG_CFG_UPD_REQ,     /* Initiating Message */
   CZT_MSG_CFG_UPD_ACK,     /* Successful Outcome */
   CZT_MSG_CFG_UPD_FAIL     /* Unsuccessful Outcome */
   },

   /* Resource Status Initiation */
   {
   CZT_MSG_RSRC_ST_REQ,     /* Initiating Message */
   CZT_MSG_RSRC_ST_RSP,     /* Successful Outcome */
   CZT_MSG_RSRC_ST_FAIL     /* Unsuccessful Outcome */
   },

   /* Resource Update Reporting */
   {
   CZT_MSG_RSRC_ST_UPD,     /* Initiating Message */
   CZT_MSG_UNKNOWN,         /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */
   },
   /* Private Message */
   {
   CZT_MSG_PRIV_MSG,        /* Initiating Message */
   CZT_MSG_UNKNOWN,         /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */
   },
   /* Mobility settings Change */
   {
    CZT_MSG_MOB_CHANGE_REQ,
    CZT_MSG_MOB_CHANGE_ACK,
    CZT_MSG_MOB_CHANGE_FAIL
   },
   /* RLF Indication */
   {
    CZT_MSG_RLF_INDICATION,
    CZT_MSG_UNKNOWN,         /* Successful Outcome */
    CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */
   },
   /* HO Report */
   {
    CZT_MSG_HO_REPORT,
    CZT_MSG_UNKNOWN,         /* Successful Outcome */
    CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */
   },
   /* Cell Activation */
   {
    CZT_MSG_CELL_ACTIVATION_REQ,
    CZT_MSG_CELL_ACTIVATION_RES,
    CZT_MSG_CELL_ACTIVATION_FAIL
   },
}; /*end czX2MsgMap */

/** @var czEvntMap 
    This static table gives the mapping of Trillium X2AP message type
    to Trillium's X2AP event. Trillium X2AP event is an input to different
    FSMs implemented for:
    -Global Procedure.
    -Basic Mobility Management Procedure.
    @note For a list of definitions of Trillium's X2AP message types refer
          to czt.h
 */

/*kworks fix: changed type S16 to U8*/
PUBLIC U8 czEvntMap[CZT_MSG_MAX] = 
{
   CZ_EVNT_HO_REQ,            /**< CZT_MSG_HO_REQ */
   CZ_EVNT_HO_REQ_ACK,        /**< CZT_MSG_HO_ACK */
   CZ_EVNT_HO_PREP_FAIL,      /**< CZT_MSG_HO_FAIL */
   CZ_EVNT_HO_CANCEL,         /**< CZT_MSG_HO_CANCEL */
   CZ_EVNT_SN_STATUS_TRFR,    /**< CZT_MSG_SN_STATUS_TRFR */
   CZ_EVNT_UE_CXT_RELEASE,    /**< CZT_MSG_UE_CXT_RELEASE */
   CZ_EVNT_LOAD_IND,          /**< CZT_MSG_LOAD_IND */ 
   CZ_EVNT_ERR_IND,           /**< CZT_MSG_ERR_IND */ 
   CZ_EVNT_SETUP_REQ,         /**< CZT_MSG_X2_SETUP */
   CZ_EVNT_SETUP_RSP,         /**< CZT_MSG_X2_SETUP_RSP */ 
   CZ_EVNT_SETUP_FAIL,        /**< CZT_MSG_X2_SETUP_FAIL */ 
   CZ_EVNT_RESET_REQ,         /**< CZT_MSG_RESET */
   CZ_EVNT_RESET_RSP,         /**< CZT_MSG_RESET_RSP */
   CZ_EVNT_ENB_CFG_UPD_REQ,   /**< CZT_MSG_CFG_UPD_REQ */
   CZ_EVNT_ENB_CFG_UPD_ACK,   /**< CZT_MSG_CFG_UPD_ACK */
   CZ_EVNT_ENB_CFG_UPD_FAIL,  /**< CZT_MSG_CFG_UPD_FAIL */
   CZ_EVNT_RES_STA_REQ,       /**< CZT_MSG_RSRC_ST_REQ */
   CZ_EVNT_RES_STA_RSP,       /**< CZT_MSG_RSRC_ST_RSP */
   CZ_EVNT_RES_STA_FAIL,      /**< CZT_MSG_RSRC_ST_FAIL */
   CZ_EVNT_RES_STA_UPDATE,    /**< CZT_MSG_RSRC_ST_UPD */
   CZ_EVNT_MOB_CHANGE_REQ,     /**< CZT_MSG_MOB_CHG_REQ */
   CZ_EVNT_MOB_CHANGE_ACK,     /**< CZT_MSG_MOB_CHG_ACK */
   CZ_EVNT_MOB_CHANGE_FAIL,    /**< CZT_MSG_MOB_CHG_FAIL */
   CZ_EVNT_RLF_INDICATION,     /**< CZT_MSG_RLF_INDICATION */
   CZ_EVNT_HO_REPORT,          /**< CZT_MSG_HO_REPORT */
   CZ_EVNT_CELL_ACTIVATION_REQ,/**< CZT_MSG_CELL_ACTIVATION_REQ */
   CZ_EVNT_CELL_ACTIVATION_RES,/**< CZT_MSG_CELL_ACTIVATION_RES */
   CZ_EVNT_CELL_ACTIVATION_FAIL,/**< CZT_MSG_CELL_ACTIVATION_FAIL */
   CZ_EVNT_PRIVATE            /**< CZ_PRIVATE */

}; /* end czEvntMap */

/** @var czX2ProcMap 
    This static table gives the mapping of X2AP procedure code to
    Trillium's X2AP Procedure categories. 
    Different X2AP Procedure categories are:
   -# CZ_GP for Global Procedure.
   -# CZ_BMP for Basic Mobility Management Procedure.
   -# CZ_ERR for Error Indication Procedure.
   -# CZ_PRIV for Private Message Procedure.
   -# CZ_PROC_UNKNOWN for Unknown Procedure.

    @note For a list of definitions of Trillium's X2AP message types refer
          to czt.h
 */

PUBLIC S16 czX2ProcMap[CZ_MAX_MSG_PROC_CODE] = 
{
   /* X2AP Procedure 
      Categories */   /* Message Procedure Code */

   CZ_BMP,            /**< HO Preparation */
   CZ_BMP,            /**< HO Cancel */
   CZ_GP,             /**< Load Indication */
   CZ_ERR,            /**< Error Indication */
   CZ_BMP,            /**< SN Status Transfer */
   CZ_BMP,            /**< UE Ctxt Release */
   CZ_GP,             /**< X2 Setup */
   CZ_GP,             /**< Reset */
   CZ_GP,             /**< Config Update */
   CZ_GP,             /**< Resource Status Initiation */
   CZ_GP,             /**< Resource Update Reporting */
   CZ_GP,              /**< Private Message */
   CZ_GP,             /**< Mobility Change Settings */
   CZ_GP,             /**< RLF Indication */
   CZ_GP,             /**< Handover Report */
   CZ_GP,             /**< CellActivation Request */
};


/** @var czGetProcCrit 
  The czGetProcCrit define criticality based on Procedure Code  */
PUBLIC U8 czGetProcCrit[CZ_MAX_MSG_PROC_CODE] =
{
   CztCriticalityrejectEnum,  /*< HO PREPARATION */
   CztCriticalityignoreEnum,  /*< HO CANCEL      */
   CztCriticalityignoreEnum,  /*< LOAD INDICATION */
   CztCriticalityignoreEnum,  /*< ERROR INDICATION */
   CztCriticalityignoreEnum,  /*< SN STATUS TRANSFER */
   CztCriticalityignoreEnum,  /*< UE CONTEXT RELEASE */
   CztCriticalityrejectEnum,  /*< X2 SETUP  */
   CztCriticalityrejectEnum,  /*< X2 RESET  */
   CztCriticalityrejectEnum,  /*< ENB CONFIG UPDATE  */
   CztCriticalityrejectEnum,  /*< RESOURCE STATUS REPORTING INITIATION  */
   CztCriticalityignoreEnum,  /*< RESOURCE STATUS REPORTING */
   CztCriticalityignoreEnum,   /*< PRIVATE MESSAGE */
   CztCriticalityignoreEnum,  /*< Mobility Setting Change */
   CztCriticalityignoreEnum,  /*< RLF Indication */
   CztCriticalityignoreEnum,  /*< Handover Report */
   CztCriticalityignoreEnum  /*< Cell Activation */
};

/** @var responseMsg 
  The responseMsg defines Response for error handling - 
  Unsuccessful msg or Error Indication */
 /* Fix for ccpu00117358 */
PUBLIC U8 responseMsg[CZT_MSG_MAX] = 
{
   CZT_MSG_HO_FAIL,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_X2_SETUP_FAIL,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_CFG_UPD_FAIL,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_RSRC_ST_FAIL,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_MOB_CHANGE_FAIL,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_CELL_ACTIVATION_FAIL,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN,
   CZT_MSG_UNKNOWN

};

/** @var czUtlGetElmSize 
    This defines size of elements.  */
/* changing the macro name */
PUBLIC U16 czUtlGetElmSize[][CZ_MAX_MSG_PROC_CODE] = 
{
   {
      /* Msg Id 0 - Cztid_handoverPrep */
      sizeof(CztProtIE_Field_HovrRqst_IEs),
      /* Msg Id 1 - Cztid_handoverCancel */
      sizeof(CztProtIE_Field_HovrCancel_IEs),
      /* Msg Id 2 - Cztid_loadInd */
      sizeof(CztProtIE_Field_LoadInform_IEs),
      /* Msg Id 3 - Cztid_errorInd */
      sizeof(CztProtIE_Field_ErrInd_IEs),
      /* Msg Id 4 - Cztid_snStatusTfr */
      sizeof(CztProtIE_Field_SNStatusTfr_IEs),
      /* Msg Id 5 - Cztid_uECntxtRls */
      sizeof(CztProtIE_Field_UECntxtRls_IEs),
      /* Msg Id 6 - Cztid_x2Setup */
      sizeof(CztProtIE_Field_X2SetupRqst_IEs),
      /* Msg Id 7 - Cztid_reset */
      sizeof(CztProtIE_Field_ResetRqst_IEs),
      /* Msg Id 8 - Cztid_eNBConfigUpd */
      sizeof(CztProtIE_Field_ENBConfigUpd_IEs),
      /* Msg Id 9 - Cztid_resourceStatusRprtngInitiation */
      sizeof(CztProtIE_Field_ResStatusRqst_IEs),
      /* Msg Id 10 - Cztid_resourceStatusRprtng */
      sizeof(CztProtIE_Field_ResStatusUpd_IEs),
      /* Msg Id 11 - Cztid_privateMsg */
      sizeof(CztPrivIE_Field_PrivMsg_IEs),
      /* Msg Id 12 - Cztid_mobilitySettingsChange */
      sizeof(CztProtIE_Field_MobilityChangeRqst_IEs),
      /* Msg Id 13 - Cztid_rLFInd */
      sizeof(CztProtIE_Field_RLFInd_IEs),
      /* Msg Id 14 - Cztid_handoverReport */
      sizeof(CztProtIE_Field_HovrReport_IEs),
      /* Msg Id 15 - Cztid_cellActvn */
      sizeof(CztProtIE_Field_CellActvnRqst_IEs)
   },

   {
      /* Msg Id 0 - Cztid_handoverPrep */
      sizeof(CztProtIE_Field_HovrRqstAckg_IEs),
      /* Msg Id 1 - Cztid_handoverCancel */
      0,
      /* Msg Id 2 - Cztid_loadInd */
      0,
      /* Msg Id 3 - Cztid_errorInd */
      0,
      /* Msg Id 4 - Cztid_snStatusTfr */
      0,
      /* Msg Id 5 - Cztid_uECntxtRls */
      0,
      /* Msg Id 6 - Cztid_x2Setup */
      sizeof(CztProtIE_Field_X2SetupResp_IEs),
      /* Msg Id 7 - Cztid_reset */
      sizeof(CztProtIE_Field_ResetResp_IEs),
      /* Msg Id 8 - Cztid_eNBConfigUpd */
      sizeof(CztProtIE_Field_ENBConfigUpdAckg_IEs),
      /* Msg Id 9 - Cztid_resourceStatusRprtngInitiation */
      sizeof(CztProtIE_Field_ResStatusResp_IEs),
      /* Msg Id 10 - Cztid_resourceStatusRprtng */
      0,
      /* Msg Id 11 - Cztid_privateMsg */
      0,
      /* Msg Id 12 - Cztid_mobilitySettingsChange */
      sizeof(CztProtIE_Field_MobilityChangeAckg_IEs),
      /* Msg Id 13 - Cztid_rLFInd */
      0,
      /* Msg Id 14 - Cztid_handoverReport */
      0,
      /* Msg Id 15 - Cztid_cellActvn */
      sizeof(CztProtIE_Field_CellActvnResp_IEs)
   },

   {
      /* Msg Id 0 - Cztid_handoverPrep */
      sizeof(CztProtIE_Field_HovrPrepFail_IEs),
      /* Msg Id 1 - Cztid_handoverCancel */
      0,
      /* Msg Id 2 - Cztid_loadInd */
      0,
      /* Msg Id 3 - Cztid_errorInd */
      0,
      /* Msg Id 4 - Cztid_snStatusTfr */
      0,
      /* Msg Id 5 - Cztid_uECntxtRls */
      0,
      /* Msg Id 6 - Cztid_x2Setup */
      sizeof(CztProtIE_Field_X2SetupFail_IEs),
      /* Msg Id 7 - Cztid_reset */
      0,
      /* Msg Id 8 - Cztid_eNBConfigUpd */
      sizeof(CztProtIE_Field_ENBConfigUpdFail_IEs),
      /* Msg Id 9 - Cztid_resourceStatusRprtngInitiation */
      sizeof(CztProtIE_Field_ResStatusFail_IEs),
      /* Msg Id 10 - Cztid_resourceStatusRprtng */
      0,
      /* Msg Id 11 - Cztid_privateMsg */
      0,
      /* Msg Id 12 - Cztid_mobilitySettingsChange */
      sizeof(CztProtIE_Field_MobilityChangeFail_IEs),
      /* Msg Id 13 - Cztid_rLFInd */
      0,
      /* Msg Id 14 - Cztid_handoverReport */
      0,
      /* Msg Id 15 - Cztid_cellActvn */
      sizeof(CztProtIE_Field_CellActvnFail_IEs)
   }
};

/**
 * @brief This function maps X2AP message to Trillium X2AP message using static
          map.
 *
 * @details It uses message choice value and message procedure code value as index
   to retrieve the Trillium X2AP message code value. It also retrieves the 
   Trillium mapping of procedure code based on different types of X2AP messages,
   i.e. Global procedure or Mobility Procedure.
 * 
 * @param[in] pdu The pointer to X2AP ASN PDU structure. 
 * @param[out] isResponse The pointer to boolean which takes value as follows:
 *                        - TRUE if the PDU is a response message.
 *                        - FALSE if the PDU is not a response message.
 * @param[out] procType The pointer to a integer which takes values as follows:
 * -# CZ_GP for Global Procedure.
 * -# CZ_BMP for Basic Mobility Management Procedure.
 * -# CZ_ERR for Error Indication Procedure.
 * -# CZ_PRIV for Private Message Procedure.
 * -# CZ_PROC_UNKNOWN for Unknown Procedure.
 *
 * @return 
    -# If valid X2AP message, the value of Trillium X2AP message type value.
    -# Else CZ_MSG_UNKNOWN.
*/
#ifdef ANSI
PUBLIC U8 czUtlGetMsgType
(
CztX2AP_PDU   *pdu,             
Bool          *isResponse,
S16            *procType
)
#else /* Else if ANSI not defined */
PUBLIC U8 czUtlGetMsgType(pdu, isResponse, procType)
CztX2AP_PDU   *pdu;                  
Bool          *isResponse;
S16            *procType;
#endif /* enfif  ANSI*/
{
   S16 czMsgVal = CZT_MSG_UNKNOWN;
   S16 msgChoice = -1;
   U8  procCode;

   CZ_TRC2(czUtlGetMsgType)
   procCode = 0;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if((pdu == NULLP) || (isResponse == NULLP) || (procType == NULLP))
      CZ_RETVALUE(RFAILED);
#endif /* end if */

   *isResponse = FALSE;
   *procType = CZ_PROC_UNKNOWN;

   if(pdu->choice.pres == TRUE)
   {
      msgChoice  = pdu->choice.val;
   }
   switch(msgChoice)
   {
      case X2AP_PDU_INITIATINGMSG:
      {
         if(TRUE == pdu->val.initiatingMsg.pres.pres)
         {
            /*kworks fix: type cast*/
            procCode = (U8) pdu->val.initiatingMsg.procedureCode.val;
            czMsgVal = czX2MsgMap[procCode][msgChoice];
            *isResponse = FALSE;
         }
         break;
      }
      case X2AP_PDU_SUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.successfulOutcome.pres.pres)
         {
            procCode = (U8) pdu->val.successfulOutcome.procedureCode.val;
            czMsgVal = czX2MsgMap[procCode][msgChoice];
            *isResponse = TRUE;
         }
         break;
      }
      case X2AP_PDU_UNSUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.unsuccessfulOutcome.pres.pres)
         {
            /*kworks fix : type cast*/
            procCode = (U8) pdu->val.unsuccessfulOutcome.procedureCode.val;
            czMsgVal = czX2MsgMap[procCode][msgChoice];
            *isResponse = TRUE;
         }
         break;
      }
      default:
         break;
   }

   /* Check if the procedure is global and mobility related */
   if(procCode < CZ_MAX_MSG_PROC_CODE)
      *procType = czX2ProcMap[procCode];

   CZ_RETVALUE(czMsgVal);

}

/**
 * @brief This function retrieves the Trillium mapping of procedure code
          based on different types of X2AP messages.
 *
 * @param[in] procCode The procedure code value from X2AP ASN PDU structure. 
 *
 * @return 
   -# CZ_GP for Global Procedure.
   -# CZ_BMP for Basic Mobility Management Procedure.
   -# CZ_ERR for Error Indication Procedure.
   -# CZ_PRIV for Private Message Procedure.
   -# CZ_PROC_UNKNOWN for Unknown Procedure.
*/


#ifdef ANSI
PUBLIC S16 czUtlGetProcTypeVal
(
S16   procCode
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlGetProcTypeVal(procCode)
S16   procCode;
#endif /* endif ANSI */
{
   S16 czProcType = CZ_PROC_UNKNOWN;

   CZ_TRC2(czUtlGetProcTypeVal)

   if(procCode >=0 && procCode < CZ_MAX_MSG_PROC_CODE)
      czProcType = czX2ProcMap[procCode];

   CZ_RETVALUE(czProcType);

}

/**
 * @brief This function records statistics for failure in outgoing mesages.
 *
 * @param[in] peer      The pointer to peer control block. 
 * @param[in] msgType   The Trillium representation of X2AP messages.
 *
 * @return 
 * -# ROK on success.
 * -# Suitable error code otherwise.
*/


#ifdef ANSI
PUBLIC S16 czUtlUpdOutErrSts
(
CzPeerCb   *peer,    
U16        msgType 
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlUpdOutErrSts(peer, msgType)
CzPeerCb   *peer;  
U16        msgType; 
#endif /* endif ANSI */
{
   /* local variables */
   CZ_TRC2(czUtlUpdOutErrSts)

   CZ_DBG_INFO((CZ_PRNT_BUF,"czUtlUpdOutErrSts: messageType(%u)",msgType));

   switch (msgType)
   {
      case CZT_MSG_HO_REQ:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.hovrRqst.errTx++;
          
         /* general statistics */
         czCb.genSts.pdu.hovrRqst.errTx++;
         break;
      }
      case CZT_MSG_HO_ACK:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.hovrReqAck.errTx++;

         /* general statistics */
         czCb.genSts.pdu.hovrReqAck.errTx++;
         break;
      }
      case CZT_MSG_HO_FAIL:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.hovrPrepFail.errTx++;

         /* general statistics */
         czCb.genSts.pdu.hovrPrepFail.errTx++;
         break;
      } 
      case CZT_MSG_HO_CANCEL:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.hovrCancel.errTx++;

         /* general statistics */
         czCb.genSts.pdu.hovrCancel.errTx++;
         break;
      }
      case CZT_MSG_SN_STATUS_TRFR:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.sNStatusTfr.errTx++;

         /* general statistics */
         czCb.genSts.pdu.sNStatusTfr.errTx++;
         break;
      }
      case CZT_MSG_UE_CXT_RELEASE:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.ueCntxtRls.errTx++;

         /* general statistics */
         czCb.genSts.pdu.ueCntxtRls.errTx++;
         break;
      } 
      case CZT_MSG_RESET:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.resetRqst.errTx++;

         /* general statistics */
         czCb.genSts.pdu.resetRqst.errTx++;
         break;
      }
      case CZT_MSG_RESET_RSP:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.resetResp.errTx++;
         /* general statistics */
         czCb.genSts.pdu.resetResp.errTx++;
         break;
      }
      case CZT_MSG_X2_SETUP:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.x2SetupRqst.errTx++;

         /* general statistics */
         czCb.genSts.pdu.x2SetupRqst.errTx++;
         break;
      }
      case CZT_MSG_X2_SETUP_RSP:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.x2SetupResp.errTx++;
         /* general statistics */
         czCb.genSts.pdu.x2SetupResp.errTx++;
         break;
      }
      case CZT_MSG_X2_SETUP_FAIL:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.x2SetupFail.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.x2SetupFail.errTx++;
         break;
      }
      case CZT_MSG_LOAD_IND:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.loadInform.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.loadInform.errTx++;
         break;
      }
      case CZT_MSG_CFG_UPD_REQ:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdRqst.errTx++;
        
         /* general statistics */
         czCb.genSts.pdu.eNBCfgUpdRqst.errTx++;
         break;
      }
      case CZT_MSG_CFG_UPD_ACK:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdAck.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.eNBCfgUpdAck.errTx++;
         break;
      }
      case CZT_MSG_CFG_UPD_FAIL:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdFail.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.eNBCfgUpdFail.errTx++;
         break;
      }
      case CZT_MSG_RSRC_ST_REQ:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.resStatusRqst.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.resStatusRqst.errTx++;
         break;
      }
      case CZT_MSG_RSRC_ST_RSP:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.resStatusResp.errTx++;
        
         /* general statistics */
         czCb.genSts.pdu.resStatusResp.errTx++;
         break;
      }
      case CZT_MSG_RSRC_ST_FAIL:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.resStatusFail.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.resStatusFail.errTx++;
         break;
      }
      case CZT_MSG_RSRC_ST_UPD:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.pdu.resStatusUpd.errTx++;
       
         /* general statistics */
         czCb.genSts.pdu.resStatusUpd.errTx++;
         break;
      }
      case CZT_MSG_ERR_ENC_DEC:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.nmbEncErr++;
         break;
      }
     case CZT_MSG_MOB_CHANGE_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeReq.errTx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeReq.errTx++;
      break;
     case CZT_MSG_MOB_CHANGE_ACK:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeAck.errTx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeAck.errTx++;
      break;
     case CZT_MSG_MOB_CHANGE_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeFail.errTx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeFail.errTx++;
      break;
      case CZT_MSG_RLF_INDICATION:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.rlfIndication.errTx++;

       /* general statistics */
          czCb.genSts.pdu.rlfIndication.errTx++;
       break;
       case CZT_MSG_HO_REPORT:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hoReport.errTx++;


       /* general statistics */
          czCb.genSts.pdu.hoReport.errTx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvReq.errTx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvReq.errTx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_RES:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvRes.errTx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvRes.errTx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvFail.errTx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvFail.errTx++;
        break;

      default:
      {
         /* peer statistics */
         peer->peerCfg.peerSts.cfm.nmbUnexpEvt++;
         break;
      }
   }

   CZ_RETVALUE(ROK);
} 

/**
*     @brief This function records statistics for failure in the incoming
*            messages.
*
*     @param[in] peer      The pointer to peer control block.
*     @param[in] msgType   The Trillium representation of X2AP messages.
*
*     @return 
*        -# ROK on success.
*        -# Suitable error code otherwise.
*/
#ifdef ANSI
PUBLIC S16 czUtlUpdInErrSts
(
CzPeerCb   *peer,    
U16        msgType 
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlUpdInErrSts(peer, msgType)
CzPeerCb   *peer;  
U16        msgType; 
#endif /*endif ANSI */
{
   /* local variables */
   CZ_TRC2(czUtlUpdInErrSts);

   CZ_DBG_INFO((CZ_PRNT_BUF,"czUtlUpdInErrSts: messageType(%u)",msgType));
   switch (msgType)
   {
     case CZT_MSG_HO_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrRqst.errRx++;
          
       /* general statistics */
          czCb.genSts.pdu.hovrRqst.errRx++;
       break;

     case CZT_MSG_HO_ACK:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrReqAck.errRx++;

       /* general statistics */
          czCb.genSts.pdu.hovrReqAck.errRx++;
       break;

     case CZT_MSG_HO_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrPrepFail.errRx++;

       /* general statistics */
          czCb.genSts.pdu.hovrPrepFail.errRx++;
       break;

     case CZT_MSG_HO_CANCEL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrCancel.errRx++;

       /* general statistics */
          czCb.genSts.pdu.hovrCancel.errRx++;
       break;

     case CZT_MSG_SN_STATUS_TRFR:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.sNStatusTfr.errRx++;

       /* general statistics */
          czCb.genSts.pdu.sNStatusTfr.errRx++;
       break;

     case CZT_MSG_UE_CXT_RELEASE:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.ueCntxtRls.errRx++;

       /* general statistics */
          czCb.genSts.pdu.ueCntxtRls.errRx++;
       break;

     case CZT_MSG_RESET:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resetRqst.errRx++;

       /* general statistics */
          czCb.genSts.pdu.resetRqst.errRx++;
            break;

     case CZT_MSG_RESET_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resetResp.errRx++;
       /* general statistics */
          czCb.genSts.pdu.resetResp.errRx++;
       break;

     case CZT_MSG_X2_SETUP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupRqst.errRx++;

       /* general statistics */
          czCb.genSts.pdu.x2SetupRqst.errRx++;
       break;

     case CZT_MSG_X2_SETUP_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupResp.errRx++;
       /* general statistics */
          czCb.genSts.pdu.x2SetupResp.errRx++;
       break;

     case CZT_MSG_X2_SETUP_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupFail.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.x2SetupFail.errRx++;
       break;

     case CZT_MSG_LOAD_IND:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.loadInform.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.loadInform.errRx++;
       break;

     case CZT_MSG_CFG_UPD_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdRqst.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdRqst.errRx++;
       break;
     case CZT_MSG_CFG_UPD_ACK:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdAck.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdAck.errRx++;
       break;

     case CZT_MSG_CFG_UPD_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdFail.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdFail.errRx++;
       break;
     case CZT_MSG_RSRC_ST_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusRqst.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusRqst.errRx++;
       break;

     case CZT_MSG_RSRC_ST_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusResp.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusResp.errRx++;
       break;
     case CZT_MSG_RSRC_ST_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusFail.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusFail.errRx++;
       break;

     case CZT_MSG_RSRC_ST_UPD:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusUpd.errRx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusUpd.errRx++;
       break;
      case CZT_MSG_ERR_ENC_DEC:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.nmbEncErr++;
       break;
     case CZT_MSG_MOB_CHANGE_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeReq.errRx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeReq.errRx++;
      break;
     case CZT_MSG_MOB_CHANGE_ACK:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeAck.errRx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeAck.errRx++;
      break;
     case CZT_MSG_MOB_CHANGE_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeFail.errRx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeFail.errRx++;
      break;
      case CZT_MSG_RLF_INDICATION:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.rlfIndication.errRx++;

       /* general statistics */
          czCb.genSts.pdu.rlfIndication.errRx++;
       break;
       case CZT_MSG_HO_REPORT:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hoReport.errRx++;

        
       /* general statistics */
          czCb.genSts.pdu.hoReport.errRx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvReq.errRx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvReq.errRx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_RES:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvRes.errRx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvRes.errRx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvFail.errRx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvFail.errRx++;
        break;

     default:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.nmbUnexpEvt++;
       
       break;
   }

   CZ_RETVALUE(ROK);
} 

/**
 * @brief This function Records statistics for outgoing mesages
 *
 * @param[in] peer      The pointer to peer control block. 
 * @param[in] msgType   The Trillium representation of X2AP messages.
 *
 * @return 
 * -# ROK on success.
 * -# Suitable error code otherwise.
*/


#ifdef ANSI
PUBLIC S16 czUtlUpdOutSts
(
CzPeerCb   *peer,    
U16        msgType 
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlUpdOutSts(peer, msgType)
CzPeerCb   *peer;  
U16        msgType; 
#endif /* endif ANSI */
{
   /* local variables */
   CZ_TRC2(czUtlUpdOutSts);

   CZ_DBG_INFO((CZ_PRNT_BUF,"czUtlUpdOutSts: messageType(%u)",msgType));
   switch (msgType)
   {
     case CZT_MSG_HO_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrRqst.tx++;
          
       /* general statistics */
          czCb.genSts.pdu.hovrRqst.tx++;
       break;

     case CZT_MSG_HO_ACK:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrReqAck.tx++;

       /* general statistics */
          czCb.genSts.pdu.hovrReqAck.tx++;
       break;

     case CZT_MSG_HO_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrPrepFail.tx++;

       /* general statistics */
          czCb.genSts.pdu.hovrPrepFail.tx++;
       break;

     case CZT_MSG_HO_CANCEL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrCancel.tx++;

       /* general statistics */
          czCb.genSts.pdu.hovrCancel.tx++;
       break;

     case CZT_MSG_SN_STATUS_TRFR:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.sNStatusTfr.tx++;

       /* general statistics */
          czCb.genSts.pdu.sNStatusTfr.tx++;
       break;

     case CZT_MSG_UE_CXT_RELEASE:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.ueCntxtRls.tx++;

       /* general statistics */
          czCb.genSts.pdu.ueCntxtRls.tx++;
       break;

     case CZT_MSG_RESET:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resetRqst.tx++;

       /* general statistics */
          czCb.genSts.pdu.resetRqst.tx++;
            break;

     case CZT_MSG_RESET_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resetResp.tx++;
       /* general statistics */
          czCb.genSts.pdu.resetResp.tx++;
       break;

     case CZT_MSG_X2_SETUP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupRqst.tx++;

       /* general statistics */
          czCb.genSts.pdu.x2SetupRqst.tx++;
       break;

     case CZT_MSG_X2_SETUP_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupResp.tx++;
       /* general statistics */
          czCb.genSts.pdu.x2SetupResp.tx++;
       break;

     case CZT_MSG_X2_SETUP_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupFail.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.x2SetupFail.tx++;
       break;

     case CZT_MSG_LOAD_IND:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.loadInform.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.loadInform.tx++;
       break;

     case CZT_MSG_CFG_UPD_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdRqst.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdRqst.tx++;
       break;
     case CZT_MSG_CFG_UPD_ACK:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdAck.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdAck.tx++;
       break;

     case CZT_MSG_CFG_UPD_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdFail.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdFail.tx++;
       break;
     case CZT_MSG_RSRC_ST_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusRqst.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusRqst.tx++;
       break;

     case CZT_MSG_RSRC_ST_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusResp.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusResp.tx++;
       break;
     case CZT_MSG_RSRC_ST_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusFail.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusFail.tx++;
       break;

     case CZT_MSG_RSRC_ST_UPD:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusUpd.tx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusUpd.tx++;
       break;
     case CZT_MSG_MOB_CHANGE_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeReq.tx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeReq.tx++;
      break;
     case CZT_MSG_MOB_CHANGE_ACK:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeAck.tx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeAck.tx++;
      break;
     case CZT_MSG_MOB_CHANGE_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeFail.tx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeFail.tx++;
      break;
      case CZT_MSG_RLF_INDICATION:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.rlfIndication.tx++;

       /* general statistics */
          czCb.genSts.pdu.rlfIndication.tx++;
       break;
       case CZT_MSG_HO_REPORT:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hoReport.tx++;


       /* general statistics */
          czCb.genSts.pdu.hoReport.tx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvReq.tx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvReq.tx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_RES:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvRes.tx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvRes.tx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvFail.tx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvFail.tx++;
        break;

     default:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.nmbUnexpEvt++;
       break;
   }
   CZ_RETVALUE(ROK);
} 


/**
 * @brief This function Records statistics for Incoming  mesages
 *
 * @param[in] peer      The pointer to peer control block. 
 * @param[in] msgType   The Trillium representation of X2AP messages.
 *
 * @return 
 * -# ROK on success.
 * -# Suitable error code otherwise.
*/
#ifdef ANSI
PUBLIC S16 czUtlUpdInSts
(
CzPeerCb   *peer,    
U16        msgType 
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlUpdInSts(peer, msgType)
CzPeerCb   *peer;  
U16        msgType; 
#endif /* endif ANSI */
{
   /* local variables */
   CZ_TRC2(czUtlUpdInSts);

   CZ_DBG_INFO((CZ_PRNT_BUF,"czUtlUpdOutInSts: messageType(%u)",msgType));
   switch (msgType)
   {
     case CZT_MSG_HO_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrRqst.rx++;
          
       /* general statistics */
          czCb.genSts.pdu.hovrRqst.rx++;
       break;

     case CZT_MSG_HO_ACK:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrReqAck.rx++;

       /* general statistics */
          czCb.genSts.pdu.hovrReqAck.rx++;
       break;

     case CZT_MSG_HO_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrPrepFail.rx++;

       /* general statistics */
          czCb.genSts.pdu.hovrPrepFail.rx++;
       break;

     case CZT_MSG_HO_CANCEL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hovrCancel.rx++;

       /* general statistics */
          czCb.genSts.pdu.hovrCancel.rx++;
       break;

     case CZT_MSG_SN_STATUS_TRFR:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.sNStatusTfr.rx++;

       /* general statistics */
          czCb.genSts.pdu.sNStatusTfr.rx++;
       break;

     case CZT_MSG_UE_CXT_RELEASE:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.ueCntxtRls.rx++;

       /* general statistics */
          czCb.genSts.pdu.ueCntxtRls.rx++;
       break;

     case CZT_MSG_RESET:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resetRqst.rx++;

       /* general statistics */
          czCb.genSts.pdu.resetRqst.rx++;
            break;

     case CZT_MSG_RESET_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resetResp.rx++;
       /* general statistics */
          czCb.genSts.pdu.resetResp.rx++;
       break;

     case CZT_MSG_X2_SETUP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupRqst.rx++;

       /* general statistics */
          czCb.genSts.pdu.x2SetupRqst.rx++;
       break;

     case CZT_MSG_X2_SETUP_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupResp.rx++;
       /* general statistics */
          czCb.genSts.pdu.x2SetupResp.rx++;
       break;

     case CZT_MSG_X2_SETUP_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.x2SetupFail.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.x2SetupFail.rx++;
       break;

     case CZT_MSG_LOAD_IND:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.loadInform.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.loadInform.rx++;
       break;

     case CZT_MSG_CFG_UPD_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdRqst.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdRqst.rx++;
       break;
     case CZT_MSG_CFG_UPD_ACK:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdAck.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdAck.rx++;
       break;

     case CZT_MSG_CFG_UPD_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.eNBCfgUpdFail.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.eNBCfgUpdFail.rx++;
       break;
     case CZT_MSG_RSRC_ST_REQ:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusRqst.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusRqst.rx++;
       break;

     case CZT_MSG_RSRC_ST_RSP:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusResp.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusResp.rx++;
       break;
     case CZT_MSG_RSRC_ST_FAIL:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusFail.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusFail.rx++;
       break;

     case CZT_MSG_RSRC_ST_UPD:
       /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.resStatusUpd.rx++;
       
       /* general statistics */
          czCb.genSts.pdu.resStatusUpd.rx++;
       break;
     case CZT_MSG_MOB_CHANGE_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeReq.rx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeReq.rx++;
      break;
     case CZT_MSG_MOB_CHANGE_ACK:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeAck.rx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeAck.rx++;
      break;
     case CZT_MSG_MOB_CHANGE_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.mobChangeFail.rx++;

       /* general statistics */
          czCb.genSts.pdu.mobChangeFail.rx++;
      break;
      case CZT_MSG_RLF_INDICATION:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.rlfIndication.rx++;

       /* general statistics */
          czCb.genSts.pdu.rlfIndication.rx++;
       break;
       case CZT_MSG_HO_REPORT:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.hoReport.rx++;

       /* general statistics */
          czCb.genSts.pdu.hoReport.rx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_REQ:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvReq.rx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvReq.rx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_RES:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvRes.rx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvRes.rx++;
        break;
        case CZT_MSG_CELL_ACTIVATION_FAIL:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.pdu.cellActvFail.rx++;

       /* general statistics */
          czCb.genSts.pdu.cellActvFail.rx++;
        break;

     default:
        /* peer statistics */
          peer->peerCfg.peerSts.cfm.nmbUnexpEvt++;
       
       break;
   }

   CZ_RETVALUE(ROK);
} 

/**
    @brief The function is used for getting the IE.
          
    @details This function return the IE from X2AP PDU based on IE ID.

    @param[in] pdu  
    @param[in] id       
    @param[in] ie 
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czUtlGetIE
(
CztX2AP_PDU *pdu,
U32         id,
TknU8       **ie
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlGetIE (pdu, id, ie)
CztX2AP_PDU *pdu;
U32         id;
TknU8       **ie;
#endif /* endif ANSI */
{
   TknU8   *currStr;    /* Start definition */
   TknU8   *startStr;   /* Start event structure */
   U8      choiceType;
   U16     numComp;
   U16     recIdx;
   U32     recId;
   U32     procCode;
   U16     size;

   CZ_TRC2(czUtlGetIE)
         
   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie = NULLP;
   
   numComp = 0;
   recIdx = 0; 

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      CZ_RETVALUE(ROK);
   }
   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case X2AP_PDU_INITIATINGMSG:
      {
         if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
         {
            CZ_RETVALUE(ROK);
         }
         startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
         procCode  = pdu->val.initiatingMsg.procedureCode.val;
         break;
      }
      case X2AP_PDU_SUCCESSFULOUTCOME:
      {
         if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
         { 
            CZ_RETVALUE(ROK);
         }
         startStr  = (TknU8 *)(&(pdu->val.successfulOutcome.value.u));
         procCode  = pdu->val.successfulOutcome.procedureCode.val;
         break;
      } 
      default: 
      {
         if ((pdu->val.unsuccessfulOutcome.pres.pres != PRSNT_NODEF) ||
            (choiceType != X2AP_PDU_UNSUCCESSFULOUTCOME))
         {
            CZ_RETVALUE(ROK);
         }

         startStr  = (TknU8 *)(&(pdu->val.unsuccessfulOutcome.value.u));
         procCode  = pdu->val.unsuccessfulOutcome.procedureCode.val;
         break;
      }
   }

   if (((TknU8 *)startStr)->pres == PRSNT_NODEF)
   {
      /* Increment it by TknU8 for TknPres */
      currStr  = (TknU8 *) ((PTR)startStr + sizeof(TknU8));
   }
   else
   {
      CZ_RETVALUE(ROK);
   }
    
   if (((TknU16 *)(currStr))->pres == PRSNT_NODEF)
   {
      /* get the Number of IEs received : noComp */
      numComp = ((TknU16 *)(currStr))->val;
   }
        
   /*  Increment currStr by TknU16 for skipping noComp */
   currStr = (TknU8 *)((PTR)currStr + sizeof(TknU16));

   currStr = *(TknU8**)currStr;

   while (recIdx < numComp) 
   {
      startStr = currStr;

      if (((TknPres *)currStr)->pres == PRSNT_NODEF)
      {   
         /* Increment it by TknU8 for TknPres */
         currStr  = (TknU8 *) ((PTR)currStr + sizeof(TknPres));

         /* Get the received Id  */
         recId  = ((CztProtIE_ID *)currStr)->val; 

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(CztProtIE_ID) + 
                                                 sizeof(CztCriticality));
            *ie = startStr;
            break;
         }
      }
      size = czUtlGetElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   CZ_RETVALUE(ROK);
} /* end of czUtlGetIE */


/**
 * @brief This function is used to get the procCode and message Type from the
 * X2AP Pdu
 *
 * @details It uses Message choice value and message procedure code value as 
    index to retrieve the X2AP message type and procCode.
 * 
 * @param[in] pdu The pointer to X2AP ASN PDU structure. 
 * @param[in] procCode The pointer to X2AP ASN PDU structure. 
 * @param[in] msgType The pointer to X2AP ASN PDU structure. 
 * @param[in] criticality The pointer to X2AP ASN PDU structure. 
 * @param[in] msgId The pointer to X2AP ASN PDU structure. 
 * 
 * @return 
    -# If valid X2AP message, the value of Trillium X2AP message type value.
    -# Else CZ_MSG_UNKNOWN.
*/
#ifdef ANSI
PUBLIC S16 czUtlGetProcMsgCrit
(
CztX2AP_PDU  *pdu,
U8           *procCode,
U8           *msgType,
U8           *criticality,
U8           *msgId
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlGetProcMsgCrit(pdu,procCode,msgType,criticality, msgId)
CztX2AP_PDU  *pdu;                  
U8           *procCode;
U8           *msgType;
U8           *criticality;
U8           *msgId;
#endif /* endif ANSI */
{

   CZ_TRC2(czUtlGetProcMsgCrit)
   
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(pdu == NULLP)
   {
      CZ_RETVALUE(RFAILED);
   }
#endif /* end if */

   if(pdu->choice.pres == TRUE)
   {
      *msgType  = pdu->choice.val;
   }
   else
   {
      CZ_RETVALUE(RFAILED);
   }
   switch(*msgType)
   {
      case X2AP_PDU_INITIATINGMSG:
      {
         if(TRUE == pdu->val.initiatingMsg.pres.pres)
         {
            /*kworks fix: type cast*/
            *procCode = (U8)pdu->val.initiatingMsg.procedureCode.val;
            *criticality = czGetProcCrit[*procCode];
            *msgId = czX2MsgMap[*procCode][*msgType];
         }
         else
         {
            CZ_RETVALUE(RFAILED);
         }
         break;
      }
      case X2AP_PDU_SUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.successfulOutcome.pres.pres)
         {
             /*kworks fix : type cast*/
            *procCode = (U8)pdu->val.successfulOutcome.procedureCode.val;
            *criticality = czGetProcCrit[*procCode];
            *msgId = czX2MsgMap[*procCode][*msgType];
         }
         else
         {
            CZ_RETVALUE(RFAILED);
         }
         break;
      }
      case X2AP_PDU_UNSUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.unsuccessfulOutcome.pres.pres)
         {
            /*kworks fix: type cast*/
            *procCode = (U8)pdu->val.unsuccessfulOutcome.procedureCode.val;
            *criticality = czGetProcCrit[*procCode];
            *msgId = czX2MsgMap[*procCode][*msgType];
         }
         else
         {
            CZ_RETVALUE(RFAILED);
         }
         break;
      }
      default:
      {
         CZ_RETVALUE(RFAILED);
      }
   }

   CZ_RETVALUE(ROK);

}


/**
    @brief The function is used to fill the Cause IE.
          
    @details This function handles the filling of the token for Cause IE.

    @param[out] causeIe  The causeIe is the pointer to the IE into which the
    details are filled into.
    @param[in] causeType The causeType is the cause type to be filled.   
    @param[in] causeVal The causeType is the cause value to be filled.   
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC Void czUtlFillCauseIe
(
CztCause *causeIe, 
U8        causeType,
U8        causeVal
)
#else /* Else if ANSI not defined */
PUBLIC Void czUtlFillCauseIe(causeIe, causeType, causeVal)
CztCause *causeIe; 
U8        causeType;
U8        causeVal;
#endif /* endif ANSI */
{
   CZ_TRC2(czUtlFillCauseIe);
   CZ_FILL_TKN(causeIe->choice,  causeType);
   switch(causeType)
   {
      case CAUSE_RADIONW:
         CZ_FILL_TKN(causeIe->val.radioNw,  causeVal);
         break;
      case CAUSE_TRANSPORT:
         CZ_FILL_TKN(causeIe->val.transport, causeVal);
         break;
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      case CZT_CAUSE_PROTOCOL:
         CZ_FILL_TKN(causeIe->val.protocol,  causeVal);
         break;
#else
      case CAUSE_PROTOCOL:
         CZ_FILL_TKN(causeIe->val.protocol,  causeVal);
         break;
#endif
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      case CZT_CAUSE_MISC:
         CZ_FILL_TKN(causeIe->val.misc,  causeVal);
         break;
#else
      case CAUSE_MISC:
         CZ_FILL_TKN(causeIe->val.misc,  causeVal);
         break;
#endif
      default:
         break;
   }
}


/**
    @brief The function is used get Error ID list from a PDU.
          
    @details This function handles finding the Error ID list from a PDU.
    PDU is the handover message to be looked into.    

    @param[in] pdu     The pdu is the PDU message to be looked into.
    @param[in] msgId   The msdId is the handover message ID.
    @param[out] ieId   The ieId is the pointer to the IE Info returned.
    @param[out] ueInfo unused
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czUtlGetBmpErrIdLst
(
CztX2AP_PDU   *pdu,
U8            msgId,
CzIeInfo      *ieId,
CztUeInfo     *ueInfo
)
#else /* Else if ANSI not defined */
PRIVATE S16 czUtlGetBmpErrIdLst( pdu, msgId, ieId, ueInfo)
CztX2AP_PDU   *pdu;
U8            msgId;
CzIeInfo      *ieId;
CztUeInfo     *ueInfo;
#endif /* endif ANSI */
{
   U8                cnt = 0;
   TknU8             *ieTkn = NULLP;
   CztUE_X2AP_ID     *x2ApUeId = NULLP;

   CZ_TRC2(czUtlGetBmpErrIdLst);
   
   czUtlGetIE(pdu,Cztid_Old_eNB_UE_X2AP_ID, &ieTkn);
   if(ieTkn == NULLP)
   {
      CZ_FILL_IE_ID(ieId[cnt],Cztid_Old_eNB_UE_X2AP_ID,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      CZ_RETVALUE(ROK);
   }
   x2ApUeId = (CztUE_X2AP_ID *)ieTkn;
   if((x2ApUeId->pres == PRSNT_NODEF) && (x2ApUeId->val <(U16) 4096))
   {
      /* ueInfo->oldUEX2APId = ((CztUE_X2AP_ID *)x2ApUeId)->val; */
   }
   else
   {
      CZ_FILL_IE_ID(ieId[cnt],Cztid_Old_eNB_UE_X2AP_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      cnt++;
   }      

   if((msgId != CZT_MSG_HO_FAIL) &&
      (msgId != CZT_MSG_HO_CANCEL))
   {
      czUtlGetIE(pdu,Cztid_New_eNB_UE_X2AP_ID, &ieTkn);
      if(ieTkn == NULLP)
      {
         CZ_FILL_IE_ID(ieId[cnt],Cztid_New_eNB_UE_X2AP_ID,
                CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
         CZ_RETVALUE(ROK);
      }
      x2ApUeId = (CztUE_X2AP_ID *)ieTkn;
      /*kworks fix: removed 0<=x2ApUeId->val check*/
      if((x2ApUeId->pres == PRSNT_NODEF) &&(x2ApUeId->val < 4096))
      {
         /* ueInfo->newUEX2APId = ((CztUE_X2AP_ID *)x2ApUeId)->val; */
      }
      else
      {
         CZ_FILL_IE_ID(ieId[cnt],Cztid_New_eNB_UE_X2AP_ID,
                CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
         cnt++;
      }      
   }
   else
   {
      czUtlGetIE(pdu, Cztid_Cause, &ieTkn);
      if(ieTkn == NULLP)
      {
         CZ_FILL_IE_ID(ieId[cnt], Cztid_Cause,
             CztCriticalityignoreEnum, CztTypOfErrmissingEnum);
      }
   } 
   CZ_RETVALUE(ROK);

}


/**
    @brief The function is used to get Error ID list from a PDU. 
          
    @details This function handles finding the Error ID list from a PDU.
    PDU is the resource management message to be looked into.

    @param[in]  pdu     The pdu is the PDU message to be looked into. 
    @param[in]  msgId   The msdId is the Resource Management message ID.
    @param[out] ieId    The ieId is the pointer to the IE Info returned.
    @param[out] ueInfo  unused
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czUtlGetRsrcErrIdLst
(
CztX2AP_PDU   *pdu,
U8            msgId,
CzIeInfo      *ieId,
CztUeInfo     *ueInfo
)
#else /* Else if ANSI not defined */
PRIVATE S16 czUtlGetRsrcErrIdLst(pdu, msgId, ieId, ueInfo)
CztX2AP_PDU   *pdu;
U8            msgId;
CzIeInfo      *ieId;
CztUeInfo     *ueInfo;
#endif /* endif ANSI */
{
   U8                cnt = 0;
   TknU8             *ieTkn = NULLP;
   CztMeasurement_ID *measId;

   CZ_TRC2(czUtlGetRsrcErrIdLst);

   czUtlGetIE(pdu, Cztid_ENB1_Measurement_ID, &ieTkn);
   if(ieTkn == NULLP)
   {
      CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB1_Measurement_ID,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      CZ_RETVALUE(ROK);
   }
   measId = (CztMeasurement_ID *)ieTkn;
   if((measId->pres != PRSNT_NODEF) || 
      (4095 < measId->val))
   {
      CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB1_Measurement_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      cnt++;
   }      
   czUtlGetIE(pdu, Cztid_ENB2_Measurement_ID, &ieTkn);
   if(ieTkn == NULLP)
   {
      CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB2_Measurement_ID,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      CZ_RETVALUE(ROK);
   }
   measId = (CztMeasurement_ID *)ieTkn;
   if((measId->pres != PRSNT_NODEF) || 
      (4095 < measId->val))
   {
      CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB2_Measurement_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      cnt++;
   }      
   if(msgId == CZT_MSG_RSRC_ST_FAIL)
   {
      czUtlGetIE(pdu, Cztid_Cause, &ieTkn);
      CZ_FILL_IE_ID(ieId[cnt], Cztid_Cause,
            CztCriticalityignoreEnum, CztTypOfErrmissingEnum);
   } 
   CZ_RETVALUE(ROK);
}


/**
    @brief The function is used to get Error ID list from a PDU.
          
    @details This function handles finding the Error ID list from a PDU.
    PDU is the resource management or UE handover message to be looked into.

    @param[in] pdu     The pdu is the PDU message to be looked into.
    @param[in] msgId   The msdId is the Resource Management message or UE
                       Handover ID.
    @param[out] ieId   The ieId is the pointer to the IE Info returned.
    @param[out] ueInfo unused.
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czUtlGetErrIdLst
(
CztX2AP_PDU        *pdu,
U8                 msgId,
CzIeInfo           *ieId,
CztUeInfo          *ueInfo
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlGetErrIdLst( pdu, msgId, ieId, ueInfo)
CztX2AP_PDU        *pdu;
U8                 msgId;
CzIeInfo           *ieId;
CztUeInfo          *ueInfo;
#endif /* endif ANSI */
{
   TknU8           *ieTkn;
   U8               cnt = 0;
   /* cz001.301: KlockWork warning fix*/
   CztECGI         *eNB1Ecgi;
   CztECGI         *eNB2Ecgi;

   CZ_TRC2(czUtlGetErrIdLst);
  
   switch(msgId)
   {
      case CZT_MSG_HO_ACK:
      case CZT_MSG_SN_STATUS_TRFR:
      case CZT_MSG_UE_CXT_RELEASE:
      case CZT_MSG_HO_FAIL:
      case CZT_MSG_HO_CANCEL:
      {
         czUtlGetBmpErrIdLst(pdu, msgId, ieId, ueInfo);
         break;
      }
      case CZT_MSG_LOAD_IND:
      case CZT_MSG_RESET_RSP:
      case CZT_MSG_CFG_UPD_ACK:
      case CZT_MSG_ERR_IND:
         break;
      case CZT_MSG_X2_SETUP_RSP:
      {
         czUtlGetIE(pdu, Cztid_GlobalENB_ID, &ieTkn); 
         if(ieTkn == NULLP)
         {
            CZ_FILL_IE_ID(ieId[cnt], Cztid_GlobalENB_ID,
                   CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
         }
         break;
      }
      case CZT_MSG_X2_SETUP_FAIL:
      case CZT_MSG_RESET:
      case CZT_MSG_CFG_UPD_FAIL:
      {
         czUtlGetIE(pdu, Cztid_Cause, &ieTkn);
         if(ieTkn == NULLP)
         {
            CZ_FILL_IE_ID(ieId[cnt], Cztid_Cause,
                   CztCriticalityignoreEnum, CztTypOfErrmissingEnum);
         }
         break;
      }
      case CZT_MSG_RSRC_ST_FAIL:
      case CZT_MSG_RSRC_ST_UPD:
      case CZT_MSG_RSRC_ST_RSP:
      {
         czUtlGetRsrcErrIdLst(pdu, msgId, ieId, ueInfo);
         break; 
      }
      case  CZT_MSG_MOB_CHANGE_REQ:
            break;
      case  CZT_MSG_MOB_CHANGE_ACK:
      case  CZT_MSG_MOB_CHANGE_FAIL:
      {
          czUtlGetIE(pdu, Cztid_ENB1_Cell_ID, &ieTkn);

          /* cz001.301:x2ap kwork fixes*/
          eNB1Ecgi = (CztECGI *)ieTkn;
          if((eNB1Ecgi == NULLP) || (eNB1Ecgi->pres.val != PRSNT_NODEF))
          {
              CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB1_Cell_ID,
                  CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
               cnt++;
          }

           czUtlGetIE(pdu, Cztid_ENB2_Cell_ID, &ieTkn);

           eNB2Ecgi = (CztECGI *)ieTkn;
           if((ieTkn == NULLP) || (eNB2Ecgi->pres.val != PRSNT_NODEF))
           {
               CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB2_Cell_ID,
                     CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
               cnt++;
           }
           
            czUtlGetIE(pdu, Cztid_ENB2_Proposed_Mobility_Params, &ieTkn);
            if(ieTkn == NULLP)
            {
                CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB2_Proposed_Mobility_Params ,
                     CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
                cnt++;
            }

            czUtlGetIE(pdu, Cztid_Cause, &ieTkn);
            if(ieTkn == NULLP)
            {
               CZ_FILL_IE_ID(ieId[cnt], Cztid_Cause,
                    CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
                cnt++;
            }
            break;
      }
      case  CZT_MSG_RLF_INDICATION:
      case  CZT_MSG_HO_REPORT:
      case  CZT_MSG_CELL_ACTIVATION_REQ:
            break;
      case  CZT_MSG_CELL_ACTIVATION_RES:
      {
            czUtlGetIE(pdu, Cztid_ActivatedCellLst, &ieTkn);
            if(ieTkn == NULLP)
            {
                CZ_FILL_IE_ID(ieId[0], Cztid_ActivatedCellLst,
                      CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
            }
            break;
      }
      case  CZT_MSG_CELL_ACTIVATION_FAIL:
      {
            czUtlGetIE(pdu, Cztid_Cause, &ieTkn);
            if(ieTkn == NULLP)
            {
                CZ_FILL_IE_ID(ieId[0], Cztid_Cause,
                      CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
            }
            break;
      }
      default:
           break;

   }
   CZ_RETVALUE(ROK);
      
}


/**
    @brief The function is used to fill Critical Diagnostics IE.
          
    @details This function handles filling of X2 Critical Diagnostics IE.
    It fills all the values and presence parameters of the token appropriately.

    @param[in] ptr        The ptr is the pointer to the Event which contains
                          The memCp in this is used for memory allocation.
    @param[out] critIe    The critIe is the pointer to the criticality
                          information is filled from the PDU.
    @param[in] pdu        The pdu is the PDU message where the criticality
                          diagnostics is extracted.
    @param[out] idlst     The idlst is the IE information list that
                          is to be filled from the PDU.
    @param[in] isErrInd   The isErrInd is a Boolen which indicate if the PDU
                          is for X2 Error Indication message.
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czUtlFillCritDiagIe
(
CztEvnt            *ptr,
CztCriticalityDiag *critIe, 
CztX2AP_PDU        *pdu,
CzIeInfo           *idlst,
Bool               isErrInd
)
#else /* Else if ANSI not defined */
PRIVATE S16 czUtlFillCritDiagIe(ptr, critIe, pdu, idlst, isErrInd)
CztEvnt            *ptr;
CztCriticalityDiag *critIe; 
CztX2AP_PDU        *pdu;
CzIeInfo           *idlst;
Bool               isErrInd;
#endif /* endif ANSI */
{
   U8  procCode;
   U8  msgType;
   U8  criticality;
   U8  msgId;
   U8  idx = 0;
   U8  numComp = 0;
   CzIeInfo  ieIdlst[CZ_MAX_IDX];

   CZ_TRC2(czUtlFillCritDiagIe);

   /* cz001.101 : Memory initialization */
   CZ_SET_ZERO((U8 *)ieIdlst, CZ_MAX_IDX * sizeof(CzIeInfo)); 
   CZ_FILL_TKN(critIe->pres, PRSNT_NODEF);
   if((TRUE == isErrInd) && (NULLP != pdu))
   {
      czUtlGetProcMsgCrit(pdu, &procCode, &msgType, &criticality,
                          &msgId);
      CZ_FILL_TKN(critIe->procedureCode, procCode);
      CZ_FILL_TKN(critIe->triggeringMsg, msgType);
      CZ_FILL_TKN(critIe->procedureCriticality, criticality);
      czUtlGetErrIdLst(pdu, msgId, ieIdlst, NULLP);
   }
   else 
   {
      /* Only the error prone IE is sent and expected here - No need to copy further
        set of IEs */
      if(idlst) 
         CZ_MEM_COPY((U8 *)ieIdlst, idlst, CZ_MAX_IDX * sizeof(CzIeInfo));
   }
   for (idx = 0; idx < CZ_MAX_IDX; idx++)
   {
      if(ieIdlst[idx].pres == TRUE)
      {
         numComp++;
      }
   }
   
   if (numComp == 0)
   {
      critIe->iEsCriticalityDiag.noComp.pres = NOTPRSNT;
      critIe->iEsCriticalityDiag.noComp.val = 0;
      critIe->iE_Extns.noComp.pres = NOTPRSNT;
      critIe->iE_Extns.noComp.val = 0;
            
      CZ_RETVALUE(ROK);
   }
   CZ_FILL_TKN(critIe->iEsCriticalityDiag.noComp, numComp);
   CZ_GET_MEM(ptr, numComp *sizeof(CztCriticalityDiag_IE_LstMember),  
                              (Ptr*)&(critIe->iEsCriticalityDiag.member));
   if(NULLP == critIe->iEsCriticalityDiag.member)
   {
      critIe->iEsCriticalityDiag.noComp.pres = NOTPRSNT;
      critIe->iEsCriticalityDiag.noComp.val = 0;
      critIe->iE_Extns.noComp.pres = NOTPRSNT;
      critIe->iE_Extns.noComp.val = 0;
      CZ_RETVALUE(ROK);
   }
   for(idx = 0; idx < numComp; idx++)
   {
     CZ_FILL_TKN(critIe->iEsCriticalityDiag.member[idx].pres, PRSNT_NODEF); 
     CZ_FILL_TKN(critIe->iEsCriticalityDiag.member[idx].iE_ID,
                                                ieIdlst[idx].ieId); 
     CZ_FILL_TKN(critIe->iEsCriticalityDiag.member[idx].iECriticality, 
                                                ieIdlst[idx].crit); 
     CZ_FILL_TKN(critIe->iEsCriticalityDiag.member[idx].typeOfErr, 
                                                ieIdlst[idx].typOfErr); 
     critIe->iEsCriticalityDiag.member[idx].iE_Extns.noComp.pres = NOTPRSNT;
     critIe->iEsCriticalityDiag.member[idx].iE_Extns.noComp.val = 0;
   }
   
   critIe->iE_Extns.noComp.pres = NOTPRSNT;
   critIe->iE_Extns.noComp.val = 0;

   CZ_RETVALUE(ROK);
}

/**
*     Details : This function creates the CztEvent for X2 Handover Cancel Message.
*               It allocates the memory and fills the values passed
*               as input parameters.
* 
*      @param[in] ueInfo      The ueInfo is the pointer to the UE information
*                             to be filled.
*      @param[in] causeType   The causeType is the cause type for cancellation
*                             to be filled
*      @param[in] causeVal    The causeType is the cause type for cancellation
*                             to be filled
*
*     Return : CztEvnt The pointer to the UI event created for X2 Handover
*              Cancel Message.
*
*     Notes : None
*
*     File  : tr_utl.c
*
**********************************************************/
#ifdef ANSI
PUBLIC CztEvnt* czUtlGenHoCancel
(
CztUeInfo   *ueInfo,
U8          causeType,
U8          causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenHoCancel(ueInfo, causeType, causeVal)
CztUeInfo   *ueInfo;
U8          causeType;
U8          causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU                   *hoCnclPdu = NULLP;
   CztProtIE_Cont_HovrCancel_IEs *contHocnl = NULLP;
   CztEvnt                       *uiEvnt    = NULLP;
   S16 numComp = 1;
   /* cz001.301: Removed unused variable. build warning fix */
   S16 cnt;
   Mem sMem;

   CZ_TRC2(czUtlGenHoCancel)
   /* Allocate the Event pointer */
   sMem.region = czCb.init.region;
   sMem.pool   = czCb.init.pool;
      
   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &sMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   hoCnclPdu = &(uiEvnt->pdu);

   CZ_FILL_TKN(hoCnclPdu->choice,  X2AP_PDU_INITIATINGMSG);
   CZ_FILL_TKN(hoCnclPdu->val.initiatingMsg.pres,PRSNT_NODEF);
   CZ_FILL_TKN(hoCnclPdu->val.initiatingMsg.procedureCode,
                                        Cztid_handoverCancel);
   CZ_FILL_TKN(hoCnclPdu->val.initiatingMsg.criticality,
                                    CztCriticalityignoreEnum);
   CZ_FILL_TKN(hoCnclPdu->val.initiatingMsg.value.u.cztHovrCancel.pres,
                                                 PRSNT_NODEF);
   contHocnl = 
        &(hoCnclPdu->val.initiatingMsg.value.u.cztHovrCancel.protocolIEs);

   if (CZ_UNKNOWN_UE_X2AP_ID != ueInfo->oldUEX2APId)
   {
      numComp++;
   }
   if(CZ_UNKNOWN_UE_X2AP_ID != ueInfo->newUEX2APId)
   {
      numComp++;
   }

   CZ_FILL_TKN(contHocnl->noComp,numComp);
   CZ_GET_MEM(uiEvnt, numComp *sizeof(CztProtIE_Field_HovrCancel_IEs),  
                                          (Ptr *)&(contHocnl->member));
   if(NULLP == contHocnl->member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }
   cnt = 0;
   
   CZ_FILL_TKN(contHocnl->member[cnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(contHocnl->member[cnt].id,Cztid_Old_eNB_UE_X2AP_ID);
   CZ_FILL_TKN(contHocnl->member[cnt].criticality,CztCriticalityrejectEnum);
   CZ_FILL_TKN(contHocnl->member[cnt].value.u.cztid_Old_eNB_UE_X2AP_ID /* Fix for ccpu00117267 */,
                                                      ueInfo->oldUEX2APId);

   cnt ++;

   if(CZ_UNKNOWN_UE_X2AP_ID != ueInfo->newUEX2APId)
   {
      CZ_FILL_TKN(contHocnl->member[cnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(contHocnl->member[cnt].id,Cztid_New_eNB_UE_X2AP_ID);
      CZ_FILL_TKN(contHocnl->member[cnt].criticality,CztCriticalityignoreEnum);
      CZ_FILL_TKN(contHocnl->member[cnt].value.u.cztid_New_eNB_UE_X2AP_ID /* Fix for ccpu00117267 */,
                                                         ueInfo->newUEX2APId);
      cnt++;
   }
      
   CZ_FILL_TKN(contHocnl->member[cnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(contHocnl->member[cnt].id,Cztid_Cause);
   CZ_FILL_TKN(contHocnl->member[cnt].criticality,CztCriticalityignoreEnum);
      
   czUtlFillCauseIe(&(contHocnl->member[cnt].value.u.cztCause), 
                                             causeType, causeVal);
      
   CZ_RETVALUE(uiEvnt);
}


/**
    @brief The function czUtlGenX2CfgUpdFailure is used for creating an Event for 
           X2 eNodeB Config Update Message.

    @details This function creates the CztEvent for X2 eNodeB Config Update
             Message. It allocates the memory and fills the values passed
             as input parameters.

    @param[out] pdu        The pdu is the pointer to the PDU of the message
                           to be filled.
    @param[in] causeType   The causeType is the cause type for failure
                           to be filled
    @param[in] causeVal    The causeType is the cause type for failure
                           to be filled

    @return
            CztEvnt The pointer to the UI event created for X2 eNodeB Config
            Update Message.
*/

#ifdef ANSI
PUBLIC CztEvnt* czUtlGenX2CfgUpdFailure
(
CztX2AP_PDU *pdu,
U8          causeType,
U8          causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenX2CfgUpdFailure(pdu, causeType, causeVal)
CztX2AP_PDU *pdu;
U8          causeType;
U8          causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU         *cfgFailPdu = NULLP;
   CztENBConfigUpdFail *failIe = NULLP;
   CztEvnt             *uiEvnt = NULLP;
   S16 numComp = 2;
   S16 czCnt;
   Mem czSMem;

   CZ_TRC2(czUtlGenX2CfgUpdFailure)

   /* Allocate the Event pointer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;
      
   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   cfgFailPdu = &(uiEvnt->pdu);
   
   CZ_FILL_TKN(cfgFailPdu->choice,  X2AP_PDU_UNSUCCESSFULOUTCOME);
   CZ_FILL_TKN(cfgFailPdu->val.unsuccessfulOutcome.pres,PRSNT_NODEF);
   CZ_FILL_TKN(cfgFailPdu->val.unsuccessfulOutcome.procedureCode,
                                                        Cztid_eNBConfigUpd);
   CZ_FILL_TKN(cfgFailPdu->val.unsuccessfulOutcome.criticality, 
                                              CztCriticalityrejectEnum);

   failIe = &(cfgFailPdu->val.unsuccessfulOutcome.value.u.cztENBConfigUpdFail);
   CZ_FILL_TKN(failIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt, numComp *sizeof(CztProtIE_Field_X2SetupFail_IEs), 
         (Ptr *)&(failIe->protocolIEs.member));
   if(NULLP == failIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }
   czCnt = 0;
   
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Cause);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                         CztCriticalityignoreEnum);
      
   czUtlFillCauseIe(&failIe->protocolIEs.member[czCnt].value.u.cztCause, 
                                             causeType, causeVal);
   czCnt++;
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_TimeToWait);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                             CztCriticalityignoreEnum);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztTimeToWait,
               czCb.nodeCb.protCfg.timeToWait);
     
   CZ_RETVALUE(uiEvnt);
}


/**
    @brief The function czUtlGenRsrcStsFail is used for creating an Event for 
           X2 Resource Status Failure Message.

    @details This function creates the CztEvent for X2 Resource Status Failure
             Message. It allocates the memory and fills the values passed
             as input parameters.

    @param[in] idLst       The ueInfo is the pointer to the IE list information
                           to be filled.
    @param[in] rsrcInfo    The ueInfo is the pointer to the resource information
                           to be filled.
    @param[in] causeType   The causeType is the cause type for failure
                           to be filled
    @param[in] causeVal    The causeType is the cause type for failure
                           to be filled

    @return
            CztEvnt The pointer to the UI event created for X2 Resource
            Status Failure Message.
*/

#ifdef ANSI
PUBLIC CztEvnt* czUtlGenRsrcStsFail
(
CzIeInfo    *idLst,
CzRsrcInfo  *rsrcInfo,
U8          causeType,
U8          causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenRsrcStsFail(idLst, rsrcInfo, causeType,causeVal)
CzIeInfo    *idLst;
CzRsrcInfo  *rsrcInfo;
U8          causeType;
U8          causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU         *rsrcFailPdu = NULLP;
   CztResStatusFail    *failIe = NULLP;
   CztEvnt             *uiEvnt = NULLP;
   S16 numComp = 3;
   S16 czCnt;
   Mem czSMem;

   CZ_TRC2(czUtlGenRsrcStsFail)

   /* Allocate the Event pointer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;
      
   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   rsrcFailPdu = &(uiEvnt->pdu);
   if(idLst != NULLP)
   {
      numComp++;
   }
   
   CZ_FILL_TKN(rsrcFailPdu->choice,  X2AP_PDU_UNSUCCESSFULOUTCOME);
   CZ_FILL_TKN(rsrcFailPdu->val.unsuccessfulOutcome.pres,PRSNT_NODEF);
   CZ_FILL_TKN(rsrcFailPdu->val.unsuccessfulOutcome.procedureCode,
                                        Cztid_resourceStatusRprtngInitiation);
   CZ_FILL_TKN(rsrcFailPdu->val.unsuccessfulOutcome.criticality, 
                                              CztCriticalityrejectEnum);

   failIe = &(rsrcFailPdu->val.unsuccessfulOutcome.value.u.cztResStatusFail);
   CZ_FILL_TKN(failIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt,numComp * sizeof(CztProtIE_Field_ResStatusFail_IEs), 
                                     (Ptr *)&(failIe->protocolIEs.member));
   if(NULLP == failIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }
   czCnt = 0;
   
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_ENB1_Measurement_ID);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                   CztCriticalityrejectEnum);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztid_ENB1_Measurement_ID,
                                                      rsrcInfo->eNb1measId);

   czCnt ++;

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_ENB2_Measurement_ID);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                CztCriticalityrejectEnum);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztid_ENB2_Measurement_ID,
                                                      rsrcInfo->eNb2measId);
      
   czCnt++;
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Cause);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                            CztCriticalityignoreEnum);
      
   czUtlFillCauseIe(&failIe->protocolIEs.member[czCnt].value.u.cztCause, 
                                             causeType, causeVal);

   czCnt++;
     
   if(idLst != NULLP)
   {
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_CriticalityDiag);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                     CztCriticalityignoreEnum);

      czUtlFillCritDiagIe(uiEvnt, 
               &(failIe->protocolIEs.member[czCnt].value.u.cztCriticalityDiag),
               NULLP, idLst, FALSE);
   }
      
   CZ_RETVALUE(uiEvnt);
}


/**
    @brief The function czUtlGenHoPrepFailure is used for creating an Event for 
           X2 Handover Preparation Failure Message.

    @details This function creates the CztEvent for X2 Handover Preparation
             Failure Message.
             It allocates the memory and fills the values passed
             as input parameters.

    @param[in] idLst       The ueInfo is the pointer to the IE list information
                           to be filled.
    @param[in] ueInfo      The ueInfo is the pointer to the UE information
                           to be filled.
    @param[in] causeType   The causeType is the cause type for failure
                           to be filled
    @param[in] causeVal    The causeType is the cause type for failure
                           to be filled

    @return
            CztEvnt The pointer to the UI event created for X2 Handover
            Preparation Failure Message.
*/

#ifdef ANSI
PUBLIC CztEvnt* czUtlGenHoPrepFailure
(
CzIeInfo    *idLst, 
CztUeInfo   *ueInfo,
U8          causeType,
U8          causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenHoPrepFailure(idLst, ueInfo, causeType, causeVal)
CzIeInfo    *idLst; 
CztUeInfo   *ueInfo;
U8          causeType;
U8          causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU         *hoPrepFailPdu = NULLP;
   CztHovrPrepFail     *failIe = NULLP;
   CztEvnt             *uiEvnt = NULLP;
   S16 numComp = 2;
   S16 czCnt;
   Mem czSMem;

   CZ_TRC2(czUtlGenHoPrepFailure)

   /* Allocate the Event pointer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;
      
   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   hoPrepFailPdu = &(uiEvnt->pdu);
   
   if (NULLP != idLst)
   {
      numComp++;
   }

   CZ_FILL_TKN(hoPrepFailPdu->choice,  X2AP_PDU_UNSUCCESSFULOUTCOME);
   CZ_FILL_TKN(hoPrepFailPdu->val.unsuccessfulOutcome.pres,PRSNT_NODEF);
   CZ_FILL_TKN(hoPrepFailPdu->val.unsuccessfulOutcome.procedureCode,
                                                    Cztid_handoverPrep);
   CZ_FILL_TKN(hoPrepFailPdu->val.unsuccessfulOutcome.criticality, 
                                              CztCriticalityrejectEnum);

   failIe = &(hoPrepFailPdu->val.unsuccessfulOutcome.value.u.cztHovrPrepFail);
   CZ_FILL_TKN(failIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt, numComp * sizeof(CztProtIE_Field_HovrPrepFail_IEs),
                                   (Ptr *)&(failIe->protocolIEs.member));
   if(NULLP == failIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }
   czCnt = 0;
   
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Old_eNB_UE_X2AP_ID);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                     CztCriticalityignoreEnum);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztUE_X2AP_ID,
                                                      ueInfo->oldUEX2APId);
   czCnt ++;

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Cause);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                 CztCriticalityignoreEnum);
      
   czUtlFillCauseIe(&failIe->protocolIEs.member[czCnt].value.u.cztCause, 
                                             causeType, causeVal);
   czCnt++;

   if(idLst)   
   {
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_CriticalityDiag);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                      CztCriticalityignoreEnum);

      czUtlFillCritDiagIe(uiEvnt, 
                &(failIe->protocolIEs.member[czCnt].value.u.cztCriticalityDiag),
                NULLP, idLst, FALSE);
   } 

   CZ_RETVALUE(uiEvnt);
}

/**
    @brief The function czUtlGenX2SetupFailure is used for creating an Event for 
           X2 Setup Failure Message.

    @details This function creates the CztEvent for X2 Setup Failure Message.
             It allocates the memory and fills the values passed
             as input parameters.

    @param[in] idLst       The ueInfo is the pointer to the IE list information
                           to be filled.
    @param[in] causeType   The causeType is the cause type for cancellation
                           to be filled
    @param[in] causeVal    The causeType is the cause type for cancellation
                           to be filled

    @return
            CztEvnt The pointer to the UI event created for X2 Setup
            Failure Message.
*/

#ifdef ANSI
PUBLIC CztEvnt* czUtlGenX2SetupFailure
(
CzIeInfo   *idLst,
U8         causeType,
U8         causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenX2SetupFailure(idLst, causeType, causeVal)
CzIeInfo  *idLst;
U8        causeType;
U8        causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU         *setupFailPdu = NULLP;
   CztX2SetupFail      *failIe = NULLP;
   CztEvnt             *uiEvnt = NULLP;
   S16 numComp = 2;
   S16 czCnt;
   Mem czSMem;

   CZ_TRC2(czUtlGenX2SetupFailure)

   /* AlloclIe->protocolIEs.member)ate the buffer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;
      
   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   setupFailPdu = &(uiEvnt->pdu);
   
   if(idLst != NULLP)
   {
      numComp++;
   }
   
   CZ_FILL_TKN(setupFailPdu->choice,  X2AP_PDU_UNSUCCESSFULOUTCOME);
   CZ_FILL_TKN(setupFailPdu->val.unsuccessfulOutcome.pres,PRSNT_NODEF);
   CZ_FILL_TKN(setupFailPdu->val.unsuccessfulOutcome.procedureCode,
                                                        Cztid_x2Setup);
   CZ_FILL_TKN(setupFailPdu->val.unsuccessfulOutcome.criticality, 
                                              CztCriticalityrejectEnum);

   failIe = &(setupFailPdu->val.unsuccessfulOutcome.value.u.cztX2SetupFail);
   CZ_FILL_TKN(failIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt, numComp *sizeof(CztProtIE_Field_X2SetupFail_IEs),
                                  (Ptr *)&(failIe->protocolIEs.member));
   if(NULLP == failIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }
   czCnt = 0;
   
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Cause);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                               CztCriticalityignoreEnum);
   
   czUtlFillCauseIe(&failIe->protocolIEs.member[czCnt].value.u.cztCause, 
                                             causeType, causeVal);
   czCnt++;
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_TimeToWait);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                 CztCriticalityignoreEnum);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztTimeToWait,
               czCb.nodeCb.protCfg.timeToWait);
     
   czCnt++;
     
   if(idLst != NULLP)
   {
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_CriticalityDiag);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                   CztCriticalityignoreEnum);

      czUtlFillCritDiagIe(uiEvnt, 
               &(failIe->protocolIEs.member[czCnt].value.u.cztCriticalityDiag),
               NULLP, idLst, FALSE);
   }
      
   CZ_RETVALUE(uiEvnt);
}

/**
     @brief   The function czUtlGenMobChgFailure is used for creating an Event
             for  Mobility Change Failure Message.

     @details This function creates the CztEvent for Mobility Change Failure Message.
               It allocates the memory and fills the values passed
               as input parameters.

     @param[in] idLst       The ueInfo is the pointer to the IE list information
                            to be filled.
     @param[in] eNB1CellEcgi
     @param[in] eNB2CellEcgi
     @param[in] causeType   The causeType is the cause type for cancellation
                            to be filled
     @param[in] causeVal    The causeType is the cause type for cancellation
                            to be filled

     @return 
          CztEvnt The pointer to the UI event created for Mobility Change
          Failure Message.
*/
#ifdef ANSI
PUBLIC CztEvnt* czUtlGenMobChgFailure
(
CzIeInfo   *idLst,
CztECGI    *eNB1CellEcgi,
CztECGI    *eNB2CellEcgi,
U8         causeType,
U8         causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenMobChgFailure(idLst, eNB1CellEcgi , eNB2CellEcgi, 
                                                      causeType, causeVal)
CzIeInfo  *idLst;
CztECGI   *eNB1CellEcgi;
CztECGI   *eNB2CellEcgi;
U8        causeType;
U8        causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU *mobChgFailPdu = NULLP;
   CztMobilityChangeFail *failIe = NULLP;
   CztEvnt  *uiEvnt = NULLP;
   S16 numComp = 3;
   S16 czCnt;
   Mem czSMem;
   /*CztProtIE_Field_MobilityChangeFail_IEs *ie;*/

   CZ_TRC2(czUtlGenMobChgFailure)

   /* AlloclIe->protocolIEs.member)ate the buffer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;

   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   mobChgFailPdu = &(uiEvnt->pdu);

   if(idLst != NULLP)
   {
      numComp++;
   }

   CZ_FILL_TKN(mobChgFailPdu->choice,  X2AP_PDU_UNSUCCESSFULOUTCOME);
   CZ_FILL_TKN(mobChgFailPdu->val.unsuccessfulOutcome.pres,PRSNT_NODEF);
   CZ_FILL_TKN(mobChgFailPdu->val.unsuccessfulOutcome.procedureCode,
                                          Cztid_mobilitySettingsChange);
   CZ_FILL_TKN(mobChgFailPdu->val.unsuccessfulOutcome.criticality,
                                              CztCriticalityrejectEnum);

   failIe = 
       &(mobChgFailPdu->val.unsuccessfulOutcome.value.u.cztMobilityChangeFail);
   CZ_FILL_TKN(failIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt, numComp *sizeof(CztProtIE_Field_MobilityChangeFail_IEs),
                                  (Ptr *)&(failIe->protocolIEs.member));
   if(NULLP == failIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }

   czCnt = 0;

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_ENB1_Cell_ID);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                               CztCriticalityignoreEnum);

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztid_ENB1_Cell_ID.pres,
                                               PRSNT_NODEF);

   CZ_FILL_TKN_STR_OSXL(
        (failIe->protocolIEs.member[czCnt].value.u.cztid_ENB1_Cell_ID.pLMN_Identity),
         eNB1CellEcgi->pLMN_Identity.len, eNB1CellEcgi->pLMN_Identity.val);

   czUtlFillTknBStr32(
  &(failIe->protocolIEs.member[czCnt].value.u.cztid_ENB1_Cell_ID.eUTRANcellIdentifier),
                                 (U16)eNB1CellEcgi->eUTRANcellIdentifier.len, 
                                 (U32)eNB1CellEcgi->eUTRANcellIdentifier.val );

   failIe->protocolIEs.member[czCnt].value.u.cztid_ENB1_Cell_ID.iE_Extns.noComp.pres =
                                                                      NOTPRSNT;

   czCnt++;

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_ENB2_Cell_ID);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                              CztCriticalityignoreEnum);

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].value.u.cztid_ENB2_Cell_ID.pres,
                                                             PRSNT_NODEF);

   CZ_FILL_TKN_STR_OSXL(
        (failIe->protocolIEs.member[czCnt].value.u.cztid_ENB2_Cell_ID.pLMN_Identity),
            eNB2CellEcgi->pLMN_Identity.len, eNB2CellEcgi->pLMN_Identity.val);

   czUtlFillTknBStr32(
  &(failIe->protocolIEs.member[czCnt].value.u.cztid_ENB2_Cell_ID.eUTRANcellIdentifier),
                                  (U16)eNB2CellEcgi->eUTRANcellIdentifier.len, 
                                 (U32)eNB2CellEcgi->eUTRANcellIdentifier.val );

   failIe->protocolIEs.member[czCnt].value.u.cztid_ENB2_Cell_ID.iE_Extns.noComp.pres =
                                                                      NOTPRSNT;

   czCnt++;

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Cause);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                     CztCriticalityignoreEnum);
   czUtlFillCauseIe(&failIe->protocolIEs.member[czCnt].value.u.cztCause,
                                                         causeType, causeVal);

   czCnt++;

   if(idLst != NULLP) 
   {
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_CriticalityDiag);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                     CztCriticalityignoreEnum);

      czUtlFillCritDiagIe(uiEvnt,
                &(failIe->protocolIEs.member[czCnt].value.u.cztCriticalityDiag),
                NULLP, idLst, FALSE);
   }

   CZ_RETVALUE(uiEvnt);
}




/**
     @brief  The function czUtlGenCellActFailure is used for creating an Event
             for  Cell Activation Failure Message.

     @details This function creates the CztEvent for Cell Activation Failure Message.
               It allocates the memory and fills the values passed
               as input parameters.

     @param[in] idLst       The ueInfo is the pointer to the IE list information
                            to be filled.
     @param[in] causeType   The causeType is the cause type for failure
                            to be filled
     @param[in] causeVal    The causeType is the cause type for failure
                            to be filled

     @return 
         CztEvnt The pointer to the UI event created for Cell Activation
         Failure Message.
*/
#ifdef ANSI
PUBLIC CztEvnt* czUtlGenCellActFailure
(
CzIeInfo   *idLst,
U8         causeType,
U8         causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenCellActFailure(idLst, causeType, causeVal)
CzIeInfo  *idLst;
U8        causeType;
U8        causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU *cellActFailPdu = NULLP;
   CztCellActvnFail *failIe = NULLP;
   CztEvnt  *uiEvnt = NULLP;
   S16 numComp = 1;
   S16 czCnt;
   Mem czSMem;

   CZ_TRC2(czUtlGenCellActFailure)

   /* AlloclIe->protocolIEs.member)ate the buffer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;

   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }

   cellActFailPdu = &(uiEvnt->pdu);

   if(idLst != NULLP)
   {
      numComp++;
   }

   CZ_FILL_TKN(cellActFailPdu->choice,  X2AP_PDU_UNSUCCESSFULOUTCOME);
   CZ_FILL_TKN(cellActFailPdu->val.unsuccessfulOutcome.pres,PRSNT_NODEF);
   CZ_FILL_TKN(cellActFailPdu->val.unsuccessfulOutcome.procedureCode,
                                                    Cztid_cellActvn);
   CZ_FILL_TKN(cellActFailPdu->val.unsuccessfulOutcome.criticality,
                                              CztCriticalityrejectEnum);

   failIe = &(cellActFailPdu->val.unsuccessfulOutcome.value.u.cztCellActvnFail);
   CZ_FILL_TKN(failIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt, numComp *sizeof(CztProtIE_Field_CellActvnFail_IEs),
                                  (Ptr *)&(failIe->protocolIEs.member));
   if(NULLP == failIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }

   czCnt = 0;

   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id,Cztid_Cause);
   CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                         CztCriticalityignoreEnum);
   czUtlFillCauseIe(&failIe->protocolIEs.member[czCnt].value.u.cztCause,
                                             causeType, causeVal);

   czCnt++;

   if(idLst != NULLP)
   {
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].id, Cztid_CriticalityDiag);
      CZ_FILL_TKN(failIe->protocolIEs.member[czCnt].criticality,
                                                   CztCriticalityignoreEnum);

      czUtlFillCritDiagIe(uiEvnt,
                &(failIe->protocolIEs.member[czCnt].value.u.cztCriticalityDiag),
                  NULLP, idLst, FALSE);
   }

   CZ_RETVALUE(uiEvnt);
}




/* removed the #if 0 for chksrc tool error
#ifdef ANSI
PUBLIC CztEvnt* czUtlSndPeerErrInd
(
Void        *pdu, 
U8          causeType,
U8          causeVal
)
#else
PUBLIC CztEvnt* czUtlSndPeerErrInd(pdu, causeType, causeVal)
Void       *pdu; 
U8          causeType;
U8          causeVal;
#endif
{
   CZ_RETVALUE(NULLP);
}
*/



/**
    @brief The function czUtlGenPeerErrInd is used for creating an Event for 
           X2 Error Indication message.

    @details This function creates the CztEvent for X2 Error Indication Message.
             It allocates the memory and fills the values passed
             as input parameters.

    @param[out] pdu        The pdu is the pointer to the PDU of the message
                           to be filled.
    @param[in] msgType     The msgType is the message type for which the error
                           indication is generated.
    @param[in] msgInfo     The msgInfo is the pointer to the message information
                           to be filled.
    @param[in] causeType   The causeType is the cause type for cancellation
                           to be filled
    @param[in] causeVal    The causeType is the cause type for cancellation
                           to be filled

    @return
            CztEvnt The pointer to the UI event created for X2 Error
            Indication Message.
*/

#ifdef ANSI
PUBLIC CztEvnt* czUtlGenPeerErrInd
(
CztX2AP_PDU *pdu, 
U8          msgType,
CzMsgInfo   *msgInfo,
U8          causeType,
U8          causeVal
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlGenPeerErrInd(pdu, msgType, msgInfo, causeType, causeVal)
CztX2AP_PDU *pdu; 
U8          msgType;
CzMsgInfo   *msgInfo;
U8          causeType;
U8          causeVal;
#endif /* endif ANSI */
{
   CztX2AP_PDU         *errIndPdu = NULLP;
   CztErrInd           *errIe = NULLP;
   CztEvnt             *uiEvnt = NULLP;
   S16                  numComp  = 1;
   S16                  cnt = 0;
   CztUeInfo            czTmpUeInfo;
   Bool                 fillCritDiag = FALSE;
   Mem                  czSMem;

   CZ_TRC2(czUtlGenPeerErrInd)

   czTmpUeInfo.oldUEX2APId = CZ_UNKNOWN_UE_X2AP_ID;
   czTmpUeInfo.newUEX2APId = CZ_UNKNOWN_UE_X2AP_ID;

   /* Allocate the buffer */
   czSMem.region = czCb.init.region;
   czSMem.pool   = czCb.init.pool;
      
   CZ_ALLOCEVNT((Ptr *)&uiEvnt, sizeof(CztEvnt), &czSMem);
   if(NULLP == uiEvnt)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_RETVALUE(NULLP);
   }
   errIndPdu = &(uiEvnt->pdu);
   
   if(msgInfo != NULLP)
   {
      if (CZ_UNKNOWN_UE_X2AP_ID != msgInfo->oldUEX2APId)
      {
         czTmpUeInfo.oldUEX2APId = msgInfo->oldUEX2APId;
         numComp++;
      }
      if (CZ_UNKNOWN_UE_X2AP_ID != msgInfo->newUEX2APId)
      {
         czTmpUeInfo.newUEX2APId = msgInfo->newUEX2APId;
         numComp++;
      }
   }
   if(msgType != CZT_MSG_UNKNOWN)
   {
      numComp++;
      fillCritDiag = TRUE;
   }
            
   CZ_FILL_TKN(errIndPdu->choice, X2AP_PDU_INITIATINGMSG);
   CZ_FILL_TKN(errIndPdu->val.initiatingMsg.pres,PRSNT_NODEF);
   CZ_FILL_TKN(errIndPdu->val.initiatingMsg.procedureCode,
                                                    Cztid_errorInd);
   CZ_FILL_TKN(errIndPdu->val.initiatingMsg.criticality, 
                                              CztCriticalityignoreEnum);

   errIe = &(errIndPdu->val.initiatingMsg.value.u.cztErrInd);
   CZ_FILL_TKN(errIe->pres,PRSNT_NODEF);
   CZ_FILL_TKN(errIe->protocolIEs.noComp, numComp);

   CZ_GET_MEM(uiEvnt, numComp *sizeof(CztProtIE_Field_ErrInd_IEs),
                              (Ptr *)&(errIe->protocolIEs.member));
   if(NULLP == errIe->protocolIEs.member)
   {
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(NULLP);
   }
   cnt = 0;
   
   if (CZ_UNKNOWN_UE_X2AP_ID != czTmpUeInfo.oldUEX2APId)
   {
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].id,Cztid_Old_eNB_UE_X2AP_ID);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].criticality,
                                                    CztCriticalityignoreEnum);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].value.u.cztid_Old_eNB_UE_X2AP_ID /* Fix for ccpu00117267 */,
                                                     czTmpUeInfo.oldUEX2APId);
      cnt ++;
   }
   if (CZ_UNKNOWN_UE_X2AP_ID != czTmpUeInfo.newUEX2APId)
   {
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].id,Cztid_New_eNB_UE_X2AP_ID);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].criticality,
                                                 CztCriticalityignoreEnum);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].value.u.cztid_New_eNB_UE_X2AP_ID /* Fix for ccpu00117267 */,
                                                 czTmpUeInfo.newUEX2APId);

      cnt++;
   } 
   CZ_FILL_TKN(errIe->protocolIEs.member[cnt].pres, PRSNT_NODEF);
   CZ_FILL_TKN(errIe->protocolIEs.member[cnt].id,Cztid_Cause);
   CZ_FILL_TKN(errIe->protocolIEs.member[cnt].criticality,
                                               CztCriticalityignoreEnum);
      
   czUtlFillCauseIe(&errIe->protocolIEs.member[cnt].value.u.cztCause, 
                                             causeType, causeVal);
     
   cnt++;

   if(fillCritDiag)
   {
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].pres, PRSNT_NODEF);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].id, Cztid_CriticalityDiag);
      CZ_FILL_TKN(errIe->protocolIEs.member[cnt].criticality,
                                                 CztCriticalityignoreEnum);

      czUtlFillCritDiagIe(uiEvnt, 
                 &(errIe->protocolIEs.member[cnt].value.u.cztCriticalityDiag),
                 pdu, NULLP, TRUE);
   }
      
   CZ_RETVALUE(uiEvnt);
}


/**
    @brief The function czMapPasnErrToCauseIe is used to get the cause type and
           causeVal based on the error Code.

    @details The function czMapPasnErrToCauseIe is used to get the cause type and
           causeVal based on the error Code.

    @param[in]  errCode     The errCode is the error code to be mapped.
    @param[out] causeType   The causeType is the cause type mapped.
    @param[out] causeVal    The causeType is the cause type mapped.

    @return
            Void
*/

#ifdef ANSI
PRIVATE Void czMapPasnErrToCauseIe
(
S16           errCode,
U8            *causeType,
U8            *causeVal
)
#else /* Else if ANSI not defined */
PRIVATE Void czMapPasnErrToCauseIe(errCode, causeType, causeVal)
S16           errCode;
U8            *causeType;
U8            *causeVal;
#endif /* endif ANSI */
{
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
   *causeType = CZT_CAUSE_PROTOCOL;
#else
   *causeType = CAUSE_PROTOCOL;
#endif

   switch (errCode)
   {
      case CM_PASN_UNEXP_VAL:
      case CM_PASN_OUT_RANGE:
      case CM_PASN_EXTRA_PARAM:
      case CM_PASN_BAD_IDX:
      case CM_PASN_UNDEF_PARAM:
      {
         *causeVal =
            CztCauseProtabstract_syntax_error_falsely_constructed_messageEnum;
         break;
      }
      case CM_PASN_MAND_MIS:
      case CM_PASN_DB_ERR:
      case CM_PASN_DEF_MIS:
      {
         *causeVal = CztCauseProtabstract_syntax_error_rejectEnum;
         break;
      }
      case CM_PASN_RES_ERR:
      {
      /* cz001.301: Fix for the build warnings */
         #ifdef CZTV1
         *causeType = CZT_CAUSE_MISC;
         #else
         *causeType = CAUSE_MISC;
         #endif
         *causeVal = CztCauseMiscom_interventionEnum;
         break;
      }
      default:
      {
         *causeVal = CztCauseProttransfer_syntax_errorEnum;
         break; 
      }
   }
}


/**
    @brief The function czUtlX2RsrcStsDecErr is used for building X2 Error
    Indication/ Resource Failure message in case of a Decode Error in an incoming X2 Resource
    status Messages.
          
    @details This function handles building X2 Error Indication message in
    case of a Decode Error in an incoming X2 Resource
    status Messages. If the measurement ID is found in the request,
    failure response is built else X2-Error Indication message is built.

    @param[in] pdu        The pdu is the PDU of the incoming message.
    @param[in] causeType  The causeType is the cause of sending Error/Failure
                          message.
    @param[in] causeVal   The causeVal is the cause of sending Error/Failure
                          message.
                             
    @return
            CztEvnt The pointer to the message built.
*/
#ifdef ANSI
PRIVATE CztEvnt* czUtlX2RsrcStsDecErr
(
CztX2AP_PDU        *pdu,
U8                  causeType,
U8                  causeVal
)
#else /* Else if ANSI not defined */
PRIVATE CztEvnt* czUtlX2RsrcStsDecErr(pdu, causeType, causeVal)
CztX2AP_PDU        *pdu;
U8                  causeType;
U8                  causeVal;
#endif /* endif ANSI */
{
   TknU8              *czIeTkn;
   CztMeasurement_ID  *czMeasId;
   CzIeInfo        czIeId[CZ_MAX_IDX];
   CzRsrcInfo       czResrcInfo;
   U8               czTmpCauseType;
   U8               czTmpCauseVal;
   U8               cnt = 0;

   CZ_TRC2(czUtlX2RsrcStsDecErr);
  
   /* cz001.101 : Memory initialization */
   CZ_SET_ZERO((U8 *)czIeId, CZ_MAX_IDX * sizeof(CzIeInfo)); 
   czUtlGetIE(pdu, Cztid_ENB1_Measurement_ID, &czIeTkn); 
   if(czIeTkn == NULLP)
   {
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_RSRC_ST_REQ, NULLP, 
                                  czTmpCauseType, czTmpCauseVal));
         
   }
   czMeasId = (CztMeasurement_ID *)czIeTkn;
   if((czMeasId->pres != PRSNT_NODEF) || 
      (4095 < czMeasId->val))
   {
      CZ_FILL_IE_ID(czIeId[cnt], Cztid_ENB1_Measurement_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      cnt++;
   }      
   czResrcInfo.eNb1measId = ((CztMeasurement_ID *)czIeTkn)->val;
   czUtlGetIE(pdu, Cztid_ENB2_Measurement_ID, &czIeTkn); 
   if(czIeTkn == NULLP)
   {
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_RSRC_ST_REQ, NULLP, 
                                  czTmpCauseType, czTmpCauseVal));
   }
   czMeasId = (CztMeasurement_ID *)czIeTkn;
   if((czMeasId->pres != PRSNT_NODEF) || 
      (4095 < czMeasId->val))
   {
      CZ_FILL_IE_ID(czIeId[cnt], Cztid_ENB2_Measurement_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
   }      
   czResrcInfo.eNb2measId = ((CztMeasurement_ID *)czIeTkn)->val;
   
   CZ_RETVALUE(czUtlGenRsrcStsFail(czIeId, &czResrcInfo, causeType, causeVal));
   
}


/**
    @brief The function czUtlX2SetupReqDecErr is used for building X2 Setup 
    Failure message in case of a Decode Error in an incoming X2 Resource
    status Messages.
          
    @details This function handles building X2 Setup Failure message in
    case of a Decode Error in an incoming X2 Setup Request
    status Messages. 

    @param[in] pdu        The pdu is the PDU of the incoming message.
    @param[in] causeType  The causeType is the cause of sending Failure
                          message.
    @param[in] causeval   The causeVal is the cause of sending Failure
                          message.
                             
    @return
            CztEvnt The pointer to the message built.
*/

#ifdef ANSI
PRIVATE CztEvnt* czUtlX2SetupReqDecErr
(
CztX2AP_PDU        *pdu,
U8                  causeType,
U8                  causeval
)
#else /* Else if ANSI not defined */
PRIVATE CztEvnt* czUtlX2SetupReqDecErr( pdu, causeType, causeval)
CztX2AP_PDU        *pdu;
U8                  causeType;
U8                  causeval;
#endif /*endif ANSI */
{
   TknU8           *czIeTkn;
   CzIeInfo        czIeId[CZ_MAX_IDX];
   U8              czTmpCauseType;
   U8              czTmpCauseVal;
  
   CZ_TRC2(czUtlX2SetupReqDecErr);
   
   CZ_SET_ZERO((U8 *)czIeId, CZ_MAX_IDX * sizeof(CzIeInfo)); 
   czUtlGetIE(pdu, Cztid_GlobalENB_ID, &czIeTkn); 
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[0], Cztid_GlobalENB_ID,
                   CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;
      CZ_RETVALUE(czUtlGenX2SetupFailure(czIeId, czTmpCauseType, 
                                                      czTmpCauseVal));
   }
   else
   {
      CZ_RETVALUE(czUtlGenX2SetupFailure(NULLP, causeType, causeval));
   }
   
}


/**

     @brief The function czUtlMobChgReqDecErr is used for building Mobility Change
            Failure message in case of a Decode Error in an incoming Mobility Change
            Request Messages.

     @details   This function handles building Mobility Change Failure message in
               case of a Decode Error in an incoming Mobility Change Request Messages.

     @param[in] pdu        The pdu is the PDU of the incoming message.
     @param[in] causeType  The causeType is the cause of sending Failure
                           message.
     @param[in] causeval   The causeVal is the cause of sending Failure
                           message.

     @return 
         CztEvnt The pointer to the message built.

*/
#ifdef ANSI
PRIVATE CztEvnt* czUtlMobChgReqDecErr
(
CztX2AP_PDU        *pdu,
U8                  causeType,
U8                  causeval
)
#else /* Else if ANSI not defined */
PRIVATE CztEvnt* czUtlMobChgReqDecErr( pdu, causeType, causeval)
CztX2AP_PDU        *pdu;
U8                  causeType;
U8                  causeval;
#endif /* endif ANSI */
{
   TknU8           *czIeTkn;
   CzIeInfo        czIeId[CZ_MAX_IDX];
   U8              czTmpCauseType;
   U8              czTmpCauseVal;
   U8              czCnt = 0;
   CztECGI         *czENB1Ecgi, *czENB2Ecgi;

   CZ_TRC2(czUtlMobChgReqDecErr);

   CZ_SET_ZERO((U8 *)czIeId, CZ_MAX_IDX * sizeof(CzIeInfo));
   czUtlGetIE(pdu, Cztid_ENB1_Cell_ID, &czIeTkn);
    if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_ENB1_Cell_ID,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      czCnt++;
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_MOB_CHANGE_REQ, NULLP,
                                     czTmpCauseType, czTmpCauseVal));
   }
   czENB1Ecgi = (CztECGI *)czIeTkn;
   if(czENB1Ecgi->pres.val != PRSNT_NODEF)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_ENB1_Cell_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      czCnt++;
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;

      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_UNKNOWN, NULLP,
                                     czTmpCauseType, czTmpCauseVal));

   }

   czUtlGetIE(pdu, Cztid_ENB2_Cell_ID, &czIeTkn);
    if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_ENB2_Cell_ID,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      czCnt++;
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_MOB_CHANGE_REQ, NULLP,
                                     czTmpCauseType, czTmpCauseVal));
   }
   czENB2Ecgi = (CztECGI *)czIeTkn;
   if(czENB2Ecgi->pres.val != PRSNT_NODEF) 
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_ENB2_Cell_ID,
             CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      czCnt++;
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1 
      czTmpCauseType = CZT_CAUSE_PROTOCOL;
#else
      czTmpCauseType = CAUSE_PROTOCOL;
#endif
      czTmpCauseVal = CztCauseProtabstract_syntax_error_rejectEnum;
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_MOB_CHANGE_REQ, NULLP,
                                     czTmpCauseType, czTmpCauseVal));
   }

   czUtlGetIE(pdu, Cztid_ENB2_Proposed_Mobility_Params, &czIeTkn);
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_ENB2_Proposed_Mobility_Params ,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      czCnt++;
   }

   czUtlGetIE(pdu, Cztid_Cause, &czIeTkn);
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_Cause,
             CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      czCnt++;
   }

   CZ_RETVALUE(czUtlGenMobChgFailure(czIeId,  czENB1Ecgi, czENB2Ecgi, 
                                                  causeType, causeval));
}





/**
     @brief The function czUtlCellActReqDecErr is used for building Cell Activation
            Failure message in case of a Decode Error in an incoming Cell Activation
            Request Messages.

     @details This function handles building Cell Activation Failure message in
               case of a Decode Error in an incoming Cell Activation Request Messages.

     @param[in] pdu        The pdu is the PDU of the incoming message.
     @param[in] causeType  The causeType is the cause of sending Failure
                           message.
     @param[in] causeval   The causeVal is the cause of sending Failure
                           message.

     @return : CztEvnt The pointer to the message built.
*/
#ifdef ANSI
PRIVATE CztEvnt* czUtlCellActReqDecErr
(
CztX2AP_PDU        *pdu,
U8                  causeType,
U8                  causeval
)
#else /* Else if ANSI not defined */
PRIVATE CztEvnt* czUtlCellActReqDecErr( pdu, causeType, causeval)
CztX2AP_PDU        *pdu;
U8                  causeType;
U8                  causeval;
#endif /* endif ANSI */
{
   TknU8           *czIeTkn;
   CzIeInfo        czIeId[CZ_MAX_IDX];

   CZ_TRC2(czUtlCellActReqDecErr);

   CZ_SET_ZERO((U8 *)czIeId, CZ_MAX_IDX * sizeof(CzIeInfo));
   czUtlGetIE(pdu, Cztid_SrvdCellsToActivate, &czIeTkn);
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[0], Cztid_SrvdCellsToActivate,
                   CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
   }
   CZ_RETVALUE(czUtlGenCellActFailure(czIeId, causeType, causeval));
}


                                


/**
    @brief The function czUtlHoReqDecErr is used for building X2 Error
    Indication/ Failure message in case of a Decode Error in an incoming X2 
    Handover Request Message.
          
    @details This function handles building X2 Failure / Error Indication message in
    case of a Decode Error in an incoming X2 Resource
    status Messages. If the Old eNodeB ID is found in the request,
    failure response is built else X2-Error Indication message is built.

    @param[in] pdu        The pdu is the PDU of the incoming message.
    @param[in] causeType  The causeType is the cause of sending Error/Failure
                          message.
    @param[in] causeVal   The causeVal is the cause of sending Error/Failure
                          message.
                             
    @return
            CztEvnt The pointer to the message built.
*/

#ifdef ANSI
PRIVATE CztEvnt* czUtlHoReqDecErr
(
CztX2AP_PDU        *pdu,
U8                  causeType,
U8                  causeVal
)
#else /* Else if ANSI not defined */
PRIVATE CztEvnt* czUtlHoReqDecErr( pdu, causeType, causeVal)
CztX2AP_PDU        *pdu;
U8                  causeType;
U8                  causeVal;
#endif /* endif ANSI */
{
   CztUE_X2AP_ID   *czOldUEX2APId;
   TknU8           *czIeTkn;
   CzIeInfo        czIeId[CZ_MAX_IDX];
   CztUeInfo       czUeInfo;
   U8              czCnt = 0;
   CZ_TRC2(czUtlHoReqDecErr);
   
   CZ_SET_ZERO((U8 *)czIeId, CZ_MAX_IDX * sizeof(CzIeInfo)); 
   czUtlGetIE(pdu, Cztid_Old_eNB_UE_X2AP_ID, &czIeTkn); 
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt],Cztid_Old_eNB_UE_X2AP_ID,
            CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_UNKNOWN, NULLP, causeType,
                                                                 causeVal));
   }
   czOldUEX2APId = (CztUE_X2AP_ID *)czIeTkn;
   if((czOldUEX2APId->pres == PRSNT_NODEF) && 
      ((0 < czOldUEX2APId->val) && (czOldUEX2APId->val < 4096)))
   {
      czUeInfo.oldUEX2APId = czOldUEX2APId->val;
   }
   else
   {
      CZ_FILL_IE_ID(czIeId[czCnt],Cztid_Old_eNB_UE_X2AP_ID,
            CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      czCnt++;
   }      
   czUtlGetIE(pdu, Cztid_Cause, &czIeTkn);
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_Cause,
            CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
      czCnt++;
   }
   czUtlGetIE(pdu, Cztid_TgetCell_ID, &czIeTkn);
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_TgetCell_ID,
            CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
      czCnt++;
   }
   czUtlGetIE(pdu, Cztid_GUMMEI_ID, &czIeTkn);
   if(czIeTkn == NULLP)
   {
      CZ_FILL_IE_ID(czIeId[czCnt], Cztid_TgetCell_ID,
            CztCriticalityrejectEnum, CztTypOfErrmissingEnum);
   }

   CZ_RETVALUE(czUtlGenHoPrepFailure(czIeId,  &czUeInfo, causeType, causeVal));
}


/**
    @brief The function czUtlSndRspMsg is used to build responses for any
    incoming message that has failed decoding.
          
    @details This function is used to build responses for any
    incoming message that has failed decoding. If incoming Request Message
    Type has a Failure Response, a Failure response is built, else X2-Error
    Indication is built.

    @param[in] pdu         The pdu is the PDU of the incoming message. 
    @param[in] pasnErr     The pasnErr defines the decode error that occured. 
                             
    @return
    CztEvnt The pointer to the message built.
*/
#ifdef ANSI
PUBLIC CztEvnt* czUtlSndRspMsg
(
CztX2AP_PDU        *pdu,
CmPAsnErr          *pasnErr
)
#else /* Else if ANSI not defined */
PUBLIC CztEvnt* czUtlSndRspMsg( pdu, pasnErr)
CztX2AP_PDU        *pdu;
CmPAsnErr          *pasnErr;
#endif /* endif ANSI */
{
   U8  czProcCode;
   U8  czMsgType;
   U8  czCriticality;
   U8  czMsgId;
   U8  czCauseType;
   U8  czCauseVal;
   U8  czRspMsgType;
   CztUE_X2AP_ID *czUeX2APId = NULLP;
   CztUeInfo czUeInfo;
   CztUeInfo *czTmpUeInfo = NULLP;
   S16 ret = ROK;
   CZ_TRC2(czUtlSndRspMsg);

   czMapPasnErrToCauseIe(pasnErr->errCode, &czCauseType, &czCauseVal); 
   ret = czUtlGetProcMsgCrit(pdu, &czProcCode, &czMsgType, &czCriticality,
                                                              &czMsgId);
   if(ret != ROK)
   {
      CZ_RETVALUE(czUtlGenPeerErrInd(pdu, CZT_MSG_UNKNOWN, NULLP, czCauseType,
               czCauseVal));
   }
   czRspMsgType =  responseMsg[czMsgId]; /* Fix for ccpu00117358 */

   switch(czRspMsgType)
   {
      case CZT_MSG_HO_FAIL:
      {
         CZ_RETVALUE(czUtlHoReqDecErr(pdu, czCauseType, czCauseVal)); 
      }
      case CZT_MSG_X2_SETUP_FAIL:
      {
         CZ_RETVALUE(czUtlX2SetupReqDecErr(pdu, czCauseType, czCauseVal)); 
      }
      case CZT_MSG_CFG_UPD_FAIL:
      {
         CZ_RETVALUE(czUtlGenX2CfgUpdFailure(pdu, czCauseType, czCauseVal)); 
      }
      case CZT_MSG_RSRC_ST_FAIL:
      {
         CZ_RETVALUE(czUtlX2RsrcStsDecErr(pdu, czCauseType, czCauseVal)); 
      }
      case CZT_MSG_MOB_CHANGE_FAIL:
      {
          CZ_RETVALUE(czUtlMobChgReqDecErr(pdu, czCauseType, czCauseVal));
      }
      case CZT_MSG_CELL_ACTIVATION_FAIL:
      {
           CZ_RETVALUE(czUtlCellActReqDecErr(pdu, czCauseType, czCauseVal));
      }
      case CZT_MSG_UNKNOWN:
      {
          czUtlGetIE(pdu, Cztid_Old_eNB_UE_X2AP_ID, (TknU8 **)&czUeX2APId); 
          if(czUeX2APId && ((0 <= czUeX2APId->val) && (czUeX2APId->val < 4096)))
          {
             czUeInfo.oldUEX2APId = (U16) czUeX2APId->val;
             czUtlGetIE(pdu, Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&czUeX2APId);
             /*kworks fix: removed 0<=ueX2APId->val check*/ 
             if(czUeX2APId && (czUeX2APId->val < 4096))
             {
                czUeInfo.newUEX2APId =(U16) czUeX2APId->val;
             }  
             else
             {
                czUeInfo.newUEX2APId = CZ_UNKNOWN_UE_X2AP_ID;
             } 
             czTmpUeInfo = &czUeInfo;
          }
         CZ_RETVALUE(czUtlGenPeerErrInd(pdu, czMsgId, 
                                     ((CzMsgInfo *)czTmpUeInfo),
                                     czCauseType, czCauseVal));
      }
      default:
         CZ_RETVALUE(NULLP);
   }
   
}


/**
    @brief The function czUtlGetCause is used for getting the cause value from
    the given cause IE.
          
    @details The function czUtlGetCause is used for getting the cause value from
    the given cause IE. It returns the value based on the Cause Type.


    @param[in] causeIE  The pointer to the IE from which the cause value
    is extracted.
                             
    @return
            U8 value for the cause value.
*/
#ifdef ANSI
PUBLIC U8 czUtlGetCause
(
CztCause *causeIE
)
#else /* Else if ANSI not defined */
PUBLIC U8 czUtlGetCause(causeIE)
CztCause *causeIE;   
#endif /* endif ANSI */
{
   U8  czCauseVal;
   
   CZ_TRC2(czUtlGetCause)

   switch(causeIE->choice.val)
   {
      case CAUSE_RADIONW:
      {
         /*kworks fix: type cast*/
         czCauseVal = (U8)causeIE->val.radioNw.val;
         break;
      }
      case CAUSE_TRANSPORT:
      {
         /*kworks fix: type cast*/
         czCauseVal = (U8) causeIE->val.transport.val;
          break;
      }
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      case CZT_CAUSE_PROTOCOL:
      {
         /*kworks fix: type cast*/
         czCauseVal = (U8) causeIE->val.protocol.val;
         break;
      }
#else
      case CAUSE_PROTOCOL:
      {
         /*kworks fix: type cast*/
         czCauseVal = (U8) causeIE->val.protocol.val;
         break;
      }
#endif
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
      case CZT_CAUSE_MISC:
      {
         /*kworks fix : type cast*/
         czCauseVal =(U8) causeIE->val.misc.val;
         break;
      }
#else
      case CAUSE_MISC:
      {
         /*kworks fix : type cast*/
         czCauseVal =(U8) causeIE->val.misc.val;
         break;
      }
#endif
      default:
         CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(czCauseVal);
} /* end of czUtlGetStream */


/**
    @brief The function czUtlGetStream is used to get an outgoing stream ID.
              
    @details The function czUtlGetStream is used to get an outgoing stream ID.
    It check for a free outgoinf stream ID in the given association of the Peer
    Control Block. If it is exhausted, the existing stream ID is return
    in circular order.

    @param[in]  peerCb  The peerCb is the peer control block for which is stream
                        ID is returned using the information in its association
                        control block.
    @param[out] strmId  Th strmId is the outgoing stream ID that is returned.
                             
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czUtlGetStream
(
CzPeerCb    *peerCb,
CztStrmId   *strmId
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlGetStream(peerCb, strmId)
CzPeerCb    *peerCb;
CztStrmId   *strmId;
#endif /* endif ANSI */
{
   CZ_TRC2(czUtlGetStream)

   *strmId = ((peerCb->nextStreamId) % peerCb->assocCb->assocCfg->locOutStrms);

   if (*strmId == peerCb->peerCfg.globalStreamId)
   {
      /* Since stream comProcStrm is reserved for common procedure messages,
           use another stream */
      *strmId = ((*strmId + 1) % peerCb->assocCb->assocCfg->locOutStrms);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF,
                    "czUtlGetStream(*strmId = %d)\n", *strmId));

   peerCb->nextStreamId = ((*strmId + 1) % 
                          peerCb->assocCb->assocCfg->locOutStrms);

   CZ_RETVALUE(ROK);
} /* end of czUtlGetStream */


/** @} */

/***********************************************************
*
*     Func : czUtlFillTknBStr32
*
*
*     Desc : czUtlFillTknBStr32
*
*     Ret  : S16
*
*     Notes : None
*
*     File  : tr_utl.c
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 czUtlFillTknBStr32
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val      /* value to be filled in */
)
#else /* Else if ANSI not defined */
PUBLIC S16 czUtlFillTknBStr32(ptr, len, val)
TknBStr32  *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
U32         val;  /* value to be filled in */
#endif /* endif ANSI */
{
   U32 valMask = 0;
   U8 czIndx;

   TRC2(czUtlFillTknBStr32)

   if (len > 32)
   {
      RETVALUE(RFAILED);
   }

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len = (U8) len;

   /* *(U32*)ptr->val = (val << (32 - len)); */
   *(U32*)ptr->val = (val);
   valMask = 0x000000FF;
   for (czIndx = 0; czIndx <(len/8); czIndx++)
   {
       ptr->val[czIndx] = val & valMask;
      valMask = valMask << 8;
   }

   RETVALUE(ROK);
}


/**********************************************************************
  
         End of file:     tr_utl.c@@/main/2 - Tue Aug 30 18:36:17 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
/main/3     cz001.301  akaranth         1. Fix for the build warnings.
*********************************************************************91*/
