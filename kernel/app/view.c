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
#include "kernel/app/view.h"
#include "php_xannotation.h"
#include "ext/standard/info.h"
#include "kernel/loader/loader.h"
#include "ext/standard/php_string.h"

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_view_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_view_render), 0, 0, 2)
    ZEND_ARG_INFO(0, template)
    ZEND_ARG_INFO(0, variables)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_view_assign), 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_view_set_auto_render), 0, 0, 0)
    ZEND_ARG_INFO(0, status)
ZEND_END_ARG_INFO()
/**
 * {{{ proto 
 * View::__construct()
 */
XAN_METHOD(View, __construct)
{
    zval *variables = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL("variables"), 1, NULL);
    array_init(variables);
}/*}}}*/

/**
 * {{{ proto 
 * View::render()
 */
XAN_METHOD(View, render)
{
    zval *variables = NULL, *dvariables, *value;
    zend_string *template_file, *render_file, *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S|z", &template_file, &variables) == FAILURE) {
        return ;
    }

    dvariables = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL("variables"), 1, NULL);
    if ( variables && !ZVAL_IS_NULL(variables) ) {
        if ( !dvariables || ZVAL_IS_NULL(dvariables)) {
            array_init(dvariables);
        }
        zend_hash_merge(Z_ARRVAL_P(dvariables), Z_ARRVAL_P(variables), zval_add_ref, 0);
    }

    Z_STRVAL(XAN_G(default_controller))[0] = tolower(Z_STRVAL(XAN_G(default_controller))[0]);
    render_file = strpprintf(0, "%s/modules/%s/views/%s/%s.%s", 
                Z_STRVAL(XAN_G(application_dir)), Z_STRVAL(XAN_G(default_module)), 
                Z_STRVAL(XAN_G(default_controller)), ZSTR_VAL(template_file), Z_STRVAL(XAN_G(view_suffix))
    );

    is_file(ZSTR_VAL(render_file));
    xan_require_file(ZSTR_VAL(render_file), dvariables, getThis(), NULL);
}/*}}}*/


/**
 * {{{
 * proto View::assign($name, $variables)
 */
XAN_METHOD(View, assign)
{
    zend_string *name;
    zval *variables, *dvariables;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &name, &variables) == FAILURE) {
        return ;
    }

    if (ZSTR_VAL(name)[0] == '\0') {
        XAN_INFO(E_ERROR, "$name was invalid!");
    }

    dvariables = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL("variables"), 1, NULL);
    add_assoc_zval(dvariables, ZSTR_VAL(name), variables);
}/*}}}*/

/**
 * {{{ 
 * proto View::setAutoRender($status = true)
 */
XAN_METHOD(View, setAutoRender)
{
    zend_bool status = 1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &status) == FAILURE) {
        return ;
    }

    XAN_G(auto_render) = status;
}/*}}}*/

/**
 * {{{
 * view's function
 */
XAN_FUNCTIONS(view)
    XAN_ME(View, __construct, arginfo_xan_view_construct, ZEND_ACC_PUBLIC)
    XAN_ME(View, render, arginfo_xan_view_render, ZEND_ACC_PUBLIC)
    XAN_ME(View, assign, arginfo_xan_view_render, ZEND_ACC_PUBLIC)
    XAN_ME(View, setAutoRender, arginfo_xan_view_set_auto_render, ZEND_ACC_PUBLIC)
XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{ proto 
 * view_init
 */
XAN_INIT(view)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan", "View", view_functions);
    xan_view_ce = zend_register_internal_class(&ce);

    XAN_PR_NULL(xan_view_ce, "variables", ZEND_ACC_PRIVATE);
}/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */