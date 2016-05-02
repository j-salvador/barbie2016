#include <stdio.h>
#include <time.h>


extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);

int main(){

    init(0);
    char c;
    int i = 1;
    while(i<100){

       take_picture();

       c = get_pixel(160,120,3);

       printf("%d\n",c);

       Sleep(0,500000);
       i = i+1;
       }
return 0;}
