#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

//ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1.5, 4);
	car.chassis_offset.Set(0, 1, 0);
	car.mass = 1000.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;
	car.color = Red;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(5, 1, 0);

	counterstart = false;
	counter_cant_go_speed = 0;

	live = 3;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (live > 0)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			if (counterstart == false && vehicle->GetKmh() < 75) acceleration = MAX_ACCELERATION;

			if (vehicle->GetKmh() <= -1)
			{
				brake = BRAKE_POWER / 2;
			}
		}
		else if (vehicle->GetKmh() >= 0)
		{
			acceleration = -MAX_ACCELERATION;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (vehicle->GetKmh() >= 0)
			{
				brake = BRAKE_POWER;
			}
			else if (vehicle->GetKmh() > -75)
			{
				acceleration = -MAX_ACCELERATION;
			}
		}
		else if (vehicle->GetKmh() <= 0)
		{
			acceleration = MAX_ACCELERATION;
		}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			if (countercantspeed == false)
			{
				counterstart = true;
			}
		}

		if (counterstart && countercantspeed == false)
		{
			counter_speed_go_go += 1 * dt;
		}
		if (counter_speed_go_go >= 2.5f && counterstart)
		{
			counterstart = false;
			countercantspeed = true;
			App->scene_intro->GoFuckingNuts.color = Red;
		}

		if (countercantspeed && counterstart == false)
		{
			counter_cant_go_speed += 1 * dt;
		}
		if (counter_cant_go_speed >= 10 && countercantspeed)
		{
			counterstart = false;
			countercantspeed = false;
			counter_speed_go_go = 0;
			counter_cant_go_speed = 0;
			App->scene_intro->GoFuckingNuts.color = Green;
		}

		//Limits/Collisions
		//EJE X
		if (vehicle->GetPos().x >= 50) // WALL 1
		{
			vec3 aux = vehicle->GetPos();
			LOG("%f %f %f", aux.x, aux.y, aux.z);
			vehicle->SetPos(50, aux.y, aux.z);
		}
		if (vehicle->GetPos().x <= -50) // WALL 2
		{
			vec3 aux = vehicle->GetPos();
			//LOG("%f %f %f", aux.x, aux.y, aux.z);
			vehicle->SetPos(-50, aux.y, aux.z);
		}
		//EJE Y
		if (vehicle->GetPos().z >= 35) // WALL 3
		{
			vec3 aux = vehicle->GetPos();
			//LOG("%f %f %f", aux.x, aux.y, aux.z);
			vehicle->SetPos(aux.x, aux.y, 35);
		}
		if (vehicle->GetPos().z <= -45) // WALL 4
		{
			vec3 aux = vehicle->GetPos();
			//LOG("%f %f %f", aux.x, aux.y, aux.z);
			vehicle->SetPos(aux.x, aux.y, -45);
		}
	}
	else 
	{
		vehicle->SetPos(0, 0, -200);
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) live--;

    // Apply controls to vehicle
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

    // Render vehicle
	vehicle->Render();

    // Info
	char title[80];
	sprintf_s(title, "Rocket League | Car speed: %.1f km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}



