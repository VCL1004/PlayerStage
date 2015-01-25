//Most of these code are based on Dr. M.V.M's solution for Lab2
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

//Macros
#define Square(x) ((x)*(x))

class Behaviors
{

        public:
                Behaviors(Position2dProxy* pp); // Constructor (Default constructor is private)
                virtual void Init(void){}; // Overwritten by childeren to given them information
                virtual bool Tick(void) =0; //  Called every frame, true is Behavior complete
                virtual void Resume(void){}; // Resume after other behavior was done
                double GetIntendedSpeed(){return(speed);};
                double GetIntendedRotation(){return(rotation);};

                void DoMove(void); //Perform the intended move (not overwriten by childen !!)

        protected:
                double speed,rotation; // The intended speed in this frame
                Position2dProxy* posProxy; // The position proxy to obain information !!

        private:
                Behaviors(){}; // Private !!

};

Behaviors:: Behaviors(Position2dProxy* pp)
{
	posProxy=pp;
	speed=0.;
	rotation=0.;
}

void Behaviors::DoMove(void)
{
	posProxy->SetSpeed(speed, rotation);
}



//----------------------------------------MoveStraight--------------------------------------//

class MoveStraight : public Behaviors
{
	public:
		MoveStraight(Position2dProxy* pp);
		~MoveStraight();
		virtual void Init(double d);
		virtual bool Tick(void);
	private:
		double goal_x;
		double goal_y;
		double distance;
};

MoveStraight::MoveStraight(Position2dProxy* pp):Behaviors(pp)
	
{
  goal_x=0.;
  goal_y=0.;
  distance=0.;
}

void MoveStraight::Init(double d)
{
  distance=d;
  goal_x=posProxy->GetXPos()+ d* cos(posProxy->GetYaw());
  goal_y=posProxy->GetYPos()+ d* sin(posProxy->GetYaw());

  printf(" Straight Move to %f %f \n" ,goal_x,goal_y);
}

#define STRAIGH_MAX_SPEED .5
#define STRAIGH_CRAWL_SPEED .1
#define STRAIGHT_TOLERANCE 0.2


bool MoveStraight::Tick(void)
{
        // How far do we still have to go?      
        double dist = sqrt( Square((goal_x - posProxy->GetXPos()))+
                            Square((goal_y - posProxy->GetYPos())));

        if(dist>1.) // Proportional speed control
                speed=STRAIGH_MAX_SPEED;
        else
                speed= dist* STRAIGH_MAX_SPEED +STRAIGH_CRAWL_SPEED;

	rotation=0.;
	
        //posProxy->SetSpeed(speed,0.);


        if(dist<STRAIGHT_TOLERANCE)
                return(1);
        else
                return(0);
}




//-------------------------------------//
#define TURN_MAX_SPEED 1. 
#define TURN_CRAWL_SPPED .001
#define TURN_TOLERANCE  .1


class Turn : public Behaviors
{
        public:
                Turn(Position2dProxy* pp);
                ~Turn();
                virtual void Init(double turn_to);
                virtual bool  Tick(void);
        private:
		double  turn_to;
		double angle;
                

};

Turn::Turn(Position2dProxy* pp):Behaviors(pp)
{
	turn_to=0.;
	angle=0.;
}

void Turn::Init(double a)
{
	angle=a;
	turn_to=posProxy->GetYaw() +a;
	if(turn_to>PI)turn_to=turn_to-PI_TWO;
	printf(" Turn Turn to %f \n",turn_to);
}

bool Turn::Tick(void)
{
        // Note, we do not  check for overshoot here!!
        double error=fabsf(turn_to-posProxy->GetYaw());
        double turn_rate;
        if(error>1.) // Proportional turn speed control
                turn_rate=TURN_MAX_SPEED;
        else
                turn_rate=TURN_MAX_SPEED*error+TURN_CRAWL_SPPED;


	speed=0.;
        rotation=turn_rate;
        //posProxy->SetSpeed(0.,turn_rate);

        if(error<TURN_TOLERANCE)
                return(1);
        else
                return(0);
}


//-------------------------------------------------------//
#define ROBOT_SPEED .3

class Random : public Behaviors
{
	public:
			Random(Position2dProxy* pp);
			~Random();
			virtual bool Tick();

	private:
			double x;
			double y;
			double a;
			int i;
};

Random::Random(Position2dProxy* pp):Behaviors(pp)
{
	// Initialize Random Seed
	 srand(time(NULL));
	 rotation=0;
}

bool Random::Tick(void)
{
	const int rand_num =rand();
	rotation= .9*rotation+ 0.007 * ((double)( 100 - ( rand_num % 200)));
	speed=ROBOT_SPEED;
	

	return(0); 
}

//-------------------------------------------------------//
class MovePosition : public Behaviors
{
	public:
			MovePosition(Position2dProxy* pp);
			~MovePosition();
			virtual bool Tick();
			virtual	void Init(player_pose2d_t tar){
			newspeed = 0.;
			target = tar;

			};

	private:
			#define DIST_TOL .1
			#define MAX_SPEED .2
			#define MAX_TURN 40.0 /* this is 40 degrees per second */
			double dist, angle, yaw;
			player_pose2d_t target;
			double ang_error, newspeed, newturnrate;
	
	protected:
			double angle_normal( double a )
		{
         		while( a < -M_PI ) a += 2.0*M_PI;
        		while( a >  M_PI ) a -= 2.0*M_PI;
         		return a;
		}
	

};

MovePosition::MovePosition(Position2dProxy* pp):Behaviors(pp)
{
}

MovePosition::~MovePosition()
{
}

bool MovePosition::Tick()
{
  dist = sqrt(Square(target.px -posProxy->GetXPos())+
	Square(target.py - posProxy->GetYPos()));

  angle = atan2(target.py - posProxy->GetYPos(),
                target.px - posProxy->GetXPos());

  yaw=posProxy->GetYaw();

  double ang_error=angle_normal(angle-yaw);

  newturnrate = limit(rtod(ang_error), -MAX_TURN,MAX_TURN );
  newturnrate = dtor(newturnrate);

  if (dist > DIST_TOL) {
    newspeed = limit(dist * MAX_SPEED, -MAX_SPEED, MAX_SPEED);
	std::cout<<newspeed<<std::endl;
  } else
    newspeed = 0.0;

  if (fabs(newspeed) < 0.01){ 
    newspeed = 0; newturnrate = 0;
    std::cout << "REACHED GOAL "<<std::endl;
    return(1);
  }

	
  return(0);

}
//--------------------------------------------------------//

class MoveNetwork : public Behaviors
{
	public:
		MoveNetwork(Position2dProxy* pp);
		~MoveNetwork();
		bool Tick();
		void Init();
	private:
		std::queue<double> points;
		double nextTarget();
		int steer(player_pose2d_t target);

	protected:
			double angle_normal( double a )
		{
         		while( a < -M_PI ) a += 2.0*M_PI;
        		while( a >  M_PI ) a -= 2.0*M_PI;
         		return a;
		}
};


MoveNetwork::MoveNetwork(Position2dProxy* pp):Behaviors(pp)
{
}

MoveNetwork::~MoveNetwork()
{
}

void MoveNetwork::Init()
{
	std::queue<double> points;
	std::ifstream readFile("myfile.txt");

	double a,b;

	while (!readFile.eof())
 
	{
		readFile>> a >>b;
		points.push(a);
		points.push(b);
	}

	readFile.close();
}

double MoveNetwork::nextTarget()
{
	double value;

	points.push(points.front());
	points.pop();
	value = points.front();

return value;
}

int MoveNetwork::steer(player_pose2d_t target)
{
  using namespace PlayerCc;

  double dist, angle;

  dist = sqrt(Square(target.px - posProxy->GetXPos())+
	Square(target.py - posProxy->GetYPos()));
  angle = atan2(target.py - posProxy->GetYPos(),
                target.px - posProxy->GetXPos());

  double yaw=posProxy->GetYaw();

  double ang_error=angle_normal(angle-yaw);
  double newspeed=0.;
  double newturnrate = limit(rtod(ang_error), -MAX_TURN,MAX_TURN );
  newturnrate = dtor(newturnrate);

   
  if (dist > DIST_TOL) 
	newspeed = limit(10.*dist * MAX_SPEED, -MAX_SPEED, MAX_SPEED);
   else

	speed = newspeed;
	rotation = newturnrate;

	return(1);
	
	
  return(0);
}

bool MoveNetwork::Tick()
{
	player_pose2d_t target;

	target.px = points.front();
	target.py = nextTarget();
	target.pa = 0;
    
    	for(;;)
	{
	steer(target);
	
		if (steer(target) == 1) 
		{
		target.px = nextTarget();
		target.py = nextTarget();	
		}
	}
return(0);		
}
//--------------------------------------------------------//

class Wait : public Behaviors
{
	public:
			Wait(Position2dProxy* pp);
			~Wait();
			virtual bool Tick();

	private:

			
	protected:
	

};

Wait::Wait(Position2dProxy* pp):Behaviors(pp)
{
}
Wait::~Wait()
{
}

bool Wait::Tick()
{
speed = 0;
rotation = 0;
return(0);
}

//----------------------------------------------------------//

int main(int argc, char **argv)
{

player_pose2d_t target;
target.px = 2;
target.py = 0;
target.pa = 0;


  // we throw exceptions on creation if we fail
  try {

    PlayerClient robot(gHostname, gPort); // Conect to server
    Position2dProxy pp(&robot, 0);   // Get a motor control device (index is 0)

    std::cout << robot << std::endl;

    pp.SetMotorEnable (true); // Turn on Motors
	// IN INIT A BEABVIOR
	
	Behaviors* behav;
	//Behaviors* behav;
	int state = 0;
	//Behaviors* wa
	behav = new MoveStraight(&pp); // This could be any of the ones defined above
	((MoveStraight*) behav)->Init(5.0); 	// Initialize it accoringely
						// (NOTE: call the right Init via 	casting !!) 

	// NO THIS IS INSIDE THE READ LOOP and is generic for all  Behaviors!

	for(;;){
		robot.Read();
		if(state ==0){
			if(behav->Tick()){
				state = 1;
			 // DONE with Behaviors
			// change bebavior (I used a queue of Behaviors for testing)
			// Note call Resume or init on Behaviors gefore using it.
			}
			
		
			else{
			// REMEMBER HERE WE WILL (next lab) CHECK IF INTENDED MOVE 		IS OK
			// USING behav-> GetIntendedSpeed() and
			// behav->GetIntendedRotation()

				behav->DoMove();
			}
		}
		else if(state ==1){
			behav = new Wait(&pp);
			((Wait*) behav)->Init();
			if(behav->Tick()){
				state=2;
				}
			else{behav->DoMove();}
		}
		else if(state ==2){
			behav = new Wait(&pp);
			((Wait*) behav)->Init();
			behav -> DoMove(); 

		}
		else {behav->DoMove();}
		
	}
   }
  catch (PlayerCc::PlayerError & e) {
    std::cerr << e << std::endl;
    return -1;
  }
}
// Coding by Qiushuang Li(VCL)