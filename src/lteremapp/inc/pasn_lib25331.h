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

     Name:     UMTS REM - PASN-LIB25331

     Type:     Header file

     Desc:     This is header file for pasn_lib25331.cpp

     File:     pasn_lib25331.h

     Sid:      pasn_lib25331.h@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:18 2016

     Prg:      Rohit Kumar 


**************************************************************************************************/


#ifndef __PASN_LIB25331_H_
#define __PASN_LIB25331_H_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

//#include <boost/shared_ptr.hpp>
//#include <3waytypes.h>
//#include <system/Trace.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "ss_err.h"        /* errors */
#include "ss_queue.h"      /* queues */
#include "ss_task.h"       /* tasking */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "gen.x"
#include "ssi.x"           /* system services */
#include "cm_mblk.x"
#include "cm_mblk.h"
#include "cm_umts.h"
#include "cm_tkns.x"
#include "rru_asn.x"
#include "rru.h"
#include "rr_env.h"
#include "rru.x"
#include "rem_common.h"

#ifdef ARGS
#undef ARGS
#define ARGS(m) m
#endif

namespace threeway
{

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////

typedef struct _rrAllSdus
{
   CmMemListCp      memCp;
   union
   {
      RruBCCH_BCH_Msg   dlBcchBchMsg;   //added for BCH decoding   
      RruSysInfoBlock               sibMsg;
      RruUL_CCCH_Msg              ulCcchMsg;     
      RruUL_DCCH_Msg               ulDcchMsg;     
      RruBCCH_FACH_Msg             dlBcchFachMsg; 
      RruDlPcchMsg                 dlPcchMsg;     
      RruDL_CCCH_Msg                dlCcchMsg;     
      RruDL_DCCH_Msg                dlDcchMsg;     
      RruMCCH_Msg                   mcchMsg;
      RruMSCH_Msg                   mschMsg;
      RruHOToUTRANCmd               hoToUTRANCmd;
      RruTrgtRNC_ToSourcRNC_Conta   trncToSrncCont;
      RruToTrgtRNC_Conta            srncToTrncCont;
      
   } m;

} RrAllSdus;


typedef struct _rrMsgParams
{
   RrAllSdus        *sdu;
} RrMsgParams;


//////////////////////////////////////////////////////////////////
//   CPASN_LIB25331 Class Declaration
//////////////////////////////////////////////////////////////////
class CPASN_LIB25331
{
   public:

	CPASN_LIB25331();
	~CPASN_LIB25331();
	
	s8 * Decode_BCCH_BCH(const u8 *pduData, 
							u32 pduLength, u8 pduNumber, bool msgCategory);

   private:
	RrMsgParams    decMsgParams;
	
};

}

#endif //__PASN_LIB25331_H_

