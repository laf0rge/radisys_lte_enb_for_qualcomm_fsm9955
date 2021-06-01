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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C source code for DB

     File:     szt_3gasn.h

     Sid:     

     Prg:      akaranth 

*********************************************************************21*/


#ifndef __SZT3GASNH__
#define __SZT3GASNH__ 

/*  szt_3gasn_h_001.main_1 :Support for Source RNC to Target RNC TC Enc/Dec. */

#define SztmaxNrOfRABs 256
#define SztmaxRAB_Subflows 7
#define Sztid_hS_DSCH_MAC_d_Flow_ID 117
#define Sztid_E_DCH_MAC_d_Flow_ID 160
#define Sztid_CN_DomainIndicator 3
#define SztmaxNrOfSRBs 8
#define Sztid_RAC 55
#define Sztid_SRB_TrCH_Mapping 98
#define Sztid_CellLoadInformGroup 121
#define Sztid_TraceRecordingSessionInform 124
#define Sztid_MBMSLinkingInform 156
#define Sztid_d_RNTI_for_NoIuCSUP 187
#define Sztid_UE_History_Inform 200
#define Sztid_SRVCC_Inform 227
#define Sztid_PSRABtobeReplaced 230
#define Sztid_CSFB_Inform 237
#define SRCCELLID_SOURCEUTRANCELLID 0
#define SRCCELLID_SOURCEGERANCELLID 1

#endif

/**********************************************************************
 
         End of file:     szt_3gasn.h
 
**********************************************************************/

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/

/********************************************************************50**
 
*********************************************************************51*/

/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/

/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$       szt_3gasn_h_001.main_1 akaranth 1. Support for Source RNC to Target RNC TC Enc/Dec.
********************************************************************91*/

