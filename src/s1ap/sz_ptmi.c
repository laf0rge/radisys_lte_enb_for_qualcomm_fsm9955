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
  
     Name:     S1AP - Management Interface
  
     Type:     C source file
  
     Desc:     C source code for the management interface primitives of
               S1AP
 
     File:     sq_ptmi.c
  
     Sid:      sq_ptmi.c@@/main/3 - Mon Jan 10 22:17:04 2011
  
     Prg:      nm
  
*********************************************************************21*/
  
/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.
 
The following functions are provided in this file:

     SzMiLszStaInd      Status Indication
     SzMiLszStaCfm      Status Confirm
     SzMiLszStsCfm      Statistics Confirm
     SzMiLszTrcInd      Trace Indication
     SzMiLszCfgCfm      Configure confirm 
     SzMiLszCntrlCfm    Control confirm 
 
It is assumed that the following functions are provided in the
S1AP protocol file:
  
     SzMiLszCfgReq      Configure Request
     SzMiLszStaReq      Status Request
     SzMiLszStsReq      Statistics Request
     SzMiLszCntrlReq    Control Request
 
*/
  
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000361     S1AP
*
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
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* SZT interface defines           */
/* sz003.301: Updted for PSF-S1AP 1.1 release. Added include of FT/HA related
 * header files */
#ifdef SZ_FTHA  
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent                    */
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP Layer management           */
#include "sz.h"            /* S1AP layer defines              */
#include "sz_err.h"        /* S1AP - error */
#include "cm_pasn.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

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
#include "sct.x"           /* SCT interface defines           */
/* sz003.301: Updted for PSF-S1AP 1.1 release. Added include of FT/HA related
 * header files */
#ifdef SZ_FTHA  
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent                    */
#endif /* SZ_FTHA */
/* sz006.301: Removed un-necessary include files  */
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* SZT interface structures        */
#include "lsz.x"           /* S1AP Layer management           */
#include "cm_pasn.x"
#include "sz.x"            /* S1AP layer structures           */

/* local defines */
#define MAXSZMI     2  

#ifndef LCSZMILSZ
#define PTSZMILSZ   1      /* Portable LGT Interface */
#else
#ifndef SM
#define PTSZMILSZ   1      /* Portable LGT Interface */
#endif
#endif

/* local typedefs */
  
/* local externs */
  
/* forward references */
 
#ifdef __cplusplus
extern "C" {
#endif
 
#ifdef PTSZMILSZ
PRIVATE S16 PtMiLszStaInd ARGS((Pst *pst, SzMngmt *sta));
PRIVATE S16 PtMiLszStaCfm ARGS((Pst *pst, SzMngmt *sta));
PRIVATE S16 PtMiLszStsCfm ARGS((Pst *pst, SzMngmt *sts));
PRIVATE S16 PtMiLszTrcInd ARGS((Pst *pst, SzMngmt *trc, Buffer *mBuf));
PRIVATE S16 PtMiLszCfgCfm ARGS((Pst *pst, SzMngmt *cfm));
PRIVATE S16 PtMiLszCntrlCfm ARGS((Pst *pst, SzMngmt *cfm));
#endif
/* sz003.301: Added SHT cntrl CFM for S1AP */
#ifdef SZ_FTHA
PRIVATE S16 PtMiShtCntrlCfm ARGS((Pst *pst, ShtCntrlCfmEvnt *cfmInfo));
#endif /* SZ_FTHA */ 

#ifdef __cplusplus
}
#endif


/* functions in other modules */
  
/* public variable declarations */
  
/* private variable declarations */
 
/*

   The following matrices define the mapping between the primitives 
   called by the layer management interface of S1AP and the 
   corresnonding primitives of the S1AP service user.
 
   The parameter MAXSZMI defines the maximum number of service users on
   top of S1AP. There is an array of functions per primitive invoked by 
   S1AP. Every array is MAXSZMI long (i.e. there are as many functions 
   as the number of service users).
 
   The disnatching is performed by the configurable variable: selector.
   The selector is configured during genESZl configuration.
 
   The selectors are:
 
   0 - loosely coupled - forawrd compatibility (#define LCSZMILSZ)
   1 - Tightly Coupled  (#define SM)
 
*/
 
/* Status Indication primitive */
PRIVATE LszStaInd SzMiLszStaIndMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszStaInd,          /* 0 - loosely coupled  */
#else
   PtMiLszStaInd,          /* 0 - tightly coupled, Portable */
#endif
#ifdef SM
   SmMiLszStaInd,          /* 1 - tightly coupled, layer management */
#else
   PtMiLszStaInd,          /* 1 - tightly coupled, Portable */
#endif
};

/* Status confirm primitive */
 
PRIVATE LszStaCfm SzMiLszStaCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszStaCfm,          /* 0 - loosely coupled  */
#else
   PtMiLszStaCfm,          /* 0 - tightly coupled, Portable */
#endif
#ifdef SM
   SmMiLszStaCfm,          /* 1 - tightly coupled, layer management */
#else
   PtMiLszStaCfm,          /* 1 - tightly coupled, Portable */
#endif
};

/* Statistic Confirm primitive */
 
PRIVATE LszStsCfm SzMiLszStsCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszStsCfm,          /* 0 - loosely coupled  */
#else
   PtMiLszStsCfm,          /* 0 - tightly coupled, Portable */
#endif
#ifdef SM
   SmMiLszStsCfm,          /* 1 - tightly coupled, layer management */
#else
   PtMiLszStsCfm,          /* 1 - tightly coupled, Portable */
#endif
};

/* Trace Indication primitive */
 
PRIVATE LszTrcInd SzMiLszTrcIndMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszTrcInd,          /* 0 - loosely coupled  */
#else
   PtMiLszTrcInd,          /* 0 - tightly coupled, Portable */
#endif
#ifdef SM
   SmMiLszTrcInd,          /* 1 - tightly coupled, layer management */
#else
   PtMiLszTrcInd,          /* 1 - tightly coupled, Portable */
#endif
};

/* configuration confirm primitive */
PRIVATE LszCfgCfm SzMiLszCfgCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszCfgCfm,          /* 0 - loosely coupled  */
#else
   PtMiLszCfgCfm,          /* 0 - loosely coupled, Portable */
#endif
#ifdef SM
   SmMiLszCfgCfm,          /* 1 - tightly coupled, layer management */
#else
   PtMiLszCfgCfm,          /* 1 - tightly coupled, Portable */
#endif
};

/* control confirm primitive */
PRIVATE LszCntrlCfm SzMiLszCntrlCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszCntrlCfm,          /* 0 - loosely coupled  */
#else
   PtMiLszCntrlCfm,          /* 0 - loosely coupled, Portable */
#endif
#ifdef SM
   SmMiLszCntrlCfm,          /* 1 - tightly coupled, layer management */
#else
   PtMiLszCntrlCfm,          /* 1 - tightly coupled, Portable */
#endif
};

/* sz003.301: Added SHT cntrl CFM for S1AP */
#ifdef SZ_FTHA
PUBLIC ShtCntrlCfm SzMiShtCntrlCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkMiShtCntrlCfm,     /* 0 - loosely coupled (default mechanism) */
#else
   PtMiShtCntrlCfm,       /* 0 - loosely coupled, portable */
#endif
#ifdef SH
   ShMiShtCntrlCfm,       /* 1 - tightly coupled, stack manager */
#else
   PtMiShtCntrlCfm,       /* 1 - tightly coupled, portable */
#endif
};
#endif /* SZ_FTHA */

/*
*     support functions
*/
                 
/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used to indicate the status of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszStaInd
(
Pst *pst,                 /* post structure */
SzMngmt *sta              /* unsolicited status */
)
#else
PUBLIC S16 SzMiLszStaInd(pst, sta)
Pst *pst;                 /* post structure */   
SzMngmt *sta;             /* unsolicited status */
#endif
{
   TRC3(SzMiLszStaInd);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszStaIndMt[pst->selector])(pst, sta); 

   RETVALUE(ROK);
} /* end of SzMiLszStaInd */

 
/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used to return the status of S1AP
*              to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszStaCfm
(
Pst *pst,                 /* post structure */     
SzMngmt *sta              /* solicited status */
)
#else
PUBLIC S16 SzMiLszStaCfm(pst, sta)
Pst *pst;                 /* post structure */     
SzMngmt *sta;             /* solicited status */
#endif
{
   TRC3(SzMiLszStaCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszStaCfmMt[pst->selector])(pst, sta); 
   
   RETVALUE(ROK);
} /* end of SzMiLszStaCfm */
 
/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used to return the statistics of S1AP  
*              to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszStsCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *sts              /* statistics */
)
#else
PUBLIC S16 SzMiLszStsCfm(pst, sts)
Pst *pst;                 /* post structure */    
SzMngmt *sts;             /* statistics */
#endif
{
   TRC3(SzMiLszStsCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszStsCfmMt[pst->selector])(pst, sts); 

   RETVALUE(ROK);
} /* end of SzMiLszStsCfm */
 
/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used to indicate the trace of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszTrcInd
(
Pst *pst,                 /* post structure */
SzMngmt *trc,             /* trace */
Buffer *mBuf              /* buffer */
)
#else
PUBLIC S16 SzMiLszTrcInd(pst, trc, mBuf)
Pst *pst;                 /* post structure */   
SzMngmt *trc;             /* trace */
Buffer *mBuf;             /* buffer */
#endif
{
   TRC3(SzMiLszTrcInd);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszTrcIndMt[pst->selector])(pst, trc, mBuf); 

   RETVALUE(ROK);
} /* end of SzMiLszTrcInd */

/*
*
*       Fun:   Configuration  Confirm
*
*       Desc:  This function is used to confirm the receipt of configuration
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszCfgCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else
PUBLIC S16 SzMiLszCfgCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif
{
   TRC3(SzMiLszCfgCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszCfgCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of SzMiLszCfgCfm */

/*
*
*       Fun:   Control  Confirm
*
*       Desc:  This function is used to confirm the receipt of control
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszCntrlCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else
PUBLIC S16 SzMiLszCntrlCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif
{
   TRC3(SzMiLszCntrlCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszCntrlCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of SzMiLszCntrlCfm */

/* sz003.301: Added SHT cntrl CFM for S1AP */
#ifdef SZ_FTHA

/**********************************************************
*
*       Fun:   System Agent Control Confirm
*
*       Desc:  This function sends System Agent Control Confirm
*
*       Ret:   ROK - ok
*
*       Notes: none
*
*       File:  sz_ptmi.c
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 SzMiShtCntrlCfm
(
Pst             *pst,            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo         /* system agent control confirm event */
)
#else
PUBLIC S16 SzMiShtCntrlCfm (pst, cfmInfo)
Pst             *pst;            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo;        /* system agent control confirm event */
#endif
{
   TRC3(SzMiShtCntrlCfm);

   (*SzMiShtCntrlCfmMt[pst->selector])(pst, cfmInfo);

   RETVALUE(ROK);
}/* SzMiShtCntrlCfm */
#endif /* SZ_FTHA */

/*
*     layer management interface Portable functions
*/
#ifdef PTSZMILSZ
/*
*
*       Fun:   Portable - Status Indication
*
*       Desc:  This function is used to indicate the status of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
 
#ifdef ANSI
PRIVATE S16 PtMiLszStaInd
(
Pst *pst,                 /* post structure */
SzMngmt *sta              /* unsolicited status */
)
#else
PRIVATE S16 PtMiLszStaInd(pst, sta)
Pst *pst;                 /* post structure */
SzMngmt *sta;             /* unsolicited status */
#endif
{
   TRC3(PtMiLszStaInd);
#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ120, (ErrVal) ERRZERO, "PtMiLszStaInd");
#endif
   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of PtMiLszStaInd */
 
/*
*
*       Fun:   Portable - Status Confirm
*
*       Desc:  This function is used to return the status of S1AP
*              to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLszStaCfm
(
Pst *pst,                 /* post structure */     
SzMngmt *sta              /* solicited status */
)
#else
PRIVATE S16 PtMiLszStaCfm(pst, sta)
Pst *pst;                 /* post structure */     
SzMngmt *sta;             /* solicited status */
#endif
{
   TRC3(PtMiLszStaCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ121, (ErrVal) ERRZERO, "PtMiLszStaCfm");
#endif


   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of PtMiLszStaCfm */
 
/*
*
*       Fun:   Portable - Statistics Confirm
*
*       Desc:  This function is used to return the statistics of Network 
*              Services to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLszStsCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *sts              /* statistics */
)
#else
PRIVATE S16 PtMiLszStsCfm(pst, sts)
Pst *pst;                 /* post structure */    
SzMngmt *sts;             /* statistics */
#endif
{
   TRC3(PtMiLszStsCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ122, (ErrVal) ERRZERO, "PtMiLszStsCfm");
#endif

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of PtMiLszStsCfm */
 
/*
*
*       Fun:   Portable - Trace Indication
*
*       Desc:  This function is used to indicate the trace of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
 
#ifdef ANSI
PRIVATE S16 PtMiLszTrcInd
(
Pst *pst,                 /* post structure */
SzMngmt *trc,             /* trace */
Buffer *mBuf              /* buffer */
)
#else
PRIVATE S16 PtMiLszTrcInd(pst, trc, mBuf)
Pst *pst;                 /* post structure */
SzMngmt *trc;             /* trace */
Buffer *mBuf;             /* buffer */
#endif
{
   TRC3(PtMiLszTrcInd);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ123, (ErrVal) ERRZERO, "PtMiLszTrcInd");
#endif

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of PtMiLszTrcInd */
 

/*
*
*       Fun:   Configuration  Confirm
*
*       Desc:  This function is used to confirm the receipt of configuration
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 PtMiLszCfgCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else
PUBLIC S16 PtMiLszCfgCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif
{
   TRC3(PtMiLszCfgCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ124, (ErrVal)ERRZERO, "PtMiLszCfgCfm");
#endif
   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of PtMiLszCfgCfm */


/*
*
*       Fun:   Control  Confirm
*
*       Desc:  This function is used to confirm the receipt of control
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 PtMiLszCntrlCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else
PUBLIC S16 PtMiLszCntrlCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif
{
   TRC3(PtMiLszCntrlCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ125, (ErrVal)ERRZERO, "PtMiLszCntrlCfm");
#endif

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of PtMiLszCntrlCfm */
#endif

/* sz003.301: Added SHT cntrl CFM for S1AP */
#ifdef SZ_FTHA

/**********************************************************
*
*       Fun:   PtMiShtCntrlCfm
*
*       Desc:  Portable version of ShtCntrlCfm primitive
*
*       Ret:   RFAILED
*
*       Notes: none
*
*       File:  sz_ptmi.c
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 PtMiShtCntrlCfm
(
Pst             *pst,            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo         /* system agent control confirm event */
)
#else
PRIVATE S16 PtMiShtCntrlCfm (pst, cfmInfo)
Pst             *pst;            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo;        /* system agent control confirm event */
#endif
{
   TRC3(PtMiShtCntrlCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal)ERRZERO,
         "PtMiShtCntrlCfm(): Invalid Selector");
#endif
   
   UNUSED(pst);
   UNUSED(cfmInfo);

   RETVALUE(RFAILED);
} /* PtMiShtCntrlCfm */
#endif /* SZ_FTHA */

/**********************************************************************

         End of file:     sq_ptmi.c@@/main/3 - Mon Jan 10 22:17:04 2011

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/3      ---      pkaX  1. Updated for S1AP release 3.1
/main/3   sz003.301   pka   1. Updated for PSF-S1AP 1.1 release
/main/3   sz006.301   ve    1. Removed un-necessary include files
/main/4   sz008.301  akaranth 1. Support for source RNC to target RNC.
*********************************************************************91*/
