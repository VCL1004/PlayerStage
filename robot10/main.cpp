#include <libplayerc++/playerc++.h>
#include <iostream>
#include <math.h>

#include "Robot.h"


using namespace PlayerCc;

std::queue<Behaviors*> behav_queue;

std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);


// We should impletent the -p arument for port here (I show this another time)
int main(int argc, char **argv)
{
  try {

    PlayerClient r_server(gHostname, 60405 ); // Conect to server

    Network* my_robot;
    MovePointRobot* my_robot2;
      
    my_robot=new Network(&r_server,0 ,1);
    my_robot2=new MovePointRobot(&r_server,1 ,1);
    std::cout << "1" << std::endl;
    r_server.Read(); // Start Data flow
    r_server.Read(); // Start Data flow

    my_robot->Init();
    my_robot2->Init();
    double x;
    double y;

    for(;;){
	x = my_robot->GetX();
	y = my_robot->GetY();

	// this blocks until new data comes; 10Hz by default
	r_server.Read();

	my_robot->Tick();
	my_robot2->toPoint(x,y);
	my_robot2->Tick();

    }

  } catch (PlayerCc::PlayerError & e){
    std::cerr << e << std::endl;
    return -1;
  }
}

