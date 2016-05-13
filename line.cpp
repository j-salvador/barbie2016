
#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);

int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(0);
    int maxSpeed = 40;//You can change this value from 0-255
    float kp = 0.5;
//-------------------------------------------------------------------------------
    //Don't change these values:
    int z = 320; //Maximum array size
    int pixCord[z]; //Initializes the array Size
    
    //int VL;
    //int VR;
//--------------------------------------------------------------------------
    while(true){ 
       take_picture();
       int s;
       int w;       
       w  = get_pixel(160,120,3); // Looks at center pixel
       if(w < 127){ //If pixel is closer to black
    	   s=0;
       }else {
           s=1;
           
       }
        //VL = maxSpeed ; 
        // VR = (-1*maxSpeed);
        if(s==1){
        	printf("White \n");
        //set_motor(1,VL);
        //set_motor(2,VR);
        }
        if(s==0){
        	printf("Black \n");
        }
}//Closers Main Loop
return 0;}
