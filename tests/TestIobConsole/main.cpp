//==============================================================================
// Project  : UGV1
// Module   : TestConsole
// File     : main.cpp
// Brief    : Console based test program for IoBoard
//==============================================================================

#include "Controller/IoBoard.h"
#include "io/SerialPort.h"
#include <iostream>

//==============================================================================
int main(int argc, char *argv[])
//==============================================================================
{
    Grape::SerialPort port;
    Ugv1::IoBoard iob(port);

    try
    {

        iob.setResponseTimeOut(5000);
        port.setPortName("/dev/ttyUSB0");
        port.open();
        port.setDataFormat(Grape::SerialPort::D8N1);
        port.setBaudRate(Grape::SerialPort::B115200);

        // version
        Ugv1::ReadBoardVersionResponse version;
        iob.getVersion(version);
        std::cout << "version: " << version.getBoardCode() << ":" << version.getBoardVersion() << ":" << version.getBoardRevision() << std::endl;

        // Analog In
        Ugv1::ReadAnalogInResponse analog;
        iob.getAnalog(analog);
        std::cout << "analog: ";
        for(int i = 0; i < 8; ++i)
        {
            std::cout << analog.getVolts(i) << " ";
        }
        std::cout << std::endl;

        // Encoders
        Ugv1::ReadMotorEncodersResponse encoders;
        iob.getMotorEncoders(encoders);
        std::cout << "Encoders: " << encoders.getEncoder(0) << " " << encoders.getEncoder(1) << std::endl;

        // speed
        Ugv1::ReadMotorSpeedResponse speed;
        iob.getMotorSpeed(speed);
        std::cout << "Speed: " << speed.getMotorSpeed(0) << " " << speed.getMotorSpeed(1) << std::endl;

        for(int i = 0; i < encoders.size(); ++i)
        {
            std::cout << std::hex << (int)(encoders[i]&0xFF) << " ";
        }
        std::cout << std::endl;
    }
    catch(Grape::Exception& ex )
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
