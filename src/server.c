#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <signal.h>

#include "server.h"

int socket_fd;

void handle_server_sigint(int sig)
{
    printf("\n");
    printf("shutdown server\n");

    // shutdown the socket but allow response to be completed
    int result = shutdown(socket_fd, SHUT_RD);
    if (result == -1)
    {
        perror("shutdown");
    }
}

int server(uint16_t port, void (*callback)(int client_fd))
{
    int result;
    sighandler_t old_sigint_handler;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // create a network socket for ip/tcp
    // https://linux.die.net/man/7/tcp
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket");
        return -1;
    }

    // bind the socket to the 0.0.0.0:3000
    // https://linux.die.net/man/7/ip
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // https://linux.die.net/man/2/bind
    result = bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (result == -1)
    {
        perror("bind");
        close(socket_fd);
        return -1;
    }

    // handle sigint to gracefully close the server
    // https://linux.die.net/man/2/sigaction
    old_sigint_handler = signal(SIGINT, &handle_server_sigint);

    // start listening on the address with max 1 connection
    // https://linux.die.net/man/2/listen
    result = listen(socket_fd, 1);
    if (result != 0)
    {
        perror("listen");
        signal(SIGINT, old_sigint_handler);
        close(socket_fd);
        return -1;
    }
    printf("start listening 0.0.0.0:%d\n", port);

    while (1)
    {
        // https://linux.die.net/man/2/accept
        int client_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_fd == -1)
        {
            if (errno == EINVAL)
            {
                // socket was shutdown
                break;
            }

            // unknown error
            perror("accept");
            signal(SIGINT, old_sigint_handler);
            close(socket_fd);
            return -1;
        }

        printf("debug: accepted connection from %s\n", inet_ntoa(client_address.sin_addr));

        // execute the defined call-back
        callback(client_fd);

        // close the connection
        close(client_fd);
    }

    signal(SIGINT, old_sigint_handler);
    close(socket_fd);
    return 0;
}
