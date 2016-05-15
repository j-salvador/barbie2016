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
float takeReading(int analogChannel);
void setSpeed(int motor, int speed);
void PWM(int dir, int motor, int speed);
void deadEndTurn();
bool navigateMaze();
void mazePID();

//------------------------------------------------------------------------------
//Initialization of global variables
float speed = 30; //Sets the speed through out the maze to 30
float dWall = 50; //Sets the average wall Distance when the robot is sensored, ie dLeft = dRight at facing foward.
bool inMaze = false; //The AVC doesn't start in the maze, when it detects it is, this will trigger.
float dWallError = 5; //The acceptable error value, that the PID can handle for dead end turns and correct.
bool readyPWM = false; //Initially set to false, since it will be at rest.
float maxSpeed = 50; //Sets the maximum speed for the AVC going through the maze.
float maxReading = 100; //This is the maximum reading for the IR sensors when they are in the maze.
float currentSpeed = 0; //A global variables to store the current speed of the AVC for PWM.

float deadSpeed = 40; //The speed at which it turns at dead ends.
float dLeft; //variables to store the readings on the left sensors
float dRight; //Variables to store the reading on the right sensor.
//Sets the motors as global variables so they can be changed easily if the order is changed
int m1 = 1;
int m2 = 2;
//Stores the sensors as global variables so that their order can be easily changed
int s1 = 1;
int s2 = 2;
//These variables simply refer to directions, the numbers are arbitrary, since strings can't be used variables are easier
int left = -2;
int right = 2;
int forward = 1;
int backward = -1;

float currDir = forward;
//------------------------------------------------------------------------------
/**
* @param anologChannel
* @method takeReading
*/
float takeReading(int analogChannel){
    //Initialization of local variables
    float x; //Float used to convert the reading to a float
    int y; //Float used to store the reading
    //If the channel is the left sensor
	if (analogChannel == s1){
        y = ReadAnalog(s1);
        x = (float)y;
        dLeft = x;
	}
    //If the channel is the right sensor
	if (analogChannel == s2){
    y = ReadAnalog(s2);
		x = (float) y;
		dRight = x;
	}
	return x; //Might not work in c
}
//------------------------------------------------------------------------------
/**
* @param dir:
* @param vel
* @method PWM
*/
void PWM(int dir, int motor, int vel){
	//Handles acceleration and turning
    //Initializes the local variables
    int lowerVel;
	int higherVel;
    int PWMspeed;
    int subtractionConst;
    //Checks if the desired velocity is larger than the current.
    if (vel > currentSpeed){
        lowerVel = currentSpeed;
        higherVel = vel;
        PWMspeed = currentSpeed;
        subtractionConst = 1;
    }
    //Checks if the desired velocity is smaller than the current
    else if (vel < currentSpeed){
        lowerVel = vel;
        higherVel = currentSpeed;
        PWMspeed = currentSpeed;
        subtractionConst = -1;
    }
    //Loops until the current velocity is equal to the desired velocity
    for (int i = 0; lowerVel < higherVel; lowerVel++){
        //Checks if the robot is turning left
        if (dir == left){
            //Makes the robot turn left
            set_PWM(s1, PWMspeed);
            set_PWM(s2, PWMspeed);
        }
        //Checks if the robot is turning right
        if (dir == right){
            //Makes the robot turn right
            set_PWM(s1, -1*PWMspeed);
            set_PWM(s2, -1*PWMspeed);
        }
        //Else if the robot is going forward
        else {
            //Makes the robot go forward
            set_PWM(s1, PWMspeed);
            set_PWM(s2, -1*PWMspeed);
        }
        PWMspeed += subtractionConst;
        Sleep (0 ,010000);
    }
	readyPWM = true; //Once the AVC has either accelerated or decelerated to the desired speed the function will stop.
    setSpeed(3, PWMspeed);
    return;
}
//------------------------------------------------------------------------------
/**
* @param motor
* @param vel
* @method setSpeed
*/
void setSpeed(int motor ,int vel){
    int dir = currDir;
	if (readyPWM == false){ //Checks if the motor is accelerating
		PWM(dir, motor, vel);	//Calls the PWM funciton if the motor is accelerating
	}

	if(motor == m1){
		//The first motor rotates in the expected direction
		set_motor(m1, vel); //Changes the 1st motors speed
	}

	else if (motor == m2){
		//The second motor rotates in the opposite direction to the 1st, speed is therefore negative.
		set_motor(m2 , -1*vel); //Changes the 2nd motors speed
	}
	else if (motor == 3){
		//If both motors are to go forward at the same speed
		set_motor(m1 , vel); //Changes the first motors speed
		set_motor(m2 , -1*vel); //Changes the second motors speed
	}
}
//------------------------------------------------------------------------------
/**
*
* @method deadEndTurn
*/
void deadEndTurn(){
	//Declaration of local variables
	//Initialization of local variables
	int turnThreshold = 10; //Stores the margin of error for this dead end turn
	while(true){ //Runs until the if condition returns the function
		//Reverses the avc while both readings are smaller than the average wall distance
		takeReading(s1);
		takeReading(s2);
		while (dLeft > dWall && dRight> dWall){
			readyPWM == false; //Sets the PWM to false, since it will decelerate to a stop, and then reverse
			currentSpeed = speed;
			setSpeed (3, -1*speed); //Buts both motors turning the opposite way
		}
		readyPWM = false; //Sets the PWM since it will come to rest
		setSpeed(3, 0); //Sets the speed to 0, making it come to rest
		currentSpeed = 0;
		//Turns the avc just enough so that the left sensor is always less than the wall distance until the turn in complete
		for (int i = 0; i<turnThreshold; i++){
			setSpeed(m1, -1*deadSpeed + i);//Turns right, so the left sensor is always smaller than the wall distance
            setSpeed(m2, deadSpeed - i); //Slows down as it comes around.
        }
		//Continues to turn right until the wall distance is greater than or equal to the left reading.
		while (dLeft > dWall){
            setSpeed(m1, -1*deadSpeed);//Turns right, so the left sensor is always smaller than the wall distance
            setSpeed(m2, deadSpeed);
			if (dLeft <= dWall){
				mazePID();//Restarts the PID after the turn is complete
				return; //Exits the loop, when the avc has completed the full 180 degree turn or more.
			}
		}
	}
}
//------------------------------------------------------------------------------
/**
*
* @method mazePID
*/
bool navigateMaze(){
  //Takes the readings from both of the IR sensors
	takeReading(s1);
	takeReading(s2);
  //If the robot is in the maze this condition should be true
	if (dLeft <= dWall-dWallError || dLeft <= dWall+dWallError){
        if (dRight <= dWall+dWallError || dRight <= dWall-dWallError){
    		  mazePID();
        }
	}
	else {
		return false;
	}
}
//------------------------------------------------------------------------------
/**
*
* @method mazePID
*/
void mazePID(){
    ///Declaration of the variables
    int speedPID; //Stores the intended speed for the avc based of the
    //Initilization of the variables
    float kp = 0.005;//maxSpeed/maxReading;
    float ki = 0;
    float kd = 0;
    int pSigIntial = 0;
    //PID signal variables
    float pSig;
    float iSig;
    float dSig;
    while (inMaze){
        //Runs while the robot is in the maze
    	while(inMaze){
    		//Takes an error signal of how far away the avc is from the center based of the left wall
            pSig = pSigIntial - takeReading(s1);
    		speedPID = currentSpeed - (int) (pSig)*kp;
            //Turn if it is not centered
    		if (pSig != 0){
    			set_motor(m1,speedPID);
    			set_motor(m2,-1*speedPID);
    		}
            //If it is centered
    		else if (pSig == 0){
    			setSpeed(3, speed); //Keep going forward
    		}
    		//If the readings are both just slightly smaller than the wall distance
    		if (takeReading(s1) > dWall-dWallError && takeReading(s2)> dWall-dWallError){
    			deadEndTurn(); //Do a deadend turn
    			return;//Stop the PID function, so that the deadEndTurn function isn't called multiple times
    		}

    	}
    }
}

int main(){
    navigateMaze();
    return 0;
}
//------------------------------------------------------------------------------
