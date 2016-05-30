#include <stdio.h>
#include <time.h>
#include <math.h>
//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int color);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor( int motor , int speed);

int main(){
  init(0);
  int r = 0;
  int l = 0;
  double current_error = 0;
  double lastCurrent_error = 0;
  double horizontal_error = 0;
  int s;
  int top;
  int bottom;
  bool quad2 = true;
  bool quad3 = false;
  int maxSpeed = 40;
  int VL;
  int VR;
  int iCount = 0;
  bool gate = true;
  bool followLine = false;
  bool maze = false;
  bool Correction = true;
  double P = 0;
  double kp = 0.27;
  double avgC = 0;
  bool line = true;
  while(gate){
    //gate code to open gate

    //once gate is open:
    gate = false;
    followLine = true;
  }
  while(followLine){ // Main Loop ---To follow line
    take_picture();
    l = 0;
    r = 0;
    top = 0;
    bottom = 0;
    current_error = 0;
    horizontal_error = 0;
    Correction = true;

    //--------------------------------------Analyzing Image-------------------------
        	for (int i = 0; i < 320; i++){
        	   int row = get_pixel(i,120,3);
        	   if(row < 127){ //If pixel is close to black ------ (Reduces noise)
        		   s=0;
        	   }else {
        		   s=1;
               Correction = false;
               if(i<160){
                 l++;
               }else{
                 r++;
               }
             }
        	   current_error = current_error + (i-160)*s; //Adds to current_error if its a white pixel
        	}//Closes For Loop
          //------Scans 2nd 1.
          for (int i = 0; i < 240; i++){

        	   int col = get_pixel(180,i,3);
        	   if(col < 127){ //If pixel is close to black ------ (Reduces noise)
        		   int s = 0;
        	   }else {
        		   int s = 1;
               if(i<120){
                top++;        //line
              }else{
                bottom++;
              }
             }
        	   horizontal_error = horizontal_error + (i-120)*s; //Adds to current_error if its a white pixel
        	}//Closes For Loop

//--------------------------------Analyzing Image Finish------------------------

if(quad2){
  //quad2 error correction if it loses the line

  //I've added under T intersection, left corner and right corner that
  //quad 2 switchs to false if one of those are detected.
if(current_error == 0 && Correction){
  iCount++;
}else{
  iCount = 0;
}

  if(iCount>0){ //Debugging
    printf("Count = %d (line has been missing for %d FPS)\n",iCount,iCount);
  }
    if( current_error<0 || current_error>0 ){
        lastCurrent_error = current_error;
    }
    if(iCount>7){ //Line has been lost for a while. Run Correction
      set_motor(1,-VL);
      set_motor(2,VR);
      Sleep(0,300000);

      if(current_error == 0){
        VL = maxSpeed;
        VR = maxSpeed;
        if(lastCurrent_error>0){
          set_motor(1,-VL);
          set_motor(2,-VR);
          Sleep(0,050000);

        }else if(lastCurrent_error<0){
          set_motor(1,VL);
          set_motor(2,VR);
          Sleep(0,050000);

        }
      }
    }//Closes iCount if statement
}
//-------------------------------Quad 3 Trigger---------------------------------
if(l >155 && r >155 && bottom >115){ //T Intersection
  //Choose left path
  //turn left

  quad2 = false;
  quad3 = true;
}else if(l >155 && bottom >115){ //Left Corner
  //turn left by fixed amount
  printf("LEFT\n");
  set_motor(1,0);
  set_motor(2,0);
  Sleep(1,000000);
  set_motor(1,-VL);
  set_motor(2,-VR);
  Sleep(0,400000);
  set_motor(1,0);
  set_motor(2,0);
  Sleep(1,000000);
  quad2 = false;
  quad3 = true;
}else if(r >155 && bottom >115){ //Right Corner
  //turn right by fixed amount
  printf("RIGHT\n");
  set_motor(1,0);
  set_motor(2,0);
  Sleep(1,000000);
  set_motor(1,VL);
  set_motor(2,VR);
  Sleep(0,400000);
  set_motor(1,0);
  set_motor(2,0);
  Sleep(1,000000);
  quad2 = false;
  quad3 = true;
}else if(quad3 && current_error == 0){ //deadEnd
//Turns 180* degrees if it loses the line and is in quad3
  printf("DeadEnd\n");
  set_motor(1,0);
  set_motor(2,0);
  Sleep(1,000000);
  set_motor(1,VL);
  set_motor(2,VR);
  Sleep(0,600000);
  set_motor(1,0);
  set_motor(2,0);
  Sleep(1,000000);
}else{ //goes straight if none Detected
  line = true;
}
//-------------------------------Quad 3 Trigger FINSIH--------------------------

//---------------------------------------Error Values---------------------------
    	avgC = avgC + (current_error/320);
    	P = (current_error/320)*kp;
//------------------------------------------------------------------------------



//----------------------------------------Motor Control------------------------
if(line){
  VL = maxSpeed - (P);
  VR = maxSpeed + (P);
  set_motor(1,VL);
  set_motor(2,-VR);
}
//--------------------------------------Detects Maze--------------------------
if(maze){ //If we reach maze this will be used to switch to maze while loop
  followLine = false;
  maze = true;
}

//------------------------------------------------------------------------------
        }//---Closes Main While Loop
while(maze){
  //solve maze easy... :D
}

      }//Closes Int Main
