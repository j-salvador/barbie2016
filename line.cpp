 
#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);
extern "C" int set_PWM( int chan , int value );

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
                if(i > maxSpeed-1){
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
       int s;
        int w;
       for (int i = 0; i < sizeof(pixCord); i++){
           w  = get_pixel(i,120,3);
           //The if and else statements Remove any noise:
           if(w < 127){ //If pixel is closer to black
                           s=0;
           }else { //white pix
                   s=1;
                   line = true;
           }
           //errorValue = errorValue + (i-160)*s; //Negative value = line left side
                                                          //Higher the value the further away th$
       }//Closes for loop
     
     
        //p =  errorValue*kp; //Rounds to a Whole number
        VL = maxSpeed ; //Added P need to add I & D
        VR = (-1*maxSpeed);
        if(s==1){
        set_motor(1,VL);
        set_motor(2,VR);
        }
        if(s==0){
        set_motor(1,0);
        set_motor(2,0);	
        }
        }

//---------------------------------------------------------------------------
}//Closers Main Loop
return 0;}
/*Notes:
 * I should Add more Debugging Print messages.
 * colorCalibration test needed to determine a value for ambient, wLight and bLight
 */
