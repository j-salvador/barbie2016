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
    double P = 0;
    double D = 0;
    double kp = 0.31;
    double kd = 0.003;
    int maxSpeed = 30;
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
    double delta_error;
    int setTime = 0;
    bool line = true;
    int iCount = 0;
    double lastCurrent_error = 0;

//----------- Quad 3 Field ------------
    bool left = false;
    bool right = false;
    bool middle = false;
    int spike = 10000; //Not sure what this is.
    bool deadEnd = false;
    double tIntersection = 999999; //Have no idea what this value is either.
//------------------------------------------------------------------------------
    while(true){
    	start = time(NULL);//Starts Timer
    	take_picture();
//--------------------------------------Analyzing Image-------------------------
    	for (int i = 0; i < 320; i++){
    	   w = get_pixel(i,120,3);
    	   if(w < 127){ //If pixel is close to black ------ (Reduces noise)
    		   s=0;
    	   }else {
    		   s=1;
    	   }
    	   current_error = current_error + (i-160)*s; //Adds to current_error if its a white pixel
    	}//Closes For Loop
//---------------------------------------Error Values---------------------------
    	avgC =  (current_error/320);
    	P = (current_error/320)*kp;
		  //printf("D = %f\nP = %f\n",D,P); //Debugging
//-------------------------------- Quad 3 Detect Line changes ------------------
      printf("Spike = %f\navg = %f\nC",current_error,avgC);
current_error = 0; //Resets current error to 0
//-----------------------------------------END OF PROGRAM-----------------------
}//Closes While Loop
return 0;}
/*Notes
 *
 *
*/
