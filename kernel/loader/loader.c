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
#include "php_main.h"
#include "ext/standard/info.h"
#include "php_xannotation.h"
#include "kernel/loader/loader.h"
#include "Zend/zend_exceptions.h"
#include "kernel/xan_class.h"
#include "Zend/zend_interfaces.h"
#include "kernel/class/aop/proxy.h"
#include "kernel/class/annotation/annotation.h"
#include "kernel/class/annotation/class_attr.h"
#include "kernel/class/annotation/class_const.h"

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
    array_init(&XAN_G(aliases));

    /* Enable auto_load */
    zval zcallback, zprepend, zfunc_name, retval, zshow;
    zend_bool prepend = 0;

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
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SS", &alias_name, &alias_path) == FAILURE){
        return ;
    }

    if ( *(ZSTR_VAL(alias_name)) != '@' ){
        XAN_INFO(E_ERROR, "$aliasName must be start with `@`!");
    }

    add_assoc_str(&XAN_G(aliases), ZSTR_VAL(alias_name) + 1, alias_path);

    RETURN_TRUE;
}/*}}}*/

/**
 * {{{ proto 
 * final Loader::loader($className)
 * This is the internal class for the loader
 */
XAN_METHOD(Loader, loader)
{
    zend_string *class_name;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &class_name) == FAILURE){
        return ;
    }

    auto_load_classfile(class_name, &XAN_G(aliases));
}/*}}}*/

/**
 * {{{ proto
 * Loader::start([$prepend = false])
 * Start the autoload step
 */
XAN_METHOD(Loader, start)
{
    zval zcallback, zprepend, zfunc_name, retval, zshow;
    zend_bool prepend = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &prepend) == FAILURE) {
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
    XAN_ME(Loader, start, arginfo_xan_loader_auto_load, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
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
    for (; i < ZSTR_LEN(string); i++) {
        if ( *(str + i) == '\\' ) {
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
    if (!ret) {
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
        if (DEBUG_MODE) {
            XAN_INFO(E_ERROR, "Path: `%s` not exists!", path);
        } else {
            char *filename = strrchr(path, '/');
            if ( !filename ) {
                filename = path;
            }
            XAN_INFO(E_ERROR, "Path: `%s` not exists!", filename + 1);
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
    zend_execute_data *orig_execute_data = NULL;
    zend_execute_data *require_call;
    orig_execute_data = EG(current_execute_data);
    op_array = zend_compile_file(&file_handle, ZEND_REQUIRE);
    EG(current_execute_data) = orig_execute_data;
    if ( op_array ) {
        require_call = zend_vm_stack_push_call_frame( ZEND_CALL_NESTED_CODE | ZEND_CALL_HAS_SYMBOL_TABLE,
            (zend_function *)op_array, 0, NULL, NULL
        );
        zend_init_execute_data( require_call, op_array, &result );
        ZEND_ADD_CALL_FLAG(require_call, ZEND_CALL_TOP);
        zend_execute_ex(require_call);
        if (EG(exception)) zend_exception_error(EG(exception), E_ERROR);
        zend_vm_stack_free_call_frame(require_call);

        destroy_op_array(op_array);
        efree_size(op_array, sizeof(zend_op_array));
        zend_destroy_file_handle(&file_handle);
    } else {
        zend_destroy_file_handle(&file_handle);
        XAN_INFO(E_ERROR, "%s", "Compile PHP file error!");
    }
}/*}}}*/

/**
 * {{{ proto 
 * Require the template file into the view file
 */
int xan_require_file(const char * file_name, zval *variables, zval *called_object_ptr, zval *return_view)
{
    zval result;
    zend_file_handle file_handle;

    file_handle.free_filename = 0;
    file_handle.type          = ZEND_HANDLE_FILENAME;
    file_handle.opened_path   = NULL;
    file_handle.filename      = file_name;
    file_handle.handle.fp     = NULL;
    zend_op_array *op_array   = NULL;
    zend_execute_data *orig_execute_data = NULL;

    orig_execute_data = EG(current_execute_data);
    op_array = zend_compile_file(&file_handle, ZEND_INCLUDE);
    EG(current_execute_data) = orig_execute_data;

    if ( op_array ) {
        zend_execute_data *require_call = zend_vm_stack_push_call_frame(
            ZEND_CALL_NESTED_CODE | ZEND_CALL_HAS_SYMBOL_TABLE,
            (zend_function *)op_array, 
            0, 
            ( called_object_ptr != NULL ) ? Z_OBJCE_P(called_object_ptr) : NULL, 
            ( called_object_ptr != NULL ) ? Z_OBJ_P(called_object_ptr) : NULL
        );
        /* To extract the variables into the view file */
        if ( (variables == NULL) || !zend_hash_num_elements(Z_ARRVAL_P(variables)) ) {
            zval empty_variables;
            array_init(&empty_variables);
            variables = &empty_variables;
        }
        if ( Z_TYPE_P(variables) == IS_ARRAY ){
            zend_array *symbol_tables = emalloc(sizeof(zend_array));
            zend_hash_init(symbol_tables, 8, NULL, ZVAL_PTR_DTOR, 0);
            zend_hash_real_init(symbol_tables, 0);
            zend_string *var_name;
            zval *var_value;
            ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(variables), var_name, var_value){
                if (EXPECTED(zend_hash_add_new(symbol_tables, var_name, var_value))) {
                    Z_TRY_ADDREF_P(var_value);
                }
            } ZEND_HASH_FOREACH_END();
            require_call->symbol_table = symbol_tables;
        }
        if (return_view &&  php_output_start_user( NULL, 0, PHP_OUTPUT_HANDLER_STDFLAGS ) == FAILURE) {
            XAN_INFO( E_WARNING, "Failed to call ob_start().");
            return 0;
        }
        
        zend_init_execute_data( require_call, op_array, &result );
        ZEND_ADD_CALL_FLAG(require_call, ZEND_CALL_TOP);
        zend_execute_ex(require_call);
        zend_vm_stack_free_call_frame(require_call);
        destroy_op_array(op_array);
        efree_size(op_array, sizeof(zend_op_array));
        if (EG(exception)) {
            zend_exception_error(EG(exception), E_ERROR);
            return 0;
        }

        if ( return_view ) { /* Store the data into the return_view zval struct and discard the data in the output */
            if (php_output_get_contents(return_view) == FAILURE) {
                php_output_end();
                XAN_INFO(E_WARNING, "Can't fetch the ob_data.");
                return 0;
            }
            if (php_output_discard() != SUCCESS ) {
                return 0;
            }
        }
        zend_destroy_file_handle(&file_handle);
        return 1;
    } else {
        XAN_INFO(E_ERROR, "View file contains PHP systax error!");
        return 0;
    }
}/*}}}*/

/**
 * {{{
 * void init_class_with_annotations(zend_string *class_name, zval *aliases);
 * To init the class with the given annotations
 * @class_name : The class which you want to do the job with the annotations.
 * @aliases    : The aliases for the autoload job.
 */
void init_class_with_annotations(zend_string *class_name, zval *aliases)
{
    class_name = zend_string_tolower(class_name);
    zend_class_entry *ce = zend_hash_str_find_ptr(CG(class_table), ZSTR_VAL(class_name), ZSTR_LEN(class_name));
    if ( !ce ) {
        return ;
    }

    zend_string *annotation_class_name;
    zval annotations, *annotation_class_value;
    array_init(&annotations);

    get_doc_comment_result(&annotations, get_class_doc_comment(ce));

    zval *annos = zend_hash_str_find(Z_ARRVAL(annotations), XAN_STRL("annotations"));
    if ( !annos || ZVAL_IS_NULL(annos) ) {
        return ;
    }

    int times = 0;
    zend_class_entry *o_ce;
    zval class_obj, retval, class_entry;
    ZVAL_STR(&class_entry, class_name);

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(annos), annotation_class_name, annotation_class_value) {
        if (*ZSTR_VAL(annotation_class_name) == '\\') {
            annotation_class_name = strpprintf(0, "%s", ZSTR_VAL(annotation_class_name) + 1);
        }

        if (zend_string_equals_literal(annotation_class_name, XAN_ANNOTATIONS "AttrAnnotation")) {
            object_init_ex(&class_obj, class_attr_ce);
            zend_call_method_with_2_params(&class_obj, class_attr_ce, NULL, "input", &retval, &class_entry, annotation_class_value);
        }
        else if (zend_string_equals_literal(annotation_class_name, XAN_ANNOTATIONS "ConstAnnotation" )) {
            object_init_ex(&class_obj, class_const_ce);
            zend_call_method_with_2_params(&class_obj, class_const_ce, NULL, "input", &retval, &class_entry, annotation_class_value);
        }
        else {
again:
            o_ce = zend_hash_find_ptr( CG(class_table), strpprintf(0, "%s", ZSTR_VAL(zend_string_tolower(annotation_class_name))) );
            if ( !o_ce ) {
                if (times >= 1) {
                    XAN_INFO(E_ERROR, "Xan can't load the class `%s`!", ZSTR_VAL(annotation_class_name));
                    return ;
                }
                only_auto_load_file(annotation_class_name, aliases);
                times++;
                goto again;
            }
            if ( !instanceof_function(o_ce, annotation_ce) ) {
                zend_array_destroy(Z_ARRVAL(annotations));
                XAN_INFO(E_ERROR, "Annotation class : `%s` must be implemented from Annotation interface!", ZSTR_VAL(annotation_class_name) );
                return ;
            }
            get_object_from_di(&XAN_G(class_di), annotation_class_name, &class_obj, o_ce);
            zend_call_method_with_2_params( &class_obj, o_ce, NULL, "input", &retval, &class_entry, annotation_class_value );
        }

    } ZEND_HASH_FOREACH_END();

    zend_array_destroy(Z_ARRVAL(annotations));
}/*}}}*/

/**
 * {{{
 * proto auto_load_classfile(zend_string *)
 */
void auto_load_classfile(zend_string *class_name, zval *aliases)
{
    zval *zvalue;
    zend_string *zkey;
    char class_alias[256] = {0};
    size_t class_alias_len = 0;

    char *slash_pos = strchr(ZSTR_VAL(class_name), '\\');
    if ( !slash_pos ) {
        class_alias_len = ZSTR_LEN(class_name);
        memcpy(class_alias, ZSTR_VAL(class_name), class_alias_len);

        zend_string *real_file_path  = strpprintf(0, "./%s.php", class_alias);
        reverse_zend_string_slash(real_file_path);
        is_file(ZSTR_VAL(real_file_path));
        require_php_file(ZSTR_VAL(real_file_path));
        init_class_with_annotations(class_name, aliases);
        return ;
    } else {
        class_alias_len = slash_pos - ZSTR_VAL(class_name);
    }
    
    memcpy(class_alias, ZSTR_VAL(class_name), class_alias_len);

    if (!zend_hash_num_elements(Z_ARRVAL_P(aliases))) {
        XAN_INFO(E_ERROR, "Please setMap the aliasName before start()!");
    }

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(aliases), zkey, zvalue) {
        if ( !strncasecmp(ZSTR_VAL(zkey), XAN_STRL(class_alias)) ) {
            zend_string *real_file_path  = strpprintf(0, "%s/%s.php", Z_STRVAL_P(zvalue), slash_pos + 1);
            reverse_zend_string_slash(real_file_path);
            is_file(ZSTR_VAL(real_file_path));
            zend_try {
                require_php_file(ZSTR_VAL(real_file_path));
            } zend_catch {
                php_request_shutdown(NULL);
            } zend_end_try();
            init_class_with_annotations(class_name, aliases);
            return ;
        }
    } ZEND_HASH_FOREACH_END();

    XAN_INFO(E_ERROR, "Class: [%s] can't be autoloaded.", ZSTR_VAL(class_name));
}/*}}}*/

/**
 * auto load the file only. not do the parsing the doc-comment job.
 */
void only_auto_load_file(zend_string *class_name, zval *aliases)
{
    zval *zvalue;
    zend_string *zkey;
    char class_alias[256] = {0};
    size_t class_alias_len = 0;

    char *slash_pos = strchr(ZSTR_VAL(class_name), '\\');
    if ( !slash_pos ) {
        class_alias_len = ZSTR_LEN(class_name);
        memcpy(class_alias, ZSTR_VAL(class_name), class_alias_len);

        zend_string *real_file_path  = strpprintf(0, "./%s.php", class_alias);
        reverse_zend_string_slash(real_file_path);
        is_file(ZSTR_VAL(real_file_path));
        require_php_file(ZSTR_VAL(real_file_path));
        if (EG(exception)) zend_exception_error(EG(exception), E_ERROR);
        return ;
    } else {
        class_alias_len = slash_pos - ZSTR_VAL(class_name);
    }
    
    memcpy(class_alias, ZSTR_VAL(class_name), class_alias_len);

    if (!zend_hash_num_elements(Z_ARRVAL_P(aliases))) {
        XAN_INFO(E_ERROR, "Please setMap the aliasName before autoLoad()!");
    }

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(aliases), zkey, zvalue) {
        if ( !strncasecmp(ZSTR_VAL(zkey), XAN_STRL(class_alias)) ) {
            zend_string *real_file_path  = strpprintf(0, "%s/%s.php", Z_STRVAL_P(zvalue), slash_pos + 1);
            reverse_zend_string_slash(real_file_path);
            is_file(ZSTR_VAL(real_file_path));
            zend_try {
                require_php_file(ZSTR_VAL(real_file_path));
            } zend_catch {
                php_request_shutdown(NULL);
            } zend_end_try();
            return ;
        }
    } ZEND_HASH_FOREACH_END();

    XAN_INFO(E_ERROR, "Class: [%s] can't be autoloaded.", ZSTR_VAL(class_name));
}/*}}}*/

/**
 * {{{
 * Recursive calling the method without the obj
 */
void recursive_call_method_without_obj(zend_class_entry *ce, zend_string *name)
{
    zval retval;
    zend_function *func = zend_hash_find_ptr(&ce->function_table, name);
    if ( !func )
        return ;
    
    if ( ce->parent ) {
        recursive_call_method_without_obj(ce->parent, name);
    }

    zend_execute( (zend_op_array *)func, &retval);
    zval_ptr_dtor(&retval);
}/*}}}*/

/* Recursive calling the mehtod  */
void recursive_call_method(zval *object, char *name, zval *retval)
{
    zval parent_obj;

    if ( Z_OBJCE_P(object)->parent ) {
        object_init_ex(&parent_obj, Z_OBJCE_P(object)->parent);
        recursive_call_method(&parent_obj, name, retval);
        zval_ptr_dtor(&parent_obj);
    }
    call_method_with_object_array(object, name, 0, NULL, retval);
    
    if ( !ZVAL_IS_NULL(retval) ) {
        if ( Z_TYPE_P(retval) == IS_STRING ) {
            php_write(Z_STRVAL_P(retval), Z_STRLEN_P(retval));
        }
        php_request_shutdown(NULL);
    }
    // if ( Z_TYPE_P(retval) == IS_FALSE ) php_request_shutdown(NULL) ;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
