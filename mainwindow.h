#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ptf200u_utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onEveryMinute();

    void on_comboBoxPortNames_currentTextChanged(const QString &arg1);

    void on_button_ON_clicked();

    void on_button_OFF_clicked();

    void on_buttonSetInput_clicked();

    void on_buttonMenu_clicked();

    void on_buttonUp_clicked();

    void on_buttonSelect_clicked();

    void on_buttonDown_clicked();

    void on_butonResetTimer_clicked();

    void on_actionOpen_Settings_triggered();

private:
    Ui::MainWindow *ui;

    PTF200U_Utils m_device;

    int m_minutes;

    void SaveSettings();

};
#endif // MAINWINDOW_H
