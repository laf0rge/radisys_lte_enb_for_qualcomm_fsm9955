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
  
        Name:    LTE PDCP Interface  
    
        Type:    C include file
  
        Desc:    This file contains the defines required by PDCP user
 
        File:    kw_pj_udx.h

        Sid:      pj_udx.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:51 2015
  
        Prg:     
  
*********************************************************************21*/
#ifndef __UDX_H__
#define __UDX_H__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*for extern "C"*/

/**
 * @file 
 * @brief Macro for UDX Interface 
 */

#define UDX_SEL_LC           0   /* UDX loose couple selector */
#define UDX_SEL_LWLC         1
#define PJ_MAX_UDX           3

#define UDX_7BIT_SN_SIZE     7               /*!< Sequence length is 7-bit */
#define UDX_12BIT_SN_SIZE    12              /*!< Sequence length is 12-bit */
#define UDX_MAX_CFG    10                    /*!< Maximum PDCP entity to be configured */ 
#define UDX_MAX_DRB    12                    /*!< Maximum DRBs per UE */

/* UDX Configuration Failure Reasons */
#define UDX_CFG_REAS_NONE                  0  /*!< No Failure */
#define UDX_CFG_REAS_ENT_CREAT_FAIL        1  /*!< PDCP Entity Creation Failure*/
#define UDX_CFG_REAS_UE_CREAT_FAIL         2  /*!< UE CB Creation Failure */
#define UDX_CFG_REAS_INVALID_PDCPID        3  /*!< Invalid PDCP ID */
#define UDX_CFG_REAS_PDCPID_PRSNT          4  /*!< PDCP ID already present */
#define UDX_CFG_REAS_INVALID_RLCMODE       5  /*!< Invalid RLC Mode */
#define UDX_CFG_REAS_PDCPID_UNKWN          6  /*!< PDCP ID Unknown */
#define UDX_CFG_REAS_UE_UNKWN              7  /*!< UE Cb Unknown */
#define UDX_CFG_REAS_CIPHER_ALGO_INVALID   8  /*!< Invalid Cipher Algo */
#define UDX_CFG_REAS_ENT_DEL_FAIL          9  /*!< PDCP Entity Deletion Failure */
#define UDX_CFG_REAS_UE_DEL_FAIL           10 /*!< UE Cb Deletion Failure */
#define UDX_CFG_REAS_REEST_FAIL            11 /*!< PDCP Reestablish Failure */
#define UDX_CFG_REAS_COMP_ALGO_INVALID     12 /*!< Compression Algo not
                                               supported */
#define UDX_CFG_REAS_RECFG_FAIL            13 /*!< PDCP Reconfiguration Failed */
#define UDX_CFG_REAS_INVALID_CFG           14 /*!< Invalid Configuration Type */
#define UDX_CFG_REAS_SAME_UEID             15 /*!< UE IDs match in ChngUeId 
                                                   Request*/
#define UDX_CFG_REAS_SAME_RBID             16 /*!< UE IDs match in ChngUeId 
                                                   Request */
#define UDX_CFG_REAS_INVALID_RBID          17 /*!< Invalid RB ID */

#define  UDX_CFG_REAS_INTINIT_FAILED      18 /*!< Integrity Init Failed */
#define  UDX_CFG_REAS_CCIPHR_FAILED       19 /*!< Control Plane Cihper Init Failed */
#define  UDX_CFG_REAS_UCIPHR_FAILED       20 /*!< User Plane Cipher Init Failed */

#define  UDX_CFG_REAS_NO_DRBS             21 /*!< No DRBs Present */
#define  UDX_CFG_REAS_OBD_TIMEOUT         22 /*!< No DRBs Present */
#define  UDX_CFG_REAS_INVALID_STATE       23 /*!< Invalid state to perform re-establishment */

/* UDX Configuration confirmations */
#define UDX_CFG_CFM_OK                      1 /*!< Confirm status OK */
#define UDX_CFG_CFM_NOK                     2 /*!< Confirm status NOK */
   
/*Event Code*/
#define UDX_EVC_RESET_FAILED                1 /*!< Reset Failed */ 

#define EVTUDXBNDREQ         0x20            /*!< Bind Request */
#define EVTUDXBNDCFM         0x21            /*!< Bind Confirmation */ 
#define EVTUDXUBNDREQ        0x22            /*!< Unbind Request */
#define EVTUDXCFGREQ         0x23            /*!< Config Request */
#define EVTUDXCFGCFM         0x24            /*!< Config Confirmation */
#define EVTUDXUEIDCHGREQ     0x25            /*!< UE ID Change Request */ 
#define EVTUDXUEIDCHGCFM     0x26            /*!< UE ID Change Confirmation */
#define EVTUDXSECCFGREQ      0x27            /*!< Security Config Request */
#define EVTUDXSECCFGCFM      0x28            /*!< Security Config Confirm */
#define EVTUDXREESTREQ       0x29            /*!< Count C Request */
#define EVTUDXREESTCFM       0x2A            /*!< Count C Confirm */
#define EVTUDXCOUNTREQ       0x2B            /*!< Count C Request */
#define EVTUDXCOUNTCFM       0x2C            /*!< Count C Confirm */
#define EVTUDXSDUSTAREQ      0x2D            /*!< SDU Status Request */
#define EVTUDXSDUSTACFM      0x2E            /*!< SDU Status Confirm */
#define EVTUDXREESTTRIG      0x2F            /*!< Reestablishment trigger */
#define EVTUDXULSTAREP       0x30            /*!< UL PDCP Status Report */
#define EVTUDXDLSTAREP       0x31            /*!< DL PDCP Status Report */
#define EVTUDXULROHCFDB      0x32            /*!< ROHC FEEDBACK PKT INFO */
#define EVTUDXDLROHCFDB      0x33            /*!< ROHC FEEDBACK PKT INFO */
#define EVTUDXDATRESUME      0x34            /*!< Data Resume */
#define EVTUDXDATFRWD        0x35            /*!< Data Forwarding */
#define EVTUDXDATRESUMECFM   0x36            /*!< Dats Resume Confirm */
#define EVTUDXDATFRWDCFM     0x37            /*!< Dats Forwarding Confirm */


#define UDX_CIPHER_KEY_LEN       16          /*!< Ciphering key length */
#define UDX_INTG_KEY_LEN         16          /*!< Integrity key length */
#define UDX_MAX_ROHC_PROF_LIST   9           /*!< ROHC profile list */
#define UDX_MAX_CFM              10          /*!< Maximum number of confirm */ 

#define UDX_CFG_ADD            0x01          /*!< Configure new PDCP entity
                                               in the requested UE */
#define UDX_CFG_MODIFY         0x02          /*!< Reconfigure PDCP entity
                                               in the requested UE */
#define UDX_CFG_DELETE         0x03          /*!< Delete PDCP entity
                                               in the requested UE */
#define UDX_CFG_REESTABLISH    0x04          /*!< Reestablish PDCP entity
                                               in the requested UE */
#define UDX_CFG_DELETE_UE      0x05          /*!< Delete UE in PDCP */


#define UDX_SEC_INTG_INFO      0x01          /*!< Integrity information */
#define UDX_SEC_CIPHER_INFO    0x02          /*!< Ciphering information */

#define UDX_SEC_ALG_NULL       0x01          /*!< NULL algorithm */
#define UDX_SEC_ALG_SNOW3G     0x02          /*!< Snow 3G algorithm */
#define UDX_SEC_ALG_AES        0x03          /*!< AES algorithm */



/* Defines which is used in ModCfg to set the bitflag */
#define UDX_RECFG_DISRQD  0x01               /*!< Discard Required is present*/
#define UDX_RECFG_CFMREQ  0x02               /*!< Confirm Required is present */

/* Flags will be used to fill the Hand off information */
#define UDX_HO_NOT_PRSNT  0       /*!< Hand of Info is not present */
#define UDX_HO_UL         1       /*!< Uplink count present */
#define UDX_HO_DL         2       /*!< Downlink count present */

/* cpj_h_001.main_2 added support for L2 Measurement */   
#ifdef LTE_L2_MEAS
#define UDX_MAX_QCI           10
#endif

/* Error Codes */
#define EUDXXXX              0   
#define ERRUDX               0

#define EUDX001         (ERRUDX + 1)
#define EUDX002         (ERRUDX + 2)
#define EUDX003         (ERRUDX + 3)
#define EUDX004         (ERRUDX + 4)
#define EUDX005         (ERRUDX + 5)
#define EUDX006         (ERRUDX + 6)
#define EUDX007         (ERRUDX + 7)
#define EUDX008         (ERRUDX + 8)
#define EUDX009         (ERRUDX + 9)
#define EUDX010         (ERRUDX + 10)
#define EUDX011         (ERRUDX + 11)
#define EUDX012         (ERRUDX + 12)
#define EUDX013         (ERRUDX + 13)
#define EUDX014         (ERRUDX + 14)
#define EUDX015         (ERRUDX + 15)
#define EUDX016         (ERRUDX + 16)
#define EUDX017         (ERRUDX + 17)
#define EUDX018         (ERRUDX + 18)
#define EUDX019         (ERRUDX + 19)
#define EUDX020         (ERRUDX + 20)
#define EUDX021         (ERRUDX + 21)
#define EUDX022         (ERRUDX + 22)
#define EUDX023         (ERRUDX + 23)
#define EUDX024         (ERRUDX + 24)
#define EUDX025         (ERRUDX + 25)
#define EUDX026         (ERRUDX + 26)
#define EUDX027         (ERRUDX + 27)
#define EUDX028         (ERRUDX + 28)
#define EUDX029         (ERRUDX + 29)
#define EUDX030         (ERRUDX + 30)
#define EUDX031         (ERRUDX + 31)
#define EUDX032         (ERRUDX + 32)
#define EUDX033         (ERRUDX + 33)
#define EUDX034         (ERRUDX + 34)
#define EUDX035         (ERRUDX + 35)
#define EUDX036         (ERRUDX + 36)
#define EUDX037         (ERRUDX + 37)
#define EUDX038         (ERRUDX + 38)
#define EUDX039         (ERRUDX + 39)
#define EUDX040         (ERRUDX + 40)
#define EUDX041         (ERRUDX + 41)
#define EUDX042         (ERRUDX + 42)
#define EUDX043         (ERRUDX + 43)
#define EUDX044         (ERRUDX + 44)
#define EUDX045         (ERRUDX + 45)
#define EUDX046         (ERRUDX + 46)
#define EUDX047         (ERRUDX + 47)
#define EUDX048         (ERRUDX + 48)
#define EUDX049         (ERRUDX + 49)
#define EUDX050         (ERRUDX + 50)
#define EUDX051         (ERRUDX + 51)
#define EUDX052         (ERRUDX + 52)
#define EUDX053         (ERRUDX + 53)
#define EUDX054         (ERRUDX + 54)
#define EUDX055         (ERRUDX + 55)
#define EUDX056         (ERRUDX + 56)
#define EUDX057         (ERRUDX + 57)
#define EUDX058         (ERRUDX + 58)
#define EUDX059         (ERRUDX + 59)
#define EUDX060         (ERRUDX + 60)
#define EUDX061         (ERRUDX + 61)
#define EUDX062         (ERRUDX + 62)
#define EUDX063         (ERRUDX + 63)
#define EUDX064         (ERRUDX + 64)
#define EUDX065         (ERRUDX + 65)
#define EUDX066         (ERRUDX + 66)
#define EUDX067         (ERRUDX + 67)
#define EUDX068         (ERRUDX + 68)
#define EUDX069         (ERRUDX + 69)
#define EUDX070         (ERRUDX + 70)
#define EUDX071         (ERRUDX + 71)
#define EUDX072         (ERRUDX + 72)
#define EUDX073         (ERRUDX + 73)
#define EUDX074         (ERRUDX + 74)
#define EUDX075         (ERRUDX + 75)
#define EUDX076         (ERRUDX + 76)
#define EUDX077         (ERRUDX + 77)
#define EUDX078         (ERRUDX + 78)
#define EUDX079         (ERRUDX + 79)
#define EUDX080         (ERRUDX + 80)
#define EUDX081         (ERRUDX + 81)
#define EUDX082         (ERRUDX + 82)
#define EUDX083         (ERRUDX + 83)
#define EUDX084         (ERRUDX + 84)
#define EUDX085         (ERRUDX + 85)
#define EUDX086         (ERRUDX + 86)
#define EUDX087         (ERRUDX + 87)
#define EUDX088         (ERRUDX + 88)
#define EUDX089         (ERRUDX + 89)
#define EUDX090         (ERRUDX + 90)
#define EUDX091         (ERRUDX + 91)
#define EUDX092         (ERRUDX + 92)
#define EUDX093         (ERRUDX + 93)
#define EUDX094         (ERRUDX + 94)
#define EUDX095         (ERRUDX + 95)
#define EUDX096         (ERRUDX + 96)
#define EUDX097         (ERRUDX + 97)
#define EUDX098         (ERRUDX + 98)
#define EUDX099         (ERRUDX + 99)
#define EUDX100         (ERRUDX + 100)
#define EUDX101         (ERRUDX + 101)
#define EUDX102         (ERRUDX + 102)
#define EUDX103         (ERRUDX + 103)
#define EUDX104         (ERRUDX + 104)
#define EUDX105         (ERRUDX + 105)
#define EUDX106         (ERRUDX + 106)
#define EUDX107         (ERRUDX + 107)
#define EUDX108         (ERRUDX + 108)
#define EUDX109         (ERRUDX + 109)
#define EUDX110         (ERRUDX + 110)
#define EUDX111         (ERRUDX + 111)
#define EUDX112         (ERRUDX + 112)
#define EUDX113         (ERRUDX + 113)
#define EUDX114         (ERRUDX + 114)
#define EUDX115         (ERRUDX + 115)
#define EUDX116         (ERRUDX + 116)
#define EUDX117         (ERRUDX + 117)
#define EUDX118         (ERRUDX + 118)
#define EUDX119         (ERRUDX + 119)
#define EUDX120         (ERRUDX + 120)
#define EUDX121         (ERRUDX + 121)
#define EUDX122         (ERRUDX + 122)


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UDX_H__ */

/********************************************************************30**
  
         End of file:     pj_udx.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:51 2015
  
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
/main/1      ---        dm            1. Initial Release.
/main/2      ---        gk            1. LTERLC Release 2.1.
/main/3 cpj_h_001.main_2 ap           1. Added support for L2 Measurement.
*********************************************************************91*/
