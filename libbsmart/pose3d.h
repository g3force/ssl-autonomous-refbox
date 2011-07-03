#ifndef POSE3D_H
#define POSE3D_H

#include "vector3.h"
#include "pose.h"
#include <ostream>

namespace BSmart
{
    /** representation for 3D Transformation and Position (Location in 3D)*/
    class Pose3D : public Double_Vector_3D
    {
        public:

        /**
         * constructor with scalar
         */
        Pose3D(const double scale = 0);

        /**
         * constructor from other vector
         * param trans translation (Vector3)
         */
        Pose3D(const Double_Vector_3D& trans);

        /**
         * constructor from 2D vector
         * param trans translation (Vector2)
         */
        Pose3D(const Double_Vector& trans);

        /**
         * constructor from rotation and translation
         * param x translation.x (double)
         * param y translation.y (double)
         * param z translation.z (double)
         */
        Pose3D(const double x, const double y, const double z);

        ~Pose3D() {};

        /**
         * Concatenation of this pose with another pose.
         * param other The other pose that will be concatenated to this one.
         * return A reference to this pose after concatenation.
         */
        Pose3D& operator+=(const Pose3D& other);

        /**
         * A concatenation of this pose and another pose.
         * param other The other pose that will be concatenated to this one.
         * return The resulting pose.
         */
        Pose3D operator+(const Pose3D& other) const;

        /**
         * Difference of this pose relative to another pose.
         * param other The other pose that will be used as origin for the new pose.
         * return A reference to this pose after calculating the difference.
         */
        Pose3D& operator-=(const Pose3D& other);

        /**
         * Difference of this pose relative to another pose.
         * param other The other pose that will be used as origin for the new pose.
         * return The resulting pose.
         */
        Pose3D operator-(const Pose3D& other) const;

        Pose3D operator*(const Pose3D& other) const;
        Pose3D operator*(const double scale) const;
        Pose3D& operator*=(const Pose3D&);

        Pose3D operator/(const Pose3D& other) const;
        Pose3D operator/(const double scale) const;
        Pose3D& operator/=(const Pose3D&);

        bool operator==(const Pose3D& other) const;
        bool operator!=(const Pose3D& other) const;

        Pose3D& operator=(const Pose3D& other);

        /**
         * Return the distance to another pose.
         * param Pose to which the distance should be calculated
         * return Distance to the Pose given as Parameter
         */
        double distance_to_3D(const BSmart::Pose3D& p) const;
        double distance_to_2D(const BSmart::Pose3D& p) const;
        double distance_to_3D(const BSmart::Pose& p) const;
        double distance_to_2D(const BSmart::Pose& p) const;
        double distance_to_3D(const BSmart::Double_Vector& p) const;
        double distance_to_2D(const BSmart::Double_Vector& p) const;

    };
}

std::ostream& operator<<(std::ostream& os, const BSmart::Pose3D& p);

#endif // POSE3D_H__
