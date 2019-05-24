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
#include "zend_smart_str.h"
#include "ext/standard/info.h"
#include "kernel/app/response.h"
#include "ext/standard/php_string.h"

/**
 * {{{ ARGINFO
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_setjsoncontent), 0, 0, 0)
    ZEND_ARG_INFO(0, contents)
    ZEND_ARG_INFO(0, json_option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_setrawcontent), 0, 0, 0)
    ZEND_ARG_INFO(0, contents)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_setheader), 0, 0, 0)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, contents)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_unsetsetheader), 0, 0, 0)
    ZEND_ARG_INFO(0, contents)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_response_send), 0, 0, 0)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{ proto 
 * Response::__construct()
 */
XAN_METHOD(Response, __construct)
{
    zval header_variables;
    array_init(&header_variables);
    zend_update_property(
        response_ce, 
        getThis(), 
        XAN_STRL(XAN_RESPONSE_HEADER_VARIABLES), 
        &header_variables
    );
}/*}}}*/


XAN_METHOD(Response, setHeader)/*{{{ proto Response::setheader($key, $value)*/
{
    zend_string *header_key, *header_value;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SS", &header_key, &header_value) == FAILURE){
        return ;
    }
    zval *response_headers = zend_read_property(
        response_ce, 
        getThis(), 
        XAN_STRL(XAN_RESPONSE_HEADER_VARIABLES), 
        1, 
        NULL
    );
    add_assoc_string(
        response_headers, 
        ZSTR_VAL(header_key), 
        ZSTR_VAL(header_value)
    );
    ZVAL_COPY(return_value, getThis());
}
/*}}}*/

XAN_METHOD(Response, unsetHeader)/*{{{ proto Response::unsetHeader($key)*/
{
    zend_string *header_key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &header_key) == FAILURE){
        return ;
    }
    zval *response_headers = zend_read_property(
        response_ce, 
        getThis(), 
        XAN_STRL(XAN_RESPONSE_HEADER_VARIABLES), 
        1, 
        NULL
    );
    zend_hash_del(Z_ARRVAL_P(response_headers), header_key);
    ZVAL_COPY(return_value, getThis());
}
/*}}}*/

/**
 * {{{ pro
 * Response::setJsonContent
 */
XAN_METHOD(Response, setJsonContent)
{
    zval *json_array;
    zend_long json_option = 256;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|l", &json_array, &json_option) == FAILURE) {
        return ;
    }
    smart_str json_str = { 0 };
    php_json_encode(&json_str, json_array, json_option);
    smart_str_0(&json_str);
    zend_update_property_string(
        response_ce, 
        getThis(), 
        XAN_STRL(XAN_RESPONSE_DATA), 
        ZSTR_VAL(json_str.s)
    );
    smart_str_free(&json_str);
    ZVAL_COPY(return_value, getThis());
} /*}}}*/

/**
 * {{{ pro
 * Response::setRawContent
 */
XAN_METHOD(Response, setRawContent)
{
    zend_string *string;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &string) == FAILURE) {
        return ;
    }
    if ( string ) {
        /* zend_class_entry *scope, zval *object, const char *name, size_t name_length, zend_string *value */
        zend_update_property_str(
            response_ce, 
            getThis(), 
            XAN_STRL(XAN_RESPONSE_DATA), 
            string
        );
    }
    ZVAL_COPY(return_value, getThis());
} /*}}}*/

/*{{{ To send the response data plus the http header to request client */
XAN_METHOD(Response, send)
{
    if (zend_parse_parameters_none() == FAILURE) return ;

    zval *response_data = zend_read_property(
        response_ce, 
        getThis(), 
        XAN_STRL(XAN_RESPONSE_DATA), 
        1, 
        NULL
    );
    zval *response_http_headers = zend_read_property(
        response_ce, 
        getThis(), 
        XAN_STRL(XAN_RESPONSE_HEADER_VARIABLES),
        1, 
        NULL
    );
    if (response_http_headers && ( Z_TYPE_P(response_http_headers) == IS_ARRAY )) {
        /* find Content-Type is set or not. if not set, set Content-Type to application/json;charset=UTF8 */
        zval *content_type = zend_hash_str_find(Z_ARRVAL_P(response_http_headers), XAN_STRL("Content-Type"));
        if ( !content_type || ZVAL_IS_NULL(content_type))
        {
            add_assoc_string( response_http_headers,  "Content-Type",  "application/json;charset=UTF-8" );
        }
        zend_string *var_name;
        zval*var_value;
        sapi_header_line ctr = {0};
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(response_http_headers), var_name, var_value){
            ctr.line_len = spprintf(
                &(ctr.line), 
                0, 
                "%s:%s", 
                ZSTR_VAL(var_name), 
                Z_STRVAL_P(var_value)
            );
            ctr.response_code = 200;                                           
            sapi_header_op(SAPI_HEADER_REPLACE, &ctr);                           
            efree(ctr.line);
        } ZEND_HASH_FOREACH_END();
        efree(ctr.line);
    }
    php_write(Z_STRVAL_P(response_data), Z_STRLEN_P(response_data));
}
/*}}}*/

/**
 * proto All functions for the Xan\Response class
 */
XAN_FUNCTIONS(response)
    XAN_ME(Response, __construct, arginfo_xan_response_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Response, setJsonContent, arginfo_xan_response_setjsoncontent, ZEND_ACC_PUBLIC)
    XAN_ME(Response, setRawContent, arginfo_xan_response_setrawcontent, ZEND_ACC_PUBLIC)
    XAN_ME(Response, setHeader, arginfo_xan_response_setheader, ZEND_ACC_PUBLIC)
    XAN_ME(Response, unsetHeader, arginfo_xan_response_unsetsetheader, ZEND_ACC_PUBLIC)
    XAN_ME(Response, send, arginfo_xan_response_send, ZEND_ACC_PUBLIC)
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

    XAN_PR_NULL(response_ce, XAN_RESPONSE_HEADER_VARIABLES, ZEND_ACC_PRIVATE);
    XAN_PR_ESTRING(response_ce, XAN_RESPONSE_DATA, ZEND_ACC_PRIVATE);
}/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */