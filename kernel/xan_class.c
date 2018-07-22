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
#include "ext/standard/php_string.h"

/**
 * {{{ All arginfo for all class funtions
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_get_class_doc_comment), 0, 0, 1)
    ZEND_ARG_INFO(0, objectOrName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_get_method_doc_comment), 0, 0, 2)
    ZEND_ARG_INFO(0, objectOrName)
    ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_parse_doc_comment), 0, 0, 1)
    ZEND_ARG_INFO(0, docComment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_get_parse_result), 0, 0, 1)
    ZEND_ARG_INFO(0, docComment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_class_parse_all_methods_doc_comment), 0, 0, 1)
    ZEND_ARG_INFO(0, objectOrName)
ZEND_END_ARG_INFO()
/*}}}*/

/**
 * {{{ proto Xan::__construct($className)
 * The construct function to construct the class with the given class name
 */
XAN_METHOD(Xan, __construct)
{
}/*}}}*/

/**
 * {{{ proto Xan::getClassDocComment($className)
 * Get the doc-comment for the given class
 * RETURN the doc-comment for the given class or object
 */
XAN_METHOD(Xan, getClassDocComment)
{
    zval *class_name;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &class_name) == FAILURE)
    {
        return ;
    }

    if (Z_TYPE_P(class_name) == IS_OBJECT)
    {
        RETURN_STR(get_class_doc_comment(Z_OBJCE_P(class_name)));
    }
    else if (Z_TYPE_P(class_name) == IS_STRING)
    {
        zend_class_entry *ce = zend_lookup_class(Z_STR_P(class_name));
        RETURN_STR(get_class_doc_comment(ce));
    }
    else
    {
        XAN_INFO(E_ERROR, "Parameter className must be Object or String!");
    }

}/*}}}*/

/**
 * {{{ Xan::getMethodDocComment($objectOrName, $methodName)
 * To obtain the doc-comment for the method of the given object
 */
XAN_METHOD(Xan, getMethodDocComment)
{
    zval *object_or_name;
    zend_string *method_name;
    zend_class_entry *class_entry;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zS", &object_or_name, &method_name) == FAILURE)
    {
        return;
    }

    if ( !ZSTR_LEN(method_name) )
    {
        XAN_INFO(E_ERROR, "MethodName must be valid string!");
    }

    method_name = zend_string_tolower(method_name);

    if (Z_TYPE_P(object_or_name) == IS_STRING)
    {
        class_entry = zend_lookup_class(Z_STR_P(object_or_name));
    }
    else if (Z_TYPE_P(object_or_name) == IS_OBJECT)
    {
        class_entry = Z_OBJCE_P(object_or_name);
    }
    else
    {
        XAN_INFO(E_ERROR, "Parameter className must be Object or String!");
    }

    zend_function *method_value = zend_hash_str_find_ptr(&class_entry->function_table, ZSTR_VAL(method_name), ZSTR_LEN(method_name));
    if (!method_value)
    {
        XAN_INFO(E_ERROR, "Method: %s not exists in Class %s!", ZSTR_VAL(method_name), ZSTR_VAL(class_entry->name));
    }

    RETURN_STR(get_function_doc_comment(&method_value->op_array));
}/*}}}*/

/**
 * {{{ proto Xan::parseDocComment($docComment)
 * To parse the doc-comment and return is as an array
 */
XAN_METHOD(Xan, parseDocComment)
{
    zval zretval;
    zend_string *doc_comment;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &doc_comment) == FAILURE)
    {
        return ;
    }
    
    zend_object *retval = zend_objects_clone_obj(getThis());
    ZVAL_OBJ(&zretval, retval);

    parse_doc_comment(&zretval, doc_comment);
    ZVAL_COPY_VALUE(return_value, &zretval);
}/*}}}*/

/**
 * {{{ proto Xan::getParseResult($docComment)
 * To obtain the result of the annotation parseing
 */
XAN_METHOD(Xan, getParseResult)
{
    zend_string *doc_comment;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &doc_comment) == FAILURE)
    {
        return ;
    }

    zval retval;
    array_init(&retval);

    get_doc_comment_result(&retval, doc_comment);

    RETURN_ZVAL(&retval, 1, NULL);
}/*}}}*/

/**
 * {{{ proto Xan::parseAllMethodsDocComment($objectOrName)
 * Parsing all methods doc-comment for the given class or object
 * Result is an object which contains the following format:
 * Object
 * {
 *     'annotations' => [
 *         'world' => [
 *             'annotations' => [],
 *             'num' => '',
 *             'body' => ''
 *         ]
 *         'xxx' => []
 *     ],
 *     'num' => 3
 * }
 */
XAN_METHOD(Xan, parseAllMethodsDocComment)
{
    zend_class_entry *ce;
    zend_function *each_func;
    zval *object_or_name, each_func_ret, all_rets;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &object_or_name) == FAILURE)
    {
        return ;
    }

    if (Z_TYPE_P(object_or_name) == IS_OBJECT)
    {
        ce = Z_OBJCE_P(object_or_name);
    }
    else if ( Z_TYPE_P(object_or_name) == IS_STRING )
    {
        ce = zend_lookup_class(Z_STR_P(object_or_name));
        if( !ce )
        {
            XAN_INFO(E_ERROR, "Class %s not found.", Z_STRVAL_P(object_or_name));
        }
    }
    else
    {
        XAN_INFO(E_ERROR, "Parameter $objectOrName must be Object or Class name.");
    }

    write_long_property_to_object(getThis(), "num", zend_hash_num_elements(&ce->function_table));

    array_init(&all_rets);
    
    ZEND_HASH_FOREACH_PTR(&ce->function_table, each_func)
    {
        array_init(&each_func_ret);
        get_doc_comment_result(&each_func_ret, get_function_doc_comment(&each_func->op_array));
        add_assoc_zval(&all_rets, ZSTR_VAL(each_func->common.function_name), &each_func_ret);

    } ZEND_HASH_FOREACH_END();

    write_zval_property_to_object(getThis(), "annotations", &all_rets);

    RETURN_ZVAL(getThis(), 1, NULL);
}/*}}}*/

/**
 * {{{ 
 * All functions to the Xan class
 */
XAN_FUNCTIONS(xan)
    XAN_ME(Xan, __construct, arginfo_xan_class_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Xan, getClassDocComment, arginfo_xan_class_get_class_doc_comment, ZEND_ACC_PUBLIC)
    XAN_ME(Xan, getMethodDocComment, arginfo_xan_class_get_method_doc_comment, ZEND_ACC_PUBLIC)
    XAN_ME(Xan, parseDocComment, arginfo_xan_class_parse_doc_comment,  ZEND_ACC_PUBLIC)
    XAN_ME(Xan, getParseResult, arginfo_xan_class_get_parse_result,  ZEND_ACC_PUBLIC)
    XAN_ME(Xan, parseAllMethodsDocComment, arginfo_xan_class_parse_all_methods_doc_comment,  ZEND_ACC_PUBLIC)
XAN_FUNCTIONS_END() /*}}}*/

/**
 * {{{ proto xan_init() 
 * function to init the xan class to the php engine
 */
XAN_INIT(xan)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Xan", xan_functions);
    xan_ce = zend_register_internal_class(&ce);
    xan_ce->ce_flags |= ZEND_ACC_FINAL;
}/*}}}*/

/**
 * {{{
 * Some functions for the tool to obtain the doc-comment from object|entry|function
 */
zend_string *get_class_doc_comment(zend_class_entry *ce)
{
    if (!ce)
    {
        return NULL;
    }
    return ce->info.user.doc_comment;
}/*}}}*/

/**
 * {{{ 
 * Get the function doc-comments
 */
zend_string *get_function_doc_comment(zend_op_array *op_array)
{
    if ( !op_array )
    {
        return NULL;
    }
    return op_array->doc_comment;
}/*}}}*/

/**
 * {{{ proto parse_line_comment(zval *retval, char *str)
 * Parsing the str such as : '@type(value="", age="")
 */
static void parse_line_comment(zval *retval, char *str)
{
    zend_long ukey;
    zval temp_array, *value;
    char anno_name[1024] = {0}, anno_attr[1024] = {0}, anno_key[1024] = {0}, anno_value[1024] = {0};
    size_t index = 0, para_left = 0, para_right = 0, c_index = 0, default_value = 1;
    for( str = str + 1; index < strlen(str); index++)
    {
        if (str[index] == '(')
        {
            para_left = index;
            memcpy(anno_name, str, index);
        }
        if (str[index] == ')')
        {
            para_right = index;
            break;
        }
    }

    if ( (para_left == 0) && (para_right == 0))
    {
        para_right = strlen(str);
        memcpy(anno_attr, str + para_left, para_right - para_left);
        add_assoc_null(retval, anno_attr);
        return ;
    }
    else if ( (para_left != 0 && para_right == 0) )
    {
        ZVAL_NULL(retval);
        return ;
    }
    memcpy(anno_attr, str + para_left + 1, para_right - para_left - 1);
    
    array_init(&temp_array);
    php_explode(strpprintf(0, "%s", ","), strpprintf(0, "%s", anno_attr), &temp_array, ZEND_LONG_MAX);

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL(temp_array), ukey, value)
    {
        /*value = "hello world"*/
        zend_string *trim_value = php_trim( Z_STR_P(value), XAN_STRL(" "), 3 );
        for(c_index = 0; c_index < ZSTR_LEN(trim_value); c_index++)
        {
            if ( *(ZSTR_VAL(trim_value) + c_index) == '=' )
            {
                default_value = 0;
                bzero(anno_key, sizeof(anno_key));
                bzero(anno_value, sizeof(anno_value));
                memcpy(anno_key, ZSTR_VAL(trim_value), c_index);
                memcpy(anno_value, ZSTR_VAL(trim_value) + c_index + 1, ZSTR_LEN(trim_value) - c_index - 1);

                add_assoc_str(&temp_array, ZSTR_VAL( php_trim(strpprintf(0, "%s", anno_key), XAN_STRL(" \""), 3)), 
                    php_trim(strpprintf(0, "%s", anno_value), XAN_STRL(" \""), 3));
            }
        }
        if (default_value)
        {
            add_next_index_str(&temp_array, php_trim(trim_value, XAN_STRL(" \""), 3));
        }
        
        zend_hash_index_del(Z_ARRVAL(temp_array), ukey);
        
    } ZEND_HASH_FOREACH_END();

    Z_TRY_ADDREF(temp_array);

    add_assoc_zval(retval, anno_name, &temp_array);
}/*}}}*/

/**
 * {{{
 * To parse the doc-comment for the object or object's method.
 * object: means the value will write to the property of the object
 */
void parse_doc_comment(zval *object, zend_string *doc_comment)
{
    if ( !ZSTR_LEN(doc_comment) )
    {
        XAN_INFO(E_ERROR, "$docComment empty.");
        return ;
    }

    zval anno, ret, func_name, pattern, replace, subject;
    char buff[1024] = {0}, body[2048] = {0};
    char *comments = ZSTR_VAL(doc_comment);
    size_t index = 0, sig_start = 0, comment_body_start = 0, comment_body_end = 0;
    array_init(&anno);

    for( ; index < ZSTR_LEN(doc_comment); index++)
    {
        if (comments[index] == ' ' || comments[index] == '*')
        {
            continue;
        }

        if (comments[index] == '\n' && comment_body_start == 0)
        {
            comment_body_start = index;
        }

        if (comments[index] == '@')
        {
            if (sig_start == 0)
            {
                comment_body_end = index;
            }
            sig_start = index;
        }

        if (comments[index] == '\n' && sig_start != 0)
        {
            bzero(buff, sizeof(buff));
            strncpy(buff, comments + sig_start, index - sig_start);
            parse_line_comment(&anno, buff);
        }
    }
    write_zval_property_to_object(object, "annotations", &anno);
    write_property_to_object(object, "num", ZSTR_VAL(strpprintf(0, "%d", zend_hash_num_elements(Z_ARRVAL(anno))) ) );

    /* To parse the commend_body */
    memcpy(body, comments + comment_body_start, comment_body_end - comment_body_start);

    ZVAL_STRING(&func_name, "preg_replace");
    ZVAL_STRING(&subject, body);
    ZVAL_STRING(&replace, "\n");
    ZVAL_STRING(&pattern, "#\\n\\**( \\**|)#");
    zval params[3] = {pattern, replace, subject};
    call_user_function(EG(function_table), NULL, &func_name, &ret, 3, params);
    zval_ptr_dtor(&func_name);
    zval_ptr_dtor(&subject);
    zval_ptr_dtor(&replace);
    zval_ptr_dtor(&pattern);
    write_property_to_object(object, "body", ZSTR_VAL(php_trim(Z_STR(ret), XAN_STRL(" \n*"), 3)));
}
/*}}}*/

/**
 * {{{ proto Get the result into the retval value
 * retval: means the result is array
 */
void get_doc_comment_result(zval *retval, zend_string *doc_comment)
{
    if (Z_TYPE_P(retval) != IS_ARRAY)
    {
        ZVAL_NULL(retval);
        return ;
    }

    if ( !ZSTR_LEN(doc_comment) )
    {
        ZVAL_NULL(retval);
        XAN_INFO(E_ERROR, "$docComment empty.");
        return ;
    }

    zval anno, ret, func_name, pattern, replace, subject;
    char buff[1024] = {0}, body[2048] = {0};
    char *comments = ZSTR_VAL(doc_comment);
    size_t index = 0, sig_start = 0, comment_body_start = 0, comment_body_end = 0;
    array_init(&anno);

    for( ; index < ZSTR_LEN(doc_comment); index++)
    {
        if (comments[index] == ' ' || comments[index] == '*')
        {
            continue;
        }

        if (comments[index] == '\n' && comment_body_start == 0)
        {
            comment_body_start = index;
        }

        if (comments[index] == '@')
        {
            if (sig_start == 0)
            {
                comment_body_end = index;
            }
            sig_start = index;
        }

        if (comments[index] == '\n' && sig_start != 0)
        {
            bzero(buff, sizeof(buff));
            strncpy(buff, comments + sig_start, index - sig_start);
            parse_line_comment(&anno, buff);
        }
    }

    add_assoc_zval(retval, "annotations", &anno);
    add_assoc_long(retval, "num", zend_hash_num_elements(Z_ARRVAL(anno)));

    /* To parse the commend_body */
    memcpy(body, comments + comment_body_start, comment_body_end - comment_body_start);

    ZVAL_STRING(&func_name, "preg_replace");
    ZVAL_STRING(&subject, body);
    ZVAL_STRING(&replace, "\n");
    ZVAL_STRING(&pattern, "#\\n\\**( \\**|)#");
    zval params[3] = {pattern, replace, subject};
    call_user_function(EG(function_table), NULL, &func_name, &ret, 3, params);
    zval_ptr_dtor(&func_name);
    zval_ptr_dtor(&subject);
    zval_ptr_dtor(&replace);
    zval_ptr_dtor(&pattern);
    add_assoc_str( retval, "body", php_trim(Z_STR(ret), XAN_STRL(" \n*"), 3) );
}/*}}}*/

/**
 * {{{ proto Write property to the object, it will call the __set magic method
 * if exists
 */
static void write_property_to_object(zval *object, char *key, char *value)
{
    zval zkey, zvalue;
    ZVAL_STRING(&zkey,key);
    ZVAL_STRING(&zvalue, value);
    const zend_object_handlers *obj_ht = Z_OBJ_HT_P(object);
    zend_class_entry *old_scope = EG(fake_scope);
    obj_ht->write_property(object, &zkey, &zvalue, NULL);
    zval_ptr_dtor(&zkey);
    zval_ptr_dtor(&zvalue);
    EG(fake_scope) = old_scope;
}/*}}}*/

/**
 * {{{ proto Write property to the object, it will call the __set magic method
 * if exists
 */
static void write_zval_property_to_object(zval *object, char *key, zval *value)
{
    zval zkey;
    ZVAL_STRING(&zkey,key);
    const zend_object_handlers *obj_ht = Z_OBJ_HT_P(object);
    zend_class_entry *old_scope = EG(fake_scope);
    obj_ht->write_property(object, &zkey, value, NULL);
    zval_ptr_dtor(&zkey);
    EG(fake_scope) = old_scope;
}/*}}}*/

/**
 * Write long value to object
 */
static void write_long_property_to_object(zval *object, char *key, zend_long value)
{
    zval zkey, zvalue;
    ZVAL_STRING(&zkey,key);
    ZVAL_LONG(&zvalue, value);
    const zend_object_handlers *obj_ht = Z_OBJ_HT_P(object);
    zend_class_entry *old_scope = EG(fake_scope);
    obj_ht->write_property(object, &zkey, &zvalue, NULL);
    zval_ptr_dtor(&zkey);
    EG(fake_scope) = old_scope;
}/*}}}*/
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

