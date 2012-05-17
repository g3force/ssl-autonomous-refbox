#include "field.h"
#include "pose.h"
#include "math.h" // used for pi
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

namespace BSmart
{
    Pose::Pose(const double scale) : Double_Vector(scale), rotation(scale)
    {
    }

    Pose::Pose(const Double_Vector& trans, const double rot)
        : Double_Vector(trans), rotation(rot)
    {
    }

    Pose::Pose(const double x, const double y, const double rot)
        : Double_Vector(x, y), rotation(rot)
    {
    }

    Pose& Pose::operator+=(const Pose& other)
    {
        x += other.x;
        y += other.y;
        rotation += other.rotation;
        return *this;
    }

    Pose Pose::operator+(const Pose& other) const
    {
        Pose p(*this);

        return p += other;
    }

    Pose& Pose::operator-=(const Pose& other)
    {
        x -= other.x;
        y -= other.y;
        rotation -= other.rotation;
        return *this;
    }

    Pose Pose::operator-(const Pose& other) const
    {
        Pose p(*this);

        return p -= other;
    }

    Pose Pose::operator*(const Pose& other) const
    {
        Pose p(*this);
        p.x *= other.x;
        p.y *= other.y;
        p.rotation *= other.rotation;
        return p;
    }

    Pose& Pose::operator*=(const Pose& other)
    {
        x *=other.x;
        y *=other.y;
        rotation *= other.rotation;
        return *this;
    }

    Pose Pose::operator*(const double other) const
    {
        Pose p(*this);

        return p *= other;
    }

    Pose Pose::operator/(const Pose& other) const
    {
        Pose p(*this);

        return p /= other;
    }

    Pose& Pose::operator/=(const Pose& other)
    {
        x /=other.x;
        y /=other.y;
        rotation /= other.rotation;
        return *this;
    }

    Pose Pose::operator/(const double other) const
    {
        Pose p(*this);

        return p /= other;
    }

    bool Pose::operator==(const Pose& other) const
    {
        return (y == other.y &&
                x == other.x &&
                rotation == other.rotation);
    }

    bool Pose::operator!=(const Pose& other) const
    {
        return !operator==(other);
    }

    Pose& Pose::operator=(const Pose& other)
    {
        x = other.x;
        y = other.y;
        rotation = other.rotation;

        return *this;
    }

    Pose& Pose::rotate(const double angle)
    {
        const double x_old = x;
        const double y_old = y;
        const double cos_phi = cos (angle);
        const double sin_phi = sin (angle);
        x = x_old * cos_phi + y_old * sin_phi;
        y = -x_old * sin_phi + y_old * cos_phi;
        return *this;
    }

    Pose& Pose::normalize_rotation()
    {
        rotation = atan2(cos(rotation), sin(rotation));
        return *this;
    }

    double Pose::angle_to(const BSmart::Pose& p) const
    {
        const double dx = p.x - x;
        const double dy = p.y - y;
        const double angle = atan2(dy, dx);
        double rot = rotation;
        if (rot < -BSmart::pi)
            rot = rotation + 2 * BSmart::pi;
        const double angle_diff = angle - rot;
        //normalize
        return atan2(sin(angle_diff),cos(angle_diff));
    }

/*
    double Pose::distance_to(const BSmart::Pose& p) const
    {
        return (p - *this).abs();
    }
*/
    double Pose::distance_to(const BSmart::Double_Vector& p) const
    {
        return (p - *this).abs();
    }

    Pose& Pose::clip_pos_to(double clipval)
    {
        if(std::abs(this->x) > clipval)
        {
            if(this->x < 0)
                this->x = -clipval;
            else
                this->x = clipval;
        }
        if(std::abs(this->y) > clipval)
        {
            if(this->y < 0)
                this->y = -clipval;
            else
                this->y = clipval;
        }

        return *this;
    }
}

std::ostream& operator<<(std::ostream& os, const BSmart::Pose& p)
{
    return os << "{x=" << p.x << ", y=" << p.y << ", rotation=" << p.rotation << "}";
}
