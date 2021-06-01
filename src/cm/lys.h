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
 
    Name:   LTE CL Layer 
 
    Type:   C include file
 
    Desc:   Defines required by LTE-CL
 
    File:   lys.h
 
    Sid:      lys.h@@/main/1 - Sun Mar 28 18:40:12 2010
 
    Prg:    pk
 
**********************************************************************/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*                   LTE-CL 
*
*/
 
#ifndef __LYS_H__
#define __LYS_H__

#define EVTLYSCFGREQ       0      /* Config request */
#define EVTLYSCFGCFM       1      /* Config Confirm */
#define EVTLYSCNTRLREQ     2      /* Control Request */
#define EVTLYSCNTRLCFM     3      /* Control Confirm */
#define EVTLYSSTSREQ       4      /* Statistics Request */
#define EVTLYSSTSCFM       5      /* Statistics Confirm */
#define EVTLYSSSTAREQ      6      /* Solicited Status Request */
#define EVTLYSSSTACFM      7      /* Solicited Status Confirm */
#define EVTLYSUSTAIND      8      /* Unsolicite Status Indication */
#define EVTLYSTRCIND       9      /* Trace Indication */
#define EVTLYSRSYSLOG      10     /* log and diagnostics */
#ifdef TENB_T3K_SPEC_CHNGS 
#define EVTLYSLOGINFOREQ   11     /*log Stream Info Req*/
#define EVTLYSLOGINFOCFM   12     /*log Stream Info Cfm*/
#endif /* TENB_T3K_SPEC_CHNGS*/
#define EVTLYSRLIND        13     /* Change RLOG Logging Level */
#define EVTLYSTPMPWRCHGREQ 14     /* Ref Signal Power config Req */
#define EVTLYSPERDREMSCAN  15      /* Periodic REM Scan Request */


#define LYS_PT_NUM_SIZE    8

#define LYS_NOT_CFG        0      /* Sap not configured */
#define LYS_UNBND          1      /* Unbound Sap State */
#define LYS_BND            2      /* Bound Sap State */
#define LYS_WAIT_BNDCFM    3      /* Await Bind cofirm state */

#define LYS_PHY_STATE_IDLE 0
#define LYS_PHY_STATE_CFG  1
#define LYS_PHY_STATE_RUN  2
#define LYS_PHY_STATE_STOP 3

/* Unsolicited status indication events */
/* TFU interface SAP bound with MAC */
#define LYS_EVENT_TFUSAP_ENB      (LCM_EVENT_LYR_SPECIFIC + 0)    
/* CTF interface SAP bound with RRM */
#define LYS_EVENT_CTFSAP_ENB      (LCM_EVENT_LYR_SPECIFIC + 1)  

#ifdef E_TM
/* To get Indication for first TTI */
#define LYS_EVENT_ETM_CELLCFG     (LCM_EVENT_LYR_SPECIFIC + 2)    
#define LYS_EVENT_ETM_PHY_START   (LCM_EVENT_LYR_SPECIFIC + 3)    
#define LYS_EVENT_ETM_PHY_STOP    (LCM_EVENT_LYR_SPECIFIC + 4)    
#endif /* E_TM */
#define LYS_USTA_DGNVAL_MEM      1
#define LYS_USTA_DGNVAL_SAP      2
#define LYS_USTA_DGNVAL_SFN_SF   3

/* TfuSap need to move to gen.h. This may be removed later */
#define STTFUSAP         STGEN + 4
#define STCTFSAP         STGEN + 2
#define STL1PHY          STGEN + 3
#ifdef E_TM
#define STETMCELL        STGEN + 5
#define STETM            STGEN + 6
#endif /* E_TM */

#define SASTOPL1                 30

#ifdef TENB_T3K_SPEC_CHNGS 
#define  LOGINFO                  1
#define  CCPUMLOGSTRMINFO         1 
#define  MLOGSTRMINFO             2
#define  SYSCORELOGSTRMINFO       4  
#define  LYS_MAX_LOG_INFO         3 /*Max number of Log Stream Info carry in one log stream 
				      info cfm*/
#endif /* TENB_T3K_SPEC_CHNGS*/
/* pack unpack error code */
#define ELYSXXX                  0
#define ERRLYS                   0

#endif /* __LYS_H__ */

/**********************************************************************
         End of file:     lys.h@@/main/1 - Sun Mar 28 18:40:12 2010
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      pk  1. Initial Release
*********************************************************************91*/
