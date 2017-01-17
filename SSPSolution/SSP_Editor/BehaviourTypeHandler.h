#ifndef SSPEDITOR_UI_ATTRHANDLER_H
#define SSPEDITOR_UI_ATTRHANDLER_H
#include "ui_SSP_Editor.h"
#include <qobject.h>
#include "Header.h"
namespace Ui {
	enum BehaviourType {
		NONE = 0,
		TRIGGER,
		DOOR,
		PATH
	};
	enum Pattern {
		CIRCULAR = 0,
		ONE_WAY,
		ROUND_TRIP
	};
	enum Numerics {
		SPEED = 0,
		TIME,
		NUM_NUMERICS
	};
	class BehaviourTypeHandler : QObject
	{

		Q_OBJECT
	private:
		BehaviourTypeHandler();
		BehaviourType	m_Current_Type;
		Pattern		m_Current_Pattern;
		QComboBox* 		m_BehaviourType;
		QDoubleSpinBox* m_Numerics[NUM_NUMERICS];
		QComboBox*		m_Pattern;
		QListWidget*	m_WaypointList;
		QPushButton*	m_Add;
		QPushButton*	m_Del;
		QPushButton*	m_Up;
		QPushButton*	m_Down;

		QLabel*			m_uniqueID;
		Container*		m_selection = nullptr;
	public:
		static BehaviourTypeHandler* GetInstance(); // Singleton
		BehaviourTypeHandler(const Ui::SSP_EditorClass* ui);
		void Initialize(const Ui::SSP_EditorClass* ui);
		~BehaviourTypeHandler();
		void SetSelection(Container* selection);
		void Deselect();
		void UpdateSelection();

		public slots:
		void on_Speed_changed(double val);
		void on_Time_changed(double val);
		void on_Pattern_changed(int val);
		void on_BehaviourType_changed(int val);

		void on_Add(double val);
		void on_Del(double val);
		void on_Up(double val);
		void on_Down(int state);
	};
}
#endif