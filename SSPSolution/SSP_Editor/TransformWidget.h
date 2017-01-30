#pragma once
#ifndef SSPEDITOR_TRANSFORMWIDGET_H
#define SSPEDITOR_TRANSFORMWIDGET_H

struct TransformWidget
{
public:
	enum AXIS
	{
		NONE = -1,
		X,
		Y,
		Z,

		NUM_AXIS
	};
private:
	bool m_active = false;
	DirectX::XMVECTOR m_colors[4];
	Container * m_selectedContainer = nullptr;
	AIComponent* m_aiContainer = nullptr;

	unsigned int m_instanceID = NULL;
	unsigned int m_modelID = NULL;
	OBB m_selectedObjectOBB;
	DirectX::XMVECTOR m_axisOBBpos[NUM_AXIS];
	OBB m_axisOBB[NUM_AXIS];
	DirectX::XMVECTOR * m_axisColors[NUM_AXIS];
	DirectX::XMVECTOR  SelectedObjectOBBColor;
	DirectX::XMVECTOR m_obbCenterPosition;
	DirectX::XMVECTOR m_obbLastPosition;
	int m_selectedAxis = NONE;
private:
	inline void m_UpdateAxies()
	{
		for (int i = 0; i < NUM_AXIS; i++)
		{

			m_axisOBBpos[i] = m_selectedContainer->position;

			
			//relative to origin
			m_axisOBBpos[i].m128_f32[i] += 1.f;

			//relative to object
			//axisOBB[i].pos.m128_f32[i] += selectedObject.ort.r[i].m128_f32[i] + .1f;
		}
	}

public:
	unsigned int GetInstanceID() { return m_selectedContainer->internalID; };
	unsigned int GetModelID() { return m_modelID; };
	Container * GetContainer() { return m_selectedContainer; };
	DirectX::XMVECTOR ** GetAxisColors() { return m_axisColors; };
	DirectX::XMVECTOR * GetAxisOBBpositons() { return m_axisOBBpos; };
	DirectX::XMVECTOR * GetSelectedObjectOBBColor(){ return &SelectedObjectOBBColor; };
	DirectX::XMVECTOR * GetOBBCenterPostition() { return &m_obbCenterPosition; };
	OBB * GetAxisOBBs() { return m_axisOBB; };
	OBB * GetSelectedObjectOBB() { return &m_selectedObjectOBB; };
	int GetSelectedAxis() { return m_selectedAxis; };
	void SetOBBCenterPosition(DirectX::XMVECTOR centerPosition) { this->m_obbCenterPosition = centerPosition; this->m_obbLastPosition = centerPosition; };


	void UpdateOBB()
	{
		if (this->m_selectedContainer->type == CHECKPOINT)
		{
			m_obbCenterPosition = this->m_selectedContainer->position;
		}
		else {
			m_obbCenterPosition = DirectX::XMVector3TransformCoord(m_obbLastPosition, this->m_selectedContainer->component.worldMatrix);
			m_selectedObjectOBB.ort = this->m_selectedContainer->component.worldMatrix;
		}

		m_UpdateAxies();
	};

	bool IsActive()
	{
		return m_active;
	};
	void setActive(bool active)
	{
		this->m_active = active;
	};
	void Select(OBB &selectedOBB, 
		Container *& selectedContainer, 
		unsigned int instanceID, 
		unsigned int modelID)
	{
		this->m_selectedObjectOBB = selectedOBB;
		this->m_selectedContainer = selectedContainer;
		this->m_instanceID = instanceID;
		this->m_modelID = modelID;


		UpdateOBB();
		setActive(true);
	};

	void Select(OBB &selectedOBB,
		CheckpointContainer *& selectedContainer)
	{
		this->m_selectedObjectOBB = selectedOBB;
		this->m_selectedContainer = selectedContainer;
		this->m_instanceID = selectedContainer->internalID;
		this->m_modelID = UINT_MAX;

		m_obbCenterPosition = selectedContainer->position;


		UpdateOBB();
		setActive(true);
	};


	void SelectAxis(int i)
	{
		m_axisColors[X] = &m_colors[X];
		m_axisColors[Y] = &m_colors[Y];
		m_axisColors[Z] = &m_colors[Z];

		if (i >= X && Z >= i)
		{
			m_selectedAxis = i;
			m_axisColors[i] = &m_colors[NUM_AXIS];
		}
		else
		{
			m_selectedAxis = NONE;
		}
	}
	void DeSelect() //empties transformwidget and makes it inactive
	{
		if (m_active)
		{
			//this->selectedObjectOBB;
			this->m_selectedContainer = nullptr;
			this->m_instanceID = NULL;
			this->m_modelID = NULL;

			SelectAxis(NONE);
			setActive(false);
		}
	};
	TransformWidget()
	{
		DirectX::XMMATRIX ident = DirectX::XMMatrixIdentity();
		for (int i = 0; i < NUM_AXIS; i++)
		{
			m_axisOBB[i].ort = ident;
			for (int j = 0; j < NUM_AXIS; j++)
			{
				m_axisOBB[i].ext[j] = 0.15;
			}
		}

		m_colors[X] = { 1.0, 0.0, 0.0, 0.0 };
		m_colors[Y] = { 0.0, 1.0, 0.0, 0.0 };
		m_colors[Z] = { 0.0, 0.0, 1.0, 0.0 };
		m_colors[NUM_AXIS] = { 1.0, 1.0, 1.0, 0.0 };

		m_axisColors[X] = &m_colors[X];
		m_axisColors[Y] = &m_colors[Y];
		m_axisColors[Z] = &m_colors[Z];

		SelectedObjectOBBColor = { .0f, .65f, .67f };
	};
};

#endif