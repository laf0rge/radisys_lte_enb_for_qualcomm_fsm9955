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
 
     Name:     EnodeB Application
  
     Type:     C source file
 
     Desc:     Portable functions for RRM
 
     File:     rmu.c
  
     Sid:      rmu.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:54:02 2013
  
     Prg:      
  
**********************************************************************/

/* header include files (.h) */
#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "ss_err.h"        /* errors */
#include "ss_dep.h"        /* implementation-specific */
#include "cm_mblk.h"
#include "cm_tkns.h"
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
#include "ss_queue.h"
#include "cm_mem.h"

/* header/extern include files (.x) */
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "ss_dep.x"        /* implementation-specific */
#include "ss_queue.x"        /* implementation-specific */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"        /* general */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#ifdef SS_DRVR_SUPPORT
#include "ss_drvr.x"
#endif
#include "cm_llist.x"
#include "cm_mem_wl.x"  
#include "ss_gen.x"        /* general */
#include "cm_tkns.x"          /* */
#include "cm_mblk.x"
#include "cm_lte.x"
#include "cm_lte_rrm.x"
#include "rmu.h"           /* Enodeb RRM interface file */
#include "rmu.x"           /* Enodeb RRM interface file*/


/**************************************************************
   FSM RRM  LC interface handler functions
***************************************************************/

PUBLIC S16 cmPkRmuFsmStatusInd
(
Pst *pst,
SuId suId,
RmuFsmStatusInd *statusInd
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmPkRmuMmeOverloadStartInd
(
Pst *pst,
SuId suId,
RmuMmeOvldStartInd *startInfo
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmPkRmuMmeOverloadStopInd
(
Pst *pst, 
SuId suId, 
RmuMmeOvldStopInd *stopinfo
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmPkRmuUeAdmitReq
(
Pst *pst, 
SuId suId, 
RmuUeAdmitReq *admitinfo
)
{
   RETVALUE(ROK);
}
/*RRM_SP3*/
PUBLIC S16 cmPkRmuUeInactInd
(
Pst *pst, 
SuId suId, 
RmuUeInactInd   *rrmUeInactInd
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmPkRmuUeHoReq
(
Pst *pst, 
SuId suId, 
RmuUeHoReq *hoinfo
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmPkRmuUeRelReq
(
Pst *pst, 
SuId suId, 
RmuUeRelReq *relinfo
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmPkRmuUeRecfgReq
(
Pst *pst, 
SuId suId, 
RmuUeRecfgReq *reconfiginfo
)
{
   RETVALUE(ROK);
}

/* SPS changes starts */
PUBLIC S16 cmPkRmuUeSpsDisInd
(
Pst *pst, 
SuId suId, 
RmuUeSpsDisInd *ueSpsDisInd
)
{
   RETVALUE(ROK);
}
/* SPS changes ends */

PUBLIC S16 cmPkRmuBndReq
(
Pst *pst, 
SuId suId, 
SpId spId
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuCellRecfgInd
(
RmuCellRecfgIndFPtr func,
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuUeRecfgInd
(
RmuUeRecfgIndFPtr func,
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}
PUBLIC S16 cmUnpkRmuUeAdmitRsp
(
RmuUeAdmitRspFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuUeHoRsp
(
RmuUeHoRspFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuUeRelRsp
(
RmuUeRelRspFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuUeRelInd
(
RmuUeRelIndFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

/**
 * @brief Function to unpack the UE ERAB release indication message.
 * 
 * @details Function to unpack the UE ERAB release indication message 
 *          received from RRM and also calls the call back function of 
 *          FSM.
 *
 * @param[in/out] func   call back function of FSM on reception of the 
 *                       UE ERAB release indcation message. 
 * @param[in] pst        Post structure on RMU interface. 
 * @param[in] mBuf       Pointer to the Buffer holds the UE ERAB release
 *                       indication information. 
 * 
 * Returns S16
 *     ROK in case of successfully unpacks and successfully returns the 
 *         call back function of FSM ; else RFAILED.
 */
PUBLIC S16 cmUnpkRmuUeERabRelInd
(
RmuUeERabRelIndFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuUeRecfgRsp
(
RmuUeRecfgRspFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

PUBLIC S16 cmUnpkRmuBndCfm
(
RmuBndCfmFPtr func, 
Pst* pst, 
Buffer* mBuf
)
{
   RETVALUE(ROK);
}

/********************************************************************30**
  
         End of file:     rmu.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:54:02 2013
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
*********************************************************************91*/
