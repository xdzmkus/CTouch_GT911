#include "Touch_Point.h"

Touch_Point::Touch_Point() : id(0), x(0), y(0), size(0)
{
}

Touch_Point::Touch_Point(uint8_t id, uint16_t x, uint16_t y, uint16_t size) : id(id), x(x), y(y), size(size)
{
}


bool Touch_Point::operator==(Touch_Point point) const
{
	return ((point.x == x) && (point.y == y) && (point.size == size));
}

bool Touch_Point::operator!=(Touch_Point point) const
{
	return ((point.x != x) || (point.y != y) || (point.size != size));
}
