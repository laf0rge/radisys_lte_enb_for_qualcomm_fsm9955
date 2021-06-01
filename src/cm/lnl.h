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

     Name:     ESON application

     Type:     

     Desc:     This file contains the 
               

     File:     lnl.h

     Sid:      lnl.h@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 15:08:06 2014

     Prg:      subhamay 

*********************************************************************21*/
#ifndef __LNLH__
#define __LNLH__

#define LNL_SEL_LC              0
#define LNL_SEL_TC              1
#define LNL_SEL_LWLC            2

/* MAX Number of Service Providers of SM */
#define MAXNLMI                 3 

/* LNL Events */
#define EVTLNLCFGREQ             1
#define EVTLNLCFGCFM             2
#define EVTLNLCNTRLREQ           3 
#define EVTLNLCNTRLCFM           4 
#define EVTLNLCFGUPDATEIND       5
#define EVTLNLSTAIND             6
#define EVTLNLSONCFGIND          7   /*!< events for sending SON configuration
                                          indication from SON module to SM*/

#define STNLGEN                 1
#define STNLPROTOCFG            2

#define LNL_TRANSID_GEN         1000
#define LNL_TRANSID_NLU         (LNL_TRANSID_GEN + 1)
#define LNL_TRANSID_CELL        (LNL_TRANSID_GEN + 2)
#define LNL_TRANSID_CELL_PCI    (LNL_TRANSID_GEN + 3)
#define LNL_TRANSID_NGH_CELL    (LNL_TRANSID_GEN + 4)


#define LNL_NOT_CFG             0      /*!< SAP not configured */
#define LNL_UNBND               1      /*!< Unbound SAP State */
#define LNL_BND                 2      /*!< Bound SAP State */
#define LNL_WAIT_BNDCFM         3      /*!< Await Bind Confirm State */

#define NL_MAX_SEL                 3
#define LNL_MAX_PCI_CFG            504
#define LNL_MAX_NGH_CELL_INFO      16
#define LNL_MAX_NGH_NGH_CELL_INFO  16
#define LNL_MAX_NGH_ENB_CFG        16
#define LNL_MAX_NUM_EARFCN         64
#define LNL_MAX_ROOTSEQ_INDX       512
#define LNL_MAX_PRACH_CFG_IDX      256
#define LNL_MAX_ZERO_CORR_CFG_ZONE 64
#define LNL_MAX_NUM_FREQ_OFFSET    256

#define LNL_MAX_POWER_OFFSET_CNT   512
#define LNL_MAX_PA_CNT             4
#define LNL_MAX_PB_CNT             8
#define LNL_MAX_EARFCN_CNT         128
#define LNL_MAX_PCI_CNT            504
#define LNL_MAX_BW_CNT             6
#define LNL_MAX_RS_TXPWR_CNT       128

#define LNL_MAX_PRB_CNT            110   /*!< Maximum number of PA values of
                                              PRB configured by SON*/
#define LNL_SON_CFG_MAX_UE         16    /*!< Maximum number of UE for which
                                              PA values can be configured by SON*/
#endif /* __LNLH__ */
/********************************************************************30**

           End of file:     lnl.h@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 15:08:06 2014

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
*********************************************************************91*/
