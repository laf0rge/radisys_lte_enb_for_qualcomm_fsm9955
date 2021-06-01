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
  
        Name:    LTE-RLC Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-RLC
 
        File:    kw_ptui.c

        Sid:      gp_ptui.c@@/main/TeNB_Main_BR/8 - Mon Aug 11 16:43:17 2014
  
        Prg:     gk
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="UIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=199;

/** @file gp_ptui.c
@brief RLC Upper Interface
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000371     LTE-RLC
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

#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

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
#if defined(MAC_RLC_UL_RBUF) || (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)) || defined(SS_RBUF)
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif
#include "kw.x"
#if defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)
#include "mt_plat_t33.h"
#include "mt_plat_t33.x"
#endif

#ifndef LCKWUIKWU
#define PTKWKWU
#endif

#ifndef LCKWUICKW
#define PTKWCKW
#endif

#ifndef NH
#define PTKWKWU
#define PTKWCKW
#endif

#ifndef PJ 
#define PTKWKWU
#define PTKWCKW
#endif


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

  
/*********************************************************************
 *             Forward Declartion for KWU Porting Functions
 ********************************************************************/
#if defined(PDCP_RLC_DL_RBUF) || defined(SS_RBUF)
PUBLIC S16 kwDlBatchProc ARGS ((Void));
PUBLIC S16 kwUtlDlFreeRlcRBuf ARGS((Void));
EXTERN void kwUtlDlBatchProcHqStaInd ARGS ((Void));
EXTERN Void kwUtlFreeDlMem ARGS((Void));
EXTERN SsRngBufCnt rngCb;
EXTERN S16 kwUtlDlBatchProcPkts ARGS((Void));
#endif

#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
PUBLIC S16 kwDlBatchProcSplit  ARGS((Void));
#endif

#ifdef PTKWKWU
PRIVATE S16 PtUiKwuBndCfm  ARGS ((
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
));

#endif /* PTKWKWU */

PUBLIC S16 PtUiKwuDatCfm ARGS ((
Pst               *pst,
SuId              suId,
KwuDatCfmInfo         *datCfm
));

PUBLIC S16 PtUiKwuStaInd ARGS ((
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
));

PUBLIC S16 PtUiKwuReEstCmpInd ARGS ((
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
));
/* kw005.201 added support for L2 Measurement */
PUBLIC S16 PtUiKwuDiscSduCfm ARGS((
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
));
PUBLIC S16 PtUiKwuFlowCntrlInd ARGS((
Pst                    *pst,
SuId                   suId,
KwuFlowCntrlIndInfo    *flowCntrlIndInfo
));
#ifdef LTE_L2_MEAS
PUBLIC S16 PtUiKwuDatAckInd ARGS((
Pst               *pst,
SuId              suId,
KwuDatAckInfo     *datAckInd
));
#endif
#if (defined(PTKWKWU) || defined(KW_PDCP))
PUBLIC S16 PtUiKwuDatInd ARGS ((
Pst               *pst,
SuId              suId,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
));
#endif /* PTKWKWU || KW_PDCP */


  
/*********************************************************************
 *             Forward Declartion for CKW Porting Functions
 ********************************************************************/
#ifdef PTKWCKW
PRIVATE S16 PtUiCkwBndCfm  ARGS ((
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
));

PUBLIC S16 PtUiCkwCfgCfm ARGS ((
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
));

PUBLIC S16 PtUiCkwUeIdChgCfm ARGS((
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
U32            transId,
CkwUeInfo      *ueInfo,
CmStatus       status
));

#endif /* PTKWCKW */


  
/*********************************************************************
 *             Primitives for KWU interface 
 ********************************************************************/

/* KWU Bind Confirm primitive */

PUBLIC KwuBndCfm kwUiKwuBndCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuBndCfm,            /* 0 - loosely coupled */
#else
   PtUiKwuBndCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef NH
   NhLiKwuBndCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiKwuBndCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
#ifndef KW_PDCP
#ifdef PJ
   PjLiKwuBndCfm,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuBndCfm,      /* 2 - tightly coupled, portable */ 
#endif /* NH */
#endif /* KW_PDCP */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuBndCfm,      /* 3 - light weight loosely coupled */ 
#else
   PtUiKwuBndCfm,      /* 3 - light weight loosely coupled, portable */
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

/* KWU Data Indication primitive */

PUBLIC KwuDatInd kwUiKwuDatIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatInd,            /* 0 - loosely coupled */
#else
   PtUiKwuDatInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef NH
   NhLiKwuDatInd,            /* 1 - tightly coupled, RRC  */
#else
   PtUiKwuDatInd,            /* 1 - tightly coupled, portable */
#endif /* NH */
#ifdef KW_PDCP
#else
#ifdef PJ
   PjLiKwuDatInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDatInd,         /* 2 - tightly coupled, portable */ 
#endif /* NH */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDatInd,         /* 3 - light weight loosely coupled */ 
#else
   PtUiKwuDatInd,         /* 3 - light weight loosely coupled, portable */
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
#endif /* KW_PDCP */
};

#ifndef KW_PDCP
PUBLIC KwuDatCfm kwUiKwuDatCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatCfm,            /* 0 - loosely coupled */
#else
   PtUiKwuDatCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuDatCfm,            /* 1 - tightly coupled, portable */
#ifdef PJ
   PjLiKwuDatCfm,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDatCfm,            /* 2 - tightly coupled, portable */
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDatCfm,         /* 3 - light weight loosely coupled */ 
#else
   PtUiKwuDatCfm,         /* 3 - light weight loosely coupled, portable */
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

/* KWU AM Status Indication primitive */

PUBLIC KwuStaInd kwUiKwuStaIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuStaInd,            /* 0 - loosely coupled */
#else
   PtUiKwuStaInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuStaInd,            /* 2 - tightly coupled, portable */
#ifdef PJ
   PjLiKwuStaInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuStaInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuStaInd,         /* 3 - light weight loosely coupled */ 
#else
   PtUiKwuStaInd,         /* 3 - light weight loosely coupled, portable */
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

PUBLIC KwuReEstCmpInd kwUiKwuReEstCmpIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuReEstCmpInd,            /* 0 - loosely coupled */
#else
   PtUiKwuReEstCmpInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuReEstCmpInd,            /* 1 - loosely coupled, portable  */
#ifdef PJ
   PjLiKwuReEstCmpInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuReEstCmpInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuReEstCmpInd,    /* 3 - light weight loosely coupled */ 
#else
   PtUiKwuReEstCmpInd,    /* 3 - light weight loosely coupled, portable */
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

/* kw005.201 added support for L2 measurement */
PUBLIC KwuDiscSduCfm kwUiKwuDiscSduCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDiscSduCfm,            /* 0 - loosely coupled */
#else
   PtUiKwuDiscSduCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuDiscSduCfm,            /* 1 - loosely coupled portable */
#ifdef PJ
   PjLiKwuDiscSduCfm,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDiscSduCfm,    /* 2 - tightly coupled, PDCP */     
#endif
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDiscSduCfm,    /* 3 - light weight loosely coupled */    
#else
   PtUiKwuDiscSduCfm,    /* 3 - light weight loosely coupled, portable */  
#endif /* PJ */
#endif /*TENB_ACC*/
};
PUBLIC KwuFlowCntrlInd kwUiKwuFlowCntrlIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuFlowCntrlInd,            /* 0 - loosely coupled */
#else
   PtUiKwuFlowCntrlInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuFlowCntrlInd,            /* 1 - loosely coupled portable */
#ifdef PJ
   PjLiKwuFlowCntrlInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuFlowCntrlInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
#ifdef LCKWUIKWU
   cmPkKwuFlowCntrlInd,            /* 3 - light weight loosely coupled */
#else
   PtUiKwuFlowCntrlInd,            /* 3 - light weight loosely coupled, portable  */
#endif /* LCKWUIKWU */
};
#ifdef LTE_L2_MEAS
PUBLIC KwuDatAckInd kwUiKwuDatAckIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatAckInd,            /* 0 - loosely coupled */
#else
   PtUiKwuDatAckInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuDatAckInd,            /* 1 - loosely coupled, portable  */
#ifdef PJ
   PjLiKwuDatAckInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDatAckInd,     /* 1 - tightly coupled, portable */  
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDatAckInd,     /* 3 - light weight loosely coupled, portable */          
#else
   PtUiKwuDatAckInd,     /* 3 - light weight loosely coupled */        
#endif /* PJ */
#endif /*TENB_ACC*/
};
#endif /* LTE_L2_MEAS */
/* KWU AM Data confirm primitive */

#endif /* KW_PDCP */

  
/****************************************************************************
 *                         KWU Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for confirming  the bind request received from KWU
 *        interface.
 *
 * @b Description:
 *
 *        This function send the bind confirm primitive to the RLC user,
 *        when it receives a bind request from its service user.
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] status  Status of Confirmation 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
PUBLIC S16 KwUiKwuBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(KwUiKwuBndCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

} /* end of KwUiKwuBndCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending the data(SDU) from upper layer. 
 *
 * @b Description:
 *
 *        This function is used to transfer a SDU received from the peer
 *        RLC entity to the service user(RRC/PDCP).
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datIndInfo  Data Request Information
 *  @param[in] mBuf         Data Buffer (SDU) 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatInd
(
Pst               *pst,
SuId              suId,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
)
#else
PUBLIC S16 KwUiKwuDatInd(pst, suId, datInd, mBuf)
Pst               *pst;
SuId              suId;
KwuDatIndInfo     *datInd;
Buffer            *mBuf;
#endif
{
   TRC3(KwUiKwuDatInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatIndMt[pst->selector])(pst, suId, datInd, mBuf);

   RETVALUE(ROK);

} /* end of KwUiKwuDatInd */

   PUBLIC int rlcDatCfmsSent = 0;

#ifndef KW_PDCP
/**
 *
 * @brief 
 *
 *        Handler for sending the data confirmation to upper layer. 
 *
 * @b Description:
 *
 *        This function is used to send a confirmation to the service
 *        user about the data received by the peer RLC entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datCfmInfo  Data Confirmation Information
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatCfm
(
Pst               *pst,
SuId              suId,
KwuDatCfmInfo     *datCfm
)
#else
PUBLIC S16 KwUiKwuDatCfm(pst, suId, datCfm)
Pst               *pst;
SuId              suId;
KwuDatCfmInfo     *datCfm;
#endif
{
   rlcDatCfmsSent++;
   TRC3(KwUiKwuDatCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatCfmMt[pst->selector])(pst, suId, datCfm);

   RETVALUE(ROK);

} /* end of KwUiKwuDatCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending the Status Indication to the upper layer. 
 *
 * @b Description:
 *
 *        This function is used only by RLC AM entity.It send status 
 *        indication to the upper layer about the maximum number of 
 *        re-transmissions reached for a RLC AM entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] staInd      RLC Entity Id  
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuStaInd
(
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
)
#else
PUBLIC S16 KwUiKwuStaInd(pst, suId, staInd)
Pst               *pst;
SuId              suId;
KwuStaIndInfo     *staInd;
#endif
{
   TRC3(KwUiKwuStaInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuStaIndMt[pst->selector])(pst, suId, staInd);

   RETVALUE(ROK);

} /* end of KwUiKwuStaInd */


/**
 *
 * @brief 
 *
 *        Handler for sending the Status Indication to the upper layer. 
 *
 * @b Description:
 *
 *        This function is used only by RLC AM entity.It send status 
 *        indication to the upper layer about the maximum number of 
 *        re-transmissions reached for a RLC AM entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] staInd      RLC Entity Id  
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuReEstCmpInd
(
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
)
#else
PUBLIC S16 KwUiKwuReEstCmpInd(pst, suId, rlcId)
Pst               *pst;
SuId              suId;
CmLteRlcId        rlcId;
#endif
{
   TRC3(KwUiKwuReEstCmpInd)
   RLOG0(L_DEBUG, "In KwUiKwuReEstCmpInd");

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuReEstCmpIndMt[pst->selector])(pst, suId, rlcId);

   RETVALUE(ROK);

} /* end of KwUiKwuReEstCmpInd */
/* kw005.201 added support for L2 Measurement */

/**
 *
 * @brief 
 *
 *        Handler for sending the Sdu Disc Cfm  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the discard of an SDU .
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] discCfm     Disc information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDiscSduCfm
(
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
)
#else
PUBLIC S16 KwUiKwuDiscSduCfm(pst, suId, discCfm)
Pst               *pst;
SuId              suId;
KwuDiscSduInfo    *discCfm;
#endif
{
   TRC3(KwUiKwuDiscSduCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDiscSduCfmMt[pst->selector])(pst, suId, discCfm);

   RETVALUE(ROK);

} /* end of KwUiKwuDiscSduCfm */ 

/**
 *
 * @brief 
 *
 *        Handler for sending Flow indication to the upper layer. 
 *
 * @b Description:
 *       
 *       This function indicates to PDCP if packets need to be stopped or
 *       started for a particular RB
 *
 *  @param[in] pst                Post structure  
 *  @param[in] suId               Service User SAP ID 
 *  @param[in] flowCntrlIndInfo   Flow control information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuFlowCntrlInd
(
Pst                   *pst,
SuId                  suId,
KwuFlowCntrlIndInfo   *flowCntrlIndInfo
)
#else
PUBLIC S16 KwUiKwuFlowCntrlInd(pst, suId, flowCntrlIndInfo)
Pst                   *pst;
SuId                  suId;
KwuFlowCntrlIndInfo   *flowCntrlIndInfo;
#endif
{
   TRC3(KwUiKwuFlowCntrlInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuFlowCntrlIndMt[pst->selector])(pst, suId, flowCntrlIndInfo);

   RETVALUE(ROK);

} /* end of KwUiKwuFlowCntrlInd */ 
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *
 *        Handler for sending the Data ack indication  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the succesfull transmission of SDU
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datAckInd   DatAckInd
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatAckInd
(
Pst               *pst,
SuId              suId,
KwuDatAckInfo     *datAckInd
)
#else
PUBLIC S16 KwUiKwuDatAckInd(pst, suId, datAckInd)
Pst               *pst;
SuId              suId;
KwuDatAckInfo     *datAckInd;
#endif
{
   TRC3(KwUiKwuDatAckInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatAckIndMt[pst->selector])(pst, suId, datAckInd);

   RETVALUE(ROK);

} /* end of KwUiKwuDatAckInd */ 
#endif /* LTE_L2_MEAS */
#endif /* KW_PDCP */


#ifdef PTKWKWU
/*************************************************************************
 *                         KWU Porting Functions
 ************************************************************************/
/**
 *
 * @brief 
 *
 *        PtUiKwuBndCfm - Portable SAP bind confirm
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PRIVATE S16 PtUiKwuBndCfm 
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
)
#else
PRIVATE S16 PtUiKwuBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8 status;                      /* status */
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuBndCfm);

   UNUSED(suId);
   UNUSED(status);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);
} /* end of PtUiKwuBndCfm */
#endif /* PTKWKWU */


/**
 *
 * @brief 
 *
 *        PtUiKwuDatCfm - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] datCfm -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDatCfm
(
Pst               *pst,
SuId              suId,
KwuDatCfmInfo    *datCfm
)
#else
PUBLIC S16 PtUiKwuDatCfm(pst, suId, datCfm)
Pst               *pst;
SuId              suId;
KwuDatCfmInfo     *datCfm;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuDatCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datCfm);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
 
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuDatCfm */


/**
 *
 * @brief 
 *
 *        PtUiKwuStaInd - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] staInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuStaInd
(
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
)
#else
PUBLIC S16 PtUiKwuStaInd(pst, suId, staInd)
Pst               *pst;
SuId              suId;
KwuStaIndInfo     *staInd;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuStaInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuStaInd */


/**
 *
 * @brief 
 *
 *        PtUiKwuReEstCmpInd - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] staInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuReEstCmpInd
(
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
)
#else
PUBLIC S16 PtUiKwuReEstCmpInd(pst, suId, rlcId)
Pst               *pst;
SuId              suId;
CmLteRlcId        rlcId;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuReEstCmpInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(rlcId);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuReEstCmpInd */

/* kw005.201 added support for L2 Measurement */
/*
 * @brief 
 *
 *        PtUiKwuDiscSduCfm - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] staInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDiscSduCfm
(
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
)
#else
PUBLIC S16 PtUiKwuDiscSduCfm(pst, suId, discCfm)
Pst               *pst;
SuId              suId;
KwuDiscSduInfo    *discCfm;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuDiscSduCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(discCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuDiscSduCfm */

/*
 * @brief 
 *
 *        PtUiKwuFlowCntrlInd - Portable Flow control idication
 *
 *  @param[in] pst          -  Post structure  
 *  @param[in] suId         -  Service user SAP ID 
 *  @param[in] flowCntrlInd -  Flow control information
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuFlowCntrlInd
(
Pst                    *pst,
SuId                   suId,
KwuFlowCntrlIndInfo    *flowCntrlIndInfo
)
#else
PUBLIC S16 PtUiKwuFlowCntrlInd(pst, suId, flowCntrlIndInfo)
Pst                    *pst;
SuId                   suId;
KwuFlowCntrlIndInfo    *flowCntrlIndInfo;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuFlowCntrlInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(flowCntrlIndInfo);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuFlowCntrlInd */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *
 *        Handler for sending the Data ack indication  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the succesfull transmission of SDU
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datAckInd   DatAckInd
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDatAckInd
(
Pst               *pst,
SuId              suId,
KwuDatAckInfo     *datAckInd
)
#else
PUBLIC S16 PtUiKwuDatAckInd(pst, suId, datAckInd)
Pst               *pst;
SuId              suId;
KwuDatAckInfo     *datAckInd;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuDatAckInd)
   
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datAckInd);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);

   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   RETVALUE(ROK);
}/* end of PtUiKwuDatAckInd */
#endif /* LTE_L2_MEAS */
#if (defined(PTKWKWU) || defined(KW_PDCP))

/**
 *
 * @brief 
 *
 *        PtUiKwuDatInd - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] datInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDatInd
(
Pst               *pst,
SuId              suId,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
)
#else
PUBLIC S16 PtUiKwuDatInd(pst, suId, datInd, mBuf)
Pst               *pst;
SuId              suId;
KwuDatIndInfo     *datInd;
Buffer            *mBuf;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiKwuDatInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);
   UNUSED(mBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuDatInd */
#endif /* PTKWKWU || KW_PDCP */


  
/*********************************************************************
 *             Primitives for CKW interface 
 ********************************************************************/

/* CKW Bind Confirm primitive */

PUBLIC CkwBndCfm kwUiCkwBndCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwBndCfm,            /* 0 - loosely coupled */
#else
   PtUiCkwBndCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCCKUICKW */
#ifdef NH
   NhLiCkwBndCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiCkwBndCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
};

/* CKW Configuration confirm primitive */

PUBLIC CkwCfgCfm kwUiCkwCfgCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwCfgCfm,            /* 0 - loosely coupled */
#else
   PtUiCkwCfgCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUICKW */
#ifdef NH
   NhLiCkwCfgCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiCkwCfgCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
};

PUBLIC CkwUeIdChgCfm kwUiCkwUeIdChgCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwUeIdChgCfm,            /* 0 - loosely coupled */
#else
   PtUiCkwUeIdChgCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUICKW */
#ifdef NH
   NhLiCkwUeIdChgCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiCkwUeIdChgCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
};


  
/****************************************************************************
 *                         CKW Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for confirming  the bind request received from CKW
 *        interface.
 *
 * @b Description:
 *
 *        This function send the bind confirm primitive to the RLC user,
 *        when it receives a bind request from its service user.
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
PUBLIC S16 KwUiCkwBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(KwUiCkwBndCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

} /* end of KwUiCkwBndCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending a configuration confirm to RRC.
 *
 * @b Description:
 *
 *        This function is used by RLC user to send a configuration
 *        confirmation to RRC after configuring(add/delete/modify)
 *        the RLC entities. 
 *
 *  @param[in] pst      Post structure  
 *  @param[in] transId  Transaction Id 
 *  @param[in] cfmInfo  Config Confirmation Info 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwCfgCfm
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
)
#else
PUBLIC S16 KwUiCkwCfgCfm(pst, suId, cfmInfo)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo;                   /* Configuration Confirm */
#endif
{
   TRC3(KwUiCkwCfgCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwCfgCfmMt[pst->selector])(pst, suId, cfmInfo);

   RETVALUE(ROK);

} /* end of KwUiCkwCfgCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending a configuration for UE ID change.
 *
 * @b Description:
 *
 *        This function is used by RLC to send a configuration
 *        confirm for UE ID change.
 *
 *  @param[in] pst      Post structure  
 *  @param[in] suId     Service User Id
 *  @param[in] transId  Transaction Id 
 *  @param[in] cfmInfo  Config Confirmation Info 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwUeIdChgCfm 
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
U32            transId,
CkwUeInfo      *ueInfo,
CmStatus       status
)
#else
PUBLIC S16 KwUiCkwUeIdChgCfm(pst, suId, transId,ueInfo,status)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
U32            transId;
CkwUeInfo      *ueInfo;
CmStatus       status;
#endif
{
   TRC3(KwUiCkwUeIdChgCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   RETVALUE(ROK);

} /* end of KwUiCkwCfgCfm */


#ifdef PTKWCKW
/*************************************************************************
 *                         CKW Porting Functions
 ************************************************************************/
/**
 *
 * @brief 
 *
 *        KwUiCkwBndCfm - CKW SAP bind confirm
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PRIVATE S16 PtUiCkwBndCfm 
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
)
#else
PRIVATE S16 PtUiCkwBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8 status;                      /* status */
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiCkwBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);
} /* end of PtUiCkwBndCfm */


/**
 *
 * @brief 
 *
 *        PtUiCkwCfgCfm - KWU SAP bind confirm
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] cfmInfo  - Configuration confirm
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiCkwCfgCfm
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
)
#else
PUBLIC S16 PtUiCkwCfgCfm(pst, suId, cfmInfo)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo;                   /* Configuration Confirm */
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiCkwCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(cfmInfo);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiCkwCfgCfm */

#ifdef ANSI
PUBLIC S16 PtUiCkwUeIdChgCfm 
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
U32            transId,
CkwUeInfo      *ueInfo,
CmStatus       status
)
#else
PUBLIC S16 PtUiCkwUeIdChgCfm(pst, suId,transId,ueInfo,status)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
U32            transId;
CkwUeInfo      *ueInfo;
CmStatus       status;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif
   TRC3(PtUiCkwUeIdChgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(status);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst->srcInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
   tKwCb = KW_GET_KWCB(pst->srcInst);
   RLOG0(L_ERROR, "Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiCkwUeIdChgCfm */
#endif /* PTKWCKW */

#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
/**
 *
 * @brief 
 *
 *        kwDlBatchProcSplit- process rbug messages
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 kwDlBatchProcSplit 
(
Void
)
#else
PUBLIC S16 kwDlBatchProcSplit()
Void;
#endif
{

/* Read from Ring Buffer and process PDCP packets */
   Void *elmIndx = NULLP;
   RxmBufReq *datReq = NULLP;
   U16 rngBufDeqIndx = 0;
   U16 rngBufDeqMaxCnt;
   U16 rngBufCurrCnt;

   rngBufDeqMaxCnt = SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   rngBufCurrCnt = SGetNumElemInRng(SS_RNG_BUF_RX_TO_DLRLC);
   if ( rngBufCurrCnt > (3 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
   {
      if ( rngBufCurrCnt > (6 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
      {
         /* Restablishment scenario */
         rngBufDeqMaxCnt = (4 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT);
      }
      else
      {
         rngBufDeqMaxCnt = (3 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT)/2;
      }
   }
#endif
   elmIndx = SRngGetRIndx(SS_RNG_BUF_RX_TO_DLRLC);
   while(NULLP != elmIndx)
   {
      datReq = (RxmBufReq *)elmIndx;
      if(datReq->mBuf != NULLP)
      {
        cmUnpkKwuDatReq(KwUiKwuDatReq, &datReq->pst, datReq->mBuf);
      }
      else
      {
         RLOG0(L_ERROR, "mBuf is NULL");
         if(datReq->mBuf)
          cmUnpkKwuDatReq(KwUiKwuDatReq, &datReq->pst, datReq->mBuf);

      }
      SsRngInfoTbl[SS_RNG_BUF_RX_TO_DLRLC].nPktProc++;//Number of pkt processed in tti
      datReq->mBuf = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_RX_TO_DLRLC);
      rngBufDeqIndx++;
      if(rngBufDeqIndx >= rngBufDeqMaxCnt)
      {
         break;
      }

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_RX_TO_DLRLC)) == NULLP)
      {
         break;
      }
   }

   RETVALUE(ROK);
}
#endif

#if defined(PDCP_RLC_DL_RBUF) || defined(SS_RBUF)
#ifdef ANSI
PUBLIC S16 kwDlBatchProc
(
Void
)
#else
PUBLIC S16 kwDlBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */

   U8 rngBufDeqIndx = 0;
   U32 rngBufDeqMaxCnt;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   U32 rngBufCurrCnt;
#endif
   /* Memory regions different for BRCM and Intel */
   /*dstProcId,srcProcId,dstEnt,dstInst,srcEnt,srcInst,prior,route,event,region,pool,selector*/
#ifdef SS_RBUF
   PRIVATE Pst rlcDlRbfuPst ={1,1,ENTKW,1,ENTPJ,1,PRIOR0,RTESPEC,KWU_EVT_DAT_REQ,1,1,0,0};
#else
   PRIVATE Pst rlcDlRbfuPst ={1,1,ENTKW,1,ENTPJ,1,PRIOR0,RTESPEC,KWU_EVT_DAT_REQ,2,1,0,0};
#endif
   Void *elmIndx = NULLP;
   KwuDatReqDetl *kwuDatReqDetl = NULLP;
   KwuDatReqInfo datReq;

   rngBufDeqMaxCnt = SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   rngBufCurrCnt = SGetNumElemInRng(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   if ( rngBufCurrCnt > (3 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
   {
      if ( rngBufCurrCnt > (5 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
      {
         /* Restablishment scenario */
         rngBufDeqMaxCnt = (4 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT);
      }
      else
      {
         rngBufDeqMaxCnt = (2 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT);
      }
   }
#endif
   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   while(NULLP != elmIndx)
   {
      kwuDatReqDetl = (KwuDatReqDetl *)elmIndx;
      datReq.rlcId = kwuDatReqDetl->rlcId;
      datReq.sduId = kwuDatReqDetl->sduId;
      datReq.lcType = kwuDatReqDetl->lcType;
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].nPktProc++;;//Number of pkt processed in tti
      if(kwuDatReqDetl->mBuf != NULLP)
      {
         KwUiKwuDatReq(&rlcDlRbfuPst, kwuDatReqDetl->spId, &datReq, kwuDatReqDetl->mBuf);
      }
      SRngIncrRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      rngBufDeqIndx++;

      if(rngBufDeqIndx >= rngBufDeqMaxCnt)
      {
         break;
      }
      elmIndx = (Void *)SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   }

   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 kwUtlDlBatchProcPkts
(
Void
)
#else
PUBLIC S16 kwUtlDlBatchProcPkts
Void;
#endif
{
    kwDlBatchProc();
#ifdef SS_RBUF
#ifdef LTE_L2_MEAS
    kwUtlDlBatchProcHqStaInd();
#endif
    kwUtlFreeDlMem();
#endif
    RETVALUE(ROK);
}   


#ifdef ANSI
PUBLIC S16 kwUtlDlFreeRlcRBuf
(
Void
)
#else
PUBLIC S16 kwUtlDlFreeRlcRBuf
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */
   Void *elmIndx = NULLP;
   KwuDatReqDetl *kwuDatReqDetl = NULLP;
   /* Free SS_RNG_BUF_DLPDCP_TO_DLRLC  */
   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   while(NULLP != elmIndx)
   {
      kwuDatReqDetl = (KwuDatReqDetl *)elmIndx;
      SRngIncrRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].nPktProc++;
      SPutMsg(kwuDatReqDetl->mBuf);
      elmIndx = NULLP;
      kwuDatReqDetl = NULLP;
      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC)) == NULLP)
      break;
   }
   RETVALUE(ROK);
}


#endif 
#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     gp_ptui.c@@/main/TeNB_Main_BR/8 - Mon Aug 11 16:43:17 2014
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
/main/1      ---      gk   1. initial release.
/main/2      ---      gk   1. LTERLC Release 2.1
/main/3      kw005.201 ap  1. Added support for L2 Measurement feature.
*********************************************************************91*/
