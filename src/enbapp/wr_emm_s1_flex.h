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

     Name:     EnodeB Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_emm.h

     Sid:      wr_emm_s1_flex.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:56:10 2015

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_EMM_S1_FLEX_H__
#define __WR_EMM_S1_FLEX_H__ 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _wrGuGrupCbList
{
   CmHashListEnt guGrupHashEnt;
   WrGuGrpIdLst  grupId;
   U8       guGrupIdArr[8];
  /* This is the list of MMECBs supported by particular GUMMEI.*/
   CmLListCp mmeList;
}WrGuGrupCbList; 

#define WR_UMM_MAX_MME_CB_LIST 100
#define CB_KEY_LEN 9
#define GU_GRUP_KEY_LEN 8

typedef struct _wrGummeiMmeInfo
{
   CmLList                   lnk;
   Bool                      isLoaded;
   WrMmeCb                   *mmePtr;
}WrGummeiMmeInfo;
 /** This structure contains
 - mmeIdList list of MMECbs supported by GUMMEI gummeiId used for RRC procedure with GUMMEI present in it.
 */
typedef struct _wrGummeiCbList
{
   CmHashListEnt gummeiHashEnt;
   U8 gummeiId[CB_KEY_LEN];
  /* This is the list of MMECBs supported by particular GUMMEI.*/
   CmLListCp mmeIdList;
}WrGummeicbList;

typedef struct  _wrGuGrupMmeInfo
{
   CmLList                   lnk;
   WrMmeCb                   *mmePtr;
}WrGuGrupMmeInfo; 


 #define WR_EMM_GET_GUGRUP_ARR(_grupId, _grupArr){\
    _grupArr[0]= (_grupId.mmeGrpId & 0xFF); \
    _grupArr[1]= ((_grupId.mmeGrpId >> 8) & 0xFF); \
    _grupArr[2]= _grupId.plmnId.mcc[0]; \
    _grupArr[3]= _grupId.plmnId.mcc[1]; \
    _grupArr[4]= _grupId.plmnId.mcc[2]; \
    _grupArr[5]= _grupId.plmnId.mnc[0]; \
    _grupArr[6]= _grupId.plmnId.mnc[1]; \
    if(_grupId.plmnId.numMncDigits == 3)  \
    _grupArr[7]= _grupId.plmnId.mnc[2];  \
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__WR_EMM_S1_FLEX_H__  */


