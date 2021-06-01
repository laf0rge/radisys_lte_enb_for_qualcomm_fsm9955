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

Name:     EnodeB Application

Type:     C include file

Desc:     This file contains the definitions related to QMI Interface


File:     wr_emm_cnm_nlqmi.h

Sid:      wr_emm_cnm_nlqmi.h@@/main/Br_Tenb_Brdcm_Bcm61750_Intg_GA2_0_TDD_GAFDD_final/1 - Thu Jun 26 19:23:41 2014

Prg:      lmeledam

 *********************************************************************21*/
#ifndef __WR_CNM__QMI_INTF_H__
#define __WR_CNM__QMI_INTF_H__

/* QMI headers */
#include "qmi_client.h"
#include "qmi_idl_lib.h"

/* NL interface */
#include "nl_api_v01.h"
#include "nl_api_types_v01.h"



/** @brief This structure contains information related to the QMI-Interface for CNM 
 *  */
typedef struct _wrCellCnmQmiCb
{
   Bool                     init;        /*! Initialization status */
   U16                      cellId;      /*! Cell Id */
   qmi_client_type          clnt_handle; /*! NL QMI client handle. */
   qmi_client_os_params     os_params;   /*! NL QMI client signal data. */
   qmi_txn_handle           txn_handle;  /*! NL QMI client transaction handle. */
   U32                      qmiSyncTransId;           /*! TransId returned by NL */
   nl_generic_rsp_msg_v01   rsp;         /*! Storage for a response message received from NL. */
} WrCellCnmQmiCb;


#define NL_REQ_TIMEOUT_SECS 10   /*!< QMI-Interface Recommended CNM-Timout In Seconds  */ 
#define NL_MEAS_PERIOD 15        /*!< QMI-Interface Recommended CNM power measurement Period  */ 

/**
 * @def QMI_FILL_DL_BW
 *
 *    This macro converts the BW in WrEutranBw enum to nl_bw_t_v01 enum
 *
 * @param[in] _dlBwVal : DL BW in WrEutranBw enum
 * @param[out] _nl_bw_t : DL BW in nl_bw_t_v01 Enum
 *
*/

#define QMI_FILL_DL_BW(_dlBwVal,_nl_bw_t) {\
   switch(_dlBwVal)\
   {\
      case WR_EUTRAN_BW_6:\
        _nl_bw_t = NL_BW_1P4_MHZ_V01 ;\
         break;\
      case WR_EUTRAN_BW_15:\
        _nl_bw_t = NL_BW_3_MHZ_V01;\
         break;\
      case WR_EUTRAN_BW_25:\
        _nl_bw_t = NL_BW_5_MHZ_V01;\
         break;\
      case WR_EUTRAN_BW_50:\
        _nl_bw_t = NL_BW_10_MHZ_V01;\
         break;\
      case WR_EUTRAN_BW_75:\
        _nl_bw_t = NL_BW_15_MHZ_V01;\
         break;\
      case WR_EUTRAN_BW_100:\
        _nl_bw_t = NL_BW_20_MHZ_V01;\
         break;\
   }\
}

/** @brief This structure is used to have band to Earfcn mapping
 *  */
typedef struct {
 U8    band;      /*! Band */
 U16   earfcnLow; /*! Lower limit of the band */
 U16   earfcnMax; /*! Upper limit of the band */
}WrCnmLteBandnfo;


EXTERN S16 wrEmmCnmInitReq (U16 cellId);
EXTERN S16 wrEmmCnmNlQmiUnInit (U16 cellId);
EXTERN S16 wrEmmCnmSendCancelReq (Void);
EXTERN S16 wrEmmCnmSendCellSyncReq  (WrEutranNeighCellCb *eutraNeghCell);
EXTERN void wrEmmCnmNlQmiIndCb ARGS((qmi_client_type, unsigned int, void *, unsigned int, void *));
EXTERN void wrEmmCnmNlQmiErrCb ARGS((qmi_client_type, qmi_client_error_type, void *));
EXTERN void wrEmmCnmNlQmiRspCb ARGS((qmi_client_type, unsigned int, void *, unsigned int, void *, qmi_client_error_type));


#endif
