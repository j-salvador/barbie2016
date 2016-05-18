#include <stdio.h>
#include <time.h>
#include <stdbool.h>


extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

void networkGate();

void networkGate(){
    init(0);		//initialises rPi
    int messageLength = 6;
    char message[24];
    connect_to_server("130.195.6.196", 1024);		//connects to server with the ip address 192.168.1.2 //130.195.6.196
    send_to_server("Please server");		//sends a message to the connected server
    receive_from_server(message); //this may be buggy! //receives message from the connected server
    message[messageLength] = '\0'; // strlen(message) = 6, sizeof(str) = 12 (with 6 nulls) http://stackoverflow.com/questions/4711449/what-does-the-0-symbol-mean-in-a-c-string
    send_to_server(message);		//sends a message to the connected server
    printf("%s", message);
    return;
}

int main(){
    networkGate();
    return 0;
}
