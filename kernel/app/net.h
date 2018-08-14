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
#ifndef XAN_NET_HELPER_H
#define XAN_NET_HELPER_H

/* Get value from $_GET */
zval *xan_get_get_vars(const char *get_key);

/* Get value from $_POST */
zval *xan_get_post_var(const char *post_key);

/* Get value from $_SERVER */
zval *xan_get_server_var(const char *server_key);

#endif /* XAN_NET_HELPER_H */




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */