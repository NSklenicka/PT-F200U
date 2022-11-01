#include "ptf200u_utils.h"

#include <QDebug>
#include <QSerialPortInfo>

PTF200U_Utils::PTF200U_Utils()
    : m_serialPort(std::make_unique<QSerialPort>())
{
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
}

void PTF200U_Utils::SetPortName(const QString &portName)
{
    m_serialPort->setPortName(portName);
}

bool PTF200U_Utils::PowerON(QString &error)
{
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        error = "Failed to open port " + m_serialPort->portName();
        return false;
    }

    SendCommand("PON");

    //might respond?
    if(m_serialPort->waitForReadyRead(500))
    {
        //get the response
        auto data = m_serialPort->readAll();
        error = "Transmission successful, got unexpected response: "+ data;
        m_serialPort->close();
        return false;
    }

    m_serialPort->close();
    return true;
}

bool PTF200U_Utils::PowerOFF(QString &error)
{
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        error = "Failed to open port " + m_serialPort->portName();
        return false;
    }

    SendCommand("POF");

    //might respond?
    if(m_serialPort->waitForReadyRead(500))
    {
        //get the response
        auto data = m_serialPort->readAll();
        error = "Transmission successful, got unexpected response: "+ data;
        m_serialPort->close();
        return false;
    }

    m_serialPort->close();
    return true;
}

bool PTF200U_Utils::SetInput(QString &error, InputOption inputOption)
{
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        error = "Failed to open port " + m_serialPort->portName();
        return false;
    }

    QByteArray command("IIS");
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

    SendCommand(command, parameter);

    //might respond?
    if(m_serialPort->waitForReadyRead(500))
    {
        //get the response
        auto data = m_serialPort->readAll();
        error = "Transmission successful, got unexpected response: "+ data;
        m_serialPort->close();
        return false;
    }

    m_serialPort->close();
    return true;
}

void PTF200U_Utils::SendCommand(QByteArray command, QByteArray parameter)
{
    //format data
    QByteArray data{"STX" + command + ":" + parameter + "ETX"};
    qDebug() << "Writing: " << data;
    m_serialPort->write(data);
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
