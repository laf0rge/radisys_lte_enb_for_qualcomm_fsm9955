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

     Name:     Layer management

     Type:     C source file

     Desc:     This file contains 
               

     File:     nlu_unpk.c

     Sid:      nlu_unpk.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 15:11:21 2014

     Prg:      subhamay 

*********************************************************************21*/

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
#include "rgr.h"
#include "nlu.h"           /* Enodeb ESON interface file */
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
#include "rgr.x"
#include "nlu.x"           /* Enodeb SON interface file*/

/**
 * @brief API for unpacking the bind request from eNB App towards SON.
 *
 * @details
 *
 *     Function: cmUnpkNluBndReq
 *
 *     This API is invoked by eNB App towards SON to bind NLU SAP.
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluBndReq (
NluBndReq         func,  /* primitive to call */
Pst               *pst,  /* post structure */
Buffer            *mBuf  /* message buffer */
)
#else
S16 cmUnpkNluBndReq (func, pst, mBuf)
NluBndReq         func;  /* primitive to call */
Pst               *pst;  /* post structure */
Buffer            *mBuf; /* message buffer */
#endif
{
    SuId            suId;
    SpId            spId;

    TRC2(cmUnpkNluBndReq);

    if (SUnpkS16(&suId, mBuf) != ROK) 
    {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
    }
    if (SUnpkS16(&spId, mBuf) != ROK) 
    {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
    }
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, suId, spId));
}

/**
 * @brief API for unpacking the bind confirm from SON towards eNB APP.
 *
 * @details
 *
 *     Function: cmUnpkNluBndCfm
 *
 *     This API is invoked by eNB App to unpack bind confirm message.
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluBndCfm (
NluBndCfm         func,  /* primitive to call */
Pst               *pst,  /* post structure */
Buffer            *mBuf  /* message buffer */
)
#else
S16 cmUnpkNluBndCfm (func, pst, mBuf)
NluBndCfm         func;  /* primitive to call */
Pst               *pst;  /* post structure */
Buffer            *mBuf; /* message buffer */
#endif
{
   SuId            suId;
   SpId            spId;
   U8              status;   

    TRC2(cmUnpkNluBndCfm);

    if (SUnpkU8(&status, mBuf) != ROK) 
    {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
    }
    if (SUnpkS16(&spId, mBuf) != ROK) 
    {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
    }
    if (SUnpkS16(&suId, mBuf) != ROK) 
    {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
    }
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, suId, spId, status));
}

/**
 * @brief API for unpacking the report new neighbor request from eNB App 
 *  towards SON
 *
 * @details
 *
 *     Function: cmUnpkNluNeighborReq
 *
 *     This API is invoked by eNB App towards SON to report any new
 *     neighbor detected in eNB
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluNeighborReq(
NluNeighborReq      func,  /* primitive to call */
Pst                 *pst,  /* post structure */
Buffer              *mBuf  /* message buffer */
)
#else
S16 cmUnpkNluNeighborReq(func, pst, mBuf)
NluNeighborReq      func;  /* primitive to call */
Pst                 *pst;  /* post structure */
Buffer              *mBuf; /* message buffer */
#endif
{
   NluNeighborReport *newNhbr = NULLP;
   SuId                  suId;

   TRC2(cmUnpkNluNeighborReq);

   if (SUnpkS16(&suId, mBuf) != ROK)
   {
     SPutMsg(mBuf);
     RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
      case NLU_SEL_LWLC:
      {
         if (cmUnpkPtr((PTR *)&newNhbr, mBuf) != ROK)
         {
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      break;

      case NLU_SEL_TC:
      case NLU_SEL_LC:
      default:
      {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, newNhbr));
}

/**
 * @brief API for unpacking the PCI mod confirm from eNB App 
 *  towards SON
 *
 * @details
 *
 *     Function: cmUnpkNluPciModIndCfm
 *
 *     This API is invoked by eNB App towards SON to report PCI mod confirm 
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluPciModIndCfm(
NluPciModCfm        func,  /* primitive to call */
Pst                 *pst,  /* post structure */
Buffer              *mBuf  /* message buffer */
)
#else
S16 cmUnpkNluPciModIndCfm(func, pst, mBuf)
NluPciModCfm        func;  /* primitive to call */
Pst                 *pst;  /* post structure */
Buffer              *mBuf; /* message buffer */
#endif
{
   NluPciModIndCfm      *pciMod = NULLP;
   SuId                  suId;

   TRC2(cmUnpkNluPciModIndCfm);

   if (SUnpkS16(&suId, mBuf) != ROK)
   {
     SPutMsg(mBuf);
     RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
      case NLU_SEL_LWLC:
      {
         if (cmUnpkPtr((PTR *)&pciMod, mBuf) != ROK)
         {
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      break;

      case NLU_SEL_TC:
      case NLU_SEL_LC:
      default:
      {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, pciMod));
}


/* Periodic REM scan for TPM */
/**
 * @brief API for unpacking the report new neighbor request from eNB App 
 *  towards SON
 *
 * @details
 *
 *     Function: cmUnpkNluPerCellSrchRsp
 *
 *     This API is invoked by eNB App towards SON to report any new
 *     neighbor detected in eNB
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluPerCellSrchRsp(
NluPerCellSrchRsp   func,  /* primitive to call */
Pst                 *pst,  /* post structure */
Buffer              *mBuf  /* message buffer */
)
#else
S16 cmUnpkNluPerCellSrchRsp(func, pst, mBuf)
NluPerCellSrchRsp   func;  /* primitive to call */
Pst                 *pst;  /* post structure */
Buffer              *mBuf; /* message buffer */
#endif
{
   NluPeriodicRemCellSearchRsp *cellSrchRsp = NULLP;
   SuId                         suId;

   TRC2(cmUnpkNluPerCellSrchRsp);

   if (SUnpkS16(&suId, mBuf) != ROK)
   {
     SPutMsg(mBuf);
     RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
      case NLU_SEL_LWLC:
      {
         if (cmUnpkPtr((PTR *)&cellSrchRsp, mBuf) != ROK)
         {
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      break;

      case NLU_SEL_TC:
      case NLU_SEL_LC:
      default:
      {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, cellSrchRsp));
}
/* Periodic REM scan for TPM End */

/**
 * @brief API for unpacking the UE Indication from eNB App 
 *  towards SON
 *
 * @details
 *
 *     This API is used to unpack UE Indication message 
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluUeInd(
NluUeInd            func,  /*!< primitive to call */
Pst                 *pst,  /*!< post structure */
Buffer              *mBuf  /*!< message buffer */
)
#else
S16 cmUnpkNluUeInd(func, pst, mBuf)
NluUeInd            func;  /*!< primitive to call */
Pst                 *pst;  /*!< post structure */
Buffer              *mBuf; /*!< message buffer */
#endif
{
   NluUeInfo      *ueInfo = NULLP;
   SuId            suId;


   if (SUnpkS16(&suId, mBuf) != ROK)
   {
     SPutMsg(mBuf);
     RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
      case NLU_SEL_LWLC:
      {
         if (cmUnpkPtr((PTR *)&ueInfo, mBuf) != ROK)
         {
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      break;

      case NLU_SEL_TC:
      case NLU_SEL_LC:
      default:
      {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, ueInfo));
}

/**
 * @brief API for unpacking the UE mearuement report Indication 
 *  from eNB App towards SON
 *
 * @details
 *
 *     This API is used to unpack UE measurement report Indication message 
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluUeMeasReportInd(
NluUeMeasRptInd     func,  /*!< primitive to call */
Pst                 *pst,  /*!< post structure */
Buffer              *mBuf  /*!< message buffer */
)
#else
S16 cmUnpkNluUeMeasReportInd(func, pst, mBuf)
NluUeMeasRptInd     func;  /*!< primitive to call */
Pst                 *pst;  /*!< post structure */
Buffer              *mBuf; /*!< message buffer */
#endif
{
   NluUeMeasRpt    *measRpt = NULLP;
   SuId            suId;


   if (SUnpkS16(&suId, mBuf) != ROK)
   {
     SPutMsg(mBuf);
     RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
      case NLU_SEL_LWLC:
      {
         if (cmUnpkPtr((PTR *)&measRpt, mBuf) != ROK)
         {
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      break;

      case NLU_SEL_TC:
      case NLU_SEL_LC:
      default:
      {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, measRpt));
}

#ifdef RGR_CQI_REPT

/**
 * @brief API for unpacking the UE CQI report Indication 
 *  from eNB App towards SON
 *
 * @details
 *
 *     This API is used to unpack UE CQI report Indication message 
 *
 *  @param[in]  func    primitive to call
 *  @param[in]  Pst     post structure
 *  @param[in]  Buffer  message buffer
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 cmUnpkNluUeCqiReportInd(
NluUeCqiRptInd      func,  /*!< primitive to call */
Pst                 *pst,  /*!< post structure */
Buffer              *mBuf  /*!< message buffer */
)
#else
S16 cmUnpkNluUeCqiReportInd(func, pst, mBuf)
NluUeCqiRptInd      func;  /*!< primitive to call */
Pst                 *pst;  /*!< post structure */
Buffer              *mBuf; /*!< message buffer */
#endif
{
   NluUeCqiRpt    *cqiRpt = NULLP;
   SuId            suId;


   if (SUnpkS16(&suId, mBuf) != ROK)
   {
     SPutMsg(mBuf);
     RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
      case NLU_SEL_LWLC:
      {
         if (cmUnpkPtr((PTR *)&cqiRpt, mBuf) != ROK)
         {
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      break;

      case NLU_SEL_TC:
      case NLU_SEL_LC:
      default:
      {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, cqiRpt));
}
#endif


/********************************************************************30**
  
         End of file:     nlu_unpk.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 15:11:21 2014
  
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
1.1          ---         1. initial release
*********************************************************************91*/

