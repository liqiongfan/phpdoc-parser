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
#include "kernel/app/net.h"
#include "php_xannotation.h"
#include "ext/standard/info.h"
#include "kernel/app/request.h"
#include "kernel/class/aop/proxy.h"
#include "ext/standard/php_string.h"

/**
 * {{{ ARGINFO
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_request_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_request_get), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, filters)
    ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_request_get_post), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, filters)
    ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_request_get_server), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, filters)
    ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()


/*}}}*/

/**
 * {{{ proto 
 * Request::__construct()
 */
XAN_METHOD(Request, __construct)
{

}/*}}}*/


/**
 * {{{
 * proto Request::get($key, $filters, $defaultValue)
 */
XAN_METHOD(Request, get)
{
    zend_string *key = NULL;
    zval *default_value = NULL, *filters = NULL, *val;

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|Szz", &key, &filters, &default_value ) == FAILURE ) {
        return ;
    }

    zval *get_vars = xan_get_get_vars(NULL);

    if ( !key || !ZSTR_LEN(key) ) RETURN_ZVAL(get_vars, 1, NULL)

    val = STR_FIND_P(get_vars, key);

    if ( !val ) {
        if ( !default_value ) RETURN_NULL()
        else RETURN_ZVAL(default_value, 1, NULL)
    } else {
        filter_callback(val, filters, return_value);
    }
}/*}}}*/


/**
 * {{{
 * proto Request::getPost($key, $filter, $defaultValue)
 */
XAN_METHOD(Request, getPost)
{
    zend_string *key = NULL;
    zval *default_value = NULL, *filters = NULL, *val;

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|Szz", &key, &filters, &default_value ) == FAILURE ) {
        return ;
    }

    zval *post_vars = xan_get_post_var(NULL);

    if ( !key || !ZSTR_LEN(key) ) RETURN_ZVAL(post_vars, 1, NULL)

    val = STR_FIND_P(post_vars, key);

    if ( !val ){
        if ( !default_value ) RETURN_NULL()
        else RETURN_ZVAL(default_value, 1, NULL)
    } else {
        filter_callback(val, filters, return_value);
    }
}/*}}}*/

/**
 * {{{
 * proto Request::getServer($key, $filter, $defaultValue)
 */
XAN_METHOD(Request, getServer)
{
    zend_string *key = NULL;
    zval *default_value = NULL, *filters = NULL, *val;

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|Szz", &key, &filters, &default_value ) == FAILURE ) {
        return ;
    }

    zval *server_vars = xan_get_server_var(NULL);

    if ( !key || !ZSTR_LEN(key) ) RETURN_ZVAL(server_vars, 1, NULL)

    val = STR_FIND_P(server_vars, key);

    if ( !val ) {
        if ( !default_value ) RETURN_NULL()
        else RETURN_ZVAL(default_value, 1, NULL)
    } else {
        filter_callback(val, filters, return_value);
    }
}/*}}}*/

XAN_FUNCTIONS(request)
    XAN_ME(Request, __construct, arginfo_xan_request_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Request, get, arginfo_xan_request_get, ZEND_ACC_PUBLIC)
    XAN_ME(Request, getPost, arginfo_xan_request_get_post, ZEND_ACC_PUBLIC)
    XAN_ME(Request, getServer, arginfo_xan_request_get_server, ZEND_ACC_PUBLIC)
XAN_FUNCTIONS_END()

/**
 * {{{
 * proto request_init()
 */
XAN_INIT(request)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan", "Request", request_functions);
    request_ce = zend_register_internal_class(&ce);
}/*}}}*/


/**
 * {{{
 * Common filter for the user-input value
 */
void filter_callback(zval *val, zval *filters, zval *retval)
{
    zend_string *key = NULL;
    zval filter_call, *each_call;

    if ( !val ) {
        ZVAL_NULL(retval);
        return ;
    } else {
        if ( !filters ) {
            ZVAL_COPY(retval, val);
            return ;
        } else {
            if ( Z_TYPE_P(filters) == IS_STRING && !Z_STRLEN_P(filters) )  {
                ZVAL_COPY(retval, val);
                return ;
            } else if ( Z_TYPE_P(filters) == IS_STRING && Z_STRLEN_P(filters) ) {
                ZVAL_COPY(retval, val);
                array_init(&filter_call);
                php_explode( strpprintf(0, "%s", "|"), Z_STR_P(filters), &filter_call, ZEND_LONG_MAX );
                ZEND_HASH_FOREACH_VAL(Z_ARRVAL(filter_call), each_call) {
                    zval params[1] = { *retval };
                    if ( Z_TYPE_P(each_call) == IS_STRING && Z_STRLEN_P(each_call) ) {
                        call_user_function(CG(function_table), NULL, each_call, retval, 1, params);
                    }

                } ZEND_HASH_FOREACH_END();
                zend_array_destroy(Z_ARRVAL(filter_call));
            } else if ( zend_is_callable(filters, 1, &key) ){
                zval params[1] = { *val };
                call_user_function(NULL, NULL, filters, retval, 1, params );
            }
        }
    }
}/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */