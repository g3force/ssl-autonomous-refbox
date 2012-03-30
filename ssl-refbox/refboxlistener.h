/**
 * @file refboxlistener.h
 * @brief RefboxListener header file
 */
#ifndef REFBOXLISTENER
#define REFBOXLISTENER

#include <QThread>
#include <QMutex>

#include <libbsmart/multicast_socket.h>
#include <libbsmart/game_states.h>

#include <log4cxx/logger.h>

/**
 * @class RefboxListener
 * @brief Listen for refbox commands on socket and deal with it
 * Socket will be opened, and listened to. Commands will be catched and actions executed, if necessary
 */
class RefboxListener : public QThread
{
    Q_OBJECT

public:
    struct GameStatePacket
    {
        char           cmd;            // Current referee command
        unsigned char  cmd_counter;    // Increments on each new command
        unsigned char  goals_blue;     // current score for blue team
        unsigned char  goals_yellow;   // current score for yellow team
        unsigned short time_remaining; // Seconds remaining for game stage
    };

    RefboxListener(BSmart::Game_States*);
    ~RefboxListener();
    void run();

public slots:
    void new_refbox_cmd(char);

private:
    static log4cxx::LoggerPtr logger;
    void execute();
    BSmart::Multicast_Socket* socket;
    BSmart::Game_States* gamestate;

    GameStatePacket gsp_last;
    char* buffer;
    size_t buflen;
    bool has_new_data;

    void input_serial(const char);
    void update_game_state(const int);


};

#endif //REFBOXLISTENER
