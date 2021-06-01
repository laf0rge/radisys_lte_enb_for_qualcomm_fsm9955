/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

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
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************
 
    Name:   RRC layer
 
    Type:   C include file
 
    Desc:   Defines required by RRC
 
    File:   rr.h
 
    Sid:      rr_db.h@@/main/1 - Sat Oct  6 06:52:56 2007
 
    Prg:    rbhat
 
**********************************************************************/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000206       RRC
*
*/
 
#ifndef __RRDBH__
#define __RRDBH__

#define rrNAS_SysInfoANSI_41 rrANSI_41_NAS_Parm
#define rrRLC_PDU_Sz rrOctetModeRLC_SzInfoTyp1
/*rr034.202 Upgraded to support RRC Rel-9 Database--*/
#define rrE_RUCCH_AcsSrvCls rrPRACH_Parting_LCR_r4
#define rrMIMO_Parms_v7f0ext rrMIMO_PilotCfg_v7f0ext
#define rrTrgtCellPreconfInfo_v890ext rrMIMO_Parms_v7f0ext
#define rrCSGIntraFreqCellInfoList rrCSGCellInfoList
#define rrSIR_TFCS rrTFCS_IdentPlain
#define rrSysInfoTyp5bis rrSysInfoTyp5
#define rrANSI_41_RAND_Inf rrANSI_41_NAS_Parm
#define rrANSI_41_UserZoneID_Inf rrANSI_41_NAS_Parm
#define rrANSI_41_PrivaNeighListInfo rrANSI_41_NAS_Parm
#define rrANSI_41_GlobaSrvRedirInfo rrANSI_41_NAS_Parm
#define rrMBMS_PFLInfo rrFreqInfo
#define rrRRCConnRls_CCCH_v690ext_IEs rrRRCConnRls_v690ext_IEs
#define rrRRCConnRls_CCCH_v860ext_IEs rrRRCConnRls_v860ext_IEs
#define rrRRCConnRls_CCCH_r5_IEs rrRRCConnRls_CCCH_r4_IEs

#endif /* __RRDBH__ */

/**********************************************************************
         End of file:     rr_db.h@@/main/1 - Sat Oct  6 06:52:56 2007
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     rbhat   1. RRC Initial Release.
             ---     vg      1. rr034.202 Upgraded to support RRC Rel-9 Database
*********************************************************************91*/
