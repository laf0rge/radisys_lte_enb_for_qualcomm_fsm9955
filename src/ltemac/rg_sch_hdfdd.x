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
 
    Name:   LTE MAC HD-FDD 
 
    Type:   C include file
 
    Desc:   Defines required by HD-FDD 
 
    File:   rg_sch_hdfdd.x
 
    Sid:      rg_sch_hdfdd.x@@/main/1 - Sat Jul 30 02:22:05 2011
 
    Prg:    rbabu
 
**********************************************************************/
/**
 *  @file rg_sch_hdfdd.x This file gives the describes the design for Half
 *  Duplex FDD feature.
 *
 *  Half duplex FDD operation is one in which the UE cannot receive and 
 *  transmit at the same time. This is more of a UE limitation, chosen to 
 *  reduce the complexity of the UE's hardware. In LTE the half duplex FDD 
 *  is implemented such that the eNodeB schedules such that the UE doesn't 
 *  transmit and receive in the same subframe. The UE unless informed that a 
 *  subframe is for Uplink transmission continues to look at the PDCCH. 

 *  Due to the delay in switching from downlink to uplink, UE is unable to 
 *  receive the last few symbols of the subframe preceding the subframe 
 *  assigned for uplink transmissions.Half duplex FDD is a UE specific 
 *  configuration.
*/


/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/
 
#ifndef __RGHDFDDX__
#define __RGHDFDDX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */




/******************************************************************************
 *                         Start of Data declarations                         *
 ******************************************************************************/

/**
  @brief Half Duplex subframtion information stored per ue. */

typedef struct rgSchUeHdFddSfInfo
{
  U16             sfn;                   /*!< Store the sfn for updated state 
                                              Default Value= 0xffff
                                          */
  U8              state;
                                         /*!< 0x00= DFLT
                                              0x01= DL DATA +(OPT:CNTRL INFO)
                                              0x02= DL CNTRL
                                              0x04= UL DATA+(OPT: CTNRL INFO) 
                                         */ 

}RgSchUeHdFddSfInfo;

/**
  @brief Half Duplex control related information per ue. */

typedef struct rgSchUeHdFddCb
{
   RgSchUeHdFddSfInfo    sf[RG_SCH_HDFDD_MAXSUB_INFO]; 
    
   CmLList               hdFddLstEnt;  /*!< Linked list entity for HD-FDD List */

}RgSchUeHdFddCb;




#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGHDFDD__ */


/**********************************************************************
         End of file:     rg_sch_hdfdd.x@@/main/1 - Sat Jul 30 02:22:05 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        gvj  1. Updated for LTE MAC Rel 3.1
$SID$        ---       rt    1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
