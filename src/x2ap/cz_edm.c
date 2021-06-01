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

     Name:     LTE-X2AP Layer 
  
     Type:     C Source file
  
     Desc:     This file handles the encoding/decoding of the X2AP messages.

     File:     cz_edm.c

     Sid:      tr_edm.c@@/main/2 - Tue Aug 30 18:36:10 2011

     Prg:       

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_pasn.h"       /* common per asn.1                */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "czt.h"           /* CZT interface defines           */
#include "lcz.h"           /* X2AP Layer management           */
#include "cz.h"            /* X2AP layer defines              */
#include "cz_err.h"        /* X2AP - error */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* common per asn.1                */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT interface structures        */
#include "lcz.x"           /* X2AP Layer management           */
#include "cz.x"            /* X2AP layer structures           */

/* local defines */

/* local typedefs */
   EXTERN CmPAsnElmDef *czMsgDb[];
/* local externs */
 
/* forward references */

/** @defgroup edmmodule X2AP Encoder/Decoder Module
 *  @{
 */
/**********************************************************************/
/* Public Function Definitions */
/**********************************************************************/

/**
 * @brief Encodes the X2AP message in ASN.1 format.
 * 
 * @details This function encodes the X2AP message into the buffer 
 * to be sent to the peer. Uses CM_PASN library for encoding.
 *
 * @param[in]  mem          Memory Region and pool info
 * @param[in]  x2apPdu      PDU to be encoded
 * @param[out] mBuf         Encoded buffer
 *
 * @return 
 *  - ROK        - If the message is successfully encoded.
 *  - RFAILED    - If the encoding is unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16 czEdmEncode
(
Mem          *mem,
CztX2AP_PDU  *x2apPdu,
Buffer       **mBuf
)
#else
PUBLIC S16 czEdmEncode (mem, x2apPdu, mBuf)
Mem          *mem;
CztX2AP_PDU  *x2apPdu;
Buffer       **mBuf;
#endif
{
   U8        dbgFlag;
   CmPAsnErr asnErr;
   S16       ret;
   U32       len=0;
   
   CZ_TRC2(czEdmEncode)

   dbgFlag = TRUE;

   
   CZ_ALLOCMBUF(mBuf);
   if (NULLP == mBuf)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Message Buffer Not allocated \n"));

      CZ_RETVALUE(RFAILED);
   }

   /* Initialize the PASN Error code */ 
   asnErr.errCode = 0;
  
   ret = cmPAsnEncMsg((TknU8 *)x2apPdu, CM_PASN_USE_MBUF, 
                      (Void *)(*mBuf), CZ_PASN_VER_ZERO, 
                      (CmPAsnElmDef **)czMsgDb, &asnErr, 
                      &len, dbgFlag, FALSE );
  
   if (ret == RFAILED)
   {
      /* It's an ASN.1 Error */
      CZ_DBG_ERROR((CZ_PRNT_BUF,
        "Transfer Syntax Error:Encoding failed, Reason %d\n",asnErr.errCode)); 
      
      CZ_FREEMBUF(*mBuf);
      CZ_RETVALUE(RFAILED);
  }

   CZ_RETVALUE(ROK);
} /* end of czEncode */

/* cz001.101 : CR fixes */

/**
 * @brief Decodes the X2AP message using the cm_pasn library.
 * 
 * @details  This function decodes the buffer received from the lower layer
 * into X2AP event to be sent the X2AP user. 
 
 * @param[in] uiEvnt Decoded X2AP PDU
 * @param[in] sMem   Memory control pointer
 * @param[in] mBuf   Buffer to be decoded
 * @param[out] err   ASN error if decoding fails 
 *
 * @return 
 *  - ROK        - If the message is successfully decoded.
 *  - RFAILED    - If the decoding is unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czEdmDecode
(
CztEvnt   **uiEvnt,
Mem       *sMem, 
Buffer    *mBuf,
CmPAsnErr *err
)
#else
PUBLIC S16 czEdmDecode (uiEvnt, sMem, mBuf, err)
CztEvnt   **uiEvnt;
Mem       *sMem; 
Buffer    *mBuf;
CmPAsnErr *err;       
#endif
{
   U32          numDecOct; /* number of octets decoded */
   S16          ret;       /* return value from different procedures */
   U8           dbgFlag;
   U32          msgLen;
   CztEvnt      *event = NULLP;
      

   CZ_TRC2(czEdmDecode)
                   
   dbgFlag = FALSE;

      
   /* Initialize the PASN Error code */ 
   CZ_ALLOCEVNT((Ptr *)&event, sizeof(CztEvnt), sMem);
   if(NULLP == event)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Allocation of CzEvnt Failed \n"));
      CZ_FREEMBUF(mBuf);
      CZ_RETVALUE(RFAILED);
   }
      
   err->errCode = 0;
   msgLen = 0;
  
   ret = (cmPAsnDecMsg((Void *)&(event->pdu), CM_PASN_USE_MBUF, 
                  (Void **)&mBuf, &numDecOct, CZ_PASN_VER_ZERO, 
                  (CmPAsnElmDef **)czMsgDb, CM_PASN_GEN_ERR, err, 
                  msgLen, (CmMemListCp *)event, dbgFlag));
  
   if (ret == RFAILED)
   {
      /* It's an ASN.1 Error */
      CZ_DBG_INFO((CZ_PRNT_BUF,
            " Syntax Error : Decoding failed, Reason %d\n", 
                               err->errCode)); 
      /* CZ_FREEEVNT(event);    */
      /* CZ_RETVALUE(RFAILED); */
   }

   CZ_FREEMBUF(mBuf);
   *uiEvnt = event;
   
   CZ_RETVALUE(ret);
} /* end of czEdmDecode */


/** @} */

/**********************************************************************
  
         End of file:     tr_edm.c@@/main/2 - Tue Aug 30 18:36:10 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
*********************************************************************91*/
