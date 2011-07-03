#include "pose3d.h"
#include "field.h"
#include "pose.h"
#include "math.h" // used for pi
#include <iostream>

namespace BSmart
{
    Pose3D::Pose3D(const double scale) : Double_Vector_3D(scale)
    {
    }

    Pose3D::Pose3D(const Double_Vector_3D& trans)
        : Double_Vector_3D(trans)
    {
    }

    Pose3D::Pose3D(const Double_Vector& trans)
        : Double_Vector_3D(trans.x, trans.y, 0.)
    {
    }

    Pose3D::Pose3D(const double x, const double y, const double z)
        : Double_Vector_3D(x, y, z)
    {
    }

    Pose3D& Pose3D::operator+=(const Pose3D& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Pose3D Pose3D::operator+(const Pose3D& other) const
    {
        Pose3D p(*this);

        return p += other;
    }

    Pose3D& Pose3D::operator-=(const Pose3D& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Pose3D Pose3D::operator-(const Pose3D& other) const
    {
        Pose3D p(*this);

        return p -= other;
    }

    Pose3D Pose3D::operator*(const Pose3D& other) const
    {
        Pose3D p(*this);
        p.x *= other.x;
        p.y *= other.y;
        p.z *= other.z;
        return p;
    }

    Pose3D Pose3D::operator*(const double other) const
    {
        Pose3D p(*this);

        return p *= other;
    }

    Pose3D& Pose3D::operator*=(const Pose3D& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Pose3D Pose3D::operator/(const Pose3D& other) const
    {
        Pose3D p(*this);

        return p /= other;
    }

    Pose3D Pose3D::operator/(const double other) const
    {
        Pose3D p(*this);

        return p /= other;
    }

    Pose3D& Pose3D::operator/=(const Pose3D& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    bool Pose3D::operator==(const Pose3D& other) const
    {
        return (y == other.y &&
                x == other.x &&
                z == other.z);
    }

    bool Pose3D::operator!=(const Pose3D& other) const
    {
        return !operator==(other);
    }

    Pose3D& Pose3D::operator=(const Pose3D& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }

    double Pose3D::distance_to_3D(const BSmart::Pose3D& p) const
    {
        return (p - *this).abs();
    }

    double Pose3D::distance_to_2D(const BSmart::Pose3D& p) const
    {
        Pose one(this->x, this->y);
        Pose two(p.x, p.y);
        return (one - two).abs();
    }

    double Pose3D::distance_to_3D(const BSmart::Pose& p) const
    {
        Pose3D tmp(p.x, p.y, 0.);
        return (tmp - *this).abs();
    }

    double Pose3D::distance_to_2D(const BSmart::Pose& p) const
    {
        Pose one(this->x, this->y);
        return (one - p).abs();
    }

    double Pose3D::distance_to_3D(const BSmart::Double_Vector& p) const
    {
        Pose3D tmp(p.x, p.y, 0.);
        return (tmp - *this).abs();
    }

    double Pose3D::distance_to_2D(const BSmart::Double_Vector& p) const
    {
        Pose one(this->x, this->y);
        return (one - p).abs();
    }
}

std::ostream& operator<<(std::ostream& os, const BSmart::Pose3D& p)
{
    return os << "{x=" << p.x << ", y=" << p.y << ", z=" << p.z << "}";
}
