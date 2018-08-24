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
#include "main/SAPI.h"
#include "php_xannotation.h"
#include "kernel/xan_class.h"
#include "kernel/db/adapter.h"
#include "ext/json/php_json.h"
#include "ext/standard/info.h"
#include "Zend/zend_smart_str.h"
#include "kernel/loader/loader.h"
#include "ext/pdo/php_pdo_driver.h"
#include "ext/standard/php_string.h"

/**
 * {{{
 * proto All arginfo for class ConfigClass
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_construct), 0, 0, 3)
    ZEND_ARG_INFO(0, dsn)
    ZEND_ARG_INFO(0, userName)
    ZEND_ARG_INFO(0, password)
    ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_create_query_command), 0, 0, 1)
    ZEND_ARG_INFO(0, prepareSql)
    ZEND_ARG_INFO(0, bindValues)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_execute), 0, 0, 0)
    ZEND_ARG_INFO(0, bindValues)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_exec_command), 0, 0, 1)
    ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_get_pdo), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_get_pdostatement), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_fetch_one), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_fetch_all), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_start_transaction), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_rollback), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_adapter_commit), 0, 0, 0)
ZEND_END_ARG_INFO()


/**
 * {{{ proto Adapter::ZEND_CONSTRUCTOR_FUNC_NAME()
 */
XAN_METHOD(Adapter, __construct)
{ 
    zval pdo_object, *options = NULL;

    zend_string *dsn, *user_name, *passwd;

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SSS|a", &dsn, &user_name, &passwd, &options) == FAILURE ) {
        return ;
    }

    xan_get_pdo_object( &pdo_object, ZSTR_VAL(dsn), ZSTR_VAL(user_name), ZSTR_VAL(passwd), options, return_value );

    if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
        ZVAL_COPY(&XAN_G(pdo_object), &pdo_object);
    }

    zend_update_property( XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), &pdo_object );
}
/*}}}*/

/**
 * {{{
 * proto Adapter::createCommand( $prepareSql [, $bindValues = [] ] )
 */
XAN_METHOD(Adapter, createQueryCommand)
{
    zend_string *prepare_sql;
    zval *bind_values = NULL, *pdo_object, *pdo_state_obj;

    if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S|a", &prepare_sql, &bind_values) == FAILURE ) {
        return ;
    }

    pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    pdo_state_obj = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_STATEMENT_OBJ), 1, NULL);
    if ( !pdo_object ) {
        if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
            XAN_INFO(E_ERROR, "Please construct the Adapter class first!");
        }
        ZVAL_COPY(pdo_object, &XAN_G(pdo_object));
    }

    if ( !ZSTR_LEN(prepare_sql) ) {
        XAN_INFO(E_ERROR, "The preapared SQL was invalid!");
    }

    xan_pdo_prepare(pdo_object, ZSTR_VAL(prepare_sql), pdo_state_obj);
    xan_check_pdo_error(pdo_object, NULL);
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * To exeucte the prepared sql with the given bind values(if gived)
 */
XAN_METHOD(Adapter, execute)
{
    zval *bind_data = NULL, *pdo_state_obj, retval;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &bind_data) == FAILURE) {
        return ;
    }
    pdo_state_obj = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_STATEMENT_OBJ), 1, NULL);
    if ( pdo_state_obj == NULL ) {
        XAN_INFO(E_ERROR, "Please call createCommand() first!");
    }

    xan_pdo_execute(pdo_state_obj, bind_data, &retval);
    xan_check_pdo_error(NULL, pdo_state_obj);
    ZVAL_COPY(return_value, getThis());
}/*}}}*/

/**
 * {{{
 * proto Adapter::execCommand($sql)
 */
XAN_METHOD(Adapter, execCommand)
{
    zval *pdo_object;
    zend_string *exec_sql;
    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S", &exec_sql) == FAILURE ) {
        return ;
    }

    if ( !ZSTR_LEN(exec_sql) ) {
        XAN_INFO(E_ERROR, "The SQL was invalid!");
    }

    pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) {
        if ( pdo_object == NULL ) {
            if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
                XAN_INFO(E_ERROR, "Please construct the Adapter class first!");
            }
            ZVAL_COPY(pdo_object, &XAN_G(pdo_object));
        }
    }
    xan_pdo_exec(pdo_object, ZSTR_VAL(exec_sql), return_value);

    xan_check_pdo_error(pdo_object, NULL);
}/*}}}*/

/**
 * {{{
 * proto Adapter::getPdo()
 */
XAN_METHOD(Adapter, getPdo)
{
    zval *pdo_object;

    if (zend_parse_parameters_none() == FAILURE) {
        return ;
    }

    pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) {
        if ( pdo_object == NULL ) {
            if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
                XAN_INFO(E_ERROR, "Please construct the Adapter class first!");
            }
            ZVAL_COPY(pdo_object, &XAN_G(pdo_object));
        }
    }

    ZVAL_COPY(return_value, pdo_object);
}/*}}}*/

/**
 * {{{
 * proto Adapter::getPdoStatement()
 */
XAN_METHOD(Adapter, getPdoStatement)
{
    zval *pdo_state_obj;

    if (zend_parse_parameters_none() == FAILURE) {
        return ;
    }
    pdo_state_obj = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_STATEMENT_OBJ), 1, NULL);
    if ( pdo_state_obj == NULL ) RETURN_NULL()

    ZVAL_COPY(return_value, pdo_state_obj);
}/*}}}*/

/**
 * {{{
 * Adapter::one()
 */
XAN_METHOD(Adapter, one)
{
    zval *pdo_state_obj = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_STATEMENT_OBJ), 1, NULL);
    if ( pdo_state_obj == NULL ) RETURN_NULL()

    xan_pdo_fetch(pdo_state_obj, return_value);
}/*}}}*/

/**
 * {{{
 * Adapter::all()
 */
XAN_METHOD(Adapter, all)
{
    zval *pdo_state_obj = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_STATEMENT_OBJ), 1, NULL);
    if ( pdo_state_obj == NULL ) RETURN_NULL()

    xan_pdo_fetch_all(pdo_state_obj, return_value);
}/*}}}*/

/**
 * {{{
 * proto Adapter::beginTransaction()
 */
XAN_METHOD(Adapter, beginTransaction)
{
    zval *pdo_object;

    if (zend_parse_parameters_none() == FAILURE) {
        return ;
    }

    pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) {
        if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
            XAN_INFO(E_ERROR, "Please construct the Adapter class first!");
        }
        ZVAL_COPY(pdo_object, &XAN_G(pdo_object));
    }

    xan_pdo_start_transaction(pdo_object, return_value);
}/*}}}*/

/**
 * {{{
 * Adapter::rollBack()
 */
XAN_METHOD(Adapter, rollBack)
{
    zval *pdo_object;

    if (zend_parse_parameters_none() == FAILURE) {
        return ;
    }

    pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) {
        if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
            XAN_INFO(E_ERROR, "Please construct the Adapter class first!");
        }
        ZVAL_COPY(pdo_object, &XAN_G(pdo_object));
    }

    xan_pdo_rollback(pdo_object, return_value);
}/*}}}*/

/**
 * Adapter::commit()
 */
XAN_METHOD(Adapter, commit)
{
    zval *pdo_object;

    if (zend_parse_parameters_none() == FAILURE) {
        return ;
    }

    pdo_object = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(PDO_OBJECT), 1, NULL);
    if ( pdo_object == NULL ) {
        if ( ZVAL_IS_NULL(&XAN_G(pdo_object)) ) {
            XAN_INFO(E_ERROR, "Please construct the Adapter class first!");
        }
        ZVAL_COPY(pdo_object, &XAN_G(pdo_object));
    }

    xan_pdo_commit(pdo_object, return_value);
}/*}}}*/

/**
 * {{{
 * All function for the Db class adapter
 */
XAN_FUNCTIONS(adapter)
    XAN_ME(Adapter, __construct, arginfo_xan_adapter_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, createQueryCommand, arginfo_xan_adapter_create_query_command, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, execute, arginfo_xan_adapter_execute, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, execCommand, arginfo_xan_adapter_exec_command, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, getPdo, arginfo_xan_adapter_get_pdo, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, getPdoStatement, arginfo_xan_adapter_get_pdostatement, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, one, arginfo_xan_adapter_fetch_one, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, all, arginfo_xan_adapter_fetch_all, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, beginTransaction, arginfo_xan_adapter_start_transaction, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, rollBack, arginfo_xan_adapter_rollback, ZEND_ACC_PUBLIC)
    XAN_ME(Adapter, commit, arginfo_xan_adapter_commit, ZEND_ACC_PUBLIC)
XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{ proto adapter_functions
 */
XAN_INIT(adapter)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan\\Db", "Adapter", adapter_functions);
    xan_adapter_ce = zend_register_internal_class(&ce);

    XAN_PR_NULL(xan_adapter_ce, PDO_OBJECT, ZEND_ACC_PROTECTED);
    XAN_PR_NULL(xan_adapter_ce, PDO_BIND_DATA, ZEND_ACC_PROTECTED);
    XAN_PR_NULL(xan_adapter_ce, PDO_STATEMENT_OBJ, ZEND_ACC_PROTECTED);

}/*}}}*/

/**
 * {{{
 * To get the PDO object with the given parameters
 */
void xan_get_pdo_object(zval *pdo_object, char *dsn, char *username, char *password, zval *options, zval *retval)
{
    zval pdo_dsn, user_name, pass_word,function_name;
    ZVAL_STRING(&pdo_dsn, dsn);
    ZVAL_STRING(&user_name, username);
    ZVAL_STRING(&pass_word, password);
    object_init_ex(pdo_object, php_pdo_get_dbh_ce());
    ZVAL_STRING(&function_name, "__construct");
    if ( options ) {
        zval params[4] = { pdo_dsn, user_name, pass_word, *options };
        call_user_function( NULL, pdo_object, &function_name, retval, 4, params );
    } else {
        zval params[3] = { pdo_dsn, user_name, pass_word };
        call_user_function( NULL, pdo_object, &function_name, retval, 3, params );
    }
    zval_ptr_dtor(&pdo_dsn);
    zval_ptr_dtor(&user_name);
    zval_ptr_dtor(&pass_word);
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To start transaction
 **/
void xan_pdo_start_transaction(zval *pdo_object, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "beginTransaction");
    call_user_function( NULL, pdo_object, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To commit the transaction
 */
void xan_pdo_commit(zval *pdo_object, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "commit");
    call_user_function( NULL, pdo_object, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To rollback the transaction chages.
 */
void xan_pdo_rollback(zval *pdo_object, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "rollBack");
    call_user_function( NULL, pdo_object, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To check wheather the PDO is intransaction or not
 */
void xan_pdo_in_transaction(zval *pdo_object, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "inTransaction");
    call_user_function( NULL, pdo_object, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To get the last insert id
 */
void xan_pdo_get_last_insert_id(zval *pdo_object, char *name, zval *retval)
{
    zval function_name, id_name;
    ZVAL_STRING(&function_name, "lastInsertId");
    if ( name ) {
        ZVAL_STRING(&id_name, name);
        zval params[] = { id_name };
        call_user_function( NULL, pdo_object, &function_name, retval, 1, params );  
    } else {
        call_user_function( NULL, pdo_object, &function_name, retval, 0, NULL );   
    }
    zval_ptr_dtor(&id_name);
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To do the prepare Job.
 * [ NOTE THAT: This function's retval is the pdostatement_obj ]
 */
void xan_pdo_prepare(zval *pdo_object, char *sql, zval *retval)
{
    zval function_name, prepare_sql;
    ZVAL_STRING(&function_name, "prepare");
    if ( sql ) {
        ZVAL_STRING(&prepare_sql, sql);
        zval params[] = { prepare_sql };
        call_user_function( NULL, pdo_object, &function_name, retval, 1, params );  
    }
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To execute the SQL with the given bind data
 */
void xan_pdo_execute(zval *pdostatement_obj, zval *bind_data, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "execute");
    if ( bind_data != NULL ){
        zval params[] = { *bind_data };
        call_user_function(NULL, pdostatement_obj, &function_name, retval, 1, params);
    } else {
        call_user_function( NULL, pdostatement_obj, &function_name, retval, 0, NULL );
    }
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To execute the INSERT|UPDATE|DELETE sql
 */
void xan_pdo_exec(zval *pdo_object, char *sql, zval *retval)
{
    zval function_name, pdo_sql;
    ZVAL_STRING(&function_name, "exec");
    ZVAL_STRING(&pdo_sql, sql);
    zval params[] = { pdo_sql };
    call_user_function(NULL, pdo_object, &function_name, retval, 1, params);
    zval_ptr_dtor(&pdo_sql);
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To fetch one row data from the DB
 */
void xan_pdo_fetch(zval *pdostatement_obj, zval *retval)
{
    zval function_name, fetch_assoc;
    ZVAL_LONG(&fetch_assoc, 2);
    ZVAL_STRING(&function_name, "fetch");
    zval params[1] = { fetch_assoc };
    call_user_function( NULL, pdostatement_obj, &function_name, retval, 1, params );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To fetch ALL satisfied data from the DB engine
 */
void xan_pdo_fetch_all(zval *pdostatement_obj, zval *retval)
{
    zval function_name, fetch_assoc;
    ZVAL_LONG(&fetch_assoc, 2);
    ZVAL_STRING(&function_name, "fetchAll");
    zval params[1] = { fetch_assoc };
    call_user_function( NULL, pdostatement_obj, &function_name, retval, 1, params );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To get the row count
 */
void xan_pdo_row_count(zval *pdostatement_obj, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "rowCount");
    call_user_function( NULL, pdostatement_obj, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * The following two function to get error info for the PDO or PDOStatement
 */
void xan_pdostate_error_info(zval *pdostatement_obj, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "errorInfo");
    call_user_function( NULL, pdostatement_obj, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}

void xan_pdo_error_info(zval *pdo_object, zval *retval)
{
    zval function_name;
    ZVAL_STRING(&function_name, "errorInfo");
    call_user_function( NULL, pdo_object, &function_name, retval, 0, NULL );
    zval_ptr_dtor(&function_name);
}/*}}}*/

/**
 * {{{
 * To check wheather the error occured or not
 */
zend_bool xan_check_pdo_error(zval *pdo_object, zval *pdostatement_obj)
{
    zval errorinfo;

    if ( pdo_object != NULL && pdostatement_obj == NULL ) {
        xan_pdo_error_info(pdo_object, &errorinfo);
    } else if ( pdo_object == NULL && pdostatement_obj != NULL ) {
        xan_pdostate_error_info(pdostatement_obj, &errorinfo);
    } else if ( pdo_object != NULL && pdostatement_obj != NULL ) {
        xan_pdostate_error_info(pdostatement_obj, &errorinfo);
    } else {
        return 1;
    }

    zval *sqlstate_code = INDEX_FIND(errorinfo, 0);
    zval *error_code    = INDEX_FIND(errorinfo, 1);
    zval *error_info    = INDEX_FIND(errorinfo, 2);

    if ( !zend_string_equals_literal(Z_STR_P(sqlstate_code), "00000") ) {
        XAN_INFO(E_ERROR, "[ SQLSTATE:%s; ERRORINFO: %s ]\n", Z_STRVAL_P(sqlstate_code), Z_STRVAL_P(error_info));
        return 0;
    } else {
        return 1;
    }
}/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
