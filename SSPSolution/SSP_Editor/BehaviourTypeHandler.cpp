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
	connect(m_triggerList, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(on_triggerSelection_Changed(QTableWidgetItem *)));
	m_eventBox = ui->EventSignalBox;
	connect(m_eventBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_eventSelection_Changed(int)));

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
		if (m_selection->internalID == 0 || m_selection->internalID == 1 || m_selection->type == ContainerType::CHECKPOINT) { // if any of the spawnpoints are selected
			m_BehaviourType->setCurrentIndex(NONE); //Close the window
			m_Current_Type = NONE; //Update current type
			m_BehaviourType->setEnabled(false);
			return;
		}
		if (m_attributes_widget->currentIndex() == 1) //if the trigger tab is up in the ui. Update it
			SetTriggerData(selection);
		m_BehaviourType->setEnabled(true);
		if (m_selection->aiComponent != nullptr) //If the selection is a platform. Load up the platform UI and fill it with the data
		{
 			m_BehaviourType->setCurrentIndex(PATH); //Open the window for path
 			m_Current_Type = PATH; //Update current type
			this->m_Numerics[SPEED]->setValue(m_selection->aiComponent->AC_speed);
			this->m_Numerics[TIME]->setValue(m_selection->aiComponent->AC_time);

			this->m_PATH_TRIGGER->setValue(m_selection->aiComponent->AC_triggered);
			this->m_Path_Trigger_Box->setChecked(m_selection->aiComponent->AC_triggered);
			this->m_Pattern->setCurrentIndex(selection->aiComponent->AC_pattern - 1);
			for (int i = 0; i < NUM_WAYPOINTS; i++)
			{
				if (this->m_ListItems[(ListItems)i] != nullptr)
				{
					this->m_WaypointList->takeItem(i);
					delete this->m_ListItems[i];
					this->m_ListItems[i] = nullptr;
				}
			}

			for (size_t i = 0; i < m_selection->aiComponent->AC_nrOfWaypoint; i++)
			{
				int temp = this->m_WaypointList->count();
				QString WaypointLabel = "Waypoint ";
				WaypointLabel += QString::number(temp + 1);
				this->m_ListItems[(ListItems)temp] = new QListWidgetItem(WaypointLabel, this->m_WaypointList);
			}
			return;
		
		}
		switch (m_selection->type) {
		//BUTTON
		case ContainerType::BUTTON:
			m_BehaviourType->setCurrentIndex(BUTTON); //Open the window for Button
			m_Current_Type = BUTTON; //Update current type
			break;
		////////////
		//CHECKPOINT
		case ContainerType::CHECKPOINT:
			this->m_CheckpointValue->setValue(((CheckpointContainer*)m_selection)->checkpointNumber);
			break;
		/////////
		//DOOR
		case ContainerType::DOOR:
			m_BehaviourType->setCurrentIndex(DOOR); //Open the window for Door
			m_Current_Type = DOOR; //Update current type
			break;
		/////////

		default:
			m_BehaviourType->setCurrentIndex(NONE); //Close the window
			m_Current_Type = NONE; //Update current type
		}
	
		
	}
}

void Ui::BehaviourTypeHandler::Deselect()
{
	m_selection = nullptr;
	ResetType(this->m_Current_Type); //SHOULD RESET EVERYTHING
	m_BehaviourType->setCurrentIndex(NONE); //Close the window
	ClearTriggerList();
	ClearEventList();
	m_availableTriggers->clear();
	m_availableTriggers->addItem(QString("None"));
	m_Current_Type = NONE; //Update current type
	this->m_CheckpointValue->setValue(0);
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
		break;
	}
	default:
		break;
	}
}

void Ui::BehaviourTypeHandler::on_Time_changed(double val)
{
	if (m_selection == nullptr)
		return;
	if (this->m_Current_Type == PATH)
	{
		int i = 0;
		//do thing
		if (m_selection->aiComponent != nullptr)
		{
			AIController cont(m_selection->aiComponent);
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
		if (m_selection->aiComponent != nullptr)
		{
			AIController cont(m_selection->aiComponent);
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
	if (m_selection->aiComponent != nullptr)
	{
		AIController cont(m_selection->aiComponent);
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
			if (m_selection->aiComponent != nullptr && val != PATH)
			{
				AIController cont(m_selection->aiComponent);
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
				}
				}
			}
		}
}
void Ui::BehaviourTypeHandler::on_button_distance_Changed(double val)
{

	assert(m_selection->type == ContainerType::BUTTON);
	((Button*)m_selection)->interactionDistance = (float)val;
}

void Ui::BehaviourTypeHandler::on_button_timer_Changed(double val)
{
	assert(m_selection->type == ContainerType::BUTTON);
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
				if (m_selection->aiComponent == nullptr)
				{
					AIComponent* newComponent = LevelHandler::GetInstance()->GetCurrentLevel()->GetAiHandler()->NewPathComponent();
					this->m_selection->aiComponent = newComponent;
					newComponent->AC_entityID = m_selection->internalID;
				}
			//}
				AIController control(m_selection->aiComponent);
				DirectX::XMVECTOR newPos = m_selection->position;
			
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

		AIController control(m_selection->aiComponent);
		control.RemoveWayPoint(currentRow);		//Remove the waypoint

		if (this->m_WaypointList->count() <= 0) //if There is no waypoints, remove the ai component
		{
			LevelHandler::GetInstance()->GetCurrentLevel()->GetAiHandler()->DeletePathComponent(m_selection->aiComponent->AC_entityID);
			m_selection->aiComponent = nullptr;
		}
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

void Ui::BehaviourTypeHandler::on_triggerSelection_Changed(QTableWidgetItem * item)
{
	Container* selected = (Container*)m_triggerList->selectedItems().at(0)->data(Qt::UserRole).value<void*>();
	
	if (selected == nullptr) //some QT sorcery sometimes calls this callback when an item is deleted
		return;
	if (m_currentEventType != selected->type)
	{
		m_currentEventType = selected->type;
		ClearEventList();
		std::vector<QString>*  strings = m_eventStrings.GetEventStringsFromType(selected->type);
		for (size_t i = 0; i < strings->size(); i++)
		{
			m_eventBox->insertItem(i+1, strings->at(i));
		}
	}
	QString hej = m_eventStrings.GetStringFromEnumID(((ListenerContainer*)m_selection)->listenEvent[m_triggerList->currentRow()]);
	m_eventBox->setCurrentText(hej);
}

void Ui::BehaviourTypeHandler::on_eventSelection_Changed(int val)
{
	if (val <= 0 ||m_triggerList->rowCount() < 1)
		return;
	Container* selected = (Container*)m_triggerList->selectedItems().at(0)->data(Qt::UserRole).value<void*>();
	if (selected == nullptr) 
		return;

	if (m_currentEventType != selected->type)
	{
		this->on_triggerSelection_Changed(m_triggerList->selectedItems().at(0)); //if by some reason, the wrong events show up on the selected object.
	}
	((ListenerContainer*)m_selection)->UpdateTriggerEvent(selected,EVENT(m_eventStrings.GetEnumIdFromString(m_eventBox->currentText())));
	m_triggerList->item(m_triggerList->currentRow(),1)->setText(m_eventBox->currentText()); //take the text from the event box and apply that to the trigger list
	
}

void Ui::BehaviourTypeHandler::on_Add_Trigger()
{

	if (m_availableTriggers->currentIndex() <= 0 || m_triggerList->rowCount() >= 20) 
		return;
	Container* selection = (Container*)m_availableTriggers->currentData(Qt::UserRole).value<void*>(); // get the pointer to the selected container
	
	m_currentEventType = selection->type;
	std::vector<QString>*  strings = m_eventStrings.GetEventStringsFromType(m_currentEventType);
	EVENT triggerEvent = EVENT(m_eventStrings.GetEnumIdFromString(strings->at(0)));
	
	//QString hej = m_eventBox->currentText();
	//triggerEvent = EVENT(m_eventStrings.GetEnumIdFromString(m_eventBox->currentText())); //Get the right EVENT enum integer.
//	m_eventStrings.GetEventStringsFromType(selection->type)->at(0);
	if (((ListenerContainer*)m_selection)->AddTrigger(selection, triggerEvent)) //add the trigger to the selected component
	{
		//m_triggerList->setCurrentCell(-1, -1);
		m_eventBox->clear();
	;
		m_eventBox->insertItem(0, "None");
		for (size_t i = 1; i < strings->size(); i++)
		{
			m_eventBox->insertItem(i, strings->at(i));
		}

		AddTriggerItemToList(selection, selection->type, -1); // if successfull, add it to the ui list
	}


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

void Ui::BehaviourTypeHandler::on_RotationTime_changed(double val)
{
	if (m_selection->type != ContainerType::DOOR)
		return;

	((Door*)m_selection)->rotateTime = (float)val;

}

void Ui::BehaviourTypeHandler::SetTriggerData(Container *& selection)
{
	if (selection->type == ContainerType::MODEL || selection->type == ContainerType::CHECKPOINT)
		return;

	//this->m_triggerTab->setEnabled(true);
	
	Level* currentLevel = LevelHandler::GetInstance()->GetCurrentLevel();
	//m_triggerList->clear();

	ClearTriggerList();
	if (((ListenerContainer*)selection)->numTriggers > 0) // if the selected container has any triggers
	{
		std::vector<QString>*  strings = m_eventStrings.GetEventStringsFromType(m_currentEventType);
		for (size_t i = 0; i < strings->size(); i++)
		{
			m_eventBox->insertItem(i + 1, strings->at(i));
		}
	
	
		for (size_t i = 0; i < ((ListenerContainer*)selection)->numTriggers; i++)
		{
			Container* trigger = ((ListenerContainer*)selection)->triggerContainers[i];
			if (trigger == nullptr) {
				trigger = LevelHandler::GetInstance()->GetCurrentLevel()->GetInstanceEntity(((ListenerContainer*)selection)->triggerEntityIds[i]);
			}


			if (trigger->type == ContainerType::MODEL) {								// this is a check to make sure that the trigger is not a model.	   									   
				((ListenerContainer*)selection)->DeleteTrigger(trigger->internalID); 	// This is because, you can add a button, then convert that button to a model,
				// when deleting,														// In that case, every listener that is connected to that button needs to remove that connection
				// every trigger is moved one step to the front in the array.			// So if the selected objects connections has changed to something that is not a trigger.
				//so decrease i by one, to iterate over 0 again							// The trigger will be removed
				i -= 1;
				continue;																
			}																																						
			AddTriggerItemToList(trigger, trigger->type, ((ListenerContainer*)selection)->listenEvent[i]);
		}
		m_triggerList->selectRow(0);
		QString string = m_eventStrings.GetStringFromEnumID(((ListenerContainer*)m_selection)->listenEvent[m_triggerList->currentRow()]); //Get the string of the EVENT enum
		m_eventBox->setCurrentText(string); //Set the correct string item in the event box
		
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
	//m_eventBox->insertItem(0, "None");
}
