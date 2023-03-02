#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QSpinBox>
#include <QLineEdit>

//#include <string.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }//
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_released();                             //开始扫描

    bool check_area(uint32_t ,uint32_t ,uint16_t ,uint16_t );  //合法判断

    void scan(QString,uint32_t,uint8_t);                       //扫描

    void on_lineEdit_textEdited(const QString &arg1);          //

    void on_lineEdit_4_textEdited(const QString &arg1);        //

    void on_spinBox_2_valueChanged(int arg1);                  //

    void on_spinBox_3_valueChanged(int arg1);                  //

    void on_spinBox_4_valueChanged(int arg1);                  //

    void on_pushButton_2_released();                           //

    void on_pushButton_3_released();                           //

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
