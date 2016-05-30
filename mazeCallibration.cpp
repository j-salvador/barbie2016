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

int main(){
  while(true){
    int s1 = ReadAnalog(0);
    int s2 = ReadAnalog(1);
    printf("Sensor A0:" + s1 + ", Sensor A1" + s2 + "\n");
  }
  return 0;
}
