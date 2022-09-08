#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define assert_ok(e) ({int x = (e); if (x < 0) { printf("%s:%d: ", __FILE__, __LINE__); fflush(stdout); perror(#e); abort(); } x;})

int main() {
    char *host = "127.0.0.1";
    uint16_t port = 8000;
    int reuseaddr = 1;
    int listen_backlog = 10;

    struct sockaddr_in server_address = {
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(host),
        .sin_family = AF_INET
    };
    int sock = assert_ok(socket(AF_INET, SOCK_STREAM, IPPROTO_IP));
    assert_ok(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof reuseaddr));
    assert_ok(bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)));
    assert_ok(listen(sock, listen_backlog));

    printf("Listening on http://%s:%hu\n", host, port);
    while (1) {
        int accepted_sock = assert_ok(accept(sock, NULL, NULL));
        write(accepted_sock, "HTTP/1.1 404 OK\nContent-Length: 12\n\nHello world!", 48);
    }
}
