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

        Sid:      kw_err.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:23:15 2015
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw_err.h
@brief RLC Hash definitions
*/

#ifndef __KW_ERR_H__
#define __KW_ERR_H__

#if (ERRCLASS & ERRCLS_INT_PAR)
#define KWLOGERROR(cb,errCls, errCode, errVal, errDesc) \
        SLogError(cb->init.ent, cb->init.inst, cb->init.procId, \
                   __FILE__, __LINE__, errCls, errCode, errVal, errDesc)

#define KW_GET_AND_VALIDATE_KWUSAP(_spId, _kwuSap, _errCode, _fn) \
{ \
   S16 _ret; \
   _ret = RFAILED; \
   _kwuSap = NULLP; \
   if((_spId < (S16) kwCb.genCfg.maxKwuSaps) && (_spId >= 0)) \
   { \
      _kwuSap = kwCb.kwuSap + _spId; \
      if((_kwuSap != NULLP) && (_kwuSap->state == KW_SAP_BND)) \
      { \
         _ret = ROK; \
      } \
   } \
   if(_ret != ROK) \
   { \
      KWLOGERROR(ERRCLS_INT_PAR, EKWxxx, (ErrVal) spId, \
            "KwUiKwuBndReq: KWU SAP State is Invalid during Bind Req"); \
      KW_SEND_SAPID_ALARM(spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_SAP);\
      RETVALUE(RFAILED); \
   } \
}
#else
#define KW_GET_AND_VALIDATE_KWUSAP(_spId, _kwuSap, _errCode, _fn) \
   _kwuSap = kwCb.kwuSap + _spId;
#define KWLOGERROR(cb, errCls, errCode, errVal, errDesc)

#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
#define KW_GET_AND_VALIDATE_CKWSAP(_cb,_ckwSap, _errCode, _fn) \
{ \
   S16 _ret; \
   _ret = RFAILED; \
   if((_ckwSap != NULLP) && (_ckwSap->state == KW_SAP_BND)) \
   { \
      _ret = ROK; \
   } \
   if(_ret != ROK) \
   { \
      KWLOGERROR(_cb,ERRCLS_INT_PAR, EKWxxx, (ErrVal) spId,\
            "KwUiCkwBndReq: CKW SAP State is Invalid during Bind Req");\
      KW_SEND_SAPID_ALARM(_cb,spId, LKW_EVENT_CKW_BND_REQ, LCM_CAUSE_INV_STATE);\
      RETVALUE(RFAILED); \
   } \
}
#endif
#if (ERRCLASS & ERRCLS_INT_PAR)
#define KW_GET_AND_VALIDATE_UDXSAP(_cb,_udxSap, _errCode, _fn) \
{ \
   S16 _ret; \
   _ret = RFAILED; \
   if((_udxSap != NULLP) && (_udxSap->state == KW_SAP_BND)) \
   { \
      _ret = ROK; \
   } \
   if(_ret != ROK) \
   { \
      KWLOGERROR(_cb,ERRCLS_INT_PAR, EKWxxx, (ErrVal) spId,\
            "KwUiCkwBndReq: UDX SAP State is Invalid during Bind Req");\
      KW_SEND_SAPID_ALARM(_cb,spId, LKW_EVENT_UDX_BND_REQ, LCM_CAUSE_INV_STATE);\
      RETVALUE(RFAILED); \
   } \
}
#endif

/* Error Codes */
#define   ERRKW        0
#define   EKWXXX       0

#define   EKW001      (ERRKW +    1)    /*     gp_amm.c: 293 */
#define   EKW002      (ERRKW +    2)    /*     gp_amm.c: 405 */
#define   EKW003      (ERRKW +    3)    /*     gp_amm.c: 419 */
#define   EKW004      (ERRKW +    4)    /*     gp_amm.c: 880 */
#define   EKW005      (ERRKW +    5)    /*     gp_amm.c:1351 */
#define   EKW006      (ERRKW +    6)    /*     gp_amm.c:1367 */
#define   EKW007      (ERRKW +    7)    /*     gp_amm.c:1561 */
#define   EKW008      (ERRKW +    8)    /*     gp_amm.c:1649 */
#define   EKW009      (ERRKW +    9)    /*     gp_amm.c:2423 */
#define   EKW010      (ERRKW +   10)    /*     gp_amm.c:2445 */
#define   EKW011      (ERRKW +   11)    /*     gp_amm.c:2555 */
#define   EKW012      (ERRKW +   12)    /*     gp_amm.c:3047 */
#define   EKW013      (ERRKW +   13)    /*     gp_amm.c:3069 */
#define   EKW014      (ERRKW +   14)    /*     gp_amm.c:3077 */
#define   EKW015      (ERRKW +   15)    /*     gp_amm.c:3147 */
#define   EKW016      (ERRKW +   16)    /*     gp_amm.c:3179 */
#define   EKW017      (ERRKW +   17)    /*     gp_amm.c:3187 */
#define   EKW018      (ERRKW +   18)    /*     gp_amm.c:4489 */

#define   EKW019      (ERRKW +   19)    /*     gp_cfg.c: 268 */
#define   EKW020      (ERRKW +   20)    /*     gp_cfg.c: 311 */
#define   EKW021      (ERRKW +   21)    /*     gp_cfg.c: 368 */
#define   EKW022      (ERRKW +   22)    /*     gp_cfg.c: 380 */

#define   EKW023      (ERRKW +   23)    /*     gp_dbm.c: 633 */
#define   EKW024      (ERRKW +   24)    /*     gp_dbm.c: 958 */
#define   EKW025      (ERRKW +   25)    /*     gp_dbm.c:1182 */
#define   EKW026      (ERRKW +   26)    /*     gp_dbm.c:1190 */
#define   EKW027      (ERRKW +   27)    /*     gp_dbm.c:1198 */
#define   EKW028      (ERRKW +   28)    /*     gp_dbm.c:1263 */

#define   EKW029      (ERRKW +   29)    /*   gp_ex_ms.c: 317 */
#define   EKW030      (ERRKW +   30)    /*   gp_ex_ms.c: 389 */
#define   EKW031      (ERRKW +   31)    /*   gp_ex_ms.c: 431 */
#define   EKW032      (ERRKW +   32)    /*   gp_ex_ms.c: 481 */
#define   EKW033      (ERRKW +   33)    /*   gp_ex_ms.c: 493 */
#define   EKW034      (ERRKW +   34)    /*   gp_ex_ms.c: 638 */
#define   EKW035      (ERRKW +   35)    /*   gp_ex_ms.c: 730 */
#define   EKW036      (ERRKW +   36)    /*   gp_ex_ms.c: 772 */
#define   EKW037      (ERRKW +   37)    /*   gp_ex_ms.c: 802 */
#define   EKW038      (ERRKW +   38)    /*   gp_ex_ms.c: 849 */
#define   EKW039      (ERRKW +   39)    /*   gp_ex_ms.c: 861 */

#define   EKW040      (ERRKW +   40)    /*     gp_lim.c: 221 */
#define   EKW041      (ERRKW +   41)    /*     gp_lim.c: 231 */

#define   EKW042      (ERRKW +   42)    /*     gp_lmm.c: 225 */
#define   EKW043      (ERRKW +   43)    /*     gp_lmm.c: 280 */
#define   EKW044      (ERRKW +   44)    /*     gp_lmm.c: 418 */
#define   EKW045      (ERRKW +   45)    /*     gp_lmm.c: 886 */
#define   EKW046      (ERRKW +   46)    /*     gp_lmm.c: 952 */
#define   EKW047      (ERRKW +   47)    /*     gp_lmm.c:1016 */
#define   EKW048      (ERRKW +   48)    /*     gp_lmm.c:1340 */

#define   EKW049      (ERRKW +   49)    /*  gp_pj_cfg.c: 505 */
#define   EKW050      (ERRKW +   50)    /*  gp_pj_cfg.c:1276 */

#define   EKW051      (ERRKW +   51)    /* gp_pj_cfg.c.tmp: 505 */
#define   EKW052      (ERRKW +   52)    /* gp_pj_cfg.c.tmp:1276 */

#define   EKW053      (ERRKW +   53)    /*  gp_pj_dlm.c: 243 */
#define   EKW054      (ERRKW +   54)    /*  gp_pj_dlm.c: 381 */
#define   EKW055      (ERRKW +   55)    /*  gp_pj_dlm.c: 518 */
#define   EKW056      (ERRKW +   56)    /*  gp_pj_dlm.c: 564 */
#define   EKW057      (ERRKW +   57)    /*  gp_pj_dlm.c: 644 */
#define   EKW058      (ERRKW +   58)    /*  gp_pj_dlm.c: 724 */
#define   EKW059      (ERRKW +   59)    /*  gp_pj_dlm.c: 864 */
#define   EKW060      (ERRKW +   60)    /*  gp_pj_dlm.c: 883 */
#define   EKW061      (ERRKW +   61)    /*  gp_pj_dlm.c: 921 */
#define   EKW062      (ERRKW +   62)    /*  gp_pj_dlm.c:1036 */
#define   EKW063      (ERRKW +   63)    /*  gp_pj_dlm.c:1049 */
#define   EKW064      (ERRKW +   64)    /*  gp_pj_dlm.c:1176 */
#define   EKW065      (ERRKW +   65)    /*  gp_pj_dlm.c:1226 */
#define   EKW066      (ERRKW +   66)    /*  gp_pj_dlm.c:1299 */
#define   EKW067      (ERRKW +   67)    /*  gp_pj_dlm.c:1324 */
#define   EKW068      (ERRKW +   68)    /*  gp_pj_dlm.c:1466 */
#define   EKW069      (ERRKW +   69)    /*  gp_pj_dlm.c:1534 */
#define   EKW070      (ERRKW +   70)    /*  gp_pj_dlm.c:1621 */
#define   EKW071      (ERRKW +   71)    /*  gp_pj_dlm.c:1637 */
#define   EKW072      (ERRKW +   72)    /*  gp_pj_dlm.c:1657 */
#define   EKW073      (ERRKW +   73)    /*  gp_pj_dlm.c:1680 */
#define   EKW074      (ERRKW +   74)    /*  gp_pj_dlm.c:1750 */
#define   EKW075      (ERRKW +   75)    /*  gp_pj_dlm.c:1772 */
#define   EKW076      (ERRKW +   76)    /*  gp_pj_dlm.c:1847 */
#define   EKW077      (ERRKW +   77)    /*  gp_pj_dlm.c:1961 */

#define   EKW078      (ERRKW +   78)    /*  gp_pj_lmm.c: 319 */
#define   EKW079      (ERRKW +   79)    /*  gp_pj_lmm.c: 806 */
#define   EKW080      (ERRKW +   80)    /*  gp_pj_lmm.c: 870 */

#define   EKW081      (ERRKW +   81)    /*  gp_pj_uim.c: 268 */
#define   EKW082      (ERRKW +   82)    /*  gp_pj_uim.c: 347 */
#define   EKW083      (ERRKW +   83)    /*  gp_pj_uim.c: 444 */
#define   EKW084      (ERRKW +   84)    /*  gp_pj_uim.c: 758 */
#define   EKW085      (ERRKW +   85)    /*  gp_pj_uim.c: 844 */
#define   EKW086      (ERRKW +   86)    /*  gp_pj_uim.c: 937 */
#define   EKW087      (ERRKW +   87)    /*  gp_pj_uim.c:1008 */
#define   EKW088      (ERRKW +   88)    /*  gp_pj_uim.c:1158 */
#define   EKW089      (ERRKW +   89)    /*  gp_pj_uim.c:1221 */
#define   EKW090      (ERRKW +   90)    /*  gp_pj_uim.c:1305 */
#define   EKW091      (ERRKW +   91)    /*  gp_pj_uim.c:1412 */
#define   EKW092      (ERRKW +   92)    /*  gp_pj_uim.c:1454 */
#define   EKW093      (ERRKW +   93)    /*  gp_pj_uim.c:1566 */

#define   EKW094      (ERRKW +   94)    /* gp_pj_uim.c.tmp: 268 */
#define   EKW095      (ERRKW +   95)    /* gp_pj_uim.c.tmp: 347 */
#define   EKW096      (ERRKW +   96)    /* gp_pj_uim.c.tmp: 444 */
#define   EKW097      (ERRKW +   97)    /* gp_pj_uim.c.tmp: 758 */
#define   EKW098      (ERRKW +   98)    /* gp_pj_uim.c.tmp: 844 */
#define   EKW099      (ERRKW +   99)    /* gp_pj_uim.c.tmp: 937 */
#define   EKW100      (ERRKW +  100)    /* gp_pj_uim.c.tmp:1008 */
#define   EKW101      (ERRKW +  101)    /* gp_pj_uim.c.tmp:1158 */
#define   EKW102      (ERRKW +  102)    /* gp_pj_uim.c.tmp:1221 */
#define   EKW103      (ERRKW +  103)    /* gp_pj_uim.c.tmp:1305 */
#define   EKW104      (ERRKW +  104)    /* gp_pj_uim.c.tmp:1412 */
#define   EKW105      (ERRKW +  105)    /* gp_pj_uim.c.tmp:1454 */
#define   EKW106      (ERRKW +  106)    /* gp_pj_uim.c.tmp:1566 */

#define   EKW107      (ERRKW +  107)    /*  gp_pj_ulm.c: 482 */
#define   EKW108      (ERRKW +  108)    /*  gp_pj_ulm.c: 812 */
#define   EKW109      (ERRKW +  109)    /*  gp_pj_ulm.c: 828 */
#define   EKW110      (ERRKW +  110)    /*  gp_pj_ulm.c: 943 */
#define   EKW111      (ERRKW +  111)    /*  gp_pj_ulm.c: 972 */
#define   EKW112      (ERRKW +  112)    /*  gp_pj_ulm.c:1047 */
#define   EKW113      (ERRKW +  113)    /*  gp_pj_ulm.c:1064 */
#define   EKW114      (ERRKW +  114)    /*  gp_pj_ulm.c:2070 */
#define   EKW115      (ERRKW +  115)    /*  gp_pj_ulm.c:2077 */

#define   EKW116      (ERRKW +  116)    /*  gp_pj_utl.c: 213 */
#define   EKW117      (ERRKW +  117)    /*  gp_pj_utl.c: 221 */
#define   EKW118      (ERRKW +  118)    /*  gp_pj_utl.c: 291 */
#define   EKW119      (ERRKW +  119)    /*  gp_pj_utl.c: 356 */
#define   EKW120      (ERRKW +  120)    /*  gp_pj_utl.c: 421 */
#define   EKW121      (ERRKW +  121)    /*  gp_pj_utl.c: 510 */
#define   EKW122      (ERRKW +  122)    /*  gp_pj_utl.c: 525 */
#define   EKW123      (ERRKW +  123)    /*  gp_pj_utl.c: 532 */
#define   EKW124      (ERRKW +  124)    /*  gp_pj_utl.c: 554 */
#define   EKW125      (ERRKW +  125)    /*  gp_pj_utl.c: 640 */
#define   EKW126      (ERRKW +  126)    /*  gp_pj_utl.c: 652 */
#define   EKW127      (ERRKW +  127)    /*  gp_pj_utl.c: 676 */
#define   EKW128      (ERRKW +  128)    /*  gp_pj_utl.c: 782 */
#define   EKW129      (ERRKW +  129)    /*  gp_pj_utl.c: 794 */
#define   EKW130      (ERRKW +  130)    /*  gp_pj_utl.c: 805 */
#define   EKW131      (ERRKW +  131)    /*  gp_pj_utl.c: 826 */
#define   EKW132      (ERRKW +  132)    /*  gp_pj_utl.c: 905 */
#define   EKW133      (ERRKW +  133)    /*  gp_pj_utl.c: 919 */
#define   EKW134      (ERRKW +  134)    /*  gp_pj_utl.c: 926 */
#define   EKW135      (ERRKW +  135)    /*  gp_pj_utl.c:1027 */
#define   EKW136      (ERRKW +  136)    /*  gp_pj_utl.c:1042 */
#define   EKW137      (ERRKW +  137)    /*  gp_pj_utl.c:1070 */
#define   EKW138      (ERRKW +  138)    /*  gp_pj_utl.c:1205 */
#define   EKW139      (ERRKW +  139)    /*  gp_pj_utl.c:1222 */
#define   EKW140      (ERRKW +  140)    /*  gp_pj_utl.c:1232 */
#define   EKW141      (ERRKW +  141)    /*  gp_pj_utl.c:2460 */
#define   EKW142      (ERRKW +  142)    /*  gp_pj_utl.c:2470 */
#define   EKW143      (ERRKW +  143)    /*  gp_pj_utl.c:2642 */
#define   EKW144      (ERRKW +  144)    /*  gp_pj_utl.c:2874 */
#define   EKW145      (ERRKW +  145)    /*  gp_pj_utl.c:2963 */
#define   EKW146      (ERRKW +  146)    /*  gp_pj_utl.c:3153 */
#define   EKW147      (ERRKW +  147)    /*  gp_pj_utl.c:3171 */
#define   EKW148      (ERRKW +  148)    /*  gp_pj_utl.c:3219 */
#define   EKW149      (ERRKW +  149)    /*  gp_pj_utl.c:3322 */
#define   EKW150      (ERRKW +  150)    /*  gp_pj_utl.c:3353 */
#define   EKW151      (ERRKW +  151)    /*  gp_pj_utl.c:3412 */
#define   EKW152      (ERRKW +  152)    /*  gp_pj_utl.c:3449 */
#define   EKW153      (ERRKW +  153)    /*  gp_pj_utl.c:3540 */

#define   EKW154      (ERRKW +  154)    /* gp_pj_utl.c.tmp: 213 */
#define   EKW155      (ERRKW +  155)    /* gp_pj_utl.c.tmp: 221 */
#define   EKW156      (ERRKW +  156)    /* gp_pj_utl.c.tmp: 291 */
#define   EKW157      (ERRKW +  157)    /* gp_pj_utl.c.tmp: 356 */
#define   EKW158      (ERRKW +  158)    /* gp_pj_utl.c.tmp: 421 */
#define   EKW159      (ERRKW +  159)    /* gp_pj_utl.c.tmp: 510 */
#define   EKW160      (ERRKW +  160)    /* gp_pj_utl.c.tmp: 525 */
#define   EKW161      (ERRKW +  161)    /* gp_pj_utl.c.tmp: 532 */
#define   EKW162      (ERRKW +  162)    /* gp_pj_utl.c.tmp: 554 */
#define   EKW163      (ERRKW +  163)    /* gp_pj_utl.c.tmp: 640 */
#define   EKW164      (ERRKW +  164)    /* gp_pj_utl.c.tmp: 652 */
#define   EKW165      (ERRKW +  165)    /* gp_pj_utl.c.tmp: 676 */
#define   EKW166      (ERRKW +  166)    /* gp_pj_utl.c.tmp: 782 */
#define   EKW167      (ERRKW +  167)    /* gp_pj_utl.c.tmp: 794 */
#define   EKW168      (ERRKW +  168)    /* gp_pj_utl.c.tmp: 805 */
#define   EKW169      (ERRKW +  169)    /* gp_pj_utl.c.tmp: 826 */
#define   EKW170      (ERRKW +  170)    /* gp_pj_utl.c.tmp: 905 */
#define   EKW171      (ERRKW +  171)    /* gp_pj_utl.c.tmp: 919 */
#define   EKW172      (ERRKW +  172)    /* gp_pj_utl.c.tmp: 926 */
#define   EKW173      (ERRKW +  173)    /* gp_pj_utl.c.tmp:1027 */
#define   EKW174      (ERRKW +  174)    /* gp_pj_utl.c.tmp:1042 */
#define   EKW175      (ERRKW +  175)    /* gp_pj_utl.c.tmp:1070 */
#define   EKW176      (ERRKW +  176)    /* gp_pj_utl.c.tmp:1205 */
#define   EKW177      (ERRKW +  177)    /* gp_pj_utl.c.tmp:1222 */
#define   EKW178      (ERRKW +  178)    /* gp_pj_utl.c.tmp:1232 */
#define   EKW179      (ERRKW +  179)    /* gp_pj_utl.c.tmp:2460 */
#define   EKW180      (ERRKW +  180)    /* gp_pj_utl.c.tmp:2470 */
#define   EKW181      (ERRKW +  181)    /* gp_pj_utl.c.tmp:2642 */
#define   EKW182      (ERRKW +  182)    /* gp_pj_utl.c.tmp:2874 */
#define   EKW183      (ERRKW +  183)    /* gp_pj_utl.c.tmp:2963 */
#define   EKW184      (ERRKW +  184)    /* gp_pj_utl.c.tmp:3153 */
#define   EKW185      (ERRKW +  185)    /* gp_pj_utl.c.tmp:3171 */
#define   EKW186      (ERRKW +  186)    /* gp_pj_utl.c.tmp:3219 */
#define   EKW187      (ERRKW +  187)    /* gp_pj_utl.c.tmp:3322 */
#define   EKW188      (ERRKW +  188)    /* gp_pj_utl.c.tmp:3353 */
#define   EKW189      (ERRKW +  189)    /* gp_pj_utl.c.tmp:3412 */
#define   EKW190      (ERRKW +  190)    /* gp_pj_utl.c.tmp:3449 */
#define   EKW191      (ERRKW +  191)    /* gp_pj_utl.c.tmp:3540 */

#define   EKW192      (ERRKW +  192)    /*    gp_ptui.c: 666 */
#define   EKW193      (ERRKW +  193)    /*    gp_ptui.c: 709 */
#define   EKW194      (ERRKW +  194)    /*    gp_ptui.c: 752 */
#define   EKW195      (ERRKW +  195)    /*    gp_ptui.c: 795 */
#define   EKW196      (ERRKW +  196)    /*    gp_ptui.c: 842 */
#define   EKW197      (ERRKW +  197)    /*    gp_ptui.c:1081 */
#define   EKW198      (ERRKW +  198)    /*    gp_ptui.c:1123 */
#define   EKW199      (ERRKW +  199)    /*    gp_ptui.c:1158 */

#define   EKW200      (ERRKW +  200)    /*     gp_tmm.c: 213 */
#define   EKW201      (ERRKW +  201)    /*     gp_tmm.c: 303 */
#define   EKW202      (ERRKW +  202)    /*     gp_tmm.c: 405 */
#define   EKW203      (ERRKW +  203)    /*     gp_tmm.c: 519 */

#define   EKW204      (ERRKW +  204)    /*     gp_tmr.c: 253 */
#define   EKW205      (ERRKW +  205)    /*     gp_tmr.c: 358 */
#define   EKW206      (ERRKW +  206)    /*     gp_tmr.c: 554 */

#define   EKW207      (ERRKW +  207)    /*     gp_uim.c: 276 */
#define   EKW208      (ERRKW +  208)    /*     gp_uim.c: 358 */
#define   EKW209      (ERRKW +  209)    /*     gp_uim.c: 437 */
#define   EKW210      (ERRKW +  210)    /*     gp_uim.c: 690 */
#define   EKW211      (ERRKW +  211)    /*     gp_uim.c: 769 */
#define   EKW212      (ERRKW +  212)    /*     gp_uim.c: 832 */
#define   EKW213      (ERRKW +  213)    /*     gp_uim.c: 912 */

#define   EKW214      (ERRKW +  214)    /*     gp_umm.c: 213 */
#define   EKW215      (ERRKW +  215)    /*     gp_umm.c: 706 */
#define   EKW216      (ERRKW +  216)    /*     gp_umm.c:1248 */

#define   EKW217      (ERRKW +  217)    /*     gp_utl.c: 227 */
#define   EKW218      (ERRKW +  218)    /*     gp_utl.c: 237 */
#define   EKW219      (ERRKW +  219)    /*     gp_utl.c: 263 */
#define   EKW220      (ERRKW +  220)    /*     gp_utl.c: 278 */
#define   EKW221      (ERRKW +  221)    /*     gp_utl.c: 383 */
#define   EKW222      (ERRKW +  222)    /*     gp_utl.c: 401 */
#define   EKW223      (ERRKW +  223)    /*     gp_utl.c: 413 */
#define   EKW224      (ERRKW +  224)    /*     gp_utl.c: 473 */
#define   EKW225      (ERRKW +  225)    /*     gp_utl.c: 598 */
#define   EKW226      (ERRKW +  226)    /*     gp_pj_tmr.c: 291 */
#define   EKW227      (ERRKW +  227)    /*     gp_pj_tmr.c: 576 */


#endif /* __KW_ERR_H__ */
  
/********************************************************************30**
  
         End of file:     kw_err.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:23:15 2015
  
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
