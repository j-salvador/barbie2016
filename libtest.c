#include <stdio.h>
#include <time.h>


int init(int d_lev);
int take_picture();
char get_pixel(int row,int col,int colour);
int Sleep(int sec, int usec);


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
