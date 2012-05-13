#include "ui_GuiControls.h"
#include "gamearea.h"
#include "guiactions.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include "global.h"
#include "filter_data.h"
#include "libbsmart/vector2.h"
#include <ctime>
#include "GuiPropertiesDlg.h"

GuiActions::GuiActions(Ui::GuiControls* gui, QObject* win) :
		QObject(win), m_gui(gui) {
	brokenRulesModel = new QStandardItemModel(0, 5);
	brokenRulesModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Time")));
	brokenRulesModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Rulename")));
	brokenRulesModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Team")));
	brokenRulesModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Bot")));
	brokenRulesModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Frame")));
}

GuiActions::~GuiActions() {
}

void GuiActions::connectActions() {
	connect(m_gui->actionAbout_Qt, SIGNAL ( triggered() ), qApp, SLOT ( aboutQt() ));
	connect(m_gui->actionFull_Screen, SIGNAL ( toggled ( bool ) ), this, SLOT ( fullscreen ( bool ) ));
	connect(m_gui->actionOPen, SIGNAL ( triggered() ), this, SLOT ( showPropertiesDlg() ));

	//Buttons for load/start logfile
	connect(m_gui->record_log, SIGNAL ( clicked() ), m_gui->gamearea->vision, SLOT ( record() ));
	connect(m_gui->load_log, SIGNAL ( clicked() ), m_gui->gamearea->vision, SLOT ( play_record() ));
	connect(m_gui->gamearea->vision, SIGNAL ( change_record_button ( QString ) ), this,
			SLOT ( change_record_button ( QString ) ));
	connect(m_gui->gamearea->vision, SIGNAL ( change_play_button ( QString ) ), this,
			SLOT ( change_play_button ( QString ) ));

	//Buttons for logfile control
	connect(m_gui->log_forward, SIGNAL ( clicked() ), m_gui->gamearea->vision->log_control, SLOT ( log_forward() ));
	connect(m_gui->log_play, SIGNAL ( clicked() ), m_gui->gamearea->vision->log_control, SLOT ( log_play() ));
	connect(m_gui->log_backward, SIGNAL ( clicked() ), m_gui->gamearea->vision->log_control, SLOT ( log_backward() ));
	connect(m_gui->log_pause, SIGNAL ( clicked() ), m_gui->gamearea->vision->log_control, SLOT ( log_pause() ));
	connect(m_gui->log_faster, SIGNAL ( clicked() ), m_gui->gamearea->vision->log_control, SLOT ( log_faster() ));
	connect(m_gui->log_slower, SIGNAL ( clicked() ), m_gui->gamearea->vision->log_control, SLOT ( log_slower() ));
	connect(m_gui->log_frame_back, SIGNAL ( clicked() ), this, SLOT ( log_frame_back() ));
	connect(m_gui->log_frame_forward, SIGNAL ( clicked() ), this, SLOT ( log_frame_forward() ));

	//Slider control
	connect(m_gui->gamearea->vision, SIGNAL ( initializeSlider ( int,int,int,int,int ) ), this,
			SLOT ( initializeSlider ( int, int, int, int, int ) ));
//        connect(m_gui->gamearea->vision, SIGNAL(update_frame(int)), m_gui->horizontalSlider,              SLOT(setValue(int)));
	connect(m_gui->horizontalSlider, SIGNAL ( actionTriggered ( int ) ), this, SLOT ( slider_action ( int ) ));
	connect(this, SIGNAL ( goto_frame ( int ) ), m_gui->gamearea->vision->log_control, SLOT ( goto_frame ( int ) ));
	connect(m_gui->horizontalSlider, SIGNAL ( sliderPressed() ), m_gui->gamearea->vision->log_control,
			SLOT ( log_pause() ));
	connect(m_gui->horizontalSlider, SIGNAL ( sliderReleased() ), m_gui->gamearea->vision->log_control,
			SLOT ( log_resume() ));
	connect(m_gui->gamearea, SIGNAL ( resizeSlider ( int ) ), this, SLOT ( resizeSlider ( int ) ));

	//Log Control
	connect(m_gui->gamearea->vision, SIGNAL ( showLogControl ( bool ) ), m_gui->logControl,
			SLOT ( setVisible ( bool ) ));
	connect(m_gui->gamearea, SIGNAL ( showLogControl ( bool ) ), m_gui->logControl, SLOT ( setVisible ( bool ) ));
	connect(m_gui->gamearea->vision, SIGNAL ( update_frame ( int ) ), this, SLOT ( update_frame ( int ) ));

	//QLCDNumber control
	//    connect(m_gui->gamearea->vision, SIGNAL(update_frame(int)), m_gui->log_frameNumber, SLOT(display(int)));
	connect(m_gui->gamearea->vision, SIGNAL ( log_size ( int ) ), m_gui->log_totalFrames, SLOT ( display ( int ) ));
	connect(m_gui->gamearea->vision->log_control, SIGNAL ( update_speed ( QString ) ), m_gui->log_speed,
			SLOT ( setText ( QString ) ));

	//Ball Status
	connect(m_gui->gamearea->particle_filter, SIGNAL ( change_ball_status ( QString ) ), this,
			SLOT ( change_ball_status ( QString ) ));
	connect(m_gui->gamearea->particle_filter, SIGNAL ( change_ball_last_touched ( QString ) ), this,
			SLOT ( change_ball_last_touched ( QString ) ));

	//Rule System Data
	connect(m_gui->show_rules, SIGNAL ( clicked() ), m_gui->gamearea, SLOT ( show_rules_changed() ));
	connect(m_gui->gamearea, SIGNAL ( change_show_rules ( QString ) ), this, SLOT ( change_show_rules ( QString ) ));

	// frame text box
	connect(m_gui->log_frameNumber, SIGNAL ( textChanged() ), this, SLOT ( gotoFrameInTextBox() ));
	connect(m_gui->gamearea->vision->log_control, SIGNAL ( enable_log_frameNumber( bool ) ), this,
			SLOT ( setLogFrameNumberEnabled(bool) ));

	// broken rules
	connect(m_gui->gamearea->rules, SIGNAL ( new_broken_rule( Broken_Rule* ) ), this,
			SLOT ( insert_into_lst_broken_rules( Broken_Rule* ) ));
	connect(m_gui->tbl_brokenRules, SIGNAL ( clicked ( QModelIndex ) ), this,
			SLOT ( brokenRuleRowSelected( QModelIndex ) ));
}

void GuiActions::fullscreen(bool f) {
	QWidget* win = qobject_cast<QWidget*>(parent());
	if (f) {
		win->showFullScreen();
	} else {
		win->showNormal();
	}
}

void GuiActions::initializeSlider(int min, int max, int singleStep, int pageStep, int tickInterval) {
	m_gui->horizontalSlider->setRange(min, max);
	m_gui->horizontalSlider->setSingleStep(singleStep);
	m_gui->horizontalSlider->setPageStep(pageStep);
	m_gui->horizontalSlider->setValue(min);
	m_gui->horizontalSlider->setTickPosition(QSlider::TicksAbove);
	m_gui->horizontalSlider->setTickInterval(tickInterval);
}

void GuiActions::resizeSlider(int width) {
	//distances
	int top = m_gui->log_backward->geometry().top();
	int width_new = (width - 30) / 4;
	int width_old = m_gui->log_backward->geometry().width();
	int height = m_gui->log_backward->geometry().height();
	int dist_x = m_gui->log_pause->geometry().left() - width_old;
	int dist_y = m_gui->log_slower->geometry().top() - height - top;
	//Main Stuff
	m_gui->horizontalSlider->setFixedWidth(width);
	m_gui->logControlWidget->setFixedWidth(width);
	m_gui->logControl->setFixedWidth(width);

	//Buttons
	//First Column
	m_gui->log_backward->setGeometry(0, top, width_new, height);
	m_gui->log_slower->setGeometry(0, top + height + dist_y, width_new, height);
	m_gui->log_frame_back->setGeometry(0, top + (height + dist_y) * 2, width_new, height);
	//Second Column
	m_gui->log_pause->setGeometry(width_new + dist_x, top, width_new, height);
	m_gui->log_speed->setGeometry(width_new + dist_x, top + height + dist_y, width_new, height);
	m_gui->log_frameNumber->setGeometry(width_new + dist_x, top + (height + dist_y) * 2, width_new, height);
	//Third Column
	m_gui->log_forward->setGeometry((width_new + dist_x) * 2, top, width_new, height);
	m_gui->log_faster->setGeometry((width_new + dist_x) * 2, top + height + dist_y, width_new, height);
	m_gui->log_frame_forward->setGeometry((width_new + dist_x) * 2, top + (height + dist_y) * 2, width_new, height);
	//Fourth Column
	m_gui->log_play->setGeometry((width_new + dist_x) * 3, top, width_new, height);
	m_gui->log_totalFrames->setGeometry((width_new + dist_x) * 3, top + (height + dist_y) * 2, width_new, height);
}

void GuiActions::change_record_button(QString text) {
	m_gui->record_log->setText(text);
}

void GuiActions::change_play_button(QString text) {
	m_gui->load_log->setText(text);
}

void GuiActions::change_ball_status(QString text) {
	m_gui->ball_status->setText(text);
}

void GuiActions::change_ball_last_touched(QString text) {
	m_gui->ball_last_touched->setText(text);
}

void GuiActions::update_frame(int frame) {
	if (m_gui->gamearea->vision->log_control->get_play_speed() != 0.0) {
		if (frame % ((int) m_gui->gamearea->vision->log_control->get_play_speed() * 10) == 0)
			force_update_frame(frame);
	}
}

void GuiActions::force_update_frame(int frame) {
	m_gui->horizontalSlider->setValue(frame);
	m_gui->log_frameNumber->setPlainText(QString::number(frame));
}

void GuiActions::slider_action(int action) {
	if (action != 0) {
		emit goto_frame(m_gui->horizontalSlider->value());
		m_gui->log_frameNumber->setPlainText(
				QString::number(m_gui->gamearea->vision->log_control->get_current_frame()));
	}
}

void GuiActions::gotoFrameInTextBox() {
//	m_gui->log_frameNumber->setAlignment( Qt::AlignCenter );
	QString frameStr = m_gui->log_frameNumber->toPlainText();

	if (!frameStr.endsWith('\n')) {
		return;
	}

	bool suc = 0;
	int frame = frameStr.toInt(&suc, 10);
//	if(m_gui->gamearea->vision->log_control->get_play_speed()==0.0 && suc) {
	emit goto_frame(frame);
	force_update_frame(m_gui->gamearea->vision->log_control->get_current_frame());
//	}
}

void GuiActions::change_show_rules(QString text) {
	m_gui->show_rules->setText(text);
}

void GuiActions::setLogFrameNumberEnabled(bool b) {
	m_gui->log_frameNumber->setEnabled(b);
}

void GuiActions::log_frame_back() {
	m_gui->gamearea->vision->log_control->log_frame_back();
	force_update_frame(m_gui->gamearea->vision->log_control->get_current_frame());
}

void GuiActions::log_frame_forward() {
	m_gui->gamearea->vision->log_control->log_frame_forward();
	force_update_frame(m_gui->gamearea->vision->log_control->get_current_frame());
}

void GuiActions::insert_into_lst_broken_rules(Broken_Rule *brokenRule) {
	if (m_gui->tbl_brokenRules->model() == NULL) {
		m_gui->tbl_brokenRules->setModel(brokenRulesModel);
//		m_gui->tbl_brokenRules->setColumnHidden(4, true);
	}
	if (brokenRule != NULL && brokenRule->rule_number >= 0) {
		QString rulename = Global::rulenames[brokenRule->rule_number - 1].c_str();
		QList<QStandardItem*> *row = new QList<QStandardItem*>();

		// time
		struct tm * timeinfo;
		BSmart::Time_Value timestamp = brokenRule->when_broken;
		timestamp /= 1000;
		timeinfo = localtime((time_t*) &timestamp);
		const char* time = ctime((const time_t*) &timestamp);
		QStandardItem *itmTimeBroken = new QStandardItem(time);
		row->append(itmTimeBroken);

		// rule
		QStandardItem *itmRulename = new QStandardItem(rulename);
		row->append(itmRulename);

		// team
		std::ostringstream team;
		if (brokenRule->rule_breaker.x == 0) {
			team << "yellow";
		} else {
			team << "blue";
		}
		QStandardItem *itmTeam = new QStandardItem(team.str().c_str());
		row->append(itmTeam);

		// rule breaker
		std::ostringstream ruleBreaker;
		ruleBreaker << brokenRule->rule_breaker.y;
		QStandardItem *itmRulebreaker = new QStandardItem(ruleBreaker.str().c_str());
		row->append(itmRulebreaker);

		// frame
		std::ostringstream frame;
		frame << brokenRule->frame_broken;
		QStandardItem *itmFrame = new QStandardItem(frame.str().c_str());
		row->append(itmFrame);

		// insert
		brokenRulesModel->insertRow(0, *row);
	}
}

void GuiActions::brokenRuleRowSelected(QModelIndex index) {
	int frame = brokenRulesModel->data(brokenRulesModel->index(index.row(), 4)).toInt();
	frame -= 100;
	if(frame < 0) frame = 0;
	emit goto_frame(frame);
	force_update_frame(m_gui->gamearea->vision->log_control->get_current_frame());
}

void GuiActions::showPropertiesDlg() {
	GuiPropertiesDlg *propDlg = new GuiPropertiesDlg();
	propDlg->show();
}
