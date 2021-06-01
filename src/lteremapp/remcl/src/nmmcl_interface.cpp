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

     Type:     C++ Source file

     Desc:     This file consists of interface related functions 
               need to add interface to Qcom NL

     File:     nmmcl_interface.cpp

     Prg:      Anjali D

**************************************************************************************************/

#include "nmmcl_interface.h"
#include "rem_datatype.h"
#include "rem_common.h"
#include "LtePhyApi.h"
extern U8 g_bStartFlag;
#ifdef __cplusplus
extern "C" {
#include "nl_rem_interface.h"
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// Class: CNmmPhyInterface
// Desc : Program entry point, storage for system wide data, etc.
///////////////////////////////////////////////////////////////////////////////////////////////////
CNmmPhyInterface::CNmmPhyInterface()
{
   //NMM_START
   m_pNmmMsg = NULL ;
   m_bExpressScan = 0;
   m_bMeasurementPeriod = 100; 

   m_pstNmmHdr = (CPHY_MSG_HEADER *) m_txBuff;
   m_pNmmStartMsg = (U8*)( m_txBuff + sizeof(CPHY_MSG_HEADER));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @Function : skipPbchMsg
//	  @param[in]: 
//    This funtions is used to get whether pbch message is required
//    to send
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CNmmPhyInterface::skipPbchMsg()
{
   return(0);
}

nl_cancel_req_msg_v01  nl_cancel_req;

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmStop
//	  @param[in]: void
//     This funtions is used to send stop NMM message.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmPhyInterface::sendNmmStop()
{
 
    unsigned int index = 0;
    for(index = 0;index<10;index++ )
    {   
   
       if(g_transID[index])
       {
           memset(&nl_cancel_req, 0, sizeof(nl_cancel_req_msg_v01));
           nl_cancel_req.trans_id_to_cancel = g_transID[index];

           rem_nl_qmi_clnt_send_req(g_remclnt.clnt_handle,
                                    NL_CANCEL_REQ_V01,
                                    "nl_cancel_req",
                                    &nl_cancel_req,
                                    sizeof( nl_cancel_req),
                                    &g_remclnt.rsp,
                                    sizeof( g_remclnt.rsp ),
                                    rem_rsp_cb,
                                    NULL,
                                    &g_remclnt.txn_handle );

           updateTransidArray(g_transID[index]);
       }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmStop
//	  @param[in]: void
//     This funtions is used to send stop NMM message.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmPhyInterface::sendNmmAbort()
{
}
