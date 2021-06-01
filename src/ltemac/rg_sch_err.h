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


/**********************************************************************
 
     Name:    LTEMAC - Error File
  
     Type:    C include file 
  
     Desc:    Error defines required by LTEMAC layer
 
     File:    rg_sch_err.h

     Sid:      rg_sch_err.h@@/main/2 - Sat Jul 30 02:22:04 2011
  
     Prg:     ns
*********************************************************************21*/
 
/*
*     The defines declared in this file correspond to those
*     used by the following TRILLIUM software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000XXX     LTEMAC Layer
*
*/

#ifndef __RGSCHERRH__
#define __RGSCHERRH__

/* defines */


/* Macro definitions */
#define RGSCHLOGERROR(_inst, errCls, errCode, errVal, errDesc) \
   SLogError(rgSchCb[_inst].rgSchInit.ent, rgSchCb[_inst].rgSchInit.inst,\
             rgSchCb[_inst].rgSchInit.procId, \
             (Txt *) __FILE__,    \
             (S32) __LINE__,      \
             (ErrCls) (errCls),   \
             (ErrCode) (errCode), \
             (ErrVal) (errVal),   \
             (Txt *) errDesc)

#ifdef LTE_L2_MEAS
#define RGSCHFILLERR(_err, _errType, _errCause) \
   _err.errType  = _errType; \
   _err.errCause = _errCause;
#endif
/* error codes */
#define   ERGBASE 0

#define   ERGXXX        (ERGBASE + 0)   /* reserved */
#define   ERRRG         (ERGBASE + 0)   /* reserved */

#define   RGSCHERR_NONE    ERGBASE

#define   ERG001      (ERRRG +    1)    /*     gk_sch.c: 535 */
#define   ERG002      (ERRRG +    2)    /*     gk_sch.c: 548 */
#define   ERG003      (ERRRG +    3)    /*     gk_sch.c: 606 */
#define   ERG004      (ERRRG +    4)    /*     gk_sch.c: 616 */
#define   ERG005      (ERRRG +    5)    /*     gk_sch.c: 691 */
#define   ERG006      (ERRRG +    6)    /*     gk_sch.c: 704 */
#define   ERG007      (ERRRG +    7)    /*     gk_sch.c: 792 */
#define   ERG008      (ERRRG +    8)    /*     gk_sch.c: 805 */
#define   ERG009      (ERRRG +    9)    /*     gk_sch.c: 879 */
#define   ERG010      (ERRRG +   10)    /*     gk_sch.c: 956 */

#define   ERG011      (ERRRG +   11)    /* gk_sch_cmn.c:24144 */

#define   ERG012      (ERRRG +   12)    /* gk_sch_sps.c:9181 */

#define   ERG013      (ERRRG +   13)    /* gk_sch_tom.c: 608 */
#define   ERG014      (ERRRG +   14)    /* gk_sch_tom.c: 684 */

#define   ERG015      (ERRRG +   15)    /* gk_sch_utl.c:4209 */
#define   ERG016      (ERRRG +   16)    /* gk_sch_utl.c:4285 */
#define   ERG017      (ERRRG +   17)    /* gk_sch_utl.c:5027 */
#define   ERG018      (ERRRG +   18)    /* gk_sch_utl.c:5084 */
#define   ERG019      (ERRRG +   19)    /* gk_sch_utl.c:5112 */
#define   ERG020      (ERRRG +   20)    /* gk_sch_utl.c:5257 */
#define   ERG021      (ERRRG +   21)    /* gk_sch_utl.c:5266 */
#define   ERG022      (ERRRG +   22)    /* gk_sch_utl.c:5897 */
#define   ERG023      (ERRRG +   23)    /* gk_sch.c:902      */
#define   ERG024      (ERRRG +   24)    /* gk_sch.c:915      */
#define   ERG025      (ERRRG +   25)    /* gk_sch.c:988      */
#define   ERG026      (ERRRG +   26)    /* gk_sch.c:998      */


/* ***********************************************************
 *                       Error Type 
 *************************************************************/
#define RGSCHERR_TYPE_BASE                0
#define RGSCHERR_CAUSE_BASE               0

/* ErrType defines for DHM */
#define RG_DHM_ERRTYPE_BASE            (RGSCHERR_CAUSE_BASE + 1)
#define RGSCHERR_DHM_SND_DAT_REQ          RG_DHM_ERRTYPE_BASE 
#define RGSCHERR_DHM_FDBK_IND             (RG_DHM_ERRTYPE_BASE + 1)
#define RGSCHERR_DHM_SND_STA_IND          (RG_DHM_ERRTYPE_BASE + 2)
#define RGSCHERR_DHM_FDBK_IND_INVALID_CB  (RGSCHERR_DHM_SND_STA_IND + 3)
#define RGSCHERR_DHM_SND_HQ_FDB_REQ       (RG_DHM_ERRTYPE_BASE + 4)
/* ErrType defines for TOM */
#define RG_TOM_ERRTYPE_BASE            (RGSCHERR_DHM_SND_HQ_FDB_REQ + 1)
#define RGSCHERR_TOM_RAREQIND             RG_TOM_ERRTYPE_BASE 
#define RGSCHERR_TOM_HARQACKIND           (RG_TOM_ERRTYPE_BASE + 1)
#define RGSCHERR_TOM_SRIND                (RG_TOM_ERRTYPE_BASE + 2)
#define RGSCHERR_TOM_DLCQIIND             (RG_TOM_ERRTYPE_BASE + 3)
#define RGSCHERR_TOM_DATIND               (RG_TOM_ERRTYPE_BASE + 4)
#define RGSCHERR_TOM_DECFAILIND           (RG_TOM_ERRTYPE_BASE + 5)
#define RGSCHERR_TOM_TAIND                (RG_TOM_ERRTYPE_BASE + 6)
#define RGSCHERR_TOM_TTIIND               (RG_TOM_ERRTYPE_BASE + 7)
/* Changes for MIMO feature addition */
#define RGSCHERR_TOM_DOAIND               (RG_TOM_ERRTYPE_BASE + 8)
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
#define RGSCHERR_TOM_RAWCQIIND               (RG_TOM_ERRTYPE_BASE + 9)
#define RGSCHERR_TOM_SRSIND               (RG_TOM_ERRTYPE_BASE + 10)
#endif
/* GOM Module related error MACROs for error type */
#define RG_GOM_ERRTYPE_BASE            (RGSCHERR_TOM_TTIIND + 1)
#define RGSCHERR_GOM_CFG_REQ              (RG_GOM_ERRTYPE_BASE) 
#define RGSCHERR_GOM_RECFG_REQ            (RG_GOM_ERRTYPE_BASE + 1)
#define RGSCHERR_GOM_DEL_REQ              (RG_GOM_ERRTYPE_BASE + 2)
#define RGSCHERR_GOM_RESET_REQ           (RG_GOM_ERRTYPE_BASE + 3)
/* L2 Measurement Module related error MACROs for error type */
#ifdef LTE_L2_MEAS
#define RG_L2M_ERRTYPE_BASE               (RGSCHERR_GOM_RESET_REQ + 1)
#define RGSCHERR_L2M_MEASREQ              (RG_L2M_ERRTYPE_BASE)
#endif
/* ***********************************************************
 *                       Error Cause 
 *************************************************************/
/* Errcause defines for DHM */
#define RG_DHM_ERRCAUSE_BASE           (RGSCHERR_GOM_DEL_REQ + 1)
#define RG_DHM_MEM_ALLOC_FAIL          (RG_DHM_ERRCAUSE_BASE )
/* Errcause defines for RAM */
#define RG_RAM_ERRCAUSE_BASE           (RG_DHM_MEM_ALLOC_FAIL + 1)
#define RGSCHERR_RAM_MEM_EXHAUST          (RG_RAM_ERRCAUSE_BASE )
#define RGSCHERR_RAM_NO_MSG3_RCVD         (RG_RAM_ERRCAUSE_BASE + 1)
#define RGSCHERR_RAM_RNTI_EXHAUST         (RG_RAM_ERRCAUSE_BASE + 2)
/* Errcause defines for RAM */
#define RG_TOM_ERRCAUSE_BASE           (RGSCHERR_RAM_RNTI_EXHAUST + 1)
#define RGSCHERR_TOM_INV_CELL_ID          (RG_TOM_ERRCAUSE_BASE )
#define RGSCHERR_TOM_MEM_EXHAUST          (RG_TOM_ERRCAUSE_BASE + 1)
/* CFG Module related error MACROs for error cause */
#define RG_CFG_ERRCAUSE_BASE               (RGSCHERR_TOM_MEM_EXHAUST + 1)
#define RGSCHERR_CFG_INVALID_RGR_CELL_CFG     (RG_CFG_ERRCAUSE_BASE) 
#define RGSCHERR_CFG_INVALID_RGR_UE_CFG       (RG_CFG_ERRCAUSE_BASE + 1)
#define RGSCHERR_CFG_INVALID_RGR_DED_LC_CFG   (RG_CFG_ERRCAUSE_BASE + 2)
#define RGSCHERR_CFG_INVALID_RGR_DED_LCG_CFG   (RG_CFG_ERRCAUSE_BASE + 2)
#define RGSCHERR_CFG_INVALID_RGR_CMN_LC_CFG   (RG_CFG_ERRCAUSE_BASE + 3)
#define RGSCHERR_CFG_INVALID_RGR_CELL_RECFG   (RG_CFG_ERRCAUSE_BASE + 4)
#define RGSCHERR_CFG_INVALID_RGR_UE_RECFG     (RG_CFG_ERRCAUSE_BASE + 5)
#define RGSCHERR_CFG_INVALID_RGR_LC_RECFG     (RG_CFG_ERRCAUSE_BASE + 6)
#define RGSCHERR_CFG_INVALID_RGR_LCG_RECFG     (RG_CFG_ERRCAUSE_BASE + 6)
#define RGSCHERR_CFG_RGR_CELL_CFG             (RG_CFG_ERRCAUSE_BASE + 7)
#define RGSCHERR_CFG_RGR_UE_CFG               (RG_CFG_ERRCAUSE_BASE + 8)
#define RGSCHERR_CFG_RGR_DED_LC_CFG           (RG_CFG_ERRCAUSE_BASE + 9)
#define RGSCHERR_CFG_RGR_DED_LCG_CFG           (RG_CFG_ERRCAUSE_BASE + 9)
#define RGSCHERR_CFG_RGR_CMN_LC_CFG           (RG_CFG_ERRCAUSE_BASE + 10)
#define RGSCHERR_CFG_RGR_CELL_RECFG           (RG_CFG_ERRCAUSE_BASE + 11)
#define RGSCHERR_CFG_RGR_UE_RECFG             (RG_CFG_ERRCAUSE_BASE + 12)
#define RGSCHERR_CFG_RGR_LC_RECFG             (RG_CFG_ERRCAUSE_BASE + 13)
#define RGSCHERR_CFG_RGR_LCG_RECFG             (RG_CFG_ERRCAUSE_BASE + 13)
#define RGSCHERR_CFG_RGR_CELL_DEL             (RG_CFG_ERRCAUSE_BASE + 14)
#define RGSCHERR_CFG_RGR_UE_DEL               (RG_CFG_ERRCAUSE_BASE + 15)
#define RGSCHERR_CFG_RGR_LC_DEL               (RG_CFG_ERRCAUSE_BASE + 16)
#define RGSCHERR_CFG_RGR_LCG_DEL               (RG_CFG_ERRCAUSE_BASE + 16)
#define RGSCHERR_CFG_INVALID_RGR_UE_RESET     (RG_CFG_ERRCAUSE_BASE + 17)
#define RGSCHERR_CFG_RGR_UE_RESET             (RG_CFG_ERRCAUSE_BASE + 18)
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
#define RGSCHERR_CFG_INVALID_RGR_SI_CFG       (RG_CFG_ERRCAUSE_BASE + 19)
#endif/*RGR_SI_SCH*/
#ifdef LTEMAC_HDFDD
#define RGSCHERR_HDFDD_SPSCFGRD               (RG_CFG_ERRCAUSE_BASE + 20)
#endif
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
#define RGSCHERR_CQIREPT                      (RG_CFG_ERRCAUSE_BASE + 21)
#endif

/* LTE_ADV_FLAG_REMOVED_START */
#define RGSCHERR_CFG_INVALID_RGR_LOAD_INF     (RG_CFG_ERRCAUSE_BASE + 22)
/* LTE_ADV_FLAG_REMOVED_END */

/* Scheduler related error causes */
#define RG_SCH_ERRCAUSE_BASE               (RGSCHERR_CFG_RGR_LC_DEL + 1)
#define RGSCHERR_SCH_CFG                      RG_SCH_ERRCAUSE_BASE 
#define RGSCHERR_SCH_LCG_NOT_CFGD             (RG_SCH_ERRCAUSE_BASE + 1)
#define RGSCHERR_SCH_NO_LCG_CFGD              (RG_SCH_ERRCAUSE_BASE + 2)
#ifdef LTE_L2_MEAS /* TODO: Values? */
#define RG_L2MEAS_ERRTYPE_BASE                (RGSCHERR_SCH_NO_LCG_CFGD + 1)
#define RGSCHERR_SCH_L2MEAS                   (RG_L2MEAS_ERRTYPE_BASE   + 2)
#define RGSCHERR_SCH_INVALID_MEAS_TYPE        (RG_L2MEAS_ERRTYPE_BASE   + 3)
#define RGSCHERR_SCH_INVALID_PARAM_RANGE      (RG_L2MEAS_ERRTYPE_BASE   + 4)
#define RGSCHERR_SCH_INVALID_CELLID           (RG_L2MEAS_ERRTYPE_BASE   + 5)
#define RGSCHERR_SCH_INVALID_MEASTYPE         (RG_L2MEAS_ERRTYPE_BASE   + 6)
#define RGSCHERR_SCH_DUP_TRANSID              (RG_L2MEAS_ERRTYPE_BASE   + 7)
#define RGSCHERR_SCH_L2MEAS_FAILED            (RG_L2MEAS_ERRTYPE_BASE   + 8)
#define RGSCHERR_SCH_ALLOC_FAILED             (RG_L2MEAS_ERRTYPE_BASE   + 9)
#define RGSCHERR_SCH_INVALID_QCI_VAL          (RG_L2MEAS_ERRTYPE_BASE   + 10)
#endif /* LTE_L2_MEAS */
   

#endif /* __RGSCHERRH__ */
/**********************************************************************
 
         End of file:     rg_sch_err.h@@/main/2 - Sat Jul 30 02:22:04 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sd   1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg004.201 rnt 1. Modified for SI Enhancement
           rg007.201   ap  1. Added support for MIMO
           rg009.201   dv  1. Added changed of TFU_UPGRADE. 
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
$SID$        ---       rt  1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
