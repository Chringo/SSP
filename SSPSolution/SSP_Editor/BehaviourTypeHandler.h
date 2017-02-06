#ifndef SSPEDITOR_UI_BEHAVIOURHANDLER_H
#define SSPEDITOR_UI_BEHAVIOURHANDLER_H
#include "ui_SSP_Editor.h"
#include <qobject.h>
#include "AIController.h"
#include "Header.h"
#include "LevelHandler.h"
#include <qtablewidget.h>
#include "EVENT_Strings.h"
namespace Ui {
	enum BehaviourType {
		NONE = 0,
		BUTTON,
		DOOR,
		PATH,
		LEVER,
		WHEEL,
		PRESSURE_PLATE,
	};
	enum Pattern {
		AI_LINEAR = 1,
		AI_CIRCULAR,
		AI_ROUNTRIP,
		AI_RANDOM,
		AI_NONE = -1
	};
	enum Numerics {
		SPEED = 0,
		TIME,
		NUM_NUMERICS
	};
	enum ListItems {
		WAYPOINT1 = 0,
		WAYPOINT2,
		WAYPOINT3,
		WAYPOINT4,
		WAYPOINT5,
		WAYPOINT6,
		WAYPOINT7,
		WAYPOINT8,
		NUM_WAYPOINTS
	};

	enum LIntSpin
	{
		R, G, B,
		INTENSITY,
		RADIUS,
		CONSTANT,
		LINEAR,
		QUADRATIC,
		NUM_LINTSPIN
	};
	class BehaviourTypeHandler : QObject
	{

		Q_OBJECT
	private:
		EventStrings		m_eventStrings;
		QTabWidget*			m_attributes_widget;
		BehaviourType		m_Current_Type;
		Pattern				m_Current_Pattern;
		unsigned int		m_Current_Waypoint_Amt = 0;
		QComboBox* 			m_BehaviourType;
		QDoubleSpinBox*		m_Numerics[NUM_NUMERICS];
		QCheckBox*			m_Path_Trigger_Box;
		QCheckBox*			m_HideLights;
		QSpinBox*			m_PATH_TRIGGER;
		QComboBox*			m_Pattern;
		QListWidget*		m_WaypointList;
		QListWidgetItem*	m_ListItems[NUM_WAYPOINTS];
		QPushButton*		m_Add;
		QPushButton*		m_Del;
		QPushButton*		m_Up;
		QPushButton*		m_Down;
		QPushButton*		m_AddCheckpoint;
		QPushButton*		m_AddLightButton;
		QSpinBox*			m_CheckpointValue;

		QLabel*			m_uniqueID;
		Container*		m_selection = nullptr;
	

#pragma region Button behaviour elements
		QSpinBox	   * m_button_tagBox;
		QDoubleSpinBox * m_button_distance;
		QDoubleSpinBox * m_button_timer;
#pragma endregion

#pragma region Door behaviour elements
		QDoubleSpinBox * m_door_rotationTime;
#pragma endregion

#pragma region Lever behavour elements
		QDoubleSpinBox * m_lever_distance;
#pragma endregion
#pragma region Wheel behaviour elements

		QDoubleSpinBox *  m_wheel_minRotation;
		QDoubleSpinBox *  m_wheel_maxRotation;
		QDoubleSpinBox *  m_wheel_interactionDist;
		QDoubleSpinBox *  m_wheel_rotationTime	  ;
		QDoubleSpinBox *  m_wheel_timeTilReset	  ;
		QDoubleSpinBox *  m_wheel_resetTime;
		QSpinBox*			m_LightIntSpinBoxes[NUM_LINTSPIN];
		QSpinBox*		  m_AmbientLightBoxes[4];
#pragma endregion



#pragma region Trigger Tab elements

		ContainerType m_currentEventType = ContainerType::BUTTON;
		QWidget*     m_triggerTab;
		QComboBox*   m_availableTriggers;
		QComboBox*	 m_eventBox;
		QTableWidget* m_triggerList;
		QPushButton* m_add_trigger;
		QPushButton* m_del_trigger;
		QString m_triggerType[NUM_PUZZLE_ELEMENTS]{
			"(unknown)",
			"(button) ",
			"(lever) ",
			"(wheel) ",
			"(door) ",
			"(magnet) ",
			"(plate) "
		};
		

#pragma endregion

	public:
		BehaviourTypeHandler();
		BehaviourTypeHandler(const Ui::SSP_EditorClass* ui);
		void Initialize(const Ui::SSP_EditorClass* ui);
		~BehaviourTypeHandler();
		void SetSelection(Container*& selection);
		void Deselect();
		void UpdateSelection();
		void ResetType(BehaviourType);
		void SetAmbientLight(Ambient amb);

		public slots:
		void on_Speed_changed(double val);
		void on_Time_changed(double val);
		void on_Path_Trigger_changed(int val);
		void on_Path_Trigger_Box_changed(int val);
		void on_Pattern_changed(int val);
		void on_BehaviourType_changed(int val);
		void on_button_distance_Changed(double val);
		void on_button_timer_Changed(double val);
		void on_CheckpointAdd();
		void on_CheckpointIndex_changed(int val);
		void on_Attributes_tab_changed(int val);

		void on_Add();
		void on_Del();

		void on_availableTriggers_index_Changed(int index);
		void on_triggerSelection_Changed(QTableWidgetItem * item);
		void on_eventSelection_Changed(int val);
		void on_Add_Trigger();
		void on_Delete_Trigger();

		void on_RotationTime_changed(double val);


#pragma region Wheel callbacks
		void on_Wheel_minRotation_changed(double val);
		void on_Wheel_maxRotation_changed(double val);
		void on_Wheel_interactionDist_changed(double val);
		void on_Wheel_rotationTime_changed(double val);
		void on_Wheel_timeTilReset_changed(double val);
		void on_Wheel_resetTime_changed(double val);
#pragma endregion

		void on_Light_Add_changed();

#pragma region Light callbacks


		void on_R_changed(int val);
		void on_G_changed(int val);
		void on_B_changed(int val);
		void on_Intensity_changed(int val);

		void on_Ambience_R_changed(int val);
		void on_Ambience_G_changed(int val);
		void on_Ambience_B_changed(int val);
		void on_Ambience_Intensity_changed(int val);

		void on_Radius_changed(int val);
		void on_Constant_changed(int val);
		void on_Linear_changed(int val);
		void on_Quadratic_changed(int val);
		void on_HideLight_changed(bool val);
#pragma endregion
#pragma region Lever callbacks
		void on_lever_distance_changed(double val);
#pragma endregion


	private:
		void SetTriggerData(Container*& selection);
		void AddTriggerItemToList(Container*& trigger, ContainerType type, int signal);
		void ClearTriggerList();
		void ClearEventList();
		EVENT SetEventListByType(ContainerType type); //Return the first event in the list
	};
}
#endif