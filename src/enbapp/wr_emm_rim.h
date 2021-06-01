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
               
     File:     wr_emm_rim.h

     Sid:      wr_emm_rim.h@@/main/Br_Tenb_Rim_Csfb_Intg/7 - Mon Nov  3 14:36:09 2014

     Prg:      nu 

*********************************************************************21*/



#ifndef __WR_EMM_RIM_H__
#define __WR_EMM_RIM_H__

#include "wr_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RIM PDUs between RIM SAPs */
#define  WR_RIM_RI_PDU         0x70  /* RAN-INFORMATION IE */
#define  WR_RIM_RIR_PDU        0x71  /* RAN-INFORMATION-REQUEST IE */
#define  WR_RIM_RIA_PDU        0x72  /* RAN-INFORMATION-ACK IE */
#define  WR_RIM_RIE_PDU        0x73  /* RAN-INFORMATION-ERROR IE */
#define  WR_RIM_RIAE_PDU       0x74  /* RAN-INFORMATION-APPLICATION ERROR*/

/* RIM Related IEIs */
#define  WR_RIM_IEI_PDU_IN_ERR       0x15  /* PDU in err */
#define  WR_RIM_IEI_APP_ID           0x4b  /* RIM Application Identity */
#define  WR_RIM_IEI_RSN              0x4c  /* RIM Sequence Number */
#define  WR_RIM_IEI_RIR_APP_CONT     0x4d  /* RAN-INFORMATION-REQUEST 
                                              Application Container */
#define  WR_RIM_IEI_RI_APP_CONT      0x4e  /* RAN-INFORMATION Application 
                                              Container */
#define  WR_RIM_IEI_PDU_IND          0x4f  /* RIM PDU Indications */
#define  WR_RIM_IEI_ROUT_INFO        0x54  /* RIM Routing Information */
#define  WR_RIM_IEI_PROT_VER         0x55  /* RIM Protocol Version Number */
#define  WR_RIM_IEI_APP_ERR_CONT     0x56  /* Application Error Container */
#define  WR_RIM_IEI_RIR_CONT         0x57  /* RAN-INFORMATION-REQUEST RIM 
                                              Container */
#define  WR_RIM_IEI_RI_CONT          0x58  /* RAN-INFORMATION RIM Container */
#define  WR_RIM_IEI_RIAE_CONT        0x59  /* RAN-INFORMATION-APPLICATION-ERROR
                                              RIM Container */
#define  WR_RIM_IEI_RIACK_CONT       0x5a  /* RAN-INFORMATION-ACK RIM 
                                              Container */
#define  WR_RIM_IEI_ERR_CONT         0x5b  /* RAN-INFORMATION-ERROR RIM 
                                              Container */         
#define  WR_RIM_IEI_RNC_ID           0x6c  /* RNC Identifier */
#define  WR_RIM_IEI_SON_TRFR_APP_ID  0x84  /* SON Transfer Application 
                                              Identity */
#define  WR_RIM_IEI_CAUSE            0x07  /* Error Cause */
#define  WR_RIM_IEI_CELL_ID          0x08  /* Cell Identifier IE */

/* RAT Choice */
#define  WR_RIM_GERAN_RAT        0    /* GERAN RAT type */         
#define  WR_RIM_UTRA_RAT         1    /* UTRAN RAT type */         
#define  WR_RIM_EUTRA_RAT        2    /* EUTRAN RAT type */         

/* RSN Values */
#define  WR_RIM_RSN_MAX   0xFFFFFFFF  /* Maximum RSN value */
#define  WR_RIM_RSN_MID   0x80000000 /* Middle value of RSN */

/* Encoding index S1AP asn db: TODO - Move it to common place */
#define WR_UMM_S1AP_ENC_IE_IDX_GLBL_ENB_ID  1

/* Maximum length in in LI in one byte */
#define WR_RIM_MAX_LEN_ONE_BYTE     0X7F

/* Maximum number of cells kept as 3 assuming
 * Macro eNbode/Nodeb can support max 3 cells as of now
 * Need to re-define the macro in case if it is more than 3 */
#define WR_RIM_MAX_CELLS             3

/* Invalid cell Id */
#define WR_RIM_INVALID_CELL_ID       0xFF


/* Allowed RIM Application Identity IEs */

/** @brief the following enum defines the possible values for RIM 
 * Application Identity
 */
typedef enum wrRimAppId 
{
   WR_RIM_APP_RESVD = 0,   /* Reserved Value */
   WR_RIM_APP_NACC,        /* Network Assisted Cell Change (NACC) */
   WR_RIM_APP_SI3,         /* System Information 3 (SI3) */
   WR_RIM_APP_MBMS,        /* MBMS data channel */
   WR_RIM_APP_SON_TRFR,    /* SON Transfer */
   WR_RIM_APP_UTRA_SI,     /* UTRA System Information (UTRA SI) */
   WR_RIM_APP_MAX          /* Maximum App Id */
} WrRimAppId;

/** @brief the following enum defines the possible values for RIM ACK 
 * Indicator 
 */
typedef enum wrRimAckInd
{
   WR_RIM_NO_ACK_REQD = 0,  /* No ACK Requested */
   WR_RIM_ACK_REQD          /* ACK Requested */
} WrRimAckInd;

/** @brief the following enum defines the possible values for RIM Protocol
 * Version number
 */
typedef enum wrRimProtVer
{
   WR_RIM_PROT_RESVD_VER = 0,  /* RIM Protocol Reserved Version */
   WR_RIM_PROT_VER1            /* RIM Protocol Version 1 */
} WrRimProtVer;

/** @brief the following enum defines the type of SI or PSI messages
 */
typedef enum wrRimSiType
{
   WR_RIM_SI = 0,     /* SI messages as specified for BCCH */
   WR_RIM_PSI         /* PSI messages as specified for BCCH */
} WrRimSiType;

/** @brief the following enum defines the possible values for
 * RAN-INFORMATION-REQUEST PDU Type Extension
 */
typedef enum wrRimRirPduExt
{
   WR_RIM_RIR_STOP = 0,   /* RAN-INFORMATION-REQUEST/Stop Report PDU */
   WR_RIM_RIR_SINGLE,     /* RAN-INFORMATION-REQUEST/Single Report PDU */
   WR_RIM_RIR_MULTI,      /* RAN-INFORMATION-REQUEST/Multiple Report PDU */
   WR_RIM_RIR_RESVD       /* Reserved */
} WrRimRirPduExt;

/** @brief the following enum defines the possible values for
 * RAN-INFORMATION PDU Type Extension
 */
typedef enum wrRimRiPduExt
{
   WR_RIM_RI_STOP = 0,    /* RAN-INFORMATION/Stop PDU */
   WR_RIM_RI_SINGLE,      /* RAN-INFORMATION/Single PDU */
   WR_RIM_RI_INIT_MULTI,  /* RAN-INFORMATION/Initial Multiple Report PDU */
   WR_RIM_RI_MULTI,       /* RAN-INFORMATION/Multiple Report PDU */
   WR_RIM_RI_END,         /* RAN-INFORMATION/End PDU */
   WR_RIM_RI_RESVD        /* Reserved */
} WrRimRiPduExt;

/** @brief the following enum defines NACC Cause field
 */
typedef enum wrRimNaccCause
{
   WR_RIM_NACC_OTHRUNSPEC = 0,          /* Other unspecified erro*/
   WR_RIM_NACC_SYNTAC_ERR,              /* Syntax error in the Application
                                           Container */
   WR_RIM_NACC_CELL_ID_NOT_MATCH,       /* Reporting Cell Identifier does 
                                           not match with the Destination 
                                           Cell Identifier or with the 
                                           Source Cell Identifier.*/
   WR_RIM_NACC_SI_PSI_TYP_ERR,          /* SI/PSI type error */
   WR_RIM_NACC_INCONSISTENT_SI_PSI_LEN, /* Inconsistent length of a SI/PSI
                                           message */
   WR_RIM_NACC_INCONSISTENT_MSG,        /* Inconsistent set of messages */
   WR_RIM_NACC_RESVD_CAUSE              /* Reserved Cause value */
} WrRimNaccCause;

/** @brief the following enum defines SI3 Cause field
 */
typedef enum wrRimSi3Cause
{
   WR_RIM_SI3_OTHRUNSPEC = 0,          /* Other unspecified erro*/
   WR_RIM_SI3_SYNTAC_ERR,              /* Syntax error in the Application
                                          Container */
   WR_RIM_SI3_CELL_ID_NOT_MATCH,       /* Reporting Cell Identifier does 
                                          not match with the Destination 
                                          Cell Identifier or with the 
                                          Source Cell Identifier.*/
   WR_RIM_SI3_INCONSISTENT_MSG,        /* Inconsistent length of SI3 message */
   WR_RIM_SI3_RESVD_CAUSE              /* Reserved Cause value */
} WrRimSi3Cause;

/** @brief the following enum defines UTRA SI Cause field
 */
typedef enum wrRimUtraSiCause
{
   WR_RIM_UTRA_SI_UNSPEC = 0,           /* Other unspecified erro*/
   WR_RIM_UTRA_SI_SYNTAC_ERR,           /* Syntax error in the Application
                                           Container */
   WR_RIM_UTRA_SI_INCONSISTENT_CELL_ID, /* Inconsistent Reporting Cell 
                                           Identifier */
   WR_RIM_UTRA_SI_RESVD_CAUSE           /* Reserved Cause value */
} WrRimUtraSiCause;

/** @brief the following enum defines Applicable Cause values at RIM
 */
typedef enum wrRimCause
{
   WR_RIM_SEMANT_ERR = 0x20,       /* Semantically incorrect PDU - 0x20 */
   WR_RIM_INVLD_MNDTRY_IE,         /* Syntax err:Invalid mandatory information- 0x21 */
   WR_RIM_MSING_MNDTRY_IE,         /* Missing mandatory IE - 0x22 */
   WR_RIM_MSING_COND_IE,           /* Missing conditional IE - 0x23 */
   WR_RIM_UNEXPCD_COND_IE,         /* Unexpected conditional IE - 0x24 */
   WR_RIM_COND_IE_ERR,             /* Syntax err:Conditional IE error - 0x25 */
   WR_RIM_PROT_ERR_UNSPEC = 0x27,  /* Protocol error -unspecified - 0x27 */
   WR_RIM_PDU_NOT_COMPTBL = 0x28,  /* Protocol not compatible with the 
                                      feature set */
   WR_RIM_UNKNOWN_RIM_APP = 0x2b   /* Unknown RIM Application Identity or
                                      RIM application disabled - 0x2b */
} WrRimCause;

typedef enum wrRimRoutDisc
{
   WR_RIM_GERAN_ROUT_DISC = 0x00, /* Geran cell is 0x00 */
   WR_RIM_UTRA_ROUT_DISC,         /* Utra cell is 0x01 */
   WR_RIM_EUTRA_ROUT_DISC         /* E-Utra cell is 0x02 */
} WrRimRoutDisc;

/* Macros to indicate the error towards RIM User */
#define WR_RIM_ERR_RIE            1 
#define WR_RIM_ERR_RIAE           2 
#define WR_RIM_ERR_TIME_OUT       3 

/* RIM USER states */
#define WR_RIM_PROC_NOT_INITIATED  0
#define WR_RIM_PROC_INITIATED      1

/* Association states */
#define WR_RIM_RIR_SINGLE_REP_TRIGGERED      0 /* entertain only RI "single" 
                                                  report  or RIE PDU */
#define WR_RIM_RIR_MULTI_REP_TRIGGERED       1 /* entertain only RI "initial 
                                                  multiple or RIE PDU */
#define WR_RIM_RIR_STOP_TRIGGERED            2 /* entertain only RI stop or
                                                  RIE PDU */
#define WR_RIM_RI_MULTI_INIT_REP_RCVD        3 /* entertain only RI with 
                                                  "multiple" "end" or RIE PDU */
#define WR_RIM_RI_MULTI_REP_RCVD             4 /* entertain only RI with 
                                                  "multiple" "end" or RIE PDU */
#define WR_RIM_RI_SINGLE_REP_RCVD            5 /* entertain only RI with 
                                                  "multiple" "end" or RIE PDU */
#define WR_RIM_RIAE_TRIGGERED                6 /* entertain only ACK for RIAE 
                                                  or RIE PDU */
#define WR_RIM_MAX_ASSOCS                  530 /* 16 Geran + 16*32  utran + for
                                                  any incoming(2)  */

/* RIM Encoding: Length of the mendatory IEs */

#define WR_RIM_LEN_APP_ID                1  /* length of app id  */
#define WR_RIM_LEN_PROT_VER              1  /* length of protocol version */
#define WR_RIM_LEN_PDU_IND               1  /* length of PDU indication */
#define WR_RIM_LEN_RSN                   4  /* length of RSN */
#define WR_RIM_LEN_ENB_ID                12 /* E-UTRAN identifier length */
#define WR_RIM_LEN_TAI                   5  /* Tracking Area Id lentgh */
#define WR_RIM_LEN_PLMN                  3  /* PLMN Id lentgh */
#define WR_RIM_LEN_GERAN_CELL_ID         8  /* Geran cell identifier length */
#define WR_RIM_LEN_RNC_ID                8  /* RNC identifier length */
#define WR_RIM_LEN_APP_ERR_CAUSE         1  /* Application error cause length */
#define WR_RIM_LEN_RIM_ERR_CAUSE         1  /* RIM error cause length */
#define WR_RIM_LEN_UTRAN_CELL_ID         9  /* UTRAN cell identifier length */
#define WR_RIM_ROUT_ADD_LEN              20 /* Routing address length */

/* RIM decoding - Length */
#define WR_RIM_MIN_ROUT_INFO_LEN          11 /* Min length of Routing Info */
#define WR_RIM_MIN_APP_CONT_LEN           2  /* Min length of Application Cont*/
#define WR_RIM_MIN_SON_APP_CONT_LEN       2  /* Min length of SON App Cont */    
#define WR_RIM_MIN_LEN_IEI_LI             2  /* Min length of IEI and LI */    

/* Macros for Packing and Unpacking */

/* Macro to validate return value */ 
/*#if (ERRCLASS & ERRCLS_DEBUG) */
#define WR_RIM_CHK_RET_VAL(_ret)                                              \
{                                                                             \
   if (_ret != ROK)                                                           \
   {                                                                          \
       RETVALUE(RFAILED);                                                     \
    }                                                                         \
}
/*#else
#define WR_RM_CHK_RET(_ret)
#endif */

/* Packing of PLMN array - For ASN encoding */
#define WR_RIM_PK_PLMN_ARR( _pkPlmnArr, _plmnId,_idx )                        \
{                                                                             \
   _pkPlmnArr[_idx++] =(((_plmnId->mcc[1])<<4) | (_plmnId->mcc[0]));          \
   if (_plmnId->numMncDigits == 2)                                            \
   {                                                                          \
      _pkPlmnArr[_idx++] =((0xf0) | (_plmnId->mcc[2]));                       \
      _pkPlmnArr[_idx++] =(((_plmnId->mnc[1])<<4) | (_plmnId->mnc[0]));       \
   } \
   else                                                                       \
   {                                                                          \
      _pkPlmnArr[_idx++] =(((_plmnId->mnc[0])<<4) | (_plmnId->mcc[2]));       \
      _pkPlmnArr[_idx++] =(((_plmnId->mnc[2])<<4) | (_plmnId->mnc[1]));       \
   }                                                                          \
}
/* Packing of PLMN array - For TLV encoding */
#define WR_RIM_TLV_PK_PLMN_ARR( _pkPlmnArr, _plmnId,_idx )                        \
{                                                                             \
   _pkPlmnArr[_idx++] =(((_plmnId->mcc[1])<<4) | (_plmnId->mcc[0]));          \
   if (_plmnId->numMncDigits == 2)                                            \
   {                                                                          \
      _pkPlmnArr[_idx++] =((0xf0) | (_plmnId->mcc[2]));                       \
      _pkPlmnArr[_idx++] =(((_plmnId->mnc[1])<<4) | (_plmnId->mnc[0]));       \
   } \
   else                                                                       \
   {                                                                          \
      _pkPlmnArr[_idx++] =((_plmnId->mnc[2])<<4) | (_plmnId->mcc[2]);            \
      _pkPlmnArr[_idx++] =(((_plmnId->mnc[1])<<4) | (_plmnId->mnc[0]));          \
   }                                                                          \
}
/* TODO: protocol message: packing of U8 at end of message */
#define WR_RIM_PK_U8(_msg, _val)                                              \
{                                                                             \
   S16 _ret;                                                                  \
   (_ret) = SAddPstMsg((Data) (_val), (_msg));                                \
    WR_RIM_CHK_RET_VAL(_ret); \
}

/* Packing of application Identity */
#define WR_RIM_PK_APP_ID(_appId, _rimPdu)                                     \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_APP_ID);                              \
   WR_RIM_PK_LI(_rimPdu, WR_RIM_LEN_APP_ID);                                  \
   WR_RIM_PK_U8(_rimPdu, _appId); \
}
/* Packing of RSN */
#define WR_RIM_PK_RSN(_rsn, _rimPdu)                                          \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_RSN);                                 \
   WR_RIM_PK_LI(_rimPdu, WR_RIM_LEN_RSN);                                     \
   WR_RIM_PK_U32(_rimPdu, _rsn); \
}
/* Packing of Version number */
#define WR_RIM_PK_VER(_ver, _rimPdu)                                          \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_PROT_VER);                            \
   WR_RIM_PK_LI(_rimPdu, WR_RIM_LEN_PROT_VER);                                \
   WR_RIM_PK_U8(_rimPdu, _ver);                                               \
}
/* Packing RIM error cause */
#define WR_RIM_PK_CAUSE(_cause, _rimPdu)                                      \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_CAUSE);                               \
   WR_RIM_PK_LI(_rimPdu, WR_RIM_LEN_RIM_ERR_CAUSE);                           \
   WR_RIM_PK_U8(_rimPdu, _cause);                                             \
}

/* Packing of PDU Indication */
#define WR_RIM_PK_PDU_IND(_ackInd, _pduExt, _rimPdu)                                        \
{                                                                                 \
   U8 _pduInd = 0x00;                                                      \
   _pduInd   = (0x07 & _pduExt );                                                    \
   _pduInd   = ((_pduInd << 1 ) | (0x01 & _ackInd));                       \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_PDU_IND);                         \
   WR_RIM_PK_LI(_rimPdu, WR_RIM_LEN_PDU_IND);                    \
   WR_RIM_PK_U8(_rimPdu, _pduInd);                                          \
}

/* Packing of Application container */
#define WR_RIM_PK_APP_CONT(_appCont, _appContIEI, _rimPdu)                    \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, _appContIEI);                                        \
   WR_RIM_PK_LI(_rimPdu, _appCont->len);                                      \
   WR_RIM_PK_STR(_rimPdu, _appCont->val, _appCont->len);                      \
}
/* Packing of SON Application Id */
#define WR_RIM_PK_SON_APP_ID(_sonApp, _rimPdu)                                \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_SON_TRFR_APP_ID);                         \
   WR_RIM_PK_LI(_rimPdu, _sonApp->len);                                       \
   WR_RIM_PK_STR(_rimPdu, _sonApp->val, _sonApp->len);                        \
}

/* Packing of Pdu in error */
#define WR_RIM_PK_PDU_IN_ERR(_pduInErr, _rimPdu)                              \
{                               \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_PDU_IN_ERR);                          \
   WR_RIM_PK_LI(_rimPdu, _pduInErr->len);                                     \
   WR_RIM_PK_STR(_rimPdu, _pduInErr->val, _pduInErr->len);                    \
}
/* Packing of Application Error container */
#define WR_RIM_PK_APP_ERR_CONT(_appErrCont, _cause, _rimPdu )                 \
{                                                                             \
   WR_RIM_PK_U8(_rimPdu, WR_RIM_IEI_APP_ERR_CONT);                        \
   WR_RIM_PK_LI(_rimPdu, (_appErrCont->len + 1));                             \
   WR_RIM_PK_U8(_rimPdu, _cause);                                             \
   WR_RIM_PK_STR(_rimPdu, _appErrCont->val, _appErrCont->len);                \
}

/* TODO: protocol message: packing of U16 at end of message */
#define WR_RIM_PK_U16(_msg, _val)                                             \
{                                                                             \
   WR_RIM_PK_U8(_msg, (U8) GetHiByte(_val));                                  \
   WR_RIM_PK_U8(_msg, (U8) GetLoByte(_val));                                  \
}
#define WR_RIM_PK_U16_ARR(_ptr, _val, _idx)                                   \
{                                                                             \
   _ptr[_idx++] = GetHiByte(_val);                                            \
   _ptr[_idx++] = GetLoByte(_val);                                            \
}    
/*  protocol message: packing of U32 at end of message */
#define WR_RIM_PK_U32(_msg, _val)                                             \
{                                                                             \
   U16 _tmp16;                                                                \
   _tmp16 = (U16) GetHiWord(_val);                                            \
   WR_RIM_PK_U16(_msg, _tmp16)                                                \
   _tmp16 = (U16) GetLoWord(_val);                                            \
   WR_RIM_PK_U16(_msg, _tmp16);                                               \
}
/* Packing of Length Indicator */
#define WR_RIM_PK_LI(_pdu, _val )                                             \
{ \
  if(_val > 0x7F )                                                            \
  { \
    WR_RIM_PK_LI_2(_pdu, _val);                                               \
  } \
  else    \
  { \
    WR_RIM_PK_LI_1(_pdu, _val);                                               \
  } \
}
 
/* Packing of Length Indicator when greater than 127 */
#define WR_RIM_PK_LI_1(_pdu, _val)                                            \
{                                                                             \
   U8 _tmpLen = _val;                                                         \
   _tmpLen |= 0x80;                                                           \
   WR_RIM_PK_U8(_pdu, _tmpLen);                                               \
}
#define WR_RIM_PK_LI_2(_pdu, _val)                                            \
{                                                                             \
   U16 _tmpLen = _val;                                                        \
   WR_RIM_PK_U16(_pdu, _tmpLen);                                              \
}
#define WR_RIM_PK_STR(_pdu, _val, _len)                                       \
{                                                                             \
    S16 _ret;                                                                 \
   _ret  = SAddPstMsgMult(_val, _len, _pdu);                                  \
    WR_RIM_CHK_RET_VAL(_ret);                                                 \
}

/* Unpack Macros */
/* Unpacking of U8 */
#define WR_RIM_UNPK_U8(_pdu, _val, _idx)                                      \
{   \
     _val = _pdu[_idx++];                                                     \
}
/* Unpacking of U16 */
#define WR_RIM_UNPK_U16(_pdu, _val, _idx)                                     \
{  \
  U8 _tmpVal = _pdu[_idx++];                                                  \
  _val = PutHiByte(_val, _tmpVal);                                            \
  _tmpVal  =  _pdu[_idx++]; \
  _val = PutLoByte(_val, _tmpVal); \
}
/* Unpacking of U32 */
#define WR_RIM_UNPK_U32(_pdu, _val, _idx)                                     \
{   \
  U16 _tmpU16 = 0;                                                            \
  WR_RIM_UNPK_U16(_pdu, _tmpU16, _idx);                                       \
  _val = PutHiWord(_val, _tmpU16);                                            \
  WR_RIM_UNPK_U16(_pdu, _tmpU16, _idx);                                       \
  _val = PutLoWord(_val, _tmpU16); \
}

/* Unpacking of string */
#define WR_RIM_UNPK_STR(_pdu, _tknStr, _idx)                                  \
{ \
 cmMemcpy(_tknStr->val, &(_pdu[_idx]), _tknStr->len);                         \
 _idx += _tknStr->len;\
}

/* Unpacking of SI/PSI type */
#define WR_RIM_UNPK_NUM_SI_PSI_TYPE(_pdu, _noOfSi, _siType, _idx )            \
{ \
  U8 _val  = _pdu[_idx++];                                                    \
  _siType = (U8) (0x01 & _val);                                               \
  _noOfSi = (U8) (_val >> 1 );                                                \
}

/* Unpacking of PDU type */
#define WR_RIM_VAL_PDU_TYPE(_pduType, _ret )                                  \
{ \
   if ( (_pduType < WR_RIM_RI_PDU) || (_pduType > WR_RIM_RIAE_PDU))   \
   {\
     _ret = RFAILED; \
   } \
   else \
   { \
     _ret = ROK; \
   } \
}

/* Unpacking of Length */
#define WR_RIM_UNPK_LEN(_pdu, _len, _idx)                                     \
{ \
  U8 _tmpLen = _pdu[_idx++];                                                  \
  if ( _tmpLen & 0x80 ) \
  { \
    _len = (_tmpLen & 0x7F ); \
  } \
  else \
  { \
    _len = ( (_tmpLen << 8) | _pdu[_idx++] );                                 \
  } \
}

/* Unpacking of Routing discriminator */
#define WR_RIM_UNPK_ROUT_DISC(_pdu, _val, _idx)                               \
{ \
  _val = 0x0F  & _pdu[_idx++];                                                \
}

/* Unpacking of PLMN - ASN Decoding   */
#define WR_RIM_UNPK_PLMN_ID(_pdu, _plmnId, _idx)                              \
{ \
    _plmnId->mcc[0] = _pdu->val[_idx] &0x0f;                                  \
    _plmnId->mcc[1] = (_pdu->val[_idx++] &0xf0) >> 4;                         \
    _plmnId->mcc[2] = _pdu->val[_idx] &0x0f;                                  \
    if ((_pdu->val[_idx] & 0xf0) == 0xf0)                                     \
    { \
       _plmnId->numMncDigits = 2; \
       _idx++;  \
       _plmnId->mnc[1] = (_pdu->val[_idx] & 0xf0) >> 4;                       \
       _plmnId->mnc[0] = _pdu->val[_idx++] & 0x0f;                            \
    } \
    else \
    { \
       _plmnId->numMncDigits = 3; \
       _plmnId->mnc[0]       = (_pdu->val[_idx++] & 0xf0) >> 4;               \
       _plmnId->mnc[1]       = _pdu->val[_idx] & 0x0f; \
       _plmnId->mnc[2]       = (_pdu->val[_idx++] & 0xf0) >> 4;               \
    } \
}
/* Unpacking of PLMN - TLV decoding  */
#define WR_RIM_TLV_UNPK_PLMN_ID(_pdu, _plmnId, _idx)                              \
{ \
    _plmnId->mcc[0] = _pdu->val[_idx] &0x0f;                                  \
    _plmnId->mcc[1] = (_pdu->val[_idx++] &0xf0) >> 4;                         \
    _plmnId->mcc[2] = _pdu->val[_idx] &0x0f;                                  \
    if ((_pdu->val[_idx] & 0xf0) == 0xf0)                                     \
    { \
       _plmnId->numMncDigits = 2; \
       _idx++;  \
       _plmnId->mnc[1] = (_pdu->val[_idx] & 0xf0) >> 4;                       \
       _plmnId->mnc[0] = _pdu->val[_idx++] & 0x0f;                            \
    } \
    else \
    { \
       _plmnId->numMncDigits = 3; \
       _plmnId->mnc[2]       = (_pdu->val[_idx++] & 0xf0) >> 4;               \
       _plmnId->mnc[1]       = (_pdu->val[_idx] & 0xf0) >> 4;               \
       _plmnId->mnc[0]       = _pdu->val[_idx++] & 0x0f; \
    } \
}

/* Unpacking of TKN string */
#define WR_RIM_UNPK_TKN_STR4(_ie, _val)                                       \
{ \
   _val = _ie->val[0];\
   _val <<= 8; \
   _val |= _ie->val[1]; \
   _val <<= 8; \
   _val |= _ie->val[2]; \
   _val <<= 8; \
   _val |= _ie->val[3]; \
   _val >>= (32 - _ie->len); \
}

/* Unpacking of PDU indication */
#define WR_RIM_UNPK_PDU_IND(_pdu, _ackInd, _pduExt, _idx )                    \
{ \
   U8 _tmpVal; \
  _tmpVal  = _pdu[_idx++];                                                    \
  _ackInd  = (_tmpVal & 0x01 );                                               \
  _pduExt  = (0x07 & (_tmpVal >> 1));                                         \
}

/* Unpacking of RIM cause */
#define WR_RIM_FILL_RIM_CAUSE(_cause, _rimCause)                              \
{ \
  _cause->pres = PRSNT_NODEF;                                                 \
  _cause->val  = _rimCause;                                                   \
}
/* Free  meomory allocated during encoding */   
#define WR_RIM_ENC_FREE_UNUSED_MEM(_encCb)                                    \
{                                                                             \
 if ( NULLP != _encCb->rirAppCont.appContIe.val )                             \
 {                                                                            \
      WR_FREE((_encCb->rirAppCont.appContIe.val),                             \
            _encCb->rirAppCont.appContIe.len);                                \
 }                                                                            \
 if ( _encCb->sonAppId.val != NULLP )                                         \
 {                                                                            \
      WR_FREE((_encCb->sonAppId.val),_encCb->sonAppId.len);                   \
 }                                                                            \
 if ( _encCb->riAppCont.appContIe.val != NULLP )                              \
 {                                                                            \
      WR_FREE((_encCb->riAppCont.appContIe.val),                              \
            _encCb->riAppCont.appContIe.len);                                 \
 }                                                                            \
}
/* Free the memory allocated by decoder */
#define WR_RIM_DEC_FREE_UNUSED_MEM(_decCb)                                               \
{                                                                             \
   if( _decCb->rirAppCont.appContIe.val != NULLP )                            \
   { \
      WR_FREE(_decCb->rirAppCont.appContIe.val,                               \
            _decCb->rirAppCont.appContIe.len);                                \
   } \
   if( _decCb->riAppCont.appContIe.val != NULLP )                             \
   { \
      WR_FREE(_decCb->riAppCont.appContIe.val,                                \
            _decCb->riAppCont.appContIe.len);                                 \
   } \
   if( _decCb->pduInErr.val != NULLP )                                        \
   { \
      WR_FREE(_decCb->pduInErr.val,                                          \
            _decCb->pduInErr.len);                                           \
   } \
   if( _decCb->appErrCnt.appCntIe.val != NULLP )                             \
   { \
      WR_FREE(_decCb->appErrCnt.appCntIe.val,                                \
            _decCb->appErrCnt.appCntIe.len);                                 \
   } \
   if(( WR_RIM_APP_UTRA_SI == _decCb->appId.val) &&                            \
       ( _decCb->riAppCont.app.utraSI.val != NULLP ))                          \
   { \
      WR_FREE(_decCb->riAppCont.app.utraSI.val,                               \
            _decCb->riAppCont.app.utraSI.len);                                \
   } \
   if(( WR_RIM_APP_NACC == _decCb->appId.val) &&                            \
      ( _decCb->riAppCont.app.nacc.si.val != NULLP ))                           \
   { \
      WR_FREE(_decCb->riAppCont.app.nacc.si.val,                              \
            _decCb->riAppCont.app.nacc.si.len);                               \
   } \
}
/* Find the cell Index using RSN */
#define WR_RIM_FIND_CELL_IDX_USING_RSN(_assocCb, _rsn, _cellIdx) \
{ \
   U8 _idx;  \
   for ( _idx = 0; _idx < WR_RIM_MAX_CELLS; _idx++ ) \
   { \
      if ( _rsn == _assocCb->cellLst[_idx].lastSentRsn ) \
      { \
         _cellIdx = _idx; \
         break; \
      } \
   } \
   if ( _idx == WR_RIM_MAX_CELLS ) \
   { \
      _cellIdx = WR_RIM_INVALID_CELL_ID; \
   }\
}
/* Get the free cell Index */
#define WR_RIM_GET_FREE_CELL_IDX(_assocCb, _cellIdx) \
{ \
   U8 _idx;  \
   for ( _idx = 0; _idx < WR_RIM_MAX_CELLS; _idx++ ) \
   { \
      if ( TRUE != _assocCb->cellLst[_idx].isPres ) \
      { \
         _cellIdx = _idx; \
         _assocCb->cellLst[_idx].cellIdx = cellIdx; \
         _assocCb->cellLst[_idx].isPres  = TRUE; \
         _assocCb->cellLst[_idx].assocCbPtr = (PTR)assocCb; \
         break; \
      } \
   } \
   if ( _idx == WR_RIM_MAX_CELLS ) \
   { \
      _cellIdx = WR_RIM_INVALID_CELL_ID; \
   }\
}
/** @brief This structure contains the following 
   U8          enbType; 
   U32         enbId;   
   U16         tac;  
   WrPlmnId    plmnId;
*/
typedef struct wrEnbIdentifier
{
   U8          enbType;  /* enodeb type */
   U32         enbId;    /* Enodeb ID */ 
   U16         tac;      /* Tracking Area Code */
   WrPlmnId    plmnId;   /* PLMN ID */
}WrEnbIdentifier;

/** @brief This structure contains the following 
   U16         lac;
   U16         rncId;
   WrPlmnId    plmnId;
   U8          rac;     
*/
typedef struct wrRncIdentifier
{
   U16         lac;      /* Location Area Code of UTRAN neighbor cell */
   U16         rncId;    /* RNC ID of UTRAN neighbor cell */
   WrPlmnId    plmnId;   /* PLMN ID */
   U8          rac;      /* Routing area code of UTRAN neighbor cell */
} WrRncIdentifier;

/** @brief This structure contains the following 
   U16         ci;   
   U16         lac;  
   U8          rac; 
   WrPlmnId    plmnId;
*/
typedef struct wrGeranCellId
{
   U16         ci;      /* Cell Identity of GERAN neighbor cell */
   U16         lac;     /* Location area code of GERAN neighbor cell */ 
   U8          rac;     /* Routing area code of GERAN neighbor cell */
   WrPlmnId    plmnId;  /* PLMN ID */
}WrGeranCellId;

/* TODO : Reuse if any alredy exisiting structure */
/** @brief This structure contains EUTRAN Global Cell Identity
   U32          eutranCellId; 
   WrPlmnId     plmnId;
*/
typedef struct wrEutraCgi
{
   U32         eutranCellId;  /* EUTRAN Cell Identity */
   WrPlmnId    plmnId;        /* PLMN ID */
}WrEutraCgi;

/** @brief This structure contains the following 
   U16          cId; 
   U16          rncId;    
   WrPlmnId     plmnId;
*/
typedef struct wrUtraCellId
{
   U16          cId;      /* CID of UTRAN neighbor cell  */
   U16          rncId;    /* RNC Id of UTRA neighbor cell */
   WrPlmnId     plmnId;   /* PLMN ID */
}WrUtraCellId;

/** @brief This structure contains the following 
   U8              ratChoice;
   WrEnbIdentifier eutraIdentity;
   WrRncIdentifier rncIdentity;
   WrGeranCellId   geranCellId;
*/
typedef struct wrRimRoutingInfo
{
   U8   ratChoice;  /* ratChoice indicates that which RAT is chosen */
   union
   {
      WrEnbIdentifier eutraIdentity;  /* eNB identifier */
      WrRncIdentifier rncIdentity;    /* RNC Identifier */ 
      WrGeranCellId   geranCellId;    /* GERAN Cell Identifier */
   }t;
}WrRimRoutingInfo;


/** @brief This structure is used by RIM Application to configure the RIM 
 * Protocol and contains the following 
   U32               tmrRirVal;   
   U32               tmrRiVal;   
   U32               tmrRiaeVal;
   U32               maxRetryCnt; 
   S16              (*rimStartTmr)(PTR cb, S16 tmrEvnt, U32 delay); 
   S16              (*rimStopTmr)(PTR cb, S16 tmrEvnt);
*/ 
typedef struct wrRimCfg 
{
   U32               tmrRirVal;   /* RIR Timer value */
   U32               tmrRiVal;    /* RI Timer value */
   U32               tmrRiaeVal;  /* RIAE Timer Value */
   U32               maxRetryCnt; /* Maximum Retransmission count */
   WrRimRoutingInfo  selfAddr;    /* Address of the node */
   S16              (*rimStartTmr)(PTR cb, S16 tmrEvnt, U32 delay);  /* Call 
                                  back API to start timer at RIM protocol */
   Void              (*rimStopTmr)(PTR cb, S16 tmrEvnt);  /* Call 
                                  back API to stop timer at RIM protocol */
}WrRimCfg;


/** @brief This structure contains the following 
   U8            ratChoice;
   WrEutraCgi    eutranCell;
   WrUtraCellId  utraCell;
   WrGeranCellId geranCell;
*/
typedef struct wrRimRptCell
{
   U8   ratChoice;  /* ratChoice indicates that which RAT is chosen */
   union
   {
      WrEutraCgi    eutranCell;  /* EUTRAN Global Cell Identity */
      WrUtraCellId  utraCell;    /* UTRAN Source Cell ID */ 
      WrGeranCellId geranCell;   /* GERAN Cell Identifier */
   }t;
}WrRimRptCell;

/** @brief This structure used to update the RIR Application information and
 * contains the following
   WrRimRirPduExt    reptType; 
   TknStrOSXL        appCont;  
*/
typedef struct wrRimRirDecInfo
{
   WrRimRirPduExt       reptType;  /* Request Type - single/multiple/stop */
   TknStrOSXL           appCont;   /*Application container.In case if validation
                                   of Application information fails then RIM APP
                                   can send this container back RIM Protocol to
                                   include it in RIAE IE */
} WrRimRirDecInfo;

/** @brief This structure contains the following 
  U8                noOfSi; 
  WrRimSiType       siType; 
  TknStrOSXL        si;  
*/
typedef struct wrRimNacc
{
   U8                 noOfSi;   /* number of SIs */
   WrRimSiType        siType;   /* SI or PSI */
   TknStrOSXL         si;       /* SI/PSI octet string */
} WrRimNacc;

/** @brief This structure used to initiate the RIR procedure and 
 * contains the following 
   WrRimAppId         appId;    
   WrRimRirPduExt     reptType;
   WrRimRptCell       rptCellId;
   WrRimRoutingInfo   dstAddr; 
   WrRimRoutingInfo   srcAddr;  
*/  
typedef struct wrRimReqInfo
{
   WrRimAppId         appId;     /* RIM Application Identity */
   WrRimRirPduExt     reptType;  /* Request Type - multiple/stop */
   WrRimRptCell       rptCellId; /* Reporting Cell Identifier */
   WrRimRoutingInfo   dstAddr;   /* Destination Address */
   WrRimRoutingInfo   srcAddr;   /* Source Address */
}WrRimReqInfo;

/** @brief This structure used to update the the RI Application Information
 * to RIM Application and contains the following
   TknU8          appErrCause;  
   WrRimRiPduExt  rptType;  
   TknStrOSXL     utraSI; 
   WrRimNacc      nacc; 
   TknStrOSXL     appCont;  
*/ 
typedef struct wrRimRiDecInfo
{
   TknU8          appErrCause;   /* If application error casue is present then 
                                   UTRA SI or GERAN SI/PSI will not be present*/
   WrRimRiPduExt  rptType;       /* RI report Type - multiple or end */ 
   union 
   {
      TknStrOSXL      utraSI; /* UTRA SI octet string */
      WrRimNacc       nacc;   /* Indicate GERAN SI or PSI octet string */
      /* TODO: structures for the remaining applciations could 
         be included based on the support
       */
   } app;
   TknStrOSXL    appCont;       /* Application container.In case if validation 
                                   of Application information fails then RIM APP
                                   can  send this Container back RIM Protocol to
                                   include it in RIAE IE */
                       
} WrRimRiDecInfo;

/** @brief This structure contains the following 
   U8            pduType;    
*/
typedef struct wrRimRiAckDecInfo
{
   U8            pduType;    /* PDU type for which ACK is received */
}WrRimRiAckDecInfo;

/** @brief This structure contains the following 
   U8                 cause;
   WrRimAppId         appId;
*/
typedef struct wrRimRiAppErrDecInfo
{
   U8                 cause;   /* Error Cause value */
   WrRimAppId         appId;   /* RIM Application Identity */
} WrRimRiAppErrDecInfo;

/** @brief This structure contains the following 
   WrRimCause     rimErrCause;   
*/
typedef struct wrRimRiErrDecInfo
{
   WrRimCause    rimErrCause;     /* RIM cause */
} WrRimRiErrDecInfo;

/** @brief This structure used by RIM Application to request RIM protocol for
 * processing of incoming RIM PDU and contains the following
   Bool                  isDecSucc;      
   Bool                  isAckRequested; 
   U8                    rimPduType;  
   WrRimAppId            appId;         
   WrRimRirDecInfo       rirDecInfo; 
   WrRimRiDecInfo        riDecInfo; 
   WrRimRiAckDecInfo     riaDecInfo;  
   WrRimRiAppErrDecInfo  riAppErrDecInfo; 
   WrRimRiErrDecInfo     riErrDecInfo; 
   Buffer                *rimErrPdu;    
   Buffer                *rimAckPdu;    
   WrRimRoutingInfo      dstAddr;   
   WrRimRoutingInfo      srcAddr;        
   WrRimRptCell          rptCellId;  
*/
typedef struct wrRimDecRspInfo
{
   Bool           isDecSucc;          /* Decodig status of RIM PDU */
   Bool           isAckRequested;     /* Ack Requested flag, TRUE if peer 
                                         requested for Ack */
   U8             rimPduType;         /* RIM PDU type */
   WrRimAppId     appId;              /* RIM Application Identity */
   union
   {
      WrRimRirDecInfo       rirDecInfo;  /* RIR PDU Decoded Info */
      WrRimRiDecInfo        riDecInfo;   /* RI PDU  Decoded Info */
      WrRimRiAckDecInfo     riaDecInfo;  /* ACK PDU  Decoded Info */
#if 0
      /* As of now, eNodeB will not received RAN-INFORMATION-APPLICATION-ERROR
       * It is useful for future use only */ 
      WrRimRiAppErrDecInfo  riAppErrDecInfo; /* RIAE PDU Decoded Info */
#endif
      WrRimRiErrDecInfo     riErrDecInfo; /* RI Error PDU Decoded Info */
   } pdu;
   Buffer            *rimErrPdu;         /* RIM protocol will update the error
                                            PDU if isDecSucc = FALSE */
   Buffer            *rimAckPdu;         /* RIM ACK PDU-It will be present only
                                            if isAckRequested is set to TRUE */
   WrRimRoutingInfo  dstAddr;            /* Destination Address */
   WrRimRoutingInfo  srcAddr;            /* Source Address */
   WrRimRptCell      rptCellId;          /* Reporting Cell Identifier */
} WrRimDecRspInfo;

/** @brief This structure used by RIM Protocol to indicate RIM Application
 * about retransmission or procedure failure due to maximum retransmission
 * reached and contains the following
  Bool              isMaxRtxReached; 
  Buffer            *rtxPdu;         
  WrRimAppId        appId;   
  WrRimRptCell      rptCellId;     
  WrRimRoutingInfo  dstAddr;  
*/ 
typedef struct wrRimToAction
{
  Bool              isMaxRtxReached; /* It can be TRUE : Report the error , 
                                        FALSE : Resend the message to MME */
  Buffer            *rtxPdu;         /* PDU is present only if maxRtxReached
                                        is FALSE */ 
  WrRimAppId         appId;          /* RIM Application Identity */
  WrRimRptCell       rptCellId;      /* Reporting Cell Identifier */
  WrRimRoutingInfo   dstAddr;        /* Destination Address to retransmit the
                                        RIM PDU */
} WrRimToAction;

/** @brief This structure used by RIM Application to indicate the time
 * out event to RIM Protocol and contains the following
  S16          event; 
  PTR          *cb;   
*/
typedef struct wrRimTimeOut
{
  S16          event;   /* Timer event */
  PTR          cb;      /* Control block to fetch the timer */
} WrRimTimeOut;

/** @brief This structure used by RIM Protocol to indicate the RIM Application
 * about received application error in RI or RIAE PDU and contains following
   U8                cause;    
   WrRimAppId        appId;    
   WrRimRoutingInfo  dstAddr;   
   WrRimRoutingInfo  srcAddr;  
   TknStrOSXL        appCont;  
*/
typedef struct wrRimAppErr
{
   U8                cause;     /* Application Error Cause - WrRimNaccCause/
                                   WrRimSi3Cause/ WrRimUtraSiCause */
   WrRimAppId        appId;     /* RIM Application Identity */
   WrRimRoutingInfo  dstAddr;   /* Destination Address */
   WrRimRoutingInfo  srcAddr;   /* Source Address */
   WrRimRptCell      rptCellId;   /* Reporting Cell Identifier */
   TknStrOSXL        appCont;   /* Application container.In case if validation 
                                   of Application information fails then RIM
                                   Application can send this Container back to
                                   RIM Protocol to include it in RIAE IE */
}WrRimAppErr; 

/** @brief This structure used by RIM Protocol to maintain the RIM association
 * hash key and contains following
  WrRimAppId         appId;  
  WrRimRoutingInfo   dstAddr; 
  WrRimRoutingInfo   srcAddr; 
*/
typedef struct wrRimAssocHashKey
{
  WrRimAppId         appId;    /* RIM Application Identity */
  WrRimRoutingInfo   dstAddr;  /* Destination Address */
  WrRimRoutingInfo   srcAddr;  /* Source Address */
  /* TODO: SON Application identity could be included in future */
} WrRimAssocHashKey;
/** @brief This structure used by RIM Protocol cell information
 * in association control bloack 
  -  Bool               isPres        set to TRUE if the cellIdx is pres
  -  U8                 cellIdx       Cell Index
  -  U8                 rtxCount      Retransmission count 
  -  U8                 reptType      report type
  -  U8                 state;        association state
  -  Buffer            *rtxPdu;       RIR PDU -Store it for retransmission
  -  WrRimRptCell       rptCellId     Reporting Cell Identifier
  -  U32                lastSentRsn   Last sent RSN 
  -  U32                lastRcvdRsn   Last Received RSN
  -  PTR                assocCbPtr    Pointer to assocCb
  -  CmTimer            timer         RIR Timer
 */

typedef struct _wrRimDstCellInfo
{
   Bool               isPres;      /* set to TRUE if the cellIdx is pres */
   U8                 cellIdx;     /* Cell Index */
   U8                 rtxCount;    /* Retransmission count */
   U8                 reptType;    /* report type */
   U8                 state;       /* association state */
   Buffer            *rtxPdu;      /* RIR PDU -Store it for retransmission */
   WrRimRptCell       rptCellId;   /* Reporting Cell Identifier */
   U32                lastSentRsn; /* Last sent RSN */
   U32                lastRcvdRsn; /* Last Received RSN */
   PTR                assocCbPtr;  /* Pointer to assocCb */
   CmTimer            timer;       /* RIR Timer */ 
} WrRimDstCellInfo;

/** @brief This structure used by RIM Protocol to maintain the data related to
 * a particular RIM association and contains following
  - CmHashListEnt      hlEnt      Entry into the hash list      
  - WrRimAssocHashKey  hashKey    Hash key
  - U32                nextSndRsn next  RSN to be sent
  - WrRimDstCellInfo   cellLst[WR_RIM_MAX_CELLS] cell List 
*/
typedef struct wrRimAssocCb
{
   CmHashListEnt      hlEnt;       /* Entry into the hash list   */
   WrRimAssocHashKey  hashKey;     /* has Key */
   U32                nextSndRsn;  /*RSN to be used while sending next RIM PDU*/
   WrRimDstCellInfo   cellLst[WR_RIM_MAX_CELLS]; /* Cell List */
} WrRimAssocCb;

/** @brief This structure contains the following
   Bool        pres;
   Bool        isAckRequested;
   U8          pduTypExt;
*/
typedef struct wrRimPduInd
{
   Bool        pres;             /* Indicates PDU ind is TRUE or FALSE */
   Bool        isAckRequested;   /* Ack is requested or not */
   U8          pduTypExt;        /* PDU type extension */
} WrRimPduInd;

/** @brief This structure contains the following
   Bool               pres;
   WrRimRptCell       rptCellId;     
   TknStrOSXL         appContIe;     
*/
typedef struct wrRimRirAppCont
{
   Bool               pres;          /* RIR App container is present or not */
   WrRimRptCell       rptCellId;     /* Reporting Cell Identifier */
   TknStrOSXL         appContIe;     /* App contianer including IEI and Len */
} WrRimRirAppCont;

/** @brief This structure contains the following
   Bool               pres;
   WrRimRptCell       rptCellId;  
   TknStrOSXL         utraSI;       
   WrRimNacc          nacc;     
   TknStrOSXL         appContIe; 
 */
typedef struct wrRimRiAppCont
{
   Bool               pres;          /* RI App Container is present or not */
   WrRimRptCell       rptCellId;     /* Reporting Cell Identifier */
   union 
   {
      TknStrOSXL      utraSI;        /* UTRA SI Octet string */
      WrRimNacc       nacc;          /* NACC info received from Geran Cell */
      /* TODO: structures for the remaining applciations could 
         be included based on the support
       */
   }app;
   TknStrOSXL     appContIe;         /* App contianer including IEI and Len */
} WrRimRiAppCont;

/** @brief This structure contains the following
  TknStrOSXL    appCntIe;
  U8            appCause;
 */
typedef struct wrRimAppErrCnt
{
  TknStrOSXL    appCntIe;     /* App Container IE */
  U8            appCause;     /* error cause value */
} WrRimAppErrCnt;

/** @brief This structure used by RIM Protocol to send/receive the information
 * to/from TLV encoder/decoder and contains following
   U8                 pduType;     
   TknU8              appId;      
   TknU8              sonAppId;    
   TknU8              version;      
   TknU8              decFailCause;   
   TknU8              rimCause;   
   TknU32             rsn;
   WrRimPduInd        pduInd;
   WrRimRirAppCont    rirAppCont;
   WrRimRiAppCont     riAppCont;
   TknStrOSXL         pduInErr;
   WrRimAppErrCnt     appErrCnt;
   WrRimRoutingInfo   srcAddr;    
   WrRimRoutingInfo   dstAddr;   
*/
typedef struct wrRimEncDecCb
{
   U8                 pduType;       /* RIM PDU Type  */
   TknU8              appId;         /* RIM Application Id */
   TknU8              version;       /* RIM Protocol Version */
   TknU8              decFailCause;  /* RIM prorocol error cause */
   TknU8              rimCause;      /* RIM error cause */
   TknU32             rsn;           /* RSN */
   WrRimPduInd        pduInd;        /* PDU indication */ 
   WrRimRirAppCont    rirAppCont;    /* RIR Application container */
   WrRimRiAppCont     riAppCont;     /* RI Application container */
   TknStrOSXL         pduInErr;      /* PDU having error */
   TknStrOSXL         sonAppId;      /* SON application Id */
   WrRimAppErrCnt     appErrCnt;     /* Application error container */
   WrRimRoutingInfo   srcAddr;       /* Source Adress */
   WrRimRoutingInfo   dstAddr;       /* Destination Adress */
} WrRimEncDecCb;

/** @brief This structure used by RIM Protocol to to hold all the RIM Protocol
 * related parameters as following 
   CmHashListCp      assocCbHL;               
   U32               assocCount;  
   U32               rirTmrVal; 
   U32               riTmrVal; 
   U32               riaeTmrVal;     
   U32               maxRetryCnt;
   S16               (*rimStartTmr)(PTR cb, S16 tmrEvnt, U32 delay);
   Void              (*rimStopTmr)(PTR cb, S16 tmrEvnt); 
*/
typedef struct wrRimCb
{
   CmHashListCp      assocCbHL;                  /* Association Hash List CP */
   U32               assocCount;                 /* Number of association  */
   U32               rirTmrVal;                  /* RIR  Timer value */
   U32               riTmrVal;                   /* RI   Timer value */
   U32               riaeTmrVal;                 /* RIAE Timer value */    
   U32               maxRetryCnt;                /* Maximum RIM PDU 
                                                    Retransmission count */
   WrRimRoutingInfo  selfAddr;                   /* Address of the node */
   S16              (*rimStartTmr)(PTR cb, S16 tmrEvnt, U32 delay); 
                                                /*callback API to start timer*/
   Void             (*rimStopTmr)(PTR cb, S16 tmrEvnt);  /* Call 
                                  back API to stop timer at RIM protocol */
} WrRimCb;


EXTERN S16 wrRimCfgReq(WrRimCfg *cfg);
EXTERN S16 wrRimRirProcReq(WrRimReqInfo *rimReqInfo, Buffer **rirPdu);
EXTERN S16 wrRimTimoutHdl(WrRimTimeOut *rimTO, WrRimToAction *timeOutAction);
EXTERN S16 wrRimShutdown(Void); 
EXTERN S16 wrRimPrcRcvdPdu(TknStrOSXL *rimPdu, WrRimDecRspInfo *rimInfo );
EXTERN S16 wrRimRiErrReq(WrRimAppErr *appErrInfo, Buffer **riPdu);
EXTERN S16 wrRimRiaeProcReq (WrRimAppErr *appErrInfo, Buffer **riaePdu);
EXTERN S16 wrRimEncPdu(WrRimEncDecCb *rimEncCb, Buffer *rimPdu);
EXTERN S16 wrRimDecPdu(TknStrOSXL *rimPdu, WrRimEncDecCb *rimDecCb);
EXTERN S16 wrRimAbortRirProc(Bool abortAll, WrRimReqInfo *rimReqInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_EMM_RIM_H__ */


/********************************************************************30**

           End of file:     wr_emm_rim.h@@/main/Br_Tenb_Rim_Csfb_Intg/7 - Mon Nov  3 14:36:09 2014

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
/main/1        ---      nu         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

