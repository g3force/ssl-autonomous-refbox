#include "ssl_refbox_rules.h"
#include <QMutex>
#include <iostream>
#include <SWI-Prolog.h>
#include "global.h"
#include <libbsmart/field.h>

// log4cxx
using namespace log4cxx;
LoggerPtr SSL_Refbox_Rules::logger ( Logger::getLogger ( "SSL_Refbox_Rules" ) );

char* argv_global;

SSL_Refbox_Rules::SSL_Refbox_Rules ( QWaitCondition* rules_wait_condition_,
                                     Filter_Data* filter_data_, BSmart::Game_States* gamestate_ )
{
    rules_wait_condition = rules_wait_condition_;
    filter_data = filter_data_;
    gamestate = gamestate_;
    play_state_old = BSmart::Game_States::HALTED;
    written = false;
    written_halftime = false;
    last_break = -1;
    touches = 0;
    internal_play_states = BSmart::Int_Vector ( 0, 0 );
}

SSL_Refbox_Rules::~SSL_Refbox_Rules()
{
    PL_halt ( 0 );
}

void SSL_Refbox_Rules::run()
{
    int result; // currently not really needed, but is used to eliminate warning

    /* Construction of arguments to initialize Prolog */
    char* program = argv_global;
    char *plav[2];
    plav[0] = program;
    plav[1] = NULL;

    /* Initialization of Prolog */

    if ( !PL_initialise ( 1, plav ) )
        PL_halt ( 1 );

    /* Field definitions */

    term_t field_width = PL_new_term_refs ( 8 );
    term_t field_height = field_width + 1;
    term_t goal_width = field_width + 2;
    term_t goal_depth = field_width + 3;
    term_t goal_height = field_width + 4;
    term_t defense_radius = field_width + 5;
    term_t defense_line = field_width + 6;
    term_t penalty_mark = field_width + 7;
    result = PL_put_integer ( field_width, BSmart::Field::field_width );
    result = PL_put_integer ( field_height, BSmart::Field::field_height );
    result = PL_put_integer ( goal_width, BSmart::Field::goal_width );
    result = PL_put_integer ( goal_depth, BSmart::Field::goal_depth );
    result = PL_put_integer ( goal_height, BSmart::Field::goal_height );
    result = PL_put_integer ( defense_radius, BSmart::Field::defense_radius );
    result = PL_put_integer ( defense_line, BSmart::Field::defense_line );
    result = PL_put_integer ( penalty_mark, BSmart::Field::penalty_mark_distance );
    predicate_t set_field = PL_predicate ( "define_field", 8, "field_definition" );
    PL_call_predicate ( NULL, PL_Q_NORMAL, set_field, field_width );

    /* Game initialization, Preparing variables */

    predicate_t game_init = PL_predicate ( "game_init", 0, "Game initialization" );
    PL_call_predicate ( NULL, PL_Q_NORMAL, game_init, NULL );

    /* ball */

    Ball_Sample ball_model;
    term_t ball_pos_x = PL_new_term_refs ( 9 );
    term_t ball_pos_y = ball_pos_x + 1;
    term_t ball_pos_z = ball_pos_x + 2;
    term_t ball_speed_x = ball_pos_x + 3;
    term_t ball_speed_y = ball_pos_x + 4;
    term_t ball_speed_z = ball_pos_x + 5;
    term_t ball_last_touched_team = ball_pos_x + 6;
    term_t ball_last_touched_id = ball_pos_x + 7;
    term_t ball_status = ball_pos_x + 8;
    predicate_t set_ball_stuff = PL_predicate ( "set_ball_stuff", 9,
                                 "set_ball_variable" );

    /* robots */
    Robot_Sample robot_model;
    bool robot_seen_tmp;
    term_t robot_team = PL_new_term_refs ( 7 );
    term_t robot_id = robot_team + 1;
    term_t robot_pos_x = robot_team + 2;
    term_t robot_pos_y = robot_team + 3;
    term_t robot_speed_x = robot_team + 4;
    term_t robot_speed_y = robot_team + 5;
    term_t robot_seen = robot_team + 6;
    predicate_t set_robot = PL_predicate ( "set_robot", 7, "set_robot_variable" );

    /* timestamp */
    term_t timestamp = PL_new_term_refs ( 1 );
    predicate_t set_timestamp = PL_predicate ( "set_timestamp", 1,
                                "set_timestamp_variable" );

    /* times: Timeout commands */
    predicate_t start_yellow_timeout = PL_predicate ( "start_yellow_timeout", 0,
                                       "start yellow timeout" );
    predicate_t end_yellow_timeout = PL_predicate ( "end_yellow_timeout", 0,
                                     "end yellow timeout" );
    predicate_t start_blue_timeout = PL_predicate ( "start_blue_timeout", 0,
                                     "start blue timeout" );
    predicate_t end_blue_timeout = PL_predicate ( "end_blue_timeout", 0,
                                   "end blue timeout" );

    // half time
    predicate_t end_first_half = PL_predicate ( "end_first_half", 0,
                                 "end first half" );

    /* gamestate */

    term_t global_play_state = PL_new_term_refs ( 1 );
    BSmart::Game_States::Play_State play_state_tmp;
    predicate_t set_global_play_state = PL_predicate ( "set_global_play_state",
                                        1, "set_global_playstate_variable" );
    predicate_t set_local_play_state_from_outside = PL_predicate (
                "set_local_play_state_from_outside", 1,
                "set_local_playstate_variable" );
    char refbox_cmd;

    // Start of game, jumping to state running
    predicate_t game_control = PL_predicate ( "game_control", 0,
                               "central gamecontrol" ); //"zentrale Spielsteuerung");

    // calling rules
    predicate_t check_rules = PL_predicate ( "check_rules", 1, "rule check" ); //"Regelüberprüfung");

    // rule_zero: Only check rules, if internal and external state equal
    predicate_t rule_zero = PL_predicate ( "rule_zero", 0, "game states equal" ); //"Spielzustaende gleich");

    // test_info
    predicate_t get_local_play_state = PL_predicate ( "get_local_play_state", 1,
                                       "get_local_play_state" );
    predicate_t get_local_next_play_state = PL_predicate (
                                                "get_local_next_play_state", 1, "get_local_next_play_state" );

    predicate_t get_left = PL_predicate ( "get_left", 1, "get_left_team" );
    predicate_t get_freekick_pos = PL_predicate ( "get_freekick_pos", 3,
                                   "get_freekick_pos" );
    predicate_t get_rule_breaker = PL_predicate ( "get_rule_breaker", 2,
                                   "Robot which breaks a rule" );

    predicate_t get_standing = PL_predicate ( "get_standing", 2, "Result" ); //"Ergebnis");

    // Drawing of messages for broken rules on GUI
    Broken_Rule broken_rule_gui;
    std::vector<Broken_Rule> broken_rule_vector;

    QMutex rules_mutex;
    for ( ;; ) {

        rules_mutex.lock();
        rules_wait_condition->wait ( &rules_mutex );

        // Timestamp
        cur_timestamp = filter_data->get_timestamp();
        result = PL_put_integer ( timestamp, cur_timestamp );
        PL_call_predicate ( NULL, PL_Q_NORMAL, set_timestamp, timestamp );
        cur_frm = filter_data->get_frame();

        // Playstate
        play_state_tmp = gamestate->get_play_state();
        result = PL_put_integer ( global_play_state, play_state_tmp );
        PL_call_predicate ( NULL, PL_Q_NORMAL, set_global_play_state,
                            global_play_state );
        if ( play_state_tmp != play_state_old ) {
            play_state_old = play_state_tmp;
            PL_call_predicate ( NULL, PL_Q_NORMAL,
                                set_local_play_state_from_outside, global_play_state );
            local_play_state_alt = play_state_tmp;
        }
        refbox_cmd = gamestate->get_refbox_cmd();
        if ( refbox_cmd != refbox_cmd_alt ) {
            switch ( refbox_cmd ) {
                    // timeout yellow start
                case 't':
                    PL_call_predicate ( NULL, PL_Q_NORMAL, start_yellow_timeout, NULL );
                    break;
                    // timeout blue start
                case 'T':
                    PL_call_predicate ( NULL, PL_Q_NORMAL, start_blue_timeout, NULL );
                    break;
                    // timeout yellow end
                case 'z':
                    PL_call_predicate ( NULL, PL_Q_NORMAL, end_yellow_timeout, NULL );
                    break;
                    // timeout blue end
                case 'Z':
                    PL_call_predicate ( NULL, PL_Q_NORMAL, end_blue_timeout, NULL );
                    break;
                case 'h':
                    PL_call_predicate ( NULL, PL_Q_NORMAL, end_first_half, NULL );
            }
            refbox_cmd_alt = refbox_cmd;
        }

        // Ball
        ball_model = filter_data->get_ball_model();
        result = PL_put_integer ( ball_pos_x, ball_model.pos.x );
        result = PL_put_integer ( ball_pos_y, ball_model.pos.y );
        result = PL_put_integer ( ball_pos_z, ball_model.pos.z );
        result = PL_put_integer ( ball_speed_x, ball_model.speed.x );
        result = PL_put_integer ( ball_speed_y, ball_model.speed.y );
        result = PL_put_integer ( ball_speed_z, ball_model.speed.z );
        result = PL_put_integer ( ball_last_touched_team,
                                  ball_model.last_touched_robot.x );
        result = PL_put_integer ( ball_last_touched_id,
                                  ball_model.last_touched_robot.y );
        result = PL_put_integer ( ball_status, ball_model.status );
        PL_call_predicate ( NULL, PL_Q_NORMAL, set_ball_stuff, ball_pos_x );

        // Robots
        for ( int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team ) {
            for ( int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id ) {
                robot_model = filter_data->get_robot_model ( team, id );
                robot_seen_tmp = filter_data->get_robot_seen ( team, id );
                result = PL_put_integer ( robot_team, team );
                result = PL_put_integer ( robot_id, id );
                result = PL_put_integer ( robot_pos_x, robot_model.pos.x );
                result = PL_put_integer ( robot_pos_y, robot_model.pos.y );
                result = PL_put_integer ( robot_speed_x, robot_model.speed.x );
                result = PL_put_integer ( robot_speed_y, robot_model.speed.y );
                result = PL_put_integer ( robot_seen, ( int ) robot_seen_tmp );

                PL_call_predicate ( NULL, PL_Q_NORMAL, set_robot, robot_team );
            }
        }

        // Load and save GUI-update for broken rules
        broken_rule_vector.clear();
        broken_rule_vector = filter_data->get_broken_rules();
        // vektor will not be cleared. All ever broken rules are contained in the vektor
//        for ( std::vector<Broken_Rule>::iterator brit =
//                    broken_rule_vector.begin(); brit != broken_rule_vector.end(); ) {
//            if ( ( cur_timestamp - brit->when_broken ) > 5000 ) {
//                std::ostringstream o;
//                o << cur_timestamp << " - " << brit->when_broken
//                << " rule " << brit->rule_number << " removed";
//                LOG4CXX_DEBUG ( logger, o.str() );
//                broken_rule_vector.erase ( brit );
//            } else {
//                brit++;
//            }
//        }
        broken_rule_gui.rule_number = -42;

        // Only check rules, if internal and external state is equal
        if ( PL_call_predicate ( NULL, PL_Q_NORMAL, rule_zero, NULL ) ) {
            PL_call_predicate ( NULL, PL_Q_NORMAL, game_control, NULL );

            term_t broken_rule = PL_new_term_refs ( 1 );

            if ( PL_call_predicate ( NULL, PL_Q_NORMAL, check_rules, broken_rule ) ) {
                int rule = -42;
                PL_get_integer ( broken_rule, &rule );

                int team = -42;
                int id = -42;
                term_t rule_breaker_team = PL_new_term_refs ( 2 );
                term_t rule_breaker_id = rule_breaker_team + 1;
                PL_call_predicate ( NULL, PL_Q_NORMAL, get_rule_breaker,
                                    rule_breaker_team );
                PL_get_integer ( rule_breaker_team, &team );
                PL_get_integer ( rule_breaker_id, &id );

                BSmart::Int_Vector rule_breaker ( team, id );

                if ( rule != last_break || ( cur_frm - last_msg ) > 100 ) {
                    int local_play_state_test = -42;
                    term_t local_play_state_test_term = PL_new_term_refs ( 1 );
                    PL_call_predicate ( NULL, PL_Q_NORMAL, get_local_play_state,
                                        local_play_state_test_term );
                    PL_get_integer ( local_play_state_test_term,
                                     &local_play_state_test );
                    int left = -42;
                    term_t left_team = PL_new_term_refs ( 1 );
                    PL_call_predicate ( NULL, PL_Q_NORMAL, get_left, left_team );
                    PL_get_integer ( left_team, &left );

                    last_break = rule;
                    last_msg = cur_frm;
                    std::ostringstream o;
                    o << cur_timestamp << " " << cur_frm << " Rule " << rule << " broken by "
                    << team << " | " << id;
                    LOG4CXX_DEBUG ( logger, o.str() );
                }

                // new broken rule
                if ( rule != -42 ) {
                    int x_tmp = -42;
                    int y_tmp = -42;
                    int z_tmp = -42;
                    int left = -42;
                    int local_play_state_test_gui = -42;
                    int standing_yellow = -42;
                    int standing_blue = -42;
                    term_t freekick_pos_x = PL_new_term_refs ( 3 );
                    term_t freekick_pos_y = freekick_pos_x + 1;
                    term_t freekick_pos_z = freekick_pos_x + 2;
                    term_t left_team = PL_new_term_refs ( 1 );
                    term_t local_play_state_test_term_gui = PL_new_term_refs ( 1 );
                    term_t t_standing_yellow = PL_new_term_refs ( 2 );
                    term_t t_standing_blue = t_standing_yellow + 1;
                    PL_call_predicate ( NULL, PL_Q_NORMAL, get_freekick_pos,
                                        freekick_pos_x );
                    PL_call_predicate ( NULL, PL_Q_NORMAL, get_left, left_team );
                    PL_call_predicate ( NULL, PL_Q_NORMAL, get_local_play_state,
                                        local_play_state_test_term_gui );
                    PL_call_predicate ( NULL, PL_Q_NORMAL, get_standing,
                                        t_standing_yellow );
                    PL_get_integer ( freekick_pos_x, &x_tmp );
                    PL_get_integer ( freekick_pos_y, &y_tmp );
                    PL_get_integer ( freekick_pos_z, &z_tmp );
                    PL_get_integer ( local_play_state_test_term_gui,
                                     &local_play_state_test_gui );
                    PL_get_integer ( t_standing_yellow, &standing_yellow );
                    PL_get_integer ( t_standing_blue, &standing_blue );
                    BSmart::Int_Vector freekick_pos ( x_tmp, y_tmp );
                    PL_get_integer ( left_team, &left );

                    broken_rule_gui.rule_number = rule;
                    broken_rule_gui.when_broken = cur_timestamp;
                    broken_rule_gui.freekick_pos = BSmart::Int_Vector ( -1, -1 );
                    broken_rule_gui.rule_breaker = BSmart::Int_Vector ( -1, -1 );
                    broken_rule_gui.circle_around_ball = false;
                    broken_rule_gui.defense_area = -1;
                    broken_rule_gui.line_for_smth = BSmart::Line ( -1., -1., -1.,
                                                    -1. );
                    broken_rule_gui.standing = BSmart::Int_Vector ( -1, -1 );
                    broken_rule_gui.frame_broken = cur_frm;

                    switch ( rule ) {
                        case 1:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            break;
                        case 3:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            break;
                        case 13:
                            break;
                        case 14:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.circle_around_ball = true;
                            break;
                        case 15:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.circle_around_ball = true;
                            broken_rule_gui.defense_area
                            = ( left == rule_breaker.x ) ? 1 : 0;
                            break;
                        case 16:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.circle_around_ball = true;
                            break;
                        case 17:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.circle_around_ball = true;
                            // left
                            if ( ( local_play_state_test_gui < 9 && left == 1 )
                                    || ( local_play_state_test_gui > 8 && left == 0 ) ) {
                                broken_rule_gui.line_for_smth = BSmart::Line (
                                                                    -2170., -2015., -2170., 2015. );
                            }
                            // right
                            else {
                                broken_rule_gui.line_for_smth = BSmart::Line ( 2170.,
                                                                -2015., 2170., 2015. );
                            }
                            break;
                        case 18:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            break;
                        case 19:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.freekick_pos = freekick_pos;
                            break;
                        case 22:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            break;
                        case 23:
                            break;
                        case 24:
                            break;
                        case 27:
                            break;
                        case 28:
                            break;
                        case 29:
                            broken_rule_gui.freekick_pos = freekick_pos;
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.standing = BSmart::Int_Vector (
                                                           standing_yellow, standing_blue );
                            break;
                        case 30:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.freekick_pos = freekick_pos;
                            break;
                        case 42:
                            broken_rule_gui.rule_breaker = rule_breaker;
                            broken_rule_gui.freekick_pos = freekick_pos;
                            broken_rule_gui.line_for_smth = BSmart::Line (
                                                                freekick_pos.x, -2015., freekick_pos.x, 2015. );
                            break;
                        default:
                            ;
                    }
                }

            }
        }

        bool broken_rule_modified = false;
        for ( std::vector<Broken_Rule>::reverse_iterator brit =
                    broken_rule_vector.rbegin(); brit != broken_rule_vector.rend(); ++brit ) {
        	if ((cur_timestamp - brit->when_broken) > 5000) {
				break;
			}
            if ( broken_rule_gui.rule_number == brit->rule_number ) {
//                std::ostringstream o;
//                o << cur_timestamp << " rule " << broken_rule_gui.rule_number <<
//                " overridden";
//                LOG4CXX_DEBUG ( logger, o.str() );
                brit->when_broken = broken_rule_gui.when_broken;
                brit->freekick_pos = broken_rule_gui.freekick_pos;
                brit->rule_breaker = broken_rule_gui.rule_breaker;
                brit->circle_around_ball = broken_rule_gui.circle_around_ball;
                brit->defense_area = broken_rule_gui.defense_area;
                brit->line_for_smth = broken_rule_gui.line_for_smth;
                brit->standing = broken_rule_gui.standing;
                broken_rule_modified = true;
            }
        }
        if ( !broken_rule_modified && broken_rule_gui.rule_number != -42 ) {
            broken_rule_vector.push_back ( broken_rule_gui );
			std::ostringstream o;
			o << cur_timestamp << " rule " << broken_rule_gui.rule_number <<
			" broken" << "Broken rules: " << broken_rule_vector.size();
			LOG4CXX_DEBUG ( logger, o.str() );
			if(broken_rule_gui.rule_number > 0 && broken_rule_gui.rule_number <= 42) {
				emit new_broken_rule(&broken_rule_gui);
			} else {
				LOG4CXX_WARN ( logger, "Invalid rule number" );
			}
        }
        filter_data->set_broken_rules ( broken_rule_vector );

        int local_play_state_test = -42;
        term_t local_play_state_test_term = PL_new_term_refs ( 1 );
        PL_call_predicate ( NULL, PL_Q_NORMAL, get_local_play_state,
                            local_play_state_test_term );
        PL_get_integer ( local_play_state_test_term, &local_play_state_test );

        int local_next_play_state_test = -42;
        term_t local_next_play_state_test_term = PL_new_term_refs ( 1 );
        PL_call_predicate ( NULL, PL_Q_NORMAL, get_local_next_play_state,
                            local_next_play_state_test_term );
        PL_get_integer ( local_next_play_state_test_term,
                         &local_next_play_state_test );

        internal_play_states.x = local_play_state_test;
        internal_play_states.y = local_next_play_state_test;
        filter_data->set_internal_play_states ( internal_play_states );

        if ( local_play_state_test != local_play_state_alt ) {
            local_play_state_alt = local_play_state_test;
        }

        emit
        new_filter_data();
        rules_mutex.unlock();
    }
}
