#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

#define MAX_CLIENTS 10
#define MAX_NAME_LEN 20

struct Character
{
    int id;
    char name[MAX_NAME_LEN];
};

struct Character database[] = {
    {1, "Kowalski"},
    {2, "Nowak"},
    {3, "Wiśniewski"} {4, "Markiewicz"}};

int main()
{
    int server_fd, client_fd;
    char client_queue[20];
    int id;
    struct Character character;

    mkfifo("server_queue", 0666);

    while (true)
    {
        server_fd = open("server_queue", O_RDONLY);
        if (server_fd == -1)
        {
            perror("Error in opening server queue");
            exit(EXIT_FAILURE);
        }

        read(server_fd, &id, sizeof(int));
        read(server_fd, client_queue, sizeof(client_queue));

        int i;
        for (i = 0; i < sizeof(database) / sizeof(database[0]); i++)
        {
            if (database[i].id == id)
            {
                character = database[i];
                break;
            }
        }

        client_fd = open(client_queue, O_WRONLY);
        if (client_fd == -1)
        {
            perror("Error in opening client queue");
            exit(EXIT_FAILURE);
        }

        if (i < sizeof(database) / sizeof(database[0]))
        {
            write(client_fd, &character, sizeof(struct Character));
        }
        else
        {
            char not_found[] = "Nie ma";
            write(client_fd, not_found, sizeof(not_found));
        }

        close(server_fd);
        close(client_fd);
    }

    return 0;
}
