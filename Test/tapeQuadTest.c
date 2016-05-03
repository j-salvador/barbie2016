#include <stdio.h>	//import libraries
#include <stdbool.h>
//Still need to import AVC related ones
//Need to add code to realise when the tape is finished/started
int main(){
	bool tapeQuadrant = true; //Is the AVC in the tape quadrant
	/**
	*  The tapeTake function simply measures a row of pixels
	*	and returns a sum which is representative of where
	*	the clusters of white pixels are.
	*
	*	#Could take more than one row of pixels for error reduction#
	*/
	int tapeTake(){
		take_picture(); 	//Takes a picture on the camera
		int sum = 0;   		//Initialize variables
		int i = -160;   	//Pictures are 320 pixels wide, makes offset 0
		int w;				//Initializes w
		for (i=0; i<160; i++){
			w = get_pixel(120,i,3); //Gets the pixel at this location
			sum = sum + i*w; 		//Error signal for location of the white line
		}
		return sum;
	}
	//Function to make the robot move forward
	void forward(){
		//Move forward
	}
	//Function to make the robot move left or right
	void turn(int dir){//-1 is left, 1 is right
		if (dir == -1){
			//Turn left
		}
		else if (dir == 1){
			//Turn right
		}
	}
	//Function to make the robot turn 90 degrees left or right
	void tapeTurn90(int dir){	//-1 left, 1 is right
		if (dir == -1){ //If the direction is right
			while (tapeTake()<0){	//While the tape is still on the left
				turn(-1);
			}
		}
		else if (dir == 1){ //if the direction is left
			while (tapeTake()>0){	//While the tape is still on the right
				turn(1);
            }
		}
	}
	/**
	* the tapeCheck() functions checks the values for the tapeTake() function.
	* These needed to be separated because the turnRight() and turnLeft() functions
	* both needed to take pictures from the camera in order to see how far through
	* the turn the robot is.
	*
	*/
	void tapeCheck(){
		int turnSum = 100; 	//The sum at a corner is about this #FIND VALUE#
		int sum = tapeTake(); //Takes a picture with the camera
		if (sum<turnSum || sum>turnSum*(-1)){	//Checks if the tape is going forward
			if (sum>0){			//If the tape is to the right of the robot
				turn(-1);		//Turns to the left
			}
			else if (sum<0){	//If the tape is to the left of the robot
				turn(1);		//Turns to the right
			}
			else{				//If none of these cases are true go straight.
				forward(); 		//Continue moving forward
			}
		}
		else { //If it is at a corner
			if (sum<turnSum){//If the tape line is to the right
				tapeTurn90(1);	//Performs a 90 degree turn right
			}
			else if (sum>turnSum*(-1)){//if the tape line is to the left
				tapeTurn90(-1);	//Performs a 90 degree turn left
			}

		}
	}

	while (tapeQuadrant == true){ //Checks if the tape part of the maze is finished.
		tapeCheck(); //Calls the tape check function
	}
	return 0;
}
