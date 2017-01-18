#include "BehaviourTypeHandler.h"

Ui::BehaviourTypeHandler::BehaviourTypeHandler()
{
}

Ui::BehaviourTypeHandler::BehaviourTypeHandler(const Ui::SSP_EditorClass * ui)
{
	
	//Deselect();
}

void Ui::BehaviourTypeHandler::Initialize(const Ui::SSP_EditorClass * ui)
{
	this->m_Numerics[SPEED] = ui->SpeedValue;
	this->m_Numerics[TIME] = ui->TimeValue;
	this->m_PATH_TRIGGER = ui->TriggerPathValue;
	connect(ui->SpeedValue, SIGNAL(valueChanged(double)), this, SLOT(on_Speed_changed(double)));
	connect(ui->TimeValue, SIGNAL(valueChanged(double)), this, SLOT(on_Time_changed(double)));
	connect(ui->TriggerPathValue, SIGNAL(valueChanged(int)), this, SLOT(on_Path_Trigger_changed(int)));

	this->m_BehaviourType = ui->BehaviourDropDown;
	this->m_Pattern = ui->PatternDropDown;
	connect(ui->BehaviourDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(on_BehaviourType_changed(int)));
	connect(ui->PatternDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Pattern_changed(int)));

	this->m_WaypointList = ui->WaypointListWidget;

	this->m_Path_Trigger_Box = ui->TriggerPathCheckBox;
	connect(ui->TriggerPathCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_Path_Trigger_Box_changed(int)));

	this->m_Add = ui->AddButton;
	this->m_Del = ui->DeleteButton;
	connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(on_Add()));
	connect(ui->DeleteButton, SIGNAL(clicked()), this, SLOT(on_Del()));
	for (int i = 0; i < NUM_WAYPOINTS; i++)
	{
		this->m_ListItems[(ListItems)i] = nullptr;
	}

}

Ui::BehaviourTypeHandler::~BehaviourTypeHandler()
{
}

void Ui::BehaviourTypeHandler::SetSelection(Container * selection)
{
	if (selection != nullptr)
	{
		m_selection = selection;
		//this->m_Numerics[SPEED]->setValue(0);
		//this->m_Numerics[TIME]->setValue(0);
		//m_uniqueID->setText(QString::number(selection->internalID));
	}
}

void Ui::BehaviourTypeHandler::Deselect()
{
	m_selection = nullptr;
	this->m_Numerics[SPEED]->setValue(0);
	this->m_Numerics[TIME]->setValue(0);
	m_uniqueID->setText(QString::number(0));
}

void Ui::BehaviourTypeHandler::UpdateSelection()
{
	SetSelection(m_selection);
}

void Ui::BehaviourTypeHandler::ResetType(BehaviourType val)
{
	switch (val)
	{
	case Ui::NONE:
		break;
	case Ui::TRIGGER:
	{
		//reset
		break;
	}
	case Ui::DOOR:
	{
		//reset
		break;
	}
	case Ui::PATH:
	{
		//UI RESET
		this->m_Numerics[SPEED]->setValue(0);
		this->m_Numerics[TIME]->setValue(0);
		this->m_PATH_TRIGGER->setValue(0);
		this->m_Path_Trigger_Box->setChecked(false);
		on_Path_Trigger_Box_changed(0);
		for (int i = 0; i < NUM_WAYPOINTS; i++)
		{
			if (this->m_ListItems[(ListItems)i] != nullptr)
			{
				this->m_WaypointList->takeItem(i);
				delete this->m_ListItems[i];
				this->m_ListItems[i] = nullptr;
			}
		}
		//remove AI COMP
		break;
	}
	default:
		break;
	}
}

void Ui::BehaviourTypeHandler::on_Time_changed(double val)
{
	if (this->m_Current_Type == PATH)
	{
		int i = 0;
		//do thing
	}
}
void Ui::BehaviourTypeHandler::on_Path_Trigger_changed(int val)
{
	this->m_PATH_TRIGGER->setValue(val);
}

void Ui::BehaviourTypeHandler::on_Path_Trigger_Box_changed(int val)
{
	switch (Qt::CheckState(val)) {

	case Qt::Unchecked:
		m_PATH_TRIGGER->setDisabled(false);
		break;
	case Qt::Checked:
	{
		m_PATH_TRIGGER->setValue(0);
		m_PATH_TRIGGER->setDisabled(true);
		break;
	}
	}
}

void Ui::BehaviourTypeHandler::on_Speed_changed(double val)
{
	if (this->m_Current_Type == PATH)
	{
		int i = 0;
		//do thing
	}
}

void Ui::BehaviourTypeHandler::on_Pattern_changed(int val)
{
	if (this->m_Current_Type == PATH)
	{
		this->m_Current_Pattern = (Pattern)val;
	}
}

void Ui::BehaviourTypeHandler::on_BehaviourType_changed(int val)
{
	ResetType(this->m_Current_Type);
	this->m_Current_Type = (BehaviourType)val;
}

void Ui::BehaviourTypeHandler::on_Add()
{
	if (this->m_Current_Type == PATH)
	{
		int temp = this->m_WaypointList->count();
		if (temp + 1 <= NUM_WAYPOINTS)
		{
			QString WaypointLabel = "Waypoint ";
			WaypointLabel += QString::number(temp + 1);
			this->m_ListItems[(ListItems)temp] = new QListWidgetItem(WaypointLabel, this->m_WaypointList);

			//do stuff
			
			if (temp == 0) { //if there was no Path when add was clicked, Add new AI component to the model

				//Ask The Ai handler to create a new Path Component
				AIComponent* newComponent = LevelHandler::GetInstance()->GetCurrentLevel()->GetAiHandler()->NewPathComponent();
				this->m_selection->aiComponent = newComponent;
			}
				AIController control(m_selection->aiComponent);
				DirectX::XMVECTOR newPos = m_selection->position;
			
				control.AddWaypoint(newPos);
		}
		
	}
}

void Ui::BehaviourTypeHandler::on_Del()
{
	if (this->m_Current_Type == PATH)
	{
		int currentRow = this->m_WaypointList->currentRow();
		if (currentRow == -1) //no item Selected
			return;
		this->m_WaypointList->takeItem(currentRow);
		delete this->m_ListItems[(ListItems)currentRow];
		this->m_ListItems[(ListItems)currentRow] = nullptr;

		AIController control(m_selection->aiComponent);
		control.RemoveWayPoint(currentRow);
		
		for (int i = currentRow; i < NUM_WAYPOINTS; i++)
		{
			if (currentRow == WAYPOINT8)
				continue;
			else if (this->m_ListItems[(ListItems)i] != nullptr)
				if (this->m_ListItems[(ListItems)i - 1] == nullptr)
				{
					QString WaypointLabel = "Waypoint ";
					WaypointLabel += QString::number(i);
					this->m_ListItems[(ListItems)i]->setText(WaypointLabel);
					this->m_ListItems[(ListItems)i - 1] = this->m_ListItems[(ListItems)i];
					this->m_ListItems[(ListItems)i] = nullptr;

				}
		}

		//do stuff
	}
}
