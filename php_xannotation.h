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

#define PHP_XANNOTATION_VERSION "0.1.9" /* Replace with version number for your extension */

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
*/

ZEND_BEGIN_MODULE_GLOBALS(xannotation)
    zval app_di;              /* global app object */
    zval aliases;             /* all aliases which used to auto_load */
    zval class_di;            /* global di for object reuse */
    zval call_chain;          /* the call chain for annotation */
    char url_pattern;         /* 0: auto, 1: get, 2: path_info */
    zval url_get_str;         /* [zend_string]the url get str for the get mode */
    zval default_module;      /* [zend_string]the default module */
    zval default_controller;  /* [zend_string]the default controller */
    zval default_action;      /* [zend_string]the default action */
    zval view_suffix;         /* [zend_string]the view suffix */
    zval url_suffix;          /* [zend_string]the url suffix */
    zend_bool must_url_suffix;/* [bool]the url must contain the url suffix [0 or 1] */
    zval allow_modules;       /* [array]all modules which can be used to route */
    zend_bool auto_render;    /* 0 disable auto_render 1 auto_render */
    zval application_dir;     /* [zend_string]the application directory */
    zval default_namespace;   /* [zend_string]the default namespace */
    zval bootstrap;           /* [zval] the bootstrap class path */
    zval pdo_object;          /* [zval] the PDO object */
ZEND_END_MODULE_GLOBALS(xannotation)

/* Always refer to the globals in your function as XANNOTATION_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define XAN_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(xannotation, v)

#if defined(ZTS) && defined(COMPILE_DL_XANNOTATION)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

/**
 * {{{
 * User should to modify this value to 0 when in production mode
 */
#define DEBUG_MODE 1
/*}}}*/

/**
 * {{{
 * The following are some macros for easy extension development
 */
#define XAN_ME                    PHP_ME
#define XAN_AB_ME                 PHP_ABSTRACT_ME
#define XAN_METHOD                PHP_METHOD
#define XAN_INIT(name)            void name##_init()
#define ARGINFO(name)             arginfo_##name
#define XAN_MALIAS                PHP_MALIAS

#define XAN_FUNCTIONS(name)       \
    static const zend_function_entry name##_functions[] = {
#define XAN_FUNCTIONS_END()       PHP_FE_END };
#define XAN_THREAD_FUNC(name)     void *thread_##name(void *arg)

#define XAN_ANNOTATIONS     "Xan\\Type\\Annotation\\"
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
#define XAN_CHECK_METHOD(object, method_name)  \
    ( ( Z_OBJ_HT_P(object)->get_method(&Z_OBJ_P(object), strpprintf(0, "%s", XAN_STRL(method_name)), NULL)) )
#define ZS_LOWER zend_string_tolower

#define STR_FIND_P(zv, zkey) zend_hash_find(Z_ARRVAL_P(zv), (zkey))
#define STRING_FIND_P(zv, skey) zend_hash_str_find(Z_ARRVAL_P(zv), XAN_STRL(skey))
#define INDEX_FIND_P(zv, ikey) zend_hash_index_find(Z_ARRVAL_P(zv), (ikey))

#define STR_FIND(zv, zkey) zend_hash_find(Z_ARRVAL(zv), (zkey))
#define STRING_FIND(zv, skey) zend_hash_str_find(Z_ARRVAL(zv), XAN_STRL(skey))
#define INDEX_FIND(zv, ikey) zend_hash_index_find(Z_ARRVAL(zv), (ikey))

#define STR_PTR_FIND_P(zv, zkey) zend_hash_find_ptr(Z_ARRVAL_P(zv), (zkey))
#define STRING_PTR_FIND_P(zv, skey) zend_hash_str_find_ptr(Z_ARRVAL_P(zv), XAN_STRL(skey))
#define INDEX_PTR_FIND_P(zv, ikey) zend_hash_index_find_ptr(Z_ARRVAL_P(zv), (ikey))

#define STR_PTR_FIND(zv, zkey) zend_hash_find_ptr(Z_ARRVAL(zv), (zkey))
#define STRING_PTR_FIND(zv, skey) zend_hash_str_find_ptr(Z_ARRVAL(zv), XAN_STRL(skey))
#define INDEX_PTR_FIND(zv, ikey) zend_hash_index_find_ptr(Z_ARRVAL(zv), (ikey))

#define Z_H_F_P  zend_hash_find_ptr
#define Z_H_I_F_P zend_hash_index_find_ptr
#define Z_H_S_F_P zend_hash_str_find_ptr
#define Z_H_A  zend_hash_add
#define Z_H_A_N zend_hash_add_new
#define Z_H_A_P zend_hash_add_ptr
#define Z_H_A_N_P zend_hash_add_new_ptr
#define Z_H_N_E zend_hash_num_elements



/*}}}*/


extern ZEND_DECLARE_MODULE_GLOBALS(xannotation);

#endif	/* PHP_XANNOTATION_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
