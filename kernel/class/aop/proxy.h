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
  | Author:  Josin                                                       |
  +----------------------------------------------------------------------+
*/

/* $Id$ */
#ifndef XAN_PROXY_H
#define XAN_PROXY_H

#define CLASS_CE "__class_ce"
#define CLASS_DI "__class_di"

#define VALUE    "value"
#define PARAMETERS "parameters"

extern zend_class_entry *proxy_ce;
void run_method(zval *function_value, zval *retval);
void get_object_from_di(zval *di, zend_string *class_name, zval *class_obj, zend_class_entry *ce);
void call_method_with_object_zval( zval *object, char *method_name, zval *parameters, zval *ret_val );
void call_method_with_object_array(zval *object, char *method_name, uint32_t param_counts, zval params[], zval *ret_val);
void call_annotation_function(zval *proxy_obj, zend_string *caller_class_ce, zend_string *function_name, zval *parameters, zval *retval);

#endif /*XAN_PROXY_H*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
