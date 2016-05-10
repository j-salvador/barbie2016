# include <stdio.h>
# include <time.h>

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int main (){

   init(1);		//initialises rPi

   connect_to_server("192.168.1.2", 1024);		//connects to server with the ip address 192.168.1.2

   send_to_server("Hello server");		//sends a message to the connected server

   char message[24];		//receives message from the connected server

   receive_from_server(message); //this may be buggy!

   printf("%s", message);

return 0;}
