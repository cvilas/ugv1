//==============================================================================
// Project  : UGV1
// Module   : TestConsole
// File     : main.cpp
// Brief    : Console based test program for IoBoard
//==============================================================================

#include "IoBoard.h"
#include "io/SerialPort.h"
#include <iostream>

//==============================================================================
int main(int argc, char *argv[])
//==============================================================================
{
    Grape::SerialPort port;
    Ugv1::IoBoard iob(port);

    iob.setResponseTimeOut(5000);
    port.setPortName("/dev/ttyUSB0");

    if( !port.open() )
    {
        int code;
        std::cerr << port.getLastError(code) << std::endl;
        return -1;
    }

    if( !port.setDataFormat(Grape::SerialPort::D8N1) )
    {
        int code;
        std::cerr << port.getLastError(code) << std::endl;
        return -1;
    }

    if( !port.setBaudRate(Grape::SerialPort::B115200) )
    {
        int code;
        std::cerr << port.getLastError(code) << std::endl;
        return -1;
    }

    // version
    Ugv1::ReadBoardVersionResponse version;
    if( iob.getVersion(version) && version.isValid() )
    {
        std::cout << "version: " << version.getBoardCode() << ":" << version.getBoardVersion() << ":" << version.getBoardRevision() << std::endl;
    }
    else
    {
        int code;
        std::cerr << iob.getLastError(code) << std::endl;
    }

    // Analog In
    Ugv1::ReadAnalogInResponse analog;
    if( iob.getAnalog(analog) && analog.isValid() )
    {
        std::cout << "analog: ";
        for(int i = 0; i < 8; ++i)
        {
            std::cout << analog.getVolts(i) << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        int code;
        std::cerr << iob.getLastError(code) << std::endl;
    }

    // Encoders
    Ugv1::ReadMotorEncodersResponse encoders;
    if( iob.getMotorEncoders(encoders) )
    {
        std::cout << "Encoders: " << encoders.getEncoder(0) << " " << encoders.getEncoder(1) << std::endl;
    }
    else
    {
        int code;
        std::cerr << iob.getLastError(code) << std::endl;
    }

    // speed
    Ugv1::ReadMotorSpeedResponse speed;
    if( iob.getMotorSpeed(speed) )
    {
        std::cout << "Speed: " << speed.getMotorSpeed(0) << " " << speed.getMotorSpeed(1) << std::endl;
    }
    else
    {
        int code;
        std::cerr << iob.getLastError(code) << std::endl;
    }
/*
    for(int i = 0; i < encoders.size(); ++i)
    {
        std::cout << std::hex << (int)(encoders[i]&0xFF) << " ";
    }
    std::cout << std::endl;
*/
    return 0;
}
