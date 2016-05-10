# include <stdio.h>
# include <time.h>
//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int Sleep( int sec , int usec );
extern "C" int set_motor( int motor , int speed);
int main (){

   init(0);
   
   printf("Test 1 start");
   //Turning Left
   Sleep (3 ,000000);
   set_motor(1, -51);
   set_motor(2, 51);
   Sleep (0 ,500000);
   set_motor(1, 0);
   set_motor(2, 0);
   //brief pause to measure how far its turned
   printf("Test 1 fin");
   Sleep (4 ,000000);
   printf("Returning to origin...");
   set_motor(1, 51);
   set_motor(2, -51);
   Sleep (0 ,500000);
   set_motor(1, 0);
   set_motor(2, 0);

   printf("Test 2.");
   //Turning Left
   Sleep (3 ,000000);
   set_motor(1, -51);
   set_motor(2, 51);
   Sleep (0 ,750000);
   set_motor(1, 0);
   set_motor(2, 0);
   //brief pause to measure how far its turned
   printf("Test 2.");
   Sleep (4 ,000000);
   printf("Returning to origin...");
   set_motor(1, 51);
   set_motor(2, -51);
   Sleep (0 ,750000);
   set_motor(1, 0);
   set_motor(2, 0);

   printf("Test 3.");
      //Turning Left
      Sleep (3 ,000000);
      set_motor(1, -51);
      set_motor(2, 51);
      Sleep (1 ,000000);
      set_motor(1, 0);
      set_motor(2, 0);
      //brief pause to measure how far its turned
      printf("Test 3.");
      Sleep (4 ,000000);
      printf("Returning to origin...");
      set_motor(1, 51);
      set_motor(2, -51);
      Sleep (1 ,000000);
      set_motor(1, 0);
      set_motor(2, 0);

   return 0;
}
