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
#ifndef XAN_ADAPTER_H
#define XAN_ADAPTER_H

#define PDO_OBJECT "_pdo"
#define PDO_BIND_DATA "_pdo_bind"
#define PDO_STATEMENT_OBJ "_pdo_state_obj"

/* To get the pdo object */
void xan_get_pdo_object(zval *pdo_object, char *dsn, char *username, char *password, zval *options, zval *retval);

/* Start transaction */
void xan_pdo_start_transaction(zval *pdo_object, zval *retval);

/* Commit the PDO transaction */
void xan_pdo_commit(zval *pdo_object, zval *retval);

/* To rollback the transaction */
void xan_pdo_rollback(zval *pdo_object, zval *retval);

/* To check wheather the PDO is in transaction */
void xan_pdo_in_transaction(zval *pdo_object, zval *retval);

/* To get the last insert id */
void xan_pdo_get_last_insert_id(zval *pdo_object, char *name, zval *retval);

/* Do the prepare job return the PDOStatement object */
void xan_pdo_prepare(zval *pdo_object, char *sql, zval *retval);

/* Execute the PDO sql */
void xan_pdo_execute(zval *pdostatement_obj, zval *bind_data, zval *retval);

/* Exec the SQL */
void xan_pdo_exec(zval *pdo_object, char *sql, zval *retval);

/* Get the SQL prepare error */
void xan_pdostate_error_info(zval *pdostatement_obj, zval *retval);
void xan_pdo_error_info(zval *pdo_object, zval *retval);

/* To check the error info */
zend_bool xan_check_pdo_error(zval *pdo_object, zval *pdostatement_obj);

/* To fetch the row data */
void xan_pdo_fetch(zval *pdostatement_obj, zval *retval);

/* To fetch all datas */
void xan_pdo_fetch_all(zval *pdostatement_obj, zval *retval);

/* To get the row count of the data */
void xan_pdo_row_count(zval *pdostatement_obj, zval *retval);

extern zend_class_entry *xan_adapter_ce;

#endif /* XAN_ADAPTER_H */




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
