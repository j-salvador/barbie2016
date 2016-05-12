#include <stdio.h>	//import libraries
#include <stdbool.h>
#include <pixel.c>

int main(){
	double wallDist = 50; //To be measured the distance the IR sensor picks up when the robot is centered by the wall.
	bool inMaze = false; //Initially the robot is not in the maze
	double distTravelled = 0; //Distance travelled, for helping with 90 degree turns
	double mazeTakeSumTop = 500; //Reading from the camera when the robot is in the maze.

	//The dist function has the parameter dir, the direction of the sensor you wish to measure
	double dist(int dir){
		double x;
		if (dir == -1){
			x = ReadAnalog(0); //Take the reading from the left sensor
		}
		else {
			x = ReadAnalog(1); //Take the reading from the right sensor
		}
		return x; //Return the value of the reading
	}

	int mazeTake(double height){
		take_picture(); 	//Takes a picture on the camera
		int sum = 0;   		//Initialize variables
		int i = 0;   	//Pictures are 320 pixels wide, makes offset 0
		int w;				//Initializes w
		for (i=0; i<320; i++){
			w = get_pixel(120,height,3); //Gets the pixel at this location
			sum = sum + i*w; 		//Error signal for location of the white line
		}
		return sum;
	}

	void mazeForward(){
		//Moves the AVC forward
		distTravelled += 1; //Add to the distance travelled each time the robot moves forward
	}
	//The turn function, has the parameter dir, the direction to turn
	void mazeTurn(int dir){
		if (dir == -1){
			//turn left
		}
		if (dir == 1){
			//turn right
		}
	}
	//The mazeTurn90() function has the parameter dir, the direction you wish to turn
	void mazeTurn90(int dir){
		if (dir == -1){
			while (dist(-1) > distTravelled){
				turn(dir); //Turns to the left direction
			}
		}
		if (dir == 1){
			while(dist(-1) > wallDist){
				turn(dir);  //Turns to the right direction
			}
		}
		distTravelled = 0; //Reset the distance travelled for each time it turns
	}
	//This function makes the decisions while navigating the maze.
	void navigateMaze(){
		if (dist(-1) <= wallDist && dist(0) > dist(-1)){
			mazeForward();
		}
		if (dist(-1) == dist(0)){
			mazeTurn90(1);
		}
		if (dist(-1)< dist(0) && dist(-1) > wallDist){
			mazeTurn90(-1);
		}
	}
	//This function continually runs, checking if the robot is in the maze.
	while (true){
		if(dist(-1) <= 4 * wallDist || dist(0) <= 4 * wallDist){ //Checks if either of the sensors can register the walls

			inMaze = true; //Tells the robot it is in the maze
		}
		while (inMaze == true){ //Runs while the robot is in the maze
			navigateMaze(); //Calls the navigate the maze funciton.
		}
	}
	return 0.0;
}