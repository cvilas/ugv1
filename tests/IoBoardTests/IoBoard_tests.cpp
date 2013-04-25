#include <QString>
#include <QtTest>
#include <Vehicle/IoBoardMessage_servo.h>
#include <Vehicle/IoBoardMessage_dio.h>
#include <Vehicle/IoBoardMessage_motor.h>

//=============================================================================
/// \class Test class for IoBoard messages
//=============================================================================
class IoBoardMessageTests : public QObject
{
    Q_OBJECT

public:
    IoBoardMessageTests();
    
private Q_SLOTS:
    void test_IoBoardCommand_format();
    void test_IoBoardCommand_setServoMode();
    void test_IoBoardCommand_writeServoOut();
    void test_IoBoardCommand_setDioOutMode();
    void test_IoBoardCommand_setMotorParams();
    void test_IoBoardCommand_setMotorSpeed();
    void test_IoBoardCommand_setMotorPIDGains();
    void test_IoBoardCommand_setMotorDriveMode();
    void test_IoBoardCommand_setMotorPower();

    void test_IoBoardResponse_validate();
};

//=============================================================================
IoBoardMessageTests::IoBoardMessageTests()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_format()
//-----------------------------------------------------------------------------
{
    // Test that command is formatted properly

    unsigned char SetServoModeCommand_result[7] = {0x55, 0xaa, 0x10, 0x01, 0x01, 0x7F, 0x90};
    Ugv1::SetDioServoModeCommand SetServoModeCommand_command(0x7F);
    for(int i = 0; i < 7; ++i)
    {
        QVERIFY(SetServoModeCommand_command[i] == SetServoModeCommand_result[i]);
    }
}

/*
void IoBoardMessageTests::test_IoBoardCommand_create()
{
    QFETCH(char, command);
    QFETCH(char, result);
    QCOMPARE(command, result);
}

void IoBoardMessageTests::test_IoBoardCommand_create_data()
{
    QTest::addColumn<char>("command");
    QTest::addColumn<char>("result");

    char SetServoModeCommand_result[7] = {0x55, 0xaa, 0x10, 0x01, 0x01, 0x7F, 0x90};
    Ugv1::SetDIOServoModeCommand SetServoModeCommand_command(0x7F);

    QTest::newRow("SetDIOServoModeCommand_header0") << SetServoModeCommand_command[0] << SetServoModeCommand_result[0];
    QTest::newRow("SetDIOServoModeCommand_header1") << SetServoModeCommand_command[1] << SetServoModeCommand_result[1];
    QTest::newRow("SetDIOServoModeCommand_header2") << SetServoModeCommand_command[2] << SetServoModeCommand_result[2];
    QTest::newRow("SetDIOServoModeCommand_payloadLength") << SetServoModeCommand_command[3] << SetServoModeCommand_result[3];
    QTest::newRow("SetDIOServoModeCommand_command") << SetServoModeCommand_command[4] << SetServoModeCommand_result[4];
    QTest::newRow("SetDIOServoModeCommand_payload") << SetServoModeCommand_command[5] << SetServoModeCommand_result[5];
    QTest::newRow("SetDIOServoModeCommand_checksum") << SetServoModeCommand_command[6] << SetServoModeCommand_result[6];
}
*/

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setServoMode()
//-----------------------------------------------------------------------------
{
    // 1. Test get/set servo mode message
    // 2. Test that command buffer is updated as expected when it is modified

    Ugv1::SetDioServoModeCommand cmd;   // default command
    unsigned int csum = *(cmd.end()-1); // check sum as is

    // toggle each bit to true one at a time
    for(int i = 0; i < 7; ++i)
    {
        cmd.setModeServo(i, true);
        QVERIFY2(cmd.isModeServo(i), "set/get failed for setServoMode(true)");
        csum += (1<<i);
        QVERIFY2((unsigned char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setServoMode(true)");
    }

    // and toggle them back to false, one at a time
    csum = *(cmd.end()-1);
    for(int i = 0; i < 7; ++i)
    {
        cmd.setModeServo(i, false);
        QVERIFY2(!cmd.isModeServo(i), "set/get failed for setServoMode(false)");
        csum -= (1<<i);
        QVERIFY2((unsigned char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setServoMode(true)");
    }
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_writeServoOut()
//-----------------------------------------------------------------------------
{
    // 1. Set a servo output speed and position in command
    // 2. Verify it is set
    for(int i = 0; i < 7; ++i)
    {
        Ugv1::WriteServoOutCommand cmd;
        unsigned char pos = (unsigned char)((double)qrand()*180.0/RAND_MAX);
        unsigned char speed = (unsigned char)((double)qrand()*255/RAND_MAX);
        unsigned char pos2, speed2;
        cmd.setChannel(i, pos, speed);
        pos2 = cmd.getPosition(i);
        speed2 = cmd.getSpeed(i);
        QVERIFY2(pos == pos2, "Position set failed");
        QVERIFY2(speed == speed2, "Speed set failed");
    }
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setDioOutMode()
//-----------------------------------------------------------------------------
{
    // 1. Test get/set digital io mode message
    // 2. Test that command buffer is updated as expected when it is modified

    Ugv1::SetDioIoModeCommand cmd;
    unsigned int csum = *(cmd.end()-1); // check sum as is

    // toggle each bit to true one at a time
    for(int i = 0; i < 11; ++i)
    {
        cmd.setModeInput(i, true);
        QVERIFY2(cmd.isModeInput(i), "set/get failed for setOutputMode(true)");
        csum += (1<<(i%8));
        QVERIFY2((unsigned char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setOutputMode(true) on toggle up");
    }

    // and toggle them back to false (equivalent to setting inputmode), one at a time
    csum = *(cmd.end()-1);
    for(int i = 0; i < 7; ++i)
    {
        cmd.setModeOutput(i, true);
        QVERIFY2(cmd.isModeOutput(i), "set/get failed for setInputMode(true)");
        csum -= (1<<(i%8));
        QVERIFY2((unsigned char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setInputMode(true) on toggle down");
    }
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setMotorParams()
//-----------------------------------------------------------------------------
{
    // 1. Set/verify encoder ppr
    // 2. set/verify gear ratio
    // 3. set/verify perimenter

    Ugv1::SetMotorParametersCommand cmd;

    cmd.setEncoderPPR(200);
    QVERIFY2(200 == cmd.getEncoderPPR(), "Encoder PPR get/set failed");

    cmd.setGearRatio(800);
    QVERIFY2(800 == cmd.getGearRatio(), "Gear ratio get/set failed");

    cmd.setWheelPerimeter(1000);
    QVERIFY2(1000 == cmd.getWheelPerimeter(), "Wheel perimeter get/set failed");
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setMotorSpeed()
//-----------------------------------------------------------------------------
{
    // 1. set/verify motor 1 speed
    // 2. set/verify motor 2 speed

    int speed1 = 600;
    int speed2 = -800;
    Ugv1::WriteMotorSpeedCommand cmd;

    cmd.setSpeed(0, speed1);
    QVERIFY2(speed1 == cmd.getSpeed(0), "Set/get fwd speed on motor 1 failed");

    cmd.setSpeed(0, speed2);
    QVERIFY2(speed2 == cmd.getSpeed(0), "Set/get rev speed on motor 1 failed");

    cmd.setSpeed(1, speed1);
    QVERIFY2(speed1 == cmd.getSpeed(1), "Set/get fwd speed on motor 2 failed");

    cmd.setSpeed(1, speed2);
    QVERIFY2(speed2 == cmd.getSpeed(1), "Set/get rev speed on motor 2 failed");
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setMotorPIDGains()
//-----------------------------------------------------------------------------
{
    Ugv1::SetMotorPidGainsCommand cmd;

    cmd.setProportionalGain(100);
    QVERIFY2(100 == cmd.getProportionalGain(), "set/get prop gain failed");

    cmd.setDerivativeGain(200);
    QVERIFY2(200 == cmd.getDerivativeGain(), "set/get derivative gain failed");

    cmd.setIntegralGain(50);
    QVERIFY2(50 == cmd.getIntegralGain(), "set/get integral gain failed");
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setMotorDriveMode()
//-----------------------------------------------------------------------------
{
    Ugv1::SetMotorDriveModeCommand cmd;

    cmd.setModeSpeedControl();
    QVERIFY2(cmd.isModeSpeedControl(), "set/get pid control failed");
    QVERIFY2(!cmd.isModeDirectPower(), "incorrect mode set (expected pid mode)");

    cmd.setModeDirectPower();
    QVERIFY2(cmd.isModeDirectPower(), "set/get direct control failed");
    QVERIFY2(!cmd.isModeSpeedControl(), "incorrect mode set (expected direct mode)");
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardCommand_setMotorPower()
//-----------------------------------------------------------------------------
{
    Ugv1::WriteMotorPowerCommand cmd;

    int power[5] = {-100, -50, 0, 50, 100};
    for(int i = 0; i < 5; ++i)
    {
        cmd.setPower(0, power[i]);
        QVERIFY2(cmd.getPower(0) == power[i], "set/get power on motor 1 failed");
    }
    for(int i = 0; i < 5; ++i)
    {
        cmd.setPower(1, power[i]);
        QVERIFY2(cmd.getPower(1) == power[i], "set/get power on motor 2 failed");
    }
}

//-----------------------------------------------------------------------------
void IoBoardMessageTests::test_IoBoardResponse_validate()
//-----------------------------------------------------------------------------
{
    // 1. Create valid message. Verify validate passes
    // 2. Create invalid message. Verify validate fails.

    // valid message
    unsigned char validResponse[9] = {0x55, 0xAA, 0x10, 0x02, Ugv1::IoBoardMessage::READ_DIO, 0x07, 0xFF, 0x1B, 0x0A};
    Ugv1::ReadDioInResponse resp;
    resp.assign(validResponse, validResponse+9);
    try
    {
        resp.validate();
    }
    catch( Ugv1::VehicleException& ex )
    {
        qDebug() << ex.what();
        QFAIL("Valid message but test failed");
    }

    // invalid length
    unsigned char invalidResponse1[9] = {0x55, 0xAA, 0x10, 0x02, Ugv1::IoBoardMessage::READ_DIO, 0x07, 0xFF, 0x1B};
    Ugv1::ReadDioInResponse resp1;
    resp1.assign(invalidResponse1, invalidResponse1+8);
    try
    {
        resp1.validate();
        QFAIL("Invalid message (length) but test failed");
    }
    catch(Ugv1::VehicleException& ex)
    {
        qDebug() << ex.what();
    }

    // invalid header
    unsigned char invalidResponse2[9] = {0x54, 0xAA, 0x10, 0x02, Ugv1::IoBoardMessage::READ_DIO, 0x07, 0xFF, 0x1B, 0x0A};
    Ugv1::ReadDioInResponse resp2;
    resp2.assign(invalidResponse2, invalidResponse2+9);
    try
    {
        resp2.validate();
        QFAIL("Invalid message (header) but test failed");
    }
    catch(Ugv1::VehicleException& ex)
    {
        qDebug() << ex.what();
    }

    // invalid id
    unsigned char invalidResponse3[9] = {0x55, 0xAA, 0x10, 0x02, Ugv1::IoBoardMessage::READ_ANALOG, 0x07, 0xFF, 0x1B, 0x0A};
    Ugv1::ReadDioInResponse resp3;
    resp3.assign(invalidResponse3, invalidResponse3+9);
    try
    {
        resp3.validate();
        QFAIL("Invalid message (id) but test failed");
    }
    catch(Ugv1::VehicleException& ex)
    {
        qDebug() << ex.what();
    }

    // invalid checksum
    unsigned char invalidResponse4[9] = {0x55, 0xAA, 0x10, 0x02, Ugv1::IoBoardMessage::READ_DIO, 0x07, 0xFF, 0x10, 0x0A};
    Ugv1::ReadDioInResponse resp4;
    resp4.assign(invalidResponse4, invalidResponse4+9);
    try
    {
        resp4.validate();
        QFAIL("Invalid message (checksum) but test failed");
    }
    catch(Ugv1::VehicleException& ex)
    {
        qDebug() << ex.what();
    }
}

QTEST_APPLESS_MAIN(IoBoardMessageTests)

#include "IoBoard_tests.moc"
