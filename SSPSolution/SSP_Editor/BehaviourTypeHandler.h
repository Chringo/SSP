#ifndef SSPEDITOR_UI_BEHAVIOURHANDLER_H
#define SSPEDITOR_UI_BEHAVIOURHANDLER_H
#include "ui_SSP_Editor.h"
#include <qobject.h>
#include "AIController.h"
#include "Header.h"
#include "LevelHandler.h"
namespace Ui {
	enum BehaviourType {
		NONE = 0,
		BUTTON,
		DOOR,
		PATH,
		LEVER,
		WHEEL,
		PRESSURE_PLATE
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
	class BehaviourTypeHandler : QObject
	{

		Q_OBJECT
	private:
		BehaviourType		m_Current_Type;
		Pattern				m_Current_Pattern;
		unsigned int		m_Current_Waypoint_Amt = 0;
		QComboBox* 			m_BehaviourType;
		QDoubleSpinBox*		m_Numerics[NUM_NUMERICS];
		QCheckBox*			m_Path_Trigger_Box;
		QSpinBox*			m_PATH_TRIGGER;
		QComboBox*			m_Pattern;
		QListWidget*		m_WaypointList;
		QListWidgetItem*	m_ListItems[NUM_WAYPOINTS];
		QPushButton*		m_Add;
		QPushButton*		m_Del;
		QPushButton*		m_Up;
		QPushButton*		m_Down;
		QPushButton*		m_AddCheckpoint;
		QSpinBox*			m_CheckpointValue;

		QLabel*			m_uniqueID;
		Container*		m_selection = nullptr;
	public:
		BehaviourTypeHandler();
		BehaviourTypeHandler(const Ui::SSP_EditorClass* ui);
		void Initialize(const Ui::SSP_EditorClass* ui);
		~BehaviourTypeHandler();
		void SetSelection(Container* selection);
		void Deselect();
		void UpdateSelection();
		void ResetType(BehaviourType);

		public slots:
		void on_Speed_changed(double val);
		void on_Time_changed(double val);
		void on_Path_Trigger_changed(int val);
		void on_Path_Trigger_Box_changed(int val);
		void on_Pattern_changed(int val);
		void on_BehaviourType_changed(int val);
		void on_CheckpointAdd();
		void on_CheckpointIndex_changed(int val);

		void on_Add();
		void on_Del();
	};
}
#endif