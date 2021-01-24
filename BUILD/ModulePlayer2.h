#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 500.0f

class ModulePlayer2 : public Module
{
public:
	//ModulePlayer(Application* app, bool start_enabled = true);
	ModulePlayer2(bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	bool counterstart;
	bool countercantspeed;
	float counter_speed_go_go;
	float counter_cant_go_speed;

	float live;
};