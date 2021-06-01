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

     Name:     Upper interface for DAM-Enbapp

     Type:     C source file

     Desc:     C source code for common packing and un-packing
               functions for upper interface.

     File:    wr_ifm_dam_utils.c

     Sid:      fc_ifm_dam_utils.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:51:47 2014

     Prg:      MM

*********************************************************************21*/


/***********************************************************************
 *
 *     This software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *
 *
 ***********************************************************************/

static const char* RLOG_MODULE_NAME="IFM";
static int RLOG_MODULE_ID=4;
static int RLOG_FILE_ID=110;
/***********************************************************************
 *        H E A D E R  I N C L U D E  F I L E S (.h)                   *
 ***********************************************************************/
#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* General */
#include "ssi.h"              /* System Services */
#include "cm_tkns.h"          /* common tokens */
#include "cm_hash.h"          /* common structs 1 */
#include "cm_mblk.h"          /* common memory */
#include "cm_llist.h"         /* cm link list */
#include "cm5.h"              /* common structs 3 */
#include "cm_inet.h"          /* common tokens  */
#include "cm_tkns.h"          /* common tokens */
#include "cm_tpt.h"           /* common transport */
#include "cm_dns.h"           /* common transport */
#include "wr_ifm_dam_utils.h" /*DAM Upper Interface */
#include "wr_dam_ifm_app.h"   /*ccpu00138576*/


/***********************************************************************
 *     H E A D E R / E X T E R N   I N C L U D E  F I L E S (.h)       *
 ***********************************************************************/
#include "gen.x"               /* General */
#include "ssi.x"               /* System Services */
#include "cm_tkns.x"           /* common tokens */
#include "cm_hash.x"           /* common structs 1 */
#include "cm_lib.x"            /* common library */
#include "cm_mblk.x"           /* common memory */
#include "cm_llist.x"          /* cm link list */
#include "cm5.x"               /* common structs 3 */
#include "cm_inet.x"           /* common transport */
#include "cm_tpt.x"            /* common transport */
#include "cm_dns.x"            /* common transport */
#include "wr_ifm_dam_utils.x"  /* DMA Upper Interface */

#define WR_DAM_FREE_BUFFER(_region,_buf) \
{                                        \
   SPutMsg(_buf);                        \
}                                        

#define cmPkDamTnlType SPkU32
#define cmPkDamEgtpTeid SPkU32

/*
  Defines for the packing and unpacking
*/

/**
* @brief This API is used to pack the function for 
* the Alarm Indication 
*
* @details
*
* Function: cmPkDamSendAlarmInd
*
* @param[in] Pst *pst
* @param[in] U16 category :For what operation the alarm is being generated
* @param[in] U16 event    :alarm is generated during this event.
* @param[in] U16 cause    :Cause for the event and alarm generation.
* @return   S16
*  /RFAILED
**/
PUBLIC S16 cmPkDamSendAlarmInd
(
Pst *pst,
U16                          category,
U16                          event,
U16                          cause
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkDamSendAlarmInd)

   if (SGetMsg(pst->region,pst->pool,&mBuf) != ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU16,category,mBuf,0,pst);
   CMCHKPKLOG(SPkU16,event,mBuf,0,pst);
   CMCHKPKLOG(SPkU16,cause,mBuf,0,pst);

   pst->event = (Event)EVTDAMALARMIND;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkDamSendAlarmInd */

/**
* @brief This API is used to unpack the 
* Alarm Indication.In turn it calls the Dam Interface 
* function to send the Alarm Indication. 
*
* @details
*
* Function: cmUnpkDamSendAlarmInd 
*
* @param[in] Pst    *pst
* @param[in] Buffer *mBuf 
* @return   S16
*  /RFAILED
**/
PUBLIC Void cmUnpkDamSendAlarmInd
(
Pst *pst,
Buffer *mBuf
)
{
   U16  cause;
   U16  event;
   U16  category;

   TRC3(cmUnpkDamSendAlarmInd)

   CMCHKUNPKLOG(SUnpkU16,&cause,mBuf,0, pst);
   CMCHKUNPKLOG(SUnpkU16,&event,mBuf,0, pst);
   CMCHKUNPKLOG(SUnpkU16,&category,mBuf,0, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);

   wrIfmDamSendAlarmInd(category,event,cause);

}/* cmUnpkDamSendAlarmInd */

/**
* @brief This API is used to pack the Dam Configuration Request 
*         and post it.
*
* @details
*
* Function: cmPkDamCfgReq
*
* @param[in] Pst      *pst
* @param[in] LwrMngmt *cfg 
* @return   S16
*  -# ROK/RFAILED
**/
PUBLIC S16 cmPkDamCfgReq
(
Pst *pst,
LwrMngmt *cfg
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkDamCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(cmPkPtr,(PTR)cfg,mBuf,0,pst);
   pst->event = (Event)EVTDAMCFGREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkDamCfgReq */

/**
* @brief This API is used to pack Dam Control Confirm
*       and also it post the message.
*
* @details
*
* Function: cmPkDamCntrlCfm
*
* @param[in] Pst      *pst
* @param[in] CmStatus *cmStatus 
* @param[in] LwrMngmt *cntrl 
* @return   S16
*  -# ROK/RFAILED
**/
PUBLIC S16 cmPkDamCntrlCfm
(
Pst *pst,
CmStatus *cmStatus,
LwrMngmt *cntrl
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkDamCntrlCfm);

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(cmPkPtr,(PTR)cntrl,mBuf,0,pst);
   CMCHKPKLOG(cmPkCmStatus,cmStatus,mBuf,0,pst);

   pst->event = (Event)EVTDAMCNTRLCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkDamCntrlCfm */



/**
* @brief This API is used to pack Dam Context Rel
*       and also posts the message.
*
* @details
*
* Function: cmPkDamSendCtxtRel 
*
* @param[in] Pst      *pst
* @param[in] U16      crnti  
* @param[in] U16      CellId 
* @param[in] U8       Event 
* @return   S16
*  -# ROK/RFAILED
**/
PUBLIC S16 cmPkDamSendCtxtRel 
(
Pst *pst,
U16 crnti,
U16 cellId,
U8  event
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkDamSendCtxtRel);

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU16,crnti,mBuf,0,pst);
   CMCHKPKLOG(SPkU16,cellId,mBuf,0,pst);
   CMCHKPKLOG(SPkU8,event,mBuf,0,pst);

   pst->event = (Event)EVTDAMCTXTRELREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/*cmPkDamSendCtxtRel*/

/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation confirm  function in DAM module. 
*
* @details
*
*     Function: cmUnPkDamSendCtxtRel
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return  Void 
**/

#ifdef ANSI
PUBLIC Void cmUnPkDamSendCtxtRel 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkDamSendCtxtRel(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U16 crnti;
   U16 cellId;
   U8  event;
 
   TRC3(cmUnPkTnlCreatCfm)
      
   CMCHKUNPKLOG(SUnpkU8,&event,mBuf,0,pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId),mBuf,0,pst);
   CMCHKUNPKLOG(SUnpkU16,(&crnti),mBuf,0,pst);

   WR_DAM_FREE_BUFFER(pst->region,mBuf);
   wrUmmSndCtxtRelReq(crnti,cellId,event);
}/* cmUnPkDamSendCtxtRel */

/**
* @brief This API is used to pack the Dam Control Request 
* post the message.
*
* @details
*
* Function: cmPkDamCntrlReq 
*
* @param[in] Pst *  pst
* @param[in] LwrMngmt *cntrl 
* @return   S16
*  -# ROK/RFAILED
**/
PUBLIC S16 cmPkDamCntrlReq
(
Pst *pst,
LwrMngmt *cntrl
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkDamCfgReq)

   if (SGetMsg(pst->region,pst->pool,&mBuf)!= ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(cmPkPtr,(PTR)cntrl,mBuf,0,pst);

   pst->event = (Event)EVTDAMCNTRLREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkDamCntrlReq */

/**
* @brief This API is used to pack the Dam Config Confirm
* and send the message.
*
* @details
*
* Function: cmPkDamCfgCfm
*
* @param[in] Pst *  pst
* @param[in] LwrMngmt *cfg
* @param[in] CmStatus *cmStatus
* @return   S16
*  -# ROK/RFAILED
**/
PUBLIC S16 cmPkDamCfgCfm
(
Pst *pst,
LwrMngmt *cfg,
CmStatus *cmStatus
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkDamCfgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(cmPkPtr,(PTR)cfg,mBuf,0,pst);
   CMCHKPKLOG(cmPkCmStatus, cmStatus,mBuf,0,pst);

   pst->event = (Event)EVTDAMCFGCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkDamCfgCfm */


/**
* @brief This API is used unpack the message and calls the 
* Dam Configuration function. 
*
* @details
*
*     Function: cmUnpkDamCfgReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

PUBLIC Void cmUnpkDamCfgReq
(
Pst *pst,
Buffer *mBuf
)
{
   LwrMngmt *cfg;

   TRC3(cmUnpkDamCfgReq)

   CMCHKUNPKLOG(cmUnpkPtr,(PTR *)&cfg, mBuf, 0, pst);

   WR_DAM_FREE_BUFFER(pst->region,mBuf);

   wrIfmDamWrCfgReq(cfg);

}/* cmUnpkDamCfgReq */

/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* Dam Control Request function in DAM module. 
*
* @details
*
*     Function: cmUnpkDamCntrlReq 
*
*
*  @param[in]  Pst *pst 
*  @param[in]  buf *mBuf
*  @return   Void 
**/

PUBLIC Void cmUnpkDamCntrlReq
(
Pst *pst,
Buffer *mBuf
)
{
   LwrMngmt *cntrl;

   TRC3(cmUnPkDamCntrlReq)

   CMCHKUNPKLOG(cmUnpkPtr,(PTR *)&cntrl, mBuf, 0, pst);

   WR_DAM_FREE_BUFFER(pst->region,mBuf);

   wrIfmDamWrCntrlReq(cntrl);

}/* cmUnpkDamCntrlReq */

/**
* @brief This API is used unpack the function and call the 
*  config confirm function.
*
*  @details
*
*  Function: cmUnpkDamCfgCfm 
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

PUBLIC Void cmUnpkDamCfgCfm
(
Pst *pst,
Buffer *mBuf
)
{
   LwrMngmt *cfg;
   CmStatus status;

   TRC3(cmUnpkDamCfgReq)

   CMCHKUNPKLOG(cmUnpkCmStatus,&status, mBuf, 0, pst);
   CMCHKUNPKLOG(cmUnpkPtr,(PTR *)&cfg, mBuf, 0, pst);

   WR_DAM_FREE_BUFFER(pst->region,mBuf);

   wrIfmDamCfgCfm(cfg,&status);

} /* cmUnpkDamCfgCfm */

/**
* @brief This API is used unpacks the message  and calls 
*        control confirm function in DAM module. 
*
* @details
*
* Function:cmUnpkDamCntrlCfm 
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

PUBLIC Void cmUnpkDamCntrlCfm
(
Pst *pst,
Buffer *mBuf
)
{
   LwrMngmt *cntrl;
   CmStatus  status;

   TRC3(cmUnPkDamCntrlReq)

   CMCHKUNPKLOG(cmUnpkCmStatus,(&status), mBuf, 0, pst);
   CMCHKUNPKLOG(cmUnpkPtr,((PTR *)&cntrl), mBuf, 0, pst);

   WR_DAM_FREE_BUFFER(pst->region,mBuf);

   wrIfmDamCntrlCfm(cntrl,&status);

}/* cmUnpkDamCntrlCfm */



/**
* @brief This API is used to send a 
* Print Throughput  Request from EnbApp to DAM.
*
* @details
*
* Function: cmPkPrnThptReq
*
* @param[in]   Pst *  pst
* @param[in]   
* @return   Void
*  -# ROK /RFAILED
**/
#ifdef ANSI
PUBLIC S16 cmPkPrnThptReq
(
Pst *pst
)
#else
PUBLIC S16 cmPkPrnThptReq(pst)
Pst * pst;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPrnThptReq)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {

      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   pst->event = (Event)EVTDAMPRNTTHPTREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkPrnThptReq */

/**
* @brief This API is used to unpack the message and 
* call the throughput printing function in DAM module. 
*
* @details
*
*     Function: cmUnPkPrnThptReq
*
*
*  @param[in]   Pst *pst 
*  @param[in]   Buffer *mbuf
*  @return  Void 
**/

#ifdef ANSI
PUBLIC Void cmUnpkPrnThptReq
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnpkPrnThptReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkPrnThptReq)

   WR_DAM_FREE_BUFFER(pst->region,mBuf);

   wrDamPrintThroughput();

}/* cmUnpkPrnThptReq */


/**
* @brief This API is used to send a 
* Tunnel Create  Request from EnbApp to DAM.
*
* @details
*
* Function: cmPkTnlCreatReq
* 
* @param[in]  Pst *  pst
* @param[in]  U32 transId 
* @param[in]  WrDamTnlInfo *tnlInfo
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkTnlCreatReq
(
Pst *pst,
U32 transId,
WrDamTnlInfo *tnlInfo
)
#else
PUBLIC S16 cmPkTnlCreatReq(pst,transId,*tnlInfo)
Pst * pst;
U32 transId;
WrDamTnlInfo *tnlInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkTnlCreatReq)
  
   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   
   CMCHKPKLOG(SPkU32,(transId),mBuf,0,pst);

   /* LWLC Packing */
   if (pst->selector == DAM_SEL_LWLC) {
      CMCHKPKLOG(cmPkPtr,((PTR)tnlInfo),mBuf,0,pst);
   }
   else
   {
      RLOG0(L_ERROR, "Incorrect Coupling Scheme selected");

     WR_DAM_FREE_BUFFER (pst->region,mBuf);

     RETVALUE(RFAILED);
   }
 
   pst->event = (Event)EVTDAMTNLCRETREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkTnlCreatReq */

/**
* @brief This API is used to send a 
* Tunnel Create  Request from EnbApp to DAM.
*
* @details
*
* Function: cmPkTnlModReq
* 
* @param[in]  Pst *  pst
* @param[in]  U32 transId 
* @param[in]  WrDamTnlModInfo *tnlInfo
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkTnlModReq
(
Pst *pst,
U32 transId,
WrDamTnlModInfo *tnlModInfo
)
#else
PUBLIC S16 cmPkTnlModReq(pst,transId,*tnlModInfo)
Pst * pst;
U32 transId;
WrDamTnlModInfo *tnlModInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkTnlModReq)
  
   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   
   CMCHKPKLOG(SPkU32,(transId), mBuf,0, pst);
   CMCHKPKLOG(cmPkPtr,((PTR)tnlModInfo),mBuf,0,pst);
 
   pst->event = (Event)EVTDAMTNLMODREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkTnlCreatReq */



/**
* @brief This API is used to send a 
* tunnel create confirm  from EnbApp to DAM.
*
* @details
*
* Function:cmPkTnlModCfm
* 
* @param[in] Pst *  pst
* @param[in] U32 transId 
* @param[in] U8 status
* @param[in] U32 lclTeid
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkTnlModCfm 
(
Pst *pst,
U32 transId,
U8 status,
U32 lclTeid
)
#else
PUBLIC S16 cmPkTnlModCfm(pst,transId,status,lclTeid)
Pst * pst;
U32 transId;
U8 status;
U32 lclTeid;
#endif
{
   Buffer *mBuf = NULLP
   TRC3(cmPkTnlModCfm)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU32,(transId),mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU8,(status),mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU32,(lclTeid),mBuf,EDAM001, pst);
  
   pst->event = (Event)EVTDAMTNLMODCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/* End of cmPkTnlCreatCfm*/



/**
* @brief This API is used to send a 
* tunnel create confirm  from EnbApp to DAM.
*
* @details
*
* Function:cmPkTnlCreatCfm
* 
* @param[in] Pst *  pst
* @param[in] U32 transId 
* @param[in] U8 status
* @param[in] U32 lclTeid
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkTnlCreatCfm 
(
Pst *pst,
U32 transId,
U8 status,
U32 lclTeid
)
#else
PUBLIC S16 cmPkTnlCreatCfm(pst,transId,status,lclTeid)
Pst * pst;
U32 transId;
U8 status;
U32 lclTeid;
#endif
{
   Buffer *mBuf = NULLP
   TRC3(cmPkTnlCreatCfm)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU8,(status ), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU32,(lclTeid), mBuf,EDAM001, pst);
  
   pst->event = (Event)EVTDAMTNLCRETCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/* End of cmPkTnlCreatCfm*/


/**
* @brief This API is used to send a 
* Tunnel Delete Req from Enbapp to DAM.
*
* @details
*
* Function:cmPkTnlDelReq
* 
* @param[in]   Pst *  pst
* @param[in]   U32 transID 
* @param[in]   WrDamTnlId *tnlId
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkTnlDelReq 
(
Pst *pst,
U32 transId,
WrDamTnlId *tnlId
)
#else
PUBLIC S16 cmPkTnlDelReq(pst,transId,*tnlId)
Pst * pst;
U32 transId;
WrDamTnlId *tnlId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkTnlDelReq)

   /* Buffer Allocation */ 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   
   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   /* LWLC Packing*/
   if (pst->selector == DAM_SEL_LWLC) {
     CMCHKPKLOG(cmPkPtr,((PTR)tnlId), mBuf, EEGT014, pst);
   }
   else
   {
      RLOG0(L_ERROR, "Incorrect Coupling Scheme selected");
     WR_DAM_FREE_BUFFER (pst->region,mBuf);
     RETVALUE(RFAILED);
   }
   
   pst->event = (Event)EVTDAMTNLDELTREQ;
   RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkTnlDelReq */ 

/**
* @brief This API is used to send a 
* Print Throughput  Request from EnbApp to DAM.
*
* @details
*
* Function:cmPkTnlStChngReq 
* 
* @param[in]  Pst *  pst
* @param[in]  WrDamTnlStInfo *tnlStInfo 
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkTnlStChngReq 
(
Pst *pst,
WrDamTnlStInfo *tnlStInfo
)
#else
PUBLIC S16 cmPkTnlStChngReq(pst,*tnlStInfo)
Pst * pst;
WrDamTnlStInfo *tnlStInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkTnlStChngReq)

   /* Buffer Allocaton */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   if (pst->selector == DAM_SEL_LWLC) {
      CMCHKPKLOG(cmPkPtr,((PTR)tnlStInfo),mBuf,EEGT014,pst);
   }
   else
   {
      RLOG0(L_ERROR, "Incorrect Coupling Scheme selected");
     WR_DAM_FREE_BUFFER (pst->region,mBuf);
      RETVALUE(RFAILED);
   }
 
   pst->event = (Event)EVTDAMTNLSTCHNGREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkTnlStChngReq */

/**
* @brief This API is used to send a 
* Ue Delete request from Enbapp to DAM.
*
* @details
*
* Function:cmPkUeDelReq 
* 
* @param[in]   Pst *  pst
* @param[in]   U32 transID 
* @param[in]   U16 cellId
* @param[in]   U16 crnti
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkUeDelReq 
(
Pst *pst,
U32 transId,
U16 cellId,
U16 crnti
)
#else
PUBLIC S16 cmPkUeDelReq(pst,transId,cellId,crnti)
Pst * pst;
U32 transId;
U16 cellId;
U16 crnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUeDelReq)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   CMCHKPKLOG(SPkU32,transId, mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,cellId, mBuf,EDAM002, pst);
   CMCHKPKLOG(SPkU16,crnti, mBuf,EDAM003, pst);

   
   pst->event = (Event)EVTDAMUEDELTREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkUeDelReq */

/**
* @brief This API is used to initate a 
* Ue Delete Confirm  from DAM to EnbApp. 
*
* @details
*
* Function:cmPkUeDelCfm 
* 
* @param[in]   Pst *  pst
* @param[in]   U32 transID 
* @param[in]   U16 cellId
* @param[in]   U16 crnti
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkUeDelCfm 
(
Pst *pst,
U32 transId,
U16 cellId,
U16 crnti
)
#else
PUBLIC S16 cmPkUeDelCfm(pst,transId,cellId,crnti)
Pst * pst;
U32 transId;
U16 cellId;
U16 crnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUeDelCfm)

   /* Buffer Allocation */ 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   
   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(cellId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(crnti), mBuf,EDAM001, pst);
   
   pst->event = (Event)EVTDAMUEDELTCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkUeDelCfm */ 


/**
* @brief This API is used to send a 
* Cell Add Request from Enbapp to DAM.
*
* @details
*
* Function:cmPkCellAddReq 
* 
* @param[in]   Pst *  pst
* @param[in]   U32 transID 
* @param[in]   WrDamCellInfo *cellInfo 
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkCellAddReq
(
Pst *pst,
U32 transId,
WrDamCellInfo *cellInfo
)
#else
PUBLIC S16 cmPkCellAddReq(pst,transId,*cellInfo)
Pst * pst;
U32 transId;
WrDamCellInfo *cellInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkCellAddReq)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Loose Coupling */
   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   /* LWLC */
   if (pst->selector == DAM_SEL_LWLC) {
      CMCHKPKLOG(cmPkPtr,((PTR)cellInfo), mBuf, EEGT014, pst);
   }
   else
   {
      RLOG0(L_ERROR, "Incorrect Coupling Scheme selected");
      RETVALUE(RFAILED);
   }

   pst->event = (Event)EVTDAMCELLADDREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkCellAddReq */

/**
* @brief This API is used to send a 
* Ue Re-establishment request from Enbapp to DAM.
*
* @details
*
* Function: cmPkReestabReq
* 
* @param[in]   Pst *  pst
* @param[in]   U32 transID 
* @param[in]   U16 cellId 
* @param[in]   U16 oldcrnti
* @param[in]   U16 newcrnti
* @return   S16
*  -# ROK/RFAILED
**/
#ifdef ANSI
PUBLIC S16 cmPkReestabReq 
(
Pst *pst,
U32 transId,
U16 cellId,
U16 oldcrnti,
U16  newcrnti
)
#else
PUBLIC S16 cmPkReestabReq(pst,transId,cellId,oldcrnti,newcrnti)
Pst * pst;
U32 transId;
U16 cellId;
U16 oldcrnti;
U16  newcrnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkReestabReq)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(cellId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(oldcrnti), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(newcrnti), mBuf,EDAM001, pst);

   pst->event = (Event)EVTDAMRESTREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkReestabReq */

/**
* @brief This API is used to send a 
* Re-establishment Confirm from DAM to EnbApp.
*
* @details
*
* Function:cmPkReestabCfm
* 
* @param[in] Pst *  pst
* @param[in] U32 transId 
* @param[in] U8 status
* @param[in] U32 lclTeid
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkReestabCfm 
(
Pst *pst,
U32 transId,
U8 status
)
#else
PUBLIC S16 cmPkReestabCfm(pst,transId,status)
Pst * pst;
U32 transId;
U8 status;
#endif
{
   Buffer *mBuf = NULLP
   TRC3(cmPkReestabCfm)
   /* Buffer Allocation */

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU8,(status ), mBuf,EDAM001, pst);

   pst->event = (Event)EVTDAMREESTABCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/*cmPkReestabCfm */


/**
* @brief This API is used to send a 
* Ue Delete request from Enbapp to DAM.
*
* @details
*
* Function:cmPkReCfgReq 
* 
* @param[in]   Pst *  pst
* @param[in]   U32 transID 
* @param[in]   U16 cellId 
* @param[in]   U16 crnti
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkReCfgReq 
(
Pst *pst,
U32 transId,
U16 cellId,
U16 crnti
)
#else
PUBLIC S16 cmPkReCfgReq(pst,transId,cellId,crnti)
Pst * pst;
U32 transId;
U16 cellId;
U16 crnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkReCfgReq)

   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU32,(transId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(cellId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(crnti), mBuf,EDAM001, pst);
   
   pst->event = (Event)EVTDAMRECFGREQ;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkReCfgReq */

/**
* @brief This API is used to send a 
* Ue Delete request from Enbapp to DAM.
*
* @details
*
* Function :  cmPkHdlHoFailure
* 
* @param[in]   Pst *  pst
* @param[in]   U16 cellId 
* @param[in]   U16 ueId 
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkHdlHoFailure 
(
Pst *pst,
U16 cellId,
U16 crnti 
)
#else
PUBLIC S16 cmPkHdlHoFailure(pst,cellId,crnti)
Pst * pst;
U16 cellId;
U16 crnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkHdlHoFailure)
   /*  Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, " Memory allocation failed. ");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU16,(cellId), mBuf,EDAM001, pst);
   /* HO_FIX: Changed SPkU32 to SPkU16 as we are unpacking
              U16 only */
   CMCHKPKLOG(SPkU16,(crnti), mBuf,EDAM001, pst);
   pst->event = (Event)EVTDAMHDLHOFAIL;
   RETVALUE(SPstTsk(pst, mBuf));

}





/**
* @brief This API is used to send a 
* Ue Delete request from Enbapp to DAM.
*
* @details
*
* Function:cmPkStartInactivityTimer
* 
* @param[in]   Pst *  pst
* @param[in]   U16 cellId 
* @param[in]   U16 crnti 
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkStartInactivityTimer 
(
Pst *pst,
U16 cellId,
U16 crnti
)
#else
PUBLIC S16 cmPkStartInactivityTimer(pst,cellId,crnti)
Pst * pst;
U16 cellId;
U16 crnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkReCfgReq)
   /*  Buffer Allocation */

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU16,(cellId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU16,(crnti), mBuf,EDAM001, pst);
   
   pst->event = (Event)EVTDAMINACTIVITYTMR;
   RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkStartInactivityTimer */

/**
* @brief This API is used to send a 
* Ue Delete request from Enbapp to DAM.
*
* @details
*
* Function:cmPkStopUeTmr
* 
* @param[in]   Pst *  pst
* @param[in]   U16 cellId 
* @param[in]   U16 ueId 
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkStopUeTmr 
(
Pst *pst,
U16 cellId,
U16 ueId 
)
#else
PUBLIC S16 cmPkStopUeTmr(pst,cellId,ueId)
Pst * pst;
U16 cellId;
U16 ueId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkReCfgReq)
   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU16,(cellId), mBuf,EDAM001, pst);
   CMCHKPKLOG(SPkU32,(ueId), mBuf,EDAM001, pst);
   
   pst->event = (Event)EVTDAMUESTOPTMR;
   RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkStopUeTmr */

/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation function in DAM module. 
*
* @details
*
*     Function: cmUnPkTnlCreatReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkTnlCreatReq
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkTnlCreatReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
    U32 transId = 0;
    WrDamTnlInfo *tnlInfo = NULLP;

    TRC3(cmUnPkTnlCreatReq)

    CMCHKUNPKLOG(cmUnpkPtr,((PTR *)&tnlInfo), mBuf, EEGT014, pst);
    CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);

    WR_DAM_FREE_BUFFER (pst->region,mBuf);

    wrIfmDamWrTnlCreatReq(transId,tnlInfo);

}/* cmUnPkTnlCreatReq */


/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation confirm  function in DAM module. 
*
* @details
*
*     Function: cmUnPkTnlCreatCfm
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkTnlCreatCfm
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkTnlCreatCfm(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U32 transId = 0;
   U8 status   = 0;
   U32 lclTeid = 0;
 
   TRC3(cmUnPkTnlCreatCfm)
      
   CMCHKUNPKLOG(SUnpkU32,(&lclTeid), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU8,&status, mBuf,EDAM002, pst);
   CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);
  
   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrUmmTnlCreatCfm(transId, status, lclTeid);

}/* cmUnPkTnlCreatCfm */

/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation function in DAM module. 
*
* @details
*
*     Function: cmUnPkTnlCreatReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnpkTnlModReq
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnpkTnlModReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
    U32 transId = 0;
    WrDamTnlModInfo *tnlModInfo = NULLP;

    TRC3(cmUnPkTnlCreatReq)


    CMCHKUNPKLOG(cmUnpkPtr,((PTR *)&tnlModInfo), mBuf, 0, pst);
    CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);
 
    WR_DAM_FREE_BUFFER (pst->region,mBuf);
    
    wrIfmDamWrTnlModReq(transId,tnlModInfo);
}/* cmUnPkTnlCreatReq */


/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation confirm  function in DAM module. 
*
* @details
*
*     Function: cmUnPkTnlCreatCfm
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnpkTnlModCfm
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnpkTnlModCfm(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U32 transId = 0;
   U8 status   = 0;
   U32 lclTeid = 0;
 
   TRC3(cmUnPkTnlCreatCfm)
      
   CMCHKUNPKLOG(SUnpkU32,(&lclTeid), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU8,&status, mBuf,EDAM002, pst);
   CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);
  
   WR_DAM_FREE_BUFFER (pst->region,mBuf);
  
   wrIfmDamTnlModCfm(transId, status, lclTeid);
}/* cmUnpkTnlModCfm */


/**
* @brief This API is used to call the function directly
* in DAM interface file.The function in turn will call the
* tunnel deletion fucntion in DAM module. 
*
* @details
*
*     Function:cmUnPkTnlDelReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkTnlDelReq 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkTnlDelReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
    U32 transId = 0;
    WrDamTnlId *tnlId = NULLP;

    TRC3(cmUnPkTnlDelReq)

    if (pst->selector == DAM_SEL_LWLC) 
    {
       CMCHKUNPKLOG(cmUnpkPtr,((PTR *)&tnlId), mBuf, EEGT014, pst);
    }
    else
    {
      RLOG0(L_ERROR, "unpacking failed due to wrong coupling selector");
      WR_DAM_FREE_BUFFER (pst->region,mBuf);
    }

    CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);
  
    WR_DAM_FREE_BUFFER (pst->region,mBuf);
    if(NULLP == tnlId)
    {
      RLOG0(L_ERROR, "cmUnPkTnlDelReq(): WrDamTnlId is NULL.");
      RETVOID;
    } 
    wrIfmDamWrTnlDelReq(transId, tnlId);

} /* cmUnPkTnlDelReq */

/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function: cmUnPkUeDelReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkUeDelReq 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkUeDelReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
    U32 transId = 0;
    U16 cellId  = 0;
    U16 crnti = 0;
   
    TRC3(cmUnPkUeDelReq)
 
   CMCHKUNPKLOG(SUnpkU16,(&crnti),mBuf,EDAM006, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId),mBuf,EDAM007, pst);
   CMCHKUNPKLOG(SUnpkU32,(&transId),mBuf,EDAM008, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrIfmDamWrUeDelReq(transId,cellId,crnti);
}/* cmUnPkUeDelReq */

/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for sending the confirmation
* for deleting the UE in wr_dam.c.
*
* @details
*
*     Function: cmUnPkUeDelCfm
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkUeDelCfm 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkUeDelCfm(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
    U32 transId = 0;
    U16 cellId = 0;
    U16 crnti = 0;
 
    TRC3(cmUnPkUeDelCfm)

    CMCHKUNPKLOG(SUnpkU16,(&crnti), mBuf,EDAM001, pst);
    CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,EDAM001, pst);
    CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);
 
    WR_DAM_FREE_BUFFER (pst->region,mBuf);
 
    wrUmmPrcDamUeDelCfm(transId, cellId, crnti);

}/* cmUnPkUeDelCfm */


/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function:cmUnPkCellAddReq 
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkCellAddReq 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkCellAddReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U32 transId = 0;
   WrDamCellInfo *cellInfo = NULLP;
 
   TRC3(cmUnPkCellAddReq)
   
   if (pst->selector == DAM_SEL_LWLC) {
      CMCHKUNPKLOG(cmUnpkPtr,((PTR *)&cellInfo), mBuf, EEGT014, pst);
   }
   else
   {
      RLOG0(L_ERROR, "unpacking failed due to wrong coupling selector");
       WR_DAM_FREE_BUFFER (pst->region,mBuf);
    }
    CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);

    WR_DAM_FREE_BUFFER (pst->region,mBuf);
    if(NULLP == cellInfo)
    {
      RLOG0(L_ERROR, " cmUnPkTnlDelReq(): cellInfo is NULL.");
      RETVOID;
    } 
  
    wrIfmDamWrCellAddReq(transId, cellInfo);

}/* cmUnPkCellAddReq  */


/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function:cmPkReestabReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkReestabReq 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkReestabReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U32 transId  = 0;
   U16 cellId   = 0;
   U16 oldcrnti = 0;
   U16 newcrnti = 0;

   TRC3(cmUnPkReestabReq)

   CMCHKUNPKLOG(SUnpkU16,(&newcrnti), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU16,(&oldcrnti), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);

   wrIfmDamWrReestabReq(transId, cellId, oldcrnti, newcrnti);

} /* cmUnPkReestabReq */


/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function:cmUnPkHdlHoFailure
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkHdlHoFailure
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkHdlHoFailure(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U16 cellId   = 0;
   U16 crnti = 0;

   TRC3(cmUnPkHdlHoFailure)
   

   CMCHKUNPKLOG(SUnpkU16,(&crnti), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,EDAM001, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrDamHdlHoFailure(cellId,crnti);
}

/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function:cmUnPkStartInactivityTmr
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
**/

#ifdef ANSI
PUBLIC Void cmUnPkStartInactivityTmr  
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkStartInactivityTmr(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U16 cellId   = 0;
   U16 crnti = 0;

   TRC3(cmUnPkStartInactivityTmr)

   CMCHKUNPKLOG(SUnpkU16,(&crnti), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,EDAM001, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrDamStartUeTmr(cellId,crnti);

}/* cmUnPkStartInactivityTmr */

/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function:cmUnPkStopUeTmr
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void cmUnPkStopUeTmr 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkStopUeTmr(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U16 cellId   = 0;
   U16 ueId = 0;

   TRC3(cmUnPkStopUeTmr)

   CMCHKUNPKLOG(SUnpkU16,(&ueId), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,EDAM001, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrDamStopUeTmr(cellId,ueId);

}/* cmUnPkStopUeTmr */


/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation confirm  function in DAM module. 
*
* @details
*
*     Function:cmUnPkReestabCfm 
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void cmUnPkReestabCfm 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkReestabCfm(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U32 transId = 0;
   U8 status   = 0;
 
   TRC3(cmUnPkReestabCfm)
      

   CMCHKUNPKLOG(SUnpkU8,&status,mBuf,EDAM002, pst);
   CMCHKUNPKLOG(SUnpkU32,&transId,mBuf,EDAM001, pst);
  
   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrIfmDamReestabCfm(transId, status);

}/* cmUnPkReestabCfm */


/**
* @brief This API is used for  unpacking the function
* UE del request.This function in turn will directly call
* the function for the deleting the UE in wr_dam.c.
*
* @details
*
*     Function:cmUnPkReCfgReq
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void cmUnPkReCfgReq 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkReCfgReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U32 transId = 0;
   U16 cellId  = 0;
   U16 crnti   = 0;
   
   TRC3(cmUnPkReCfgReq)
  

   CMCHKUNPKLOG(SUnpkU16,(&crnti), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,EDAM001, pst);
   CMCHKUNPKLOG(SUnpkU32,(&transId), mBuf,EDAM001, pst);
 
   WR_DAM_FREE_BUFFER (pst->region,mBuf);

   wrIfmDamWrReCfgReq(transId, cellId, crnti);

}/* cmUnPkReCfgReq */

/**
* @brief This API is used to call the function directly
* in DAM interface file.This function in turn will call the
* tunnel creation function in DAM module. 
*
* @details
*
*     Function:cmUnPkTnlStChngReq 
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void cmUnPkTnlStChngReq 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkTnlStChngReq(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
    WrDamTnlStInfo *tnlStInfo = NULLP;

    TRC3(cmUnPkTnlStChngReq)

   if (pst->selector == DAM_SEL_LWLC) {
      CMCHKUNPKLOG(cmUnpkPtr,((PTR *)&tnlStInfo), mBuf, EEGT014, pst);
   }
   else
   {
      RLOG0(L_ERROR, "unpacking failed due to wrong coupling selector");
      WR_DAM_FREE_BUFFER (pst->region,mBuf);
      RETVOID;
   }
  
   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrIfmDamWrChngTnlState(tnlStInfo);

}/* cmUnPkTnlStChngReq */

/* Element packing */


/*
*
*  Fun:  cmPkCmTnlInfo
*
*  Desc:  This function packs the DAM Tunnel Info.
*
*  Ret:   ROK
*
*  Notes: None
*
*  File:  wr_ifm_dam_utils.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkDamTnlId 
(
WrDamTnlId         *tnlId,
Buffer             *mBuf         /* message buffer */
)
#else
PUBLIC S16 cmPkDamTnlId(tnlId, mBuf)
WrDamTnlId         *tnlId;
Buffer             *mBuf;        /* message buffer */
#endif
{
   CMCHKPK(SPkU16,tnlId->cellId,mBuf);
   CMCHKPK(SPkU16,tnlId->crnti,mBuf);
   CMCHKPK(SPkU8,tnlId->drbId,mBuf);
   CMCHKPK(cmPkDamTnlType,tnlId->tnlType,mBuf);

   RETVALUE(ROK);

}/* End of cmPkDamTnlId */

/*
*
*    Fun:  cmPkCmTnlInfo 
*
*    Desc:  This function packs the DAM Tunnel Info. 
*
*    Ret:   ROK
*
*    Notes: None
*
*    File:  wr_ifm_dam_utils.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkCmTnlInfo 
(
WrDamTnlInfo             *tnlInfo,     
Buffer                   *mBuf         /* message buffer */
)
#else
PUBLIC S16 cmPkCmTnlInfo(tnlInfo,mBuf)
WrDamTnlInfo             *tnlInfo;     
Buffer                   *mBuf;        /* message buffer */
#endif
{
  TRC3(cmPkCmTnlInfo)
  CMCHKPK(SPkU32,tnlInfo->transId,mBuf);
  CMCHKPK(cmPkDamTnlId,&tnlInfo->tnlId,mBuf);
  CMCHKPK(SPkU32,tnlInfo->dscp,mBuf);
  CMCHKPK(cmPkDamTnlType,tnlInfo->tnlType,mBuf);
  CMCHKPK(cmPkCmTptAddr,&tnlInfo->dstAddr,mBuf);
  CMCHKPK(cmPkCmTptAddr,&tnlInfo->srcAddr,mBuf);
  CMCHKPK(cmPkDamEgtpTeid,tnlInfo->remTeid,mBuf);

  RETVALUE(ROK);
}/* End of cmPkCmTnlInfo*/


/*
*
*    Fun:  cmPkDamCellInfo 
*
*    Desc:  This function packs the DAM Cell Info. 
*
*    Ret:   ROK
*
*    Notes: None
*
*    File:  wr_ifm_dam_utils.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkDamCellInfo 
(
WrDamCellInfo  *cellInfo,
Buffer         *mBuf         /* message buffer */
)
#else
PUBLIC S16 cmPkDamCellInfo(cellInfo,mBuf)
WrDamCellInfo  *cellInfo;
Buffer         *mBuf;        /* message buffer */
#endif
{
  TRC3(cmPkDamCellInfo)
  CMCHKPK(SPkU16,cellInfo->cellId,mBuf);
  CMCHKPK(SPkU16,cellInfo->startRnti,mBuf);
  CMCHKPK(SPkU16,cellInfo->maxRntis,mBuf);

  RETVALUE(ROK);

}/* cmPkDamCellInfo */

/*
*
*    Fun:  cmPkDamPdcpDatCfm 
*
*    Desc:  This function packs the DAM Tunnel Info. 
*
*    Ret:   ROK
*
*    Notes: None
*
*    File:  wr_ifm_dam_utils.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkDamPdcpDatCfm
(
Pst         *pst,
CmLtePdcpId  pdcpId
)
#else
PUBLIC S16 cmPkDamPdcpDatCfm(pst,pdcp)
Pst         *pst;
CmLtePdcpId  pdcpId
#endif
{
  Buffer *mBuf = NULLP;

  TRC3(cmPkDamCellInfo)

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(cmPkLtePdcpId,&pdcpId,mBuf,0,pst);
   pst->event = (Event)EVTDAMPDCPDATCFM;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkDamPdcpDatCfm */

/*
*
*    Fun:  cmUnpkDamPdcpDatCfm 
*
*    Desc:  This function packs the DAM Tunnel Info. 
*
*    Ret:   ROK
*
*    Notes: None
*
*    File:  wr_ifm_dam_utils.c
*
*/
#ifdef ANSI
PUBLIC Void cmUnpkDamPdcpDatCfm
(
Pst    *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnpkDamPdcpDatCfm(pst,pdcp)
Pst         *pst;
Buffer *mBuf;
#endif
{

  CmLtePdcpId  pdcpId;

  TRC3(cmPkDamCellInfo)

  CMCHKUNPKLOG(cmUnpkLtePdcpId,&pdcpId,mBuf,0,pst);

  WR_DAM_FREE_BUFFER (pst->region,mBuf);
  wrIfmDamPdcpDatCfm(pdcpId);

}/* End of cmPkDamCellInfo */

/**
* @brief This API is used to pack the EGTP-U error indication
*
* @details
*
* Function: cmPkEgtpErrInd
*
* @param[in] Pst *  pst
* @param[in] U32 lclTeid
* @param[in] U32 numDrbs
* @param[in] U16 crnti
* @return   S16
*  -# ROK/RFAILED
**/
PUBLIC S16 cmPkEgtpErrInd
(
Pst *pst,
U32 lclTeid, 
U32                          numDrbs,
U16                          crnti
)
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkEgtpErrInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPK(SPkU32,lclTeid,mBuf);
   CMCHKPK(SPkU32,numDrbs,mBuf);
   CMCHKPK(SPkU16,crnti,mBuf);

   pst->event = (Event)EVTDAMEGTERRIND;
   RETVALUE(SPstTsk(pst, mBuf));

}/* cmPkEgtpErrInd */

/*
*
*    Fun:  cmUnpkEgtpErrInd
*
*    Desc:  This function packs the DAM Tunnel Info. 
*
*    Ret:   ROK
*
*    Notes: None
*
*    File:  wr_ifm_dam_utils.c
*
*/
#ifdef ANSI
PUBLIC Void cmUnpkEgtpErrInd
(
Pst    *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnpkEgtpErrInd(pst,mBuf)
Pst         *pst;
Buffer *mBuf;
#endif
{

  U32 lclTeid = 0;
  U32 numDrbs = 0;
  U16 crnti = 0;

  TRC3(cmUnpkEgtpErrInd)

  CMCHKUNPK(SUnpkU16,&crnti, mBuf);
  CMCHKUNPK(SUnpkU32,&numDrbs, mBuf);
  CMCHKUNPK(SUnpkU32,&lclTeid, mBuf);

  WR_DAM_FREE_BUFFER (pst->region,mBuf);
  wrUmmPrcEgtpErrInd(lclTeid,numDrbs,crnti);

}/* End of cmUnpkEgtpErrInd */

/*ccpu00138576 start*/
/**
* @brief This API is used to  
* set Data rcvd flag from Enbapp to DAM.
*
* @details
*
* Function:cmPkSetDataRcvdFlag
* 
* @param[in]   Pst *  pst
* @param[in]   U16 cellId 
* @param[in]   U16 ueId 
* @return   S16
*  -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkSetDataRcvdFlag 
(
Pst *pst,
U16 cellId,
U16 ueId 
)
#else
PUBLIC S16 cmPkSetDataRcvdFlag(pst,cellId,ueId)
Pst * pst;
U16 cellId;
U16 ueId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkSetDataRcvdFlag)
   /* Buffer Allocation */
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU16,(cellId), mBuf,0, pst);
   CMCHKPKLOG(SPkU32,(ueId), mBuf,0, pst);
   
   pst->event = (Event)EVTDAMDATRCVDFLAG;
   RETVALUE(SPstTsk(pst, mBuf));

} /* end of cmPkSetDataRcvdFlag */

/**
* @brief This API is used for  unpacking the function
* Set Data Rcvd Flag
*
* @details
*
*     Function:cmUnPkSetDataRcvdFlag
*
*
*  @param[in]  pst 
*  @param[in]  buf 
*  @return   Void
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void cmUnPkSetDataRcvdFlag 
(
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC Void cmUnPkSetDataRcvdFlag(pst, mBuf)
Pst *pst;
Buffer *mBuf;
#endif
{
   U16 cellId   = 0;
   U16 ueId = 0;

   TRC3(cmUnPkSetDataRcvdFlag)

   CMCHKUNPKLOG(SUnpkU16,(&ueId), mBuf,0, pst);
   CMCHKUNPKLOG(SUnpkU16,(&cellId), mBuf,0, pst);

   WR_DAM_FREE_BUFFER (pst->region,mBuf);
   wrIfmDamWrSetDataRcvdFlag(cellId,ueId);

}/* end of cmUnPkSetDataRcvdFlag */
/*ccpu00138576 end*/

/********************************************************************30**

           End of file:     fc_ifm_dam_utils.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:51:47 2014

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
/main/1        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
