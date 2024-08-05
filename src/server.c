#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct net_stream
{
    int io_fd;
};

int server(uint16_t port, void (*callback)(int client_fd))
{

    // create a network socket for ip/tcp
    // https://linux.die.net/man/7/tcp
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket");
        return -1;
    }

    // bind the socket to the 0.0.0.0:3000
    // https://linux.die.net/man/7/ip
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // https://linux.die.net/man/2/bind
    int r = bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (r == -1)
    {
        perror("bind");
        return -1;
    }

    // start listening on the address with max 1 connection
    // https://linux.die.net/man/2/listen
    r = listen(socket_fd, 1);
    if (r != 0)
    {
        perror("listen");
        return -1;
    }

    printf("start listening 0.0.0.0:%d\n", port);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    while (1)
    {
        // https://linux.die.net/man/2/accept
        int client_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_len);
        printf("debug: accepted connection from %s\n", inet_ntoa(client_address.sin_addr));

        // execute the defined call-back
        callback(client_fd);

        // close the connection
        close(client_fd);
    }
    // TODO: SIGTERM
};
