/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
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
#ifndef XAN_CLASS_H
#define XAN_CLASS_H

extern zend_class_entry *xan_ce;

zend_string *get_class_doc_comment(zend_class_entry *ce);
zend_string *get_function_doc_comment(zend_op_array *op_array);
void parse_doc_comment(zval *object, zend_string *doc_comment);
static void get_doc_comment_result(zval *retval, zend_string *doc_comment);
static void write_property_to_object(zval *object, char *key, char *value);
static void write_zval_property_to_object(zval *object, char *key, zval *value);
static void write_long_property_to_object(zval *object, char *key, zend_long value);
#endif /* XAN_CLASS_H */




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
