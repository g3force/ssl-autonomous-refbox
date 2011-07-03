#ifndef VECTOR3_H
#define VECTOR3_H

#include <ostream>
#include <cmath>

namespace BSmart
{
/** A representation of a three-dimensional vector */
template < class T > class Vector3
{
public:
    /** The values */
    T x, y, z;

    /** Default constructor. */
    Vector3 < T > ()
    {
        x = y = z = 0;
    }



    /**
     * Constructing with same values for all dimensions
     * param xyz The value to be assigned to all dimensions
     */
    Vector3 < T > (T xyz): x(xyz), y(xyz), z(xyz)
    {
    }



    /**
     * brief Constructing with different values for each dimension
     * param x The value to be assigned to the x component
     * param y The value to be assigned to the y component
     * param z The value to be assigned to the z component
     */
    Vector3 < T > (T x, T y, T z): x(x), y(y), z(z)
    {
    }



    /**
     * Copy constructor
     * param other The vector that is copied to this one
     */
    Vector3 < T > (const Vector3 < T > &other_vector)
    {
        *this = other_vector;
    }

    //brief virtual destructor to allow derived classes to clean-up
    virtual ~Vector3()
    {
    }



    /**
     * Assignment operator
     * param other_vector The vector that should be assigned to this vector
     * return The reference to the assigned vector
     */
    Vector3 < T > &operator= (const Vector3 < T > &other_vector)
    {
        x = other_vector.x;
        y = other_vector.y;
        z = other_vector.z;
        return *this;
    }



    /**
     * Compare another vector with this vector
     * param other_vector The other vector to be compared with this one
     * return True if both all equal, false otherwise
     */
    bool operator== (const Vector3 < T > &other_vector) const
    {
        return (x == other_vector.x && y == other_vector.y && z == other_vector.z);
    }



    /**
     * Compare another vector with this vector
     * param other_vector The other vector that to be compared to this one.
     * return True if all are not equal, false otherwise
     */
    bool operator!= (const Vector3 < T > &other_vector) const
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
     * Addition of a vector to this vector
     * @param other_vector The other vector that will be added to this one
     * @return The reference to the vector after the addition
     */
    Vector3 < T > &operator+= (const Vector3 < T > &other_vector)
    {
        x += other_vector.x;
        y += other_vector.y;
        z += other_vector.z;
        return *this;
    }



    /**
     * Substraction of another vector from this one
     * @param other_vector The other vector to subtract from the main vector
     * @return The reference to the vector after the addition
     */
    Vector3 < T > &operator-= (const Vector3 < T > &other_vector)
    {
        x -= other_vector.x;
        y -= other_vector.y;
        z -= other_vector.z;
        return *this;
    }



    /**
     * Multiplication of this vector by a factor
     * @param factor The factor
     * @return The reference to the vector after the multiplication by the factor
     */
    Vector3 < T > &operator*= (const T & factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }



    /**
     * Division of this vector by a factor
     * @param factor The factor
     * @return The reference to the vector after the division by the factor
     */
    Vector3 < T > &operator/= (const T & factor)
    {
        if (factor == 0)
        return *this;
        x /= factor;
        y /= factor;
        z /= factor;
        return *this;
    }



    /**
     * Addition of a vector to this vector
     * @param other_vector The other vector that will be added to this one
     * @return A new vector with the value of the addition
     */
    Vector3 < T > operator+ (const Vector3 < T > &other_vector) const
    {
        return Vector3 < T > (*this) += other_vector;
    }



    /**
     * Substraction of another vector from this one
     * @param other_vector The other vector to subtract from the main vector
     * @return  A new vector with the value of the subtraction
     */
    Vector3 < T > operator- (const Vector3 < T > &other_vector) const
    {
        return Vector3 < T > (*this) -= other_vector;
    }



    /**
     * Multiplication of this vector with another one
     * @param other_vector The other vector this one will be multiplied
     * @return A ector with the value of the multiplication
     */
    T operator* (const Vector3 < T > &other_vector) const
    {
        return x * other_vector.x + y * other_vector.y + z * other_vector.z;
    }



    /**
     * Multiplication of this vector by a factor
     * @param factor The factor
     * @return A vector with the value of the multiplication
     */
    Vector3 < T > operator* (const T & factor) const
    {
        return Vector3 < T > (*this) *= factor;
    }



    /**
     * Division of this vector by a factor
     * @param factor The factor
     * @return A vector with the value of the multiplication
     */
    Vector3 < T > operator/ (const T & factor) const
    {
        return Vector3 < T > (*this) /= factor;
    }

    //returns length of the vector
    double length() const
    {
        return sqrt(x*x + y*y + z*z);
    }

}; //class Vector3

// Some typdefs
typedef Vector3<double> Double_Vector_3D;
typedef Vector3<float> Float_Vector_3D;
typedef Vector3<int> Int_Vector_3D;
} //namespace BSmart

// For easy printing
std::ostream& operator<<(std::ostream& os, const BSmart::Double_Vector_3D& v);
std::ostream& operator<<(std::ostream& os, const BSmart::Float_Vector_3D& v);
std::ostream& operator<<(std::ostream& os, const BSmart::Int_Vector_3D& v);
#endif
