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
 
    Name:   LTE MAC DLFS scheduler
 
    Type:   C include file
 
    Desc:   Defines required by DLFS scheduler
 
    File:   rg_sch_dlfs.h
 
    Sid:      rg_sch_dlfs.h@@/main/2 - Sat Jul 30 02:22:03 2011
 
    Prg:    sd
 
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


#ifdef TFU_UPGRADE
#ifndef __RGDLFSH__
#define __RGDLFSH__

/* DLFS MACROs*/
#define RG_SCH_DLFS_DFLT_CQI 6
#define RG_SCH_DLFS_RETX_TRUE TRUE
#define RG_SCH_DLFS_RETX_FALSE FALSE
#define RG_SCH_DLFS_INVALID_CQI 16


/* DLFS Function MACROs */
#define RG_SCH_DLFS_GET_CELL(_cell) ((RgSchDlfsCellCb *)\
      ((RgSchCmnCell*)((_cell)->sc.sch))->dl.dlfsCell) 
#define RG_SCH_DLFS_GET_UE(_ue) ((RgSchDlfsUeCb *)((RgSchCmnUe *)((_ue)->sch))->dl.dlfsUe)
#define RG_SCH_DLFS_GET_SF(_sf) (RgSchDlfsSf *)((_sf)->dlfsSf)
#define RG_SCH_DLFS_CW1_WEIGHT 40
#define RG_SCH_DLFS_CW0_WEIGHT (100 - RG_SCH_DLFS_CW1_WEIGHT)
#define RG_SCH_DLFS_CQI_TO_MODODR(_x, _y) do {\
   if (_x <= 6) *(_y) = 2; \
   else if (_x <= 9) *(_y) = 4; \
   else *(_y) = 6;\
} while(0)
#define RG_SCH_DLFS_MODODR_TO_RETX_MCS(_x, _y) do {\
   if (_x == 2) *(_y) = 29; \
   else if (_x == 4) *(_y) = 30; \
   else *(_y) = 31;\
} while(0)

#define RG_SCH_DLFS_GET_PRD_SBSIZE(_sbSize, _totDlBw)\
RG_SCH_DLFS_GET_LARGE_SBSIZE(_sbSize, _totDlBw)

#define RG_SCH_DLFS_GET_APRD_MODE30_SBSIZE(_sbSize, _totDlBw)\
RG_SCH_DLFS_GET_LARGE_SBSIZE(_sbSize, _totDlBw)

#define RG_SCH_DLFS_GET_APRD_MODE20_SBSIZE(_sbSize, _totDlBw)\
RG_SCH_DLFS_GET_SMALL_SBSIZE(_sbSize, _totDlBw)

#define RG_SCH_DLFS_GET_SMALL_SBSIZE(_sbSize, _totDlBw)\
{\
   if ((_totDlBw) > 63)\
   {\
      *(_sbSize) = 4;\
   }\
   else if ((_totDlBw) > 26)\
   {\
      *(_sbSize) = 3;\
   }\
   else if ((_totDlBw) > 10)\
   {\
      *(_sbSize) = 2;\
   }\
   else\
   {\
      if ((_totDlBw) > 7)\
      {\
         *(_sbSize) = 2;\
      }\
      else\
      {\
         *(_sbSize) = 0;\
      }\
   }\
}\

#define RG_SCH_DLFS_GET_LARGE_SBSIZE(_sbSize, _totDlBw)\
{\
   if ((_totDlBw) > 63)\
   {\
      *(_sbSize) = 8;\
   }\
   else if ((_totDlBw) > 26)\
   {\
      *(_sbSize) = 6;\
   }\
   else if ((_totDlBw) > 10)\
   {\
      *(_sbSize) = 4;\
   }\
   else\
   {\
      if ((_totDlBw) > 7)\
      {\
         *(_sbSize) = 4;\
      }\
      else\
      {\
         *(_sbSize) = 0;\
      }\
   }\
}\

#ifndef LTE_TDD
#define RG_SCH_DLFS_GET_PRDICITY_AND_OFFST(_idx, _prdicity, _offset)\
{\
   if ((_idx) <= 1)\
   {*(_prdicity) = 2; *(_offset) = (U8)(_idx); }\
   else if ((_idx) <= 6)\
   {*(_prdicity) = 5; *(_offset) = (U8)((_idx) - 2); }\
   else if ((_idx) <= 16)\
   {*(_prdicity) = 10; *(_offset) = (U8)((_idx) -  7);}\
   else if ((_idx) <= 36)\
   {*(_prdicity) = 20; *(_offset) = (U8)((_idx) - 17); }\
   else if ((_idx) <= 76)\
   {*(_prdicity) = 40; *(_offset) = (U8)((_idx) - 37); }\
   else if ((_idx) <= 156)\
   {*(_prdicity) = 80; *(_offset) = (U8)((_idx) - 77); }\
   else if ((_idx) <= 316)\
   {*(_prdicity) = 160; *(_offset) = (U8)((_idx) - 157); }\
   else if ((_idx) == 317)\
   {*(_prdicity) = 0; *(_offset) = 0; }\
   else if ((_idx) <= 349)\
   {*(_prdicity) = 32; *(_offset) = (U8)((_idx) - 318); }\
   else if ((_idx) <= 413)\
   {*(_prdicity) = 64; *(_offset) = (U8)((_idx) - 350); }\
   else if ((_idx) <= 541)\
   {*(_prdicity) = 128; *(_offset) = (U8)((_idx) - 414); }\
   else if ((_idx) <=1023)\
   {*(_prdicity) = 0; *(_offset) = 0;}\
}
#else
#define RG_SCH_DLFS_GET_PRDICITY_AND_OFFST(_idx, _prdicity, _offset)\
{\
   if ((_idx) == 1)\
   {*(_prdicity) = 1; *(_offset) = (U8)(_idx); }\
   else if ((_idx) <= 5)\
   {*(_prdicity) = 5; *(_offset) = (U8)((_idx) - 1); }\
   else if ((_idx) <= 15)\
   {*(_prdicity) = 10; *(_offset) = (U8)((_idx) - 6);}\
   else if ((_idx) <= 35)\
   {*(_prdicity) = 20; *(_offset) = (U8)((_idx) - 16);}\
   else if ((_idx) <= 75)\
   {*(_prdicity) = 40; *(_offset) = (U8)((_idx) - 36);}\
   else if ((_idx) <= 155)\
   {*(_prdicity) = 80; *(_offset) = (U8)((_idx) - 76);}\
   else if ((_idx) <= 315)\
   {*(_prdicity) = 160; *(_offset) = (U8)((_idx) - 156);}\
   else if ((_idx) <=1023)\
   {*(_prdicity) = 0; *(_offset) = 0;}\
}
#endif /* ifndef LTE_TDD */

#define	RG_SCH_DLFS_WEIGH_AVG_SBCQI(_sbCqi,_sbCqi1) { \
  if (_sbCqi1 > 0) \
  _sbCqi = ((_sbCqi * RG_SCH_DLFS_CW0_WEIGHT) + (_sbCqi1 * (RG_SCH_DLFS_CW1_WEIGHT)))/100;	\
}
/* DLFS Error MACROs */
#define RGSCHERR_DLFS_MEMERR 0

#endif /* __RGDLFSH__ */
#endif


/**********************************************************************
         End of file:     rg_sch_dlfs.h@@/main/2 - Sat Jul 30 02:22:03 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      $      ---      sd  1. LTE MAC 2.1 release
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
$SID$        ---     rt      1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
