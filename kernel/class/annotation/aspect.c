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
#include "Zend/zend_inheritance.h"
#include "kernel/class/annotation/aspect.h"
#include "kernel/class/annotation/annotation.h"

/**
 * {{{
 * proto All arginfo for class Aspect
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_aspect_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_aspect_input), 0, 0, 2)
    ZEND_ARG_INFO(0, classEntry)
    ZEND_ARG_INFO(0, annotations)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{ 
 * proto Aspect::__construct()
 */
XAN_METHOD(Aspect, __construct)
{
}/*}}}*/


/**
 * {{{ 
 * proto Aspect::input($classEntry, $annotations)
 */
XAN_METHOD(Aspect, input)
{
    zval *annotations;
    zend_string *ce_name;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &ce_name, &annotations) == FAILURE)
    {
        return ;
    }
    /* To do nothing */

}/*}}}*/

/**
 * {{{
 * proto Aspect's fucntions
 */
XAN_FUNCTIONS(aspect_annotation)
    XAN_ME(Aspect, __construct, arginfo_xan_class_aspect_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Aspect, input, arginfo_xan_class_aspect_input, ZEND_ACC_PUBLIC)

XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{
 * aspect_annotation_init()
 */
XAN_INIT(aspect_annotation)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Aspect", aspect_annotation_functions);
    aspect_ce = zend_register_internal_class(&ce);

    zend_class_implements(aspect_ce, 1, annotation_ce);
}/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
