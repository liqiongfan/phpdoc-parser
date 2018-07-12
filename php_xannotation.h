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

#ifndef PHP_XANNOTATION_H
#define PHP_XANNOTATION_H

extern zend_module_entry xannotation_module_entry;
#define phpext_xannotation_ptr &xannotation_module_entry

#define PHP_XANNOTATION_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_XANNOTATION_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_XANNOTATION_API __attribute__ ((visibility("default")))
#else
#	define PHP_XANNOTATION_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(xannotation)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(xannotation)
*/

/* Always refer to the globals in your function as XANNOTATION_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define XANNOTATION_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(xannotation, v)

#if defined(ZTS) && defined(COMPILE_DL_XANNOTATION)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

/**
 * {{{
 * User should to modify this value to 0 when in production mode
 */
#define DEBUG_MODE 0
/*}}}*/

/**
 * {{{
 * The following are some macros for easy extension development
 */
#define XAN_ME                    PHP_ME
#define XAN_AB_ME                 PHP_ABSTRACT_ME
#define XAN_METHOD                PHP_METHOD
#define XAN_INIT(name)            void name##_init()
#define ARGINFO(name)                 arginfo_##name

#define XAN_FUNCTIONS(name)       \
    static const zend_function_entry name##_functions[] = {
#define XAN_FUNCTIONS_END()       PHP_FE_END };
#define XAN_THREAD_FUNC(name)     void *thread_##name(void *arg)

#define XAN_INFO(type, info...)   php_error_docref(NULL, type, ##info)
#define XAN_ENTRY_OBJ(obj)             (Z_OBJCE_P(obj)),(obj)
#define XAN_FOREACH_STR_VAL(ht, key, value) ZEND_HASH_FOREACH_STR_KEY_VAL(ht, key, value)
#define XAN_CC_STRING(entry, name, string_value)         zend_declare_class_constant_string(entry, XAN_STRL(name), string_value)
#define XAN_CC_LONG(entry, name, long_value)             zend_declare_class_constant_long(entry, XAN_STRL(name), long_value)
#define XAN_CC_BOOL(entry, name, bool_value)             zend_declare_class_constant_bool(entry, XAN_STRL(name), bool_value)
#define XAN_CC_DOUBLE(entry, name, double_value)         zend_declare_class_constant_double(entry, XAN_STRL(name), double_value)
#define XAN_CC_ZVAL(entry, name, zval_value)             zend_declare_class_constant(entry, XAN_STRL(name), zval_value)
#define XAN_CC_NULL(entry, name)                         zend_declare_class_constant_null(entry, XAN_STRL(name))
#define XAN_PR_NULL(entry, name, type)                   zend_declare_property_null(entry, XAN_STRL(name), type)
#define XAN_PR_BOOL(entry, name, bool_value, type)       zend_declare_property_bool(entry, XAN_STRL(name), bool_value, type)
#define XAN_PR_LONG(entry, name, long_value, type)       zend_declare_property_long(entry, XAN_STRL(name), long_value, type)
#define XAN_PR_DOUBLE(entry, name, double_value, type)   zend_declare_property_double(entry, XAN_STRL(name), double_value, type)
#define XAN_PR_STRING(entry, name, string_value, type)   zend_declare_property_string(entry, XAN_STRL(name), string_value, type)
#define XAN_PR_ESTRING(entry, name, type)                zend_declare_property_string(entry, XAN_STRL(name), "", type)
#define XAN_STRL(str)                  (str),(sizeof(str)-1)
#define XAN_STRS(str)                  (str),(sizeof(str))
/*}}}*/

#endif	/* PHP_XANNOTATION_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
