#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //populate com ports
    ui->comboBoxPortNames->addItems(m_device.GetSerialPortNames());

    //populate input options
    ui->comboBoxInput->addItems(m_device.InputOptionList());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBoxPortNames_currentTextChanged(const QString &arg1)
{
    m_device.SetPortName(arg1);
}





void MainWindow::on_button_ON_clicked()
{
    QString error;
    if(!m_device.PowerON(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}


void MainWindow::on_button_OFF_clicked()
{
    QString error;
    if(!m_device.PowerOFF(error))
    {
        QMessageBox::critical(this, "Error", error);
    }
}


void MainWindow::on_pushButton_clicked()
{
    QString error;
    if(!m_device.SetInput(error, (PTF200U_Utils::InputOption) ui->comboBoxInput->currentIndex()))
    {
        QMessageBox::critical(this, "Error", error);
    }
}

