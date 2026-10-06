#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void translate(char *msg)
{
    char *shortform[] = {
        "tbh", "ig", "tbf", "atm", "irl",
        "lol", "asap", "omg", "ttyl", "idk", "nvm"
    };

    char *formal[] = {
        "to be honest", "I guess", "to be fair",
        "at the moment", "in real life",
        "laughing out loud", "as soon as possible",
        "oh my god", "talk to you later",
        "I don't know", "never mind"
    };

    char result[1000] = "";
    char *word = strtok(msg, " ");

    
    while (word != NULL)
    {
        char punctuation = '\0';
        int len = strlen(word);

        if (word[len - 1] == ',' || word[len - 1] == '.' ||
            word[len - 1] == '!' || word[len - 1] == '?')
        {
            punctuation = word[len - 1];
            word[len - 1] = '\0';
        }

        int found = 0;

        for (int i = 0; i < 11; i++)
        {
            if (strcmp(word, shortform[i]) == 0)
            {
                strcat(result, formal[i]);
                found = 1;
                break;
            }
        }

        if (!found)
            strcat(result, word);

        if (punctuation != '\0')
        {
            int n = strlen(result);
            result[n] = punctuation;
            result[n + 1] = '\0';
        }

        strcat(result, " ");
        word = strtok(NULL, " \n");
    }

    strcpy(msg, result);
}

int main()
{
    int sock;
    char msg[1000];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Server waiting...\n");

    recvfrom(sock, msg, sizeof(msg), 0,
             (struct sockaddr *)&client, &len);

    translate(msg);

    sendto(sock, msg, strlen(msg) + 1, 0,
           (struct sockaddr *)&client, len);

    close(sock);

    return 0;
}