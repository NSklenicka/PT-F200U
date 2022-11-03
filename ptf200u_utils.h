#ifndef PTF200U_UTILS_H
#define PTF200U_UTILS_H

#include <QStringList>
#include <QSerialPort>

/*
 * class for Panasonic PT-F200U projector with QSerialPort
 *
 * Usage:
 *
 * QString portName = PTF200U_Utils::GetSerialPortNames().first();
 *
 * PTF200U_Utils device;
 * QString error;
 * device.SetPortName(portName);
 * if(!device.PowerON(error))
 * {
 *      //handle error;
 * }
 *
 */


class PTF200U_Utils
{
public:

    [[nodiscard]] static QStringList GetSerialPortNames();

    enum InputOption: uint8_t
       {
        VIDEO,
        COMPUTER1,
        COMPUTER2,
        COMPONENT,
        SVIDEO
       };

    [[nodiscard]] static QStringList InputOptionList();

    explicit PTF200U_Utils();
    ~PTF200U_Utils();

    bool SetPortName(QString & error, QString const& portName);

    bool cmd_PowerON(QString &error);

    bool cmd_PowerOFF(QString &error);

    bool cmd_SetInput(QString& error, InputOption inputOption);

    bool cmd_Menu(QString& error);

    bool cmd_Enter(QString& error);

    bool cmd_UpKey(QString& error);

    bool cmd_DownKey(QString& error);

    bool cmd_LeftKey(QString& error);

    bool cmd_RightKey(QString& error);

    bool cmd_VolumePlus(QString& error);

    bool cmd_VolumeMinus(QString& error);

private:

    std::unique_ptr<QSerialPort> m_serialPort;

    void SendCommand( QByteArray command);
    void SendCommand(QByteArray command, QByteArray parameter);

    bool WaitForResponse(QString & error);

};

#endif // PTF200U_UTILS_H

