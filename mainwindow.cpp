#include "mainwindow.h"
#include "ui_mainwindow.h"

QString  Start_ip;                   //创建起始IP地址字符串变量
QString  End_ip;                     //创建结束IP地址字符串变量
QString  current_ip;                 //创建当前IP地址字符串变量

uint16_t Start_ipdata[4];
uint16_t End_ipdata[4];
uint16_t current_ipdata[4];

uint32_t Start_port;                 //创建起始端口变量
uint32_t End_port;                   //创建结束端口变量
uint32_t current_port;               //创建当前TCP端口变量

uint16_t wait_time_ms;               //创建一个超时变量

uint16_t start_time;
uint16_t end_time;

bool mutex=true;

QTextEdit *textEdit; //创建一个窗口对象
QSpinBox *spinBox_2; //创建一个数据框对象
QSpinBox *spinBox_3; //创建一个数据框对象
QSpinBox *spinBox_4; //创建一个数据框对象
QProgressBar *progressBar;                       //创建进度条对象
QProgressBar *progressBar_2;                   //创建进度条对象
QLabel *label_10;                      //当前ip显示
QLabel *label_11;

QTcpSocket socket(0);  //创建一个QTcpSocket类的socket(0)对象


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::check_area(uint32_t star_ip,uint32_t end_ip,uint16_t star_port,uint16_t end_port)
{
    bool ok=true;
    for(int i=0;i<4;i++)
    {
        if(Start_ipdata[i]>0xFF || End_ipdata[i]>0xFF)  //判断ip地址是否超出8位
        {
             this->ui->textEdit->QTextEdit::append("ERROR:IP ILLEGAL");
            ok=false;
        }
    }

    if(star_ip>end_ip)
    {
        this->ui->textEdit->QTextEdit::append("ERROR:IP RANGE WRONG");
        ok=false;
    }

    else if(star_port > end_port)  //判断端口域
    {
        this->ui->textEdit->QTextEdit::append("ERROR:PORT RANGE WRONG");
        ok=false;
    }
    return ok;
}

void MainWindow::scan(QString ip,uint32_t port_number,uint8_t overtime)
{
    this->ui->label_10->setText(ip);//下方的ip显示
    this->ui->label_11->setText(QString::number(port_number));//下方的端口显示
    socket.abort();                  //退出当前连接并重设接口
    socket.connectToHost(ip,port_number);
    int8_t soctype = socket.socketType();  //检查端口类型
    QString socType[3]={" TCP"," UDP"," SCTP"}; //端口类型列表
    if(socket.waitForConnected(overtime))
    {
        qDebug()<<ip<<":"<<port_number<<"established";

        QString display = ip;  //附上当前ip地址
        display.append(":");
        display.append(QString::number(port_number));   //附上当前端口号
        display.append(socType[soctype]);   //附上当前端口的类型
        display.append(" established");
        this->ui->textEdit->QTextEdit::append(display);
    }
    else
    {
        qDebug()<<ip<<":"<<port_number<<"Silence!";   //如果没有连接则输出静默
    }
}

void MainWindow::on_pushButton_released()    //开始扫描按钮
{
    start_time = clock();//时间记录
    this->ui->textEdit->QTextEdit::clear();  //清空窗口内容
    qDebug() << "Scaning...";   //程序输出一个debug信息表示开始扫描
    mutex=true;
    Start_ip = this->ui->lineEdit->text();
    End_ip = this->ui->lineEdit_4->text();
    Start_port =  this->ui->spinBox_3->value();
    End_port   =  this->ui->spinBox_4->value();
    wait_time_ms = this->ui->spinBox_2->value();

    QStringList Start_ipdata_list = Start_ip.split(".");//拆分
    QStringList End_iplist =End_ip.split(".");
    for(int i=0;i<4;i++)
    {
        Start_ipdata[i]=Start_ipdata_list[i].toInt();//移动
        End_ipdata[i]=End_iplist[i].toInt();
    }

    uint32_t stat_ip=(Start_ipdata[0]<<24)+(Start_ipdata[1]<<16)+(Start_ipdata[2]<<8)+Start_ipdata[3];// 合成ip
    uint32_t end_ip=(End_ipdata[0]<<24)+(End_ipdata[1]<<16)+(End_ipdata[2]<<8)+End_ipdata[3];
    uint32_t crt_ip=stat_ip;

    this->ui->progressBar->QProgressBar::setRange(Start_port,End_port);//进度条设置
    this->ui->progressBar_2->QProgressBar::setRange(stat_ip,end_ip);

    if(check_area(stat_ip,end_ip,Start_port,End_port))//合法
    { for(int i=0;i<4;i++) {current_ipdata[i]=Start_ipdata[i];}
        while(crt_ip<=end_ip)
        {
            if(current_ipdata[3]>0xFF) {current_ipdata[2]++;current_ipdata[3]=0;} //进位
            if(current_ipdata[2]>0xFF) {current_ipdata[1]++;current_ipdata[2]=0;}
            if(current_ipdata[1]>0xFF) {current_ipdata[0]++;current_ipdata[1]=0;}

            for(current_port=Start_port ; current_port<=End_port ; current_port ++)
            {
                while(!mutex){qApp->processEvents();}
                current_ip=QString("%1").arg(current_ipdata[0])
                        +QString(".")+QString("%1").arg(current_ipdata[1])
                        +QString(".")+QString("%1").arg(current_ipdata[2])
                        +QString(".")+QString("%1").arg(current_ipdata[3]);
                scan(current_ip,current_port,wait_time_ms);
                this->ui->progressBar->QProgressBar::setValue(current_port);
                this->ui->progressBar_2->QProgressBar::setValue(crt_ip);
                qApp->processEvents();   //在单线程程序中防止操作系统报程序未响应
            }
            current_ipdata[3]++;
            crt_ip++;
        }
        end_time=clock();    //标记扫描时间
        end_time -= start_time;  //计算扫描时间
        this->ui->textEdit->QTextEdit::append("THE SCANNING ENDS,TIME USED:"+QString::number(end_time)+"ms");

    }

}

void MainWindow::on_pushButton_2_released()   //clear display
{
    this->ui->textEdit->QTextEdit::clear();
    this->ui->progressBar->QProgressBar::setValue(0);
    this->ui->progressBar_2->QProgressBar::setValue(0);
    qDebug()<<"clear data";
}


void MainWindow::on_pushButton_3_released()   //暂停/继续按钮
{
    mutex=!mutex;
    if(mutex){ui->pushButton_3->setText("Pause");}
    else{ui->pushButton_3->setText("Start");}  //修改按钮标志
    qDebug()<<"stop scanning="<<!mutex;
}
void MainWindow::on_lineEdit_textEdited(const QString &arg1)   //当编辑起始IP地址后
{
    qDebug()<<"Start_ip:"<<arg1;
}

void MainWindow::on_lineEdit_4_textEdited(const QString &arg1)  //当编辑结束IP地址后
{
    qDebug()<<"End_IP:"<<arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)   //当编辑超时时间后
{
    qDebug()<<"wait_time_ms:"<<arg1;
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)//当编辑起始端口值后
{
    qDebug()<<"Start_port:"<<arg1;
}

void MainWindow::on_spinBox_4_valueChanged(int arg1) //当编辑结束端口值后
{
    qDebug()<<"End_port:"<<arg1;
}





