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
               

     File:     wr_dbg_log.h

     Sid:      wr_dbg_log.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:56:05 2015

     Prg:      sasahni

*********************************************************************21*/

#ifndef __WR_DBG_LOG_H__
#define __WR_DBG_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WR_DBG_CIRLOG

#define MAX_FILES_ALLOWED         10
#define MAX_NUM_LOGS_ALLOWED      20000
#define MAX_FILENAME_LEN          512
/* Max file size in bytes. Currently defined max file size as 3 MB */
#define MAX_FILE_SIZE             3145728

/* Macro MAX_LOG_BUF_SIZE should be a perfect power of 2 */
#define MAX_CIRBUF_LEN            1048576
#define MAX_LOG_BUF_SIZE          256
#define MAX_TIME_STAMP            80
#define WR_DBG_CIRLOG_TIMESTAMP   1

#define WR_CIRBUF_REGION          0
#define WR_CIRBUF_POOL            0
/* Macro defines the maximum log types 
*  - Console 
*  - Circular Buffer
*/
#define MAX_LOG_TYPE 2

typedef struct wrDbgDebgFP
{
   FILE        *dbgLogFp[MAX_FILES_ALLOWED];
   U8          currentWritingFile;
   U32         spaceLeftInFile[MAX_FILES_ALLOWED];
}WrDbgFP;
typedef struct wrDbgCirBuf
{
   Txt    *bufFront;       /*!< Start pointer of the circular buffer */
   Txt    *bufRear;        /*!< End pointer of the circular buffer   */
   Txt    *bufHead;        /*!< Start pointer of the circular buffer */
   Txt    *bufTail;        /*!< End pointer of the circular buffer   */
   U32     bufLen;         /*!< length of circular buffer            */
   Txt    *dbgLogBuf;      /*!< Log buffer used to store values      */
}WrDbgCirBuf;

typedef S32 (*WrSetPF)(TskInit*, S8*, S32, S8*);
typedef S32 (*WrPrntPF)(S8*, S32, S8*,...);

/* extern declarations */
EXTERN WrSetPF wrSetPf[]; 
EXTERN WrPrntPF wrPrntPf[];
EXTERN S32 wrSetCons(TskInit *init,S8 *fileName,S32 lineNo, S8 *layerName);
EXTERN S32 wrSetCirBuf(TskInit *init,S8 *fileName,S32 lineNo, S8 *layerName);
EXTERN S32 wrConsPrnt(S8 *prntBuf, S32 maxLogSize, S8 *fmtStr,...);
EXTERN S32 wrCirBufPrnt(S8 *prntBuf, S32 maxLogSize, S8 *fmtStr,...);

#define DBGP_CIRLOG(_tskInit, _layerName, _msgClass, _arg) \
EXTERN Bool wrInitSuccess;                                               \
if ((_tskInit)->dbgMask & (_msgClass))                                   \
{                                                                        \
   wrSetPf[wrInitSuccess](_tskInit,__FILE__,__LINE__,_layerName);        \
   wrPrntPf[wrInitSuccess] _arg;                                         \
}                                                                        

EXTERN WrDbgCirBuf wrGbDbgCirBuf;
EXTERN WrDbgFP wrGbDbgFP;

EXTERN S16 wrDbgLogInit ARGS((void));
EXTERN S16 wrDbgLogDeinit ARGS((void));
EXTERN S16 wrDbgWriteCircBufToFile ARGS((void));
EXTERN S16 wrDbgWriteToFile ARGS((U32  sizeToCopy,Txt  *startPtr,Txt  *endPtr));
#endif /*WR_DBG_CIRLOG*/
EXTERN S16 wrLogDump ARGS((void));

#ifdef SS_SEGV_SIG_HDLR
#define MAX_BKTRACE_LEVEL 10
EXTERN S16 wrSigSegvActvTsk ARGS((void));
#endif /* SS_SEGV_SIG_HDLR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_DBG_LOG_H__ */


/********************************************************************30**

           End of file:     wr_dbg_log.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:56:05 2015

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
        ---      sasahni         1. initial release logging feature
*********************************************************************91*/
