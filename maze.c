#include <stdio.h>	//import libraries
#include <stdbool.h>
#include <pixel.c>
#include <time.h>
#include <math.h>
#include <bool.h>


//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);	//Sets up the hardware on the AVC for c and c++ compatibility
extern "C" int take_picture();	//Allows us to access the AVC's camera in our methods.
extern "C" char get_pixel(int row,int col,int color); //Allows us scan every pixel of the picture taken, in 3 different modes
extern "C" int Sleep(int sec, int usec); //Lets the computer sleep for set amounts of time
extern "C" int set_motor( int motor , int speed); //Changes a motors speed
extern "C" char ReadAnalog(int ch_adc);//Takes a reading from the ADC at the analog channel selected

//extern "C" int set_PWM( int chan , int value ); 
//extern "C" char set_pixel(int row, int col,char red,char green, char blue);

/**
* 
*/
int main(){
	//Declaration of global variables
	float speed, turnSpeed, dLeft, dRight, dWall,dWallError;
	int m1,m2; //Stores the motors in variables so they can easily be changed if this is wrong.
	int s1,s2; //Stores the sensors in varibales so they can easily be changed around
	bool inMaze; //Boolean for storing if the robot is in the maze
	bool readyPWM; //Boolean for storing if the robot's engines are warmed up

	//Initialization of global variables
	speed = 30; //Sets the speed through out the maze to 30
	turnSpeed = 10; //Selected speed for when the robot is turning at a deadend, may want to make it slow down towards the end
	dWall = 50; //Sets the average wall Distance when the robot is sensored, ie dLeft = dRight at facing foward.
	inMaze = false; //The AVC doesn't start in the maze, when it detects it is, this will trigger.
	dWallError = 5; //The acceptable error value, that the PID can handle for dead end turns and correct.
	readyPWM = false; //Initially set to false, since it will be at rest.
	
	//Sets the motors as global variables so they can be changed easily if the order is changed
	m1 = 1; 
	m2 = 2;

	/**
	* @param anologChannel
	* @method takeReading
	*/
	void takeReading(int analogChannel){
		if (analogChannel == s1){ //The first sensor s1, will take the dLeft readings
			dLeft = ReadAnalog(analogChannel); //Sets the dLeft global variables to the value of the IR sensor
		}
		if (analogChannel == s2){ //The second sensor s2, will take the dRight readings
			dRight = ReadAnalog(analogChannel) //Sets the dRight global variables to the value of the IR sensor
		}
	}

	/**
	* @param motor
	* @param speed
	* @method setSpeed
	*/
	void setSpeed(int motor ,int vel){
		if(motor == m1){
			//The first motor rotates in the expected direction
			set_motor(motor , vel); //Changes the 1st motors speed
		}

		else if (motor == m2){
			//The second motor rotates in the opposite direction to the 1st, speed is therefore negative.
			set_motor(vel , -1*vel); //Changes the 2nd motors speed
		}
		else if (motor == 3){
			//If both motors are to go forward at the same speed
			set_motor(m1 , vel); //Changes the first motors speed
			set_motor(m2 , -1*vel); //Changes the second motors speed
		}
	}
	/**
	* @param dir: 
	* @param setSpeed
	* @method mTurn
	*/
	void PWM(char dir, int setSpeed){
		//Handles acceleration and turning
	}

	/**
	* @param dir: this character takes the values L and R for left and right respectively
	* @method mTurn
	*/
	void mTurn(char dir, int setSpeed){
		if (readyPWM == false){ //Checks if the motor is accelerating
			PWM(dir, setSpeed);	//Calls the PWM funciton if the motor is accelerating
		}
		else if (readyPWM == true) { //If the motor is warmed up
			if (dir == "L"){ //Checks if the direction is left
				//Sets the speed according to one wheel being backwards
				setSpeed(m1, -1*setSpeed);
				setSpeed(m2, setSpeed);
			}
			if (dir == "R"){ //Checks if the direction is right
				//Sets the speed opposite to the previous statement
				setSpeed(m1, setSpeed);
				setSpeed(m2, -1*setSpeed);
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
			while (takeReading("L") > wDist && takeReading("R")> wDist){
				readyPWM == false; //Sets the PWM to false, since it will decelerate to a stop, and then reverse
				setSpeed (3, -1*speed); //Buts both motors turning the opposite way
			}
			readyPWM = false; //Sets the PWM since it will come to rest
			setSpeed(3, 0); //Sets the speed to 0, making it come to rest
			//Turns the avc just enough so that the left sensor is always less than the wall distance until the turn in complete
			for (int i = 0; i<turnThreshold; i++){
				mTurn("R", turnSpeed);//Turns right, so the left sensor is always smaller than the wall distance
			}
			//Continues to turn right until the wall distance is greater than or equal to the left reading.
			while (takeReading("L") > wDist){
				mTurn("R", turnSpeed);
				if (takeReading("L") <= wDist){
					return; //Exits the loop, when the avc has completed the full 180 degree turn or more.
				}
			}
		}
	}

	void mazePID(){
		///Declaration of the variables
		float kd, ki, kd, pSig, iSig, dSig;
		int speedPID; //Stores the intended speed for the avc based of the
		char dirPID; //Stores the intended direction for the avc
		//Initilization of the variables
		kd = maxSpeed/maxReading;
		ki = 0;
		kd = 0;

		while(inMaze){
			//Takes an error signal of how far away the avc is from the center based of the left wall
			pSig = takeReading("L") - dWall; //makes 0 the value when the avc is centered.
			if (pSig > 0){ //If the avc is slightly to the right of perpendicular to the wall
				dirPID = "L"; //Change the direction to left
			}
			else if (pSig < 0){ //If the avc is slight to the left of perpendicular to the wall
				dirPID = "R"; //Change the direciton to right
			}
			if (pSig != 0){ //Turn if it is not centered
				set_motor(m1,(pSig)*kd );
				set_motor(m2, -1*(pSig)*kd);
				//mTurn(dirPID, (pSig)*kd); //Turn accordingly
			}
			else if (pSig == 0){ //If it is centered
				setSpeed(3, speed); //Keep going forward
			}
		}
	}

	
	return 0;
}
