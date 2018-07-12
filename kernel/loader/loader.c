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
#include "kernel/loader/loader.h"
#include "Zend/zend_exceptions.h"

/**
 * {{{
 * All arginfos for all methods for Xan\Loader class
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_loader_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_loader_set_map), 0, 0, 2)
    ZEND_ARG_INFO(0, aliasName)
    ZEND_ARG_INFO(0, aliasPath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_loader_auto_load), 0, 0, 0)
    ZEND_ARG_INFO(0, prepend)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(xan_loader_loader), 0, 0, 1)
    ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

/*}}}*/

/**
 * {{{ proto 
 * Loader::__construct()
 */
XAN_METHOD(Loader, __construct)
{
    zval *aliases = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(ALL_ALIAS), 1, NULL);
    array_init(aliases);
}/*}}}*/

/**
 * {{{ proto 
 * Loader::setMap($aliasName, $aliasPath)
 * $aliasName: must be start with '@'
 * $aliasPath: The absolute path of the directory
 */
XAN_METHOD(Loader, setMap)
{
    zend_string *alias_name, *alias_path;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SS", &alias_name, &alias_path) == FAILURE)
    {
        return ;
    }

    if ( *(ZSTR_VAL(alias_name)) != '@' )
    {
        XAN_INFO(E_ERROR, "$aliasName must be start with `@`!");
    }

    zval *aliases = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(ALL_ALIAS), 1, NULL);

    add_assoc_str(aliases, ZSTR_VAL(alias_name) + 1, alias_path);

    RETURN_TRUE;
}/*}}}*/

/**
 * {{{ proto 
 * final Loader::loader($className)
 * This is the internal class for the loader
 */
XAN_METHOD(Loader, loader)
{
    zval *zvalue;
    char class_alias[256] = {0};
    zend_string *class_name, *zkey;
    size_t class_alias_len = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &class_name) == FAILURE)
    {
        return ;
    }

    char *slash_pos = strchr(ZSTR_VAL(class_name), '\\');
    if ( !slash_pos )
    {
        XAN_INFO(E_ERROR, "Please set the alias map first.");
    }
    class_alias_len = slash_pos - ZSTR_VAL(class_name);
    memcpy(class_alias, ZSTR_VAL(class_name), class_alias_len);

    zval *aliases = zend_read_property(XAN_ENTRY_OBJ(getThis()), XAN_STRL(ALL_ALIAS), 1, NULL);

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(aliases), zkey, zvalue)
    {
        if ( !strncasecmp(ZSTR_VAL(zkey), XAN_STRL(class_alias)) )
        {
            zend_string *real_file_path  = strpprintf(0, "%s/%s.php", Z_STRVAL_P(zvalue), slash_pos + 1);
            reverse_zend_string_slash(real_file_path);
            is_file(ZSTR_VAL(real_file_path));
            require_php_file(ZSTR_VAL(real_file_path));
            RETURN_TRUE;
        }
    } ZEND_HASH_FOREACH_END();

    XAN_INFO(E_ERROR, "Class: [%s] can't be autoloaded.", ZSTR_VAL(class_name));
}/*}}}*/

/**
 * {{{ proto
 * Loader::autoLoad([$prepend = false])
 * Start the autoload step
 */
XAN_METHOD(Loader, autoLoad)
{
    zval zcallback, zprepend, zfunc_name, retval, zshow;
    zend_bool prepend = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &prepend) == FAILURE)
    {
        return ;
    }

    array_init(&zcallback);
    ZVAL_LONG(&zprepend, prepend);
    ZVAL_LONG(&zshow, 1);
    ZVAL_STRING(&zfunc_name, "spl_autoload_register");

    add_next_index_zval(&zcallback, getThis());
    add_next_index_string(&zcallback, "loader");

    zval params[3] = { zcallback, zshow, zprepend };

    call_user_function(EG(function_table), NULL, &zfunc_name, &retval, 3, params);
    
    zval_ptr_dtor(&zshow);
    zval_ptr_dtor(&retval);
    zval_ptr_dtor(&zprepend);
    zval_ptr_dtor(&zcallback);
    zval_ptr_dtor(&zfunc_name);

    RETURN_TRUE;
}/*}}}*/

/**
 * {{{ proto
 * loader_functions definitions for Xan\Loader class
 */
XAN_FUNCTIONS(loader)
    XAN_ME(Loader, __construct, arginfo_xan_loader_construct, ZEND_ACC_PUBLIC)
    XAN_ME(Loader, setMap, arginfo_xan_loader_set_map, ZEND_ACC_PUBLIC)
    XAN_ME(Loader, loader, arginfo_xan_loader_loader, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
    XAN_ME(Loader, autoLoad, arginfo_xan_loader_auto_load, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
XAN_FUNCTIONS_END()
/*}}}*/

/**
 * {{{ proto loader_init()
 * To load the class to the php kernel.
 */
XAN_INIT(loader)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Xan", "Loader", loader_functions);
    loader_ce = zend_register_internal_class(&ce);

    XAN_PR_NULL(loader_ce, ALL_ALIAS, ZEND_ACC_PUBLIC);
}/*}}}*/

/**
 * {{{ proto
 * reverse_zend_string_slash from '\' to '/' to resolve the path problem
 */
void reverse_zend_string_slash(zend_string *string)
{
    int i = 0;
    char *str = ZSTR_VAL(string);
    for (; i < ZSTR_LEN(string); i++)
    {
        if ( *(str + i) == '\\' )
        {
            *(str + i) = '/';
        }
    }
}/*}}}*/

/**
 * {{{ proto
 * Return the current directory
 */
void get_current_cwd(char *path)
{
#if 0
    char path[MAXPATHLEN];
#endif
    char *ret=NULL;

    #if HAVE_GETCWD
        ret = VCWD_GETCWD(path, MAXPATHLEN);
    #elif HAVE_GETWD
        ret = VCWD_GETWD(path);
    #endif
    if (!ret){
        path[0] = '.';
        path[1] = '\0';
    }
}/*}}}*/

/**
 * {{{
 * proto is_file(char *path) : to check whether the file is exists or not.
 * when in production mode, each file will only be show the file name not the read path
 */
void is_file(char *path)
{
    if (access(path, F_OK) == -1) {
        if (DEBUG_MODE)
        {
            XAN_INFO(E_ERROR, "File: %s not exists!", path);
        }
        else
        {
            char *filename = strrchr(path, '/');
            if ( !filename )
            {
                filename = path;
            }
            XAN_INFO(E_ERROR, "File: %s not exists!", filename + 1);
        }
    }
}/*}}}*/

/**
 * {{{
 * proto require_php_file(char *file_name)
 * to require the php file
 */
void require_php_file(char *file_name)
{
    zval result;
    zend_file_handle file_handle;
    file_handle.free_filename = 0;
    file_handle.type          = ZEND_HANDLE_FILENAME;
    file_handle.opened_path   = NULL;
    file_handle.filename      = file_name;
    file_handle.handle.fp     = NULL;
    zend_op_array *op_array   = NULL;
    
    zend_try {
        op_array = zend_compile_file(&file_handle, ZEND_REQUIRE);
    } zend_end_try();

    if (EG(exception)) zend_exception_error(EG(exception), E_ERROR);

    zend_execute_data *require_call = zend_vm_stack_push_call_frame( ZEND_CALL_NESTED_CODE | ZEND_CALL_HAS_SYMBOL_TABLE,
          (zend_function *)op_array, 0, NULL, NULL
    );

    zend_init_execute_data( require_call, op_array, &result );
    ZEND_ADD_CALL_FLAG(require_call, ZEND_CALL_TOP);
    zend_execute_ex(require_call);
    zend_vm_stack_free_call_frame(require_call);
    destroy_op_array(op_array);
    efree_size(op_array, sizeof(zend_op_array));

    if (EG(exception)) zend_exception_error(EG(exception), E_ERROR);
    
    zend_destroy_file_handle(&file_handle);
}/*}}}*/




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
