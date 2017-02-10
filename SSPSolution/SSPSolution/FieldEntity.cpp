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
	this->m_field = field;
	return 1;
}

int FieldEntity::Update(float deltaTime, InputHandler * inputHandler)
{
	if (this->m_field->F_first_inside && this->m_field->F_second_inside)
	{
		this->m_subject.Notify(this->m_entityID, FIELD_CONTAINS);
		printf("YOU'RE IN THE KILL ZONE!");
	}

	// TODO: More EVENT messages if needed

	return 1;
}

int FieldEntity::React(int entityID, EVENT reactEvent)
{

	return 1;
}

Field * FieldEntity::GetField()
{
	return this->m_field;
}
//void FieldEntity::SetField(Field * field)
//{
//	this->m_field = field;
//}
