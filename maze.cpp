#include <iostream>
#include <stdio.h>	//import libraries
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);
extern "C" int set_PWM( int chan , int value );
extern "C" char set_pixel(int row, int col,char red,char green, char blue);

//extern "C" int set_PWM( int chan , int value );
//extern "C" char set_pixel(int row, int col,char red,char green, char blue);

float takeReading(int analogChannel);
void setSpeed(int motor, int speed);
void PWM(int dir, int speed);
void mTurn(int dir, int setSpeed);
void deadEndTurn();
void mazeCheck();
void mazePID();
int main();

//Initialization of global variables
float speed = 30; //Sets the speed through out the maze to 30
float dWall = 50; //Sets the average wall Distance when the robot is sensored, ie dLeft = dRight at facing foward.
bool inMaze = false; //The AVC doesn't start in the maze, when it detects it is, this will trigger.
float dWallError = 5; //The acceptable error value, that the PID can handle for dead end turns and correct.
bool readyPWM = false; //Initially set to false, since it will be at rest.
float maxSpeed = 50;
float maxReading = 100;
float deadSpeed = 40;

float dLeft;
float dRight;

//Sets the motors as global variables so they can be changed easily if the order is changed
int m1 = 1;
int m2 = 2;

int s1 = 1;
int s2 = 2;

int left = -2;
int right = 2;
int forward = 1;
int backward = -1;

/**
* @param anologChannel
* @method takeReading
*/
float takeReading(int analogChannel){
    int sensor;
    float x;
    int y;
	if (analogChannel == s1){
        y = ReadAnalog(s1);
        x = (float)y;
        dLeft = x;
	}
	if (analogChannel == s2){
		sensor = s2;
        y = ReadAnalog(s2);
		x = (float) y;
		dRight = x;
	}
	return x; //Might not work in c
}

/**
* @param dir:
* @param setSpeed
* @method mTurn
*/
void PWM(int dir, int vel){
	//Handles acceleration and turning
	int lowerVel;
	int higherVel;
	if (vel )
	readyPWM = true;
	return;
}

/**
* @param motor
* @param speed
* @method setSpeed
*/
void setSpeed(int motor ,int vel){
    int dir = 0;
	if (readyPWM == false){ //Checks if the motor is accelerating
		PWM(dir, vel);	//Calls the PWM funciton if the motor is accelerating
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


/**
* @param dir: this character takes the values L and R for left and right respectively
* @method mTurn
*/
void mTurn(int dir, int vel){
	if (readyPWM == false){ //Checks if the motor is accelerating
		PWM(dir, vel);	//Calls the PWM funciton if the motor is accelerating
	}
	else if (readyPWM == true) { //If the motor is warmed up
		if (dir == left){ //Checks if the direction is left
			//Sets the speed according to one wheel being backwards
			setSpeed(m1, -1*vel);
			setSpeed(m2, vel);
		}
		if (dir == right){ //Checks if the direction is right
			//Sets the speed opposite to the previous statement
			setSpeed(m1, vel);
			setSpeed(m2, -1*vel);
		}
	}
}
/**
* @param dir: this character takes the values L and R for left and right respectively
* @method mTurn
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
			currSpeed = speed;
			setSpeed (3, -1*speed); //Buts both motors turning the opposite way
		}
		readyPWM = false; //Sets the PWM since it will come to rest
		setSpeed(3, 0); //Sets the speed to 0, making it come to rest
		currSpeed = 0;
		//Turns the avc just enough so that the left sensor is always less than the wall distance until the turn in complete
		for (int i = 0; i<turnThreshold; i++){
            readyPWM = false;
			mTurn(right, deadSpeed);//Turns right, so the left sensor is always smaller than the wall distance
		}
		//Continues to turn right until the wall distance is greater than or equal to the left reading.
		while (dLeft > dWall){
			mTurn(right, deadSpeed);
			if (dLeft <= dWall){
				mazePID();//Restarts the PID after the turn is complete
				return; //Exits the loop, when the avc has completed the full 180 degree turn or more.
			}
		}
	}
}
/**
*
* @method mazePID
*/
void mazeCheck(){
	takeReading(s1);
	takeReading(s2);
	if (dLeft == dWall-dWallError && dRight == dWall-dWallError){
		inMaze = true;
	}
	if (dLeft == dWall+dWallError && dRight == dWall+dWallError){
		inMaze = true;
	}
	else {
		inMaze = false;
	}
}
/**
*
* @method mazePID
*/
void mazePID(){
	///Declaration of the variables
	int speedPID; //Stores the intended speed for the avc based of the
	char dirPID; //Stores the intended direction for the avc
	//Initilization of the variables
	float kp = maxSpeed/maxReading;
	float ki = 0;
	float kd = 0;

	float pSig;
	float iSig;
	float dSig;

	while(inMaze){
		//Takes an error signal of how far away the avc is from the center based of the left wall
		speedPID = (int) (pSig)*kp;
		if (pSig != 0){ //Turn if it is not centered
			set_motor(m1,speedPID);
			set_motor(m2,speedPID);
			//mTurn(dirPID, (pSig)*kd); //Turn accordingly
		}
		else if (pSig == 0){ //If it is centered
			setSpeed(3, speed); //Keep going forward
		}
		//If the readings are both just slightly smaller than the wall distance
		if (takeReading(s1) > dWall-dWallError && takeReading(s2)> dWall-dWallError){
			deadEndTurn(); //Do a deadend turn
			return;//Stop the PID function, so that the deadEndTurn function isn't called multiple times
		}

	}
}

/**
*
*/
int main(){
    InitHardware();
    while(!inMaze){
	mazeCheck();
}
	return 0;
}

