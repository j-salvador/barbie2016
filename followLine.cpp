#include <stdio.h>
#include <time.h>
#include <math.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);
extern "C" int set_PWM( int chan , int value );
extern "C" char set_pixel(int row, int col,char red,char green, char blue);

int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(0);
    int maxSpeed = 102;//You can change this value from 0-255
    float kp = 0.5;
//-------------------------------------------------------------------------------
    //Don't change these values:
    int z = 320; //Maximum array size
    int pixCord[z]; //Initializes the array Size
    bool on = false; //PWM
    int VL;
    int VR;
    float errorValue;  
    float p;
    //----------------------------------------------------------------------------
while(true){ //Infinite Loop when robot turns on.

	//When Power is applied turn motors on a period of (0.01*speed) Seconds
	if(!(on)){ //Checks if AVC has already warmed up
	for (int i = 0; i < maxSpeed; i++){ // 30 will be replaced with maxSpeed
		set_PWM(1 ,i);
		set_PWM(2 ,i);
		Sleep (0 ,010000);
		if(i == maxSpeed){
			on = true; //AVC motor's are warmed up and ready to go
		}
	}
	}
//--------------------------------------------------------------------------
    while(true){ //Loop - Breaks once single frame is scanned.
    //Every cycle analyzes one single frame.
       bool line = false;
       take_picture();
       errorValue = 0;
       p=0;
       int w, s;
       for (int i = 0; i < sizeof(pixCord); i++){
    	   pixCord[i] = get_pixel(i,120,3);
    	   //The if and else statements Remove any noise:
    	   if(pixCord[i] < 127){ //If pixel is closer to black
    		   w = 255;
    		   pixCord[i] = set_pixel(i, 120,w,w,w); //Possible method for mapping
			   s=0;
    	   }else { //white pixel
    		   w= 255 ;
    		   pixCord[i] = set_pixel(i, 120,w,w,w);
    		   s=1;
    		   line = true;
    	   }
    	   errorValue = errorValue + (i-160)*s; //Negative value = line left side
    	   	   	   	   	   	   	  //Higher the value the further away the AVC is to the line.
       }//Closes for loop
     errorValue = errorValue/sizeof(pixCord); //Averages error value, Once for loop is complete 
     if(line){
    	 break;
    	  
     }else{
    	 // No line detected.
    	 set_motor(1,0);
    	 set_motor(2,0);
    	 Sleep (0 ,200000);
    	 set_motor(1,-100);//Start Pivoting left.
    	 set_motor(2,-100);
    	 Sleep (0 ,800000);
    	 //This means the loop will repeat in attempt to re-find the line
    	 //pivot();
     }
    }
//---------------------------------------------------------------------------
    //Movement of Robot:
    //Robot will run at maxSpeed unless it loses the line, then it will decrease the nth motor speed
    //corresponding to the side the line is on.
    	p = roundf(1 * errorValue) / 1; //Rounds to a Whole number 
    	VL = maxSpeed - p; //Added P need to add I & D
		VR = (-1*maxSpeed) + p;
    	set_motor(1,VL);
    	set_motor(2,VR);
    	

//---------------------------------------------------------------------------
}//Closers Main Loop
return 0;}
/*Notes:
 * I should Add more Debugging Print messages.
 * colorCalibration test needed to determine a value for ambient, wLight and bLight
 */
