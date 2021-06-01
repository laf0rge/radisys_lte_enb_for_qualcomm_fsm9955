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
     Type:     C include file
     Desc:     This file contains logging framework include file.
     File:     rl_interface.h
     Sid:      rl_redirect.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 21:19:44 2015
     Prg:      Total eNB
     Auth:		Vikram Korde, Aug 2013

*********************************************************************21*/
///////////////////////////////////////////////////////////////////////////////
// @ author : Vikram Korde  
// @ description: This is header file is used by applications who want to 
// define LOG Macros. This file can be refered for integrating log library
// into any application. API's defined in the file should be used by the 
// program to modify data at runtime.
///////////////////////////////////////////////////////////////////////////////

#ifndef __RL_REDIRECT_H
#define __RL_REDIRECT_H

#ifdef NH_REDIRECT_RLOGS		/* NH */
#define LOG_ARGN(_level, _fmtStr, ...) DBGP(&nhCb.init, "RRC:", 1, (NH_PRNT_BUF, _fmtStr"\n", ##__VA_ARGS__))
#define LOG_SPL(_level, _splenum, _splArg, _fmtStr, ...) DBGP(&nhCb.init, "RRC:", 1, (NH_PRNT_BUF, _fmtStr " %s:%ld\n", ##__VA_ARGS__,  g_splStr[_splenum], _splArg))
#endif				/* NH */

#ifdef WR_REDIRECT_RLOGS			/* WR */
#define LOG_ARGN(_level, _fmtStr, ...) DBGP_CIRLOG(&wrCb.init, "ENBAPP:", 1, (WR_PRNT_BUF,_fmtStr"\n", ##__VA_ARGS__))
//#define LOG_ARG0(_level, _fmtStr) DBGP_CIRLOG(&wrCb.init, "ENBAPP:", 1, (WR_PRNT_BUF, _fmtStr "\n"))
#define LOG_SPL(_level, _splenum, _splArg, _fmtStr, ...) DBGP_CIRLOG(&wrCb.init, "ENBAPP:", 1, (WR_PRNT_BUF, _fmtStr " %s:%ld\n", g_splStr[_splenum], _splArg, ##__VA_ARGS__))
#endif

//#define RLOG0(_level, _lstr) 											LOG_ARG0(_level, _lstr)
#define RLOG0(_level, _lstr) 											LOG_ARGN(_level, _lstr)
#define RLOG1(_level, _lstr, _arg1) 								LOG_ARGN(_level, _lstr, _arg1)
#define RLOG2(_level, _lstr, _arg1, _arg2) 						LOG_ARGN(_level, _lstr, _arg1, _arg2)
#define RLOG3(_level, _lstr, _arg1, _arg2, _arg3) 				LOG_ARGN(_level, _lstr, _arg1, _arg2, _arg3)
#define RLOG4(_level, _lstr, _arg1, _arg2, _arg3, _arg4) 	LOG_ARGN(_level, _lstr, _arg1, _arg2, _arg3, _arg4)

#define RLOG_STR(_level, _lstr, _strarg)					LOG_ARGN(_level, _lstr, _strarg)
#define RLOG_HEX(_level, _lstr, _hexdata, _hexlen)		LOG_ARGN(_level, _lstr, _hexdata, _hexlen)

#define RLOG_ARG0(_level, _splenum, _splArg, _lstr) \
		LOG_SPL(_level, _splenum, _splArg, _lstr, 0, 0, 0, 0)

#define RLOG_ARG1(_level, _splenum, _splArg, _lstr, _arg1) \
		LOG_SPL(_level, _splenum, _splArg, _lstr, _arg1, 0, 0, 0)

#define RLOG_ARG2(_level, _splenum, _splArg, _lstr, _arg1, _arg2) \
		LOG_SPL(_level, _splenum, _splArg, _lstr, _arg1, _arg2, 0, 0)

#define RLOG_ARG3(_level, _splenum, _splArg, _lstr, _arg1, _arg2, _arg3) \
		LOG_SPL(_level, _splenum, _splArg, _lstr, _arg1, _arg2, _arg3, 0)

#define RLOG_ARG4(_level, _splenum, _splArg, _lstr, _arg1, _arg2, _arg3, _arg4) \
		LOG_SPL(_level, _splenum, _splArg, _lstr, _arg1, _arg2, _arg3, _arg4)

#endif // __RLOG_REDIRECT_H
