#include "ptf200u_utils.h"

#include <QDebug>
#include <QSerialPortInfo>

/*
 *
 * Computer 2 not working...
 *
 *
 */



PTF200U_Utils::PTF200U_Utils()
    : m_serialPort(std::make_unique<QSerialPort>())
{
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
}

PTF200U_Utils::~PTF200U_Utils()
{
    m_serialPort->close();
}

bool PTF200U_Utils::SetPortName(QString & error, const QString &portName)
{
    if (portName.isEmpty())
    {
        return true;
    }

    m_serialPort->close();
    m_serialPort->setPortName(portName);
    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        error = "Failed to open port " + m_serialPort->portName();
        return false;
    }

    return true;
}

bool PTF200U_Utils::cmd_PowerON(QString &error)
{
    SendCommand("PON");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_PowerOFF(QString &error)
{
    SendCommand("POF");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_SetInput(QString &error, InputOption inputOption)
{
    QByteArray parameter;

    switch(inputOption)
    {
        case VIDEO:
            parameter = "VID";
            break;
        case COMPUTER1:
            parameter = "RG1";
            break;
        case COMPUTER2:
            parameter = "RG2";
            break;
        case COMPONENT:
            parameter = "YUV";
            break;
        case SVIDEO:
            parameter = "SVD";
            break;
        default:
        {
            error = "option not supported.";
            m_serialPort->close();
            return false;
        }
    }

    SendCommand("IIS", parameter);
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_Menu(QString& error)
{
    SendCommand("OMN");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_UpKey(QString& error)
{
    SendCommand("OCU");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_Enter(QString& error)
{
    SendCommand("OEN");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_DownKey(QString& error)
{
    SendCommand("OCD");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_LeftKey(QString& error)
{
    SendCommand("OCL");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_RightKey(QString& error)
{
    SendCommand("OCR");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_VolumePlus(QString& error)
{
    SendCommand("AUU");
    return WaitForResponse(error);
}

bool PTF200U_Utils::cmd_VolumeMinus(QString& error)
{
    SendCommand("AUD");
    return WaitForResponse(error);
}

void PTF200U_Utils::SendCommand(QByteArray command)
{
    QByteArray data;
    data.append(0x02);
    data.append(command);
    data.append(0x03);

    qDebug() << "Writing: " << data;
    m_serialPort->write(data);
}

void PTF200U_Utils::SendCommand(QByteArray command, QByteArray parameter)
{
    QByteArray data;

    data.append(0x02);
    data.append(command);
    data.append(':');
    data.append(parameter);
    data.append(0x03);

    qDebug() << "Writing: " << data;
    m_serialPort->write(data);
}

bool PTF200U_Utils::WaitForResponse(QString & error)
{
    QByteArray static const ER401{ "\2ER401\3" };
    QByteArray static const ER402{ "\2ER402\3" };

    constexpr int timeoutms = 1500;
    QByteArray buffer;

    while(m_serialPort->waitForReadyRead(timeoutms))
    {        
        auto data{ m_serialPort->readAll() };
        qDebug() << "Read: " << data;
        buffer.append( data );
        if (data == "\x03")//end transmission
        {
            break;
        }
    }

    if (buffer.isEmpty())
    {
        error = QString("No respose from the device after %1ms.").arg(timeoutms);
        return false;
    }

    if (buffer == ER401)
    {
        error = QString("ER401: Commands cannot be accepted or the command does not exist.");
        return false;
    }

    if (buffer == ER402)
    {
        error = QString("ER402: Parameter error.");
        return false;
    }

    return true;
}

QStringList PTF200U_Utils::GetSerialPortNames()
{
    QList <QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QStringList portNameList;
    portNameList.reserve(portList.size());
    for(auto const& portInfo : portList)
    {
        portNameList << portInfo.portName();
    }
    return portNameList;
}

QStringList PTF200U_Utils::InputOptionList()
{
    return QStringList
       {
        "VIDEO",
        "COMPUTER1",
        "COMPUTER2",
        "COMPONENT",
        "SVIDEO"
       };
}