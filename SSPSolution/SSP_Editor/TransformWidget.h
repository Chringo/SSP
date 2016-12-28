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
	Container * selectedContainer;

	 int modelIndex = NONE;
	 unsigned int instanceID = NULL;
	 unsigned int modelID = NULL;
public:
	OBB selectedObjectOBB;
	OBB axisOBB[NUM_AXIS];
	DirectX::XMVECTOR * axisColors[NUM_AXIS];
	unsigned int selectedAxis = NONE;


public:
	unsigned int GetInstanceID() { return instanceID; };
	unsigned int GetModelID() { return modelID; };
	int GetModelIndex() { return modelIndex; };
	Container * GetContainer() { return selectedContainer; };

	void UpdateOBB(const OBB &boundingBox)
	{
		selectedObjectOBB.ext[0] = boundingBox.ext[0];
		selectedObjectOBB.ext[1] = boundingBox.ext[1];
		selectedObjectOBB.ext[2] = boundingBox.ext[2];
		selectedObjectOBB.ort = boundingBox.ort;
		selectedObjectOBB.pos = boundingBox.pos;
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
		Container * selectedContainer, 
		int modelIndex, 
		unsigned int instanceID, 
		unsigned int modelID)
	{
		this->selectedObjectOBB = selectedOBB;
		this->selectedContainer = selectedContainer;
		this->modelIndex = modelIndex;
		this->instanceID = instanceID;
		this->modelID = modelID;

		for (int i = 0; i < NUM_AXIS; i++)
		{
			axisOBB[i].ort = selectedOBB.ort;
			axisOBB[i].pos = selectedOBB.pos;
			//relative to origin
			axisOBB[i].pos.m128_f32[i] += 1.f;

			//relative to object
			//axisOBB[i].pos.m128_f32[i] += selectedObject.ort.r[i].m128_f32[i] + .1f;
		}

		setActive(true);
	};

	void SelectAxis(int i)
	{
		axisColors[X] = &m_colors[X];
		axisColors[Y] = &m_colors[Y];
		axisColors[Z] = &m_colors[Z];

		if (i >= X && Z >= i)
		{
			selectedAxis = i;
			axisColors[i] = &m_colors[NUM_AXIS];
		}
		else
		{
			selectedAxis = NONE;
		}
	}
	void DeSelect()
	{
		if (m_active)
		{
			//this->selectedObjectOBB;
			this->selectedContainer = nullptr;
			this->modelIndex = NONE;
			this->instanceID = NULL;
			this->modelID = NULL;

			SelectAxis(NONE);
			setActive(false);
		}
	};
	TransformWidget()
	{
		for (int i = 0; i < NUM_AXIS; i++)
		{
			for (int j = 0; j < NUM_AXIS; j++)
			{
				axisOBB[i].ext[j] = 0.15;
			}
		}

		m_colors[X] = { 1.0, 0.0, 0.0, 0.0 };
		m_colors[Y] = { 0.0, 1.0, 0.0, 0.0 };
		m_colors[Z] = { 0.0, 0.0, 1.0, 0.0 };
		m_colors[NUM_AXIS] = { 1.0, 1.0, 1.0, 0.0 };

		axisColors[X] = &m_colors[X];
		axisColors[Y] = &m_colors[Y];
		axisColors[Z] = &m_colors[Z];

	};
};

#endif