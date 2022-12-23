#include <iostream>
#include <map>
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

using namespace std;

const unsigned short port=5678;
const unsigned max_buffer_size = 128;

void error(const char* msg) {
    cerr << msg << endl;
}

[[noreturn]] void error_and_exit(const char* msg) {
    error(msg);
    exit (1);
}

#define TCP_CONNECTION_OK 1
#define TCP_CONNECTION_ERROR 0
#define GENERAL_SOCKET_ERROR -1

struct ClientInfo {
        int fd;
        uint32_t ipv4;
        int status;
};

//void signal_handler(int);
void log(const char* msg, ClientInfo *c);
void thread_function(ClientInfo c, shared_ptr<map <string, unsigned>> p);

int main() {

    int server_fd;

    map <string, unsigned> population;
    population.insert(pair <string, unsigned short> ("Aba", 4674));
    population.insert(pair <string, unsigned short> ("Dunaújváros", 48104));
    population.insert(pair <string, unsigned short> ("Paks", 19625));

    shared_ptr<map <string, unsigned>> sp = make_shared<map <string, unsigned>>(population);

    if ((server_fd = socket(AF_INET,SOCK_STREAM, 0)) == GENERAL_SOCKET_ERROR)
        error_and_exit("Error creating server socket");

    sockaddr_in server_addr;
    unsigned int optval = 1;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == GENERAL_SOCKET_ERROR)
        error_and_exit("Error in setsockopt");

    if (bind(server_fd, reinterpret_cast<struct sockaddr*> (&server_addr), static_cast<socklen_t> (sizeof(struct sockaddr))) == GENERAL_SOCKET_ERROR)
        error_and_exit("Error in bind");

    if (listen(server_fd, 8) == GENERAL_SOCKET_ERROR)
        error_and_exit("Error in bind");

    cout << "Server started: " << endl;

    struct ClientInfo clientInfo;

    while (1) {
        sockaddr_in client_addr;
        socklen_t size = static_cast<socklen_t> (sizeof(struct sockaddr));

        clientInfo.fd = accept(server_fd, reinterpret_cast<struct sockaddr*> (&client_addr), &size);
        clientInfo.ipv4 =  client_addr.sin_addr.s_addr;
        clientInfo.status=(clientInfo.fd == -1)?TCP_CONNECTION_ERROR:TCP_CONNECTION_OK;

        if (clientInfo.status==TCP_CONNECTION_OK) {
            log("Client connected", &clientInfo);
            thread t(thread_function, clientInfo, sp);
            t.detach();
        }
    }
}

void threat_function(ClientInfo c, shared_ptr<map <string, unsigned>> p) {

    char in_message [max_buffer_size];
    char out_message [max_buffer_size];

    ssize_t bytes;
    bzero(&in_message,sizeof (in_message));

    while((bytes=recv(c.fd, in_message, max_buffer_size,0))) {

        string in_msg = in_message;
        bzero(&out_message,max_buffer_size);
        bzero(&in_message,max_buffer_size);

        //cout << "::" << in_msg << "::" << endl;

        map <string, unsigned>::iterator it = p->find(in_msg);
        if (it != p->end()) {
           strcpy(out_message, to_string(it->second).c_str());
        }
        else {
           strcpy(out_message,"not found");
        }

        send(c.fd, out_message, strlen (out_message), 0);
    }
    log("Client disconnected", &c);
}

void log(const char* msg, ClientInfo *c) {
    cout << msg << ": " << c->fd <<  ", ";
    char ipstr[16];
    unsigned char buf[sizeof(c->ipv4)];
    memcpy(buf, &(c->ipv4), sizeof(c->ipv4));
    inet_ntop(AF_INET, buf, ipstr, INET_ADDRSTRLEN);
    cout << ipstr  <<  endl;
}
