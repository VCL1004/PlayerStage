//Name: Qiushuang Li(VCL)
//Student#: 1206650
#include <libplayerc++/playerc++.h>
#include <iostream>

    using namespace PlayerCc;

    std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
    // NOTE !!! PLAYER_PORTNUM is the port number and has to be unique for each student,
   // and be set in the plaer server with -p
    uint32_t        gPort(66650); // Replace this with your port number !!



int main(int argc, char **argv)
{

  // we throw exceptions on creation if we fail
  try {

    PlayerClient robot(gHostname, gPort); // Conect to server
    Position2dProxy pp(&robot, 0);   // Get a motor control device (index is 0)

// Speed and turn settings
       double turn_rate = 0.889;
       double move_speed = 0.27;
       
       timespec move_sleep = { 5, 0 };
       timespec turn_sleep = { 3, 0 };
       timespec stop_sleep = { 10, 0 };
    
       
       robot.Read();
    
       // move 1
       pp.SetSpeed(move_speed, 0);
       nanosleep(&move_sleep, NULL);
       //turn 1
       pp.SetSpeed(0, turn_rate);
       nanosleep(&turn_sleep, NULL);
    
       // move 2
       pp.SetSpeed(move_speed, 0);
       nanosleep(&move_sleep, NULL);
       // turn 2
       pp.SetSpeed(0, turn_rate);
       nanosleep(&turn_sleep, NULL);
    
       // move 3
       pp.SetSpeed(move_speed, 0);
       nanosleep(&move_sleep, NULL);
       // turn 3
       pp.SetSpeed(0, turn_rate);
       nanosleep(&turn_sleep, NULL);
    
       // move 4
       pp.SetSpeed(move_speed, 0);
       nanosleep(&move_sleep, NULL);
       // turn 4
       pp.SetSpeed(0, turn_rate);
       nanosleep(&turn_sleep, NULL);
    
       // Set motor stop command and wait so they can propagate
       //pp.SetSpeed(0, 0);
       //nanosleep(&stop_sleep, NULL);
} //end try
  catch (PlayerCc::PlayerError & e) {
    std::cerr << e << std::endl;
    return -1;
  }
}
// Coding by Qiushuang Li(VCL)