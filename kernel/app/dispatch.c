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
#include "kernel/xan_class.h"
#include "ext/standard/info.h"
#include "kernel/loader/loader.h"
#include "kernel/class/aop/proxy.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"

/**
 * {{{
 * Dispatch the url to the different module & controller & action
 */
void xan_dispatch_url(zend_string *url)
{
    /* Some variables */
    char status = 0;
    zend_class_entry *controller_class;
    zval *module_name, controller_obj, retval, temp_url_array;
    zend_string *module_path, *controller_path, *controller_class_name, *action_name;

    /* If passing the url to parsing the url to obtain the module & controller & action */
    if ( url && ZSTR_LEN(url) ) {

        int i = 0, j;
        zval *key, *value;
        array_init(&temp_url_array);

        /* If user explicitly use the urlSuffix, then we must keep the url end with the given suffix */
        if (XAN_G(must_url_suffix)) {
            char *dot_pos = strrchr(ZSTR_VAL(url), '.');
            if (!dot_pos)
                XAN_INFO(E_ERROR, "The url must be ended with : `%s`!", Z_STRVAL(XAN_G(url_suffix)));
            
            if ( (Z_STRLEN(XAN_G(url_suffix)) != strlen(dot_pos + 1) )
                || ( memcmp(Z_STRVAL(XAN_G(url_suffix)), dot_pos + 1, Z_STRLEN(XAN_G(url_suffix))) != 0 ) ) {
                zend_array_destroy(Z_ARRVAL(temp_url_array));
                XAN_INFO(E_ERROR, "Url wrong!");
            }

            /* To pass the new url to the engine. */
            url = zend_string_init(ZSTR_VAL(url), dot_pos - ZSTR_VAL(url), 0);
        }

        url = php_trim(url, XAN_STRL("/"), 3 );
        php_explode(strpprintf(0, "%s", "/"), url, &temp_url_array, ZEND_LONG_MAX);

        /* Module */
        zval *t_m = zend_hash_index_find(Z_ARRVAL(temp_url_array), i);
        if ( t_m ) {
            ZVAL_COPY(&XAN_G(default_module), t_m);
            zend_hash_index_del(Z_ARRVAL(temp_url_array), i);
            i++;
        }

        /* Controller */
        zval *t_c = zend_hash_index_find(Z_ARRVAL(temp_url_array), i);
        if ( t_c ) {
            ZVAL_COPY(&XAN_G(default_controller), t_c);
            zend_hash_index_del(Z_ARRVAL(temp_url_array), i);
            i++;
        }

        /* Action */
        zval *t_a = zend_hash_index_find(Z_ARRVAL(temp_url_array), i);
        if ( t_a ) {
            ZVAL_COPY(&XAN_G(default_action), t_a);
            zend_hash_index_del(Z_ARRVAL(temp_url_array), i);
            i++;
        }

        zval *global_gets = xan_get_get_vars(NULL);

        /* if have other path variables put it into the $_GET */
        for (j = i; j < zend_hash_num_elements(Z_ARRVAL(temp_url_array)) + i; j++) {
            key = zend_hash_index_find(Z_ARRVAL(temp_url_array), j++);
            if ( !key ) break;
            value = zend_hash_index_find(Z_ARRVAL(temp_url_array), j);
            if ( !value )
                add_assoc_null(global_gets, Z_STRVAL_P(key));
            else
                add_assoc_zval(global_gets, Z_STRVAL_P(key), value);
        }

        /* if using the GET mode to parsing the router, unset the get string */
        if ( XAN_G(url_pattern) == 1 )
            zend_hash_str_del(Z_ARRVAL_P(global_gets), Z_STRVAL(XAN_G(url_get_str)), Z_STRLEN(XAN_G(url_get_str)) );

        /* You can add some other logic to do what you want to do the job. */
    }

    /* Using the module & controller & action to do the job */
    /* 1. to judge the module was allowed or not to access, if not, dispaly the error info */
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL(XAN_G(allow_modules)), module_name) {
        if ( strncasecmp(Z_STRVAL(XAN_G(default_module)), Z_STRVAL_P(module_name), Z_STRLEN_P(module_name)) == 0 ) {
            status = 1; break;
        }
    } ZEND_HASH_FOREACH_END();
    if ( !status )
        XAN_INFO(E_ERROR, "Module `%s` are not allowed to access!", Z_STRVAL(XAN_G(default_module)));

    /* Find the module & controller & action was exists and render the view if auto render */
    module_path = strpprintf( 0, "%s/modules/%s", Z_STRVAL(XAN_G(application_dir)), Z_STRVAL(XAN_G(default_module)) );
    is_file(ZSTR_VAL(module_path));

    /*Controller exists or not.*/
    Z_STRVAL(XAN_G(default_controller))[0] = toupper(Z_STRVAL(XAN_G(default_controller))[0]);
    controller_path = strpprintf( 0, "%s/controllers/%s.php", ZSTR_VAL(module_path), Z_STRVAL(XAN_G(default_controller)) );
    is_file(ZSTR_VAL(controller_path));
    /* Z_STRVAL(XAN_G(default_controller))[0] = tolower(Z_STRVAL(XAN_G(default_controller))[0]); */

    /* After requiring the controller file, find the controller class exists or not. */
    controller_class_name = strpprintf( 
        0, "%s\\modules\\%s\\controllers\\%s", Z_STRVAL(XAN_G(default_namespace)), 
        Z_STRVAL(XAN_G(default_module)), Z_STRVAL(XAN_G(default_controller)) 
    );

class_entry:
    /* To find the controller class_entry */
    controller_class = zend_hash_find_ptr( CG(class_table), zend_string_tolower(controller_class_name) );
    if ( !controller_class ) {
        /*Require the Controller file*/
        require_php_file(ZSTR_VAL(controller_path));
        /* Do the Annotation parsing */
        init_class_with_annotations(controller_class_name, &XAN_G(aliases));

        /* If required the php file */
        goto class_entry;
    }

    /* Calling the init function */
    recursive_call_method_without_obj(controller_class, strpprintf(0, "%s", "init"));

    if (EG(exception)) zend_exception_error(EG(exception), E_ERROR);

    /* get_object_from_di(&XAN_G(class_di), controller_class_name, &controller_obj, controller_class); */
    action_name = strpprintf(0, "%sAction", Z_STRVAL(XAN_G(default_action)));

    /* After founding the controller class do the annotation job */
    if ( ZVAL_IS_NULL(&XAN_G(call_chain)) )
        array_init(&XAN_G(call_chain));

    /* Call function with AOP think. */
    call_annotation_function(NULL, controller_class_name, action_name, NULL, &retval);

    /* If in auto render mode, to auto render the view */
    if ( XAN_G(auto_render) ) {
        zval view_obj, t_ret;
        zend_class_entry *view_ce = zend_hash_str_find_ptr(CG(class_table), XAN_STRL("xan\\view"));
        get_object_from_di(&XAN_G(class_di), strpprintf(0, "%s", "view"), &view_obj, view_ce);
        if (Z_TYPE_P(&view_obj) == IS_OBJECT) {
            zend_call_method_with_0_params(&view_obj, view_ce, NULL, ZEND_CONSTRUCTOR_FUNC_NAME, &t_ret);
            zend_call_method_with_1_params( &view_obj, view_ce, NULL, "render", &t_ret, &XAN_G(default_action) );
        }
        zval_ptr_dtor(&t_ret);
    }

    /* If using the url_suffix, remember to release the url memory from the zend_string_init function call */
    if ( XAN_G(must_url_suffix) && url ) {
        
        zend_string_release(url);
        /* After finishing job destroy the array */
        zend_array_destroy(Z_ARRVAL(temp_url_array));
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