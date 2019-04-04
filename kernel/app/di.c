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
#include "kernel/app/di.h"
#include "ext/standard/info.h"
#include "ext/session/php_session.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_interfaces.h"


ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_di_set), 0, 0, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_di_get), 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()


/** {{{ proto Di::set($key, $value)
 */
XAN_METHOD(Di, set)
{
    zval retval;
    zend_string *key;
    zend_fcall_info zfi;
    zend_fcall_info_cache zfic;

    int num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sf", 
            &key, &zfi, &zfic, &zfi.param_count, &zfi.params) == FAILURE ) { return ; }

    
    zfi.retval = &retval;
    zend_call_function(&zfi, &zfic);

    if ( Z_TYPE(retval) == IS_OBJECT && !ZVAL_IS_NULL(&XAN_G(class_di)) ) {
        add_assoc_zval(&XAN_G(class_di), ZSTR_VAL(key), &retval ); RETURN_TRUE
    } else {
        XAN_INFO(E_ERROR, "Parameter must return a class object.");
        RETURN_FALSE
    }
}/*}}}*/

/**
 * proto proto Di::get($key)
 */
XAN_METHOD(Di, get)
{
    zend_string *key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE ) {
        return ;
    }

    if ( !ZSTR_LEN(key) || !key ) {
        XAN_INFO(E_ERROR, "$key must be valid.");
    }

    if (ZVAL_IS_NULL(&XAN_G(class_di))) {
        XAN_INFO(E_ERROR, "Di container must be set before get operation.");
    }

    zval *object = zend_hash_find(Z_ARRVAL(XAN_G(class_di)), key ); 
    if (object) {
        RETURN_ZVAL(object, 1, NULL);
    } else {
        XAN_INFO(E_WARNING, "Value not found in Di object named '%s'.", ZSTR_VAL(key)); RETURN_NULL()
    }
    
}/*}}}*/

/*{{{ proto Di's functions 
 */
XAN_FUNCTIONS(di)
    XAN_ME(Di, set, arginfo_xan_di_set, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    XAN_ME(Di, get, arginfo_xan_di_get, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
XAN_FUNCTIONS_END()
/*}}}*/

/* {{{ proto xdi_init(void)
 */
XAN_INIT(xdi)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan", "Di", di_functions);
    xan_di_ce = zend_register_internal_class(&ce);
    xan_di_ce->ce_flags |= ZEND_ACC_FINAL;

}/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */