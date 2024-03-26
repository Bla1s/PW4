#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

void send_request(const char *server_queue_path, const char *request)
{
    std::ofstream server_queue(server_queue_path);
    server_queue << request << std::endl;
    std::cout << "Wysłany request do pliku server_queue: " << request << std::endl;
}

std::string receive_response(const char *client_queue_path)
{
    std::ifstream client_queue(client_queue_path);
    std::string response_length, response;
    client_queue >> response_length >> response;
    return response;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Użycie: ./client <ID>" << std::endl;
        return 1;
    }

    int id = std::stoi(argv[1]);

    char client_queue_path[50];
    sprintf(client_queue_path, "/tmp/client_%d_queue", id);

    if (access(client_queue_path, F_OK) != -1)
    {
        unlink(client_queue_path);
    }

    mkfifo(client_queue_path, 0666);

    const char *server_queue_path = "/tmp/server_queue";

    char request[50];
    sprintf(request, "%d %s\n", id, client_queue_path);
    std::cout << "Utworzony request: " << request;

    send_request(server_queue_path, request);

    std::string response = receive_response(client_queue_path);
    std::cout << "Odpowiedź od serwera: " << response << std::endl;

    return 0;
}
