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

        Sid:      kw_ptli.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:25 2014
  
        Prg:     gk
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="LIM";
static int RLOG_FILE_ID=2;
static int RLOG_MODULE_ID=2048;
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
#ifdef KW_PDCP
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#endif
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
#ifdef KW_PDCP
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#endif
#include "kw.x"
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif

#ifndef LCKWLIRGU
#define PTKWRGU
#endif

#ifndef RG
#define PTKWRGU
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */



#ifdef RLC_MAC_DAT_REQ_RBUF
PUBLIC S16 kwLiRguDatReqRbuf(Pst *Post,SpId spId,Void *datReq);
#endif

#ifdef RLC_MAC_STA_RSP_RBUF
PUBLIC S16 kwLiRguStaRspRbuf(Pst *Post,SpId spId,Void  *staRsp);
#endif
#ifdef MAC_RLC_HARQ_STA_RBUF
EXTERN S16 KwDlHarqStaBatchProc (Void);
#endif


/*********************************************************************
 *             Forward Declaration of RGU Porting Functions
 ********************************************************************/
EXTERN S16 PtLiRguBndReq ARGS((
Pst         *post,
SuId        suId,
SpId        spId
));

EXTERN S16 PtLiRguUbndReq ARGS ((
Pst         *post,
SpId        spId,
Reason      reason
));

EXTERN S16 PtLiRguCDatReq ARGS ((
Pst               *post,
SpId              spId,
RguCDatReqInfo    *datReq
));

EXTERN S16 PtLiRguDDatReq ARGS ((
Pst               *post,
SpId              spId,
RguDDatReqInfo    *datReq
));

EXTERN S16 PtLiRguCStaRsp ARGS ((
Pst               *post,
SpId              spId,
RguCStaRspInfo    *staRsp
));

EXTERN S16 PtLiRguDStaRsp ARGS ((
Pst               *post,
SpId              spId,
RguDStaRspInfo    *staRsp
));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
EXTERN S16 PtLiRguL2mUlThrpMeasReq ARGS ((
Pst                      *post,
SpId                     spId,
RguL2MUlThrpMeasReqInfo  *l2mUlThrpMeasReq
));
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS  */


/*********************************************************************
 *             Primitives for RGU interface 
 ********************************************************************/

/* RGU Bind Request primitive */

PUBLIC RguBndReq kwLiRguBndReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguBndReq,            /* 0 - loosely coupled */
#else
   PtLiRguBndReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguBndReq,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguBndReq,            /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguBndReq,            /* 0 - loosely coupled */
#else
   PtLiRguBndReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
};

/* RGU Unbind Request primitive */

PUBLIC RguBndReq kwLiRguUbndReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguUbndReq,            /* 0 - loosely coupled */
#else
   PtLiRguUbndReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguUbndReq,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguUbndReq,            /* 1 - tightly coupled, portable */ /* RG */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguUbndReq,            /* 0 - loosely coupled */
#else
   PtLiRguUbndReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
};

/* RGU Common Channel Data Request primitive */

PUBLIC RguCDatReq kwLiRguCDatReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguCDatReq,            /* 0 - loosely coupled */
#else
   PtLiRguCDatReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguCDatReq,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguCDatReq,            /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguCDatReq,            /* 0 - loosely coupled */
#else
   PtLiRguCDatReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
};

/* RGU Dedicated Channel Data Request primitive */

PUBLIC RguDDatReq kwLiRguDDatReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguDDatReq,            /* 0 - loosely coupled */
#else
   PtLiRguDDatReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguDDatReq,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguDDatReq,            /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguDDatReq,            /* 0 - loosely coupled */
#else
   PtLiRguDDatReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
};

/* RGU Common Channel Status Response primitive */

PUBLIC RguCStaRsp kwLiRguCStaRspMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguCStaRsp,            /* 0 - loosely coupled */
#else
   PtLiRguCStaRsp,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguCStaRsp,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguCStaRsp,            /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguCStaRsp,            /* 0 - loosely coupled */
#else
   PtLiRguCStaRsp,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
};

/* RGU Dedicated Channel Status Response primitive */

PUBLIC RguDStaRsp kwLiRguDStaRspMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguDStaRsp,            /* 0 - loosely coupled */
#else
   PtLiRguDStaRsp,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguDStaRsp,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguDStaRsp,            /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguDStaRsp,            /* 0 - LWLC loosely coupled */
#else
   PtLiRguDStaRsp,            /* 0 - LWLC loosely coupled, portable  */
#endif /* LCRGUIRGU */
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/* RGU L2 Measurement Ul Ip Throughput Measurement Request primitive */

PUBLIC RguL2MUlThrpMeasReq kwLiRguL2MUlThrpMeasReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguL2MUlThrpMeasReq,            /* 0 - loosely coupled */
#else
   PtLiRguL2mUlThrpMeasReq,            /* 0 - loosely coupled, portable  */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguL2MUlThrpMeasReq,            /* 1 - tightly coupled, MAC  */
#else
   PtLiRguL2mUlThrpMeasReq,            /* 1 - tightly coupled, portable */
#endif /* RG */
};
#endif /* LTE_RLC_R9 */
#endif /*  LTE_L2_MEAS */

/****************************************************************************
 *                         RGU Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for RGU SAP bind Request.
 *
 * @b Description:
 *
 *        This function is used by RLC to request for binding to 
 *        MAC for accessing MAC services.This function binds MAC's 
 *        SAP (identified by spId) with the service user's SAP 
 *        (identified by suId).
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 KwLiRguBndReq
(
Pst  *post,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                       /* Service Provider Id */
)
#else
PUBLIC S16 KwLiRguBndReq(post, suId, spId)
Pst  *post;                       /* post structure */
SuId suId;                      /* Service User Id */
SpId spId;                      /* Service Provider Id */
#endif
{
   TRC3(KwLiRguBndReq)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguBndReqMt[post->selector])(post, suId, spId);

   RETVALUE(ROK);

} /* end of KwLiRguBndReq */


/**
 *
 * @brief 
 *
 *        Handler for bind confirmation from MAC.
 *
 * @b Description:
 *
 *        This function handles the bind confirmation received
 *        from MAC. 
 *
 *  @param[in] post     - Post structure  
 *  @param[in] suId    - Service provider SAP ID 
 *  @param[in] reason  - Reason of confirmation
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 KwLiRguUbndReq
(
Pst         *post,
SpId        spId,
Reason      reason
)
#else
PUBLIC S16 KwLiRguUbndReq(post, spId, reason)
Pst         *post;
SpId        spId;
Reason      reason;
#endif
{
   TRC3(KwLiRguUbndReq)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguUbndReqMt[post->selector])(post, spId, reason);

   RETVALUE(ROK);

} /* end of KwLiRguUbndReq */

  
/**
 *
 * @brief 
 *
 *        Handler for sending the PDU from RLC to MAC for common logical channels. 
 *
 * @b Description:
 *
 *        This function sends a PDU of a common logical channel to MAC
 *        along with timing info for BCCH and PCCH and RNTI for CCCH. 
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] datIndInfo  Data Request Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguCDatReq
(
Pst               *post,
SpId              spId,
RguCDatReqInfo    *datReq
)
#else
PUBLIC S16 KwLiRguCDatReq(post, spId, datReq)
Pst               *post;
SpId              spId;
RguCDatReqInfo    *datReq;
#endif
{
   TRC3(KwLiRguCDatReq)
//    printf("CSHP: KwLiRguCDatReq  ");
#ifdef RLC_MAC_DAT_REQ_RBUF
    //printf("to RING  \n");
       post->event=EVTRGUCDATREQ;
      if((kwLiRguDatReqRbuf(post, spId, datReq)) != ROK)
      {

            SPutStaticBuffer(post->region, post->pool,                      
                      (Data *) datReq, sizeof(RguCDatReqInfo), 0);             
         RETVALUE(RFAILED);
      }
#else

   // printf("to Pack Sel is %d  \n",post->selector);
   /* jump to specific primitive depending on configured selector */
   (*kwLiRguCDatReqMt[post->selector])(post, spId, datReq);
#endif
   RETVALUE(ROK);

} /* end of KwLiRguCDatReq */

  
/**
 *
 * @brief 
 *
 *        Handler for sending PDU(s) from RLC to MAC for dedicated logical channels. 
 *
 * @b Description:
 *
 *        This function sends PDU(s) to MAC via one or more dedicated 
 *        logical channels along with the Buffer Occupancy of these
 *        channels.
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] datIndInfo  Data Request Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguDDatReq
(
Pst               *post,
SpId              spId,
RguDDatReqInfo    *datReq
)
#else
PUBLIC S16 KwLiRguDDatReq(post, spId, datReq)
Pst               *post;
SpId              spId;
RguDDatReqInfo    *datReq;
#endif
{
   TRC3(KwLiRguDDatReq)
#ifdef RLC_MAC_DAT_REQ_RBUF
        post->event=EVTRGUDDATREQ;
      if((kwLiRguDatReqRbuf(post, spId, datReq)) != ROK)
      {

      SPutStaticBuffer(post->region, post->pool,                      
                      (Data *) datReq, sizeof(RguDDatReqInfo), 0);             
         RETVALUE(RFAILED);
      }
#else
   /* jump to specific primitive depending on configured selector */
   (*kwLiRguDDatReqMt[post->selector])(post, spId, datReq);
#endif 
   RETVALUE(ROK);

} /* end of KwLiRguDDatReq */

  
/**
 *
 * @brief  
 *
 *        Handler for reporting the Buffer Occupancy to MAC 
 *     for common logical channels.
 *
 * @b Description:
 *
 *        This function reports the Buffer Occupancy of a common logical
 *        channel to MAC along with timing information for BCCH and PCCH
 *        and RNTI for CCCH. 
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] staRspInfo  Status Response Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguCStaRsp
(
Pst               *post,
SpId              spId,
RguCStaRspInfo    *staRsp
)
#else
PUBLIC S16 KwLiRguCStaRsp(post, spId, staRsp)
Pst               *post;
SpId              spId;
RguCStaRspInfo    *staRsp;
#endif
{
   TRC3(KwLiRguCStaRsp)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguCStaRspMt[post->selector])(post, spId, staRsp);
   RETVALUE(ROK);

} /* end of KwLiRguCStaRsp */

  
/**
 *
 * @brief  
 *
 *        Handler for reporting the Buffer Occupancy to MAC 
 *        for dedicated logical channels.
 *
 * @b Description:
 *
 *        This function reports the Buffer Occupancy of one or more
 *        dedicated logical channels to MAC. 
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] staRspInfo  Status Response Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguDStaRsp
(
Pst               *post,
SpId              spId,
RguDStaRspInfo    *staRsp
)
#else
PUBLIC S16 KwLiRguDStaRsp(post, spId, staRsp)
Pst               *post;
SpId              spId;
RguDStaRspInfo    *staRsp;
#endif
{
   TRC3(KwLiRguDStaRsp)
#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)
       post->event= EVTRGUDSTARSP;
      if((kwLiRguStaRspRbuf(post, spId, staRsp)) != ROK)
      {
         RETVALUE(RFAILED);
      }
#else
     /* jump to specific primitive depending on configured selector */
     (*kwLiRguDStaRspMt[post->selector])(post, spId, staRsp);
#endif 
   RETVALUE(ROK);

} /* end of KwLiRguDStaRsp */


/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
  
/**
 *
 * @brief 
 *
 *        Handler for sending ulThrpMeasReqInfo from RLC to MAC for UL ip throughput measurement. 
 *
 * @b Description:
 *
 *        This function sends ulThrpMeasReqInfo from RLC to MAC whenver UL ip throughput
 * measurement is ON for a single or multiple qci in a UE. This is an indication for MAC
 * to start the T2/T1 time stamps for the coresponding LCHs in the UE.
 *
 *  @param[in] post                    Post structure  
 *  @param[in] spId                   Service Provider ID
 *  @param[in] ulThrpMeasReqInfo      Ul ip measurement request info
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguL2MUlThrpMeasReq
(
Pst                     *post,
SpId                    spId,
RguL2MUlThrpMeasReqInfo *l2mUlThrpMeasReq
)
#else
PUBLIC S16 KwLiRguL2MUlThrpMeasReq(post, spId, l2mUlThrpMeasReq)
Pst                     *post;
SpId                    spId;
RguL2MUlThrpMeasReqInfo *l2mUlThrpMeasReq;
#endif
{
   TRC3(KwLiRguL2MUlThrpMeasReq)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguL2MUlThrpMeasReqMt[post->selector])(post, spId, l2mUlThrpMeasReq);

   RETVALUE(ROK);

} /* end of KwLiRguL2MUlThrpMeasReq */

#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */



/****************************************************************************
 *                         Porting Functions
 ***************************************************************************/

/**
 *
 * @brief 
 *
 *        PtLiRguBndReq - portable bind request
 *
 *  @param[in] post   - Post structure  
 *  @param[in] suId  - Service user SAP ID 
 *  @param[in] spId  - Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguBndReq
(
Pst         *post,
SuId        suId,
SpId        spId
)
#else
PUBLIC S16 PtLiRguBndReq(post, suId, spId)
Pst         *post;
SuId        suId;
SpId        spId;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguBndReq)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   RLOG0(L_ERROR,"Improper selector value");
   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);
} /* end of PtLiRguBndReq() */

  
/**
 *
 * @brief 
 *
 *        PtLiRguUbndReq - portable bind request
 *
 *  @param[in] post    -  Post structure  
 *  @param[in] spId   -  Service user SAP ID 
 *  @param[in] reason -  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguUbndReq
(
Pst         *post,
SpId        spId,
Reason      reason
)
#else
PUBLIC S16 PtLiRguUbndReq(post, spId, reason)
Pst         *post;
SpId        spId;
Reason      reason;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguUbndReq)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);
} /* end of PtLiRguUbndReq() */

  
/**
 *
 * @brief 
 *
 *        PtLiRguCDatReq - portable common channel data request
 *
 *  @param[in] post    -  Post structure  
 *  @param[in] spId   -  Service user SAP ID 
 *  @param[in] datReq -  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguCDatReq
(
Pst               *post,
SpId              spId,
RguCDatReqInfo    *datReq
)
#else
PUBLIC S16 PtLiRguCDatReq(post, spId, datReq)
Pst               *post;
SpId              spId;
RguCDatReqInfo    *datReq;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguCDatReq)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(datReq);

   RETVALUE(ROK);
} /* end of PtLiRguCDatReq() */

  
/**
 *
 * @brief 
 *
 *        PtLiRguDDatReq - portable dedicated channel data request
 *
 *  @param[in] post    -  Post structure  
 *  @param[in] spId   -  Service user SAP ID 
 *  @param[in] datReq -  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguDDatReq
(
Pst               *post,
SpId              spId,
RguDDatReqInfo    *datReq
)
#else
PUBLIC S16 PtLiRguDDatReq( post, spId, datReq)
Pst               *post;
SpId              spId;
RguDDatReqInfo    *datReq;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguDDatReq)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(datReq);

   RETVALUE(ROK);
} /* end of PtLiRguDDatReq() */

  
/**
 *
 * @brief 
 *
 *        PtLiRguCStaRsp - portable common channel Status Response
 *
 *  @param[in] post    -  Post structure  
 *  @param[in] spId   -  Service user SAP ID 
 *  @param[in] staRsp -  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguCStaRsp
(
Pst               *post,
SpId              spId,
RguCStaRspInfo    *staRsp
)
#else
PUBLIC S16 PtLiRguCStaRsp(post, spId, staRsp)
Pst               *post;
SpId              spId;
RguCStaRspInfo    *staRsp;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguCStaRsp)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(staRsp);

   RETVALUE(ROK);
} /* end of PtLiRguCStaRsp() */

  
/**
 *
 * @brief 
 *
 *        PtLiRguDStaRsp - portable dedicated channel Status Response
 *
 *  @param[in] post    -  Post structure  
 *  @param[in] spId   -  Service user SAP ID 
 *  @param[in] staRsp -  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguDStaRsp
(
Pst               *post,
SpId              spId,
RguDStaRspInfo    *staRsp
)
#else
PUBLIC S16 PtLiRguDStaRsp(post, spId, staRsp)
Pst               *post;
SpId              spId;
RguDStaRspInfo    *staRsp;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguDStaRsp)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(staRsp);

   RETVALUE(ROK);
} /* end of PtLiRguDStaRsp() */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
  
/**
 *
 * @brief 
 *
 *      PtLiRguL2mUlThrpMeasReq - portable L2 Measurement Ul Thoughput Measurement Request
 *
 *  @param[in] post    -  Post structure  
 *  @param[in] spId   -  Service user SAP ID 
 *  @param[in] l2mUlThrpMeasReq -  Ul Throughput Measurement Request
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiRguL2mUlThrpMeasReq
(
Pst                     *post,
SpId                    spId,
RguL2MUlThrpMeasReqInfo *l2mUlThrpMeasReq
)
#else
PUBLIC S16 PtLiRguL2mUlThrpMeasReq(post, spId, l2mUlThrpMeasReq)
Pst                     *post,
Pst                      *post;
SpId                     spId;
RguL2MUlThrpMeasReqInfo  *l2mUlThrpMeasReq;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_DEBUG)
   KwCb   *tKwCb;
#endif

   TRC3(PtLiRguL2mUlThrpMeasReq)
#if (ERRCLASS & ERRCLS_DEBUG)
   tKwCb = KW_GET_KWCB(post->srcInst);
   RLOG0(L_ERROR,"Improper selector value");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(l2mUlThrpMeasReq);

   RETVALUE(ROK);
} /* end of PtLiRguL2mUlThrpMeasReq() */

#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

#ifdef MAC_RLC_UL_RBUF
PUBLIC S16 kwUlBatchProc ARGS ((Void));
EXTERN Void kwUtlFreeUlRBuf ARGS((void));

#ifdef ANSI
PUBLIC S16 kwUlBatchProc
(
Void
)
#else
PUBLIC S16 kwUlBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */
   RguDDatIndInfo   *datInd;
   Void *elmIndx = NULLP;
   PRIVATE Pst rlcUlRbfuPst={1,1,ENTKW,0,ENTRG,0,PRIOR0,RTESPEC,EVTRGUDDATIND,0,0,0,0};
/* Read from Ring Buffer and process PDCP packets */

#ifndef SS_RBUF
   RguDedDatInd1 *rguDatInd = NULLP;
   U8 rngBufDeqIndx = 0;

   elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   while(NULLP != elmIndx)
   {
      rguDatInd = (RguDedDatInd1 *)elmIndx;
      datInd = (RguDDatIndInfo*) rguDatInd->msg; 
      SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].nPktProc++;;//Number of pkt processed in tti
      if(datInd != NULLP)
      {
         KwLiRguDDatInd(&rlcUlRbfuPst, 0, datInd);
      }
      else
      {
         RLOG0(L_ERROR,"Received NULL buffer");
      }
      rguDatInd->msg=NULLP;
      SRngIncrRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      elmIndx = NULLP;
      rguDatInd = NULLP;

      rngBufDeqIndx++;

      //if(rngBufDeqIndx >= SS_RNG_MAX_ULMAC_TO_ULRLC_DQ_CNT)
       // break;

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC)) == NULLP)
      break;
   }
#else
   elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   while(NULLP != elmIndx)
   {
      datInd = (RguDDatIndInfo *)elmIndx;
      KwLiRguDDatInd(&rlcUlRbfuPst, 0, datInd);

      elmIndx = NULLP;
      datInd = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC)) == NULLP)
      	break;
   }
#endif
   RETVALUE(ROK);

}

/**
 *
 * @brief 
 *        Handler to clear Ring buffer from UL RLC
 *
 * @details
 *         This function clears all the ring buffer content from  UL RLC
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void kwUtlFreeUlRBuf(void)
#else
PUBLIC Void kwUtlFreeUlRBuf()  
#endif
{
   RguDDatIndInfo *datInd;
   U32            elem;
   U8             numLch;
   U8             numPdu;

   TRC2(kwUtlFreeUlRBuf)
   /* Free SS_RNG_BUF_ULMAC_TO_ULRLC  */
   while((SDeqSRngBuf (SS_RNG_BUF_ULMAC_TO_ULRLC, &elem) == ROK))
   {
      datInd = (RguDDatIndInfo *)elem; 
      for(numLch = 0; numLch< datInd->numLch; numLch++)
      {
         for(numPdu = 0; numPdu < datInd->lchData[numLch].pdu.numPdu; numPdu++)
         {
            if(datInd->lchData[numLch].pdu.mBuf[numPdu])
            {
               KW_FREE_BUF_WC(datInd->lchData[numLch].pdu.mBuf[numPdu]);
            }
         }
      }
      KW_PST_FREE(0, 0, datInd, sizeof(RguDDatIndInfo)); 
   }
}
#endif
#ifdef RLC_MAC_STA_RSP_RBUF
#ifdef ANSI
PUBLIC S16 kwLiRguStaRspRbuf
(
Pst               *post,
SpId              spId,
Void             *staRsp
)
#else
PUBLIC S16 kwLiRguStaRspRbuf(post, spId, staRsp)
Pst               *post;
SpId              spId;
Void              *staRsp;
#endif /* ANSI */
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;

   RguDStaRspInfo  *staRspInfo = NULL;
   elem = SRngGetWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
   if (NULLP != elem)
   {
      staRspInfo = (RguDStaRspInfo *)elem;
      cmMemcpy((U8 *)staRspInfo, (U8 *)staRsp, sizeof(RguDStaRspInfo)); 
      SRngIncrWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].pktRate++;
   }   
   else
   {
      RLOG0(L_ERROR,"RLC DL STA RSP RBUF is FULL!!! ");
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].pktDrop++;
      ret1 = RFAILED;
   }
   RETVALUE(ret1);
} /* cmPkKwuDatReq */

#endif
#ifdef RLC_MAC_DAT_REQ_RBUF
#ifdef ANSI
PUBLIC S16 kwLiRguDatReqRbuf
(
Pst               *post,
SpId              spId,
Void             *datReq
)
#else
PUBLIC S16 kwLiRguDatReqRbuf(post, spId, datReq)
Pst               *post;
SpId              spId;
Void             *datReq;
#endif /* ANSI */
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;
   RguInfoRingElem *datReqRing=NULLP;
   elem = SRngGetWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
   if (NULLP != elem)
   {
      datReqRing = (RguInfoRingElem *) elem;
      datReqRing->spId = spId;
      datReqRing->event = post->event;
      datReqRing->msg =datReq;
      SRngIncrWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].pktRate++;
   }   
   else
   {
      printf("RLC DL DAT REQ RBUF is FULL!!! \n");
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].pktDrop++;
      ret1 = RFAILED;
   }
   RETVALUE(ret1);
} /* cmPkKwuDatReq */

#endif 

#ifdef __cplusplus
}
#endif /* __cplusplus */
  
/********************************************************************30**
         End of file:     kw_ptli.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:25 2014
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
/main/3   kw005.201   rd   1. Added the interface fn KwLiRguL2MUlThrpMeasReq
                           2. Added the pt function PtLiRguL2mUlThrpMeasReq.
*********************************************************************91*/
