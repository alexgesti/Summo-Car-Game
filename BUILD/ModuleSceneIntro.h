#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;
struct PhysVehicle3D;

class ModuleSceneIntro : public Module
{
public:
	//ModuleSceneIntro(Application* app, bool start_enabled = true);
    ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
    update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysVehicle3D* body1, PhysVehicle3D* body2);

private:

	void DummyValues();

public:

	PhysVehicle3D* dummy;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Sphere live1;
	Sphere live2;
	Sphere live3;
	Sphere GoFuckingNuts;

	Sphere live4;
	Sphere live5;
	Sphere live6;
	Sphere GoFuckingNuts2;

	Cube greed;
	Cube wall1;
	Cube wall2;
	Cube wall3;
	Cube wall4;
	Cube aux1;
	Cube aux2;
	Cube aux3;
};
