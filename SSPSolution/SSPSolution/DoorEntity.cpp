#include "DoorEntity.h"

DoorEntity::DoorEntity()
{
}


DoorEntity::~DoorEntity()
{
}

int DoorEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, std::vector<ElementState> subjectStates, float rotateTime, float minRotation, float maxRotation)
{
	this->InitializeBase(entityID, pComp, gComp, nullptr);

	this->m_closed_pos = pComp->PC_pos;
	this->m_originalOrto = pComp->PC_OBB.ort;
	this->m_isOpened = false;
	this->m_rotateTime = rotateTime;
	this->m_minRotation = minRotation;
	this->m_maxRotation = maxRotation;
	float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);
	this->m_minRotation += currentYRotation;
	this->m_maxRotation += currentYRotation;
	this->m_rotatePerSec = (this->m_maxRotation - this->m_minRotation) / this->m_rotateTime;
	this->SyncComponents();

	//Get pivot point

	DirectX::XMVECTOR pivot = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5f);
	pivot = DirectX::XMVectorAdd(pivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2f));
	pivot = DirectX::XMVectorAdd(pivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[0], 0.2f));
	//Create translation matrix to pivot (Tx)
	DirectX::XMMATRIX pivotMatrix = DirectX::XMMatrixTranslationFromVector(pivot);
	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, pivotMatrix);
	this->m_subjectStates = subjectStates;

	return 0;
}

int DoorEntity::Update(float dT, InputHandler * inputHandler)
{


	if (m_animationActive)
	{
		PhysicsComponent* ptr = this->GetPhysicsComponent(); //Get this doors physicscomponent
		DirectX::XMMATRIX rot = DirectX::XMMatrixIdentity(); //init Rot
		
		static float lastFrameRotValue = 0;
		float frameRot = m_animSpeed * dT;
#pragma region 
		if (m_targetRot == 0) // If we are playing the closing animation
		{

			if (m_currRot + frameRot < m_targetRot) // if the animation is not yet finished
			{
				m_currRot += frameRot;
				float sinRot = (pow(sin(m_animSpeed * dT ) + 1, 4.0f) / 4.0f); // calculate current rotation value
				rot = DirectX::XMMatrixRotationAxis(this->m_pComp->PC_OBB.ort.r[1], DirectX::XMConvertToRadians(-frameRot));
				lastFrameRotValue = m_currRot;
			}
			else {
			

				m_animationActive = false; //disable animation flag
				m_currRot = m_targetRot; // set the variables to match
			
				this->SyncComponents();
				m_pComp->PC_OBB.ort = m_originalOrto;
				//Sync the graphical component to match the bounding box
				//Get pivot point
				DirectX::XMVECTOR pivot = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5f);
				pivot = DirectX::XMVectorAdd(pivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2f));
				pivot = DirectX::XMVectorAdd(pivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[0], 0.2f));
				//Create translation matrix to pivot (Tx)
				DirectX::XMMATRIX pivotMatrix = DirectX::XMMatrixTranslationFromVector(pivot);
				this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, pivotMatrix);
				return 0;
			}

		}
		else //We are playing the opening animation
		{

			if (m_currRot - frameRot > m_targetRot) // if the animation is not yet finished
			{
				m_currRot -= frameRot;
				float sinRot = (pow(sin(m_animSpeed * dT) + 1, 4.0f) / 4.0f); // calculate current rotation value

				rot = DirectX::XMMatrixRotationAxis(this->m_pComp->PC_OBB.ort.r[1], DirectX::XMConvertToRadians(frameRot));
				lastFrameRotValue = m_currRot;
			}
			else { // It has reached the end of the animation, the door is fully open
				m_animationActive = false; //deactivate animation
				m_currRot = m_targetRot;   // Set the variables to match exactly
				rot = DirectX::XMMatrixRotationAxis(this->m_pComp->PC_OBB.ort.r[1], DirectX::XMConvertToRadians(m_targetRot - lastFrameRotValue)); ;

				
			}

		}
#pragma endregion Determine state of the animation and create the right rotation matrix

#pragma region 

		/*
		To rotate around a pivot point.
		- Translate to pivot point,    (pivotTranslationMatrix)
		- Get yer olde rotation matrix (rotationMatrix)
		- pointVector = pointVector * (pivotTranslationMatrix * rotationMatrix)  //scale should be included but omitted for simplicity, we do not animate scale here
		- pointVector = pointVector * -pivotTranslationMatrix

		Tx = pivotpoint
		Point' = Tx + (R * S) * (Point -Tx)

		We have to do a bit differently here because the Obb holds the rotation and the physicscomponent holds the position
		So they are not in the same matrix.

		We only rotate the bounding box, but the position is both rotated and translated
		*/

		//A big problem here is that the graphical component has a different pivot point than the bounding box

		//Get pivot point, Which is at the bottom right of the door
		DirectX::XMVECTOR localPivot = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5f); // -Y
		
		//move the pivot point to be relative to the bounding box, 
		localPivot = DirectX::XMVectorAdd(localPivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], 1.2f));// + Z

		//create an offset vector  | localPivot - pos
		DirectX::XMVECTOR pivot = DirectX::XMVectorSubtract(localPivot, this->m_pComp->PC_pos);

		//Create translation matrix to offset (Tx)
		DirectX::XMMATRIX pivotMatrix = DirectX::XMMatrixTranslationFromVector(pivot);
		//Create the inverse of that translation, (-Tx)
		DirectX::XMVECTOR pivotDet = DirectX::XMMatrixDeterminant(pivotMatrix);
		DirectX::XMMATRIX inversePivot = DirectX::XMMatrixInverse(&pivotDet, pivotMatrix);

		ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, rot); //Multiply the bounding box with the rotation only!

		rot = DirectX::XMMatrixMultiply(pivotMatrix,rot); // Tx + R | Update the rotation matrix to include the pivot translation

		this->m_pComp->PC_pos = DirectX::XMVector3TransformCoord(this->m_pComp->PC_pos, rot); // Translate and rotate the position around the pivot
		this->m_pComp->PC_pos = DirectX::XMVector3TransformCoord(this->m_pComp->PC_pos, inversePivot); // Translate the position back to its position
		this->SyncComponents(); // Update components
		

		//Get pivot point, Which is at the bottom right of the door
		 localPivot = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5f); // -Y
		 pivot = DirectX::XMVectorAdd(pivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[0], 0.2f)); //+ X
		//Create a matrix for the graphical part, Since it is visually offset
		 localPivot = DirectX::XMVectorAdd(localPivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[0], 0.2f));
		DirectX::XMMATRIX graphicsOffsetMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorAdd(localPivot, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2f)));

		this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, graphicsOffsetMatrix); //Multiply with the graphicsMatrix to align the model

	
	}
#pragma endregion Rotate around pivot (By Martin)

#pragma region Commented code, Author not known, (Kim?,Sebastian?)

	//
	//if (this->m_isOpened)
	//{
	//	this->m_pComp->PC_pos = this->m_closed_pos;
	//	if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_maxRotation)
	//	{
	//		//this->m_pComp->PC_rotationVelocity = DirectX::XMVectorSet(0, -0.2, 0, 0);
	//		//DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
	//		//DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
	//		//DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

	//		//float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

	//		//float rotationValue = this->m_rotatePerSec * dT;
	//		//if (rotationValue + currentYRotation > this->m_maxRotation)
	//		//{
	//		//	rotationValue = this->m_maxRotation - currentYRotation;
	//		//}
	//		//

	//		////We do not need to rotate the Y-axis
	//		//We need to limit the rotation so it doesn't go over the limits
	//		
	//		//this->m_gComp->modelPtr->GetOBBData().


	//		//this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), yRotationMatrix);

	//		//rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
	//		//rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
	//		//this->m_pComp->PC_OBB.ort.r[0] = rotationX;
	//		//this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

	//		//this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
	//		//if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_maxRotation)
	//		//{
	//		//	this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_maxRotation);
	//		//}

	//		//DirectX::XMVECTOR test = DirectX::XMVectorSet(1.2, -1.5, 0, 0);
	//		//DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(test);
	//		//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, matrix);
	//	}

	//	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 3.14f / 2.0f, 1);
	//	this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(this->m_originalOrto, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(this->m_pComp->PC_rotation)));

	//	this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[2], -1));
	//	this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[0], 1));


	//	this->SyncComponents();
	//	DirectX::XMVECTOR offSet = DirectX::XMVectorScale(this->m_originalOrto.r[1], -1.5f);
	//	offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_originalOrto.r[2], -0.2f));
	//	offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_originalOrto.r[0], -1.2f));
	//	DirectX::XMMATRIX offSet_matrix = DirectX::XMMatrixTranslationFromVector(offSet);
	//	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);



	//}
	//else
	//{

	//	this->m_pComp->PC_pos = this->m_closed_pos;

	//	if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
	//	{
	//		//this->m_pComp->PC_rotationVelocity = DirectX::XMVectorSet(0, 0.2, 0, 0);
	//		//DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
	//		//DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
	//		////DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

	//		//float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

	//		////float rotationValue = this->m_rotatePerSec * dT * -1;
	//		////if (rotationValue + currentYRotation < this->m_minRotation)
	//		////{
	//		////	rotationValue = this->m_minRotation - currentYRotation;
	//		////	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
	//		////}


	//		//////We do not need to rotate the Y-axis
	//		//DirectX::XMMATRIX yRotationMatrix = DirectX::XMMatrixRotationY(-3.14 / 2);
	//		////We need to limit the rotation so it doesn't go over the limits

	//		//this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
	//		//this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), yRotationMatrix);
	//		//this->m_isOpened = false;
	//		//rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
	//		//rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
	//		//this->m_pComp->PC_OBB.ort.r[0] = rotationX;
	//		//this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

	//		//this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
	//		//if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
	//		//{
	//		//	this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
	//		//}


	//		//DirectX::XMVECTOR test = DirectX::XMVectorSet(1.2, -1.5, 0, 0);
	//		//DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(test);
	//		//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, matrix);
	//	}
	//	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
	//	this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(this->m_originalOrto, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(this->m_pComp->PC_rotation)));
	//	
	//	this->SyncComponents();

	//}
		
#pragma endregion Commented code

	return 0;
}

int DoorEntity::React(int entityID, EVENT reactEvent)
{
	//Kims stuff, "crazy but elegant" - Oscar 2017-01-23
	//I, Kim, do not remember our old crazy solution.
	int i = 0;
	for (std::vector<ElementState>::iterator element = this->m_subjectStates.begin(); element != this->m_subjectStates.end(); element++)
	{
		//Match the EntityID
		if ((element->entityID == entityID))
		{
			//There is a special case of Wheel Incrementation
			if (element->desiredState < EVENT::WHEEL_0 || element->desiredState > EVENT::WHEEL_100)
			{
				//We are not listening for incrementation events from wheels
				element->desiredStateReached = element->desiredState == reactEvent;
			}
			else
			{
				//We are listening for incrementation events from wheel so ignore state events
				if (reactEvent != EVENT::WHEEL_INCREASING && reactEvent != EVENT::WHEEL_DECREASING && reactEvent != EVENT::WHEEL_RESET)
				{
					element->desiredStateReached = element->desiredState == reactEvent;
				}
			}
		}
		i += element->desiredStateReached;
	}
	//The state of the door changed, it iseither not opened or not closed anymore
	if (this->m_isOpened != (i == this->m_subjectStates.size()))
	{
		if(m_isOpened)
			m_targetRot = 0;
		else
			m_targetRot = -m_activatedRotation;
		//Play sound
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
		m_animationActive = true;
		this->m_needSync = true; // for network
		this->GetPhysicsComponent()->PC_active = false;
		SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_DOOR_OPENING, pos, false, false);
	}
	
	this->m_isOpened = i == this->m_subjectStates.size();


	return 0;
}

bool DoorEntity::SetIsOpened(bool isOpened)
{
	bool lastValue = this->m_isOpened;
	this->m_isOpened = isOpened;
	return lastValue;
}

bool DoorEntity::GetIsOpened()
{
	return this->m_isOpened;
}

bool DoorEntity::AddSubjectState(ElementState subjectState)
{
	bool result = true;
	this->m_subjectStates.push_back(subjectState);
	return result;
}

bool DoorEntity::AddSubjectState(unsigned int entityID, EVENT requiredEvent)
{
	bool result = true;
	ElementState newElementState;
	newElementState.entityID = entityID;
	newElementState.desiredState = requiredEvent;
	newElementState.desiredStateReached = false;
	this->m_subjectStates.push_back(newElementState);
	return result;
}
