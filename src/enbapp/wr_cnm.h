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

/* CNM API messages */

#ifndef __WR_CNM_H__
#define __WR_CNM_H__

/* Macros for ICTA Msg Id's */
#define WR_CNM_ICTA_START_REQ   90
#define WR_CNM_ICTA_START_RESP  91
#define WR_CNM_ICTA_STOP_REQ    92
#define WR_CNM_ICTA_STOP_RESP   93
#define WR_CNM_ICTA_SYNC_IND    96
#define WR_CNM_ICTA_FOE_REQ     98
#define WR_CNM_ICTA_FOE_RESP    99

#define WR_CNM_MAX_VENDOR_LIST  6 /* same MAX_VENDOR_LIST listed in interface API header file */
#define WR_CNM_MAX_EARFCN_TABLE_SIZE 8 /* from freq band 33 to 40
                                        * For FDD, this needs to be modified
                                        * */
#define WR_CNM_RADIO_ELMNT_SIZE 20 /* From the CNM API doc */

typedef struct wrCnmIctaStartVendorSpcfc
{
   U8 ictaRadioAddElement[WR_CNM_RADIO_ELMNT_SIZE];
   U8 enbRadioAddElement[WR_CNM_RADIO_ELMNT_SIZE];
}__attribute__((__packed__))WrCnmIctaStartVendorSpcfc;

typedef struct wrCnmIctaStartMsg
{
    U16 msgLen;
    U8  msgType;
    U8  phyEntityId;
    U16 nghEarfcn; 
    U16 servEarfcn; 
    S16 nghPci;
    S16 nghCellCp;
    S16 nghSpecSfCfg;
    S16 nghTxAntCount;
    S16 nghTrackNRb;
    S16 trackPeriod;
    U8  vendorSpcfcLstCount;
    U8  radioChipType;
    WrCnmIctaStartVendorSpcfc vendorSpcfcLst;
} __attribute__ ((__packed__))WrCnmIctaStartMsg;


typedef struct wrCnmIctaStopMsg
{
    U16 msgLen;
    U8  msgType;
    U8  phyEntityId;
    U8  vendorSpcfcLstCount;
    U16 vendorSpcfcLst[WR_CNM_MAX_VENDOR_LIST];
} __attribute__ ((__packed__))WrCnmIctaStopMsg;

typedef struct wrCnmIctaFoeReqMsg
{
    U16 msgLen;
    U8  msgType;
    U8  phyEntityId;
    U8  vendorSpcfcLstCount;
    U16 vendorSpcfcLst[WR_CNM_MAX_VENDOR_LIST];
} __attribute__ ((__packed__))WrCnmIctaFoeReqMsg;


typedef struct wrCnmEarfcnTable
{
  U16   fDlLow;
  U16   nOffsetDl;
  U16   earfcnStart;
  U16   earfcnEnd;
  U8    freqBand;
}WrCnmEarfcnTable;
#endif
