#ifndef POSE_H
#define POSE_H

#include "vector2.h"
#include <ostream>

namespace BSmart
{
    /** representation for 2D Transformation and Position (Location + Orientation)*/
    class Pose : public Double_Vector
    {
        public:

        /** Rotation as an angle*/
        double rotation;

        /**
         * constructor from rotation
         * param rot rotation (double)
         */
        Pose(const double scale = 0);

        /**
         * constructor from rotation and translation
         * param rot rotation (double)
         * param trans translation (Vector2)
         */
        Pose(const Double_Vector& trans, const double rot = 0);

        /**
         * constructor from rotation and translation
         * param x translation.x (double)
         * param y translation.y (double)
         * param rot rotation (double)
         */
        Pose(const double x, const double y, const double rot = 0);

        /**
         * Concatenation of this pose with another pose.
         * param other The other pose that will be concatenated to this one.
         * return A reference to this pose after concatenation.
         */
        Pose& operator+=(const Pose& other);

        /**
         * A concatenation of this pose and another pose.
         * param other The other pose that will be concatenated to this one.
         * return The resulting pose.
         */
        Pose operator+(const Pose& other) const;

        /**
         * Difference of this pose relative to another pose.
         * param other The other pose that will be used as origin for the new pose.
         * return A reference to this pose after calculating the difference.
         */
        Pose& operator-=(const Pose& other);

        /**
         * Difference of this pose relative to another pose.
         * param other The other pose that will be used as origin for the new pose.
         * return The resulting pose.
         */
        Pose operator-(const Pose& other) const;

        Pose operator*(const Pose& other) const;
        Pose operator*(const double scale) const;
        Pose& operator*=(const Pose&);

        Pose operator/(const Pose& other) const;
        Pose operator/(const double scale) const;
        Pose& operator/=(const Pose&);

        bool operator==(const Pose& other) const;
        bool operator!=(const Pose& other) const;

        Pose& operator=(const Pose& other);

        /**
         * Rotate this pose by a rotation
         * param angle Angle to rotate.
         * return A reference to this pose after rotation
         */
        Pose& rotate(const double angle);

        /**
         * Normalize the rotation of this pose.
         * return A reference to this pose after rotation.
         */
        Pose& normalize_rotation();

        /**
         * Return the angle (positive or negative) to another pose.
         * param Pose to which the angle should be calculated
         * return Angle to the Pose given as Parameter, from -PI to PI (0.0 is directly ahead)
         */
        double angle_to(const BSmart::Pose& p) const;

        /**
         * Return the distance to another pose.
         * param Pose to which the distance should be calculated
         * return Distance to the Pose given as Parameter
         */
//        double distance_to(const BSmart::Pose& p) const;
        double distance_to(const BSmart::Double_Vector& p) const;

        /**
         * Looks at pos coordinates and clips the value to the absolute
         * value of clipval depending on the initial sign.
         *
         * attention: Assumes clipval to be positive
         */
        Pose& clip_pos_to(double clipval);
    };
}

std::ostream& operator<<(std::ostream& os, const BSmart::Pose& p);

#endif // POSE_H__
