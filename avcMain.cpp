#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
//Includes the c files in the same repository
#include "networkgate.cpp"
#include "line.cpp"
#include "maze.cpp"
//Keeps the console open on windows for reading print messages
using namespace std;
//Declaration and Initialization of global variables for the avcMain routine
bool done = false;
bool gateDone = false;
bool tapeDone = false;
bool mazeDone = false;

//-----------------------------------------------------------------------------
/**
* @method main
*/
int main(){
    InitHardware(0);//Initilize the RPis hardware
    while (!done){
        gateCheck(); //if (gateDone) {return;} else {runGate}
        if (gateDone){
            tapeCheck(); //if (tapeDone) {return;} else {runTape}
            if (tapeDone){
                mazeCheck(); //if (mazeDone) {return;} else {runMaze}
                if (mazeDone){
                    done = true;
                }
            }
        }
    }
    std::cin.get();
	return 0;
}
