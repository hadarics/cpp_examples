#include <iostream>
#include <map>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>

using namespace std;

const int proj_id = 0xabcd;
const char mq_file[15] = "/tmp/mq_server";
const unsigned perm = 0660;

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

    struct message message;

    key_t key = ftok(mq_file,proj_id);
    if (key == -1) error_and_exit("Error in ftok");

    int sqid = msgget (key, IPC_CREAT | perm);
    if (sqid < 0) error_and_exit("Error in msgget");

    cout << "Server started." << endl;

    map <string, unsigned> population;
    population.insert(pair <string, unsigned short> ("Aba", 4674));
    population.insert(pair <string, unsigned short> ("Dunaújváros", 48104));
    population.insert(pair <string, unsigned short> ("Paks", 19625));

    while(1) {
        // read a received message
        if (msgrcv (sqid, &message, sizeof (struct message_text), 0, 0) == -1) {
            error_and_exit("Error in msgrcv");
        }
        else {
            cout << "Message received from client: " << message.message_text.buf << endl;

            if (string(message.message_text.buf) == ":squit") {
                strcpy(message.message_text.buf,":quit");
                if (msgsnd (sqid, &message, sizeof (struct message_text), 0) == -1) {
                    error_and_exit("Error in msgsend");
                }
                break;
            }

            // lookup data
            map <string, unsigned>::iterator it = population.find(message.message_text.buf);
            if (it != population.end()) {
                strcpy(message.message_text.buf, to_string(it->second).c_str());
            }
            else {
                strcpy(message.message_text.buf,"not found");
            }

            // send reply message to client
            if (msgsnd (sqid, &message, sizeof (struct message_text), 0) == -1) {
                error_and_exit("Error in msgsend");
            }
            else {
                cout << "Server response sent to client: " << message.message_text.buf << endl;
            }

        }

    }
    // remove message queue
    if (msgctl (sqid, IPC_RMID, nullptr) == -1) error_and_exit("Error in msgctl");
    cout << "Server ended." << endl;
}
