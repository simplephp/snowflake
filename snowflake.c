/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_snowflake.h"
#include "snowflake_lib.h"

/* If you declare any globals in php_snowflake.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(snowflake)
*/
ZEND_DECLARE_MODULE_GLOBALS(snowflake)

/* True global resources - no need for thread safety here */
static int le_snowflake;

/* {{{ snowflake_functions[]
 *
 * Every user visible function must have an entry in snowflake_functions[].
 */
const zend_function_entry snowflake_functions[] = {
	PHP_FE(uuid,	NULL)
	PHP_FE_END	/* Must be the last line in snowflake_functions[] */
};
/* }}} */

/* {{{ snowflake_module_entry
 */
zend_module_entry snowflake_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"snowflake",
	snowflake_functions,
	PHP_MINIT(snowflake),
	PHP_MSHUTDOWN(snowflake),
    NULL,
    NULL,
	PHP_MINFO(snowflake),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SNOWFLAKE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SNOWFLAKE
ZEND_GET_MODULE(snowflake)
#endif

/* {{{ PHP_INI */

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("snowflake.work_id",  "0", ZEND_INI_SYSTEM, OnUpdateLong, work_id, zend_snowflake_globals, snowflake_globals)
PHP_INI_END()

/* }}} */

/* {{{ php_snowflake_init_globals
 */

static void php_snowflake_init_globals(zend_snowflake_globals *snowflake_globals)
{
	snowflake_globals->work_id = 0;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(snowflake)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
    REGISTER_INI_ENTRIES();
    int ret = SUCCESS;

    if (SNOWFLAKE_G(work_id) <= 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "\"snowflake.work_id\" must be set greater than zero");
        ret = FAILURE;
    }
    if (SNOWFLAKE_G(work_id) >= WORKID_WIDTH) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "\"snowflake.work_id\" must be set smaller than %d", WORKID_WIDTH);
        ret = FAILURE;
    }
	return ret;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(snowflake)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
    UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(snowflake)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(snowflake)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(snowflake)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "snowflake support", "enabled");
    php_info_print_table_row(2, "version", PHP_SNOWFLAKE_VERSION);
    php_info_print_table_row(2, "author", AUTHOR);
    php_info_print_table_row(2, "email",EMAIL);
	php_info_print_table_end();
    DISPLAY_INI_ENTRIES();
	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto int uuid() */
PHP_FUNCTION(uuid)
{
    TSRMLS_FETCH();
    set_workid(100);
    int64_t ret = get_unique_id();
    if (ret) {
        RETURN_LONG(ret);
    } else {
        RETURN_BOOL(0);
    }
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
