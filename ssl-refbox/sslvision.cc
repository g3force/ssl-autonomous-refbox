/**
 * @file sslvision.cc
 * @brief SSLVision source file
 */
#include "sslvision.h"
#include <limits>
#include <fstream>
//#include <google/protobuf/io/zero_copy_stream.h>
#include <QFileDialog>
#include <QDateTime>
#include <libbsmart/field.h>
#include <string>
#include "../ConfigFile/ConfigFile.h"
#include "global.h"

// log4cxx
using namespace log4cxx;
using namespace std;
LoggerPtr SSLVision::logger(Logger::getLogger("SSLVision"));

/**
 * @brief Initialize SSLVision object with default values
 * Set global vars (read config file if necessary), open socket for ssl-vision
 * @param data_
 * @param gamestate_
 * @param new_data_wait_condition_
 */
SSLVision::SSLVision(Pre_Filter_Data* data_, BSmart::Game_States* gamestate_, QWaitCondition* new_data_wait_condition_) :
		data(data_), gamestate(gamestate_) {
	LOG4CXX_DEBUG( logger, "create SSLVisison object");

	std::ostringstream o;

	new_data_wait_condition = new_data_wait_condition_;
	//robot radius in pixel
	robot_r = 20;
	string message = "";

	cam_height = Global::config.read<int>("cam_height", 580);
	cam_width = Global::config.read<int>("cam_width", 780);
	o.str("");
	o << "cam_height=" << cam_height << " cam_width=" << cam_width;
	LOG4CXX_INFO( logger, o.str());

	socket = 0;
	buffer = new char[MaxDataGramSize];

	string ssl_vision_ip = Global::config.read<string>("ssl_vision_ip", "224.5.23.2");
	uint16_t ssl_vision_port = Global::config.read<uint16_t>("ssl_vision_port", 40101);

	o.str("");
	if (!(o << ssl_vision_port))
		LOG4CXX_ERROR( logger, "Could not convert port to string");
	message = "IP: ";
	message += ssl_vision_ip;
	message += " port: ";
	message += o.str();
	LOG4CXX_INFO( logger, message);

	try {
		socket = new BSmart::Multicast_Socket();
		socket->bind(ssl_vision_ip.c_str(), ssl_vision_port);
		socket->set_non_blocking();
	} catch (BSmart::IO_Exception e) {
		string message = "Receive_PM init ";
		message += e.what();
		LOG4CXX_DEBUG( logger, message);
	}
	fileName = QDir::homePath();

	rec = false;
	play = false;
	log_control = new Log_Control();
	reset_transformed_percept(transformed_percept);
	standard_sleep_time = 25;
	queue_filled = false;
	tf_percept_queue_all.clear();
	for (int i = 0; i < 2; i++)
		tf_percept_queue_one_ball[i].clear();
	//    current_frame = 0;
	LOG4CXX_DEBUG( logger, "End create SSLVision");
}

SSLVision::~SSLVision() {
}

/**
 * @brief Start SSLVision in an endless loop
 * <ol>
 * <li>Get new transformed_percept</li>
 * <li>Analyze and prepare transformed_percept</li>
 * <li>Apply partical filter, if queue is filled</li>
 * </ol>
 */
void SSLVision::run() {
	LOG4CXX_DEBUG( logger, "run()");

	if (Global::logFile != NULL) {
		play_record(Global::logFile);
	}

	while (1) {
		reset_transformed_percept(transformed_percept);
		// see if new frame has been received
		int exec = execute(transformed_percept);
		//transformed_percept.sleep_time = time;

		// fill queue
		switch (exec) {
		case -2: // camera id > 1
			LOG4CXX_DEBUG( logger, "Very strange");
			break;
		case -1: //no frame received
			break;
		default: // frame received
			// check done for heuristical reasons
			analyse_percepts();
			tf_percept_queue_all.push_back(transformed_percept);
		}

		// queue is filled when greater than 30 and will be set not filled, if below 10 again
		if (queue_filled) {
			if (tf_percept_queue_all.size() < 10) {
				queue_filled = false;
				//LOG4CXX_DEBUG(logger, "queue empty");
			}
		} else if (tf_percept_queue_all.size() > 30) {
			queue_filled = true;
			//LOG4CXX_DEBUG(logger, "queue filled");
		}

		// process percept through Particle Filter and other system
		if (queue_filled) {
			// take current frame out of buffer and delete it from buffer
			transformed_percept = tf_percept_queue_all.front();
			tf_percept_queue_all.erase(tf_percept_queue_all.begin());

			// if there are more with only one ball
			if (tf_percept_queue_one_ball[transformed_percept.cam_id].size() > 0) {
				// if the first is the same as the current
				if (transformed_percept.current_frame
						== tf_percept_queue_one_ball[transformed_percept.cam_id][0].current_frame) {
					// delete first and go on
					tf_percept_queue_one_ball[transformed_percept.cam_id].erase(
							tf_percept_queue_one_ball[transformed_percept.cam_id].begin());
					// if there is still a frame
					if (tf_percept_queue_one_ball[transformed_percept.cam_id].size() > 0) {
						int index = 0;
						if (tf_percept_queue_one_ball[transformed_percept.cam_id].size() > 1)
							index = 1;
						// prepare collision heuristic
						transformed_percept.ball_direction_after =
								(tf_percept_queue_one_ball[transformed_percept.cam_id][index].balls[0]
										- transformed_percept.balls[0]);
						int timediff = tf_percept_queue_one_ball[transformed_percept.cam_id][index].frame_received
								- transformed_percept.frame_received;
						if (timediff == 0) {
							transformed_percept.ball_direction_after = BSmart::Pose(0., 0., 0.);
						} else {
							transformed_percept.ball_direction_after /= timediff;
						}
					}
				}
			}
			// if only one robot percept is found
			for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
				for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
					if (tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].size() > 0) {
						// if first is the same as current
						if (transformed_percept.current_frame
								== tf_percept_queue_one_robot[transformed_percept.cam_id][team][id][0].current_frame) {
							// delete first and go on
							tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].erase(
									tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].begin());
							if (tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].size() > 0) {
								int index = 0;
								if (tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].size() > 1)
									index = 1;

								int timediff =
										(tf_percept_queue_one_robot[transformed_percept.cam_id][team][id][index].frame_received
												- transformed_percept.frame_received);

								if (timediff == 0) {
									; //robot_direction_before is used.
								} else {
									transformed_percept.robot_direction[team][id] =
											(tf_percept_queue_one_robot[transformed_percept.cam_id][team][id][index].robots[team][id][0]
													- transformed_percept.robots[team][id][0]);
									transformed_percept.robot_direction[team][id] /= timediff;
								}
							}
						}
					}
				}
			}

			//write data from transformed_percept into pf_data
			reset_data(transformed_percept.cam_id);

			//set frame number
			data->set_newest_frame(transformed_percept.current_frame);
			data->set_timestamp(transformed_percept.frame_received);

			if (transformed_percept.balls.size() == 0) {
				data->set_ball_framenumber(transformed_percept.cam_id, transformed_percept.ball_frame_number);
				data->set_ball_timestamp(transformed_percept.cam_id, transformed_percept.frame_received);
			} else {
				data->clear_balls(transformed_percept.cam_id);
				data->set_balls(transformed_percept.cam_id, transformed_percept.balls);
			}

			if (transformed_percept.has_one_ball) {
				data->set_ball_direction_before(transformed_percept.ball_direction_before);
				data->set_ball_direction_after(transformed_percept.ball_direction_after);
			}

			for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
				for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
					data->set_robots(transformed_percept.cam_id, team, id, transformed_percept.robots[team][id]);
					if (transformed_percept.has_one_robot[team][id]) {
						data->set_robot_direction(team, id, transformed_percept.robot_direction[team][id]);
					}
				}
			}

			if (play) {
				emit new_refbox_cmd(transformed_percept.refbox_cmd[0]);
				emit update_frame(transformed_percept.current_frame);
			}

			new_data_wait_condition->wakeAll();
			emit
			new_frame();

			msleep(abs(transformed_percept.sleep_time));
		} else {
			msleep(standard_sleep_time);
		}
	}
}

/**
 * @brief Receive and process a frame, either from outside or from log file
 * Data is collected an stored in trans_perc
 * @param trans_perc Transformed_Percept to store received data
 * @return error code
 */
int SSLVision::execute(Transformed_Percept& trans_perc) {
	Log_Frame* log_frame;
	SSL_DetectionFrame* det_frame;
	bool has_new_frame = false;
	int time_diff = standard_sleep_time;

	// while a frame is received
	while (!play && recv(frame)) {
		//test camera_id
		if (frame.camera_id() > 1) {
			trans_perc.sleep_time = standard_sleep_time;
			std::ostringstream o;
			o << "Got Percept from CAM: ";
			o << frame.camera_id();
			LOG4CXX_DEBUG( logger, o.str());
			return -2;
		}
		npc = 0;

		// process frame
		trans_perc.frame_received = frame.t_capture();
		process_balls(trans_perc);
		process(trans_perc, 1); // blue
		process(trans_perc, 0); // yellow

		if (rec) {
			logs.add_log();
			log_frame = logs.mutable_log(logs.log_size() - 1);
			char refbox_cmd = gamestate->get_refbox_cmd();
			log_frame->set_refbox_cmd(&refbox_cmd);

			det_frame = log_frame->mutable_frame();
			*det_frame = frame;
		}
		has_new_frame = true;
	}

	if (!play && log_control->get_current_frame() != 0)
		end_play_record();

	// Play logfile
	if (play) {
		if (log_control->get_next_frame() < 0)
			end_play_record();
		else {

			log_frame = logs.mutable_log(log_control->get_current_frame());
			det_frame = log_frame->mutable_frame();
			frame = *det_frame;

			// read refbox_cmd from logfile into gamestate
			std::string tmp = log_frame->refbox_cmd();
			trans_perc.refbox_cmd = tmp;

			// process frame
			trans_perc.frame_received = BSmart::Systemcall::get_current_system_time();
			process_balls(trans_perc);
			process(trans_perc, 1); // blue
			process(trans_perc, 0); // yellow
		}

		trans_perc.current_frame = log_control->get_current_frame();

		// Find out how long to sleep
		if (!(log_control->get_prop_next_frame() < 0)) {
			double old_time = frame.t_capture();
			double new_time = logs.log(log_control->get_prop_next_frame()).frame().t_capture();
			double timediff = new_time - old_time;
			has_new_frame = true;
			if (log_control->get_play_speed() != 0) {
				time_diff = abs(((timediff * 1000) / log_control->get_play_speed()));
			} else {
				time_diff = standard_sleep_time;
			}
		}
	}

	// distance between frames in ms
	if (has_new_frame) {
		trans_perc.sleep_time = time_diff;
		//LOG4CXX_DEBUG ( logger, "return execute() with 1" );
		return 1;
	}
	trans_perc.sleep_time = standard_sleep_time;
	return -1;
}

/**
 * @brief Receive a frame from multicast socket
 * @param frame resulting SSL_DetectionFrame
 * @return length of received frame
 */
int SSLVision::recv(SSL_DetectionFrame& frame) {
	int len = 0;
	try {
		len = socket->read(buffer, MaxDataGramSize);
	} catch (BSmart::Multicast_Socket::No_Data_Available nd) {
		if (++npc >= 10) {
			// Happens quite frequently ;)
			//LOG4CXX_WARN(logger, "No data!");
		}
	} catch (BSmart::IO_Exception e) {
		std::ostringstream o;
		o << "IOException in recv(): " << e.what();
		LOG4CXX_ERROR( logger, o.str());
		return 0;
	}

	SSL_WrapperPacket packet;
	len = packet.ParseFromArray(buffer, len);

	// read camera position out of geometry data
	if (packet.has_geometry()) {
		//LOG4CXX_DEBUG(logger, "geometry");
		SSL_GeometryData geo_data = packet.geometry();
		int n_cams = geo_data.calib_size();
		for (int i = 0; i < n_cams; ++i) {
			SSL_GeometryCameraCalibration cam = geo_data.calib(i);
			int camID = cam.camera_id();
			BSmart::Pose3D cam_pos(cam.derived_camera_world_tx(), cam.derived_camera_world_ty(),
					cam.derived_camera_world_tz());
			data->set_camera_pos(camID, cam_pos);
		}
	}
	if (packet.has_detection()) {
		frame = packet.detection();
//		std::ostringstream o;
//		o << "return recv() with ";
//		o << len;
		//LOG4CXX_DEBUG ( logger, o.str() );
		return len;
	} else {
		return 0;
	}
}

/**
 * @brief Process ball(s) by assigning trans_perc values to the Ball Percept
 * @param trans_perc
 */
void SSLVision::process_balls(Transformed_Percept& trans_perc) {
	int n_balls = frame.balls_size();
	Ball_Percept pBall;

	trans_perc.cam_id = frame.camera_id();

	if (n_balls == 0) {
		trans_perc.ball_frame_number = frame.frame_number();
	} else {
		for (int i = 0; i < n_balls; ++i) {
			SSL_DetectionBall ball = frame.balls(i);
			pBall.x = ball.x();
			pBall.y = ball.y();
			pBall.confidence = ball.confidence() * 100;
			pBall.framenumber = frame.frame_number();
			pBall.cam = frame.camera_id();
			pBall.timestamp = frame.t_capture() * 1000;

			if (!(pBall.x == 0. && pBall.y == 0.))
				trans_perc.balls.push_back(pBall);
		}
	}
}

/**
 * @brief Process robots of the team with the given color.
 * Create a robot percept and assign to it the values from the trans_perc.<br />
 * @param trans_perc Transformed_Percept
 * @param color 1 -> blue, 0 -> yellow
 */
void SSLVision::process(Transformed_Percept& trans_perc, char color) {
	int n_bots = color == 1 ? frame.robots_blue_size() : frame.robots_yellow_size();

	for (int i = 0; i < n_bots; ++i) {
		SSL_DetectionRobot robot = color == 1 ? frame.robots_blue(i) : frame.robots_yellow(i);

		/* following code doesn't work anymore...
		 * It seems, that it expects the origin of coordinates not in the
		 * middle of the field and thus are not negative...
		 * Anyway, without this code, everything runs fine :)
		 */
//		if ( (robot.pixel_y() + robot_r > cam_height)
//		|| (robot.pixel_y() - robot_r < 0)
//		|| (robot.pixel_x() + robot_r > cam_width)
//		|| (robot.pixel_x() - robot_r < 0) ) {
//		continue;
//		}
		Robot_Percept pRobot;
		pRobot.x = robot.x();
		pRobot.y = robot.y();
		pRobot.id = robot.robot_id();
		pRobot.color = color == 1 ? SSLRefbox::Colors::BLUE : SSLRefbox::Colors::YELLOW;
		pRobot.rotation_known = robot.has_orientation();
		if (pRobot.rotation_known) {
			pRobot.rotation = robot.orientation();
			//LOG4CXX_DEBUG(logger, pRobot.rotation);
		}

		pRobot.confidence = robot.confidence() * 100;
		pRobot.framenumber = frame.frame_number();
		pRobot.cam = frame.camera_id();
		pRobot.timestamp = frame.t_capture() * 1000;

		trans_perc.robots[(int) color][pRobot.id].push_back(pRobot);
	}
}

/**
 * @brief Reset data (balls and robots)
 * @param cam Camera id
 */
void SSLVision::reset_data(int cam) {
	//LOG4CXX_DEBUG ( logger, "reset_data" );
	//Ball_Percept pBall;
	//clear ball vector
	data->clear_balls(cam);
	//clear robots
	data->clear_robots(cam);
}

/**
 * @brief Reset everything in tranformed_percept.
 * @param trans_perc
 */
void SSLVision::reset_transformed_percept(Transformed_Percept& trans_perc) {
	trans_perc.cam_id = 0;
	trans_perc.ball_frame_number = 0;
	trans_perc.balls.clear();
	trans_perc.has_one_ball = false;
	trans_perc.ball_direction_before.x = 0.;
	trans_perc.ball_direction_before.y = 0.;
	trans_perc.ball_direction_after.x = 0.;
	trans_perc.ball_direction_after.y = 0.;

	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			trans_perc.robots[team][id].clear();
			trans_perc.has_one_robot[team][id] = false;
			trans_perc.robot_direction[team][id].x = 0.;
			trans_perc.robot_direction[team][id].y = 0.;
		}
	}

	trans_perc.refbox_cmd = "";
	trans_perc.current_frame = 0;
	trans_perc.sleep_time = 0;
	trans_perc.frame_received = 0;
}

/**
 * @brief Analyze transformed_percept and set some values
 * Robots and ball will be analyzed.
 */
void SSLVision::analyse_percepts() {

	// if only one ball in percept found
	if (transformed_percept.balls.size() == 1) {
		transformed_percept.has_one_ball = true;

		int size_one_ball = tf_percept_queue_one_ball[transformed_percept.cam_id].size();

		if (size_one_ball == 1) {
			transformed_percept.ball_direction_before = (transformed_percept.balls[0]
					- tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - 1].balls[0]);
			int timediff = (transformed_percept.frame_received
					- tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - 1].frame_received);

			if (timediff == 0) {
				transformed_percept.ball_direction_before =
						tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - 1].ball_direction_before;
			} else {
				transformed_percept.ball_direction_before /= timediff;
			}
		} else if (size_one_ball > 1) {
			int diff = 2;
			if (size_one_ball > 2)
				diff = 3;
			transformed_percept.ball_direction_before =
					(tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - 1].balls[0]
							- tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - diff].balls[0]);
			int timediff = (tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - 1].frame_received
					- tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - diff].frame_received);

			if (timediff == 0) {
				transformed_percept.ball_direction_before =
						tf_percept_queue_one_ball[transformed_percept.cam_id][size_one_ball - diff].ball_direction_before;
			} else {
				transformed_percept.ball_direction_before /= timediff;
			}

		}
		tf_percept_queue_one_ball[transformed_percept.cam_id].push_back(transformed_percept);
	}

	//if only one robot percept per robot is found
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			if (transformed_percept.robots[team][id].size() == 1) {
				transformed_percept.has_one_robot[team][id] = true;

				int size_one_robot = tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].size();
				if (size_one_robot > 0) {
					transformed_percept.robot_direction[team][id] =
							(transformed_percept.robots[team][id][0]
									- tf_percept_queue_one_robot[transformed_percept.cam_id][team][id][size_one_robot
											- 1].robots[team][id][0]);
					int timediff =
							(transformed_percept.frame_received
									- tf_percept_queue_one_robot[transformed_percept.cam_id][team][id][size_one_robot
											- 1].frame_received);

					if (timediff == 0) {
						transformed_percept.robot_direction[team][id] =
								tf_percept_queue_one_robot[transformed_percept.cam_id][team][id][size_one_robot - 1].robot_direction[team][id];
					} else {
						transformed_percept.robot_direction[team][id] /= timediff;
					}
				}
				tf_percept_queue_one_robot[transformed_percept.cam_id][team][id].push_back(transformed_percept);
			}
		}
	}
}

/**
 * @brief Start or stop record depending on `rec` and `play` flags
 */
void SSLVision::record() {
	if (!rec) {
		if (!play) {
			rec = true;
			start_record();
		}
	} else {
		rec = false;
		end_record();
	}
}

/**
 * @brief Prepare for starting record (actually does not start it)
 * clear log and change button
 */
void SSLVision::start_record() {
	logs.clear_log();
	emit
	change_record_button("  End Record  ");
	LOG4CXX_DEBUG( logger, "Start Recording");
}

/**
 * @brief Finish a record by asking for the saving location and write log to file
 * <ol>
 * <li>prepare fileName</li>
 * <li>save</li>
 * <li>change button in GUI</li>
 * </ol>
 * @return error code
 */
int SSLVision::end_record() {
	//change fileName into directory
	if (fileName != QDir::homePath()) {
		int last_slash = 0;
		for (int i = 0; i < fileName.length(); i++) {
			if (fileName.at(i).toAscii() == (QChar('/')))
				last_slash = i;
		}
		fileName.remove(last_slash, fileName.length() - last_slash);
	}

	fileName = QFileDialog::getSaveFileName((QWidget*) this->parent(), tr("Save Logfile"), fileName,
			tr("Log Files (*.log)"));

	if (fileName.isEmpty()) {
		fileName = QDir::homePath();
		fileName.append('/');
		fileName.append(QDateTime::currentDateTime().toString(Qt::ISODate));
	}

	if (!fileName.endsWith(".log"))
		fileName.append(".log");

	std::fstream output(fileName.toAscii().constData(), std::ios::out | std::ios::trunc | std::ios::binary);
	emit
	change_record_button("Record Logfile");

	if (!logs.SerializeToOstream(&output)) {
		LOG4CXX_WARN( logger, "Failed to write logfile.");
		return -1;
	}

	output.close();
	std::ostringstream o;
	o << logs.log_size() << " Frames recorded, written to " << fileName.toAscii().constData();
	LOG4CXX_DEBUG( logger, o.str());
	return 0;
}

/**
 * @brief Play or stop play record according to global `play` variable
 */
void SSLVision::play_record(QString logFile) {
	if (!play) {
		if (!rec) {
			if (!start_play_record(logFile)) {
				play = true;
			}
		}
	} else {
		end_play_record();
	}
}

/**
 * @brief Start play of a log file record
 * @return error code
 */
int SSLVision::start_play_record(QString logFile) {
	LOG4CXX_INFO( logger, "Start Play Record");
	std::ostringstream o;

	if (logFile.isEmpty()) {
		//change fileName into directory
		if (fileName != QDir::homePath()) {
			int last_slash = 0;
			for (int i = 0; i < fileName.length(); i++) {
				if (fileName.at(i).toAscii() == (QChar('/')))
					last_slash = i;
			}
			fileName.remove(last_slash, fileName.length() - last_slash);
		}

		// What data shall I read?
		fileName = QFileDialog::getOpenFileName((QWidget*) this->parent(), tr("Open Logfile"), fileName,
				tr("Log Files (*.log)"));
	} else {
		fileName = logFile;
	}
	o << "fileName: " << fileName.toAscii().constData();
	LOG4CXX_DEBUG( logger, o.str());

	// Read the existing log.
	std::fstream input(fileName.toAscii().constData(), std::ios::in | std::ios::binary);
	if (!input) {
		o.str("");
		o << fileName.toAscii().constData() << ": File not found.";
		LOG4CXX_ERROR( logger, o.str());
		return -1;
	}
	// critical line
	else if (!logs.ParseFromIstream(&input)) {
		LOG4CXX_ERROR( logger, "Failed to parse Logfile.");
		//workaround for the issue of loading too big files - not a definitive solution !
		//	return -1;
	}

	LOG4CXX_INFO( logger, "File successfully loaded");
	//workaround for the issue of loading too big files - not a definitive solution !
	//if (logs.IsInitialized() && logs.log_size() > 0
	//		&& logs.log(0).IsInitialized()) {
	if (logs.log_size() > 0) {
		o.str("");
		o << "Size of logfile: " << logs.log_size();
		LOG4CXX_INFO( logger, o.str());

		o.str("");
		o << "Start Command: " << logs.mutable_log(0)->refbox_cmd();
		LOG4CXX_INFO( logger, o.str());

		log_control->reset(logs.log_size());
		emit
		log_size(logs.log_size());
		//initializeSlider(int min, int max, int singleStep, int pageStep, int tickInterval)
		emit
		initializeSlider(0, logs.log_size(), 1, 100, 1800);
		emit showLogControl(true);
	} else {
		LOG4CXX_DEBUG( logger, "Logfile seems to be empty or damaged");
		return -1;
	}
	input.close();
	emit
	change_play_button(" End Play  ");
	return 0;
}

/**
 * @brief Stop play of a log file record
 */
void SSLVision::end_play_record() {
	LOG4CXX_INFO( logger, "Start end_play_record");
	play = false;
	log_control->reset(0);
	//    current_frame = 0;
	reset_data(0);
	reset_data(1);
	logs.clear_log();
	emit
	showLogControl(false);
	emit
	change_play_button("Play Record");
	LOG4CXX_DEBUG( logger, "End end_play_record");
}
