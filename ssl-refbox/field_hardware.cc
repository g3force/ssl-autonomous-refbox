#include "field_hardware.h"
#include <libbsmart/field.h>

Field_Hardware::Field_Hardware()
{
    ;
}

const Field_Bar Field_Hardware::field_bars_ball[Field_Hardware::NUMBER_FIELD_BARS] =
{
/**/
    //border left
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_outer_field_corner.x + BSmart::Field::ball_radius , BSmart::Field::lower_own_outer_field_corner.y + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_own_outer_field_corner.x + BSmart::Field::ball_radius , BSmart::Field::upper_own_outer_field_corner.y - BSmart::Field::ball_radius ) , Field_Hardware::BAR_VERTICAL , 80),
    //border above
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_outer_field_corner.x + BSmart::Field::ball_radius , BSmart::Field::upper_own_outer_field_corner.y - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_outer_field_corner.x - BSmart::Field::ball_radius , BSmart::Field::upper_opponent_outer_field_corner.y - BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , 80),
    // border right
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_outer_field_corner.x - BSmart::Field::ball_radius , BSmart::Field::upper_opponent_outer_field_corner.x - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::lower_opponent_outer_field_corner.x - BSmart::Field::ball_radius , BSmart::Field::lower_opponent_outer_field_corner.y + BSmart::Field::ball_radius ) , Field_Hardware::BAR_VERTICAL , 80),
    //border below
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_outer_field_corner.x - BSmart::Field::ball_radius , BSmart::Field::lower_opponent_outer_field_corner.y + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::lower_own_outer_field_corner.x + BSmart::Field::ball_radius , BSmart::Field::lower_own_outer_field_corner.y + BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , 80),
/**/
    //goal left inside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x , BSmart::Field::lower_own_goal_corner.y + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x + BSmart::Field::ball_radius , BSmart::Field::lower_own_goal_corner_back.y + BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x + BSmart::Field::ball_radius , BSmart::Field::lower_own_goal_corner_back.y + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x + BSmart::Field::ball_radius , BSmart::Field::upper_own_goal_corner_back.y - BSmart::Field::ball_radius) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x + BSmart::Field::ball_radius, BSmart::Field::upper_own_goal_corner_back.y - BSmart::Field::ball_radius) , BSmart::Pose(BSmart::Field::upper_own_goal_corner.x , BSmart::Field::upper_own_goal_corner.y - BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal left outside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x , BSmart::Field::lower_own_goal_corner.y - BSmart::Field::goal_material - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::ball_radius , BSmart::Field::lower_own_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::ball_radius , BSmart::Field::lower_own_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::ball_radius , BSmart::Field::upper_own_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::ball_radius ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::ball_radius , BSmart::Field::upper_own_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner.x , BSmart::Field::upper_own_goal_corner.y + BSmart::Field::goal_material + BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal left goalpost front
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x + BSmart::Field::ball_radius , BSmart::Field::lower_own_goal_corner.y ) , BSmart::Pose(BSmart::Field::lower_own_goal_corner.x + BSmart::Field::ball_radius , BSmart::Field::lower_own_goal_corner.y - BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_goal_corner.x + BSmart::Field::ball_radius , BSmart::Field::upper_own_goal_corner.y ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner.x + BSmart::Field::ball_radius , BSmart::Field::upper_own_goal_corner.y + BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    //goal right inside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x , BSmart::Field::lower_opponent_goal_corner.y + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x - BSmart::Field::ball_radius, BSmart::Field::lower_opponent_goal_corner_back.y + BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x - BSmart::Field::ball_radius , BSmart::Field::lower_opponent_goal_corner_back.y + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x - BSmart::Field::ball_radius , BSmart::Field::upper_opponent_goal_corner_back.y - BSmart::Field::ball_radius ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x - BSmart::Field::ball_radius , BSmart::Field::upper_opponent_goal_corner_back.y - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x , BSmart::Field::upper_opponent_goal_corner.y - BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal right outside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x , BSmart::Field::lower_opponent_goal_corner.y - BSmart::Field::goal_material - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::ball_radius, BSmart::Field::lower_opponent_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::ball_radius, BSmart::Field::lower_opponent_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::ball_radius , BSmart::Field::upper_opponent_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::ball_radius ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::ball_radius , BSmart::Field::upper_opponent_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::ball_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x , BSmart::Field::upper_opponent_goal_corner.y + BSmart::Field::goal_material + BSmart::Field::ball_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal right goalpost front
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x - BSmart::Field::ball_radius , BSmart::Field::lower_opponent_goal_corner.y ) , BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x - BSmart::Field::ball_radius , BSmart::Field::lower_opponent_goal_corner.y - BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x - BSmart::Field::ball_radius , BSmart::Field::upper_opponent_goal_corner.y ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x - BSmart::Field::ball_radius , BSmart::Field::upper_opponent_goal_corner.y + BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height )
};

const Field_Goalpost Field_Hardware::field_goalposts_ball[Field_Hardware::NUMBER_FIELD_GOALPOSTS] =
{
    //left below
    Field_Goalpost( BSmart::Field::lower_own_goal_corner , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x , BSmart::Field::lower_own_goal_corner.y - BSmart::Field::goal_material ) , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    //left above
    Field_Goalpost( BSmart::Field::upper_own_goal_corner , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::upper_own_goal_corner.x , BSmart::Field::upper_own_goal_corner.y + BSmart::Field::goal_material ) , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    //right below
    Field_Goalpost( BSmart::Field::lower_opponent_goal_corner , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x , BSmart::Field::lower_opponent_goal_corner.y - BSmart::Field::goal_material ) , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    //right above
    Field_Goalpost( BSmart::Field::upper_opponent_goal_corner , BSmart::Field::ball_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x , BSmart::Field::upper_opponent_goal_corner.y + BSmart::Field::goal_material ) , BSmart::Field::ball_radius , BSmart::Field::goal_height )
    //middle circle for testing
//    Field_Goalpost( BSmart::Field::center_mark, BSmart::Field::center_radius, 42 )
};

const Field_Bar Field_Hardware::field_bars_robot[Field_Hardware::NUMBER_FIELD_BARS] =
{
/**/
    //border left
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_outer_field_corner.x + BSmart::Field::robot_radius , BSmart::Field::lower_own_outer_field_corner.y + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_own_outer_field_corner.x + BSmart::Field::robot_radius , BSmart::Field::upper_own_outer_field_corner.y - BSmart::Field::robot_radius ) , Field_Hardware::BAR_VERTICAL , 80),
    //border above
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_outer_field_corner.x + BSmart::Field::robot_radius , BSmart::Field::upper_own_outer_field_corner.y - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_outer_field_corner.x - BSmart::Field::robot_radius , BSmart::Field::upper_opponent_outer_field_corner.y - BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , 80),
    // border right
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_outer_field_corner.x - BSmart::Field::robot_radius , BSmart::Field::upper_opponent_outer_field_corner.x - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::lower_opponent_outer_field_corner.x - BSmart::Field::robot_radius , BSmart::Field::lower_opponent_outer_field_corner.y + BSmart::Field::robot_radius ) , Field_Hardware::BAR_VERTICAL , 80),
    //border below
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_outer_field_corner.x - BSmart::Field::robot_radius , BSmart::Field::lower_opponent_outer_field_corner.y + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::lower_own_outer_field_corner.x + BSmart::Field::robot_radius , BSmart::Field::lower_own_outer_field_corner.y + BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , 80),
/**/
    //goal left inside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x , BSmart::Field::lower_own_goal_corner.y + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x + BSmart::Field::robot_radius , BSmart::Field::lower_own_goal_corner_back.y + BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x + BSmart::Field::robot_radius , BSmart::Field::lower_own_goal_corner_back.y + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x + BSmart::Field::robot_radius , BSmart::Field::upper_own_goal_corner_back.y - BSmart::Field::robot_radius) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x + BSmart::Field::robot_radius, BSmart::Field::upper_own_goal_corner_back.y - BSmart::Field::robot_radius) , BSmart::Pose(BSmart::Field::upper_own_goal_corner.x , BSmart::Field::upper_own_goal_corner.y - BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal left outside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x , BSmart::Field::lower_own_goal_corner.y - BSmart::Field::goal_material - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::robot_radius , BSmart::Field::lower_own_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::robot_radius , BSmart::Field::lower_own_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::robot_radius , BSmart::Field::upper_own_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::robot_radius ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_goal_corner_back.x - BSmart::Field::goal_material - BSmart::Field::robot_radius , BSmart::Field::upper_own_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner.x , BSmart::Field::upper_own_goal_corner.y + BSmart::Field::goal_material + BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal left goalpost front
    Field_Bar( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x + BSmart::Field::robot_radius , BSmart::Field::lower_own_goal_corner.y ) , BSmart::Pose(BSmart::Field::lower_own_goal_corner.x + BSmart::Field::robot_radius , BSmart::Field::lower_own_goal_corner.y - BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_own_goal_corner.x + BSmart::Field::robot_radius , BSmart::Field::upper_own_goal_corner.y ) , BSmart::Pose(BSmart::Field::upper_own_goal_corner.x + BSmart::Field::robot_radius , BSmart::Field::upper_own_goal_corner.y + BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    //goal right inside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x , BSmart::Field::lower_opponent_goal_corner.y + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x - BSmart::Field::robot_radius, BSmart::Field::lower_opponent_goal_corner_back.y + BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x - BSmart::Field::robot_radius , BSmart::Field::lower_opponent_goal_corner_back.y + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x - BSmart::Field::robot_radius , BSmart::Field::upper_opponent_goal_corner_back.y - BSmart::Field::robot_radius ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x - BSmart::Field::robot_radius , BSmart::Field::upper_opponent_goal_corner_back.y - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x , BSmart::Field::upper_opponent_goal_corner.y - BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal right outside
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x , BSmart::Field::lower_opponent_goal_corner.y - BSmart::Field::goal_material - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::robot_radius, BSmart::Field::lower_opponent_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::robot_radius, BSmart::Field::lower_opponent_goal_corner_back.y - BSmart::Field::goal_material - BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::robot_radius , BSmart::Field::upper_opponent_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::robot_radius ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner_back.x + BSmart::Field::goal_material + BSmart::Field::robot_radius , BSmart::Field::upper_opponent_goal_corner_back.y + BSmart::Field::goal_material + BSmart::Field::robot_radius ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x , BSmart::Field::upper_opponent_goal_corner.y + BSmart::Field::goal_material + BSmart::Field::robot_radius ) , Field_Hardware::BAR_HORIZONTAL , BSmart::Field::goal_height ),
/**/
    //goal right goalpost front
    Field_Bar( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x - BSmart::Field::robot_radius , BSmart::Field::lower_opponent_goal_corner.y ) , BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x - BSmart::Field::robot_radius , BSmart::Field::lower_opponent_goal_corner.y - BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height ),
    Field_Bar( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x - BSmart::Field::robot_radius , BSmart::Field::upper_opponent_goal_corner.y ) , BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x - BSmart::Field::robot_radius , BSmart::Field::upper_opponent_goal_corner.y + BSmart::Field::goal_material ) , Field_Hardware::BAR_VERTICAL , BSmart::Field::goal_height )
};

const Field_Goalpost Field_Hardware::field_goalposts_robot[Field_Hardware::NUMBER_FIELD_GOALPOSTS] =
{
    //left below
    Field_Goalpost( BSmart::Field::lower_own_goal_corner , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::lower_own_goal_corner.x , BSmart::Field::lower_own_goal_corner.y - BSmart::Field::goal_material ) , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    //left above
    Field_Goalpost( BSmart::Field::upper_own_goal_corner , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::upper_own_goal_corner.x , BSmart::Field::upper_own_goal_corner.y + BSmart::Field::goal_material ) , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    //right below
    Field_Goalpost( BSmart::Field::lower_opponent_goal_corner , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::lower_opponent_goal_corner.x , BSmart::Field::lower_opponent_goal_corner.y - BSmart::Field::goal_material ) , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    //right above
    Field_Goalpost( BSmart::Field::upper_opponent_goal_corner , BSmart::Field::robot_radius , BSmart::Field::goal_height ),
    Field_Goalpost( BSmart::Pose(BSmart::Field::upper_opponent_goal_corner.x , BSmart::Field::upper_opponent_goal_corner.y + BSmart::Field::goal_material ) , BSmart::Field::robot_radius , BSmart::Field::goal_height )
};

Field_Obstacle::Field_Obstacle()
{
    ;
}

Field_Bar::Field_Bar()
{
    ;
}

Field_Bar::Field_Bar(BSmart::Pose point_a_, BSmart::Pose point_b_, Field_Hardware::Obstacle obstacle_, double height_)
                    : point_a(point_a_), point_b(point_b_), line(point_a_, point_b_), height(height_)
{
    dist = point_a.distance_to(point_b);
    obstacle = obstacle_;
}

Field_Bar::Field_Bar(const Field_Bar& other)
{
    obstacle = other.obstacle;
    point_a  = other.point_a;
    point_b  = other.point_b;
    line     = other.line;
    dist     = other.dist;
    height   = other.height;
}

Field_Goalpost::Field_Goalpost()
{
    ;
}

Field_Goalpost::Field_Goalpost(BSmart::Pose center_, int radius_, int height_)
                              : center(center_), circle(BSmart::Circle(center_, radius_)), radius(radius_), height(height_)
{
    this->obstacle = Field_Hardware::GOALPOST;
}

Field_Goalpost::Field_Goalpost(const Field_Goalpost& other)
{
    obstacle = other.obstacle;
    center   = other.center;
    circle   = other.circle;
    radius   = other.radius;
    height   = other.height;
}
