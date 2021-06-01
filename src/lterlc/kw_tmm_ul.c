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

     Name:     LTE-RLC Layer 
  
     Type:     C file
  
     Desc:     Source code for RLC Transparent mode assembly and
               reassembly.This file contains following functions
                
                  --kwTmmQSdu
                  --kwTmmSndToLi
                  --kwTmmRcvFrmLi
                  --kwTmmReEstablish 

     File:     gp_tmm_ul.c

     Sid:      gp_tmm_ul.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:44 2016

     Prg:      vp 

**********************************************************************/
static const char* RLOG_MODULE_NAME="TMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=201;

/** @file gp_tmm_ul.c
@brief RLC Transparent Mode module
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000372     LTE-MAC
 *      1000369     LTE-RRC
 */

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_err.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"

/** @addtogroup tmmode */
/*@{*/

#define KW_MODULE (KW_DBGMASK_TM | KW_DBGMASK_UL)

/**
 * @brief
 *    Handler to process the Data Indication from the lower layer and send the 
 *    SDU to upper layer.
 *
 * @details
 *    This function processes the PDU receivied from the lower layer and sends 
 *    the same pdu as SDU to the upper layer.It sends CRNTI along with the SDU 
 *    for CCCH.
 *            
 * @param[in] gCb     RLC Instance Control Block
 * @param[in] rbCb    RB control block. 
 * @param[in] tCrnti  TCRNTI  
 * @param[in] pdu     PDU
 * 
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 */
#ifdef CCPU_OPT
#ifdef ANSI
PUBLIC Void kwTmmRcvFrmLi
(
KwCb        *gCb,
KwUlRbCb    *rbCb,  
CmLteRnti   tCrnti,  
Buffer      *pdu      
)
#else
PUBLIC Void kwTmmRcvFrmLi(gCb,rbCb, tCrnti, pdu)
KwCb        *gCb;
KwUlRbCb    *rbCb;  
CmLteRnti   tCrnti; 
Buffer      *pdu;  
#endif
#else
#ifdef ANSI
PUBLIC Void kwTmmRcvFrmLi
(
KwCb       *gCb,
KwUlRbCb   *rbCb,         
Buffer     *pdu            
)
#else
PUBLIC Void kwTmmRcvFrmLi(gCb,rbCb, pdu)
KwCb       *gCb;
KwUlRbCb   *rbCb;         
Buffer     *pdu;         
#endif
#endif 
{
   KwuDatIndInfo   *datIndInfo;   /* Data Indication Information */
   MsgLen          msgLen;
 
   TRC2(kwTmmRcvFrmLi) 

   /* Creating static memory for KwuDatIndInfo. #else will be 
    * removed once the sanity testing is performed for all platforms */
#if 1
   KwuDatIndInfo datIndInfoTmp;
   datIndInfo = &datIndInfoTmp;
#else
   KW_SHRABL_STATIC_BUF_ALLOC(gCb->u.ulCb->kwuUlSap->pst.region,
         gCb->u.ulCb->kwuUlSap->pst.pool, datIndInfo, sizeof(KwuDatIndInfo));
#endif
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( datIndInfo == NULLP )
   {   
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
            "Memory Allocation failed UEID:%d CELLID:%d",
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);   
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   KW_MEM_CPY(&(datIndInfo->rlcId),&(rbCb->rlcId),sizeof(CmLteRlcId));
#ifdef CCPU_OPT 
   if ( rbCb->lch.lChType == CM_LTE_LCH_CCCH ) 
   {
      datIndInfo->tCrnti = tCrnti;
   }
#endif 
   gCb->genSts.pdusRecv++;
   SFndLenMsg(pdu, &msgLen);
   gCb->genSts.bytesRecv += msgLen;
   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb,KWU_EVT_DAT_IND, pdu);
   }
   KwUiKwuDatInd( &gCb->u.ulCb->kwuUlSap->pst, 
                  gCb->u.ulCb->kwuUlSap->suId, 
                  datIndInfo, pdu);
   
   RETVOID;
}

/**
 * @brief 
 *    Handler to process the re-establishment request received from the upper 
 *    layer. It is just a hook for future enhancement. 
 *
 *            
 * @param[in] gCb   RLC Instance Control Block
 * @param[in] rbCb  RB control block. 
 *
 * @return  S16
 *    -# ROK 
 *       
 **/
#ifdef ANSI
PUBLIC Void kwTmmUlReEstablish
(
KwCb        *gCb,
KwUlRbCb    *rbCb        
)
#else
PUBLIC Void kwTmmUlReEstablish(rbCb)
KwCb        *gCb;
KwRbCb      *rbCb;          
#endif
{
   TRC2(kwUlTmmReEstablish)

   RLOG_ARG0(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,"do nothing for TMM for ReEstablish");
   RETVOID;
}

#ifdef _cplusplus
}
#endif
/*@}*/
/********************************************************************30**
         End of file:     gp_tmm_ul.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:44 2016
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
/main/1      ---       vp   1. initial release.
*********************************************************************91*/
