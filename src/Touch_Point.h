// Touch_Point.h

#ifndef _TOUCH_POINT_h
#define _TOUCH_POINT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Touch_Point
{
public:
    Touch_Point();
    Touch_Point(uint8_t id, uint16_t x, uint16_t y, uint16_t size);

    bool operator==(Touch_Point) const;
    bool operator!=(Touch_Point) const;

    uint8_t id;
    uint16_t x;
    uint16_t y;
    uint16_t size;
};

#endif

