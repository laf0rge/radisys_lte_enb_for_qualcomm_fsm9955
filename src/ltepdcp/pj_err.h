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
  
        Name:     LTE RLC file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for RLC 
 
        File:     kw_err.h

        Sid:      pj_err.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:47 2015
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw_err.h
@brief RLC Hash definitions
*/

#ifndef __PJ_ERR_H__
#define __PJ_ERR_H__


#if (ERRCLASS & ERRCLS_INT_PAR)
#define PJLOGERROR(_cb, errCls, errCode, errVal, errDesc) \
        SLogError(_cb->init.ent, _cb->init.inst, _cb->init.procId, \
                   __FILE__, __LINE__, errCls, errCode, errVal, errDesc)

#define PJ_GET_AND_VALIDATE_CPJSAP(_cb, _cpjSap, _errCode, _fn, _ret) \
{ \
   _ret = RFAILED; \
   if((_cpjSap != NULLP) && (_cpjSap->state == PJ_SAP_BND)) \
   { \
      _ret = ROK; \
   } \
   if(_ret != ROK) \
   { \
      PJLOGERROR(_cb, ERRCLS_INT_PAR, EPJXXX, (ErrVal) spId,\
            "PjUiCpjBndReq: CPJ SAP State is Invalid during Bind Req");\
      PJ_SEND_SAPID_ALARM(_cb, spId, LPJ_EVENT_CPJ_BND_REQ, LCM_CAUSE_INV_STATE);\
      ret = RFAILED; \
   } \
}
#define PJ_GET_AND_VALIDATE_PJUSAP(_cb, _pjuSap, _errCode, _fn, _ret) \
{ \
   _ret = RFAILED; \
   if((_pjuSap != NULLP) && (_pjuSap->state == PJ_SAP_BND)) \
   { \
      _ret = ROK; \
   } \
   if(_ret != ROK) \
   { \
      PJLOGERROR(_cb, ERRCLS_INT_PAR, EPJXXX, (ErrVal) spId,\
            "PjUiPjuBndReq: PJU SAP State is Invalid during Bind Req");\
      PJ_SEND_SAPID_ALARM(_cb, spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_STATE);\
      _ret = RFAILED; \
   } \
}

#else
#define PJLOGERROR(_cb, errCls, errCode, errVal, errDesc)

#endif

/* Error Codes */
#define   ERRPJ        0
#define   EPJXXX       0


#define   EPJ001      (ERRPJ +    1)    /*     gp_amm.c: 293 */
#define   EPJ002      (ERRPJ +    2)    /*     gp_amm.c: 405 */
#define   EPJ003      (ERRPJ +    3)    /*     gp_amm.c: 419 */
#define   EPJ004      (ERRPJ +    4)    /*     gp_amm.c: 880 */
#define   EPJ005      (ERRPJ +    5)    /*     gp_amm.c:1351 */
#define   EPJ006      (ERRPJ +    6)    /*     gp_amm.c:1367 */
#define   EPJ007      (ERRPJ +    7)    /*     gp_amm.c:1561 */
#define   EPJ008      (ERRPJ +    8)    /*     gp_amm.c:1649 */
#define   EPJ009      (ERRPJ +    9)    /*     gp_amm.c:2423 */
#define   EPJ010      (ERRPJ +   10)    /*     gp_amm.c:2445 */
#define   EPJ011      (ERRPJ +   11)    /*     gp_amm.c:2555 */
#define   EPJ012      (ERRPJ +   12)    /*     gp_amm.c:3047 */
#define   EPJ013      (ERRPJ +   13)    /*     gp_amm.c:3069 */
#define   EPJ014      (ERRPJ +   14)    /*     gp_amm.c:3077 */
#define   EPJ015      (ERRPJ +   15)    /*     gp_amm.c:3147 */
#define   EPJ016      (ERRPJ +   16)    /*     gp_amm.c:3179 */
#define   EPJ017      (ERRPJ +   17)    /*     gp_amm.c:3187 */
#define   EPJ018      (ERRPJ +   18)    /*     gp_amm.c:4489 */

#define   EPJ019      (ERRPJ +   19)    /*     gp_cfg.c: 268 */
#define   EPJ020      (ERRPJ +   20)    /*     gp_cfg.c: 311 */
#define   EPJ021      (ERRPJ +   21)    /*     gp_cfg.c: 368 */
#define   EPJ022      (ERRPJ +   22)    /*     gp_cfg.c: 380 */

#define   EPJ023      (ERRPJ +   23)    /*     gp_dbm.c: 633 */
#define   EPJ024      (ERRPJ +   24)    /*     gp_dbm.c: 958 */
#define   EPJ025      (ERRPJ +   25)    /*     gp_dbm.c:1182 */
#define   EPJ026      (ERRPJ +   26)    /*     gp_dbm.c:1190 */
#define   EPJ027      (ERRPJ +   27)    /*     gp_dbm.c:1198 */
#define   EPJ028      (ERRPJ +   28)    /*     gp_dbm.c:1263 */

#define   EPJ029      (ERRPJ +   29)    /*   gp_ex_ms.c: 317 */
#define   EPJ030      (ERRPJ +   30)    /*   gp_ex_ms.c: 389 */
#define   EPJ031      (ERRPJ +   31)    /*   gp_ex_ms.c: 431 */
#define   EPJ032      (ERRPJ +   32)    /*   gp_ex_ms.c: 481 */
#define   EPJ033      (ERRPJ +   33)    /*   gp_ex_ms.c: 493 */
#define   EPJ034      (ERRPJ +   34)    /*   gp_ex_ms.c: 638 */
#define   EPJ035      (ERRPJ +   35)    /*   gp_ex_ms.c: 730 */
#define   EPJ036      (ERRPJ +   36)    /*   gp_ex_ms.c: 772 */
#define   EPJ037      (ERRPJ +   37)    /*   gp_ex_ms.c: 802 */
#define   EPJ038      (ERRPJ +   38)    /*   gp_ex_ms.c: 849 */
#define   EPJ039      (ERRPJ +   39)    /*   gp_ex_ms.c: 861 */

#define   EPJ040      (ERRPJ +   40)    /*     gp_lim.c: 221 */
#define   EPJ041      (ERRPJ +   41)    /*     gp_lim.c: 231 */

#define   EPJ042      (ERRPJ +   42)    /*     gp_lmm.c: 225 */
#define   EPJ043      (ERRPJ +   43)    /*     gp_lmm.c: 280 */
#define   EPJ044      (ERRPJ +   44)    /*     gp_lmm.c: 418 */
#define   EPJ045      (ERRPJ +   45)    /*     gp_lmm.c: 886 */
#define   EPJ046      (ERRPJ +   46)    /*     gp_lmm.c: 952 */
#define   EPJ047      (ERRPJ +   47)    /*     gp_lmm.c:1016 */
#define   EPJ048      (ERRPJ +   48)    /*     gp_lmm.c:1340 */

#define   EPJ049      (ERRPJ +   49)    /*  gp_pj_cfg.c: 505 */
#define   EPJ050      (ERRPJ +   50)    /*  gp_pj_cfg.c:1276 */

#define   EPJ051      (ERRPJ +   51)    /* gp_pj_cfg.c.tmp: 505 */
#define   EPJ052      (ERRPJ +   52)    /* gp_pj_cfg.c.tmp:1276 */

#define   EPJ053      (ERRPJ +   53)    /*  gp_pj_dlm.c: 243 */
#define   EPJ054      (ERRPJ +   54)    /*  gp_pj_dlm.c: 381 */
#define   EPJ055      (ERRPJ +   55)    /*  gp_pj_dlm.c: 518 */
#define   EPJ056      (ERRPJ +   56)    /*  gp_pj_dlm.c: 564 */
#define   EPJ057      (ERRPJ +   57)    /*  gp_pj_dlm.c: 644 */
#define   EPJ058      (ERRPJ +   58)    /*  gp_pj_dlm.c: 724 */
#define   EPJ059      (ERRPJ +   59)    /*  gp_pj_dlm.c: 864 */
#define   EPJ060      (ERRPJ +   60)    /*  gp_pj_dlm.c: 883 */
#define   EPJ061      (ERRPJ +   61)    /*  gp_pj_dlm.c: 921 */
#define   EPJ062      (ERRPJ +   62)    /*  gp_pj_dlm.c:1036 */
#define   EPJ063      (ERRPJ +   63)    /*  gp_pj_dlm.c:1049 */
#define   EPJ064      (ERRPJ +   64)    /*  gp_pj_dlm.c:1176 */
#define   EPJ065      (ERRPJ +   65)    /*  gp_pj_dlm.c:1226 */
#define   EPJ066      (ERRPJ +   66)    /*  gp_pj_dlm.c:1299 */
#define   EPJ067      (ERRPJ +   67)    /*  gp_pj_dlm.c:1324 */
#define   EPJ068      (ERRPJ +   68)    /*  gp_pj_dlm.c:1466 */
#define   EPJ069      (ERRPJ +   69)    /*  gp_pj_dlm.c:1534 */
#define   EPJ070      (ERRPJ +   70)    /*  gp_pj_dlm.c:1621 */
#define   EPJ071      (ERRPJ +   71)    /*  gp_pj_dlm.c:1637 */
#define   EPJ072      (ERRPJ +   72)    /*  gp_pj_dlm.c:1657 */
#define   EPJ073      (ERRPJ +   73)    /*  gp_pj_dlm.c:1680 */
#define   EPJ074      (ERRPJ +   74)    /*  gp_pj_dlm.c:1750 */
#define   EPJ075      (ERRPJ +   75)    /*  gp_pj_dlm.c:1772 */
#define   EPJ076      (ERRPJ +   76)    /*  gp_pj_dlm.c:1847 */
#define   EPJ077      (ERRPJ +   77)    /*  gp_pj_dlm.c:1961 */

#define   EPJ078      (ERRPJ +   78)    /*  gp_pj_lmm.c: 319 */
#define   EPJ079      (ERRPJ +   79)    /*  gp_pj_lmm.c: 806 */
#define   EPJ080      (ERRPJ +   80)    /*  gp_pj_lmm.c: 870 */

#define   EPJ081      (ERRPJ +   81)    /*  gp_pj_uim.c: 268 */
#define   EPJ082      (ERRPJ +   82)    /*  gp_pj_uim.c: 347 */
#define   EPJ083      (ERRPJ +   83)    /*  gp_pj_uim.c: 444 */
#define   EPJ084      (ERRPJ +   84)    /*  gp_pj_uim.c: 758 */
#define   EPJ085      (ERRPJ +   85)    /*  gp_pj_uim.c: 844 */
#define   EPJ086      (ERRPJ +   86)    /*  gp_pj_uim.c: 937 */
#define   EPJ087      (ERRPJ +   87)    /*  gp_pj_uim.c:1008 */
#define   EPJ088      (ERRPJ +   88)    /*  gp_pj_uim.c:1158 */
#define   EPJ089      (ERRPJ +   89)    /*  gp_pj_uim.c:1221 */
#define   EPJ090      (ERRPJ +   90)    /*  gp_pj_uim.c:1305 */
#define   EPJ091      (ERRPJ +   91)    /*  gp_pj_uim.c:1412 */
#define   EPJ092      (ERRPJ +   92)    /*  gp_pj_uim.c:1454 */
#define   EPJ093      (ERRPJ +   93)    /*  gp_pj_uim.c:1566 */

#define   EPJ094      (ERRPJ +   94)    /* gp_pj_uim.c.tmp: 268 */
#define   EPJ095      (ERRPJ +   95)    /* gp_pj_uim.c.tmp: 347 */
#define   EPJ096      (ERRPJ +   96)    /* gp_pj_uim.c.tmp: 444 */
#define   EPJ097      (ERRPJ +   97)    /* gp_pj_uim.c.tmp: 758 */
#define   EPJ098      (ERRPJ +   98)    /* gp_pj_uim.c.tmp: 844 */
#define   EPJ099      (ERRPJ +   99)    /* gp_pj_uim.c.tmp: 937 */
#define   EPJ100      (ERRPJ +  100)    /* gp_pj_uim.c.tmp:1008 */
#define   EPJ101      (ERRPJ +  101)    /* gp_pj_uim.c.tmp:1158 */
#define   EPJ102      (ERRPJ +  102)    /* gp_pj_uim.c.tmp:1221 */
#define   EPJ103      (ERRPJ +  103)    /* gp_pj_uim.c.tmp:1305 */
#define   EPJ104      (ERRPJ +  104)    /* gp_pj_uim.c.tmp:1412 */
#define   EPJ105      (ERRPJ +  105)    /* gp_pj_uim.c.tmp:1454 */
#define   EPJ106      (ERRPJ +  106)    /* gp_pj_uim.c.tmp:1566 */

#define   EPJ107      (ERRPJ +  107)    /*  gp_pj_ulm.c: 482 */
#define   EPJ108      (ERRPJ +  108)    /*  gp_pj_ulm.c: 812 */
#define   EPJ109      (ERRPJ +  109)    /*  gp_pj_ulm.c: 828 */
#define   EPJ110      (ERRPJ +  110)    /*  gp_pj_ulm.c: 943 */
#define   EPJ111      (ERRPJ +  111)    /*  gp_pj_ulm.c: 972 */
#define   EPJ112      (ERRPJ +  112)    /*  gp_pj_ulm.c:1047 */
#define   EPJ113      (ERRPJ +  113)    /*  gp_pj_ulm.c:1064 */
#define   EPJ114      (ERRPJ +  114)    /*  gp_pj_ulm.c:2070 */
#define   EPJ115      (ERRPJ +  115)    /*  gp_pj_ulm.c:2077 */

#define   EPJ116      (ERRPJ +  116)    /*  gp_pj_utl.c: 213 */
#define   EPJ117      (ERRPJ +  117)    /*  gp_pj_utl.c: 221 */
#define   EPJ118      (ERRPJ +  118)    /*  gp_pj_utl.c: 291 */
#define   EPJ119      (ERRPJ +  119)    /*  gp_pj_utl.c: 356 */
#define   EPJ120      (ERRPJ +  120)    /*  gp_pj_utl.c: 421 */
#define   EPJ121      (ERRPJ +  121)    /*  gp_pj_utl.c: 510 */
#define   EPJ122      (ERRPJ +  122)    /*  gp_pj_utl.c: 525 */
#define   EPJ123      (ERRPJ +  123)    /*  gp_pj_utl.c: 532 */
#define   EPJ124      (ERRPJ +  124)    /*  gp_pj_utl.c: 554 */
#define   EPJ125      (ERRPJ +  125)    /*  gp_pj_utl.c: 640 */
#define   EPJ126      (ERRPJ +  126)    /*  gp_pj_utl.c: 652 */
#define   EPJ127      (ERRPJ +  127)    /*  gp_pj_utl.c: 676 */
#define   EPJ128      (ERRPJ +  128)    /*  gp_pj_utl.c: 782 */
#define   EPJ129      (ERRPJ +  129)    /*  gp_pj_utl.c: 794 */
#define   EPJ130      (ERRPJ +  130)    /*  gp_pj_utl.c: 805 */
#define   EPJ131      (ERRPJ +  131)    /*  gp_pj_utl.c: 826 */
#define   EPJ132      (ERRPJ +  132)    /*  gp_pj_utl.c: 905 */
#define   EPJ133      (ERRPJ +  133)    /*  gp_pj_utl.c: 919 */
#define   EPJ134      (ERRPJ +  134)    /*  gp_pj_utl.c: 926 */
#define   EPJ135      (ERRPJ +  135)    /*  gp_pj_utl.c:1027 */
#define   EPJ136      (ERRPJ +  136)    /*  gp_pj_utl.c:1042 */
#define   EPJ137      (ERRPJ +  137)    /*  gp_pj_utl.c:1070 */
#define   EPJ138      (ERRPJ +  138)    /*  gp_pj_utl.c:1205 */
#define   EPJ139      (ERRPJ +  139)    /*  gp_pj_utl.c:1222 */
#define   EPJ140      (ERRPJ +  140)    /*  gp_pj_utl.c:1232 */
#define   EPJ141      (ERRPJ +  141)    /*  gp_pj_utl.c:2460 */
#define   EPJ142      (ERRPJ +  142)    /*  gp_pj_utl.c:2470 */
#define   EPJ143      (ERRPJ +  143)    /*  gp_pj_utl.c:2642 */
#define   EPJ144      (ERRPJ +  144)    /*  gp_pj_utl.c:2874 */
#define   EPJ145      (ERRPJ +  145)    /*  gp_pj_utl.c:2963 */
#define   EPJ146      (ERRPJ +  146)    /*  gp_pj_utl.c:3153 */
#define   EPJ147      (ERRPJ +  147)    /*  gp_pj_utl.c:3171 */
#define   EPJ148      (ERRPJ +  148)    /*  gp_pj_utl.c:3219 */
#define   EPJ149      (ERRPJ +  149)    /*  gp_pj_utl.c:3322 */
#define   EPJ150      (ERRPJ +  150)    /*  gp_pj_utl.c:3353 */
#define   EPJ151      (ERRPJ +  151)    /*  gp_pj_utl.c:3412 */
#define   EPJ152      (ERRPJ +  152)    /*  gp_pj_utl.c:3449 */
#define   EPJ153      (ERRPJ +  153)    /*  gp_pj_utl.c:3540 */

#define   EPJ154      (ERRPJ +  154)    /* gp_pj_utl.c.tmp: 213 */
#define   EPJ155      (ERRPJ +  155)    /* gp_pj_utl.c.tmp: 221 */
#define   EPJ156      (ERRPJ +  156)    /* gp_pj_utl.c.tmp: 291 */
#define   EPJ157      (ERRPJ +  157)    /* gp_pj_utl.c.tmp: 356 */
#define   EPJ158      (ERRPJ +  158)    /* gp_pj_utl.c.tmp: 421 */
#define   EPJ159      (ERRPJ +  159)    /* gp_pj_utl.c.tmp: 510 */
#define   EPJ160      (ERRPJ +  160)    /* gp_pj_utl.c.tmp: 525 */
#define   EPJ161      (ERRPJ +  161)    /* gp_pj_utl.c.tmp: 532 */
#define   EPJ162      (ERRPJ +  162)    /* gp_pj_utl.c.tmp: 554 */
#define   EPJ163      (ERRPJ +  163)    /* gp_pj_utl.c.tmp: 640 */
#define   EPJ164      (ERRPJ +  164)    /* gp_pj_utl.c.tmp: 652 */
#define   EPJ165      (ERRPJ +  165)    /* gp_pj_utl.c.tmp: 676 */
#define   EPJ166      (ERRPJ +  166)    /* gp_pj_utl.c.tmp: 782 */
#define   EPJ167      (ERRPJ +  167)    /* gp_pj_utl.c.tmp: 794 */
#define   EPJ168      (ERRPJ +  168)    /* gp_pj_utl.c.tmp: 805 */
#define   EPJ169      (ERRPJ +  169)    /* gp_pj_utl.c.tmp: 826 */
#define   EPJ170      (ERRPJ +  170)    /* gp_pj_utl.c.tmp: 905 */
#define   EPJ171      (ERRPJ +  171)    /* gp_pj_utl.c.tmp: 919 */
#define   EPJ172      (ERRPJ +  172)    /* gp_pj_utl.c.tmp: 926 */
#define   EPJ173      (ERRPJ +  173)    /* gp_pj_utl.c.tmp:1027 */
#define   EPJ174      (ERRPJ +  174)    /* gp_pj_utl.c.tmp:1042 */
#define   EPJ175      (ERRPJ +  175)    /* gp_pj_utl.c.tmp:1070 */
#define   EPJ176      (ERRPJ +  176)    /* gp_pj_utl.c.tmp:1205 */
#define   EPJ177      (ERRPJ +  177)    /* gp_pj_utl.c.tmp:1222 */
#define   EPJ178      (ERRPJ +  178)    /* gp_pj_utl.c.tmp:1232 */
#define   EPJ179      (ERRPJ +  179)    /* gp_pj_utl.c.tmp:2460 */
#define   EPJ180      (ERRPJ +  180)    /* gp_pj_utl.c.tmp:2470 */
#define   EPJ181      (ERRPJ +  181)    /* gp_pj_utl.c.tmp:2642 */
#define   EPJ182      (ERRPJ +  182)    /* gp_pj_utl.c.tmp:2874 */
#define   EPJ183      (ERRPJ +  183)    /* gp_pj_utl.c.tmp:2963 */
#define   EPJ184      (ERRPJ +  184)    /* gp_pj_utl.c.tmp:3153 */
#define   EPJ185      (ERRPJ +  185)    /* gp_pj_utl.c.tmp:3171 */
#define   EPJ186      (ERRPJ +  186)    /* gp_pj_utl.c.tmp:3219 */
#define   EPJ187      (ERRPJ +  187)    /* gp_pj_utl.c.tmp:3322 */
#define   EPJ188      (ERRPJ +  188)    /* gp_pj_utl.c.tmp:3353 */
#define   EPJ189      (ERRPJ +  189)    /* gp_pj_utl.c.tmp:3412 */
#define   EPJ190      (ERRPJ +  190)    /* gp_pj_utl.c.tmp:3449 */
#define   EPJ191      (ERRPJ +  191)    /* gp_pj_utl.c.tmp:3540 */

#define   EPJ192      (ERRPJ +  192)    /*    gp_ptui.c: 666 */
#define   EPJ193      (ERRPJ +  193)    /*    gp_ptui.c: 709 */
#define   EPJ194      (ERRPJ +  194)    /*    gp_ptui.c: 752 */
#define   EPJ195      (ERRPJ +  195)    /*    gp_ptui.c: 795 */
#define   EPJ196      (ERRPJ +  196)    /*    gp_ptui.c: 842 */
#define   EPJ197      (ERRPJ +  197)    /*    gp_ptui.c:1081 */
#define   EPJ198      (ERRPJ +  198)    /*    gp_ptui.c:1123 */
#define   EPJ199      (ERRPJ +  199)    /*    gp_ptui.c:1158 */

#define   EPJ200      (ERRPJ +  200)    /*     gp_tmm.c: 213 */
#define   EPJ201      (ERRPJ +  201)    /*     gp_tmm.c: 303 */
#define   EPJ202      (ERRPJ +  202)    /*     gp_tmm.c: 405 */
#define   EPJ203      (ERRPJ +  203)    /*     gp_tmm.c: 519 */

#define   EPJ204      (ERRPJ +  204)    /*     gp_tmr.c: 253 */
#define   EPJ205      (ERRPJ +  205)    /*     gp_tmr.c: 358 */
#define   EPJ206      (ERRPJ +  206)    /*     gp_tmr.c: 554 */

#define   EPJ207      (ERRPJ +  207)    /*     gp_uim.c: 276 */
#define   EPJ208      (ERRPJ +  208)    /*     gp_uim.c: 358 */
#define   EPJ209      (ERRPJ +  209)    /*     gp_uim.c: 437 */
#define   EPJ210      (ERRPJ +  210)    /*     gp_uim.c: 690 */
#define   EPJ211      (ERRPJ +  211)    /*     gp_uim.c: 769 */
#define   EPJ212      (ERRPJ +  212)    /*     gp_uim.c: 832 */
#define   EPJ213      (ERRPJ +  213)    /*     gp_uim.c: 912 */

#define   EPJ214      (ERRPJ +  214)    /*     gp_umm.c: 213 */
#define   EPJ215      (ERRPJ +  215)    /*     gp_umm.c: 706 */
#define   EPJ216      (ERRPJ +  216)    /*     gp_umm.c:1248 */

#define   EPJ217      (ERRPJ +  217)    /*     gp_utl.c: 227 */
#define   EPJ218      (ERRPJ +  218)    /*     gp_utl.c: 237 */
#define   EPJ219      (ERRPJ +  219)    /*     gp_utl.c: 263 */
#define   EPJ220      (ERRPJ +  220)    /*     gp_utl.c: 278 */
#define   EPJ221      (ERRPJ +  221)    /*     gp_utl.c: 383 */
#define   EPJ222      (ERRPJ +  222)    /*     gp_utl.c: 401 */
#define   EPJ223      (ERRPJ +  223)    /*     gp_utl.c: 413 */
#define   EPJ224      (ERRPJ +  224)    /*     gp_utl.c: 473 */
#define   EPJ225      (ERRPJ +  225)    /*     gp_utl.c: 598 */
#define   EPJ226      (ERRPJ +  226)    /*     gp_pj_tmr.c: 291 */
#define   EPJ227      (ERRPJ +  227)    /*     gp_pj_tmr.c: 576 */
#endif /* __KW_ERR_H__ */
  
/********************************************************************30**
  
         End of file:     pj_err.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:47 2015
  
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
/main/1      ---      nm   1. Initial release.
/main/2      ---      nm   1. LTERLC Release 2.1
*********************************************************************91*/
