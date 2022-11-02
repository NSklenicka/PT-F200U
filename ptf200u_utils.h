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
 * ;
 * if(!device.SetInput(error, COMPUTER1))
 * {
 *      //handle error;
 * }
 */

typedef unsigned short U8;

class PTF200U_Utils
{
public:

    [[nodiscard]] static QStringList GetSerialPortNames();

    enum InputOption: U8
       {
        VIDEO,
        COMPUTER1,
        COMPUTER2,
        COMPONENT,
        SVIDEO
       };

    static QStringList InputOptionList();

    explicit PTF200U_Utils();
    ~PTF200U_Utils(){};

    void SetPortName(QString const& portName);

    bool PowerON(QString &error);

    bool PowerOFF(QString &error);

    //bool SetVolume(QString &error, U8 volume) const;

    bool SetInput(QString &error, InputOption inputOption);

    //static void ToggleShutter();

    //static LampCondition GetLampCondition();

private:

    std::unique_ptr<QSerialPort> m_serialPort;

    void SendCommand( QByteArray command, QByteArray parameter = QByteArray());

    bool WaitForResponse(QString & error);

};

#endif // PTF200U_UTILS_H

