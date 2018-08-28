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
#include "php_xannotation.h"
#include "kernel/app/session.h"
#include "ext/standard/info.h"
#include "ext/session/php_session.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_interfaces.h"
#include "ext/spl/spl_iterators.h"

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_get_raw), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_set), 0, 0, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_get), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_unset), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_exists), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_current), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_key), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_next), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_rewind), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_valid), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_session_count), 0, 0, 0)
ZEND_END_ARG_INFO()

/** {{{ proto Session::__construct(void) 
 */
XAN_METHOD(Session, __construct)
{
    zval *status;

    status = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(XAN_SESSION_STATUS), 1, NULL);
    if ( Z_TYPE_P(status) == IS_TRUE ) {
        return ;
    }

    if ( PS(session_status) != php_session_active ) {
        php_session_start();
    }

    if ( PS(session_status) != php_session_active ) {
		XAN_INFO(E_ERROR, "Can't start SESSION!");
        return ;
	}

    zend_update_property_bool(XAN_ENTRY_OBJ(getThis()), XAN_STRL(XAN_SESSION_STATUS), IS_TRUE);
    zend_update_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(XAN_SESSION_INSTANCE), Z_REFVAL(PS(http_session_vars)));
}/*}}}*/

/** {{{
 * proto Session::getRaw()
 */
XAN_METHOD(Session, getRaw)
{
    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }

    if ( PS(session_status) != php_session_active ) {
        php_session_start();
    }

    if ( PS(session_status) != php_session_active ) {
		XAN_INFO(E_ERROR, "Can't start SESSION!");
        return ;
	}

    RETURN_ZVAL( Z_REFVAL(PS(http_session_vars)), 1, NULL);
}/*}}}*/

/** {{{ proto Session::set($key, $value)
 */
XAN_METHOD(Session, set)
{
    zend_string *key;
    zval *value, keys, *each_key, temp_value;

    int num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &key, &value) == FAILURE ) {
        return ;
    }

    if ( !ZSTR_LEN(key) ) {
        RETURN_NULL()
    }

    array_init(&keys);
    php_explode(
        strpprintf(0, "%c", '.'),
        key,
        &keys,
        ZEND_LONG_MAX
    );

    num = Z_H_N_E(Z_ARRVAL(keys));

    if ( num == 1 ) {
        zend_array_destroy(Z_ARRVAL(keys));
        php_set_session_var(key, value, NULL);
    } else {
        zval *session_vars = Z_REFVAL(PS(http_session_vars));
        ZVAL_COPY(&temp_value, session_vars);

        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(keys), each_key) {

            zval *val = STR_FIND(temp_value, Z_STR_P(each_key));

            if ( !val ) {
                if ( num == 1 ) {
                    add_assoc_zval(&temp_value, Z_STRVAL_P(each_key), value);
                } else {
                    zval middle_value;
                    array_init(&middle_value);
                    add_assoc_zval(&temp_value, Z_STRVAL_P(each_key), &middle_value);
                    ZVAL_COPY(&temp_value, &middle_value);
                }
            } else if ( val ) {
                if ( num == 1 ) {
                    zend_hash_update(Z_ARRVAL(temp_value), Z_STR_P(each_key), value);
                } else {
                    if ( Z_TYPE_P(val) == IS_ARRAY ) {

                        ZVAL_COPY(&temp_value, val);

                    } else {
                        zval middle_value;
                        array_init(&middle_value);
                        zend_hash_del(Z_ARRVAL(temp_value), Z_STR_P(each_key));
                        add_assoc_zval(&temp_value, Z_STRVAL_P(each_key), &middle_value);
                        ZVAL_COPY(&temp_value, &middle_value);
                    }
                }
            }
            num--;
        } ZEND_HASH_FOREACH_END();
        zend_array_destroy(Z_ARRVAL(keys));
    }
}/*}}}*/

/**
 * proto proto Session::get($key)
 */
XAN_METHOD(Session, get)
{
    zend_string *key;
    zval keys, result_key, temp_value, *temp_key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE ) {
        return ;
    }

    if ( !ZSTR_LEN(key) || !key ) {
        XAN_INFO(E_ERROR, "$key must be valid.");
    }

    zval *array = Z_REFVAL(PS(http_session_vars));
    array_init(&keys);
    php_explode( strpprintf(0, "%c", '.'), key, &keys, ZEND_LONG_MAX );
    int num = Z_H_N_E(Z_ARRVAL(keys));
    if ( num > 1 ) {
        ZVAL_STR(&result_key, key);
        ZVAL_COPY(&temp_value, array);
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(keys), temp_key) {
            if ( Z_TYPE(temp_value) != IS_ARRAY ) RETURN_NULL();
            zval *t_v = zend_hash_find(Z_ARRVAL(temp_value), Z_STR_P(temp_key));
            if ( !t_v || ZVAL_IS_NULL(t_v) ) RETURN_NULL();
            ZVAL_COPY(&temp_value, t_v);
            ZVAL_COPY(&result_key, temp_key);
        } ZEND_HASH_FOREACH_END();
        ZVAL_COPY(return_value, &temp_value);
        zval_ptr_dtor(&temp_value);
        zval_ptr_dtor(&result_key);
    } else {
        if ( !zend_hash_exists( Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))), key) ) RETURN_NULL();
        zval *val = php_get_session_var(key);
        ZVAL_COPY(return_value, val);
    }
    zend_array_destroy(Z_ARRVAL(keys));
}/*}}}*/

/**
 * proto proto Session::unset($key)
 */
XAN_METHOD(Session, unset)
{
    zend_string *key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE ) {
        return ;
    }

    if ( !ZSTR_LEN(key) ) {
        XAN_INFO(E_ERROR, "$key must be valid.");
    }

    zend_hash_del(Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))), key);
    
    RETURN_TRUE
}/*}}}*/

/**
 * proto proto Session::count(void)
 */
XAN_METHOD(Session, count)
{
    zend_string *key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE ) {
        return ;
    }

    zend_long num = Z_H_N_E(Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))));
    ZVAL_LONG(return_value, num);
}/*}}}*/

/**
 * proto proto Session::exists($key)
 */
XAN_METHOD(Session, exists)
{
    zend_string *key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE ) {
        return ;
    }

    RETURN_BOOL( zend_hash_exists( Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))), key) );
}/*}}}*/

/**
 * proto proto Session::current(void)
 */
XAN_METHOD(Session, current)
{
    zend_string *key;

    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }

    RETURN_ZVAL( zend_hash_get_current_data( Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))) ), 1, NULL);
}/*}}}*/

/**
 * proto proto Session::key(void)
 */
XAN_METHOD(Session, key)
{
    zend_string *key;

    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }

    zend_hash_get_current_key_zval( Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))), return_value );
}/*}}}*/

/**
 * proto proto Session::next(void)
 */
XAN_METHOD(Session, next)
{
    zend_string *key;

    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }
    HashTable *array = Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars)));

	zend_hash_move_forward(array);
}/*}}}*/

/**
 * proto proto Session::rewind(void)
 */
XAN_METHOD(Session, rewind)
{
    zend_string *key;

    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }

    HashTable *array = Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars)));

    zend_hash_internal_pointer_reset(array);
}/*}}}*/

/**
 * proto proto Session::valid(void)
 */
XAN_METHOD(Session, valid)
{
    zend_string *key;

    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }

    HashTable *array = Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars)));
    RETURN_BOOL( zend_hash_has_more_elements(array) == SUCCESS );
}/*}}}*/


/*{{{ proto Session's functions 
 */
XAN_FUNCTIONS(session)
    XAN_ME(Session, __construct, arginfo_xan_session_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Session, getRaw, arginfo_xan_session_get_raw, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
    XAN_ME(Session, set, arginfo_xan_session_set, ZEND_ACC_PUBLIC)
    XAN_ME(Session, get, arginfo_xan_session_get, ZEND_ACC_PUBLIC)
    XAN_ME(Session, current, arginfo_xan_session_current, ZEND_ACC_PUBLIC)
    XAN_ME(Session, key, arginfo_xan_session_key, ZEND_ACC_PUBLIC)
    XAN_ME(Session, next, arginfo_xan_session_next, ZEND_ACC_PUBLIC)
    XAN_ME(Session, rewind, arginfo_xan_session_rewind, ZEND_ACC_PUBLIC)
    XAN_ME(Session, valid, arginfo_xan_session_valid, ZEND_ACC_PUBLIC)
    XAN_ME(Session, count, arginfo_xan_session_count, ZEND_ACC_PUBLIC)
    XAN_MALIAS(Session, __set, set, arginfo_xan_session_set, ZEND_ACC_PUBLIC)
    XAN_MALIAS(Session, __get, get, arginfo_xan_session_get, ZEND_ACC_PUBLIC)
    XAN_MALIAS(Session, offsetGet, get, arginfo_xan_session_get, ZEND_ACC_PUBLIC)
    XAN_MALIAS(Session, offsetSet, set, arginfo_xan_session_set, ZEND_ACC_PUBLIC)
    XAN_MALIAS(Session, offsetUnset, unset, arginfo_xan_session_unset, ZEND_ACC_PUBLIC)
    XAN_MALIAS(Session, offsetExists, exists, arginfo_xan_session_exists, ZEND_ACC_PUBLIC)
XAN_FUNCTIONS_END()
/*}}}*/

/* {{{ proto session_init(void)
 */
XAN_INIT(session)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan", "Session", session_functions);
    xan_session_ce = zend_register_internal_class(&ce);
    xan_session_ce->ce_flags |= ZEND_ACC_FINAL;

#if defined(HAVE_SPL) && PHP_VERSION_ID < 70200
	zend_class_implements(xan_session_ce, 3, zend_ce_iterator, zend_ce_arrayaccess, spl_ce_Countable);
#elif PHP_VERSION_ID >= 70200
	zend_class_implements(xan_session_ce, 3, zend_ce_iterator, zend_ce_arrayaccess, zend_ce_countable);
#else
	zend_class_implements(xan_session_ce, 2, zend_ce_iterator, zend_ce_arrayaccess);
#endif

    XAN_PR_BOOL(xan_session_ce, XAN_SESSION_STATUS, 0, ZEND_ACC_PRIVATE);
    XAN_PR_NULL(xan_session_ce, XAN_SESSION_INSTANCE, ZEND_ACC_PRIVATE);
}/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */