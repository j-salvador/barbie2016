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
    double P = 5;
    double D = 0;
    double kp = 0.0020;
    double kd = 0;
    int maxSpeed = 51;
    int VL;
    int VR;
    time_t start; //Sets 0 Point for timer to begin the clock at.
    time_t finish; //Sets finishing time
    int dif;
    int count = 0;
    double tim = 0.03;
    double avgC = 0;
    double current_error = 0;
    double previous_error = 0;
    double total_error = 0;
    double delta_error;
//-----------------------------------------------------------
    while(true){
    	start = tim(NULL);//Starts Timer
    	take_picture();
//--------------------------------------Analyzing Image--------------------------------
    	for (int i = 0; i < 320; i++){
    	   w = get_pixel(i,120,3);
    	   if(w < 127){ //If pixel is close to black ------ (Reduces noise)
    		   s=0;
    	   }else {
    		   s=1;
    	   }
    	   current_error = current_error + (i-160)*s; //Adds to current_error if its a white pixel
    	}//Closes For Loop
//--------------------------------------Time Stamp & FPS---------------------------
    	finish = time(NULL);
    	dif = finish -start;
    	if(dif==1){// Once happens when 1 second has passed.
    		tim = 1/count; //Updates the current time taken for AVC to process 1 Frame
    		count = 0;
    	}
//---------------------------------------Error Values-------------------------------
    	avgC = current_error/320;
    	P = avgC*kp;
		D = ((avgC-previous_error)/tim)*kd;
		printf("D = %f\nP = %f\n",D,P); //Debugging
    	
		//Initializing values for next frame
		previous_error = avgC;
    	current_error = 0; //Resets current error to 0
		avgC = 0;
    	count++;
//----------------------------------------Motor Control--------------------------
      	VL = maxSpeed - (P);// - (D);
    	VR = maxSpeed + (P);// + (D);
    	set_motor(1,VL);
    	set_motor(2,-VR);    	
    	
    }//Closes While Loop
return 0;}
/*
 * If the line is to the Left then the Current Error value should be positive.
 * Add if statement for timer if(start){start = time(NULL);}
 *
*/
/*if(timer){
 *	start = time(NULL);
 *	timer = false;
 *}
 *cycles through main while loop
 *finish = time(NULL); // Finish time gets larger and larger until it is 1 second more than start
 *if(dif==1){
 *	timer = true; //Restarts clock
 *}
*/
