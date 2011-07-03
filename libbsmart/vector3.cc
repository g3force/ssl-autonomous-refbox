#include <ostream>
#include "vector3.h"

std::ostream& operator<<(std::ostream& os, const BSmart::Double_Vector_3D& v)
{
    return os << "{x=" << v.x << " y=" << v.y << " z=" << v.z << "}";
}

std::ostream& operator<<(std::ostream& os, const BSmart::Float_Vector_3D& v)
{
    return os << "{x=" << v.x << " y=" << v.y << " z=" << v.z << "}";
}

std::ostream& operator<<(std::ostream& os, const BSmart::Int_Vector_3D& v)
{
    return os << "{x=" << v.x << " y=" << v.y << " z=" << v.z << "}";
}
