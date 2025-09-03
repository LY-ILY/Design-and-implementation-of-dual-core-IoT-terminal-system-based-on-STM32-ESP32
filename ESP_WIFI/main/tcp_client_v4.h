#ifndef __TCP_CLIENT_V4_H__
#define __TCP_CLIENT_V4_H__

#include "sdkconfig.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>            // struct addrinfo
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_log.h"

#define HOST_IP_ADDR "192.168.84.54"//需要链接的目标ip
#define PORT 8020//需要链接的目标端口
/*配置参数*/

void TCP_Client_Init(void);
void my_TCP_send_recv(char data[]);
void TCP_close(void);

#endif
