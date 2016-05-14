#include <stdio.h>
#include <time.h>
#include <math.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);

int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
	//Initializing variables
    init(0);
    int w;
    int s;
    double errorValue;
    int maxSpeed = 60;
    int VL;
	int VR;
    double kp = 1;

//-----------------------------------------------------------
    while(true){ //A loop which takes a picture scans it then changes the robot's speed
       take_picture(); 
       errorValue = 0; //Resets errorValue for P
       for (int i = 0; i < 32; i++){ //Scans for every 10 pixels across the frame
    	   w = get_pixel(i*10,120,3);  //an int from 0-255. 
    	   	   	   	   	   	   	   	   //0 = Perfect Black 
    	   	   	   	   	   	   	   	   //255 = Perfect white
    	   if(w < 127){ //Removes Noise
    		   s=0;
    	   }else {
    		   s=1;
    	   }
    	   errorValue = errorValue + (i-160)*s; //Sum of errorValue
       }//Closes For Loop

       errorValue = round(errorValue); //Rounds errorValue to a whole number
       printf("Error Value: %d" ,errorValue); //Prints Error Value
       VL = maxSpeed - (errorValue*kp);
       VR = maxSpeed + (errorValue*kp);
       printf("Left Motor Speed: %i",VL);
       printf("Right Motor Speed: %i",VR);
       set_motor(1,VL);
       set_motor(2,-VR);
       Sleep(0,200000); //Limits FPS to 5 --- Just for testing
    }//Closes While Loop
return 0;}
