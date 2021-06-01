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
*************************************************************************************************17*/


/**************************************************************************************************

     Name:     REM-APP

     Type:     C++ Source file

     Desc:     This file consists of the decoder functions for WCDMA related messages

     File:     rem_3g_rrcmsg_decoder.cpp

     Sid:      rem_3g_rrcmsg_decoder.cpp@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:21 2013

     Prg:      Chetan H

**************************************************************************************************/

#define CMFILE_REORG_2
#define SS_SINGLE_THREADED
#define _REENTRANT
#define DEBUGNOEXIT
/*DEBUGP
CM_INET2 -DCCPU_OPT*/

/* Header include files */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link list defines */
#include "cm_hash.h"       /* Common Hashlist defines */
//#include "cm_lte.h"        /* Common Umts defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"       /* Common LTE defines */
#include "rru_asn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Linklist definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_pasn.x"       /* Common Pasn definitions */
#include "cm_lib.x"        /* Common Library */
#include "rru_asn.x"

#include "rem_common.h"
#include "rem_interface.h"

EXTERN CmPAsnElmDef *rrSysInfoTyp1_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp2_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp3_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp4_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp5_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp6_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp7_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp8_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp9_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp10_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp11_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp11bis_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp12_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp13_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp13_1_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp13_2_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp13_3_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp13_4_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp15_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp15_1_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp15_2_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp15_3_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp15_4_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp15_5_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp16_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp18_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTyp20_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTypSB1_Pdu[];
EXTERN CmPAsnElmDef *rrSysInfoTypSB2_Pdu[];
EXTERN CmPAsnElmDef *rrMasteInfBlock_Pdu[];

typedef struct rrc3GDecodePdu
{
   CmMemListCp    memCp;
   union
   {
      RruSysInfoTyp1      sib1;
      RruSysInfoTyp2      sib2;
      RruSysInfoTyp3      sib3;
      RruSysInfoTyp4      sib4;
      RruSysInfoTyp5      sib5;
      RruSysInfoTyp5      sib5bis;
      RruSysInfoTyp6      sib6;
      RruSysInfoTyp7      sib7;
      RruSysInfoTyp8      sib8;
      RruSysInfoTyp9      sib9;
      RruSysInfoTyp10     sib10;
      RruSysInfoTyp11     sib11;
      RruSysInfoTyp11bis  sib11bis;
      RruSysInfoTyp12     sib12;
      RruSysInfoTyp13     sib13;
      RruSysInfoTyp13_1   sib13_1;
      RruSysInfoTyp13_2   sib13_2;
      RruSysInfoTyp13_3   sib13_3;
      RruSysInfoTyp13_4   sib13_4;
      RruSysInfoTyp15     sib15;
      RruSysInfoTyp15_1   sib15_1;
      RruSysInfoTyp15_2   sib15_2;
      RruSysInfoTyp15_3   sib15_3;
      RruSysInfoTyp15_4   sib15_4;
      RruSysInfoTyp15_5   sib15_5;
      RruSysInfoTyp16     sib16;
      RruSysInfoTyp18     sib18;
      RruSysInfoTyp20     sib20;/*rru_x_001.main_10 upgraded to support SIB20*/
      RruSysInfoTypSB1    sb1;
      RruSysInfoTypSB2    sb2;
      RruMasteInfBlock    mib;

   }u;
} Rrc3GDecodePdu;

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void*
//
// This function gets entity for REM Application defined in class MessagingEntity
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void decode3gBCCHMessage(void *pBuff)
{ 

    S16 ret;
    CmPAsnErr err;
    bool dbgFlag = TRUE;
    void *pdu = NULL;
    U32   numDecOct = 0;
    Buffer *mBuf = NULL;
    MsgLen pduLength = 0;
    U8 pduNumber=0;   // Ambika : 28-11-12 
 							//There is a bug here . PDU Number is local
	 						// and used in switch without initialization.
	 					   // Should be changed in next release for 3G support
    /*void *retVal = NULL;*/

    CmPAsnElmDef **elmDef = NULL;
    Rrc3GDecodePdu *decodePdu = NULL;

    //copy consecutive bytes of src pBuff to mBuf
    ret = SAddPstMsgMult((Data *)pBuff, pduLength, mBuf);
    if(ret != ROK)
    {
		#ifdef DEBUG_PRINT
        remLog(REM_TRACE_INFO,"CPASN_LIB25331::Decode_BCCH_BCH: SAddPstMsgMult() failed"); 
		#endif
        SPutMsg(mBuf); //deallocate the message back
    }

    switch(pduNumber)
    {
        case RruSIB_TypmastInfBlockEnum:
            pdu = (void*)&(decodePdu->u.mib);
            elmDef = rrMasteInfBlock_Pdu;
            //RruMasteInfBlock
        break;

        case RruSIB_TypsysInfoBlockTyp1Enum:
            pdu = (void*)&(decodePdu->u.sib1);
            elmDef = rrSysInfoTyp1_Pdu;
            //RruSysInfoTyp1
        break;

        case RruSIB_TypsysInfoBlockTyp2Enum:
            pdu = (void*)&(decodePdu->u.sib2);
            elmDef = rrSysInfoTyp2_Pdu;

            //RruSysInfoTyp2
            break;

        case RruSIB_TypsysInfoBlockTyp3Enum:
            pdu = (void*)&(decodePdu->u.sib3);
            elmDef = rrSysInfoTyp3_Pdu;
            //RruSysInfoTyp3
            break;

        case RruSIB_TypsysInfoBlockTyp4Enum:
            pdu = (void*)&(decodePdu->u.sib4);
            elmDef = rrSysInfoTyp4_Pdu;

            //RruSysInfoTyp4
            break;
        case RruSIB_TypsysInfoBlockTyp5Enum:
            pdu = (void*)&(decodePdu->u.sib5);
            elmDef = rrSysInfoTyp5_Pdu;

            //RruSysInfoTyp5
            break;

        case RruSIB_TypsysInfoBlockTyp6Enum:
            pdu = (void*)&(decodePdu->u.sib6);
            elmDef = rrSysInfoTyp6_Pdu;

            //RruSysInfoTyp6
            break;

        case RruSIB_TypsysInfoBlockTyp7Enum:
            pdu = (void*)&(decodePdu->u.sib7);
            elmDef = rrSysInfoTyp7_Pdu;

            //RruSysInfoTyp7
            break;
        case RruSIB_TypsysInfoBlockTyp11Enum:
            pdu = (void*)&(decodePdu->u.sib11);
            elmDef = rrSysInfoTyp11_Pdu;

            //RruSysInfoTyp11
            break;

        case RruSIB_TypsysInfoBlockTyp12Enum:
            pdu = (void*)&(decodePdu->u.sib12);
            elmDef = rrSysInfoTyp12_Pdu;
            //RruSysInfoTyp12
            break;

        case RruSIB_TypsysInfoBlockTyp13Enum:
            pdu = (void*)&(decodePdu->u.sib13);
            elmDef = rrSysInfoTyp13_Pdu;

            //RruSysInfoTyp13
            break;

        case RruSIB_TypsysInfoBlockTyp13_1Enum:
            pdu = (void*)&(decodePdu->u.sib13_1);
            elmDef = rrSysInfoTyp13_1_Pdu;

            //RruSysInfoTyp13_1
            break;

        case RruSIB_TypsysInfoBlockTyp13_2Enum:
            pdu = (void*)&(decodePdu->u.sib13_2);
            elmDef = rrSysInfoTyp13_2_Pdu;

            //RruSysInfoTyp13_2
            break;

        case RruSIB_TypsysInfoBlockTyp13_3Enum:
            pdu = (void*)&(decodePdu->u.sib13_3);
            elmDef = rrSysInfoTyp13_3_Pdu;

            //RruSysInfoTyp13_3
            break;
       case RruSIB_TypsysInfoBlockTyp13_4Enum:
            pdu = (void*)&(decodePdu->u.sib13_4);
            elmDef = rrSysInfoTyp13_4_Pdu;

            //RruSysInfoTyp13_4
            break;

        case RruSIB_TypsysInfoBlockTyp15Enum:
            pdu = (void*)&(decodePdu->u.sib15);
            elmDef = rrSysInfoTyp15_Pdu;

            //RruSysInfoTyp15
            break;

        case RruSIB_TypsysInfoBlockTyp15_1Enum:
            pdu = (void*)&(decodePdu->u.sib15_1);
            elmDef = rrSysInfoTyp15_1_Pdu;

            //RruSysInfoTyp15_1
            break;

        case RruSIB_TypsysInfoBlockTyp15_2Enum:
            pdu = (void*)&(decodePdu->u.sib15_2);
            elmDef = rrSysInfoTyp15_2_Pdu;

            //RruSysInfoTyp15_2
            break;

        case RruSIB_TypsysInfoBlockTyp15_3Enum:
            pdu = (void*)&(decodePdu->u.sib15_3);
            elmDef = rrSysInfoTyp15_3_Pdu;
            //RruSysInfoTyp15_3
            break;

        case RruSIB_TypsysInfoBlockTyp15_4Enum:
            pdu = (void*)&(decodePdu->u.sib15_4);
            elmDef = rrSysInfoTyp15_4_Pdu;

            //RruSysInfoTyp15_4
            break;

        case RruSIB_TypsysInfoBlockTyp15_5Enum:
            pdu = (void*)&(decodePdu->u.sib15_5);
            elmDef = rrSysInfoTyp15_5_Pdu;

            //RruSysInfoTyp15_5
            break;

        case RruSIB_TypsysInfoBlockTyp16Enum:
            pdu = (void*)&(decodePdu->u.sib16);
            elmDef = rrSysInfoTyp16_Pdu;

            //RruSysInfoTyp16
            break;

        case RruSIB_TypsysInfoBlockTyp18Enum:
            pdu = (void*)&(decodePdu->u.sib18);
            elmDef = rrSysInfoTyp18_Pdu;

            //RruSysInfoTyp18
            break;
        case RruSIB_TypschengBlock1Enum: //gp check this
            pdu = (void*)&(decodePdu->u.sb1);
            elmDef = rrSysInfoTypSB1_Pdu;

            //RruSysInfoTypSB1
            break;

        case RruSIB_TypschengBlock2Enum: //gp check this
            pdu = (void*)&(decodePdu->u.sb2);
            elmDef = rrSysInfoTypSB2_Pdu;

            //RruSysInfoTypSB2
            break;

        default:
            //nothing but to return
        break;
    }
    ret = cmPAsnDecMsgUalign ((void*)(pdu), CM_PASN_USE_MBUF,
       (void **)&mBuf, &numDecOct, 0, elmDef,
       CM_PASN_GEN_ERR, &err, pduLength,
       (CmMemListCp *)&decodePdu, dbgFlag);

    if (ROK != ret)
    {
       // TRACE_PRINTF("pduNumber %d decoding failed", pduNumber);
    }
}
