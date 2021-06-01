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
  
     Name:     RRC Layer
 
     Type:     C source file
  
     Desc:     Initialisation of RRC task

     File:     nh_init.c
  
     Sid:      ni_init.c@@/main/3 - Fri Jul  1 01:13:19 2011
   
     Prg:      dsahoo
  
**********************************************************************/




/* Header include files (.h) */



#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"        /* Common Pasn defines */

#include "nhu.h"           /* RRU Interface defines */
#include "lnh.h"           /* LRR Interface defines */

#include "nh.h"            /* RRC defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_pasn.x"       /* Common Hashlist definitions */
#include "cm_lte.x"       /* Common Pasn defines */

#include "nhu.x"           /* RRU Interface includes */
#include "lnh.x"           /* LRR Interface includes */

#include "nh.x"            /* RRC includes */

#ifdef LTE_RRC_DISSECTOR
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include "cm_inet.h"
#include "cm_inet.x"
#include "brcm_tx_fastpath.h"
#endif /* LTE_RRC_DISSECTOR */



/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


#ifdef LTE_RRC_DISSECTOR
PUBLIC S16 nhDissectrInit(Void);
#endif /* LTE_RRC_DISSECTOR */

/**
 *
 *     @brief This function is the RRC Layer's Init task and is called
 *            during RRC task's registration
 *
 *     @details 
 *              
 *
 *     @param[in] entity     RRC Entity
 *     @param[in] inst       RRC Instance
 *     @param[in] region     Memory Region
 *     @param[in] reason     Reason
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PUBLIC S16 nhActvInit
(
Ent entity,            /* Entity */
Inst inst,             /* Instance */
Region region,         /* Region */
Reason reason          /* Reason */
)
#else /* ANSI */
PUBLIC S16 nhActvInit(entity, inst, region, reason)
Ent entity;            /* Entity */
Inst inst;             /* Instance */
Region region;         /* Region */
Reason reason;         /* Reason */
#endif /* ANSI */
{
   S16   i;
   
   TRC3(nhActvInit);

   /* memset the full control block to null */
   cmMemset ((U8 *)&nhCb, 0, sizeof(NhCb));

   nhCb.init.procId = SFndProcId();
   nhCb.init.ent = entity;
   nhCb.init.inst = inst;
   nhCb.init.region = region;
   nhCb.init.reason = reason;
   nhCb.init.cfgDone = FALSE;
   nhCb.init.pool = 0;
   nhCb.init.acnt = FALSE;
   nhCb.init.trc = FALSE;
   
   /* Set the default status to "No Un solicited Status Indication" */
   nhCb.init.usta = FALSE;

   /* Timing queue control point */
   nhCb.nhGenTqCp.nxtEnt = 0;
   nhCb.nhGenTqCp.tmrLen = NH_TQNUMENT;

   /* Initialise the timing queue */
   for (i = 0; i < NH_TQNUMENT; i++)
   {
     nhCb.nhGenTq[i].first = NULLP;
     nhCb.nhGenTq[i].tail = NULLP;
   }

   /* Initialize timing queues for Lower Layer Cfg */
   nhCb.nhLLyrCfgTqCp.nxtEnt = 0;
   nhCb.nhLLyrCfgTqCp.tmrLen = NH_TQNUMENT;

   for (i = 0; i < NH_TQNUMENT; i++)
   {
      nhCb.nhLLyrCfgTq[i].first = NULLP;
      nhCb.nhLLyrCfgTq[i].tail = NULLP;
   }


#ifdef NH_LOG_TO_FILE
   nhCb.dbgFp = NULLP;
   nhCb.fileLogEnb = FALSE;
   nhCb.nmbDbgLines = 0;
   nhCb.cfgDbgLines = 0;
   cmMemset( (U8* )nhCb.filePath, NH_ZERO_VAL, LNH_MAX_FILE_PATH);
#endif /* NH_LOG_TO_FILE */

   RETVALUE(ROK);

} /* end of function rrActvInit */

#ifdef LTE_RRC_DISSECTOR
/**
 * @brief Initialization function for LTERRC Dissector
 *
 * @details
 *
 *     Function : nhDissectrInit
 *     
 *     This function is responsible for doing CL initialization pertaining
 *     to Freescale physical layer.
 *     
 *  @param[in]  Void
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 nhDissectrInit
(
 Void  
)
#else
PUBLIC S16 nhDissectrInit(Void)
#endif
{
   S16 ret = ROK;
   struct hostent * hp;
   
   nhCb.udpSock = socket(AF_INET, SOCK_DGRAM, 0);
   if(nhCb.udpSock < 0)
   {
      nhCb.udpSock = 0;
      perror("Socket cant be created");
      
      ret = RFAILED;
   } 
   nhCb.server.sin_family = AF_INET;
   hp = gethostbyname((const char *)nhCb.genCfg.ipAddress);
   if ( hp == 0){
      perror("Unknown Host");
      ret = RFAILED;
   }
   bcopy((char *)hp->h_addr,
         (char *)&nhCb.server.sin_addr,
         hp->h_length);
   nhCb.server.sin_port = htons(nhCb.genCfg.portNumber);

   RETVALUE(ret);
}

/**
 * @brief This function sends message to configured IP addr using UDP socket.
 *
 * @details
 *
 *     Function : nhDissectrSndMsg
 *     
 *     This function is responsible for sending msg to a configured IP addr.
 *     
 *  @param[in]  Void
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 nhDissectrSndMsg
(
 Buffer *mBuf,
 U8      msgCategory
)
#else
PUBLIC S16 nhDissectrSndMsg(mBuf, msgCategory)
Buffer *mBuf;
U8      msgCategory;
#endif
{
   S16 ret = ROK;
   Data buf[5000] = {0};
   Data *bufPtr = NULLP;
   MsgLen msgLen=0, cpLen=0;
   unsigned int socklen=0;
#ifdef BRDCM
   //tx_fpath_ctl_t tx_fpath_ctl;
#endif

#if 0
#else
   socklen=sizeof(struct sockaddr_in);
   msgLen = 0;
   cpLen =0;
   SFndLenMsg(mBuf, &msgLen);
   if(msgLen >= 4999)
   {
      RETVALUE(RFAILED);
   }

   if(mBuf->b_cont == NULLP)
   {
      RETVALUE(RFAILED);
   }
 
   buf[0] = msgCategory;
   bufPtr = buf;
   bufPtr++;
   SCpyMsgFix(mBuf, 0, msgLen,(Data *)bufPtr, &cpLen);

   ret = sendto(nhCb.udpSock,(U8 *)buf, msgLen+1,
         0,
         (const struct sockaddr *)&nhCb.server,
         socklen);
   if (ret < 0)
   {
      printf("\n Error in Sending Request to dissector\n") ;
   }
#endif
   if(ret < 0 )
   {
      printf("\n Error:  sendMsg is failed file=%s line=%u\n",__FILE__, __LINE__);
   }
   RETVALUE(ret);
}
#endif /*LTE_RRC_DISSECTOR */

/**********************************************************************
  
         End of file:     ni_init.c@@/main/3 - Fri Jul  1 01:13:19 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      ds               1. LTE RRC Release 1.1.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
/main/4      ---      njha             1. Added SS_DIAG flag.
*********************************************************************91*/
