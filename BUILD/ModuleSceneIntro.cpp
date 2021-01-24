#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

//ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	DummyValues();

	//Player1
	live1.Scale(3, 3, 3);
	live2.Scale(3, 3, 3);
	live3.Scale(3, 3, 3);
	GoFuckingNuts.Scale(5, 5, 5);

	live1.color = Red;
	live2.color = Red;
	live3.color = Red;
	GoFuckingNuts.color = Green;

	live1.SetPos(60, 5, 40);
	live2.SetPos(50, 5, 40);
	live3.SetPos(40, 5, 40);
	GoFuckingNuts.SetPos(55, 8, -35);

	//Player2
	live4.Scale(3, 3, 3);
	live5.Scale(3, 3, 3);
	live6.Scale(3, 3, 3);
	GoFuckingNuts2.Scale(5, 5, 5);

	live4.color = Blue;
	live5.color = Blue;
	live6.color = Blue;
	GoFuckingNuts2.color = Green;

	live4.SetPos(-60, 5, 40);
	live5.SetPos(-50, 5, 40);
	live6.SetPos(-40, 5, 40);
	GoFuckingNuts2.SetPos(-55, 8, -35);

	greed.Scale(200, 0.1f, 200);
	greed.color = White;

	wall1.Scale(10, 4, 150);
	wall1.SetPos(57.5f, 0, 0);
	wall1.color = Green;

	aux1.Scale(45, 4, 150);
	aux1.SetPos(75, 0, 0);
	aux1.color = Green;
	aux2.Scale(45, 4, 150);
	aux2.SetPos(-75, 0, 0);
	aux2.color = Green;
	aux3.Scale(150, 4, 45);
	aux3.SetPos(0, 0, 60);
	aux3.color = Green;

	wall2.Scale(10, 4, 150);
	wall2.SetPos(-57.5f, 0, 0);
	wall2.color = Green;
	
	wall3.Scale(150, 4, 10);
	wall3.SetPos(0, 0, 42.5f);
	wall3.color = Green;

	wall4.Scale(150, 4, 10);
	wall4.SetPos(0, 0, -52.5f);
	wall4.color = Green;

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
	greed.Render();
	wall1.Render();
	wall2.Render();
	wall3.Render();
	wall4.Render();
	aux1.Render();
	aux2.Render();
	aux3.Render();

	live1.Render();
	live2.Render();
	live3.Render();
	live4.Render();
	live5.Render();
	live6.Render();
	GoFuckingNuts.Render();
	GoFuckingNuts2.Render();

	if (App->player->live == 2) live1.SetPos(0, 0, -200);
	if (App->player->live == 1) live2.SetPos(0, 0, -200);
	if (App->player->live == 0) live3.SetPos(0, 0, -200);

	if (App->player2->live == 2) live4.SetPos(0, 0, -200);
	if (App->player2->live == 1) live5.SetPos(0, 0, -200);
	if (App->player2->live == 0) live6.SetPos(0, 0, -200);

    // Move camera (overrides camera controls in ModuleCamera3D)
    App->camera->Reference = dummy->GetPos(); // Set camera reference to dummy
    App->camera->Position = vec3(0, 100, 0); // Set camera to dummy
    App->camera->Position -= dummy->GetFwdAxis(); // Move camera away on dummy's fwd axis
    App->camera->LookAt(dummy->GetPos()); // Look at dummy
    App->camera->GetViewMatrix(); // Refresh camera viewpoint
	
	OnCollision(App->player->vehicle, App->player2->vehicle);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
    return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysVehicle3D* body1, PhysVehicle3D* body2)
{
	if ((body1->GetPos().x < (body2->GetPos().x + 3)) && ((body1->GetPos().x + 3) > body2->GetPos().x) &&
		(body1->GetPos().y < (body2->GetPos().y + 2.5f)) && ((body1->GetPos().y + 2.5f) > body2->GetPos().y) &&
		(body1->GetPos().z < (body2->GetPos().z + 5)) && ((body1->GetPos().z + 5) > body2->GetPos().z))
	{
		if (App->player->counterstart)
		{
			App->player2->live--;
			App->player->counter_speed_go_go = 10;
		}

		if (App->player2->counterstart)
		{
			App->player->live--;
			App->player2->counter_speed_go_go = 10;
		}
	}
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

