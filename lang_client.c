#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    char msg[1000];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter sentence: ");
    fgets(msg, sizeof(msg), stdin);

    sendto(sock, msg, strlen(msg) + 1, 0,
           (struct sockaddr *)&server, len);

    recvfrom(sock, msg, sizeof(msg), 0,
             (struct sockaddr *)&server, &len);

    printf("Translated: %s\n", msg);

    close(sock);

    return 0;
}