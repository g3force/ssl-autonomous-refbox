#ifndef FIELD_H
#define FIELD_H

#include "pose.h"

namespace BSmart
{
namespace Field
{
/**
 * Class used to define the points of the field. There are certain parameters
 * that have to be set - see "//needed". Other points depend on those but describe
 * only relative coordinates that will be calculated from the needed ones - see
 * "//calculated".
 *
 * Every component of the B-Smart software should use this class as it grants
 * the possibility to change parameters fast and central for all applications.
 *
 * BEWARE: If values get odd we have to use double instead of int
 */

//line thickness
const double line_thickness = 10;
//goal material
const int goal_material = 20;
//needed
const double ball_radius = 21.5;
const double robot_radius = 90.0;
const double robot_height = 150.0;
//mathematically not correct, but proved practicable
const double distance_robot2ball = 110;
const int center_radius = 500;

const int defense_radius = 500;
/** Length of line between the two pies of defense area */
const int defense_line = 350;

/** Distance the robots have to keep to the ball when gamestate is Stop */
const int stop_distance = 500;

const int field_width = 6040;
const int field_height = 4030;
const int goal_depth = 180;
const int goal_width = 700;
const int goal_height = 155;

/** width of the green area beyond the field lines */
const int off_width = 250;
/** Distance of penalty Mark to outer Field-Line */
const int penalty_mark_distance = 450;
/** Distance of Free-Kick Mark to outer vertical Field-Line */
const int free_kick_x_dist = 600;
/** Distance of Free-Kick Mark to outer horizontal Field-Line */
const int free_kick_y_dist = 100;

//calculated

//auxiliary calculated parameters to increase code readability

/** width of whole green area including off-area */
const int width = field_width + 2 * off_width;
/** height of whole green area including off-area */
const int height = field_height + 2 * off_width;
const int half_width = width / 2;
const int half_height = height / 2;
const int half_field_width = field_width / 2;
const int half_field_height = field_height / 2;
const int half_goal_width = goal_width / 2;

/** Height of the oval defense area measured on the goal-line */
const int defense_height = 2 * defense_radius + defense_line;
const int half_defense_height = defense_height / 2;
const int half_defense_line_length = half_defense_height - defense_radius;

//Field corners
/** upper corner of field lines on opponents side */
const Pose upper_opponent_field_corner
    = Pose(half_field_width, half_field_height);
/**lower corner of field lines on opponents side */
const Pose lower_opponent_field_corner
    = Pose(half_field_width, -half_field_height);
/** upper corner of field lines on own side */
const Pose upper_own_field_corner
    = Pose(-half_field_width, half_field_height);
/** lower corner of field lines on own side */
const Pose lower_own_field_corner
    = Pose(-half_field_width, -half_field_height);

//Outer field corners - similar to field corners, but with off-size added
const Pose upper_opponent_outer_field_corner
    = Pose(half_width, half_height);
const Pose lower_opponent_outer_field_corner
    = Pose(half_width, -half_height);
const Pose upper_own_outer_field_corner
    = Pose(-half_width, half_height);
const Pose lower_own_outer_field_corner
    = Pose(-half_width, -half_height);

//Kickoff Line
/** point on the middle of the upper field line */
const Pose upper_middle_mark
    = Pose(0, half_field_height);
/** point on the middle of the lower field line */
const Pose lower_middle_mark
    = Pose(0, -half_field_height);

//Kickoff Circle
const Pose center_mark
    = Pose(0, 0);
/** upper point where circle and middle-line cross */
const Pose upper_center_circle_cross
    = Pose(0, center_radius);
/** lower point where circle and middle-line cross */
const Pose lower_center_circle_cross
    = Pose(0, -center_radius);
/** point of middle circle at 0° */
const Pose own_center_circle_quad
    = Pose(-center_radius, 0);
/** point of middle circle at 180° */
const Pose opponent_center_circle_quad
    = Pose(center_radius, 0);

//Penalty
const Pose opponent_penalty_mark
    = Pose(half_field_width - penalty_mark_distance, 0);
const Pose own_penalty_mark
    = Pose(-half_field_width + penalty_mark_distance, 0);

//Goals
/** upper left corner of opponents goal */
const Pose upper_opponent_goal_corner
    = Pose(half_field_width, half_goal_width);
/** lower left corner of opponents goal */
const Pose lower_opponent_goal_corner
    = Pose(half_field_width, -half_goal_width);
/** lower right corner of opponents goal */
const Pose lower_opponent_goal_corner_back
    = Pose(half_field_width + goal_depth, -half_goal_width);
/** upper right corner of opponents goal */
const Pose upper_opponent_goal_corner_back
    = Pose(half_field_width + goal_depth, half_goal_width);
/** upper right corner of own goal */
const Pose upper_own_goal_corner
    = Pose(-half_field_width, half_goal_width);
/** lower right corner of own goal */
const Pose lower_own_goal_corner
    = Pose(-half_field_width, -half_goal_width);
/** lower left corner of own goal */
const Pose lower_own_goal_corner_back
    = Pose(-half_field_width - goal_depth, -half_goal_width);
/** upper left corner of own goal */
const Pose upper_own_goal_corner_back
    = Pose(-half_field_width - goal_depth, half_goal_width);

//Defense
/** upper point where defense circle and field line cross */
const Pose upper_opponent_defense_corner
    = Pose(half_field_width, half_defense_height);
/** upper point where defense circle and field line cross */
const Pose lower_opponent_defense_corner
    = Pose(half_field_width, -half_defense_height);
/** upper point where defense circle and field line cross */
const Pose upper_own_defense_corner
    = Pose(-half_field_width, half_defense_height);
/** upper point where defense circle and field line cross */
const Pose lower_own_defense_corner
    = Pose(-half_field_width, -half_defense_height);
/** center point for upper defense-subcircle on field line*/
const Pose opponent_goal_upper_defense_circle_center
    = Pose(half_field_width, half_defense_line_length);
/** center point for lower defense-subcircle on field line*/
const Pose opponent_goal_lower_defense_circle_center
    = Pose(half_field_width, -half_defense_line_length);
const Pose own_goal_upper_defense_circle_center
    = Pose(-half_field_width, half_defense_line_length);
const Pose own_goal_lower_defense_circle_center
    = Pose(-half_field_width, -half_defense_line_length);
/** middle of the goal on field line */
const Pose opponent_goal_middle_mark
    = Pose(half_field_width, 0);
/** middle of the goal on field line */
const Pose own_goal_middle_mark
    = Pose(-half_field_width, 0);


//Additional marks at the sideline
/** point on the upper field line that halves the opponents field-side */
const Pose upper_opponent_field_half_mark
    = Pose(half_field_width / 2, half_field_height);
/** point on the lower field line that halves the opponents field-side */
const Pose lower_opponent_field_half_mark
    = Pose(half_field_width / 2, -half_field_height);
/** point on the upper field line that halves the own field-side */
const Pose upper_own_field_half_mark
    = Pose(-half_field_width / 2, half_field_height);
/** point on the lower field line that halves the own field-side */
const Pose lower_own_field_half_mark
    = Pose(-half_field_width / 2, -half_field_height);

//Freekick
const Pose upper_opponent_freekick_mark
    = Pose(half_field_width - free_kick_x_dist, half_field_height - free_kick_y_dist);
const Pose lower_opponent_freekick_mark
    = Pose(half_field_width - free_kick_x_dist, -half_field_height + free_kick_y_dist);
const Pose upper_own_freekick_mark
    = Pose(-half_field_width + free_kick_x_dist, half_field_height - free_kick_y_dist);
const Pose lower_own_freekick_mark
    = Pose(-half_field_width + free_kick_x_dist, -half_field_height + free_kick_y_dist);

}//namespace Field
}//namespace
#endif
