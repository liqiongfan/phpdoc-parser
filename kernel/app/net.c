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
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

/**
 * {{{ Get value from $_GET
 */
zval *xan_get_get_vars(const char *get_key)
{
    zval *get_variables = &PG(http_globals)[TRACK_VARS_GET];
    if ( !get_key ) {
        return get_variables;
    } else {
        zend_string *xget_key = zend_string_init(get_key, strlen(get_key), 0);
        zval *result = zend_hash_find( Z_ARRVAL_P(get_variables), xget_key );
        zend_string_release(xget_key);
        return result;
    }
}/*}}}*/

/**
 * {{{
 * Get the value from $_POST
 */
zval *xan_get_post_var(const char *post_key)
{
    zval *post_variables = &PG(http_globals)[TRACK_VARS_POST];
    if (!post_key) {
        return post_variables;
    } else {
        zend_string *xpost_key = zend_string_init(post_key, strlen(post_key), 0);
        zval *result = zend_hash_find(Z_ARRVAL_P(post_variables), xpost_key);
        zend_string_release(xpost_key);
        return result;
    }
}/*}}}*/

 /**
  * {{{ Get value from $_SERVER variables
  **/
zval *xan_get_server_var(const char *server_key)
{
    if (PG(auto_globals_jit)) {
        zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
        zend_is_auto_global(server_str);
        zend_string_release(server_str);
    }
    /* Get the $_SERVER['xxx'] value */
    zval *server_variables = &PG(http_globals)[TRACK_VARS_SERVER];
    if (!server_key) {
        return server_variables;
    } else {
        zend_string *xserver_key = zend_string_init(server_key, strlen(server_key), 0);
        zval *result = zend_hash_find(Z_ARRVAL_P(server_variables), xserver_key);
        zend_string_release(xserver_key);
        return result;
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