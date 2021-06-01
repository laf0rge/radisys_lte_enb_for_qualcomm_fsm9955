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



/************************************************************************

   Name:    X2AP Layer

   Type:    C Source File
   
   Desc:    C Source code for the protable functions of X2AP lower
            interface layer

   File:     cz_ptli.c

   Sid:      tr_ptli.c@@/main/2 - Tue Aug 30 18:36:14 2011

   Prg :     aj

************************************************************************/



/*
The following functions are provided in this file:
   SctBndReq             - Bind Request
      SctEndpOpenReq        - End point open request
      SctEndpCloseReq       - End point close request
      SctSetPriReq          - Set primary address request
      SctStaReq             - Status request
      SctTermReq            - Termination request
      SctAssocReq           - Association request
      SctDatReq             - Data request
      SctAssocRsp           - Association response
      SctHBeatReq           - Heart beat request

It should be noted that not all of these functions may be required
by a particular network layer service provider.

*/

/* header include files (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_dns.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common socket library           */
#include "cm_dns.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "czt.h"           /* CZT interface defines           */
#include "lcz.h"           /* X2AP Layer management           */
#include "cz.h"            /* X2AP layer defines              */
#include "cz_err.h"            /* X2AP layer defines              */
#ifdef SB
#include "lsb.h"            /* SCTP layer managment defines */
#endif /*SB*/
#ifdef HI
#include "lhi.h"            /* TUCL layer managment defines */
#endif /*hi*/

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
#ifdef SB
#include "lsb.x"           /* SCTP layer managment structures */
#endif /*SB*/
#ifdef HI
#include "lhi.x"           /* TUCL layer managment structures */
#endif /*hi*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (!defined SB || !defined LCCZLISCT || !defined HI_LKSCTP || !defined HI || !defined DM)
/* portable functions */


PRIVATE S16 PtLiSctBndReq            ARGS((Pst           *pst,
                                          SuId           suId,
                                          SpId           spId));


#ifdef SCT_ENDP_MULTI_IPADDR
PRIVATE S16 PtLiSctEndpOpenReq ARGS (( Pst *pst,
                                    SpId spId,
                                    UConnId suEndpId,
                                    U16 port,
                                    SctNetAddrLst  *srcAddrLst));
#else                                    
PRIVATE S16 PtLiSctEndpOpenReq ARGS((Pst            *pst,
                                    SpId            spId,
                                    UConnId         suEndpId,
                                    SctPort         port,
                                    CmNetAddr      *intfNAddr));
#endif /* SCT_ENDP_MULTI_IPADDR */


PRIVATE S16 PtLiSctEndpCloseReq      ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        endpId,
                                          U8             endpIdType));



#ifdef SCT3
PRIVATE S16 PtLiSctAssocReq          ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spEndpId,
                                          UConnId        suAssocId,
                                          CmNetAddr     *priDstNAddr,
                                          SctPort        dstPort,
                                          SctStrmId      outStrms,
                                          SctNetAddrLst *dstNAddrLst,
                                          SctNetAddrLst *srcNAddrLst,
                                          SctTos        tos,
                                          Buffer        *vsInfo));
#else
PRIVATE S16 PtLiSctAssocReq          ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spEndpId,
                                          UConnId        suAssocId,
                                          CmNetAddr     *priDstNAddr,
                                          SctPort        dstPort,
                                          SctStrmId      outStrms,
                                          SctNetAddrLst *dstNAddrLst,
                                          SctNetAddrLst *srcNAddrLst,
                                          Buffer        *vsInfo));
#endif /* end of SCT3 */


#ifdef SCT3
PRIVATE S16 PtLiSctAssocRsp          ARGS((Pst               *pst,
                                          SpId               spId,
                                          UConnId            spEndpId,
                                          SctAssocIndParams *assocIndParams,
                                          SctTos             tos,
                                          SctResult          result,
                                          Buffer            *vsInfo));
#else
PRIVATE S16 PtLiSctAssocRsp          ARGS((Pst               *pst,
                                          SpId               spId,
                                          UConnId            spEndpId,
                                          SctAssocIndParams *assocIndParams,
                                          SctResult          result,
                                          Buffer            *vsInfo));
#endif /* end of SCT3 */

PRIVATE S16 PtLiSctHBeatReq   ARGS((Pst            *pst,
                                   SpId            spId,
                                   UConnId         spAssocId,
                                   CmNetAddr      *dstNAddr,
                                   U16             intervalTime,
                                   SctStatus       status));

PRIVATE S16 PtLiSctTermReq           ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        assocId,
                                          U8             assocIdType,
                                          Bool           abrtFlg));


PRIVATE S16 PtLiSctSetPriReq         ARGS((Pst           *pst,
                                           SpId          spId,
                                           UConnId       spAssocId,
                                           CmNetAddr     *dstNAddr));

PRIVATE S16 PtLiSctDatReq            ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spAssocId,
                                          CmNetAddr     *dstNAddr,
                                          SctStrmId      strmId,
                                          Bool           unorderFlg,
                                          Bool           nobundleFlg,
                                          U16            lifeTime,
                                          U32            protId,
                                          Buffer        *mBuf));


PRIVATE S16 PtLiSctStaReq            ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spAssocId,
                                          CmNetAddr     *dstNAddr,
                                          U8             staType));
#endif /* end of (!defined SB || !defined LCCZLISCT || !defined HI_LKSCTP || !defined HI || !defined DM) */
#ifdef __cplusplus
}
#endif /* __cplusplus */



/***********************************************************************
The following matrices define the mapping between the primitives called 
by the UI of SCTP layer and the corresponding primitives in SCTP

CZ_SCT_MAX_SEL defines the maximum number of valid selector values.
The selectors are
0 - Loosely coupled with SCTP (#define LCCZLISCT)
1 - Tightly coupled with SCTP (#define SB)
2 - Tightly coupled with KSCTP (#define HI)
**********************************************************************/


/* cz001.101 : CR Fixes */
/* Bind request primitive */
PRIVATE SctBndReq CzLiSctBndReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctBndReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtLiSctBndReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctBndReq,          /* 1 - tightly coupled, Service Provider SB */
#else
   PtLiSctBndReq,          /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctBndReq,          /* 2 - tightly coupled, Service Provider HI */
#else
   PtLiSctBndReq,          /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctBndReq,          /* 3 - tightly coupled, Service Provider HI */
#else
   PtLiSctBndReq,          /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctBndReqMt */


/* End point open request primitive */
PRIVATE  SctEndpOpenReq CzLiSctEndpOpenReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctEndpOpenReq,     /* 0 - loosely coupled */
#else
   PtLiSctEndpOpenReq,     /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctEndpOpenReq,     /* 1 - tightly coupled, stub layer */
#else
   PtLiSctEndpOpenReq,     /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctEndpOpenReq,     /* 2 - tightly coupled, stub layer */
#else
   PtLiSctEndpOpenReq,     /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctEndpOpenReq,     /* 3 - tightly coupled, stub layer */
#else
   PtLiSctEndpOpenReq,     /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctEndpOpenReqMt */


/* End point close request primitive */
PRIVATE  SctEndpCloseReq CzLiSctEndpCloseReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctEndpCloseReq,     /* 0 - loosely coupled */
#else
   PtLiSctEndpCloseReq,     /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctEndpCloseReq,     /* 1 - tightly coupled, stub layer */
#else
   PtLiSctEndpCloseReq,     /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctEndpCloseReq,     /* 2 - tightly coupled, stub layer */
#else
   PtLiSctEndpCloseReq,     /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctEndpCloseReq,     /* 3 - tightly coupled, stub layer */
#else
   PtLiSctEndpCloseReq,     /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctEndpCloseReqMt */

/* Set primary address request primitive */
PRIVATE  SctSetPriReq CzLiSctSetPriReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctSetPriReq,      /* 0 - loosely coupled */
#else
   PtLiSctSetPriReq,      /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctSetPriReq,      /* 1 - tightly coupled, stub layer */
#else
   PtLiSctSetPriReq,      /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctSetPriReq,      /* 2 - tightly coupled, stub layer */
#else
   PtLiSctSetPriReq,      /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctSetPriReq,      /* 3 - tightly coupled, stub layer */
#else
   PtLiSctSetPriReq,      /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctSetPriReqMt */

/* Status request primitive */
PRIVATE  SctStaReq CzLiSctStaReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctStaReq,         /* 0 - loosely coupled */
#else
   PtLiSctStaReq,         /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctStaReq,        /* 1 - tightly coupled, stub layer */
#else
   PtLiSctStaReq,        /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctStaReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSctStaReq,        /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctStaReq,        /* 3 - tightly coupled, stub layer */
#else
   PtLiSctStaReq,        /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctStaReqMt */


/* Termination request primitive */
PRIVATE SctTermReq CzLiSctTermReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctTermReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtLiSctTermReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctTermReq,          /* 1 - tightly coupled, Service Provider SB */
#else
   PtLiSctTermReq,          /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctTermReq,          /* 2 - tightly coupled, Service Provider HI */
#else
   PtLiSctTermReq,          /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctTermReq,          /* 3 - tightly coupled, Service Provider HI */
#else
   PtLiSctTermReq,          /* 3 - tightly coupled, portable */
#endif
}; /*CzLiSctTermReqMt */


/* Association request primitive */
PRIVATE SctAssocReq CzLiSctAssocReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctAssocReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtLiSctAssocReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctAssocReq,          /* 1 - tightly coupled, Service Provider SB */
#else
   PtLiSctAssocReq,          /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctAssocReq,          /* 2 - tightly coupled, Service Provider HI */
#else
   PtLiSctAssocReq,          /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctAssocReq,          /* 3 - tightly coupled, Service Provider HI */
#else
   PtLiSctAssocReq,          /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctAssocReqMt */


/* Data request primitive */
PRIVATE SctDatReq CzLiSctDatReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctDatReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtLiSctDatReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctDatReq,          /* 1 - tightly coupled, Service Provider SB */
#else
   PtLiSctDatReq,          /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctDatReq,          /* 2 - tightly coupled, Service Provider HI */
#else
   PtLiSctDatReq,          /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctDatReq,          /* 3 - tightly coupled, Service Provider HI */
#else
   PtLiSctDatReq,          /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctDatReqMt */


/* Association response primitive */
PRIVATE SctAssocRsp CzLiSctAssocRspMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
   cmPkSctAssocRsp,          /* 0 - loosely coupled (default mechanism) */
#else
   PtLiSctAssocRsp,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbUiSctAssocRsp,          /* 1 - tightly coupled, Service Provider SB */
#else
   PtLiSctAssocRsp,          /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
   HiUiSctAssocRsp,          /* 2 - tightly coupled, Service Provider HI */
#else
   PtLiSctAssocRsp,          /* 2 - tightly coupled, portable */
#endif
#ifdef DM
   DmUiSctAssocRsp,          /* 3 - tightly coupled, Service Provider HI */
#else
   PtLiSctAssocRsp,          /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctAssocRspMt */


/* Heartbeat request primitive */
PRIVATE SctHBeatReq CzLiSctHBeatReqMt[CZ_SCT_MAX_SEL] =
{
#ifdef LCCZLISCT
    cmPkSctHBeatReq,        /* 0 - loosely coupled */
#else
    PtLiSctHBeatReq,        /* 0 - tightly coupled, portable */
#endif
#ifdef SB
    SbUiSctHBeatReq,        /* 1 - tightly coupled, layer management */
#else
    PtLiSctHBeatReq,        /* 1 - tightly coupled, portable */
#endif
#if (defined(HI) && defined(HI_LKSCTP))
    HiUiSctHBeatReq,        /* 2 - tightly coupled, layer management */
#else
    PtLiSctHBeatReq,        /* 2 - tightly coupled, portable */
#endif
#ifdef DM
    DmUiSctHBeatReq,        /* 3 - tightly coupled, layer management */
#else
    PtLiSctHBeatReq,        /* 3 - tightly coupled, portable */
#endif
}; /* CzLiSctHBeatReqMt */


/************************************************************************
                   Function Definitions
************************************************************************/


/*
*
*       Fun:   Bind request
*
*       Desc:  This function is used to bind two upper SAPs.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctBndReq
(
Pst *pst,                 /* post structure */
SuId suId,                /* Su SAP Id */
SpId spId                 /* Sp SAP Id */
)
#else
PUBLIC S16 CzLiSctBndReq(pst, suId, spId)
Pst *pst;                 /* post structure */
SuId suId;                /* Su SAP Id */
SpId spId;                /* Sp SAP Id */
#endif
{
   CZ_TRC3(CzLiSctBndReq);

   CZ_DBGP ((czCb.init.prntBuf, 
           "CzLiSctBndReq(pst, suId: %d, spId: %d)\n", suId, spId));

   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctBndReqMt[pst->selector])(pst, suId, spId));
} /* end of CzLiSctBndReq */




/*
*
*       Fun:   Opening of endpoint request
*
*       Desc:  This function is used to request the opening of
*              an endpoint at the SCTP layer.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef SCT_ENDP_MULTI_IPADDR
#ifdef ANSI
PUBLIC S16 CzLiSctEndpOpenReq       
(
Pst            *pst,
SpId           spId,
UConnId        suEndpId,
SctPort        port,
SctNetAddrLst  *srcAddrLst /* Interface IP address   */
)
#else /* ANSI */
PUBLIC S16 CzLiSctEndpOpenReq  (pst, spId, suEndpId, port, srcAddrLst)
Pst            *pst;
SpId           spId;
UConnId        suEndpId;
SctPort        port;
SctNetAddrLst  *srcAddrLst;         /* Interface IP address   */
#endif /* ANSI */
#else /* SCT_ENDP_MULTI_IPADDR */
#ifdef ANSI
PUBLIC S16 CzLiSctEndpOpenReq       
(
Pst            *pst,
SpId           spId,
UConnId        suEndpId,
SctPort        port,
CmNetAddr      *intfNAddr
)
#else /* ANSI */
PUBLIC S16 CzLiSctEndpOpenReq  (pst, spId, suEndpId, port, intfNAddr)
Pst            *pst;
SpId           spId;
UConnId        suEndpId;
SctPort        port;
CmNetAddr      *intfNAddr;
#endif /* ANSI */
#endif /* SCT_ENDP_MULTI_IPADDR */
{
   CZ_TRC3(CzLiSctEndpOpenReq);
   
#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctEndpOpenReq(pst, spId: %d, suEndpId: %ld, port: %d )\n",
              spId, suEndpId, port));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctEndpOpenReq(pst, spId: %d, suEndpId: %d, port: %d )\n",
              spId, suEndpId, port));
#endif

   /* jump to specific primitive depending on configured selector */
#ifdef SCT_ENDP_MULTI_IPADDR
   CZ_RETVALUE((*CzLiSctEndpOpenReqMt[pst->selector])(pst, spId, suEndpId, port, 
                                                   srcAddrLst));
#else
   CZ_RETVALUE((*CzLiSctEndpOpenReqMt[pst->selector])(pst, spId, suEndpId, port, 
                                                   intfNAddr));
#endif
} /* end of CzLiSctEndpOpenReq */




/*
*
*       Fun:   Endpoint closing request
*
*       Desc:  This function is used to send a request for the closing of 
*              the SCTP endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctEndpCloseReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAPID */
UConnId endpId,           /* endpoint ID */
U8 endpIdType             /* endpoint ID type */
)
#else
PUBLIC S16 CzLiSctEndpCloseReq(pst, spId, endpId, endpIdType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAPID */
UConnId endpId;           /* endpoint ID */
U8 endpIdType;            /* endpoint ID type */
#endif
{
   CZ_TRC3(CzLiSctEndpCloseReq);
   
#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctEndpCloseReq(pst, spId: %d, endpId: %ld, "
              "endpIdType: %d)\n",
              spId, endpId, endpIdType));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctEndpCloseReq(pst, spId: %d, endpId: %d, "
              "endpIdType: %d)\n",
              spId, endpId, endpIdType));
#endif

   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctEndpCloseReqMt[pst->selector])(pst, spId, endpId, 
                                                    endpIdType));
} /* end of CzLiSctEndpCloseReq */



#ifdef SCT3

/*
*
*       Fun:   Association establishment request
*
*       Desc:  This function is used to send a request for the closing of 
*              the SCTP endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,          /* service provider endpoint ID */
UConnId suAssocId,         /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
SctPort dstPort,           /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
SctTos        tos,
Buffer *vsInfo              /* vendor specific info */
)
#else
PUBLIC S16 CzLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, dstPort,
outStrms, dstNAddrLst, srcNAddrLst, tos, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;          /* service provider endpoint ID */
UConnId suAssocId;         /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
SctPort dstPort;           /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
SctTos        tos;
Buffer *vsInfo;             /* vendor specific info */
#endif
#else
#ifdef ANSI
PUBLIC S16 CzLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,          /* service provider endpoint ID */
UConnId suAssocId,         /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
SctPort dstPort,           /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
Buffer *vsInfo              /* vendor specific info */
)
#else
PUBLIC S16 CzLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, dstPort,
outStrms, dstNAddrLst, srcNAddrLst, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;          /* service provider endpoint ID */
UConnId suAssocId;         /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
SctPort dstPort;           /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
Buffer *vsInfo;             /* vendor specific info */
#endif
#endif /* SCT3 */
{
   CZ_TRC3(CzLiSctAssocReq);
   
      /* ICNC : print priDstNAddr, dst(src)NAddrLst contents */

#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctAssocReq(pst, spId: %d, spEndpId: %ld, "
              "suAssocId: %ld, dstPort: %d, outStrms: %d)\n",
              spId, spEndpId, suAssocId, dstPort, outStrms));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctAssocReq(pst, spId: %d, spEndpId: %d, "
              "suAssocId: %d, dstPort: %d, outStrms: %d)\n",
              spId, spEndpId, suAssocId, dstPort, outStrms));
#endif

#ifdef SCT3
   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctAssocReqMt[pst->selector])(pst, spId, spEndpId, suAssocId,
                                                priDstNAddr, dstPort, outStrms, 
                                                dstNAddrLst, srcNAddrLst, tos,
                                                vsInfo));
#else
   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctAssocReqMt[pst->selector])(pst, spId, spEndpId, suAssocId,
                                                priDstNAddr, dstPort, outStrms, 
                                                dstNAddrLst, srcNAddrLst, 
                                                vsInfo));
#endif
} /* end of CzLiSctAssocReq */



/*
*
*       Fun:   Association Establishment response
*
*       Desc:  This function is used by the service user to respond to an 
*              association iniitialization indication by accepting the 
*              association.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef SCT3
#ifdef ANSI
PUBLIC S16 CzLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spAssocId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctTos             tos,
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PUBLIC S16 CzLiSctAssocRsp(pst, spId, spAssocId, assocIndParams, tos, result, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spAssocId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctTos             tos;
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#else
#ifdef ANSI
PUBLIC S16 CzLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spAssocId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PUBLIC S16 CzLiSctAssocRsp(pst, spId, spAssocId, assocIndParams, result, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spAssocId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#endif /* SCT3 */
{
   CZ_TRC3(CzLiSctAssocRsp);
   
   if (assocIndParams->type == SCT_ASSOC_IND_INIT)
   {
       /* ICNC : print peerAddrLst, localAddrLst & hstName contents */

       SctInitParams *ip;

       ip = &(assocIndParams->t.initParams);

#ifndef ALIGN_64BIT
       CZ_DBGP ((czCb.init.prntBuf, 
               "\nCzLiSctAssocRsp(pst, spId: %d, spAssocId: %ld, "
               "result: %d, assocIndParams->type: %d, "
               "initParams: iTag: %ld, a_rwnd: %ld, "
               "outStrms: %d, inStrms: %d)\n",
               spId, spAssocId, result, assocIndParams->type,
               ip->iTag, ip->a_rwnd, ip->outStrms, ip->inStrms));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "\nCzLiSctAssocRsp(pst, spId: %d, spAssocId: %d, "
              "result: %d, assocIndParams->type: %d, "
              "initParams: iTag: %d, a_rwnd: %d, "
              "outStrms: %d, inStrms: %d)\n",
              spId, spAssocId, result, assocIndParams->type,
              ip->iTag, ip->a_rwnd, ip->outStrms, ip->inStrms));
#endif

      }
      else if (assocIndParams->type == SCT_ASSOC_IND_COOKIE)
      {
         /* ICNC : print peerAddrLst contents */

         SctCookieParams *cp;

         cp = &(assocIndParams->t.cookieParams);

#ifndef ALIGN_64BIT
         CZ_DBGP ((czCb.init.prntBuf, 
                 "\nCzLiSctAssocRsp(pst, spId: %d, spAssocId: %ld, "
                 "result: %d, assocIndParams->type: %d, "
                 "cookieParams: spAssocId: %ld, suAssocId: %ld, "
                 "peerPort: %d)\n",
                 spId, spAssocId, result, assocIndParams->type,
                 cp->spAssocId, cp->suAssocId, cp->peerPort));
#else
         CZ_DBGP ((czCb.init.prntBuf, 
                 "\nCzLiSctAssocRsp(pst, spId: %d, spAssocId: %d, "
                 "result: %d, assocIndParams->type: %d, "
                 "cookieParams: spAssocId: %d, suAssocId: %d, "
                 "peerPort: %d)\n",
                 spId, spAssocId, result, assocIndParams->type,
                 cp->spAssocId, cp->suAssocId, cp->peerPort));
#endif
   }

#ifdef SCT3
   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctAssocRspMt[pst->selector])(pst, spId, spAssocId, 
                                                assocIndParams, tos, result, 
                                                vsInfo));
#else
   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctAssocRspMt[pst->selector])(pst, spId, spAssocId, 
                                                assocIndParams, result, 
                                                vsInfo));
#endif
} /* end of CzLiSctAssocRsp */

/*
*
*       Fun:   Enable/Disable Heartbeat request
*
*       Desc:  This function is used by the service user to enable/disable
*              heartbeat either on an association or on a destination transport
*              address in an association.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  su_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 CzLiSctHBeatReq
(
Pst                *pst,                /* Post Structure */
SpId                spId,               /* Service Provider Identifier */
UConnId             spAssocId,          /* Service provider Association ID */
CmNetAddr          *dstNAddr,           /* IP Network Address pointer */
U16                 intervalTime,       /* Heart-Beat Periodicity Timeout */
SctStatus           status              /* Status */
)
#else
PUBLIC S16 CzLiSctHBeatReq(pst, spId, spAssocId, dstNAddr, intervalTime,
   status)
Pst                *pst;                /* Post Structure */
SpId                spId;               /* Service Provider Identifier */
UConnId             spAssocId;          /* Service provider Association ID */
CmNetAddr          *dstNAddr;           /* IP Network Address pointer */
U16                 intervalTime;       /* Heart-Beat Periodicity Timeout */
SctStatus           status;             /* Status */
#endif
{
   CZ_TRC3(CzLiSctHBeatReq)
   /* jump to specific primitive depending on configured selector */
   (*CzLiSctHBeatReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr,
                                       intervalTime, status);
   CZ_RETVALUE(ROK);
} /* end of CzLiSctHBeatReq */



/*
*
*       Fun:   Association Termination Request
*
*       Desc:  This function is used to request the termination of an 
*              association which is either established or being established.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctTermReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId assocId,          /* association ID */
U8 assocIdType,           /* association ID type */
Bool abrtFlg              /* abort flag */
)
#else
PUBLIC S16 CzLiSctTermReq(pst, spId, assocId, assocIdType, abrtFlg)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId assocId;          /* association ID */
U8 assocIdType;           /* association ID type */
Bool abrtFlg;             /* abort flag */
#endif
{
   CZ_TRC3(CzLiSctTermReq);
   
#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctTermReq(pst, spId: %d, assocId: %ld, "
              "assocIdType: %d, abrtFlg: %d)\n",
              spId, assocId, assocIdType, abrtFlg));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctTermReq(pst, spId: %d, assocId: %d, "
              "assocIdType: %d, abrtFlg: %d)\n",
              spId, assocId, assocIdType, abrtFlg));
#endif

   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctTermReqMt[pst->selector])(pst, spId, assocId, assocIdType, 
                                               abrtFlg));
} /* end of CzLiSctTermReq */



/*
*
*       Fun:   Set Primary Destination Address request
*
*       Desc:  This function is used to set a particular
*              primary destination address.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctSetPriReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr       /* dest. network address */
)
#else
PUBLIC S16 CzLiSctSetPriReq(pst, spId, spAssocId, dstNAddr)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
#endif
{
   CZ_TRC3(CzLiSctSetPriReq);
   
      /* ICNC : print dstNAddr contents */

#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctSetPriReq(pst, spId: %d, spAssocId: %ld)\n",
              spId, spAssocId));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctSetPriReq(pst, spId: %d, spAssocId: %d)\n",
              spId, spAssocId));
#endif

   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctSetPriReqMt[pst->selector])(pst, spId, spAssocId,
                                                 dstNAddr));
} /* end of CzLiSctSetPriReq */




/*
*
*       Fun:   Data request
*
*       Desc:  This function is used to request the service provider to send a
*              user datagram to the destination.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctDatReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr,      /* dest. network address */
SctStrmId strmId,         /* stream ID */
Bool unorderFlg,          /* unordered delivery flag */
Bool nobundleFlg,         /* nobundleFlg */
U16 lifetime,             /* datagram lifetime */
U32 protId,               /* protocol ID */
Buffer *mBuf              /* message buffer */
)
#else
PUBLIC S16 CzLiSctDatReq(pst, spId, spAssocId, dstNAddr, strmId, unorderFlg, 
                         nobundleFlg, lifetime, protId, mBuf)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
SctStrmId strmId;         /* stream ID */
Bool unorderFlg;          /* unordered delivery flag */
Bool nobundleFlg;         /* nobundleFlg */
U16 lifetime;             /* datagram lifetime */
U32 protId;               /* protocol ID */
Buffer *mBuf;             /* message buffer */
#endif
{
   CZ_TRC3(CzLiSctDatReq);
   
      /* ICNC : print dstNAddr contents */

#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctDatReq(pst, spId: %d, spAssocId: %ld, "
              "strmId: %d, unorderFlg: %d, nobundleFlg: %d, "
              "lifetime: %d, protId: %ld)\n",
              spId, spAssocId, strmId, unorderFlg, nobundleFlg,
              lifetime, protId));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctDatReq(pst, spId: %d, spAssocId: %d, "
              "strmId: %d, unorderFlg: %d, nobundleFlg: %d, "
              "lifetime: %d, protId: %d)\n",
              spId, spAssocId, strmId, unorderFlg, nobundleFlg,
              lifetime, protId));
#endif


   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctDatReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr, 
                                              strmId, unorderFlg, nobundleFlg, 
                                              lifetime, protId, mBuf));
} /* end of CzLiSctDatReq */



/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to retrieve 
*              unsent/unacknowledged/undelivered datagrams from the service 
*              provider and to get statistical information from the service 
*              provider.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 CzLiSctStaReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr,      /* dest. network address */
U8 staType                /* status type */
)
#else
PUBLIC S16 CzLiSctStaReq(pst, spId, spAssocId, dstNAddr, staType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
U8 staType;               /* status type */
#endif
{
   CZ_TRC3(CzLiSctStaReq);

      /* ICNC : print dstNAddr contents */

#ifndef ALIGN_64BIT
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctStaReq(pst, spId: %d, spAssocId: %ld, "
              "staType: %d)\n",
              spId, spAssocId, staType));
#else
      CZ_DBGP ((czCb.init.prntBuf, 
              "CzLiSctStaReq(pst, spId: %d, spAssocId: %d, "
              "staType: %d)\n",
              spId, spAssocId, staType));
#endif

   /* jump to specific primitive depending on configured selector */
   CZ_RETVALUE((*CzLiSctStaReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr, 
                                              staType));
} /* end of CzLiSctStaReq */


#if (!defined SB || !defined LCCZLISCT || !defined HI_LKSCTP || !defined HI || !defined DM)


/*
*
*       Fun:   Portable bind Request
*
*       Desc:  This function is used to request a bind
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctBndReq
(
Pst *pst,                 /* post structure */
SuId suId,                /* Service user SAP Id */
SpId spId                /* Service provider SAP Id */
)
#else
PRIVATE S16 PtLiSctBndReq(pst, suId, spId)
Pst *pst;                 /* post structure */
SuId suId;                /* Service user SAP Id */
SpId spId;                /* Service provider SAP Id */
#endif
{
   CZ_TRC3(PtLiSctBndReq);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);
   CZ_RETVALUE(ROK);
} /* end of PtLiSctBndReq */




/*
*
*       Fun:   Portable open endpoint request
*
*       Desc:  This function is used to request a new endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef SCT_ENDP_MULTI_IPADDR
#ifdef ANSI
PRIVATE S16 PtLiSctEndpOpenReq
(
Pst *pst,                    /* post structure */
SpId spId,                   /* Service provider SAP Id */
UConnId suEndpId,          /* Service user Endpoint Id */
U16 port,                    /* SCTP Port number */
SctNetAddrLst  *srcAddrLst         /* Interface IP address   */
)
#else /* ANSI */
PRIVATE S16 PtLiSctEndpOpenReq(pst, spId, suEndpId, port, srcAddrLst)
Pst *pst;                    /* post structure */
SpId spId;                   /* Service provider SAP Id */
UConnId suEndpId;          /* Service user Endpoint Id */
U16 port;                    /* SCTP Port number */
SctNetAddrLst  *srcAddrLst;        /* Interface IP address   */
#endif /* ANSI */
#else /* SCT_ENDP_MULTI_IPADDR */
#ifdef ANSI
PRIVATE S16 PtLiSctEndpOpenReq
(
Pst *pst,                    /* post structure */
SpId spId,                   /* Service provider SAP Id */
UConnId suEndpId,          /* Service user Endpoint Id */
U16 port,                    /* SCTP Port number */
CmNetAddr      *intfNAddr
)
#else /* ANSI */
PRIVATE S16 PtLiSctEndpOpenReq(pst, spId, suEndpId, port, intfNAddr)
Pst *pst;                    /* post structure */
SpId spId;                   /* Service provider SAP Id */
UConnId suEndpId;          /* Service user Endpoint Id */
U16 port;                    /* SCTP Port number */
CmNetAddr      *intfNAddr;
#endif /* ANSI */
#endif /* SCT_ENDP_MULTI_IPADDR */
{
   CZ_TRC3(PtLiSctEndpOpenReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(suEndpId);
   UNUSED(port);
#ifdef SCT_ENDP_MULTI_IPADDR   
   UNUSED(srcAddrLst);
#else
   UNUSED(intfNAddr);
#endif
   CZ_RETVALUE(ROK);
} /* end of PtLiSctEndpOpenReq */




/*
*
*       Fun:   Portable close endpoint request
*  
*       Desc:  This function is used to close an endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctEndpCloseReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAPID */
UConnId endpId,           /* endpoint ID */
U8 endpIdType             /* endpoint ID type */
)
#else
PRIVATE S16 PtLiSctEndpCloseReq(pst, spId, endpId, endpIdType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAPID */
UConnId endpId;           /* endpoint ID */
U8 endpIdType;            /* endpoint ID type */
#endif
{
   CZ_TRC3(PtLiSctEndpCloseReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(endpId);
   UNUSED(endpIdType);
   CZ_RETVALUE(ROK);
} /* end of PtLiSctEndpCloseReq */




/*
*
*       Fun:   Portable Association request
*
*       Desc:  This function is used to request a new association
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/
#ifdef SCT3
#ifdef ANSI
PRIVATE S16 PtLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,        /* service provider endpoint ID */
UConnId suAssocId,      /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
U16 dstPort,               /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
SctTos          tos,
Buffer *vsInfo          /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, 
                            dstPort, outStrms, dstNAddrLst, srcNAddrLst, tos,vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;        /* service provider endpoint ID */
UConnId suAssocId;      /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
U16 dstPort;               /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
SctTos          tos;
Buffer *vsInfo;          /* vendor specific info */
#endif
#else /*-- SCT3 --*/
#ifdef ANSI
PRIVATE S16 PtLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,        /* service provider endpoint ID */
UConnId suAssocId,      /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
U16 dstPort,               /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
Buffer *vsInfo          /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, 
                            dstPort, outStrms, dstNAddrLst, srcNAddrLst, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;        /* service provider endpoint ID */
UConnId suAssocId;      /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
U16 dstPort;               /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
Buffer *vsInfo;          /* vendor specific info */
#endif
#endif /*-- SCT3 --*/
{
   CZ_TRC3(PtLiSctAssocReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(spEndpId);
   UNUSED(suAssocId);
   UNUSED(priDstNAddr);
   UNUSED(dstPort);
   UNUSED(outStrms);
   UNUSED(dstNAddrLst);
   UNUSED(srcNAddrLst);
   if (vsInfo != (Buffer *)NULLP)
   {
      SPutMsg(vsInfo);
   }

   CZ_RETVALUE(ROK);
} /* end of PtLiSctAssocReq */




/*
*
*       Fun:   Portable association response
*
*       Desc:  This function is used to respond to an association indication
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/
#ifdef SCT3
#ifdef ANSI
PRIVATE S16 PtLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spAssocId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctTos          tos,
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocRsp(pst, spId, spAssocId, assocIndParams, tos, result, 
                            vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spAssocId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctTos          tos;
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#else /*-- SCT3 --*/
#ifdef ANSI
PRIVATE S16 PtLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spAssocId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocRsp(pst, spId, spAssocId, assocIndParams, result, 
                            vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spAssocId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#endif /*-- SCT3 --*/
{
   CZ_TRC3(PtLiSctAssocRsp);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(spAssocId);
   UNUSED(assocIndParams);
   UNUSED(result);
   if (vsInfo != (Buffer *)NULLP)
   {
      SPutMsg(vsInfo);
   }

   CZ_RETVALUE(ROK);
} /* end of PtLiSctAssocRsp */




/*
*
*       Fun:   Portable termination request
*
*       Desc:  This function is used to terminate an association
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctTermReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId assocId,          /* association ID */
U8 assocIdType,           /* association ID type */
Bool abrtFlg              /* abort flag */
)
#else
PRIVATE S16 PtLiSctTermReq(pst, spId, assocId, assocIdType, abrtFlg)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId assocId;          /* association ID */
U8 assocIdType;           /* association ID type */
Bool abrtFlg;             /* abort flag */
#endif
{
   CZ_TRC3(PtLiSctTermReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(assocId);
   UNUSED(assocIdType);
   UNUSED(abrtFlg);
   CZ_RETVALUE(ROK);
} /* end of PtLiSctTermReq */




/*
*
*       Fun:   Portable set Primary destination address request
*
*       Desc:  This function is used to set a particular
*              primary destination address
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctSetPriReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr       /* dest. network address */
)
#else
PRIVATE S16 PtLiSctSetPriReq(pst, spId, spAssocId, dstNAddr)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
#endif
{
   CZ_TRC3(PtLiSctSetPriReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(spAssocId);
   UNUSED(dstNAddr);
   CZ_RETVALUE(ROK);
} /* end of PtLiSctSetPriReq */



/*
*
*       Fun:   Portable data Request
*
*       Desc:  This function is used to send data to a peer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctDatReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr,      /* dest. network address */
SctStrmId strmId,         /* stream ID */
Bool unorderFlg,          /* unordered delivery flag */
Bool nobundleFlg,         /* nobundleFlg */
U16 lifetime,             /* datagram lifetime */
U32 protId,               /* protocol ID */
Buffer *mBuf              /* message buffer */
)
#else
PRIVATE S16 PtLiSctDatReq(pst, spId, spAssocId, dstNAddr, strmId, unorderFlg,
   nobundleFlg, lifetime, protId, mBuf)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
SctStrmId strmId;         /* stream ID */
Bool unorderFlg;          /* unordered delivery flag */
Bool nobundleFlg;         /* nobundleFlg */
U16 lifetime;             /* datagram lifetime */
U32 protId;               /* protocol ID */
Buffer *mBuf;             /* message buffer */
#endif
{
   CZ_TRC3(PtLiSctDatReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(spAssocId);
   UNUSED(dstNAddr);
   UNUSED(strmId);
   UNUSED(unorderFlg);
   UNUSED(nobundleFlg);
   UNUSED(lifetime);
   UNUSED(protId);
   if (mBuf != (Buffer *)NULLP)
   {
      SPutMsg(mBuf);
   }

   CZ_RETVALUE(ROK);
} /* end of PtLiSctDatReq */



/*
*
*       Fun:   Portable status request
*
*       Desc:  This function is used to request status
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cz_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctStaReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr,      /* dest. network address */
U8 staType                /* status type */
)
#else
PRIVATE S16 PtLiSctStaReq(pst, spId, spAssocId, dstNAddr, staType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
U8 staType;               /* status type */
#endif
{
   CZ_TRC3(PtLiSctStaReq);


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(spAssocId);
   UNUSED(dstNAddr);
   UNUSED(staType);
   CZ_RETVALUE(ROK);
} /* end of PtLiSctStaReq */


/*
*
*       Fun:   Portable heart beat request
*
*       Desc:  Dummy heart beat request, customize if necessary
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  su_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiSctHBeatReq
(
Pst                *pst,                /* Post Structure */
SpId                spId,               /* Service Provider Identifier */
UConnId             spAssocId,          /* Service provider Association ID */
CmNetAddr          *dstNAddr,           /* IP Network Address pointer */
U16                 intervalTime,       /* Heart-Beat Periodicity Timeout */
SctStatus           status              /* Status */
)
#else
PRIVATE S16 PtLiSctHBeatReq(pst, spId, spAssocId, dstNAddr, intervalTime,
   status)
Pst                *pst;                /* Post Structure */
SpId                spId;               /* Service Provider Identifier */
UConnId             spAssocId;          /* Service provider Association ID */
CmNetAddr          *dstNAddr;           /* IP Network Address pointer */
U16                 intervalTime;       /* Heart-Beat Periodicity Timeout */
SctStatus           status;             /* Status */
#endif
{
   CZ_TRC3(PtLiSctHBeatReq);
   
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(spAssocId);
   UNUSED(dstNAddr);
   UNUSED(intervalTime);
   UNUSED(status);

   CZ_RETVALUE(ROK);
} /* end of PtLiSctHBeatReq */

#endif /* end of (!defined SB || !defined LCCZLISCT || !defined HI_LKSCTP || !defined HI !defined DM) */





/********************************************************************30**

         End of file:     tr_ptli.c@@/main/2 - Tue Aug 30 18:36:14 2011

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
/main/1      ---     aj               1. LTE-X2AP Initial Release.
           cz001.101 mm               1. CR Fixes, Memory Leak fixes.
/main/2      ---     pt               1. LTE-X2AP 3.1 release.
*********************************************************************91*/
