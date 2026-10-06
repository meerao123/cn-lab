#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    char filename[100];
    char response[10000];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter filename: ");
    scanf("%s", filename);

    sendto(sock, filename, strlen(filename) + 1, 0,
           (struct sockaddr *)&server, len);

    recvfrom(sock, response, sizeof(response), 0,
             (struct sockaddr *)&server, &len);

    printf("\n%s\n", response);

    close(sock);

    return 0;
}