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

     Name:

     Type:     H File.

     Desc:    

     File:     rb_common.h

     Sid:      rb_common.h@@/main/TeNB_Main_BR/1 - Wed Nov 27 19:56:45 2013

     Prg:

************************************************************************/

#ifndef RB_COMMON_H
#define RB_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Permitted range of instance id's of cores is between 0x33 and 0x64 */
#define CORE_0_ID 0
#define CORE_1_ID 1
#define CORE_2_ID 2
#define CORE_3_ID 3
#define RB_END 999

#ifndef CPU_NUM
#define CPU_NUM 4
#endif
#define MAX_ICPU_BUF_SIZE       10 * 1024
#define MAX_NUM_CORES           CPU_NUM
#define SVSR_CHANNEL            0
#define MT_PROCESSOR_ID_LOW     0
#define MT_PROCESSOR_ID_HIGH    0
#define IARM_PROCESSOR_ID_LOW   1
#define IARM_PROCESSOR_ID_HIGH  1

/*The IIDs from 51 to (51 + twice the number of cores)  are
*  reserved for the SSI application
* Permitted range of instance id's of cores is between 51 and 100
*/
#define IID_CORE 51
#define IID_CORE_NUM (IID_CORE + (2 * MAX_NUM_CORES))

#define TST0ENT      ENTSS+ 1  /* test 0 entity */
#define TST1ENT      ENTSS+ 2  /* test 1 entity */
#define TST2ENT      ENTSS+ 3  /* test 2 entity */
#define TST3ENT      ENTSS+ 4  /* test 3 entity */
#define TSTINST0     0         /* test instance 0 */
#define TSTINST1     1         /* test instance 1 */

/* User need to bind the TAPA tasks with
 * corresponding lower arm core id's.
 */

/*Entitiy, Instance, CoreID(0 to MAX_NUM_CORES)*/
#ifdef RB_ACC_TEST
#ifdef RB_SINGLE_CORE
#define ICPUINIT {\
    {TST0ENT, TSTINST0, CORE_3_ID},\
    {TST1ENT, TSTINST0, CORE_3_ID},\
    {TST2ENT, TSTINST0, CORE_3_ID},\
    {TST3ENT, TSTINST0, CORE_3_ID},\
    {RB_END, RB_END, RB_END}\
}
#else
#define ICPUINIT {\
    {TST0ENT, TSTINST0, CORE_0_ID},\
    {TST1ENT, TSTINST0, CORE_1_ID},\
    {TST2ENT, TSTINST0, CORE_2_ID},\
    {TST3ENT, TSTINST0, CORE_3_ID},\
    {RB_END, RB_END, RB_END}\
}
#endif
#else
#ifndef CL_MAC_LWLC
#define ICPUINIT {\
    {ENTKW, 0, CORE_3_ID},\
    {ENTPJ, 0, CORE_3_ID},\
    {ENTRG, 0, CORE_3_ID},\
    {ENTRG, 1, CORE_3_ID},\
    {ENTYS, 0, CORE_3_ID},\
    {ENTTF, 0, CORE_3_ID},\
    {RB_END, RB_END, RB_END}\
}
#else
#define ICPUINIT {\
    {ENTKW, 0, CORE_2_ID},\
    {ENTPJ, 0, CORE_1_ID},\
    {ENTRG, 0, CORE_2_ID},\
    {ENTRG, 1, CORE_3_ID},\
    {ENTYS, 0, CORE_3_ID},\
    {ENTTF, 0, CORE_3_ID},\
    {ENTKW, 1, CORE_1_ID},\
    {ENTYS, 1, CORE_1_ID},\
    {RB_END, RB_END, RB_END}\
}
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /*RB_COMMON_H*/


/********************************************************************30**

         End of file:     rb_common.h@@/main/TeNB_Main_BR/1 - Wed Nov 27 19:56:45 2013

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

   ver        pat        init                  description
---------  ---------  ----------  -------------------------------------
/main/1      --------     ------    1. Initial release
*********************************************************************91*/
