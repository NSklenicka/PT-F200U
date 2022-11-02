#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>
#include <QStandardPaths>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>

#include <QDebug>

static const QString settingsPath{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/projector/settings.txt"};
#define DECLARE_SETTINGS QSettings settings(settingsPath, QSettings::IniFormat);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "settings path: " << settingsPath;

    DECLARE_SETTINGS
    restoreGeometry(settings.value("geometry").toByteArray());//window size, pos, maximized

    //set com port
    auto portNames{ m_device.GetSerialPortNames() };
    ui->comboBoxPortNames->addItems(portNames);
    auto lastPort = settings.value("port").toString();
    if ( (!lastPort.isEmpty()) && portNames.contains(lastPort))
    {
        ui->comboBoxPortNames->setCurrentIndex(ui->comboBoxPortNames->findText(lastPort));
    }

    //set input option
    auto inputOptions{ m_device.InputOptionList() };
    ui->comboBoxInput->addItems(inputOptions);
    auto lastInputIndex = settings.value("inputIndex").toInt();
    ui->comboBoxPortNames->setCurrentIndex(lastInputIndex);

    //start timer
    m_minutes = settings.value("timer", 60).toInt();
    ui->timerDisplay->display(m_minutes);
    QTimer* minuteTimer = new QTimer(this);
    minuteTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(minuteTimer, SIGNAL(timeout()), this, SLOT(onEveryMinute()));
    minuteTimer->start( 60 * 1000);//every minute
}

void MainWindow::SaveSettings()
{
    DECLARE_SETTINGS
    settings.setValue("geometry", saveGeometry());
    settings.setValue("port", ui->comboBoxPortNames->currentText());
    settings.setValue("inputIndex", ui->comboBoxInput->currentIndex());
    settings.setValue("timer", m_minutes);
}

MainWindow::~MainWindow()
{
    SaveSettings();
    delete ui;
}

void MainWindow::onEveryMinute()
{
    if (ui->timerDisplay->value() > 0)
    {
        ui->timerDisplay->display(ui->timerDisplay->value() - 1);
    }
    
    if (ui->timerDisplay->value() == 0)
    {
        QString error;
        if (!m_device.cmd_PowerOFF(error))
        {
            //an error occurred. I guess just try again in another minute.
        }
        else
        {
            QApplication::quit();
        }
    }
}

void MainWindow::on_comboBoxPortNames_currentTextChanged(const QString &arg1)
{
    QString error;
    if (!m_device.SetPortName(error, arg1))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_button_ON_clicked()
{
    QString error;
    if(!m_device.cmd_PowerON(error))
    {
        QMessageBox::critical(this, "Error", error);
    }

}

void MainWindow::on_button_OFF_clicked()
{
    QString error;
    if(!m_device.cmd_PowerOFF(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_buttonSetInput_clicked()
{
    QString error;
    if (!m_device.cmd_SetInput(error, (PTF200U_Utils::InputOption)ui->comboBoxInput->currentIndex()))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_buttonMenu_clicked()
{
    QString error;
    if (!m_device.cmd_Menu(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_buttonUp_clicked()
{
    QString error;
    if (!m_device.cmd_UpKey(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_buttonSelect_clicked()
{
    QString error;
    if (!m_device.cmd_Enter(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_buttonDown_clicked()
{
    QString error;
    if (!m_device.cmd_DownKey(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::on_butonResetTimer_clicked()
{
    ui->timerDisplay->display(m_minutes);
}

void MainWindow::on_actionOpen_Settings_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(settingsPath));
}

