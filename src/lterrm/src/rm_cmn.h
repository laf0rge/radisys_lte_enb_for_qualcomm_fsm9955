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

     Type:     CPP include file

     Desc:     This file contains 
               

     File:     rm_cmn.h

     Sid:      rm_cmn.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:39 2013

     Prg:      hkrishna, jbm 

*********************************************************************21*/


#ifndef __RM_CMN_H__
#define __RM_CMN_H__


   /* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"
#include "szt_asn.h"       /* S1AP RRM control Interface */
#include "rmu.h"
#include "lrm.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"
#include "cm_lte.x"
#include "cm_lte_rrm.x"

#include "ss_diag.h"

#include "rmu.h"
#include "rmu.x"
#include "lrm.h"
#include "lrm.x"
#include "rgm.h"
#include "rgm.x"

#include "rm.h"

extern RmCb g_stRmCb;

#define RM_NUM_SF_PER_SFN             10 

#define RM_RCM_MAX_UES_SUPPORTED TENB_MAX_UE_SUPPORTED
#define RM_NUM_CQI_RES_PER_RB         12
#define RM_RCM_MAX_OFFSET_ARRAY_SIZE  160

#define RM_RBC_GBR_MIN_QCI   1  /*!< Minumum GBR QCI number */
#define RM_RBC_GBR_MAX_QCI   4  /*!< Maximum GBR QCI number */
#define RM_RBC_MAX_QCI       9  /*!< Maximum QCI number */

/*!< MIN ARP value for an ERAB  */
#define MIN_ARP_VAL     1

/*!< MAX ARP value for an ERAB  */
#define MAX_ARP_VAL     15

/*!< Contails all the possible GBR QCI numbers */
enum rmGbrQciNum
{
   RM_QCI_ONE=1,  /*!< Value of QCI 1 */
   RM_QCI_TWO,    /*!< Value of QCI 2 */
   RM_QCI_THREE,  /*!< Value of QCI 3 */
   RM_QCI_FOUR    /*!< Value of QCI 4 */
};

/*!< Returns the divide and round off the value */
#define RM_ROUND(_x, _y) (((_x) + (_y)/2) / (_y))

/*!< Macro for 5ms Periodicity */
#define RM_RCM_SR_PERIODICTY_5MS  5

/**
 * @brief  Structure to hold the required GBR PRBs in % for accepted bearer
 *         by Radio Admission controller.
 * 
 * @details
 * - uiReqPrbForDl : Number of DL PRBs required in % for to satisfy the DL GBR.
 * - uiReqPrbForUl : Number of UL PRBs required in % for to satisfy the UL GBR.
 * - bERabId : ERAB Identity. 
 */
typedef struct rrmErabAdmitted
{
   U32  uiReqPrbForDl;  /*!< Required PRB for DL */
   U32  uiReqPrbForUl;  /*!< Required PRB for UL */
   U8   bERabId;       /*!< ERAB Identifier */
} RrmErabAdmitted;

/**
 * @brief  Structure to hold the required GBR PRBs in % for rejected bearer
 *         by Radio Admission controller.
 * 
 * @details
 * - uiReqPrbForDl : Number of DL PRBs required in % for to satisfy the DL GBR.
 * - uiReqPrbForUl : Number of UL PRBs required in % for to satisfy the UL GBR.
 * - uiDiffPrbForDl: Difference of DL PRBs during modification of DL GBR
 * - uiDiffPrbForUl: Difference of UL PRBs during modification of UL GBR
 * - uiQci   : QoS Class Identifier
 * - bArpVal : ARP value of rejected ERAB. 
 * - bCause  : cause for ERAB rejection. 
 * - bERabId : ERAB Identity. 
 * - bIsPreemptable : Pre-emption Vulnerability
 * - bErabPreemptCap : Pre-emption Capability
 * - bIsModifyReq  : Modification Request flag
 */
typedef struct rrmErabRejected
{
   U32   uiReqPrbForDl;  /*!< Required PRB for DL */
   U32   uiReqPrbForUl;  /*!< Required PRB for UL */
   U32   uiDiffPrbForDl;  /*!< Difference PRB for DL */
   U32   uiDiffPrbForUl;  /*!< Difference PRB for UL */
   U32   uiQci;         /*!< QCI Value */
   U8    bArpVal;        /*!< ARP value of ERAB*/
   U8    bCause;         /*!< Cause Value*/
   U8    bERabId;        /*!< ERAB Identifier*/
   bool  bIsPreemptable; /*!< Pre-emption Vulnerability */
   bool  bErabPreemptCap; /*!< Pre-emption Capability */
   bool  bIsModifyReq;    /*!< Modification Request flag*/
} RrmErabRejected;

/**
 * @brief  Structure to hold the Response information(like accepted/rejected
 *         bearers information) by Radio Admission controller.
 * 
 * @details
 * - eRabAcceptedLst : Accepted bearers information.
 * - eRabRejectedLst : Rejected bearers information.
 * - nErabsAccepted : Number of bearers accepted. 
 * - nErabsRejected : Number of berarers rejected. 
 * - isGBRBearerExists : Is at least one GBR bearer exists. 
 */
typedef struct rrmRbcRspInfo
{
  RrmErabAdmitted   eRabAcceptedLst[RMU_MAX_NUM_ERABS];/*!<accepted bearer information */ 
  RrmErabRejected   eRabRejectedLst[RMU_MAX_NUM_ERABS];/*!<Rejected bearer information*/
  U8                nErabsAccepted;/*!<Number of bearers accepted */
  U8                nErabsRejected;/*!<Number of bearers rejected */       
  bool              isGBRBearerExists;/*!<does at least one GBR bearer exists */
  U16               usCRNTI; /*!< CRNTI of the UE */
} RrmRbcRspInfo;

/*!< Contains the UL subframe info */
typedef struct rmTddUlSfInfo
{
   U8 bNumUlSfs;                    /*!< Number of Feedbacks for DL Subframes */
   U8 bUlSfNum[RM_NUM_SF_PER_SFN];  /*!< List of Subframe Number */
} RmTddUlSfInfo;

/*!< Contails the duplex modes */
typedef enum rmDuplexMode
{
    RM_MODE_FDD=1,   /*!< FDD mode */
    RM_MODE_TDD      /*!< TDD mode */
} RmDuplexMode;

class CRmRcmResMgr
{
   public:
    U16  getAvailUlSubframeLst(U16 usSrPrdcty, U16  *usAvalUlSfs);
    static U8   getNumUlSf(LrmUlDlConfig enUlDlConfig);

   public:
      static RmTddUlSfInfo m_astRmTddUlSfInfo[LRM_UL_DL_CONFIG_MAX];

   public:
      LrmUlDlConfig m_enUlDlCfg;
};

    
inline U8 CRmRcmResMgr::getNumUlSf(LrmUlDlConfig enUlDlConfig)
{
   return m_astRmTddUlSfInfo[enUlDlConfig].bNumUlSfs;
}

inline U8* rrmAlloc(U16 usMsgLen)
{
	U8 *pbMsg;
	RM_ALLOC(&pbMsg, usMsgLen);
	return pbMsg;
}

inline void rrmFree(U8* pbMsg, U16 usMsgLen)
{
	RM_FREE(pbMsg, usMsgLen);
}

#endif /* __RM_CMN__H__ */
/********************************************************************30**

           End of file:     rm_cmn.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:39 2013

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
