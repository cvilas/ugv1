#include <QString>
#include <QtTest>
#include <IoBoardCommand.h>

class IoBoardTests : public QObject
{
    Q_OBJECT

public:
    IoBoardTests();
    
private Q_SLOTS:
    void test_IoBoardCommand_create();
    void test_IoBoardCommand_create_data();

    void test_IoBoardCommand_setServoMode();
};

IoBoardTests::IoBoardTests()
{
}

void IoBoardTests::test_IoBoardCommand_create()
{
    QFETCH(char, command);
    QFETCH(char, result);
    QCOMPARE(command, result);
}

void IoBoardTests::test_IoBoardCommand_create_data()
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

void IoBoardTests::test_IoBoardCommand_setServoMode()
{
    Ugv1::SetDIOServoModeCommand cmd;
    for(int i = 0; i < 7; ++i)
    {
        cmd.setServoMode(i, true);
        QVERIFY(cmd.isServoModeSet(i)==true);

        cmd.setServoMode(i, false);
        QVERIFY(cmd.isServoModeSet(i)==false);
    }
}

QTEST_APPLESS_MAIN(IoBoardTests)

#include "IoBoard_tests.moc"
