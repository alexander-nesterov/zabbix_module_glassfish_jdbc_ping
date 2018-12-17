#define MODULE_NAME     "glassfish-jdbc.so"
#define HTTP_ACCEPT     "Accept: application/json"
#define HTTP_USERAGENT  "zabbix-agent"
#define SSL_VERIFYPEER  0
#define SSL_VERIFYHOST  0
#define URL_LENGTH      200
#define AUTH_LENGTH     100
#define REGEX_GROUP     1
#define DEBUG           0
#define SUCCESS         1
#define FAILED          0

#define GLASSFISH_PING_CONNECTION_POOL	"management/domain/resources/ping-connection-pool"

size_t write_data_callback(void *contents, size_t size, size_t nmemb, void *userp);
int curl_init(void);
void curl_set_opt(const char *fullURL, const char *user, const char *password);
const char *parse_data(char *data, const char *regex);
char *get_data();