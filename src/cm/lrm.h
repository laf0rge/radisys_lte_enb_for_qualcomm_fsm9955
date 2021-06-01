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

/*
 * lrm.h
 *
 *  Created on: Nov 15, 2012
 *      Author: jbm
 */

#ifndef LRM_H_
#define LRM_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef LRM_NPLC
#define LRM_PACK_STRUCT __attribute__((packed))
#else
#define LRM_PACK_STRUCT
#endif


#define LRM_CELL_RECFGTYPE_ACB_MODATA     1
#define LRM_CELL_RECFGTYPE_ACB_MOSIG      2
#define LRM_CELL_RECFGTYPE_ACB_EMERGENCY  4
#define LRM_CELL_RECFGTYPE_WAIT_TIME      8
#define LRM_CELL_RECFGTYPE_CSG_PARAM      16
#define LRM_CELL_RECFGTYPE_TRANS_MODE     32 

#define LRM_ACB_SPECIAL_AC_11   0x80
#define LRM_ACB_SPECIAL_AC_12   0x40
#define LRM_ACB_SPECIAL_AC_13   0x20
#define LRM_ACB_SPECIAL_AC_14   0x10
#define LRM_ACB_SPECIAL_AC_15   0x08

/* LRM Events */
#define EVTLRMBASEEVT      0
#define EVTLRMCFGREQ       1
#define EVTLRMCFGCFM       2
#define EVTLRMCNTRLREQ     3 
#define EVTLRMCNTRLCFM     4 
#define EVTLRMSTAIND       5 
#define EVTLRMENBSTAIND    6 
#define EVTLRMMAX          7 

/* LRM SAP state */
#define LRM_SAP_UNBOUND             1
#define LRM_SAP_BINDING             2
#define LRM_SAP_BOUND               3

#define RMU_INTERFACE_NON_SSI       1
#define RMU_INTERFACE_SSI           2

#define STRMGEN         1
#define STRMPROTOCFG    128

#define MAXRMLRM        2      /* Maxiumum selector values */

#define LRM_MAX_PT_NUM_SIZE   8 

#define LRM_NOT_CFG           0      /* SAP not configured */
#define LRM_UNBND             1      /* Unbound SAP State */
#define LRM_BND               2      /* Bound SAP State */
#define LRM_WAIT_BNDCFM       3      /* Await Bind Confirm State */

/* selector(coupling) values */
#define LRM_SEL_LC            0
#define LRM_SEL_TC            1
#define LRM_SEL_LWLC          2

/* Type of RRM Interface */
#define RRM_INTERFACE_NON_SSI 	1
#define RRM_INTERFACE_SSI 		   2
#define RRM_INTERFACE_SSI_ST     3   /*RRM Interface when RRM task is with Single Thread from App*/

   /* TYpe of Algo TO used in RRM for PCQI */
#define RM_RCM_ALGO_BASIC        1
#define RM_RCM_ALGO_ALGO_TYPE1   2

/*LRM USTA status types*/
#define LRM_USTA_STA_ENB            1
#define LRM_USTA_STA_ALARM          2 

#define LRM_RESET_CFG             1   

/** @enum RLrmCause
 *
 * This enumeration represents failure causes that are indicated in
 * configuration responses.
 *
 * - LRM_CAUSE_CELLID_UNKNOWN          0
 * - LRM_CAUSE_MMEID_UNKNOWN           1
 * - LRM_CAUSE_UEID_UNKNOWN            2
 * - LRM_CAUSE_OUT_OF_RESOURCE         3
 * - LRM_CAUSE_MAX_CAPACITY_REACHED    4
 * - LRM_CAUSE_CELL_ALREADY_EXISTS     5
 * - LRM_CAUSE_MME_ALREADY_EXISTS      6
 * - LRM_CAUSE_ENB_ALREADY_UP          7
 * - LRM_CAUSE_NOT_APP                 8
 * - LRM_CAUSE_OTHER                   9
 */
typedef enum LrmCause
{
	LRM_CAUSE_NOT_APP,
	LRM_CAUSE_CELLID_UNKNOWN,
	LRM_CAUSE_MMEID_UNKNOWN,
	LRM_CAUSE_CELL_ALREADY_EXISTS,
	LRM_CAUSE_MME_ALREADY_EXISTS,
	LRM_CAUSE_ENB_ALREADY_UP,
	LRM_CAUSE_MME_ALREADY_UP,
	LRM_CAUSE_MME_ALREADY_DOWN,
   LRM_CAUSE_INVALID_CFG_TYPE,
	LRM_CAUSE_INVALID_CFG_INFO,
	LRM_CAUSE_OTHER
} LrmCause;

typedef struct LrmStatusCause
{
	LrmCause 				 enCause;
	U8 						 bStatus;
} LRM_PACK_STRUCT LrmStatusCause;

#ifdef __cplusplus
}
#endif
#endif /* LRM_H_ */
