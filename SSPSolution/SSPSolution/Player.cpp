#include "Player.h"



Player::Player()
{
	this->m_speed = 5.0f;
	this->m_grabbed = nullptr;
}


Player::~Player()
{
}

int Player::Initialize()
{
	int result = 0;
	this->m_speed = 0.01f;
	this->m_grabbed = nullptr;
	this->m_lookDir = DirectX::XMVectorSet(0, 0, 1, 0);
	return result;
}

int Player::Update(float dT, InputHandler* inputHandler)
{
	int result = 0;
	//Determine the player behavior that should accour based on the user input
	//Map the user input to values
	int sideways = 0, forwards = 0;
	float rotationY = 0.0f;
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
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_P))
	{
		//assumes grabbedis ALWAYS the ball and never nullptr
		if (this->m_grabbed != nullptr)
		{
			float strenght = 1.5f;
			this->m_grabbed->GetPhysiscComponent()->PC_velocity = DirectX::XMVectorScale(DirectX::XMVectorAdd(this->m_lookDir, DirectX::XMVectorSet(0, 1.5, 0, 0)), strenght);
		}

	}
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_I))
	{
		//assumes grabbedis ALWAYS the ball and never nullptr
		if (this->m_grabbed != nullptr)
		{
			float strenght = 1.5f;
			this->m_grabbed->GetPhysiscComponent()->PC_velocity = DirectX::XMVectorScale(this->m_lookDir, strenght);
		}

	}
	//Check if player is grounded


	//Check if the player should update its physics component
	if (this->m_pComp->PC_entityID == 0)
	{
		if (forwards != 0 || sideways != 0)
		{
			//Use those values for the player behaviour calculations
			//Get the rotation around the Y-axis, also called the Yaw axis
			//float yaw = this->m_pComp->rotation.y;

			//Define a quaternion rotation so we can rotate the velocity vector
			//DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, DirectX::XMScalarASin(yaw / 2.0f), 0.0f, DirectX::XMScalarACos(yaw / 2.0f));
			float forwardsVel = 0.0f, sidewaysVel = 0.0f;
			DirectX::XMVECTOR velocity = DirectX::XMVectorSet(m_speed * sideways, 0.0f, m_speed * forwards, 1.0f);
			velocity = DirectX::XMVectorScale(this->m_lookDir, m_speed * forwards);
			velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorScale(this->m_rightDir, m_speed*sideways));
			//Rotate the velocity vector
			//velocity = DirectX::XMVector3Rotate(velocity, rotation);
			//Add the velocity to our physicsComponent
			this->m_pComp->PC_velocity = DirectX::XMVectorAdd(this->m_pComp->PC_velocity, velocity);

		}
		//Temporary fix for moving data between components
	}


	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
	//End the update
	return result;
}

int Player::React(int entityID, EVENT reactEvent)
{
	int result = 0;

	return result;
}

void Player::SetGrabbed(Entity * ptr)
{
	this->m_grabbed = ptr;
}

float Player::SetSpeed(float speed)
{
	float oldSpeed = this->m_speed;
	this->m_speed = speed;
	return oldSpeed;
}

void Player::SetLookDir(DirectX::XMVECTOR lookDir)
{
	this->m_lookDir = lookDir;
}

void Player::SetUpDir(DirectX::XMVECTOR upDir)
{
	this->m_upDir = upDir;
}

void Player::SetRightDir(DirectX::XMVECTOR rightDir)
{
	this->m_rightDir = rightDir;
}
