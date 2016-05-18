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
    double current_error;
    double P = 0;
    double kp = 0.0020;
    double kd = 0.0005;
    int maxSpeed = 51;
    int VL;
    int VR;
    time_t start; //Sets 0 Point for timer to begin the clock at.
    time_t finish; //Sets finishing time
    int dif;
    int count = 0;
    double time = 0;
    
    double previous_error = 0;
//-----------------------------------------------------------
    while(true){
    	start = time(NULL);
    	take_picture();
    	previous_error = current_error;
    	current_error = 0;
    	for (int i = 0; i < 320; i++){
    	   w = get_pixel(i,120,3);
    	   if(w < 127){ //If pixel is close to black
    		   s=0;
    	   }else {
    		   s=1;
    	   }

    	   current_error = current_error + (i-160)*s; //Adds to current_error if its a white pixel
    	}//Closes For Loop

    	//current_error = round(current_error); //Rounds current_error to a whole number
    	P = current_error*kp;
    	D = ((current_error-previous_error)/time)*kd;
    			
    	//printf("Error Value: %d \n" ,P);
    	VL = maxSpeed - (P) - (D);
    	VR = maxSpeed + (P) + (D);
    	set_motor(1,VL);
    	set_motor(2,-VR);
    	//Time Stamp:
    	finish = time(NULL);
    	dif = finish -start;
    	if(dif==1){
    		printf("FPS = %i \n",count);
    		time = 1/count;
    		count = 0;
    	}
    	count++;
    }//Closes While Loop
return 0;}
/*
 * Need too add Time Stamp.
 * Then measure amount of fps to test if time Stamp is working
 * Only then can I add 'I' in PID
*/
