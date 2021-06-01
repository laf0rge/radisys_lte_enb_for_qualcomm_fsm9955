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

     Name:     RRM Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     rm_ptmi.c

     Sid:      qf_ptmi.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:34 2013

     Prg:     jbm 

*********************************************************************21*/

/* header include files */

#include "rm.h"
#include "rm_cmn.h"

#if (!defined(LCSMRMMILRM) || !defined(TCSMRMMILRM) || !defined(LWLCSMRMMILRM))
#define PTWRMILWR
#endif

#define RM_MAX_SEL       3      /* maximum number of selectors */

/* forward references */
#ifdef PTWRMILWR
PUBLIC S16 PtMiLrmCfgCfm    (Pst *pst, RmMngmt *cfg);
PUBLIC S16 PtMiLrmCntrlCfm   (Pst *pst, RmMngmt *cfm );
#endif

/* private variable definitions */
PRIVATE LrmCfgCfm rmMiLrmCfgCfmMt[RM_MAX_SEL] =
{
#ifdef LCSMRMMILRM
   cmPkLrmCfgCfm,
#else
   PtMiLrmCfgCfm,
#endif

#ifdef TCSMRMMILRM 
#ifdef RMU_DM
   DmMiLrmCfgCfm,
#else
   SmMiLrmCfgCfm,
#endif /*RMU_DM*/
#else
   PtMiLrmCfgCfm,
#endif 

#ifdef LWLCSMRMMILRM
   cmPkLwLcLrmCfgCfm,
#else
   PtMiLrmCfgCfm,
#endif
};


/** @brief Control Confirm primitive Matrix */
PRIVATE CONSTANT LrmCntrlCfm RmMiLrmCntrlCfmMt[RM_MAX_SEL] =
{
#ifdef LCSMRMMILRM
   cmPkLrmCntrlCfm,
#else
   PtMiLrmCntrlCfm,
#endif

#ifdef TCSMRMMILRM 
#ifdef RMU_DM
   DmMiLrmCfgCfm,
#else
   SmMiLrmCntrlCfm,
#endif /*RMU_DM*/
#else
   PtMiLrmCntrlCfm,
#endif 

#ifdef LWLCSMRMMILRM
   cmPkLwLcLrmCntrlCfm
#else
   PtMiLrmCntrlCfm
#endif

};




/*
*     layer management interface functions
*/
/*
*
*       Fun:    RmMiLrmCfgCfm
*
*       Desc:   configuration confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   rm_ptmi.c
*
*/

PUBLIC S16 RmMiLrmCfgCfm
(
Pst         *pst,
RmMngmt    *cfg
)
{
   S16 ret = ROK;

   TRC3(RmMiLrmCfgCfm)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= RM_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   ret = (*rmMiLrmCfgCfmMt[pst->selector])(pst, cfg);

   RETVALUE(ret);
}

/**
 * @brief Layer Manager Control confirm handler. 
 *
 * @details
 *
 *     Function : RmMiLrmCntrlCfm
 *     
 *     This function handles the control
 *     confirm invoked by RRM to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrmCntrlCfm() or SmMiLrmCntrlCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RmMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 RmMiLrmCntrlCfm
(
Pst      *pst,    /* post structure  */
RmMngmt  *cfm     /* control confirm structure  */
)
{
   
   TRC3(RmMiLrmCntrlCfm);

   (*RmMiLrmCntrlCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);
   
}/*-- RmMiLrmCntrlCfm --*/





/* Portable functions */
#ifdef PTWRMILWR
/*
*
*       Fun:    PtMiLrmCfgCfm
*
*       Desc:   Dummy configuration confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   rm_ptmi.c
*
*/

PUBLIC S16 PtMiLrmCfgCfm
(
Pst                     *pst,
RmMngmt                 *cfm
)
{
   TRC3(PtMiLrmCfgCfm)
   UNUSED(pst);
   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(RmMngmt));
   RETVALUE(RFAILED);
}

/**
 * @brief Portable Function definition for Layer Manager Control 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrmCntrlCfm 
 *     
 *     This function handles the control 
 *     confirm invoked by RRM to Layer Manager.
 *     Users of RRM who intend to provide a glue logic 
 *     for portability of control Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RmMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 PtMiLrmCntrlCfm
(
Pst *pst,               /* post structure */
RmMngmt *cfm            /* Control Confirm */
)
{
   TRC3(PtMiLrmCntrlCfm)
   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(RmMngmt));
   RETVALUE(ROK);
}/* end of PtMiLrmCntrlCfm */


#endif

/********************************************************************30**

           End of file:     qf_ptmi.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:34 2013

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
*********************************************************************91*/
