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

     Name:     LTE RRC Layer

     Type:     C extern file

     Desc:     Contains extern definitions for LTE RRC ASN1.C database

     File:     nh_db.x

     Sid:      nh_db.x@@/main/3 - Fri Jul  1 01:13:11 2011

     Prg:      chebli 

**********************************************************************/
#ifndef __NHDBX__
#define __NHDBX__



#ifdef __cplusplus
EXTERN "C" {
#endif
EXTERN CmPAsnElmDef *nhUL_CCCH_Pdu[];
EXTERN CmPAsnElmDef *nhUL_DCCH_Pdu[];
EXTERN CmPAsnElmDef *nhDL_CCCH_Pdu[];
EXTERN CmPAsnElmDef *nhDL_DCCH_Pdu[];
EXTERN CmPAsnElmDef *nhPCCH_Pdu[];
EXTERN CmPAsnElmDef *nhBCCH_BCH_Pdu[];
EXTERN CmPAsnElmDef *nhBCCH_DL_SCH_Pdu[];
EXTERN CmPAsnElmDef *nhUE_EUTRA_Cap_Pdu[];
EXTERN CmPAsnElmDef *nhHovrCmd_Pdu[];
EXTERN CmPAsnElmDef *nhHovrPrepInfo_Pdu[];
EXTERN CmPAsnElmDef *nhUERadioAccessCapInfo_Pdu[];
EXTERN CmPAsnElmDef *nhUECapInfo_Pdu[];
#ifdef NHU_VER2
EXTERN CmPAsnElmDef *nhVarShortMAC_Input_Pdu[];
#endif /* NHU_VER2 */

/* SPS changes starts */
EXTERN CmPAsnElmDef *nhUERadioAccessCap9A0Info_Pdu[];
/* SPS changes ends */

#ifdef __cplusplus
}
#endif


#endif /* __NHDBX__ */

/**********************************************************************
         End of file:     nh_db.x@@/main/3 - Fri Jul  1 01:13:11 2011
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

   ver       pat      init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      chebli           1. LTE RRC Initial Release.
/main/2      ---      chebli           1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/

