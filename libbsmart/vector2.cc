#include <ostream>
#include "vector2.h"

std::ostream& operator<<(std::ostream& os, const BSmart::Double_Vector& v)
{
    return os << "{x=" << v.x << " y=" << v.y << "}";
}

std::ostream& operator<<(std::ostream& os, const BSmart::Float_Vector& v)
{
    return os << "{x=" << v.x << " y=" << v.y << "}";
}

std::ostream& operator<<(std::ostream& os, const BSmart::Int_Vector& v)
{
    return os << "{x=" << v.x << " y=" << v.y << "}";
}
