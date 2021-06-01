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

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_utils.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=82;



#include "wr.h"
#include "cz.h"
#include "wr_umm_trans.h"
#include "wr_utils.h"
#include "wr_smm_smallcell.h"
#include "wr_umm_ecsfb_utils.h"
#include "wr_umm_csfbupdtcell.h" /*rel10*/
#include "wr_umm_meas_fun.h" /*rel10*/
#include "wr_emm_ecsfb_utils.h"
#include "wr_kpi.h"
#include <sys/time.h>
#define WR_X2AP_MAX_MSG_PROC_CODE 16

EXTERN U16 szElmSize[][SZT_MAX_PROC_ID];
EXTERN U16 czUtlGetElmSize[][WR_X2AP_MAX_MSG_PROC_CODE];
EXTERN WrEmmCb   wrEmmCb;
EXTERN S16 wrChkCpuLoadAndUpdInstr(U8 *instruction, CmLteCellId cellId);

PUBLIC Void wrUtlPrntIncTansType2
(
U8                           transTyp,
U16                          crnti,
U16                          msgTyp,
U32                          transId
);
PUBLIC S16 wrFillSectorId
(
TknStrOSXL                  *ptr,
U8                           len,
Data                         *val
);

PUBLIC Void wrPrintS1apCauseInfo
(
U8                           causeType,
U8                           causeVal
);

EXTERN CmLListCp * wrEmmGetUtraFddFreqLst
(
 WrCellId                 cellId
);

EXTERN Bool wrUmmChkRegPlmnCells
(
WrUeCb						  *ueCb,
WrNrUtranFreqCb			  *freqNode,
U16                       *pscCount,
U16                        *pscLst
);
MiscCause s1apMiscFailCause[8] = 
{
	{L_EVENT,"Control Processing Overload"},
	{L_EVENT,"Not enough User Plane Processing Resources"},
	{L_EVENT,"Hardware Failure"},
	{L_EVENT,"O&M InterventionEnum"},
	{L_EVENT,"Unspecified"},
	{L_EVENT,"Unknown PLMN"}
};

TportCause s1apTportFailCause[4] = 
{
	{L_EVENT,"Transport Resource Unavailable"},
	{L_EVENT,"Unspecified"}
};

RnwCause s1apRnwFailCause[64] =
{
   {L_EVENT,"Unspecified"},
   {L_EVENT,"TX2RELOCOverall Expiry"},
   {L_EVENT,"Successful Handover"},
   {L_EVENT,"Release due to E-UTRAN Generated Reason"},
   {L_EVENT,"Handover Cancelled"}, 
   {L_EVENT,"Partial Handover"}, 
   {L_EVENT,"Handover Failure In Target EPC/eNB Or Target System"},
   {L_EVENT,"Handover Target not allowed"},
   {L_EVENT,"TS1RELOCoverall Expiry"},
   {L_EVENT,"TS1RELOCprep Expiry"},
   {L_EVENT,"Cell not available"},
   {L_EVENT,"Unknown Target ID"},
   {L_EVENT,"No Radio Resources Available in Target Cell"},
   {L_EVENT,"Unknown or already allocated MME UE S1AP ID"},
   {L_EVENT,"Unknown or already allocated eNB UE S1AP ID"},
   {L_EVENT,"Unknown or inconsistent pair of UE S1AP ID"},
   {L_EVENT,"Handover desirable for radio reasons"},
   {L_EVENT,"Time critical handover"},
   {L_EVENT,"Resource optimisation handover"},
   {L_EVENT,"Reduce load in serving cell"}, 
   {L_EVENT,"User inactivity"},
   {L_EVENT,"Radio Connection With UE Lost"}, 
   {L_EVENT,"Load Balancing TAU Required"}, 
   {L_EVENT,"CS Fallback Triggered"},
   {L_EVENT,"UE Not Available For PS Service"}, 
   {L_EVENT,"Radio resources not available"},
   {L_EVENT,"Failure in the Radio Interface Procedure"},
   {L_EVENT,"Invalid QoS combination"}, 
   {L_EVENT,"Inter-RAT redirection"},
   {L_EVENT,"Interaction with other procedure"}, 
   {L_EVENT,"Unknown E-RAB ID"},
   {L_EVENT,"Multiple E-RAB ID instances"}, 
   {L_EVENT,"Encryption and/or integrity protection algorithms not supported"}, 
   {L_EVENT,"S1 intra system Handover triggered"}, 
   {L_EVENT,"S1 inter system Handover triggered"}, 
   {L_EVENT,"X2 Handover triggered"} ,
   {L_EVENT,"Redirection towards 1xRTT"},
   {L_EVENT,"Not supported QCI value"},
   {L_EVENT,"invalid CSG Id"}
};

NasCause s1apNasFailCause[8] =
{
	{L_EVENT,"Normal Release"},
	{L_EVENT,"Authentication Failure"},
	{L_EVENT,"Detach"},
	{L_EVENT,"Unspecified"},
	{L_EVENT,"CSG Subscription Expiry"}
};

ProtCause s1apProtFailCause[8] =
{
   {L_EVENT,"Transfer Syntax Error"},
   {L_EVENT,"Abstract Syntax Error (Reject)"},
   {L_EVENT,"Abstract Syntax Error (Ignore and Notify)"},
   {L_EVENT,"Message not Compatible with Receiver State"},
   {L_EVENT,"Semantic Error"},
   {L_EVENT,"Abstract Syntax Error (Falsely Constructed Message)"},
   {L_EVENT,"Unspecified"}
};



/**
 *  @brief This function prints the transaction for which the message is
 *         reveived
 *
 *      Function: wrUtlPrntIncTansType
 *
 *          Processing steps:
 *          - check for the type of transaction
 *          - print the transaction type
 *
 *  @param[in]  transTyp : transaction type
 *  @return void
 */
PUBLIC Void wrUtlPrntIncTansType
(
U8                           transTyp,
U16                          crnti,
U16                          msgTyp,
U32                          transId
)
{
   switch(transTyp)
   {
      case  WR_UMM_RRC_SETUP_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_RRC_SETUP_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_INIT_CTXT_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_INIT_CTXT_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_ERAB_SETUP_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_ERAB_SETUP_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_UL_NAS_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_UL_NAS_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_DL_NAS_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_DL_NAS_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_LC_RAB_RLS_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_LC_RAB_RLS_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_RRC_REESTAB_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_RRC_REESTAB_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_CTXT_REL_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_CTXT_REL_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_RAB_REL_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_RAB_REL_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_ERAB_MOD_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_ERAB_MOD_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_UE_CTXT_MOD_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_UE_CTXT_MOD_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case  WR_UMM_MEAS_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_MEAS_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case WR_UMM_ECSFB_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_ECSFB_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case WR_UMM_DL_CDMA_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_DL_CDMA_TRANS (%ld) is reveived", msgTyp, transId);
         break;
      case WR_UMM_RRC_CSFB_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_RRC_CSFB_PARAMS_TRANS (%ld) is reveived", msgTyp, transId);
         break;
       case WR_UMM_TX_MODE_CHNG_TRANS:
          RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
                "WR_UMM_TX_MODE_CHNG_TRANS (%ld) is reveived", msgTyp, transId);
          break;
         /* LTE_ADV starts */
      case WR_UMM_LTE_ADV_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_LTE_ADV_TRANS (%ld) is reveived", msgTyp, transId);
         break;
         /* LTE_ADV ends */
      default:
         wrUtlPrntIncTansType2(transTyp, crnti, msgTyp, transId);
         break;
   }
}

/**
 *  @brief This function copies the transport address from source to
 *         destination
 *
 *      Function: wrCpyCmTptAddr
 *
 *          Processing steps:
 *          - check for the type of IP version
 *          - copy the IP address and port from source pointer to destination
 *            pointer
 *
 *  @param[out] dst : destination pointer to be updated with address
 *  @param[in]  src : source pointer containing IP address
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrCpyCmTptAddr
(
CmTptAddr                    *dst,
CmTptAddr                    *src
)
{
   if((src == NULLP) || (dst == NULLP))
   {
      RLOG0(L_ERROR, "Src & Dst Invalid");
      RETVALUE(RFAILED);
   }

   dst->type = src->type;
   if(src->type == CM_INET_IPV4ADDR_TYPE)
   {
      dst->u.ipv4TptAddr.port = src->u.ipv4TptAddr.port;
      dst->u.ipv4TptAddr.address = src->u.ipv4TptAddr.address;
   }
   else if(src->type == CM_INET_IPV6ADDR_TYPE)
   {
      dst->u.ipv6TptAddr.port = src->u.ipv6TptAddr.port;
      cmMemcpy((U8*)dst->u.ipv6TptAddr.ipv6NetAddr, 
                (U8*)src->u.ipv6TptAddr.ipv6NetAddr, sizeof(CmInetIpAddr6));
   }

   RETVALUE( ROK);
} /* end of wrCpyCmTptAddr */ 

/**
 *  @brief This function copies the transport address from source to
 *         destination network address format
 *
 *      Function: wrCpyTptToNetAddr 
 *
 *          Processing steps:
 *          - check for the type of IP version
 *          - copy the IP address from source pointer to destination
 *            pointer
 *
 *  @param[out] dst : destination pointer to be updated with address
 *  @param[in]  src : source pointer containing IP address
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrCpyTptToNetAddr
(
CmNetAddr                    *dst,
CmTptAddr                    *src
)
{
   if((src == NULLP) || (dst == NULLP))
   {
      RLOG0(L_ERROR, "Src & Dst Invalid");
      RETVALUE(RFAILED);
   }

   dst->type = src->type;
   if(src->type == CM_INET_IPV4ADDR_TYPE)
   {
      dst->u.ipv4NetAddr = src->u.ipv4TptAddr.address;
   }
   else if(src->type == CM_INET_IPV6ADDR_TYPE)
   {
      cmMemcpy((U8*)dst->u.ipv6NetAddr, 
                (U8*)src->u.ipv6TptAddr.ipv6NetAddr, sizeof(CmIpv6NetAddr));
   }

   RETVALUE( ROK);
} /* end of wrCpyTptToNetAddr */ 


/**
 *  @brief This function validates the IP address present in string notation 
 *         for both IPv4 and IPv6 and store them in binary notation. 
 *
 *      Function: wrUpdateIpAddr 
 *
 *          Processing steps:
 *          - check for the type of IP address from the input ip address
 *            in string notation
 *          - Convert the string notation to binary notation ip address
 *            format
 *          - Copy the binary notation ip address to destination pointer
 *
 *  @param[out] binIpAddr : destination pointer to be updated with address
 *  @param[in]  strIpAddr : source pointer containing IP address in string format
 *  @return S32
    -# Success : ROK
    -# Failure : RFAILED
 *
 */
#ifdef ANSI
PUBLIC S32 wrUpdateIpAddr 
(
U8 *strIpAddr, 
CmTptAddr *binIpAddr
)
#else
PUBLIC S32 wrUpdateIpAddr (strIpAddr, binIpAddr)
U8 *strIpAddr;
CmTptAddr *binIpAddr;
#endif
{ 
   S32 ret;
   CmInetAddrInfo addrInfo;
   CmInetAddrInfo *res;
   CmInetAddrInfo *tmp;
   Void *ptr;    
 
   TRC2(wrUpdateIpAddr);
 
   ret = ROK;

   cmMemset((U8*)&addrInfo, 0, sizeof(CmInetAddrInfo));

   addrInfo.ai_family = PF_UNSPEC;
   addrInfo.ai_socktype = SOCK_STREAM;
   addrInfo.ai_flags |= AI_CANONNAME;
   ret = cmInetGetAddrInfo((S8 *)strIpAddr, NULLP,&addrInfo,&res);
   if(ret != ROK)
   {
      //RLOG_STR(L_ERROR, "Validation of IP address failed:[%s]",(S8 *)strIpAddr);
      printf("Validation of IP address failed:[%s]\n",(S8 *)strIpAddr);
      RETVALUE(ret);
   }
  
   tmp = res;
   if (res)
   {
      switch (res->ai_family)
      {
        case AF_INET:
          ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
          binIpAddr->type = CM_TPTADDR_IPV4;
          cmMemcpy((U8 *)(&binIpAddr->u.ipv4TptAddr.address),ptr, 4);
          binIpAddr->u.ipv4TptAddr.address = 
             CM_INET_HTON_U32(binIpAddr->u.ipv4TptAddr.address);
          break;
        case AF_INET6:
          ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
          binIpAddr->type = CM_TPTADDR_IPV6;
          cmMemcpy((U8 *)(&binIpAddr->u.ipv6TptAddr.ipv6NetAddr), ptr, 16);
          break;
      }
      res = res->ai_next;
   }

   cmInetFreeAddrInfo(tmp);
   RETVALUE(ret);
}


/**
 *  @brief This function  Retrieve message index
 *
 *      Function: wrSztUtilsGetMsgIdx
 *
 *          Processing steps:
 *          - get the message type from the PDU
 *          - switch to the message type
 *             - if the message present 
 *                - update the message Index
 *             - else
 *                - inialize the message index
 *
 *  @param[out] msgIdx : pointer for returning the message index
 *  @param[in]  pdu    : pointer to the S1AP PDU
 *  @return S16 
    -# Success : ROK
    -# Failure : RFAILED 
 */

PUBLIC S16 wrSztUtilsGetMsgIdx
(
U8                           *msgIdx,
S1apPdu                      *pdu
)
{
   U8                        msgType;
   U8                        procCode;

   if(pdu->pdu.choice.pres == TRUE)
   {
      msgType  = pdu->pdu.choice.val;
   }
   else
   {
      *msgIdx = SZT_MSG_ID_NO_DATA;
      RETVALUE(ROK);
   }
   switch(msgType)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         if(pdu->pdu.val.initiatingMsg.pres.pres == TRUE)
         {
            procCode = pdu->pdu.val.initiatingMsg.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else 
         { 
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      }
      case S1AP_PDU_SUCCESSFULOUTCOME:
      {
         if(pdu->pdu.val.successfulOutcome.pres.pres == TRUE)
         {
            procCode = pdu->pdu.val.successfulOutcome.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else
         {
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      }
      case S1AP_PDU_UNSUCCESSFULOUTCOME:
      {
         if(pdu->pdu.val.unsuccessfulOutcome.pres.pres == TRUE)
         {
            procCode = pdu->pdu.val.unsuccessfulOutcome.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else
         {
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      }
      default:
      {
         break;
      }
   }
   RETVALUE(ROK);

}

/** @var wrUtlX2MsgMap 
    This static table gives the mapping of X2AP message type's procedure code 
    and choice values to Trillium's X2AP message type representation.
    @note For a list of definitions of Trillium's X2AP message types refer
          to czt.h
 */
PUBLIC S16 wrUtlX2MsgMap[CZ_MAX_MSG_PROC_CODE][CZ_MAX_MSG_CHOICE_VAL] = 
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
   }
};

PUBLIC U16 wrUtlX2GetElmSize[][CZ_MAX_MSG_PROC_CODE] = 
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
/*
 *
 *      Fun:   wrUtlX2GetMsgIdx
 *
 *      Desc:  Retrieve X2AP message index
 *
 *      Ret:   czMsgVal - message value - ok
 *             CZT_MSG_UNKNOWN          - failure
 *
 *      Notes: None.
 *
 *
 */
PUBLIC S16 wrUtlX2GetMsgIdx
(
CztX2AP_PDU   *pdu
)
{
   S16 czMsgVal = CZT_MSG_UNKNOWN;
   S16 msgChoice = -1;
   U8  procCode;

   TRC2(wrUtlX2GetMsgIdx)

   if(pdu == NULLP)
      RETVALUE(RFAILED);

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
            procCode = pdu->val.initiatingMsg.procedureCode.val;
            czMsgVal = wrUtlX2MsgMap[procCode][msgChoice];
         }
      }
      break;
      case X2AP_PDU_SUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.successfulOutcome.pres.pres)
         {
            procCode = pdu->val.successfulOutcome.procedureCode.val;
            czMsgVal = wrUtlX2MsgMap[procCode][msgChoice];
         }
         break;
      }
      case X2AP_PDU_UNSUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.unsuccessfulOutcome.pres.pres)
         {
            procCode = pdu->val.unsuccessfulOutcome.procedureCode.val;
            czMsgVal = wrUtlX2MsgMap[procCode][msgChoice];
         }
         break;
      }
      default:
      break;
   }

   RETVALUE(czMsgVal);
}/* wrUtlX2GetMsgIdx */

/*
 *
 *      Fun:   wrUtlX2GetMsgIdx
 *
 *      Desc:  Retrieve X2AP message index
 *
 *      Ret:   czMsgVal - message value - ok
 *             CZT_MSG_UNKNOWN          - failure
 *
 *      Notes: None.
 *
 *
 */
#ifdef ANSI
PUBLIC S16 wrUtlX2GetCztIE
(
CztX2AP_PDU *pdu,
U32         id,
void       **ie
)
#else
PUBLIC S16 wrUtlX2GetCztIE (pdu, id, ie)
CztX2AP_PDU *pdu;
U32         id;
void       **ie;
#endif
{
   TknU8   *currStr;   /* Start definition */
   TknU8   *startStr;   /* Start event structure */
   U8      choiceType;
   U16      numComp;
   U16      recIdx;
   U32      recId;
   U32      procCode;
   U16      size;

   TRC2(wrUtlX2GetCztIE)

   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie = NULLP;

   numComp = 0;
   recIdx = 0; 

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      RETVALUE(ROK);
   }
   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case X2AP_PDU_INITIATINGMSG:
         {
            if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
            {
               RETVALUE(ROK);
            }
            startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
            procCode  = pdu->val.initiatingMsg.procedureCode.val;
            break;
         }
      case X2AP_PDU_SUCCESSFULOUTCOME:
         {
            if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
            { 
               RETVALUE(ROK);
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
               RETVALUE(ROK);
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
      RETVALUE(ROK);
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
         recId   = ((CztProtIE_ID *)currStr)->val; 

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(CztProtIE_ID) + sizeof(CztCriticality));
            //*ie = startStr;
            *(TknU8 **)ie = startStr;
            break;
         }
      }
      size = wrUtlX2GetElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   RETVALUE(ROK);
} /* end of wrUtlX2GetCztIE */

/** @brief This function determines whether the eNB is Source or Target 
 *         for the corresponding UE, and gets the x2ap ue id depending on it.
 *
 * @details
 *
 *     Function: wrUtlX2GetX2UeId
 *
 *         Processing steps:
 *         - determine whether eNB is source or target based on msgType
 *         - if source, then
 *           - fetch old_enb_x2ap_id 
 *         - else
 *           - fetch new_enb_x2ap_id
 *
 * @param[in]  msgType : incoming x2ap msg type
 * @param[in]  pdu     : x2ap pdu
 * @param[out] ueX2apid: ue x2ap id
 * @return S16
 *     - ROK
 */
PUBLIC S16 wrUtlX2GetX2UeId
(
S16                          msgType,
CztX2AP_PDU                  *pdu,
U32                          *ueX2apId
)
{
   TknU8                     *ie;
   U32                       ieId = 0;
   switch(msgType)
   {
      case CZT_MSG_HO_ACK:
      case CZT_MSG_HO_FAIL:
      case CZT_MSG_UE_CXT_RELEASE:
         ieId = Cztid_Old_eNB_UE_X2AP_ID;
      break;
      case CZT_MSG_HO_CANCEL:
      case CZT_MSG_SN_STATUS_TRFR:
         ieId = Cztid_New_eNB_UE_X2AP_ID;
      break;
      default:
      {
        RLOG1(L_ERROR, "UE X2AP ID not found: msgType[%d]", msgType);
        RETVALUE(RFAILED);
      }
   }
   wrUtlX2GetCztIE(pdu, ieId, (void **)&ie);
   if(ie == NULLP)
   {
      RLOG1(L_ERROR, "UE X2AP ID not found ieId [%ld]", ieId);
      RETVALUE(RFAILED);
   }
   *ueX2apId = ((CztUE_X2AP_ID*)ie)->val;
   RETVALUE(ROK);
} /* end of wrUtlX2GetX2UeId */

/**
 *  @brief This function fills the token and value for an tknU8 datatype
 *
 *      Function: wrFillTknU8
 *
 *          Processing steps:
 *          - update token present field as TRUE
 *          - update the value received
 *
 *
 *  @param[out] encParam : pointer the tokenparameter
 *  @param[in]  val      : value to be updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16  wrFillTknU8
(
TknU8                        *encParam,
U8                           val
)
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
} /* end of wrFillTknU8 */ 

/**
 *  @brief This function fills the token and value for  TknU16 structure
 *
 *      Function: wrFillTknU16
 *
 *          Processing steps:
 *          - update token present field as TRUE
 *          - update the value received
 *
 *
 *  @param[out] encParam : pointer the tokenparameter
 *  @param[in]  val      : value to be updated
 *  @return  ROK
 */
PUBLIC S16  wrFillTknU16
(
TknU16                       *encParam,
U16                          val
)
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
                                    
}/* end of wrFillTknU16 */

/**
 *  @brief This function fills the token and value for  TknS32 structure
 *
 *      Function: wrFillTknS32
 *
 *          Processing steps:
 *          - update token present field as TRUE
 *          - update the value received
 *
 *
 *  @param[out] encParam : pointer the tokenparameter
 *  @param[in]  val      : value to be updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16  wrFillTknS32
(
TknS32                         *encParam,
S32                            val
)
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
} /* end of wrFillTknS32 */

/**
 *  @brief This function fills the token and value for  TknU32 structure
 *
 *      Function: wrFillTknU32
 *
 *          Processing steps:
 *          - update token present field as TRUE
 *          - update the value received
 *
 *
 *  @param[out] encParam : pointer the tokenparameter
 *  @param[in]  val      : value to be updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16  wrFillTknU32
(
TknU32                         *encParam,
U32                            val
)
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
} /* end of wrFillTknU32 */

/**
 *  @brief This function fills the structure TknStrOSXL
 *
 *      Function: wrFillTknStrOSXL
 *
 *          Processing steps:
 *          - allocate memory to val in TknStrOSXL
 *          - update token present field as TRUE
 *          - update the memory with the value  received
 *
 *
 *  @param[out] ptr  : TknStrOSXL stucture pointer to be updated
 *  @param[in]  len  : length for which memory to allocated
 *  @param[in]  val  : value getting updated
 *  @param[in]  mem  : pointer to memCp
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrFillTknStrOSXL
(
TknStrOSXL                   *ptr,
U16                          len,
Data                         *val,
CmMemListCp                  *mem
)
{
   U16                       idx;

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(mem, len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}


/**
 *  @brief This function fills the structure TknStrBSXL
 *
 *      Function: wrFillTknStrBSXL
 *
 *          Processing steps:
 *          - allocate memory to val in TknStrBSXL
 *          - update token present field as TRUE
 *          - update the memory with the value  received
 *
 *
 *  @param[out] ptr  : TknStrBSXL stucture pointer to be updated
 *  @param[in]  len  : length for which memory to allocated
 *  @param[in]  val  : value getting updated
 *  @param[in]  mem  : pointer to memCp
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */

PUBLIC S16 wrFillTknStrBSXL
(
TknStrBSXL                   *ptr,
U16                          len,
Data                         *val,
CmMemListCp                  *mem
)
{
   U16                       idx;

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(mem, (len + 7)/8 , (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < (len + 7)/8; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function fills the structure TknStrOSXL1
 *
 *      Function: wrFillTknStrOSXL1
 *
 *          Processing steps:
 *          - allocate memory to val in TknStrOSXL1
 *          - update token present field as TRUE
 *          - update the memory with the value  received
 *
 *
 *  @param[out] ptr : TknStrOSXL stucture pointer to be updated
 *  @param[in]  len : length for which memory to allocated
 *  @param[in]  val : value getting updated
 *  @param[in]  mem : pointer to memCp
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */

PUBLIC S16 wrFillTknStrOSXL1
(
TknStrOSXL                   *ptr,
U16                          len,
U32                          val,
CmMemListCp                  *mem
)
{
   U16                       idx;
   U8                        tmpVal;

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(mem, len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      tmpVal = (U8)(val >> (8 * idx));
      ptr->val[len - (idx + 1)] = tmpVal;

   }

   RETVALUE(ROK);
}




/**
 *  @brief This function fills the structure TknStr4U32
 *
 *      Function: wrFillTknStr32
 *
 *          Processing steps:
 *          - update token present field as TRUE
 *          - fill Bit string of specified length
 *
 *
 *  @param[out] ptr  : TknBStr32 stucture pointer to be updated
 *  @param[in]  len  : length of message to be updated
 *  @param[in]  val  : value getting updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrFillTknBStr32
(
TknBStr32                    *ptr,
U16                          len,
U32                          val
)
{
   U8                        idx;
   U8                        octets[4];

   /* make the element as present */
   if (len > 32)
   {
      RETVALUE(RFAILED);
   }

   octets[0] = ((val << (32 - len)) & 0xff000000) >> 24;
   octets[1] = ((val << (32 - len)) & 0x00ff0000) >> 16;
   octets[2] = ((val << (32 - len)) & 0x0000ff00) >> 8;
   octets[3] = ((val << (32 - len)) & 0x000000ff);
   ptr->pres = TRUE;
   ptr->len  = len;
   for(idx = 0; idx < 4; idx++)
   {
      ptr->val[idx] = octets[idx];
   }

   RETVALUE(ROK);
} /* end of wrFillTknBStr32 */


/**
 *  @brief This function fills the Octet string of length less than 4
 *
 *      Function: wrFillTknStr4
 *
 *          Processing steps:
 *          - if number of bytes more than four reurn failure
 *          - update token present field as TRUE
 *          - fill Byte string of specified length
 *
 *  @param[out] ptr  : TknStr4 stucture pointer to be updated
 *  @param[in]  len   : length of message to be updated
 *  @param[in]  val  : value getting updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrFillTknStr4
(
TknStr4                      *ptr,
U8                           len,
U32                          val
)
{
   U8                        idx;
   U32                       adjVal = val << ((4 - len) * 8);

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = (adjVal & 0xff000000) >> 24;
      adjVal <<= 8;
   }

   RETVALUE(ROK);
} /* end of wrFillTknStr4 */

/**
 *  @brief This function  Retrieve U32 from Str4 token string.
 *
 *      Function: wrGetU32FrmTknStr4
 *
 *          Processing steps:
 *           - Convert token str4 to U32
 *
 *  @param[out] val : pointer to value
 *  @param[in]  ie  : pointer to token srting
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */

PUBLIC S16 wrGetU32FrmTknStr4
(
   U32                       *val,
   TknStr4                   *ie
)
{
   U8           len = ie->len;
   U8           idx;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }

   *val = 0;
   for (idx = 0; idx < len; idx++)
   {
      *val = (*val<< 8) + ie->val[idx];
   }
   RETVALUE(ROK);
}

 /**
 *  @brief This function  Retrieve U16 from Str4 token string.
 *
 *      Function: wrGetU16FrmTknStr4
 *
 *          Processing steps:
 *           - Convert token str4 to U16
 *
 *  @param[out] val : pointer to value
 *  @param[in]  ie  : pointer to token srting
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */

PUBLIC S16 wrGetU16FrmTknStr4
(
   U16                       *val,
   TknStr4                   *ie
)
{
   U8           len = ie->len;
   U8           idx;

   if (len > 2)
   {
      RETVALUE(RFAILED);
   }

   *val = 0;
   for (idx = 0; idx < len; idx++)
   {
      *val = (*val<< 8) + ie->val[idx];
   }
   RETVALUE(ROK);
}
/**
 *  @brief This function fills the Octet string of length less than 4
 *
 *      Function: wrFill2TknStr4
 *
 *          Processing steps:
 *          - if number of bytes more than four reurn failure
 *          - update token present field as TRUE
 *          - fill Byte string of specified length
 *
 *  @param[out] ptr  : TknStr4 stucture pointer to be updated
 *  @param[in]  len  : length of message to be updated
 *  @param[in]  val  : value getting updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrFill2TknStr4
(
TknStr4                      *ptr,
U8                           len,
U8                           *val
)
{
   U8                        idx;

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
} /* end of wrFillTknStr4 */

/**
 *  @brief This function validate the two PLMNs are equal or not
 *
 *      Function: wrUtlPlmnsEqual
 *
 *          Processing steps:
 *          -  validate MCC, MNC size and MNC
 *          -  if matches 
 *             - return TRUE
 *          - else 
 *             - return FALSE
 *
 *  @param[in]  plmnId1 : PLMN1 to be validated
 *  @param[in]  plmnId2 : PLMN2 to be validated
 *  @return Bool
 */
PUBLIC Bool wrUtlPlmnsEqual
(
WrPlmnId                     *plmnId1,
WrPlmnId                     *plmnId2
)
{
   if ((plmnId1->mcc[0] != plmnId2->mcc[0]) ||
       (plmnId1->mcc[1] != plmnId2->mcc[1]) ||
       (plmnId1->mcc[2] != plmnId2->mcc[2]))
   {
      RETVALUE(FALSE);
   }
   if (plmnId1->numMncDigits != plmnId2->numMncDigits)
   {
      RETVALUE(FALSE);
   }
   if ((plmnId1->mnc[0] != plmnId2->mnc[0]) ||
       (plmnId1->mnc[1] != plmnId2->mnc[1]))
   {
      RETVALUE(FALSE);
   }
   if (plmnId1->numMncDigits == 3)
   {
      if (plmnId1->mnc[2] != plmnId2->mnc[2])
      {
         RETVALUE(FALSE);
      }
   }
   RETVALUE(TRUE);
}

/**
 *  @brief This is an utility function to get WrPlmnID from the message
 *
 *      Function: wrUtlGetPlmnId
 *
 *          Processing steps:
 *          -  decode the PLMN from the message 
 *          - update it to the pointer provided
 *
 *  @param[out] plmnId : pointer to update the PLMN
 *  @param[in]  plmnIe : pointer to message containing PLMN IE
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUtlGetPlmnId
(
WrPlmnId                     *plmnId,
SztPLMNidentity              *plmnIe
)
{
    plmnId->mcc[0] = plmnIe->val[0] &0x0f;
    plmnId->mcc[1] = (plmnIe->val[0] &0xf0) >> 4;
    plmnId->mcc[2] = plmnIe->val[1] &0x0f;
    if ((plmnIe->val[1] & 0xf0) == 0xf0)
    {
       plmnId->numMncDigits = 2;
       plmnId->mnc[1]       = (plmnIe->val[2] & 0xf0) >> 4;
       plmnId->mnc[0]       = plmnIe->val[2] & 0x0f;
    }
    else
    {
       plmnId->numMncDigits = 3;
       plmnId->mnc[0]       = (plmnIe->val[1] & 0xf0) >> 4;
       plmnId->mnc[1]       = plmnIe->val[2] & 0x0f;
       plmnId->mnc[2]       = (plmnIe->val[2] & 0xf0) >> 4;
    }
  
    RETVALUE(ROK); 
}

PUBLIC S16 wrUtlFillPlmn
(
WrPlmnId                     *plmnId,
SztPLMNidentity              *plmnIe
)
{
   U8      pLMNId[3];
   U16     idx;

   WR_ALLOC(&plmnIe->val, WR_PLMNID_IE_LEN * sizeof(U8));
   if (plmnIe->val == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   plmnIe->pres = PRSNT_NODEF;
   plmnIe->len  = WR_PLMNID_IE_LEN;

   pLMNId[0] =(((plmnId->mcc[1])<<4) | (plmnId->mcc[0]));
   if (plmnId->numMncDigits == 2)
   {
      pLMNId[1] =((0xf0) | (plmnId->mcc[2]));
      pLMNId[2] =(((plmnId->mnc[1])<<4) | (plmnId->mnc[0]));
   }
   else
   {
      pLMNId[1] =(((plmnId->mnc[0])<<4) | (plmnId->mcc[2]));
      pLMNId[2] =(((plmnId->mnc[2])<<4) | (plmnId->mnc[1]));
   }

   for(idx = 0; idx < plmnIe->len; idx++)
   {
      plmnIe->val[idx] = pLMNId[idx];
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function Retrieve message index
 *
 *      Function: wrSzGetIE
 *
 *          Processing steps:
 *          - check if the PDU is present
 *          - if present
 *             - switch to message type
 *             - check for the message
 *             - if present
 *                - get the start of string and procedure code
 *             - else return ROK
 *          - else return ROK
 *          - check the message in the string is present or not
 *          - if not present return 
 *          - get number of IE in the message
 *          - travese through the message
 *          - for each IE
 *             - if id matches 
 *                - update the double pointer to return the IE
 *                -brak the loop
 *          - return ROK
 *
 *  @param [in]  pdu : pointer to s1ap PDU
 *  @param [in]  id  : ID of the IE to be fetched 
 *  @param [out] ie  : double pointer to return back thee  poointtr IE
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrSzGetIE
(
SztS1AP_PDU                  *pdu,
U32                          id,
TknU8                        **ie
)
{
   TknU8                     *currStr;
   TknU8                     *startStr;
   U8                        choiceType;
   U16                       numComp;
   U16                       recIdx;
   U32                       recId;
   U32                       procCode;
   U16                       size;

   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie     = NULLP;
   numComp = 0;
   recIdx  = 0; 

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      RETVALUE(ROK);
   }

   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
         procCode  = pdu->val.initiatingMsg.procedureCode.val;
         break;
      }
      case S1AP_PDU_SUCCESSFULOUTCOME:
      {
         if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.successfulOutcome.value.u));
         procCode  = pdu->val.successfulOutcome.procedureCode.val;
         break;
      }
      default: 
      {
         if ((pdu->val.unsuccessfulOutcome.pres.pres != PRSNT_NODEF) ||
             (choiceType != S1AP_PDU_UNSUCCESSFULOUTCOME))
         {
            RETVALUE(ROK);
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
      RETVALUE(ROK);
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
         recId  = ((SztProtIE_ID *)currStr)->val; 

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(SztProtIE_ID) 
                       + sizeof(SztCriticality));

            *ie = startStr;
            break;
         }
      }

      size = szElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   RETVALUE(ROK);
} /* end of wrSzGetIE */


/* We ignore any value above 4Gbps, so we consider only last 4 octets */
PUBLIC S16 wrGetBitRate
(
SztBitRate                   *ie,
U32                          *val
)
{
   U32                       startIdx = 0;
   U32                       idx;

   *val = 0;
   if (ie->len >= 5)
   {
      startIdx = ie->len - 4;
   }
   for(idx = startIdx; idx < ie->len; idx++)
   {
      *val = (*val << 8) | (ie->val[idx]);
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrUtlFillBitRate
(
CmMemListCp                  *mem,
TknStrOSXL                   *ptr,
U16                          len,
U32                          val
)
{
   U16                       idx;
   U8                        offset;

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(mem, len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      offset = (len - (idx + 1)) * 8;
      ptr->val[idx] = (U8)(val >> offset);
   }

   RETVALUE(ROK);
}

PUBLIC U32 wrUtlGetTeid
(
TknStrOSXL                   *ie
)
{
   U32                       teid;

   teid = ie->val[0];
   teid = (teid << 8) | (ie->val[1]);
   teid = (teid << 8) | (ie->val[2]);
   teid = (teid << 8) | (ie->val[3]);

   RETVALUE(teid);
}

PUBLIC S16 wrUtlGetTportAddr
(
TknStrBSXL                   *ie,
CmTptAddr                    *addr
)
{
   U32                       idx;
   switch(ie->len)
   {
      case 32:
      {
         addr->u.ipv4TptAddr.address = 0;
         addr->type = CM_TPTADDR_IPV4;
         for (idx = 0; idx < 4; idx++)
         {
            addr->u.ipv4TptAddr.address <<= 8;
            addr->u.ipv4TptAddr.address |= ie->val[idx];
         }
         RETVALUE(ROK);
      }
      case 128:
      {
         addr->type = CM_TPTADDR_IPV6;
         for (idx = 0; idx < 16; idx++)
         {
            addr->u.ipv6TptAddr.ipv6NetAddr[idx] = ie->val[idx];
         }
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}

/**
 *  @brief This function Fill BSXL token string.
 *
 *      Function: wrUtlFillU32BSXL  
 *
 *          Processing steps:
 *           - Fill BSXL token string   
 *
 *  @param[in] mem : pointer to CmMemListCp 
 *  @param[out]  ie  : pointer to token srting
 *  @param[in] val :  value
 *  @param[in] len : Length
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUtlFillU32BSXL
(
CmMemListCp                  *mem,
TknStrBSXL                   *ie,
U32                          val,
U8                           len
)
{
   U32                       noOcts = (len + 7)/8;
   U32                       newVal = val << (32 - len);
   U32                       idx;

   if(len > 32)
   {
      RETVALUE(RFAILED);
   }
   WR_GET_MEM(mem, noOcts, &ie->val);
   if (ie->val == NULLP)
   {
      RETVALUE(RFAILED);
   }
   ie->len  = len;
   ie->pres = PRSNT_NODEF;
   for(idx = 0; idx < noOcts; idx++)
   {
      ie->val[idx] = (newVal & 0xff000000) >> 24;
      newVal <<= 8;
   }
   RETVALUE(ROK);
}
/**
 *  @brief This function Fill BSXL token string
 *          whose length is fixed to 4096 bits.
 *      Function: wrUtlFillU4096BSXL  
 *
 *          Processing steps:
 *           - Fill BSXL token string   
 *
 *  @param[in] mem : pointer to CmMemListCp 
 *  @param[out] ptr: pointer to token srting
 *  @param[in] val :  value
 *  @param[in] len : Length
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrFillTknStr4096BSXL
(
TknStrBSXL                   *ptr,
U16                          numBits,
Data                         *val,
CmMemListCp                  *mem
)
{
   U16                       idx = 0;
   U16                       len = 0;

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len  = 4096;/* Size is fixed and it is 4096
                        TODO: Add a Macro */

   /* 4096/8 = 512*/
   if (cmGetMem(mem, 512, (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   len = numBits / 8;
   if(numBits % 8 != 0)
   {
      len++;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}                              

/**
 *  @brief This function Fill BSXL token string
 *          whose length is fixed to 16384 bits.
 *      Function: wrFillTknStr16384BSXL  
 *
 *          Processing steps:
 *           - Fill BSXL token string   
 *
 *  @param[out] ptr: pointer to token srting
 *  @param[in]  numBits:  Number of bits
 *  @param[in] val : Value
 *  @param[in] mem : pointer to CmMemListCp 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrFillTknStr16384BSXL
(
TknStrBSXL                   *ptr,
U16                          numBits,
Data                         *val,
CmMemListCp                  *mem
)
{
   U16                       idx = 0;
   U16                       len = 0;

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len  = 16384; /* Size is fixed and it is 16384 */

   /* 16384/8 = 2048 */
   if (cmGetMem(mem, 2048, (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, " cmGetMem failed ");
      RETVALUE(RFAILED);
   }

   len = numBits / 8;
   if(numBits % 8 != 0)
   {
      len++;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
} 

/**
 *  @brief This function retreives the numOfBits
 *  and UL BitMap from  BSXL token string
 *
 *      Function: wrRetrvBitMapfrm4096BSXL
 *
 *          Processing steps:
 *           - Fill the UL BitMap and NumOfBits 
 *             from BSXL.
 *
 *  @param[out] ptr: pointer to token srting
 *  @param[in] ulBitMap  : Ul Bit Map 
 *  @param[in] numOfBits : Length of UL Bit Map
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrRetrvBitMapfrm4096BSXL
(
TknStrBSXL                   *ptr,
U16                           *numOfBits,
U8                           **ulBitMap
)
{
   U8          byteLen = 0;           /* To store the nos of non-zero value bytes */
   S32         i = 0;
   U16         numBits = 0;


   for(i = 511; i >= 0; i--)
   {
      /* Traverse the BSXL from end till a non-zero value is found*/
      if(ptr->val[i] != 0)
         break;
   }
   if( i < 0)
   {
      RLOG0(L_ERROR, "Invalid Bit map received");
      *numOfBits = 0;
      RETVALUE(ROK);
   }
   byteLen = i + 1;
   numBits = byteLen * 8;

   WR_ALLOC(ulBitMap, byteLen * sizeof(U8));
   if (NULLP == *ulBitMap)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Now travers the BSXL from beginnig to retreive the BitMap */
   WR_MEM_COPY(*ulBitMap, ptr->val, byteLen * sizeof(U8));

   /* Store the num of Bits */
   *numOfBits = numBits;

   RETVALUE(ROK);
}                              

/**
 *  @brief This function retreives the numOfBits
 *  and UL BitMap from  BSXL token string
 *
 *      Function: wrRetrvBitMapfrm16384BSXL
 *
 *          Processing steps:
 *           - Fill the UL BitMap and NumOfBits 
 *             from BSXL.
 *
 *  @param[out] ptr: pointer to token srting
 *  @param[in] ulBitMap  : Ul Bit Map 
 *  @param[in] numOfBits : Length of UL Bit Map
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrRetrvBitMapfrm16384BSXL
(
TknStrBSXL                   *ptr,
U16                           *numOfBits,
U8                           **ulBitMap
)
{
   U16          byteLen = 0;           /* To store the nos of non-zero value bytes */
   S32         idx = 0;
   U16         numBits = 0;


   for(idx = 2047; idx >= 0; idx--)
   {
      /* Traverse the BSXL from end till a non-zero value is found*/
      if(ptr->val[idx] != 0)
         break;
   }
   if( idx < 0)
   {
      RLOG0(L_ERROR, "Invalid Bit map received");
      *numOfBits = 0;
      RETVALUE(ROK);
   }
   byteLen = idx + 1;
   numBits = byteLen * 8;

   WR_ALLOC(ulBitMap, byteLen * sizeof(U8));
   if (NULLP == *ulBitMap)
   {
      RLOG0(L_ERROR, "wrRetrvBitMapfrm4096BSXL: WR_ALLOC failed");
      RETVALUE(RFAILED);
   }

   /* Now travers the BSXL from beginnig to retreive the BitMap */
   WR_MEM_COPY(*ulBitMap, ptr->val, byteLen * sizeof(U8));

   /* Store the num of Bits */
   *numOfBits = numBits;

   RETVALUE(ROK);
}                              

/**
 *  @brief This function  Retrieve U32 from BSXL token string.
 *
 *      Function: wrUtlGetU32FrmBSXL
 *
 *          Processing steps:
 *           - Convert token BSXL to U32
 *
 *  @param[out] val : pointer to value
 *  @param[in]  ie  : pointer to token srting
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUtlGetU32FrmBSXL
(
U32                          *val,
TknStrBSXL                   *ie
)
{

   if((ie->pres != PRSNT_NODEF) || (ie->len  > 32))
   {
      RETVALUE(RFAILED);
   }
   (*val) |= ie->val[0];
   (*val) <<= 8;
   (*val) |= ie->val[1];
   (*val) <<= 8;
   (*val) |= ie->val[2];
   (*val) <<= 8;
   (*val) |= ie->val[3];
   (*val) >>= (32 - ie->len);

   RETVALUE(ROK);
}

/**
 *  @brief This function  Retrieve U16 from BSXL token string.
 *
 *      Function: wrUtlGetU16FrmBSXL
 *
 *          Processing steps:
 *           - Convert token BSXL to U16
 *
 *  @param[out] val : pointer to value
 *  @param[in]  ie  : pointer to token srting
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUtlGetU16FrmBSXL
(
U16                          *val,
TknStrBSXL                   *ie
)
{

   if((ie->pres != PRSNT_NODEF) || (ie->len  > 16))
   {
      RETVALUE(RFAILED);
   }
   (*val) |= ie->val[0];
   (*val) <<= 8;
   (*val) |= ie->val[1];
   (*val) >>= (16 - ie->len);

   RETVALUE(ROK);
}

/**
 *  @brief This function  Retrieve U32 from BStr32 token string.
 *
 *      Function: wrUtlGetU32FrmBStr32
 *
 *          Processing steps:
 *           - Convert token BStr32 to U32
 *
 *  @param[out] val : pointer to value
 *  @param[in]  ie  : pointer to token srting
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUtlGetU32FrmBStr32
(
U32                          *val,
TknBStr32                    *ie
)
{
   (*val) = 0;

   (*val) |= ie->val[0];
   (*val) <<= 8;
   (*val) |= ie->val[1];
   (*val) <<= 8;
   (*val) |= ie->val[2];
   (*val) <<= 8;
   (*val) |= ie->val[3];
   (*val) >>= (32 - ie->len);
   RETVALUE(ROK);
}

/**
 *  @brief This function returns the DSCP for a QCI
 *
 *      Function: wrUtlGetDscp
 *
 *          Processing steps:
 *          - fetch the DSCP from configparams for the given QCI
 *          - update the pointer
 *
 *  @param[in]  cellId: Cell Id
 *  @param[in]  qci  : QCI for which DSCP is to returned
 *  @param[out] dscp : pointer to fetch the DSCP
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUtlGetDscp
(
U16                         cellId,
U32                          qci,
U8                           *dscp
)
{
   WrUmmCellCb               *ummCellCb = NULLP;
   
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb != NULLP)
   {
      *dscp = ummCellCb->wrRabCfgParams[qci].dscp;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/**
 *  @brief This function returns the RLC Mode of a radio bearer for a QCI
 *
 *      Function: wrUtlGetRbMode
 *
 *          Processing steps:
 *          - fetch the RLC Mode from the configparams for the given QCI
 *          - update the pointer
 *
 *  @param[in]  cellId: Cell Id
 *  @param[in]  qci    : QCI for which DSCP is to returned
 *  @param[out] rbMode : pointer to fetch the RLC Mode for the RB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUtlGetRbMode
(
U16                          cellId,
U32                          qci,
U8                           *rbMode
)
{
   WrUmmCellCb               *ummCellCb = NULLP;
   
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb != NULLP)
   {
      *rbMode = ummCellCb->wrRabCfgParams[qci].rlcMode;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

PUBLIC S16 wrUtlGetRbDir
(
U16                          cellId,
U32                          qci,
U8                           *rbDir
)
{
   WrUmmCellCb               *ummCellCb = NULLP;
   
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb != NULLP)
   {
      *rbDir = ummCellCb->wrRabCfgParams[qci].rlcDir;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/* SPS changes starts */
/*
 * This function is used to get SPS-RNTI at the time of handover.
 */
PUBLIC S16 wrUtlGetSpsRnti
(
U16                          *spsRnti,
U16                          cellId
)
{
   CmLList                   *tmpNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
  
   /* Get UE CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG0(L_ERROR, "wrUtlGetSpsRnti: Cell Id not found");
      RETVALUE(RFAILED);
   }
   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&cellCb->spsRntiDb.freeRntiLst, tmpNode);
   if (NULLP != tmpNode)
   {
      cmLListDelFrm(&cellCb->spsRntiDb.freeRntiLst, tmpNode);
      /* Add this into inuse preamble List */
      tmpNode->next = NULLP;
      tmpNode->prev = NULLP;
      cmLListAdd2Tail(&cellCb->spsRntiDb.inUseRntiLst, tmpNode); 
      *spsRnti = ((WrRntinode *)tmpNode->node)->rnti;
   }
   else
   {
      RLOG0(L_ERROR, "wrUtlGetSpsRnti: SPS RNTI not available");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* End of wrUtlGetSpsRnti.*/

/*
 * This function is used to free SPS-RNTI allocated at the time of 
 * handover.
 */
PUBLIC S16 wrUtlFreeSpsRnti
(
U16                          spsRnti,
U16                          cellId
)
{
   CmLList                   *tmpNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
  
   /* Get UE CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&cellCb->spsRntiDb.inUseRntiLst, tmpNode);
   while (NULLP != tmpNode)
   {
      if(spsRnti == ((WrRntinode *)tmpNode->node)->rnti)
      {
         RLOG1(L_DEBUG,"wrUtlFreeSpsRnti: Freeing SPS RNTI [%u]", spsRnti);
         cmLListDelFrm(&cellCb->spsRntiDb.inUseRntiLst, tmpNode);
         /* Add this into inuse preamble List */
         tmpNode->next = NULLP;
         tmpNode->prev = NULLP;
         cmLListAdd2Tail(&cellCb->spsRntiDb.freeRntiLst, tmpNode); 
         RETVALUE(ROK);
      }
      CM_LLIST_NEXT_NODE(&cellCb->spsRntiDb.inUseRntiLst, tmpNode);
   }
   RETVALUE(RFAILED);
}/* End of wrUtlFreeSpsRnti.*/
/* SPS changes ends */

/*
 * This function is used to get dedicated crnti at the time of handover.
 */
PUBLIC S16 wrUtlGetDedCrnti
(
U16                          *rnti,
U16                          cellId
)
{
   CmLList                   *tmpNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
  
   /* Get UE CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&cellCb->rntiDb.freeRntiLst, tmpNode);
   if (NULLP != tmpNode)
   {
      cmLListDelFrm(&cellCb->rntiDb.freeRntiLst, tmpNode);
      /* Add this into inuse preamble List */
      tmpNode->next = NULLP;
      tmpNode->prev = NULLP;
      cmLListAdd2Tail(&cellCb->rntiDb.inUseRntiLst, tmpNode); 
      *rnti = ((WrRntinode *)tmpNode->node)->rnti;
   }
   else
   {
      RLOG0(L_ERROR, "Dedicated RNTI not available");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* End of wrUtlGetDedCrnti.*/

/*
 * This function is used to free dedicated crnti allocated at the time of 
 * handover.
 */
PUBLIC S16 wrUtlFreeDedCrnti
(
U16                          rnti,
U16                          cellId
)
{
   CmLList                   *tmpNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
  
   /* Get UE CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&cellCb->rntiDb.inUseRntiLst, tmpNode);
   while (NULLP != tmpNode)
   {
      if(rnti == ((WrRntinode *)tmpNode->node)->rnti)
      {
         RLOG1(L_DEBUG, "Freeing dedicated RNTI [%d]", rnti);
         cmLListDelFrm(&cellCb->rntiDb.inUseRntiLst, tmpNode);
         /* Add this into inuse preamble List */
         tmpNode->next = NULLP;
         tmpNode->prev = NULLP;
         cmLListAdd2Tail(&cellCb->rntiDb.freeRntiLst, tmpNode); 
         RETVALUE(ROK);
      }
      CM_LLIST_NEXT_NODE(&cellCb->rntiDb.inUseRntiLst, tmpNode);
   }
   RLOG0(L_ERROR, "Dedicated RNTI not found in inuse list");
   RETVALUE(RFAILED);
}/* End of wrUtlFreeDedCrnti.*/


/*
 *
 *      Fun:   wrUtlGetDedPreamble
 *
 */
PUBLIC S16 wrUtlGetDedPreamble
(
U16                          *prmblId,
U16                          cellId
)
{
   CmLList                   *tmpNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
  
   /* Get UE CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&cellCb->prmblDb.freePrmblLst, tmpNode);
   if (NULLP != tmpNode)
   {
      cmLListDelFrm(&cellCb->prmblDb.freePrmblLst, tmpNode);
      /* Add this into inuse preamble List */
      tmpNode->next = NULLP;
      tmpNode->prev = NULLP;
      cmLListAdd2Tail(&cellCb->prmblDb.inUsePrmblLst, tmpNode); 
      *prmblId = ((WrPrmblnode *)tmpNode->node)->prmblId;
   }
   else
   {
      RLOG0(L_ERROR, " Dedicated Preamble not available");
      /* As per the spec when there is no dedicated preamble is available
         and still the Target eNB wants to admit the UE then it assigns
         0 as a dedicated preamble.
         So that Ue can trigger contention based RACH */
      *prmblId = 0;      
   }

   RETVALUE(ROK);
}/* end of wrUtlGetDedPreamble */

/*
 *
 *      Fun:   wrUtlFreeDedPreamble
 *
 */
PUBLIC S16 wrUtlFreeDedPreamble
(
U16                          prmblId,
U16                          cellId
)
{
   CmLList                   *tmpNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;


   /* When dedicated preamble is not allocated*/
   if(WR_VAL_ZERO == prmblId)
   {
     RETVALUE(ROK);
   }
 
   /* Get UE CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Get the first node from the inuse list */
   CM_LLIST_FIRST_NODE(&cellCb->prmblDb.inUsePrmblLst, tmpNode);
   while (NULLP != tmpNode)
   {
      if(prmblId == ((WrPrmblnode *)tmpNode->node)->prmblId)
      {
         RLOG1(L_DEBUG, "Freeing the dedicated preamble [%d]", prmblId);
         cmLListDelFrm(&cellCb->prmblDb.inUsePrmblLst, tmpNode);
         /* Add this into free preamble List */
         tmpNode->next = NULLP;
         tmpNode->prev = NULLP;
         cmLListAdd2Tail(&cellCb->prmblDb.freePrmblLst, tmpNode);
         RETVALUE(ROK);
      }
      CM_LLIST_NEXT_NODE(&cellCb->prmblDb.inUsePrmblLst, tmpNode);
   }
   RLOG0(L_ERROR, "Dedicated preamble not found in inuse list");

   RETVALUE(RFAILED);
} /* end of wrUtlFreeDedPreamble */

/**
 *  @brief This function copies the plmn information
 *
 *      Function: wrUtlCopyPlmnId
 *
 *          Processing steps:
 *          - copy the values from plmn1 to plmn2
 *
 *  @param[in]   plmnId1 : source plmn id
 *  @param[out]  plmnId2 : target plmn id
 *  @return S16  ROK
 */
PUBLIC S16 wrUtlCopyPlmnId
(
WrPlmnId                     *plmn1,
WrPlmnId                     *plmn2
)
{
   plmn2->numMncDigits  = plmn1->numMncDigits;
   plmn2->mcc[0]  = plmn1->mcc[0];
   plmn2->mcc[1]  = plmn1->mcc[1];
   plmn2->mcc[2]  = plmn1->mcc[2];
   plmn2->mnc[0]  = plmn1->mnc[0];
   plmn2->mnc[1]  = plmn1->mnc[1];
   if(plmn1->numMncDigits > 2)
   {
      plmn2->mnc[2] = plmn1->mnc[2];
   }
   else
   {
      plmn2->mnc[2] = 0;
   }
   RETVALUE(ROK);
}

PUBLIC Void wrUtlPrntIncTansType2
(
U8                           transTyp,
U16                          crnti,
U16                          msgTyp,
U32                          transId
)
{
    switch(transTyp)
    {
       case  WR_UMM_S1_SRC_HO_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_S1_SRC_HO_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_S1_TGT_HO_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_S1_TGT_HO_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_X2_SRC_HO_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_X2_SRC_HO_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_X2_TGT_HO_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_X2_TGT_HO_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_CSFB_REDIR_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_CSFB_REDIR_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_RAD_RES_MOD_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_RAD_RES_MOD_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_RRM_UEADMIT_RESP:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "RRM UE ADMIT RESP (%ld) is reveived", msgTyp, transId);
          break;
       case  WR_UMM_RRM_UERECFG_RESP:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "RRM UE RECFG RESP reveived", msgTyp, transId);
          break;
       case WR_UMM_LOCATION_REPORT_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_LOCATION_REPORT_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case WR_UMM_ANR_MEAS_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_ANR_MEAS_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case WR_UMM_EVENT_ANR_TRANS:
         RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
            "WR_UMM_EVENT_ANR_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       case WR_UMM_CSFB_CCO_TRANS:
          RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] for transaction - "
                "WR_UMM_CSFB_CCO_TRANS (%ld) is reveived", msgTyp, transId);
          break;
       default:
          RLOG_ARG2(L_DEBUG, DBG_CRNTI, crnti,"Message [%d] received with "
             "INVALID TRANSACTION ID [%d] ", transTyp, crnti);
    }

}
 

#ifdef RSYS_WIRESHARK
#ifdef ANSI
PUBLIC S16 wrInitWireShark
(
)
#else
PUBLIC S16 wrInitWireShark()
#endif
{
   
   if( (0 == wrSmDfltWiresharkPort) ||
       (0 == wrSmDfltWiresharkDstIpAddr))
   {
      RLOG0(L_ERROR,"update wireshark destination port and Ip address in wr_cfg.txt for logging");
      RETVALUE(RFAILED);	
   }
   g_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   if (g_sockfd < 0)
   {
      return (-1);
   }

#if 0 /* Dont know why this FD getting created again and also it is not  bieng used */
   g_sockfd1 = socket(AF_INET, SOCK_DGRAM, 0);

   if (g_sockfd < 0)
   {
      return (-1);
   }
#endif

   g_serv_addr.sin_family = AF_INET;
   g_serv_addr.sin_port   = htons(wrSmDfltWiresharkPort);
   /* TODO: Need to take care of IPv6 */
   g_serv_addr.sin_addr.s_addr = htonl(wrSmDfltEnbIpAddr[0].u.ipv4TptAddr.address);
   bind(g_sockfd, &g_serv_addr, sizeof(g_serv_addr));

#if 1
   g_serv_addr1.sin_family = AF_INET;
   g_serv_addr1.sin_port   = htons(wrSmDfltWiresharkPort);
   g_serv_addr1.sin_addr.s_addr = htonl(wrSmDfltWiresharkDstIpAddr);
#endif

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 wrHandleWireSharkMsg
(
Buffer     *mBuf
)
#else
PUBLIC S16 wrHandleWireSharkMsg(mBuf)
Buffer     *mBuf;
#endif
{
   MsgLen len;
   U8     statBuffer[10000];
   MsgLen cLen;
   U32     bufOffset=0;
   U16     tmp16;
   S16     ret;

   /* mBuf Contents */
   U8      radioType=1;
   U8      direction;
   U8      rntiType;
   U16     rnti;
   U16     ueId;
   U16     subframe;
   U8      isPredefinedData=0;
   U8      retx=0;
   U8      crcStatus=1;

   /* Added trace macro */
   TRC2(wrHandleWireSharkMsg)

   SFndLenMsg(mBuf, &len);
   if (len==0)
   {
      SPutMsg(mBuf);
      /* Added RETVALUE */
      RETVALUE(ROK);
   }


   SUnpkU16(&subframe, mBuf);
   SUnpkU16(&ueId, mBuf);
   SUnpkU16(&rnti, mBuf);
   SUnpkU8(&rntiType, mBuf);
   SUnpkU8(&direction, mBuf);

   memset(statBuffer, 0, sizeof(statBuffer));

   /* Need to modify condition for rrcLogEnable flag if this flag is enabled then send
    * BCCH and PCCH with  prepended with 1 byte for ASN message category, in this case
    * RRC dissector will be used to decode msg else MAC-LTE dissector wil be used
    * */
   if(wrCb.enblSIAndPagngLog)
   {
      if(ueId == 65534)
      {
         statBuffer[bufOffset++] = 5;
      }
      if(ueId == 65535)
      {
         statBuffer[bufOffset++] = 7;
      }
   }
   else
   {
      /* Start Buffer Tag for logging */
      memcpy(statBuffer+bufOffset, WR_MAC_LTE_START_STRING,
            strlen(WR_MAC_LTE_START_STRING));
      bufOffset += strlen(WR_MAC_LTE_START_STRING);

      /* MAC Packet Info fileds */
      statBuffer[bufOffset++]    = radioType;
      statBuffer[bufOffset++]    = direction;
      statBuffer[bufOffset++]    = rntiType;

      if (ueId == 65535)
         statBuffer[bufOffset-1] = 4;
      if (ueId == 0)
         statBuffer[bufOffset-1] = 2;

      /* RNTI */
      statBuffer[bufOffset++] = WR_MAC_LTE_RNTI_TAG;
      tmp16 = htons(rnti);
      memcpy(statBuffer+bufOffset, &tmp16, 2);
      bufOffset+= 2;

      /* UEId */
      statBuffer[bufOffset++] = WR_MAC_LTE_UEID_TAG;
      tmp16 = htons(ueId);
      memcpy(statBuffer+bufOffset, &tmp16, 2);
      bufOffset+= 2;


      /* Subframe number */
      statBuffer[bufOffset++] = WR_MAC_LTE_SUBFRAME_TAG;
      tmp16 = htons(subframe);
      memcpy(statBuffer+bufOffset, &tmp16, 2);
      bufOffset+= 2;

      statBuffer[bufOffset++] = WR_MAC_LTE_CRC_STATUS_TAG;
      statBuffer[bufOffset++] = crcStatus;

      /********/
      if (!isPredefinedData) {
         statBuffer[bufOffset++] = WR_MAC_LTE_PREDFINED_DATA_TAG;
         statBuffer[bufOffset++] = isPredefinedData;
      }

      if (retx != 0) {
         statBuffer[bufOffset++] = WR_MAC_LTE_RETX_TAG;
         statBuffer[bufOffset++] = retx;
      }

      /* Actual Payload */
      statBuffer[bufOffset++] = WR_MAC_LTE_PAYLOAD_TAG;
   }


   if (mBuf != NULLP)
   {
      SFndLenMsg(mBuf, &len);
      ret = SCpyMsgFix(mBuf, (MsgLen)0, (MsgLen)len,
            (Data*) (&statBuffer[bufOffset]), (MsgLen*)&cLen);
      if (ret != ROK)
      {
         SPutMsg(mBuf); 
         RETVALUE(ROK);
      }
      SPutMsg(mBuf);
   }

   bufOffset += len;

   ret = sendto(g_sockfd, statBuffer, bufOffset, 0, &g_serv_addr1, sizeof(g_serv_addr1));



   /* Added RETVALUE */
   RETVALUE(ROK);
}

#endif

/**
 *  *  @brief This function fills encSztSectorIdInfo structure
 *   *
 *    *      Function: wrUmmEncCsfbSectorId
 *     *
 *      *
 *       *  @param[in]    marketId
 *        *  @param[in]    switchNum
 *         *  @param[in]    sectorNum
 *          *  @param[out]   encSztSectorIdInfo
 *           *  @return void
 *            */  
PUBLIC S16 wrUmmEncCsfbSectorId
(
 U16              marketId,
 U8               switchNum,
 U16              cellId,
 U8               sectorNum,
 TknStrOSXL       *ptr,
 U8               len
 )
{
   
      /* make the element as present */
      ptr->pres = TRUE;
      ptr->len =  len;
      WR_ALLOC(&(ptr->val), len * sizeof(U8));
      if (NULLP == ptr->val)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      ptr->val[0] = ((marketId & 0xff00) >> 8);
      ptr->val[1] = (marketId & 0x00ff);
      ptr->val[2] = (switchNum & 0x00ff);
      ptr->val[3] = ((cellId &  0x0ff0) >> 4);
      ptr->val[4] = ((cellId & 0x000f) << 4);
      ptr->val[4] |= (sectorNum & 0x000f); 
   
   RETVALUE(ROK);
}

/**
 *  @brief This function copies the sId, nId and baseId from CId
 *
 *      Function: wrGetCdmaParamFromCID
 *
 *
 *  @param[out]    sId
 *  @param[out]    nId
 *  @param[out]    baseId
 *  @param[in]     ratType 
 *  @param[in]     cId
 *  @return void
 */
PUBLIC Void wrGetCdmaParamFromCID
(
U16                         *sId,
U16                         *nId,
U16                         *baseId,
U32                         ratType,
U64                         cId
)
{
   switch(ratType)
   {
      case CDMA2000_1XRTT:
         {
            *baseId = (U16)((cId & 0x00007fff80000000) >> 31);
            *sId = (U16)((cId & 0x000000007fff0000) >> 16);
            *nId = (U16)((cId & 0x000000000000ffff));
            break;
         }
      case CDMA2000_HRPD:
         {
            /* TBD - For HRPD to get CdmaParams From CID */
            break;
         }
         RETVOID;
   }
}

/**
 *  @brief This function is used for getting DRX QCI feature flag
 *
 *      Function: wrGetDrxQCIFeatureFlag
 *
 *@param[in]    cellId : cell id
 *@param[out]    flag : feature flag
 *
 *
 *  @return ROK/RFAILED
 */
PUBLIC S16 wrGetDrxQCIFeatureFlag
(
 WrCellId                  cellId,
 Bool                      *flag
)
{
   WrCellCb                *cellCb = NULLP;
   
   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)
   {
      RLOG1(L_ERROR, "Error in getting WR CELL CB for cellId = %lu",
                  (U32)cellId);
      RETVALUE(RFAILED);
   }
  *flag = cellCb->isDRXQCIEnabled;
  RETVALUE(ROK);
}

/**
 *  @brief This function Handles the Rab messages(Setup/Modify/Release)
 *  when other transactions are going on. And also fill
 *  the appropriate causeTyp and casueVal per Rab w.r.t ongoing transactions.
 *
 *      Function: wrUmmHdlAndPrcRabMsg
 *
 *  @param [out] incTrans : pointer to transaction control block
 *  @param [in]  cause    : pointer to cause: cause type and cause value.
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmHdlAndPrcRabMsg
(
WrUmmTransCb              *incTrans,
WrUmmMsgCause             *cause
)
{
   U32                  idx = 0;
   U32                  idx1 = 0;

   switch(incTrans->transTyp)
   {
      case WR_UMM_ERAB_SETUP_TRANS:
         {
            WrUmmRabSetupTrans                  *rabSetup = NULLP;
            SztE_RABSetupRqst                   *rabSetupIE = NULLP;
            SztProtIE_Field_E_RABSetupRqstIEs   *ie = NULLP;
            SztE_RABToBeSetupItemBrSUReq        *rabInfo = NULLP;
            SztE_RABToBeSetupLstBrSUReq         *rabLst = NULLP;

            rabSetup   = &incTrans->u.rabSetupTrans;
            rabSetupIE = &incTrans->msg->u.s1Pdu->pdu->pdu.val.initiatingMsg.
                                value.u.sztE_RABSetupRqst;
            rabSetup->numRabs = 0;

            for(idx1 = 0;idx1 < rabSetupIE->protocolIEs.noComp.val; idx1++)
            {
               ie = &rabSetupIE->protocolIEs.member[idx1];
               if(ie->id.val == Sztid_E_RABToBeSetupLstBrSUReq)
               {
                  rabSetup->numRabs = ie->value.u.sztE_RABToBeSetupLstBrSUReq.
                                        noComp.val;
                  WR_ALLOC(&rabSetup->rabs, rabSetup->numRabs *
                        sizeof(WrUmmRabSetupRabInfo));
                  if (rabSetup->rabs == NULLP)
                  {
                     RLOG0(L_FATAL, "Memory allocation failed.");
                     RETVALUE(RFAILED);
                  }
                  rabLst = &ie->value.u.sztE_RABToBeSetupLstBrSUReq;
                  for (idx = 0; idx < rabSetup->numRabs; idx++)
                  {
                     if(rabLst->member[idx].value.u.sztE_RABToBeSetupItemBrSUReq.
                                            pres.pres != NOTPRSNT)
                     {
                        rabInfo = &rabLst->member[idx].value.u.
                                    sztE_RABToBeSetupItemBrSUReq;
                        rabSetup->rabs[idx].erabId      = rabInfo->e_RAB_ID.val;
                        rabSetup->rabs[idx].rabAllowed      = FALSE;
                        rabSetup->rabs[idx].cause.causeTyp  = cause->causeTyp;
                        rabSetup->rabs[idx].cause.causeVal  = cause->causeVal;
                     }
                  }
                  /*  Send ERAB Setup Response over S1 interface to MME  */
                  wrUmmRabSetupSndS1apRsp(incTrans);
                  break;
               }
            }
         }
         break;
      case WR_UMM_ERAB_MOD_TRANS:
         {
            WrUmmRabModifyTransCb               *rabTrans = NULLP;
            SztE_RABMdfyRqst                    *rabmod = NULLP;
            SztProtIE_Field_E_RABMdfyRqstIEs    *ie = NULLP;
            SztE_RABToBeMdfdItemBrModReq        *rabInfo = NULLP;
            SztE_RABToBeMdfdLstBrModReq         *rabLst = NULLP;
            rabTrans  = &incTrans->u.rabModifyTransCb;
            rabmod    = &incTrans->msg->u.s1Pdu->pdu->pdu.val.initiatingMsg.
                            value.u.sztE_RABMdfyRqst;
            rabTrans->numRabs = 0;

            for(idx1 = 0;idx1 < rabmod->protocolIEs.noComp.val; idx1++)
            {
               ie = &rabmod->protocolIEs.member[idx1];
               if(ie->id.val == Sztid_E_RABToBeMdfdLstBrModReq)
               {
                  rabTrans->numRabs = ie->value.u.sztE_RABToBeMdfdLstBrModReq.
                                        noComp.val;
                  WR_ALLOC(&rabTrans->rabs, rabTrans->numRabs *
                        sizeof(WrUmmRabModifyRabInfo));
                  if (rabTrans->rabs == NULLP)
                  {
                     RLOG0(L_FATAL, "Memory allocation failed.");
                     RETVALUE(RFAILED);
                  }
                  rabLst = &ie->value.u.sztE_RABToBeMdfdLstBrModReq;
                  for (idx = 0; idx < rabTrans->numRabs; idx++)
                  {
                     if(rabLst->member[idx].value.u.sztE_RABToBeMdfdItemBrModReq.
                                                pres.pres  != NOTPRSNT)
                     {
                        rabInfo     = &rabLst->member[idx].value.u.
                                        sztE_RABToBeMdfdItemBrModReq;
                        rabTrans->rabs[idx].erabId     = rabInfo->e_RAB_ID.val;
                        rabTrans->rabs[idx].rabAllowed = FALSE;
                        rabTrans->rabs[idx].cause.causeTyp  = cause->causeTyp;
                        rabTrans->rabs[idx].cause.causeVal  = cause->causeVal;
                     }
                  }
                  /* Send the ERAB modify response with failed Bearer list */
                  wrUmmRabModSndS1apRsp(incTrans);
                  break;
               }
            }
         }
         break;
      case WR_UMM_RAB_REL_TRANS:
         {
            WrUmmRabRelTransCb                  *rabRelTrans = NULLP;
            SztE_RABRlsCmmd                     *rabRel = NULLP;
            SztProtIE_Field_E_RABRlsCmmdIEs     *ie = NULLP;
            SztE_RABLst                         *rabLst = NULLP;
            rabRelTrans   = &incTrans->u.rabRelTransCb;
            rabRel        = &incTrans->msg->u.s1Pdu->pdu->pdu.val.
                               initiatingMsg.value.u.sztE_RABRlsCmmd;
            rabRelTrans->numRabs = 0;

            for(idx1 = 0;idx1 < rabRel->protocolIEs.noComp.val; idx1++)
            {
               ie = &rabRel->protocolIEs.member[idx1];
               if(ie->id.val == Sztid_E_RABToBeRlsdLst)
               {
                  rabRelTrans->numRabs = ie->value.u.sztE_RABLst.noComp.val;
                  WR_ALLOC(&rabRelTrans->rabs, rabRelTrans->numRabs *
                                sizeof(WrUmmRabRelRabInfo));
                  if (rabRelTrans->rabs == NULLP)
                  {
                     RLOG0(L_FATAL, "Memory allocation failed.");
                     RETVALUE(RFAILED);
                  }
                  rabLst = &ie->value.u.sztE_RABLst;
                  for (idx = 0; idx < rabRelTrans->numRabs; idx++)
                  {
                     if(rabLst->member[idx].value.u.sztE_RABItem.pres.pres
                                            != NOTPRSNT)
                     {
                        rabRelTrans->rabs[idx].erabId    = rabLst->member[idx].
                                            value.u.sztE_RABItem.e_RAB_ID.val;
                        rabRelTrans->rabs[idx].rabfailed          = TRUE;
                        rabRelTrans->rabs[idx].relCause.causeTyp  = cause->causeTyp;
                        rabRelTrans->rabs[idx].relCause.causeVal  = cause->causeVal;
                     }
                  }
                  /*  Send ERAB Release Response over S1 interface to MME  */
                  wrUmmRabRelSndS1apRsp(incTrans);
                  break;
               }
            }
         }
         break;
      default:
         {
            ALARM("Transaction %d received at wrUmmHdlAndPrcRabMsg\n",
                  incTrans->transTyp);
         }
         break;
   } /* end of switch */
   RETVALUE(ROK);
}

/**
 *  @brief This function fills the TknStrOSXL sectorId
 *
 *      Function: wrFillSectorId
 *
 *          Processing steps:
 *          - allocate memory to val in TknStrOSXL
 *          - update token present field as TRUE
 *          - update the memory with the value  received
 *
 *
 *  @param[out] ptr  : TknStrOSXL stucture pointer to be updated
 *  @param[in]  len  : length for which memory to allocated
 *  @param[in]  val  : value getting updated
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrFillSectorId
(
TknStrOSXL                  *ptr,
U8                           len,
Data                         *val
)
{
   U16                       idx;
   
   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;
   WR_ALLOC(&(ptr->val), len * sizeof(U8));
   if (NULLP == ptr->val)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}

/*This fucntion is used to validate the forbidden LA*/
/**
 *  @brief This function validates whether the CS-domain PLMN is present in
 *         Serving PLMN or EPLMN and not in Forbidden LAs list.
 *
 *      Function: wrValidateRegLai
 *
 *          Processing steps:
 *          - check if lai is present in Serving PLMN
 *          - check if lai is present in Equivalent PLMN
 *          - check if lai is NOT present in Forbidden LAs
 *
 *  @param[out] valFlag: Flag
 *  @param[in]  *hoRstrctInfo    : HRL info of the UE
 *              lai              : Registered LAI
 *  @return Bool 
    -# Success : TRUE
    -# Failure : FALSE 
 */
PUBLIC Bool wrValidateRegLai
(
   WrHoRstrctInfo *hoRstrctInfo,
   WrRegLai        lai
)
{
   U32     cnt;
   U32     cnt1;
   Bool    valFlag = FALSE;

   /* Check if CS-Domain PLMN is equal to Serving PLMN*/
   if(wrEmmPlmnPlmnsEqual(&lai.plmnId,&hoRstrctInfo->srvngPlmn))
   {
      valFlag = TRUE;
      RLOG0(L_INFO,"CS-Domain PLMN matches Serving PLMN");
   }
   /*Check if CS-Domain PLMN is equal to EPLMNs*/
   for(cnt = 0; (valFlag == FALSE && cnt < hoRstrctInfo->noOfEplmn); cnt++)
   {
      if(wrEmmPlmnPlmnsEqual(&lai.plmnId, &hoRstrctInfo->ePlmn[cnt]))
      {
         valFlag = TRUE;
         RLOG0(L_INFO,"CS-Domain PLMN matches E-PLMN");
      }
   } 
   /*Check if Registered LAI is present in Forbidden LAs list*/
   for(cnt1 = 0;(valFlag == TRUE && cnt1 < hoRstrctInfo->noOfLAs); cnt1++)
   {
      if(TRUE == 
            wrEmmPlmnPlmnsEqual(&lai.plmnId,&hoRstrctInfo->frbdnLAs[cnt1].plmnId))
      {
         /*Ignore this Forbidden LA*/
         for(cnt = 0; cnt < hoRstrctInfo->frbdnLAs[cnt1].noOfLac; cnt++)
         {
            if(lai.lac == hoRstrctInfo->frbdnLAs[cnt1].lac[cnt])
            {
               RLOG0(L_WARNING,"Registered LAI is Forbidden");
               RETVALUE(FALSE);
            }
         }
      }
   }
   RETVALUE(valFlag);
}
/**
 *  @brief This function checks if PLMN RAT is forbidden for UE 
 *
 *      Function:  wrChkAllRatsForbidden
 *
 *          Processing steps:
 *          - if all RATs are forbidden then return TRUE
 *          - get PLMN RAT priority
 *          - check the forbidden inter RATs against PLMN RAT priority
 *
 *  @param[in]  *ueCb    :  UE control block
 *  @return Bool 
    -# Match Found : TRUE
    -# Otherwise : FALSE 
 */
PUBLIC Bool wrChkAllRatsForbidden
(
 WrUeCb *ueCb
)
{
   WrRatPriorityList ratPriority;
   WrCellCb          *cellCb = NULLP;
	U32                count = 0;

   RLOG0(L_INFO, "wrChkAllRatsForbidden");

   if (NULLP == ueCb->hoRstrctInfo)
   {
      RLOG0(L_INFO,"No HO Restrcition List for this UE\n");
      RETVALUE(FALSE);
   }
   if(SztForbiddenInterRATsallEnum == ueCb->hoRstrctInfo->frbdnInterRATs.val)
   {
      RLOG0(L_WARNING,"All RATs are Forbidden for this UE");
      RETVALUE(TRUE);
   }

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Cannot find cellCb for cellId");
      RETVALUE(FALSE);
   }
   
   if(RFAILED == wrEmmGetRatPriority(&ueCb->plmnId, &ratPriority, ueCb->cellId))
   {
      RLOG0(L_ERROR,"Invalid RAT Priority");
      RETVALUE(FALSE);
   }

   for (count = 0; count < ratPriority.numberOfRats; count++)
   {      
      switch(ratPriority.rat[count])
      {
         case UTRA_FDD:
         case UTRA_TDD:
         {
               if((SztForbiddenInterRATsutranEnum == 
                        ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
                     (SztForbiddenInterRATsgeranandutranEnum == 
                      ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
                     (SztForbiddenInterRATscdma2000andutranEnum ==
                      ueCb->hoRstrctInfo->frbdnInterRATs.val))
               {
                  RLOG0(L_WARNING,"Selected and Forbidden RAT is UTRAN");
               }
               else
               {
                  RETVALUE(FALSE);
               }
               break;
         }
         case GERAN:
          {
            if((SztForbiddenInterRATsgeranEnum == 
                        ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
                 (SztForbiddenInterRATsgeranandutranEnum == 
                        ueCb->hoRstrctInfo->frbdnInterRATs.val))
            {
               RLOG0(L_WARNING,"Selected and Forbidden RAT is GERAN");
            }
            else
            {
               RETVALUE(FALSE);
            }
            break;
         }
         case CDMA2000_HRPD:
         case CDMA2000_1XRTT:
         {
            if((SztForbiddenInterRATscdma2000Enum == 
                     ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
              (SztForbiddenInterRATscdma2000andutranEnum ==
                     ueCb->hoRstrctInfo->frbdnInterRATs.val))
            {
               RLOG0(L_WARNING,"Selected and Forbidden RAT is CDMA2000");
            }
            else
            {
               RETVALUE(FALSE);
            }
            break;
         }
         default:
         {
            RLOG0(L_ERROR,"Invalid RAT priority");
            break;
         }
      }
   }
   RETVALUE(TRUE);
}
PUBLIC Bool wrChkSingleRatForbidden
(
 WrUeCb *ueCb, 
 wrCsfbRatType *ratPriority
)
{
   RLOG0(L_INFO, "wrChkSingleRatForbidden");

   if (NULLP == ueCb->hoRstrctInfo)
   {
      RLOG0(L_INFO,"No HO Restrcition List for this UE\n");
      RETVALUE(FALSE);
   }
   if (ueCb->hoRstrctInfo->frbdnInterRATs.pres == NOTPRSNT)
   {
      RLOG0(L_INFO,"No forbiddenrat for this UE\n");
      RETVALUE(FALSE);
   }
   
   if(SztForbiddenInterRATsallEnum == ueCb->hoRstrctInfo->frbdnInterRATs.val)
   {
      RLOG0(L_INFO,"All RATs are Forbidden for this UE");
      RETVALUE(TRUE);
   }

   switch(*ratPriority)
   {
      case UTRA_FDD:
      case UTRA_TDD:
         {
            if((SztForbiddenInterRATsutranEnum == 
                     ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
              (SztForbiddenInterRATsgeranandutranEnum == 
                     ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
              (SztForbiddenInterRATscdma2000andutranEnum ==
                     ueCb->hoRstrctInfo->frbdnInterRATs.val))
            {
               RLOG0(L_WARNING,"Selected and Forbidden RAT is UTRAN");
               RETVALUE(TRUE);
            }
         }
         break;
      case GERAN:
         {
            if((SztForbiddenInterRATsgeranEnum == 
                     ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
              (SztForbiddenInterRATsgeranandutranEnum == 
                     ueCb->hoRstrctInfo->frbdnInterRATs.val))
            {
               RLOG0(L_WARNING,"Selected and Forbidden RAT is GERAN");
               RETVALUE(TRUE);
            }
         }
         break;
      case CDMA2000_HRPD:
      case CDMA2000_1XRTT:
         {
            if((SztForbiddenInterRATscdma2000Enum == 
                     ueCb->hoRstrctInfo->frbdnInterRATs.val) ||
              (SztForbiddenInterRATscdma2000andutranEnum ==
                     ueCb->hoRstrctInfo->frbdnInterRATs.val))
            {
               RLOG0(L_WARNING,"Selected and Forbidden RAT is CDMA2000");
               RETVALUE(TRUE);
            }
         }
         break;
      default:
         {
            RLOG0(L_ERROR,"Invalid RAT priority");
            break;
         }
   }

   RETVALUE(FALSE);
}
/**
 *  @brief This function fills the redirection information of GERAN neighbours
 *         serving the same PLMN as that of CS-Domain PLMN
 *
 *      Function: wrGetCsdomainPLMNNeighFreqs
 *
 *          Processing steps:
 *          - allocate memory to redirInfo
 *          - Loop through the list of GERAN neighbours serving CS-Domain PLMN
 *          - Update redirInfo with matching neighbour's BCCH ARFCN 
 *            and their count
 *
 *
 *  @param[in]  cellId        : cell Identity
 *  @param[in]  csdomainPlmn  : CS-Domain PLMN
 *  @param[in]  redirInfo      : redirInfo stucture pointer to be updated
 *  @param[out] redirInfo      : updated redirInfo stucture pointer
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrGetCsdomainPLMNNeighFreqs
(
 WrUeCb                    *ueCb,
 WrPlmnId                  csdomainPlmn,
 WrUmmRedirectInfo         **redirInfo
)
{
   CmLList                                       *lnk;
   WrGeranCellCb                                 *cellNode;
   WrGeranFreqCb                                 *freqNode;
   WrCellCb                                      *cellCb;
   U8                                            arfcnCnt=0;
   NhuIRAT_ParamsGERAN_v920                      *geranV920 = NULLP;
   Bool                                          eRedir_r9 = FALSE;
   
   TRC2(wrGetCsdomainPLMNNeighFreqs)

   geranV920 = &ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsGERAN_v920;
   if((NhuAccessStratumRlsrel9Enum <= 
            ueCb->ueCap->eUTRA_Cap.accessStratumRls.val) && 
         (PRSNT_NODEF == geranV920->pres.pres) && 
         (PRSNT_NODEF == geranV920->e_RedirectionGERAN_r9.pres))
   {
      eRedir_r9 = TRUE;
   }
   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR, "Error in getting WR CELL CB for cellId = %lu",
                  (U32)ueCb->cellId);
	   RETVALUE(RFAILED);
   }
   /*Memory is freed during TransRel*/ 
   WR_ALLOC(redirInfo, sizeof(WrUmmRedirectInfo));
   if (NULLP == *redirInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,lnk); 
   (*redirInfo)->ratTyp = WR_UMM_MEAS_GERAN_FREQ;
   while(NULLP != lnk)
   {
      cellNode = (WrGeranCellCb *)lnk->node;
      if((WR_UMM_MAX_GERAN_ARFCN > arfcnCnt) && 
            (TRUE == wrFindNghCellLaiPres(ueCb,&cellNode->plmnId,cellNode->lac, 0)) && 
            (TRUE == wrChkGeranBandSup(ueCb->ueCap,cellNode->freqCb->bandInd)))
      {
         (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].arfcn = 
            cellNode->freqCb->arfcn;
         /*Default value of redirInfo.bandIndicator = WR_GERAN_DCS1800
          * This will be filled in wrUmmFillNhuGeranRedirectInfo */
         if(WR_GERAN_PCS1900 == cellNode->freqCb->bandInd)
         {
            (*redirInfo)->val.geranInfo.bandInd = WR_GERAN_PCS1900;
         }
         (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].bsic = cellNode->bsic;
         /* siPsiType 0:SI 1:PSI */
         if((eRedir_r9) && (!cellNode->siPsiType) && 
               (0 < cellNode->noOfsiPsi))/*Fill only SIs.*/
         {
            U32  idx;
            (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].noOfsiPsi = 
               cellNode->noOfsiPsi;
            for(idx = 0;idx < cellNode->noOfsiPsi; idx++)
            {
                (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].siListGeran[idx] =
                   cellNode->siOrPsi[idx];
            }
         }
         (*redirInfo)->val.geranInfo.cellInfoCnt = ++arfcnCnt;
      }
      lnk = lnk->next;
   }
   /*If no neighbour serves the CS-Domain PLMN, fill all the neighbour ARFCNs*/
   if(!arfcnCnt)
   {
      CM_LLIST_FIRST_NODE(&cellCb->geranNbrFreqLst,lnk);
      while(NULLP != lnk)
      {
         freqNode = (WrGeranFreqCb *)lnk->node;
         if(TRUE == wrChkGeranBandSup(ueCb->ueCap,freqNode->bandInd))
         {
            (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].arfcn = 
                       freqNode->arfcn;
            /*Default value of redirInfo.bandIndicator = WR_GERAN_DCS1800
             * This will be filled in wrUmmFillNhuGeranRedirectInfo */
            if(WR_GERAN_PCS1900 == freqNode->bandInd)
            {
               (*redirInfo)->val.geranInfo.bandInd = WR_GERAN_PCS1900;
            }
            (*redirInfo)->val.geranInfo.cellInfoCnt = ++arfcnCnt;
         }
         lnk = lnk->next;
      }  
   }
   /*If no redirection information is found then release the UE without
    * redirection*/
   if(!arfcnCnt)
   {
      WR_FREE(*redirInfo,sizeof(WrUmmRedirectInfo));
   }
   RETVALUE(ROK); 
}
/**
 *  @brief This function checks if UE supports the band
 *
 *      Function: wrChkGeranBandSup 
 *
 *          Processing steps:
 *          - Loop through the UE supported band list 
 *          - Match bandInd with UE supported band list 
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @param[in]  bandInd   :  Band Indicator
 *  @return Bool 
    -# Match Found : TRUE
    -# Otherwise : FALSE 
 */
PUBLIC Bool wrChkGeranBandSup
(  
  WrUeCapInfo *ueCap,
  U8    bandInd
)
{
   U32     idx;
   U32     noComp;
   U32     gsmBand;
   Bool    ret = FALSE;

   ret = wrChkGeranSupp(ueCap);

   if(FALSE == ret)
   {
      RETVALUE(ret);
   }
   noComp = 
      ueCap->eUTRA_Cap.interRAT_Params.geran.supportedBandLstGERAN.noComp.val;
   
   for(idx = 0; idx < noComp; idx++)
   {
      gsmBand = 
      ueCap->eUTRA_Cap.interRAT_Params.geran.supportedBandLstGERAN.member[idx].val;
   
      /*Check for GSM900E*/
      if ((NhuSuppBandGERANgsm900EEnum == gsmBand) && 
            (WR_GERAN_GSM900 == bandInd))
      {
          RETVALUE(TRUE);
      }
      /*Check for GSM850*/
      if ((NhuSuppBandGERANgsm850Enum == gsmBand) && 
            (WR_GERAN_GSM850 == bandInd))
      {
          RETVALUE(TRUE);
      }
      /*Check for DCS1800 or PCS1900*/
      if ((NhuSuppBandGERANgsm1800Enum == gsmBand) && 
            (WR_GERAN_DCS1800 == bandInd)) 
      {
         RETVALUE(TRUE);
      }
      if((NhuSuppBandGERANgsm1900Enum == gsmBand) && 
             (WR_GERAN_PCS1900 == bandInd))
      {
          RETVALUE(TRUE);
      }
   }

   RLOG0(L_WARNING,"UE does not support GERAN bands");
   RETVALUE(FALSE); 

}  
 
/**
 *  @brief This function finds candidate neighbour cells for measurements  
 *
 *      Function: wrFndCandidateNeighs 
 *
 *          Processing steps:
 *              1. Derive wrCellCb                                 
 *              2. Get the in-use cell list
 *              3. Loop through the list matching the PLMN
 *              4. Store the matching ngh infor in transCb
 *              5. Return TRUE if more than one ngh else FALSE
 *
 *  @param[in]  transCb          :  Transaction control block  
 *  @return Bool 
    -# Match Found : TRUE
    -# Otherwise : FALSE 
 */
PUBLIC Bool wrFndCandidateNeighs 
(
 WrUmmTransCb   *transCb
)
{
   WrCellCb       *cellCb;
   CmLList        *lnk = NULLP;
   WrGeranCellCb  *cellNode = NULLP;
   U32            nghCnt = 0;
   WrUeCb         *ueCb = transCb->ueCb;

   WR_GET_CELLCB(cellCb, ueCb->cellId);

   if(NULLP == cellCb)
   {        
      RLOG1(L_ERROR, "Error in getting WR CELL CB for cellId = %lu",
            (U32)ueCb->cellId);
      RETVALUE(RFAILED);
   }
   
   CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,lnk);
   while(NULLP != lnk)
   {
      cellNode = (WrGeranCellCb *)lnk->node;
      if((TRUE ==  wrChkGeranBandSup(ueCb->ueCap,cellNode->freqCb->bandInd)) && 
            (TRUE == wrFindNghCellLaiPres(ueCb,&cellNode->plmnId,cellNode->lac, 0)))
      {
         transCb->u.csfbGeranTrans.measObjCfgdNeig[nghCnt++] = 
            (WrUmmGeranCellCb*)cellNode;
      }
      lnk = lnk->next;
   }

   transCb->u.csfbGeranTrans.cfgdNeighCnt = nghCnt;

   if(nghCnt > 1)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}

/**
 *  @brief This function will fill the ratLst with RAT types
 *
 *      Function: wrFillRatLstRatType 
 *
 *          Processing steps:
 *             - Check the repective RAT types in UE cap and if its octet string
 *             is present and fill the ratType array
 *
 *  @param[in]  *ueCap        :  UE capability
 *  @param[in]  *ratLst       :  Array of RAT types
 *  @param[in]  *handInUeCdma :  Set to TRUE if CDMA ratType is included
 *  @return Bool 
    -# Match Found : TRUE
    -# Otherwise : FALSE 
 */
PUBLIC Void wrFillRatLstRatType
(
 WrUeCapInfo   *ueCap,
 WrUmmRatLst   *ratLst,
 Bool          *handInUeCdma
)
{
   U32                                idx = 0;
   NhuUE_EUTRA_CapinterRAT_Params     *iRATParams = NULLP;

   iRATParams = &ueCap->eUTRA_Cap.interRAT_Params;

   /*Fill EUTRAN RAT type*/
   ratLst->ratType[idx++] = NhuRAT_TypeutraEnum;

   /*Fill CDMA2000 RAT type*/
   if((FALSE == *handInUeCdma) && 
         (PRSNT_NODEF == iRATParams->cdma2000_1xRTT.pres.pres) &&
         (0 < iRATParams->cdma2000_1xRTT.supportedBandLst1XRTT.noComp.val)
         && (ueCap->ueCdmaOctStr.pres != PRSNT_NODEF))
   {
      ratLst->ratType[idx++] = NhuRAT_Typcdma2000_1XRTTEnum;  
      *handInUeCdma = TRUE;
   }
   /*Fill GERAN RAT Type*/
   /* setting handInUeCdma to TRUE is a not a good solution
    * need to be addressed */
   if( (FALSE == *handInUeCdma) &&
         (PRSNT_NODEF == iRATParams->geran.pres.pres) && 
         (0 < iRATParams->geran.supportedBandLstGERAN.noComp.val)
         && ((ueCap->ueGeranPsOctStr.pres != PRSNT_NODEF) ||
            (ueCap->ueGeranCsOctStr.pres != PRSNT_NODEF)))
   {
      ratLst->ratType[idx++] = NhuRAT_Typgeran_csEnum;
      ratLst->ratType[idx++] = NhuRAT_Typgeran_psEnum;
      *handInUeCdma = TRUE;
   }
   ratLst->noComp = idx;
   RETVOID;
}
/**
 *  @brief This function checks if UE supports the GERAN band
 *
 *      Function: wrChkGeranSupp
 *
 *          Processing steps:
 *             - Check UE cap if IRAT GERAN is present in EUTRA capabilities
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @return Bool 
    -# If Present : TRUE
    -# Otherwise  : FALSE 
 */
PUBLIC Bool wrChkGeranSupp
(
 WrUeCapInfo *ueCap
)
{
   if((PRSNT_NODEF != ueCap->eUTRA_Cap.interRAT_Params.pres.pres) || 
         (PRSNT_NODEF != ueCap->eUTRA_Cap.interRAT_Params.geran.pres.pres))
   {
      RLOG0(L_WARNING,"UE does not support GERAN RAT ");
      RETVALUE(FALSE);
   }
   RETVALUE(TRUE);
}

/**
 *  @brief This function checks if UE supports the UTRAN-FDD band
 *
 *      Function: wrChkUtranFddSupp
 *
 *          Processing steps:
 *             - Check UE cap if IRAT FDD is present in EUTRA capabilities
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @return Bool 
    -# If Present : TRUE
    -# Otherwise  : FALSE 
 */
PUBLIC Bool wrChkUtranFddSupp
(
 WrUeCapInfo *ueCap
)
{
   if(PRSNT_NODEF != ueCap->eUTRA_Cap.interRAT_Params.pres.pres)
   {
      RLOG0(L_WARNING,"UE does not support UTRAN FDD RAT ");
      RETVALUE(FALSE);
   }

   if((PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.utraFDD.pres.pres) && 
         (ueCap->eUTRA_Cap.interRAT_Params.utraFDD.supportedBandLstUTRA_FDD.noComp.val > 0))
   {
      RLOG0(L_INFO,"UE supports UTRAN FDD ");
      RETVALUE(TRUE);
   }
   else
   {
      RLOG0(L_WARNING,"UE does not support UTRAN FDD RAT ");
      RETVALUE(FALSE);
   }
}

/**
 *  @brief This function checks if UE supports the CDMA2000-1xRTT band
 *
 *      Function: wrChkUtranFddSupp
 *
 *          Processing steps:
 *             - Check UE cap if IRAT CDMA2000-1xRTT  is present in EUTRA capabilities
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @return Bool 
    -# If Present : TRUE
    -# Otherwise  : FALSE 
 */
PUBLIC Bool wrChkCdma2000_1xRTTSupp
(
 WrUeCapInfo *ueCap
)
{
   if(PRSNT_NODEF != ueCap->eUTRA_Cap.interRAT_Params.pres.pres)
   {
      RLOG0(L_WARNING,"UE does not support CDMA2000_1xRTT RAT ");
      RETVALUE(FALSE);
   }

   if((PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.pres.pres) && 
         (ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.supportedBandLst1XRTT.noComp.val > 0))
   {
      RLOG0(L_INFO,"UE supports CDMA2000_1xRTT ");
      RETVALUE(TRUE);
   }
   else
   {
      RLOG0(L_WARNING,"UE does not support CDMA2000_1xRTT RAT ");
      RETVALUE(FALSE);
   }
}


/**
 *  @brief This function checks if UE supports the CDMA2000_HRPD band
 *
 *      Function: wrChkUtranCdma2000_HRPDSupp
 *
 *          Processing steps:
 *             - Check UE cap if IRAT CDMA2000_HRPD  is present in EUTRA capabilities
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @return Bool 
    -# If Present : TRUE
    -# Otherwise  : FALSE 
 */
PUBLIC Bool wrChkCdma2000_HrpdSupp
(
 WrUeCapInfo *ueCap
)
{
   if(PRSNT_NODEF != ueCap->eUTRA_Cap.interRAT_Params.pres.pres)
   {
      RLOG0(L_WARNING,"UE does not support CDMA2000_HRPD RAT ");
      RETVALUE(FALSE);
   }

   if((PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.pres.pres) && 
         (ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.supportedBandLstHRPD.noComp.val > 0))
   {
      RLOG0(L_INFO,"UE supports CDMA2000_HRPD ");
      RETVALUE(TRUE);
   }
   else
   {
      RLOG0(L_WARNING,"UE does not support CDMA2000_Hrpd RAT ");
      RETVALUE(FALSE);
   }
}

/**
 *  @brief This function checks if UE supports the UTRAN-TDD band
 *
 *      Function: wrChkUtranTddSupp
 *
 *          Processing steps:
 *             - Check UE cap if IRAT GERAN is present in EUTRA capabilities
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @return Bool 
    -# If Present : TRUE
    -# Otherwise  : FALSE 
 */
PUBLIC Bool wrChkUtranTddSupp
(
 WrUeCapInfo *ueCap
)
{
   if(PRSNT_NODEF != ueCap->eUTRA_Cap.interRAT_Params.pres.pres)
   {
      RLOG0(L_WARNING,"UE does not support IRAT, Hence UTRAN TDD RAT also not supported");
      RETVALUE(FALSE);
   }

   else if((PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.pres.pres) && 
         (ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.supportedBandLstUTRA_TDD128.noComp.val > 0))
   {
      RLOG0(L_INFO,"UE supports UTRAN TDD 128 ");
      RETVALUE(TRUE);
   }
   else if ((PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.pres.pres) && 
         (ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.supportedBandLstUTRA_TDD384.noComp.val > 0))
   {
      RLOG0(L_INFO,"UE supports UTRAN TDD 384 ");
      RETVALUE(TRUE);
   }
   else if ((PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.pres.pres) && 
         (ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.supportedBandLstUTRA_TDD768.noComp.val > 0))
   {
      RLOG0(L_INFO,"UE supports UTRAN TDD 768 ");
      RETVALUE(TRUE);
   }
   else
   {
      RLOG0(L_WARNING,"UE does not support none of the UTRAN TDD Bands");
      RETVALUE(FALSE);
   }
}


/**
 *  @brief This function checks if UE supports the UTRAN-TDD band
 *
 *      Function: wrChkUtranTddSupp
 *
 *          Processing steps:
 *             - Check UE cap if IRAT GERAN is present in EUTRA capabilities
 *
 *  @param[in]  *ueCap    :  UE capability
 *  @return Bool 
    -# If Present : TRUE
    -# Otherwise  : FALSE 
 */
PUBLIC Void  wrChkUtranTddERedirSupp
(
 WrUeCapInfo *ueCap,
 WrRedirectionUtraSupp *redirType
)
{
	*redirType = WR_REDIRECTION_NOT_SUPP;

	if((ueCap->eUTRA_Cap.accessStratumRls.val > NhuAccessStratumRlsrel8Enum) &&
         (ueCap->eUTRA_Cap.nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
			( ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.pres.pres == PRSNT_NODEF))
	{
		if( ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.e_RedirectionUTRA_r9.val
				== NhuIRAT_ParamsUTRA_v920e_RedirectionUTRA_r9supportedEnum)
		{
			*redirType = WR_REDIRECTION_REL9_SUPP;		
		}
	}

   if(ueCap->eUTRA_Cap.accessStratumRls.val == NhuAccessStratumRlsrel10Enum)
   {
      if ((ueCap->eUTRA_Cap.nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
            ( ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF)  &&
            ( ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
            ( ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.interRAT_ParamsUTRA_TDD_v1020.pres.pres == PRSNT_NODEF))
      {
         if ((ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.interRAT_ParamsUTRA_TDD_v1020.\
                  e_RedirectionUTRA_TDD_r10.pres == PRSNT_NODEF)
               && (ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.interRAT_ParamsUTRA_TDD_v1020.\
                  e_RedirectionUTRA_TDD_r10.val == NhuIRAT_ParamsUTRA_TDD_v1020e_RedirectionUTRA_TDD_r10supportedEnum))
         {
            *redirType = WR_REDIRECTION_REL10_SUPP;			
         }
      }
   }
}

/** @brief This function is used to find suitable RAT for  CSFB transaction.
 *
 * @details
 *
 *     Function: wrRatSelectionAlgorithm
 *     
 *         Processing steps:
 *     1) Find RAT priority list for this PLMN
 *     2) Find if UE is capable of this RAT
 *     3) Find is TeNB is configured with LAI/HRL filtered cells so that we can select this RAT.
 *     
 * @param[in]  transCb    : pointer to CSFb transaction control block
 * @return S16
 *    -#Success : ROK
 */
	
PUBLIC S16 wrRatSelectionAlgorithm 
(
   WrUmmTransCb	*transCb, 
   wrCsfbRatType *ratType
)
{
   U16                        cellId;
   WrUeCb                     *ueCb;
   WrUmmCellCb                *ummCellCb;
   CmLList                    *lnk = NULLP;
   WrCellCb                   *emmCellCb;
   WrRatPriorityList           ratPriorityList;
   U32                         count = 0;
   Bool                        isUeSupported = FALSE;
   WrUmmMeasCdmaMeasObjNode   *cdmaMeasObj = NULLP;
   U8                          targetBand;
   WrNrUtranFreqCb            *utranFreqCb = NULLP;
   WrUtraNeighCellCb          *neighCellCb = NULLP;
   CmLListCp                  *utraTddFreqLst = NULLP;
   WrUtraFreqCb               *utraFreqCb = NULLP;
   WrNrUtranFreqCb            *freqNode  = NULLP;

   TRC2(wrRatSelectionAlgorithm)
      RLOG0(L_INFO, "wrRatSelectionAlgorithm Called ");

   ueCb = transCb->ueCb;
   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (NULLP == ummCellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   WR_GET_CELLCB(emmCellCb, ueCb->cellId);
   if(NULLP == emmCellCb)
   {			
      RLOG1(L_ERROR, "Error in getting WR CELL CB for cellId = %lu",
            (U32)ueCb->cellId);
      RETVALUE(RFAILED);
   }

   if(RFAILED == wrEmmGetRatPriority(&ueCb->plmnId, &ratPriorityList, cellId))
   {
      RLOG0(L_ERROR, " Failure in finding PLMN-RAT priority ");
      RETVALUE(RFAILED);
   }

   if ((ratPriorityList.numberOfRats == 0) || 
         (ratPriorityList.numberOfRats > MAX_RAT_TYPE))
   {
      RLOG1(L_ERROR, "Failure in finding PLMN-RAT priority"
            "ratPriorityList.numberOfRats = %d", ratPriorityList.numberOfRats);
      RETVALUE(RFAILED);
   }

   for (count = 0; count < ratPriorityList.numberOfRats; count++)
   {
      if (wrChkSingleRatForbidden(ueCb, &ratPriorityList.rat[count]))
         continue;

      switch(ratPriorityList.rat[count])
      {
         case GERAN:
            {
               RLOG0(L_INFO, "GERAN PLMN RAT is Configured ");
               if(TRUE != wrChkGeranSupp(ueCb->ueCap))
               {
                  continue;
               }

               if(emmCellCb->geranNbrFreqLst.count > 0) 
               {						
                  RLOG0(L_INFO, "GERAN PLMN RAT is SELECTED");
                  *ratType = GERAN;
                  RETVALUE(ROK);
               }

               break;
            }

         case UTRA_FDD:
            {
               RLOG0(L_INFO, "UTRA FDD PLMN RAT is CONFIGURED");
               if(TRUE != wrChkUtranFddSupp(ueCb->ueCap))
               {
                  continue;
               }

               lnk = emmCellCb->nrCb->utranCb.utranFreqList.first;
               while(lnk != NULLP)
               {
                  utranFreqCb = (WrNrUtranFreqCb*)lnk->node;
                  if(utranFreqCb != NULLP)
                  {
                     /*Get bandclass for corresponding ARFCN*/
                     wrUmmFndUTRABandFDD(utranFreqCb->arfcn, &targetBand);

                     /*Check whether the bandclass is UE supported or not*/
                     isUeSupported = wrUmmUTRAFDDBandSupported(ueCb,targetBand);
                     if(isUeSupported == TRUE)
                     {
                        cmLListFirst(&utranFreqCb->nrWlLst);
                        while(cmLListCrnt(&utranFreqCb->nrWlLst))
                        {
                           neighCellCb = (WrUtraNeighCellCb *) cmLListNode(cmLListCrnt(&utranFreqCb->nrWlLst));
                           if ((neighCellCb != NULLP) && 
                                 (TRUE == wrFindNghCellLaiPres(ueCb,
                                                               &neighCellCb->plmnId[0],
                                                               neighCellCb->lac, 1)))
                           {									
                              RLOG0(L_INFO, "UTRA FDD PLMN RAT is SELECTED");
                              *ratType = UTRA_FDD;
                              RETVALUE(ROK);
                           }

                           cmLListNext(&utranFreqCb->nrWlLst);
                        }
                     }						
                  }

                  lnk = lnk->next;
               }				

               break;
            }
         case UTRA_TDD: 
            {			
               RLOG0(L_INFO, "UTRA TDD PLMN RAT is CONFIGURED");
               if(TRUE != wrChkUtranTddSupp(ueCb->ueCap))
               {
                  continue;
               }

               utraTddFreqLst = wrEmmGetUtraTddFreqLst(ueCb->cellId);
               if (NULLP == utraTddFreqLst)
               {
                  RLOG0(L_ERROR, "utraTddFreqLst is NULLP");
                  RETVALUE(NULLP);
               }

               lnk = utraTddFreqLst->first;

               while(lnk != NULLP)
               {
                  utraFreqCb = (WrUtraFreqCb *) lnk->node;
                  targetBand = utraFreqCb->t.utraTddFreq.bandIndicator;

                  isUeSupported = wrUmmUtraTddBandSupported(ueCb, 
                        targetBand, 
                        utraFreqCb->t.utraTddFreq.tddMode);
                  if(isUeSupported == TRUE)
                  {
                     freqNode = wrEmmAnrGetFreqNode(utraFreqCb->t.utraTddFreq.arfcn, 
                           &emmCellCb->nrCb->utranTddCb);
                     if (NULLP == freqNode)
                     {
                        RLOG0(L_ERROR, "freqNode is NULLP");
                        RETVALUE(NULLP);
                     }

                     cmLListFirst(&freqNode->nrWlLst);
                     while(cmLListCrnt(&freqNode->nrWlLst))
                     {
                        neighCellCb = (WrUtraNeighCellCb *) cmLListNode(cmLListCrnt(&freqNode->nrWlLst));
                        if ((neighCellCb != NULLP) && 
                              (TRUE == 
                               wrFindNghCellLaiPres(ueCb, 
                                  &neighCellCb->plmnId[0], 
                                  neighCellCb->lac, 1)))
                        {									
                           RLOG0(L_INFO, "UTRA TDD PLMN RAT is SELECTED");
                           *ratType = UTRA_TDD;
                           RETVALUE(ROK);
                        }

                        cmLListNext(&freqNode->nrWlLst);
                     }
                  }

                  lnk = lnk->next;
               }
               break;

            }

         case CDMA2000_1XRTT: 
         case CDMA2000_HRPD:
            {
               RLOG0(L_INFO, "CDMA2000_1XRTT PLMN RAT is CONFIGURED");
               if(CDMA2000_1XRTT == ratPriorityList.rat[count])
               {
                  if(TRUE != wrChkCdma2000_1xRTTSupp(ueCb->ueCap))
                  {
                     continue;
                  }
                  lnk = ummCellCb->measCellCb->cdma1xMeasObjLst.first;
               }
               else
               {
                  if(TRUE != wrChkCdma2000_HrpdSupp(ueCb->ueCap))
                  {
                     continue;
                  }
                  lnk = ummCellCb->measCellCb->cdmaHrpdMeasObjLst.first;
               }

               /*Enter the loop if "ONLY dualRx" is supported by the eNodeB*/
               if ((TRUE == wrEmmIsDualRxSupported(ueCb->cellId)) &&
                     (FALSE == wrEmmIsDualRxTxSupported(cellId)) &&
                     (LWR_ECSFB_NOT_SUPPORTED == emmCellCb->csfbCfgGrp.eCsfbMeasSup) &&
                     (FALSE == emmCellCb->csfbCfgGrp.cdma1xRttSup))
               {
                  if (FALSE == wrUmmUtilGetUeDualRx(ueCb, 
                           ratPriorityList.rat[count]))
                  {
                     continue;
                  }
                  if (TRUE == wrUmmUtilGetUeDualRxTx(ueCb, 
                           ratPriorityList.rat[count]))
                  {
                     continue;
                  }

               }

               while (NULLP != lnk)
               {
                  cdmaMeasObj = (WrUmmMeasCdmaMeasObjNode*)lnk->node;
                  if(NULLP != cdmaMeasObj)
                  {
                     if((CDMA2000_1XRTT == ratPriorityList.rat[count]) && 
                           (NhuCDMA2000_Typtype1XRTTEnum == cdmaMeasObj->cdmaType))
                     {
                        isUeSupported = wrUmmChk1xRttCdmaBandSupported(ueCb,
                              cdmaMeasObj->bandClass);

                        if ((TRUE == isUeSupported) && 
                              (ummCellCb->measCellCb->cdma1xMeasObjLst.count))
                        {
                           RLOG0(L_INFO, "CDMA2000_1XRTT PLMN RAT is SELECTED");
                           *ratType = CDMA2000_1XRTT;
                           RETVALUE(ROK);
                        }
                     }
                     /* For HRPD it is required */
                     else if((CDMA2000_HRPD == ratPriorityList.rat[count]) && 
                           (NhuCDMA2000_TyptypeHRPDEnum == cdmaMeasObj->cdmaType))
                     {
                        isUeSupported = wrUmmChkCdmaHrpdBandSupported(ueCb,
                              cdmaMeasObj->bandClass);
                        if ((TRUE == isUeSupported) && 
                              (ummCellCb->measCellCb->cdmaHrpdMeasObjLst.count))
                        {
                           RLOG0(L_INFO, "CDMA2000_HRPD PLMN RAT is SELECTED");
                           *ratType = CDMA2000_HRPD;
                           RETVALUE(ROK);
                        }
                     }
                  }

                  lnk = lnk->next;
               }

               break;
            }
         default:
            {
               break;
            }
      }/*end of Switch */
   }/*end of FOR */

   RLOG0(L_INFO, "WR_UMM_TRANS_RELEASE_CALL called with Cause WR_UMM_CTXT_CSFB_REL");
   transCb->state 		= WR_UMM_TRANS_RELEASE_CALL;
   transCb->errorCause	= WR_UMM_CTXT_CSFB_REL; 						  
   RETVALUE(RFAILED);
}	

/**
 *  @brief This function validates whether the neighbor cell's LAI is present in
 *         RegisteredLAI ,Serving PLMN or EPLMN and not in Forbidden LAs list.
 *
 *      Function:  wrFindNghCellLaiPres 
 *
 *          Processing steps:
 *          - check if lai is present in Registered LAI
 *          - check if lai is present in Serving PLMN
 *          - check if lai is present in Equivalent PLMN
 *          - check if lai is NOT present in Forbidden LAs
 *
 *  @param[out] valFlag: Flag
 *  @param[in]  *ueCb            : UE control block
 *              *plmnId          : Ngh cell's PLMN
 *              lac              : Ngh Cell's LAC
 *  @return Bool 
    -# Success : TRUE
    -# Failure : FALSE 
 */
PUBLIC Bool wrFindNghCellLaiPres
(
   WrUeCb          *ueCb,
   WrPlmnId        *plmnId,
   U16             lac,
   U16 				 ratSelAlgo
)
{
   U32             cnt;
   U32             cnt1;
   Bool            valFlag = FALSE;
   WrHoRstrctInfo  *hoRstrctInfo = ueCb->hoRstrctInfo;

   /* Check if Neighbor cell PLMN is equal to Serving PLMN*/
   if(wrEmmPlmnPlmnsEqual(plmnId,&ueCb->regLai.plmnId))
   {
      valFlag = TRUE;
      RLOG0(L_INFO,"Neighbor cell PLMN matches RegLai PLMN");
   }
	
   if(NULLP != hoRstrctInfo)
   {
      if((FALSE == valFlag) && 
            (wrEmmPlmnPlmnsEqual(plmnId,&hoRstrctInfo->srvngPlmn)))
      {
         valFlag = TRUE;
         RLOG0(L_INFO,"Neighbor cell PLMN matches Serving PLMN");
      }

      /*Check if Neighbor cell PLMN is equal to EPLMNs*/
      for(cnt = 0; (valFlag == FALSE && cnt < hoRstrctInfo->noOfEplmn); cnt++)
      {
         if(wrEmmPlmnPlmnsEqual(plmnId, &hoRstrctInfo->ePlmn[cnt]))
         {
            valFlag = TRUE;
            RLOG0(L_INFO,"Neighbor cell PLMN matches E-PLMN");
         }
      } 
      /*Check if Neighbor cell LAI is present in Forbidden LAs list*/
      for(cnt1 = 0;(valFlag == TRUE && cnt1 < hoRstrctInfo->noOfLAs); cnt1++)
      {
         if(TRUE == 
               wrEmmPlmnPlmnsEqual(plmnId,&hoRstrctInfo->frbdnLAs[cnt1].plmnId))
         {
            /*Ignore this Forbidden LA*/
            for(cnt = 0; cnt < hoRstrctInfo->frbdnLAs[cnt1].noOfLac; cnt++)
            {
               if(lac == hoRstrctInfo->frbdnLAs[cnt1].lac[cnt])
               {
                  RLOG0(L_WARNING,"Neighbor cell LAI is Forbidden");
                  RETVALUE(FALSE);
               }
            }
         }
      }
   }
	else
	{
		/*if it is called from RatSelection Algorithm, then no HRL means, this cell is suiatable for CSFB*/
		if (ratSelAlgo == TRUE)
		{
            valFlag = TRUE;
            RLOG0(L_INFO,"HRL not present, hence no restrictions applied for CSFB, hence considering this cell for CSFB.");
      }
	}
   RETVALUE(valFlag);
}

/** @brief: This function is called when during UTRA TDD measCfg for CSFB
 *          only one freq was found for measCfg. So fill that freq and cells
 *          in RedirInfo and call csfbRelUe().
 * */
PUBLIC S16 wrUmmPopUtraRedirAndRel
(
 WrUeCb              *ueCb,
 WrUmmRedirectInfo   *redirInfo
 )
{
   WrUmmMeasUtraMeasObjNode      *utraMeasObj = NULLP;
   WrMeasNrUtraCellNode          *nrWCellObjNode = NULLP;
   TknStrOSXL                    utraSI;
   U8                             idx = 0;
   WrRedirectionUtraSupp		redirType;

   /* As there is only 1 entry, it shud be the first entry of the list. */
   utraMeasObj = (WrUmmMeasUtraMeasObjNode *) 
      cmLListNode(cmLListFirst(&ueCb->hrlMeasCfg.utraTddMeasObjLst));

   wrChkUtranTddERedirSupp(ueCb->ueCap, &redirType);
            if (redirType == WR_REDIRECTION_REL9_SUPP)
            {
               redirInfo->eRedirUtraR9Sup = TRUE;
            }

            if (redirType == WR_REDIRECTION_REL10_SUPP)
            {
               redirInfo->val.utraTddInfo.eRedirUtraR10Sup = TRUE;
            }
   redirInfo->val.utraTddInfo.arfcnTddLst[0] = utraMeasObj->arfcn;
   redirInfo->val.utraTddInfo.noOfTddArfcn++;

   cmLListFirst(&utraMeasObj->cellLst);
   while(cmLListCrnt(&utraMeasObj->cellLst))
   {
      nrWCellObjNode = (WrMeasNrUtraCellNode *) cmLListNode\
                       (cmLListCrnt(&utraMeasObj->cellLst));
      if(TRUE == wrEmmGetNeighUtraTddSIInfo(nrWCellObjNode->psc, 
               utraMeasObj->arfcn, redirInfo->cellId, &utraSI))
         {
            redirInfo->val.utraTddInfo.utraCellRedirInfo[idx].pscId =
            nrWCellObjNode->psc;
            redirInfo->val.utraTddInfo.utraCellRedirInfo[idx].arfcn =
               utraMeasObj->arfcn;
         redirInfo->val.utraTddInfo.utraCellRedirInfo[idx].utraSI = utraSI;
            redirInfo->val.utraTddInfo.noOfPscs++;
            idx++;
      }
      cmLListNext(&utraMeasObj->cellLst);
   } /* End of outer while */
   RETVALUE(ROK);
}

/**
 *  @brief This function gets the absolute 
 *  dedicated TA timer values from the enum
 *
 *      Function: wrUtlGetDedTaTmrVal 
 *
 *
 *  @param[in]  taTimerDedicated     
 *  @param[in]  taTmr   
 *  @return Bool 
 */
PUBLIC Bool wrUtlGetDedTaTmrVal 
(
U8    taTimerDedicated,
U16   *taTmr         
 )
{
   switch(taTimerDedicated)
   {
      case WR_TIME_ALIGNMENT_TIMER_500:
         {
            *taTmr = 500;   
            break;
         }
      case WR_TIME_ALIGNMENT_TIMER_750:
         {
            *taTmr = 750;   
            break;
         }
      case WR_TIME_ALIGNMENT_TIMER_1280:
         {
            *taTmr = 1280;   
            break;
         }
      case WR_TIME_ALIGNMENT_TIMER_1920:
         {
            *taTmr = 1920;   
            break;
         }
      case WR_TIME_ALIGNMENT_TIMER_2560:
         {
            *taTmr = 2560;   
            break;
         }
      case WR_TIME_ALIGNMENT_TIMER_5120:
         {
            *taTmr = 5120;   
            break;
         }
      case WR_TIME_ALIGNMENT_TIMER_10240:
         {
            *taTmr = 10240;   
            break;
         }
      default:
         {
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
}
/** @brief This function finds out RSCP  value of a UTRA Cell   
 * @details
 *
 *     Function: wrUmmGetUtraRscp
 *
 *
 * @param [in]  utraMeasResult
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmGetUtraRscp
(
   NhuMeasResultUTRAmeasResult *utraMeasResult
)
{
   S32 rscp = WR_UMM_MIN_VAL_SHORT;

   if(utraMeasResult->utra_RSCP.pres != NOTPRSNT)
   {
      rscp = utraMeasResult->utra_RSCP.val;
   }
   RETVALUE(rscp);

}/*wrUmmGetUtraRscp*/

/** @brief This function finds out ECNO  value of a UTRA Cell   
 * @details
 *
 *     Function: wrUmmGetUtraEcno
 *
 *
 * @param [in]  utraMeasResult
 * @return U8
 */
PUBLIC U8 wrUmmGetUtraEcno
(
   NhuMeasResultUTRAmeasResult *utraMeasResult
)
{
   U8 ecno = INVALID_ECN0;/*rel10*/

   if(utraMeasResult->utra_EcN0.pres != NOTPRSNT)
   {
      ecno = utraMeasResult->utra_EcN0.val;
   }
   RETVALUE(ecno);

}/*wrUmmGetUtraEcno*/




/* RRC-REL10-Upgrade  start */
PUBLIC Void wrUtlUpdateUeCatV1020
(
 WrUeCb *ueCb, 
 WrUeCapInfo *ueCap
 )
{

   NhuUE_EUTRA_Cap_v920_IEs *tmpEutraCap = &(ueCap->eUTRA_Cap.nonCriticalExtn);
   if((tmpEutraCap->pres.pres == PRSNT_NODEF) &&
         (tmpEutraCap->nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
         (tmpEutraCap->nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
         (tmpEutraCap->nonCriticalExtn.nonCriticalExtn.ue_Category_v1020.pres == PRSNT_NODEF))
   {                      
      ueCb->ueCatV1020 = ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.ue_Category_v1020.val;
   }

   RETVOID;
}/* End of wrUtlUpdateUeCatV1020 */
/* RRC-REL10-Upgrade end */

      

/** @brief This function is responsible to fetch the HRL UeCb
 * to send the RRC Reconfig msg based on updated TAC
*
*  @details
* Function: wrUpdtHrlUeCb 
*
*   Processing steps:
*     - Fetch the UeCb and then call the wrUmmMeasReCfgAftDlNas to send RRC
*     Reconfiguration msg based on updated TAC by OAM/SMM
*
*  @param[in]  cellId: Cell Id
*  @return S16
*       SUCCESS : ROK
*       FAILED  : RFAILED
**/
PUBLIC S16 wrUpdtHrlUeCb
(
 U8             cellId
)
{
   WrUmmCellCb               *cellCb;
   WrUeCb                    *ueCb;
   U8                        ueCnt;

   TRC2(wrUpdtHrlUeCb)

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
   {
      ueCb = cellCb->ueCbs[ueCnt];
      if((NULLP != ueCb) && (NULLP != ueCb->hoRstrctInfo))
      {
         wrUmmMeasReCfgAftDlNas(ueCb);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is used to identify whether an ARFCN is available for 
 *          CSFB towards UTRA.
 *
 * @details
 *
 *     Function: wrUmmChkArfcnAvail
 *
 *         Processing steps:
 *     1) Access all the listed cells of the received utraCellMeasObj
 *     2) Check whether the cell is allowed for mobility or not 
 *     3) if all the cells listed in the received utraCellMeasObj
 *          are forbidden then return as this ARFCN is not avail.
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : TRUE 
 *    -#Failure : FALSE
 */
PRIVATE Bool wrUmmChkArfcnAvail
(
WrUeCb                    *ueCb,
WrNrUtranFreqCb           *freqNode,
U16                       *pscCount,
U16                       *pscLst
 )
{
   WrUtraNeighCellCb        *neighCellCb = NULLP;                               
   U8                       hrlDec = 0;                              

   RLOG0(L_INFO, "wrUmmChkArfcnAvail Called");
   cmLListFirst(&freqNode->nrWlLst);
   while(cmLListCrnt(&freqNode->nrWlLst))
   {
      neighCellCb = (WrUtraNeighCellCb *) cmLListNode(cmLListCrnt(&freqNode->nrWlLst));
      if(neighCellCb == NULLP)
      {
         RLOG0(L_ERROR, "ZERo Cells CONFIGURED ");
         RETVALUE(FALSE);
      }

      /* Check for HRL */
      hrlDec = wrMbltyRestrc(WR_INTER_SYS_UTRA_MBLTY, 
            (PTR *)neighCellCb, NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);

      if((hrlDec != WR_MEAS_MBLTY_NOT_APPL_TO_CELL) && 
            (*pscCount < WR_UMM_MAX_UTRA_CELLS_WITH_SI))
      {
         if (neighCellCb->duplexMode == WR_UTRA_MODE_TDD)
         {
            pscLst[(*pscCount)++] = neighCellCb->t.utraTdd.psc;
         }
         else
         {
            pscLst[(*pscCount)++] = neighCellCb->t.utraFdd.psc;
         }
      }
      cmLListNext(&freqNode->nrWlLst);
   }

   if(*pscCount == 0)
   {
      RLOG0(L_WARNING, "All cells of this arfcn are forbidden");
      RETVALUE(FALSE);
   }
   else
   {
      RLOG1(L_INFO, "PSC Count = [%d]", *pscCount);
      RETVALUE(TRUE);
   }
}

/** @brief This function is used to identify whether registered LAI.plmn cells are available for this  ARFCN.
 *
 * @details
 *
 * 	 Function: wrUmmChkRegPlmnCells
 *
 * 		  Processing steps:
 * 	 1) Access all the listed cells of the received freqNode
 * 	 2) Check whether there is any registered PLMN cell. 
 * 	 3) if all the cells listed in the received freqNode
 * 			are not matching registered LAI then return as this ARFCN is not avail.
 *
 * @param[in]	ueCb: Ue control block
 * @return
 * 	-#Success : TRUE 
 * 	-#Failure : FALSE
 */
PUBLIC Bool wrUmmChkRegPlmnCells
(
WrUeCb                     *ueCb,
WrNrUtranFreqCb            *freqNode,
U16                        *pscCount,
U16                        *pscLst
)
{
	WrUtraNeighCellCb        *neighCellCb = NULLP;										  
	U8                       laiDec = 0;										

   RLOG0(L_INFO, "wrUmmChkRegPlmnCells  Called");
	cmLListFirst(&freqNode->nrWlLst);
	while(cmLListCrnt(&freqNode->nrWlLst))
	{
      neighCellCb = (WrUtraNeighCellCb *) cmLListNode(cmLListCrnt(&freqNode->nrWlLst));
      if(neighCellCb == NULLP)
      {
         RETVALUE(FALSE);
      }
		
		/* Check for Registered LAI.plmn */
      laiDec = wrFindRegLaiPlmnCell(&ueCb->regLai, neighCellCb, ueCb->hoRstrctInfo);
		
      if((laiDec == TRUE) && (*pscCount < WR_UMM_MAX_UTRA_CELLS_WITH_SI))
      {
         if (neighCellCb->duplexMode == WR_UTRA_MODE_TDD)
         {
            pscLst[(*pscCount)++] = neighCellCb->t.utraTdd.psc;
         }
         else
         {
            pscLst[(*pscCount)++] = neighCellCb->t.utraFdd.psc;
         }
      }
      cmLListNext(&freqNode->nrWlLst);
   }

	if(*pscCount == 0)
	{
		RLOG0(L_WARNING, "All cells of this arfcn are forbidden, LAI not matching");
      RETVALUE(FALSE);
	}
	else
   {
		RLOG1(L_INFO, "wrUmmChkRegPlmnCells PSC Count = [%d]", *pscCount);
      RETVALUE(TRUE);
   }
}


/** @brief This function is used to Fill UTRA TDD redirection information for RRC
 * connection release
 *
 * @details
 *
 * 	 Function: wrUmmFillDfltUtraTddRedirectInfo
 *
 * 		  Processing steps:
 * 	 1) check the Ue capabilities
 * 	 3) if Ue cap is present in ue cb select UTRA TDD frequency based on Ue
 * 	 capabilities
 *
 * @param[in]	ueCb: Ue control block
 * @return
 * 	-#Success : WrUmmRedirectInfo*
 * 	-#Failure : RFAILED
 */
PUBLIC  WrUmmRedirectInfo* wrUmmFillDfltUtraTddRedirectInfo
(
	WrUeCb						  *ueCb,
	Bool								laiCellsTobeSent
)
{
   WrUmmCellCb                *cellCb = NULLP;
   CmLListCp                  *utraTddFreqLst = NULLP;
   CmLList                    *lnk = NULLP;
   WrUtraFreqCb               *utraFreqCb = NULLP;
   U8                         targetBand = 0;
   Bool                       isUeSupported;
   WrNrUtranFreqCb            *freqNode  = NULLP;
   WrUmmRedirectInfo          *redirInfo = NULLP;
   Bool                       laiCellsFound = FALSE;
   WrUtraNeighCellCb          *nbrUtraCellNode = NULLP;
   CmLListCp                  *utraCellLst = NULLP;
   CmLList                    *nbrUtraCelllnk = NULLP;
   U8                         index1			  = 0;
   WrRedirectionUtraSupp      redirType;
   U8                         wrMaxArfcn = 0; 
   U16                        pscLst[WR_UMM_MAX_UTRA_CELLS_WITH_SI]={0}; 
   U16                        pscCount = 0;
   U16                        idx = 0;
   Bool                       arfcnAvail = FALSE;
   WrNrUtranCb                *utranCb;

   TRC2(wrUmmFillDfltUtraTddRedirectInfo)

      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, "Failed to get cellCB");
         RETVALUE(NULLP);		
      }

   if(ueCb->ueCap == NULLP)
   {
      RETVALUE(NULLP);
   } 

   wrChkUtranTddERedirSupp(ueCb->ueCap, &redirType);

   switch(redirType)
   {
      case WR_REDIRECTION_NOT_SUPP:
      case WR_REDIRECTION_REL9_SUPP:
         {
            wrMaxArfcn = 1;
         }
         break;
      case WR_REDIRECTION_REL10_SUPP:
         {
            wrMaxArfcn = WR_UMM_MAX_UTRA_TDD_ARFCN_REL10;
         }
         break;
      default:
         break;
   }

   utraTddFreqLst = wrEmmGetUtraTddFreqLst(ueCb->cellId);
   if (NULLP == utraTddFreqLst)
   {
      RLOG0(L_ERROR, "utraTddFreqLst is NULLP");
      RETVALUE(NULLP);
   }
   lnk = utraTddFreqLst->first;

   while(lnk != NULLP)
   {		
      pscCount = 0;
      utraFreqCb = (WrUtraFreqCb *) lnk->node;


      targetBand = utraFreqCb->t.utraTddFreq.bandIndicator;
      isUeSupported = wrUmmUtraTddBandSupported(ueCb, targetBand, 
                                      utraFreqCb->t.utraTddFreq.tddMode);

      if(isUeSupported == TRUE)
      {
         utranCb = wrEmmGetUtranCb(ueCb->cellId, WR_UTRA_MODE_TDD);
         if (utranCb == NULLP)
         {
            RLOG0(L_ERROR, "wrEmmGetUtranCb returns NULLP");
            RETVALUE(NULLP);
         }

         freqNode = wrEmmAnrGetFreqNode(utraFreqCb->t.utraTddFreq.arfcn, utranCb);
         if (NULLP == freqNode)
         {
            RLOG0(L_ERROR, "freqNode is NULLP");
            RETVALUE(NULLP);
         }

         if (laiCellsTobeSent == TRUE)
         {
            /*LAI*/
            arfcnAvail = wrUmmChkRegPlmnCells(ueCb, freqNode, &pscCount, pscLst);
            if(arfcnAvail == FALSE)
            {
               lnk = lnk->next;
               if ((lnk == NULLP) && (laiCellsFound == FALSE))
               {
                  /*This is added to send equivalent cells information 
                   * in redirectInfo in case no LAI.plmn cells are found*/
                  lnk = utraTddFreqLst->first;
                  laiCellsTobeSent = FALSE;
               }
               continue;
            }
            laiCellsFound = TRUE;
         }	
         else if ((ueCb->ueServTyp == WR_UMM_NORMAL_SERV) &&
                                     (ueCb->hoRstrctInfo != NULLP))
         {
            /*For HRL*/
            /* Check whether all the cells of this UTRA arfcn are forbidden or not. */
            arfcnAvail = wrUmmChkArfcnAvail(ueCb, freqNode, &pscCount, pscLst);
            if(arfcnAvail == FALSE)
            {
               lnk = lnk->next;
               continue;
            }
         }

         RLOG0(L_DEBUG, "Filling UTRA TDD Redirection Info");	

         if (redirInfo == NULLP)
         {
            WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
            if (NULLP == redirInfo)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(NULLP);
            }

            redirInfo->cellId = ueCb->cellId;
            redirInfo->ratTyp = WR_UMM_MEAS_UTRA_TDD_FREQ;

            if (redirType == WR_REDIRECTION_REL9_SUPP)
            {
               redirInfo->eRedirUtraR9Sup = TRUE;
            }

            if (redirType == WR_REDIRECTION_REL10_SUPP)
            {
               redirInfo->val.utraTddInfo.eRedirUtraR10Sup = TRUE;
            }
         }

         redirInfo->val.utraTddInfo.arfcnTddLst[redirInfo->val.utraTddInfo.noOfTddArfcn++] = freqNode->arfcn;

         if (redirType != WR_REDIRECTION_NOT_SUPP)
         {
            /*Get the UTRA WL cell count */
            utraCellLst = &freqNode->nrWlLst;
            nbrUtraCelllnk = utraCellLst->first;
            idx = 0;
            while (nbrUtraCelllnk != NULLP)
            { 
               nbrUtraCellNode = (WrUtraNeighCellCb*) nbrUtraCelllnk->node;
               if (nbrUtraCellNode != NULLP)
               {
                  /*check if maximum cells possible for Redirection information
                   * is reached*/
                  if(index1 < WR_UMM_MAX_UTRA_CELLS_WITH_SI)
                  {					 
                     /*if selected cell is the one in PSCList*/
                     if (arfcnAvail == TRUE)
                     {
                        if(pscLst[idx] == nbrUtraCellNode->t.utraTdd.psc)
                        {
                           if(nbrUtraCellNode->utraSI.pres == PRSNT_NODEF)
                           {
                              /*if cells donot have SI, they will not be added in the list...*/
                              redirInfo->val.utraTddInfo.utraCellRedirInfo[index1].utraSI = 
                                 nbrUtraCellNode->utraSI;	
                              redirInfo->val.utraTddInfo.utraCellRedirInfo[index1].pscId = 
                                 nbrUtraCellNode->t.utraTdd.psc;
                              redirInfo->val.utraTddInfo.utraCellRedirInfo[index1].arfcn = 
                                 nbrUtraCellNode->t.utraTdd.arfcn;								
                              index1++;
                           }
                           /*all cells present in the pscLst are searched, so need
                            * to break from the list*/
                           if (++idx >= pscCount)
                           {
                              break;
                           }
                        }
                     }
                     else
                     {
                        if(nbrUtraCellNode->utraSI.pres == PRSNT_NODEF)
                        {
                           /*if cells donot have SI, they will not be added in the list...*/
                           redirInfo->val.utraTddInfo.utraCellRedirInfo[index1].utraSI = 
                              nbrUtraCellNode->utraSI;	
                           redirInfo->val.utraTddInfo.utraCellRedirInfo[index1].pscId = 
                              nbrUtraCellNode->t.utraTdd.psc;
                           redirInfo->val.utraTddInfo.utraCellRedirInfo[index1].arfcn = 
                              nbrUtraCellNode->t.utraTdd.arfcn;								
                           index1++;
                        }
                     }
                  }
                  else
                  {
                     break;
                  }
               }

               nbrUtraCelllnk = nbrUtraCelllnk->next;
            }

            redirInfo->val.utraTddInfo.noOfPscs = index1;
         }

         /*in case of rel 10 UEs, we have 6 frequencies,
          * hence we need to exit only when we have 6 frequencies.*/
         if ((redirInfo->val.utraTddInfo.noOfTddArfcn >= wrMaxArfcn) ||
                                (index1 >= WR_UMM_MAX_UTRA_CELLS_WITH_SI))
         {
            /*Break out from the freq list and Send redirect Info*/
            break;
         }			
      }

      lnk = lnk->next;
      arfcnAvail = FALSE;

      if ((laiCellsTobeSent == TRUE) && 
            (lnk == NULLP) && (laiCellsFound == FALSE))
      {
         /*we set lnk to the first node. In case LAI.plmn cells are not found,
          * we need to traverse the list and find HRL matching cells if HRL 
          * is there. If HRL is not there, we send any cell.*/
         lnk = utraTddFreqLst->first;
         laiCellsTobeSent = FALSE;
      }
   }

   RETVALUE(redirInfo); 
}

      
/** @brief This function is used to Fill UTRA FDD redirection information for RRC
 * connection release
 *
 * @details
 *
 *     Function: wrUmmFillUtraFddRedirectInfo
 *
 *         Processing steps:
 *     1) check the Ue capabilities
 *     3) if Ue cap is present in ue cb select WCDMA frequency based on Ue
 *     capabilities
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PUBLIC  WrUmmRedirectInfo* wrUmmFillUtraFddRedirectInfo
(
   WrUeCb                    *ueCb,
	Bool								laiCellsTobeSent
)
{
   WrUmmCellCb                *cellCb         = NULLP;
   CmLListCp						*utraFddFreqLst = NULLP;
   CmLList							*lnk            = NULLP;
   WrUtraFreqCb					*utraFreqCb     = NULLP;
   U8 								targetBand      = 0;
   Bool								isUeSupported   = FALSE;
   WrNrUtranFreqCb			  *freqNode        = NULLP;
   WrUmmRedirectInfo 		  *redirInfo       = NULLP;
   Bool								laiCellsFound   = FALSE;
   WrUtraNeighCellCb 		  *nbrUtraCellNode = NULLP;
   CmLListCp					  *utraCellLst     = NULLP;
   CmLList						  *nbrUtraCelllnk  = NULLP;
   U8 								index1			 = 0;
   U16                        pscLst[WR_UMM_MAX_UTRA_CELLS_WITH_SI] = {0}; 
   U16                        pscCount        = 0;
   U16                        idx             = 0;
   Bool		                  arfcnAvail      = FALSE;
   WrNrUtranCb                *utranCb        = NULLP;

   TRC2(wrUmmFillUtraFddRedirectInfo)
   RLOG0(L_INFO, "wrUmmFillUtraFddRedirectInfo called ");

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Failed to get cellCB");
      RETVALUE(NULLP);		
   }

   if(ueCb->ueCap == NULLP)
   {
      RETVALUE(NULLP);
   } 
   utraFddFreqLst = wrEmmGetUtraFddFreqLst(ueCb->cellId);
   if (NULLP == utraFddFreqLst)
   {
      RLOG0(L_ERROR, "utraFddFreqLst is NULLP");
      RETVALUE(NULLP);
   }
   lnk = utraFddFreqLst->first;

   while(lnk != NULLP)
   {		
      pscCount = 0;
      utraFreqCb = (WrUtraFreqCb *) lnk->node;
      if(WR_UTRA_MODE_FDD == utraFreqCb->duplexMode)
      {
         wrUmmFndUTRABandFDD(utraFreqCb->t.utraFddFreq.arfcnDl, &targetBand);

         isUeSupported = wrUmmUTRAFDDBandSupported(ueCb, targetBand);
         /* this should be highest cell reselection priority frequency */
         if(isUeSupported == TRUE)
         {
            utranCb = wrEmmGetUtranCb(ueCb->cellId, WR_UTRA_MODE_FDD);
            if (utranCb == NULLP)
            {
               RLOG0(L_ERROR, "wrEmmGetUtranCb returns NULLP");
               RETVALUE(NULLP);
            }

            freqNode = wrEmmAnrGetFreqNode(utraFreqCb->t.utraFddFreq.arfcnDl, utranCb);
            if (NULLP == freqNode)
            {
               RLOG0(L_ERROR, "freqNode is NULLP");
               RETVALUE(NULLP);
            }

            if (laiCellsTobeSent == TRUE)
            {
               /*LAI*/
               arfcnAvail = wrUmmChkRegPlmnCells(ueCb, freqNode, &pscCount, pscLst);
               if(arfcnAvail == FALSE)
               {
                  lnk = lnk->next;
                  if ((lnk == NULLP) && (laiCellsFound == FALSE))
                  {
                     /*This is added to send equivalent cells information 
                      * in redirectInfo in case no LAI.plmn cells are found*/
                     lnk = utraFddFreqLst->first;
                     laiCellsTobeSent = FALSE;
                  }
                  continue;
               }
               laiCellsFound = TRUE;
            }	
            /* Check whether all the cells of this UTRA arfcn are forbidden or not. */
            else if((ueCb->ueServTyp == WR_UMM_NORMAL_SERV) 
                  && (ueCb->hoRstrctInfo != NULLP))
            {
               RLOG0(L_INFO, "Validating HO restrictions for UTRA FDD Cells");
               arfcnAvail = wrUmmChkArfcnAvail(ueCb, freqNode, &pscCount, pscLst);
               if(arfcnAvail == FALSE)
               {
                  lnk = lnk->next;
                  continue;
               }
            }

            RLOG0(L_DEBUG, "Filling UTRA FDD Redirection Info");   
            WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
            if (NULLP == redirInfo)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(NULLP);
            }
            redirInfo->cellId = ueCb->cellId;
            redirInfo->ratTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
            redirInfo->val.utraInfo.duplexTyp = WR_UMM_MEAS_DUPLEX_MODE_FDD;
            redirInfo->val.utraInfo.arfcn = utraFreqCb->t.utraFddFreq.arfcnDl;
            /*Get the UTRA WL cell count */
            utraCellLst = &freqNode->nrWlLst;
            nbrUtraCelllnk = utraCellLst->first;
            idx = 0;

            while (nbrUtraCelllnk != NULLP)
            { 
               nbrUtraCellNode = (WrUtraNeighCellCb*) nbrUtraCelllnk->node;
               if (nbrUtraCellNode != NULLP)
               {
                  if(index1 < WR_UMM_MAX_UTRA_CELLS_WITH_SI)
                  {
                     if (pscLst[idx] == nbrUtraCellNode->t.utraFdd.psc)
                     {
                        redirInfo->val.utraInfo.utraCellRedirInfo[index1++].pscId = 
                           nbrUtraCellNode->t.utraFdd.psc;
                        if (++idx >= pscCount)                        
                        {
                           /*Since all the PSC in the list is covered, we can
                            * exit from the loop*/
                           break;
                        }
                     }
                     else if (arfcnAvail == FALSE)
                     {
                        redirInfo->val.utraInfo.utraCellRedirInfo[index1++].pscId = 
                           nbrUtraCellNode->t.utraFdd.psc;
                     }
                  }
                  else
                  {
                     break;
                  }
               }
               nbrUtraCelllnk = nbrUtraCelllnk->next;
            }
            RLOG1(L_INFO, "Number of UTRA FDD PSCs = %d", index1);
            redirInfo->val.utraInfo.noOfPscs = index1;
         }
      }
      lnk = lnk->next;
      arfcnAvail = FALSE;

      if ((laiCellsTobeSent == TRUE) && 
            (lnk == NULLP) && (laiCellsFound == FALSE))
      {
         /*we set lnk to the first node. In case LAI.plmn cells are not found,
          * we need to traverse the list and find HRL matching cells if HRL 
          * is there. If HRL is not there, we send any cell.*/
         lnk = utraFddFreqLst->first;
         laiCellsTobeSent = FALSE;
      }
   }
   if(NULLP != redirInfo)
   {
      RLOG2(L_INFO, "UTRA FDD Redirection Info contains ARFCN = %d, Number of"
            "UTRA FDD PSCs = %d", redirInfo->val.utraInfo.arfcn,
            redirInfo->val.utraInfo.noOfPscs);
   }
   RETVALUE(redirInfo);
}

/** @brief This function is used to print the S1ap msgs cause  
 *
 * @details
 *
 *     Function: wrPrintS1apCause 
 *
 *         Processing steps:
 *
 * @param[in]  SztCause 
 * @return
 *    -#Success : 
 *    -#Failure : 
 */
PUBLIC  U8 wrPrintS1apCause 
(
   SztCause     *cause
)
{
   U8           causeVal = 0;
   switch(cause->choice.val)
   {
      case CAUSE_RADIONW:
         causeVal = cause->val.radioNw.val;
         RLOG_STR(s1apRnwFailCause[causeVal].logLevel,"[S1AP]:RADIO NW failure with cause as [%s]",
                  (PSTR)s1apRnwFailCause[causeVal].cause);
         break;
      case CAUSE_TRANSPORT:
         causeVal = cause->val.transport.val;
         RLOG_STR(s1apTportFailCause[causeVal].logLevel,"[S1AP]:TRANSPORT failure with cause as [%s]",
                  (PSTR)s1apTportFailCause[causeVal].cause);
         break;
      case CAUSE_NAS:
         causeVal = cause->val.nas.val;
         RLOG_STR(s1apNasFailCause[causeVal].logLevel,"[S1AP]:NAS failure with cause as [%s]",
                  (PSTR)s1apNasFailCause[causeVal].cause);
         break;
      case CAUSE_PROTOCOL:
         causeVal = cause->val.protocol.val;
         RLOG_STR(s1apProtFailCause[causeVal].logLevel,"[S1AP]:PROTOCOL failure with cause as [%s]",
                  (PSTR)s1apProtFailCause[causeVal].cause);
         break;
      case CAUSE_MISC:
         causeVal = cause->val.misc.val;
         RLOG_STR(s1apMiscFailCause[causeVal].logLevel,"[S1AP]:MISCELLANIOUS failure with cause as [%s]",
                  (PSTR)s1apMiscFailCause[causeVal].cause);
         break;
      default:
         RLOG0(L_ERROR,"Invalid S1AP cause case");
         break;
   }
   RETVALUE(causeVal);
}

/** @brief This function is used to print the S1ap msgs cause  
 *
 * @details
 *
 *     Function: wrPrintS1apCauseInfo
 *
 *         Processing steps:
 *
 * @param[in]  SztCause 
 * @return
 *    -#Success : 
 *    -#Failure : 
 */
PUBLIC Void wrPrintS1apCauseInfo
(
U8                           causeType,
U8                           causeVal
)
{
   switch(causeType)
   {
      case CAUSE_RADIONW:
         if (causeVal < 64)
         { 
            RLOG_STR(s1apRnwFailCause[causeVal].logLevel,"[S1AP]:RADIO NW failure with cause as [%s]",
                  (PSTR)s1apRnwFailCause[causeVal].cause);
         }
         break;
      case CAUSE_TRANSPORT:
         if (causeVal < 4)
         { 
            RLOG_STR(s1apTportFailCause[causeVal].logLevel,"[S1AP]:TRANSPORT failure with cause as [%s]",
                  (PSTR)s1apTportFailCause[causeVal].cause);
         }
         break;
      case CAUSE_NAS:
         if (causeVal < 8)
         { 
            RLOG_STR(s1apNasFailCause[causeVal].logLevel,"[S1AP]:NAS failure with cause as [%s]",
                  (PSTR)s1apNasFailCause[causeVal].cause);
         }
         break;
      case CAUSE_PROTOCOL:
         if (causeVal < 8)
         { 
            RLOG_STR(s1apProtFailCause[causeVal].logLevel,"[S1AP]:PROTOCOL failure with cause as [%s]",
                  (PSTR)s1apProtFailCause[causeVal].cause);
         }
         break;
      case CAUSE_MISC:
         if (causeVal < 8)
         { 
            RLOG_STR(s1apMiscFailCause[causeVal].logLevel,"[S1AP]:MISCELLANIOUS failure with cause as [%s]",
                  (PSTR)s1apMiscFailCause[causeVal].cause);
         }
         break;
      default:
         RLOG0(L_ERROR,"Invalid S1AP cause case");
         break;
   }
}

/** @brief This function is used check CPU utilization and
 * take necessary action based upon upper/lower CPU threshold. 
 *
 *
 * @details
 *
 *     Function: wrCpuLoadCntrl
 *
 *         Processing steps:
 *
 * @param[in]  Void 
 * @return
 * RETVOID
 */
PUBLIC Void wrCpuLoadCntrl(CmLteCellId cellId)
{

   U8 instruction = 0;
#ifdef  T2200_FDD_CPU_OVRLD 
   wrChkCpuLoadAndUpdInstr(&instruction, cellId);
#endif
   if ( instruction != 0 )
   {
      wrEmmCellSchdCmdCpuOvrldReCfg(cellId, instruction);
   }
   RETVOID;
} /* wrCpuLoadCntrl */


void getLogTimeStrng(char* ts)
{
#ifndef SS_LINUX
     struct timespec ptime;
#else
     struct timeval ptime;
#endif
     struct tm* tm;
     int microseconds;

#ifndef SS_LINUX
     clock_gettime(CLOCK_REALTIME, &ptime);
     microseconds = ptime.tv_nsec / 1000;
#else
     gettimeofday(&ptime, NULL);
     microseconds = ptime.tv_usec/1000;
#endif
     /* Obtain the time of day, and convert it to a tm
      * struct. --*/
      tm = localtime (&ptime.tv_sec);
    
      if (tm) sprintf(ts,"%d_%d_%d_%d_%d_%d.%03d",tm->tm_year+1900, tm->tm_mon+1,tm->tm_mday,
            tm->tm_hour,tm->tm_min,tm->tm_sec,microseconds);
}

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
