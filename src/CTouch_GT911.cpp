#include "CTouch_GT911.h"

#include <Wire.h>

GT911::GT911(uint8_t C_SDA, uint8_t C_SCL, uint8_t C_INT, uint8_t C_RST, uint8_t I2C_ADDR)
	: pin_sda(C_SDA), pin_scl(C_SCL), pin_int(C_INT), pin_rst(C_RST), dev_addr(I2C_ADDR)
{
    _screenWidth = 320;
    _screenHeight = 480;
    _screenOrientation = 0;
}

void GT911::init(isr_TouchDetected onTouchDetected)
{
    delay(1);   // let the system to stabilise (rise of AVDD and VDDIO).
    
    pinMode(pin_int, OUTPUT);
    pinMode(pin_rst, OUTPUT);

    digitalWrite(pin_int, LOW);
    digitalWrite(pin_rst, LOW);

    delay(11);                  // wait more than 10mS

    digitalWrite(pin_int, dev_addr == GT911_ADDR2);

    delayMicroseconds(110);     // wait more than 100uS

    digitalWrite(pin_rst, HIGH);

    delay(6);                   // wait more than 5mS

    digitalWrite(pin_int, LOW);

    delay(55);                  // wait more than 50mS ...
    
    pinMode(pin_int, INPUT);

    delay(100);

    if (onTouchDetected != NULL)
    {
        attachInterrupt(digitalPinToInterrupt(pin_int), onTouchDetected, FALLING);     // Interrupt FROM GT911.
    }

    Wire.setClock(400000);          // I2C port speed.
    Wire.begin(pin_sda, pin_scl);   // I2C port start.
}

void GT911::setDimensions(uint16_t screenWidth, uint16_t screenHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
}

void GT911::setRotation(uint8_t screenOrientation)
{
    if (screenOrientation > 3) return;

    _screenOrientation = screenOrientation;
}

void GT911::process()
{
    uint8_t status = I2C_ReadByte(dev_addr, GT911_POINTS_INFO);

    if (status & 0b10000000)
    {
        uint8_t touches = status & 0xF;

        for (uint8_t i = 0; i < touches; i++)
        {
            uint8_t data[7];
            I2C_ReadBlock(dev_addr, GT911_POINTS_DATA + i * 8, data, 7);
            points[i] = parsePoint(data);
        }

        switch (touches)
        {
        case 1:
            if (cb_1_Point) cb_1_Point(points[0]);
            break;
        case 2:
            if (cb_2_Point) cb_2_Point(points[0], points[1]);
            break;
        case 3:
            if (cb_3_Point) cb_3_Point(points[0], points[1], points[2]);
            break;
        case 4:
            if (cb_4_Point) cb_4_Point(points[0], points[1], points[2], points[3]);
            break;
        case 5:
            if (cb_5_Point) cb_5_Point(points[0], points[1], points[2], points[3], points[4]);
            break;
        default:
            break;
        }
    }

    I2C_SendByte(dev_addr, 0x814E, 0x00);
}

void GT911::setCallback_1_PointTouched(cb_1_Point_Touched cb)
{
    cb_1_Point = cb;
}

void GT911::setCallback_2_PointTouched(cb_2_Point_Touched cb)
{
    cb_2_Point = cb;
}

void GT911::setCallback_3_PointTouched(cb_3_Point_Touched cb)
{
    cb_3_Point = cb;
}

void GT911::setCallback_4_PointTouched(cb_4_Point_Touched cb)
{
    cb_4_Point = cb;
}

void GT911::setCallback_5_PointTouched(cb_5_Point_Touched cb)
{
    cb_5_Point = cb;
}

Touch_Point GT911::parsePoint(uint8_t* data)
{
    uint8_t id = data[0];
    uint16_t x = data[1] + (data[2] << 8);
    uint16_t y = data[3] + (data[4] << 8);
    uint16_t size = data[5] + (data[6] << 8);

    uint16_t temp;

    switch (_screenOrientation)
    {
    case 0: // Portrait
        // Do nothing, coordinates are already in the correct orientation
        break;
    case 1: // Landscape
        // Swap x and y coordinates and invert the x-axis
        temp = x;
        x = y;
        y = _screenWidth - temp;
        break;
    case 2: // Portrait upside down
        // Invert x and y coordinates
        x = _screenWidth - x;
        y = _screenHeight - y;
        break;
    case 3: // Landscape (flipped)
        // Swap x and y coordinates and invert the y-axis
        temp = x;
        x = _screenHeight - y;
        y = temp;
        break;
    default:
        // Invalid screen orientation
        break;
    }

    return Touch_Point(id, x, y, size);
}

uint8_t GT911::I2C_SendByte(uint8_t Device_Address, uint16_t Register_Address, uint8_t Data)
{
    Wire.beginTransmission(Device_Address);
    Wire.write(highByte(Register_Address));
    Wire.write(lowByte(Register_Address));
    Wire.write(Data);
    return Wire.endTransmission();
}

uint8_t GT911::I2C_SendBlock(uint8_t Device_Address, uint16_t Register_Address, uint8_t* Data, uint8_t Size)
{
    Wire.beginTransmission(Device_Address);
    Wire.write(highByte(Register_Address));
    Wire.write(lowByte(Register_Address));
    Wire.write(Data, Size);
    return Wire.endTransmission();
}

uint8_t GT911::I2C_ReadByte(uint8_t Device_Address, uint16_t Register_Address)
{
    Wire.beginTransmission(Device_Address);
    Wire.write(highByte(Register_Address));
    Wire.write(lowByte(Register_Address));
    Wire.endTransmission();
    Wire.requestFrom(Device_Address, (uint8_t) 1);
    return Wire.read();
}

uint8_t GT911::I2C_ReadBlock(uint8_t Device_Address, uint16_t Register_Address, uint8_t* Data, uint8_t Size)
{
    Wire.beginTransmission(Device_Address);
    Wire.write(highByte(Register_Address));
    Wire.write(lowByte(Register_Address));
    Wire.endTransmission();
    Wire.requestFrom(Device_Address, Size);

    uint8_t receivedBytes = 0;

    while (receivedBytes < Size && Wire.available())
        Data[receivedBytes++] = Wire.read();

    return receivedBytes;
}

