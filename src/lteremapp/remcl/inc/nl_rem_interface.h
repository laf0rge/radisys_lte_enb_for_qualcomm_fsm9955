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
 *  nl_rem_interface.h
 *
 *  Created on: 18-Jul-2016
 *  Author: adhanne
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <sys/select.h>
#include <unistd.h>

#ifndef _NL_REM_INTF_
#define _NL_REM_INTF_


/* Infra headers */
#include "procPrivsWrapper.h"
/* QMI headers */
#include "qmi_client.h"
#include "qmi_idl_lib.h"

/* NL interface */
#include "nl_api_v01.h"
/*============================================================================

  INTERNAL DEFINITIONS AND TYPES

============================================================================*/

#define TRUE  1
#define FALSE 0

#define NL_REQ_TIMEOUT_SECS 10  /* Max timeout for NL QMI requests */ 

extern bool  g_firstQcomRssiRsp;   /* Variables to check the response */
extern bool  g_qcomCellSearchRsp;
extern bool  g_qcomMibReadRsp;
extern bool  g_qcomSib1ReadRsp;
extern bool  g_qcomAllSibReadRsp;
extern unsigned int   g_transID[10];
extern int sibRequest;
/*! Return type. */
typedef enum
{
   NL_FAILURE = -1,  /* value -1 failure 0 success */
   NL_SUCCESS = 0

} nl_ret_type;

/*============================================================================
STRUCT:  nl_ind_type
============================================================================*/
/*! @brief
  Indication message data type
 */
/*==========================================================================*/
/*! Indication message data type. */
typedef struct
{
   /* Message ID of the indication. */
   unsigned int               msg_id;

   /*! Message data. */
   union
   {
      nl_init_ind_msg_v01          init;         /* Indication Message pointers*/
      nl_freq_scan_ind_msg_v01     freq_scan;
      nl_cell_search_ind_msg_v01   cell_search;   
      nl_mib_read_ind_msg_v01      mib_read;
      nl_sib_sched_ind_msg_v01     sib_sched;
      nl_sib_read_ind_msg_v01      sib_read;
      nl_cancel_ind_msg_v01        nl_cancel;
   };

} nl_ind_type;

/*! NL_INIT_REQ_V01 message. */
extern nl_init_req_msg_v01 nl_init_req_msg;


/*============================================================================
STRUCT:  g_client
============================================================================*/
/*! @brief
  Application data structure.
 */
/*==========================================================================*/
typedef struct g_client
{
   /*! NL QMI client handle. */
   qmi_client_type            clnt_handle;

   /*! NL QMI client signal data. */
   qmi_client_os_params       os_params;

   /*! NL QMI client transaction handle. */
   qmi_txn_handle             txn_handle;

   /*! Storage for a response message received from NL. */
   nl_generic_rsp_msg_v01     rsp;

   /*! Storage for an indication message received from NL. */
   nl_ind_type                ind;

   /*! Semaphore. */
   sem_t                      req_sem;

} g_client;

extern g_client g_remclnt;

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
 );

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
 );

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
 );

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
 );

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
 );

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
 );

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
  
  unsigned int transid 
  
 );

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
  
 unsigned int transid
  
 );

 /*============================================================================
FUNCTION:  decodeQcomNlInitInd
============================================================================*/
 /*! @brief
   Decode received nl_init indication.

   @details
   This function is called by the rem_ind_cb to forward
   indication to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomNlInitInd(unsigned char status);

 /*============================================================================
FUNCTION:  decodeQcomRssiRsp
============================================================================*/
 /*! @brief
   Decode received Freq_scan response.

   @details
   This function is called by the rem_rsp_cb to send 
   received status to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomRssiRsp(unsigned char status);

 /*============================================================================
FUNCTION:  decodeQcomFreqScanInd
============================================================================*/
 /*! @brief
   Decode received Freq_scan indication.

   @details
   This function is called by the rem_ind_cb to forward
   indication to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomFreqScanInd(unsigned char rssi,unsigned short int usArFCN);

 /*============================================================================
FUNCTION:  decodeQcomRssiCnf
============================================================================*/
 /*! @brief
   Send received Rssi confirmation status.

   @details
   This function is called by the rem_ind_cb to send 
   confirmation status to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomRssiCnf(unsigned char status);

 /*============================================================================
FUNCTION:  decodeQcomCellSearchRsp
============================================================================*/
 /*! @brief
   Decode received Cell_search response.

   @details
   This function is called by the rem_rsp_cb to send 
   received status to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomCellSearchRsp(unsigned char status);


 /*============================================================================
FUNCTION:  decodeQcomCellSearchInd
============================================================================*/
 /*! @brief
   Decode received Cell_search indication.

   @details
   This function is called by the rem_ind_cb to forward
   indication to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomCellSearchInd(void);

 /*============================================================================
FUNCTION:  decodeQcomCellSearchCnf
============================================================================*/
 /*! @brief
   Send received Cell_search confirmation status.

   @details
   This function is called by the rem_ind_cb to send 
   confirmation status to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomCellSearchCnf(unsigned char status);

 /*============================================================================
FUNCTION:  decodeQcomMibReadInd
============================================================================*/
 /*! @brief
   Decode received Mib_read indication.

   @details
   This function is called by the rem_ind_cb to forward
   indication to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomMibReadInd(void);

 /*============================================================================
FUNCTION:  decodeQcomReadSib1Ind
============================================================================*/
 /*! @brief
   Decode received sib1_read indication.

   @details
   This function is called by the rem_ind_cb to forward
   indication to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomReadSib1Ind(void);

 /*============================================================================
FUNCTION:  decodeQcomSib1CfgCnf
============================================================================*/
 /*! @brief
   Send received Sib1 confirmation status.

   @details
   This function is called by the rem_ind_cb to send 
   confirmation status to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomSib1CfgCnf(unsigned char status);

 /*============================================================================
FUNCTION:  decodeQcomReadSibInd
============================================================================*/
 /*! @brief
   Decode received sib_read indication.

   @details
   This function is called by the rem_ind_cb to forward
   indication to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
 void decodeQcomReadSibInd(void);

 /*============================================================================
FUNCTION:  decodeQcomSibReadCfgCnf
============================================================================*/
 /*! @brief
   Send received Sib1 confirmation status.

   @details
   This function is called by the rem_ind_cb to send 
   confirmation status to rem_fsm.

   @return
   None.
  */
 /*==========================================================================*/
void decodeQcomSibReadCfgCnf(unsigned char status);
#endif
