#include "Player.h"



Player::Player()
{
	this->m_speed = 1.0f;
	this->m_grabbed = nullptr;
}


Player::~Player()
{
}

int Player::Initialize()
{
	int result = 0;
	this->m_speed = 1.0f;
	this->m_grabbed = nullptr;
	return result;
}

int Player::Update(float dT, InputHandler* inputHandler)
{
	int result = 0;
	//Determine the player behavior that should accour based on the user input
	//Map the user input to values
	int sideways = 0, forwards = 0;
	if (inputHandler->IsKeyDown(SDL_SCANCODE_W))
	{
		forwards++;
	}
	if (inputHandler->IsKeyDown(SDL_SCANCODE_S))
	{
		forwards--;
	}
	if (inputHandler->IsKeyDown(SDL_SCANCODE_D))
	{
		sideways++;
	}
	if (inputHandler->IsKeyDown(SDL_SCANCODE_A))
	{
		sideways--;
	}
	//Check if player is grounded


	//Check if the player should update its physics component
	if (forwards != 0 || sideways != 0)
	{
		//Use those values for the player behaviour calculations
		//Get the rotation around the Y-axis, also called the Yaw axis
		//float yaw = this->m_pComp->rotation.y;
		
		//Define a quaternion rotation so we can rotate the velocity vector
		//DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, DirectX::XMScalarASin(yaw / 2.0f), 0.0f, DirectX::XMScalarACos(yaw / 2.0f));
		float forwardsVel = 0.0f, sidewaysVel = 0.0f;
		DirectX::XMVECTOR velocity = DirectX::XMVectorSet(m_speed * sideways * dT, 0.0f, m_speed * forwards * dT, 1.0f);
		//Rotate the velocity vector
		//velocity = DirectX::XMVector3Rotate(velocity, rotation);
		//Add the velocity to our physicsComponent
		this->m_pComp->m_velocity = DirectX::XMVectorAdd(this->m_pComp->m_velocity, velocity);

	}
	
	//End the update
	return result;
}

int Player::React(int entityID, EVENT reactEvent)
{
	int result = 0;

	return result;
}
