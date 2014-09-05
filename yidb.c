/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
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

/* $Id: header 321634 2012-01-01 13:15:04Z felipe $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_yidb.h"

/* If you declare any globals in php_yidb.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(yidb)
*/

/* True global resources - no need for thread safety here */
static int le_yidb;

zend_class_entry *yidb_ce;


ZEND_BEGIN_ARG_INFO(arg_yidb_connect, 0)
    ZEND_ARG_INFO(0, ip)
    ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arg_yidb_set, 0)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO(arg_yidb_get, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arg_yidb_exptime, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO(arg_yidb_delete, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()



const zend_function_entry yidb_class_functions[] = {
    PHP_ME(Yidb, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(Yidb, __destruct,  NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
    PHP_ME(Yidb, connect,     arg_yidb_connect, ZEND_ACC_PUBLIC)
    PHP_ME(Yidb, set,     arg_yidb_set, ZEND_ACC_PUBLIC)
    PHP_ME(Yidb, get,     arg_yidb_get, ZEND_ACC_PUBLIC)
    PHP_ME(Yidb, exptime,     arg_yidb_exptime, ZEND_ACC_PUBLIC)
    PHP_ME(Yidb, delete,     arg_yidb_delete, ZEND_ACC_PUBLIC)
    PHP_FE_END /* Must be the last line in yidb_class_functions[] */
};



/* {{{ yidb_functions[]
 *
 * Every user visible function must have an entry in yidb_functions[].
 */
const zend_function_entry yidb_functions[] = {
	PHP_FE(confirm_yidb_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(yidb_test,	NULL)
	PHP_FE_END	/* Must be the last line in yidb_functions[] */
};
/* }}} */

/* {{{ yidb_module_entry
 */
zend_module_entry yidb_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"yidb",
	yidb_functions,
	PHP_MINIT(yidb),
	PHP_MSHUTDOWN(yidb),
	PHP_RINIT(yidb),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(yidb),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(yidb),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_YIDB
ZEND_GET_MODULE(yidb)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("yidb.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_yidb_globals, yidb_globals)
    STD_PHP_INI_ENTRY("yidb.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_yidb_globals, yidb_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_yidb_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_yidb_init_globals(zend_yidb_globals *yidb_globals)
{
	yidb_globals->global_value = 0;
	yidb_globals->global_string = NULL;
}
*/
/* }}} */




/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(yidb)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry yidbclass;
	INIT_CLASS_ENTRY(yidbclass, "Yidb", yidb_class_functions);
	yidb_ce = zend_register_internal_class_ex(&yidbclass, NULL, NULL TSRMLS_CC);
 
	zend_declare_property_long(yidb_ce, "_sockfd", strlen("_sockfd"),-1,ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(yidb)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(yidb)
{
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(yidb)
{
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(yidb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yidb support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_yidb_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_yidb_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "yidb", arg);
	RETURN_STRINGL(strg, len, 0);
}


PHP_FUNCTION(yidb_test){
	 
	php_printf("wo yun");
}


PHP_METHOD(Yidb, __construct) {
    //php_printf("Yidb __construct called.");
}
  
PHP_METHOD(Yidb, __destruct) {
	int sockfd;
   //php_printf("Yidb __destruct called.<br/>");
	zval *self,*sockbox;

 
	self = getThis();
	sockbox = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_sockfd"), 0 TSRMLS_CC);

	sockfd = (int)Z_LVAL_P(sockbox);
	
	if(sockfd>0){
		close(sockfd);
	}
}
  
PHP_METHOD(Yidb, connect) {
	char *arg_ip=NULL;
    int arg_ip_len;
    int arg_port;
    int sockfd;

    zval *value,*self;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &arg_ip, &arg_ip_len,&arg_port) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	int len,result;

	struct sockaddr_in address;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(arg_ip);
	address.sin_port = htons(arg_port);

	len = sizeof(address);

	result = connect(sockfd,(struct sockaddr *)&address,len);
	if(result==-1){
		php_error(E_WARNING, "Yidb: not connected");
		RETURN_FALSE;
	}

  	//shezhi
    //php_printf("ip:%s,port:%d",arg_ip,arg_port);

    self = getThis();
	MAKE_STD_ZVAL(value);
    ZVAL_LONG(value, (long)sockfd);
    //SEPARATE_ZVAL_TO_MAKE_IS_REF(&value);
    zend_update_property(Z_OBJCE_P(self), self, ZEND_STRL("_sockfd"), value TSRMLS_CC);

   	RETURN_TRUE;
}

PHP_METHOD(Yidb, set) {
    int sockfd;
	char *arg_key = NULL,*arg_value = NULL;
    int arg_key_len,arg_value_len,exptime=0;
	zval *self,*sockbox;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|l", &arg_key, &arg_key_len,&arg_value, &arg_value_len,&exptime) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	self = getThis();
	sockbox = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_sockfd"), 0 TSRMLS_CC);

	sockfd = (int)Z_LVAL_P(sockbox);
	
	if(sockfd<0){
		php_error(E_WARNING, "Yidb: not connected");
		RETURN_FALSE;
	}
 	//php_printf("set %s %d",arg_key,exptime);
	//send
	char sendStr[100];
	char buffer[1000];
	int len;
	sprintf(sendStr,"set %s %s %d",arg_key,arg_value,exptime);

 
	write(sockfd,sendStr,sizeof(sendStr));
	len=read(sockfd,buffer,sizeof(buffer));
  
 
	if(strcmp(buffer,"200")==0){
		RETURN_TRUE;
	}else{
		RETURN_FALSE;
	}


}

PHP_METHOD(Yidb, get) {
	int sockfd;
	char *arg = NULL;
    int arg_len;
	zval *self,*sockbox;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	self = getThis();
	sockbox = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_sockfd"), 0 TSRMLS_CC);

	sockfd = (int)Z_LVAL_P(sockbox);
	
	if(sockfd<0){
		php_error(E_WARNING, "Yidb: not connected");
		RETURN_FALSE;
	}

	//send recv
	char sendStr[100];
	char buffer[1000];
	memset(buffer,0,sizeof(buffer));
	int len;

	sprintf(sendStr,"get %s",arg);

	write(sockfd,sendStr,sizeof(sendStr));
	len=read(sockfd,buffer,sizeof(buffer));  
	
    //php_printf("receive from server:%s\n",buffer); 

    if(strlen(buffer)==0){
    	RETURN_FALSE; 
    }
    char *str;
    len = spprintf(&str, 0, "%s", buffer);

	RETURN_STRING(str,1);
	
	//RETURN_STRING(Z_STRVAL_P(name), 0);
}

PHP_METHOD(Yidb, exptime) {
    int sockfd;
	char *arg = NULL;
    int arg_len;
	zval *self,*sockbox;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	self = getThis();
	sockbox = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_sockfd"), 0 TSRMLS_CC);

	sockfd = (int)Z_LVAL_P(sockbox);
	
	if(sockfd<0){
		php_error(E_WARNING, "Yidb: not connected");
		RETURN_FALSE;
	}

	//send recv
	char sendStr[100];
	char buffer[1000];
	memset(buffer,0,sizeof(buffer));
	int len;

	sprintf(sendStr,"exptime %s",arg);

	write(sockfd,sendStr,sizeof(sendStr));
	len=read(sockfd,buffer,sizeof(buffer));  
	
    //php_printf("receive from server:%s\n",buffer); 

    if(strlen(buffer)==0){
    	RETURN_FALSE; 
    }
    char *str;
    len = spprintf(&str, 0, "%s", buffer);

	RETURN_STRING(str,1);
}


PHP_METHOD(Yidb, delete) {
	int sockfd;
	char *arg = NULL;
    int arg_len;
	zval *self,*sockbox;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	self = getThis();
	sockbox = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_sockfd"), 0 TSRMLS_CC);

	sockfd = (int)Z_LVAL_P(sockbox);
	
	if(sockfd<0){
		php_error(E_WARNING, "Yidb: not connected");
		RETURN_FALSE;
	}

	//send recv
	char sendStr[100];
	char buffer[1000];
	memset(buffer,0,sizeof(buffer));
	int len;

	sprintf(sendStr,"delete %s",arg);

	write(sockfd,sendStr,sizeof(sendStr));
	len=read(sockfd,buffer,sizeof(buffer));  
	
    //php_printf("receive from server:%s\n",buffer); 

    if(strcmp(buffer,"200")==0){
		RETURN_TRUE;
	}else{
		RETURN_FALSE;
	}
	
	//RETURN_STRING(Z_STRVAL_P(name), 0);
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
