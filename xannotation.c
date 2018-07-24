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
#include "Zend/zend_interfaces.h"

/* If you declare any globals in php_xannotation.h uncomment this:
*/
ZEND_DECLARE_MODULE_GLOBALS(xannotation)

/* True global resources - no need for thread safety here */
static int le_xannotation;

/**
 * All declaration for the function in the init
 */
XAN_INIT(xan);
XAN_INIT(loader);
XAN_INIT(aop_proxy);
XAN_INIT(annotation);
XAN_INIT(config_class);
XAN_INIT(attr_annotation);
XAN_INIT(const_annotation);
XAN_INIT(aspect_annotation);

/*}}}*/

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("xannotation.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_xannotation_globals, xannotation_globals)
    STD_PHP_INI_ENTRY("xannotation.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_xannotation_globals, xannotation_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string get_xan_version()
   Return a string which contains the version of the xan extension */
PHP_FUNCTION(get_xan_version)
{
	if (zend_parse_parameters_none() == FAILURE)
	{
		return ;
	}
	RETURN_STRING(PHP_XANNOTATION_VERSION);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/* {{{ php_xannotation_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_xannotation_init_globals(zend_xannotation_globals *xannotation_globals)
{
	xannotation_globals->global_value = 0;
	xannotation_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(xannotation)
{
#if defined(COMPILE_DL_XANNOTATION) && defined(ZTS)
    ZEND_INIT_MODULE_GLOBALS(xannotation, NULL, NULL);
#endif
	xan_init();
	loader_init();
	aop_proxy_init();
	annotation_init();
	config_class_init();
	attr_annotation_init();
	const_annotation_init();
	aspect_annotation_init();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xannotation)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(xannotation)
{
#if defined(COMPILE_DL_XANNOTATION) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	ZVAL_NULL(&XAN_G(class_di));
	ZVAL_NULL(&XAN_G(call_chain));
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(xannotation)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(xannotation)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xannotation support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ xannotation_functions[]
 *
 * Every user visible function must have an entry in xannotation_functions[].
 */
const zend_function_entry xannotation_functions[] = {
	PHP_FE(get_xan_version,	NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ xannotation_module_entry
 */
zend_module_entry xannotation_module_entry = {
	STANDARD_MODULE_HEADER,
	"xannotation",
	xannotation_functions,
	PHP_MINIT(xannotation),
	PHP_MSHUTDOWN(xannotation),
	PHP_RINIT(xannotation),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(xannotation),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(xannotation),
	PHP_XANNOTATION_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_XANNOTATION
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(xannotation)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
