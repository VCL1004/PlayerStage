#include <libplayerc++/playerc++.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include<stdlib.h>
    using namespace PlayerCc;

    std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
    // NOTE !!! PLAYER_PORTNUM is the port number and has to be unique for each student,
   // and be set in the plaer server with -p
    uint32_t        gPort(60405); // Replace this with your port number !!
void random_walk( double &turn )
{     
	int max=8;
	double oldSpeed = turn;

        double pi = 3.1415926535;
        double randnum=(rand()%100-50.0)/100;//(range from -1 to 1)
	randnum = randnum*2*pi;
        double newturn;
	double l = 0.75;
	newturn = l * oldSpeed + ( 1 - l ) * randnum;

	turn = newturn;
}


int main(int argc, char **argv)
{
    try {

    PlayerClient robot(gHostname, gPort); // Conect to server
    Position2dProxy pp(&robot, 0);   // Get a motor control device (index is 0)

    RangerProxy sonarProxy(&robot, 0);
        sonarProxy.RequestGeom();
        sonarProxy.RequestConfigure();
    std::cout << robot << std::endl;

    pp.SetMotorEnable (true); // Turn on Motors
    //FILE* file;
    //file=fopen("test.txt","w");
      

    srand(time(NULL));
    // go into  a loop
    //double speed,turn,aa;
    //double x,y;
  for(;;){
        robot.Read();
   random_walk( turn );
	
   pp.SetSpeed( 2, turn );
   x=pp.GetXPos();
   y=pp.GetYPos();
   fprintf(file,"%f,%f \n",x,y);
   printf("%d",rand());



           
     
    }
  }
  catch (PlayerCc::PlayerError & e) {
    std::cerr << e << std::endl;
    return -1;
  }
}
//Coding by Qiushuang Li(VCL)