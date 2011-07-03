#ifndef GAMEAREA_H
#define GAMEAREA_H
#include <QtOpenGL/QGLWidget>
#include <QWaitCondition>
#include <libbsmart/game_states.h>
#include "sslvision.h"
#include "pre_filter_data.h"
#include "refboxlistener.h"
#include "filter_data.h"
#include "particle_filter.h"
#include "pf_tester.h"
#include "glextra.h"
#include "ssl_refbox_rules.h"

class Gamearea : public QGLWidget
{
    Q_OBJECT

public:
	Gamearea(QWidget*);
	~Gamearea();
	void paintGL();
	void initializeGL();
	void resizeGL(int, int);
	void timerEvent(QTimerEvent*);
	void bitmap_output(void*);
    QWaitCondition* rules_wait_condition;
    QWaitCondition* new_data_wait_condition;
    Pre_Filter_Data* pf_data;
    BSmart::Game_States* gamestate;
    SSLVision* vision;
    RefboxListener* refbox_listener;
    Filter_Data* filter_data;
    Particle_Filter_Mother* particle_filter;
    PF_Tester* pf_tester;
    SSL_Refbox_Rules* rules;

public slots:
    //draw
    void show_world();
    void show_rules_changed();

signals:
    //optimize Slider size
    void resizeSlider(int);
    void showLogControl(bool);
    void change_show_rules(QString);

private:
    int m_timer;
    GLuint  m_field;
    GLuint  world_model;
    GLExtra glextra;

    BSmart::Time_Value start_time;
    bool show_rule_data;
};
#endif /* GAMEAREA_H */
