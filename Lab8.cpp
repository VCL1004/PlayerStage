#include <libplayerc++/playerc++.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <fstream>

using namespace PlayerCc;

std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
uint32_t        gPort(60405);

// Constants
#define PI_TWO  3.14159265358979323846  *2.
#define PI  3.14159265358979323846
#define PI_HALF  3.14159265358979323846 *.5

class Collision
{
    public:
        Collision(Position2dProxy* pp, RangerProxy* lp);
    
        bool CheckMovement(double speed, double rotation);
    
        // Give a direction that has "free space to move into
        double DirectionOfFreeSpace(void);
    
    
    private:
        Position2dProxy* pos_proxy
        RangerProxy*  ranger;
}

