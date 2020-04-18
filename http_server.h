#ifndef KHTTPD_HTTP_SERVER_H
#define KHTTPD_HTTP_SERVER_H

#include <net/sock.h>
#include "bignum.h"

struct http_server_param {
    struct socket *listen_socket;
};

extern int http_server_daemon(void *arg);

#endif
