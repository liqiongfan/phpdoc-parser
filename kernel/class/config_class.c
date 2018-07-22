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
#include "ext/standard/info.h"
#include "php_xannotation.h"
#include "ext/standard/php_string.h"
#include "kernel/class/config_class.h"

/**
 * {{{
 * proto All arginfo for class ConfigClass
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_config_class_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_config_class_set_constant), 0, 0, 3)
    ZEND_ARG_INFO(0, className)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_config_class_set_attribute), 0, 0, 3)
    ZEND_ARG_INFO(0, object)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
    ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_config_set_access_flag), 0, 0, 3)
    ZEND_ARG_INFO(0, className)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()
/*}}}*/

/**
 * {{{ proto ConfigClass::__construct()
 */
XAN_METHOD(ConfigClass, __construct)
{
}/*}}}*/

/**
 * {{{
 * proto ConfigClass::setContant($className, $key, $value)
 */
XAN_METHOD(ConfigClass, setConstant)
{
    zval *value;
    zend_string *class_name, *key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SSz", &class_name, &key, &value) == FAILURE)
    {
        return ;
    }

    if ( !ZSTR_LEN(class_name) )
    {
        XAN_INFO(E_ERROR, "$className must be valid class name with namespace.");
    }


    zend_class_entry *ce = zend_lookup_class(zend_string_tolower(class_name));
    if ( !ce )
    {
        XAN_INFO(E_ERROR, "Can't found class `%s`!", ZSTR_VAL(class_name));
    }

    zend_class_constant *constant = emalloc(sizeof(zend_class_constant));
    ZVAL_COPY( &constant->value, value);
    constant->ce = ce;
    constant->doc_comment = NULL;
    constant->value.u2.access_flags = ZEND_ACC_PUBLIC;
    add_const_to_class(ce, key, constant);
    
    RETURN_TRUE ;
}/*}}}*/

/**
 * {{{ 
 * proto ConfigClass::setAttribute($object, $key, $value, $flags = ConfigClass::PUBLIC)
 */
XAN_METHOD(ConfigClass, setAttribute)
{
    zend_string *key;
    zval *value, *object, member;
    zend_long flags = ZEND_ACC_PUBLIC;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oSz|l", &object, &key, &value, &flags) == FAILURE)
    {
        return ;
    }

    zend_property_info *property_info = zend_hash_str_find_ptr( &Z_OBJCE_P(object)->properties_info, ZSTR_VAL(key), ZSTR_LEN(key) );
    if ( property_info )
    {
        property_info->flags = flags;
        uint32_t property_offset = property_info->offset;
        zval *variable_ptr = OBJ_PROP( Z_OBJ_P(object), property_offset );
        zend_assign_to_variable(variable_ptr, value, IS_CV);
    }
    else
    {
        ZVAL_STR(&member, key);
        Z_OBJ_HT_P(object)->write_property(object, &member, value, NULL );
    }
}/*}}}*/

/**
 * {{{
 * proto ConfigClass::setAccessFlags($className, $key, $flags)
 */
XAN_METHOD(ConfigClass, setAccessFlags)
{
    zend_string *class_name, *key;
    zend_long flags = ZEND_ACC_PUBLIC;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SSl", &class_name, &key, &flags) == FAILURE)
    {
        return ; 
    }

    if ( !ZSTR_LEN(class_name) )
    {
        XAN_INFO(E_ERROR, "$className must be valid class name with namespace.");
    }

    zend_class_entry *ce = zend_lookup_class(class_name);
    if ( !ce )
    {
        XAN_INFO(E_ERROR, "Can't found class `%s`!", ZSTR_VAL(class_name));
    }

    zend_property_info *property_info = zend_hash_str_find_ptr( &ce->properties_info, ZSTR_VAL(key), ZSTR_LEN(key) );

    if ( !property_info )
    {
        RETURN_FALSE;
    }

    property_info->flags = flags;

    RETURN_TRUE;
}/*}}}*/

/**
 * {{{
 * proto ConfigClass's all functions
 */
XAN_FUNCTIONS(config_class)
    XAN_ME(ConfigClass, __construct, arginfo_xan_config_class_construct, ZEND_ACC_PUBLIC)
    XAN_ME(ConfigClass, setConstant, arginfo_xan_config_class_set_constant, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    XAN_ME(ConfigClass, setAttribute, arginfo_xan_config_class_set_attribute, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    XAN_ME(ConfigClass, setAccessFlags, arginfo_xan_config_set_access_flag, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{
 * config_class_init()
 */
XAN_INIT(config_class)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan\\Type", "ConfigClass", config_class_functions);
    config_class_ce = zend_register_internal_class(&ce);
    config_class_ce->ce_flags |= ZEND_ACC_FINAL;

    XAN_CC_LONG(config_class_ce, "FINAL", ZEND_ACC_FINAL);
    XAN_CC_LONG(config_class_ce, "PUBLIC", ZEND_ACC_PUBLIC);
    XAN_CC_LONG(config_class_ce, "STATIC", ZEND_ACC_STATIC);
    XAN_CC_LONG(config_class_ce, "PRIVATE", ZEND_ACC_PRIVATE);
    XAN_CC_LONG(config_class_ce, "PROTECTED", ZEND_ACC_PROTECTED);
}/*}}}*/

/**
 * {{{ 
 * proto To add const attr to class
 */
void add_const_to_class(zend_class_entry *ce, zend_string *key, zend_class_constant *value)
{
    zend_hash_add_ptr( &ce->constants_table, key, value );
}/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
