#ifndef LAB0_TINY_H
#define LAB0_TINY_H

#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>

/* Global variable*/
int listen_fd;
bool noise;

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 9999 /* use this port if none given as arg to main() */
#endif

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;

typedef struct {
    char filename[512];
    off_t offset; /* for support Range */
    size_t end;
} http_request;

char *process(int fd, struct sockaddr_in *clientaddr);
int open_listenfd(int port);

#endif /* LAB0_TINY_H */