/*************************************************
 *Author*        :wayne
 *Description*   :配置信息文件
 *Created Time*  : Wed 08 Apr 2020 11:21:22 AM CST

**************************************************/

#ifndef UTIL_H
#define UTIL_H

#define PATHLEN 128
#define LISTENQ 1024
#define BUFLEN 8192
#define DELIM "="

#define CONF_OK 0
#define CONF_ERROR -1

#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct conf
{
		char root[PATHLEN];
		int port;
		int thread_num;
}conf_t;

int read_conf(char* filename, conf_t* conf);
void handle_for_sigpipe();
int socket_bind_listen(int port);
int make_socket_non_blocking(int fd);
void accept_connection(int listen_fd, int epoll_fd, char* path);

#endif
