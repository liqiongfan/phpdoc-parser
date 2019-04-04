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
#include "kernel/app/net.h"
#include "php_xannotation.h"
#include "zend_smart_str.h"
#include "kernel/db/model.h"
#include "ext/standard/info.h"
#include "kernel/db/adapter.h"
#include "Zend/zend_inheritance.h"
#include "ext/standard/php_string.h"

/**
 * {{{
 * All arginfos for the Model class
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_init), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_table_name), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_set_adapter), 0, 0, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_fields), 0, 0, 1)
    ZEND_ARG_INFO(0, fields)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_where), 0, 0, 1)
    ZEND_ARG_INFO(0, where)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_group_by), 0, 0, 1)
    ZEND_ARG_INFO(0, groupBy)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_having), 0, 0, 1)
    ZEND_ARG_INFO(0, having)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_isset), 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_order_by), 0, 0, 1)
    ZEND_ARG_INFO(0, orderBy)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_limit), 0, 0, 1)
    ZEND_ARG_INFO(0, limit)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_all), 0, 0, 0)
    ZEND_ARG_INFO(0, bindValues)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_one), 0, 0, 0)
    ZEND_ARG_INFO(0, bindValues)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_update), 0, 0, 0)
    ZEND_ARG_INFO(0, datas)
    ZEND_ARG_INFO(0, bindValues)
    ZEND_ARG_INFO(0, where)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_save), 0, 0, 1)
    ZEND_ARG_INFO(0, datas)
    ZEND_ARG_INFO(0, bindValues)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_batch_save), 0, 0, 1)
    ZEND_ARG_INFO(0, datas)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_delete), 0, 0, 0)
    ZEND_ARG_INFO(0, where)
    ZEND_ARG_INFO(0, bindValues)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_get), 0, 0, 1)
    ZEND_ARG_INFO(0, argName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_set), 0, 0, 2)
    ZEND_ARG_INFO(0, argName)
    ZEND_ARG_INFO(0, argValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_as_array), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_get_executed_sql), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_model_get_afected_rows), 0, 0, 0)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{
 * Model::__construct()
 * in outer place the construct can't be replace in child class for the `final` keyword
 */
XAN_METHOD(Model, __construct)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return ;
    }

    if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
        XAN_INFO(E_ERROR, "Please set the Adapter object into the global container!");
    }

    /* Read table name from tableName function  */
    call_method_with_object_array(getThis(), "tablename", 0, NULL, return_value);
    if ( Z_TYPE_P(return_value) != IS_STRING || ( Z_TYPE_P(return_value) == IS_STRING && !Z_STRLEN_P(return_value)) ) {
        XAN_INFO(E_ERROR, "tableName() must return the valid full name of the table!");
    }

    /* Set the values */
    zend_update_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_TABLENAME), return_value);
    zend_update_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), &XAN_G(pdo_object));

    /* PDO_MODEL_DATA */
    zval *model_data = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
    array_init(model_data);

    /* Run the `init` method if exists */
    if ( XAN_CHECK_METHOD(getThis(), "init") != NULL ) {
        call_method_with_object_array(getThis(), "init", 0, NULL, return_value);
    }
}/*}}}*/

/**
 * {{{
 * proto Model::init()
 */
XAN_METHOD(Model, init)
{
}/*}}}*/

/**
 * {{{
 * Model::tableName()
 */
XAN_METHOD(Model, tableName)
{
}/*}}}*/

/**
 * {{{
 * proto Model::setAdapter($adapterDiKey)
 */
XAN_METHOD(Model, setAdapter)
{
    zend_string *key;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
        return ;
    }

    if ( !ZSTR_LEN(key) ) {
        RETURN_FALSE
    }

    if ( ZVAL_IS_NULL(&XAN_G(class_di)) ) {
        XAN_INFO(E_ERROR, "Please set adapter into Di first!");
    }

    zval *adapter = zend_hash_find( Z_ARRVAL(XAN_G(class_di)), key);
    if ( adapter == NULL ) {
        XAN_INFO(E_ERROR, "`%s` object not exists in Di container~", ZSTR_VAL(key));
    }

    zval *pdo_object = zend_read_property(XAN_ENTRY_OBJ(adapter), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object != NULL ) {
        zend_update_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), pdo_object);
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}/*}}}*/

/**
 * {{{
 * proto Model::fields($fields)
 */
XAN_METHOD(Model, fields)
{
    zval *fields;
    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &fields) == FAILURE ) {
        return ;
    }

    smart_str sql = { 0 };
    zval *pdo_sql = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_FIELDS), 1, NULL);
    if ( pdo_sql != NULL && !ZVAL_IS_NULL(pdo_sql) ) {
        smart_str_appends(&sql, Z_STRVAL_P(pdo_sql));
    }

    switch(Z_TYPE_P(fields)) {
        case IS_ARRAY:
            combine_array_fields(fields, &sql);
            break;
        case IS_STRING:
            smart_str_appends(&sql, Z_STRVAL_P(fields));
            break;
        default:
            XAN_INFO(E_ERROR, "Parameters only accept array or string value!");
            break;
    }
    if ( sql.s ) {
        smart_str_0(&sql);
        zend_update_property_str( XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_FIELDS), sql.s );
        smart_str_free(&sql);
    }
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * proto Model::where($where)
 * This function can be called many times to generate long where conditions
 * The function only generate the WHERE condition not contains the `WHERE` keyword
 */
XAN_METHOD(Model, where)
{
    zval *where;
    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &where) == FAILURE ) {
        return ;
    }

    smart_str where_str = { 0 };

    zend_update_property_null(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA));

    zval *pdo_sql_where = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), 1, NULL);
    if ( pdo_sql_where != NULL && !ZVAL_IS_NULL(pdo_sql_where) && Z_STRLEN_P(pdo_sql_where) ) {
        smart_str_appends(&where_str, Z_STRVAL_P(pdo_sql_where));
        smart_str_appends(&where_str, " AND ");
    }

    switch (Z_TYPE_P(where)) {
        case IS_ARRAY:
            combine_array_where(where, &where_str, NULL, 0, 0);
            break;
        case IS_STRING:
            smart_str_appends(&where_str, Z_STRVAL_P(where));
            break;
        default:
            break;
    }
    if ( where_str.s ) {
        smart_str_0(&where_str);
        zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), where_str.s );
        smart_str_free(&where_str);
    }
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * Model::groupBy($groupBy)
 * To combine the GROUP BY conditon, such as:
 * [ 'a.id' => 'DESC', 'max(a)' => 'ASC' ]
 */
XAN_METHOD(Model, groupBy)
{
    zval *group_by;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &group_by) == FAILURE) {
        return ;
    }    

    smart_str groupby = { 0 };

    switch(Z_TYPE_P(group_by)) {
        case IS_ARRAY:
            combine_array_group_orderby(group_by, &groupby, 1);
            break;
        case IS_STRING:
            smart_str_appends(&groupby, Z_STRVAL_P(group_by));
            break;
        default:
            XAN_INFO(E_ERROR, "Parameters only accept array or string value!");
            break;
    }
    if ( groupby.s ){
        smart_str_0(&groupby);
        zend_update_property_str( XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_GROUPBY), groupby.s );
        smart_str_free(&groupby);
    }
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * Model::orderBy($orderBy)
 * To combine the ORDER BY conditon, such as:
 * [ 'a.id' => 'DESC', 'max(a)' => 'ASC' ]
 */
XAN_METHOD(Model, orderBy)
{
    zval *order_by;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &order_by) == FAILURE) {
        return ;
    }    

    smart_str orderby = { 0 };

    switch(Z_TYPE_P(order_by)) {
        case IS_ARRAY:
            combine_array_group_orderby(order_by, &orderby, 0);
            break;
        case IS_STRING:
            smart_str_appends(&orderby, Z_STRVAL_P(order_by));
            break;
        default:
            XAN_INFO(E_ERROR, "Parameters only accept array or string value!");
            break;
    }
    if ( orderby.s ){
        smart_str_0(&orderby);
        zend_update_property_str( XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_ORDERBY), orderby.s );
        smart_str_free(&orderby);
    }
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * proto Model::__get($key)
 */
XAN_METHOD(Model, __get)
{
    zval member;
    zend_string *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
        return ;
    }

    zval *model_datas = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
    if ( model_datas ) {
        zval *value = STR_FIND_P(model_datas, key);
        if ( value ) {
            RETURN_ZVAL(value, 1, NULL);
        }
    }

    ZVAL_STR(&member, key);
    return_value = Z_OBJ_HT_P(getThis())->read_property(getThis(), &member, 1, NULL, return_value);
    zval_ptr_dtor(&member);
}/*}}}*/

/**
 * {{{
 * proto Model::__isset($key)
 */
XAN_METHOD(Model, __isset)
{
    zval member;
    zend_string *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
        return ;
    }

    zval *model_datas = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
    if ( model_datas ) {
        zval *value = STR_FIND_P(model_datas, key);
        if ( value ) {
            RETURN_TRUE
        }
    }

    ZVAL_STR(&member, key);
    return_value = Z_OBJ_HT_P(getThis())->read_property(getThis(), &member, 1, NULL, return_value);
    zval_ptr_dtor(&member);
    if (return_value) { 
        RETURN_TRUE; 
    } else { 
        RETURN_FALSE 
    }
}/*}}}*/

/**
 * {{{
 * proto Model::__set($key, $value)
 */
XAN_METHOD(Model, __set)
{
    int status = 0;
    zend_string *key;
    zval *value, *field, member, *model_fields;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &key, &value) == FAILURE ) {
        return ;
    }

again:
    model_fields = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_TABLEFIELDS), 1, NULL);
    if ( model_fields && !ZVAL_IS_NULL(model_fields) ) {
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(model_fields), field) {
            if ( Z_TYPE_P(field) == IS_ARRAY ) {
                zval *exist = STRING_FIND_P(field, "Field");
                if ( exist ) {
                    if (Z_TYPE_P(exist) == IS_STRING) {
                        if ( zend_string_equals(key, Z_STR_P(exist)) ) {
                            status = -1;
                            zval *model_datas = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
                            if (!model_datas || ZVAL_IS_NULL(model_datas)) {
                                array_init(model_datas);
                            }
                            if ( Z_TYPE_P(value) != IS_LONG || Z_TYPE_P(value) != IS_DOUBLE)
                                zval_add_ref(value);
                            add_assoc_zval(model_datas, ZSTR_VAL(key), value);
                        }
                    }
                }
            }
        } ZEND_HASH_FOREACH_END();
    } else {
        if ( status < 1 ) {
            /* To get the table fields into the object property */
            zval *pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
            xan_get_table_fields(getThis(), pdo_object, NULL );
            status++;
            goto again;
        }
    }

    /* If property not exists in the table fields array. write the property into the object */
    if ( status != -1 ) {
        ZVAL_STR(&member, key);
        Z_OBJ_HT_P(getThis())->write_property(getThis(), &member, value, NULL);
        zval_ptr_dtor(&member);
    }
}/*}}}*/

/**
 * {{{
 * proto Model::limit($limit)
 */
XAN_METHOD(Model, limit)
{
    zend_long limit, offset = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &limit, &offset) == FAILURE) {
        return ;
    }

    smart_str limit_str = { 0 };

    if ( limit ) {
        smart_str_append_long(&limit_str, limit);
    }

    if ( offset ) {
        smart_str_appends(&limit_str, " OFFSET ");
        smart_str_append_long(&limit_str, offset);
    }

    if ( !ZSTR_LEN(limit_str.s) ) {
        smart_str_free(&limit_str);
        RETURN_ZVAL(getThis(), 1, NULL);
    }

    if ( limit_str.s ){
        smart_str_0(&limit_str);
        zend_update_property_str( XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_LIMIT), limit_str.s );
        smart_str_free(&limit_str);
    }
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * proto Model::all()
 */
XAN_METHOD(Model, all)
{
    zval *bind_values = NULL, retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &bind_values) == FAILURE) {
        return ;
    }

    /* To chech whether the model_data exists or not */
    zval *model_data = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
    if ( model_data && !ZVAL_IS_NULL(model_data) && Z_TYPE_P(model_data) == IS_ARRAY && Z_H_N_E(Z_ARRVAL_P(model_data)) ) {
        smart_str where_str = { 0 };
        combine_array_where(model_data, &where_str, NULL, 0, 0);
        smart_str_0(&where_str);
        zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), where_str.s);
        smart_str_free(&where_str);
    }

    zend_string *select_sql = combine_all_sql(getThis(), SELECT);

    zval *pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) RETURN_NULL()
    xan_get_table_fields(getThis(), pdo_object, NULL);
    if (EG(exception)) {
        RETURN_NULL()
    }

    zval *as_array = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_ARRAY), 1, NULL);

    xan_pdo_prepare( pdo_object, ZSTR_VAL(select_sql), return_value);
    xan_check_pdo_error(pdo_object, NULL);
    if (EG(exception)) {
        RETURN_NULL()
    }
    xan_pdo_execute(return_value, bind_values, &retval);
    xan_check_pdo_error(NULL, return_value);
    if (EG(exception)) {
        RETURN_NULL()
    }
    xan_pdo_fetch_all(return_value, &retval);

    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_EXECUTE_SQL), select_sql);

    if ( as_array && Z_LVAL_P(as_array) ) {
        if ( Z_TYPE(retval) != IS_ARRAY ) {
            array_init(return_value);
        } else {
            ZVAL_COPY(return_value, &retval);
        }
    } else {
        /* Array of the return value */
        array_init(return_value);
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(retval), bind_values) {
            
            zval temp_obj;
            object_init(&temp_obj);
            // zend_object *obj = zend_objects_clone_obj(getThis());
            // ZVAL_OBJ(&temp_obj, obj);
            // zval_add_ref(bind_values);
            zend_merge_properties(&temp_obj, Z_ARRVAL_P(bind_values));
            add_next_index_zval(return_value, &temp_obj);
        } ZEND_HASH_FOREACH_END();
    }
}/*}}}*/

/**
 * {{{
 * proto Model::one([,$bindValue=[]])
 */
XAN_METHOD(Model, one)
{
    int in = 0;
    zend_string *key;
    zval *bind_values = NULL, retval, *val, *value, member;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &bind_values) == FAILURE) {
        return ;
    }
    
    /* To chech whether the model_data exists or not */
    zval *model_data = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
    if ( model_data && !ZVAL_IS_NULL(model_data) && Z_TYPE_P(model_data) == IS_ARRAY && Z_H_N_E(Z_ARRVAL_P(model_data)) ) {
        smart_str where_str = { 0 };
        combine_array_where(model_data, &where_str, NULL, 0, 0);
        smart_str_0(&where_str);
        zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), where_str.s);
        smart_str_free(&where_str);
    }

    zend_string *select_sql = combine_all_sql(getThis(), SELECT);

    zval *pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) RETURN_NULL()

    xan_get_table_fields(getThis(), pdo_object, NULL);
    if (EG(exception)) {
        RETURN_NULL()
    }

    zval *as_array = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_ARRAY), 1, NULL);

    xan_pdo_prepare( pdo_object, ZSTR_VAL(select_sql), return_value);
    xan_check_pdo_error(pdo_object, NULL);
    if (EG(exception)) {
        RETURN_NULL()
    }
    xan_pdo_execute(return_value, bind_values, &retval);
    xan_check_pdo_error(NULL, return_value);
    if (EG(exception)) {
        RETURN_NULL()
    }
    xan_pdo_fetch(return_value, &retval);
    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_EXECUTE_SQL), select_sql);
    if ( as_array && Z_LVAL_P(as_array) ) {
        if ( Z_TYPE(retval) != IS_ARRAY ) {
            array_init(return_value);
        } else {
            ZVAL_COPY(return_value, &retval);
        }
    } else {
        if ( Z_TYPE(retval) == IS_ARRAY ) {
            zend_merge_properties(getThis(), Z_ARRVAL(retval));
        }
        ZVAL_COPY(return_value, getThis());
    }
}/*}}}*/

/**
 * {{{
 * proto Model::update($where, $data[, $bindValues])
 * UPDATE `pms_provider` SET `id` = 1, `grade` = 'E' WHERE `pid` = 10
 */
XAN_METHOD(Model, update)
{
    zval *where, *data, *bind_values = NULL, retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|az", &data, &bind_values, &where) == FAILURE ) {
        return ;
    }

    smart_str set_str = { 0 };
    combine_array_key_equal_value(data, &set_str);
    smart_str_0(&set_str);
    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_UPD_SET), set_str.s);
    smart_str_free(&set_str);

    smart_str t_str = { 0 };
    zval *where_str = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), 1, NULL);
    if ( where_str && !ZVAL_IS_NULL(where_str) && Z_TYPE_P(where_str) == IS_STRING && Z_STRLEN_P(where_str) > 0 ) {
        smart_str_appends(&t_str, Z_STRVAL_P(where_str));
        smart_str_appends(&t_str, " AND ");
    }

    if ( where ) {
        if ( Z_TYPE_P(where) == IS_ARRAY ) {
            combine_array_where(where, &t_str, NULL, 0, 0);
        } else if ( Z_TYPE_P(where) == IS_STRING ) {
            smart_str_appends(&t_str, Z_STRVAL_P(where));
        } else {
            XAN_INFO(E_ERROR, "Parameters $where must be array or string!");
            return ;
        }
    }

    smart_str_0(&t_str);
    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), t_str.s);
    smart_str_free(&t_str);

    zend_string *update_sql = combine_all_sql(getThis(), UPDATE);

    xan_execute_sql(getThis(), update_sql, bind_values, return_value, &retval);
}/*}}}*/

/**
 * {{{
 * proto Model::save($data, $bindValues)
 * INSERT INTO `pms_provider` (`id`, `grade`) VALUES( 1, 'E' )
 * The models' MODEL_DATA will be add to the SQL's fields & values colum
 */
XAN_METHOD(Model, save)
{
    zval *datas = NULL, *bind_values = NULL, retval;
    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|aa", &datas, &bind_values) == FAILURE ) {
        return ;
    }

    smart_str fields = { 0 }, values = { 0 };

    zval *model_data = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_DATA), 1, NULL);
    if ( model_data && !ZVAL_IS_NULL(model_data) && Z_TYPE_P(model_data) == IS_ARRAY && Z_H_N_E(Z_ARRVAL_P(model_data)) ) {
        if ( datas ) {
            zend_hash_merge(Z_ARRVAL_P(model_data), Z_ARRVAL_P(datas), (copy_ctor_func_t) zval_add_ref, 1);
        }
        combine_array_insert_data(model_data, &fields, &values);
    } else {
        if ( datas ) {
            combine_array_insert_data(datas, &fields, &values);
        }
    }

    smart_str_0(&fields);
    smart_str_0(&values);
    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_INS_FIELD), fields.s);
    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_INS_DATA), values.s);
    smart_str_free(&fields);
    smart_str_free(&values);

    zend_string *insert_sql = combine_all_sql(getThis(), INSERT);
    xan_execute_sql(getThis(), insert_sql, bind_values, return_value, &retval);
}/*}}}*/

/**
 * {{{
 * proto Model::delete($where, $bindValues = [])
 * DELETE * FROM `pms_provider` WHERE `id` = 1
 */
XAN_METHOD(Model, delete)
{
    zval *where = NULL, *bind_values = NULL, retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|za", &where, &bind_values) == FAILURE ) {
        return ;
    }

    smart_str t_str = { 0 };
    zval *where_str = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), 1, NULL);
    if ( where_str && !ZVAL_IS_NULL(where_str) && Z_TYPE_P(where_str) == IS_STRING && Z_STRLEN_P(where_str) > 0 ) {
        smart_str_appends(&t_str, Z_STRVAL_P(where_str));
        smart_str_appends(&t_str, " AND ");
    }

    if ( where ) {
        if ( Z_TYPE_P(where) == IS_ARRAY ) {
            combine_array_where(where, &t_str, NULL, 0, 0);
        } else if ( Z_TYPE_P(where) == IS_STRING ) {
            smart_str_appends(&t_str, Z_STRVAL_P(where));
        } else {
            XAN_INFO(E_ERROR, "Parameters $where must be array or string!");
            return ;
        }
    }

    smart_str_0(&t_str);
    zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_WHERE), t_str.s);
    smart_str_free(&t_str);

    zend_string *delete_sql = combine_all_sql(getThis(), DELETE);

    xan_execute_sql(getThis(), delete_sql, bind_values, return_value, &retval);
}/*}}}*/

/**
 * {{{
 * proto Model::having($having)
 * This function can be called many times to generate long having conditions
 * The function only generate the HAVING condition not contains the `HAVING` keyword
 */
XAN_METHOD(Model, having)
{
    zval *having;
    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &having) == FAILURE ) {
        return ;
    }

    smart_str having_str = { 0 };

    zval *pdo_sql_having = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_HAVING), 1, NULL);
    if ( pdo_sql_having != NULL && !ZVAL_IS_NULL(pdo_sql_having) && Z_STRLEN_P(pdo_sql_having) ) {
        smart_str_appends(&having_str, Z_STRVAL_P(pdo_sql_having));
        smart_str_appends(&having_str, " AND ");
    }

    switch (Z_TYPE_P(having)) {
        case IS_ARRAY:
            combine_array_where(having, &having_str, NULL, 0, 0);
            break;
        case IS_STRING:
            smart_str_appends(&having_str, Z_STRVAL_P(having));
            break;
        default:
            break;
    }
    if ( having_str.s ){
        smart_str_0(&having_str);
        zend_update_property_str(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_SQL_HAVING), having_str.s);
        smart_str_free(&having_str);
    }
    ZVAL_COPY(return_value, getThis());

}/*}}}*/

/**
 * {{{
 * proto Model::asArray()
 */
XAN_METHOD(Model, asArray)
{
    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }
    zend_update_property_long(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_MODEL_ARRAY), 1);
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * proto Model::getExecutedSql()
 */
XAN_METHOD(Model, getExecutedSql)
{
    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }
    zval *value = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_EXECUTE_SQL), 1, NULL);
    ZVAL_COPY(return_value, value);
}/*}}}*/

/**
 * {{{
 * proto Model::getAffectedRows()
 */
XAN_METHOD(Model, getAffectedRows)
{
    if (zend_parse_parameters_none() == FAILURE ) {
        return ;
    }
    zval *rows_count = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_ROWS_COUNT), 1, NULL);
    ZVAL_COPY(return_value, rows_count);
    zend_update_property_long(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_ROWS_COUNT), 0);
}/*}}}*/

/**
 * {{{ proto 
 * Model::batchSave($datas = [])
 * INSERT INTO `pms_provider` (`a`, `b`, `d`) VALUES ('aa', 'bb', 'dd'), ('aaa', 'bbb', 'ddd')
 */
XAN_METHOD(Model, batchSave)
{

}/*}}}*/

/**
 * {{{
 * model_functions
 */
XAN_FUNCTIONS(model)
    XAN_AB_ME(Model, tableName, arginfo_xan_model_table_name)
    XAN_ME(Model, __construct, arginfo_xan_model_construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, setAdapter, arginfo_xan_model_set_adapter, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, init, arginfo_xan_model_init, ZEND_ACC_PUBLIC )
    XAN_ME(Model, fields, arginfo_xan_model_fields, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, where, arginfo_xan_model_where, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, groupBy, arginfo_xan_model_group_by, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, having, arginfo_xan_model_having, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, orderBy, arginfo_xan_model_order_by, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, __set, arginfo_xan_model_set, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, __get, arginfo_xan_model_get, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, __isset, arginfo_xan_model_isset, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, getExecutedSql, arginfo_xan_model_get_executed_sql, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, limit, arginfo_xan_model_limit, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, asArray, arginfo_xan_model_as_array, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    XAN_ME(Model, all, arginfo_xan_model_all, ZEND_ACC_PUBLIC )
    XAN_ME(Model, one, arginfo_xan_model_one, ZEND_ACC_PUBLIC )
    XAN_ME(Model, save, arginfo_xan_model_save, ZEND_ACC_PUBLIC )
    XAN_ME(Model, update, arginfo_xan_model_update, ZEND_ACC_PUBLIC )
    XAN_ME(Model, delete, arginfo_xan_model_delete, ZEND_ACC_PUBLIC )
    XAN_ME(Model, getAffectedRows, arginfo_xan_model_get_afected_rows, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)

XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{
 * model_init()
 */
XAN_INIT(model)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan\\Db", "Model", model_functions);
    xan_model_ce = zend_register_internal_class(&ce);

    zend_do_inheritance(xan_model_ce, xan_adapter_ce);
    xan_model_ce->ce_flags |= ZEND_ACC_ABSTRACT;

    XAN_PR_ESTRING(xan_model_ce, PDO_TABLENAME, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL_FIELDS, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL_WHERE, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL_GROUPBY, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL_ORDERBY, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL_HAVING, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_EXECUTE_SQL, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_SQL_LIMIT, ZEND_ACC_PROTECTED);
       /* XAN_PR_LONG(xan_model_ce, PDO_ROWS_COUNT, 0, ZEND_ACC_PROTECTED); */
       XAN_PR_LONG(xan_model_ce, PDO_MODEL_ARRAY, 0, ZEND_ACC_PROTECTED);
       XAN_PR_NULL(xan_model_ce, PDO_BIND_VALUE, ZEND_ACC_PROTECTED);
       XAN_PR_NULL(xan_model_ce, PDO_TABLEFIELDS, ZEND_ACC_PROTECTED);
       XAN_PR_NULL(xan_model_ce, PDO_MODEL_DATA,  ZEND_ACC_PROTECTED);
       XAN_PR_NULL(xan_model_ce, PDO_FIELDS_ARR,  ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_PRI_FIELD, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_INS_FIELD, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_INS_DATA, ZEND_ACC_PROTECTED);
    XAN_PR_ESTRING(xan_model_ce, PDO_UPD_SET, ZEND_ACC_PROTECTED);
}/*}}}*/

/**
 * {{{
 * To execute the SQL with the gien SQL
 */
void xan_execute_sql(zval *model_object, zend_string *sql, zval *bind_values, zval *ret_val, zval *retval)
{
    zval rows, *pdo_object;
    
    /* Get the PDO object */
    pdo_object = zend_read_property(XAN_ENTRY_OBJ(model_object), XAN_STRL(PDO_OBJECT), 1, NULL);
    
    /* Do the prepare job. */
    xan_pdo_prepare(pdo_object, ZSTR_VAL(sql), ret_val);

    /* Check wheather the prepare occured the error. */
    if (!xan_check_pdo_error(pdo_object, NULL)) return ;

    /* Execute the prepared SQL with the given binding values */
    xan_pdo_execute(ret_val, bind_values, retval);

    /* Check wheather the error info occured when executing */
    if (!xan_check_pdo_error(pdo_object, ret_val)) return ;

    /* Save the affected rows */
    xan_pdo_row_count(ret_val, &rows);
    zend_update_property(XAN_ENTRY_OBJ(model_object), XAN_STRL(PDO_ROWS_COUNT), &rows);
    
    /* Return the executing result into the `retval` variable. release the temp variable */
    ZVAL_COPY(ret_val, retval);
    zval_ptr_dtor(&rows);
}/*}}}*/

/**
 * Get the table's fields & set the fields into the object
 */
void xan_get_table_fields(zval *object, zval *pdo_object, zval *table_name)
{
    zend_string *key;
    zval pobject, pdo_state_obj, retval, *each_field, *value, t_name;

    if ( table_name == NULL ) {
        value = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_TABLENAME), 1, NULL);
        ZVAL_COPY(&t_name, value);
    } else {
        ZVAL_COPY(&t_name, table_name);
    }

    zend_string *colums = strpprintf(0, "SHOW COLUMNS FROM %s", Z_STRVAL(t_name));

    if ( pdo_object == NULL ) {
        ZVAL_COPY(&pobject, &XAN_G(pdo_object));
    } else {
        ZVAL_COPY(&pobject, pdo_object);
    }

    zval *table_fields = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_FIELDS_ARR), 1, NULL);
    if (ZVAL_IS_NULL(table_fields)) {
        array_init(table_fields);
    }

    xan_pdo_prepare(&pobject, ZSTR_VAL(colums), &pdo_state_obj);
    xan_check_pdo_error(&pobject, NULL);
    if (EG(exception)) {
        return ;
    }
    xan_pdo_execute(&pdo_state_obj, NULL, &retval );
    xan_check_pdo_error(NULL, &pdo_state_obj);
    if (EG(exception)) {
        return ;
    }
    xan_pdo_fetch_all(&pdo_state_obj, &retval);

    ZEND_HASH_FOREACH_VAL(Z_ARRVAL(retval), each_field) {
        if ( Z_TYPE_P(each_field) != IS_ARRAY ) continue;
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(each_field), key, value ) {
            zval *fields = zend_hash_str_find(Z_ARRVAL_P(each_field), XAN_STRL("Field"));
            if ( key && zend_string_equals_literal(key, "Key") ) {
                if ( zend_string_equals_literal(Z_STR_P(value), "PRI") ) {
                    zend_update_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_PRI_FIELD), fields);
                    break;
                }
            }
        } ZEND_HASH_FOREACH_END();

    } ZEND_HASH_FOREACH_END();

    /* To keep the fields info. */
    zend_update_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_TABLEFIELDS), &retval);

    zval_ptr_dtor(&pobject);
    zval_ptr_dtor(&pdo_state_obj);
    zval_ptr_dtor(&retval);
}/*}}}*/

/**
 * {{{
 * To compine the array fields into the fields string & values string
 * It combine the INSERT INTO (a, b, c) VALUES (aa, bb, cc)
 */
void combine_array_insert_data(zval *data, smart_str *fields, smart_str *values)
{
    zval *value;
    zend_string *key;
    int i = 0, num, quote = 0;

    if ( Z_TYPE_P(data) == IS_ARRAY ) {

        num = Z_H_N_E(Z_ARRVAL_P(data));
        num = num ? num - 1 : num;

        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(data), key, value) {

            if ( !ZSTR_LEN(key) ) {
                continue;
            }

            smart_str_appendc(fields, '`');
            smart_str_appends(fields, ZSTR_VAL(key));
            smart_str_appendc(fields, '`');

            switch ( Z_TYPE_P(value) ) {
                case IS_LONG:
                    smart_str_append_long(values, Z_LVAL_P(value));
                    break;
                case IS_DOUBLE:
                case IS_OBJECT:
                    convert_to_string(value);
                case IS_STRING:
                    if ( Z_STRVAL_P(value)[0] == ':' || Z_STRVAL_P(value)[0] == '?' ) {
                        quote = 0;
                    } else {
                        quote = 1;
                    }
                    if ( quote )
                        smart_str_appendc(values, '\'');
                    smart_str_append(values, php_addslashes(Z_STR_P(value), 1) );
                    if ( quote )
                        smart_str_appendc(values, '\'');
                    break;
            } 

            if ( i++ < num ) {
                smart_str_appendc(fields, ',');
                smart_str_appendc(values, ',');
            }
        } ZEND_HASH_FOREACH_END();
    }
}/*}}}*/

/**
 * To combine the array fields
 * such as [ 'a', 'id', 'address' ]
 * or [ 'a' => 'aa', 'id' => 'sid' ] to use the rename feature
 */
void combine_array_fields(zval *fields, smart_str *str)
{
    int k = 0, key = 0, quote = 0;

    Bucket *bucket;

    if ( Z_TYPE_P(fields) == IS_ARRAY ) {

        key = Z_H_N_E(Z_ARRVAL_P(fields));
        key = key ? key - 1 : key;

        ZEND_HASH_FOREACH_BUCKET( Z_ARRVAL_P(fields), bucket ) {

            if ( Z_TYPE(bucket->val) == IS_STRING || Z_TYPE(bucket->val) == IS_LONG || Z_TYPE(bucket->val) == IS_DOUBLE ) {
                
                if ( !bucket->key ) {
                    switch (Z_TYPE(bucket->val)) {
                        case IS_LONG:
                            convert_to_string(&bucket->val);
                        case IS_STRING:
                            if ( (strchr(Z_STRVAL(bucket->val), '(') != NULL) 
                                || (strchr(Z_STRVAL(bucket->val), '.') != NULL) 
                            ) {
                                quote = 1;
                            }  else {
                                quote = 0;
                            }
                            
                            if ( !quote ) {
                                smart_str_appendc(str, '`');
                            }
                            smart_str_appends(str, Z_STRVAL(bucket->val));
                            if ( !quote ) {
                                smart_str_appendc(str, '`');
                            }
                            break;
                        case IS_DOUBLE:
                            smart_str_appendc(str, '`');
                            smart_str_appends(str, ZSTR_VAL(strpprintf(0, "%f", Z_DVAL(bucket->val))));
                            smart_str_appendc(str, '`');
                            break;
                    }
                } else {
                    if ( (strchr(ZSTR_VAL(bucket->key), '(') != NULL) 
                        || (strchr(ZSTR_VAL(bucket->key), ',') != NULL) 
                    ) {
                        quote = 1;
                    } else {
                        quote = 0;
                    }
                    if ( !quote ) {
                        smart_str_appendc(str, '`');
                    }
                    smart_str_appends(str, ZSTR_VAL(bucket->key));
                    if ( !quote ) {
                        smart_str_appendc(str, '`');
                    }
                    smart_str_appends(str, " AS ");
                    switch (Z_TYPE(bucket->val)) {
                        case IS_STRING:
                            smart_str_appends(str, Z_STRVAL(bucket->val));
                            break;
                        case IS_LONG:
                            smart_str_append_long(str, Z_LVAL(bucket->val));
                            break;
                        case IS_DOUBLE:
                            smart_str_appends(str, ZSTR_VAL(strpprintf(0, "%f", Z_DVAL(bucket->val))));
                            break;
                    }
                }
            }

            if ( k++ < key ) {
                smart_str_appendc(str, ',');
            }

        } ZEND_HASH_FOREACH_END();
    }
}/*}}}*/

/**
 * {{{
 * To combine the array data
 * UPDATE SET xx = xx, xx = xx
 */
void combine_array_key_equal_value(zval *data, smart_str *str)
{
    zval *value;
    zend_string *key;
    int i = 0, num, quote = 0;

    if (Z_TYPE_P(data) == IS_ARRAY) {

        num = Z_H_N_E(Z_ARRVAL_P(data));
        num = num ? num - 1 : num;

        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(data), key, value) {

            if ( !ZSTR_LEN(key) ) continue;

            if ( strchr(ZSTR_VAL(key), '.') != NULL ||
                 strchr(ZSTR_VAL(key), '(') != NULL
            ) {
                quote = 0;
            } else {
                quote = 1;
            }

            if ( quote ) smart_str_appendc(str, '`');
            smart_str_appends(str, ZSTR_VAL(key));
            if ( quote ) smart_str_appendc(str, '`');
            smart_str_appends(str, " = ");

            switch (Z_TYPE_P(value)) {
                case IS_LONG:
                    smart_str_append_long(str, Z_LVAL_P(value));
                    break;
                case IS_DOUBLE:
                case IS_OBJECT:
                    convert_to_string(value);
                case IS_STRING:
                    if ( Z_STRVAL_P(value)[0] == ':'
                    ) {
                        quote = 0;
                    } else {
                        quote = 1;
                    }

                    if ( quote )
                        smart_str_appendc(str, '\'');
                    smart_str_appends(str, ZSTR_VAL(php_addslashes(Z_STR_P(value), 1)) );
                    if ( quote )
                        smart_str_appendc(str, '\'');
                    break;
            }

            if ( i++ < num ) {
                smart_str_appends(str, ", ");
            }
        } ZEND_HASH_FOREACH_END();
    }
}/*}}}*/

/**
 * To combine the array fields
 * such as [ 'a' => '11', 'b' => 2, 'a - 1' => '3', 'a' => [ '>' => '2' ] ]
 */
void combine_array_where(zval *where, smart_str *str, zend_string *ikey, int inner, int quote)
{
    int i = 0, key = 0, kquote = quote;

    Bucket *bucket;
    zval logic_word;

    if ( Z_TYPE_P(where) == IS_ARRAY ) {

        zval *loword = STRING_FIND_P(where, "__xor");
        if ( loword == NULL ) {
            ZVAL_STRING(&logic_word, "AND");
        } else {
            ZVAL_COPY(&logic_word, loword);
            zend_hash_str_del(Z_ARRVAL_P(where), XAN_STRL("__xor"));
        }

        key = Z_H_N_E(Z_ARRVAL_P(where));
        key = key ? key - 1 : key;

        ZEND_HASH_FOREACH_BUCKET(Z_ARRVAL_P(where), bucket) {

            if ( !bucket->key || !ZSTR_LEN(bucket->key) ) {
                continue;
            }

            if (!inner) {
                if ( strchr(ZSTR_VAL(bucket->key), '(') != NULL && strchr(ZSTR_VAL(bucket->key), '.') != NULL) {
                    kquote = 1;
                } else {
                    kquote = 0;
                }
            }
            if ( Z_TYPE(bucket->val) == IS_ARRAY ) {
                smart_str_appendc(str, '(');
            }

            if ( !kquote && !inner ) {
                smart_str_appendc(str, '`');
            }
            
            if ( inner && i > 0 ) {
                if (!kquote) {
                    smart_str_appendc(str, '`');
                }
                smart_str_appends(str, ZSTR_VAL(ikey));
                if (!kquote) {
                    smart_str_appendc(str, '`');
                }
            }
            smart_str_appends(str, ZSTR_VAL(bucket->key));
            
            if ( !kquote && !inner  ) {
                smart_str_appendc(str, '`');
            }
            
            if ( Z_TYPE(bucket->val) != IS_ARRAY ) {
                
                if ( !inner ) {
                    smart_str_appends(str, " = ");
                }

                switch (Z_TYPE(bucket->val)) {
                    case IS_STRING:
                        if ( Z_STRVAL(bucket->val)[0] != ':' && Z_STRVAL(bucket->val)[0] != '?' ) {
                            smart_str_appendc(str, '\'');
                        }
                        smart_str_appends(str, ZSTR_VAL(php_addslashes(Z_STR(bucket->val), 1)) );
                        if ( Z_STRVAL(bucket->val)[0] != ':' && Z_STRVAL(bucket->val)[0] != '?' ) {
                            smart_str_appendc(str, '\'');
                        }
                        break;
                    case IS_LONG:
                        smart_str_append_long(str, Z_LVAL(bucket->val));
                        break;
                    case IS_DOUBLE:
                        smart_str_appends(str, ZSTR_VAL(strpprintf(0, "%f", Z_DVAL(bucket->val))));
                        break;
                    default:
                        break;
                }

                if ( i < key ) {
                    /*smart_str_appends(str, " AND ");*/
                    smart_str_appendc(str, ' ');
                    smart_str_appends(str, Z_STRVAL(logic_word));
                    smart_str_appendc(str, ' ');
                } else {
                    if ( !inner ) {
                        smart_str_appends(str, " ");
                    }
                }

            } else {
                combine_array_where(&bucket->val, str, bucket->key, 1, kquote);
                smart_str_appends(str, ")");
                if ( i < key ) {
                    /* smart_str_appends(str, " AND "); */
                    smart_str_appendc(str, ' ');
                    smart_str_appends(str, Z_STRVAL(logic_word));
                    smart_str_appendc(str, ' ');
                }
            }

            i++;

        } ZEND_HASH_FOREACH_END();

        if ( str->s && ZSTR_LEN(str->s) > 1 ) {
            if ( ZSTR_VAL(str->s)[ZSTR_LEN(str->s) - 1] == ' ' ) {
                ZSTR_VAL(str->s)[ZSTR_LEN(str->s) - 1] = '\0';
            }
        }
    }

    zval_ptr_dtor(&logic_word);
}/*}}}*/

/**
 * {{{
 * To combine the by condition
 * such as :
 * 
 * [ 'a.id' => 'ASC', 'max(a)' => 'WITH ROLLUP' ]
 */
void combine_array_group_orderby(zval *by_condition, smart_str *str, int groupby)
{
    Bucket *bucket;

    if ( Z_TYPE_P(by_condition) == IS_ARRAY ) {

        ZEND_HASH_FOREACH_BUCKET(Z_ARRVAL_P(by_condition), bucket) {

            if ( !bucket->key ) {
                if ( !groupby ) {
                    smart_str_appends(str, Z_STRVAL(bucket->val));
                    smart_str_appendc(str, ' ');
                    smart_str_appends(str, "DESC");
                    smart_str_appendc(str, ',');
                } else if ( groupby ) {
                    smart_str_appends(str, Z_STRVAL(bucket->val));
                    smart_str_appendc(str, ',');
                }
                continue;
            }

            if ( Z_TYPE(bucket->val) != IS_STRING || ( Z_TYPE(bucket->val) == IS_STRING &&  
                !zend_string_equals_literal( ZS_LOWER(Z_STR(bucket->val)), "asc") &&
                !zend_string_equals_literal( ZS_LOWER(Z_STR(bucket->val)), "desc") && 
                !zend_string_equals_literal( ZS_LOWER(Z_STR(bucket->val)), "with rollup")
             ) ) {
                continue;
            }

            if ( !groupby && ( zend_string_equals_literal( ZS_LOWER(Z_STR(bucket->val)), "with rollup") ) ) {
                continue;
            } 

            smart_str_appends(str, ZSTR_VAL(bucket->key));
            smart_str_appendc(str, ' ');
            smart_str_appends(str, ZSTR_VAL(php_string_toupper(Z_STR(bucket->val))));
            smart_str_appendc(str, ',');

        } ZEND_HASH_FOREACH_END();

        if ( str->s && ZSTR_LEN(str->s) ) {
            if ( ZSTR_VAL(str->s)[ZSTR_LEN(str->s) - 1] == ',' ) {
                ZSTR_VAL(str->s)[ZSTR_LEN(str->s) - 1] = '\0';
            }
        }
    }
}/*}}}*/

/**
 * To comtine the SQL and return
 * NOTE THAT:
 *  In current condition, Xan's model only support `INSERT` | `DELETE` | `UPDATE` | `SELECT`
 */
zend_string *combine_all_sql(zval *object, int type)
{
    zend_string *result_sql;

    zval *fields     = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_FIELDS), 1, NULL);
    zval *group_by   = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_GROUPBY), 1, NULL);
    zval *having     = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_HAVING), 1, NULL);
    zval *order_by   = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_ORDERBY), 1, NULL);
    zval *limit      = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_LIMIT), 1, NULL);
    
    zval *table_name = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_TABLENAME), 1, NULL);
    zval *where      = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_WHERE), 1, NULL);

    zval *ins_fields = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_INS_FIELD), 1, NULL);
    zval *ins_data   = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_INS_DATA), 1, NULL);
    zval *upd_data   = zend_read_property(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_UPD_SET), 1, NULL);

    switch (type) {
        case SELECT:
            result_sql = strpprintf(0, 
                "SELECT %s FROM %s%s%s%s%s%s%s%s%s%s%s",
                !Z_STRLEN_P(fields) ? "*" : Z_STRVAL_P(fields),
                Z_STRVAL_P(table_name),
                !Z_STRLEN_P(where) ? "" : " WHERE ",
                !Z_STRLEN_P(where) ? "" : Z_STRVAL_P(where),
                !Z_STRLEN_P(group_by) ? "" : " GROUP BY ",
                !Z_STRLEN_P(group_by) ? "" : Z_STRVAL_P(group_by),
                !Z_STRLEN_P(having) ? "" : " HAVING ",
                !Z_STRLEN_P(having) ? "" : Z_STRVAL_P(having),
                !Z_STRLEN_P(order_by) ? "" : " ORDER BY ",
                !Z_STRLEN_P(order_by) ? "" : Z_STRVAL_P(order_by),
                !Z_STRLEN_P(limit) ? "" : " LIMIT ",
                !Z_STRLEN_P(limit) ? "" : Z_STRVAL_P(limit)
            ); 
            break;
        case INSERT:
            result_sql = strpprintf(0, 
                "INSERT INTO `%s` (%s) VALUES(%s)",
                Z_STRVAL_P(table_name),
                Z_STRVAL_P(ins_fields),
                Z_STRVAL_P(ins_data)
            );
            break;
        case UPDATE:
            result_sql = strpprintf(0,
                "UPDATE `%s` SET %s%s%s",
                Z_STRVAL_P(table_name),
                Z_STRVAL_P(upd_data),
                !Z_STRLEN_P(where) ? "" : " WHERE ",
                !Z_STRLEN_P(where) ? "" : Z_STRVAL_P(where)
            );
            break;
        case DELETE:
            result_sql = strpprintf(0,
                "DELETE FROM `%s`%s%s",
                Z_STRVAL_P(table_name),
                !Z_STRLEN_P(where) ? "" : " WHERE ",
                !Z_STRLEN_P(where) ? "" : Z_STRVAL_P(where)
            );
            break;
    }

    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_FIELDS), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_WHERE), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_GROUPBY), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_HAVING), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_ORDERBY), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_SQL_LIMIT), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_INS_FIELD), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_INS_DATA), "");
    zend_update_property_string(XAN_ENTRY_OBJ(object), XAN_STRL(PDO_UPD_SET), "");

    return result_sql;
}/*}}}*/

/**
 * {{{
 * The following two function was used to call the function with the given parameters
 */
void call_method_with_object_array(zval *object, char *method_name, uint32_t param_counts, zval params[], zval *ret_val)
{
    if (XAN_CHECK_METHOD(object, method_name) != NULL) {
        zval function_name;
        ZVAL_STRING(&function_name, method_name);
        zend_try {
            call_user_function( NULL, object, &function_name, ret_val, param_counts, params );
        } zend_catch {
            if (EG(exception)) zend_exception_error(EG(exception), E_ERROR);
        } zend_end_try();
        zval_ptr_dtor(&function_name);
    }
}/*}}}*/

/**
 * {{{
 * call_method_with_object_zval
 * was used to call the function with some params which passed by php-array, not C array;
 */
void call_method_with_object_zval( zval *object, char *method_name, zval *parameters TSRMLS_CC, zval *ret_val )
{
    uint32_t p_counts = !parameters ? 0 : zend_hash_num_elements(Z_ARRVAL_P(parameters));
    if ( p_counts )  {
        uint32_t n = 0;
        zval *val_para;

        zval *params = (zval *)emalloc(sizeof(zval) * p_counts );
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(parameters), val_para) {
            ZVAL_COPY(&params[n], val_para);
            n++;
        } ZEND_HASH_FOREACH_END();
        
        call_method_with_object_array( object, method_name, p_counts, params, ret_val );

        efree(params);
    } else {
        call_method_with_object_array(object, method_name, 0, NULL, ret_val);
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


