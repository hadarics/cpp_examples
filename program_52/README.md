# # TCP/IP alapú kliens mintapélda
* [Source](#source)
* [Description](#description)

## Source
```cpp
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

const unsigned short port=5678;
const unsigned max_buffer_size = 128;
static const char *server_ip = "192.168.4.10";

void error(const char* msg) {
    cerr << msg << endl;
}

[[noreturn]] void error_and_exit(const char* msg) {
    error(msg);
    exit (1);
}

int main()
{
    int client_fd = 0;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)     {
        error_and_exit("Error creating client socket");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);

    if (connect(client_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        error_and_exit("Error creating client socket");
    }

    cout << "Available commands: " << endl;
    //cout << " :squit -- server quit " << endl;
    cout << " :quit -- client quit " << endl;
    cout << " placename -- query place based on name " << endl;

    char in_message [max_buffer_size];
    char out_message [max_buffer_size];
    char message [max_buffer_size];
    memset(in_message, 0, max_buffer_size);
    memset(out_message, 0, max_buffer_size);

    cout << "Type the command: ";

    while((cin >> out_message)) {
        memset(message, 0, sizeof (message));
        strcat(message, out_message);
        //strcat(message, "\n");

        if (send(client_fd, message, strlen (message)+1, 0) == -1) error_and_exit("Error in send");

        if (string(out_message) == ":quit") break;

        if (recv(client_fd, in_message, max_buffer_size,0) == -1) {
            error_and_exit ("Client: recv");
        }
        else {
            cout << string("Message received: ") + string (in_message) << endl;
        }
        if (string(in_message) == ":quit") break;
            cout << "Type the command: ";

        memset(in_message, 0, max_buffer_size);
        memset(out_message, 0, max_buffer_size);
    }

    close(client_fd);
    return 0;
}
```

## Description


> A működéshez szükséges a program_51-ben, vagy a program_53-ban bemutatott szerver futtatása.

