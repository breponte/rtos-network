#include "host.cpp"

int main() {
    cout << "Host 1 initializing" << endl;
    Host * host1 = new Host("127.0.0.1", "127.0.0.2");
    cout << "Host 2 initializing" << endl;
    Host * host2 = new Host("127.0.0.2", "127.0.0.10");
    cout << "Host initialized" << endl;

    host1->sendMessage();
    host1->sendMessage();
    sleep(3);
    host2->receiveMessage();
    host2->receiveMessage();
    sleep(3);
    host2->sendMessage();
    host1->receiveMessage();

    return 0;
}