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

     Name:     Radisys Logging Framework
     Type:     C source file
     Desc:     This file contains logging framework implementation.
     File:     rl_soc.c
     Sid:      rl_soc_brcm.h@@/main/tenb_5.0_RIB_GA/1 - Mon Feb 15 12:52:13 2016
     Prg:      Total eNB
	  Auth:		Sushant , Feb 2015

*********************************************************************21*/
/**********************************************************************
 @ author : Sushant Kumar 
 @ description: This source file has SOC specific functions required for 
 logging framework
************************************************************************/

#ifdef LTE_QCOM
#define TENB_L2_PROC_ID 0
#define RLOG_LIMIT_L2_COUNT 40
#define L2LOG_BUFF_SIZE 8192
#else
#define TENB_L2_PROC_ID 1
#define RLOG_LIMIT_L2_COUNT 45
#define L2LOG_BUFF_SIZE 10000
#endif
#define L2LOG_BUFF_BLOCK_SIZE L2LOG_BUFF_SIZE/4
#define RLOGTICKSCNTTOPRCL2LOGS 10
#define RLOG_LIMIT_L3_COUNT 500

PUBLIC void rlInitL2SocSpecific(void);
PUBLIC void processL2LogBuff(void);
PUBLIC void rlGetL2LogBufPtr(void *mBuf,U32 *logLen,Data **logPtr);
PUBLIC void rlInvalidateL2LogsInCache(Data *ptr,U32   len);

extern void rlInitL2SocSpecific(void);
extern void processL2LogBuff(void);
extern void rlGetL2LogBufPtr(void *mBuf,U32 *logLen,Data **logPtr);

