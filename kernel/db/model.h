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
#ifndef XAN_MODEL_H
#define XAN_MODEL_H

#define PDO_TABLENAME   "_table_name"
#define PDO_SQL         "_pdo_sql"
#define PDO_SQL_FIELDS  "_pdo_fields"
#define PDO_SQL_WHERE   "_pdo_where"
#define PDO_SQL_GROUPBY "_pdo_groupby"
#define PDO_SQL_HAVING  "_pdo_having"
#define PDO_SQL_ORDERBY "_pdo_orderby"
#define PDO_SQL_LIMIT   "_pdo_limit"
#define PDO_BIND_VALUE  "_pdo_bind_data"
#define PDO_TABLEFIELDS "_pdo_table_fields"
#define PDO_FIELDS_ARR  "_pdo_fields_arr"
#define PDO_MODEL_ARRAY "_pdo_as_array"
#define PDO_EXECUTE_SQL "_pdo_execute_sql"
#define PDO_PRI_FIELD   "_pdo_primary_key"
#define PDO_MODEL_DATA  "_pdo_model_data"
#define PDO_INS_FIELD   "_pdo_insert_fields"
#define PDO_INS_DATA    "_pdo_insert_data"
#define PDO_UPD_SET     "_pdo_update_data"
/* #define PDO_ROWS_COUNT  "_pdo_rows_count" */

/**
 * ALL ActiveRecord TYPE
 */
enum {
    SELECT,
    INSERT,
    UPDATE,
    DELETE
};

void call_method_with_object_zval( zval *object, char *method_name, zval *parameters, zval *ret_val );
void call_method_with_object_array(zval *object, char *method_name, uint32_t param_counts, zval params[], zval *ret_val);

/* Exeucte the SQL */
void xan_execute_sql(zval *model_object, zend_string *sql, zval *bind_values, zval *ret_val, zval *retval);

/* Get the table's fields */
void xan_get_table_fields(zval *object, zval *pdo_object, zval *table_name);

/* Combine array fields */
void combine_array_fields(zval *fields, smart_str *str);

/* Combine array where condition */
void combine_array_where(zval *where, smart_str *str, zend_string *ikey, int inner, int quote);

/* Combine array group|order by condition */
void combine_array_group_orderby(zval *by_condition, smart_str *str, int groupby);

/* combine the array data into fields & values */
void combine_array_insert_data(zval *data, smart_str *fields, smart_str *values);

/* To combine the UPDATE SET */
void combine_array_key_equal_value(zval *data, smart_str *str);

/* Combine the SQL */
zend_string *combine_all_sql(zval *object, int type);

extern zend_class_entry *xan_model_ce;

#endif /* XAN_MODEL_H */




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
