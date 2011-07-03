#ifndef LINE_H
#define LINE_H

#include "vector2.h"
#include "vector3.h"

namespace BSmart
{


class Line
{
public:
    Line()
    {
        ;
    }

    Line(const double& x1_, const double& y1_, const double& x2_, const double& y2_): p1(x1_,y1_), p2(x2_, y2_)
    {
    }

    Line(const Double_Vector& point1, const Double_Vector& point2): p1(point1), p2(point2)
    {
    }

    Line(const Double_Vector_3D& point1, const Double_Vector_3D& point2): p1(point1.x, point1.y), p2(point2.x, point2.y)
    {
    }

    double length() const
    {
        return (p1 - p2).abs();
    }

    Double_Vector p1;
    Double_Vector p2;
};

} // End Namespace

#endif
