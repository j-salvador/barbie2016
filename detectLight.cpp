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
    //int maxSpeed = 100;
    //int VL;
	//int VR;
    //double kp = 1;

//-----------------------------------------------------------
    while(true){
       take_picture();
       errorValue = 0;
       for (int i = 0; i < 32; i++){
    	   w = get_pixel(i*10,120,3);
    	   if(w < 127){ //If pixel is close to black
    		   s=0;
    	   }else {
    		   s=1;
    	   }

    	   errorValue = errorValue + (i-160)*s; //Adds to errorValue if its a white pixel
	   errorValue = errorValue/32;	
       }//Closes For Loop

       errorValue = round(errorValue); //Rounds errorValue to a whole number
       printf("Error Value: %d \n" ,errorValue);
       //VL = maxSpeed - (errorValue*kp);
       //VR = maxSpeed + (errorValue*kp);
       //set_motor(1,VL);
       //set_motor(2,-VR);
    }//Closes While Loop
return 0;}
