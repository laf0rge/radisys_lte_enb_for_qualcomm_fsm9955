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
     File:     rl_rlog.c
     Sid:      lf_platform.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:57:02 2014
     Prg:      Total eNB
	  Auth:		Vikram Korde, Aug 2013

*********************************************************************21*/
/**************************************************************************
@ author : Vikram Korde  
@ description: This is source file which has implementaion of logging 
framework.
****************************************************************************/

#include "rl_interface.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#ifndef LTE_QCOM_L2_RLOG
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/time.h>
#endif
#include "rl_rlog.h"
#include "rl_platform.h"

/*	VARIABLE DECLARATION SECTION */

/* Thread-specific data key visible to all threads */
#ifndef LTE_QCOM_L2_RLOG
static pthread_key_t	g_threadkey;

extern pthread_mutex_t g_logmutex;
extern THREAD_DATA* g_pCirList[RLOG_MAX_THREADS];
void* rlAlloc(size_t mem_size)
{
	return malloc(mem_size);
}

void rlFree(void* pMem)
{
	free(pMem);
}

void* rlCalloc(size_t mem_size)
{
	return calloc(mem_size, 1);
}

void rlSetThreadSpecificData(const void *pThrData)
{
	int retVal = pthread_setspecific(g_threadkey, pThrData);
	
	if( retVal!=0 ) {
      fprintf(stderr, "Failed to associate the value with the key or invalid key");
      _exit(0);
   }
}

#ifdef RLOG_USE_CIRCULAR_BUFFER
/*******************************************************************************************
@param[in] pThreadData - Thread specific data
@brief This function is called whenever thread is being destroyed. This function will delete 
thread specific data allocated during thread registration.
********************************************************************************************/
void deInitThread(void* pThreadData)
{

	THREAD_DATA* pThrData = (THREAD_DATA*)(pThreadData);

	if( pThreadData == NULL )
		return;

	/* lock the mutex, to make sure no one is accessing this buffer */
	pthread_mutex_lock(&g_logmutex);

	g_pCirList[pThrData->listIndex]  = NULL;

	if( pThrData->logBuff != NULL )
		rlFree(pThrData->logBuff);

	rlFree(pThreadData);

	/* unlock the mutex */
	pthread_mutex_unlock(&g_logmutex);
}
#endif

void* rlGetThreadSpecData(void)
{
	return (void*) pthread_getspecific(g_threadkey);
}

void rlInitPlatformSpecific(void)
{
#ifdef RLOG_USE_CIRCULAR_BUFFER
	pthread_key_create(&g_threadkey, &deInitThread);
#endif
}
#endif

