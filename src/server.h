#include <stdint.h>

int server(uint16_t port, void (*callback)(int client_fd));
