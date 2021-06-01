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

/**************************************************************************************************

     Name:     UMTS REM - PASN-LIB25331

     Type:     C++ Source file

     Desc:     This file is interface between UMTS decoder and rem

     File:     pasn_lib25331.cpp

     Sid:      pasn_lib25331.cpp@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:26 2016

     Prg:      Rohit Kumar 


**************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <system/Trace.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "pasn_lib25331.h"
#include "cm_pasn.h"
#include "cm_pasn.x"

using namespace threeway;

#ifdef __cplusplus
extern "C" {
#endif

extern PUBLIC CmPAsnElmDef rrBCCH_BCH_Msg;
extern PUBLIC CmPAsnElmDef *rrBCCH_BCH_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp1_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp2_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp3_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp4_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp5_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp6_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp7_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp11_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp12_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp13_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp13_1_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp13_2_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp13_3_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp13_4_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp15_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp15_1_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp15_2_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp15_3_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp15_4_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp15_5_Pdu[]; 
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp16_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTyp18_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTypSB1_Pdu[];
extern PUBLIC CmPAsnElmDef *rrSysInfoTypSB2_Pdu[];
extern PUBLIC CmPAsnElmDef *rrMasteInfBlock_Pdu[];

extern bool g_debugModeFlag;

#ifdef __cplusplus
}
#endif

static Mem sMem;

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////

CPASN_LIB25331::CPASN_LIB25331()
{
    sMem.region = DFLT_REGION;
    sMem.pool = DFLT_POOL;

    decMsgParams.sdu = NULL;

    if(cmAllocEvnt(sizeof(RrAllSdus), RR_MEM_PDU_SIZE, &sMem, 
        (Ptr *)&(decMsgParams.sdu)) != ROK)
    {
        TRACE_PRINTF("#### cmAllocEvnt Failed. Failed to allocate memory");
        exit(0);
    }
}


CPASN_LIB25331::~CPASN_LIB25331()
{
    cmFreeMem((char*)decMsgParams.sdu);    
}

s8 * CPASN_LIB25331::Decode_BCCH_BCH(const u8 *pduData, u32 pduLength, 
                                          u8 pduNumber, bool msgCategory)

{
    S16 ret = NULL;
    CmPAsnErr err;
    Bool dbgFlag = TRUE;    
    if(g_debugModeFlag)
    {
       dbgFlag = TRUE;    
    }
    else
    {
       dbgFlag = FALSE;    
    }

    void *pdu = NULL;
    U32   numDecOct = 0;
    Buffer *mBuf = NULL;

    CmPAsnElmDef **elmDef = NULL;

    if (ROK != SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf))
    {
        TRACE_PRINTF("CPASN_LIB25331::Decode_BCCH_BCH: SGetMsg failed.");

        if(mBuf)
        {
            cmFreeMem((Ptr)mBuf);
        }
        
        return((s8*) NULL);
    }

    //copy consecutive bytes of src pduData to mBuf
    ret = SAddPstMsgMult((Data *)pduData, pduLength, mBuf);
    if(ret != ROK)
    {
        TRACE_PRINTF("CPASN_LIB25331::Decode_BCCH_BCH: SAddPstMsgMult() failed");
        SPutMsg(mBuf); //deallocate the message back
        return ((s8*) NULL);
    }

    //SPrntMsg(mBuf,0,0);

    if(true == msgCategory)
    {
        TRACE_PRINTF("CPASN_LIB25331::Decode_BCCH_BCH: processing message %d", pduNumber);
        if(RRU_MSG_DLBCCHBCH == pduNumber)
        {                
            pdu = (void*)&(decMsgParams.sdu->m.dlBcchBchMsg);
            elmDef = rrBCCH_BCH_Pdu;        

            ret = cmPAsnDecMsgUalign ((void*)(pdu), CM_PASN_USE_MBUF, 
               (void **)&mBuf, &numDecOct, 0, elmDef, 
               CM_PASN_GEN_ERR, &err, pduLength, 
               (CmMemListCp *)&decMsgParams.sdu->memCp, dbgFlag);

            SPutMsg(mBuf); //deallocate the message back

            if (ROK != ret)
            {
                TRACE_PRINTF("msgCategory %d decoding failed", pduNumber);
                return ((s8*) NULL);        
            }

            TRACE_PRINTF("cmPAsnDecMsgUalign() returned success numDecOct=%ld\n",numDecOct);

            return (s8*)&(decMsgParams.sdu->m.dlBcchBchMsg.msg);            
        }
    }
    else
    {
        TRACE_PRINTF("CPASN_LIB25331::Decode_BCCH_BCH: processing system Info Type %d", pduNumber);    
        switch(pduNumber)
        {
            case RruSIB_TypmastInfBlockEnum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.mib);
                elmDef = rrMasteInfBlock_Pdu;        
            break;

            case RruSIB_TypsysInfoBlockTyp1Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib1);    
                elmDef = rrSysInfoTyp1_Pdu;
            break;

            case RruSIB_TypsysInfoBlockTyp2Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib2);
                elmDef = rrSysInfoTyp2_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp3Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib3);
                elmDef = rrSysInfoTyp3_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp4Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib4);
                elmDef = rrSysInfoTyp4_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp5Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib5);
                elmDef = rrSysInfoTyp5_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp6Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib6);
                elmDef = rrSysInfoTyp6_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp7Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib7);
                elmDef = rrSysInfoTyp7_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp11Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib11);
                elmDef = rrSysInfoTyp11_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp12Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib12);
                elmDef = rrSysInfoTyp12_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp13Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib13);
                elmDef = rrSysInfoTyp13_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp13_1Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib13_1);
                elmDef = rrSysInfoTyp13_1_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp13_2Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib13_2);
                elmDef = rrSysInfoTyp13_2_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp13_3Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib13_3);
                elmDef = rrSysInfoTyp13_3_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp13_4Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib13_4);
                elmDef = rrSysInfoTyp13_4_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp15Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib15);
                elmDef = rrSysInfoTyp15_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp15_1Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib15_1);
                elmDef = rrSysInfoTyp15_1_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp15_2Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib15_2);
                elmDef = rrSysInfoTyp15_2_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp15_3Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib15_3);
                elmDef = rrSysInfoTyp15_3_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp15_4Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib15_4);
                elmDef = rrSysInfoTyp15_4_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp15_5Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib15_5);
                elmDef = rrSysInfoTyp15_5_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp16Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib16);
                elmDef = rrSysInfoTyp16_Pdu;
                break;

            case RruSIB_TypsysInfoBlockTyp18Enum:
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sib18);
                elmDef = rrSysInfoTyp18_Pdu;
                break;

         case RruSIB_TypschengBlock1Enum: 
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sb1);
                elmDef = rrSysInfoTypSB1_Pdu;
                break;

         case RruSIB_TypschengBlock2Enum: 
                pdu = (void*)&(decMsgParams.sdu->m.sibMsg.m.sb2);
                elmDef = rrSysInfoTypSB2_Pdu;
                break;        

            default:
                return (s8*) NULL;
            break;
        }

        ret = cmPAsnDecMsgUalign ((void*)(pdu), CM_PASN_USE_MBUF, 
           (void **)&mBuf, &numDecOct, 0, elmDef, 
           CM_PASN_GEN_ERR, &err, pduLength, 
           (CmMemListCp *)&decMsgParams.sdu->memCp, dbgFlag);

        SPutMsg(mBuf); //deallocate the message back

        if (ROK != ret)
        {
            TRACE_PRINTF("pduNumber %d decoding failed", pduNumber);
            return ((s8*) NULL);
        }            

        return (s8*)(pdu);
    }

    return (s8*) NULL;    
}
