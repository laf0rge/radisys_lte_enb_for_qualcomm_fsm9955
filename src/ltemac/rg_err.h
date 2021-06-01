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
 
     File:    rg_err.h

     Sid:      rg_err.h@@/main/3 - Sat Jul 30 02:21:59 2011
  
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

#ifndef __RGERRH__
#define __RGERRH__

/* defines */


/* Macro definitions */
#define RGLOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(rgCb.rgInit.ent, rgCb.rgInit.inst, rgCb.rgInit.procId, \
             (Txt *) __FILE__,    \
             (S32) __LINE__,      \
             (ErrCls) (errCls),   \
             (ErrCode) (errCode), \
             (ErrVal) (errVal),   \
             (Txt *) errDesc)

/* error codes */
#define   ERGBASE 0

#define   ERGXXX        (ERGBASE + 0)   /* reserved */
#define   ERRRG         (ERGBASE + 0)   /* reserved */

#define   RGERR_NONE    ERGBASE

#define   ERG001      (ERRRG +    1)    /*     gk_rom.c: 221 */
#define   ERG002      (ERRRG +    2)    /*     gk_rom.c: 235 */
#define   ERG003      (ERRRG +    3)    /*     gk_rom.c: 365 */
#define   ERG004      (ERRRG +    4)    /*     gk_rom.c: 476 */
#define   ERG005      (ERRRG +    5)    /*     gk_rom.c: 623 */
#define   ERG006      (ERRRG +    6)    /*     gk_rom.c: 738 */
#define   ERG007      (ERRRG +    7)    /*     gk_rom.c: 933 */

#define   ERG008      (ERRRG +    8)    /*     gk_uim.c: 227 */
#define   ERG009      (ERRRG +    9)    /*     gk_uim.c: 237 */
#define   ERG010      (ERRRG +   10)    /*     gk_uim.c: 293 */
#define   ERG011      (ERRRG +   11)    /*     gk_uim.c: 302 */
#define   ERG012      (ERRRG +   12)    /*     gk_uim.c: 413 */
#define   ERG013      (ERRRG +   13)    /*     gk_uim.c: 423 */
#define   ERG014      (ERRRG +   14)    /*     gk_uim.c: 520 */
#define   ERG015      (ERRRG +   15)    /*     gk_uim.c: 529 */
#define   ERG016      (ERRRG +   16)    /*     gk_uim.c: 612 */
#define   ERG017      (ERRRG +   17)    /*     gk_uim.c: 621 */
#define   ERG018      (ERRRG +   18)    /*     gk_uim.c: 695 */
#define   ERG019      (ERRRG +   19)    /*     gk_uim.c: 704 */
#define   ERG020      (ERRRG +   20)    /*     gk_uim.c: 777 */
#define   ERG021      (ERRRG +   21)    /*     gk_uim.c: 786 */
#define   ERG022      (ERRRG +   22)    /*     gk_uim.c:1061 */
#define   ERG023      (ERRRG +   23)    /*     gk_uim.c:1071 */
#define   ERG024      (ERRRG +   24)    /*     gk_uim.c:1128 */
#define   ERG025      (ERRRG +   25)    /*     gk_uim.c:1137 */
#define   ERG026      (ERRRG +   26)    /*     gk_uim.c:1253 */
#define   ERG027      (ERRRG +   27)    /*     gk_uim.c:1266 */

#define   ERG028      (ERRRG +   28)    /*     gk_utl.c: 196 */
#define   ERG029      (ERRRG +   29)    /*     gk_utl.c: 267 */
#define   ERG030      (ERRRG +   30)    /*     gk_utl.c: 320 */
#define   ERG031      (ERRRG +   31)    /*     gk_utl.c: 605 */


/* ***********************************************************
 *                       Error Type 
 *************************************************************/
#define RGERR_TYPE_BASE                0
#define RGERR_CAUSE_BASE               0

/* ErrType defines for MUX */
#define RG_MUX_ERRTYPE_BASE            RGERR_TYPE_BASE
#define RGERR_MUX_BLD_PDU              (RG_MUX_ERRTYPE_BASE + 1)
#define RGERR_MUX_BLD_CEHDR_FAIL       (RG_MUX_ERRTYPE_BASE + 2)
#define RGERR_MUX_BLD_CE_FAIL          (RG_MUX_ERRTYPE_BASE + 3)
#define RGERR_MUX_BLD_SDUHDR_FAIL      (RG_MUX_ERRTYPE_BASE + 4)
#define RGERR_MUX_BLD_SDU_FAIL         (RG_MUX_ERRTYPE_BASE + 5)
#define RGERR_MUX_BLD_PADHDR_FAIL      (RG_MUX_ERRTYPE_BASE + 6)
#define RGERR_MUX_BLD_PAD_FAIL         (RG_MUX_ERRTYPE_BASE + 7)
#define RGERR_MUX_BLD_BI_FAIL          (RG_MUX_ERRTYPE_BASE + 8)
#define RGERR_MUX_BLD_RAPIDHDR_FAIL    (RG_MUX_ERRTYPE_BASE + 9)
#define RGERR_MUX_BLD_RAPID_FAIL       (RG_MUX_ERRTYPE_BASE + 10)
#define RGERR_MUX_BLD_RAR_PDU          (RG_MUX_ERRTYPE_BASE + 11)
/* ErrType defines for DHM */
#define RG_DHM_ERRTYPE_BASE            (RGERR_MUX_BLD_RAR_PDU + 1)
#define RGERR_DHM_SND_DAT_REQ           RG_DHM_ERRTYPE_BASE 
#define RGERR_DHM_FDBK_IND             (RG_DHM_ERRTYPE_BASE + 1)
#define RGERR_DHM_SND_STA_IND          (RG_DHM_ERRTYPE_BASE + 2)
/* ErrType defines for L2Measurement */
#ifdef LTE_L2_MEAS
#define RGERR_L2M_ERRTYPE_BASE         RG_DHM_ERRTYPE_BASE
#define RGERR_L2M_MEASREQ              (RGERR_L2M_ERRTYPE_BASE + 1)   
#define RGERR_DHM_SND_HARQ_STA_IND     (RGERR_L2M_ERRTYPE_BASE + 2)
#define RGERR_L2M_INV_PARAM            (RGERR_L2M_ERRTYPE_BASE + 3)
#define RGERR_L2M_INV_CELL_ID          (RGERR_L2M_ERRTYPE_BASE + 4)
#endif /* LTE_L2_MEAS */
#define RGERR_DHM_SND_HQ_FDB_REQ       (RG_DHM_ERRTYPE_BASE + 3)
/* ErrType defines for ROM */
#define RG_ROM_ERRTYPE_BASE            (RGERR_DHM_FDBK_IND + 1)
#define RGERR_ROM_DEDDATREQ            RG_ROM_ERRTYPE_BASE 
#define RGERR_ROM_CMNDATREQ            (RG_ROM_ERRTYPE_BASE + 1)
#define RGERR_ROM_DEDSTARSP            (RG_ROM_ERRTYPE_BASE + 2)
#define RGERR_ROM_CMNSTARSP            (RG_ROM_ERRTYPE_BASE + 3)
/* ErrType defines for TOM */
#define RG_TOM_ERRTYPE_BASE            (RGERR_ROM_CMNSTARSP + 1)
#define RGERR_TOM_RAREQIND             (RG_TOM_ERRTYPE_BASE)
#define RGERR_TOM_HARQACKIND           (RG_TOM_ERRTYPE_BASE + 1)
#define RGERR_TOM_SRIND                (RG_TOM_ERRTYPE_BASE + 2)
#define RGERR_TOM_DLCQIIND             (RG_TOM_ERRTYPE_BASE + 3)
#define RGERR_TOM_DATIND               (RG_TOM_ERRTYPE_BASE + 4)
#define RGERR_TOM_DECFAILIND           (RG_TOM_ERRTYPE_BASE + 5)
#define RGERR_TOM_TTIIND               (RG_TOM_ERRTYPE_BASE + 6)
/* COM Module related error MACROs for error type */
#define RG_COM_ERRTYPE_BASE            (RGERR_TOM_TTIIND + 1)
#define RGERR_COM_CFG_REQ              RG_COM_ERRTYPE_BASE 
#define RGERR_COM_RECFG_REQ            (RG_COM_ERRTYPE_BASE + 1)
#define RGERR_COM_DEL_REQ              (RG_COM_ERRTYPE_BASE + 2)
#define RGERR_COM_RESET_REQ            (RG_COM_ERRTYPE_BASE + 3)
/* GOM Module related error MACROs for error type */
#define RG_GOM_ERRTYPE_BASE            (RGERR_COM_DEL_REQ + 1)
#define RGERR_GOM_CFG_REQ              RG_GOM_ERRTYPE_BASE 
#define RGERR_GOM_RECFG_REQ            (RG_GOM_ERRTYPE_BASE + 1)

/* ***********************************************************
 *                       Error Cause 
 *************************************************************/
/* Errcause defines for MUX */
#define RG_MUX_ERR_CAUSE_BASE          RGERR_CAUSE_BASE
#define RGERR_MUX_MEM_ALLOC_FAIL       RG_MUX_ERR_CAUSE_BASE + 1
#define RGERR_MUX_BLD_HDR_FAIL         RG_MUX_ERR_CAUSE_BASE + 2
#define RGERR_FDBK_IND_INVALID_CB      RG_MUX_ERR_CAUSE_BASE + 3
/* Errcause defines for DHM */
#define RG_DHM_ERRCAUSE_BASE           RGERR_FDBK_IND_INVALID_CB + 1
#define RG_DHM_MEM_ALLOC_FAIL          RG_DHM_ERRCAUSE_BASE 
/* Errcause defines for RAM */
#define RG_RAM_ERRCAUSE_BASE           RGERR_FDBK_IND_INVALID_CB + 1
#define RGERR_RAM_MEM_EXHAUST          RG_RAM_ERRCAUSE_BASE 
#define RGERR_RAM_NO_MSG3_RCVD         RG_RAM_ERRCAUSE_BASE + 1
#define RGERR_RAM_RNTI_EXHAUST         RG_RAM_ERRCAUSE_BASE + 2
/* Errcause defines for ROM */
#define RG_ROM_ERRCAUSE_BASE           RGERR_RAM_RNTI_EXHAUST + 1
#define RGERR_ROM_INV_CELL_ID          RG_ROM_ERRCAUSE_BASE 
#define RGERR_ROM_DELAYED_DATREQ       RG_ROM_ERRCAUSE_BASE + 1
#define RGERR_ROM_INV_UE_ID            RG_ROM_ERRCAUSE_BASE + 2
#define RGERR_ROM_INV_LC_ID            RG_ROM_ERRCAUSE_BASE + 3
#define RGERR_ROM_INV_RNTI             RG_ROM_ERRCAUSE_BASE + 4
#define RGERR_ROM_MEM_EXHAUST          RG_ROM_ERRCAUSE_BASE + 5
#define RGERR_ROM_INV_DAT_LEN          RG_ROM_ERRCAUSE_BASE + 6
/* Errcause defines for RAM */
#define RG_TOM_ERRCAUSE_BASE           RGERR_ROM_INV_DAT_LEN + 1
#define RGERR_TOM_INV_CELL_ID          RG_TOM_ERRCAUSE_BASE 
#define RGERR_TOM_MEM_EXHAUST          RG_TOM_ERRCAUSE_BASE + 1
/* Errcause defines for DUX */
#define RG_DUX_ERRCAUSE_BASE           RGERR_TOM_MEM_EXHAUST + 1
#define RGERR_DUX_INV_LCID_RX          RG_DUX_ERRCAUSE_BASE 
#define RGERR_DUX_MEM_EXHAUST          RG_DUX_ERRCAUSE_BASE + 1
#define RGERR_DUX_UNPACK_FAILURE       RG_DUX_ERRCAUSE_BASE + 2
#define RGERR_DUX_RLC_PDU_CREAT_FAIL   RG_DUX_ERRCAUSE_BASE + 3
#define RGERR_DUX_RLC_DATIND_FAIL      RG_DUX_ERRCAUSE_BASE + 4
#define RGERR_DUX_DBM_FAILURE          RG_DUX_ERRCAUSE_BASE + 5
#define RGERR_DUX_INV_PDU_RX           RG_DUX_ERRCAUSE_BASE + 6
/* CFG Module related error MACROs for error cause */
#define RG_CFG_ERRCAUSE_BASE               RGERR_DUX_INV_PDU_RX + 1
#define RGERR_CFG_INVALID_CRG_CELL_CFG     RG_CFG_ERRCAUSE_BASE 
#define RGERR_CFG_INVALID_CRG_UE_CFG       RG_CFG_ERRCAUSE_BASE + 1
#define RGERR_CFG_INVALID_CRG_DED_LC_CFG   RG_CFG_ERRCAUSE_BASE + 2
#define RGERR_CFG_INVALID_CRG_CMN_LC_CFG   RG_CFG_ERRCAUSE_BASE + 3
#define RGERR_CFG_INVALID_CRG_CELL_RECFG   RG_CFG_ERRCAUSE_BASE + 4
#define RGERR_CFG_INVALID_CRG_UE_RECFG     RG_CFG_ERRCAUSE_BASE + 5
#define RGERR_CFG_INVALID_CRG_LC_RECFG     RG_CFG_ERRCAUSE_BASE + 6
#define RGERR_CFG_INVALID_RGR_CELL_CFG     RG_CFG_ERRCAUSE_BASE + 7
#define RGERR_CFG_INVALID_RGR_CELL_RECFG   RG_CFG_ERRCAUSE_BASE + 8
#define RGERR_CFG_CRG_CELL_CFG             RG_CFG_ERRCAUSE_BASE + 9
#define RGERR_CFG_CRG_UE_CFG               RG_CFG_ERRCAUSE_BASE + 10
#define RGERR_CFG_CRG_DED_LC_CFG           RG_CFG_ERRCAUSE_BASE + 11
#define RGERR_CFG_CRG_CMN_LC_CFG           RG_CFG_ERRCAUSE_BASE + 12
#define RGERR_CFG_CRG_CELL_RECFG           RG_CFG_ERRCAUSE_BASE + 13
#define RGERR_CFG_CRG_UE_RECFG             RG_CFG_ERRCAUSE_BASE + 14
#define RGERR_CFG_CRG_LC_RECFG             RG_CFG_ERRCAUSE_BASE + 15
#define RGERR_CFG_RGR_CELL_CFG             RG_CFG_ERRCAUSE_BASE + 16
#define RGERR_CFG_RGR_CELL_RECFG           RG_CFG_ERRCAUSE_BASE + 17
#define RGERR_CFG_CRG_CELL_DEL             RG_CFG_ERRCAUSE_BASE + 18
#define RGERR_CFG_CRG_UE_DEL               RG_CFG_ERRCAUSE_BASE + 19
#define RGERR_CFG_CRG_LC_DEL               RG_CFG_ERRCAUSE_BASE + 20
#define RGERR_CFG_INVALID_CRG_UE_RESET     RG_CFG_ERRCAUSE_BASE + 21
/* Scheduler related error causes */
#define RG_SCH_ERRCAUSE_BASE               RGERR_CFG_CRG_LC_DEL + 1
#define RGERR_SCH_CFG                      RG_SCH_ERRCAUSE_BASE 
#define RGERR_SCH_LCG_NOT_CFGD             RG_SCH_ERRCAUSE_BASE + 1
#define RGERR_SCH_NO_LCG_CFGD              RG_SCH_ERRCAUSE_BASE + 2

   

#endif /* __RGERRH__ */
/********************************************************************30**
  
         End of file:     rg_err.h@@/main/3 - Sat Jul 30 02:21:59 2011
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ns   1. Initial Release.
/main/2      ---     ns   1. LTE MAC 2.1 release
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
$SID$        ---     rt   1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
