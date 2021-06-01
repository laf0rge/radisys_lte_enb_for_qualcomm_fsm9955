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

     Name:    EGTP - Transport Module

     Type:    C source file

     Desc:    This file contains hash defines, enum etc
              used in Transport Module.

     File:    eg_tpt.h

     Sid:      eg_tpt.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:18:50 2015

     Prg:     ad 

*********************************************************************21*/
#ifndef __EG_TPT_H__
#define __EG_TPT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Max No. of Hash Bin */
#define EG_MAX_NUM_HASHBIN_SEQNO    500


#ifdef NOT_USED
/* This will check whether MSB is set in SeQ No. or Not */
#define EG_IS_MSB_SET_IN_SEQ_NO(_seqNo, _isMsbSet)     \
{\
   U16 msb = 1;                                       \
   U16 tmp = 0;                                       \
                                                      \
   (_isMsbSet) = FALSE;                               \
   msb = msb << 23;                                    \
   tmp = msb & _seqNo;                                 \
                                                      \
   tmp = tmp >> 23;                                    \
                                                      \
   if (tmp == 1)                                      \
      (_isMsbSet) = TRUE;                              \
}
#define EG_EXAM_U32(_param, _mBuf, _count) \
{ \
   U16   nwWord[EG_LEN_TWO]; \
   U32   tmpU32 = 0; \
   U16   tmpCnt = _count; \
   nwWord[0] = 0; nwWord[1] = 0; \
   EG_EXAM_U16(nwWord[0], _mBuf, tmpCnt); \
   tmpCnt = tmpCnt + 2;\
   EG_EXAM_U16(nwWord[1], _mBuf, tmpCnt); \
   tmpU32 = PutHiWord(tmpU32, nwWord[0]); \
   tmpU32 = PutLoWord(tmpU32, nwWord[1]); \
   _param = tmpU32; \
}

#endif /* NOT_USED */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __EG_TPT_H__ */



   


/********************************************************************30**

         End of file:     eg_tpt.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:18:50 2015

*********************************************************************31*/


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
----------- -------- ---- -----------------------------------------------
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      rsharon           1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
*********************************************************************91*/
