#include "ydcx.h"
#include "ui_ydcx.h"
#include <QString>
#include <QtNetwork>
#include <QRegExp>
#include <QDebug>
#include <QTextCodec>
ydcx::ydcx(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ydcx)
{
    ui->setupUi(this);
    ui->tblWgt_goudian->setColumnWidth(0,130);
    ui->tblWgt_goudian->setColumnWidth(1,60);
    ui->tblWgt_goudian->setColumnWidth(2,60);
    ui->tblWgt_goudian->setColumnWidth(3,60);
    ui->tblWgt_goudian->setColumnWidth(4,60);
    ui->tblWgt_goudian->setColumnWidth(5,60);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    setWindowTitle(tr(u8"北邮用电查询"));
    connect(ui->bt_search,SIGNAL(clicked()),this,SLOT(bt_search_clicked()));
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply *)));
}

void ydcx::bt_search_clicked(){
    request_website();

}

void ydcx::request_website(){
    QString ydcx = QString("http://ydcx.bupt.edu.cn/see.aspx?useid=") + ui->ledit_building->text() + "-" + ui->ledit_home->text();
    manager -> get(QNetworkRequest(QUrl(ydcx)));
}

void ydcx::replyFinished(QNetworkReply *reply){
    QString html = reply->readAll();
    // do something
    //qDebug()<<html;
    localAnalyze(html);
    reply->deleteLater();
}

void ydcx::localAnalyze(QString &html){
    QString fileContent(html);
/*
        QFile file("101516.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream fileread(&file);
        fileContent = fileread.readAll();
*/


    //qDebug()<< fileContent;   //Right

    //访问次数  <font color=\\\\\"blue\\\\\" size=\\\\\"4px\\\\\" >\&nbsp;(\\d{3,8})\&nbsp;</font>
    qDebug()<<u8"第几位查询者";
    QString str_number_of_visit("<font color=\\\"blue\\\" size=\\\"4px\\\" >\&nbsp;(\\d{3,8})\&nbsp;</font>");
    QRegExp rx_number_of_visit(str_number_of_visit);
    int pos = rx_number_of_visit.indexIn(fileContent);
    //qDebug()<<pos;
    //qDebug()<<rx_number_of_visit.matchedLength();
    //qDebug()<<rx_number_of_visit.capturedTexts();
    qDebug()<<rx_number_of_visit.cap(1);
    ui->tEdit_fangwenzhe->setText(u8"    亲爱的同学，你是第 " + rx_number_of_visit.cap(1) + u8" 个查询用电量的人哦！");

    QString str_labfangjianhao("labfangjianhao\\\">(\\d{1,2}\-\\d{3,4})");
    QRegExp rx_labfangjianhao(str_labfangjianhao);
    pos = rx_labfangjianhao.indexIn(fileContent,pos);
    //qDebug()<<pos;
    //qDebug()<<rx_labfangjianhao.capturedTexts();
    qDebug()<<rx_labfangjianhao.cap(1);
    ui->label_labfangjianhao_2->setText(rx_labfangjianhao.cap(1));

    ui->label_usertype_2->setText(u8"电价");

    QString str_Labbiaohao("Labbiaohao\\\">(\\d+)");
    QRegExp rx_Labbiaohao(str_Labbiaohao);
    pos = rx_Labbiaohao.indexIn(fileContent,pos);
    //qDebug()<< pos;
    qDebug() << rx_Labbiaohao.cap(1);
    ui->label_labbiaohao_2->setText(rx_Labbiaohao.cap(1));

    QString str_onlinenow("onlinenow\\\">(\\d+\.\\d+\.\\d+\.\\d+)");
    QRegExp rx_onlinenow(str_onlinenow);
    pos = rx_onlinenow.indexIn(fileContent,pos);
    //qDebug()<< pos ;
    //qDebug()<< rx_onlinenow.capturedTexts();
    qDebug()<< rx_onlinenow.cap(1);
    ui->label_laifangip_2->setText(rx_onlinenow.cap(1));

    QString str_time_energy("color=\\\"#4A3C8C\\\">(\\d{4,}/\\d{1,2}/\\d{1,2}) 0:00:00</font></td><td align=\\\"center\\\"><font color=\\\"#4A3C8C\\\">(\\d+)");
    QRegExp rx_time_energy(str_time_energy);
    while((pos = rx_time_energy.indexIn(fileContent,pos)) != -1){
        //qDebug() << pos;
        qDebug() << rx_time_energy.cap(1) << " " << rx_time_energy.cap(2);
        ui->listWgt_time_energy->addItem(rx_time_energy.cap(1) + "\t" + rx_time_energy.cap(2));
        pos += rx_time_energy.matchedLength();
    }

    QString str_goudian(u8"color=\\\"Black\\\">(\\d{4,}/\\d{1,2}/\\d{1,2} \\d{1,2}:\\d{1,2}:\\d{1,2})</font></td>"
                        "<td align=\\\"center\\\"><font color=\\\"Black\\\">(\\d+)</font></td>"
                        "<td align=\\\"center\\\"><font color=\\\"Black\\\">(\\d+)</font></td>"
                        "<td align=\\\"center\\\"><font color=\\\"Black\\\">(免 费|购 电)</font></td>"
                        "<td align=\\\"center\\\"><font color=\\\"Black\\\">(加电完成)</font></td>"
                        "<td align=\\\"center\\\"><font color=\\\"Black\\\">(马老师|张老师)");
    QRegExp rx_goudian(str_goudian);

    //pos = rx_goudian.indexIn(fileContent,0);
    //qDebug() << pos;
    //qDebug() << rx_goudian.capturedTexts();
    //qDebug() << rx_goudian.cap(1)<< rx_goudian.cap(2)<< rx_goudian.cap(3)<< rx_goudian.cap(4)<< rx_goudian.cap(5)<< rx_goudian.cap(6);
    pos = 0;
    int raw = 0;
    while((pos = rx_goudian.indexIn(fileContent,pos)) != -1){
        //qDebug() << pos;

        qDebug() << rx_goudian.cap(1)<< rx_goudian.cap(2)<< rx_goudian.cap(3)<< rx_goudian.cap(4)<< rx_goudian.cap(5)<< rx_goudian.cap(6);
        //tableWidget->setItem(0,0,new QTableWidgetItem("Jan"));
        ui->tblWgt_goudian->setItem(raw,0,new QTableWidgetItem(rx_goudian.cap(1)));
        ui->tblWgt_goudian->setItem(raw,1,new QTableWidgetItem(rx_goudian.cap(2)));
        ui->tblWgt_goudian->setItem(raw,2,new QTableWidgetItem(rx_goudian.cap(3)));
        ui->tblWgt_goudian->setItem(raw,3,new QTableWidgetItem(rx_goudian.cap(4)));
        ui->tblWgt_goudian->setItem(raw,4,new QTableWidgetItem(rx_goudian.cap(5)));
        ui->tblWgt_goudian->setItem(raw,5,new QTableWidgetItem(rx_goudian.cap(6)));
        raw++;
        pos += rx_goudian.matchedLength();
        //qDebug() << r
    }
}

ydcx::~ydcx()
{
    delete ui;
}
