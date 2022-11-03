#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ptf200u_utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTimer;

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

    void on_checkBoxTimerEnable_toggled(bool checked);

    void on_buttonLeft_clicked();

    void on_buttonRight_clicked();

    void on_buttonVolumeUp_clicked();

    void on_buttonVolumeDown_clicked();

private:
    Ui::MainWindow *ui;

    PTF200U_Utils m_device;

    int m_minutes;

    void SaveSettings();

    QTimer* m_timer;

    void StartTimer();
    void StopTimer();

    QString error;

};
#endif // MAINWINDOW_H
