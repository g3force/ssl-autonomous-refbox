#ifndef VECTOR2_H
#define VECTOR2_H

#include <ostream>
#include <cmath>

namespace BSmart
{
/** A representation of a two-dimensional vector */
template < class T > class Vector2
{
public:
    /** The values */
    T x, y;

    /** Default constructor. */
    Vector2 < T > ()
    {
        x = y = 0;
    }



    /**
     * Constructing with same values for both dimensions
     * param xy The value to be assigned to both dimensions
     */
    Vector2 < T > (T xy): x(xy), y(xy)
    {
    }



    /**
     * brief Constructing with different values for each dimension
     * param x The value to be assigned to the x component
     * param y The value to be assigned to the y component
     */
    Vector2 < T > (T x, T y): x(x), y(y)
    {
    }



    /**
     * Copy constructor
     * param other The vector that is copied to this one
     */
    Vector2 < T > (const Vector2 < T > &other_vector)
    {
        *this = other_vector;
    }

    //brief virtual destructor to allow derived classes to clean-up
    virtual ~Vector2()
    {
    }



    /**
     * Assignment operator
     * param other_vector The vector that should be assigned to this vector
     * return The reference to the assigned vector
     */
    Vector2 < T > &operator= (const Vector2 < T > &other_vector)
    {
        x = other_vector.x;
        y = other_vector.y;
        return *this;
    }



    /**
     * Compare another vector with this vector
     * param other_vector The other vector to be compared with this one
     * return True if both are equal, false otherwise
     */
    bool operator== (const Vector2 < T > &other_vector) const
    {
        return (x == other_vector.x && y == other_vector.y);
    }



    /**
     * Compare another vector with this vector
     * param other_vector The other vector that to be compared to this one.
     * return True if both are not equal, false otherwise
     */
    bool operator!= (const Vector2 < T > &other_vector) const
    {
        return !(*this == other_vector);
    }



    /**
     * Calculation of the absolut length.
     * return The absolut length.
     */
    T abs () const
    {
        return (T) sqrt ((*this) * (*this));
    }



    /**
     * Rotate Vector clockwise
     * @param degrees the value in rad for the rotation
     * @param offs offset for the point, this vector should be rotated around
     * @return The rotated Vector
     **/
    Vector2 < T > rotate (double degree, const Vector2 < T >& offs = Vector2 < T > (0, 0))
    {
        T x_old = x - offs.x;
        T y_old = y - offs.y;
        T cos_phi = cos (degree);
        T sin_phi = sin (degree);
        x = x_old * cos_phi + y_old * sin_phi;
        y = -x_old * sin_phi + y_old * cos_phi;

        x += offs.x;
        y += offs.y;
        return *this;
    }



    /**
     * Return a rotated vector withound manipulating the vectors values
     *
     * @param degrees the value in degrees for the rotation
     * @param offs offset for the point, this vector should be rotated around
     * @return The rotated Vector
     **/
    Vector2 < T > rotated (double degree, const Vector2 < T >& offs = Vector2 < T > (0, 0)) const
    {
        Vector2 < T > rotated_vec(*this);
        rotated_vec.rotate(degree, offs);

        return rotated_vec;
    }



    /**
     * Calculation of the angle of this vector
     * @return the angle of the vector
     */
    double angle () const
    {
        return atan2 (y, x);
    }



    /**
     * Normalization of the vector.
     * @param len The length the vector should be normalized
     * @return The normalized vector
     */
    Vector2 < T > normalize (T len)
    {
        if (abs () == 0)
        return *this;
        return *this *= len / abs ();
    }



    /**
     * Normalize this vector.
     * @return The normalized vector.
     */
    Vector2 < T > normalize ()
    {
        if (abs () == 0)
        return *this;
        return *this /= abs ();
    }



    /**
     * Transposition of this vector.
     * @return The transposed vector.
     */
    Vector2 < T > transpose ()
    {
        T buffer = x;
        x = y;
        y = buffer;
        return *this;
    }



    /**
     * Addition of a vector to this vector
     * @param other_vector The other vector that will be added to this one
     * @return The reference to the vector after the addition
     */
    Vector2 < T > &operator+= (const Vector2 < T > &other_vector)
    {
        x += other_vector.x;
        y += other_vector.y;
        return *this;
    }



    /**
     * Substraction of another vector from this one
     * @param other_vector The other vector to subtract from the main vector
     * @return The reference to the vector after the addition
     */
    Vector2 < T > &operator-= (const Vector2 < T > &other_vector)
    {
        x -= other_vector.x;
        y -= other_vector.y;
        return *this;
    }



    /**
     * Multiplication of this vector by a factor
     * @param factor The factor
     * @return The reference to the vector after the multiplication by the factor
     */
    Vector2 < T > &operator*= (const T & factor)
    {
        x *= factor;
        y *= factor;
        return *this;
    }



    /**
     * Division of this vector by a factor
     * @param factor The factor
     * @return The reference to the vector after the division by the factor
     */
    Vector2 < T > &operator/= (const T & factor)
    {
        if (factor == 0)
        return *this;
        x /= factor;
        y /= factor;
        return *this;
    }



    /**
     * Addition of a vector to this vector
     * @param other_vector The other vector that will be added to this one
     * @return A new vector with the value of the addition
     */
    Vector2 < T > operator+ (const Vector2 < T > &other_vector) const
    {
        return Vector2 < T > (*this) += other_vector;
    }



    /**
     * Substraction of another vector from this one
     * @param other_vector The other vector to subtract from the main vector
     * @return  A new vector with the value of the subtraction
     */
    Vector2 < T > operator- (const Vector2 < T > &other_vector) const
    {
        return Vector2 < T > (*this) -= other_vector;
    }



    /**
     * Multiplication of this vector with another one
     * @param other_vector The other vector this one will be multiplied
     * @return A ector with the value of the multiplication
     */
    T operator* (const Vector2 < T > &other_vector) const
    {
        return x * other_vector.x + y * other_vector.y;
    }



    /**
     * Multiplication of this vector by a factor
     * @param factor The factor
     * @return A vector with the value of the multiplication
     */
    Vector2 < T > operator* (const T & factor) const
    {
        return Vector2 < T > (*this) *= factor;
    }



    /**
     * Division of this vector by a factor
     * @param factor The factor
     * @return A vector with the value of the multiplication
     */
    Vector2 < T > operator/ (const T & factor) const
    {
        return Vector2 < T > (*this) /= factor;
    }

    /** calculate the distance of an object to a line between 2 other objects */
    double calculate_dist(Vector2<T> from, Vector2<T> to)
    {
        double lambda = (((to.x - from.x)*x + (to.y - from.y)*y)
                          - ((to.x - from.x)*from.x + (to.y - from.y)*from.y))
                          / ((to.x - from.x)*(to.x - from.x) + (to.y - from.y)*(to.y - from.y));
        // calculate projection-point
        Vector2<T> projPoint;
        projPoint.x = from.x + lambda*(to.x - from.x);
        projPoint.y = from.y + lambda*(to.y - from.y);
        // calculate distance-vector
        Vector2<T> distVec;
        distVec.x = x - projPoint.x;
        distVec.y = y - projPoint.y;

        // check if the distance to line is realy between the two points
        double BP = sqrt(pow(to.x-x,2) + pow(to.y-y,2));
        double AB = sqrt(pow(to.x-from.x,2) + pow(to.y-from.y,2));
        double AP = sqrt(pow(from.x-x,2) + pow(from.y-y,2));
        double BP_pow2 = pow(BP,2);
        double AB_pow2 = pow(AB,2);
        double AP_pow2 = pow(AP,2);

        double angle_to_line_1 = acos((BP_pow2+AB_pow2-AP_pow2)/(2*BP*AB));
        double angle_to_line_2 = acos((AP_pow2+AB_pow2-BP_pow2)/(2*AP*AB));

        if(angle_to_line_1>M_PI/2.)
            return BP;
        else if(angle_to_line_2>M_PI/2.)
            return AP;
        else
            return sqrt(pow(distVec.x,2) + pow(distVec.y,2));
    }

    double length() const
    {
        return sqrt(x*x + y*y);
    }

}; //class Vector2

// Some typdefs
typedef Vector2<double> Double_Vector;
typedef Vector2<float> Float_Vector;
typedef Vector2<int> Int_Vector;
} //namespace BSmart

// For easy printing
std::ostream& operator<<(std::ostream& os, const BSmart::Double_Vector& v);
std::ostream& operator<<(std::ostream& os, const BSmart::Float_Vector& v);
std::ostream& operator<<(std::ostream& os, const BSmart::Int_Vector& v);
#endif
