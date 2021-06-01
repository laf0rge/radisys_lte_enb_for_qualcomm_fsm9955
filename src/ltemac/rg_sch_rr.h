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
 
    Name:   LTE MAC Round Robin scheduler
 
    Type:   C include file
 
    Desc:   Defines required by Round Robin scheduler
 
    File:   rg_sch_rr.h
 
    Sid:      rg_sch_rr.h@@/main/2 - Sat Jul 30 02:22:09 2011
 
    Prg:    rr
 
**********************************************************************/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/


#ifndef __RGSCHRRH__
#define __RGSCHRRH__

#define RG_RR_MIN_DCCH_LCID 1 /*!< Minimum value for DDCH LCID */
#define RG_RR_MAX_DCCH_LCID 2 /*!< Maximum value for DDCH LCID */
#define RG_RR_SR_GRNT       3 /*!< SR grant in bytes */



#define RG_SCH_RR_ADD_TO_TX_LIST(ue,rrCell,rrUe) \
{\
   if (rrUe->ueTxLnk.node == NULLP)\
   {\
      if(ue->csgMmbrSta == TRUE)\
      {\
         rgSCHRrCListAdd2Tail(&(rrCell->prioUeTxLst), &(rrUe->ueTxLnk));\
      }\
      else\
      {\
         rgSCHRrCListAdd2Tail(&(rrCell->nonPrioUeTxLst), &(rrUe->ueTxLnk));\
      }\
      rrUe->ueTxLnk.node = (PTR)ue;\
   }\
}

#define RG_SCH_RR_DEL_UE_FRM_TX_LIST(ue,rrCell,rrUe) \
{\
   if (rrUe->ueTxLnk.node != NULLP)\
   {\
      if(ue->csgMmbrSta == TRUE)\
      {\
         rgSCHRrCListDelFrm(&(rrCell->prioUeTxLst), &(rrUe->ueTxLnk));\
      }\
      else\
      {\
         rgSCHRrCListDelFrm(&(rrCell->nonPrioUeTxLst), &(rrUe->ueTxLnk));\
      }\
      rrUe->ueTxLnk.node = NULLP;\
   }\
}

#define RG_SCH_RR_DEL_UE_FRM_LIST_AND_ADD_TO_TAIL(ue,rrCell,rrUe) \
{\
   if(ue->csgMmbrSta == TRUE)\
   {\
      rgSCHRrCListDelFrm(&(rrCell->prioUeTxLst), &(rrUe->ueTxLnk));\
      rgSCHRrCListAdd2Tail(&(rrCell->prioUeTxLst), &(rrUe->ueTxLnk));\
   }\
   else\
   {\
      rgSCHRrCListDelFrm(&(rrCell->nonPrioUeTxLst), &(rrUe->ueTxLnk));\
      rgSCHRrCListAdd2Tail(&(rrCell->nonPrioUeTxLst), &(rrUe->ueTxLnk));\
   }\
}

#define RG_SCH_RR_UPD_UE_IN_PRIO_NONPRIO_TX_LIST(oldCSGMbrSta, newCSGMbrSta, rrCell, rrUe) \
{\
   if ((oldCSGMbrSta != newCSGMbrSta) && (rrUe->ueTxLnk.node != NULLP))\
   {\
      if (oldCSGMbrSta == TRUE)\
      {\
         rgSCHRrCListDelFrm(&(rrCell->prioUeTxLst), &(rrUe->ueTxLnk));\
         rgSCHRrCListAdd2Tail(&(rrCell->nonPrioUeTxLst), &(rrUe->ueTxLnk));\
      }\
      else\
      {\
         rgSCHRrCListDelFrm(&(rrCell->nonPrioUeTxLst), &(rrUe->ueTxLnk));\
         rgSCHRrCListAdd2Tail(&(rrCell->prioUeTxLst), &(rrUe->ueTxLnk));\
      }\
      rrUe->ueTxLnk.node = (PTR)ue;\
   }\
}
#endif /* __RGSCHRRH__ */


/**********************************************************************
         End of file:     rg_sch_rr.h@@/main/2 - Sat Jul 30 02:22:09 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rr  1. LTE MAC 2.1 release
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---       rt    1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
