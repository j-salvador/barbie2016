 //imports all of the main libraries
#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

//#include "avcMain.cpp"

//------------------------------------------------------------------------------
//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);
extern "C" int set_PWM( int chan , int value );
extern "C" char set_pixel(int row, int col,char red,char green, char blue);
extern "C" int ReadAnalog(int ch_adc);
//-----------------------------------------------------------------------------
//Declaraiton of the functions.
bool navigateMaze();
bool mazePID();
void turn(int dir);

//Declare variables
int leftSensor = 0; //Stores the digital pin value of the first sensor
int rightSensor = 1; //Stores the digital pin value of the second sensor
int leftMotor = 1; //Stores the GPIO pin number of the left motor
int rightMotor = 2; //Stores the GPIO pin number of the right motor
int left = 1; //arbitrary number to store for checking left
int right = -1; //arbitrary number to store for checking right
int dRight; //Stores the current readings for the right sensor
int dLeft;//Stores the current readings for the left sensor

int main(){
    navigateMaze();
    std::cin.get();
    return 0;
}

bool navigateMaze(){
    int mazeThreshold = 600; //Should be slightly more than the wall distance reading
    int mazeState = false; //Initially the maze is not complete
    //----------------------------------------------
    dLeft = ReadAnalog(leftSensor); //Takes a reading from both sensors
    dRight = ReadAnalog(rightSensor);
    if (dLeft <= mazeThreshold && dRight <= mazeThreshold){//Checks if it is in the maze
        //Runs the maze code
        mazeState = mazePID(); //the mazePID function will return true when the maze is done
    }
    //Returns false is not in the maze code
    else {
        mazeState = false; //Returns false if not in the maze
    }
    return mazeState; //Returns the value of the maze state
}

bool mazePID(){
    //Initilize/Declare the local variables
    int pSig, dSig;
    int kp = 0.31;
    int kd = 0.003;
    int pastError = 0;
    int timePeriod = 0.001;
    int wallDistReading = 500;
    int maxSpeed = 40;
    int VL, VR;
    int currentError;
    int mazeDerivitive = 0;
    int turnThreshold = wallDistReading;
    //---------------------------------------------
    //Loop that runs while the maze isn't complete
    while (true){
        dLeft = ReadAnalog(leftSensor);
        dRight = ReadAnalog(rightSensor);
        if (dLeft > turnThreshold && dRight <= wallDistReading){ //Should it turn left
            turn(left);
        }
        else if (dRight > turnThreshold && dLeft <= wallDistReading){ //Should it turn right
            turn(right);
        }
        else if (dLeft < wallDistReading && dRight < wallDistReading){ //Should it turn around
            turn(right);
        }
        currentError = round(wallDistReading - ReadAnalog(leftSensor));
        printf("Error Value: %d" ,currentError ); //Prints Error Value
        mazeDerivitive = round((currentError - pastError)/timePeriod);
        VL = maxSpeed - (currentError*kp) + (mazeDerivitive*kd);
        VR = maxSpeed - (currentError*kp) + (mazeDerivitive*kd);
        printf("Left Motor Speed: %i",VL); //Adjust the speed on each motor according to the derivitive and proportional signals
        printf("Right Motor Speed: %i",VR);
        set_motor(leftMotor,VL);
        set_motor(rightMotor,VR);
        Sleep(0,200000); //Limits FPS to 5 --- Just for testing
    }
}

void turn(int dir){
    //Declare and initialze local variables
    int turnSpeed = 30;
    bool turnDone = false;
    int turnDoneThreshold = 400; //A distance slightly less than what the sensor reads to the walls when it is centered.
    int turnConstant = 1; //Starts at the default of left;
    if (dir == right){ //Checks if the turn direction is right
        turnConstant = -1; //Reverses the direction the motors will spin
    }
    else if (dir == left){ //Or if the turn direction is left
        turnConstant = 1; //Keeps the direction the motors will spin
    }
    //-----------------------------------------------
    //Runs while the robot is not centered about the new turning direction
    while (!turnDone){
        //Turns the left and right motors in the right directions
        set_motor(leftMotor, turnConstant * turnSpeed); //Makes the left motor turn according to the turn direction
        set_motor(rightMotor, -1*turnConstant * turnSpeed); //Makes the right motor turn according to the turn direction
        //Re-checks the readings on the sensors
        if (ReadAnalog(leftSensor) > turnDoneThreshold && ReadAnalog(rightSensor) > turnDoneThreshold){
            turnDone = true; //Breaks the loop when the robot has clearly completed a turn.
        }
        //Loop should end when the robot is roughly centered about a wall.
    }
}
