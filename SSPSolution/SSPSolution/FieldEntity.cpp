#include "FieldEntity.h"

FieldEntity::FieldEntity()
{
}

FieldEntity::~FieldEntity()
{
}

int FieldEntity::Initialize(unsigned int entityID, Field* field)
{
	this->InitializeBase(entityID, nullptr, nullptr, nullptr);
	this->m_field = field;
	return 1;
}

//Returns 1 if the internal conditions are met.
int FieldEntity::Update(float deltaTime, InputHandler * inputHandler)
{
	int result = 0;
	if (this->m_field->F_first_inside && this->m_field->F_second_inside)
	{
		this->m_subject.Notify(this->m_entityID, FIELD_CONTAINS);
		result = 1;
	}
	// TODO: More EVENT messages if needed
	return result;
}

int FieldEntity::React(unsigned int entityID, EVENT reactEvent)
{

	return 1;
}

Field * FieldEntity::GetField()
{
	return this->m_field;
}
