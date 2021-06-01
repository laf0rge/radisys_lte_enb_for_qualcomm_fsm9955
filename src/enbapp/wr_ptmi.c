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

     Desc:     C code for the APIs implemented by the layer manager, viz.
               LWR confirms and indications

     File:     wr_ptmi.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#include "wr.h" 
#include "wr_lwr.h"

#if (!defined(LCWRMILWR) || !defined(SM) || !defined(LWLCWRMILWR))
#define PTWRMILWR
#endif

#define WR_LWR_MAX_SEL       3      /* maximum number of selectors */

/* forward references */
#ifdef PTWRMILWR
PRIVATE S16 PtMiLwrCfgCfm    (Pst *pst, LwrMngmt *cfg);
PRIVATE S16 PtMiLwrCntrlCfm  (Pst *pst, LwrMngmt *cntrl);
PRIVATE S16 PtMiLwrStaInd  (Pst *pst, LwrMngmt *usta);
PUBLIC S16 PtMiLwrAddNeighCfgReq(Pst *pst,LwrMngmt *cntrl);
PUBLIC S16 PtMiLwrDynCfgCfm(Pst *pst,LwrDynMngmt *cfgCfm);
PUBLIC S16 PtMiLwrAddNeighCfgReq(Pst  *pst,LwrMngmt   *cntrl);
PUBLIC S16 PtMiLwrUpDateNhgCellCfgReq(Pst *pst,LwrMngmt *cntrl);
#endif
PUBLIC S16 WrMiLwrDynCfgCfm(Pst *pst,LwrDynMngmt *dynCfm);
PUBLIC S16 WrMiLwrAddNeighCfgReq(Pst  *pst,LwrMngmt   *cfgReq );

EXTERN S16 SmMiLwrUpDateNhgCellCfgReq(Pst *pst,LwrMngmt *nghCellCfgReq);
EXTERN S16 cmPkLwrUpDateNhgCellCfgReq (Pst *pst,LwrMngmt *cfg);

/* Status Indication primitive */
PRIVATE LwrStaInd WrMiLwrStaIndMt[WR_LWR_MAX_SEL] =
{
#ifdef LCWRMILWR
   cmPkLwrStaInd,          /* 0 - loosely coupled  */
#else
   PtMiLwrStaInd,          /* 0 - loosely coupled, portable */
#endif /* LCWRMILWR */
#ifdef SM
   SmMiLwrStaInd,          /* 1 - tightly coupled, layer management */
#else
   PtMiLwrStaInd,          /* 1 - tightly coupled, portable */
#endif /* SM */
#ifdef LWLCWRMILWR
   cmPkLwrStaInd,          /* 2 - Light weight loosely coupled  */
#else
   PtMiLwrStaInd,          /* 2 - Light weight loosely, portable */
#endif /* LCWRMILWR */
};

/* private variable definitions */
PRIVATE LwrCfgCfm wrMiLwrCfgCfmMt[WR_LWR_MAX_SEL] =
{
#ifdef LCWRMILWR
   cmPkLwrCfgCfm,            /* 0 - loosely coupled  */
#else
   PtMiLwrCfgCfm,            /* 0 - loosely coupled, portable */
#endif 
#ifdef SM                                                                
   SmMiLwrCfgCfm,            /* 1 - tightly coupled, layer management */
#else                                                                    
   PtMiLwrCfgCfm,            /* 1 - tightly coupled, portable */
#endif /* SM */                                                          
#ifdef LWLCWRMILWR
   cmPkLwrCfgCfm,            /* 2 - Light weight loosely coupled  */
#else
   PtMiLwrCfgCfm,            /* 2 - Light weight loosely, portable */
#endif  
};

PRIVATE LwrCntrlCfm wrMiLwrCntrlCfmMt[WR_LWR_MAX_SEL] =
{
#ifdef LCWRMILWR
   cmPkLwrCntrlCfm,       /* 0 - loosely coupled  */
#else
   PtMiLwrCntrlCfm,       /* 0 - loosely coupled, portable */
#endif
#ifdef SM                                                             
   SmMiLwrCntrlCfm,       /* 1 - tightly coupled, layer management */
#else
   PtMiLwrCntrlCfm,       /* 1 - tightly coupled, portable */
#endif 
#ifdef LWLCWRMILWR                                                    
   cmPkLwrCntrlCfm,       /* 2 - Light weight loosely coupled  */
#else
   PtMiLwrCntrlCfm,       /* 2 - Light weight loosely, portable */
#endif
};
PRIVATE LwrUpDateNhgCellCfg wrMiLwrUpDateNhgCellCfgMt[WR_LWR_MAX_SEL] =
{
#ifdef LCWRMILWR
   cmPkLwrUpDateNhgCellCfgReq,    /* 0 - loosely coupled  */
#else
   PtMiLwrUpDateNhgCellCfgReq,    /* 0 - loosely coupled, portable */
#endif
#ifdef SM                                                           
   SmMiLwrUpDateNhgCellCfgReq,    /* 1 - tightly coupled, layer management */
#else
   PtMiLwrUpDateNhgCellCfgReq,    /* 1 - tightly coupled, portable */
#endif 
#ifdef LWLCWRMILWR                                                  
   cmPkLwrUpDateNhgCellCfgReq,    /* 2 - Light weight loosely coupled  */
#else
   PtMiLwrUpDateNhgCellCfgReq,    /* 2 - Light weight loosely, portable */
#endif
}; 

PRIVATE LwrDynCfgCfm wrMiLwrDynCfgCfmMt[WR_LWR_MAX_SEL] =
{
#ifdef LCWRMILWR
   cmPkLwrDynCfgCfm,    /* 0 - loosely coupled  */
#else
   PtMiLwrDynCfgCfm,    /* 0 - loosely coupled, portable */
#endif
#ifdef SM                                                           
   SmMiLwrDynCfgCfm,    /* 1 - tightly coupled, layer management */
#else
   PtMiLwrDynCfgCfm,    /* 1 - tightly coupled, portable */
#endif 
#ifdef LWLCWRMILWR                                                  
   cmPkLwrDynCfgCfm,    /* 2 - Light weight loosely coupled  */
#else
   PtMiLwrDynCfgCfm,    /* 2 - Light weight loosely, portable */
#endif
}; 

PRIVATE LwrCntrlReq wrMiLwrAddNeighCfgReqMt[WR_LWR_MAX_SEL] =
{
#ifdef LCWRMILWR
   cmPkLwrAddNeighCfgReq,    /* 0 - loosely coupled  */
#else
   PtMiLwrAddNeighCfgReq,    /* 0 - loosely coupled, portable */
#endif
#ifdef SM                                                                
   SmMiLwrAddNeighCfgReq,    /* 1 - tightly coupled, layer management */
#else
   PtMiLwrAddNeighCfgReq,    /* 1 - tightly coupled, portable */
#endif 
#ifdef LWLCWRMILWR                                                       
   cmPkLwrAddNeighCfgReq,    /* 2 - Light weight loosely coupled  */
#else
   PtMiLwrAddNeighCfgReq,    /* 2 - Light weight loosely, portable */
#endif
}; 
/*
*     layer management interface functions
*/
/*
*
*       Fun:    WrMiLwrCfgCfm
*
*       Desc:   configuration confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   wr_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 WrMiLwrCfgCfm
(
Pst         *pst,
LwrMngmt    *cfg
)
#else
PUBLIC S16 WrMiLwrCfgCfm(pst, cfg)
Pst         *pst;
LwrMngmt    *cfg;
#endif /* ANSI */
{
   TRC3(WrMiLwrCfgCfm)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= WR_LWR_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*wrMiLwrCfgCfmMt[pst->selector])(pst, cfg));
}

/*
*
*       Fun:    WrMiLwrCntrlCfm
*
*       Desc:   control confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   rr_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 WrMiLwrCntrlCfm
(
Pst         *pst,             /* post structure */         
LwrMngmt      *cntrl            /* management structure */   
)
#else
PUBLIC S16 WrMiLwrCntrlCfm(pst, cntrl)
Pst         *pst;             /* post structure */         
LwrMngmt     *cntrl;           /* management structure */   
#endif
{
   TRC3(WrMiLwrCntrlCfm)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= WR_LWR_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*wrMiLwrCntrlCfmMt[pst->selector])(pst, cntrl));
}
/**
*
*       Fun:   WrMiLwrUpDateNhgCellCfg 
*
*       Desc:  send neighbor cell info to sm 
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   fc_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 WrMiLwrUpDateNhgCellCfg 
(
Pst         *pst,             /* post structure */         
LwrMngmt    *nghCellCfg
)
#else
PUBLIC S16 WrMiLwrUpDateNhgCellCfg(pst, nghCellCfg)
Pst         *pst;             /* post structure */         
LwrMngmt    *nghCellCfg;
#endif
{
   TRC3(WrMiLwrUpDateNhgCellCfg)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= WR_LWR_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*wrMiLwrUpDateNhgCellCfgMt[pst->selector])(pst, nghCellCfg));
}


/**
*
*       Fun:    WrMiLwrDynCfgCfm
*
*       Desc:   control confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   rr_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 WrMiLwrDynCfgCfm
(
Pst         *pst,             /* post structure */         
LwrDynMngmt *dynCfm
)
#else
PUBLIC S16 WrMiLwrDynCfgCfm(pst, cntrl)
Pst         *pst;             /* post structure */         
LwrDynMngmt *dynCfm;
#endif
{
   TRC3(WrMiLwrDynCfgCfm)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= WR_LWR_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*wrMiLwrDynCfgCfmMt[pst->selector])(pst, dynCfm));
}

/**************************************************************************
 *
 *      Fun:  WrMiLwrStaInd  
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  wr_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 WrMiLwrStaInd
(
Pst *pst,                 /* Post structure */
LwrMngmt *usta             /* Unsolicited status */
)
#else
PUBLIC S16 WrMiLwrStaInd(pst, usta)
Pst *pst;                 /* Post structure */   
LwrMngmt *usta;            /* Unsolicited status */
#endif
{
   TRC3(WrMiLwrStaInd);

   /* jump to specific primitive depending on configured selector */
   (*WrMiLwrStaIndMt[pst->selector])(pst, usta); 

   RETVALUE(ROK);
} /* end of function WrMiLwrStaInd */


/* Portable functions */
#ifdef PTWRMILWR
/*
*
*       Fun:    PtMiLwrCfgCfm
*
*       Desc:   Dummy configuration confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   wr_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLwrCfgCfm
(
Pst                     *pst,
LwrMngmt                *cfg
)
#else
PUBLIC S16 PtMiLwrCfgCfm(pst, cfg)
Pst                     *pst;
LwrMngmt                *cfg;
#endif
{
   TRC3(PtMiLwrCfgCfm)
   UNUSED(pst);
   UNUSED(cfg);
   RETVALUE(RFAILED);
}

/*
*
*       Fun:    PtMiLwrCntrlCfm
*
*       Desc:   Dummy control confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   wr_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLwrCntrlCfm
(
Pst                     *pst,
LwrMngmt                *cntrl
)
#else
PUBLIC S16 PtMiLwrCntrlCfm(pst, cntrl)
Pst                     *pst;
LwrMngmt                *cntrl;
#endif
{
   TRC3(PtMiLwrCntrlCfm)
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
}
/*
*
*       Fun:   PtMiLwrUpDateNhgCellCfgReq 
*
*       Desc:   Dummy control confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   wr_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLwrUpDateNhgCellCfgReq 
(
Pst                     *pst,
LwrMngmt                *cntrl
)
#else
PUBLIC S16 PtMiLwrUpDateNhgCellCfgReq(pst, cntrl)
Pst                     *pst;
LwrMngmt                *cntrl;
#endif
{
   TRC3(PtMiLwrCntrlCfm)
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
}/*end of PtMiLwrUpDateNhgCellCfgReq*/




/**
*
*       Fun:    
*
*       Desc:   Dummy Dynnamic confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   wr_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLwrDynCfgCfm
(
Pst         *pst,
LwrDynMngmt *cfgCfm

)
#else
PUBLIC S16 PtMiLwrDynCfgCfm(pst, cfgCfm)
Pst         *pst,
LwrDynMngmt *cfgCfm
#endif
{
   TRC3(PtMiLwrCntrlCfm)
   UNUSED(pst);
   UNUSED(cfgCfm);
   RETVALUE(RFAILED);
}


/**************************************************************************
 *
 *      Fun:  PtMiLwrStaInd (Portable) 
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  wr_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLwrStaInd
(
Pst *pst,                 /* Post structure */
LwrMngmt *usta             /* Unsolicited status */
)
#else
PUBLIC S16 PtMiLwrStaInd(pst, usta)
Pst *pst;                 /* Post structure */   
LwrMngmt *usta;            /* Unsolicited status */
#endif
{
   TRC3(PtMiLwrStaInd);

   UNUSED(pst);
   UNUSED(usta);


   RETVALUE(ROK);
} /* end of PtMiLwrStaInd */

/*
*
*       Fun:    PtMiLwrCntrlCfm
*
*       Desc:   Dummy control confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   wr_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLwrAddNeighCfgReq
(
Pst                     *pst,
LwrMngmt                *cntrl
)
#else
PUBLIC S16 PtMiLwrAddNeighCfgReq(pst, cntrl)
Pst                     *pst;
LwrMngmt                *cntrl;
#endif
{
   TRC3(PtMiLwrAddNeighCfgReq)
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
}

#endif

/*
 * *
 * *       Fun:    WrMiLwrAddNeighCfgReq
 * *
 * *       Desc:   control confirm
 * *
 * *       Ret:    ROK     - succeeded
 * *               RFAILED - failed
 * *
 * *       Notes:  None
 * *
 * *       File:   wr_ptmi.c
 * *
 * */

#ifdef ANSI
PUBLIC S16 WrMiLwrAddNeighCfgReq
(
Pst         *pst,             /* post structure */
LwrMngmt    *cfgReq            /* management structure */
)
#else
PUBLIC S16 WrMiLwrAddNeighCfgReq(pst, cfgReq)
Pst         *pst;             /* post structure */
LwrMngmt    *cfgReq;           /* management structure */
#endif
{
   TRC3(WrMiLwrAddNeighCfgReq)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= WR_LWR_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*wrMiLwrAddNeighCfgReqMt[pst->selector])(pst, cfgReq));
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
