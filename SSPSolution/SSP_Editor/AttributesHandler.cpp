#include "AttributesHandler.h"



Ui::AttributesHandler::AttributesHandler()
{
}


Ui::AttributesHandler * Ui::AttributesHandler::GetInstance()
{
	static Ui::AttributesHandler instance;
	return &instance;
}

Ui::AttributesHandler::AttributesHandler(const Ui::SSP_EditorClass * ui)
{
	this->Initialize(ui);

}

void Ui::AttributesHandler::Initialize(const Ui::SSP_EditorClass * ui)
{

	m_numericBoxes[TRANSLATE][X] = ui->xValue_translate;
	m_numericBoxes[TRANSLATE][Y] = ui->yValue_translate;
	m_numericBoxes[TRANSLATE][Z] = ui->zValue_translate;
	connect(ui->xValue_translate, SIGNAL(valueChanged(double)), this, SLOT(on_translate_X_changed(double)));
	connect(ui->yValue_translate, SIGNAL(valueChanged(double)), this, SLOT(on_translate_Y_changed(double)));
	connect(ui->zValue_translate, SIGNAL(valueChanged(double)), this, SLOT(on_translate_Z_changed(double)));

	m_numericBoxes[ROTATE][X] = ui->xValue_rot;
	m_numericBoxes[ROTATE][Y] = ui->yValue_rot;
	m_numericBoxes[ROTATE][Z] = ui->zValue_rot;
	connect(ui->xValue_rot, SIGNAL(valueChanged(double)), this, SLOT(on_rotation_X_changed(double)));
	connect(ui->yValue_rot, SIGNAL(valueChanged(double)), this, SLOT(on_rotation_Y_changed(double)));
	connect(ui->zValue_rot, SIGNAL(valueChanged(double)), this, SLOT(on_rotation_Z_changed(double)));

	m_numericBoxes[SCALE][X] = ui->xValue_scale;
	m_numericBoxes[SCALE][Y] = ui->yValue_scale;
	m_numericBoxes[SCALE][Z] = ui->zValue_scale;
	connect(ui->xValue_scale, SIGNAL(valueChanged(double)), this, SLOT(on_scale_X_changed(double)));
	connect(ui->yValue_scale, SIGNAL(valueChanged(double)), this, SLOT(on_scale_Y_changed(double)));
	connect(ui->zValue_scale, SIGNAL(valueChanged(double)), this, SLOT(on_scale_Z_changed(double)));

	m_nameBox = ui->lineEdit;

	m_uniqueID = ui->uniqueIDLabel;

	m_isStaticBox = ui->isStaticCheck;
	connect(ui->isStaticCheck, SIGNAL(stateChanged(int)), this, SLOT(on_isStatic_changed(int)));
	m_BehaviourHandler = new BehaviourTypeHandler(ui);
	m_BehaviourHandler->Initialize(ui);

	Deselect();
}

Ui::AttributesHandler::~AttributesHandler()
{
	delete this->m_BehaviourHandler;
}

void Ui::AttributesHandler::SetSelection(Container * selection)
{
	if (selection != nullptr)
	{
		m_selection = selection;
		m_numericBoxes[TRANSLATE][X]->setValue(selection->position.m128_f32[X]);
		m_numericBoxes[TRANSLATE][Y]->setValue(selection->position.m128_f32[Y]);
		m_numericBoxes[TRANSLATE][Z]->setValue(selection->position.m128_f32[Z]);


		m_numericBoxes[ROTATE][X]->setValue(selection->rotation.m128_f32[X]);
		m_numericBoxes[ROTATE][Y]->setValue(selection->rotation.m128_f32[Y]);
		m_numericBoxes[ROTATE][Z]->setValue(selection->rotation.m128_f32[Z]);

		m_numericBoxes[SCALE][X]->setValue(1.0f);
		m_numericBoxes[SCALE][Y]->setValue(1.0f);
		m_numericBoxes[SCALE][Z]->setValue(1.0f);

		m_nameBox->setText("oops");
		
		m_uniqueID->setText(QString::number(selection->internalID));
	
		m_isStaticBox->setChecked(m_selection->isStatic);
		m_BehaviourHandler->SetSelection(selection);
	
	}

}

void Ui::AttributesHandler::Deselect()
{
	m_selection = nullptr;
	m_numericBoxes[TRANSLATE][X]->setValue (0);
	m_numericBoxes[TRANSLATE][Y]->setValue (0);
	m_numericBoxes[TRANSLATE][Z]->setValue (0);


	m_numericBoxes[ROTATE][X]->setValue(0);
	m_numericBoxes[ROTATE][Y]->setValue(0);
	m_numericBoxes[ROTATE][Z]->setValue(0);

	m_numericBoxes[SCALE][X]->setValue(0);
	m_numericBoxes[SCALE][Y]->setValue(0);
	m_numericBoxes[SCALE][Z]->setValue(0);

	m_uniqueID->setText(QString::number(0));
	m_BehaviourHandler->Deselect();

}

void Ui::AttributesHandler::UpdateSelection()
{
	SetSelection(m_selection); // This function updates. no need to rewrite
}

void Ui::AttributesHandler::on_translate_X_changed(double val)
{
	if (m_selection != nullptr) {
		m_selection->position.m128_f32[X] = float(val);
		m_selection->isDirty = true;
	}
}

void Ui::AttributesHandler::on_translate_Y_changed(double val)
{
	if(m_selection != nullptr){
		m_selection->position.m128_f32[Y] = float(val);
		m_selection->isDirty = true;
	}
}

void Ui::AttributesHandler::on_translate_Z_changed(double val)
{
	if (m_selection != nullptr) {
		m_selection->position.m128_f32[Z] = float(val);
		m_selection->isDirty = true;
	}
}

void Ui::AttributesHandler::on_rotation_X_changed(double val)
{
	if (m_selection != nullptr) {
		m_selection->rotation.m128_f32[X] = float(val);
		m_selection->isDirty = true;
	}
}

void Ui::AttributesHandler::on_rotation_Y_changed(double val)
{
	if (m_selection != nullptr) {
		m_selection->rotation.m128_f32[Y] = float(val);
		m_selection->isDirty = true;
	}
}

void Ui::AttributesHandler::on_rotation_Z_changed(double val)
{
	if (m_selection != nullptr) {
		m_selection->rotation.m128_f32[Z] = float(val);
		m_selection->isDirty = true;
	}
}

void Ui::AttributesHandler::on_scale_X_changed(double val)
{
	if (m_selection != nullptr) {
	}
}

void Ui::AttributesHandler::on_scale_Y_changed(double val)
{
	if (m_selection != nullptr) {}
}

void Ui::AttributesHandler::on_scale_Z_changed(double val)
{
	if (m_selection != nullptr) {}
}

void Ui::AttributesHandler::on_isStatic_changed(int state)
{
	if (m_selection != nullptr)
	{
		switch (Qt::CheckState(state)) {

		case Qt::Unchecked:
			m_selection->isStatic = false;
			break;
		case Qt::Checked:
			m_selection->isStatic = true;
			break;
		}
	}
}
