#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

//ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	DummyValues();

	live1.Scale(3, 3, 3);
	live2.Scale(3, 3, 3);
	live3.Scale(3, 3, 3);

	live1.color = Red;
	live2.color = Red;
	live3.color = Red;

	live1.SetPos(60, 5, 40);
	live2.SetPos(50, 5, 40);
	live3.SetPos(40, 5, 40);
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
    Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	live1.Render();
	live2.Render();
	live3.Render();

	if (App->player->live == 2) live1.SetPos(0, 0, -200);
	if (App->player->live == 1) live2.SetPos(0, 0, -200);
	if (App->player->live == 0) live3.SetPos(0, 0, -200);

    // Move camera (overrides camera controls in ModuleCamera3D)
    App->camera->Reference = dummy->GetPos(); // Set camera reference to dummy
    App->camera->Position = vec3(0, 100, 0); // Set camera to dummy
    App->camera->Position -= dummy->GetFwdAxis(); // Move camera away on dummy's fwd axis
    App->camera->LookAt(dummy->GetPos()); // Look at dummy
    App->camera->GetViewMatrix(); // Refresh camera viewpoint
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

    return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::DummyValues()
{
	VehicleInfo dum;

	// Car properties ----------------------------------------
	dum.chassis_size.Set(0, 0, 0);
	dum.chassis_offset.Set(0, 0, 0);
	dum.mass = 0;
	dum.suspensionStiffness = 0;
	dum.suspensionCompression = 0;
	dum.suspensionDamping = 0;
	dum.maxSuspensionTravelCm = 0;
	dum.frictionSlip = 0;
	dum.maxSuspensionForce = 0;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	dum.num_wheels = 0;
	dum.wheels = 0;

	dummy = App->physics->AddVehicle(dum);
	dummy->SetPos(0, 0, 0);
}

