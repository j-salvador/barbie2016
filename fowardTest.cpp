# include <stdio.h>
# include <time.h>
//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int Sleep( int sec , int usec );
extern "C" int set_motor( int motor , int speed);
int main (){

   init(0);
   
   printf("Test 1 start");
   //Forward
   Sleep (3 ,000000);
   set_motor(1, -200);
   set_motor(2, 200);
   Sleep (5 ,500000);
   set_motor(1, 0);
   set_motor(2, 0);
   Sleep (1 ,500000);
   //Left
   set_motor(1, 200);
   set_motor(2, 0);
   Sleep (5 ,500000);
   set_motor(1, 0);
   set_motor(2, 0);
   Sleep (1 ,500000);
   //Right
   set_motor(1, 0);
   set_motor(2, -200);
   Sleep (5 ,500000);
   set_motor(1, 0);
   set_motor(2, 0);
   return 0;
}
