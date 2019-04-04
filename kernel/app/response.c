/*
  +----------------------------------------------------------------------+
  | Xannotations---PHP Version 7                                         |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Josin                                                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "kernel/app/net.h"
#include "php_xannotation.h"
#include "ext/standard/info.h"
#include "kernel/app/response.h"
#include "ext/standard/php_string.h"

/**
 * {{{ ARGINFO
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{ proto 
 * Response::__construct()
 */
XAN_METHOD(Response, __construct)
{

}/*}}}*/

/**
 * proto All functions for the Xan\Response class
 */
XAN_FUNCTIONS(response)
    XAN_ME(Response, __construct, arginfo_xan_response_construct, ZEND_ACC_PUBLIC)
XAN_FUNCTIONS_END()
/**}}}*/

/**
 * {{{
 * proto response_init()
 */
XAN_INIT(response)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan", "Response", response_functions);
    response_ce = zend_register_internal_class(&ce);
}/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */