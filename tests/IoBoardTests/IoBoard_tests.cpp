#include <QString>
#include <QtTest>
#include <IoBoardCommand_servo.h>
#include <IoBoardCommand_dio.h>

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
    void test_IoBoardCommand_setDioOutMode();
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

    char SetServoModeCommand_result[7] = {0x55, 0xaa, 0x10, 0x01, 0x01, 0x7F, 0x90};
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
        QVERIFY2((char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setServoMode(true)");
    }

    // and toggle them back to false, one at a time
    csum = *(cmd.end()-1);
    for(int i = 0; i < 7; ++i)
    {
        cmd.setModeServo(i, false);
        QVERIFY2(!cmd.isModeServo(i), "set/get failed for setServoMode(false)");
        csum -= (1<<i);
        QVERIFY2((char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setServoMode(true)");
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
        cmd.setModeOutput(i, true);
        QVERIFY2(cmd.isModeOutput(i), "set/get failed for setOutputMode(true)");
        csum += (1<<(i%8));
        QVERIFY2((char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setOutputMode(true)");
    }

    // and toggle them back to false (equivalent to setting inputmode), one at a time
    csum = *(cmd.end()-1);
    for(int i = 0; i < 7; ++i)
    {
        cmd.setModeInput(i, true);
        QVERIFY2(cmd.isModeInput(i), "set/get failed for setInputMode(true)");
        csum -= (1<<(i%8));
        QVERIFY2((char)(csum&0xFF) == *(cmd.end()-1), "checksum incorrect after setInputMode(true)");
    }
}

QTEST_APPLESS_MAIN(IoBoardMessageTests)

#include "IoBoard_tests.moc"
