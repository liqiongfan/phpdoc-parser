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

/**
 * {{{
 * proto All arginfo for class ConfigClass
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_annotation_input), 0, 0, 2)
    ZEND_ARG_INFO(0, classEntry)
    ZEND_ARG_INFO(0, annotations)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{ proto
 * Annotation::input($annotations = [])
 */
XAN_METHOD(Annotation, input)
{
}/*}}}*/

/**
 * {{{
 * functions for annotation
 */
XAN_FUNCTIONS(annotation)
    XAN_AB_ME(Annotation, input, arginfo_xan_annotation_input)
XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{ annotation_init()
 */
XAN_INIT(annotation)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan\\Type\\Annotation", "Annotation", annotation_functions);
    annotation_ce = zend_register_internal_interface(&ce);
}/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
