#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"

/**
 * Read the HTTP request form the client file descriptor in the http_request object
 *  and prepare the http_response object.
 *
 * This method only support http/1.0, in case the request is a http/0.9 it will fail.
 *
 * Return 0 if the request was successfully parsed otherwise return -1 on error.
 *
 * TODO: Handle limits for request line, headers and headers number
 * TODO: Store the request in request object that can be forwarded
 * TODO: Handle and return errors as HTTP errors
 *
 */
int parse_http_request(int client_fd)
{

    FILE *request = fdopen(client_fd, "r");

    // Parse Request-Line
    char *request_line = NULL;
    size_t zero_len = 0;
    ssize_t read_n;

    // Note: a huge request that doesn't have a new line will blow up the serer
    // memory, so this method is unsafe.
    read_n = getline(&request_line, &zero_len, request);
    if (read_n == -1)
    {
        perror("request_line: getline");
        return -1;
    }

    // TODO: Because method remains the same pointer as request_line and request_line now
    //  contains null value, if we free request_line method, request_uri and http_version
    //  will point to random or reusable space in memory. To avoid this issue we need to copy
    //  each value in a new space in memory before we can free the request_line.
    char *method = strtok(request_line, " ");
    char *request_uri = strtok(NULL, " ");
    char *http_version = strtok(NULL, " ");

    // Parse Headers

    // We are pre-allocating 32 potential headers of undefined length.
    int headers_max_len = 32;
    char **headers = malloc(headers_max_len * sizeof(char *));
    int headers_len = 0;
    while (1)
    {
        char *header_line = NULL;
        size_t zero_len = 0;
        ssize_t read_n;

        read_n = getline(&header_line, &zero_len, request);
        if (read_n == -1)
        {
            perror("header_line: getline");
            return -1;
        }

        if (read_n == 2 && strcmp(header_line, "\r\n") == 0)
        {
            // An empty line divides the Headers from the Body
            break;
        }

        // If we reached the max len of the pre-allocated headers array
        // we resize it and add 32 more entries
        if (headers_len >= headers_max_len)
        {
            headers_max_len += 32;
            headers = realloc(headers, headers_max_len);
            if (headers == NULL)
            {
                perror("headers: realloc");
                return -1;
            }
        }
        headers[headers_len] = header_line;
        headers_len++;
    }

    // Let's print everything for testing
    printf("method: %s\n", method);
    printf("uri: %s\n", request_uri);
    printf("version: %s\n", http_version);
    for (int i = 0; i < headers_len; i++)
    {
        printf("header(%d): %s", i, headers[i]);
    }
    return 0;
}

void handle_client_connection(int client_fd)
{
    printf("start parsing request\n");

    parse_http_request(client_fd);

    char message[] = "HTTP/1.0 200 OK\r\n";
    int r = write(client_fd, message, sizeof(message));
    if (r == -1)
    {
        perror("write");
        return;
    }
}

int main(void)
{

    int r = server(3004u, handle_client_connection);
    if (r == -1)
    {
        return 1;
    }
    return 0;
}
