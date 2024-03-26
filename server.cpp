#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <map>
#include <sstream>

void handle_sighup(int signum)
{
    std::cout << "Received SIGHUP signal. Continuing execution." << std::endl;
}

void handle_sigterm(int signum)
{
    std::cout << "Received SIGTERM signal. Exiting." << std::endl;
    exit(EXIT_SUCCESS);
}

void handle_sigusr1(int signum)
{
    std::cout << "Received SIGUSR1 signal. Exiting." << std::endl;
    exit(EXIT_SUCCESS);
}

void send_response(const char *client_queue_path, const std::string &response)
{
    std::ofstream client_queue(client_queue_path);
    client_queue << response.length() << ' ' << response;
}

void handle_client(const std::string &request)
{
    int id;
    std::string client_queue_path;
    std::istringstream iss(request);
    iss >> id >> client_queue_path;

    static std::map<int, std::string> database = {
        {1, "Malinowski"},
        {2, "Iksiński"},
        {3, "Markiewicz"},
        {4, "Wiśniewski"},
        {5, "Czaja"}};
    std::string response = (database.find(id) != database.end()) ? database[id] : "Nie ma";

    send_response(client_queue_path.c_str(), response);
}

void handle_clients(const char *server_queue_path)
{
    while (true)
    {
        std::ifstream server_queue(server_queue_path);
        std::string request;
        while (std::getline(server_queue, request))
        {
            std::cout << "Received request: " << request << std::endl;
            handle_client(request);
        }
        server_queue.close();
        sleep(3);
    }
}

int main()
{
    signal(SIGHUP, handle_sighup);
    signal(SIGTERM, handle_sigterm);
    signal(SIGUSR1, handle_sigusr1);

    const char *server_queue_path = "/tmp/server_queue";

    if (access(server_queue_path, F_OK) != -1)
    {
        unlink(server_queue_path);
    }

    mkfifo(server_queue_path, 0666);

    handle_clients(server_queue_path);

    return 0;
}
