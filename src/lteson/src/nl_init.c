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

     Name:     SON Application

     Type:     

     Desc:     This file contains 
               

     File:     nl_init.c

     Sid:      bk_init.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:14 2014

     Prg:      subhamay 

*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"        /* Common Pasn defines */
#include "lnl.h"           /* NL Interface defines */
#include "nlu.h"
#include "nl.h"            /* SON defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_pasn.x"       /* Common Hashlist definitions */
#include "cm_lte.x"       /* Common Pasn defines */

#include "lnl.x"           /* NL Interface includes */
#include "nlu.x"
#include "nl.x"            /* SON includes */

/* local defines */
PUBLIC NlMemMngr nlMemMngr;
/* local typedefs */
 
/* local externs */
 
/* forward references */


/**
 *
 *     @brief This function is the SON Client Init task and is called
 *            during SON task's registration
 *
 *     @details 
 *              
 *
 *     @param[in] entity     SON Entity
 *     @param[in] inst       SON Instance
 *     @param[in] region     Memory Region
 *     @param[in] reason     Reason
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PUBLIC S16 nlActvInit
(
Ent entity,            /* Entity */
Inst inst,             /* Instance */
Region region,         /* Region */
Reason reason          /* Reason */
)
#else /* ANSI */
PUBLIC S16 nlActvInit(entity, inst, region, reason)
Ent entity;            /* Entity */
Inst inst;             /* Instance */
Region region;         /* Region */
Reason reason;         /* Reason */
#endif /* ANSI */
{
   
   TRC3(nlActvInit);

   /* memset the full control block to null */
   cmMemset ((U8 *)&nlCb, 0, sizeof(NlCb));

   nlCb.init.procId = SFndProcId();
   nlCb.init.ent = entity;
   nlCb.init.inst = inst;
   nlCb.init.region = region;
   nlCb.init.reason = reason;
   nlCb.init.cfgDone = FALSE;
  /*TODO*/
   nlCb.init.pool = 0;
   nlCb.init.acnt = FALSE;
   nlCb.init.trc = FALSE;
   
   /* Set the default status to "No Un solicited Status Indication" */
   nlCb.init.usta = FALSE;

   /* Set the debug mask to zero */
#ifdef DEBUGP
   nlCb.init.dbgMask = 0x00000000;
#endif /* DEBUGP */
#ifdef SS_DIAG
   nlCb.init.logMask = 0x0;
#endif
   nlMemMngr.nlPool   = nlCb.init.pool;
   nlMemMngr.nlRegion = nlCb.init.region;
   /*Initiate the cell cb hash table*/
   cmHashListInit(&(nlCb.cellCbList), NL_EUTRAN_HASH_NO_BINS, 0,
          FALSE, CM_HASH_KEYTYPE_U32MOD, nlCb.init.region, nlCb.init.pool);
   nlInitSonDB();
   RETVALUE(ROK);

} /* end of function nlActvInit */







/********************************************************************30**

           End of file:     bk_init.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:51:14 2014

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

