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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    setWindowTitle(tr(u8"北邮用电查询"));
    connect(ui->bt_search,SIGNAL(clicked()),this,SLOT(bt_search_clicked()));
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply *)));
}

void ydcx::bt_search_clicked(){
    localAnalyze();
}

void ydcx::request_website(){
    QString ydcx = QString("http://ydcx.bupt.edu.cn/see.aspx?useid=") + ui->ledit_building->text() + "-" + ui->ledit_home->text();
    manager -> get(QNetworkRequest(QUrl(ydcx)));
}

void ydcx::replyFinished(QNetworkReply *reply){
    QString html = reply->readAll();
    // do something
    reply->deleteLater();
}

void ydcx::localAnalyze(){
    QFile file("101516.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream fileread(&file);
    QString fileContent(fileread.readAll());
    //qDebug()<< fileContent;   //Right

    //访问次数  你是第</FONT> <font color=\\\"blue\\\" size=\\\"4px\\\" >&nbsp;57915&nbsp;</font><FONT  size=\\\"4px\\\" >位查询者
    qDebug()<<u8"第几位查询者";
    QString str_number_of_visit("<font color=\\\\\"blue\\\\\" size=\\\\\"4px\\\\\" >\&nbsp;(\\d{3,8})\&nbsp;</font>");
    QRegExp rx_number_of_visit(str_number_of_visit);
    int pos = rx_number_of_visit.indexIn(fileContent);
    //qDebug()<<pos;
    //qDebug()<<rx_number_of_visit.matchedLength();
    qDebug()<<rx_number_of_visit.capturedTexts();
    qDebug()<<rx_number_of_visit.cap(1);
    ui->tEdit_fangwenzhe->setText(u8"    亲爱的同学，你是第 " + rx_number_of_visit.cap(1) + u8" 个查询此寝室用电量的人哦！");

}

ydcx::~ydcx()
{
    delete ui;
}
