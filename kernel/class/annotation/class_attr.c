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
#include "kernel/class/annotation/annotation.h"
#include "kernel/class/annotation/class_attr.h"

/**
 * {{{
 * proto All arginfo for class ConfigClass
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_attr_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_attr_input), 0, 0, 2)
    ZEND_ARG_INFO(0, classEntry)
    ZEND_ARG_INFO(0, annotations)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{ 
 * proto AttrAnnotation::__construct()
 */
XAN_METHOD(AttrAnnotation, __construct)
{
}/*}}}*/


/**
 * {{{ 
 * proto AttrAnnotation::input($classEntry, $annotations)
 */
XAN_METHOD(AttrAnnotation, input)
{
    zval *annotations;
    zend_string *ce_name;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sa", &ce_name, &annotations) == FAILURE)
    {
        return ;
    }

    if ( !zend_hash_num_elements(Z_ARRVAL_P(annotations)) )
    {
        return ;
    }

    ce_name = zend_string_tolower(ce_name);
    zend_class_entry *ce = zend_hash_str_find_ptr(CG(class_table), ZSTR_VAL(ce_name), ZSTR_LEN(ce_name));
    if ( !ce )
    {
        return ;
    }

    zval *each_class_attr_value;
    zend_string *each_class_attr_name;

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(annotations), each_class_attr_name, each_class_attr_value)
    {
        zend_declare_property_ex(ce, each_class_attr_name, each_class_attr_value, ZEND_ACC_PUBLIC, NULL);

    } ZEND_HASH_FOREACH_END();

}/*}}}*/

/**
 * {{{
 * proto AttrAnnotation's fucntions
 */
XAN_FUNCTIONS(attr_annotation)
    XAN_ME(AttrAnnotation, __construct, arginfo_xan_class_attr_construct, ZEND_ACC_PUBLIC)
    XAN_ME(AttrAnnotation, input, arginfo_xan_class_attr_input, ZEND_ACC_PUBLIC)

XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{
 * attr_annotation_init()
 */
XAN_INIT(attr_annotation)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan\\Type\\Annotation", "AttrAnnotation", attr_annotation_functions);
    class_attr_ce = zend_register_internal_class(&ce);

    // zend_do_inherit_interfaces(class_attr_ce, annotation_ce);
    zend_class_implements(class_attr_ce, 1, annotation_ce);
}/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
