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

/**************************************************************************************************

     Name:     REM-APP

     Type:     C Source file

     Desc:     This file consists of LTE related functions for sending messages to Qcom

     File:     nl_rem_interface.c

     Prg:      Anjali D

**************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////
//   Local includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "nl_rem_interface.h"
unsigned int g_transID[10]   = {0}; /*Global array for Transaction Id's*/
unsigned int g_transIdIdx    = 0;/*Index for global array*/
extern                g_client g_remclnt;

/*============================================================================
FUNCTION:  rem_nl_qmi_clnt_create
============================================================================*/
/*! @details
  Provides a generic wrapper for creating a client.

  @return
  NL_SUCCESS if successful, NL_FAILURE otherwise.
 */
/*==========================================================================*/
nl_ret_type  rem_nl_qmi_clnt_create
(
 /*! Service object. */
 qmi_idl_service_object_type  service_object,

 /*! Pointer to the handle of the client being created. */
 qmi_client_type *          clnt_handle,

 /*! Pointer to the signal data of the client being created. */
 qmi_client_os_params *     os_params,

 /*! Function to invoke upon reception of an indication message. */
 qmi_client_ind_cb          ind_cb,

 /*! Function to invoke upon the occurrance of an error. */
 qmi_client_error_cb        err_cb
 )
{
   qmi_client_error_type      rc;
   nl_ret_type                ret;

   if ( clnt_handle == NULL || os_params == NULL )
   {
      printf( "NL_REM::ERROR: Invalid params!\n" );
      return NL_FAILURE;
   }

   /* Wait for the NL QMI Service to come up. Once it is available, initialize
      the client and connect to the service. */
   rc = qmi_client_init_instance( service_object,
         QMI_CLIENT_INSTANCE_ANY,
         rem_ind_cb,
         NULL,
         os_params,
         NL_REQ_TIMEOUT_SECS,
         clnt_handle );

   /* Check that the init and connection occurred successfully... */
   if ( rc != QMI_NO_ERR )
   {
      printf( "NL_REM::ERROR: Client init failed!\n" );
      ret = NL_FAILURE;
   }
   else
   {
      /* Register client error callback */
      rc = qmi_client_register_error_cb( *clnt_handle, rem_err_cb, NULL );

      /* Check that the callback registration occurred successfully */
      if ( rc != QMI_NO_ERR )
      {
         printf( "NL_REM::ERROR: Failed to register err_cb!\n" );
         ret = NL_FAILURE;
      }
      else
      {
         ret = NL_SUCCESS;
      }
   }

   return ret;

} /* nl_qmi_clnt_create */

/*============================================================================
FUNCTION:  rem_nl_qmi_clnt_send_req
============================================================================*/
/*! @details
  Provides a generic wrapper for sending an asynchronous request message.

  @return
  NL_SUCCESS if successful, NL_FAILURE otherwise.
 */
/*==========================================================================*/
nl_ret_type  rem_nl_qmi_clnt_send_req
(
 /*! Opaque handle used by the infrastructure to identify different services. */
 qmi_client_type            clnt_handle,

 /*! Message ID of the request. */
 unsigned int               msg_id,

 /*! Message name string used for logging purposes. */
 const char *               msg_str,

 /*! Pointer to the request data structure. */
 void *                     req_ptr,

 /*! Size of the request. */
 unsigned int               req_len,

 /*! Pointer to the response data structure. */
 void *                     rsp_ptr,

 /*! Size of the response. */
 unsigned int               rsp_len,

 /*! Response callback function. */
 qmi_client_recv_msg_async_cb  rsp_cb_func,

 /*! Data passed into the response callback. */
 void *                     rsp_cb_data,

 /*! Transmission handle. */
 qmi_txn_handle *           txn_handle
 )
{
   qmi_client_error_type      rc = QMI_INTERNAL_ERR;
   nl_ret_type                ret;

   if ( clnt_handle == NULL || msg_str == NULL || rsp_ptr == NULL ||
         txn_handle == NULL )
   {
      printf( "NL_REM::ERROR: Invalid params!\n" );
      return NL_FAILURE;
   }

   /* Note: Intentionally refrain from NULL checking the req_ptr as the request
      message may be empty. */

   printf( "NL_REM::Sending async msg %d (%s)...\n", msg_id, msg_str );

   /* Send the message */
   rc = qmi_client_send_msg_async( clnt_handle,
         msg_id,
         req_ptr,
         req_len,
         rsp_ptr,
         rsp_len,
         rsp_cb_func,
         rsp_cb_data,
         txn_handle );

   /* Check for any errors in the msg send */
   if ( rc != QMI_NO_ERR )
   {
      printf( "NL_REM::ERROR: Failed to send async msg! error: %d\n", rc );
      ret = NL_FAILURE;
   }
   else
   {
      ret = NL_SUCCESS;
   }

   return ret;

} /* rem_nl_qmi_clnt_send_req */

/*============================================================================
FUNCTION:  rem_nl_qmi_clnt_release
============================================================================*/
/*! @details
  Provides a generic wrapper for releasing a client.

  @return
  NL_SUCCESS if successful, NL_FAILURE otherwise.
 */
/*==========================================================================*/
nl_ret_type  rem_nl_qmi_clnt_release
(
 /*! Handle of the client that should be released. */
 qmi_client_type            clnt_handle
 )
{
   qmi_client_error_type      rc;
   nl_ret_type                ret;

   if ( clnt_handle == NULL )
   {
      printf( "NL_REM::ERROR: Invalid params!\n" );
      return NL_FAILURE;
   }

   /* Release the client */
   rc = qmi_client_release( clnt_handle );

   /* Check that the release occurred successfully... */
   if ( rc != QMI_NO_ERR )
   {
      printf( "NL_REM::ERROR: Client release failed!\n" );
      ret = NL_FAILURE;
   }
   else
   {
      ret = NL_SUCCESS;
   }

   return ret;

} /* rem_nl_qmi_clnt_release */

/*============================================================================
  CALLBACK FUNCTION:  rem_ind_cb
  ============================================================================*/
/*! @brief
  Client indication callback.

  @details
  This callback function is called by the QMI client infrastructure when it
  receives an indication for the client.

  @return
  None.
 */
/*==========================================================================*/
void  rem_ind_cb
(
 /*! Opaque handle used by the infrastructure to identify different services. */
 qmi_client_type            clnt_handle,

 /*! Message ID of the indication. */
 unsigned int               msg_id,

 /*! Buffer holding the encoded indication. */
 void *                     ind_buf,

 /*! Length of the encoded indication. */
 unsigned int               ind_buf_len,

 /*! Cookie value supplied by the client during registration. */
 void *                     ind_cb_data
 )
{
   qmi_client_error_type   rc;
   void *                  ind_msg_ptr;
   unsigned int            ind_msg_len;
   char *                  msg_str;
   int                     i;
   unsigned char status =  0;

   /* Set pointer to the message specific storage */
   switch ( msg_id )
   {
      case NL_INIT_IND_V01:
         msg_str = "nl_init_ind";
         ind_msg_ptr = &g_remclnt.ind.init;
         ind_msg_len = sizeof( g_remclnt.ind.init );
         break;

      case NL_FREQ_SCAN_IND_V01:
         msg_str = "nl_freq_scan_ind";
         ind_msg_ptr = &g_remclnt.ind.freq_scan;
         ind_msg_len = sizeof( g_remclnt.ind.freq_scan );
         break;

      case NL_CELL_SEARCH_IND_V01:
         msg_str = "nl_cell_search_ind";
         ind_msg_ptr = &g_remclnt.ind.cell_search;
         ind_msg_len = sizeof( g_remclnt.ind.cell_search );
         break;

      case NL_MIB_READ_IND_V01:
         msg_str = "nl_mib_read_ind";
         ind_msg_ptr = &g_remclnt.ind.mib_read;
         ind_msg_len = sizeof( g_remclnt.ind.mib_read );
         break;

      case NL_SIB_SCHED_IND_V01:
         msg_str = "nl_sib_sched_ind";
         ind_msg_ptr = &g_remclnt.ind.sib_sched;
         ind_msg_len = sizeof( g_remclnt.ind.sib_sched );
         break;

      case NL_SIB_READ_IND_V01:
         msg_str = "nl_sib_read_ind";
         ind_msg_ptr = &g_remclnt.ind.sib_read;
         ind_msg_len = sizeof( g_remclnt.ind.sib_read );
         break;

      case NL_CANCEL_IND_V01:
          msg_str = "nl_cancel_ind";
          ind_msg_ptr = &g_remclnt.ind.nl_cancel;
          ind_msg_len = sizeof(g_remclnt.ind.nl_cancel);
          break;

      default:
         printf( "NL_REM::ERROR: Unhandled indication! %d\n", msg_id );
         msg_str = "unhandled_ind";
         ind_msg_ptr = NULL;
         ind_msg_len = 0;
         break;
   } /* switch */

   /* Ensure we have a valid message to decode */
   if ( ind_msg_ptr != NULL )
   {
      /* Decode the indication message */
      rc = qmi_client_message_decode( clnt_handle,
            QMI_IDL_INDICATION,
            msg_id,
            ind_buf,
            ind_buf_len,
            ind_msg_ptr,
            ind_msg_len );
      if ( rc == QMI_NO_ERR )
      {
         printf( "NL_REM::Received indication %d (%s)\n", msg_id, msg_str );
      }
      else
      {
         printf( "NL_REM::ERROR: Decode failure! msg_id %d (%s), error %d\n",
               msg_id, msg_str, rc );
      }
      switch ( msg_id )
      {
         case NL_INIT_IND_V01:
            updateTransidArray(g_remclnt.ind.init.op_rsp.trans_id);
            msg_str = "nl_init_ind";
            status=0;
            decodeQcomNlInitInd(status);
            break;

         case NL_FREQ_SCAN_IND_V01:
            updateTransidArray(g_remclnt.ind.freq_scan.op_rsp.trans_id);
            msg_str = "nl_freq_scan_ind";
            for(i=0;i<((&g_remclnt.ind.freq_scan)->chan_cnt);i++)
            {
               decodeQcomFreqScanInd((&g_remclnt.ind.freq_scan)->rssi_meas[i].rssi,
                     (&g_remclnt.ind.freq_scan)->rssi_meas[i].chan);
            }
            /*This is the last rssi indication, reset all gloabal parameters */
            /* Send Confirmation to REM Application */
            status = 0;
            decodeQcomRssiCnf(status);
            break;

         case NL_CELL_SEARCH_IND_V01:
             updateTransidArray(g_remclnt.ind.cell_search.op_rsp.trans_id);
             msg_str = "nl_cell_search_ind";
             decodeQcomCellSearchInd();
             decodeQcomCellSearchCnf(status);
             break;

         case NL_MIB_READ_IND_V01:
             updateTransidArray(g_remclnt.ind.mib_read.op_rsp.trans_id);
             msg_str = "nl_mib_read_ind";
             decodeQcomMibReadInd();
             break;

         case NL_SIB_SCHED_IND_V01:
            updateTransidArray(g_remclnt.ind.sib_sched.op_rsp.trans_id);
            msg_str = "nl_sib_sched_ind";
            decodeQcomReadSib1Ind();
            break;

         case NL_SIB_READ_IND_V01:
            updateTransidArray(g_remclnt.ind.sib_read.op_rsp.trans_id);
            msg_str = "nl_sib_read_ind";
            decodeQcomReadSibInd();
            break;

         case NL_CANCEL_IND_V01:
            msg_str = "nl_cancel_ind";
            printf("The canceled transID is:%d\n",g_remclnt.ind.nl_cancel.canceled_trans_id);
            printf("TRANSID FOR NL_CANCEL is:%d\n",g_remclnt.ind.nl_cancel.op_rsp.trans_id );
            break;

         default:
            printf( "NL_REM::ERROR: Unhandled indication! %d\n", msg_id );
            msg_str = "unhandled_ind";
            ind_msg_ptr = NULL;
            ind_msg_len = 0;
            break;
      } /* switch case for handling decoding of messages*/
   }

   /* Notify the application that the request is complete and the client has
      received the indication message */
   sem_post( &g_remclnt.req_sem );

} /* rem_ind_cb */



/*============================================================================
  CALLBACK FUNCTION:  rem_err_cb
  ============================================================================*/
/*! @brief
  Client error callback.

  @details
  This callback is invoked if the service encounters any problems or goes
  down entirely.

  @return
  None.
 */
/*==========================================================================*/
void  rem_err_cb
(
 /*! Opaque handle used by the infrastructure to identify different services. */
 qmi_client_type            clnt_handle,

 /*! Type of error which has occurred. */
 qmi_client_error_type      error,

 /*! Data associated with the error. */
 void *                     data
 )
{

   printf( "NL_REM::ERROR: Client err_cb invoked!\n" );

} /* rem_err_cb */



/*============================================================================
    FUNCTION:  fillTransidArray
      ============================================================================*/
/*! @brief
   Filling transaction Id array

   @details
   This function called by rem_rsp_cb to fill global array for transaction Id's   

    @return
     None.
               */
/*==========================================================================*/


void fillTransidArray
(
/*Input transaction Id*/
 unsigned int transID
 )
{
   g_transID[g_transIdIdx]= transID;
   printf("TRANSID IN ARRAY[%d] IS %d\n",
            g_transIdIdx, g_transID[g_transIdIdx]);
   g_transIdIdx++;

}/*fillTransidArray*/

/*============================================================================
      FUNCTION:  updateTransidArray
            ============================================================================*/
/*! @brief
     Updating transaction Id array

     @details
     This function called by rem_ind_cb to update global array for transaction Id's

     @return
      None.
                                   */
/*==========================================================================*/


void updateTransidArray
(
 /*Input Transaction Id*/
 unsigned int transid
)
{

   unsigned int index = 0;

   for(index = 0;index<10;index++)
   {
      if(g_transID[index])
           printf("TRANSID IN ARRAY[%d] IS %d\n",
               index, g_transID[index]);
   }

   for(index = 0;index<10;index++)
   {
       
      if(transid == g_transID[index])
      {
         g_transID[index] = 0;
         g_transIdIdx--;
         printf("TRANSID Deleted IN ARRAY[%d] IS Remaining %d\n",
               index, g_transIdIdx);
         break;
      }

   }

}/*updateTransidArray*/


/*============================================================================
  CALLBACK FUNCTION:  rem_rsp_cb
  ============================================================================*/
/*! @brief
  Client response callback.

  @details
  This callback function is called by the QMI client infrastructure when it
  receives an asynchronous response for the client.

  @return
  None.
 */
/*==========================================================================*/
void  rem_rsp_cb
(
 /*! Opaque handle used by the infrastructure to identify different services. */
 qmi_client_type            clnt_handle,

 /*! Message ID of the response. */
 unsigned int               msg_id,

 /*! Buffer holding the decoded response. */
 void *                     buf,

 /*! Length of the decoded response. */
 unsigned int               len,

 /*! Cookie value supplied by the client. */
 void *                     resp_cb_data,

 /*! Error value. */
 qmi_client_error_type      transp_err
 )
{
   /* Sending status to REM-App */
   unsigned char status = 0;
   unsigned int transId = 0;
   /* Copy the response message */
   g_remclnt.rsp = *( ( nl_generic_rsp_msg_v01 * ) buf );
  
   /* Check the response status */
   if ( g_remclnt.rsp.op_rsp.status == NL_ERR_NONE_V01 )
   {
      transId = g_remclnt.rsp.op_rsp.trans_id;

      fillTransidArray(transId);
      printf( "NL_REM::Received response %d : status=%d\n",
            msg_id,
            g_remclnt.rsp.op_rsp.status );
      if(g_firstQcomRssiRsp)
      {
         //send Rssi response to REM
         decodeQcomRssiRsp(status);
         g_firstQcomRssiRsp = FALSE;
      }
      if(g_qcomCellSearchRsp)
      {
         decodeQcomCellSearchRsp(status);
         g_qcomCellSearchRsp = FALSE;
      }
      if(g_qcomSib1ReadRsp)
      {
         decodeQcomSib1CfgCnf(status);
         g_qcomSib1ReadRsp = FALSE;
      }
      if(g_qcomAllSibReadRsp)
      {
         decodeQcomSibReadCfgCnf(status);
         g_qcomAllSibReadRsp = FALSE;
      }
      /* Received positive acknowledgement */
   }
   else
   {
      if(g_firstQcomRssiRsp)
      {
         /* fill status as band not supported */
         status = 0x83;
         /* Received an error status code */
         //send Rssi response to REM
         decodeQcomRssiRsp(status);
      }
      printf( "NL_REM::ERROR: Received response %d : status=%d\n",
            msg_id,
            g_remclnt.rsp.op_rsp.status );

      /* As the request was rejected, there will be no subsequent indication
         message sent to the client. Notify the application accordingly. */
      sem_post( &g_remclnt.req_sem );
   }
} /* rem_rsp_cb */

