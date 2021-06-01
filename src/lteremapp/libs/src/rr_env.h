/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

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
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************
 
    Name:   RRC layer
 
    Type:   C include file
 
    Desc:   Defines environment variables required by RRC
 
    File:   rr_env.h
 
    Sid:      rr_env.h@@/main/1 - Thu Jul 10 17:27:21 2008
 
    Prg:    st
 
**********************************************************************/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000206       RRC
*
*/
 
#ifndef __RRENVH__
#define __RRENVH__

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


#ifdef RRC_MT
#define RR_MT_MAX_THREADS  8
/* These defines are with respect to the packed buffer format received
 * from the lower layer RLC/MAC/PDCP on the (RLU/CRL)/CMK/CTC interfaces
 * and from the RRC User on the upper interface RRU.
 */
#define RR_RX_RRU_MBUF_ID_CHOICE_IDX               4
#define RR_RX_RRU_MBUF_HDR_UE_ID_IDX               5
#define RR_RX_RRU_MBUF_DAT_REQ_MSG_CAT_IDX         16
#define RR_RX_RRU_MBUF_DAT_RSP_MSG_CAT_IDX         15

#define RR_RX_RRU_MBUF_CNCL_UE_DELTRANS_UE_ID_IDX        2
#define RR_RX_RRU_MBUF_RELOC_PREP_CMT_UE_ID_IDX          15

#define RR_RLC_RLC_ID_IDX                 2 
#define RR_RLC_HDR_UE_ID_IDX              3 
#define RR_RLC_UL_CCCH_HDR                14

#define RR_MAC_UEORCELLID_IDX             2
#define RR_MAC_HDR_UE_ID_IDX              3 

#define RR_PDCP_CFGCTXTCFM_CMNORDED_IDX   6
#define RR_PDCP_CFGCTXTCFM_UE_ID_IDX      7
#define RR_PDCP_RELOCCFM_CMNORDED_IDX     2
#define RR_PDCP_RELOCCFM_UE_ID_IDX        3
#endif

/* Macros for RRU data structures */
/* rr011.202 KlockWorks Fix, changed 
   RRU_MAX_RB_PER_CELL_CFG 4 to 8 */
#define RRU_MAX_RB_PER_CELL_CFG    8
#define RRU_MAX_RB_PER_UE_CFG      8
#define RRU_MAX_RB_PER_RRC_CONN   32
#define RRU_MAX_RRC_TRCHS         32
#define RRU_MAX_TFS_PER_CFG       10
#define RRU_MAX_RLC_SIZE_LST_LEN  12
#define RRU_MAX_ROHC_PROF_LST_LEN 4
#define RRU_MAX_HDR_COMPR_INFO_LIST_LEN   2
#define RRU_MAX_TFCS_CFG          64
#define RRU_MAX_TF_PER_TRCH       16
#define RRU_MAX_TTI_PER_TRCH      4
#define RRU_MAX_MES_PER_CFG       32
#define RRU_MAX_LOGCH_PER_RLC     2

/*********************************************************************
 General Defines. Can be changed by the customers
**********************************************************************/
/*  UE defines */
#define MAX_SIG_RB_SET_UP        8
#define MAX_RB_PER_RAB           8
#define MAX_RAB_SETUP            16

/* PDCO SN Info */
#define MAX_RB_ALL_RABS          27

/* Macro definitions */
#define MAX_RB_PER_RRC_CONN      32
#define MAX_RRC_TRCHS            32

/* This parameter should be decided by the RRC User */
/* The max size of RRC PDU */
#define RR_MEM_PDU_SIZE       36000

#define RR_MAX_TMRS              255
#define RR_MAX_RLS_RPT_TMRS   5 
#define RR_MAX_CFG_TMRS          10


/* SAMPLE KASUMI Macro Definition */
/* Following definitions are used by the RRC to implement sample 
   Kasumi Algorithm. Incase the PTRRINTGF9 flag is enabled User has to 
   define its own Macros */

#ifndef PTRRINTGF9
 
typedef U8     KU8;
typedef U16    KU16;
typedef U32    KU32;

/*----- a 64-bit structure to help with endian issues -----*/

typedef union {
   KU32 b32[2];
   KU16 b16[4];
   KU8  b8[8];
} REGISTER64;

/*------- unions: used to remove "endian" issues ------------------------*/
typedef union {
   KU32 b32;
   KU16 b16[2];
   KU8  b8[4];
} K1WORD;

typedef union {
   KU16 b16;
   KU8  b8[2];
} K2WORD;

/*-------- globals: The subkey arrays -----------------------------------*/

#ifndef RRC_MT
Void rrKasumi(KU8 *data);
Void rrKeySchedule(KU8 *key);
#else
Void rrKasumi(KU8 *data, U32 thrdIdx);
Void rrKeySchedule(KU8 *key, U32 thrdIdx);
#endif /* RRC_MT */

#endif /* PTRRINTGF9 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RRENVH__ */

/**********************************************************************
         End of file:     rr_env.h@@/main/1 - Thu Jul 10 17:27:21 2008
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     st   1. RRC Initial Release.
             ---     rer  1. (rr011.202) Klockworks related fixes
*********************************************************************91*/

