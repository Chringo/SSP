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
	connect(ui->SpeedValue, SIGNAL(valueChanged(double)), this, SLOT(on_Speed_changed(double)));
	connect(ui->SpeedValue, SIGNAL(valueChanged(double)), this, SLOT(on_Time_changed(double)));

	this->m_BehaviourType = ui->BehaviourDropDown;
	this->m_Pattern = ui->PatternDropDown;
	connect(ui->BehaviourDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(on_BehaviourType_changed(int)));
	connect(ui->PatternDropDown, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Pattern_changed(int)));

	this->m_WaypointList = ui->WaypointListWidget;

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
		m_uniqueID->setText(QString::number(selection->internalID));
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

void Ui::BehaviourTypeHandler::on_Time_changed(double val)
{
	if (this->m_Current_Type == PATH)
	{
		int i = 0;
		//do thing
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
	//MUST ADD A CHECK THAT IF CURRENT TYPE WAS (SAY PATH) ALL PATH STUFF IS REMOVED BEFORE CHANGING CURRENT TYPE!
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

void Ui::BehaviourTypeHandler::on_Up(double val)
{
	if (this->m_Current_Type == PATH)
	{
		//do thing
	}
}

void Ui::BehaviourTypeHandler::on_Down(int state)
{
	if (this->m_Current_Type == PATH)
	{
		//do thing
	}
}
