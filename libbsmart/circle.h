#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector2.h"
#include <ostream>

namespace BSmart
{

class Circle : public Double_Vector
{
public:
    Circle()
    {
        ;
    }

    Circle(double x_, double y_, double radius_):Double_Vector(x_,y_), radius(radius_)
    {
    }

    Circle(const Double_Vector& point, double radius_): Double_Vector(point), radius(radius_)
    {
    }

    Circle(const Circle& other): Double_Vector(other.x, other.y), radius(other.radius)
    {
    }

/*    const Circle& operator=(const Circle& other)
    {
    }
*/

    double radius;
};

} // End namespace


std::ostream& operator<<(std::ostream& os, const BSmart::Circle& p);

#endif
