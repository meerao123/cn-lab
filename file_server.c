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

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&server, sizeof(server));

    printf("File server waiting...\n");

    while (1)
    {
        recvfrom(sock, filename, sizeof(filename), 0,
                 (struct sockaddr *)&client, &len);

        if (fork() == 0)
        {
            FILE *fp = fopen(filename, "r");

            if (fp == NULL)
            {
                sprintf(response, "File not found. Server PID: %d",
                        getpid());
            }
            else
            {
                sprintf(response, "Server PID: %d\n", getpid());

                int n = strlen(response);
                int bytes = fread(response + n, 1, sizeof(response) - n - 1, fp);

                response[n + bytes] = '\0';


                fclose(fp);
            }

            sendto(sock, response, strlen(response) + 1, 0,
                   (struct sockaddr *)&client, len);

            close(sock);
            return 0;
        }
    }

    close(sock);
    return 0;
}