/**
 * @file sslvision.h
 * @brief SSLVision header file
 */
#ifndef SSLVISION
#define SSLVISION

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <proto/messages_robocup_ssl_detection.pb.h>
#include <proto/messages_robocup_ssl_geometry.pb.h>
#include <proto/messages_robocup_ssl_wrapper.pb.h>
#include <proto/messages_robocup_ssl_refbox_log.pb.h>
#include <libbsmart/multicast_socket.h>
#include <libbsmart/game_states.h>
#include "pre_filter_data.h"
#include "log_control.h"
#include <log4cxx/logger.h>

/**
 * @brief data store for current data received from SSL-vision/log file
 */
struct Transformed_Percept
{
    int cam_id;
    unsigned int ball_frame_number;
    Ball_Percept_List balls;
    bool has_one_ball;
    BSmart::Pose ball_direction_before;
    BSmart::Pose ball_direction_after;

    Robot_Percept_List robots[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
    bool has_one_robot[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
    BSmart::Pose robot_direction[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];

    std::string refbox_cmd;
    int current_frame;
    int sleep_time;
    BSmart::Time_Value frame_received;

};

/**
 * @class SSLVision
 * @brief The SSLVision class receives data from ssl-vision and prepares it.
 * It also handles log files.
 */
class SSLVision : public QThread
{
    Q_OBJECT

public:
    SSLVision(Pre_Filter_Data*, BSmart::Game_States*, QWaitCondition*);
    ~SSLVision();
    void run();
    Log_Control* log_control;

public slots:
    void record();
    void play_record();

signals:
    //gamearea->glDraw
    void new_frame();
    //Slider and QLCDNumber
    void update_frame(int);
    //QLCDNumber
    void log_size(int);
    //Slider
    void initializeSlider(int,int,int,int,int);
    //LogControl
    void showLogControl(bool);
    //change Button text
    void change_record_button(QString);
    void change_play_button(QString);
    //refboxlistener
    void new_refbox_cmd(char);

private:
    static log4cxx::LoggerPtr logger;
    int execute(Transformed_Percept&);
    int standard_sleep_time;
    QWaitCondition* new_data_wait_condition;

    int  recv(SSL_DetectionFrame&);
    void process_balls(Transformed_Percept&);
    void process(Transformed_Percept&, char);
    void process_blue(Transformed_Percept&);
    void process_yellow(Transformed_Percept&);
    void reset_data(int);
    int npc;

    BSmart::Multicast_Socket* socket;

    bool queue_filled;
    Transformed_Percept transformed_percept;
    std::vector<Transformed_Percept> tf_percept_queue_all;
    std::vector<Transformed_Percept> tf_percept_queue_one_ball[2];
    std::vector<Transformed_Percept> tf_percept_queue_one_robot[2][Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
    void reset_transformed_percept(Transformed_Percept&);
    void analyse_percepts();

    int robot_r;
    int cam_height;
    int cam_width;
    //constant from ssl-vision/src/shared/net/robocup_ssl_client.h
    static const int MaxDataGramSize = 65536;
    char* buffer;
    SSL_DetectionFrame frame;

    Pre_Filter_Data* data;
    BSmart::Game_States* gamestate;

    Refbox_Log logs;
//    int current_frame;
    bool rec;
    void start_record();
    int end_record();
    bool play;
    int start_play_record();
    void end_play_record();
    QString fileName;
};

#endif //SSLVISION
