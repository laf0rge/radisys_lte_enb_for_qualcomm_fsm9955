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

     Name:    TCP UDP Convergence Layer - external 
              interface - portable
  
     Type:    C source file
  
     Desc:    Portable unpacking routines.
              
     File:    hi_ex_pt.c
  
     Sid:      hi_ex_pt.c@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:39:00 2015
  
     Prg:     asa
  
*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

/* external headers */
#ifdef HI_TLS
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* common timer */
#include "cm_err.h"        /* common error */
#include "hi_err.h"        /* TUCL error defines */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport defines */
#include "hit.h"           /* upper interface */
#ifdef FTHA
#include "sht.h"           /* SHT Interface header files */
#endif /* FTHA */
#include "lhi.h"           /* TUCL layer mgt defines */
#include "hi.h"            /* TUCL internal defines */
  
/* header/extern include files (.x) */
  
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_hash.x"       /* common hash */
#include "cm_llist.x"      /* common linked list */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common sockets */
#include "cm_tpt.x"        /* common transport typedefs */
#include "hit.x"           /* upper interface */
#ifdef FTHA
#include "sht.x"           /* SHT Interface header file */
#endif /* FTHA */
#include "lhi.x"           /* TUCL layer mgt structures */

#ifdef HI_LKSCTP
#include "sct.h"
#include "sct.x"
#endif

#include "hi.x"            /* TUCL internal */
  

/* local defines */
  
/* local typedefs */
  
/* local externs */
  
/* forward references */
  
/* functions in other modules */
  
/* public variable declarations */
  
/* private variable declarations */
  

/*
*     support functions
*/


#ifdef HI_TLS
/*
*
*       Fun:    hiOSSLPostConCheck
*
*       Desc:   Called from TUCL to perform post connection
*               verification. Must be implemented by the customer.
*
*       Ret:    X509_V_OK                               - ok
*               X509_V_ERR_APPLICATION_VERIFICATION     - failed
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC long hiOSSLPostConCheck
(
SSL             *ssl,           /* TLS connection */
CmTptAddr       *peerAddr       /* peer's address */
)
#else
PUBLIC long hiOSSLPostConCheck(ssl, peerAddr)
SSL             *ssl;           /* TLS connection */
CmTptAddr       *peerAddr;      /* peer's address */
#endif
{
   TRC2(hiOSSLPostConCheck);


   return X509_V_OK;
}


/*
*
*       Fun:    hiOSSLPasswdCb
*
*       Desc:   Called from OpenSSL to provide a password to
*               decrypt the private key file. Must be implemented
*               by the customer.
*
*       Ret:    password length - ok
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC int hiOSSLPasswdCb
(
char            *buf,           /* buffer for password */
int             size,           /* available buffer size */
int             rwflag,         /* reading or writing */
Void            *userdata       /* specified at time of cb registration */
)
#else
PUBLIC int hiOSSLPasswdCb(buf, size, rwflag, userdata)
char            *buf;           /* buffer for password */
int             size;           /* available buffer size */
int             rwflag;         /* reading or writing */
Void            *userdata;      /* specified at time of cb registration */
#endif
{
   TRC2(hiOSSLPasswdCb);

   buf[0] = '\0';

   return 0;
}

#ifdef HI_TLS_EXT_SESS_CACHE

/*
*
*       Fun:    hiOSSLNewSessCb
*
*       Desc:   Called from OpenSSL when a new SSL_SESSION is created
*               so that it may be cached. Must be implemented by the
*               customer.
*
*       Ret:    non-zero        - ok, cached
*               zero            - failed, not cached
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC int hiOSSLNewSessCb
(
SSL_CTX         *ctx,           /* context */
SSL_SESSION     *session        /* session */
)
#else
PUBLIC int hiOSSLNewSessCb(ctx, session)
SSL_CTX         *ctx;           /* context */
SSL_SESSION     *session;       /* session */
#endif
{
   TRC2(hiOSSLNewSessCb);


   return 0;
}


/*
*
*       Fun:    hiOSSLDelSessCb
*
*       Desc:   Called from OpenSSL when an SSL_SESSION is destroyed
*               so that it may be removed from the cache. Must be
*               implemented by the customer.
*
*       Ret:    Void
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC Void hiOSSLDelSessCb
(
SSL_CTX         *ctx,           /* context */
SSL_SESSION     *session        /* session */
)
#else
PUBLIC Void hiOSSLDelSessCb(ctx, session)
SSL_CTX         *ctx;           /* context */
SSL_SESSION     *session;       /* session */
#endif
{
   TRC2(hiOSSLDelSessCb);


   return;
}


/*
*
*       Fun:    hiOSSLGetSessCb
*
*       Desc:   Called from OpenSSL to query the external cache for
*               session resumption. Must be implemented by the customer.
*
*       Ret:    non-NULL        - ok
*               NULL            - failed
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC long hiOSSLGetSessCb
(
SSL_CTX         *ctx,           /* context */
unsigned char   *id,            /* session id (peer identifier) */
int             len,            /* session id length */
int             *ref            /* non-zero if ref count to be incremented */
)
#else
PUBLIC long hiOSSLGetSessCb(ctx, id, len, ref)
SSL_CTX         *ctx;           /* context */
unsigned char   *id;            /* session id (peer identifier) */
int             len;            /* session id length */
int             *ref;           /* non-zero if ref count to be incremented */
#endif
{
   TRC2(hiOSSLGetSessCb);


   return NULL;
}
#endif /* HI_TLS_EXT_SESS_CACHE */

/* hi008.105 : Added some more callback function for TLS */
#ifdef HI_TLS_EXT_CALLBACK
/*
*
*       Fun:    hiOSSLGetInfo
*
*       Desc:   Called from the OpenSSL library 
*
*       Ret:    Null
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC Void hiOSSLGetInfo
(
CONSTANT SSL  *ssl, 
int        where, 
int        ret
)
#else 
PUBLIC Void hiOSSLGetInfo(ssl, where, ret)
CONSTANT SSL  *ssl;
int        where;
int        ret;
#endif /* ANSI */
{
   TRC2(hiOSSLGetInfo)
   
   return;
}

/*
*
*       Fun:    hiOSSLGetMsg
*
*       Desc:   Called from the OpenSSL library 
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/

#ifdef ANSI
PUBLIC Void hiOSSLGetMsg
(
int         write_p,
int         version,
int         content_type,
CONSTANT Void  *buf,
size_t      len,
SSL         *ssl
)
#else
PUBLIC Void hiOSSLGetMsg( write_p, version, content_type, buf, len, ssl)
int         write_p;
int         version;
int         content_type;
CONSTANT Void  *buf;
size_t      len;
SSL         *ssl;
#endif
{
   TRC2(hiOSSLGetMsg)

   return; 
}
/* hi008.105 : moved function hiOSSLVerifyCb from tl_bdy3.c 
               file to hi_ex_pt.c file */
/*
*
*       Fun:    hiOSSLVerifyCb
*
*       Desc:   Called from the OpenSSL library 
*
*       Ret:    int
*
*       Notes:  None
*
*       File:   hi_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC int hiOSSLVerifyCb
(
int             ok,             /* OpenSSL verification status */
X509_STORE_CTX  *store          /* certificate store */
)
#else
PUBLIC int hiOSSLVerifyCb(ok, store)
int             ok;             /* OpenSSL verification status */
X509_STORE_CTX  *store;         /* certificate store */
#endif
{
   int          err;
   HiAlarmInfo  alInfo;


   /* was there a verification error? */
   if (!ok)
   {
      /* get the error */
      err = X509_STORE_CTX_get_error(store);

      /* send an alarm */
      alInfo.spId = -1;
      alInfo.type = LHI_ALARMINFO_OSSL_ERR;
      alInfo.inf.osslError = err;
      hiSendAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_PI_INV_EVT,
                  LCM_CAUSE_DECODE_ERR, &alInfo); 
   }


   return ok;
} /* hiOSSLVerifyCb() */
#endif /* HI_TLS_EXT_CALLBACK */

#endif /* HI_TLS */


/********************************************************************30**

         End of file:     hi_ex_pt.c@@/main/tenb_5.0_SIB_KNOWNSENSE/1 - Mon Aug 10 18:39:00 2015

*********************************************************************31*/
/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/
 
/********************************************************************70**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
1.1          ---      asa  1. initial release.
/main/2       ---      cvp  1. changed the copyright header.
             /main/4                cvp  1. Added sht include files.
/main/4      ---      cvp  1. changed the copyright header.
/main/5      ---       kp   1. Updated for release 1.5.
/main/5+    hi008.105  ss   1. Added some extra callback function for SSL.
                            2. Moved function hiOSSLVerifyCb from tl_bdy3.c 
                               file to hi_ex_pt.c file.
/main/6      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
