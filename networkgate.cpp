#include <stdio.h>
#include <time.h>
#include <stdbool.h>

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

void networkGate();
//bool run_gate = true;

void networkGate(){
    init(1);		//initialises rPi
    char message[24]; //initialises array of chars, size 24
    connect_to_server("130.195.6.196", 1024);		//connects to server @130.195.6.196:1024
    send_to_server("Please");		//sends "Please" to server
    receive_from_server(message); //receives message
    send_to_server(message);	//sends back received message
    //run_gate = false;
}

int main(){
    networkGate();
    return 0;
}
