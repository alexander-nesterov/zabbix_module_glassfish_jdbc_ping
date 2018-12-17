#include "sysinc.h"
#include "module.h"
#include "common.h"
#include "log.h"
#include "version.h"
#include <curl/curl.h>
#include <pcre.h>
#include "glassfish-jdbc.h"

static int zbx_module_glassfish_ping_connection_pool(AGENT_REQUEST *request, AGENT_RESULT *result);

static ZBX_METRIC keys[] =
/*          KEY                          FLAG                   FUNCTION                   TEST PARAMETERS */
{
    {"glassfish.ping.connection.pool", CF_HAVEPARAMS, zbx_module_glassfish_ping_connection_pool, NULL},
    {NULL}
};

/******************************************************************************
*                                                                            *
* Function: zbx_module_api_version                                           *
*                                                                            *
* Purpose: returns version number of the module interface                    *
*                                                                            *
* Return value: ZBX_MODULE_API_VERSION - version of module.h module is       *
*               compiled with, in order to load module successfully Zabbix   *
*               MUST be compiled with the same version of this header file   *
*                                                                            *
******************************************************************************/
int zbx_module_api_version(void)	
{
    return ZBX_MODULE_API_VERSION;
}

/******************************************************************************
*                                                                            *
* Function: zbx_module_init                                                  *	
*                                                                            *	
* Purpose: the function is called on agent startup                           *	
*          It should be used to call any initialization routines             *	
*                                                                            *	
* Return value: ZBX_MODULE_OK - success                                      *	
*               ZBX_MODULE_FAIL - module initialization failed               *	
*                                                                            *	
* Comment: the module won't be loaded in case of ZBX_MODULE_FAIL             *
*                                                                            *	
******************************************************************************/
int zbx_module_init(void)
{
    srand(time(NULL));

    zabbix_log(LOG_LEVEL_INFORMATION, 
               "Module: %s - build with agent: %d.%d.%d; libcurl: %s; pcre: %d.%d (%s:%d)",
               MODULE_NAME, ZABBIX_VERSION_MAJOR, ZABBIX_VERSION_MINOR, ZABBIX_VERSION_PATCH, 
               LIBCURL_VERSION, PCRE_MAJOR, PCRE_MINOR,
               __FILE__, __LINE__);

    return ZBX_MODULE_OK;
}

/******************************************************************************
*                                                                            *
* Function: zbx_module_uninit                                                *
*                                                                            *
* Purpose: the function is called on agent shutdown                          *
*          It should be used to cleanup used resources if there are any      *
*                                                                            *
* Return value: ZBX_MODULE_OK - success                                      *
*               ZBX_MODULE_FAIL - function failed                            *
*                                                                            *
******************************************************************************/
int zbx_module_uninit(void)
{
    return ZBX_MODULE_OK;
}

/******************************************************************************
*                                                                            *
* Function: zbx_module_item_list                                             *
*                                                                            *
* Purpose: returns list of item keys supported by the module                 *
*                                                                            *
* Return value: list of item keys                                            *
*                                                                            *
******************************************************************************/
ZBX_METRIC *zbx_module_item_list()
{
    return keys;
}

/******************************************************************************
*                                                                            *
* Function: zbx_module_glassfish_ping_connection_pool                        *
*                                                                            *
* Purpose:                                                                   *
*                                                                            *
* Return value: FAILURE - 0, SUCCESS - 1                                     *
*                                                                            *
* Example: glassfish.ping.connection.pool["https://{HOST.CONN}", 8888,       *
* "pool", "exit_code.:.(\w+).,", "user", "password"]                         *
*                                                                            *
* Utility: asadmin -u user -p port ping-connection-pool pool_name            *
*                                                                            *
******************************************************************************/
static int zbx_module_glassfish_ping_connection_pool(AGENT_REQUEST *request, AGENT_RESULT *result)
{
    char *data;
    const char *dataRes;
    int res;
    int value;

    zabbix_log(LOG_LEVEL_DEBUG, 
               "Module: %s - param num: %d (%s:%d)", 
               MODULE_NAME, request->nparam, __FILE__, __LINE__);

    if (6 != request->nparam)
    {
        SET_MSG_RESULT(result, strdup("Invalid number of parameters"));

        zabbix_log(LOG_LEVEL_DEBUG, "Error in module: %s - invalid number of parameters (%s:%d)", 
                   MODULE_NAME, __FILE__, __LINE__);

        return SYSINFO_RET_FAIL;
    }

    res = curl_init();

    if (res != CURLE_OK)
    {
        SET_MSG_RESULT(result, strdup("Error initilization libcurl"));

        zabbix_log(LOG_LEVEL_DEBUG, 
                   "Error in module: %s - could not initilization libcurl (%s:%d)", 
                   MODULE_NAME, __FILE__, __LINE__);

        return SYSINFO_RET_FAIL;
    }

    char *host = get_rparam(request, 0);
    char *port = get_rparam(request, 1);
    char *namePool = get_rparam(request, 2);
    char *regex = get_rparam(request, 3);
    char *user = get_rparam(request, 4);
    char *password = get_rparam(request, 5);

    char fullURL[URL_LENGTH];

    zbx_snprintf(fullURL, URL_LENGTH, "%s:%s/%s/?appname=&id=%s&modulename=&targetName=&__remove_empty_entries__=true", 
                 host, port, GLASSFISH_PING_CONNECTION_POOL, namePool);

    curl_set_opt(fullURL, user, password);

    data = get_data();

    zabbix_log(LOG_LEVEL_DEBUG, 
               "Module: %s - raw data: %s (%s:%d)", 
               MODULE_NAME, data, __FILE__, __LINE__);

    dataRes = parse_data(data, regex);

    zabbix_log(LOG_LEVEL_DEBUG, 
               "Module: %s - parse data: %s (%s:%d)", 
               MODULE_NAME, dataRes, __FILE__, __LINE__);

    zbx_free(data);

    if (dataRes == NULL)
    {
        SET_MSG_RESULT(result, strdup("Result is empty"));

        zabbix_log(LOG_LEVEL_DEBUG, 
                   "Error in module: %s - result is empty (%s:%d)", 
                   MODULE_NAME, __FILE__, __LINE__);

        return SYSINFO_RET_FAIL;
    }

    if (strcmp(dataRes, "SUCCESS") == 0)
        value = SUCCESS;
    else
        value = FAILED;

    SET_UI64_RESULT(result, value);
    return SYSINFO_RET_OK;
}