// SPDX-License-Identifier: MIT
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/termios.h>

#include "ews_config.h"
#include "ews.h"


extern const uint8_t server_private_der[];
extern const uint32_t server_private_der_len;
extern const uint8_t server_public_der[];
extern const uint32_t server_public_der_len;

static ews_t *ews;
static struct termios termios;
static void ws_loop(ews_ws_t *ws);

static void sigint(int signum)
{
    if (signum == 0) {
        ews_destroy(ews);
    }

    termios.c_lflag |= ECHO | ICANON;
    tcsetattr(fileno(stdin), 0, &termios);
}

int main(int argc, char *argv[])
{
    ews_config_t config = {
#if CONFIG_EWS_HTTP_CLIENTS > 0
        .http_listen_port = 8080,
#endif

#if CONFIG_EWS_HTTPS_CLIENTS > 0
        .https_listen_port = 8443,
        .https_crt = server_public_der,
        .https_crt_len = server_public_der_len,
        .https_pk = server_private_der,
        .https_pk_len = server_private_der_len,
#endif
    };

    ews = ews_init(&config);
    ews_routes_append(ews, "/test", ews_routes_test_handler, 0);
    ews_routes_append(ews, "/*", ews_ws_route_handler, 1, ws_loop);
    ews_routes_append(ews, "/*", ews_routes_stdio_get_handler, 1, "../html");

    struct sigaction action = { 0 };
    memset(&action, 0, sizeof(action));
    action.sa_handler = &sigint;
    sigaction(SIGINT, &action, NULL);

    tcgetattr(fileno(stdin), &termios);
    termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(fileno(stdin), 0, &termios);

    getchar();
    sigint(0);

    exit(EXIT_SUCCESS);
}

static void ws_loop(ews_ws_t *ws)
{
    char buf[1024];
    int len;
    int flags;

    while (true) {
        len = ws->ops->recv(ws, &flags, buf, sizeof(buf));
        if (len < 0) {
            break;
        }
        ws->ops->send(ws, 0, buf, len);
    }
}
