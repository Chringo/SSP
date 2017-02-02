#include "FieldEntity.h"

FieldEntity::FieldEntity()
{
}

FieldEntity::~FieldEntity()
{
}

int FieldEntity::Initialize(int entityID, Field* field)
{
	this->InitializeBase(entityID, nullptr, nullptr, nullptr);
	this->m_field = nullptr;
	return 1;
}

int FieldEntity::Update(float deltaTime, InputHandler * inputHandler)
{

	return 1;
}

int FieldEntity::React(int entityID, EVENT reactEvent)
{

	return 1;
}
