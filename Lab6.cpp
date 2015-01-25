
#include <libplayerc++/playerc++.h>
#include <iostream>
#include <queue> //
#include <stdio.h>
#include <fstream>
#include <string>
using namespace PlayerCc;

std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
// NOTE !!! PLAYER_PORTNUM is the port number and has to be unique for each student,
// and be set in the plaer server with -p
uint32_t        gPort(60405); // Replace this with your port number !!



int main(int argc, char **argv)
{
    int a;
    int b;
    double xpos;
    double ypos;
    double distance;
    double angle;
    double oangle;
    double aa,bb;
    FILE* input;
    // we throw exceptions on creation if we fail
    std::queue<double> qx;
    std::queue<double> qy;
    std::ifstream points;
    points.open("points.txt");
    while (points>>aa>>bb)
    {qx.push(aa);
        qy.push(bb);
        
    }
    printf(" %f \n",bb);
    
    
    try {
        
        PlayerClient robot(gHostname, gPort); // Conect to server
        Position2dProxy pp(&robot, 0);   // Get a motor control device (index is 0)
        
        std::cout << robot << std::endl;
        
        pp.SetMotorEnable (true); // Turn on Motors
        
        
        // go into  a loop
        while(distance!=0)
        {
            
            
            a=0;
            
            robot.Read();
            
            oangle=pp.GetYaw();
            distance=sqrt((qx.front()-pp.GetXPos())*(qx.front()-pp.GetXPos())+(qy.front()-pp.GetYPos())*(qy.front()-pp.GetYPos()));
            
            angle=atan2(qy.front()-pp.GetYPos(),qx.front()-pp.GetXPos())-oangle;
            
            if(angle< -3.14159){
                angle=angle+ 2*3.14159;
            }
            else if(angle>3.14159)
            {
                angle= angle-2*3.14159;
            }
            
            //printf("%f \n",distance);
            //printf("%f \n",angle);
            if(distance<0.05){
                pp.SetSpeed(0,0);
                qx.push(qx.front());
                qy.push(qy.front());
                qx.pop();//
                qy.pop();//
            }
            else if(distance>1)
                pp.SetSpeed(0.5,angle);
            else
                pp.SetSpeed(0.05,angle);
        }
        
    }
    catch (PlayerCc::PlayerError & e) {
        std::cerr << e << std::endl;
        return -1;
    }
}
//coding by VCL(Qiushuang Li)