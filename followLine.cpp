#include <stdio.h>
#include <time.h>

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
    char c;
    int z = 320; //Maximum array size
    int pixCord[z]; //Initializes the array Size
    int ambient = 0; //?? I will need to do the colorCalibration test before determining a value
    int e = 0; //error - Research PID
    //You can change these value from 0-255
    int maxSpeed = 102;
    bool on = false;
    int VL = maxSpeed; //Velocity of Left & Right Motor if on top of line
    int VR = maxSpeed;
    int wLight = 200;// Not sure what this is, testing tomorrow.
    int bLight = 50;// Same again not sure what value should be here.
    				//-- Anything below wLight reading is going to be changed to a black pixel
    float Psignal;
    float kp = 0.5;
    int sum;
while(true){ //Infinite Loop when robot turns on.

	//When Power is applied turn motors on a period of (0.01*speed) Seconds
	if(!(on)){ //Checks if AVC has already warmed up
	for (int i = 0; i < maxSpeed; i++){
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
       bool Line = false;
       take_picture();
       sum = 0;
       int w, s;
       for (int i = 0; i < sizeof(pixCord); i++){
    	   pixCord[i] = get_pixel(i,120,3);
    	   //Removes noise:
    	   if(pixCord[i] < 127){ //If pixel is closer to black
    		   w = 255;
    		   pixCord[i] = set_pixel(i, 120,w,w,w); //Possible method for mapping
			   s=0;
    	   }else { //white pixel
    		   w= 255 ;
    		   pixCord[i] = set_pixel(i, 120,w,w,w);
    		   s=1;
    	   }
    	   sum = sum + (i-160)*s; //Negative value = line left side
    	   	   	   	   	   	   	  //Higher the value the further away the AVC is to the line.
       }
     if(sum == 0){
    	 bool Line = false; // No line detected.
    	 //Insert Movement to rotate AVC by 45 to 90 degrees
    	 //This means the loop will repeat in attempt to re-find the line
    	 //pivot();
     }else{
         bool Line = true; // line detected.
         break;
     }
    }
//---------------------------------------------------------------------------
    //Movement of Robot:
    	sum = sum/sizeof(pixCord);
    	Psignal = sum*kp; // sum is the error value    	
    	VL = 30 *Psignal;
		VR = 30 *Psignal;
    	set_motor(1,VL);
    	set_motor(2,-1*VR);
    	//f(x) = 0.1(b^x)+0

//---------------------------------------------------------------------------
}//Closers Main Loop
return 0;}
/*Notes:
 * I should Add more Debugging Print messages.
 */
