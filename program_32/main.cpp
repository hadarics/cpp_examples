#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>

using namespace std;

const int proj_id = 0xabcd;
const char mq_file[15] = "/tmp/mq_server";

struct message_text {
    int qid;
    char buf [28];
};

struct message {
    long message_type;
    struct message_text message_text;
};

[[noreturn]] void error_and_exit(const char* msg) {
    cerr << msg << endl;
    exit (1);
}

int main() {
    struct message send_message, rcvd_message;

    key_t key = ftok(mq_file,proj_id);
    if (key == -1) error_and_exit("Error in ftok");

    int cqid = msgget (key, 0);
    if (cqid < 0) error_and_exit("Error in msgget");

    send_message.message_type = 1;
    send_message.message_text.qid = cqid;

    cout << "Available commands: " << endl;
    cout << " :squit -- server quit " << endl;
    cout << " :quit -- client quit " << endl;
    cout << " placename -- query place based on name " << endl;

    cout << "Type the command: ";

    while(cin >> send_message.message_text.buf) {
        if (string(send_message.message_text.buf) == ":quit") break;

        if (msgsnd (cqid, &send_message, sizeof (struct message_text), 0) == -1) error_and_exit("Error in msgsnd");

        if (msgrcv (cqid, &rcvd_message, sizeof (struct message_text), 0, 0) == -1) {
            error_and_exit("Error in msgrcv");
        }
        else {
            cout << string("Message received: ") + string (rcvd_message.message_text.buf) << endl;
        }
        if (string(rcvd_message.message_text.buf) == ":quit") break;
        cout << "Type the command: ";
        }
    cout << "Client ended." << endl;
}
