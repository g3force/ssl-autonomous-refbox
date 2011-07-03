#ifndef MATH_H
#define MATH_H

#include <vector>
#include <cstdlib> //random()

#include "circle.h"
#include "line.h"

namespace BSmart
{
    const double pi = 3.1415926535897932384626433832795;

    /** constant for a full circle*/
    const double pi2 = 2.0*pi;

    /** constant for three quater circle*/
    const double pi3_2 = 1.5*pi;

    /** constant for a quarter circle*/
    const double pi_2 = 0.5*pi;

    /** constant for a 1/8 circle*/
    const double pi_4 = pi*0.25;

    /** constant for a 1/16 circle*/
    const double pi_8 = pi*0.125;

    /** constant for a 3/8 circle*/
    const double pi3_4 = pi*0.75;

    /** constant for a 1 degree*/
    const double pi_180 = pi/180.0;

    inline double deg_to_rad(double degree)
    {
        return degree * pi / 180.0;
    }

    inline double rad_to_deg(double radiant)
    {
        return radiant * 180.0 / pi;
    }

    inline double normalize(double angle) {
      return atan2(sin(angle),cos(angle));
    }

    inline int min(const int a, const int b)
    {
        return (a > b) ? b : a;
    }

    inline int max(const int a, const int b)
    {
        return (a < b) ? b : a;
    }


    inline int random(int min = 0, int max = 1)
    {
        //exchanges values
        if (min > max)
        {
            int temp(max);
            max = min;
            min = temp;
        }
        return min + (::random() % (max - min + 1));
    }

    /** Checks if three points are ordered counter clockwise*/
    inline int ccw(const Double_Vector& p0,
                   const Double_Vector& p1,
                   const Double_Vector& p2)
    {
        const double ccw_dx1 = p1.x - p0.x;
        const double ccw_dy1 = p1.y - p0.y;
        const double ccw_dx2 = p2.x - p0.x;
        const double ccw_dy2 = p2.y - p0.y;
        if((ccw_dx1*ccw_dy2) > (ccw_dy1*ccw_dx2))
        {
            return 1;
        }
        if((ccw_dx1*ccw_dy2) < (ccw_dy1*ccw_dx2))
        {
            return -1;
        }
        // Now (dx1*dy2 == dy1*dx2) must be true
        if((ccw_dx1*ccw_dx2 < 0.0) || (ccw_dy1*ccw_dy2 < 0.0))
        {
            return -1;
        }
        if((ccw_dx1*ccw_dx1 + ccw_dy1*ccw_dy1) >= (ccw_dx2*ccw_dx2 + ccw_dy2*ccw_dy2))
        {
            return 0;
        }
        return 1;
    }

    inline bool test_intersection(const Line& l1, const Line& l2)
    {
        return (((ccw(l1.p1, l1.p2, l2.p1) * ccw(l1.p1,l1.p2,l2.p2)) <= 0)
                && ((ccw(l2.p1, l2.p2, l1.p1) * ccw(l2.p1,l2.p2,l1.p2)) <= 0));
    }

    //gives only a heuristic, faster than test_intersection
    inline bool test_intersection_2(const Line& l1, const Line& l2)
    {
        if(std::min(l1.p1.x, l1.p2.x) > std::max(l2.p1.x, l2.p2.x))
            return false;
        if(std::max(l1.p1.x, l1.p2.x) < std::min(l2.p1.x, l2.p2.x))
            return false;
        if(std::min(l1.p1.y, l1.p2.y) > std::max(l2.p1.y, l2.p2.y))
            return false;
        if(std::max(l1.p1.y, l1.p2.y) < std::min(l2.p1.y, l2.p2.y))
            return false;

        return true;
    }


    inline bool intersection_point(const Line& l1,
                                   const Line& l2,
                                   Double_Vector* intersection = 0)
    {
        const Double_Vector& p = l1.p1;
        const Double_Vector v(l1.p2 - l1.p1);
        const Double_Vector& q = l2.p1;
        const Double_Vector w(l2.p2 - l2.p1);

        const double denominator = w.x*v.y - w.y*v.x;
        if(denominator == 0.0) return false; // the two lines are parallel

        const double s = ((q.y - p.y) * w.x - (q.x - p.x) * w.y) / denominator;
        const double t = ((q.y - p.y) * v.x - (q.x - p.x) * v.y) / denominator;

        // they would intersecpt, but are too short
        if(s < 0.0 || s > 1.0 || t < 0.0 || t > 1.0) return false;
        if(intersection) *intersection = p + v * s;

        return true;
    }

    //gives only a heuristic, faster than intersection_point
    //only checks for segments, not for complete lines
    inline bool test_intersection(const Circle& circle,
                                  const Line& line)
    {
        if((circle.x+circle.radius) < std::min(line.p1.x, line.p2.x))
            return false;
        if((circle.x-circle.radius) > std::max(line.p1.x, line.p2.x))
            return false;
        if((circle.y+circle.radius) < std::min(line.p1.y, line.p2.y))
            return false;
        if((circle.y-circle.radius) > std::max(line.p1.y, line.p2.y))
            return false;
        return true;
    }

    inline bool intersection_point(const Circle& circle,
                                   const Line& line,
                                   std::vector<Double_Vector>& intersections)
    {
        // circle: x² + y² = r²
        //              y = sqrt(r² - x²)
        // line  :      y = mx + b

        const Double_Vector v(line.p2-line.p1);
        const Double_Vector d(line.p1-circle);
        const double r = circle.radius;
        const double vv = v * v;
        const double dv = d * v;
        const double dd = d * d;
        if(vv != 0.0)
        {
            const double term = - dv/vv;
            const double denominator = (vv*(r*r - dd) + dv*dv) / (vv*vv);
            if(denominator < 0.0)
            {
                return false;
            }
            const double sqrtValue = sqrt(denominator);
            double s = term + sqrtValue;
            //test auf Länge?
/*
            if((s < 0.0) || (s > line.length()))
            {
                return false;
            }
*/
            Double_Vector p1(line.p1 + (v*s));
            intersections.push_back(p1);
            if(sqrtValue != 0.0)
            {
                s = term - sqrtValue;
                Double_Vector p2(line.p1 + (v*s));
                intersections.push_back(p2);
            }
            return true;
        }
        else
            return false;
    }

    // Ortoganal distance
/*    inline double distance(const Line& line, const Double_Vector& point)
    {
    }
*/

/*    void intersectLineAndLine(const Line& l1, const Line& l2,
                              std::vector<PfVec>& intersections)
    {
        if(testIntersection(l1,l2))
        {
            PfVec p = l1.p1;
            PfVec v = (l1.p2 - l1.p1);
            PfVec q = l2.p1;
            PfVec w = (l2.p2 - l2.p1);
            PfVec u(q-p);
            double denominator = w.x*v.y - w.y*v.x;
            if(denominator != 0.0)
            {
                double s = (u.y*v.x - u.x*v.y) / denominator;
                PfVec pt(q);
                pt += (w*s);
                intersections.push_back(pt);
            }
        }
    }
*/
}



#endif
