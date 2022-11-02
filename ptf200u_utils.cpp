#include "ptf200u_utils.h"

#include <QDebug>
#include <QSerialPortInfo>

/*
 *
 * Computer 2 not working...
 *
 *
 * */
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

    return WaitForResponse(error);
}

bool PTF200U_Utils::PowerOFF(QString &error)
{
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        error = "Failed to open port " + m_serialPort->portName();
        return false;
    }

    SendCommand("POF");

    return WaitForResponse(error);
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

    return WaitForResponse(error);
}

void PTF200U_Utils::SendCommand(QByteArray command, QByteArray parameter)
{
    //format data
    QByteArray data;
    data.append(0x02);
    data.append(command);

    if(!parameter.isEmpty())
    {
        data.append(':');
        data.append(parameter);
    }

    data.append(0x03);

    qDebug() << "Writing: " << data;
    auto wrote = m_serialPort->write(data);
    //qDebug() << "Number bytes wrote: " << wrote;
}

bool PTF200U_Utils::WaitForResponse(QString & error)
{
    bool ret = false;
    int timeoutms = 3000;

    if(m_serialPort->waitForReadyRead(timeoutms))
    {
        //get the response
        QByteArray expected ("\x02");
        auto data = m_serialPort->readAll();
        qDebug() << "Read: " << data;
        if(data == expected)
        //if(true)
        {
            ret = true;
        }
        else
        {
           error = QString("Response: %1, expected %2!").arg((QString)data).arg((QString)expected);
           ret = false;
        }

    }
    else
    {
        error = "Timeout after ms " + QString::number(timeoutms);
        ret = false;
    }

    m_serialPort->close();
    return ret;
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
