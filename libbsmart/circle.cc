#include "circle.h"

std::ostream& operator<<(std::ostream& os, const BSmart::Circle& p)
{
    return os << "{x=" << p.x << ", y=" << p.y << ", r=" << p.radius << "}";
}
