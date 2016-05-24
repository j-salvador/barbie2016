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
    double kp = 0.27;
    double kd = 0.003;
    int maxSpeed = 40;
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
    bool white_light;
    int l = 0;
    int r = 0;
    int turn_180 = 0;
//------------------------------------------------------------------------------
    while(true){
    	start = time(NULL);//Starts Timer
    	take_picture();
      white_light = true;
      l = 0;
      r = 0;
//--------------------------------------Analyzing Image-------------------------
    	for (int i = 0; i < 320; i++){
    	   w = get_pixel(i,120,3);
    	   if(w < 127){ //If pixel is close to black ------ (Reduces noise)
    		   s=0;
    	   }else {
    		   s=1;
           white_light = false;
           if(i<160){
             l++;
           }else{
             r++;
           }
         }
    	   current_error = current_error + (i-160)*s; //Adds to current_error if its a white pixel
    	}//Closes For Loop

//-------------------------------------No Line Detected-------------------------
        if(l >100 || r >100){//Spin around 180}

//--------------Debugging-------------------------
if(l >100 || r >100){
        printf("Left = %d\nRight = %d\n"l,r)//Debugging

      printf("Turning Around %d\n",turn_180);
    }
      //VL = maxSpeed;
      //VR = maxSpeed;
      //set_motor(1,VL);
      //set_motor(2,VR);
      //Sleep(0,100000);
      //turn_180++;
//--------------Debugging ENDS-------------------------


        if(current_error == 0 && white_light){ //error is 0 and black_light
          line = false;
          VL = maxSpeed;
          VR = maxSpeed;
          set_motor(1,-VL);//Sets motors to reverse
        	set_motor(2,VR);
          iCount++; //Counts number of frames line has been missing for
        }else{
          line = true;
          iCount = 0;
        }
        if(iCount>0){ //Debugging
          printf("Count = %d (line has been missing for %d FPS)\n",iCount,iCount);
        }
          if( current_error<0 || current_error>0 ){
              lastCurrent_error = current_error;
          }
          if(iCount>7){ //Line has been lost for a while. Run Correction
              if(lastCurrent_error>0){
                set_motor(1,-VL);
              	set_motor(2,-VR);
                Sleep(0,100000);
              }else if(lastCurrent_error<0){
                set_motor(1,VL);
              	set_motor(2,VR);
                Sleep(0,100000);
              }
          }//Closes iCount if statement

//--------------------------------------Time Stamp & FPS------------------------
      finish = time(NULL);
    	dif = finish -start;
    	if(dif==1){// Only occurs when 1 second has passed when 1 second has passed.
    		    tim = 1/count; //Updates the current time taken for AVC to process 1 Frame
    		    count = 0;
    	}
//---------------------------------------Error Values---------------------------
    	avgC = avgC + (current_error/320);
    	P = (current_error/320)*kp;
		  //printf("D = %f\nP = %f\n",D,P); //Debugging
    	current_error = 0; //Resets current error to 0
		  setTime++;
      count++;
//----------------------------- D value for PID --------------------------------
      if(setTime == 1){ //This means our D value changes every frame

       	D = ((avgC-previous_error)/(0.03))*kd;
        previous_error = avgC;
    		avgC = 0;
    		setTime = 0;
    	}
//----------------------------------------Motor Control-------------------------
    if(line){


      VL = maxSpeed - (P) + (D);
    	VR = maxSpeed + (P) - (D);
    	set_motor(1,VL);
    	set_motor(2,-VR);
    }
//-----------------------------------------END OF PROGRAM-----------------------
}//Closes While Loop
return 0;}
/*
 * Kp & Kd values could be tuned more finely if time persists
 * Need to add: D = round(D); & P = round(P);
 *Time Stamp needs fixing
*/
