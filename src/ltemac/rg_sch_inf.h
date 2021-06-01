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
     Name:    Scheduler interface - RG_SCH_INF

     Type:    C header file

     Desc:    Constants needed for interface

     File:    rg_sch_inf.h

     Sid:      rg_sch_inf.h@@/main/2 - Sat Jul 30 02:22:06 2011

     Prg:      

*********************************************************************21*/

#ifndef __GKSCH_H__ 
#define __GKSCH_H__

#define RGINF_MAX_NUM_DED_LC         10
#define RGINF_MAX_TB_PER_UE          2
#define RGINF_MAX_NUM_UE_PER_TTI     1
#define RGINF_MAX_LCG_PER_UE   4
/* RRM_SP1_START */
#define RGINF_MAX_GBR_QCI_REPORTS  4
/* RRM_SP1_END */
#define RGINF_BCH_INFO   (1<<0)
#define RGINF_BCCH_INFO  (1<<1)
#define RGINF_PCCH_INFO  (1<<2) 
/* Event corresponding to each primitive at this interface */
#define EVTINFCELLREGREQ          1
#define EVTINFSFALLOCREQ          2
#define EVTINFRLSHQREQ            3
#define EVTINFRLSRNTIREQ          4
#define EVTINFDEDBOUPDTREQ        5
#define EVTINFCMNBOUPDTREQ        6
#define EVTINFSFRECPIND           7
/* Added support for SPS*/
#ifdef LTEMAC_SPS
#define EVTINFSPSLCREG               8
#define EVTINFSPSLCDEREG             9
#define EVTINFSPSRELIND              10
#define EVTINFSPSRESET               18
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
#define EVTINFL2MEASREQ              11
#define EVTINFL2MEASCFM              12
/*Added for radisys oam*/
#define EVTINFL2MEASSENDREQ          14
#define EVTINFL2MEASSTOPREQ          15
#define EVTINFL2MEASSTOPCFM          16
#endif
/*Fix: Inform UE delete to scheduler*/
#define EVTINFUEDELIND               13

#define EVTINFLCGREG                 17

#define RGSCHINF_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      SPutMsg((_buf)); \
      _buf = NULLP; \
   } \
}
#endif /* __GKSCH_H__ */

/********************************************************************30**

         End of file:     rg_sch_inf.h@@/main/2 - Sat Jul 30 02:22:06 2011

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
/main/1      ---   rr 1. LTE MAC 2.1 release
        rg008.201 rsharon 1. Added support for SPS.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
$SID$        ---       rt    1. LTE MAC 4.1 Release.                           
*********************************************************************91*/

