#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main()
{
    int sock;
    char msg[100];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Time server waiting...\n");

    while (1)
    {
        recvfrom(sock, msg, sizeof(msg), 0,
                 (struct sockaddr *)&client, &len);

        if (fork() == 0)
        {
            time_t t = time(NULL);
            char *current_time = ctime(&t);

            sendto(sock, current_time, strlen(current_time) + 1, 0,
                   (struct sockaddr *)&client, len);

            close(sock);
            return 0;
        }
    }

    close(sock);
    return 0;
}