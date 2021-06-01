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

     Type:     C source file

     Desc:     This file contains interface with Qualcom NL QMI Service

     File:     wr_emm_cnm_nlqmi.c

     Sid:      fc_emm_cnm_nlqmi.c@@/main/Br_Tenb_Brdcm_Bcm61750_Intg_GA2_0_TDD_GAFDD_final/2 - Mon Jun 30 15:48:26 2014

     Prg:      lmeledam

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=142;

/* header include files (.h) */
#include<stdio.h>
#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_emm.h"
#include "wr_smm_init.h"
#include "wr_emm_cnm.h"
/* Infra headers */
#include "procPrivsWrapper.h"
#include "wr_emm_cnm_nlqmi.h"

/* QMI CNM Cb */
WrCellCnmQmiCb wrCellCnmQmiCb;

CONSTANT WrCnmLteBandnfo cnmLteBandList[] =
{
   // BAND , NDL_Low, NDL_Max
   { 1, 0, 599 },
   { 2, 600, 1199 },
   { 3, 1200,1949 },
   { 4, 1950,2399 },
   { 5, 2400,2649 },
   { 6, 2650,2749 },
   { 7, 2750,3449 },
   { 8, 3450,3779 },
   { 9, 3800,4149 },
   { 10,4150,4749 },
   { 11,4750,4949 },
   { 12,5010,5179 },
   { 13,5180,5279 },
   { 14,5280,5379 },
   { 17,5730,5849 },
   { 18,5850,5999 },
   { 19,6000,6149 },
   { 20,6150,6449 },
   { 21,6450,6599 },
   { 22,6600,7399 },
   { 23,7500,7699 },
   { 24,7700,8039 },
   { 25,8040,8689 },
   { 26,8690,9039 },
   { 27,9040,9209 },
   { 28,9210,9659 },
   { 33,36000,36199},
   { 34,36200,36349},
   { 35,36350,36949},
   { 36,36950,37549},
   { 37,37550,37749},
   { 38,37750,38249},
   { 39,38250,38649},
   { 40,38650,39649},
   { 41,39650,41589},
   { 42,41590,43589},
   { 43,43590,45589},
   { 44,45590,46589},
   { 0, 0, 0 }
};

PRIVATE U8  wrEmmCnmGetLteBand(U16 );

/** @brief This utility function returns the band from the earfcn
 *
 * @details
 *
 *     Function: wrEmmCnmGetLteBand
 *
 *         Processing steps:
 *         - Compares the EARFCN against the upper and lower limits of band.
 *         - Return the band
 *
 * @param[in] earfcn : EARFCN
 * @return U8
 *     - Band
 */
PRIVATE U8  wrEmmCnmGetLteBand(U16 earfcn)
{
   U32 idx;
   for(idx=0; cnmLteBandList[idx].band !=0; idx++)
   {
      if((earfcn >= cnmLteBandList[idx].earfcnLow) && (earfcn <= cnmLteBandList[idx].earfcnMax))
      {
         RETVALUE (cnmLteBandList[idx].band);
      }
   }
   RETVALUE(0);
}



/** @brief This function is responsible for Initialization of the 
 *         NL-QMI interface for CNM
 *
 * @details
 *
 *     Function: wrEmmCnmNlQmiInit
 *
 *         Processing steps:
 *         - Allocate WrCellCnmQmiCb
 *         - Initialize the QMI interface 
 *         - send NL_INIT_REQ to QMI Interface
 *
 * @param[in]   cellId  : cell identifier
 * @return U16
 *     -# Success  : ROK
 *     -# Failure  : RFAILED
 */
PUBLIC S16 wrEmmCnmInitReq (U16 cellId)
{
   qmi_client_error_type      rc;
   nl_init_req_msg_v01 nl_init_req_msg;

   RLOG1(L_INFO, "NL_CNM:: Received INIT Req from Cell Id%u \n", cellId);

   wrCellCnmQmiCb.cellId = cellId;

   /* Wait for the NL QMI Service to come up. Once it is available, initialize
      the client and connect to the service. */
   rc = qmi_client_init_instance( nl_get_service_object_v01(),
            QMI_CLIENT_INSTANCE_ANY,
            wrEmmCnmNlQmiIndCb,
            (void *) NULLP,
            &wrCellCnmQmiCb.os_params,
            NL_REQ_TIMEOUT_SECS,
            &wrCellCnmQmiCb.clnt_handle);

   /* Check that the init and connection occurred successfully... */
   if ( rc != QMI_NO_ERR )
   {
      RLOG0(L_ERROR, "NL_CNM::ERROR: Client init failed!\n" );
      RETVALUE(RFAILED);
   }
   else
   {
      /* Register client error callback */
      rc = qmi_client_register_error_cb( wrCellCnmQmiCb.clnt_handle, wrEmmCnmNlQmiErrCb, (void* )NULLP);

      /* Check that the callback registration occurred successfully */
      if ( rc != QMI_NO_ERR )
      {
         RLOG0(L_ERROR, "NL_CNM::ERROR: Failed to register err_cb!\n" );
         RETVALUE(RFAILED);
      }
   }

   nl_init_req_msg.config_valid = FALSE;
   nl_init_req_msg.config = 0;

   /* Send the message */
   rc = qmi_client_send_msg_async(  wrCellCnmQmiCb.clnt_handle,
         NL_INIT_REQ_V01,
         &nl_init_req_msg,
         sizeof( nl_init_req_msg_v01),
         &wrCellCnmQmiCb.rsp,
         sizeof( wrCellCnmQmiCb.rsp ),
         wrEmmCnmNlQmiRspCb,
         (void *)NULLP,
         &wrCellCnmQmiCb.txn_handle);

   /* Check for any errors in the msg send */
   if ( rc != QMI_NO_ERR )
   {
      RLOG1(L_ERROR, "NL_CNM::ERROR: Failed to send async msg! error: %d\n", rc );
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Un-Initialization of the 
 *         NL-QMI interface for CNM
 *
 * @details
 *
 *     Function: wrEmmCnmNlQmiUnInit
 *
 *         Processing steps:
 *
 * @param[in]   cellId  : cell identifier
 * @return U16
 *     -# Success  : ROK
 *     -# Failure  : RFAILED
 */
PUBLIC S16 wrEmmCnmNlQmiUnInit (U16 cellId)
{
   qmi_client_error_type      rc;

   RLOG1(L_INFO, "NL_CNM:: Received UN-INIT Req from Cell Id%u \n", cellId);

  /* Disconnect from the NL Service and cleanup the NL QMI client */
  rc = qmi_client_release( wrCellCnmQmiCb.clnt_handle );
  if (QMI_NO_ERR != rc ) 
  {
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}




/** @brief This function handles the response for messages from QMI
 *
 * @details
 *   This callback function is called by the QMI client infrastructure when it
 *   receives an asynchronous response for the client.
 *
 *     Function: wrEmmCnmNlQmiRspCb
 *
 *         Processing steps:
 *         - call the enbapp function corresponding to the Message Id 
 *
 * @param [in] clnt_handle :  Opaque handle used by the infrastructure to identify different services.
 * @param [in] msg_id      :  Message ID of the response.
 * @param [in] buf         :  Buffer holding the decoded response.
 * @param [in] len         :  Length of the decoded response.
 * @param [in] resp_cb_data:  Cookie value supplied by the client.
 * @param [in] transp_err  :  Error value.
 * @return Void
 */
PUBLIC void wrEmmCnmNlQmiRspCb (
   qmi_client_type        clnt_handle, 
   unsigned int           msg_id,      
   void *                 buf,         
   unsigned int           len,         
   void *                 resp_cb_data,
   qmi_client_error_type  transp_err)
{
   nl_generic_rsp_msg_v01 *genRsp = ( nl_generic_rsp_msg_v01 * ) buf ;
   U16 cellId;

   RLOG2(L_INFO, "NL_CNM:: Received QMI-RSP msgId %d status %u\n", msg_id,genRsp->op_rsp.status );

   cellId = wrCellCnmQmiCb.cellId;

   /* Set pointer to the message specific storage */
   switch ( msg_id )
   {
      case NL_INIT_RSP_V01:
         if ( NL_ERR_NONE_V01 != genRsp->op_rsp.status  )
         {
            /* Sent the failure to Application */
            wrEmmCnmPrcCnmInitResp(cellId, RFAILED);
         }
         break;
      case NL_CELL_SYNC_RSP_V01:
         wrCellCnmQmiCb.qmiSyncTransId = genRsp->op_rsp.trans_id;

         if ( NL_ERR_NONE_V01 != genRsp->op_rsp.status  )
         {
            /* Sent the failure to Application */
            wrEmmCnmPrcSyncRspQmiIntf(cellId, RFAILED);
         }
         else
         {
            wrEmmCnmPrcSyncRspQmiIntf(cellId, ROK);
         }
         break;
      case NL_CANCEL_RSP_V01:
            RLOG0(L_INFO, "NL_CNM: NL_CANCEL_RSP_V01 ");
      break;
      default:
         RLOG1(L_ERROR, "NL_CNM::ERROR: Unhandled Response ! %d\n", msg_id );
         break;
   } /* switch */
} /* wrEmmCnmNlQmiRspCb */

/** @brief This function handles the Indication from QMI
 *
 * @details
 *  This callback function is called by the QMI client infrastructure when it
 *  receives an indication for the client.
 *
 *     Function: wrEmmCnmNlQmiIndCb
 *
 *         Processing steps:
 *         - call the enbapp function corresponding to the Message Id 
 *
 * @param [in]   clnt_handle, : Opaque handle used by the infrastructure to identify different services.
 * @param [in]   msg_id,      : Message ID of the indication.
 * @param [in]   ind_buf,     : Buffer holding the encoded indication.
 * @param [in]   ind_buf_len, : Length of the encoded indication.
 * @param [in]   ind_cb_data  : Cookie value supplied by the client during registration.
 * @return Void
 */
PUBLIC void wrEmmCnmNlQmiIndCb(
   qmi_client_type            clnt_handle, /*! Opaque handle used by the infrastructure to identify different services. */
   unsigned int               msg_id,      /*! Message ID of the indication. */
   void *                     ind_buf,     /*! Buffer holding the encoded indication. */
   unsigned int               ind_buf_len, /*! Length of the encoded indication. */
   void *                     ind_cb_data  /*! Cookie value supplied by the client during registration. */)
{
   U16 cellId;

   qmi_client_error_type      rc;
   U8 status;
   nl_init_ind_msg_v01 initInd;
   nl_cell_sync_ind_msg_v01 cellSyncInd;

   cellId = wrCellCnmQmiCb.cellId;

   /* Set pointer to the message specific storage */
   switch ( msg_id )
   {
      case NL_INIT_IND_V01:
         rc = qmi_client_message_decode( clnt_handle,
               QMI_IDL_INDICATION,
               NL_INIT_IND_V01,
               ind_buf,
               ind_buf_len,
               &initInd,
               sizeof(nl_init_ind_msg_v01));
         if ( rc == QMI_NO_ERR )
         {
            RLOG1(L_INFO, "NL_CNM::Received indication %d \n", msg_id);
            if(NL_ERR_NONE_V01 == initInd.op_rsp.status)
            {
               wrEmmCnmPrcCnmInitResp(cellId, ROK);
            }
            else
            {
               wrEmmCnmPrcCnmInitResp(cellId, RFAILED);
            }
         }
         else
         {
            RLOG2(L_ERROR, "NL_CNM::ERROR: Decode failure! msg_id %d , error %d\n",
                  msg_id, rc );
         }

         break;
      case NL_CELL_SYNC_IND_V01:
      {
         rc = qmi_client_message_decode( clnt_handle,
               QMI_IDL_INDICATION,
               NL_CELL_SYNC_IND_V01,
               ind_buf,
               ind_buf_len,
               &cellSyncInd,
               sizeof(nl_cell_sync_ind_msg_v01));
         if ( rc == QMI_NO_ERR )
         {
            RLOG1(L_INFO, "NL_CNM::Received indication %d\n", msg_id );
            if(NL_ERR_NONE_V01 == cellSyncInd.op_rsp.status)
            {
               status = ROK;
            }
            else
            {
               status = RFAILED;
            }
            wrEmmCnmPrcSyncIndQmiIntf (cellId, status, 
                  cellSyncInd.carrier.chan, 
                  cellSyncInd.sync_info_lte.pwr_meas.pci , 
                  cellSyncInd.sync_info_lte.pwr_meas.rsrp, 
                  cellSyncInd.pwr_params.bw);
         }
         else
         {
            RLOG2(L_ERROR, "NL_CNM::ERROR: Decode failure! msg_id %d , error %d\n",
                  msg_id, rc );
         }
         break;
      }
      case NL_CANCEL_IND_V01:
         RLOG0(L_INFO, "NL_CNM: NL_CANCEL_IND_V01 ");
      break;
      default:
         RLOG1(L_ERROR, "NL_CNM::ERROR: Unhandled indication! %d\n", msg_id );
         break;
   } /* switch */
} /* wrEmmCnmNlQmiIndCb */

/** @brief This function handles the Indication from QMI
 *
 * @details
 *  This callback is invoked if the service encounters any problems or goes
 *  down entirely.
 *
 *     Function: wrEmmCnmNlQmiErrCb
 *
 *         Processing steps:
 *         - Log the error
 *
 * @param [in] clnt_handle : Opaque handle used by the infrastructure to identify different services.
 * @param [in] error       : Type of error which has occurred.
 * @param [in] data        : Data associated with the error.
 * @return Void
 */

PUBLIC void  wrEmmCnmNlQmiErrCb
(
   qmi_client_type            clnt_handle, /*! Opaque handle used by the infrastructure to identify different services. */
   qmi_client_error_type      error,       /*! Type of error which has occurred. */
   void *                     data         /*! Data associated with the error. */
)
{
  RLOG0(L_ERROR, "NL_CNM::ERROR: Client err_cb invoked!\n" );
} /* wrEmmCnmNlQmiErrCb */


/** @brief This function is responsible for initiating NL_CELL_SYNC_REQ towards QMI
 *
 * @details
 *
 *     Function: wrEmmCnmSendCellSyncReq
 *
 *         Processing steps:
 *         - Fill the QMI interface structure 
 *         - Send Sync Req (NL_CELL_SYNC_REQ)
 *
 * @param [in] pCnmQmiCb    : CB to store QMI interface information
 * @param [in] eutraNeghCell: Neighbor cell Info to Sync
 * @return U16
 *     -# Success  : ROK
 *     -# Failure  : RFAILED
 */
PUBLIC S16 wrEmmCnmSendCellSyncReq  (WrEutranNeighCellCb *eutraNeghCell)
{
   nl_cell_sync_req_msg_v01 nlCellSyncReq;
   qmi_client_error_type rc;
   U16 ret = ROK;
   RLOG0(L_INFO, "NL_CNM:: Received CNM Sync Req \n");

   eutraNeghCell->isCnmPerformed = TRUE;

   nlCellSyncReq.carrier.rat  = NL_RAT_LTE_V01 ;
   nlCellSyncReq.carrier.band = wrEmmCnmGetLteBand (eutraNeghCell->earfcnDl);
   nlCellSyncReq.carrier.chan = eutraNeghCell->earfcnDl;


   QMI_FILL_DL_BW(eutraNeghCell->dlBw, nlCellSyncReq.pwr_params.bw);
   nlCellSyncReq.pwr_params.meas_period = NL_MEAS_PERIOD;

   nlCellSyncReq.cell_id             = eutraNeghCell->pci;

   nlCellSyncReq.sync_mode_valid     = TRUE;
   nlCellSyncReq.sync_mode = NL_SYNC_MODE_PHASE_V01;

   nlCellSyncReq.sync_type_valid     =TRUE;
   nlCellSyncReq.sync_type = NL_SYNC_TYPE_NO_OTA_V01;

   /* All optional TLVs set to FALSE */
   nlCellSyncReq.gps_time_info_valid = FALSE;
   nlCellSyncReq.start_time_valid    = FALSE;
   nlCellSyncReq.priority_valid      = FALSE;
   nlCellSyncReq.sync_interval_valid = FALSE;
   nlCellSyncReq.phase_offset_valid  = FALSE;
   RLOG1(L_INFO, "NL_CNM:: Sync Req Band: %u \n",nlCellSyncReq.carrier.band);
   RLOG1(L_INFO, "NL_CNM:: Sync Req Chann: %u \n",nlCellSyncReq.carrier.chan);
   RLOG1(L_INFO, "NL_CNM:: Sync Req BW : %u \n",nlCellSyncReq.pwr_params.bw)
   RLOG1(L_INFO, "NL_CNM:: Sync Req measp: %u \n",nlCellSyncReq.pwr_params.meas_period);
   RLOG1(L_INFO, "NL_CNM:: Sync Req pci: %u \n",nlCellSyncReq.cell_id);
   

   /* Send the message */
   rc = qmi_client_send_msg_async(  wrCellCnmQmiCb.clnt_handle,
         NL_CELL_SYNC_REQ_V01,
         &nlCellSyncReq,
         sizeof( nl_cell_sync_req_msg_v01),
         &wrCellCnmQmiCb.rsp,
         sizeof( wrCellCnmQmiCb.rsp ),
         wrEmmCnmNlQmiRspCb,
         (void *) NULLP,
         &wrCellCnmQmiCb.txn_handle);

   /* Check for any errors in the msg send */
   if ( rc != QMI_NO_ERR )
   {
      RLOG1(L_ERROR, "NL_CNM::ERROR: Failed to send async msg! error: %d\n", rc );
      ret = RFAILED;
   }
   RETVALUE(ret);
}

/** @brief This function is responsible for Cancelling the Cell-Sync-Req towards QMI
 *
 * @details
 *
 *     Function: wrEmmCnmSendCancelReq
 *
 *         Processing steps:
 *         - Fill the QMI interface structure 
 *         - Send Sync Req (NL_CELL_SYNC_REQ)
 *
 * @param [in] pCnmQmiCb    : CB to store QMI interface information
 * @param [in] eutraNeghCell: Neighbor cell Info to Sync
 * @return U16
 *     -# Success  : ROK
 *     -# Failure  : RFAILED
 */
PUBLIC S16 wrEmmCnmSendCancelReq (Void)
{
   nl_cancel_req_msg_v01 nlCancelReq;
   qmi_client_error_type rc;
   U16 ret = ROK;
   RLOG0(L_INFO, "NL_CNM:: Received CNM Cancel Sync Req \n");

   nlCancelReq.trans_id_to_cancel = wrCellCnmQmiCb.qmiSyncTransId;
   nlCancelReq.priority_valid      = FALSE;

   /* Send the message */
   rc = qmi_client_send_msg_async(  wrCellCnmQmiCb.clnt_handle,
         NL_CANCEL_REQ_V01,
         &nlCancelReq,
         sizeof( nl_cancel_req_msg_v01),
         &wrCellCnmQmiCb.rsp,
         sizeof( wrCellCnmQmiCb.rsp ),
         wrEmmCnmNlQmiRspCb,
         (void *) NULLP,
         &wrCellCnmQmiCb.txn_handle);

   /* Check for any errors in the msg send */
   if ( rc != QMI_NO_ERR )
   {
      RLOG1(L_ERROR, "NL_CNM::ERROR: Failed to send Cancel Req  error: %d\n", rc );
      ret = RFAILED;
   }
   RETVALUE(ret);
}

/********************************************************************30**

           End of file:     fc_emm_cnm.c@@/main/Br_Tenb_Brdcm_Bcm61750_Intg_GA2_0_TDD_GAFDD_final/2 - Mon Jun 30 15:48:26 2014

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

