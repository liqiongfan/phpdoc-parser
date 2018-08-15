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
#include "kernel/app/dispatch.h"
#include "kernel/app/response.h"
#include "kernel/class/aop/proxy.h"
#include "ext/standard/php_string.h"

/**
 * {{{ ARGINFO
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_forward), 0, 0, 1)
    ZEND_ARG_INFO(0, url)
    ZEND_ARG_INFO(0, getParams)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_redirect), 0, 0, 1)
    ZEND_ARG_INFO(0, url)
    ZEND_ARG_INFO(0, getParams)
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
 * {{{
 * proto Response::forward($url, $getParams = [])
 */
XAN_METHOD(Response, forward)
{
    zend_string *url, *key;
    zval *get_params = NULL, *value;

    if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S|a", &url, &get_params) == FAILURE) {
        return ;
    }

    if ( !ZSTR_LEN(url) ) {
        return ;
    }

    if ( get_params && zend_hash_num_elements( Z_ARRVAL_P(get_params)) ) {
        
        ZVAL_STRING(return_value, "");
        ZEND_HASH_FOREACH_STR_KEY_VAL( Z_ARRVAL_P(get_params), key, value ) {
            
            if ( !ZSTR_LEN(key) ) continue;
            convert_to_string(value);
            ZVAL_STR(return_value, strpprintf(0, "%s/%s/%s", Z_STRVAL_P(return_value), ZSTR_VAL(key), Z_STRVAL_P(value) ));

        } ZEND_HASH_FOREACH_END();
    
        if ( XAN_G(must_url_suffix) ) {
            url = strpprintf(0, "%s%s.%s", ZSTR_VAL(url), Z_STRVAL_P(return_value), Z_STRVAL(XAN_G(url_suffix)) );
        } else {
            url = strpprintf(0, "%s%s", ZSTR_VAL(url), Z_STRVAL_P(return_value) );
        }
    } else {
        if ( XAN_G(must_url_suffix) ) {
            url = strpprintf(0, "%s.%s", ZSTR_VAL(url), Z_STRVAL(XAN_G(url_suffix)));
        }
    }

    /* Dispatch the url */
    xan_dispatch_url(url);

    /* Release the url memory */
    zend_string_release(url);
}/*}}}*/

/**
 * {{{
 * proto Response::redirect($url, $getParams = [])
 */
XAN_METHOD(Response, redirect)
{
    zend_string *url, *key;
    zval *get_params = NULL, *value;

    if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S|a", &url, &get_params) == FAILURE) {
        return ;
    }

    if ( !ZSTR_LEN(url) ) {
        return ;
    }

    if ( get_params && zend_hash_num_elements( Z_ARRVAL_P(get_params)) ) {
        
        ZVAL_STRING(return_value, "");
        ZEND_HASH_FOREACH_STR_KEY_VAL( Z_ARRVAL_P(get_params), key, value ) {
            
            if ( !ZSTR_LEN(key) ) continue;
            convert_to_string(value);
            ZVAL_STR(return_value, strpprintf(0, "%s/%s/%s", Z_STRVAL_P(return_value), ZSTR_VAL(key), Z_STRVAL_P(value) ));

        } ZEND_HASH_FOREACH_END();
    
        if ( XAN_G(must_url_suffix) ) {
            url = strpprintf(0, "%s%s.%s", ZSTR_VAL(url), Z_STRVAL_P(return_value), Z_STRVAL(XAN_G(url_suffix)) );
        } else {
            url = strpprintf(0, "%s%s", ZSTR_VAL(url), Z_STRVAL_P(return_value) );
        }
    } else {
        if ( XAN_G(must_url_suffix) ) {
            url = strpprintf(0, "%s.%s", ZSTR_VAL(url), Z_STRVAL(XAN_G(url_suffix)));
        }
    }

    /* Using the header to redirect the url */
    sapi_header_line ctr = {0};
    ctr.line_len = spprintf(
        &(ctr.line), 
        0, 
        "%s: %s%s", 
        "Location", 
        ZSTR_VAL(url)[0] == '/' ? "" : "/",
        ZSTR_VAL(url)
    );
    ctr.response_code = 200;                                           
    sapi_header_op(SAPI_HEADER_REPLACE, &ctr);
    efree(ctr.line);

    /* Release the url info */
    zend_string_release(url);
}/*}}}*/

/**
 * proto All functions for the Xan\Response class
 */
XAN_FUNCTIONS(response)
    XAN_ME(Response, __construct, arginfo_xan_response_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Response, forward, arginfo_xan_response_forward, ZEND_ACC_PUBLIC)
    XAN_ME(Response, redirect, arginfo_xan_response_redirect, ZEND_ACC_PUBLIC)
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