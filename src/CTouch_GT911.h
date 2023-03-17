#ifndef _CTouch_GT911_h
#define _CTouch_GT911_h

#include <Arduino.h>

#include "Touch_Point.h"

#define GT911_ADDR1 0x5D
#define GT911_ADDR2 0x14

#define GT911_POINTS_INFO  0X814E
#define GT911_POINTS_DATA  0X814F

typedef void (*isr_TouchDetected)();

typedef void (*cb_1_Point_Touched)(const Touch_Point&);
typedef void (*cb_2_Point_Touched)(const Touch_Point&, const Touch_Point&);
typedef void (*cb_3_Point_Touched)(const Touch_Point&, const Touch_Point&, const Touch_Point&);
typedef void (*cb_4_Point_Touched)(const Touch_Point&, const Touch_Point&, const Touch_Point&, const Touch_Point&);
typedef void (*cb_5_Point_Touched)(const Touch_Point&, const Touch_Point&, const Touch_Point&, const Touch_Point&, const Touch_Point&);

class GT911
{
public:

	GT911(uint8_t C_SDA, uint8_t C_SCL, uint8_t C_INT, uint8_t C_RST, uint8_t I2C_ADDR = GT911_ADDR1);

    void init(isr_TouchDetected onTouchDetected = NULL);

    void process();

    void setCallback_1_PointTouched(cb_1_Point_Touched cb);
    void setCallback_2_PointTouched(cb_2_Point_Touched cb);
    void setCallback_3_PointTouched(cb_3_Point_Touched cb);
    void setCallback_4_PointTouched(cb_4_Point_Touched cb);
    void setCallback_5_PointTouched(cb_5_Point_Touched cb);

protected:

    Touch_Point parsePoint(uint8_t* data);

    uint8_t I2C_SendByte(uint8_t Device_Address, uint16_t Register_Address, uint8_t Data);
    uint8_t I2C_SendBlock(uint8_t Device_Address, uint16_t Register_Address, uint8_t* Data, uint8_t Size);

    uint8_t I2C_ReadByte(uint8_t Device_Address, uint16_t Register_Address);
    uint8_t I2C_ReadBlock(uint8_t Device_Address, uint16_t Register_Address, uint8_t* Data, uint8_t Size);

private:

    const uint8_t pin_sda;
    const uint8_t pin_scl;
    const uint8_t pin_int;
    const uint8_t pin_rst;
    const uint8_t dev_addr;

    Touch_Point points[5];

    cb_1_Point_Touched cb_1_Point = NULL;
    cb_2_Point_Touched cb_2_Point = NULL;
    cb_3_Point_Touched cb_3_Point = NULL;
    cb_4_Point_Touched cb_4_Point = NULL;
    cb_5_Point_Touched cb_5_Point = NULL;
};

#endif

