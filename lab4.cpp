#include <libplayerc++/playerc++.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
using namespace std;
using namespace PlayerCc;

std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
// NOTE !!! PLAYER_PORTNUM is the port number and has to be unique for each student,
// and be set in the plaer server with -p
uint32_t        gPort(60405); // Replace this with your port number !!

const double PI = 3.14;

void AvoidObstacles(double *forwardSpeed, double *turnSpeed, \
                    RangerProxy &sp)
{
    //will avoid obstacles closer than 40cm
    double avoidDistance = 0.8;
    //will turn away at 60 degrees/sec
    int avoidTurnSpeed = 30;
    //front 0
    //left  1
    //right 2
    //left  3 90
    //right 4 90
    if(sp[3] < 0.4 || sp[1] < 0.4)
    {
        *forwardSpeed = 0;
        //turn left
        *turnSpeed = avoidTurnSpeed;
        
        printf("avoiding obstacle turn left\n");
        return;
    }
    else if(sp[4] < 0.4 || sp[2] < 0.4)
    {
        *forwardSpeed = 0;
        //turn right
        *turnSpeed = (-1)*avoidTurnSpeed;
        
        printf("avoiding obstacle turn right\n");
        return;
    }
    else if( (sp[0] < avoidDistance))
    {
        //back off
        *forwardSpeed = -1;
        *turnSpeed = 90;
        printf("avoiding obstacle back off\n");
        return;
    }
    else if(sp[2] > avoidDistance || sp[1] > avoidDistance || sp[0] > avoidDistance){
        *forwardSpeed = 1;
        *turnSpeed = 0;
    }
    
    return; //do nothing
}

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
    
    double dist, angle, oangle;
    double newturnrate;
    double newspeed;
    try {
        using namespace PlayerCc;
        
        PlayerClient robot(gHostname, gPort); // Conect to server
        
        Position2dProxy pp(&robot, 0);   // Get a motor control device (index is 0)
        
        RangerProxy sonarProxy(&robot, 0);
        sonarProxy.RequestGeom();
        sonarProxy.RequestConfigure();
        
        
        std::cout << robot << std::endl;
        std::cout << sonarProxy << endl;
        
        pp.SetMotorEnable (true); // Turn on Motors
        //FILE* file;
        //file=fopen("test.txt","w");
        
        //double speed = 1, turn = 0;
        
        //srand(time(NULL));
        // go into  a loop
        //double speed,turn,aa;
        //double x,y;
        
        while(dist!=0){
            robot.Read();
            oangle = pp.GetYaw();
            
            dist = sqrt((1 - pp.GetXPos())*
                        (1 - pp.GetXPos()) +
                        (1 - pp.GetYPos())*
                        (1 - pp.GetYPos()));
            
            angle = atan2(1 - pp.GetYPos(),1 - pp.GetXPos()) - oangle;
            
            cout << "oangle : " << oangle << endl;
            cout << "angle : " << angle << endl;
            cout << "distance : " << dist << endl;
            
            if(angle < -3.14159)
                angle = angle + 2 * 3.14159;
            else if(angle > 3.14159)
                angle = angle - 2 * 3.14159;
            
            
            if (dist < 0.01)
                pp.SetSpeed(0, 0);
            
            else if(dist > 1.0)
                pp.SetSpeed(1 , angle);
            else
                pp.SetSpeed(0.01 , angle);
            
            
            
            
        }
    }
    catch (PlayerCc::PlayerError & e) {
        std::cerr << e << std::endl;
        return -1;
    }
}
//Coding by Qiushuang Li(VCL)