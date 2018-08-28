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
#ifndef XAN_LOADER_H
#define XAN_LOADER_H

#include <string.h>
#define ALL_ALIAS "all_alias"

void is_file(char *path);
void get_current_cwd(char *path);
void require_php_file(char *file_name);
int  xan_require_file(const char * file_name, zval *variables, zval *called_object_ptr, zval *return_view);
void reverse_zend_string_slash(zend_string *string);
void auto_load_classfile(zend_string *file, zval *aliases);
void only_auto_load_file(zend_string *file, zval *aliases);
void init_class_with_annotations(zend_string *class_name, zval *aliases);
void recursive_call_method_without_obj(zend_class_entry *ce, zend_string *name);
void recursive_call_method(zval *object, char *name, zval *retval);
extern zend_class_entry *loader_ce;

#endif /*XAN_LOADER_H*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
