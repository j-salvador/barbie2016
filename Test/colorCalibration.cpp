#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);


int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(0);
    char c;
    int pixCord[32]; //Initializes the array Size
    int x;
    int pixelColor;
    float avgColor;
    float totAvgColor;
    float finalAns;

    while(x <1000){ //Infinite Loop, every cycle analyzes one single frame.
    	pixelColor = 0;
    	avgColor = 0;

       take_picture();
       //The Array sets Color values for the Pixels (for every 10 pixels across, starting from 0)

       //For Example: pixCord [0] = 0 pixCord[1] = 10... PixCord[32] = 320

       for (int i = 0; i < sizeof(pixCord); i++){
    	   pixCord[i] = get_pixel(i*10,120,3);
    	   pixelColor = pixelColor + pixCord[i];
       }
       avgColor = pixelColor/sizeof(pixCord); //calculates the average color of
       totAvgColor = totAvgColor+ avgColor;
       printf("%f\n", avgColor); //Prints average Pixel Color of one single Frame
       Sleep(0,020000);
       x++;
    }
    finalAns = totAvgColor/1000;
    printf("The Total average Color of 1000 Frames is: %f\n", finalAns);


return 0;}
