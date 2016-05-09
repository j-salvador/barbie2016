#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);
extern "C" int set_PWM( int chan , int value );

int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(0);
    char c;
    int z = 32; //Maximum array size
    int pixCord[z]; //Initializes the array Size
    int ambient = 0; //?? I will need to do the colorCalibration test before determining a value
    int e = 0; //error - Research PID
    //You can change these value from 0-255
    int maxSpeed = 102;
    int SlowVL = 82;
    int SlowVR = 82;
    bool on = false;
    int VL = maxSpeed; //Velocity of Left & Right Motor if on top of line
    int VR = maxSpeed;
    int wLight = 200;// Not sure what this is, testing tomorrow.
    int bLight = 50;// Same again not sure what this is.

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
    while(true){ //Loop - Breaks once single frame is scanned.
    //Every cycle analyzes one single frame.
    	bool noLine = false;
       take_picture();

       while(true){
       for (int i = 0; i < sizeof(pixCord); i++){//Records values for every 10 pixels across eg: 0,10,20...310,320
    	   pixCord[i] = get_pixel(i*10,120,3);
    	   if(z == sizeof(pixCord)){
    	       	   break; //Do I need a while loop here?
    	   }

       }
       }
       for (int i = 0; i < sizeof(pixCord); i++){
       if(pixCord[i] <wLight){ //Detects if there is actually a line in from of the robot.
    	   noLine = true;

       }else if(pixCord[i] >wLight ){ //white light detected
    	   noLine = false;
    	   break; //Begin movement of AVC
    	   //turn 45* and take another picture and try to find the line
       }
       }
       if(noLine){ //No line = true
    	   set_motor(1, VL);
    	   set_motor(2, VR);
    	//Don't break, //turn 90* and take another picture and try to find the line
       }
    }
    //Determine position of line:

    //This Algorithm will be changed but at the moment it only determines
    //if the line is to the left or to the right of the centered pixel at (160,120).

    int Q1Sum;
    int Q2Sum;
    int avgQ1;
    int avgQ2;
    //Read light intensities of pixCord[0] to pixCord[15] 16values
    for (int i = 0; i < 15; i++){
    	Q1Sum = Q1Sum + pixCord[i]; //Sum of white light detected on Left Quadrant
    }
    	avgQ1 = Q1Sum/16;
    //Read light intensities of pixCord[16] to pixCord[32]
    for (int i = 16; i < 32; i++){
        	Q2Sum = Q2Sum + pixCord[i]; //Sum of white light detected on Right Quadrant
        }
        avgQ2 = Q1Sum/16;

        //By taking ambient light into account the robot will only correct its movement when the AVC is not directly over the line
       if( (pixCord[15] > (wLight -e) ) || (pixCord[16] > (wLight -e) ) ){ //if center pixels detect white line
    	   VL = maxSpeed; //Sets robot to Max Velocity
    	   VR = maxSpeed;
    	   set_motor(1, VL);
    	   set_motor(2, VR);
       }
       else if(avgQ1 > (avgQ2 +ambient)){ //Line is to the left
    	   VL = SlowVL; //Slow left Motor speed so AVC drifts to left
    	   set_motor(1, VL);
    	   Sleep(0,100000);
       }
       else if(avgQ2 > (avgQ1 +ambient)){ //Line is to the right
    	   VR = SlowVR;
    	   set_motor(1, VR); //Slow right Motor speed
    	   Sleep(0,100000);
       }else {
    	   printf("No Line is present.");

       }
}


return 0;}
/*Notes:
 * I should Add more Debugging Print messages.
 */