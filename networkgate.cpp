#include <stdio.h>
#include <time.h>
#include <stdbool.h>

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

void networkGate();

void networkGate(){
    init(1);		//initialises rPi
    char message[24];
    connect_to_server("130.195.6.196", 1024);		//connects to server with the ip address 192.168.1.2 //130.195.6.196
    send_to_server("Please");		//sends a message to the connected server
    receive_from_server(message); //this may be buggy! //receives message from the connected server
    send_to_server(message);		//sends a message to the connected server
}

int main(){
    networkGate();
    return 0;
}
