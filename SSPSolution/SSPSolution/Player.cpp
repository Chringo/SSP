#include "Player.h"



Player::Player()
{
	this->m_speed = 5.0f;
	this->m_grabbed = nullptr;
	this->m_isAiming = false;
}


Player::~Player()
{
}


int Player::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AnimationComponent* aComp)
{
	int result = 0;

	this->InitializeBase(entityID, pComp, gComp, aComp);
	this->m_speed = 0.01f;
	this->m_grabbed = nullptr;
	this->m_lookDir = DirectX::XMVectorSet(0, 0, 1, 0);
	this->m_carryOffset = DirectX::XMVectorSet(0, 2, 0, 0);

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
#ifdef _DEBUG
		/*If the player is currently walking, dont update any information from here.*/
			if (m_grabbed != nullptr)
			{
				if (!stateExists(PLAYER_RUN_FORWARD_BALL))
				{
					SetAnimationComponent(PLAYER_RUN_FORWARD_BALL, 0.25f, Blending::SMOOTH_TRANSITION, true);
					this->m_aComp->previousState = PLAYER_RUN_FORWARD_BALL;
				}
			}
			else
			{
				if (!stateExists(PLAYER_RUN_FORWARD))
				{
					SetAnimationComponent(PLAYER_RUN_FORWARD, 0.25f, Blending::SMOOTH_TRANSITION, true);
					this->m_aComp->previousState = PLAYER_RUN_FORWARD;
				}
			}
	}
	/*If the player is not walking, go to idle.*/
	else if(inputHandler->IsKeyReleased(SDL_SCANCODE_W))
	{
		if (m_grabbed != nullptr)
		{
			SetAnimationComponent(PLAYER_BALL_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_BALL_IDLE;
		}
		else
		{
			SetAnimationComponent(PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_IDLE;
		}
#endif // _DEBUG
	}

	if (inputHandler->IsKeyDown(SDL_SCANCODE_S))
	{
		forwards--;

#ifdef _DEBUG
		if (m_grabbed != nullptr)
		{
			if (!stateExists(PLAYER_RUN_BACKWARD_BALL))
			{
				SetAnimationComponent(PLAYER_RUN_BACKWARD_BALL, 0.25f, Blending::SMOOTH_TRANSITION, true);
				this->m_aComp->previousState = PLAYER_RUN_BACKWARD_BALL;
			}
		}
		else
		{
			if (!stateExists(PLAYER_RUN_BACKWARD))
			{
				SetAnimationComponent(PLAYER_RUN_BACKWARD, 0.25f, Blending::SMOOTH_TRANSITION, true);
				this->m_aComp->previousState = PLAYER_RUN_BACKWARD;
			}
		}
	}
	else if (inputHandler->IsKeyReleased(SDL_SCANCODE_S))
	{
		if (m_grabbed != nullptr)
		{
			SetAnimationComponent(PLAYER_BALL_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_BALL_IDLE;
		}
		else
		{
			SetAnimationComponent(PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_IDLE;
		}
		
#endif // _DEBUG

	}
	if (inputHandler->IsKeyDown(SDL_SCANCODE_D))
	{
		sideways++;
#ifdef _DEBUG
		if (m_grabbed != nullptr)
		{
			if (!stateExists(PLAYER_RUN_RIGHT_BALL))
			{
				SetAnimationComponent(PLAYER_RUN_RIGHT_BALL, 0.25f, Blending::SMOOTH_TRANSITION, true);
				this->m_aComp->previousState = PLAYER_RUN_RIGHT_BALL;
			}
		}
		else
		{
			if (!stateExists(PLAYER_RUN_RIGHT))
			{
				SetAnimationComponent(PLAYER_RUN_RIGHT, 0.25f, Blending::SMOOTH_TRANSITION, true);
				this->m_aComp->previousState = PLAYER_RUN_RIGHT;
			}
		}
	}
	else if (inputHandler->IsKeyReleased(SDL_SCANCODE_D))
	{
		if (m_grabbed != nullptr)
		{
			SetAnimationComponent(PLAYER_BALL_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_BALL_IDLE;
		}
		else
		{
			SetAnimationComponent(PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_IDLE;
		}	
#endif
	}

	if (inputHandler->IsKeyDown(SDL_SCANCODE_A))
	{
		sideways--;

#ifdef _DEBUG
		if (m_grabbed != nullptr)
		{
			if (!stateExists(PLAYER_RUN_LEFT_BALL))
			{
				SetAnimationComponent(PLAYER_RUN_LEFT_BALL, 0.25f, Blending::SMOOTH_TRANSITION, true);
				this->m_aComp->previousState = PLAYER_RUN_LEFT_BALL;
			}
		}
		else
		{
			if (!stateExists(PLAYER_RUN_LEFT))
			{
				SetAnimationComponent(PLAYER_RUN_LEFT, 0.25f, Blending::SMOOTH_TRANSITION, true);
				this->m_aComp->previousState = PLAYER_RUN_LEFT;
			}
		}
	}
	else if (inputHandler->IsKeyReleased(SDL_SCANCODE_A))
	{
		if (m_grabbed != nullptr)
		{
			SetAnimationComponent(PLAYER_BALL_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_BALL_IDLE;
		}
		else
		{
			SetAnimationComponent(PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true);
			this->m_aComp->previousState = PLAYER_IDLE;
		}
#endif // _DEBUG
	}

	if (this->m_grabbed != nullptr)
	{
		this->m_grabbed->GetPhysicsComponent()->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, this->m_carryOffset);
	}

	if (inputHandler->IsKeyPressed(SDL_SCANCODE_P))
	{
		//assumes grabbed is ALWAYS the ball
		if (this->m_grabbed != nullptr)
		{
#ifdef _DEBUG
			SetAnimationComponent(PLAYER_THROW, 0.25f, Blending::SMOOTH_TRANSITION, false);
			this->m_aComp->previousState = PLAYER_THROW;
#endif // _DEBUG

			float strength = 1.0f;
			this->m_grabbed->GetPhysicsComponent()->PC_velocity = DirectX::XMVectorScale(DirectX::XMVectorAdd(this->m_lookDir, DirectX::XMVectorSet(0, 1.5f, 0, 0)), strength);
			this->SetGrabbed(nullptr);	//Release the entity
		}

	}
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_I))
	{
		//assumes grabbed is ALWAYS the ball
		if (this->m_grabbed != nullptr)
		{
#ifdef _DEBUG
			SetAnimationComponent(PLAYER_THROW, 0.25f, Blending::SMOOTH_TRANSITION, false);
			this->m_aComp->previousState = PLAYER_THROW;
#endif // DEBUG


			float strength = 1.5f;
			this->m_grabbed->GetPhysicsComponent()->PC_velocity = DirectX::XMVectorScale(this->m_lookDir, strength);
			this->SetGrabbed(nullptr);	//Relsease the entity
		}

	}
	//Check if player is grounded

	//Check if the player CAN	 update its physics component
	if (this->m_pComp != nullptr)
	{
		//Check if the player should update its physics component
		//if (this->m_pComp->PC_entityID == 0)
		//{
		if (forwards != 0 || sideways != 0)
			{
				//Use those values for the player behaviour calculations
				//Get the rotation around the Y-axis, also called the Yaw axis
				//float yaw = this->m_pComp->rotation.y;

				//Define a quaternion rotation so we can rotate the velocity vector
				//DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, DirectX::XMScalarASin(yaw / 2.0f), 0.0f, DirectX::XMScalarACos(yaw / 2.0f));
				float forwardsVel = 0.0f, sidewaysVel = 0.0f;
				DirectX::XMVECTOR velocity = DirectX::XMVectorSet(m_speed * sideways, 0.0f, m_speed * forwards, 1.0f);
				DirectX::XMVECTOR lookAtDir = this->m_lookDir;
				lookAtDir.m128_f32[1] = 0.0f;
				velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(lookAtDir), m_speed * forwards * dT);
				//velocity.m128_f32[1] = 0.0f; // doing this makes it a forward vector instead of view direction
				velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorScale(this->m_rightDir, m_speed*sideways * dT));
				//Rotate the velocity vector
				//velocity = DirectX::XMVector3Rotate(velocity, rotation);
				//Add the velocity to our physicsComponent

				this->m_pComp->PC_velocity = DirectX::XMVectorAdd(this->m_pComp->PC_velocity, velocity);
			}

		//}
		if (this->m_gComp != nullptr)
		{
			this->UnsafeSyncComponents();
		}
	}

	//End the update
	return result;
}

int Player::React(int entityID, EVENT reactEvent)
{
	int result = 0;

	return result;
}

Entity* Player::SetGrabbed(Entity * entityPtr)
{
	Entity* oldValue = nullptr;
	
	if (entityPtr != this->m_grabbed)	//Cant be the same entity that we are currently holding
	{
		oldValue = this->m_grabbed;
		this->m_grabbed = entityPtr;

		if (this->m_grabbed != nullptr)	//If we grab something that is not a nullptr
		{
			this->m_grabbed->SetGrabbed(this);	//Set the new entity to be grabbed by this entity	
		}
		if (oldValue != nullptr)	//If we drop something
		{
			oldValue->SetGrabbed(nullptr);	//Set the old entity to NOT grabbed
		}
	}
	return oldValue;	//Returns nullptr if nothing is droped for the new entity
}

float Player::SetSpeed(float speed)
{
	float oldSpeed = this->m_speed;
	this->m_speed = speed;
	return oldSpeed;
}

DirectX::XMVECTOR Player::SetLookDir(DirectX::XMVECTOR lookDir)
{
	DirectX::XMVECTOR oldValue = this->m_lookDir;
	this->m_lookDir = lookDir;
	return oldValue;
}

DirectX::XMVECTOR Player::SetUpDir(DirectX::XMVECTOR upDir)
{
	DirectX::XMVECTOR oldValue = this->m_upDir;
	this->m_upDir = upDir;
	return oldValue;
}

DirectX::XMVECTOR Player::SetRightDir(DirectX::XMVECTOR rightDir)
{
	DirectX::XMVECTOR oldValue = this->m_rightDir;
	this->m_rightDir = rightDir;
	return oldValue;
}

void Player::SetAiming(bool isAming)
{
	this->m_isAiming = isAming;
}

bool Player::stateExists(int animationState)
{
	/*If the previous state is not equal to current state.*/
	if (m_aComp->previousState != animationState)
	{
		m_aComp->previousState = animationState;
		return false;
	}
	/*If the previous state is equal to current state.*/
	else
	{
		return true;
	}
}

void Player::SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping)
{
	this->m_aComp->m_TransitionDuration = transitionDuration;
	this->m_aComp->target_State = this->m_aComp->animation_States->at(animationState)->GetAnimationStateData();
	this->m_aComp->target_State->stateIndex = animationState;
	this->m_aComp->blendFlag = blendingType;
	this->m_aComp->target_State->isLooping = isLooping;
}

void Player::SetBall(Entity * ball)
{
	this->m_ball = ball;
}

float Player::GetSpeed()
{
	return this->m_speed;
}

DirectX::XMVECTOR Player::GetLookDir()
{
	return this->m_lookDir;
}

DirectX::XMVECTOR Player::GetUpDir()
{
	return this->m_upDir;
}

DirectX::XMVECTOR Player::GetRightDir()
{
	return this->m_rightDir;
}

bool Player::GetIsAming()
{
	return this->m_isAiming;
}

Entity * Player::GetGrabbed()
{
	return this->m_grabbed;
}

Entity * Player::GetBall()
{
	return this->m_ball;
}
