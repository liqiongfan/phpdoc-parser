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
#include "kernel/xan_class.h"
#include "kernel/loader/loader.h"
#include "kernel/class/aop/proxy.h"
#include "ext/standard/php_string.h"

void t_info(int type, char *info, ...)
{
    zend_string *tinfo = strpprintf(type, info);
    php_write(ZSTR_VAL(tinfo), ZSTR_LEN(tinfo));
}

/**
 * {{{
 * proto All arginfo for class ConfigClass
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_aop_proxy_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_aop_proxy_instance), 0, 0, 1)
    ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_aop_proxy_call), 0, 0, 2)
    ZEND_ARG_INFO(0, functionName)
    ZEND_ARG_INFO(0, functionParameters)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_aop_proxy_get), 0, 0, 1)
    ZEND_ARG_INFO(0, attrName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_aop_proxy_set), 0, 0, 2)
    ZEND_ARG_INFO(0, attrName)
    ZEND_ARG_INFO(0, attrValue)
ZEND_END_ARG_INFO()

/**
 * {{{ proto AopProxy::__construct()
 */
XAN_METHOD(AopProxy, __construct)
{ }
/*}}}*/

/**
 * {{{
 * proto AopProxy::instance($className)
 */
XAN_METHOD(AopProxy, instance)
{
    zend_string *class_name;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &class_name) == FAILURE)
    {
        return ;
    }

    if ( !ZSTR_LEN(class_name) )
    {
        XAN_INFO(E_ERROR, "Class `%s` not valid.", ZSTR_VAL(class_name));
    }

    zend_class_entry *ce = zend_lookup_class(class_name);
    if ( !ce )
    {
        XAN_INFO(E_ERROR, "Class `%s` not found.", ZSTR_VAL(class_name));
    }

    object_init_ex(return_value, proxy_ce);

    zend_update_property_str(XAN_ENTRY_OBJ(return_value), XAN_STRL(CLASS_CE), class_name);
}/*}}}*/

XAN_METHOD(AopProxy, __call)
{
    zval *parameters, caller_obj, ret_val;
    zend_string *function_name;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sa", &function_name, &parameters) == FAILURE)
    {
        return ;
    }

    /* The class container:`&XAN_G(class_di)` */
    if (ZVAL_IS_NULL(&XAN_G(class_di)))
    {
        array_init(&XAN_G(class_di));
    }

    /* Proxy class */
    zval *class_name = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(CLASS_CE), 1, NULL);
    if ( !class_name )
    {
        XAN_INFO(E_ERROR, "Please use the `instance` to get an object proxy.");
    }
    /* Proxy class_entry */
    zend_class_entry *ce = zend_lookup_class(zend_string_tolower(Z_STR_P(class_name)));
    if ( !ce )
    {
        XAN_INFO(E_ERROR, "Class `%s` not found.", Z_STRVAL_P(class_name));
    }

    get_object_from_di(&XAN_G(class_di), Z_STR_P(class_name), &caller_obj, ce);

    /* To found wheather the class is @Aspect */
    zval annotations;
    array_init(&annotations);
    get_doc_comment_result(&annotations, get_class_doc_comment(ce) );

    zval *class_annotations = zend_hash_str_find(Z_ARRVAL(annotations), XAN_STRL("annotations"));
    if ( !class_annotations || ZVAL_IS_NULL(class_annotations) )
    {
        goto exit_no_annotation;
    }

    zval *aspect = zend_hash_str_find(Z_ARRVAL_P(class_annotations), XAN_STRL("Aspect"));
    if ( aspect )
    {
        /* Found the aspect annotation */
        zend_string *func_name;
        zval func_annotations, *function_value;
        zend_function *calling_function = zend_hash_find_ptr(&ce->function_table, zend_string_tolower(function_name));
        array_init(&func_annotations);
        get_doc_comment_result(&func_annotations, get_function_doc_comment(&calling_function->op_array));

        zval *all_annotations = zend_hash_str_find( Z_ARRVAL(func_annotations), XAN_STRL("annotations") );
        
        if ( all_annotations && zend_hash_num_elements(Z_ARRVAL_P(all_annotations)))
        {
            zval *before_func_name = zend_hash_str_find(Z_ARRVAL_P(all_annotations), XAN_STRL("before"));
            zval *after_func_name = zend_hash_str_find(Z_ARRVAL_P(all_annotations), XAN_STRL("after"));
            zval *success_func_name = zend_hash_str_find(Z_ARRVAL_P(all_annotations), XAN_STRL("success"));
            zval *failure_func_name = zend_hash_str_find(Z_ARRVAL_P(all_annotations), XAN_STRL("failure"));
            
            run_method(before_func_name, return_value);
            
            ZVAL_TRUE(&ret_val);
            call_method_with_object_params( &caller_obj, ZSTR_VAL(zend_string_tolower(function_name)), parameters, &ret_val );

            if (Z_TYPE_INFO(ret_val) == IS_TRUE)
            {
                run_method(success_func_name, return_value);
            }
            else
            {
                run_method(failure_func_name, return_value);
            }

            run_method(after_func_name, return_value);

            return ;
        }
    }

exit_no_annotation:
    call_method_with_object_params( &caller_obj, ZSTR_VAL(zend_string_tolower(function_name)), parameters, return_value );
}

/**
 * {{{
 * proto AopProxy::__set($attrName, $attrValue)
 */
XAN_METHOD(AopProxy, __set)
{
    zend_string *key;
    zval *value, class_obj, z_key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &key, &value) == FAILURE)
    {
        return ;
    }

    /* The class container:`&XAN_G(class_di)` */
    if (ZVAL_IS_NULL(&XAN_G(class_di)))
    {
        array_init(&XAN_G(class_di));
    }

    /* Proxy class */
    zval *class_name = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(CLASS_CE), 1, NULL);
    if ( !class_name )
    {
        XAN_INFO(E_ERROR, "Please use the `instance` to get an object proxy.");
    }
    /* Proxy class_entry */
    zend_class_entry *ce = zend_lookup_class(zend_string_tolower(Z_STR_P(class_name)));
    if ( !ce )
    {
        XAN_INFO(E_ERROR, "Class `%s` not found.", Z_STRVAL_P(class_name));
    }

    get_object_from_di(&XAN_G(class_di), Z_STR_P(class_name), &class_obj, ce);

    ZVAL_STR(&z_key, key);
    Z_OBJ(class_obj)->handlers->write_property(&class_obj, &z_key, value, NULL);
}/*}}}*/

/**
 * {{{ proto AopProxy::__get($attrName)
 */
XAN_METHOD(AopProxy, __get)
{
    zval class_obj, z_key;
    zend_string *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) ==FAILURE)
    {
        return ;
    }

    /* The class container:`&XAN_G(class_di)` */
    if (ZVAL_IS_NULL(&XAN_G(class_di)))
    {
        array_init(&XAN_G(class_di));
    }
    /* Proxy class */
    zval *class_name = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(CLASS_CE), 1, NULL);
    if ( !class_name )
    {
        XAN_INFO(E_ERROR, "Please use the `instance` to get an object proxy.");
    }
    /* Proxy class_entry */
    zend_class_entry *ce = zend_lookup_class(zend_string_tolower(Z_STR_P(class_name)));
    if ( !ce )
    {
        XAN_INFO(E_ERROR, "Class `%s` not found.", Z_STRVAL_P(class_name));
    }

    get_object_from_di(&XAN_G(class_di), Z_STR_P(class_name), &class_obj, ce);
    
    ZVAL_STR(&z_key, key);
    RETURN_ZVAL( Z_OBJ(class_obj)->handlers->read_property(&class_obj, &z_key, BP_VAR_IS, NULL, NULL), 1, NULL);
}/*}}}*/

XAN_FUNCTIONS(aop_proxy)
    XAN_ME(AopProxy, __construct, arginfo_xan_aop_proxy_construct, ZEND_ACC_PRIVATE)
    XAN_ME(AopProxy, instance, arginfo_xan_aop_proxy_instance, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    XAN_ME(AopProxy, __call, arginfo_xan_aop_proxy_call, ZEND_ACC_PUBLIC)
    XAN_ME(AopProxy, __get, arginfo_xan_aop_proxy_get, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(AopProxy, __set, arginfo_xan_aop_proxy_set, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
XAN_FUNCTIONS_END()

/**
 * {{{ proto aop_proxy_functions
 */
XAN_INIT(aop_proxy)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan\\Aop", "Proxy", aop_proxy_functions);
    proxy_ce = zend_register_internal_class(&ce);

    XAN_PR_ESTRING(proxy_ce, CLASS_CE, ZEND_ACC_PRIVATE);
    XAN_PR_NULL(proxy_ce, CLASS_DI, ZEND_ACC_PRIVATE);
}/*}}}*/

/**
 * {{{
 * run_method()
 * To run the function with the parameters
 * function_value was the array contains class name & method name concat with a `.` char
 * such as: [`Base.before`], means to run the Base class's before method
 */
void run_method(zval *function_value, zval *retval)
{
    zval val, z_obj;
    array_init(&val);
    zend_class_entry *c_ce;
    zval *fun_name, func_parameters/*In Used*/;

    /*all parameters need to passed to the user function */
    array_init(&func_parameters);

    if ( !function_value || ZVAL_IS_NULL(function_value) ) return ;

    if (zend_hash_num_elements( Z_ARRVAL_P(function_value) )  == 1 )
    {
        fun_name = zend_hash_index_find(Z_ARRVAL_P(function_value), 1);
    }
    else
    {
        fun_name = zend_hash_str_find( Z_ARRVAL_P(function_value), XAN_STRL(VALUE) );
        zval *paras = zend_hash_str_find( Z_ARRVAL_P(function_value), XAN_STRL(PARAMETERS) );
        if ( Z_STRLEN_P(paras) )
        {
            php_explode(strpprintf(0, "%s", "||"), Z_STR_P(paras), &func_parameters, ZEND_LONG_MAX);
        }
    }
    
    php_explode(strpprintf(0, "%s", "."), Z_STR_P(fun_name), &val, ZEND_LONG_MAX);
    if ( zend_hash_num_elements( Z_ARRVAL(val) ) <= 1)
        return ;
    
    zval *c_name = zend_hash_index_find(Z_ARRVAL(val), 0);
    zval *method_name = zend_hash_index_find(Z_ARRVAL(val), 1);
    
    if (Z_STRVAL_P(c_name)[0] == '\\')
    {
        ZVAL_STRING(c_name, Z_STRVAL_P(c_name) + 1);
    }
    c_ce = zend_lookup_class( zend_string_tolower(Z_STR_P(c_name)) );

    get_object_from_di(&XAN_G(class_di), Z_STR_P(c_name), &z_obj, c_ce);

    if (zend_hash_num_elements(Z_ARRVAL(func_parameters)))
        call_method_with_object_params(&z_obj, Z_STRVAL_P(method_name), &func_parameters, retval);
    else
        call_user_function(NULL, &z_obj, method_name, retval, 0, NULL);
}

/**
 * Get object from di when object not in di
 * create it and put it into the di
 */
void get_object_from_di(zval *di, zend_string *class_name, zval *class_obj, zend_class_entry *ce)
{
    if (zend_hash_find( Z_ARRVAL_P(di), zend_string_tolower(class_name)))
    {
        ZVAL_COPY(class_obj, zend_hash_find( Z_ARRVAL_P(di), zend_string_tolower(class_name)));
    }
    else
    {
        object_init_ex(class_obj, ce);
        add_assoc_zval(di, ZSTR_VAL(zend_string_tolower(class_name)), class_obj);
    }
}/*}}}*/

/**
 * {{{
 * The following two function was used to call the function with the given parameters
 */
void call_method_with_object(zval *object, char *method_name, uint32_t param_counts, zval params[], zval *ret_val)
{
    if (XAN_CHECK_METHOD(object, method_name) != NULL)
    {
        zval function_name;
        ZVAL_STRING(&function_name, method_name);
        call_user_function( NULL, object, &function_name, ret_val, param_counts, params );
        zval_ptr_dtor(&function_name);
    }
}

void call_method_with_object_params( zval *object, char *method_name, zval *parameters, zval *ret_val )
{
    uint32_t p_counts = zend_hash_num_elements(Z_ARRVAL_P(parameters));
    if ( p_counts ) 
    {
        uint32_t n = 0;
        zval *val_para;

        zval *params = (zval *)emalloc(sizeof(zval) * p_counts );
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(parameters), val_para) {
            ZVAL_COPY(&params[n], val_para);
            n++;
        } ZEND_HASH_FOREACH_END();
        
        call_method_with_object( object, method_name, p_counts, params, ret_val );

        efree(params);
    }
    else
    {
        call_method_with_object(object, method_name, 0, NULL, ret_val);
    }
}
/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */