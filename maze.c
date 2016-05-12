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
	int m1,m2;
	int s1,s2;
	bool inMaze;

	//Initialization of global variables
	speed = 30; //Sets the speed through out the maze to 30
	turnSpeed = 10;
	dWall = 50; //Sets the average wall Distance when the robot is sensored, ie dLeft = dRight at facing foward.
	inMaze = false; //The AVC doesn't start in the maze, when it detects it is, this will trigger.
	dWallError = 5;
	
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
		if(motor == 1){
			//The first motor rotates in the expected direction
			set_motor(motor , vel); //Changes the 1st motors speed
		}

		else if (motor == m1){
			//The second motor rotates in the opposite direction to the 1st, speed is therefore negative.
			set_motor(vel , -1*vel); //Changes the 2nd motors speed
		}
		else if (motor == m2){
			//If both motors are to go forward at the same speed
			set_motor(m1 , vel); //Changes the first motors speed
			set_motor(m2 , -1*vel); //Changes the second motors speed
		}
	}

	/**
    * @param motor
    * @param vel
    */
	void speed(int motor, int vel){
		//Declaration of variables
		float speedChange, temp, velError;
		//Initialization of variables
		speedChange = 1;
		velError = 5;
		//Checks if the AVC is going slower than the set speed
		if (speed < vel){
			speedChange = speedChange //This constant is used in the loop to increase the speed
		}
		//Checks if the AVC is going faster than the set speed
		if (speed > vel){
			speedChange = -1* speedChange; //This constant is used in the loop to decrease the speed
			temp = vel; //Stores the desired velocity temporarily since it is overwritten
			//Swaps the velocity with the speed, so that the conditions for the loop aren't broken
			vel = speed; 
			speed = temp;
		}
		//Increases or decreases the speed until it reaches the desired speed
		for (speed; speed < vel; speed += speedChange){ 
			temp = speed;
			speed = vel;
			vel = speed;
			setSpeed(motor, speed); //Sets the motors speed up to the desired velocity 
			Sleep (0 ,010000); //Small breaks to makes sure the motor doesn't stress itself
			temp = vel;
			vel = speed 
			vel = temp;
		}
		//If the speed if is near enough the desired velocity it will maintain constant speed
		if (speed == vel + velError || speed == vel - velError){
			setSpeed(vel);
		}
	}
	/**
	* @param dir: this character takes the values L and R for left and right respectively
	* @method mTurn
	*/
	void mTurn(char dir){
		speed(3, turnSpeed);
		if (dir == "L"){
			speed(m1, -1*turnSpeed);
			speed(m2, turnSpeed);
		}
		if (dir == "R"){
			speed(m1, turnSpeed);
			speed(m2, -1*turnSpeed);
		}
	}

	//ONE DIRECTION ONLY
	void LROnly(char dir){
		//Declaration of local varibales
		String oppDir;
		//Initialization of local variables
		if (dir == "L"){
			oppDir = "R";
		}
		else if (dir == "R"){
			oppDir = "L";
		}
		//When turning the dir & oppDir distance will be larger and smaller than the wall distance respectively
		while(takeReading(dir) > wallDist || takeReading(oppDir) > wallDist){
			mTurn(dir);//Continues to turn right until a break condition or the while loop condition is triggered.
			//Break when wall distance is equal.
			if (takeReading(dir) == wallDist && takeReading(oppDir) == wallDist){
				break;
			}
			//Break if the avc turns too far in the direction
			else if (takeReading(dir) <= takeReading(oppDir)){
				break;
			}
		}
	}

	//FORWARD + TURN
	void fowardOrTurn(char dir){
		//Declaration of local variables
		char oppDir;
		//Initialization of local variables
		if (dir == "L"){
			oppDir = "R";
		}
		else if (dir == "R"){
			oppDir = "L";
		}
		while (takeReading(dir) > wallDist && takeReading(oppDir) != wallDist){
			if (takeReading(dir) < wallDist){
				break;
			}
			mTurn(dir);
		}
	}

	//FORWARD ONLY
	void forwardOnly() {
		//If both the walls are equal distance apart just move forward.
		while (takeReading("L") > wDist == takeReading("R")> wDist){
			setSpeed(3 , speed);
		}
	}

	//BACK ONLY
	void backOnly(){
		//If both of the sensors take a distance less than the wall
		while(takeReading("L") > wDist && takeReading("R")> wDist){
			//Breaks when both of the readings are equal
			if (takeReading("L") == wallDist && takeReading("R") == wallDist){
				break;
			}
			mTurn("R");//Turns to the right in the maze
		}
	}

	//LEFT + FORWARD + RIGHT
	void leftFrwdRight(){
		while (takeReading("L") > wallDist && takeReading("R") != wallDist){
			if (takeReading("L") == wallDist){
				break;
			}
			mTurn("L");
	}

	//Need to add methods that recognise the wallside and then adjust accordingly

	//void navigateMaze(){}




	return 0;
}
