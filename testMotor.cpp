# include <stdio.h>
# include <time.h>
//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int Sleep( int sec , int usec );
extern "C" int set_motor( int motor , int speed);
int main (){
   // This sets up the RPi hardware and ensures everything is working correctly.
   init(0);
   //while loop which slowly turns the motors until at max Speed over 2.55seconds.
   int i; // "i" (speed) is slowly increasing by 1 every 0.01second
   while(i<255){
   set_motor(0, i); //Pin 1 responsible for the left wheel turning at "i" speed.
   set_motor(1, i); //Pin 2 responsible for the right wheel turning at "i" speed.
   Sleep (0 ,010000); //Keeps motors at set speed for 0.01seconds before increasing the speed
   i++;
   }
   //Robot continues to travel at max speed for 3 Seconds in the forward direction:
   set_motor(0, 255);
   set_motor(1, 255);
   Sleep (2 ,500000);
   //Robot will then experience a sudden stop:
   set_motor(0, 0);
   set_motor(1, 0);

   //Test turning Left, the movement will be a lot like a tank with tank tracks.
   set_motor(0, -127); //Left wheel will rotate backwards at 50% speed
   set_motor(1, 127); // Right wheel will rotate forwards at 50% speed
   Sleep (2 ,000000); //robot will turn left for 2 Seconds
   //Robot will then experience a sudden stop:
   set_motor(0, 0);
   set_motor(1, 0);
   Sleep (2 ,000000);

   /*Problem with the above method of turning left is we only can know the robot has turned
   * a certain amount of degrees by using guess and check.
   */
   //Test turning Right
   set_motor(0, 127); //Left wheel will rotate forwards at 50% speed
   set_motor(1, -127); // Right wheel will rotate backwards at 50% speed
   Sleep (2 ,000000); //robot will turn right for 2 Seconds
   //Robot will then experience a sudden stop:
   set_motor(0, 0);
   set_motor(1, 0);
   return 0;
}
