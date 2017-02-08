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
	m_attributes_widget = ui->CustomBehaviourTabWidget;
	m_availableTriggers = ui->availableTriggers;
	m_triggerList		= ui->TriggerTableWidget;
	m_triggerList->horizontalHeader()->show();
	m_eventBox = ui->EventSignalBox;
	connect(m_triggerList, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(on_triggerSelection_Changed(QTableWidgetItem *)));
	connect(m_eventBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_eventSelection_Changed(int)));
	connect(m_availableTriggers, SIGNAL(currentIndexChanged(int)), this, SLOT(on_availableTriggers_index_Changed(int)));

	m_add_trigger = ui->AddTriggerButton;
	connect(m_add_trigger, SIGNAL(clicked()), this, SLOT(on_Add_Trigger()));
	m_del_trigger = ui->DeleteTriggerButton;
	connect(m_del_trigger, SIGNAL(clicked()), this, SLOT(on_Delete_Trigger()));
	connect(ui->CustomBehaviourTabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_Attributes_tab_changed(int)));
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
	this->m_uniqueID = ui->uniqueIDLabel;
	connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(on_Add()));
	connect(ui->DeleteButton, SIGNAL(clicked()), this, SLOT(on_Del()));
	for (int i = 0; i < NUM_WAYPOINTS; i++)
	{
		this->m_ListItems[(ListItems)i] = nullptr;
	}

	m_button_tagBox   = ui->button_tagBox;
	m_button_distance = ui->button_dist_box;
	m_button_timer    = ui->button_timer_box;
	connect(ui->button_dist_box, SIGNAL(valueChanged(double)), this, SLOT(on_button_distance_Changed(double)));
	this->m_AddCheckpoint = ui->CheckPointADD;
	connect(ui->CheckPointADD, SIGNAL(clicked()), this, SLOT(on_CheckpointAdd()));
	this->m_CheckpointValue = ui->CheckPointValue;
	connect(ui->CheckPointValue, SIGNAL(valueChanged(int)), this, SLOT(on_CheckpointIndex_changed(int)));
	connect(ui->button_timer_box, SIGNAL(valueChanged(double)), this, SLOT(on_button_timer_Changed(double)));



	this->m_door_rotationTime = ui->rotateTimeBox;
	connect(ui->rotateTimeBox, SIGNAL(valueChanged(double)), this, SLOT(on_RotationTime_changed(double)));


#pragma region Lever ui elements

	m_lever_distance = ui->lever_interactionDist;
	connect(m_lever_distance, SIGNAL(valueChanged(double)), this, SLOT(on_lever_distance_changed(double)));
#pragma endregion
#pragma region Wheel ui elements

	m_wheel_minRotation		= ui->wheel_minRotationBox;
	m_wheel_maxRotation		= ui->wheel_maxRotationBox;
	m_wheel_interactionDist	= ui->wheel_interactionDistBox;
	m_wheel_rotationTime	= ui->wheel_rotationTimeBox;
	m_wheel_timeTilReset	= ui->wheel_timeTilResetBox;
	m_wheel_resetTime		= ui->wheel_resetTimeBox;
	connect(m_wheel_minRotation,	 SIGNAL(valueChanged(double)), this, SLOT(on_Wheel_minRotation_changed(double)));
	connect(m_wheel_maxRotation,	 SIGNAL(valueChanged(double)), this, SLOT(on_Wheel_maxRotation_changed(double)));
	connect(m_wheel_interactionDist, SIGNAL(valueChanged(double)), this, SLOT(on_Wheel_interactionDist_changed(double)));
	connect(m_wheel_rotationTime,    SIGNAL(valueChanged(double)), this, SLOT(on_Wheel_rotationTime_changed(double)));
	connect(m_wheel_timeTilReset,	 SIGNAL(valueChanged(double)), this, SLOT(on_Wheel_timeTilReset_changed(double)));
	connect(m_wheel_resetTime,		 SIGNAL(valueChanged(double)), this, SLOT(on_Wheel_resetTime_changed(double)));

#pragma endregion

#pragma region Light ui elements

	m_AmbientLightBoxes[LIntSpin::R] = ui->Amb_R;
	m_AmbientLightBoxes[LIntSpin::G] = ui->Amb_G;
	m_AmbientLightBoxes[LIntSpin::B] = ui->Amb_B;
	m_AmbientLightBoxes[LIntSpin::INTENSITY] = ui->Amb_Intensity;

	connect(m_AmbientLightBoxes[LIntSpin::R], SIGNAL(valueChanged(int)), this, SLOT(on_Ambience_R_changed(int)));
	connect(m_AmbientLightBoxes[LIntSpin::G], SIGNAL(valueChanged(int)), this, SLOT(on_Ambience_G_changed(int)));
	connect(m_AmbientLightBoxes[LIntSpin::B], SIGNAL(valueChanged(int)), this, SLOT(on_Ambience_B_changed(int)));
	connect(m_AmbientLightBoxes[LIntSpin::INTENSITY], SIGNAL(valueChanged(int)), this, SLOT(on_Ambience_Intensity_changed(int)));

	m_AddLightButton = ui->ADD_Light_Button;
	m_LightIntSpinBoxes[LIntSpin::R] = ui->R_Colorvalue;
	m_LightIntSpinBoxes[LIntSpin::G] = ui->G_Colorvalue;
	m_LightIntSpinBoxes[LIntSpin::B] = ui->B_Colorvalue;
	m_LightIntSpinBoxes[LIntSpin::INTENSITY] = ui->IntensityValue;

	connect(m_LightIntSpinBoxes[LIntSpin::R], SIGNAL(valueChanged(int)), this, SLOT(on_R_changed(int)));
	connect(m_LightIntSpinBoxes[LIntSpin::G], SIGNAL(valueChanged(int)), this, SLOT(on_G_changed(int)));
	connect(m_LightIntSpinBoxes[LIntSpin::B], SIGNAL(valueChanged(int)), this, SLOT(on_B_changed(int)));
	connect(m_LightIntSpinBoxes[LIntSpin::INTENSITY], SIGNAL(valueChanged(int)), this, SLOT(on_Intensity_changed(int)));

	m_LightIntSpinBoxes[LIntSpin::RADIUS] = ui->RadiusValue;
	m_LightIntSpinBoxes[LIntSpin::CONSTANT] = ui->ConstantValue;
	m_LightIntSpinBoxes[LIntSpin::LINEAR] = ui->LinearValue;
	m_LightIntSpinBoxes[LIntSpin::QUADRATIC] = ui->QuadValue;

	connect(m_LightIntSpinBoxes[LIntSpin::RADIUS], SIGNAL(valueChanged(int)), this, SLOT(on_Radius_changed(int)));
	connect(m_LightIntSpinBoxes[LIntSpin::CONSTANT], SIGNAL(valueChanged(int)), this, SLOT(on_Constant_changed(int)));
	connect(m_LightIntSpinBoxes[LIntSpin::LINEAR], SIGNAL(valueChanged(int)), this, SLOT(on_Linear_changed(int)));
	connect(m_LightIntSpinBoxes[LIntSpin::QUADRATIC], SIGNAL(valueChanged(int)), this, SLOT(on_Quadratic_changed(int)));
	connect(m_AddLightButton, SIGNAL(clicked()), this, SLOT(on_Light_Add_changed()));

	m_HideLights = ui->HideLight;
	connect(m_HideLights, SIGNAL(toggled(bool)), this, SLOT(on_HideLight_changed(bool)));

#pragma endregion
}

Ui::BehaviourTypeHandler::~BehaviourTypeHandler()
{
}

void Ui::BehaviourTypeHandler::SetSelection(Container *& selection)
{
	if (selection != nullptr)
	{
		Deselect(); //reset values
		m_selection = selection;
		if (m_selection->internalID == 0 || m_selection->internalID == 1 ) { // if any of the spawnpoints are selected
			m_BehaviourType->setCurrentIndex(NONE); //Close the window
			m_Current_Type = NONE; //Update current type
			m_BehaviourType->setEnabled(false);
			return;
		}
		if (m_attributes_widget->currentIndex() == 1) //if the trigger tab is up in the ui. Update it
			SetTriggerData(selection);
		m_BehaviourType->setEnabled(true);

		switch (m_selection->type) {
//BUTTON
#pragma region Button
		case ContainerType::BUTTON:
			m_BehaviourType->setCurrentIndex(BUTTON); //Open the window for Button
			m_Current_Type = BUTTON; //Update current type
			break;
#pragma endregion
////////////
//CHECKPOINT
#pragma region Checkpoint
		case ContainerType::CHECKPOINT:
			this->m_CheckpointValue->setValue(((CheckpointContainer*)m_selection)->checkpointNumber);
			break;
#pragma endregion
/////////
//DOOR
#pragma region Door
		case ContainerType::DOOR:
			m_BehaviourType->setCurrentIndex(DOOR); //Open the window for Door
			m_Current_Type = DOOR; //Update current type
			break;
#pragma endregion
/////////
//AI
#pragma region AI
		case ContainerType::AI:
			m_BehaviourType->setCurrentIndex(PATH); //Open the window for path
			m_Current_Type = PATH; //Update current type
			this->m_Numerics[SPEED]->setValue(((AiContainer*)m_selection)->aiComponent.AC_speed);
			this->m_Numerics[TIME]->setValue(((AiContainer*)m_selection)->aiComponent.AC_time);

			this->m_PATH_TRIGGER->setValue(((AiContainer*)m_selection)->aiComponent.AC_triggered);
			this->m_Path_Trigger_Box->setChecked(((AiContainer*)m_selection)->aiComponent.AC_triggered);
			this->m_Pattern->setCurrentIndex(((AiContainer*)m_selection)->aiComponent.AC_pattern - 1);
			for (int i = 0; i < NUM_WAYPOINTS; i++)
			{
				if (this->m_ListItems[(ListItems)i] != nullptr)
				{
					this->m_WaypointList->takeItem(i);
					delete this->m_ListItems[i];
					this->m_ListItems[i] = nullptr;
				}
			}
			for (size_t i = 0; i < ((AiContainer*)m_selection)->aiComponent.AC_nrOfWaypoint; i++)
			{
				int temp = this->m_WaypointList->count();
				QString WaypointLabel = "Waypoint ";
				WaypointLabel += QString::number(temp + 1);
				this->m_ListItems[(ListItems)temp] = new QListWidgetItem(WaypointLabel, this->m_WaypointList);
			}
			break;
#pragma endregion
/////////
//Wheel
#pragma region WHEEL
		case ContainerType::WHEEL:
			m_BehaviourType->setCurrentIndex(WHEEL); //Open the window for wheel
			m_Current_Type = WHEEL; //Update current type

			break;
#pragma endregion
/////////
//Lever
#pragma region Lever
		case ContainerType::LEVER:
			m_BehaviourType->setCurrentIndex(LEVER); //Open the window for LEVER
			m_Current_Type = LEVER; //Update current type
			break;
#pragma endregion
/////////

#pragma region LIGHT
		case ContainerType::LIGHT:
		
			m_LightIntSpinBoxes[R]->setValue(((Point*)m_selection)->data->color.r * 255);
			m_LightIntSpinBoxes[G]->setValue(((Point*)m_selection)->data->color.g * 255);
			m_LightIntSpinBoxes[B]->setValue(((Point*)m_selection)->data->color.b * 255);
			m_LightIntSpinBoxes[INTENSITY]->setValue(((Point*)m_selection)->data->intensity * 25);

			m_LightIntSpinBoxes[RADIUS]->setValue(((Point*)m_selection)->data->radius * 100);
			m_LightIntSpinBoxes[CONSTANT]->setValue(((Point*)m_selection)->data->falloff.constant * 100);
			m_LightIntSpinBoxes[LINEAR]->setValue(((Point*)m_selection)->data->falloff.linear * 100);
			m_LightIntSpinBoxes[QUADRATIC]->setValue(((Point*)m_selection)->data->falloff.quadratic * 100);
			m_attributes_widget->setCurrentIndex(3);
			break;
#pragma endregion
		default:
			m_BehaviourType->setCurrentIndex(NONE); //Close the window
			m_Current_Type = NONE; //Update current type
		}
	
		
	}
}

void Ui::BehaviourTypeHandler::Deselect()
{
	m_selection = nullptr;
	m_currentEventType = ContainerType::NONE;
	ResetType(this->m_Current_Type); //SHOULD RESET EVERYTHING
	m_BehaviourType->setCurrentIndex(NONE); //Close the window
	m_attributes_widget->setCurrentIndex(0);
	
	ClearTriggerList();
	ClearEventList();
	m_availableTriggers->clear();
	m_availableTriggers->addItem(QString("None"));
	m_Current_Type = NONE; //Update current type
	this->m_CheckpointValue->setValue(0);

	m_LightIntSpinBoxes[R]->setValue(0);
	m_LightIntSpinBoxes[G]->setValue(0);
	m_LightIntSpinBoxes[B]->setValue(0);
	m_LightIntSpinBoxes[INTENSITY]->setValue(0);

	m_LightIntSpinBoxes[RADIUS]->setValue(0);
	m_LightIntSpinBoxes[CONSTANT]->setValue(0);
	m_LightIntSpinBoxes[LINEAR]->setValue(0);
	m_LightIntSpinBoxes[QUADRATIC]->setValue(0);

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
	case Ui::BUTTON:
	{

		m_button_tagBox		->setValue(0);
		m_button_distance	->setValue(0);
		m_button_timer		->setValue(0);
		//reset
		break;
	}
	case Ui::DOOR:
	{
		m_door_rotationTime->setValue(0);
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
		break;
	}
	case Ui::WHEEL:
	{
		m_wheel_interactionDist	->setValue(0.0f);
		m_wheel_rotationTime	->setValue(0.0f);
		m_wheel_timeTilReset	->setValue(0.0f);
		m_wheel_minRotation		->setValue(0.0f);
		m_wheel_maxRotation		->setValue(0.0f);
		m_wheel_resetTime		->setValue(0.0f);

	}

	case Ui::LEVER:
		
		m_lever_distance->setValue(0);
		break;
	default:
		break;
	}
}

void Ui::BehaviourTypeHandler::SetAmbientLight(Ambient amb)
{

	m_AmbientLightBoxes[R]->setValue(amb.r * 255);
	m_AmbientLightBoxes[G]->setValue(amb.g * 255) ;
	m_AmbientLightBoxes[B]->setValue(amb.b * 255) ;
	m_AmbientLightBoxes[INTENSITY]->setValue(amb.intensity * 250);
}

void Ui::BehaviourTypeHandler::on_Time_changed(double val)
{
	if (m_selection == nullptr)
		return;
	if (this->m_Current_Type == PATH)
	{
		int i = 0;
		//do thing
		if (m_selection->type == AI)
		{
			AIController cont(&((AiContainer*)m_selection)->aiComponent);
			cont.SetTime(val);
		}
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
	if (m_selection == nullptr)
		return;
	if (this->m_Current_Type == PATH)
	{
		int i = 0;
		//do thing
		if (m_selection->type == AI)
		{
			AIController cont(&((AiContainer*)m_selection)->aiComponent);
			cont.SetSpeed((float)val);
		}
	}
}

void Ui::BehaviourTypeHandler::on_Pattern_changed(int val)
{

 	if (this->m_Current_Type == PATH)
	{
		this->m_Current_Pattern = (Pattern)(val + 1);
	}
	if (m_selection == nullptr)
		return;
	if (m_selection->type == AI )
	{
		AIController cont(&((AiContainer*)m_selection)->aiComponent);
		cont.SetPattern(val + 1);
	}
}

void Ui::BehaviourTypeHandler::on_BehaviourType_changed(int val)
{

	
		ResetType(this->m_Current_Type);
		this->m_Current_Type = (BehaviourType)val;
		m_BehaviourType->setCurrentIndex(val);
		//remove AI COMP
		if (m_selection != nullptr)
		{
			if (m_selection->type == AI && val != PATH)
			{
				AIController cont(&((AiContainer*)m_selection)->aiComponent);
				cont.DeletePath();
				return;
			}
		
			else {
				switch (val)
				{
				case BehaviourType::NONE: //if none, convert back to normal container

					if (m_selection->type != ContainerType::MODEL)
					{
						Container* cont = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToContainer(m_selection); // convert to container
						m_selection->isDirty = true;
					}
					break;
				case BehaviourType::BUTTON: 
				{
					if (m_selection->type == ContainerType::MODEL) {
						Button* newButton = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToButton(m_selection); //convert from container to button
						m_selection->isDirty = true;
					}
					else if (m_selection->type != ContainerType::MODEL && m_selection->type != ContainerType::BUTTON) //if the selection is not a container or button, 
					{		
						Container* cont = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToContainer(m_selection); // convert to container
						Button* newButton = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToButton(m_selection); //convert from container to button
						m_selection->isDirty = true;
					}
					
					m_button_tagBox->setValue((int)m_selection->internalID);
					m_button_distance->setValue(((Button*)m_selection)->interactionDistance);
					m_button_timer->setValue(((Button*)m_selection)->resetTime);
					break;
				}
				case BehaviourType::DOOR:
				{
					if (m_selection->type == ContainerType::MODEL) {
						Door* newDoor = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToDoor(m_selection); //convert from container to door
						m_selection->isDirty = true;
					}
					else if (m_selection->type != ContainerType::MODEL && m_selection->type != ContainerType::DOOR) //if the selection is not a container or door, 
					{
						Container* cont = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToContainer(m_selection); // convert to container
						Door* newDoor = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToDoor(m_selection); //convert from container to door
						m_selection->isDirty = true;
					}

					m_door_rotationTime->setValue(((Door*)m_selection)->rotateTime);
					break;
				}
				case BehaviourType::PATH:
				{
					if (m_selection->type == ContainerType::MODEL) {
						AiContainer* newAI = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToAI(m_selection); //convert from container to ai
						m_selection->isDirty = true;
					}
					else if (m_selection->type != ContainerType::MODEL && m_selection->type != ContainerType::AI) //if the selection is not a container or button, 
					{
						Container* cont = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToContainer(m_selection); // convert to container
						AiContainer* newAI = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToAI(m_selection); //convert from container to ai
						m_selection->isDirty = true;
					}
					break;
				}
				case BehaviourType::WHEEL:
				{
					if (m_selection->type == ContainerType::MODEL) {
						Wheel* newWheel = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToWheel(m_selection); //convert from container to wheel
						m_selection->isDirty = true;
					}
					else if (m_selection->type != ContainerType::MODEL && m_selection->type != ContainerType::WHEEL) //if the selection is not a container or door, 
					{
						Container* cont = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToContainer(m_selection); // convert to container
						Wheel* newWheel = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToWheel(m_selection);	   //convert from container to wheel
						m_selection->isDirty = true;
					}

					m_wheel_interactionDist	 ->setValue(((Wheel*)m_selection)->interactionDistance)	 ;
					m_wheel_rotationTime	 ->setValue(((Wheel*)m_selection)->rotateTime)			 ;
					m_wheel_timeTilReset	 ->setValue(((Wheel*)m_selection)->timeToReset)			 ;
					m_wheel_minRotation		 ->setValue(((Wheel*)m_selection)->minRotation)			 ;
					m_wheel_maxRotation		 ->setValue(((Wheel*)m_selection)->maxRotation)			 ;
					m_wheel_resetTime		 ->setValue(((Wheel*)m_selection)->resetTime)			 ;
					break;
				}

				case BehaviourType::LEVER:
				{
					if (m_selection->type == ContainerType::MODEL) {
						Lever* newLever = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToLever(m_selection); //convert from container to lever
						m_selection->isDirty = true;
					}
					else if (m_selection->type != ContainerType::MODEL && m_selection->type != ContainerType::LEVER) //if the selection is not a container or door, 
					{
						Container* cont = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToContainer(m_selection); // convert to container
						Lever* newLever = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToLever(m_selection);	   //convert from container to lever
						m_selection->isDirty = true;
					}
					m_lever_distance->setValue(((Lever*)m_selection)->interactionDistance);
					break;
				}
					
				}
			}
		}
}
void Ui::BehaviourTypeHandler::on_button_distance_Changed(double val)
{

	if (m_selection == nullptr || m_selection->type != BUTTON)
		return;
	((Button*)m_selection)->interactionDistance = (float)val;
}

void Ui::BehaviourTypeHandler::on_button_timer_Changed(double val)
{
	if (m_selection == nullptr || m_selection->type != BUTTON)
		return;
	((Button*)m_selection)->resetTime = (float)val;
}

void Ui::BehaviourTypeHandler::on_CheckpointAdd()
{
	LevelHandler::GetInstance()->GetCurrentLevel()->AddCheckpointEntity();
}

void Ui::BehaviourTypeHandler::on_CheckpointIndex_changed(int val)
{
	if (m_selection == nullptr || m_selection->type != CHECKPOINT)
		return;
	else 
	{
		((CheckpointContainer*)m_selection)->checkpointNumber = val;
	}
}

void Ui::BehaviourTypeHandler::on_Attributes_tab_changed(int val)
{
	if (m_selection)
	{
		if (val == 1)
			SetTriggerData(m_selection);
	}

}
#pragma region AI functions
void Ui::BehaviourTypeHandler::on_Add()
{
	if (m_selection == nullptr)
		return;
	if (this->m_Current_Type == PATH)
	{
		int temp = this->m_WaypointList->count();
		if (temp + 1 <= NUM_WAYPOINTS)
		{
			QString WaypointLabel = "Waypoint ";
			WaypointLabel += QString::number(temp + 1);
			this->m_ListItems[(ListItems)temp] = new QListWidgetItem(WaypointLabel, this->m_WaypointList);

			//do stuff
			
			//if (temp == 0) { //if there was no Path when add was clicked, Add new AI component to the model
				//Ask The Ai handler to create a new Path Component
				if (m_selection->type != AI)
				{

					AiContainer* newComponent = LevelHandler::GetInstance()->GetCurrentLevel()->ConvertToAI(m_selection);
					newComponent->aiComponent.AC_entityID = m_selection->internalID;
				}
			//}
			AIController control(&((AiContainer*)m_selection)->aiComponent);
			DirectX::XMVECTOR newPos = m_selection->position;
			newPos = ((AiContainer*)m_selection)->OBBCenterPos;
			control.AddWaypoint(newPos);
		}
	}
}

void Ui::BehaviourTypeHandler::on_Del()
{
	if (m_selection == nullptr)
		return;

	if (this->m_Current_Type == PATH)
	{
		int currentRow = this->m_WaypointList->currentRow();
		if (currentRow == -1) //no item Selected
			return;
		this->m_WaypointList->takeItem(currentRow);
		delete this->m_ListItems[(ListItems)currentRow];
		this->m_ListItems[(ListItems)currentRow] = nullptr;

		AIController control(&((AiContainer*)m_selection)->aiComponent);
		control.RemoveWayPoint(currentRow);		//Remove the waypoint


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
#pragma endregion

#pragma region Door functions
void Ui::BehaviourTypeHandler::on_RotationTime_changed(double val)
{
	
	if (m_selection == nullptr || m_selection->type != ContainerType::DOOR)
		return;

	((Door*)m_selection)->rotateTime = (float)val;

}
#pragma endregion

#pragma region Wheel callbacks
void Ui::BehaviourTypeHandler::on_Wheel_minRotation_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::WHEEL)
		return;
	
	((Wheel*)m_selection)->minRotation = (float)val;
	
}

void Ui::BehaviourTypeHandler::on_Wheel_maxRotation_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::WHEEL)
		return;
	
	((Wheel*)m_selection)->maxRotation = (float)val;

}

void Ui::BehaviourTypeHandler::on_Wheel_interactionDist_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::WHEEL)
		return;


	((Wheel*)m_selection)->interactionDistance = (float)val;
	

}

void Ui::BehaviourTypeHandler::on_Wheel_rotationTime_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::WHEEL)
		return;
	((Wheel*)m_selection)->rotateTime = (float)val;
	

}

void Ui::BehaviourTypeHandler::on_Wheel_timeTilReset_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::WHEEL)
		return;

	((Wheel*)m_selection)->timeToReset = (float)val;
	

}

void Ui::BehaviourTypeHandler::on_Wheel_resetTime_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::WHEEL)
		return;
	((Wheel*)m_selection)->resetTime = (float)val;

}
#pragma endregion
#pragma region Lever callbacks
void Ui::BehaviourTypeHandler::on_lever_distance_changed(double val)
{
	if (m_selection == nullptr || m_selection->type != ContainerType::LEVER)
		return;

	((Lever*)m_selection)->interactionDistance = (float)val;

}

#pragma endregion
#pragma region Trigger Functions

void Ui::BehaviourTypeHandler::SetTriggerData(Container *& selection)
{
	if (selection->type == ContainerType::MODEL || selection->type == ContainerType::CHECKPOINT || selection->type == ContainerType::LIGHT)
		return;

	//this->m_triggerTab->setEnabled(true);
	
	Level* currentLevel = LevelHandler::GetInstance()->GetCurrentLevel();
	//m_triggerList->clear();

	ClearTriggerList();
	if (((ListenerContainer*)selection)->numTriggers > 0) // if the selected container has any triggers
	{
		
	
	
		for (size_t i = 0; i < ((ListenerContainer*)selection)->numTriggers; i++)
		{
			//This is a quick and ugly fix. Basically, ask the level if the trigger exists. So that it wont crash if a connected trigger has been
			//deleted. Unfortunately this is done for every trigger

			Container* trigger = LevelHandler::GetInstance()->GetCurrentLevel()->GetInstanceEntity(((ListenerContainer*)selection)->triggerEntityIds[i]);
			if (trigger == nullptr) { //this means it has been deleted completely
				((ListenerContainer*)selection)->DeleteTrigger(((ListenerContainer*)selection)->triggerEntityIds[i]);
				//if (((ListenerContainer*)selection)->numTriggers < 1)//if there are no other triggers
				//	continue;
				i -= 1;
				continue;
			}

			int signal = -1;
			Container* heldTrigger = ((ListenerContainer*)selection)->triggerContainers[i];
			if (heldTrigger == nullptr)
				heldTrigger = trigger;
			if (heldTrigger->type != trigger->type) { // Try to get the entity, (if it has been converted)
				((ListenerContainer*)selection)->triggerContainers[i] = LevelHandler::GetInstance()->GetCurrentLevel()->GetInstanceEntity(((ListenerContainer*)selection)->triggerEntityIds[i]);
				
			}
			else{
				signal = ((ListenerContainer*)selection)->listenEvent[i];
			}


			if (trigger->type == ContainerType::MODEL) {								// this is a check to make sure that the trigger is not a model.	   									   
				((ListenerContainer*)selection)->DeleteTrigger(trigger->internalID); 	// This is because, you can add a button, then convert that button to a model,
				// when deleting,														// In that case, every listener that is connected to that button needs to remove that connection
				// every trigger is moved one step to the front in the array.			// So if the selected objects connections has changed to something that is not a trigger.
				//so decrease i by one, to iterate over 0 again							// The trigger will be removed
				i -= 1;
				continue;																
			}																																						
			AddTriggerItemToList(trigger, trigger->type, signal);
		}
		m_triggerList->selectRow(0);
		if (((ListenerContainer*)m_selection)->triggerContainers[m_triggerList->currentRow()] == nullptr) {
			((ListenerContainer*)m_selection)->triggerContainers[m_triggerList->currentRow()] = currentLevel->GetInstanceEntity(((ListenerContainer*)m_selection)->triggerEntityIds[m_triggerList->currentRow()]);	
		}
		if (((ListenerContainer*)m_selection)->numTriggers > 0) //Make sure that there are any triggers
		{
			m_currentEventType = ((ListenerContainer*)m_selection)->triggerContainers[m_triggerList->currentRow()]->type;
			SetEventListByType(m_currentEventType);
			QString string = m_eventStrings.GetStringFromEnumID(((ListenerContainer*)m_selection)->listenEvent[m_triggerList->currentRow()]); //Get the string of the EVENT enum
			m_eventBox->setCurrentText(string); //Set the correct string item in the event box
		}
		
	}
	
	m_availableTriggers->clear();
	m_availableTriggers->addItem(QString("None"));
	for (size_t i = 0; i < ContainerType::NUM_PUZZLE_ELEMENTS; i++)
	{
		const std::vector<Container*> * container = currentLevel->GetPuzzleElements(ContainerType(i));
		
		for (size_t j = 0; j < container->size(); j++)
		{
			if (container->at(j)->internalID == selection->internalID) // do not make it possible to connect any instance to itself
				continue;

			QString name = m_triggerType[i];
			name.append(QString::number(container->at(j)->internalID));
			
		   m_availableTriggers->addItem(name, qVariantFromValue((void*)container->at(j)));	
		}
	}
}

void Ui::BehaviourTypeHandler::AddTriggerItemToList(Container *& trigger, ContainerType type, int signal)
{
	QTableWidgetItem* newItem = new QTableWidgetItem();
	QString name = m_triggerType[type]; // Get Type of container as name
	name.append(QString::number(trigger->internalID)); //append the id after the type in the name
	newItem->setText(name); //set the name
	newItem->setData(Qt::UserRole, qVariantFromValue((void*)trigger)); //Set a container pointer to the new item in the list
																	   //selection
	QString eventName;
	if (signal == -1)
		eventName = "None";
	else
		eventName = m_eventStrings.GetStringFromEnumID(signal);

	m_triggerList->insertRow(m_triggerList->rowCount()); //insert a new row
	m_triggerList->setItem(m_triggerList->rowCount() - 1, 0, newItem); // insert the new item

	m_triggerList->setItem(m_triggerList->rowCount() - 1, 1, new QTableWidgetItem(eventName)); //insert a string for the event
	m_triggerList->selectRow(m_triggerList->rowCount() - 1); //select the new item

}

void Ui::BehaviourTypeHandler::ClearTriggerList()
{
	while (m_triggerList->rowCount() > 0) {
		m_triggerList->removeRow(m_triggerList->rowCount() - 1);
	}
}

void Ui::BehaviourTypeHandler::ClearEventList()
{
	m_eventBox->clear();

}

EVENT Ui::BehaviourTypeHandler::SetEventListByType(ContainerType type)
{
	if (type == NONE)
		return EVENT::WHEEL_RESET;
	std::vector<QString>*  strings = m_eventStrings.GetEventStringsFromType(m_currentEventType);
	m_eventBox->clear();
	for (size_t i = 0; i < strings->size(); i++)
	{
		m_eventBox->insertItem(i, strings->at(i));
	}
	return  EVENT(m_eventStrings.GetEnumIdFromString(strings->at(0)));
}

void Ui::BehaviourTypeHandler::on_Delete_Trigger()
{
	if (m_triggerList->rowCount() > 0)
	{
		Container* selection = (Container*)m_triggerList->selectedItems().at(0)->data(Qt::UserRole).value<void*>();
		((ListenerContainer*)m_selection)->DeleteTrigger(selection->internalID);
		m_triggerList->removeRow(m_triggerList->currentRow());
	}

}

void Ui::BehaviourTypeHandler::on_Add_Trigger()
{

	if (m_availableTriggers->currentIndex() <= 0 || m_triggerList->rowCount() >= 20)
		return;
	Container* selection = (Container*)m_availableTriggers->currentData(Qt::UserRole).value<void*>(); // get the pointer to the selected container
	//EVENT triggerEvent = EVENT(-1);
	int eventSignal = -1;
	if (m_currentEventType != selection->type){
		m_currentEventType = selection->type;
		eventSignal = (int) SetEventListByType(selection->type); //Get a standard value
	}
	else{
		
		eventSignal =m_eventStrings.GetEnumIdFromString(m_eventBox->currentText());
	}
	
	if (((ListenerContainer*)m_selection)->AddTrigger(selection, EVENT(eventSignal))) //add the trigger to the selected component
	{

		AddTriggerItemToList(selection, selection->type, eventSignal); // if successfull, add it to the ui list
	}


}

void Ui::BehaviourTypeHandler::on_eventSelection_Changed(int val)
{
	if (val <= 0 || m_triggerList->rowCount() < 1)
		return;
	Container* selected = (Container*)m_triggerList->selectedItems().at(0)->data(Qt::UserRole).value<void*>();
	if (selected == nullptr)
		return;

	if (m_currentEventType != selected->type)
	{
		this->on_triggerSelection_Changed(m_triggerList->selectedItems().at(0)); //if by some reason, the wrong events show up on the selected object.
	}
	((ListenerContainer*)m_selection)->UpdateTriggerEvent(selected, EVENT(m_eventStrings.GetEnumIdFromString(m_eventBox->currentText())));
	m_triggerList->item(m_triggerList->currentRow(), 1)->setText(m_eventBox->currentText()); //take the text from the event box and apply that to the trigger list

}

void Ui::BehaviourTypeHandler::on_availableTriggers_index_Changed(int index)
{
	if (m_availableTriggers->currentIndex() <= 0)
		return;
	Container* selected = (Container*)m_availableTriggers->currentData(Qt::UserRole).value<void*>(); // get the pointer to the selected container
	if (selected == nullptr)
		return;
	if (m_currentEventType != selected->type)
	{
		m_currentEventType = selected->type;
		SetEventListByType(selected->type);
	}


}

void Ui::BehaviourTypeHandler::on_triggerSelection_Changed(QTableWidgetItem * item)
{
	Container* selected = (Container*)m_triggerList->selectedItems().at(0)->data(Qt::UserRole).value<void*>();

	if (selected == nullptr) //some QT sorcery sometimes calls this callback when an item is deleted
		return;
	if (m_currentEventType != selected->type)
	{
		m_currentEventType = selected->type;
		SetEventListByType(selected->type);
	}
	QString hej = m_eventStrings.GetStringFromEnumID(((ListenerContainer*)m_selection)->listenEvent[m_triggerList->currentRow()]);
	m_eventBox->setCurrentText(hej);
}
#pragma endregion
void Ui::BehaviourTypeHandler::on_Light_Add_changed()
{
	LevelHandler::GetInstance()->GetCurrentLevel()->AddPointLight();

}

#pragma region Light callbacks

void Ui::BehaviourTypeHandler::on_R_changed(int val)
{
	const int dpi = 255; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->color.r != realVal) {
			((Point*)m_selection)->data->color.r = float(realVal);
			this->m_selection->isDirty = true;
		}
	}

}
void Ui::BehaviourTypeHandler::on_G_changed(int val)
{
	const int dpi = 255; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->color.g != realVal) {
			((Point*)m_selection)->data->color.g = float(realVal);
			this->m_selection->isDirty = true;
		}
	}

}
void Ui::BehaviourTypeHandler::on_B_changed(int val)
{
	const int dpi = 255; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->color.b != realVal) {
			((Point*)m_selection)->data->color.b = float(realVal);
			this->m_selection->isDirty = true;
		}
	}

}
void Ui::BehaviourTypeHandler::on_Intensity_changed(int val)
{
	const int dpi = 25; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->intensity != realVal) {
			((Point*)m_selection)->data->intensity = float(realVal);
			this->m_selection->isDirty = true;
		}
	}
}
void Ui::BehaviourTypeHandler::on_Ambience_R_changed(int val)
{
	const int dpi = 255; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value
	LightController::GetInstance()->SetAmbientR(realVal);
}
void Ui::BehaviourTypeHandler::on_Ambience_G_changed(int val)
{
	const int dpi = 255; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value
	LightController::GetInstance()->SetAmbientG(realVal);
}
void Ui::BehaviourTypeHandler::on_Ambience_B_changed(int val)
{
	const int dpi = 255; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value
	LightController::GetInstance()->SetAmbientB(realVal);
}
void Ui::BehaviourTypeHandler::on_Ambience_Intensity_changed(int val)
{
	const int dpi = 250; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value
	LightController::GetInstance()->SetAmbientIntensity(realVal);
}
void Ui::BehaviourTypeHandler::on_Radius_changed(int val)
{
	const int dpi = 100; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->radius != realVal) {
			((Point*)m_selection)->data->radius = realVal;
			this->m_selection->isDirty = true;
		}
	}
}
void Ui::BehaviourTypeHandler::on_Constant_changed(int val)
{
	const int dpi = 100; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->falloff.constant != realVal) {
			((Point*)m_selection)->data->falloff.constant = realVal;
			this->m_selection->isDirty = true;
		}
	}
}
void Ui::BehaviourTypeHandler::on_Linear_changed(int val)
{
	const int dpi = 100; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->falloff.linear != realVal) {
			((Point*)m_selection)->data->falloff.linear = realVal;
			this->m_selection->isDirty = true;
		}
	}
}
void Ui::BehaviourTypeHandler::on_Quadratic_changed(int val)
{
	const int dpi = 100; // Any constant 10^n
	double realVal = double(val) / double(dpi); // float value

	if (m_selection != nullptr) {
		if (m_selection->type == LIGHT && ((Point*)m_selection)->data->falloff.quadratic != realVal) {
			((Point*)m_selection)->data->falloff.quadratic = realVal;
			this->m_selection->isDirty = true;
		}
	}
}
void Ui::BehaviourTypeHandler::on_HideLight_changed(bool val)
{
	LightController::GetInstance()->DisplayLightRadius(val);
}
#pragma endregion