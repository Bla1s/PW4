#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

#define MAX_NAME_LEN 20

struct Character
{
    int id;
    char name[MAX_NAME_LEN];
};

int main()
{
    int server_fd, client_fd;
    char server_queue[] = "server_queue";
    char client_queue[20];
    int id;
    struct Character character;

    sprintf(client_queue, "/tmp/client_queue_%d", getpid());
    mkfifo(client_queue, 0666);

    server_fd = open(server_queue, O_WRONLY);
    if (server_fd == -1)
    {
        perror("Error in opening server queue");
        exit(EXIT_FAILURE);
    }

    std::cout << "Podaj ID postaci: ";
    std::cin >> id;
    write(server_fd, &id, sizeof(int));
    write(server_fd, client_queue, sizeof(client_queue));

    client_fd = open(client_queue, O_RDONLY);
    if (client_fd == -1)
    {
        perror("Error in opening client queue");
        exit(EXIT_FAILURE);
    }

    read(client_fd, &character, sizeof(struct Character));
    if (strcmp(character.name, "Nie ma") == 0)
    {
        std::cout << "Nie ma postaci o podanym ID." << std::endl;
    }
    else
    {
        std::cout << "Nazwisko postaci o podanym ID: " << character.name << std::endl;
    }

    close(server_fd);
    close(client_fd);
    unlink(client_queue);

    return 0;
}
